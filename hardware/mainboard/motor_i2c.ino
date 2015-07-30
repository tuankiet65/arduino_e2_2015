#include <Wire.h>
#define I2C_BUS 0x69
#define M_FORWARD 10
#define M_BACKWARD 11
#define M_LEFT 12
#define M_RIGHT 13
#define I2C_INIT 14 // param is random
#define I2C_SET_DIRECTION 15
#define I2C_SET_SPEED 17
#define I2C_SUCCESS 18
#define I2C_FAIL 19

void motorI2CInit() {
	Wire.begin();
	motorI2CSendCommand(I2C_INIT, 255);
}

unsigned char motorI2CSendCommand(unsigned char command, unsigned char param) {
	unsigned char waitTime = 0;
	Wire.beginTransmission(I2C_BUS);
	Wire.write(command);
	Wire.write(param);
	Wire.endTransmission();
	delay(50);
	Wire.requestFrom(I2C_BUS, 2);
	while (Wire.available() != 2 && waitTime < 50) {
		delay(50);
		waitTime++;
	}
	if (waitTime >= 50)
		return 0;
	unsigned char command_reply = Wire.read();
	unsigned char result = Wire.read();
	return (command_reply == command && result == I2C_SUCCESS);
}
