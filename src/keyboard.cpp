#include "globals.h"
#include "keyboard.h"
#include "menu.h"
#include "structures.h"
#include "useful.h"

extern struct TesterStatus testerStatus;
extern struct StepsData stepsData;

extern File dataFile;

boolean prevBtnOk = false;
boolean prevBtnUp = false;
boolean prevBtnDown = false;
boolean prevBtnStart = false;

void btnOk_clicked(){
  key_menu(KEY_OK);
}

void btnUp_clicked(){
  key_menu(KEY_UP);
}

void btnDown_clicked(){
  key_menu(KEY_DOWN);
}

void btnStart_clicked(){
  if (testerStatus.sdStat == SD_ON){
          testerStatus.sdStat = SD_WRITING;
          
          testerStatus.fileName = "dlog_";
          testerStatus.fileName += String(stepsData.fileNumber);
          testerStatus.fileName += ".txt";

          stepsData.fileNumber++;
          EEPROM.put(0, stepsData.fileNumber);

          init_compressors();

          init_menu();
          
          testerStatus.curComp = stepsData._StartComp - 1;
          //testerStatus.curComp = curComp + 1;
          
          dataFile = SD.open(testerStatus.fileName, FILE_WRITE);
          
          testerStatus.curStep = LOWERING_TO_START;
        }
        else if(testerStatus.sdStat == SD_WRITING){
          testerStatus.sdStat = SD_ON;
          dataFile.close();
          testerStatus.curStep = IDLE_CYCLE;
        }
}

void keyboard_scan(){
  if (testerStatus.sdStat != SD_WRITING){
    if (digitalRead(BTN_OK) != prevBtnOk){
      delayMicroseconds(300);
      if (digitalRead(BTN_OK) != prevBtnOk){
        prevBtnOk = digitalRead(BTN_OK);
        if (prevBtnOk){
          btnOk_clicked();
        }
      }
    }
  
    if (digitalRead(BTN_UP) != prevBtnUp){
      delayMicroseconds(300);
      if (digitalRead(BTN_UP) != prevBtnUp){
        prevBtnUp = digitalRead(BTN_UP);
        if (prevBtnUp){
          btnUp_clicked();
        }
      }
    }
  
    if (digitalRead(BTN_DOWN) != prevBtnDown){
      delayMicroseconds(300);
      if (digitalRead(BTN_DOWN) != prevBtnDown){
        prevBtnDown = digitalRead(BTN_DOWN);
        if (prevBtnDown){
          btnDown_clicked();
        }
      }
    }
  }
  

  if (digitalRead(BTN_START) != prevBtnStart){
    delayMicroseconds(300);
    if (digitalRead(BTN_START) != prevBtnStart){
      prevBtnStart = digitalRead(BTN_START);
      if (prevBtnStart){
        btnStart_clicked();
      }
    }
  }
}