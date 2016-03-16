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

/*
 * Anything further than FOLLOWING_DISTANCE + THRESHOLD is considered not near a wall
 */
#define FRONT_SENSOR_THRESHOLD 10
#define FRONT_LEFT_SENSOR_THRESHOLD 10
#define BACK_LEFT_SENSOR_THRESHOLD 10

#define FOLLOWING_TOLLERANCE 1

#define OUTSIDE_CORNER_THRESHOLD 10 // Difference between front and back sensors to determine an outside corner

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

void turnLeft() {
  rightServo.write(180);
  leftServo.write(180);
}

void turnRight() {
  leftServo.write(0);
  rightServo.write(0);
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
    
    if ( frontLeftDistance > (FRONT_LEFT_THRESHOLD + LEFT_FOLLOWING_DISTANCE) && backLeftDistance > (BACK_LEFT_THRESHOLD + LEFT_FOLLOWING_DISTANCE)) {
      // There's nothing to our left and nothing in front
      // We're in open space - keep going straight
      forward();
    } else if ( frontLeftDistance > (FRONT_LEFT_THRESHOLD + LEFT_FOLLOWING_DISTANCE) && backLeftDistance < (BACK_LEFT_THRESHOLD + LEFT_FOLLOWING_DISTANCE)) {
      // The front sensor doesn't sense a wall but the back one does

      if ( backLeftDistance < LEFT_FOLLOWING_DISTANCE && (frontLeftDistance - backLeftDistance) > OUTSIDE_CORNER_THRESHOLD ) {
        // The back sensor is close enough to our following distance
        // This is probably an inside corner
        turnLeft();
      }
    } else if ( frontLeftDistance < LEFT_FOLLOWING_DISTANCE && backLeftDistance < LEFT_FOLLOWING_DISTANCE) {
      // Follow the wall
      forward();
    }
  }
  
  delay(100); 
}
