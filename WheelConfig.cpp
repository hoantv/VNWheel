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

  configCPR = 1334;
//    configCPR = 10000;
  configMaxAngle = (uint16_t) 900;
  configInverted = false;
  configUsePinZ = false;
  configResetEncoderPosition = false;
  controlMode = PULSE_DIR;
  //  controlMode = PWM_POS_NEG;
  constantGainConfig = 100;
  rampGainConfig = 100;;
  squareGainConfig = 100;;
  sinGainConfig = 100;
  triangleGainConfig = 100;;
  sawToothDownGainConfig = 100;;
  sawToothUpGainConfig = 100;;
  springGainConfig = 100;
  damperGainConfig = 100;
  inertiaGainConfig = 100;;
  frictionGainConfig = 100;;
  totalGainConfig = 100;;

}
