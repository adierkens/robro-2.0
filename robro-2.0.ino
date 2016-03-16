#include <Ultrasonic.h>
#include <Servo.h>

#define RIGHT_SERVO_PIN 9
#define LEFT_SERVO_PIN 10
#define RIGHT_SERVO_STOP 93
#define LEFT_SERVO_STOP 95

#define FRONT_SENSOR_TRIG_PIN 42
#define FRONT_SENSOR_ECHO_PIN 43

#define FRONT_LEFT_SENSOR_TRIG_PIN 36
#define FRONT_LEFT_SENSOR_ECHO_PIN 37

#define PWR_PIN_0 22 
#define GND_PIN_0 52

#define BACK_LEFT_SENSOR_TRIG_PIN 28
#define BACK_LEFT_SENSOR_ECHO_PIN 29

#define FRONT_FOLLOWING_DISTANCE 15
#define LEFT_FOLLOWING_DISTANCE 15

#define FOLLOWING_TOLLERANCE 7

Ultrasonic front_ultrasonic(FRONT_SENSOR_TRIG_PIN,FRONT_SENSOR_ECHO_PIN);
Ultrasonic front_left_ultrasonic(FRONT_LEFT_SENSOR_TRIG_PIN, FRONT_LEFT_SENSOR_ECHO_PIN);
Ultrasonic back_left_ultrasonic(BACK_LEFT_SENSOR_TRIG_PIN, BACK_LEFT_SENSOR_ECHO_PIN);

Servo rightServo, leftServo; 

void stopServos() {
  Serial.println("Stopping");
  rightServo.write(RIGHT_SERVO_STOP);
  leftServo.write(LEFT_SERVO_STOP);
}

void forward() {
  Serial.println("Forward");
  rightServo.write(180);
  leftServo.write(0);
}

void turnRight() {
  Serial.println("Right turn");
  rightServo.write(180);
  leftServo.write(180);
}

void turnLeft() {
  Serial.println("Left turn");
  leftServo.write(0);
  rightServo.write(0);
}

void slightLeft() {
  Serial.println("Slight left");
  rightServo.write(150);
  leftServo.write(0);
}

void slightRight() {
  Serial.println("Slight right");
  rightServo.write(180);
  leftServo.write(30);
}

void setup() {
  Serial.begin(9600);//set Serial Baud rate
  Serial.println("Ultrasonic sensor starting!!!!!");
  rightServo.attach(RIGHT_SERVO_PIN);
  leftServo.attach(LEFT_SERVO_PIN);
  
  // Power and Gnd for front left sensor
  pinMode(PWR_PIN_0, OUTPUT);
  digitalWrite(PWR_PIN_0, HIGH);
  
  pinMode(GND_PIN_0, OUTPUT);
  digitalWrite(GND_PIN_0, LOW);
}

void loop() {
  long frontmicrosec = front_ultrasonic.timing();
  float frontDistance = front_ultrasonic.CalcDistance(frontmicrosec,Ultrasonic::CM);
  
  long frontLeftMicrosec = front_left_ultrasonic.timing();
  float frontLeftDistance = front_left_ultrasonic.CalcDistance(frontLeftMicrosec, Ultrasonic::CM);
  delay(10);
  long backLeftMicrosec = back_left_ultrasonic.timing();
  float backLeftDistance = back_left_ultrasonic.CalcDistance(backLeftMicrosec, Ultrasonic::CM);

  Serial.print("Front: ");
  Serial.print(frontDistance);
  Serial.print(" Left Front: ");
  Serial.print(frontLeftDistance);
  Serial.print(" Left Back: ");
  Serial.println(backLeftDistance);

  if (frontLeftDistance < LEFT_FOLLOWING_DISTANCE) {
    slightRight();
  } else {
    slightLeft();
  }
  
  delay(10); 
}
