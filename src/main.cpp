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

const byte  m_pwm_pin = D8;   ///< pin for led
const DateTime m_start_time = DateTime(2000, 1, 1, 8, 10, 0); ///< time when surise start
const uint8_t m_sunrise_time = 30;    ///< duration of sunrise
const uint8_t m_min_in_h = 60;        ///< minutes in an hour
const unsigned long m_refresh_time_ms = 15000;  ///< time of repeting check time is in range and sending message
const uint8_t m_starting_pwm = 0;     ///< starting duty cycle pwm for sunrise
const uint8_t m_pwm_resolution = 255; ///< finish duty cycle pwm for sunrise

/**
 * @brief calculate hours and minutes to only minutes
 * @param time hours and minutes to calculate
 * @return minutes
 */
uint16_t min_calculate(const DateTime& time)
{
  uint16_t time_minute = time.hour() * m_min_in_h;
  time_minute += time.minute();
  return time_minute;
}

/**
 * @brief changing duty cycle pwm output proportionally to sunrise progress
 * @param time_in_min time from sunrise start
 */
void sunrise(const int time_in_min)
{
  auto pwm = map(time_in_min, 0, m_sunrise_time, m_starting_pwm, m_pwm_resolution);
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
  static unsigned long last_loop_time = 0;
  unsigned long loop_time = millis();
  if(loop_time - last_loop_time > m_refresh_time_ms)
  {
    DateTime now = m_rtc.now();
    Serial.print(now.hour(), DEC);
    Serial.print(":");
    Serial.print(now.minute(), DEC);
    Serial.print(":");
    Serial.println(now.second(), DEC);
    last_loop_time = millis();
    auto sunrise_time = min_calculate(now) - min_calculate(m_start_time);
    if((sunrise_time >= 0) && (sunrise_time <= m_sunrise_time ))
    {
      sunrise(sunrise_time);
    }
  }
}