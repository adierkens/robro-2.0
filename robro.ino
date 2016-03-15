#include <Ultrasonic.h>
#include <Servo.h>

#define RIGHT_SERVO_PIN 9
#define LEFT_SERVO_PIN 10
#define RIGHT_SERVO_STOP 93
#define LEFT_SERVO_STOP 95
#define FRONT_SENSOR_TRIG_PIN 42
#define FRONT_SENSOR_ECHO_PIN 43
#define FRONT_LEFT_SENSOR_TRIG_PIN 51
#define FRONT_LEFT_SENSOR_ECHO_PIN 49
#define FRONT_LEFT_SENSOR_POWER_PIN 53
#define FRONT_LEFT_SENSOR_GND_PIN 47
//
#define BACK_LEFT_SENSOR_TRIG_PIN 28
#define BACK_LEFT_SENSOR_ECHO_PIN 29

Ultrasonic front_ultrasonic(FRONT_SENSOR_TRIG_PIN,FRONT_SENSOR_ECHO_PIN);
Ultrasonic front_left_ultrasonic(FRONT_LEFT_SENSOR_TRIG_PIN, FRONT_LEFT_SENSOR_ECHO_PIN);
Ultrasonic back_left_ultrasonic(BACK_LEFT_SENSOR_TRIG_PIN, BACK_LEFT_SENSOR_ECHO_PIN);

Servo rightServo, leftServo; 

void stopServos() {
  rightServo.write(RIGHT_SERVO_STOP);
  leftServo.write(LEFT_SERVO_STOP);
}

void forward() {
  rightServo.write(178);
  leftServo.write(0);
}

void setup() {
  Serial.begin(9600);//set Serial Baud rate
  Serial.println("Ultrasonic sensor starting!!!!!");
  rightServo.attach(RIGHT_SERVO_PIN);
  leftServo.attach(LEFT_SERVO_PIN);
  
  // Power and Gnd for front left sensor
  pinMode(FRONT_LEFT_SENSOR_POWER_PIN, OUTPUT);
  digitalWrite(FRONT_LEFT_SENSOR_POWER_PIN, HIGH);
  
  pinMode(FRONT_LEFT_SENSOR_GND_PIN, OUTPUT);
  digitalWrite(FRONT_LEFT_SENSOR_GND_PIN, LOW);
}

void loop() {
  long frontmicrosec = front_ultrasonic.timing();
  float frontDistance = front_ultrasonic.CalcDistance(frontmicrosec,Ultrasonic::CM);
  
  long frontLeftMicrosec = front_left_ultrasonic.timing();
  float frontLeftDistance = front_left_ultrasonic.CalcDistance(frontLeftMicrosec, Ultrasonic::CM);
  
  if (frontDistance > 25) {
    forward();
  } else {
    stopServos();        
  }
  
  delay(100); 
}
