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

LiquidCrystal_I2C lcd(LCD_BACKPACK_I2C_BUS,2,1,0,4,5,6,7);

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

void LCDInputName(char **name){
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
					if (*name[i]==0)
						*name[i]=' ';
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
				*name[i]=NineKeypadChar(button-100, &pressCount, capsLock);
				lcd.write(*name[i]);
				lcd.setCursor(cursorPos, 1);
		}
	}
}
