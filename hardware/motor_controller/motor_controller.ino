#include <Wire.h>
#define I2C_BUS 0x69
#define M_FORWARD 10
#define M_BACKWARD 11
#define M_LEFT 12
#define M_RIGHT 13
#define I2C_INIT 14
#define I2C_SET_DIRECTION 15
#define I2C_SET_SPEED 17
#define I2C_SUCCESS 18
#define I2C_FAIL 19
#define M_A 8
#define M_B 9

unsigned char result_arr[2];

void setup() {
	Serial.begin(9600);
	Serial.println("Okay");
	Wire.begin(I2C_BUS);
	Wire.onReceive(i2cCommandHandler);
	Wire.onRequest(i2cRequestHandler);
}

void i2cCommandHandler(int numBytes) {
	unsigned char request, param;
	if (numBytes != 2) {
		Serial.print("Not enough bytes: need 2, got ");
		Serial.println(numBytes);
		setResult(request, I2C_FAIL);
		return;
	}
	request = Wire.read();
	param = Wire.read();
	switch (request) {
	case I2C_INIT:
		Serial.println("Received I2C_INIT");
		motorInit();
		break;
	case I2C_SET_DIRECTION:
		switch (param) {
		case M_LEFT:
			Serial.println("Received I2C_SET_DIRECTION M_LEFT");
			motorSetDirection(M_LEFT);
			break;
		case M_RIGHT:
			Serial.println("Received I2C_SET_DIRECTION M_RIGHT");
			motorSetDirection(M_RIGHT);
			break;
		case M_FORWARD:
			Serial.println("Received I2C_SET_DIRECTION M_FORWARD");
			motorSetDirection(M_FORWARD);
			break;
		case M_BACKWARD:
			Serial.println("Received I2C_SET_DIRECTION M_BACKWARD");
			motorSetDirection(M_BACKWARD);
			break;
		default:
			Serial.println("Received I2C_TURN with invalid param");
			setResult(request, I2C_FAIL);
			return;
			break;
		}
		break;
	case I2C_SET_SPEED:
		Serial.print("Received I2C_SET_SPEED ");
		Serial.println(param);
		motorSetSpeed(255);
		break;
	}
	Serial.println("Write success");
	setResult(request, I2C_SUCCESS);
}

void setResult(unsigned char request, unsigned char result) {
	result_arr[0] = request;
	result_arr[1] = result;
}

void i2cRequestHandler() {
	Wire.write(result_arr, 2);
}

void loop() {
}
