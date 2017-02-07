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

void setup() {
  Serial.begin(115200); // debug
  Serial.println("uarm testing");
  Serial1.begin(115200); // to uarm
}

void loop() {
  if(Serial.available() > 0)
  {
    int input = Serial.read();
    Serial.println(input);
    switch (input) {
      case 'a':
        x = 150.0; y = 150.0; z = 150.0; s = 1.0;
        break;
      case 'b':
        x = -160.0; y = 200.0; z = 60.0; s = 1.0;
        break;
      case '4':
        x -= step_size;
        break;   
      case '6':
        x += step_size;
        break;   
      case '2':
        y -= step_size;
        break;   
      case '8':
        y += step_size;
        break; 
      case '-':
        z -= step_size;
        break;   
      case '+':
        z += step_size;
        break; 
      case '*':
        pump_on();
        break;       
      case '/':
        pump_off();
        break;   
      case 's': // toggle step size between 1, 10 and 50
        if(step_size == 1) {step_size = 10;}
        else if(step_size == 10) {step_size = 50;}
        else if(step_size == 50) {step_size = 1;}
        Serial.print("step_size: ");
        Serial.println(step_size);
        break;           
      case '!':
        goto_pos(1);
        break;  
      case '@':
        goto_pos(2);
        break;     
      case '#':
        goto_pos(3);
        break;  
      case '$':
        goto_pos(4);
        break;               
      case 'q':
        store_pos(1);
        break;  
      case 'w':
        store_pos(2);
        break;       
      case 'e':
        store_pos(3);
        break;  
      case 'r':
        store_pos(4);
        break;                
    }
    set_position();
  }
  delay(1);
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
  Serial.print(" F");
  Serial.print(s,1);
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

