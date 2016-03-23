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

#define BLACK_TAPE_MIN 750
#define BLACK_TAPE_MAX 1000

#define GRAY_TAPE_MIN 0
#define GRAY_TAPE_MAX 250

Servo rightServo;
Servo leftServo;
QTRSensorsRC qtrc((unsigned char[]) { 30, 31, 32, 33, 34, 35, 36, 37}, NUM_SENSORS, TIMEOUT, QTR_NO_EMITTER_PIN); 
unsigned int sensorValues[NUM_SENSORS];

void stopServos() {
  Serial.println("Stopping");
  rightServo.write(RIGHT_SERVO_STOP);
  leftServo.write(LEFT_SERVO_STOP);
}

void backwards() {
  Serial.println("Moving backwards");
  rightServo.write(180);
  leftServo.write(0);
}

void forward() {
  Serial.println("Moving forwards");
  rightServo.write(0);
  leftServo.write(180);
}

void setup() {
  Serial.begin(9600);
  
  rightServo.attach(RIGHT_SERVO_PIN);
  leftServo.attach(LEFT_SERVO_PIN);

  delay(1000);
  for (int i = 0; i < 250; i++) {
    qtrc.calibrate();
    delay(20);
  }
}

void readSensorValues() {
  qtrc.readCalibrated(sensorValues);
}



void loop() {
  readSensorValues();

  for (unsigned char i = 0; i < NUM_SENSORS; i++) {
    Serial.print(sensorValues[i]);
    Serial.print('\t');
  }
  Serial.println();
  
  delay(250);
}
