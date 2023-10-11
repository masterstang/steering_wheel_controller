#include <Joystick.h>

#define STW_PIN A0
#define ACC_PIN 14
#define BRK_PIN 15

Joystick_ Joystick;

int xVal = 0;
double xAxis = 0;
//float multiplier = 1.4;
float multiplier = 5.6;

int accVal = HIGH;
int brkVal = HIGH;

void setup() {
  pinMode(ACC_PIN, INPUT);
  pinMode(BRK_PIN, INPUT);

  Joystick.setXAxisRange(-110, 110);
  Joystick.setYAxisRange(-127, 127);
  Joystick.begin();

  //Serial.begin(9600);
}

void loop() {
  xVal = analogRead(STW_PIN);
  accVal = digitalRead(ACC_PIN);
  brkVal = digitalRead(BRK_PIN);

  // hardcoded float = (max - min) / 1023
  xAxis = ((0.2150537634 * xVal) - 110) * multiplier;
   
  if (xAxis < -110) {
    xAxis = -110;
  }
  else if (xAxis > 110) {
    xAxis = 110;
  }

  Joystick.setXAxis(xAxis);
  Joystick.setYAxis(0);
  Joystick.setButton(1, accVal == LOW ? 1 : 0);
  Joystick.setButton(2, brkVal == LOW ? 1 : 0);
  Joystick.sendState();
  
  //Serial.println(xAxis);
  //Serial.println(accVal);
}
