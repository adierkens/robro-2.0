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

#define BACK_LEFT_SENSOR_TRIG_PIN 28
#define BACK_LEFT_SENSOR_ECHO_PIN 29

/*
 * Define all the following distances
 */

#define FRONT_FOLLOWING_DISTANCE 10
#define LEFT_FOLLOWING_DISTANCE 10

#define FOLLOWING_TOLLERANCE 3

Ultrasonic front_ultrasonic(FRONT_SENSOR_TRIG_PIN,FRONT_SENSOR_ECHO_PIN);
Ultrasonic front_left_ultrasonic(FRONT_LEFT_SENSOR_TRIG_PIN, FRONT_LEFT_SENSOR_ECHO_PIN);
Ultrasonic back_left_ultrasonic(BACK_LEFT_SENSOR_TRIG_PIN, BACK_LEFT_SENSOR_ECHO_PIN);

Servo rightServo, leftServo; 

void stopServos() {
  rightServo.write(RIGHT_SERVO_STOP);
  leftServo.write(LEFT_SERVO_STOP);
}

void forward() {
  rightServo.write(180);
  leftServo.write(0);
}

void turnLeft() {
  rightServo.write(180);
  leftServo.write(180);
}

void turnRight() {
  leftServo.write(0);
  rightServo.write(0);
}

void slightRight() {
  rightServo.write(170);
  leftServo.write(0);
}

void slightLeft() {
  rightServo.write(180);
  leftServo.write(10);
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

  long backLeftMicrosec = back_left_ultrasonic.timing();
  float backLeftDistance = back_left_ultrasonic.CalcDistance(backLeftMicrosec, Ultrasonic::CM);

  Serial.print("Front: ");
  Serial.print(frontDistance);
  Serial.print(" Left Front: ");
  Serial.print(frontLeftDistance);
  Serial.print(" Left Back: ");
  Serial.println(backLeftDistance);

  if (frontDistance < FRONT_FOLLOWING_DISTANCE) {
    // We hit a wall
    // Turn right - and continue
    turnRight();
  } else {
    // Nothing in front of us
    // Check for alignment and outside corners
    
    if ( frontLeftDistance < LEFT_FOLLOWING_DISTANCE) {
      if (backLeftDistance < LEFT_FOLLOWING_DISTANCE) {
        
        bool frontWithinTollerance = frontLeftDistance > (LEFT_FOLLOWING_DISTANCE + FOLLOWING_TOLLERANCE);
        bool backWithinTollerance = backLeftDistance > (LEFT_FOLLOWING_DISTANCE + FOLLOWING_TOLLERANCE);

        if (!frontWithinTollerance || !backWithinTollerance) {
          // One of the sensors is closer to the wall than we'd like

          if (frontWithinTollerance) {
            // The back is closer to the wall than we'd like
            // but the front is fine. We're turning away from it anyway so just keep going straight
            forward();
          } else if (backWithinTollerance) {
            // The front is closer to the wall than we'd like
            // but the back is fine. Turn slightly right so we don't hit it.
            slightRight();
          } else {
            // Both are too close to the wall.
            slightRight();
          }

        } else {
          // Both are within the following distance
          // Keep following the wall
          forward();
        }
      } else {
        // Front Left is within following distance
        // Back left isn't
        // We're skewed heading towards the wall, just keep going forward 
          forward();
      }
    
    } else {
      // Front Left is too far from the wall
      if (backLeftDistance < LEFT_FOLLOWING_DISTANCE) {
        // We hit an outside corner
        turnLeft();
      } else {
        // Both front and back left are too far from the wall
        forward();
      }
    }
  }
  
  delay(100); 
}
