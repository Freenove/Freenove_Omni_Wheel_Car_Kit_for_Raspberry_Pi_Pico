/**********************************************************************
  Filename    : Sketch_09.1_Infrared
  Description : Use Raspberry Pi Pico read infrared key values
  Auther      : www.freenove.com
  Modification: 2024/08/23
**********************************************************************/
const int IR_Pin = 22;// Define the infrared pin number

int logList[32]; 
unsigned long startTime; 
int endTime, end2Time; 
int flagCode = 0; 
int irPin; 
bool irState = true; 

void setup() { 
  Serial.begin(115200);// Set the serial port baud rate to 115200
  IR_Init(IR_Pin);// Infrared pin initialization
}

void loop() {
  if(flagCode){ 
    int irValue = IR_Decode(flagCode);// Read infrared key values
    Serial.println(irValue, HEX);// Print infrared key values
    IR_Release(); 
  } 
}

void IR_Init(int pin) { 
  irPin = pin; 
  pinMode(irPin, INPUT_PULLUP);// Enable infrared pin as input pull-up
  attachInterrupt(digitalPinToInterrupt(irPin), IR_Read, CHANGE);// Start infrared interrupt
} 
 
void IR_Read() { 
  if (irState == true) { 
    unsigned long lowTime, highTime, intervalTime; 
    int num = 0; 
    while (digitalRead(irPin) == LOW) { 
      startTime = micros(); 
      while (digitalRead(irPin) == LOW) { 
        lowTime = micros(); 
      } 
      intervalTime = lowTime - startTime; 
      while (digitalRead(irPin) == HIGH) { 
        highTime = micros(); 
        intervalTime = highTime - lowTime; 
        if (intervalTime > 10000) { 
          end2Time = millis(); 
          if (num == 32) { 
            flagCode = 1; 
            endTime = millis(); 
          } 
          else if (num == 0 && end2Time - endTime > 300 && end2Time - endTime < 400) { 
            flagCode = 2; 
            endTime = millis(); 
          } 
          return; 
        } 
      } 
      if (intervalTime < 2000) { 
        if (intervalTime < 700) { 
         logList[num ++] = 0; 
        } 
        else { 
          logList[num ++] = 1; 
        } 
      } 
    } 
  } 
} 
 
unsigned long IR_Decode(int &code) { 
  unsigned long irData = 0; 
  irState=false; 
  if (code == 1) { 
    code = 0; 
    for (int i = 0; i < 32; i ++) { 
      if (logList[i] == 0) { 
        irData <<= 1; 
      } 
      else { 
        irData <<= 1; 
        irData ++; 
      } 
      logList[i] = 0; 
    } 
  } 
  if (code == 2) { 
    code = 0; 
    irData = 0xffffffff; 
  } 
  return irData; 
} 
 
void IR_Release(){ 
  irState=true; 
} 