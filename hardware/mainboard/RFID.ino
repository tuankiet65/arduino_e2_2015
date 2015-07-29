#include "SPI.h"
#include "MFRC522.h"
#define SS_PIN 10
#define RST_PIN 10
MFRC522 mfrc522(SS_PIN, RST_PIN);

// checked
void RFIDInit(){
	SPI.begin();
	mfrc522.PCD_Init();
}

// checked
unsigned char RFIDGetCardUID(unsigned long *uid){
	if (!mfrc522.PICC_IsNewCardPresent())
		return 0;
	if (!mfrc522.PICC_ReadCardSerial())
		return 0;
	if (mfrc522.uid.size!=4)
		return 0;
	int i;
	for (i=0; i<4; i++)
		*uid=(*uid<<8)+mfrc522.uid.uidByte[i];
	mfrc522.PICC_HaltA();
	return 1;
}
