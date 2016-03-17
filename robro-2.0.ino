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

#define FOLLOWING_DISTANCE 20
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

void setup() {
  Serial.begin(9600);
  rightServo.attach(RIGHT_SERVO_PIN);
  leftServo.attach(LEFT_SERVO_PIN);
   
  pinMode(PWR_PIN_0, OUTPUT);
  digitalWrite(PWR_PIN_0, HIGH);
  
  pinMode(GND_PIN_0, OUTPUT);
  digitalWrite(GND_PIN_0, LOW);
}

void printDistance(distance d) {
  Serial.print("Front: ");
  Serial.print(d.front);
  Serial.print(" FrontSide: ");
  Serial.print(d.frontSide);
  Serial.print(" BackSide: ");
  Serial.println(d.backSide);
}

distance calcDistance() {

  distance calcD;
  
  long frontSideMicroSec = front_side_ultrasonic.timing();
  calcD.frontSide = front_side_ultrasonic.CalcDistance(frontSideMicroSec, Ultrasonic::CM);
  delay(2);
  long backSideMicroSec = back_side_ultrasonic.timing();
  calcD.backSide = back_side_ultrasonic.CalcDistance(backSideMicroSec, Ultrasonic::CM);
  long frontMicroSec = front_ultrasonic.timing();
  calcD.front = front_ultrasonic.CalcDistance(frontMicroSec, Ultrasonic::CM);
  
  printDistance(calcD);
  return calcD;  
}

bool ran = false;

void handleOutsideCorner(distance d) {
  Serial.println("Hit outside corner");

  // Initiate the left turn
  float right = 0;
  float left = LEFT_SERVO_STOP + 2;
  
  leftServo.write(left);
  rightServo.write(right);  
  delay(2500);

  float diff = d.frontSide - d.backSide;
  
  while ( diff > FOLLOWING_TOLLERANCE || diff < -FOLLOWING_TOLLERANCE) {
    left += 2;

    leftServo.write(left);
    rightServo.write(right);

    d = calcDistance();
    delay(5);
  }
  Serial.println("Done with corner");
  forward();
  delay(50);
  
  stopServos();
  ran = true;
}

void loop() {

  distance d = calcDistance();

  if (ran) {
    return;
  }

  // Follow wall
  if ((d.frontSide - d.backSide) > (2 * FOLLOWING_DISTANCE)) {
    handleOutsideCorner(d);
  } else {
    forward();
  }
}
