#include "globals.h"
#include "structures.h"
#include "useful.h"
#include "menu.h"
#include "keyboard.h"
#include "compressorTester.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, 12);

struct TesterStatus testerStatus = {0, 0, 0, 0, "NULL"};
struct WorkingData workingData;
struct StepsData stepsData;
struct CompressorInfo compressorInfo[6];


unsigned long lastDisplayUpdatedTime = 0;
unsigned long lastSampleTime = 0;
unsigned long compOnTime = 0;
unsigned long coldPauseTimer = 0;

unsigned long restTime = 0;

int samples[10] = {800};

//byte curComp = 0;


int counterPause = 0;

File dataFile;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  if (!SD.begin(4)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    //return;
    testerStatus.sdStat = SD_OFF;
  }
  else{
    Serial.println("card initialized.");
    testerStatus.sdStat = SD_ON;
  }

  pinMode(PRESSURE_SENS, INPUT);
  pinMode(CURRENT_SENS, INPUT);
  
  pinMode(BTN_START, INPUT);
  pinMode(BTN_UP, INPUT);
  pinMode(BTN_DOWN, INPUT);
  pinMode(BTN_OK, INPUT);
  pinMode(VALVE_PORT, OUTPUT);
  init_compressors();

  read_from_EEPROM();
  init_values();

  testerStatus.curStep = IDLE_CYCLE;

  display.clearDisplay();
  display.setTextSize(2);             // Draw 2X-scale text
  display.setCursor(0,20);   
  display.setTextColor(WHITE);
  display.println(F("Compressor Tester"));
  display.display();
  delay(2000);

  init_menu();
  display_menu();


}

void loop() {
  // put your main code here, to run repeatedly:
  keyboard_scan();
  tester_state_machine();
  
  if (millis() - lastSampleTime > 24){
    if (testerStatus.curStep == COLD_START || testerStatus.curStep == HOT_START){
      testerStatus.workTime += (millis() - lastSampleTime);

      if (testerStatus.workTime > stepsData._CompWorkTime){
        for (byte i = 0; i < 6; i++){
          digitalWrite(compressorInfo[i].compressorPort, LOW);
        }
        digitalWrite(VALVE_PORT, LOW);
        testerStatus.curStep = LOWERING_TO_COLD;
        testerStatus.workTime = 0;
      }
    }
    workingData.curSample = analogRead(PRESSURE_SENS);
    workingData.curCurrentSample = analogRead(CURRENT_SENS);
    workingData.filtSample = fir_filter(samples, workingData.curSample);
    lastSampleTime = millis();
    //Serial.println(workingData.curSample);
  }
  
  if (testerStatus.curStep == COLD_PAUSE){
    if(millis() - coldPauseTimer > 60000){
      restTime = 0;
      for(byte i = 0; i < stepsData._NumOfComp; i++){
        if (millis() - compressorInfo[i].cycleStartTime > restTime) restTime = millis() - compressorInfo[i].cycleStartTime;
        Serial.print(compressorInfo[i].cycleStartTime);
        Serial.print("-");
        Serial.print(millis() - compressorInfo[i].cycleStartTime);
        Serial.print(" ");
      }
      Serial.println();
      coldPauseTimer = millis();
    }
  }

  if (millis() - lastDisplayUpdatedTime > 1000){
    display_menu();
    lastDisplayUpdatedTime = millis();
  }
}