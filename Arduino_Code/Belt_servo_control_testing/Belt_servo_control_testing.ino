

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

// From a "beardy" on SFE webstie for continous servo:
//  https://www.sparkfun.com/products/9347#comment-4eaad849757b7fd351005174
//
//I’m sending ‘microsecond’ commands from an Arduino - rather than angular commands - as it offers higher resolution.
//1000uS is full speed forward
//1500uS is STOP
//2000uS is full speed reverse
//…anything in between gives a proportional speed. Very handy.
//

int control = 1500;    // stop
char input; // revice input from serial to control

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  myservo.writeMicroseconds(control); //initialized as 1500, Stop servo
  Serial.begin(9600);
}

void loop() {

  if(Serial.available() > 1)
  {
    input = Serial.read();
    switch (input) 
    {
      case '0':    // your hand is on the sensor
        Serial.println("stop");
        control = 1500;
        break;
      case '1':    // your hand is close to the sensor
        Serial.println("forward");
        control = 1400;
        break;
      case '2':    // your hand is a few inches from the sensor
        Serial.println("2");
        control = 1600;
        break;
    }
    myservo.writeMicroseconds(control);
    delay(100);
    control = 1500;
    myservo.writeMicroseconds(control);
  }

}

