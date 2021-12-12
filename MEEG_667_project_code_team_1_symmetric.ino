#include <Servo.h>

// Definition of variables associated with motors based on board's schematic
int E1 = 5;     //M1 Torque Control (Left Motor)
int E2 = 6;     //M2 Torque Control (Right Motor)
int M1 = 4;     //M1 Direction Control (Left Motor)
int M2 = 7;     //M1 Direction Control (Right Motor)

// Initialization of useful variables
int RightSensor = 2;  // Variable associated with the line-tracking sensor placed on the left of the robot
int LeftSensor = 0;   // Variable associated with the line-tracking sensor placed on the right of the robot
int CenterSensor = 1; // Variable associated with the line-tracking sensor placed on the center of the robot

int sensor_left = 0;  // Variable to store the status of the line-tracking sensor placed on the left of the robot
int sensor_right = 0; // Variable to store the status of the line-tracking sensor placed on the right of the robot
int sensor_center = 0;// Variable to store the status of the line-tracking sensor placed on the center of the robot
int timer = 0;        // Variable to store time instants to avoid 360 turns

void stop(void)                   // Function that stops both motors
{
digitalWrite(E1,LOW); // Send zero torque to the left motor
digitalWrite(E2,LOW); // Send zero torque to the right motor
}
void back_off (char a,char b)     // Function that rotates motors backwards
{
analogWrite (E1,a);   // Apply user defined torque to the left motor
digitalWrite(M1,HIGH);// Set torque direction to HIGH for backwards rotation
analogWrite (E2,b);   // Apply user defined torque to the right motor
digitalWrite(M2,HIGH);// Set torque direction to HIGH for backwards rotation
}
void advance (char a,char b)     // Function that rotates motors forward
{
analogWrite (E1,a);   // Apply user defined torque to the left motor 
digitalWrite(M1,LOW); // Set torque direction to LOW for forward rotation
analogWrite (E2,b);   // Apply user defined torque to the right motor
digitalWrite(M2,LOW); // Set torque direction to LOW for forward rotation
}
void turn_R (char a,char b)     // Function that turns the robot in-place towards to the right
{
analogWrite (E1,a);   // Apply user defined torque to the left motor
digitalWrite(M1,LOW); // Set torque direction to LOW for forward rotation
analogWrite (E2,b);   // Apply user defined torque to the right motor
digitalWrite(M2,HIGH);// Set torque direction to HIGH for backwards rotation
}
void turn_L (char a,char b)     // Function that turns the robot in-place towards to the left
{
analogWrite (E1,a);   // Apply user defined torque to the left motor
digitalWrite(M1,HIGH);// Set torque direction to HIGH for backwards rotation
analogWrite (E2,b);   // Apply user defined torque to the right motor
digitalWrite(M2,LOW); // Set torque direction to LOW for forward rotation
}

void setup(void)
{
}

void loop()
{
//Wait for button push
  sensor_left = digitalRead(LeftSensor);                                  // Load the status of the line-tracking sensor on the left
  sensor_center = digitalRead(CenterSensor);                              // Load the status of the line-tracking sensor on the center
  sensor_right = digitalRead(RightSensor);                                // Load the status of the line-tracking sensor on the right

  timer = 0;                                                              // Initialization of timer variable to avoid 360 turns
  
// sensor_something = 0 => detect the line
// sensor_something = 1 => not detecting the line

// There is a total of 8 different cases (2^3)
if (sensor_left == 1 && sensor_center == 1 && sensor_right == 1) {        // None of the three sensors detect any lines, keep moving forward
    advance (132,175);
} else if(sensor_left == 1 && sensor_center == 1 && sensor_right == 0) {  // Only the right sensor detects a line, turn right
    delay(20);                                                            // Small delay to keep moving forward before turning for allignment purposes
    stop ();                                                              // Stop the robot
    timer = millis();                                                     // Store the timing that the robot initiated rotation
    while (digitalRead(CenterSensor) == 1){                               // while not detecting center sensor
      advance (150,0);                                                    // Turn right while moving forward
      if (millis()>=timer+1800 || digitalRead(LeftSensor) == 0){          // Check whether the left sensor detected the line or if enough time for a full rotation has passed by 
        if (millis()>=timer+1800){                                        // Check whether the robot completed approx. a full rotation
          back_off (150,0);                                               // Turn left while moving backward
          delay(1200);                                                    // Delay to execute backward motion for enough time in order to return to the initial position
          advance(132,175);                                               // Move staight forward (different torques between the motors were necessary to maintain straight path)
          delay(200);                                                     // Delay to execute straight motion for enought time
        }
        break;                                                            // Break from rotation loop
     }
    }
} else if(sensor_left == 1 && sensor_center == 0 && sensor_right == 1) {  // Only the center sensor detects a line, move forward
    advance (132,175);  
} else if(sensor_left == 1 && sensor_center == 0 && sensor_right == 0) {  // Both the center and right sensors detect a line, turn to the right
    advance (150,0); 
} else if(sensor_left == 0 && sensor_center == 1 && sensor_right == 1) {  // Only the left sensor detects a line, turn left
    delay(20);                                                            // Small delay to keep moving forward before turning for allignment purposes
    stop ();                                                              // Stop the robot
    timer = millis();                                                     // Store the timing that the robot initiated rotation
    while (digitalRead(CenterSensor) == 1){                               // while not detecting center sensor
      advance (0,150);                                                    // Turn left while moving forward
      if (millis()>=timer+1800 || digitalRead(RightSensor) == 0){         // Check whether the right sensor detected the line or if enough time for a full rotation has passed by 
        if (millis()>=timer+1800){                                        // Check whether the robot completed approx. a full rotation
          back_off (0,150);                                               // Turn right while moving backward
          delay(1200);                                                    // Delay to execute backward motion for enough time in order to return to the initial position
          advance(132,175);                                               // Move staight forward (different torques between the motors were necessary to maintain straight path)
          delay(200);                                                     // Delay to execute straight motion for enought time
        }
        break;                                                            // Break from rotation loop
     }
  }
} else if(sensor_left == 0 && sensor_center == 1 && sensor_right == 0) {  // Both the left and right sensors detect a line, keep moving forward (it happens)
    advance (132,175);                                                    // Move staight forward (different torques between the motors were necessary to maintain straight path) 
} else if(sensor_left == 0 && sensor_center == 0 && sensor_right == 1) {  // Both the center and left sensors detect a line, turn to the left
    advance (0,150);                                                      // Turn left while moving forward
} else if(sensor_left == 0 && sensor_center == 0 && sensor_right == 0) {  // All three sensors see lines, this is a crossroad, keep moving forward
    advance (132,175);                                                    // Move staight forward (different torques between the motors were necessary to maintain straight path)
}

}
