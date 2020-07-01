#ifndef DS18B20_H
#include <DS18B20.h>
#endif

#ifndef ARDUINO_H
#include "Arduino.h"
#endif

#ifndef WIRE_H
#include <Wire.h>
#endif

#ifndef DM860_H
#include "DM860.h"
#endif
#ifndef HM1500LF_H
#include "HM1500LF.h"
#endif
#ifndef GIUDITTALEDLAMPS_H
#include "giudittaLedLamps.h"
#endif
#ifndef SMOOTHED_H
#include <Smoothed.h>
#endif


#define ONE_WIRE_BUS 14
#define BOILER_T 201
#define RADIATOR_T 45
#define ROOM_T 145

#define WHITE_LAMP 46
#define COLORED_LAMP 45
#define WHITE_LAMP_E 30
#define COLORED_LAMP_E 31

#define STEPPER_MOTOR_EN 9
#define STEPPER_MOTOR_PULSE 7
#define STEPPER_MOTOR_DIR 5
#define RH_SENSOR A0
#define WATERING_PUMP 33
#define AIR_VENTILATION 37
#define FOG 34
#define CHAMBER_HEAT 32
#define BOILER 35
#define UV_LAMP 36

extern DS18B20 ds;
extern DM860 stepperMotor;
extern HM1500LF sondaRH;
extern TwoWire Wire1;
extern giudittaLedLamps lampade;
extern float chamber_T;
extern float boiler_T;
extern float air_T;
extern uint16_t humidity;
extern Smoothed <float> chamberT;
extern Smoothed <float> boilerT;
extern Smoothed <float> airT;
extern Smoothed <int> hr;
#define MAX_ARGUMENT_NUMBER 5

typedef struct {
  byte _type_code;
  char* _type_name;
  int _type_size;
} typeCode;



class i2cProtocol {
  public:

    union all_type_u {
      byte _u[4];
      int32_t _u_uint32;
      int _u_int;
      float _u_float;
      byte _u_byte;
      char _u_char;
      bool _u_bool;
    } u_converter[MAX_ARGUMENT_NUMBER];
    byte _encoded_msg[20];
    byte current_encoded_msg_size = 0;
    void Decode(byte *_msg, byte _size);
    void argsDecode(byte *_msg, byte _size);
    byte findArgsIndexFromCode(byte _code);
    byte findArgsIndexFromName(char* _name);
    void argsEncode(byte howMany, char* _type_name[5]);

  private:
    const static byte dict_size = 3;
    const typeCode typeCodeDict[dict_size] {
      {0xFF, "bool", 1},
      {0xFE, "int", 2},
      {0xFD, "float", 4}
    };
};
