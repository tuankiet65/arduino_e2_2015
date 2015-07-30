#include <digitalWriteFast.h>
#define US_TRIGGER_PIN 6
#define US_ECHO_PIN 7
#define US_MAX_DISTANCE 40

void ultrasoundInit(){
	pinModeFast(US_TRIGGER_PIN, OUTPUT);
	pinModeFast(US_ECHO_PIN, INPUT);
	digitalWriteFast(US_TRIGGER_PIN, LOW);
}

unsigned long ultrasoundRead(){
	digitalWriteFast(US_TRIGGER_PIN, LOW);
	delayMicroseconds(2);
	digitalWriteFast(US_TRIGGER_PIN, HIGH);
	delayMicroseconds(15);
	digitalWriteFast(US_TRIGGER_PIN, LOW);
	unsigned long result=pulseIn(US_ECHO_PIN, HIGH);
	if (result==0 || result>270000)
		return 0;
	else
		return result;
}

unsigned char ultrasoundHasObstacle(){
	unsigned char i, samplingRate=0;
	unsigned long result=0, tmp;
	float resultCm;
	for (i=0; i<10; i++){
		tmp=ultrasoundRead();
		if (tmp){
			result+=tmp;
			samplingRate++;
		}
	}
	result/=samplingRate;
	if (result==0)
		return 0;
	else {
		resultCm=float(result)/58;
		return (resultCm<=20);
	}
		
}