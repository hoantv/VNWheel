#include "FfbWheel.h"
#include "Encoder.h"
#include "DigitalWriteFast.h"

Wheel_ Wheel;
#define BAUD_RATE 9600

#define PWM_POS 5
#define PWM_NEG 6
#define PULSE 10
#define DIR 11

int32_t total_force = 0;
int32_t last_total_force = 0;

void setup() {

  attachInterrupt(digitalPinToInterrupt(interruptA), calculateEncoderPostion, CHANGE);
  attachInterrupt(digitalPinToInterrupt(interruptB), calculateEncoderPostion, CHANGE);
  Wheel.begin();
  Serial.begin(BAUD_RATE);
}
void loop() {
  Wheel.encoder.updatePosition();
  if (Wheel.encoder.minValue <= Wheel.encoder.currentPosition <= Wheel.encoder.maxValue) {
    Wheel.xAxis(map(Wheel.encoder.currentPosition, Wheel.encoder.minValue , Wheel.encoder.maxValue, -32768, 32767));
    Wheel.write();
  }
  Wheel.RecvFfbReport();

  total_force = Wheel.ffbEngine.ForceCalculator(Wheel.encoder);
  Serial.println(total_force);
  //  Serial.println(Wheel.encoder.currentPosition);
  //  when reach max and min wheel range, max force to prevent wheel goes over.
  if (Wheel.encoder.currentPosition >= Wheel.encoder.maxValue) {
    total_force = 255;
  } else if (Wheel.encoder.currentPosition <= Wheel.encoder.minValue) {
    total_force = -255;
  }
  total_force = constrain(total_force, -255, 255);
  total_force = total_force * 0.1;
  if (last_total_force * total_force < 0) {
    last_total_force = total_force;
    total_force = 0;
  } else {
    last_total_force = total_force;
  }
  if (total_force == 0) { //maybe remove in production
    digitalWrite(PULSE, 0);
    digitalWrite(DIR, 0);
  }

  setPWM(total_force, Wheel.wheelConfig.controlMode);
}

void calculateEncoderPostion() {
  Wheel.encoder.tick();
}

void setPWM(int32_t total_force, uint8_t ControlMode) {
  switch (ControlMode) {
    case PULSE_DIR:
      //      if (total_force > 0) digitalWrite(DIR, HIGH);
      //      else digitalWrite(DIR, LOW);
      //      analogWrite(PULSE, abs(total_force));
      //      break;
      if (total_force > 0) {
        analogWrite(PULSE, abs(total_force));
      }
      else {
        analogWrite(DIR, abs(total_force));
      }

    case 'PWM_POS_NEG':
      if (total_force > 0) {
        //        analogWrite(PWM_POS, abs(total_force));
        analogWrite(PULSE, abs(total_force));
      }

      else {
        analogWrite(PWM_NEG, abs(total_force));
        analogWrite(DIR, abs(total_force));
      }
      break;
    default:
      break;

  }
}
