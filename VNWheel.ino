#include "FfbWheel.h"
#include "Encoder.h"
#include "DigitalWriteFast.h"
#include "PID_v1.h"

Wheel_ Wheel;
#define BAUD_RATE 115200

#define PULSE 9
#define DIR 10

int32_t total_force = 0;
int32_t last_total_force = 0;

double Setpoint, Input, Output;
//double Kp=2, Ki=5, Kd=1;
double Kp = 0.1 , Ki = 30 , Kd =  0;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);
bool initialRun = true;


#include "PWM.h"
Pwm pwm;

void setup() {
  pinMode (PULSE, OUTPUT);
  pinMode (DIR, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(interruptA), calculateEncoderPostion, CHANGE);
  attachInterrupt(digitalPinToInterrupt(interruptB), calculateEncoderPostion, CHANGE);  pwm.begin();
  
  pwm.setPWM(0);
  Wheel.begin();
  Input = Wheel.encoder.currentPosition;
  myPID.SetMode(AUTOMATIC);
  myPID.SetSampleTime(0.01);
  myPID.SetOutputLimits(-50, 50);
  Serial.begin(BAUD_RATE);
}
void loop() {
  if (initialRun == true ) {
//    position control is not correctly, wheel runs over disired postion serveral times before stop
    pwm.setPWM(10);
    gotoPosition(Wheel.encoder.minValue);
    gotoPosition(Wheel.encoder.maxValue);
    gotoPosition( 0);
    initialRun = false;
    pwm.setPWM(0);
  } else
  {
    // assign for re-test without initialRun
    //        Serial.print("currentVelocity: ");
    //        Serial.print(Wheel.encoder.maxVelocity);
    //        Serial.print(" maxAcceleration: ");
    //        Serial.println(Wheel.encoder.maxAcceleration);
    //        Serial.print("   maxPositionChange: ");
//    Serial.println(Wheel.encoder.currentPosition);
//    Wheel.encoder.maxPositionChange = 1151;
//    Wheel.encoder.maxVelocity  = 72;
//    Wheel.encoder.maxAcceleration = 33;
    Wheel.encoder.updatePosition();
    if (Wheel.encoder.currentPosition > Wheel.encoder.maxValue) {
      Wheel.xAxis(32767);
    } else if (Wheel.encoder.currentPosition < Wheel.encoder.minValue) {
      Wheel.xAxis(-32767);
    } else {
      Wheel.xAxis(map(Wheel.encoder.currentPosition, Wheel.encoder.minValue , Wheel.encoder.maxValue, -32768, 32767));
    }

    Wheel.RecvFfbReport();
    Wheel.write();
    total_force = Wheel.ffbEngine.ForceCalculator(Wheel.encoder);    
    total_force = constrain(total_force, -255, 255);
    //  Serial.println(Wheel.encoder.currentPosition);
    //  when reach max and min wheel range, max force to prevent wheel goes over.
    if (Wheel.encoder.currentPosition >= Wheel.encoder.maxValue) {
      total_force = 255;
    } else if (Wheel.encoder.currentPosition <= Wheel.encoder.minValue) {
      total_force = -255;
    }
  }
//  set total gain = 0.2 need replace by wheelConfig.totalGain.
  pwm.setPWM(total_force * 0.2);
}


void gotoPosition(int32_t targetPosition) {
  Setpoint = targetPosition;
  while (Wheel.encoder.currentPosition != targetPosition) {
    Setpoint = targetPosition;
    Wheel.encoder.updatePosition();
    Input = Wheel.encoder.currentPosition ;
    myPID.Compute();
    pwm.setPWM(-Output);
    CalculateMaxSpeedAndMaxAcceleration();
    Serial.print("Encoder Position: ");
    Serial.print(Wheel.encoder.currentPosition);
    Serial.print("  ");
    Serial.print(Setpoint);
    Serial.print("  ");
    Serial.print("PWM: ");
    Serial.println(Output);
  }
}

void CalculateMaxSpeedAndMaxAcceleration() {
  if (Wheel.encoder.maxVelocity < abs(Wheel.encoder.currentVelocity)) {
    Wheel.encoder.maxVelocity = abs(Wheel.encoder.currentVelocity);
  }
  if (Wheel.encoder.maxAcceleration < abs(Wheel.encoder.currentAcceleration)) {
    Wheel.encoder.maxAcceleration = abs(Wheel.encoder.currentAcceleration);
  }
  if (Wheel.encoder.maxPositionChange < abs(Wheel.encoder.positionChange)) {
    Wheel.encoder.maxPositionChange = abs(Wheel.encoder.positionChange);
  }
}

void calculateEncoderPostion() {
  Wheel.encoder.tick();
}
