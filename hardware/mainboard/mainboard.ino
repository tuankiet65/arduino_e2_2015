#include "LiquidCrystal_I2C.h"
#define M_FORWARD 10
#define M_BACKWARD 11
#define M_LEFT 12
#define M_RIGHT 13
#define I2C_INIT 14
#define I2C_SET_DIRECTION 15
#define I2C_SET_SPEED 17

void setup(){
	Serial.begin(9600);
	lineSensorInit();
	IRRemoteInit();
	motorI2CInitInterface();
	motorI2CSendCommand(I2C_INIT, 255);
	motorI2CSendCommand(I2C_SET_SPEED, 220);
	LCDInit();
	RFIDInit();
	ultrasoundInit();
	if (!EEPROMNumberOfDest())
		interfaceRunSetup();
}

void loop(){
	mainInterface();
}
