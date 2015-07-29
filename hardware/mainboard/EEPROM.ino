#include <EEPROM.h>
#define EEPROM_SPEED 1023
/* eeprom struct:
0: number of dest
n => n+16: stuct dest (17 bytes)
	- n => n+3: uid
	- n+4 => n+16: name, end with null
*/

struct dest {
	unsigned long uid;
	char name[13];
};

void EEPROMWipe(){
	int i;
	for (i=0; i<1024; i++)
		EEPROM.write(i, 0);
}

unsigned char EEPROMGetSpeed(){
	return EEPROM.read(EEPROM_SPEED);
}

void EEPROMSetSpeed(unsigned char speed){
	return EEPROM.write(EEPROM_SPEED, speed);
}

int EEPROMNumberOfDest(){
	return EEPROM.read(0);
}

//checked
struct dest EEPROMReadDest(int address){
	struct dest dest_;
	EEPROM.get((address-1)*17+1, dest_);
	return dest_;
}

//checked
void EEPROMAddDest(unsigned long uid, char name[]){
	struct dest dest_;
	dest_.uid=uid;
	int i;
	for (i=0; i<14; i++)
		dest_.name[i]=name[i];
	int numberOfDest=EEPROMNumberOfDest();
	numberOfDest++;
	EEPROM.write(0, numberOfDest);
	EEPROM.put((numberOfDest-1)*17+1, dest_);
}
