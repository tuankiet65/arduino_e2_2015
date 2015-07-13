#include <digitalWriteFast.h>
#define US_TRIGGER_PIN 8
#define US_ECHO_PIN 9

void ultrasoundInit(){
	pinModeFast(US_TRIGGER_PIN, OUTPUT);
	pinModeFast(US_ECHO_PIN, INPUT);
	digitalWriteFast(US_TRIGGER_PIN, LOW);
}

unsigned long ultrasoundRead(){
	digitalWriteFast(US_TRIGGER_PIN, LOW);
	digitalWriteFast(US_TRIGGER_PIN, HIGH);
	delayMicroseconds(15);
	digitalWriteFast(US_TRIGGER_PIN, LOW);
	unsigned long result=pulseIn(US_ECHO_PIN, HIGH);
	if (result==0 || result>270000)
		return 0;
	else
		return result;
}

float ultrasoundReadCm(){
	unsigned long result=ultrasoundRead();
	if (result==0)
		return -1.0f;
	else
		return float(ultrasoundRead())/58;
}

float ultrasoundReadInch(){
	unsigned long result=ultrasoundRead();
	if (result==0)
		return -1.0f;
	else
		return float(ultrasoundRead())/148;
}