#include "PWM.h"

 Pwm::Pwm(void) {

 }
 
 Pwm::~Pwm() {
	 
 }

void Pwm::begin(){
  setPWM(0);
  // * PWM Pins 9 & 10, Timer 1 is using: Channel A OCR1A at Pin 9 and Channel B OCR1B  at Pin 10 *
    TCCR1A = 0b10100000;
    TCCR1B = 0b00010001;   
    ICR1 = MAXFORCE;
    OCR1A = 0;
    OCR1B = 0;   
  
}
 
void Pwm::setPWM(int16_t force) {
	int nomalizedForce = map (force, -255,255, MINFORCE, MAXFORCE); 
	if (nomalizedForce >= 0) {
		PWM10 = nomalizedForce;
		PWM9 = 0;
	} else {
		PWM10 = 0;
		PWM9 = -nomalizedForce;
	}
 }
 
