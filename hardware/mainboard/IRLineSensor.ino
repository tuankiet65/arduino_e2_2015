#define IN1 A0
#define IN2 A1
#define IN3 A2
#define IN4 A3
// IR sensor maximum level (common)
#define IRTOP_LV 500
// IR maximum level (specific)
#define TLV 200 // limit of IR1 (top)
#define BMLV 100 // limit of IR3 (mid bottom)
#define NTL_LV 400 // limit of IR2 (closest to LCD)
#define OTL_LV 200 // limit of IR4 (second closest to LCD)
// Motor direction
#define M_LEFT 12
#define M_RIGHT 13
#define INLINE 1

void initSensor() {
  pinMode(IN1, INPUT);
  pinMode(IN2, INPUT);
  pinMode(IN3, INPUT);
  pinMode(IN4, INPUT);
}

int isInLine() {
  // ir > 300 = inline; else outline
  int ir1 = analogRead(IN1);
  int ir3 = analogRead(IN3);
  return (ir1>=TLV && ir3>=BMLV);
}

int getStatus() {
  if (analogRead(IN2)>NTL_LV)
    return M_RIGHT;
  if (analogRead(IN4)>OTL_LV)
    return M_LEFT;
  if (isInLine())
    return INLINE;
  return 0;
}

int getValue(int port) {
  return analogRead(port);
}
