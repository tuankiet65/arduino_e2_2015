#define PIEZO_PIN 5
#define ERROR_FREQ 500
#define OKAY_FREQ 500
#define ERROR_DURA 500
#define OKAY_DURA 100

void piezoInit(){
	pinMode(PIEZO_PIN, OUTPUT);
}

void piezoErrorSound(){
	tone(PIEZO_PIN, ERROR_FREQ, ERROR_DURA);
}

void piezoOkaySound(){
	tone(PIEZO_PIN, OKAY_FREQ, OKAY_DURA);
}

void piezoNoTone(){
	noTone(PIEZO_PIN);
}
