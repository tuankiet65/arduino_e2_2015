#include "FastIO.h"
#include "I2CIO.h"
#include "LCD.h"
#include "LiquidCrystal.h"
#include "LiquidCrystal_I2C.h"
#define LCD_BACKPACK_I2C_BUS 0x27
#define LCD_BACKPACK_I2C_BUS 0x27
#define UNDEF 255
#define ZERO 100
#define ONE_HUNDRED_PLUS 100
#define TWO_HUNDRED_PLUS 200
#define ONE 101
#define TWO 102
#define THREE 103
#define FOUR 104
#define FIVE 105
#define SIX 106
#define SEVEN 107
#define EIGHT 108
#define NINE 109
#define CHANNEL_DOWN 110
#define CHANNEL 111
#define CHANNEL_UP 112
#define PREV 113
#define NEXT 114
#define PLAY_PAUSE 115
#define VOL_DOWN 116
#define VOL_UP 117
#define EQ 118
#define I2C_SET_DIRECTION 15
#define I2C_SET_SPEED 17

LiquidCrystal_I2C lcd(LCD_BACKPACK_I2C_BUS, 2, 1, 0, 4, 5, 6, 7);

void LCDInit(){
	lcd.setBacklightPin(3, POSITIVE);
	lcd.backlight();
	lcd.begin(16, 2);
	lcd.clear();
}

void LCDMessage(const char msg1[]="", const char msg2[]=""){
	lcd.clear();
	lcd.print(msg1);
	lcd.setCursor(0, 1);
	lcd.print(msg2);
}

unsigned char NineKeypadChar(unsigned char number, unsigned char *numPress, unsigned char capsLock){
	const unsigned char PROGMEM char_pos[]={0, 33, 65, 68, 71, 74, 77, 80, 84, 87, 91};
	if (number==1)
		*numPress%=16;
	else if (number==7 || number==9)
		*numPress%=5;
	else
		*numPress%=4;
	if (!(*numPress))
		return 48+number;
	if (!capsLock)
		return tolower(char_pos[number]+*numPress-1);
	return (char_pos[number]+*numPress-1);
}

void interfaceInputName(char name[], const char msg[]=""){
	unsigned char cursorPos=0, button, lastButton=0, pressCount=0, capsLock=0, i;
	LCDMessage(msg);
	lcd.setCursor(0, 1);
	lcd.blink();
	while (1){
		while (!IRRemoteReceive(&button) || button==UNDEF);
		switch (button){
			case PREV:
			case CHANNEL_DOWN:
			case VOL_DOWN:
				if (!cursorPos)
					cursorPos=12;
				cursorPos--;
				lcd.setCursor(cursorPos, 1);
				lastButton=0;
				pressCount=0;
				break;
			case NEXT:
			case CHANNEL_UP:
			case VOL_UP:
				cursorPos++;
				if (cursorPos>11)
					cursorPos=0;
				lcd.setCursor(cursorPos, 1);
				lastButton=0;
				pressCount=0;
				break;
			case CHANNEL:
			case PLAY_PAUSE:
			case EQ:
				for (i=0; i<12; i++)
					if (name[i]==0)
						name[i]=' ';
				name[12]=0;
				lcd.noBlink();
				lcd.clear();
				return;
			case ZERO:
				capsLock=!capsLock;
				break;
			case ONE:
			case TWO:
			case THREE:
			case FOUR:
			case FIVE:
			case SIX:
			case SEVEN:
			case EIGHT:
			case NINE:
				if (button==lastButton)
					pressCount++;
				else {
					lastButton=button;
					pressCount=1;
				}
				name[cursorPos]=NineKeypadChar(button-100, &pressCount, capsLock);
				lcd.write(name[cursorPos]);
				lcd.setCursor(cursorPos, 1);
		}
		piezoOkaySound();
	}
}

unsigned char interfaceSetSpeed(unsigned char *speed, unsigned char ir_result){
	unsigned char result=0;
	switch (ir_result){
		case VOL_UP:
		case NEXT:
		case CHANNEL_UP:
			if (*speed==255)
				break;
			(*speed)++;
			result=1;
			break;
		case VOL_DOWN:
		case PREV:
		case CHANNEL_DOWN:
			if (*speed<=50)
				break;
			(*speed)--;
			result=1;
			break;
	}
	motorI2CSendCommand(I2C_SET_SPEED, *speed);
	EEPROMSetSpeed(*speed);
	lcd.setCursor(0, 1);
	lcd.print(F("Speed: "));
	lcd.print(*speed);
	if (*speed<100)
		lcd.print(" ");
	return result;
}

void interfaceRunSetup(){
	EEPROMSetSpeed(222);
	unsigned char ir_result, speed=EEPROMGetSpeed(), hasFinished=0;
	unsigned long uid, initialUid, currTime, lastUid, count=0;
	char name[13]={0};
	LCDMessage("Setup required", "Press any key");
	while (!IRRemoteReceive(&ir_result) || ir_result==255);
	LCDMessage("Place robot into", "initial position");
	while (!RFIDGetCardUID(&uid));
	interfaceInputName(name, "Enter name:");
	EEPROMAddDest(uid, name);
	initialUid=uid;
	lastUid=uid;
	motorI2CSendCommand(I2C_SET_DIRECTION, M_FORWARD);
	LCDMessage("Discovering...");
	interfaceSetSpeed(&speed, 255);
	while (!hasFinished){
		motorI2CSendCommand(I2C_SET_DIRECTION, lineSensorRead());
		currTime=millis();
		while (millis()-currTime<100){
			if (ultrasoundHasObstacle()){
				motorI2CSendCommand(I2C_SET_SPEED, 0);
				LCDMessage("Obstacle", "detected");
				while (ultrasoundHasObstacle()){
					piezoErrorSound();
					delay(1000);
				}
				piezoNoTone();
				LCDMessage("Discovering...");
				interfaceSetSpeed(&speed, 255);
			}
			if (IRRemoteReceive(&ir_result))
				if (interfaceSetSpeed(&speed, ir_result))
					piezoOkaySound();
				else
					piezoErrorSound();
			if (RFIDGetCardUID(&uid)){
				if (uid==initialUid || uid!=lastUid){
					motorI2CSendCommand(I2C_SET_SPEED, 0);
					if (uid==initialUid || count!=0)
						hasFinished=1;
					else {
						lastUid=uid;
						interfaceInputName(name, "Enter name: ");
						EEPROMAddDest(uid, name);
						LCDMessage("Discovering...");
						interfaceSetSpeed(&speed, 255);
						count++;
					}
				}
			}
		}
	}
	LCDMessage("Setup completed", "Press any key");
	while (!IRRemoteReceive(&ir_result) || ir_result==255);
}

void interfacePrintDestName(struct destStruct dest){
	lcd.setCursor(0, 1);
	lcd.print(dest.name);
}

unsigned char interfaceWarning(const char msg_line1[]="", const char msg_line2[]=""){
	unsigned char ir_result;
	LCDMessage(msg_line1, msg_line2);
	while (!IRRemoteReceive(&ir_result) || ir_result==255);
	piezoOkaySound();
	return (ir_result==EQ);
}

void mainInterface(){
	struct destStruct dest, baseDest=EEPROMReadDest(1);
	unsigned char ir_result, curr_dest=1, numberOfDest=EEPROMNumberOfDest();
	char name[13]={0};
	LCDMessage("Choose dest:");
	dest=EEPROMReadDest(curr_dest);
	interfacePrintDestName(dest);
	while (1){
		while (!IRRemoteReceive(&ir_result) || ir_result==255);
		piezoOkaySound();
		switch (ir_result){
			case ZERO:
				if (interfaceWarning("Erase all data?")){
					LCDMessage("Erasing...");
					EEPROMWipe();
					LCDMessage("Erase completed", "Please reset");
					while (1);
				}
			case ONE:
			case TWO:
			case THREE:
			case FOUR:
			case FIVE:
			case SIX:
			case SEVEN:
			case EIGHT:
			case NINE:
				ir_result-=100;
				if (ir_result<=curr_dest){
					curr_dest=ir_result;
					dest=EEPROMReadDest(curr_dest);
					interfacePrintDestName(dest);
				}
				break;
			case VOL_UP:
			case CHANNEL_UP:
			case NEXT:
				curr_dest++;
				if (curr_dest>numberOfDest)
					curr_dest=1;
				dest=EEPROMReadDest(curr_dest);
				interfacePrintDestName(dest);
				break;
			case VOL_DOWN:
			case CHANNEL_DOWN:
			case PREV:
				curr_dest--;
				if (curr_dest<1)
					curr_dest=numberOfDest;
				dest=EEPROMReadDest(curr_dest);
				interfacePrintDestName(dest);
				break;
			case PLAY_PAUSE:
				move(dest);
				LCDMessage("Waiting...");
				while (!IRRemoteReceive(&ir_result) || ir_result==255);
				move(baseDest);
				return;
			case EQ:
				interfaceInputName(name, "Enter new name:");
				EEPROMModifyDestName(curr_dest, name);
				LCDMessage("Choost dest:");
				dest=EEPROMReadDest(curr_dest);
				interfacePrintDestName(dest);
				break;
			case CHANNEL:
				if (interfaceWarning("Delete this dest?")){
					EEPROMRemoveDest(curr_dest);
					if (numberOfDest==1){
						LCDMessage("No dests found", "Restart to setup");
						while (1);
					}
				}
				return;
		}
	}
}

void move(struct destStruct dest){
	unsigned char speed=EEPROMGetSpeed(), ir_result, hasFinished=0;
	unsigned long uid;
	LCDMessage("Moving...");
	motorI2CSendCommand(I2C_SET_DIRECTION, M_FORWARD);
	interfaceSetSpeed(&speed, 255);
	while (!hasFinished){
		motorI2CSendCommand(I2C_SET_DIRECTION, lineSensorRead());
		unsigned long curr_time=millis();
		while (millis()-curr_time<200){
			if (ultrasoundHasObstacle()){
				motorI2CSendCommand(I2C_SET_SPEED, 0);
				LCDMessage("Obstacle", "detected");
				while (ultrasoundHasObstacle()){
					piezoErrorSound();
					delay(1000);
				}
				piezoNoTone();
				LCDMessage("Moving...");
				interfaceSetSpeed(&speed, 255);
			}
			if (IRRemoteReceive(&ir_result))
				interfaceSetSpeed(&speed, ir_result);
			if (RFIDGetCardUID(&uid)){
				if (uid==dest.uid){
					hasFinished=1;
					motorI2CSendCommand(I2C_SET_SPEED, 0);
					return;
				}
			}
		}
	}
}