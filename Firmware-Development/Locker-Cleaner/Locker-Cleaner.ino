//   V7

#include <Servo.h> 
#include <LiquidCrystal.h>

#define L1_S1 14
#define L2_S1 15
#define L3_S1 16
#define L4_S1 17

#define STEPPER_X_DIR 2
#define STEPPER_X_CLK 3
#define STEPPER_Y_DIR 4
#define STEPPER_Y_CLK 5

#define L1_SERVO 10
#define L2_SERVO 11
#define L3_SERVO 12
#define L4_SERVO 13

#define RELAY 18

#define L1_IR_SENSOR A0
#define L2_IR_SENSOR A1
#define L3_IR_SENSOR A2
#define L4_IR_SENSOR A3

int IR_THRESHOLD_VALUE = 500;

int L1_STEPPER_X_STEPS = 1000;
int L1_STEPPER_Y_STEPS = 1000;
int L2_STEPPER_X_STEPS = 2000;
int L2_STEPPER_Y_STEPS = 1000;
int L3_STEPPER_X_STEPS = 3000;
int L3_STEPPER_Y_STEPS = 3100;
int L4_STEPPER_X_STEPS = 4530;
int L4_STEPPER_Y_STEPS = 3100;

int SERVO_INITIAL_ANGLE = 0;
int SERVO_FINAL_ANGLE = 90;

int sensor_1_value;
int sensor_2_value;
int sensor_3_value;
int sensor_4_value;

Servo myservo_1, myservo_2, myservo_3, myservo_4;

const int rs = 31, en = 32, d4 = 33, d5 = 34, d6 = 35, d7 = 36;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

unsigned long previousMillis = 0;
unsigned long currentMillis = 0;
const long interval = 1000;

bool L1_SERVO_FLAG = 0;
bool L2_SERVO_FLAG = 0;
bool L3_SERVO_FLAG = 0;
bool L4_SERVO_FLAG = 0;

bool L1_CHK_PASS_FLG = 0;
bool L2_CHK_PASS_FLG = 0;
bool L3_CHK_PASS_FLG = 0;
bool L4_CHK_PASS_FLG = 0;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);

  myservo_1.attach(L1_SERVO);
  myservo_2.attach(L2_SERVO);
  myservo_3.attach(L3_SERVO);
  myservo_4.attach(L4_SERVO);

  pinMode(L1_S1, INPUT_PULLUP);
  pinMode(L2_S1, INPUT_PULLUP);
  pinMode(L3_S1, INPUT_PULLUP);
  pinMode(L4_S1, INPUT_PULLUP);

  pinMode(L1_SERVO, OUTPUT);
  pinMode(L2_SERVO, OUTPUT);
  pinMode(L3_SERVO, OUTPUT);
  pinMode(L4_SERVO, OUTPUT);

  pinMode(STEPPER_X_DIR, OUTPUT);
  pinMode(STEPPER_X_CLK, OUTPUT);
  pinMode(STEPPER_Y_DIR, OUTPUT);
  pinMode(STEPPER_Y_CLK, OUTPUT);

  pinMode(RELAY, OUTPUT);


  myservo_1.write(SERVO_INITIAL_ANGLE);
  myservo_2.write(SERVO_INITIAL_ANGLE);
  myservo_3.write(SERVO_INITIAL_ANGLE);
  myservo_4.write(SERVO_INITIAL_ANGLE);
  delay(1000);



}
void loop() {
  Serial.println("Loop");
  delay(500);

  // **************************************LOCKER_1**************************************************

  lcd.setCursor(0, 0);
  lcd.print("READY TO USE    ");

  sensor_1_value = analogRead(L1_IR_SENSOR);

  if ((digitalRead(L1_S1)) && (sensor_1_value > IR_THRESHOLD_VALUE))
  {
    Serial.println("Loop 1");
    Serial.println(digitalRead(26));
    delay(500);

    if (!L1_CHK_PASS_FLG) {

      if (!(L1_SERVO_FLAG))
      {

        lcd.setCursor(0, 0);
        lcd.print("L1 STERILIZING  ");

        myservo_1.write(SERVO_FINAL_ANGLE);
        delay(500);


        digitalWrite(STEPPER_X_DIR, HIGH);
        digitalWrite(STEPPER_Y_DIR, HIGH);

        for (int i = 0 ; i < L1_STEPPER_X_STEPS ; i++)
        {
          digitalWrite(STEPPER_X_CLK, HIGH);
          delayMicroseconds(500);
          digitalWrite(STEPPER_X_CLK, LOW);
          delayMicroseconds(500);
        }

        for (int i = 0 ; i < L1_STEPPER_Y_STEPS ; i++)
        {
          digitalWrite(STEPPER_Y_CLK, HIGH);
          delayMicroseconds(500);
          digitalWrite(STEPPER_Y_CLK, LOW);
          delayMicroseconds(500);
        }
        delay(1000);

        digitalWrite(RELAY, HIGH);
        previousMillis = millis();


        L1_SERVO_FLAG = 1;

      }

      currentMillis = millis();
      if (currentMillis - previousMillis >= interval)
      {
        previousMillis = currentMillis;
        digitalWrite(RELAY, LOW);
        myservo_1.write(SERVO_INITIAL_ANGLE);

        digitalWrite(STEPPER_X_DIR, LOW);
        digitalWrite(STEPPER_Y_DIR, LOW);

        for (int i = 0 ; i < L1_STEPPER_X_STEPS ; i++)
        {
          digitalWrite(STEPPER_X_CLK, HIGH);
          delayMicroseconds(500);
          digitalWrite(STEPPER_X_CLK, LOW);
          delayMicroseconds(500);
        }

        for (int i = 0 ; i < L1_STEPPER_Y_STEPS ; i++)
        {
          digitalWrite(STEPPER_Y_CLK, HIGH);
          delayMicroseconds(500);
          digitalWrite(STEPPER_Y_CLK, LOW);
          delayMicroseconds(500);
        }
        L1_CHK_PASS_FLG = 1;
      }
    }
  }

  else if (sensor_1_value < IR_THRESHOLD_VALUE || !digitalRead(L1_S1))
  {
    Serial.println("Loop 1 else");
    delay(500);
    myservo_1.write(SERVO_INITIAL_ANGLE);
    L1_SERVO_FLAG = 0;
    L1_CHK_PASS_FLG = 0;
  }

  else{}





  //   ****************************************LOCKER_2***********************************************


  sensor_2_value = analogRead(L2_IR_SENSOR);

  if ((digitalRead(L2_S1)) && (sensor_2_value > IR_THRESHOLD_VALUE))  {

    Serial.println("Loop 2");
    Serial.println(digitalRead(27));
    delay(500);

    lcd.setCursor(0, 0);
    lcd.print("L2 STERILIZING  ");

    if (!L2_CHK_PASS_FLG) {

      if (!(L2_SERVO_FLAG))
      {

        lcd.setCursor(0, 0);
        lcd.print("L2 STERILIZING  ");

        myservo_2.write(SERVO_FINAL_ANGLE);
        delay(500);


        digitalWrite(STEPPER_X_DIR, HIGH);
        digitalWrite(STEPPER_Y_DIR, HIGH);

        for (int i = 0 ; i < L2_STEPPER_X_STEPS ; i++)
        {
          digitalWrite(STEPPER_X_CLK, HIGH);
          delayMicroseconds(500);
          digitalWrite(STEPPER_X_CLK, LOW);
          delayMicroseconds(500);
        }

        for (int i = 0 ; i < L2_STEPPER_Y_STEPS ; i++)
        {
          digitalWrite(STEPPER_Y_CLK, HIGH);
          delayMicroseconds(500);
          digitalWrite(STEPPER_Y_CLK, LOW);
          delayMicroseconds(500);
        }
        delay(1000);

        digitalWrite(RELAY, HIGH);
        previousMillis = millis();

        L2_SERVO_FLAG = 1;

      }
      currentMillis = millis();
      if (currentMillis - previousMillis >= interval)
      {
        previousMillis = currentMillis;
        digitalWrite(RELAY, LOW);
        myservo_2.write(SERVO_INITIAL_ANGLE);

        digitalWrite(STEPPER_X_DIR, LOW);
        digitalWrite(STEPPER_Y_DIR, LOW);

        for (int i = 0 ; i < L2_STEPPER_X_STEPS ; i++)
        {
          digitalWrite(STEPPER_X_CLK, HIGH);
          delayMicroseconds(500);
          digitalWrite(STEPPER_X_CLK, LOW);
          delayMicroseconds(500);
        }

        for (int i = 0 ; i < L2_STEPPER_Y_STEPS ; i++)
        {
          digitalWrite(STEPPER_Y_CLK, HIGH);
          delayMicroseconds(500);
          digitalWrite(STEPPER_Y_CLK, LOW);
          delayMicroseconds(500);
        }
        L2_CHK_PASS_FLG = 1;
      }
    }
  }

  else if (sensor_2_value < IR_THRESHOLD_VALUE || !digitalRead(L2_S1))
  {
    Serial.println("Loop 2 else");
    delay(500);
    myservo_2.write(SERVO_INITIAL_ANGLE);
    L2_SERVO_FLAG = 0;
    L2_CHK_PASS_FLG = 0;
  }

  else{}





  //   ****************************************LOCKER_3***********************************************


  sensor_3_value = analogRead(L3_IR_SENSOR);

  if ((digitalRead(L3_S1)) && (sensor_3_value > IR_THRESHOLD_VALUE))
  {

    Serial.println("Loop 3");
    Serial.println(digitalRead(28));
    delay(500);

    if (!L3_CHK_PASS_FLG) {
      if (!(L3_SERVO_FLAG))
      {

        lcd.setCursor(0, 0);
        lcd.print("L3 STERILIZING  ");

        myservo_3.write(SERVO_FINAL_ANGLE);
        delay(500);

        digitalWrite(STEPPER_X_DIR, HIGH);
        digitalWrite(STEPPER_Y_DIR, HIGH);

        for (int i = 0 ; i < L3_STEPPER_X_STEPS ; i++)
        {
          digitalWrite(STEPPER_X_CLK, HIGH);
          delayMicroseconds(500);
          digitalWrite(STEPPER_X_CLK, LOW);
          delayMicroseconds(500);
        }

        for (int i = 0 ; i < L3_STEPPER_Y_STEPS ; i++)
        {
          digitalWrite(STEPPER_Y_CLK, HIGH);
          delayMicroseconds(500);
          digitalWrite(STEPPER_Y_CLK, LOW);
          delayMicroseconds(500);
        }
        delay(1000);

        digitalWrite(RELAY, HIGH);
        previousMillis = millis();

        L3_SERVO_FLAG = 1;

      }


      currentMillis = millis();
      if (currentMillis - previousMillis >= interval)
      {
        previousMillis = currentMillis;
        digitalWrite(RELAY, LOW);
        myservo_3.write(SERVO_INITIAL_ANGLE);

        digitalWrite(STEPPER_X_DIR, LOW);
        digitalWrite(STEPPER_Y_DIR, LOW);

        for (int i = 0 ; i < L3_STEPPER_X_STEPS ; i++)
        {
          digitalWrite(STEPPER_X_CLK, HIGH);
          delayMicroseconds(500);
          digitalWrite(STEPPER_X_CLK, LOW);
          delayMicroseconds(500);
        }

        for (int i = 0 ; i < L3_STEPPER_Y_STEPS ; i++)
        {
          digitalWrite(STEPPER_Y_CLK, HIGH);
          delayMicroseconds(500);
          digitalWrite(STEPPER_Y_CLK, LOW);
          delayMicroseconds(500);
        }
        L3_CHK_PASS_FLG = 1;
      }
    }
  }

  else if (sensor_3_value < IR_THRESHOLD_VALUE || !digitalRead(L3_S1))
  {
    Serial.println("Loop 3 else");
    delay(500);
    myservo_3.write(SERVO_INITIAL_ANGLE);
    L3_SERVO_FLAG = 0;
    L3_CHK_PASS_FLG = 0;
  }

  else{}



  //   ****************************************LOCKER_4***********************************************


  sensor_4_value = analogRead(L4_IR_SENSOR);

  if ((digitalRead(L4_S1)) && (sensor_4_value > IR_THRESHOLD_VALUE))
  {
    Serial.println("Loop 4");
    Serial.println(digitalRead(29));
    delay(500);
    if (!L4_CHK_PASS_FLG) {
      if (!(L4_SERVO_FLAG))
      {

        lcd.setCursor(0, 0);
        lcd.print("L4 STERILIZING  ");
        myservo_4.write(SERVO_FINAL_ANGLE);
        delay(500);


        digitalWrite(STEPPER_X_DIR, HIGH);
        digitalWrite(STEPPER_Y_DIR, HIGH);

        for (int i = 0 ; i < L4_STEPPER_X_STEPS ; i++)
        {
          digitalWrite(STEPPER_X_CLK, HIGH);
          delayMicroseconds(500);
          digitalWrite(STEPPER_X_CLK, LOW);
          delayMicroseconds(500);
        }

        for (int i = 0 ; i < L4_STEPPER_Y_STEPS ; i++)
        {
          digitalWrite(STEPPER_Y_CLK, HIGH);
          delayMicroseconds(500);
          digitalWrite(STEPPER_Y_CLK, LOW);
          delayMicroseconds(500);
        }
        delay(1000);

        digitalWrite(RELAY, HIGH);
        previousMillis = millis();
        L4_SERVO_FLAG = 1;
      }
      currentMillis = millis();
      if (currentMillis - previousMillis >= interval)
      {
        previousMillis = currentMillis;
        digitalWrite(RELAY, LOW);
        myservo_4.write(SERVO_INITIAL_ANGLE);

        digitalWrite(STEPPER_X_DIR, LOW);
        digitalWrite(STEPPER_Y_DIR, LOW);

        for (int i = 0 ; i < L4_STEPPER_X_STEPS ; i++)
        {
          digitalWrite(STEPPER_X_CLK, HIGH);
          delayMicroseconds(500);
          digitalWrite(STEPPER_X_CLK, LOW);
          delayMicroseconds(500);
        }

        for (int i = 0 ; i < L4_STEPPER_Y_STEPS ; i++)
        {
          digitalWrite(STEPPER_Y_CLK, HIGH);
          delayMicroseconds(500);
          digitalWrite(STEPPER_Y_CLK, LOW);
          delayMicroseconds(500);
        }
        L4_CHK_PASS_FLG = 1;
      }
    }
  }

  else if (sensor_4_value < IR_THRESHOLD_VALUE || !digitalRead(L4_S1))
  {
    Serial.println("Loop 4 else");
    delay(500);
    myservo_4.write(SERVO_INITIAL_ANGLE);
    L4_SERVO_FLAG = 0;
    L4_CHK_PASS_FLG = 0;
  }

  else{}
  
}
