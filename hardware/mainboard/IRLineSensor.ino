/*
	Reused some of https://github.com/pololu/qtr-sensors-arduino
*/

#include <digitalWriteFast.h>
#include <avr/pgmspace.h>
#define IN1 A0 // Top
#define IN2 A1 //near 1st to lcd
#define IN3 A2 //near 2nd to lcd
#define IN4 A3 //near 3rd to lcd
// Motor direction
#define M_LEFT 12
#define M_RIGHT 13

int calibrateMax[4]={1023}, calibrateMin[4]={0}, result[4];
const unsigned char PROGMEM sensorPin[4]={IN1, IN2, IN3, IN4};

void lineSensorInit() {
	pinModeFast(IN1, INPUT);
	pinModeFast(IN2, INPUT);
	pinModeFast(IN3, INPUT);
	pinModeFast(IN4, INPUT);
}

/*void lineSensorCalibrate(){
	int tmpCalMax[4], tmpCalMin[4], rawRead[4], i, i2;
	for (i=0; i<4; i++){
		tmpCalMax[i]=analogRead(sensorPin[i]);
		tmpCalMin[i]=tmpCalMax[i];
	}
	for (i=1; i<10; i++){
		for (i2=0; i2<4; i2++){
			rawRead[i2]=analogRead(sensorPin[i2]);
			if (rawRead[i2]>tmpCalMax[i2])
				tmpCalMax[i2]=rawRead[i2];
			if (rawRead[i2]<tmpCalMin[i2])
				tmpCalMin[i2]=rawRead[i2];
		}
	}
	for (i=0; i<4; i++){
		if (tmpCalMax[i]>calibrateMax[i])
			calibrateMax[i]=tmpCalMax[i];
		if (tmpCalMin[i]<calibrateMin[i])
			calibrateMin[i]=tmpCalMin[i];
	}
}

void lineSensorRead2(){
	int i, x, resultRaw[4]={0}, i2, z;
	unsigned int denominator;
	for (i=0; i<4; i++){
		for (i2=0; i2<4; i2++){
			z=analogRead(sensorPin[i2]);
			resultRaw[i2]+=z;
			Serial.print(z);
			Serial.print(" ");
		}
		Serial.println();
	}
	for (i=0; i<4; i++){
		resultRaw[i]=(resultRaw[i]+2)/4;
		denominator=calibrateMax[i]-calibrateMin[i];
		if (denominator!=0)
			x = (((signed long)resultRaw[i]) - calibrateMin[i])* 1000 / denominator;
		if (x < 0)
			x = 0;
		else if (x > 1000)
			x = 1000;
		result[i] = (denominator/1000*x)+calibrateMin[i];
	}
}*/

int lineSensorRead(){
	unsigned long ir1=0, ir2=0, ir3=0, ir4=0, i;
	for (i=0; i<10; i++){
		ir1+=analogRead(0);
		ir2+=analogRead(1);
		ir3+=analogRead(2);
		ir4+=analogRead(3);
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