#include "Encoder.h"
#include "DigitalWriteFast.h"



Encoder::Encoder() {
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
  currentPinA = digitalReadFast(encoderPinA);
  currentPinB = digitalReadFast(encoderPinB);
  if (inverted == false) {
    currentPosition += parsePosition();
  }
  else {
    currentPosition += -parsePosition();
  }
  if (currentPosition > maxValue) {
    currentPosition = maxValue;
  }
  if (currentPosition < minValue) {
    currentPosition = minValue;
  }
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
  lastPinA = currentPinA;
  lastPinB = currentPinB;
  positionChange = currentPosition - lastPosition; 
  uint64_t encoderCurrentTime = (uint64_t) millis();
  currentPositionVelocity = positionChange/(encoderCurrentTime - lastEncoderTime);
  positionAcceleration = (currentPositionVelocity - lastPositionVelocity)/(encoderCurrentTime - lastEncoderTime);
  lastEncoderTime = encoderCurrentTime;
  lastPositionVelocity = currentPositionVelocity;
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
