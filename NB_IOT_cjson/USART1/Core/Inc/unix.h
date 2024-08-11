#ifndef  _UNIX_H_
#define  _UNIX_H_

#include "main.h"

typedef struct {
  uint16_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
} DateTime;

typedef struct {
    char year[5];
    char month[3];
    char day[3];
    char hour[3];
    char minute[3];
    char second[3];
} String_DateTime;

//typedef struct {
//  char* year;
//  char* month;
//  char* day;
//  char* hour;
//  char* minute;
//  char* second;
//} String_DateTime;

uint64_t dateTimeToTimestamp(DateTime dt);

#endif