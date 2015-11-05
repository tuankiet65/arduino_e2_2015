#include <EEPROM.h>
#define EEPROM_SPEED_ADDR 1023
/* eeprom struct:
0: number of dest
n => n+16: stuct dest (17 bytes)
	- n => n+3: uid
	- n+4 => n+16: name, end with null
*/

struct destStruct {
	unsigned long uid;
	char name[13];
};

void EEPROMWipe(){
	int i;
	for (i=0; i<EEPROM.length(); i++)
		EEPROM.write(i, 0);
}

unsigned char EEPROMGetSpeed(){
	return EEPROM.read(EEPROM_SPEED_ADDR);
}

void EEPROMSetSpeed(unsigned char speed){
	return EEPROM.write(EEPROM_SPEED_ADDR, speed);
}

int EEPROMNumberOfDest(){
	return EEPROM.read(0);
}

int EEPROMDestAddress(unsigned char dest){
	return (dest-1)*17+1;
}

struct destStruct EEPROMReadDest(unsigned char address){
	struct destStruct dest;
	EEPROM.get(EEPROMDestAddress(address), dest);
	return dest;
}

void EEPROMAddDest(unsigned long uid, char name[]){
	unsigned char numberOfDest=EEPROMNumberOfDest(), i;
	numberOfDest++;
	struct destStruct dest;
	dest.uid=uid;
	for (i=0; i<14; i++)
		dest.name[i]=name[i];
	EEPROM.write(0, numberOfDest);
	EEPROM.put(EEPROMDestAddress(numberOfDest), dest);
}

void EEPROMRemoveDest(unsigned char dest){
	unsigned char numberOfDest=EEPROMNumberOfDest(), i;
	for (i=dest; i<=numberOfDest-1; i++)
		EEPROM.put(EEPROMDestAddress(i), EEPROMReadDest(i+1));
}

void EEPROMModifyDestName(unsigned char destNum, char newName[]){
	unsigned char i;
	struct destStruct dest=EEPROMReadDest(destNum);
	for (i=0; i<14; i++)
		dest.name[i]=newName[i]; // dies at 11th iteration; don't know why
	EEPROM.put(EEPROMDestAddress(destNum), dest);
}
