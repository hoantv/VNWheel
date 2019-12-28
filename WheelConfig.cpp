#include "WheelConfig.h"

WheelConfig::WheelConfig() {
  SetDefault();
}

WheelConfig::~WheelConfig() {
  // Auto-generated destructor stub
}

void WheelConfig::SetDefault() {
  // reset everything to zero here.
  // pointers to objects have to be deleted.

//  configCPR = 1334;
  configCPR = 10000;
  configMaxAngle = (uint16_t) 900;
  configInverted = false;
  configUsePinZ = false;
  configResetEncoderPosition = false;
  controlMode = PULSE_DIR;
//  controlMode = PWM_POS_NEG;
  
}
