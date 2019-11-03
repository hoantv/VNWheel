#include "FfbWheel.h"
#include "Encoder.h"

Wheel_ Wheel;
#define BAUD_RATE 9600



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
  Wheel.ffbEngine.ForceCalculator(0);
//  Serial.println(Wheel.ffbEngine.ForceCalculator(0));

}

void calculateEncoderPostion() {
  Wheel.encoder.updatePosition();
}
