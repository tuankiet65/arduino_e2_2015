// !PID
#include <digitalWriteFast.h>
#define IN1 2// Sensor T1 (left)
#define IN2 3 // Sensor T2 (left)
#define IN3 4 // Sensor T3 (Top)
#define IN4 5 // Sensor T4 (right)
#define IN5 6 // Sensor T5 (right)
// Motor direction
#define M_FORWARD 10
#define M_LEFT 12
#define M_RIGHT 13

void lineSensorInit() {
	pinModeFast(IN1, INPUT);
	pinModeFast(IN2, INPUT);
	pinModeFast(IN3, INPUT);
	pinModeFast(IN4, INPUT);
	pinModeFast(IN5, INPUT);
}

int lineSensorRead(){
	unsigned char ir1, ir2, ir3, ir4, ir5;
	ir1=digitalReadFast(IN1);
	ir2=digitalReadFast(IN2);
	ir3=digitalReadFast(IN3);
	ir4=digitalReadFast(IN4);
	ir5=digitalReadFast(IN5);
	if (ir3)
		return M_FORWARD;
	else if (ir1 || ir2)
		return M_LEFT;
	else if (ir4 || ir5)
		return M_RIGHT;
}
