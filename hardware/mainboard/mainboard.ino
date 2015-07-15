#define INLINE 1
#define M_FORWARD 10
#define M_BACKWARD 11
#define M_LEFT 12
#define M_RIGHT 13
#define I2C_INIT 14
#define I2C_SET_DIRECTION 15
#define I2C_SET_SPEED 17
#define I2C_SUCCESS 18
#define I2C_FAIL 19

void setup() {
	Serial.begin(9600);
	motorI2CInitInterface();
	while (!motorI2CSendCommand(I2C_INIT, 255)) {
		Serial.println("Fail sending I2C_INIT");
		delay(1000);
	}
	while (!motorI2CSendCommand(I2C_SET_SPEED, 255)) {
		Serial.println("Fail sending I2C_CHANGE_SPEED");
		delay(1000);
	}
}

void loop() {
	switch(getStatus()){
		case M_LEFT:
			motorI2CSendCommand(I2C_SET_DIRECTION, M_LEFT);
			break;
		case M_RIGHT:
			motorI2CSendCommand(I2C_SET_DIRECTION, M_LEFT);
			break;
		case INLINE:
			motorI2CSendCommand(I2C_SET_DIRECTION, M_FORWARD);
			break;
	}
	delay(100);
}
