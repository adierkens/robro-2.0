#include <QTRSensors.h>
#include <Servo.h>

#define RIGHT_SERVO_PIN 10
#define LEFT_SERVO_PIN 9

#define RIGHT_SERVO_STOP 95
#define LEFT_SERVO_STOP 93

#define NUM_SENSORS   8     // number of sensors used
#define TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN   44    // emitter is controlled by digital pin 2
#define SENSOR_START_PIN 30

#define M_START 4
#define M_RIGHT RIGHT_SERVO_STOP - M_START
#define M_LEFT  LEFT_SERVO_STOP + M_START

#define KP 0.01 * (M_START - 1)
#define KD 0.01

const int calibratedMax[NUM_SENSORS] = {2500, 2500, 2500, 2500, 2500, 2500, 2500, 2500};
const int calibratedMin[NUM_SENSORS] = { 348,  348, 296, 248, 252, 300, 300, 456 };
int lastError = 0;
Servo rightServo;
Servo leftServo;
QTRSensorsRC qtrc((unsigned char[]) { 30, 31, 32, 33, 34, 35, 36, 37}, NUM_SENSORS, TIMEOUT, EMITTER_PIN); 
unsigned int sensorValues[NUM_SENSORS];

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

void loop() {
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
