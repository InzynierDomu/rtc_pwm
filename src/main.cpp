/**
 * @file main.cpp
 * @brief sunrise effect for RTC and LED(PWM)
 * @author by Szymon Markiewicz
 * @details http://www.inzynierdomu.pl/
 * @date 07-2021
 */

#include "RTClib.h"

#include <Arduino.h>

RTC_DS1307 m_rtc; ///< DS1307 RTC

const byte m_pwm_pin = 6; ///< pin for led
const uint8_t m_starting_pwm = 0; ///< starting duty cycle pwm for sunrise
const uint8_t m_pwm_resolution = 255; ///< finish duty cycle pwm for sunrise

const DateTime m_sunrise_time = DateTime(2000, 1, 1, 18, 5, 0); ///< time when surise start
const DateTime m_sunset_time = DateTime(2000, 1, 1, 23, 57, 0); ///< time when sunset start
const uint8_t m_duration = 31; ///< duration of sunrise or sunset
const uint8_t m_min_in_h = 60; ///< minutes in an hour
const unsigned long m_refresh_time_ms = 5000; ///< time of repeting check time is in range and sending message


uint8_t lookup_table[] = {0,  1,  2,  3,  4,  5,  7,   9,   12,  15,  18,  22,  27,  32,  38,  44,
                          51, 58, 67, 76, 86, 96, 108, 120, 134, 148, 163, 180, 197, 216, 235, 255};

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

uint8_t map_on_lookup_table(uint8_t time)
{
  return lookup_table[time];
}

/**
 * @brief changing duty cycle pwm output proportionally to sunrise progress
 * @param time_in_min time from sunrise start
 */
void sunrise(const int time_in_min)
{
  auto pwm = map_on_lookup_table(time_in_min);
  analogWrite(m_pwm_pin, pwm);
  Serial.println(pwm);
}

/**
 * @brief changing duty cycle pwm output proportionally to sunset progress
 * @param time_in_min time from sunset start
 */
void sunset(const int time_in_min)
{
  auto pwm = map_on_lookup_table(time_in_min);
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

#ifdef DEBUG
  if (!m_rtc.isrunning())
  {
    Serial.println("RTC is NOT running");
  }
#endif

  m_rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  DateTime now = m_rtc.now();
  if ((min_calculate(now) > min_calculate(m_sunrise_time)) && (min_calculate(now) < min_calculate(m_sunset_time)))
  {
    analogWrite(m_pwm_pin, m_pwm_resolution);
  }
  else
  {
    analogWrite(m_pwm_pin, m_starting_pwm);
  }
}

/**
 * @brief main loop
 */
void loop()
{
  static unsigned long last_loop_time = 0;
  unsigned long loop_time = millis();


  if (loop_time - last_loop_time > m_refresh_time_ms)
  {
    DateTime now = m_rtc.now();
    Serial.print(now.hour(), DEC);
    Serial.print(":");
    Serial.print(now.minute(), DEC);
    Serial.print(":");
    Serial.println(now.second(), DEC);
    last_loop_time = millis();

    if ((min_calculate(now) >= min_calculate(m_sunrise_time)) && (min_calculate(now) <= (min_calculate(m_sunrise_time) + m_duration)))
    {
      sunrise(min_calculate(now) - min_calculate(m_sunrise_time));
      Serial.println("sunrise");
    }
    else if ((min_calculate(now) >= min_calculate(m_sunset_time)) && (min_calculate(now) <= (min_calculate(m_sunset_time) + m_duration)))
    {
      sunset(min_calculate(now) - min_calculate(m_sunset_time));
      Serial.println("sunset");
    }
  }
}