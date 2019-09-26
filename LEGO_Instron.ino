#include <AccelStepper.h>
#define HALFSTEP 8

// Motor pin definitions
#define motorPin1  8     // IN1 on the ULN2003 driver 1
#define motorPin2  9     // IN2 on the ULN2003 driver 1
#define motorPin3  10    // IN3 on the ULN2003 driver 1
#define motorPin4  11    // IN4 on the ULN2003 driver 1

// LED pin definitions
#define greenLED   2
#define yellowLED  3
#define redLED     4
#define barcodeLED 6

// Initialize with pin sequence IN1-IN3-IN2-IN4 for using the AccelStepper with 28BYJ-48
AccelStepper stepper(HALFSTEP, motorPin1, motorPin3, motorPin2, motorPin4);
byte robotStep = 0;
int robotPath[] = {800, 2000, 3150, 4100, 0};

unsigned long counter = 0;
const unsigned long k  = 30000;
const unsigned long k2 = k*2;
const unsigned long k3 = k*3;

void setup() 
{
  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(barcodeLED, OUTPUT);
  
  // put your setup code here, to run once:
  stepper.setMaxSpeed(1000.0);
  stepper.setAcceleration(200.0);
  stepper.setSpeed(500);
  stepper.moveTo(robotPath[robotStep]);
}

void loop() 
{
  // Switch LightStack LEDs
  counter++;  
  if (counter > 0 && counter < k) 
  {
    digitalWrite(greenLED, HIGH);
    digitalWrite(yellowLED, LOW);
    digitalWrite(redLED, LOW);
  }
  else if (counter > k && counter < k2) 
  {
    digitalWrite(greenLED, LOW);
    digitalWrite(yellowLED, HIGH);
    digitalWrite(redLED, LOW);
  }
  else if (counter > k2 && counter < k3) 
  {
    digitalWrite(greenLED, LOW);
    digitalWrite(yellowLED, LOW);
    digitalWrite(redLED, HIGH);
  }
  else if (counter > k3) counter = 0;

  digitalWrite(barcodeLED, LOW);

  if (stepper.distanceToGo() == 0)
  {
    delay(1000);
        
    // Increase and check robot step
    if (++robotStep >= sizeof(robotPath)/2) robotStep = 0;
    
    // Is this barcode reading station?
    if (robotStep == 2)
    {
      for (int i=0; i<3; i++)
      {
        digitalWrite(barcodeLED, HIGH);
        delay(250);
        digitalWrite(barcodeLED, LOW);
        delay(750);
      }
    }
    
    delay(1000);
    stepper.moveTo(robotPath[robotStep]);
  }
      
  stepper.run();
 }
