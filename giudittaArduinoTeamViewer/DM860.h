#ifndef TIMER4_H
#include "timer4.h"
#endif


class DM860
{
  private:
  int pin_pulse;
  int pin_en;
  int pin_dir;
  int pulse_rev;
  int ratio;
  int rpm;
  int frequency;
  bool toggle_dir = false;
  timer4 timer;

  int16_t pwm_frequency;
  
  public:

  void begin(int _pin_pulse,int _pin_en, int _pin_dir, int _pulse_rev, int _ratio);

  void setPWMFrequency(uint16_t _pwmFrequency);

  int16_t calculateFrequencyFromRPM(int _rpm);

  void toggleDir();

  void start();

  void stop();
  
};
