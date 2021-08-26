/**
 * @file main.cpp
 * @brief sunrise effect for RTC and LED(PWM)
 * @author by Szymon Markiewicz
 * @details http://www.inzynierdomu.pl/
 * @date 07-2021
 */

#include <Arduino.h>
#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 m_rtc; ///< DS1307 RTC

const byte  m_pwm_pin = D8;
const DateTime m_start_time = DateTime(2000, 1, 1, 23, 44, 0); ///< time when realy turn on
const int m_sunrise_time = 30;
const int m_min_in_h = 60;            ///< minutes in an hour
const int m_refresh_time_ms = 15000;  ///< time of repeting check time is in range and sending message
const int m_starting_pwm = 1;

/**
 * @brief calculate hours and minutes to only minutes
 * @param time hours and minutes to calculate
 * @return minutes
 */
int min_calculate(const DateTime& current)
{
  int current_minute = current.hour() * m_min_in_h;
  current_minute += current.minute();
  return current_minute;
}

/**
 * @brief 
 * @param time_in_min time from sunrise start
 */
void sunrise(const int time_in_min)
{
  auto pwm = map(time_in_min, 0, m_sunrise_time, m_starting_pwm, 255);
  analogWrite(m_pwm_pin, pwm);
  Serial.println(pwm);
}

/**
 * @brief setup
 */
void setup() 
{
  Wire.begin();
  Serial.begin(9600);  

  if (!m_rtc.begin()) 
  {
    Serial.println("Couldn't find RTC");
  }

  if (!m_rtc.isrunning())
  {
    Serial.println("RTC is NOT running, let's set the time!");    
  }

  m_rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  analogWrite(m_pwm_pin, 0);
}

/**
 * @brief main loop
 */
void loop() 
{  
  static long last_loop_time = 0;
  long loop_time = millis();
  if(loop_time - last_loop_time > m_refresh_time_ms)
  {
    DateTime time = m_rtc.now();
    Serial.print(time.hour(), DEC);
    Serial.print(":");
    Serial.print(time.minute(), DEC);
    Serial.print(":");
    Serial.println(time.second(), DEC);
    last_loop_time = millis();
    auto sunrise_time = min_calculate(time) - min_calculate(m_start_time);
    if((sunrise_time >= 0) && (sunrise_time <= m_sunrise_time ))
    {
      sunrise(sunrise_time);
    }
  }
}