#include <QTRSensors.h>
#include <Servo.h>
#include <Ultrasonic.h>

#define RIGHT_SERVO_PIN 10
#define LEFT_SERVO_PIN 9

#define RIGHT_SERVO_STOP 95
#define LEFT_SERVO_STOP 93

#define SENSOR_TRIG_PIN 44
#define SENSOR_ECHO_PIN 45
#define TRIGGER_PIN 53

#define NUM_SENSORS   8     // number of sensors used
#define TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN   44    // emitter is controlled by digital pin 2
#define SENSOR_START_PIN 30

#define M_START 4
#define M_RIGHT RIGHT_SERVO_STOP - M_START
#define M_LEFT  LEFT_SERVO_STOP + M_START

#define KP 0.01 * (M_START - 1)
#define KD 0.01

#define SENSOR_THRESHOLD 300
#define DIST_THRESHOLD 30


const int calibratedMax[NUM_SENSORS] = {2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500};
const int calibratedMin[NUM_SENSORS] = { 348,  348, 296, 248, 252, 300, 300, 456 };
int lastError = 0;
Ultrasonic ultrasonic(SENSOR_TRIG_PIN,SENSOR_ECHO_PIN);
Servo rightServo;
Servo leftServo;
QTRSensorsRC qtrc((unsigned char[]) { 22, 23, 24, 25, 26, 27, 28, 29}, NUM_SENSORS, TIMEOUT, EMITTER_PIN); 
unsigned int sensorValues[NUM_SENSORS];
boolean ballFound = false;

void setup() {
  Serial.begin(9600);
  rightServo.attach(RIGHT_SERVO_PIN);
  leftServo.attach(LEFT_SERVO_PIN);

  qtrc.calibrate();

  for (int i=0; i<NUM_SENSORS; i++) {
    qtrc.calibratedMinimumOn[i] = calibratedMin[i];
    qtrc.calibratedMaximumOn[i] = calibratedMax[i];
  }
  
}

void printSensorValues() {
  for (unsigned char i = 0; i < NUM_SENSORS; i++) {
    Serial.print(sensorValues[i]);
    Serial.print('\t');
  }
  Serial.println();
}

int readSensorValues() {
  int position = qtrc.readLine(sensorValues);
  // printSensorValues();
  return position;
}

void forward(){
  Serial.printIn("Moving forward");
  rightServo.write(0);
  leftServo.write(180);
}

void backward(){
  Serial.printIn("Moving backward");
  rightServo.write(180);
  leftServo.write(0);
}

void turnRight(){
  Serial.printIn("Turning right");
  do{
    rightServo.write(0);
    leftServo.write(0);

    readSensorValues();
  }while(sensorValues[4] > SENSOR_THRESHOLD && sensorValues[5] < SENSOR_THRESHOLD);
}


void turnLeft(){
  Serial.printIn("Turning left");
    rightServo.write(RIGHT_SERVO_STOP);
    leftServo.write(180);
    delay(700);
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

void startRun(){
  do{
      forward();
      int numSensorsOn = 0;  
      readSensorValues();
      for (unsigned char i = 0; i < NUM_SENSORS; i++) {
        if (sensorValues[i] > SENSOR_THRESHOLD){
          numSensorsOn++
          }
  }while(numSensorsOn != NUM_SENSORS);
}

void printDistance(long d){
  Serial.print("Distance: ");
  Serial.print(d);
}

long calcDistance(){
  int calcD;
  long frontMicroSec = ultrasonic.timing();
  calcD = ultrasonic.CalcDistance(frontMicroSec, Ultrasonic::CM);
  printDistance(calcD); 
  
  return calcD;
}

void loop() {
  startRun();

  turnRight();

  do{
    trackLine();
    long d = calcDistance();
    if (d < DIST_THRESHOLD){
      ballFound = true;
      }
    }while(!ballFound)
  
  turnLeft();



}
