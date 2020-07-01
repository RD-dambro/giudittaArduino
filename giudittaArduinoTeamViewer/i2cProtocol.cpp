#ifndef I2CPROTOCOL_H
#include "i2cProtocol.h"
#endif

//OneWire oneWireBoiler(ONE_WIRE_BUS_BOILER), oneWireCamera(ONE_WIRE_BUS_CAMERA), oneWireVentilazione(ONE_WIRE_BUS_VENTILAZIONE);
DS18B20 ds(ONE_WIRE_BUS);
giudittaLedLamps lampade;

DM860 stepperMotor;
HM1500LF sondaRH;
//TwoWire Wire1;

//Funzione utile per capire l'indice del dizionario dei tipi a seconda del nome del tipo di variabile
byte i2cProtocol::findArgsIndexFromName(char* _name) {
  byte index = -1;

  for (byte i = 0; i < this->dict_size; i++) {
    if (_name == this->typeCodeDict[i]._type_name) {
      index = i;
    }
  }
  return index;
}

//Funzione utile per capire l'indice del dizionario dei tipi a seconda del codice del tipo
byte i2cProtocol::findArgsIndexFromCode(byte _code) {
  byte index = - 1;

  for (byte i = 0; i < this->dict_size; i++) {
    if (_code == this->typeCodeDict[i]._type_code) {
      index = i;
    }
  }
  return index;
}


//Funzione per effettuare il Decode del messaggio codificato per la parte degli argomenti
void i2cProtocol::argsDecode(byte *_msg, byte _size) {
  memset(&u_converter[0], 0, MAX_ARGUMENT_NUMBER);
  //Storing Phase
  byte tmp[_size];
  for (int i = 0; i < _size; i++) {
    tmp[i] = *_msg;
    _msg++;
  }

  int i = 0;
  int args_cnt = 0;
  while (i < _size) {
    int current_arg_size = this->typeCodeDict[this->findArgsIndexFromCode(tmp[i])]._type_size;
    for (int j = 1; j <= current_arg_size; j++) {
      u_converter[args_cnt]._u[current_arg_size - j] = tmp[i + j];
    }
    i += current_arg_size + 1;
    args_cnt++;
  }
}

//Funzione per codificare il messaggio da inviare al Master
void i2cProtocol::argsEncode(byte howMany, char* _type_name[5]) {

  int lenght = 0;
  for (int i = 0; i < howMany; i++) {
    _encoded_msg[lenght] = this->typeCodeDict[this->findArgsIndexFromName(_type_name[i])]._type_code;
    lenght++;
    for (int j = 0; j < this->typeCodeDict[this->findArgsIndexFromName(_type_name[i])]._type_size ; j++) {
      _encoded_msg[lenght] = u_converter[i]._u[this->typeCodeDict[this->findArgsIndexFromName(_type_name[i])]._type_size - j - 1];
      lenght++;
    }
  }
}

//Funzione di switching che permette di svolgere diverse funzioni a seconda della codifica del primo byte, il resto del messaggio viene inviato a argsDecode
void i2cProtocol::Decode(byte *_msg, byte _size) {
  memset(&u_converter[0], 0, MAX_ARGUMENT_NUMBER);
  if (_size > 1) {
    this->argsDecode(_msg + 1, _size - 1);
  }

  //Serial.println("Sto nello switch");
  //Serial.println(*_msg, HEX);
  char* types[5] = {0, 0, 0, 0, 0};
  switch (*_msg) {

    case 0x02: //Spegni Motore
      //Serial.println("SpegniMotore");
      if (u_converter[0]._u_bool) {
        stepperMotor.start();
      }
      else {
        stepperMotor.stop();
      }
      break;
    case 0x01: //Inverti Motore
      stepperMotor.toggleDir();
      //Serial.println("DIR");
      break;
    case 0x03: //Cambia valore Motore

      //Serial.println("Motore");
      //uint16_t freq = stepperMotor.calculateFrequencyFromRPM(u_converter[0]._u_int);
      Serial.println(stepperMotor.calculateFrequencyFromRPM(u_converter[0]._u_int));
      stepperMotor.setPWMFrequency(stepperMotor.calculateFrequencyFromRPM(u_converter[0]._u_int));
      //stepperMotor.start();
      break;

    case 0x04: //Spegni-Accendi Lampade Bianche
      //Serial.println("Lampade");
      digitalWrite(WHITE_LAMP_E, u_converter[0]._u_bool);

      break;
    case 0x05: //Cambia valore Lampade Bianche
      if (u_converter[0]._u_int >= 0) {
        //digitalWrite(WHITE_LAMP_E, HIGH);
        lampade.setDuty(0, (uint16_t)map(u_converter[0]._u_int, 0, 100, 0, 254));
      }
      else {
        digitalWrite(WHITE_LAMP_E, LOW);
      }
      break;
    case 0x06: //Spegni - accendi Lampade Colorate
      digitalWrite(COLORED_LAMP_E, u_converter[0]._u_bool);
      break;
    case 0x07: //Cambia Lampade Colorate
      if (u_converter[0]._u_int >= 0) {
        //digitalWrite(COLORED_LAMP_E, HIGH);
        lampade.setDuty(1, (uint16_t)map(u_converter[0]._u_int, 0, 100, 0, 254));
      }
      else {
        digitalWrite(COLORED_LAMP_E, LOW);
      }      break;
    case 0x08: // TOGGLE Lampade UV
      //Serial.println("Ciao");
      //digitalWrite(13,u_converter[0]._u_bool);
      if (u_converter[0]._u_bool) {
        digitalWrite(UV_LAMP, HIGH);
      }
      else {
        digitalWrite(UV_LAMP, LOW);
      }
      break;
    case 0x0B: // TOGGLE Boiler
      //Serial.println("Boiler");
      digitalWrite(BOILER, u_converter[0]._u_bool);
      break;
    case 0x0A: // TOGGLE Fog
      Serial.println("FOG");
      digitalWrite(FOG, u_converter[0]._u_bool);
      break;
    case 0x09: // TOGGLE Watering
      //Serial.println("WATERING");
      digitalWrite(WATERING_PUMP, u_converter[0]._u_bool);
      break;
    case 0x0C: // TOGGLE Ventilazione
      //Serial.println("Vent");
      digitalWrite(AIR_VENTILATION, u_converter[0]._u_bool);
      break;
    case 0x0D: // TOGGLE Riscaldamento Camera
      //Serial.println("RISCAldamento");
      digitalWrite(CHAMBER_HEAT, u_converter[0]._u_bool);
      break;
    case 0x14: // Leggi Sensore Temperatura Camera
      //memset(&u_converter[0], 0, MAX_ARGUMENT_NUMBER);
      current_encoded_msg_size = 5;
      
      u_converter[0]._u_float = chamberT.get();
      
      types[0] = "float";
      this->argsEncode(1, types);
      break;
    case 0x15: // Leggi Sensore Temperatura Boiler
      //memset(&u_converter[0], 0, MAX_ARGUMENT_NUMBER);
      current_encoded_msg_size = 5;
      
      u_converter[0]._u_float = boilerT.get();
      types[0] = "float";
      this->argsEncode(1, types);
      break;
    case 0x16: // Leggi Sensore Temperatura Ventilazione
      //memset(&u_converter[0], 0, MAX_ARGUMENT_NUMBER);
      current_encoded_msg_size = 5;
      
      u_converter[0]._u_float = airT.get();
      
      types[0] = "float";
      this->argsEncode(1, types);
      break;
    case 0x17: // Leggi Flussimetro
      current_encoded_msg_size = 18;
      u_converter[0]._u_float = chamber_T;
      u_converter[1]._u_float = boiler_T;
      u_converter[2]._u_float = air_T;
      u_converter[3]._u_int = hr.get();
      types[0] = "float";
      types[1] = "float";
      types[2] = "float";
      types[3] = "int";
      this->argsEncode(4, types);
      break;
    case 0x18: // Leggi Umidità
      current_encoded_msg_size = 3;
      u_converter[0]._u_int = (int)sondaRH.getRHLinear();
      types[0] = "int";
      this->argsEncode(1, types);
      break;
    case 0xFF:
      //Serial.println("Stop");
      digitalWrite(WHITE_LAMP_E, LOW);
      digitalWrite(COLORED_LAMP_E, LOW);
      digitalWrite(UV_LAMP, LOW);
      digitalWrite(FOG, LOW);
      digitalWrite(BOILER, LOW);
      digitalWrite(AIR_VENTILATION, LOW);
      digitalWrite(WATERING_PUMP, LOW);
      digitalWrite(CHAMBER_HEAT, LOW);
      stepperMotor.stop();
      break;
    default:
      Serial.println("DEFAULT");
      break;

  }
}
