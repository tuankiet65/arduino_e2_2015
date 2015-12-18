// !PID
#define IN1 A0// Sensor T1 (left)
#define IN2 A1 // Sensor T2 (left)
#define IN3 A2 // Sensor T3 (Top)
#define IN4 A3 // Sensor T4 (right)
#define IN5 4 // Sensor T5 (right)
// Motor direction
#define M_FORWARD 10
#define M_LEFT 12
#define M_RIGHT 13

volatile int currOperation=M_FORWARD;

void lineSensorInit() {
	pinMode(IN1, INPUT);
	pinMode(IN2, INPUT);
	pinMode(IN3, INPUT);
	pinMode(IN4, INPUT);
	pinMode(IN5, INPUT);
}

int lineSensorRead(){
	unsigned char ir1, ir2, ir3, ir4, ir5, res=M_FORWARD;
	// 1=in line 0=out line
	ir1=!(digitalRead(IN1));
	ir2=!(digitalRead(IN2));
	ir3=!(digitalRead(IN3));
	ir4=!(digitalRead(IN4));
	ir5=!(digitalRead(IN5));
	Serial.print(ir1);
	Serial.print(" ");
	Serial.print(ir2);
	Serial.print(" ");
	Serial.print(ir3);
	Serial.print(" ");
	Serial.print(ir4);
	Serial.print(" ");
	Serial.print(ir5);
	Serial.print(" ");
	switch (currOperation){
		case M_FORWARD:
			Serial.print("M_FORWARD ");
			if (ir5){
				currOperation=M_RIGHT;
				res=M_RIGHT;
			} else if (ir1){
				currOperation=M_LEFT;
				res=M_LEFT;
			} else
			 	res=M_FORWARD;
			break;
		case M_RIGHT:
			Serial.print("M_RIGHT ");
			if (ir2 && ir3 && ir4){
				currOperation=M_FORWARD;
				res=M_FORWARD;
			} else if (ir1){
				currOperation=M_LEFT;
				res=M_LEFT;
			} else
				res=M_RIGHT;
			break;
		case M_LEFT:
			Serial.print("M_LEFT ");
			if (ir2 && ir3 && ir4){
				currOperation=M_FORWARD;
				res=M_FORWARD;
			} else if (ir5){
				currOperation=M_RIGHT;
				res=M_RIGHT;
			} else
				res=M_LEFT;
			break;
	}
	switch (res){
		case M_FORWARD:
			Serial.println("M_FORWARD");
			break;
		case M_RIGHT:
			Serial.println("M_RIGHT");
			break;
		case M_LEFT:
			Serial.println("M_LEFT");
			break;
	}
	return res;
}

void lineSensorDebug(){
	unsigned char ir1, ir2, ir3, ir4, ir5;
	ir1=digitalRead(IN1);
	ir2=digitalRead(IN2);
	ir3=digitalRead(IN3);
	ir4=digitalRead(IN4);
	ir5=digitalRead(IN5);
	Serial.print(ir1);
	Serial.print(" ");
	Serial.print(ir2);
	Serial.print(" ");
	Serial.print(ir3);
	Serial.print(" ");
	Serial.print(ir4);
	Serial.print(" ");
	Serial.print(ir5);
	Serial.print(" ");
	Serial.println();
}