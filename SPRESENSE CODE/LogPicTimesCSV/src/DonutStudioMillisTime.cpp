/*
  DonutStudioMillisTime.h - Library to utilize the millis function as a clock
  Created by Donut Studio, March 11, 2024.
  Released into the public domain.
*/

#include "Arduino.h"
#include "DonutStudioMillisTime.h"

/* --- CONSTRUCTOR --- */

MillisTime::MillisTime(int hour, int minute, int second)
{
  refreshDuration();

  if (hour != 0)
    setHour(hour);
  if (minute != 0)
    setMinute(minute);
  if (second != 0)
    setSecond(second);
}


/* --- METHODS --- */
/* MAIN */

void MillisTime::refreshDuration()
{
  unsigned long currentTime = millis();
  _duration = currentTime - _previousDay;

  // check if a new day already started 1day = 24hours = 1.440minutes = 86.400seconds = 86.400.000milliseconds
  if (_duration >= 86400000)
  {
    _duration = _duration % 86400000;
    _previousDay = currentTime - _duration;
  }
}
void MillisTime::resetTime()
{
  _previousDay = millis();
  refreshDuration();
}


/* RELATIVE TIME */

int MillisTime::getHours()
{
  unsigned long seconds = getAdditionTime();
  return (seconds / 3600) % 24;
}
int MillisTime::getHours12()
{
  int h = getHours();
  return h > 12 ? (h - 12) : h;
}
int MillisTime::getMinutes()
{
  unsigned long seconds = getAdditionTime();
  return (seconds / 60) % 60;
}
int MillisTime::getSeconds()
{
  unsigned long seconds = getAdditionTime();
  return seconds % 60;
}

bool MillisTime::isPM()
{
  return getHours() > 12;
}

bool MillisTime::isTime(int hour, int minute, int second)
{
  return isHour(hour) && isMinute(minute) && isSecond(second);
}
bool MillisTime::isHour(int hour)
{
  return getHours() == hour;
}
bool MillisTime::isMinute(int minute)
{
  return getMinutes() == minute;
}
bool MillisTime::isSecond(int second)
{
  return getSeconds() == second;
}


/* --- ABSOLUTE TIME --- */

int MillisTime::getAbsoluteHours()
{
  refreshDuration();
  unsigned long seconds = _duration / 1000;
  return (seconds / 3600) % 24;
}
int MillisTime::getAbsoluteMinutes()
{
  refreshDuration();
  unsigned long seconds = _duration / 1000;
  return (seconds / 60) % 60;
}
int MillisTime::getAbsoluteSeconds()
{
  refreshDuration();
  unsigned long seconds = _duration / 1000;
  return seconds % 60;
}


/* --- SET TIME --- */

void MillisTime::setHour(int hour)
{
  if (hour < 0)
    hour = 23;
  hour = hour % 24;
  int h = getAbsoluteHours();
  setAdditionHour((24 - h) + hour);
}
void MillisTime::setMinute(int minute)
{
  if (minute < 0)
    minute = 59;
  minute = minute % 60;
  int m = getAbsoluteMinutes();
  setAdditionMinute((60 - m) + minute);
}
void MillisTime::setSecond(int second)
{
  if (second < 0)
    second = 59;
  second = second % 60;
  int s = getAbsoluteSeconds();
  setAdditionSecond((60 - s) + second);
}
void MillisTime::resetMilliseconds()
{
  refreshDuration();
  _previousDay += (unsigned long)1000 - (_duration % 1000);
}


/* --- RELATIVE ADDITION --- */

void MillisTime::setAdditionHour(int additionHour)
{
  if (additionHour < 0)
    additionHour = 23;
  _hourAddition = additionHour % 24;
}
int MillisTime::getAdditionHour()
{
  return _hourAddition;
}
void MillisTime::setAdditionMinute(int additionMinute)
{
  if (additionMinute < 0)
    additionMinute = 59;
  _minuteAddition = additionMinute % 60;
}
int MillisTime::getAdditionMinute()
{
  return _minuteAddition;
}
void MillisTime::setAdditionSecond(int additionSecond)
{
  if (additionSecond < 0)
    additionSecond = 59;
  _secondAddition = additionSecond % 60;
}
int MillisTime::getAdditionSecond()
{
  return _secondAddition;
}

unsigned long MillisTime::getAdditionTime()
{
  refreshDuration();
  unsigned long seconds = _duration / 1000;
  seconds += (unsigned long)_secondAddition;
  seconds += (unsigned long)_minuteAddition * 60;
  seconds += (unsigned long)_hourAddition * 3600;
  return seconds;
}


/* --- PHRASED TIME RELATIVE --- */

String MillisTime::getPhrasedTime(bool twelveFormat)
{
  String h = getPhrasedHours(twelveFormat);
  String m = getPhrasedMinutes();
  String s = getPhrasedSeconds();
  return h + ":" + m + ":" + s;
}
String MillisTime::getPhrasedHours(bool twelveFormat)
{
  int h = 0;
  if (twelveFormat)
    h = getHours12();
  else
    h = getHours();
  return h < 10 ? "0" + String(h) : String(h);
}
String MillisTime::getPhrasedMinutes()
{
  int m = getMinutes();
  return m < 10 ? "0" + String(m) : String(m);
}
String MillisTime::getPhrasedSeconds()
{
  int s = getSeconds();
  return s < 10 ? "0" + String(s) : String(s);
}