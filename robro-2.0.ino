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

#define FOLLOWING_DISTANCE 20
#define FOLLOWING_TOLLERANCE 5

// #define SENSOR_RIGHT_SIDE

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
  rightServo.write(0);
  leftServo.write(180);
}

void forward() {
  Serial.println("Moving forward");
  rightServo.write(180);
  leftServo.write(0);
}

void turnRight() {
  Serial.println("Turning right");
  rightServo.write(180);
  leftServo.write(180);
}

void turnLeft() {
  Serial.println("Turning left");
  leftServo.write(0);
  rightServo.write(0);
}

void turnSlightLeft(float distance) {
  Serial.println("Turning slightly left");
  float newd = distance - FOLLOWING_DISTANCE;
  if (distance - FOLLOWING_DISTANCE > FOLLOWING_DISTANCE){
    newd = FOLLOWING_DISTANCE;
  }
  long right = RIGHT_SERVO_STOP + 5 + 2 * newd;
  long left = LEFT_SERVO_STOP - 5 - 5 * newd;
  rightServo.write(right);
  leftServo.write(left);
}

void turnSlightRight(float distance) {
  Serial.println("Turning slightly right");
  float newd = FOLLOWING_DISTANCE - distance;

  float right = RIGHT_SERVO_STOP + 5 + 5 * newd;
  float left = LEFT_SERVO_STOP - 5 - 2 * newd;
  rightServo.write(right);
  leftServo.write(left);
}

void setup() {
  Serial.begin(9600);
  rightServo.attach(RIGHT_SERVO_PIN);
  leftServo.attach(LEFT_SERVO_PIN);
   
  pinMode(PWR_PIN_0, OUTPUT);
  digitalWrite(PWR_PIN_0, HIGH);
  
  pinMode(GND_PIN_0, OUTPUT);
  digitalWrite(GND_PIN_0, LOW);
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
  float frontSideDistance = front_side_ultrasonic.CalcDistance(frontSideMicroSec, Ultrasonic::CM);
  delay(5);
  long backSideMicroSec = back_side_ultrasonic.timing();
  float backSideDistance = back_side_ultrasonic.CalcDistance(backSideMicroSec, Ultrasonic::CM);

  long frontMicroSec = front_ultrasonic.timing();
  float frontDistance = front_ultrasonic.CalcDistance(frontMicroSec, Ultrasonic::CM);

  printDistance(frontDistance, frontSideDistance, backSideDistance);

  float avgDistance = (frontSideDistance + backSideDistance) / 2;


  if (frontDistance < FOLLOWING_DISTANCE) {
    turnRight();
  } else {
    if (frontSideDistance < FOLLOWING_DISTANCE) {
      bool frontSideWithinTollerance = frontSideDistance > (FOLLOWING_DISTANCE + FOLLOWING_TOLLERANCE);

      if (backSideDistance < FOLLOWING_DISTANCE) {

        
        bool backSideWithinTollerance = backSideDistance > (FOLLOWING_DISTANCE + FOLLOWING_TOLLERANCE);

        if (!frontSideWithinTollerance || !backSideWithinTollerance) {

          if (frontSideWithinTollerance) {
            forward();
          } else if (backSideWithinTollerance) {
            turnSlightRight(avgDistance);
          } else {
            turnSlightRight(avgDistance);
          }
          
        } else {
          forward();
        }
      } else {
        if (!frontSideWithinTollerance) {
          turnSlightRight(avgDistance);
        } else {
          forward();
        }
      }
    } else {
      if (backSideDistance < FOLLOWING_DISTANCE) {
        turnLeft();
      } else {
        forward();
      }
    }
  }  
}
