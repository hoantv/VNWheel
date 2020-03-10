#include "Encoder.h"
#include "DigitalWriteFast.h"

Encoder::Encoder() {
  pinMode (encoderPinA, INPUT_PULLUP);
  pinMode (encoderPinB, INPUT_PULLUP);
  pinMode (encoderPinZ, INPUT_PULLUP);
}

Encoder::~Encoder() {
}

void Encoder::setConfig(WheelConfig wheelConfig) {
  cPR = wheelConfig.configCPR ;
  maxAngle = wheelConfig.configMaxAngle;
  inverted = wheelConfig.configInverted;
  resetPosition = wheelConfig.configResetEncoderPosition;
  maxValue = (float)maxAngle / 2 / 360 * cPR ;
  minValue = -maxValue;
  usePinZ = wheelConfig.configUsePinZ;
  initVariables();
}

void Encoder::initVariables() {
  currentPosition = 0;
  lastPosition = 0;
  correctPosition = 0;
  currentPinA = LOW;
  currentPinB = LOW;
  currentPinZ = LOW;
  lastPinA = LOW;
  lastPinB = LOW;
  lastPinZ = LOW;
  z1stUp = false;
  maxAcceleration = 0;
  maxVelocity = 0;
  lastEncoderTime = (uint32_t) millis();
  lastVelocity = 0;
}

void  Encoder::updatePosition() {
    
  if (usePinZ) {
    currentPinZ = digitalReadFast(encoderPinZ);
    if (z1stUp) {
      correctPosition = correctPosition; //found correct position
      z1stUp = true;
    } else {
      if (currentPosition > correctPosition * 0.05 || currentPosition < correctPosition * 0.05  ) {
        currentPosition = correctPosition;
      }
    }
  } 
  positionChange = currentPosition - lastPosition;
  uint32_t currentEncoderTime = (int32_t) millis();
  int16_t diffTime = (int16_t)(currentEncoderTime - lastEncoderTime) ;
  if (diffTime > 0) {
    currentVelocity = positionChange / diffTime;
    currentAcceleration = (abs(currentVelocity) - abs(lastVelocity)) / diffTime;
    lastEncoderTime = currentEncoderTime;
    lastVelocity = currentVelocity;
  }
  lastPosition = currentPosition;
}

int8_t Encoder::parsePosition() { //4 state
  if (lastPinA && lastPinB) {
    if (!currentPinA && currentPinB) return 1;
    if (currentPinA && !currentPinB) return -1;
  }
  else if (!lastPinA && lastPinB) {
    if (!currentPinA && !currentPinB) return 1;
    if (currentPinA && currentPinB) return -1;
  }
  else if (!lastPinA && !lastPinB) {
    if (currentPinA && !currentPinB) return 1;
    if (!currentPinA && currentPinB) return -1;
  }
  else if (lastPinA && !lastPinB) {
    if (currentPinA && currentPinB) return 1;
    if (!currentPinA && !currentPinB) return -1;
  }
}

//this code copy from Rotary Encoder of Matthias Hertel
const int8_t KNOBDIR[] = {
  0, -1,  1,  0,
  1,  0,  0, -1,
  -1,  0,  0,  1,
0,  1, -1,  0  };

void Encoder::tick(void)
{
  int sig1 = digitalReadFast(encoderPinA);
  int sig2 = digitalReadFast(encoderPinB);
  int8_t thisState = sig1 | (sig2 << 1);

  if (oldState != thisState) {
    currentPosition += KNOBDIR[thisState | (oldState<<2)];
    oldState = thisState;
  } 
} 
