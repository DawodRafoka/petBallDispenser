/*
* Authors: Dawod Rafoka & James Bunnill
* Project: Pet Ball Dispenser
* Class: CompE-496 | Senior Design B
* Date: 10/10/2021
* File: MainProgram.ino intefaces with the Arduino
*/

/*#########################################################*/
/*#################### INITIALIZERS #######################*/
/*#########################################################*/

/***** INCLUDE LIBRARIES/FILES ******/
#include <Stepper.h>
#include <LcdMenu.h>
#include <SimpleRotary.h>
#include <DueFlashStorage.h>
#include <efc.h>
#include <flash_efc.h>
#include <AccelStepper.h>


/*****      DEFINITIONS        ******/
//LCD Screen
#define LCD_ROWS 2
#define LCD_COLS 16


//Rotary Encoder
#define CLK 2 //Generate interupts using CLK signal
#define DT 3  //to read the DT signal
#define SW 4  //to read the Knob Push switch

//Sensors
#define LIGHT_SENSOR_READ A0
#define flashAddress 4
int sensorReadRes, intensityActivationValue; //analogReadResolution can return 12bits
byte storeToMemory[sizeof(intensityActivationValue)];

//Stepper Motor & Ground
#define ClOCKWISE 1
#define COUNTERClOCKWISE 0


/*****       VARIABLES         ******/
// STEPPER & DC MOTOR & JAM
uint8_t StepPin = 5;
uint8_t DirPin = 6;
uint8_t DCM1 = 50;
uint8_t DCM2 = 52;
uint8_t LimHome = 51;
uint8_t LimDeploy = 53;
uint8_t UnjamAmount = 400//intally 320
uint8_t Jams = 0;
uint8_t JammedDelay = 20;
uint8_t Attempts = 0;
uint8_t MaxAttempts = 3;
int HOME = -1;
int DEPLOYED = 1;
int ReadyPos = -160;
int DelpoyedPos = -1800;

// Initialize LcdMenu with the menu items
const int rs = 32, en = 30, d4 = 28, d5 = 26, d6 = 24, d7 = 22;

// Declare the array of members
extern String about[];
// Initialize the array
String about[] = {"Ver:1.0 ->", "Members->", "Dawod  :)", "James  :D", "Jared  ;D", "Othman ;)"};
int aboutLength = sizeof(about) / sizeof(about[0]);

/***** FUNCTIONS DECLURATIONS  ******/
void readLightSensor();
void resetCallback(uint8_t pos);
// void turnDCmotorON();
// void turnDCmotorOFF();
// void DCmotorControl();
char *integerToCharPtr(int value);
bool lightSensorCalibrate();

// Construct the Rotary Encoder
SimpleRotary rotary(CLK, DT, SW);

// Construct the LcdMenu
LcdMenu menu(LCD_ROWS, LCD_COLS);

// Construct the Stepper Motor Driver
// Stepper stepperMotor(stepsPerRevolution, stepDIR, stepSTEP);

//Construct EEPROM non-volatile memory
DueFlashStorage flashStorage;

//STEPPER
AccelStepper Nema17(AccelStepper::DRIVER, StepPin, DirPin);

int UnJam(int DIR);
int Dispense(int n);
int Deploy();
int TurnOffDCMotor();
int TurnOnDCMotor();

// Declare the main menu
extern MenuItem mainMenu[];
// Initialize the main menu items
MenuItem mainMenu[] = {ItemHeader(),
                       MenuItem("Ball Dispenser"),
                       MenuItem("Despense"),
                       MenuItem("Unjam"),
                       MenuItem("Calibrate"),
                       ItemList("About", about, aboutLength, resetCallback),
                       ItemFooter()};

// To track and control the rotary encoder
unsigned int position = 0;
unsigned int aboutPosition = 0;
const int DESPENCE = 1;
const int UNJAM = 2;
const int CALIBRATE = 3;
const int ABOUT = 4;
byte r, p, pLong; //rotate, press, long-press

// Extra Variables
int analogValue;
char tempCharPtr[10];

/*#########################################################*/
/*#################### MAIN PROGRAM #######################*/
/*#########################################################*/
void setup()
{
    Serial.begin(9600);
    menu.setupLcdWithMenu(rs, en, d4, d5, d6, d7, mainMenu);
    //STEPPER MOTOR DRIVER
    // pinMode(stepSTEP, OUTPUT);
    // pinMode(stepDIR, OUTPUT);
    // pinMode(currentSensor1, INPUT);
    // pinMode(currentSensor2, INPUT);

    //DC MOTOR DRIVER
    // pinMode(DcEN, OUTPUT);
    // pinMode(DcDIR, OUTPUT);
    // Turn off motors - initially
    // turnDCmotorOFF();

    //Light Sensor Calibartion Initialization

    analogReadResolution(12); //Making the resolution 12 bits instead of 10
    //Flash is reset evertime a new code is uploaded
    //Check if this is the first time, returns 255 if so
    uint8_t codeRunningForFirstTime = flashStorage.read(0);

    if (codeRunningForFirstTime)
    {
        //Initialize with default intensityActivationValue
        intensityActivationValue = 3686; //Corresponding 90% of MAX=4096
        //Create byte array to store the value in memory
        memcpy(storeToMemory, &intensityActivationValue, sizeof(intensityActivationValue));
        flashStorage.write(flashAddress, storeToMemory, sizeof(intensityActivationValue)); //store it in non-volatile space
        //write 0 to address 0 to show it's not the first time anymore
        flashStorage.write(0, 0);
    }
    else
    {
        //Create byte array to read the value from memory
        byte *valueFromMemory = flashStorage.readAddress(flashAddress);
        memcpy(&intensityActivationValue, valueFromMemory, sizeof(intensityActivationValue));
    }

    // STEPPER
    pinMode(A1,INPUT);
    pinMode(A2,INPUT);
    pinMode(LimHome,INPUT_PULLUP);
    pinMode(LimDeploy,INPUT_PULLUP);
    pinMode(DCM1, OUTPUT);
    pinMode(DCM2,OUTPUT);
    
    //Nema17.init();
    Nema17.setMaxSpeed(1000);
    Nema17.setSpeed(10);
    Nema17.setAcceleration(5);
    
    //inital homming
    HomeStepper();
}

void loop()
{
    //Read the values from the rotary encoder
    r = rotary.rotate(); //0 not running, 1 CW, 2 CCW
    p = rotary.push();
    pLong = rotary.pushLong(500);

    sensorReadRes = analogRead(LIGHT_SENSOR_READ);
    if(sensorReadRes > intensityActivationValue)
    {
        Serial.print("Light Dispensing Activated");
        Serial.print( " | SensorRead: ");
        Serial.print(sensorReadRes);
        Serial.print(" | Trigger Value: ");
        Serial.print(intensityActivationValue);
        menu.displayNotification("Laser Dispense", 1);
        Dispense(1);
    }
    
    if (r == 1)
    {
        if (position > 0)
            position--;

        menu.up();
        Serial.print(position);
        Serial.print("CCW \n");
    }
    else if (r == 2)
    {
        if (position < 4)
            position++;

        menu.down();
        Serial.print(position);
        Serial.print("CW \n");
    }

    if (p == 1) //User pressed the knob
    {
        //determine the position the user is on
        switch (position)
        {
        case DESPENCE:
            menu.down(); menu.up(); //So Notifications don't obstruct the value
            // DCmotorControl("Despence...", 2000);
            menu.displayNotification("Dispensing!", 2000);
            if(Dispense(1) == 1){                
                menu.displayNotification("Failed Dispense, please check mechanisim", 2000);
                //Failed Dispense
            }
            else{
                menu.displayNotification("Success!", 2000);
                //Succesfull Dispense
            }
            break;
        case UNJAM:
            menu.down(); menu.up(); //So Notifications don't obstruct the value
            break;
        case CALIBRATE:
            menu.down(); menu.up(); //So Notifications don't obstruct the value
            Serial.print("Previous Intensity: ");
            Serial.print(intensityActivationValue);
            menu.displayNotification("Calibrating...", 1000);
            if (lightSensorCalibrate())
            {
                delay(500); // CHECKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK
                menu.displayNotification("Done!", 1);

                Serial.print(" | ");
                Serial.print("New Intensity: ");
                Serial.print(intensityActivationValue);
                Serial.print("\n");
            }
            else
            {
                Serial.print("Calibartion Failed\n");
            }
            break;
        case ABOUT:
            aboutPosition++; //to prevent overflow
            if (aboutPosition < aboutLength)
            {
                menu.right();
            }
            else
            {
                while (aboutPosition > 0)
                {
                    menu.left();
                    aboutPosition--;
                }
            }
            break;
        default:
            break;
        }

        Serial.print("Pressed \n");
    }

    // if (pLong == 1) //User pressed the knob from more than half second
    // {
    //     Serial.println("half Sec Long Pushed");
    //     DCmotorControl("Despence...", 2000);
    //     //menu.displayNotification("Despending...", 2000);
    // }
}

/*#########################################################*/
/*###################### FUNCTIONS ########################*/
/*#########################################################*/

char *integerToCharPtr(int value)
{
    String tempString = String(value);
    tempString.toCharArray(tempCharPtr, sizeof(tempCharPtr));
    return tempCharPtr;
}

void readLightSensor()
{
    analogValue = analogRead(LIGHT_SENSOR_READ);

    menu.displayNotification(integerToCharPtr(analogValue), 500);
    Serial.println(analogRead(A0));
}

// void DCmotorControl(String message, int _delay)
// {
//     turnDCmotorON();
//     message.toCharArray(tempCharPtr, sizeof(tempCharPtr));
//     menu.displayNotification(tempCharPtr, 2000);
//     delay(_delay);
//     turnDCmotorOFF();
// }


void resetCallback(uint8_t pos)
{
    Serial.println(about[pos]);
}

bool lightSensorCalibrate()
{
    sensorReadRes = analogRead(LIGHT_SENSOR_READ);
    if ((sensorReadRes > 4060) || (sensorReadRes <= 0))
    {
        //Area too bright, sensor not functioning
        return false;
    }
    else
    {
        if ((sensorReadRes > 0) && (sensorReadRes < 820))
        {                                    //0-20%
            intensityActivationValue = 3072; //make it activate at 75%
        }
        else if ((sensorReadRes >= 820) && (sensorReadRes < 1638))
        {                                    //20-40%
            intensityActivationValue = 3482; //make it activate at 85%
        }
        else if ((sensorReadRes >= 1638) && (sensorReadRes < 2048))
        {                                    //40-50%
            intensityActivationValue = 3686; //make it activate at 90%
        }
        else if ((sensorReadRes >= 2048) && (sensorReadRes < 2458))
        {                                    //50-60%
            intensityActivationValue = 3789; //make it activate at 92.5%
        }
        else if ((sensorReadRes >= 2458) && (sensorReadRes < 2867))
        {                                    //60-70%
            intensityActivationValue = 3891; //make it activate at 95%
        }
        else
        { //Risky, greater than 70% set it to 98% of max value
            intensityActivationValue = 4014;
        }

        //store it in FlashMemory for next time
        memcpy(storeToMemory, &intensityActivationValue, sizeof(intensityActivationValue));
        flashStorage.write(flashAddress, storeToMemory, sizeof(intensityActivationValue)); //store it in non-volatile space

        return true;
    }
}



void HomeStepper(){
  while(digitalRead(LimDeploy) == HIGH){ 
    Nema17.moveTo(DEPLOYED); 
    Nema17.run();
    DEPLOYED--;
  }
  Nema17.stop();
  while(digitalRead(LimHome) == HIGH)
  {
    Nema17.moveTo(HOME);   
    Nema17.run();
    HOME++;
  }
  Nema17.stop();
  Nema17.setCurrentPosition(0);
  Nema17.moveTo(ReadyPos);
  while(Nema17.distanceToGo() != 0 && digitalRead(LimDeploy) == HIGH){
    Nema17.run();
  }
  Nema17.stop();

}
/*Dispense operates DC MOTOR and STEPPER MOTOR to deploy N number of balls*/
int Dispense(int n){
  for (size_t i = 0; i < n; i++)
  {
    if(Deploy() == 1){
        UnJam(-1);
        if(Attempts >= MaxAttempts){
            return 1;// FAiled to dispesne
        }
        Attempts++;
    }
    i--;
  }
  return 0; //success
}

int UnJam(int DIR){
  switch(DIR){
    case ClOCKWISE:  
        Nema17.move(UnjamAmount);
        while(Nema17.distanceToGo() != 0 && digitalRead(LimDeploy) == HIGH && digitalRead(LimHome) == HIGH){    
          Nema17.run();
        }
        Nema17.stop();
        break; //Start movement into the counterclockwise direction
    case COUNTERClOCKWISE: 
        Nema17.move(-UnjamAmount);
        while(Nema17.distanceToGo() != 0 && digitalRead(LimDeploy) == HIGH && digitalRead(LimHome) == HIGH){    
          Nema17.run();
        }
        Nema17.stop();

        break; //Start movement into the clockwise direction
    default: 
        
        Nema17.move(-UnjamAmount);
        while(Nema17.distanceToGo() != 0 && digitalRead(LimDeploy) == HIGH && digitalRead(LimHome) == HIGH){    
          Nema17.run();
        }
        Nema17.stop();
        Nema17.move(UnjamAmount);
        while(Nema17.distanceToGo() != 0 && digitalRead(LimDeploy) == HIGH && digitalRead(LimHome) == HIGH){    
          Nema17.run();
        }
        Nema17.stop();
        break; //Jiggle movement
  }
  return 0;
}

/*Deploy operates DC MOTOR and STEPPER MOTOR to deploy 1 ball*/
int Deploy(){
  /* Starting at home position 0 step 100 steps to delpoy position then ste*/
  uint8_t Jammed = 3;
  uint8_t JamCount = 0;
  TurnOnDCMotor();
  
  Nema17.moveTo(DelpoyedPos);
  while(Nema17.distanceToGo() != 0 && digitalRead(LimDeploy) == HIGH){    
     if(Nema17.currentPosition() <(ReadyPos - 500)){
        if(CheckJam()){
           UnJam(ClOCKWISE);
           Nema17.moveTo(DelpoyedPos);
           JamCount++;      
        }
        if(JamCount > Jammed){
          return 1;
        }
     }
    Nema17.run();
  }
  Nema17.stop();
  delay(1000);
  TurnOffDCMotor();  
  JamCount = 0;
  Nema17.moveTo(ReadyPos);
  while(Nema17.distanceToGo() != 0 && digitalRead(LimHome) == HIGH)
  {
    if(Nema17.currentPosition() < (DelpoyedPos + 500)){
        if(CheckJam()){
           UnJam(COUNTERClOCKWISE);
           Nema17.moveTo(ReadyPos);
           JamCount++;           
        }
        if(JamCount > Jammed){
          return 1;
        }
     }
    Nema17.run();
  }
  Nema17.stop();
  //delay(1000);
  return 0; //success
}

int CheckJam(){
//  Serial.print("Checking Jam"); 
  int ADCResult = (analogRead(A2) -  analogRead(A1));
//  Serial.print("ADCResult: ");  
//  Serial.print(ADCResult);
  if(JamThreshold < ADCResult){
    if(Jams > JammedDelay){
        Jams = 0;
        return 1;
    }
    else{
      Jams++;
//      Serial.print("Jams: ");        
//      Serial.print(Jams);
      return 0;      
    }
  } 
  else{
      return 0;
  }   
} 

int TurnOffDCMotor(){
  digitalWrite(DCM1, LOW);
  digitalWrite(DCM2, LOW);  
} 

int TurnOnDCMotor(){
  digitalWrite(DCM1, HIGH);
  digitalWrite(DCM2, LOW);  
} 
