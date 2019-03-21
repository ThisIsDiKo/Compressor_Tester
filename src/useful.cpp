#include "useful.h"
#include "globals.h"
#include "structures.h"

#define FILENUMBER_ADDR   0
#define P0_PSI_ADDR       1
#define P1_PSI_ADDR       3
#define P2_PSI_ADDR       5
#define T0_SEC_ADDR       7
#define T1_SEC_ADDR       9
#define COMPWORK_ADDR     11
#define DUTYCYCLE_ADDR    13
#define COMPNUM_ADDR      15
#define COMPSTART_ADDR    16

extern struct StepsData stepsData;
extern struct CompressorInfo compressorInfo[6];

int psi_to_adc(int pressure_psi){
  int pressure_adc = 0;
  pressure_adc = 5 * pressure_psi + 245;
  return pressure_adc;
}

unsigned long sec_to_millis(int sec){
  return (unsigned long) sec * 1000;
}

unsigned long minutes_to_millis(int minute){
  return (unsigned long) minute * 60 *1000;
}

//Чтение данных из памяти
void read_from_EEPROM(){
  EEPROM.get(FILENUMBER_ADDR, stepsData.fileNumber);
  
  EEPROM.get(P0_PSI_ADDR, stepsData.p0_psi);
  EEPROM.get(P1_PSI_ADDR, stepsData.p1_psi);
  EEPROM.get(P2_PSI_ADDR, stepsData.p2_psi);

  EEPROM.get(T0_SEC_ADDR, stepsData.time0_sec);
  EEPROM.get(T1_SEC_ADDR, stepsData.time1_sec);

  EEPROM.get(COMPWORK_ADDR, stepsData.compWorkTime_minutes);
  EEPROM.get(DUTYCYCLE_ADDR, stepsData.dutyCycleTime_minutes);

  EEPROM.get(COMPNUM_ADDR, stepsData._NumOfComp);
  EEPROM.get(COMPSTART_ADDR, stepsData._StartComp);
}

//Запись данных в память
void write_to_EEPROM(){
  EEPROM.put(FILENUMBER_ADDR, stepsData.fileNumber);
  
  EEPROM.put(P0_PSI_ADDR, stepsData.p0_psi);
  EEPROM.put(P1_PSI_ADDR, stepsData.p1_psi);
  EEPROM.put(P2_PSI_ADDR, stepsData.p2_psi);

  EEPROM.put(T0_SEC_ADDR, stepsData.time0_sec);
  EEPROM.put(T1_SEC_ADDR, stepsData.time1_sec);

  EEPROM.put(COMPWORK_ADDR, stepsData.compWorkTime_minutes);
  EEPROM.put(DUTYCYCLE_ADDR, stepsData.dutyCycleTime_minutes);

  EEPROM.put(COMPNUM_ADDR, stepsData._NumOfComp);
  EEPROM.put(COMPSTART_ADDR, stepsData._StartComp);
}

void init_values(){
  stepsData._P0 = psi_to_adc(stepsData.p0_psi);
  stepsData._P1 = psi_to_adc(stepsData.p1_psi);
  stepsData._P2 = psi_to_adc(stepsData.p2_psi);

  stepsData._Time0 = sec_to_millis(stepsData.time0_sec);
  stepsData._Time1 = sec_to_millis(stepsData.time1_sec);
  stepsData._CompWorkTime = minutes_to_millis(stepsData.compWorkTime_minutes);
  stepsData._DutyCycleTime = minutes_to_millis(stepsData.dutyCycleTime_minutes);

  Serial.println("-----init values-----");
  Serial.println(stepsData._P0);
  Serial.println(stepsData._P1);
  Serial.println(stepsData._P2);
  Serial.println(stepsData._Time0);
  Serial.println(stepsData._Time1);
  Serial.println(stepsData._CompWorkTime);
  Serial.println(stepsData._DutyCycleTime);
  Serial.println(stepsData._NumOfComp);
  Serial.println(stepsData._StartComp);
  Serial.println("---------------------");
}

void init_compressors(){
  byte i = 0;
  byte compressorPorts[6] = COMP_PORTS;
  for(i = 0; i < 6; i++){
    compressorInfo[i].cycleNum = 0;
    compressorInfo[i].cycleStartTime = 0;
    compressorInfo[i].compressorPort = compressorPorts[i];
    pinMode(compressorPorts[i], OUTPUT);
  }
}

