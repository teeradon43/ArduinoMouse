/*------------------ Glove Mouse ---------------------------
 * Author: Teeradon Chanhom
 * Equipment: STM32F427/F767 + MPU6050
 * 
 * Fn:
 * 1. Enable
 * 2. Left Click (Hold to drag)
 * 3. Right Click
 * 4. Auto Click (try 100ms)
 * 5. Change DPI (1/2/3)
 * 6. Scroll

----------------------------------------------------------*/
#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include <HMouse.h>

#define EN 4
#define LBtn 5
#define RBtn 6
#define FN1 7
#define FN0 8
#define AUTO_CLICK 9

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

MPU6050 mpu;
int16_t ax, ay, az, gx, gy, gz;
int vx, vy;

boolean enable = false;

//INPUT AND BOUNCE
int LState, oldLState = 0; 
int RState, oldRState = 0;
int AutoState = 0;
int f1State = 0;
int f0State = 0;

int mode = 0; // 0:Normal, 1: Scroll, 2: ChangeDPI, 3: Calibrate

int DPI = 1;

void setup() {
  
  Serial.begin(115200);
  
  Wire.begin();
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("!!MPU ERROR!!");
    delay(500);
  }

  pinMode(EN, INPUT);
  pinMode(LState, INPUT);
  pinMode(RState, INPUT);
  pinMode(FN1, INPUT);
  pinMode(FN0, INPUT);
  pinMode(AUTO_CLICK, INPUT);
}

void getMotion(int* x, int* y){
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  *x = gx/2400;
  *y = -gz/2400;
}

void handleMouse(){
  // GET BUTTON STATE
  LState = digitalRead(LBtn);
  RState = digitalRead(RBtn);
  AutoState = digitalRead(AUTO_CLICK);

  if(AutoState == 1){
    HMouse.click();
  }
  
  if(LState != oldLState){ // Raising, Falling Left
    if(LState == 1) // Raising Left
    {
      HMouse.press(MOUSE_LEFT);
    }
    else if(LState == 0) // Falling Left
    {
      HMouse.release(MOUSE_LEFT);
    }
  }
  
  if(RState != oldRState && RState == 0) // Falling Right Only
  {
    HMouse.click(MOUSE_RIGHT);
  }
  
  oldLState = LState;
  oldRState = RState;
}

void moveMouse2screenCenter() {
  moveMouse2UpperLeftCorner();
  for (int i = 0; i < 5; i++) {
    HMouse.move(64, 0, 0);
  }
  for (int i = 0; i < 4; i++) {
    HMouse.move(0, 48, 0);
  }
}

void moveMouse2UpperLeftCorner() {
  for (int i = 0; i < 6; i++) {
    HMouse.move(-127, -127, 0);
  }
}

void checkMode(int f1, int f2){
  mode = f1*2 + f2;
}


void loop() {
  enable = digitalRead(EN);
  Serial.print("EN : ");Serial.println(enable);
  
  if(enable == 1)
  {
    f1State = digitalRead(FN1);
    f0State = digitalRead(FN0);

    getMotion(&vx,&vy);
    vx *= DPI;
    vy *= DPI;

    checkMode(f1State, f0State);

//  Serial.print("MODE : ");Serial.println(mode);
    switch(mode){
      case(0): // Normal
          HMouse.move(vx, vy);
          handleMouse();
          break;
      case(1): // Scroll
          vx /= 4;
          vx /= 4;
          HMouse.move(0, 0, vy, vx);
          break;
      case(2): // ChangeDPI
          DPI++;
          if(DPI>3)DPI = 1;
          while( digitalRead(FN1)&& digitalRead(FN0)==0 ); // what until DPI Signal's gone
          break;
      case(3): // Calibrate
          moveMouse2screenCenter();
          while( digitalRead(FN1)&& digitalRead(FN0) ); // what until DPI Signal's gone
          break;
      default:
          Serial.println("FN ERROR");
          break;
    }
  }
}
