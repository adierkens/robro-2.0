#include <Ultrasonic.h>
#include <Servo.h>

/*
 * Define wall follow side
 */

#define LEFT_SETUP true //****** True = Sensors on left side/setup for left wall follow. ||| False = Sensors on right side/setup for right wall follow.

/*
 * Define sensor/servo pins
 */
 
#define RIGHT_SERVO_PIN 9
#define LEFT_SERVO_PIN 10
#define RIGHT_SERVO_STOP 93
#define LEFT_SERVO_STOP 95

#define FRONT_SENSOR_TRIG_PIN 42
#define FRONT_SENSOR_ECHO_PIN 43

#define FRONT_SIDE_SENSOR_TRIG_PIN 51
#define FRONT_SIDE_SENSOR_ECHO_PIN 49
#define FRONT_SIDE_SENSOR_POWER_PIN 53
#define FRONT_SIDE_SENSOR_GND_PIN 47

//#define BACK_SIDE_SENSOR_TRIG_PIN 28
//#define BACK_SIDE_SENSOR_ECHO_PIN 29

/*
 * Define all the following distances
 */

#define FRONT_FOLLOWING_DISTANCE 10
#define SIDE_FOLLOWING_DISTANCE 10

#define FOLLOWING_TOLLERANCE 2

/*
 * Setup sensors and servos
 */

Ultrasonic front_ultrasonic(FRONT_SENSOR_TRIG_PIN,FRONT_SENSOR_ECHO_PIN);
Ultrasonic front_side_ultrasonic(FRONT_SIDE_SENSOR_TRIG_PIN, FRONT_SIDE_SENSOR_ECHO_PIN);
//Ultrasonic back_side_ultrasonic(BACK_SIDE_SENSOR_TRIG_PIN, BACK_SIDE_SENSOR_ECHO_PIN);

Servo rightServo, leftServo; 

/*
 * Helper functions
 */

void stopServos() {
  rightServo.write(RIGHT_SERVO_STOP);
  leftServo.write(LEFT_SERVO_STOP);
}

void forward() {
  rightServo.write(180);
  leftServo.write(0);
}

void spinLeft() {
  rightServo.write(180);
  leftServo.write(180);
}

void spinRight() {
  rightServo.write(0);
  leftServo.write(0);
}

void turnRight() {
  rightServo.write(180);
  leftServo.write(40);
}

void turnLeft() {
  rightServo.write(150);
  leftServo.write(0);
}

void backRight() {
  rightServo.write(0);
  leftServo.write(LEFT_SERVO_STOP);
}

void backLeft() {
  rightServo.write(RIGHT_SERVO_STOP);
  leftServo.write(180);
}

void slightRight() {
  rightServo.write(140);
  leftServo.write(0);
}

void slightLeft() {
  rightServo.write(180);
  leftServo.write(40);
}

/*
 * Setup function
 */

void setup() {
  Serial.begin(9600);//set Serial Baud rate
  Serial.println("Ultrasonic sensor starting!!!!!");
  rightServo.attach(RIGHT_SERVO_PIN);
  leftServo.attach(LEFT_SERVO_PIN);
  
  // Power and Gnd for front left sensor
  pinMode(FRONT_SIDE_SENSOR_POWER_PIN, OUTPUT);
  digitalWrite(FRONT_SIDE_SENSOR_POWER_PIN, HIGH);
  
  pinMode(FRONT_SIDE_SENSOR_GND_PIN, OUTPUT);
  digitalWrite(FRONT_SIDE_SENSOR_GND_PIN, LOW);
}

/*
 * Main code
 */

void loop() {
  long frontmicrosec = front_ultrasonic.timing();
  float frontDistance = front_ultrasonic.CalcDistance(frontmicrosec,Ultrasonic::CM);
  
  long frontSideMicrosec = front_side_ultrasonic.timing();
  float frontSideDistance = front_side_ultrasonic.CalcDistance(frontSideMicrosec, Ultrasonic::CM);

//  long backSideMicrosec = back_side_ultrasonic.timing();
//  float backSideDistance = back_side_ultrasonic.CalcDistance(backSideMicrosec, Ultrasonic::CM);

  Serial.print("Front: ");
  Serial.print(frontDistance);
  Serial.print(" Side Front: ");
  Serial.print(frontSideDistance);
//  Serial.print(" Side Back: ");
//  Serial.println(backSideDistance);

  if (LEFT_SETUP) {
    if (frontDistance < FRONT_FOLLOWING_DISTANCE) {
      // We hit a wall
      // Turn right - and continue
      backRight();
    } 
    else {
      // Nothing in front of us
      // Check for alignment and outside corners
      
      if (frontSideDistance < SIDE_FOLLOWING_DISTANCE - FOLLOWING_TOLLERANCE) {
        //We are too close to the wall, turn away.
        turnRight();
      }
      else if (frontSideDistance > SIDE_FOLLOWING_DISTANCE - FOLLOWING_TOLLERANCE) {
        //We are too far from the wall, turn towards it.
        turnLeft();
      }
      else {
        //All distances are within tollerances. Continue forward.
        forward();
      }
    }
  }
  else {
    if (frontDistance < FRONT_FOLLOWING_DISTANCE) {
      // Object (wall/door) ahead
      // Turn away - and continue
      backLeft();
    } 
    else {
      // Nothing in front of us
      // Check for alignment
      
      if (frontSideDistance < SIDE_FOLLOWING_DISTANCE - FOLLOWING_TOLLERANCE) {
        //We are too close to the wall, turn away.
        turnLeft();
      }
      else if (frontSideDistance > SIDE_FOLLOWING_DISTANCE + FOLLOWING_TOLLERANCE) {
        //We are too far from the wall, turn towards it.
        turnRight();
      }
      else {
        //All distances are within tollerances. Continue forward.
        forward();
      }
    }
  }
    
//      bool frontWithinTollerance = frontSideDistance > (SIDE_FOLLOWING_DISTANCE + FOLLOWING_TOLLERANCE);
//      
//      if (backSideDistance < SIDE_FOLLOWING_DISTANCE) {
//        bool backWithinTollerance = backSideDistance > (SIDE_FOLLOWING_DISTANCE + FOLLOWING_TOLLERANCE);
//
//        if (!frontWithinTollerance || !backWithinTollerance) {
//          // One of the sensors is closer to the wall than we'd like
//
//          if (frontWithinTollerance) {
//            // The back is closer to the wall than we'd like
//            // but the front is fine. We're turning away from it anyway so just keep going straight
//            forward();
//          }// else if (backWithinTollerance) {
//            // The front is closer to the wall than we'd like
//            // but the back is fine. Turn slightly right so we don't hit it.
////            slightRight();
//          } else {
//            // Both are too close to the wall.
//            slightRight();
//          }
//
//        } else {
//          // Both are within the following distance
//          // Keep following the wall
//          forward();
//        }
//      } else {
//        // Front Left is within following distance
//        // Back left isn't
//        // We're skewed heading towards the wall
//
//          if (!frontWithinTollerance ) {
//            // Front is too close to the wall
//            slightRight();
//          } else { 
//            forward();
//          }
//      }
//    
//    } else {
//      // Front side is too far from the wall
//      if (backSideDistance < SIDE_FOLLOWING_DISTANCE) {
//        // We hit an outside corner
//        turnLeft();
//      } else {
//        // Both front and back side are too far from the wall
//        forward();
//      }
//    }
//  }
  
  delay(100); 
}
