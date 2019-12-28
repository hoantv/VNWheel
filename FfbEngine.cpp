/*
  Force Feedback Joystick Math
  Joystick model specific code for calculating force feedback.
  Copyright 2016  Jaka Simonic
  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.
  The author disclaim all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

#include "FfbEngine.h"
#include "HIDReportType.h"
#include "debug.h"

FfbEngine::FfbEngine() {
}
FfbEngine::~FfbEngine() {
}

void FfbEngine::SetFfb(FfbReportHandler* reporthandler) {
  ffbReportHandler = reporthandler;
}

int32_t FfbEngine::ConstantForceCalculator(volatile TEffectState&  effect) {
  return (int32_t)effect.magnitude * effect.gain / 255;
}

int32_t FfbEngine::RampForceCalculator(volatile TEffectState&  effect) {
  int32_t rampForce = effect.startMagnitude + effect.elapsedTime * (effect.endMagnitude - effect.startMagnitude) / effect.duration;
  return rampForce;
}

int32_t FfbEngine::SquareForceCalculator(volatile TEffectState&  effect) {
  int32_t offset = effect.offset * 2;
  uint32_t magnitude = effect.magnitude;
  uint32_t elapsedTime = effect.elapsedTime;
  uint32_t phase = effect.phase;
  uint32_t period = effect.period;

  int32_t maxMagnitude = offset + magnitude;
  int32_t minMagnitude = offset - magnitude;
  uint32_t phasetime = (phase * period) / 255;
  uint32_t timeTemp = elapsedTime + phasetime;
  uint32_t reminder = timeTemp % period;
  int32_t tempforce;
  if (reminder > (period / 2)) tempforce = minMagnitude;
  else tempforce = maxMagnitude;
  return ApplyEnvelope(effect, tempforce);

}

int32_t FfbEngine::SinceForceCalculator(volatile TEffectState&  effect) {
  float offset = effect.offset * 2;
  float magnitude = effect.magnitude;
  float phase = effect.phase;
  float timeTemp = effect.elapsedTime;
  float period = effect.period;

  float angle = ((timeTemp / period) + (phase / 255) * period) * 2 * PI;
  float sine = sin(angle);
  float tempforce = sine * magnitude;
  tempforce += offset;
  return ApplyEnvelope(effect, tempforce);
}

int32_t FfbEngine::TriangleForceCalculator(volatile TEffectState&  effect) {
  float offset = effect.offset * 2;
  float magnitude = effect.magnitude;
  float elapsedTime = effect.elapsedTime;
  uint32_t phase = effect.phase;
  uint32_t period = effect.period;
  float periodF = effect.period;

  float maxMagnitude = offset + magnitude;
  float minMagnitude = offset - magnitude;
  uint32_t phasetime = (phase * period) / 255;
  uint32_t timeTemp = elapsedTime + phasetime;
  float reminder = timeTemp % period;
  float slope = ((maxMagnitude - minMagnitude) * 2) / periodF;
  float tempforce = 0;
  if (reminder > (periodF / 2)) tempforce = slope * (periodF - reminder);
  else tempforce = slope * reminder;
  tempforce += minMagnitude;
  return ApplyEnvelope(effect, tempforce);
}

int32_t FfbEngine::SawtoothDownForceCalculator(volatile TEffectState&  effect) {
  float offset = effect.offset * 2;
  float magnitude = effect.magnitude;
  float elapsedTime = effect.elapsedTime;
  float phase = effect.phase;
  uint32_t period = effect.period;
  float periodF = effect.period;

  float maxMagnitude = offset + magnitude;
  float minMagnitude = offset - magnitude;
  int32_t phasetime = (phase * period) / 255;
  uint32_t timeTemp = elapsedTime + phasetime;
  float reminder = timeTemp % period;
  float slope = (maxMagnitude - minMagnitude) / periodF;
  float tempforce = 0;
  tempforce = slope * (period - reminder);
  tempforce += minMagnitude;
  return ApplyEnvelope(effect, tempforce);
}

int32_t FfbEngine::SawtoothUpForceCalculator(volatile TEffectState&  effect) {
  float offset = effect.offset * 2;
  float magnitude = effect.magnitude;
  float elapsedTime = effect.elapsedTime;
  uint32_t phase = effect.phase;
  uint32_t period = effect.period;
  float periodF = effect.period;

  float maxMagnitude = offset + magnitude;
  float minMagnitude = offset - magnitude;
  int32_t phasetime = (phase * period) / 255;
  uint32_t timeTemp = elapsedTime + phasetime;
  float reminder = timeTemp % period;
  float slope = (maxMagnitude - minMagnitude) / periodF;
  float tempforce = 0;
  tempforce = slope * reminder;
  tempforce += minMagnitude;
  return ApplyEnvelope(effect, tempforce);
}

int32_t FfbEngine::ConditionForceCalculator(volatile TEffectState&  effect, int32_t inputForce, int32_t metric) {
  float deadBand = effect.deadBand;
  float cpOffset = effect.cpOffset;
  float negativeSaturation = -effect.negativeSaturation;
  float positiveSaturation = effect.positiveSaturation;
  float negativeCoefficient = effect.negativeCoefficient;
  float positiveCoefficient = effect.positiveCoefficient;
  if (metric < (cpOffset - deadBand)) {
    float tempForce = (metric - (cpOffset - deadBand)) * negativeCoefficient;
    //    tempForce = ((cpOffset - deadBand) - metric) * negativeCoefficient;
    return (int32_t) (tempForce < negativeSaturation ? negativeSaturation : tempForce);
  }
  else if (metric > (cpOffset + deadBand)) {
    float tempForce = (metric - (cpOffset + deadBand)) * positiveCoefficient;
    return  (int32_t)(tempForce > positiveSaturation ? positiveSaturation : tempForce);
  }
  else return 0;
}




int32_t FfbEngine::ForceCalculator(Encoder encoder)
{
  int32_t force = 0;

  for (uint8_t id = 0; id <= MAX_EFFECTS; id++)
  {
    volatile TEffectState &effect = ffbReportHandler->gEffectStates[id];

    if ((effect.state & MEFFECTSTATE_PLAYING) && ((effect.elapsedTime <= effect.duration) || (effect.duration == USB_DURATION_INFINITE)) && !ffbReportHandler->devicePaused)
    {
//          ReportPrint(effect);
      switch (effect.effectType)
      {
        case USB_EFFECT_CONSTANT:
//          ReportPrint(effect);
          force += ConstantForceCalculator(effect);
//          Serial.print("force ");
//          Serial.println (force);
          break;
        case USB_EFFECT_RAMP:
          force += RampForceCalculator(effect);
          break;
        case USB_EFFECT_SQUARE:
          break;
        case USB_EFFECT_SINE:
          force +=  SinceForceCalculator(effect);
          break;
        case USB_EFFECT_TRIANGLE:
          force += TriangleForceCalculator(effect);
          break;
        case USB_EFFECT_SAWTOOTHDOWN:
          force += SawtoothDownForceCalculator(effect);

          break;
        case USB_EFFECT_SAWTOOTHUP:
          force += SawtoothUpForceCalculator(effect);
          break;
        case USB_EFFECT_SPRING:
          //          position
          //          ReportPrint(effect);
          force += ConditionForceCalculator(effect, force, NormalizeRange(encoder.currentPosition, encoder.maxValue * 2) );
          //          Serial.print("force ");
          //          Serial.println (force);
          break;
        case USB_EFFECT_DAMPER:

          break;
        case USB_EFFECT_INERTIA:

          break;
        case USB_EFFECT_FRICTION:
          //          position change
          //          ReportPrint(effect);
          force += ConditionForceCalculator(effect, force, NormalizeRange(encoder.positionChange, encoder.maxValue * 2) );
          //          Serial.println (force);
          break;
        case USB_EFFECT_CUSTOM:
          break;
      }
      ffbReportHandler->gEffectStates[id].elapsedTime = (uint64_t)millis()  - ffbReportHandler->gEffectStates[id].startTime;
      
    }
  }
   
  return constrain(force, -255, 255);
}

int32_t FfbEngine::NormalizeRange(int32_t x, int32_t wheelRange) {
  return (int32_t)( (float) (x / wheelRange) * 255);
}

int32_t  FfbEngine::ApplyGain(uint8_t value, uint8_t gain)
{
  int32_t value_32 = (int16_t)value;
  return ((value_32 * gain) / 255);
}

int32_t FfbEngine::ApplyEnvelope(volatile TEffectState&  effect, int32_t value)
{
  int32_t magnitude = ApplyGain(effect.magnitude, effect.gain);
  int32_t attackLevel = ApplyGain(effect.attackLevel, effect.gain);
  int32_t fadeLevel = ApplyGain(effect.fadeLevel, effect.gain);
  int32_t newValue = magnitude;
  int32_t attackTime = effect.attackTime;
  int32_t fadeTime = effect.fadeTime;
  int32_t elapsedTime = effect.elapsedTime;
  int32_t duration = effect.duration;

  if (elapsedTime < attackTime)
  {
    newValue = (magnitude - attackLevel) * elapsedTime;
    newValue /= attackTime;
    newValue += attackLevel;
  }
  if (elapsedTime > (duration - fadeTime))
  {
    newValue = (magnitude - fadeLevel) * (duration - elapsedTime);
    newValue /= fadeTime;
    newValue += fadeLevel;
  }

  newValue *= value;
  newValue /= 255;
  return newValue;
}
