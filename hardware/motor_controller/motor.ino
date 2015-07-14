#include <digitalWriteFast.h>
// L298N pin on Arduino
#define M_ENA 10
#define M_IN1 4 
#define M_IN2 5
#define M_ENB 11
#define M_IN3 6
#define M_IN4 7
#define M_A 8 // Motor 1 number
#define M_B 9 // Motor 2 number
// Motor direction
#define M_FORWARD 10
#define M_BACKWARD 11
#define M_LEFT 12
#define M_RIGHT 13

int current_speed=0;

void motorInit(){
	pinModeFast(M_ENA, OUTPUT);
	pinModeFast(M_IN1, OUTPUT);
	pinModeFast(M_IN2, OUTPUT);
	pinModeFast(M_ENB, OUTPUT);
	pinModeFast(M_IN3, OUTPUT);
	pinModeFast(M_IN4, OUTPUT);
	motorStop();
	motorSetSpeed(0);
}

void motorStop(){
	digitalWriteFast(M_IN1, LOW);
	digitalWriteFast(M_IN2, LOW);
	digitalWriteFast(M_IN3, LOW);
	digitalWriteFast(M_IN4, LOW);
}

void motorSetSpeed(unsigned char speed){
	analogWrite(M_ENA, speed);
	analogWrite(M_ENB, speed);
	current_speed=speed;
}

void motorMove(unsigned char motor, unsigned char direction){
	switch (motor){
		case M_A:
			switch (direction) {
				case M_FORWARD:
					digitalWriteFast(M_IN1, LOW);
					digitalWriteFast(M_IN2, HIGH);
					break;
				case M_BACKWARD:
					digitalWriteFast(M_IN1, HIGH);
					digitalWriteFast(M_IN2, LOW);
					break;
			}
			break;
		case M_B:
			switch (direction) {
				case M_FORWARD:
					digitalWriteFast(M_IN3, LOW);
					digitalWriteFast(M_IN4, HIGH);
					break;
				case M_BACKWARD:
					digitalWriteFast(M_IN3, HIGH);
					digitalWriteFast(M_IN4, LOW);
					break;
			}
			break;
	}
}

void motorSetDirection(unsigned char direction){
	switch (direction){
		case M_LEFT:
			motorMove(M_A, M_FORWARD);
			motorMove(M_B, M_BACKWARD);
			break;
		case M_RIGHT:
			motorMove(M_A, M_BACKWARD);
			motorMove(M_B, M_FORWARD);
			break;
		case M_FORWARD:
			motorMove(M_A, M_FORWARD);
			motorMove(M_B, M_FORWARD);
			break;
		case M_BACKWARD:
			motorMove(M_A, M_BACKWARD);
			motorMove(M_B, M_BACKWARD);
			break;
	}
}

void motorSlowDown(unsigned long delayLength){
	unsigned char i;
	for (i=current_speed-1; i>1; i--){
		motorSetSpeed(i);
		delay(delayLength);
	}
	motorSetSpeed(0);
	motorStop();
}
