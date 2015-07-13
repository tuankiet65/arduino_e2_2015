#include <Wire.h>
#define I2C_BUS 0x69
#define I2C_INIT 14
#define I2C_MOVE 15
#define I2C_TURN 16
#define I2C_CHANGE_SPEED 17
#define I2C_SUCCESS 18
#define I2C_FAIL 19
#define M_A 8 // Motor 1 number
#define M_B 9 // Motor 2 number
// Motor direction
#define M_FORWARD 10
#define M_BACKWARD 11
#define M_LEFT 12
#define M_RIGHT 13

void setup(){
	Serial.begin(9600);
	Wire.begin(I2C_BUS);
	Wire.onReceive(i2cHandler);
}


void i2cHandler(int numBytes){
	unsigned char request, param;
	if (numBytes!=2) {
		Serial.print("Not enough bytes: need 2, got ");
		Serial.println(numBytes);
		Wire.write(I2C_FAIL);
		return;
	}
	request=Wire.read();
	param=Wire.read();
	switch (request){
		case I2C_INIT:
			Serial.println("Received I2C_INIT");
			motorInit();
			break;
		case I2C_TURN:
			switch (param){
				case M_LEFT:
					Serial.println("Received I2C_TURN M_LEFT");
					motorTurn(M_LEFT);
					break;
				case M_RIGHT:
					Serial.println("Received I2C_TURN M_RIGHT");
					motorTurn(M_RIGHT);
					break;
				default:
					Serial.println("Received I2C_TURN with invalid param");
					Wire.write(I2C_FAIL);
					return;
					break;
			}
			break;
		case I2C_MOVE:
			switch (param){
				case M_FORWARD:
					Serial.println("Received I2C_MOVE M_FORWARD");
					motorMove(M_A, M_FORWARD);
					motorMove(M_B, M_FORWARD);
					break;
				case M_BACKWARD:
					Serial.println("Received I2C_MOVE M_BACKWARD");
					motorMove(M_A, M_BACKWARD);
					motorMove(M_B, M_BACKWARD);
					break;
				default:
					Serial.println("Received I2C_MOVE with invalid param");
					Wire.write(I2C_FAIL);
					return;
					break;
			}
			break;
		case I2C_CHANGE_SPEED:
			Serial.print("Received I2C_CHANGE_SPEED ");
			Serial.println(param);
			motorSetSpeed(param);
			break;
		default:
			Serial.print("Received invalid command");
			Wire.write(I2C_FAIL);
			return;
			break;
	}
	Serial.print("Write success");
	Wire.write(I2C_SUCCESS);
}

void loop(){
}
