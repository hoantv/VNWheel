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

//float FfbEngine::FfbCos(float angle)
//{
//  return (float)cos(angle);
////  return 0;
//}
//
//float FfbEngine::FfbSin(float angle)
//{
//  return (float)sin(angle);
////  return 0;
//}
//
//
//int32_t FfbEngine::ApplyGain(uint8_t value, uint8_t gain)
//{
//  int32_t value_32 = (int32_t)value;
//  return ((value_32 * gain) / 255); // maximum gain 255 => %
//}
//
//int32_t FfbEngine::ApplyEnvelope(volatile TEffectState&  effect, int32_t value) {
//
//  int32_t magnitude = ApplyGain(effect.magnitude, effect.gain);
//  int32_t attackLevel = ApplyGain(effect.attackLevel, effect.gain);
//  int32_t fadeLevel = ApplyGain(effect.fadeLevel, effect.gain);
//  int32_t newValue = magnitude;
//  int32_t attackTime = effect.attackTime;
//  int32_t fadeTime = effect.fadeTime;
//  int32_t elapsedTime = effect.elapsedTime;
//  int32_t duration = effect.duration;
//
//  if (elapsedTime < attackTime)
//  {
//    newValue = (magnitude - attackLevel) * elapsedTime;
//    newValue /= attackTime;
//    newValue += attackLevel;
//  }
//  if (elapsedTime > (duration - fadeTime))
//  {
//    newValue = (magnitude - fadeLevel) * (duration - elapsedTime);
//    newValue /= fadeTime;
//    newValue += fadeLevel;
//  }
//
//  newValue *= value;
//  newValue /= 255;
//
//  return newValue;
//}
//
//
//void FfbEngine::ApplyDirection(volatile TEffectState&  effect, int32_t force, int32_t* axes){
//  float directionX = effect.directionX;
//  float directionY = effect.directionY;
//  if (effect.enableAxis == DIRECTION_ENABLE)
//  {
//    float angle = (directionX * 2) * DEG_TO_RAD;
//    float fForce = force;
//    axes[0] += (int32_t)(FfbCos(angle) * fForce);
//    axes[1] += (int32_t)(FfbSin(angle) * fForce);
//  }
//  else
//  {
//    if (effect.enableAxis & X_AXIS_ENABLE)
//    {
//      float angle = (directionX * 2) * DEG_TO_RAD;
//      float fForce = force;
//      axes[0] += (int32_t)(FfbCos(angle) * fForce);
//    }
//
//    if (effect.enableAxis & Y_AXIS_ENABLE)
//    {
//      float angle = (directionY * 2) * DEG_TO_RAD;
//      float fForce = force;
//      axes[1] += (int32_t)(FfbSin(angle) * fForce);
//    }
//  }
//}

int32_t FfbEngine::ConstantForceCalculator(volatile TEffectState&  effect) {
  return (int32_t)effect.magnitude * effect.gain / 255;
}

int32_t FfbEngine::RampForceCalculator(volatile TEffectState&  effect) {
  int32_t rampForce = effect.startMagnitude + effect.elapsedTime * (effect.endMagnitude - effect.startMagnitude) / effect.duration;
  effect.elapsedTime = millis() - effect.startTime;
  return rampForce;
}

int32_t FfbEngine::ForceCalculator(int32_t encoderPosition)
{
  int32_t force = 0;

  for (uint8_t id = 0; id <= MAX_EFFECTS; id++)
  {
    volatile TEffectState &effect = ffbReportHandler->gEffectStates[id];

    if ((effect.state & MEFFECTSTATE_PLAYING) && ((effect.elapsedTime <= effect.duration) || (effect.duration == USB_DURATION_INFINITE)) && !ffbReportHandler->devicePaused)
    {
      switch (effect.effectType)
      {
        case USB_EFFECT_CONSTANT:
          force += ConstantForceCalculator(effect);
          break;
        case USB_EFFECT_RAMP:
          force += RampForceCalculator(effect);
          ReportPrint(effect);
          break;
        case USB_EFFECT_SQUARE:

          break;
        case USB_EFFECT_SINE:

          break;
        case USB_EFFECT_TRIANGLE:

          break;
        case USB_EFFECT_SAWTOOTHDOWN:

          break;
        case USB_EFFECT_SAWTOOTHUP:

          break;
        case USB_EFFECT_SPRING:

          break;
        case USB_EFFECT_DAMPER:

          break;
        case USB_EFFECT_INERTIA:

          break;
        case USB_EFFECT_FRICTION:
          break;
        case USB_EFFECT_CUSTOM:
          break;
      }
      ffbReportHandler->gEffectStates[id].elapsedTime += WHEEL_SAMPLE_RATE_MS;
      return force;
    }
  }
}
