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
#ifndef FFBENGINE_h
#define FFBENGINE_h

#include <Arduino.h>
#include "HIDReportType.h"
#include "FfbReportHandler.h"

#define WHEEL_SAMPLE_RATE_MS     10
#define WHEEL_RANGE             0x03B7

#ifndef PI
#define PI                      (float)3.14159265359 //consider using library defined value of PI
#endif
#ifndef DEG_TO_RAD
#define DEG_TO_RAD              ((float)(PI/180))
#endif
#define NORMALIZE_RANGE(x)     ((int32_t)((x*255)/WHEEL_RANGE))

class FfbEngine {
  public:
    FfbEngine();
    ~FfbEngine();
    void SetFfb(FfbReportHandler* reporthandler);
     FfbReportHandler* ffbReportHandler;
//
//    float FfbCos(float angle);
//    float FfbSin(float angle); 
//    int32_t ApplyGain(uint8_t value, uint8_t gain);
//    int32_t ApplyEnvelope(volatile TEffectState&  effect, int32_t value);
//    void ApplyDirection(volatile TEffectState&  effect, int32_t force, int32_t* axes);
//    void CalcCondition(volatile TEffectState&  effect, int32_t * outValue, int32_t* inValue);       
//    void FfbGetFeedbackValue(int16_t* axisPosition, int16_t* out);

    int32_t ConstantForceCalculator(volatile TEffectState&  effect);
    int32_t RampForceCalculator(volatile TEffectState&  effect);
    int32_t ForceCalculator(int32_t encoderPosition);
   
};


#endif
