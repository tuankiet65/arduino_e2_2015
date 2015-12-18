#define I2C_INIT 14 // param is random
#define I2C_SET_DIRECTION 15
#define I2C_SET_SPEED 17
#define I2C_SUCCESS 18
#define I2C_FAIL 19

void setup(){
	Serial.begin(9600);
	lineSensorInit();
	IRRemoteInit();
	motorI2CInit();
	RFIDInit();
	ultrasoundInit();
	piezoInit();
	LCDInit();
	interfaceRunSetup();
}

void loop(){	
	mainInterface();
}
