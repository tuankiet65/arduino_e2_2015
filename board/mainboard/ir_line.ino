#define IR_IN1 A0
#define IR_IN2 A1
#define IR_IN3 A2
#define IR_IN4 A3
#define INLINE 1;
// Motor direction
#define M_LEFT 12
#define M_RIGHT 13

void initSensor() {
	pinMode(IR_IN1, INPUT);
	pinMode(IR_IN2, INPUT);
	pinMode(IR_IN3, INPUT);
	pinMode(IR_IN4, INPUT);
}

// Not sure to work, needs more work
unsigned char isInLine() {
	// ir>300=inline else outline
	int ir1 = analogRead(IR_IN1);
	int ir2 = analogRead(IR_IN2);
	int ir3 = analogRead(IR_IN3);
	int ir4 = analogRead(IR_IN4);
	if ((ir1>300) && (ir3>300)) 
		return true;
	return false;
}

// Also not sure to work properly, needs more work
unsigned char getLineDirection() {
	if (isInLine())
		return INLINE;
	if (analogRead(IN2)>300)
		return M_RIGHT;
	if (analogRead(IN4)>300)
		return M_LEFT;
	return 0;
}

int getValue(int port) {
	return analogRead(port);
}