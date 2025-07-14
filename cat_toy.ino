/* This code is for a "peek-a-boo" style cat toy. The circuit uses an ultrasonic sensor with distance function. When the cat, or any object, 
is detected within 6 inches of the sensor, mounted at the top of a doorway on one side of a box, a toy attached to a servo motor swings
to appear in the doorway. If the cat moves within 3 inches of the doorway, the toy "hides" by swinging 90 degrees to a solid wall. 
The first prototype of the toy built to run this code utilized the SparkFun Electronics Inventors' kit with Qwiic RedBoard microcontroller.
Code made possible by the developers at Arduino and their forums.*/

#include <Servo.h>            //include the servo library

const int trigPin1 = 6;   //connects to the trigger pin on US sensor 1
const int echoPin1 = 7;   //connects to the echo pin on US sensor 1

const int trigPin2 = 11;   //connects to the trigger pin on US sensor 2
const int echoPin2 = 12;   //connects to the echo pin on US sensor 2

const int LED1 = 2;        //pin to control LED1
const int LED2 = 4;        //pin to control LED2

float distance1 = 0;                     //stores distance measured by US sensor 1
float distance2 = 0;
float distance3 = 0;
float distance4 = 0;

const int ELAPSED_TIME1 = 1000; //milliseconds, 1st timer will control how often US sensor 1 updates
const int TOY_SHOWN_ELAPSED_TIME1 = 3000; //milliseconds, this timer will control how long the toy is shown in the door?
const int ELAPSED_TIME2 = 1000; //milliseconds, 2nd timer will control how often US sensor 2 updates
const int TOY_SHOWN_ELAPSED_TIME2 = 3000;
const int ELAPSED_TIME3 = 1000;

unsigned long previousTime1 = 0;  // initialize to 0, separate timing variable in case ELAPSED_TIME1 not equal to ELAPSED_TIME2 in a future program modification
unsigned long previousTime2 = 0;  // initialize to 0, separate timing variable in case ELAPSED_TIME1 not equal to ELAPSED_TIME2
unsigned long previousTime3 = 0;  //initialize to 0, separate timing variable
unsigned long currentTime;        // store # elapsed milliseconds since the program began

Servo myServo;              //create a servo object

void setup() 
{
  Serial.begin(9600);

  myServo.attach(9);          //tell the servo object that its servo is plugged into pin number

  pinMode(trigPin1, OUTPUT);   //the trigger pin will output pulses of electricity
  pinMode(echoPin1, INPUT);    //the echo pin will measure the duration of pulses coming back from the distance sensor

  pinMode(trigPin2, OUTPUT);   //the trigger pin will output pulses of electricity
  pinMode(echoPin2, INPUT);    //the echo pin will measure the duration of pulses coming back from the distance sensor

  pinMode(LED1, OUTPUT); //set LED1 output pin
  pinMode(LED2, OUTPUT); //set LED2 output pin
  //pinMode(LEDSwitch, OUTPUT); //set ON/OFF LED output pin

  //set motor to start (hide) position
  myServo.write(90);
}

void loop() 
{
  currentTime = millis(); //millis() function returns # milliseconds that have elapsed since program began

  //if motion detected by either ultrasonic detector w/in x amount distance (?maybe 10-12in)
  if(currentTime - previousTime1 > ELAPSED_TIME1)
  {
    previousTime1 = currentTime;
    distance1 = getDistance(trigPin1, echoPin1);

    //REMOVE AFTER TESTING
    Serial.print("Distance 1: ");
    Serial.println(distance1);

    if(distance1 <= 6)
    {
      //quickly move to hide
      myServo.write(90);
      //swing myServo to door 1
      myServo.write(0);
      digitalWrite(LED1, HIGH); //turn ON LED1
    }
  }

  if(currentTime - previousTime2 > ELAPSED_TIME2)
  {
    previousTime2 = currentTime;
    distance2 = getDistance(trigPin2, echoPin2);

    //REMOVE AFTER TESTING
    Serial.print("Distance 2: ");
    Serial.println(distance2);

    if(distance2 <= 6)
    {
      //start at hide
      myServo.write(90);
      //move to door 2
      myServo.write(180);
      digitalWrite(LED2, HIGH); //turn ON LED2
    }
  }
  /*AT THE SAME TIME
  check to see if there is an object detected within 3in of the toy*/

  //if motion detected within 3in immediately hide FAST
  if(currentTime - previousTime3 > ELAPSED_TIME3)
  {
    previousTime3 = currentTime;
    distance3 = getDistance(trigPin1, echoPin1);
    distance4 = getDistance(trigPin2, echoPin2);

    if(distance3 <= 3 || distance4 <= 3)
    {
      //swing myServo to start position, hide!
      myServo.write(90);
      //turn off LED1 and LED2
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
    }
    else
    {
      return;
    }
  }
}


//Functions

//RETURNS THE DISTANCE MEASURED BY THE HC-SR04 DISTANCE SENSOR
float getDistance(int trigPin, int echoPin)
{
  float echoTime;                   //variable to store the time it takes for a ping to bounce off an object
  
  float calculatedDistance;         //variable to store the distance calculated from the echo time

  //send out an ultrasonic pulse that's 10ms long
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  echoTime = pulseIn(echoPin, HIGH);      //use the pulsein command to see how long it takes for the
                                          //pulse to bounce back to the sensor
  //REMOVE AFTER TESTING
  Serial.println(echoTime);

  calculatedDistance = echoTime / 148.0;  //calculate the distance of the object that reflected the pulse (half the bounce time multiplied by the speed of sound)

  return calculatedDistance;              //send back the distance that was calculated
}
