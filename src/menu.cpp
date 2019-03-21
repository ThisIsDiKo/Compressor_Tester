#include "globals.h"
#include "menu.h"
#include "keyboard.h"
#include "structures.h"
#include "useful.h"

#define PSI_INCREMENT       5
#define SECONDS_INCREMENT   1
#define MINUTES_INCREMENT   1

struct menuItem{
  void *Next;
  void *Previous;
  void *Parent;
  void *Child;
  uint8_t Select;
  String Text;
} ;

menuItem* selectedMenuItem;

#define MAKE_MENU(Name, Next, Previous, Parent, Child, Select, Text) \
  extern menuItem Next;     \
  extern menuItem Previous; \
  extern menuItem Parent;   \
  extern menuItem Child;  \
  menuItem Name = {(void*)&Next, (void*)&Previous, (void*)&Parent, (void*)&Child, (uint8_t)Select, Text}

#define PREVIOUS   ((menuItem*)(selectedMenuItem->Previous))
#define NEXT       ((menuItem*)(selectedMenuItem->Next))
#define PARENT     ((menuItem*)(selectedMenuItem->Parent))
#define CHILD      ((menuItem*)(selectedMenuItem->Child))
#define SELECT      (selectedMenuItem->Select)

#define NULL_ENTRY Null_Menu

struct menuItem  Null_Menu = {(void*)0, (void*)0, (void*)0, (void*)0, 0, "0"};

MAKE_MENU(m_stat,  NULL_ENTRY,    NULL_ENTRY,  NULL_ENTRY, m_p0, 1, "Stat");

MAKE_MENU(m_p0,         m_p1,           NULL_ENTRY,   NULL_ENTRY, m_p0Set,          0, "P0");
MAKE_MENU(m_p1,         m_p2,           m_p0,         NULL_ENTRY, m_p1Set,          0, "P1");
MAKE_MENU(m_p2,         m_time1,        m_p1,         NULL_ENTRY, m_p2Set,          0, "P2");
MAKE_MENU(m_time0,      m_time1,        m_p2,         NULL_ENTRY, m_time0Set,       0, "Time0");
MAKE_MENU(m_time1,      m_compTime,     m_time0,      NULL_ENTRY, m_time1Set,       0, "Time1");
MAKE_MENU(m_compTime,   m_dutyCycle,    m_time1,      NULL_ENTRY, m_compTimeSet,    0, "compTime");
MAKE_MENU(m_dutyCycle,  m_compNum,      m_compTime,   NULL_ENTRY, m_dutyCycleSet,   0, "dutyCycle");
MAKE_MENU(m_compNum,    m_compStart,    m_dutyCycle,  NULL_ENTRY, m_compNumSet,     0, "compNum");
MAKE_MENU(m_compStart,  m_back,         m_compNum,    NULL_ENTRY, m_compStartSet,   0, "compStart");
MAKE_MENU(m_back,       NULL_ENTRY,     m_compStart,  NULL_ENTRY, m_stat,           0, "back");

MAKE_MENU(m_p0Set,        NULL_ENTRY,    NULL_ENTRY,  NULL_ENTRY, m_p0,         2, "P0 (psi)");
MAKE_MENU(m_p1Set,        NULL_ENTRY,    NULL_ENTRY,  NULL_ENTRY, m_p1,         3, "P1 (psi)");
MAKE_MENU(m_p2Set,        NULL_ENTRY,    NULL_ENTRY,  NULL_ENTRY, m_p2,         4, "P2 (psi)");
MAKE_MENU(m_time0Set,     NULL_ENTRY,    NULL_ENTRY,  NULL_ENTRY, m_time0,      5, "Time0 (s)");
MAKE_MENU(m_time1Set,     NULL_ENTRY,    NULL_ENTRY,  NULL_ENTRY, m_time1,      6, "Time1 (s)");
MAKE_MENU(m_compTimeSet,  NULL_ENTRY,    NULL_ENTRY,  NULL_ENTRY, m_compTime,   7, "Work (m)");
MAKE_MENU(m_dutyCycleSet, NULL_ENTRY,    NULL_ENTRY,  NULL_ENTRY, m_dutyCycle,  8, "Cycle (m)");
MAKE_MENU(m_compNumSet,   NULL_ENTRY,    NULL_ENTRY,  NULL_ENTRY, m_compNum,    9, "Comp num");
MAKE_MENU(m_compStartSet, NULL_ENTRY,    NULL_ENTRY,  NULL_ENTRY, m_compStart,  10, "Comp start");

extern struct StepsData stepsData;
extern struct TesterStatus testerStatus;
extern struct CompressorInfo compressorInfo[6];
extern Adafruit_SSD1306 display;

extern unsigned long restTime;

void menuChange(struct menuItem* NewMenu){
  if ((void*)NewMenu == (void*)&NULL_ENTRY)
    return;
 
  selectedMenuItem = NewMenu;
}

void init_menu(){
    selectedMenuItem = (menuItem*)&m_stat; 
}

void key_menu(uint8_t key){
  switch(key){
    case KEY_OK:{
      if (SELECT > 1){
        init_values();
        write_to_EEPROM();
      }
      menuChange(CHILD);
      break;
    }
    case KEY_UP:{
      if (SELECT > 1){
        switch(SELECT){
          case 2:{
            stepsData.p0_psi += PSI_INCREMENT;
            if (stepsData.p0_psi > 200) stepsData.p0_psi = 200;
            break;
          }
          case 3:{
            stepsData.p1_psi += PSI_INCREMENT;
            if (stepsData.p1_psi > 200) stepsData.p1_psi = 200;
            break;
          }
          case 4:{
            stepsData.p2_psi += PSI_INCREMENT;
            if (stepsData.p2_psi > 200) stepsData.p2_psi = 200;
            break;
          }
          case 5:{
            stepsData.time0_sec += SECONDS_INCREMENT;
            if (stepsData.time0_sec > 60) stepsData.time0_sec = 60;
            break;
          }
          case 6:{
            stepsData.time1_sec += SECONDS_INCREMENT;
            if (stepsData.time1_sec > 60) stepsData.time0_sec = 60;
            break;
          }
          case 7:{
            stepsData.compWorkTime_minutes += MINUTES_INCREMENT;
            if (stepsData.compWorkTime_minutes > 40) stepsData.compWorkTime_minutes = 40;
            break;
          }
          case 8:{
            stepsData.dutyCycleTime_minutes += MINUTES_INCREMENT;
            if (stepsData.dutyCycleTime_minutes > 90) stepsData.dutyCycleTime_minutes = 90;
            break;
          }
          case 9:{
            stepsData._NumOfComp += 1;
            if (stepsData._NumOfComp > 6) stepsData._NumOfComp = 6;
            break;
          }
          case 10:{
            stepsData._StartComp += 1;
            if (stepsData._StartComp > stepsData._NumOfComp) stepsData._StartComp = stepsData._NumOfComp;
            break;
          }
        }
      }
      else{
        menuChange(PREVIOUS);
      }
      break;
    }
    case KEY_DOWN:{
      if (SELECT > 1){
        switch(SELECT){
          case 2:{
            stepsData.p0_psi -= PSI_INCREMENT;
            if (stepsData.p0_psi < 0) stepsData.p0_psi = 0;
            break;
          }
          case 3:{
            stepsData.p1_psi -= PSI_INCREMENT;
            if (stepsData.p1_psi < 0) stepsData.p1_psi = 0;
            break;
          }
          case 4:{
            stepsData.p2_psi -= PSI_INCREMENT;
            if (stepsData.p2_psi < 0) stepsData.p2_psi = 0;
            break;
          }
          case 5:{
            stepsData.time0_sec -= SECONDS_INCREMENT;
            if (stepsData.time0_sec < 0) stepsData.time0_sec = 0;
            break;
          }
          case 6:{
            stepsData.time1_sec -= SECONDS_INCREMENT;
            if (stepsData.time1_sec < 0) stepsData.time0_sec = 0;
            break;
          }
          case 7:{
            stepsData.compWorkTime_minutes -= MINUTES_INCREMENT;
            if (stepsData.compWorkTime_minutes < 0) stepsData.compWorkTime_minutes = 0;
            break;
          }
          case 8:{
            stepsData.dutyCycleTime_minutes -= MINUTES_INCREMENT;
            if (stepsData.dutyCycleTime_minutes < 0) stepsData.dutyCycleTime_minutes = 0;
            break;
          }
          case 9:{
            stepsData._NumOfComp -= 1;
            if (stepsData._NumOfComp < 1) stepsData._NumOfComp = 1;
            break;
          }
          case 10:{
            stepsData._StartComp -= 1;
            if (stepsData._StartComp < 1) stepsData._StartComp = 1;
            break;
          }
        }
      }
      else{
        menuChange(NEXT);
      }
      
      break;
    }
    default:{
      break;
    }
  }

  display_menu();
  return;
}

void display_menu(){
  menuItem* tempMenu;
  //Serial.println("menu draw");
  if (SELECT == 1){
    //Serial.println("Stat Menu draw");
    draw_status_screen();
  }
  else if (SELECT == 0){
    //Serial.println("1st child draw");
    tempMenu = selectedMenuItem;
    
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextColor(WHITE);
    display.setTextSize(2);

    tempMenu = (menuItem*)(selectedMenuItem->Previous);
    if ((void*)tempMenu != (void*)&NULL_ENTRY){
      display.println(tempMenu->Text); 
    }
    else{
      display.println(); 
    }

    if ((void*)selectedMenuItem != (void*)&NULL_ENTRY){
      display.setTextColor(BLACK, WHITE); // Draw 'inverse' text
      display.println(selectedMenuItem->Text); 
    }
    else{
      display.println(); 
    }

    tempMenu = (menuItem*)(selectedMenuItem->Next);
    if ((void*)tempMenu != (void*)&NULL_ENTRY){
      
      display.setTextColor(WHITE);
      display.println(tempMenu->Text); 
    }
    else{
      display.println(); 
    }
  }
  else if (SELECT > 1){
    //Serial.println("2nd child draw");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextColor(WHITE);
    display.setTextSize(2);

    if ((void*)selectedMenuItem != (void*)&NULL_ENTRY){
      display.println(selectedMenuItem->Text); 
    }
    else{
      display.println(); 
    }

    switch(SELECT){
      case 2:{
        display.setCursor(40, 40);
        display.print(stepsData.p0_psi);
        break;
      }
      case 3:{
        display.setCursor(40, 40);
        display.print(stepsData.p1_psi);
        break;
      }
      case 4:{
        display.setCursor(40, 40);
        display.print(stepsData.p2_psi);
        break;
      }
      case 5:{
        display.setCursor(40, 40);
        display.print(stepsData.time0_sec);
        break;
      }
      case 6:{
        display.setCursor(40, 40);
        display.print(stepsData.time1_sec);
        break;
      }
      case 7:{
        display.setCursor(40, 40);
        display.print(stepsData.compWorkTime_minutes);
        break;
      }
      case 8:{
        display.setCursor(40, 40);
        display.print(stepsData.dutyCycleTime_minutes);
        break;
      }
      case 9:{
        display.setCursor(40, 40);
        display.print(stepsData._NumOfComp);
        break;
      }
      case 10:{
        display.setCursor(40, 40);
        display.print(stepsData._StartComp);
        break;
      }
    }
  }
  display.display();
}

void draw_status_screen(){
  String message = "";
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextColor(WHITE);
  display.setTextSize(2);
  
  message = "SD ";
  switch(testerStatus.sdStat){
    case 0:
      message += "OFF";
      break;
    case 1:
      message += "ON";
      break;
    case 2:
      message += "WRITING";
      break;
  }
  display.println(message);

  message = "Comp: ";
  message += String(testerStatus.curComp+1);
  display.println(message);

  message = "Cycle: ";
  message += String(compressorInfo[testerStatus.curComp].cycleNum);
  display.println(message);

  display.setTextSize(1);
  switch(testerStatus.curStep){
    case COLD_START:
      message = "CS";
      break;
    case PAUSE_HIGH:
      message = "PH";
      break;
    case LOWERING:
      message = "LO";
      break;
    case PAUSE_LOW:
      message = "PL";
      break;
    case HOT_START:
      message = "HS";
      break;
    case LOWERING_TO_COLD:
      message = "LTC";
      break;
    case COLD_PAUSE:
      message = "CP";
      break;
    case LOWERING_TO_START:
      message = "LTS";
      break;
    case START_PAUSE:
      message = "SP";
      break;
    case IDLE_CYCLE:
      message = "IDLE";
      break;
    
  }
  
  display.print(message);
  message = " ";
  message += String(testerStatus.workTime / 1000);
  message += " ";
  message += String((stepsData._DutyCycleTime - restTime) / 1000);
  display.println(message);
  display.println(testerStatus.fileName);
  display.display();
}