#include "globals.h"
#include "compressorTester.h"
#include "structures.h"

extern struct TesterStatus testerStatus;
extern struct StepsData stepsData;
extern struct CompressorInfo compressorInfo[6];
extern struct WorkingData workingData;

//Hello github


extern File dataFile;

char message[128] = {0};
byte i = 0;

void cs_handler(){
  if (workingData.filtSample < stepsData._P1){
    digitalWrite(compressorInfo[testerStatus.curComp].compressorPort, HIGH);
  }
  else{
    digitalWrite(compressorInfo[testerStatus.curComp].compressorPort, LOW);
    compressorInfo[testerStatus.curComp].cycleNum += 1;

    workingData.stepEndTime = millis() - compressorInfo[testerStatus.curComp].cycleStartTime;
    workingData.stepEndPressure = workingData.filtSample;
    workingData.stepEndCurrent = workingData.curCurrentSample;

    sprintf(message, "Comp # %d", testerStatus.curComp+1);
    if (dataFile) {
      dataFile.println(message);
    }
    sprintf(message, "Cyle # %d", compressorInfo[testerStatus.curComp].cycleNum);
    if (dataFile) {
      dataFile.println(message);
    }
    Serial.println(message);
    
    /*
     * cs,csStartTime,csStartPress,csStartVoltage,csEndTime,csEndPress,csEndVoltage\n
     */
    sprintf(message, "cs,%ld,%d,%d,%ld,%d,%d\n", workingData.stepStartTime,
                                                 workingData.stepStartPressure,
                                                 workingData.stepStartCurrent,
                                                 workingData.stepEndTime,
                                                 workingData.stepEndPressure,
                                                 workingData.stepEndCurrent);
    if (dataFile) {
      dataFile.println(message);
    }
    // dataFile.close();
    // dataFile = SD.open(testerStatus.fileName, FILE_WRITE);

    Serial.print(message);
    
    workingData.stepStartTime = workingData.stepEndTime;
    workingData.stepStartPressure = workingData.stepEndPressure;
    
    workingData.lastTimePoint = millis();
    testerStatus.curStep = PAUSE_HIGH;

    Serial.println("Pause High");
  }
}

void pause_high_handler(){
  if (millis() - workingData.lastTimePoint > stepsData._Time0){
    workingData.stepEndTime = millis() - compressorInfo[testerStatus.curComp].cycleStartTime;
    workingData.stepEndPressure = workingData.filtSample;
    /*
     * ph,pauseStartTime,pauseStartPress,pauseEndTime,pauseEndPress\n
     */
    sprintf(message, "ph,%ld,%d,%ld,%d\n",  workingData.stepStartTime,
                                            workingData.stepStartPressure,
                                            workingData.stepEndTime,
                                            workingData.stepEndPressure);
    if (dataFile) {
      dataFile.println(message);
    }
    // dataFile.close();
    // dataFile = SD.open(testerStatus.fileName, FILE_WRITE);

    Serial.print(message);
    Serial.print("Comp: ");
    Serial.println(testerStatus.workTime);

    workingData.stepStartTime = workingData.stepEndTime;
    workingData.stepStartPressure = workingData.stepEndPressure;
    
    workingData.lastTimePoint = millis();
    testerStatus.curStep = LOWERING;
    Serial.println("Lowering");
  }
}

void lowering_handler(){
  if (workingData.filtSample > stepsData._P2){
    digitalWrite(VALVE_PORT, HIGH);
  }
  else{
    digitalWrite(VALVE_PORT, LOW);
    workingData.stepEndTime = millis() - compressorInfo[testerStatus.curComp].cycleStartTime;
    workingData.stepEndPressure = workingData.filtSample;
    
    /*
     * lo,loweringStartTime,loweringStartPress,loweringEndTime,loweringEndPress\n
     */
    sprintf(message, "lo,%ld,%d,%ld,%d\n",  workingData.stepStartTime,
                                            workingData.stepStartPressure,
                                            workingData.stepEndTime,
                                            workingData.stepEndPressure);
    if (dataFile) {
      dataFile.println(message);
    }
    // dataFile.close();
    // dataFile = SD.open(testerStatus.fileName, FILE_WRITE);
    Serial.print(message);

    workingData.stepStartTime = workingData.stepEndTime;
    workingData.stepStartPressure = workingData.stepEndPressure;
    
    workingData.lastTimePoint = millis();
    testerStatus.curStep = PAUSE_LOW;
    Serial.println("Pause Low");
  }
}

void pause_low_handler(){
  if (millis() - workingData.lastTimePoint > stepsData._Time1){
    workingData.stepEndTime = millis() - compressorInfo[testerStatus.curComp].cycleStartTime;
    workingData.stepEndPressure = workingData.filtSample;
    /*
     * pl,pauseStartTime,pauseStartPress,pauseEndTime,pauseEndPress\n
     */
    sprintf(message, "pl,%ld,%d,%ld,%d\n",  workingData.stepStartTime,
                                            workingData.stepStartPressure,
                                            workingData.stepEndTime,
                                            workingData.stepEndPressure);
    if (dataFile) {
      dataFile.println(message);
    }
    // dataFile.close();
    // dataFile = SD.open(testerStatus.fileName, FILE_WRITE);
    Serial.print(message);
    Serial.print("Comp: ");
    Serial.println(testerStatus.workTime);

    workingData.stepStartTime = workingData.stepEndTime;
    workingData.stepStartPressure = workingData.stepEndPressure;
    workingData.stepStartCurrent = workingData.curCurrentSample;
    
    workingData.lastTimePoint = millis();

    testerStatus.curStep = HOT_START;
    Serial.println("Hot Start");
  }
}

void hs_handler(){
  if (workingData.filtSample < stepsData._P1){
    digitalWrite(compressorInfo[testerStatus.curComp].compressorPort, HIGH);
  }
  else{
    digitalWrite(compressorInfo[testerStatus.curComp].compressorPort, LOW);
    workingData.stepEndTime = millis() - compressorInfo[testerStatus.curComp].cycleStartTime;
    workingData.stepEndPressure = workingData.filtSample;
    workingData.stepEndCurrent = workingData.curCurrentSample;

    /*
     * hs,hsStartTime,hsStartPress,hsStartVoltage,hsEndTime,hsEndPress,hsEndVoltage\n
     */
    sprintf(message, "hs,%ld,%d,%d,%ld,%d,%d\n", workingData.stepStartTime,
                                                 workingData.stepStartPressure,
                                                 workingData.stepStartCurrent,
                                                 workingData.stepEndTime,
                                                 workingData.stepEndPressure,
                                                 workingData.stepEndCurrent);
    if (dataFile) {
      dataFile.println(message);
    }
    // dataFile.close();
    // dataFile = SD.open(testerStatus.fileName, FILE_WRITE);
    Serial.print(message);
    
    workingData.stepStartTime = workingData.stepEndTime;
    workingData.stepStartPressure = workingData.stepEndPressure;
    
    workingData.lastTimePoint = millis();
    testerStatus.curStep = PAUSE_HIGH;
    Serial.println("Pause High");
  }
}

void lowering_to_cold_handler(){
  if (workingData.curSample > stepsData._P0){
    digitalWrite(VALVE_PORT, HIGH);
  }
  else{
    digitalWrite(VALVE_PORT, LOW);

    workingData.lastTimePoint = millis();
    testerStatus.curStep = COLD_PAUSE;
    Serial.println("Pause Cold");
  }
}

void cold_pause_handler(){
  uint8_t i = 0;
  uint8_t j = 0;
  for(i = 0; i < stepsData._NumOfComp; i++){
    j = (stepsData._StartComp - 1 + i) % stepsData._NumOfComp;
    Serial.print("i: ");
    Serial.print(j);
    Serial.print(" ");
    Serial.println(compressorInfo[j].cycleStartTime); 
    if (compressorInfo[j].cycleStartTime == 0){
      testerStatus.curComp = j;
      testerStatus.curStep = START_PAUSE;
      return;
    }
  }

  for(i = 0; i < stepsData._NumOfComp; i++){
    j = (stepsData._StartComp - 1 + i % stepsData._NumOfComp);
    if (millis() - compressorInfo[j].cycleStartTime > stepsData._DutyCycleTime){
      testerStatus.curComp = j;
      testerStatus.curStep = START_PAUSE;
      return;
    }
  }
}

void lowering_to_start_handler(){
  if (workingData.curSample > stepsData._P0){
    //Serial.println("lowering");
    digitalWrite(VALVE_PORT, HIGH);
  }
  else{
    digitalWrite(VALVE_PORT, LOW);

    workingData.lastTimePoint = millis();
    testerStatus.curStep = START_PAUSE;
    Serial.println("Pause Start");
  }
}

void start_pause_handler(){
    if (millis() - workingData.lastTimePoint  > 3000){

    workingData.stepStartTime = 0;
    workingData.stepStartPressure = workingData.filtSample;
    workingData.stepStartCurrent = workingData.curCurrentSample;
    
    testerStatus.curStep = COLD_START;
    Serial.println("Cold Start");
    compressorInfo[testerStatus.curComp].cycleStartTime = millis();

    for(byte i = 0; i < stepsData._NumOfComp; i++){
        Serial.print(compressorInfo[i].cycleStartTime);
        Serial.print(" ");
    }

    Serial.println();
  }
}

void tester_state_machine(){
  switch(testerStatus.curStep){
    case COLD_START:
      cs_handler();
      break;
    case PAUSE_HIGH:
   // update_display();
      pause_high_handler();
      break;
    case LOWERING:
    //update_display();
      lowering_handler();
      break;
    case PAUSE_LOW:
    //update_display();
      pause_low_handler();
      break;
    case HOT_START:
    //update_display();
      hs_handler();
      break;
    case LOWERING_TO_COLD:
    //update_display();
      lowering_to_cold_handler();
      break;
    case COLD_PAUSE:
    //update_display();
      cold_pause_handler();
      break;
    case LOWERING_TO_START:
    //update_display();
      lowering_to_start_handler();
      break;
    case START_PAUSE:
    //update_display();
      start_pause_handler();
      break;
    case IDLE_CYCLE:
      for (i = 0; i < 6; i++){
        digitalWrite(compressorInfo[i].compressorPort, LOW);
      }
      digitalWrite(VALVE_PORT, LOW);
      break;
  }
}

int fir_filter(int *s, int sample){
  uint32_t filteredSample = 0;
  
//  uint32_t FIRCoef[10] = {
//           10200,
//            12611,
//            14637,
//            16006,
//            16492,
//            16006,
//            14637,
//            12611,
//            10200,
//             7667
//        };
//  uint32_t DCgain = 131072;
 int32_t FIRCoef[10] = {
           -1800,
         3259,
        -4640,
         5632,
        27319,
         5632,
        -4640,
         3259,
        -1800,
          545
        };
  int32_t DCgain = 32768;
  
  uint8_t i = 0;

  for (i = 10 - 1; i> 0; i--){
    s[i] = s[i-1];
  }

  s[0] = sample;
  filteredSample = 0;

  for (i = 0 ; i < 10; i++){
    filteredSample += FIRCoef[i] * (int32_t)s[i];
  }
  filteredSample = filteredSample / DCgain;

  return (int) filteredSample;
}