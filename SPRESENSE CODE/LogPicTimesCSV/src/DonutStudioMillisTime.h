/*
  DonutStudioMillisTime.h - Library to utilize the millis function as a clock
  Created by Donut Studio, March 11, 2024.
  Released into the public domain.
*/

#ifndef DonutStudioMillisTime_h
#define DonutStudioMillisTime_h

#include "Arduino.h"


class MillisTime
{
  /* --- PUBLIC --- */
  public:
    /* --- CONSTRUCTOR --- */

    // constructor for the class with the relative time to start from, default=12h:00m:00s
    MillisTime(int hour = 12, int minute = 0, int second = 0);


    /* --- METHODS --- */
    /* MAIN */

    // recalculate the current duration between the last day and current time
    void refreshDuration();
    // resets the time to 00h:00m:00s and restarts to count
    void resetTime();


    /* RELATIVE TIME */

    // get the relative hours(0-23)
    int getHours();
    // get the relative hours(0-12)
    int getHours12();
    // get the relative minutes(0-59)
    int getMinutes();
    // get the relative seconds(0-59)
    int getSeconds();

    // returns true if the relative time is PM
    bool isPM();
    // check if the relative time is equal to the given time
    bool isTime(int hour, int minute, int second);
    // check if the relative hour is equal to the given hour
    bool isHour(int hour);
    // check if the relative minute is equal to the given minute
    bool isMinute(int minute);
    // check if the relative second is equal to the given second
    bool isSecond(int second);


    /* ABSOLUTE TIME */

    // get the absolute hours(0-23)
    int getAbsoluteHours();
    // get the absolute minutes(0-59)
    int getAbsoluteMinutes();
    // get the absolute seconds(0-59)
    int getAbsoluteSeconds();


    /* SET TIME */

    // set the hour
    void setHour(int hour);
    // set the minute
    void setMinute(int minute);
    // set the second
    void setSecond(int second);
    // set the milliseconds of the relative time back to 0
    void resetMilliseconds();


    /* RELATIVE ADDITION */

    // set the hour addition (for relative time)
    void setAdditionHour(int additionHour);
    // get the hour addition (for relative time)
    int getAdditionHour();
    // set the minute addition (for relative time)
    void setAdditionMinute(int additionMinute);
    // get the minute addition (for relative time)
    int getAdditionMinute();
    // set the second addition (for relative time)
    void setAdditionSecond(int additionSecond);
    // get the second addition (for relative time)
    int getAdditionSecond();
    // get the relative time(in seconds) by adding the additions onto the absolute time
    unsigned long getAdditionTime();


    /* PHRASED TIME RELATIVE */

    // returns the current time phrased into the following format: hh:mm:ss
    String getPhrasedTime(bool twelveFormat = false);
    // returns the current hours phrased into the following format: hh
    String getPhrasedHours(bool twelveFormat = false);
    // returns the current minutes phrased into the following format: mm
    String getPhrasedMinutes();
    // returns the current seconds phrased into the following format: ss
    String getPhrasedSeconds();


  /* --- PRIVATE --- */
  private:
    /* --- VARIABLES --- */
    unsigned long _duration = 0;
    unsigned long _previousDay = 0;

    int _hourAddition = 0; // save storage: change to byte
    int _minuteAddition = 0; // save storage: change to byte
    int _secondAddition = 0; // save storage: change to byte
};
#endif