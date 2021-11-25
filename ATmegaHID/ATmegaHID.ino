#include <HMouse.h>

uint8_t vx, vy, input = 0;
char txt[6][20] = {"STOP","MOVE LEFT","MOVE RIGHT"," "," ","TOGGLE ENABLE"};
bool enable = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  toggleEnable();
}

void loop() {
  // put your main code here, to run repeatedly:
  
  if(Serial.available() > 0) // READ Serial
  {
    input = Serial.readString().toInt();
    if(input==5)toggleEnable();
    Serial.print("INPUT : ");
    Serial.print(input);
    Serial.print(" ");
    Serial.print(txt[input]);
    Serial.print(" ");
    Serial.println(enable);
  }
  
  switch(input){ // APPLY Value
    case 0: 
      vx=0;
      break;
    case 1:
      vx--;
      break;
    case 2:
      vx++;
      break;      
  }
  
  // WORKING
  HMouse.move(vx, vy);
  delay(200);
}

void toggleEnable(){
  if(enable){ HMouse.end(); enable=0;}
  else{ HMouse.begin(); enable=1;}
}
