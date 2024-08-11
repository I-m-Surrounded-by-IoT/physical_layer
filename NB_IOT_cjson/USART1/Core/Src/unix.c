#include "main.h"
#include "unix.h"

uint32_t daysFrom1970(uint16_t year, uint8_t month, uint8_t day) 
{
  uint32_t days = 0;
  for (uint16_t y = 1970; y < year; y++) {
    days += (y % 4 == 0 && (y % 100 != 0 || y % 400 == 0)) ? 366 : 365;
  }

   static const uint8_t daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  for (uint8_t m = 1; m < month; m++) {
    days += daysInMonth[m - 1];
    if (m == 2 && (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))) {
      days++;
    }
  }

  days += day - 1; 

  return days;
}


uint64_t dateTimeToTimestamp(DateTime dt) 
{
  uint32_t days = daysFrom1970(dt.year, dt.month, dt.day);
  uint64_t seconds = days * 86400UL +  
                     (dt.hour) * 3600UL +  
                     dt.minute * 60UL +  
                     dt.second;           

  return seconds;
}