#define INLINE 1
#define M_FORWARD 10
#define M_BACKWARD 11
#define M_LEFT 12
#define M_RIGHT 13
#define I2C_INIT 14
#define I2C_SET_DIRECTION 15
#define I2C_SET_SPEED 17
#define I2C_SUCCESS 18
#define I2C_FAIL 19
char *name;
void setup() {
	Serial.begin(9600);
	IRRemoteInit();
	LCDInit();
	LCDInputName(&name);
	Serial.write(name, 12);
}

void loop() {
}
