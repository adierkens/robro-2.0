#include <Ultrasonic.h>
#include <Servo.h>

#define RIGHT_SERVO_PIN 10
#define LEFT_SERVO_PIN 9
#define RIGHT_SERVO_STOP 95
#define LEFT_SERVO_STOP 93

#define FRONT_SENSOR_TRIG_PIN 42
#define FRONT_SENSOR_ECHO_PIN 43

#define FRONT_LEFT_SENSOR_TRIG_PIN 36
#define FRONT_LEFT_SENSOR_ECHO_PIN 37

#define PWR_PIN_0 22 
#define GND_PIN_0 52

#define BACK_LEFT_SENSOR_TRIG_PIN 28
#define BACK_LEFT_SENSOR_ECHO_PIN 29

#define FOLLOWING_DISTANCE 15
#define SIDE_DISTANCE 5
#define FOLLOWING_TOLLERANCE 5

struct distance {
  long frontSide;
  long backSide;
  long front;
};

Ultrasonic front_ultrasonic(FRONT_SENSOR_TRIG_PIN,FRONT_SENSOR_ECHO_PIN);
Ultrasonic front_side_ultrasonic(FRONT_LEFT_SENSOR_TRIG_PIN, FRONT_LEFT_SENSOR_ECHO_PIN);
Ultrasonic back_side_ultrasonic(BACK_LEFT_SENSOR_TRIG_PIN, BACK_LEFT_SENSOR_ECHO_PIN);

Servo rightServo, leftServo; 

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
  Serial.println("Moving forward");
  rightServo.write(0);
  leftServo.write(180);
}

void backRight() {
  rightServo.write(180);
  leftServo.write(LEFT_SERVO_STOP);
  delay(400);
}

void turnSlightLeft() {
  Serial.println("Turning slightly left");
  rightServo.write(100);
  leftServo.write(60);
}

void turnSlightRight() {
  Serial.println("Turning slightly right");
  rightServo.write(120);
  leftServo.write(88);
}

void wallFollow(float frontSideDistance) {
  float distance = frontSideDistance - SIDE_DISTANCE;
  float right = RIGHT_SERVO_STOP - 15;
  float left = LEFT_SERVO_STOP + 15;
  if (distance > 0.5) {
    left -= 9;
  }
  else if (distance < -0.5) {
    right += 9;
  }
//  right -= distance * 18;
//  left -= distance * 18;
  
  rightServo.write(right);
  leftServo.write(left);

  Serial.println(distance);
  Serial.println(right);
  Serial.println(left);
  delay(500);
}

void setup() {
  Serial.begin(9600);
  rightServo.attach(RIGHT_SERVO_PIN);
  leftServo.attach(LEFT_SERVO_PIN);
   
  pinMode(PWR_PIN_0, OUTPUT);
  digitalWrite(PWR_PIN_0, HIGH);
  
  pinMode(GND_PIN_0, OUTPUT);
  digitalWrite(GND_PIN_0, LOW);

  rightServo.write(RIGHT_SERVO_STOP + 20);
  leftServo.write(LEFT_SERVO_STOP - 19);
}

void printDistance(float front, float frontSide, float backSide) {
  Serial.print("Front: ");
  Serial.print(front);
  Serial.print(" FrontSide: ");
  Serial.print(frontSide);
  Serial.print(" BackSide: ");
  Serial.println(backSide);
}

void loop() {

  long frontSideMicroSec = front_side_ultrasonic.timing();
  float frontSideDistance = front_side_ultrasonic.CalcDistance(frontSideMicroSec, Ultrasonic::IN);
  
  long backSideMicroSec = back_side_ultrasonic.timing();
  float backSideDistance = back_side_ultrasonic.CalcDistance(backSideMicroSec, Ultrasonic::IN);

  long frontMicroSec = front_ultrasonic.timing();
  float frontDistance = front_ultrasonic.CalcDistance(frontMicroSec, Ultrasonic::IN);

  printDistance(frontDistance, frontSideDistance, backSideDistance);

  //CORNER IF STATEMENTS

  if(frontDistance < FOLLOWING_DISTANCE) {
    
  }

  //Follow wall
  wallFollow(frontSideDistance);
}
