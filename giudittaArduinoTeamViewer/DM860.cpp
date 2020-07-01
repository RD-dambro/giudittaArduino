#ifndef DM860_H
#include "DM860.h"
#endif


void DM860::begin(int _pin_pulse, int _pin_en, int _pin_dir, int _pulse_rev, int _ratio)
{
  pin_pulse = _pin_pulse;
  pin_en = _pin_en;
  pin_dir = _pin_dir;
  pulse_rev = _pulse_rev;
  ratio = _ratio;

  pinMode(pin_en, OUTPUT);
  pinMode(pin_dir, OUTPUT);
  digitalWrite(pin_dir, HIGH);

  timer.beginStepper();
}


void DM860::setPWMFrequency(uint16_t _pwm_frequency)
{
  pwm_frequency = _pwm_frequency;
  timer.setStepperSpeed(pwm_frequency);
}


int16_t DM860::calculateFrequencyFromRPM(int _rpm)
{
  return (uint16_t)(((uint32_t)((uint32_t)pulse_rev*ratio*_rpm))/60.00);
}

void DM860::toggleDir()
{
  toggle_dir = not(toggle_dir);
  
  if (toggle_dir){digitalWrite(pin_dir, HIGH);}
  else{digitalWrite(pin_dir, LOW);} 
}

void DM860::start()
{
  digitalWrite(pin_en, LOW);
}
void DM860::stop()
{
  digitalWrite(pin_en, HIGH);
}
