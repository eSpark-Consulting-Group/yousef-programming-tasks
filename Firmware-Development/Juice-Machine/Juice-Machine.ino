#include <Arduino.h>
#include "BasicStepperDriver.h"
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <HCSR04.h>

#define DOWN_BUTTON_PIN 5
#define UP_BUTTON_PIN 6
#define SELECT_BUTTON_PIN 7

#define SERVO_PIN 2

#define MOTOR_STEPS 500
#define RPM 120
#define DIR 3
#define STEP 4

#define STEPPER_START_LIMIT_SWITCH 12
#define STEPPER_STOP_LIMIT_SWITCH 13

#define START_ULTRASONIC_TRIG_PIN 8
#define START_ULTRASONIC_ECHO_PIN 9
#define STOP_ULTRASONIC_TRIG_PIN 10
#define STOP_ULTRASONIC_ECHO_PIN 11

#define PUMP_RELAY_1 14
#define PUMP_RELAY_2 15
#define PUMP_RELAY_3 16
#define PUMP_RELAY_4 17

#define DC_MOTOR_1 18
#define DC_MOTOR_2 19
#define DC_MOTOR_3 20
#define DC_MOTOR_4 21


int UP_BUTTON_STATE = 1;
int LAST_UP_BUTTON_STATE = 1;

int DOWN_BUTTON_STATE = 1;
int LAST_DOWN_BUTTON_STATE = 1;

int SELECT_BUTTON_STATE = 1;
int LAST_SELECT_BUTTON_STATE = 1;

int COUNTER = 0;
int SELECT = -1;
int PREVIOUS_COUNTER = 0;

int TOTAL_ITEMS = 7;

float START_ULTRASONIC_DISTANCE = 0;
float STOP_ULTRASONIC_DISTANCE = 0;

int SERVO_FINAL_ANGLE = 40 ;

//const int rs = 22, en = 23, d4 = 24, d5 = 25, d6 = 26, d7 = 27;
LiquidCrystal_I2C lcd(0x20, 20, 4);
//HCSR04 hc(START_ULTRASONIC_TRIG_PIN, START_ULTRASONIC_ECHO_PIN);
//HCSR04 hc1(STOP_ULTRASONIC_TRIG_PIN, STOP_ULTRASONIC_ECHO_PIN);
Servo myservo;
BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP);



void setup() {

  Serial.begin(9600);
   lcd.begin();
   lcd.clear();
   lcd.backlight();

   myservo.attach(SERVO_PIN);
   myservo.write(0);

   stepper.begin(RPM);

  pinMode(UP_BUTTON_PIN, INPUT_PULLUP);
  pinMode(DOWN_BUTTON_PIN, INPUT_PULLUP);
  pinMode(SELECT_BUTTON_PIN, INPUT_PULLUP);

  pinMode(STEPPER_START_LIMIT_SWITCH, INPUT_PULLUP);
  pinMode(STEPPER_STOP_LIMIT_SWITCH, INPUT_PULLUP);

  pinMode(PUMP_RELAY_1, OUTPUT);
  pinMode(PUMP_RELAY_2, OUTPUT);
  pinMode(PUMP_RELAY_3, OUTPUT);
  pinMode(PUMP_RELAY_4, OUTPUT);

  pinMode(DC_MOTOR_1, OUTPUT);
  pinMode(DC_MOTOR_2, OUTPUT);
  pinMode(DC_MOTOR_3, OUTPUT);
  pinMode(DC_MOTOR_4, OUTPUT);
}

void loop() {

  //*****************************************ORDERING I2C LCD************************************************
  

  PREVIOUS_COUNTER = COUNTER ;
  UP_BUTTON_STATE = digitalRead(UP_BUTTON_PIN);
  DOWN_BUTTON_STATE = digitalRead(DOWN_BUTTON_PIN);
  SELECT_BUTTON_STATE = digitalRead(SELECT_BUTTON_PIN);
  
  if (UP_BUTTON_STATE != LAST_UP_BUTTON_STATE)
  {
    if (UP_BUTTON_STATE == HIGH) 
    {
      
      if(COUNTER < TOTAL_ITEMS-1)
      {
        COUNTER++;
        Serial.println(COUNTER);
      }
      else if(COUNTER == TOTAL_ITEMS-1)
      {
        COUNTER = 0;
        Serial.println(COUNTER);
      }
    } 
    // Delay a little bit to avoid bouncing
    delay(50);
  }


if (DOWN_BUTTON_STATE != LAST_DOWN_BUTTON_STATE)
{
    
    if (DOWN_BUTTON_STATE == HIGH) 
    {
      
      if(COUNTER > 0)
      {
         COUNTER--;
         Serial.println(COUNTER);
      }
      else if(COUNTER == 0)
      {
         COUNTER = TOTAL_ITEMS-1 ;
         Serial.println(COUNTER);
      }
    }
  // Delay a little bit to avoid bouncing
  delay(50);
 }
  LAST_UP_BUTTON_STATE = UP_BUTTON_STATE;
  LAST_DOWN_BUTTON_STATE = DOWN_BUTTON_STATE;
  
  if(COUNTER != PREVIOUS_COUNTER)
  {
  lcd.clear();
  }
  
  if(COUNTER<=3)
  {
  lcd.setCursor(0, COUNTER);
  lcd.print("-->");
  
  lcd.setCursor(4, 0);
  lcd.print("Juice 1");
  lcd.setCursor(4, 1);
  lcd.print("Juice 2");
  lcd.setCursor(4, 2);
  lcd.print("Juice 3");
  lcd.setCursor(4, 3);
  lcd.print("Juice 4");
  }
  else if(COUNTER>3)
  {
  lcd.setCursor(0, COUNTER-4);
  lcd.print("-->");

  lcd.setCursor(4, 0);
  lcd.print("Mix 1");
  lcd.setCursor(4, 1);
  lcd.print("Mix 2");
  lcd.setCursor(4, 2);
  lcd.print("Mix 3");
  }



  if (SELECT_BUTTON_STATE != LAST_SELECT_BUTTON_STATE)
  {
    if (SELECT_BUTTON_STATE == HIGH) 
    {
      SELECT = COUNTER ;
      if(COUNTER <= 3)
      {
      lcd.setCursor(12, COUNTER);
      }
      else if(COUNTER > 3)
      {
      lcd.setCursor(12, COUNTER-4);
      }
      lcd.print("(SEL)");
      Serial.println(COUNTER);
    }
    delay(50);
  }
  LAST_SELECT_BUTTON_STATE = SELECT_BUTTON_STATE;

  Serial.println(COUNTER);

  //************************************STARTING PART**********************************************


    
    START_ULTRASONIC_DISTANCE = hc.dist();

    if((START_ULTRASONIC_DISTANCE < 15) && (digitalRead(STEPPER_START_LIMIT_SWITCH))==0 && (SELECT >= 0) && (SELECT < TOTAL_ITEMS)  )
    {
      myservo.write(SERVO_FINAL_ANGLE);
      delay(2000);
      myservo.write(0);
      stepper.move(MOTOR_STEPS);
      delay(5000);  //to be adjusted according to requirement


  //**************************************JUICE FILLING***********************************************
  
      
      if(SELECT == 0)
      {
        digitalWrite(PUMP_RELAY_1, HIGH);
      }
      else if(SELECT == 1)
      {
        digitalWrite(PUMP_RELAY_2, HIGH);
      }
      else if(SELECT == 2)
      {
        digitalWrite(PUMP_RELAY_3, HIGH);
      }
      else if(SELECT == 3)
      {
        digitalWrite(PUMP_RELAY_4, HIGH);
      }
      else if(SELECT == 4)
      {
        digitalWrite(PUMP_RELAY_1, HIGH);
        digitalWrite(PUMP_RELAY_2, HIGH);
      }
      else if(SELECT == 5)
      {
        digitalWrite(PUMP_RELAY_3, HIGH);
        digitalWrite(PUMP_RELAY_4, HIGH);
      }
      else if(SELECT == 6)
      {
        digitalWrite(PUMP_RELAY_1, HIGH);
        digitalWrite(PUMP_RELAY_2, HIGH);
        digitalWrite(PUMP_RELAY_3, HIGH);
        digitalWrite(PUMP_RELAY_4, HIGH);
      }
      delay(2000); //to be adjusted according to requirement 
      digitalWrite(PUMP_RELAY_1, LOW);
      digitalWrite(PUMP_RELAY_2, LOW);
      digitalWrite(PUMP_RELAY_3, LOW);
      digitalWrite(PUMP_RELAY_4, LOW);

      delay(2000); //to be adjusted according to requirement


    //************************************AFTER FILLING********************************************
      

      stepper.move(MOTOR_STEPS);
      delay(5000); //to be adjusted according to requirement
      digitalWrite(DC_MOTOR_1, HIGH);
      digitalWrite(DC_MOTOR_2, HIGH);
      delay(5000);  //to be adjusted according to requirement
      digitalWrite(DC_MOTOR_1, LOW);
      digitalWrite(DC_MOTOR_2, LOW);

      stepper.move(MOTOR_STEPS);
      delay(5000); //to be adjusted according to requirement
      digitalWrite(DC_MOTOR_3, HIGH);
      digitalWrite(DC_MOTOR_4, HIGH);
      delay(5000);  //to be adjusted according to requirement
      digitalWrite(DC_MOTOR_3, LOW);
      digitalWrite(DC_MOTOR_4, LOW);
      
      
      while(digitalRead(STEPPER_STOP_LIMIT_SWITCH) != 0)
      {
        stepper.move(MOTOR_STEPS);
      }


      while(hc1.dist() < 15) // Monitor stop ultrasonic to see if cup is taken or not
      {
        delay(10);
      }

      while(digitalRead(STEPPER_START_LIMIT_SWITCH) != 0)     //stepper goes to initial position
      {
        stepper.move(-MOTOR_STEPS);
      }
      
     
    }





    
    if(START_ULTRASONIC_DISTANCE >= 15)
    {
    lcd.clear();
    while (START_ULTRASONIC_DISTANCE > 15)
    {
      lcd.setCursor(0, 0);
      lcd.print("CUPS NOT AVAILABLE");
    }
    lcd.clear();
    }

    SELECT = -1;

}
