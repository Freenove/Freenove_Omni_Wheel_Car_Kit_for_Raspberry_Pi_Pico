/***********************************************************************************
  Filename    : Sketch_13.1_Kinematic_analysis
  Description : Use Raspberry Pi Pico Control the trolley to move in all directions
  Auther      : www.freenove.com
  Modification: 2024/08/23
************************************************************************************/

const int wheel1_A_pin = 8; // Motor1 drive pin
const int wheel1_B_pin = 9; // Motor1 drive pin

const int wheel2_A_pin = 12;// Motor2 drive pin
const int wheel2_B_pin = 13;// Motor2 drive pin

const int wheel3_A_pin = 15;// Motor3 drive pin
const int wheel3_B_pin = 14;// Motor3 drive pin

const int wheel4_A_pin = 20;// Motor4 drive pin
const int wheel4_B_pin = 21;// Motor4 drive pin

int a1,b1,a2,b2,a3,b3,a4,b4;
int v1,v2,v3,v4;

void setup() {
  // put your setup code here, to run once:
  Motor_init();
}

void loop() {
  // put your main code here, to run repeatedly:
  Motor_Control(200,0);    // Forward
  delay(1000);          
  Motor_Control(-200,0);   // Back
  delay(1000);
  Motor_Control(200,90);   // Left translation
  delay(1000);
  Motor_Control(200,-90);  // Right translation
  delay(1000);
  Motor_Control(255,45);   // Shift 45 degrees to the left
  delay(1000);
  Motor_Control(255,-135); // Shift 135 degrees to the right
  delay(1000);
  Motor_Control(255,135);  // Shift 135 degrees to the left
  delay(1000);
  Motor_Control(255,-45);  // Shift 45 degrees to the right
  delay(5000);

}
void Motor_Control(int speed_v,int speed_a)
{
  int vx =  - speed_v * sin ( speed_a * PI / 180 );
  int vy =    speed_v * cos ( speed_a * PI / 180 );

  v1 = -vx;
  v2 = -vy;
  v3 =  vx;
  v4 =  vy;

  Motor_direction();//Motor limiting and motor output orientation
}

void Motor_direction()
{
  if(v1 > 0){ a1 = v1; b1 = LOW; }else{ a1 = LOW; b1 = -v1;}
  if(v2 > 0){ a2 = v2; b2 = LOW; }else{ a2 = LOW; b2 = -v2;}
  if(v3 > 0){ a3 = v3; b3 = LOW; }else{ a3 = LOW; b3 = -v3;}
  if(v4 > 0){ a4 = v4; b4 = LOW; }else{ a4 = LOW; b4 = -v4;}

  analogWrite(wheel1_A_pin,a1);
  analogWrite(wheel1_B_pin,b1);

  analogWrite(wheel2_A_pin,a2);
  analogWrite(wheel2_B_pin,b2);

  analogWrite(wheel3_A_pin,a3);
  analogWrite(wheel3_B_pin,b3);

  analogWrite(wheel4_A_pin,a4);
  analogWrite(wheel4_B_pin,b4);
}

void Motor_init()
{
  // Set the motor PWM frequency to 16 kHz
  analogWriteFreq(16000);
  // Enable the motor drive pin to output mode
  pinMode(wheel1_A_pin,OUTPUT); 
  pinMode(wheel1_B_pin,OUTPUT);
  pinMode(wheel2_A_pin,OUTPUT);
  pinMode(wheel2_B_pin,OUTPUT);
  pinMode(wheel3_A_pin,OUTPUT);
  pinMode(wheel3_B_pin,OUTPUT);
  pinMode(wheel4_A_pin,OUTPUT);
  pinMode(wheel4_B_pin,OUTPUT);
}