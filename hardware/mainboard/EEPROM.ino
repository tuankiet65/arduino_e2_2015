#include <EEPROM.h>
#define HAS_SETUP_ADDRESS 1023

struct dest {
	int RFIDSerial;
	char name[12];
};

struct dest mem;

int haveRunSetup(){
	return (EEPROM.read(HAS_SETUP_ADDRESS==255));
}
