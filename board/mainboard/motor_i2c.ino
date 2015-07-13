#include <Wire.h>
#define I2C_BUS 0x69
#define M_FORWARD 10
#define M_BACKWARD 11
#define M_LEFT 12
#define M_RIGHT 13
#define I2C_INIT 14 // Param is required, but can be a random number
#define I2C_MOVE 15
#define I2C_TURN 16
#define I2C_CHANGE_SPEED 17
#define I2C_SUCCESS 18 // Success/fail check reserved
#define I2C_FAIL 19

unsigned char isInitInterface=0;

void motorI2CInitInterface(){
	Wire.begin();
	isInitInterface=255;
}

void motorI2CSendCommand(unsigned char command, unsigned char param){
	int waitTime=0;
	Wire.beginTransmission(I2C_BUS);
	Wire.write(command);
	Wire.write(param);
	Wire.endTransmission();
}