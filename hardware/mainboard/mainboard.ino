void setup(){
	Serial.begin(9600);
	lineSensorInit();
	IRRemoteInit();
	motorI2CInit();
	RFIDInit();
	ultrasoundInit();
	piezoInit();
	LCDInit();
	if (!EEPROMNumberOfDest())
		interfaceRunSetup();
}

void loop(){
	mainInterface();
}
