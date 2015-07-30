#include <digitalWriteFast.h>
#define IN1 A0 // Top
#define IN2 A1 //near 1st to lcd
#define IN3 A2 //near 2nd to lcd
#define IN4 A3 //near 3rd to lcd
// Motor direction
#define M_FORWARD 10
#define M_LEFT 12
#define M_RIGHT 13

void lineSensorInit() {
	pinModeFast(IN1, INPUT);
	pinModeFast(IN2, INPUT);
	pinModeFast(IN3, INPUT);
	pinModeFast(IN4, INPUT);
}

int lineSensorRead(){
	int ir1=0, ir2=0, ir3=0, ir4=0;
	unsigned char i;
	for (i=0; i<10; i++){
		ir1+=analogRead(IN1);
		ir2+=analogRead(IN2);
		ir3+=analogRead(IN3);
		ir4+=analogRead(IN4);
	}
	ir1/=10;
	ir2/=10;
	ir3/=10;
	ir4/=10;
	if (ir1>1010)
		return M_FORWARD;
	else if (ir4>1010)
		return M_LEFT;
	else if (ir2<1000)
		return M_RIGHT;
}
