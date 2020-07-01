class HM1500LF
{
  private:
  int pin;

  public:

  void begin(int _pin);

  float getRHLinear();

  float getRHPolynomial();

  float getRHCompensated(float _RH, float _T);
  
};
