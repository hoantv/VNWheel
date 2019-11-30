#include "FfbWheel.h"
#include "Encoder.h"
#include "DigitalWriteFast.h"

Wheel_ Wheel;
#define BAUD_RATE 9600

#define PWM_POS 5
#define PWM_NEG 6
#define PULSE 5
#define DIR 6


void setup() {
  pinMode (encoderPinA, INPUT);
  pinMode (encoderPinB, INPUT);
  pinMode (encoderPinZ, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptA), calculateEncoderPostion, CHANGE);
  attachInterrupt(digitalPinToInterrupt(interruptB), calculateEncoderPostion, CHANGE);
  Wheel.begin();
  Serial.begin(BAUD_RATE);
}
void loop() {
  Wheel.xAxis(Wheel.encoder.currentPosition);
  Wheel.write();
  Wheel.RecvFfbReport();
  int32_t total_force = Wheel.ffbEngine.ForceCalculator(Wheel.encoder);
  setPWM(total_force, Wheel.wheelConfig.controlMode);  
  Serial.println(total_force);
//  when reach max and min wheel range, max force to prevent wheel goes over.
  if (Wheel.encoder.currentPosition == Wheel.encoder.maxValue) {
    setPWM(-255, Wheel.wheelConfig.controlMode);  
  } 
  if (Wheel.encoder.currentPosition == Wheel.encoder.minValue) {
    setPWM(255, Wheel.wheelConfig.controlMode);  
  } 
}

void calculateEncoderPostion() {
  Wheel.encoder.updatePosition();
}

void setPWM(int32_t total_force, uint8_t ControlMode) {
  switch (ControlMode) {
    case PULSE_DIR:
      if (total_force > 0) digitalWrite(DIR, HIGH);
      else digitalWrite(DIR, LOW);
      analogWrite(PULSE, abs(total_force));
      break;
    case 'PWM_POS_NEG':
      if (total_force > 0) analogWrite(PWM_POS, abs(total_force));
      else analogWrite(PWM_NEG, abs(total_force));
      break;
    default:
      break;

  }
}
