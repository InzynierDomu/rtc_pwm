# rtc_pwm
 Sunrsie effect with RTC and LED(PWM)
* [About](#About)
* [Scheme](#Scheme)
* [IDE](#IDE)
## About
To config project for own needs You should change start sunrise(in example 8:10):
```cpp
const DateTime m_start_time = DateTime(2000, 1, 1, 8, 10, 0); ///< time when surise start
```
and
```cpp
const uint8_t m_sunrise_time = 30;    ///< duration of sunrise
```

If you have different resolution than 8bits (0-255) or You don't work with whole range, You need to change:
```cpp
const uint8_t m_starting_pwm = 0;     ///< starting duty cycle pwm for sunrise
const uint8_t m_pwm_resolution = 255; ///< finish duty cycle pwm for sunrise
```
## Scheme

## IDE
The project is prepared for the Platform IO environment. A video on how to install such an environment can be watched on this [video](https://youtu.be/Em9NuebT2Kc).
<br><br>
Formatting is done using clang-format. The description of the tool configuration is in the [video](https://youtu.be/xxuaOG0WjIE).
<br><br>
The code contains a comment prepared for doxygen, their use is described in the [video](https://youtu.be/1YKJtrCsPD4).