#include <SparkFun_TB6612.h>

//enable talking with sparkfun motor driver
#define AIN1 12 //1 of 2 that controls direction left motor spins
#define BIN1 9 //1 of 2 that controls direciton right motor spins
#define AIN2 13 //2 of 2 that controls direction left motor spins
#define BIN2 8 //2 of 2 that controls direction right motor spins
#define PWMA 11 //controls speed of motor A
#define PWMB 10 //controls speed of motor B
#define STBY 7 //allows H-bridge to work when high
#define trigPin A1
#define echoPin A0

//Initializes all values for motor A
Motor leftMotor = Motor(12, 13, 11, -1, 7); 

//Initializes all values for motor B
Motor rightMotor = Motor(9, 8, 10, -1, 7); 

const int lightPin = A5;
const int ledPin = A4;

const int forwardButtonPin = 3;
const int leftButtonPin = 2; 
const int rightButtonPin = 6;
const int startButtonPin = 4; //4
const int backButtonPin = 5; //5



boolean fBPressed = false;
boolean lBPressed = false;
boolean rBPressed = false;
boolean sBPressed = false; 
boolean bBPressed = false;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
}

void loop() {

  int buttonDirections[20];
  for(int i =0; i <20; i++)
    buttonDirections[i] = 0;
  
  Serial.println("Ready to go!");
  
  if(analogRead(lightPin) < 275)
    digitalWrite(ledPin, HIGH);
  else
    digitalWrite(ledPin, LOW);
  
  //takes in all the button inputs and create the array with them
  for(int i = 0; i < 20; i++) {
    
    //function won't do anything until a button is pressed here
    while(!fBPressed || !rBPressed || !lBPressed || !sBPressed || !bBPressed) {

      //8 is forward, 4 is left, 6 is right, 5 is start, based off number pad

      /*if button gets pressed, fBPressed turns true, and the first part of the array is
        saved.*/
      fBPressed = digitalRead(forwardButtonPin);
      if(fBPressed) {
        buttonDirections[i] = 8; //assigns forward value to spot in array
        delay(250); //stops buttons from taking in too much input
        Serial.println("FBPRESS");
        break;
      }
      bBPressed = digitalRead(backButtonPin);
      if(bBPressed) {
        buttonDirections[i] = 2; //assigns backward value to spot in array
        delay(250); //stops buttons from taking in too much input
        Serial.println("BBPRESS");
        break;
      }
      lBPressed = digitalRead(leftButtonPin);
      if(lBPressed) {
        buttonDirections[i] = 4; //assigns left turn value to array spot
        delay(250); //stops buttons from taking in too much input
        Serial.println("LBPRESS");
        break;
      }
      rBPressed = digitalRead(rightButtonPin);
      if(rBPressed) {
        buttonDirections[i] = 6; //assigns right turn value to array spot
        delay(250); //stops buttons from taking in too much input
        Serial.println("RBPRESS");
        break;
      }
      sBPressed = digitalRead(startButtonPin);
      if(sBPressed) {
        delay(250); //stops buttons from taking in too much input
        Serial.println("SBPRESS");
        break;
      }
    }
    
    //stops the array building for loop to go into movement section
    if(sBPressed) {

      //resets all button booleans to prep for after movement section
      fBPressed = false;
      lBPressed = false;
      rBPressed = false;
      sBPressed = false; 
      bBPressed = false;
      break;
    }
  
    //resets all button booleans to prep for next value finding
    fBPressed = false;
    lBPressed = false;
    rBPressed = false;
    sBPressed = false; 
    bBPressed = false;
  }

  for(int i = 0; i < 20; i++) {
    Serial.println(buttonDirections[i]);
  }
    
  //using the values from the array, this will carry out each motion function
  for(int i =0; i < 20; i++) {

    int ultra = Ultrasonic();
    while(ultra < 400 && ultra > 0) {
      Stop();
      ultra = Ultrasonic();
    }
    if(buttonDirections[i] == 8) {
      MoveForward();
    }
    if(buttonDirections[i] == 4)
      TurnLeft();
    if(buttonDirections[i] == 6)
      TurnRight();
    if(buttonDirections[i] == 2)
      BackItUp();
  }
  
}

void MoveForward () {
  Serial.println("MoveForward Called");

  //drive(int speed, int duration);
  leftMotor.drive(100, 400);
  rightMotor.drive(100, 400);
  
  leftMotor.brake();
  rightMotor.brake();

  delay(2000);

}

void TurnLeft () {

  //drive(int speed, int duration);
  leftMotor.drive(-100, 500);
  rightMotor.drive(100, 500);
  
  Serial.println("TurnLeft Called");

   
  leftMotor.brake();
  rightMotor.brake();
  //to allow motors to respond

  delay(2000);
}

void TurnRight () {
  
  //drive(int speed, int duration);
  leftMotor.drive(100, 400);
  rightMotor.drive(-100, 400);

  Serial.println("TurnRight Called");

 
  leftMotor.brake();
  rightMotor.brake();
  //to allow motors to respond

  delay(2000);
}

void BackItUp () {

  //drive(int speed, int duration);
  leftMotor.drive(-100, 400);
  rightMotor.drive(-100, 400);
  
  leftMotor.brake();
  rightMotor.brake();

  delay(2000);
}

void Stop () {

  leftMotor.brake();
  rightMotor.brake();
  Serial.println("Stop Called");

  //to allow motors to respond
  delay(2000);
}

int Ultrasonic () {
  
  long duration, distance;
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);

  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  int echoVal = pulseIn(echoPin, HIGH);
  // less than 200 means super close to wall.
  delay(500);

  return echoVal;
  
}

