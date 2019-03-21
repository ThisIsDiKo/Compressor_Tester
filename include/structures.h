#include <Arduino.h>

enum WORK_CYCLE {
    COLD_START,
    PAUSE_HIGH,
    LOWERING,
    PAUSE_LOW,
    HOT_START,
    LOWERING_TO_COLD,
    COLD_PAUSE,
    LOWERING_TO_START,
    START_PAUSE,
    IDLE_CYCLE,
};

enum SD_STATE {
    SD_OFF,
    SD_ON,
    SD_WRITING,
};

enum KEYS_ENUM {
    KEY_OK,
    KEY_DOWN,
    KEY_UP
};

struct TesterStatus{
  uint8_t sdStat;
  uint8_t curComp;
  uint8_t curStep;
  unsigned long workTime;
  String fileName;
};

struct CompressorInfo{
  uint8_t cycleNum;
  unsigned long cycleStartTime;
  uint8_t compressorPort;
};

struct WorkingData{
  unsigned long lastTimePoint;
  int curSample;
  int filtSample;
  int curCurrentSample;

  unsigned long stepStartTime;
  int stepStartPressure;
  int stepStartCurrent;
  unsigned long stepEndTime;
  int stepEndPressure;
  int stepEndCurrent;
};

struct StepsData{
  int _P0;
  int _P1;
  int _P2;
  unsigned long _Time0;
  unsigned long _Time1;
  unsigned long _CompWorkTime;
  unsigned long _DutyCycleTime;
  uint8_t _NumOfComp;
  uint8_t _StartComp;

  int p0_psi;
  int p1_psi;
  int p2_psi;

  int time0_sec;
  int time1_sec;
  int compWorkTime_minutes;
  int dutyCycleTime_minutes;
  uint8_t fileNumber;
};