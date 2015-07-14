#include <Wire.h>
#define I2C_BUS 0x69
#define I2C_INIT 14
#define I2C_SET_DIRECTION 15
#define I2C_SET_SPEED 17
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
	Wire.onReceive(i2cCommandHandler);
	Wire.onRequest(i2cRequestHandler);
}

unsigned char last_command, last_result;

void i2cCommandHandler(int numBytes){
	unsigned char request, param;
	if (numBytes!=2) {
		Serial.print("Not enough bytes: need 2, got ");
		Serial.println(numBytes);
		setResult(request, I2C_FAIL);
		return;
	}
	request=Wire.read();
	param=Wire.read();
	switch (request){
		case I2C_INIT:
			Serial.println("Received I2C_INIT");
			motorInit();
			break;
		case I2C_SET_DIRECTION:
			switch (param){
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
	}
	Serial.println("Write success");
	setResult(request, I2C_SUCCESS);
}
void setResult(unsigned char request, unsigned char result){
	last_command=request;
	last_result=result;
}
void i2cRequestHandler(){
	Wire.write(last_command);
	Wire.write(last_result);
}
void loop(){
}
