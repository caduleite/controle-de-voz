/*        
   Conexão Arduino ao VoiceRecognitionModule
   2   ------->     TX
   3   ------->     RX
*/

#include <SoftwareSerial.h>
#include "VoiceRecognitionV3.h"

VR myVR(2,3);    // 2:RX 3:TX.

uint8_t records[7]; // save record
uint8_t buf[64];

int led = 13;

#define LIGAR_LED    (0)
#define DESLIGAR_LED   (1) 

void printSignature(uint8_t *buf, int len)
{
  int i;
  for(i=0; i<len; i++){
    if(buf[i]>0x19 && buf[i]<0x7F){
      Serial.write(buf[i]);
    }
    else{
      Serial.print("[");
      Serial.print(buf[i], HEX);
      Serial.print("]");
    }
  }
}

void printVR(uint8_t *buf)
{
  Serial.println("VR Index\tGroup\tRecordNum\tSignature");

  Serial.print(buf[2], DEC);
  Serial.print("\t\t");

  if(buf[0] == 0xFF){
    Serial.print("NONE");
  }
  else if(buf[0]&0x80){
    Serial.print("UG ");
    Serial.print(buf[0]&(~0x80), DEC);
  }
  else{
    Serial.print("SG ");
    Serial.print(buf[0], DEC);
  }
  Serial.print("\t");

  Serial.print(buf[1], DEC);
  Serial.print("\t\t");
  if(buf[3]>0){
    printSignature(buf+4, buf[3]);
  }
  else{
    Serial.print("NONE");
  }
  Serial.println("\r\n");
}

void setup()
{
  /** Inicializando */
  myVR.begin(9600);
  
  Serial.begin(115200);
  Serial.println("Elechouse Voice Recognition V3 Module\r\nControl LED sample");
  
  pinMode(led, OUTPUT);
    
  if(myVR.clear() == 0){
    Serial.println("Recognizer cleared.");
  }else{
    Serial.println("Not find VoiceRecognitionModule.");
    Serial.println("Please check connection and restart Arduino.");
    while(1);
  }
  
  if(myVR.load((uint8_t)LIGAR_LED) >= 0){
    Serial.println("onRecord loaded");
  }
  
  if(myVR.load((uint8_t)DESLIGAR_LED) >= 0){
    Serial.println("offRecord loaded");
  }
}

void loop()
{
  int ret;
  ret = myVR.recognize(buf, 50);
  if(ret>0){
    switch(buf[1]){
      case LIGAR_LED:
        /** Ligar o LED */
        digitalWrite(led, HIGH);
        break;
      case DESLIGAR_LED:
        /** Desligar o LED*/
        digitalWrite(led, LOW);
        break;
      default:
        Serial.println("Record function undefined");
        break;
    }
    /** Voz reconhecida */
    printVR(buf);
  }
}
