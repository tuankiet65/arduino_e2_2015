void setup(){
	pinMode(A4, OUTPUT);
	pinMode(A5, OUTPUT);
}

void loop(){
	digitalWrite(A4, HIGH);
	delay(2000);
	digitalWrite(A4, LOW);
	digitalWrite(A5, HIGH);
	delay(2000);
	digitalWrite(A5, LOW);
}