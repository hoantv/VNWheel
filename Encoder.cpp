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
  lastEncoderTime = (uint64_t) millis();
  lastPositionVelocity = 0;
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
  uint64_t encoderCurrentTime = (uint64_t) millis();
  if (encoderCurrentTime > lastEncoderTime) {
    currentPositionVelocity = positionChange / (encoderCurrentTime - lastEncoderTime);
    positionAcceleration = (currentPositionVelocity - lastPositionVelocity) / (encoderCurrentTime - lastEncoderTime);
    lastEncoderTime = encoderCurrentTime;
    lastPositionVelocity = currentPositionVelocity;
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
    
//    if (thisState == LATCHSTATE) {
//      _positionExt = _position >> 2;
//      _positionExtTimePrev = _positionExtTime;
//      _positionExtTime = millis();
//    }
    
    oldState = thisState;
  } // if
//  currentPosition = constrain(currentPosition, minValue, maxValue); // remove in production
} // tick()
