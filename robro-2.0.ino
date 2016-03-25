#include <QTRSensors.h>
#include <Servo.h>

#define RIGHT_SERVO_PIN 10
#define LEFT_SERVO_PIN 9

#define RIGHT_SERVO_STOP 95
#define LEFT_SERVO_STOP 93

#define FRONT_IR_PIN 9
#define LEFT_IR_PIN 10
#define RIGHT_IR_PIN 8

#define NUM_SENSORS   8     // number of sensors used
#define TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN   44    // emitter is controlled by digital pin 2
#define SENSOR_START_PIN 30

#define THRESH 150 // Any calibrated value > THRESH is on gray or black

#define M_START 4
#define M_RIGHT RIGHT_SERVO_STOP - M_START
#define M_LEFT  LEFT_SERVO_STOP + M_START

#define KP 0.0025 * (M_START - 3)
#define KD 0.01

const int calibratedMax[NUM_SENSORS] = {2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500};
const int calibratedMin[NUM_SENSORS] = { 348,  348, 296, 248, 252, 300, 300, 456 };

int lastError = 0;
Servo rightServo;
Servo leftServo;
QTRSensorsRC qtrc((unsigned char[]) { 30, 31, 32, 33, 34, 35, 36, 37}, NUM_SENSORS, TIMEOUT, EMITTER_PIN); 
unsigned int sensorValues[NUM_SENSORS];
unsigned int lastSensorValues[NUM_SENSORS];
bool stop = false;

unsigned long lastTrippedRight = 0;
unsigned long lastTrippedLeft = 0;
unsigned long lastTrippedFront = 0;

void setup() {
  Serial.begin(9600);
  rightServo.attach(RIGHT_SERVO_PIN);
  leftServo.attach(LEFT_SERVO_PIN);

  pinMode(40, OUTPUT);
  digitalWrite(40, HIGH);
  pinMode(38, OUTPUT);
  digitalWrite(38, LOW);

  pinMode(41, OUTPUT);
  digitalWrite(41, HIGH);
  pinMode(39, OUTPUT);
  digitalWrite(39, LOW);

  pinMode(53, OUTPUT);
  digitalWrite(53, HIGH);
  pinMode(52, OUTPUT);
  digitalWrite(52, LOW);

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
  for (int i=0; i<NUM_SENSORS; i++) {
    lastSensorValues[i] = sensorValues[i];
  }
  int position = qtrc.readLine(sensorValues);
  //printSensorValues();

  if (rightTripped()) {
    lastTrippedRight = millis();
  }

  if (leftTripped()) {
    lastTrippedLeft = millis();
  }

  if (frontTripped()) {
    lastTrippedFront = millis();
  }
  
  return position;
}

bool allOverLine() {
  for (int i=0; i<NUM_SENSORS; i++){ 
    if (sensorValues[i] < THRESH) {
      return false;
    }
  }
  return true;
}

bool noneOverLine() {
  for (int i=0; i<NUM_SENSORS; i++) {
    if (sensorValues[i] > THRESH) {
      return false;
    }
  }
  return true;
}

bool frontTripped() {
  float front = analogRead(FRONT_IR_PIN);

  if (front > THRESH) {
    for (int i=0; i<5; i++) {
      front = analogRead(FRONT_IR_PIN);
      if (front < THRESH) {
        return false;
      }
    }
  }
  
  return front > THRESH;
}

bool rightTripped() {
  float right = analogRead(RIGHT_IR_PIN);
  return right > THRESH;
}

bool leftTripped() {
  float left = analogRead(LEFT_IR_PIN);
  return left > THRESH;
}

void loop() {
  int position = readSensorValues();  
  int error = position - 3500;
  int motorSpeed = KP * error + KD * (error - lastError);
  lastError = error;

  int rightSpeed = min(180, max(0, M_RIGHT - motorSpeed));
  int leftSpeed = min(180, max(0, M_LEFT - motorSpeed));

//  if (rightTripped() && leftTripped()) {
//
//    rightServo.write(0);
//    leftServo.write(180);
//    delay(5);
//    
//    if (frontTripped()) {
//        rightSpeed = 0;
//        leftSpeed = 180;
//    } else {
//
//      rightServo.write(180);
//      leftServo.write(180);
//
//      while (!frontTripped()) {
//        delay(1);
//      }
//      
//      return;
//    }
//  } else if (!rightTripped() && !frontTripped()) {
//
//       rightServo.write(0);
//      leftServo.write(0);
//      while (!frontTripped()) {
//        delay(1);
//      }
//      return;
//  } else if (!leftTripped() && !frontTripped()) {
//
//
//       rightServo.write(180);
//      leftServo.write(180);
//      while (!frontTripped()) {
//        delay(1);
//      }
//      return;
//  }
//
//  if (noneOverLine()) {
//    Serial.println("None over the line");
//    rightServo.write(180);
//    leftServo.write(0);
//    delay(50);
//
//     rightServo.write(180);
//     leftServo.write(180);
//     delay(40);
//
//     rightServo.write(0);
//     delay(40);
//    
//    return;
//  }
//  
  rightServo.write(rightSpeed);
  leftServo.write(leftSpeed);
  delay(3);
  
}
