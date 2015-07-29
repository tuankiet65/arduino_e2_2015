#include "FastIO.h"
#include "I2CIO.h"
#include "LCD.h"
#include "LiquidCrystal.h"
#include "LiquidCrystal_I2C.h"
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

LiquidCrystal_I2C lcd(LCD_BACKPACK_I2C_BUS, 2, 1, 0, 4, 5, 6, 7);

void LCDInit(){
	lcd.setBacklightPin(3, POSITIVE);
	lcd.backlight();
	lcd.begin(16, 2);
	lcd.clear();
}

unsigned char NineKeypadChar(unsigned char number, unsigned char *numPress, unsigned char capsLock){
	const unsigned char PROGMEM char_pos[]={0, 33, 65, 68, 71, 74, 77, 80, 84, 87, 91};
	if (number==1)
		*numPress%=15;
	else if (number==7 || number==9)
		*numPress%=4;
	else
		*numPress%=3;
	if (!capsLock)
		return tolower(char_pos[number]+*numPress);
	else
		return (char_pos[number]+*numPress);
}

//checked
void interfaceInputName(char name[]){
	unsigned char cursorPos=0, button, lastButton=0, pressCount=0, capsLock=0;
	lcd.setCursor(0, 1);
	lcd.blink();
	while (1){
		while (!IRRemoteReceive(&button) || button==UNDEF);
		switch (button){
			case PREV:
			case CHANNEL_DOWN:
			case VOL_DOWN:
				cursorPos--;
				if (cursorPos<0)
					cursorPos=11;
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
			case EQ:
				int i;
				for (i=0; i<11; i++)
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
				if (button==lastButton){
					pressCount++;
				} else {
					lastButton=button;
					pressCount=0;
				}
				name[cursorPos]=NineKeypadChar(button-100, &pressCount, capsLock);
				lcd.write(name[cursorPos]);
				lcd.setCursor(cursorPos, 1);
		}
	}
}

void interfaceSetSpeed(unsigned char *speed, unsigned char ir_result){
	switch (ir_result){
		case VOL_UP:
			if (*speed==255)
				break;
			*speed++;
			break;
		case VOL_DOWN:
			if (*speed<=50)
				break;
			*speed--;
			break;
	}
	motorI2CSendCommand(I2C_SET_SPEED, *speed);
	EEPROMSetSpeed(*speed);
	lcd.setCursor(7, 10);
	lcd.print(*speed);
	if (*speed<100)
		lcd.print(" ");
}

void interfaceRunSetup(){
	unsigned char ir_result, speed=EEPROMGetSpeed(), hasFinished=0;
	unsigned long uid, initialUid, currTime;
	char name[13]={0};
	lcd.clear();
	lcd.print("You need to setup");
	lcd.setCursor(0, 1);
	lcd.print("Press any key");
	while (!IRRemoteReceive(&ir_result));
	lcd.clear();
	lcd.print("Place the robot into the line, on the initial position");
	while (!RFIDGetCardUID(&uid));
	lcd.clear();
	lcd.print("Enter name:");
	interfaceInputName(name);
	lcd.clear();
	lcd.print("Saving");
	EEPROMAddDest(uid, name);
	initialUid=uid;
	lcd.clear();
	lcd.print("Discovering");
	lcd.setCursor(0, 1);
	lcd.print("Speed: ");
	lcd.print(speed);
	motorI2CSendCommand(I2C_SET_DIRECTION, M_FORWARD);
	motorI2CSendCommand(I2C_SET_SPEED, speed);
	while (!hasFinished){
		motorI2CSendCommand(I2C_SET_DIRECTION, lineSensorRead());
		currTime=millis();
		while (millis()-currTime<200){
			if (IRRemoteReceive(&ir_result))
				interfaceSetSpeed(&speed, ir_result);		
			if (RFIDGetCardUID(&uid)){
				motorI2CSendCommand(I2C_SET_SPEED, 0);
				if (uid==initialUid)
					hasFinished=1;
				else {
					lcd.clear();
					lcd.print("Enter name:");
					interfaceInputName(name);
					lcd.clear();
					lcd.print("Saving");
					EEPROMAddDest(uid, name);
					motorI2CSendCommand(I2C_SET_SPEED, speed);
					lcd.clear();
					lcd.print("Discovering");
					lcd.setCursor(0, 1);
					lcd.print("Speed: ");
					lcd.print(speed);
				}
			}
		}
	}
	lcd.clear();
	lcd.print("Setup completed. Press any key to exit");
	while (!IRRemoteReceive(&ir_result));
	lcd.clear();
}

void LCDPrintDestName(struct dest dest_){
	lcd.setCursor(0, 1);
	lcd.print(dest_.name);
}

void mainInterface(){
	struct dest dest_;
	unsigned char ir_res, curr_dest=1, numberOfDest=EEPROMNumberOfDest();
	lcd.clear();
	lcd.print("Choose your dest");
	lcd.setCursor(0, 1);
	dest_=EEPROMReadDest(curr_dest);
	lcd.print(dest_.name);
	while (1){
		while (!IRRemoteReceive(&ir_res));
		switch (ir_res){
			case ONE:
			case TWO:
			case THREE:
			case FOUR:
			case FIVE:
			case SIX:
			case SEVEN:
			case EIGHT:
			case NINE:
				if (ir_res-100<=curr_dest){
					curr_dest=ir_res-100;
					dest_=EEPROMReadDest(curr_dest);
					LCDPrintDestName(dest_);
				}
				break;
			case VOL_UP:
			case CHANNEL_UP:
				curr_dest++;
				if (curr_dest>numberOfDest)
					curr_dest=1;
				dest_=EEPROMReadDest(curr_dest);
				LCDPrintDestName(dest_);
				break;
			case VOL_DOWN:
			case CHANNEL_DOWN:
				curr_dest--;
				if (!curr_dest)
					curr_dest=numberOfDest;
				dest_=EEPROMReadDest(curr_dest);
				LCDPrintDestName(dest_);
				break;
			case EQ:
			case CHANNEL:
				move(dest_);
				return;		
		}
	}
}

void move(struct dest dest_){
	unsigned char speed=EEPROMGetSpeed(), ir_result, hasFinished=0;
	unsigned long uid;
	lcd.clear();
	lcd.print("Moving");
	lcd.setCursor(0, 1);
	lcd.print("Speed: ");
	lcd.print(speed);
	motorI2CSendCommand(I2C_SET_SPEED, speed);
	motorI2CSendCommand(I2C_SET_DIRECTION, M_FORWARD);
	while (!hasFinished){
		motorI2CSendCommand(I2C_SET_DIRECTION, lineSensorRead());
		unsigned long curr_time=millis();
		while (millis()-curr_time<200){
			if (IRRemoteReceive(&ir_result))
				interfaceSetSpeed(&speed, ir_result);
			if (RFIDGetCardUID(&uid)){
				if (uid==dest_.uid){
					hasFinished=1;
					motorI2CSendCommand(I2C_SET_SPEED, 0);
					return;
				}
			}
		}
	}
}

// debugging function
void LCDLine(int ir1, int ir2, int ir3, int ir4){
	lcd.clear();
	lcd.setCursor(5, 0);
	lcd.print(ir1);
	lcd.setCursor(0, 1);
	lcd.print(ir2);
	lcd.setCursor(5, 1);
	lcd.print(ir3);
	lcd.setCursor(10, 1);
	lcd.print(ir4);
}
