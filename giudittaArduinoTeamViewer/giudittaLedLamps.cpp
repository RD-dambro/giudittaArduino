#include "giudittaLedLamps.h"
void giudittaLedLamps::begin(){
  pinMode(46,OUTPUT);
  pinMode(45,OUTPUT);  
  TCCR5A = 0;
  TCNT5 = 0;
  TCCR5A = 0b10100001;
  TCCR5B = 0b00001100;
}

void giudittaLedLamps::setDuty(int _lamp,int _duty){
  switch (_lamp) {
    case 0:
      OCR5AL = _duty;
      break;
    case 1:
      OCR5BL = _duty;
      break;  
    default:
    break;
  }
}
