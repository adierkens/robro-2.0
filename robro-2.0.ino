#include <QTRSensors.h>
#include <Servo.h>
#include <Ultrasonic.h>

#define RIGHT_SERVO_PIN 10
#define LEFT_SERVO_PIN 9

#define RIGHT_SERVO_STOP 90
#define LEFT_SERVO_STOP 90

#define BALL_SENSOR_THRESH 700

#define SENSOR_TRIG_PIN 44
#define SENSOR_ECHO_PIN 45
#define TRIGGER_PIN 53

#define NUM_SENSORS   8     // number of sensors used
#define TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN   32    // emitter is controlled by digital pin 2
#define SENSOR_START_PIN 30

#define M_START 4
#define M_RIGHT RIGHT_SERVO_STOP - M_START
#define M_LEFT  LEFT_SERVO_STOP + M_START

#define KP 0.007
#define KD 0.03

#define SENSOR_THRESHOLD 300
#define DIST_THRESHOLD 30

#define BALL_TRIGGER_PIN 48

const int calibratedMax[NUM_SENSORS] = {2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500};
const int calibratedMin[NUM_SENSORS] = { 348,  348, 296, 248, 252, 300, 300, 456 };

int lastError = 0;
Ultrasonic ultrasonic(SENSOR_TRIG_PIN,SENSOR_ECHO_PIN);
Servo rightServo;
Servo leftServo;
QTRSensorsRC qtrc((unsigned char[]) { 22, 23, 24, 25, 26, 27, 28, 29}, NUM_SENSORS, TIMEOUT, EMITTER_PIN); 
unsigned int sensorValues[NUM_SENSORS];
boolean ballFound = false;

void printSensorValues() {
  for (unsigned char i = 0; i < NUM_SENSORS; i++) {
    Serial.print(sensorValues[i]);
    Serial.print('\t');
  }
  Serial.println();
}

int readSensorValues() {
  int position = qtrc.readLine(sensorValues);
  return position;
}

void forward(){
  rightServo.write(0);
  leftServo.write(180);
}

void backwards(){
  rightServo.write(180);
  leftServo.write(0);
}

void turnRight(){
  rightServo.write(180);
  leftServo.write(180);
}

void turnLeft(){
  rightServo.write(0);
  leftServo.write(0);
}

void trackLine() {
  int position = readSensorValues();  
  int error = position - (1000 * (NUM_SENSORS - 1))/2;
  int motorSpeed = KP * error + KD * (error - lastError);
  lastError = error;

  int rightSpeed = min(180, max(0, M_RIGHT - motorSpeed));
  int leftSpeed = min(180, max(0, M_LEFT - motorSpeed));

  rightServo.write(rightSpeed);
  leftServo.write(leftSpeed);
  delay(2);
}

bool hitLine() {
  readSensorValues();
  for (int i=0; i < NUM_SENSORS; i++) {
    if (sensorValues[i] > SENSOR_THRESHOLD) {
      return true;
    }
  }
  return false;
}

bool allOnLine() {
  readSensorValues();
  for (int i=0; i < NUM_SENSORS; i++) {
    if (sensorValues[i] <= SENSOR_THRESHOLD) {
      return false;
    }
  }
  return true;
}

bool senseBall() {
  for (int i=0; i<2; i++) {
      int dist = calcDistance();
      if (dist > DIST_THRESHOLD) {
        return false;
      }
  }

  return true;

}

void right90() {
  turnRight();
  delay(600);
}

void left90() {
  turnLeft();
  delay(100);
}

bool readyToFire() {
  float analogOut = analogRead(1);
  Serial.println(analogOut);
  return analogOut < BALL_SENSOR_THRESH;
}

void falconKick() {
  forward();
  delay(10);
  
  do {
    delay(2);
    
  } while(!readyToFire());
  
  digitalWrite(BALL_TRIGGER_PIN, HIGH);
  delay(100);
  digitalWrite(BALL_TRIGGER_PIN, LOW);

  while(!hitLine()) {
    delay(1);
  }
  
}

long calcDistance(){
  int calcD;
  long frontMicroSec = ultrasonic.timing();
  calcD = ultrasonic.CalcDistance(frontMicroSec, Ultrasonic::CM);
  return calcD;
}



void loop() {  
  rightServo.write(90);
  leftServo.write(90);
}

void setup() {
  Serial.begin(9600);
  pinMode(BALL_TRIGGER_PIN, OUTPUT);
  rightServo.attach(RIGHT_SERVO_PIN);
  leftServo.attach(LEFT_SERVO_PIN);

  qtrc.calibrate();

  for (int i=0; i<NUM_SENSORS; i++) {
    qtrc.calibratedMinimumOn[i] = calibratedMin[i];
    qtrc.calibratedMaximumOn[i] = calibratedMax[i];
  }

  pinMode(33, OUTPUT);
  pinMode(32, OUTPUT);
  digitalWrite(33, HIGH);
  digitalWrite(32, LOW);



  do {
    forward();
  } while (!hitLine());

  delay(40);

  turnRight();
  delay(700);

  do {
    trackLine();
  } while(!senseBall());


  backwards();
  delay(200);

  turnLeft();
  delay(850);

  falconKick();

}
