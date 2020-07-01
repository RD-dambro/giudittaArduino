#include "i2cProtocol.h"

i2cProtocol i2c_test;
//byte prova[3] = {0x99,0xFF,0x01};
float chamber_T = 0;
float boiler_T = 0;
float air_T = 0;
uint16_t humidity = 0;
Smoothed <float> chamberT;
Smoothed <float> boilerT;
Smoothed <float> airT;
Smoothed <int> hr;

int n = 0, i = 0;
void setup() {
  //Serial.begin(9600);
  //while (!Serial) {}  // wait for serial port to connect. Needed for native USB port only
  // I2C Setup
  //Wire.begin(4);
  //chamberT.begin(SMOOTHED_AVERAGE, 10);
  //boilerT.begin(SMOOTHED_AVERAGE, 10);
  //airT.begin(SMOOTHED_AVERAGE, 10);
  hr.begin(SMOOTHED_AVERAGE, 10);

  delay(3000);
  Wire.begin(4);
  Wire.onReceive(i2c_commandHandler);
  Wire.onRequest(i2c_requestHandler);
  //Wire1.begin();

  //SondaBoiler Setup

  //SondaCamera Setup

  //SondaVentilazione Setup

  //Lamps Setup
  lampade.begin();
  //StepperMotor Setup
  stepperMotor.begin(STEPPER_MOTOR_PULSE, STEPPER_MOTOR_EN, STEPPER_MOTOR_DIR, 3200, 5);
  //SondaHM Setup
  sondaRH.begin(RH_SENSOR);

  humidity = (int)sondaRH.getRHLinear();
  if (humidity < 100 && humidity > 10) {
    hr.add(humidity);
  }
  //PINMODE INTERRUTTORI SETUP

  pinMode(WATERING_PUMP, OUTPUT);
  pinMode(AIR_VENTILATION, OUTPUT);
  pinMode(FOG, OUTPUT);
  pinMode(CHAMBER_HEAT, OUTPUT);
  pinMode(BOILER, OUTPUT);
  pinMode(UV_LAMP, OUTPUT);
  pinMode(WHITE_LAMP_E, OUTPUT);
  pinMode(COLORED_LAMP_E, OUTPUT);
  //pinMode(STEPPER_MOTOR_DIR, OUTPUT);
  //digitalWrite(STEPPER_MOTOR_DIR, HIGH);
  //pinMode(13,OUTPUT);
  digitalWrite(WATERING_PUMP, LOW);
  digitalWrite(FOG, LOW); //2002 K4
  digitalWrite(AIR_VENTILATION, LOW); //2003 K3
  digitalWrite(CHAMBER_HEAT, LOW); //2007 K7
  digitalWrite(BOILER, LOW); //2006 K8
  digitalWrite(UV_LAMP, LOW); //2005 K6
  digitalWrite(WHITE_LAMP_E, LOW);
  digitalWrite(COLORED_LAMP_E, LOW);



  //resetRPi
  int pin_rpi = 38;
  pinMode(pin_rpi, OUTPUT);
  digitalWrite(pin_rpi, LOW);
  delay(100);
  digitalWrite(pin_rpi, HIGH);

  //digitalWrite(pin_rpi, LOW);

  //i2c_test.Decode(&prova[0],3);
}

void i2c_commandHandler(int howMany) {
  byte _msg[howMany];
  int cnt = 0;
  while (Wire.available()) {
    _msg[cnt] = Wire.read();
    cnt++;
  }
  for (int i = 0; i < howMany; i++) {
    Serial.println(_msg[i]);
  }
  i2c_test.Decode(&_msg[0], howMany);
}

void i2c_requestHandler() {
  //Serial.println(i2c_test._encoded_msg[0]);
  for (int i = 0; i < i2c_test.current_encoded_msg_size; i++) {
    Wire.write(i2c_test._encoded_msg[i]);
  }
  memset(&i2c_test._encoded_msg[0], 0, 5);
}

void loop() {
  
    while (ds.selectNext())
    {
      uint8_t address[8];
      ds.getAddress(address);
      
      switch (address[7])
      {
        case BOILER_T:
          boiler_T = ds.getTempC();
          break;

        case RADIATOR_T:
          chamber_T = ds.getTempC();
          break;

        case ROOM_T:
          air_T = ds.getTempC();
          break;

        default: break;
      }     
      
      delay(1000);
    }
  delay(500);
}
