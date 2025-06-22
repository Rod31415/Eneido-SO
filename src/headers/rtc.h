#ifndef RTC_H
#define RTC_H
#include "utilities.h"
#include "functions.h"


struct datetime{
  uint8 sec;
  uint8 min;
  uint8 hour;
  uint8 day;
  uint8 month;
  uint8 year;
};

void printRTC();
void readRTC(datetime *dt);
uint32 time();

#endif
