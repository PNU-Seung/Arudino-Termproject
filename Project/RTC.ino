#include <RTClib.h>

RTC_DS1307 RTC;
DateTime now;
char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};



void setRTC() {
  RTC.begin();
  if (!RTC.isrunning()) {
    Serial.println("Adjusting time...");
    RTC.adjust(DateTime("Jun 25 2019", "10:00:00"));
  }
  now = RTC.now();
  printTime();
}



inline bool isNighttime() {
  if (now.hour() > 23 || now.hour() < 6)
    return true;
  else
    return false;
}

void printTime()
{
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.print(" (");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.println(')');
}
