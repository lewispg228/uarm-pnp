#include <EEPROM.h>

// servo_movement_status is used in glide functions to make smoother motion
// note, if a servo is stopped, then it is ready to accelerate, so it's in
// the "ACCERATING" status
#define ACCELERATING 0
#define AT_FULL_SPEED 1
#define DECELERATING 2
int servo_movement_status[4] = {ACCELERATING, ACCELERATING, ACCELERATING, ACCELERATING};
int angle_increment[4] = {1,1,1,1};
int servo_angle[4] = {100, 120, 30, 90}; // base, left, right, hand
int destination_angle[4] = {100, 120, 30, 90}; // used for accel/decel functions

float x = 150.0;
float y = 150.0;
float z = 150.0;
float s = 25.0;
int step_size = 1;
int cmd_count = 1;

float x_1 = 100.0;
float y_1 = 100.0;
float z_1 = 100.0;

float x_2 = 100.0;
float y_2 = 100.0;
float z_2 = 100.0;

float x_3 = 100.0;
float y_3 = 100.0;
float z_3 = 100.0;

float x_4 = 100.0;
float y_4 = 100.0;
float z_4 = 100.0;

/// commands learned from sniffing the pyuarm comm

// get position angle: P200
// set angle of servo: G202 N0 V55   //  N<servo number 0 -3> V<angle 0-180>  , note, servo 0 is the rotational servo in the base, 1 is left, 2 is right, 3 is hand



void setup() {
  Serial.begin(115200); // debug
  Serial.println("uarm testing");
  Serial1.begin(115200); // to uarm
}

void loop() {
  if(Serial.available() > 0)
  {
    int input = Serial.read();
    Serial.println(char(input));
    switch (input) {
      case 'a':
        x = 150.0; y = 150.0; z = 150.0; s = 001.0;
        break;
      case 'b':
        x = -160.0; y = 200.0; z = 60.0; s = 001.0;
        break;
      case '0':
        servo_angle[0] -= step_size;
        break;   
      case '.':
        servo_angle[0] += step_size;
        break;   
      case '1':
        servo_angle[1] -= step_size;
        break;   
      case '2':
        servo_angle[1] += step_size;
        break; 
      case '4':
        servo_angle[2] -= step_size;
        break;   
      case '5':
        servo_angle[2] += step_size;
        break; 
      case '7':
        servo_angle[3] -= step_size;
        break;   
      case '8':
        servo_angle[3] += step_size;
        break;         
      case '*':
        pump_on();
        break;       
      case '/':
        pump_off();
        break;   
      case 's': // toggle step size between 1, 10 and 20
        if(step_size == 1) {step_size = 10;}
        else if(step_size == 10) {step_size = 20;}
        else if(step_size == 20) {step_size = 1;}
        Serial.print("step_size: ");
        Serial.println(step_size);
        break;           
      case '!':
        glide_to_pos_angle(1,70);
        break;  
      case '@':
        glide_to_pos_angle(2,70);
        break;     
      case '#':
        glide_to_pos_angle(3,70);
        break;  
      case '$':
        glide_to_pos_angle(4,70);
        break;               
      case 'q':
        store_pos_angle(1);
        break;  
      case 'w':
        store_pos_angle(2);
        break;       
      case 'e':
        store_pos_angle(3);
        break;  
      case 'r':
        store_pos_angle(4);
        break;      
      case 'p':
        get_pos_angle();
        break;              
    }
    set_position_angle(servo_angle[0], servo_angle[1], servo_angle[2], servo_angle[3]);
  }
  delay(1);
  
  if(Serial1.available() > 0)
  {
    int input = Serial1.read();
    Serial.print(char(input));
  }
}

void set_position()
{
  //Serial1.print("#74 G0 X150.0 Y150.0 Z150.0 F100.0\n");
  //Serial1.print("#74");
  send_cmd_count();
  Serial1.print(" G0");
  Serial1.print(" X");
  Serial1.print(x,1);
  Serial1.print(" Y");
  Serial1.print(y,1);
  Serial1.print(" Z");
  Serial1.print(z,1);
  Serial1.print(" F");
  Serial1.print(s,1);
  Serial1.print("\n");
  
  Serial.print("#");
  Serial.print(cmd_count);
  Serial.print(" G0");
  Serial.print(" X");
  Serial.print(x,1);
  Serial.print(" Y");
  Serial.print(y,1);
  Serial.print(" Z");
  Serial.print(z,1);
  Serial.print(" F001.0");
  //Serial.print(s,1);
  Serial.print("\n");
}

void pump_on()
{
  Serial1.print("#74");
  Serial1.print(" M231 V1\n");
}


void pump_off()
{
  Serial1.print("#74");
  Serial1.print(" M231 V0\n");
}

void get_pos_angle()
{
  send_cmd_count();
  Serial1.print(" P200\n");
}

void get_pos()
{
  Serial1.print("#74");
  Serial1.print(" P220\n");
  delay(10);
}

void send_cmd_count()
{
  cmd_count++;
  Serial1.print("#");
  Serial1.print(cmd_count);
}

void goto_pos(int pos)
{
  switch (pos) {
    case 1:
      x = x_1;
      y = y_1;
      z = z_1;
      set_position();
      break;
    case 2:
      x = x_2;
      y = y_2;
      z = z_2;
      set_position();
      break;
    case 3:
      x = x_3;
      y = y_3;
      z = z_3;
      set_position();
      break;
    case 4:
      x = x_4;
      y = y_4;
      z = z_4;
      set_position();
      break;      
  }
}

void store_pos(int pos)
{
  switch (pos) {
    case 1:
      x_1 = x;
      y_1 = y;
      z_1 = z;
      break;
    case 2:
      x_2 = x;
      y_2 = y;
      z_2 = z;
      break;
    case 3:
      x_3 = x;
      y_3 = y;
      z_3 = z;
      break;    
    case 4:
      x_4 = x;
      y_4 = y;
      z_4 = z;
      break;  
  }
}

void set_position_angle(int s0, int s1, int s2, int s3)
{
  // DEBUG
  Serial.print("servo angles: ");
  Serial.print(s0);
  Serial.print(", ");
  Serial.print(s1);
  Serial.print(", ");
  Serial.print(s2);
  Serial.print(", ");
  Serial.println(s3);

  Serial.print("angle increments: ");
  Serial.print(angle_increment[0]);
  Serial.print(", ");
  Serial.print(angle_increment[1]);
  Serial.print(", ");
  Serial.print(angle_increment[2]);
  Serial.print(", ");
  Serial.println(angle_increment[3]);
  
  send_cmd_count();
  Serial1.print(" G202");
  Serial1.print(" N0 V");
  Serial1.print(s0);
  Serial1.print("\n");

  send_cmd_count();
  Serial1.print(" G202");
  Serial1.print(" N1 V");
  Serial1.print(s1);
  Serial1.print("\n");

  send_cmd_count();
  Serial1.print(" G202");
  Serial1.print(" N2 V");
  Serial1.print(s2);
  Serial1.print("\n");

  send_cmd_count();
  Serial1.print(" G202");
  Serial1.print(" N3 V");
  Serial1.print(s3);
  Serial1.print("\n");
}


void store_pos_angle(int pos)
{
  switch (pos) {
    case 1:
      EEPROM.write(0, servo_angle[0]);
      EEPROM.write(1, servo_angle[1]);
      EEPROM.write(2, servo_angle[2]);
      EEPROM.write(3, servo_angle[3]);
      break;
    case 2:
      EEPROM.write(4, servo_angle[0]);
      EEPROM.write(5, servo_angle[1]);
      EEPROM.write(6, servo_angle[2]);
      EEPROM.write(7, servo_angle[3]);
      break;
    case 3:
      EEPROM.write(8, servo_angle[0]);
      EEPROM.write(9, servo_angle[1]);
      EEPROM.write(10, servo_angle[2]);
      EEPROM.write(11, servo_angle[3]);
      break;    
    case 4:
      EEPROM.write(12, servo_angle[0]);
      EEPROM.write(13, servo_angle[1]);
      EEPROM.write(14, servo_angle[2]);
      EEPROM.write(15, servo_angle[3]);
      break;  
  }
}

void goto_pos_angle(int pos)
{
  switch (pos) {
    case 1:
      servo_angle[0] = EEPROM.read(0);
      servo_angle[1] = EEPROM.read(1);
      servo_angle[2] = EEPROM.read(2);
      servo_angle[3] = EEPROM.read(3);
      break;
    case 2:
      servo_angle[0] = EEPROM.read(4);
      servo_angle[1] = EEPROM.read(5);
      servo_angle[2] = EEPROM.read(6);
      servo_angle[3] = EEPROM.read(7);
      break;
    case 3:
      servo_angle[0] = EEPROM.read(8);
      servo_angle[1] = EEPROM.read(9);
      servo_angle[2] = EEPROM.read(10);
      servo_angle[3] = EEPROM.read(11);
      break;
    case 4:
      servo_angle[0] = EEPROM.read(12);
      servo_angle[1] = EEPROM.read(13);
      servo_angle[2] = EEPROM.read(14);
      servo_angle[3] = EEPROM.read(15);
      break;      
  }
  set_position_angle(servo_angle[0], servo_angle[1], servo_angle[2], servo_angle[3]);
}

// glide from one position to the next, with some control of speed.
// basically, break up the travel distance and add in some delays to slow it down.
void glide_to_pos_angle(int destination, int delay_time)
{
    boolean glide_complete = false;
      
    switch (destination) {
    case 1:
      destination_angle[0] = EEPROM.read(0);
      destination_angle[1] = EEPROM.read(1);
      destination_angle[2] = EEPROM.read(2);
      destination_angle[3] = EEPROM.read(3);
      break;
    case 2:
      destination_angle[0] = EEPROM.read(4);
      destination_angle[1] = EEPROM.read(5);
      destination_angle[2] = EEPROM.read(6);
      destination_angle[3] = EEPROM.read(7);
      break;
    case 3:
      destination_angle[0] = EEPROM.read(8);
      destination_angle[1] = EEPROM.read(9);
      destination_angle[2] = EEPROM.read(10);
      destination_angle[3] = EEPROM.read(11);
      break;
    case 4:
      destination_angle[0] = EEPROM.read(12);
      destination_angle[1] = EEPROM.read(13);
      destination_angle[2] = EEPROM.read(14);
      destination_angle[3] = EEPROM.read(15);
      break;      
  }

  int max_angle_increment = 5;
  int min_decel_steps = 10;
  angle_increment[0] = 1;
  angle_increment[1] = 1;
  angle_increment[2] = 1;
  angle_increment[3] = 1;
  
  while(glide_complete == false)
  {
    if(destination_angle[0] > servo_angle[0]) servo_angle[0] += angle_increment[0];
    else if(destination_angle[0] < servo_angle[0])servo_angle[0] -= angle_increment[0];

    if(destination_angle[1] > servo_angle[1]) servo_angle[1] += angle_increment[1];
    else if(destination_angle[1] < servo_angle[1]) servo_angle[1] -= angle_increment[1];

    if(destination_angle[2] > servo_angle[2]) servo_angle[2] += angle_increment[2];
    else if(destination_angle[2] < servo_angle[2])servo_angle[2] -= angle_increment[2];
    
    if(destination_angle[3] > servo_angle[3]) servo_angle[3] += angle_increment[3];
    else if(destination_angle[3] < servo_angle[3]) servo_angle[3] -= angle_increment[3];

    set_position_angle(servo_angle[0], servo_angle[1], servo_angle[2], servo_angle[3]);

    delay(delay_time);
    
    if( (destination_angle[0] == servo_angle[0]) &&
        (destination_angle[1] == servo_angle[1]) &&
        (destination_angle[2] == servo_angle[2]) &&
        (destination_angle[3] == servo_angle[3]) )
        {
          glide_complete = true;
        }  
    calculate_all_angle_increments();  
  }
}

void calculate_all_angle_increments()
{
  calculate_angle_increment(destination_angle[0], servo_angle[0], 0); 
  calculate_angle_increment(destination_angle[1], servo_angle[1], 1);
  calculate_angle_increment(destination_angle[2], servo_angle[2], 2);
  calculate_angle_increment(destination_angle[3], servo_angle[3], 3);
}

void calculate_angle_increment(int destination_angle, int current_angle, int s) // "s" is for servo number 0-3
 {
  int difference = destination_angle - current_angle;
  difference = abs(difference);
  Serial.print("difference: ");
  Serial.println(difference);
  if(difference <= 15) servo_movement_status[s] = DECELERATING;
  else if(angle_increment[s] == 5) servo_movement_status[s] = AT_FULL_SPEED;
  else servo_movement_status[s] = ACCELERATING;

  switch(servo_movement_status[s]){
    case DECELERATING:
      if(angle_increment[s] > 1) angle_increment[s]--;
      break;
    case AT_FULL_SPEED:
      // do nothing, stay at full speed change (aka angle increment should be 5)
      break;
    case ACCELERATING:
      if(angle_increment[s] < 5) angle_increment[s]++;
      break;            
  }
 }

