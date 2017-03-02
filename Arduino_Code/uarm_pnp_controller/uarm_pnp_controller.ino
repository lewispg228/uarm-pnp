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

int step_size = 1; // used for manual driving
int cmd_count = 1;

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
      case '+':
        pump_on();
        break;       
      case '-':
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
      case '%':
        glide_to_pos_angle(5,70);
        break;        
      case '^':
        glide_to_pos_angle(6,70);
        break;     
      case '&':
        glide_to_pos_angle(7,70);
        break;  
      case '*':
        glide_to_pos_angle(8,70);
        break;        
      case '(':
        glide_to_pos_angle(9,70);
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
      case 't':
        store_pos_angle(5);
        break;          
      case 'y':
        store_pos_angle(6);
        break;   
      case 'u':
        store_pos_angle(7);
        break;   
      case 'i':
        store_pos_angle(8);
        break;   
      case 'o':
        store_pos_angle(9);
        break;                                             
      case 'p':
        get_pos_angle();
        break;         
      case 'x':
        run_sequence();
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
  int mem_bank_location_start = 4*pos; // each bank is 4 spots, cuz there's 4 servo angles to remember.
  
  EEPROM.write(mem_bank_location_start, servo_angle[0]);
  EEPROM.write((mem_bank_location_start+1), servo_angle[1]);
  EEPROM.write((mem_bank_location_start+2), servo_angle[2]);
  EEPROM.write((mem_bank_location_start+3), servo_angle[3]);
}

void goto_pos_angle(int pos)
{
  int mem_bank_location_start = 4*pos;

  servo_angle[0] = EEPROM.read(mem_bank_location_start);
  servo_angle[1] = EEPROM.read((mem_bank_location_start+1));
  servo_angle[2] = EEPROM.read((mem_bank_location_start+2));
  servo_angle[3] = EEPROM.read((mem_bank_location_start+3));

  set_position_angle(servo_angle[0], servo_angle[1], servo_angle[2], servo_angle[3]);
}

// glide from one position to the next, with some control of speed.
// basically, break up the travel distance and add in some delays to slow it down.
void glide_to_pos_angle(int destination, int delay_time)
{
  boolean glide_complete = false;

  // pull in destination from eeprom - just like we do when we use goto_pos_angle(), but this time, store it as destinations
  int mem_bank_location_start = 4*destination;

  destination_angle[0] = EEPROM.read(mem_bank_location_start);
  destination_angle[1] = EEPROM.read((mem_bank_location_start+1));
  destination_angle[2] = EEPROM.read((mem_bank_location_start+2));
  destination_angle[3] = EEPROM.read((mem_bank_location_start+3));

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
    calculate_all_angle_increments(false);  
  }
}

void calculate_all_angle_increments(boolean debug)
{
  calculate_angle_increment(0, false); 
  calculate_angle_increment(1, false);
  calculate_angle_increment(2, false);
  calculate_angle_increment(3, false);
  if(debug)
  {
    Serial.print("angle increments: ");
    Serial.print(angle_increment[0]);
    Serial.print(", ");
    Serial.print(angle_increment[1]);
    Serial.print(", ");
    Serial.print(angle_increment[2]);
    Serial.print(", ");
    Serial.println(angle_increment[3]);    
  }
}

void calculate_angle_increment(int s, boolean debug) // "s" is for servo number 0-3
 {
  int difference = destination_angle[s] - servo_angle[s];
  difference = abs(difference);
  if(debug) 
  {
    Serial.print("difference: ");
    Serial.println(difference);
  }
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

 void run_sequence()
 {
  for(int i = 1 ; i < 10 ; i++)
  {
    glide_to_pos_angle(i,70);
    delay(500);
  }
 }

