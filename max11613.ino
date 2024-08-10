/***********************
* MAX11613 emulation 
* on Arduino pro-mini
* 
* (c) LU9DA - 2024
* 
* free for use
* not for sale
* 
* this header MAY remains 
* on your implementation
* 
************************/


#include <Wire.h>

const uint8_t I2C_ADDRESS = 0x34; //Generic Board Protocol
const int ledPin = LED_BUILTIN; 
const int ADC0 = A0;
const int ADC1 = A1;
const int ADC2 = A2;
const int ADC3 = A3;
const long interval = 1000;  

int ledState = LOW; 
unsigned long previousMillis = 0;
uint8_t received;
uint8_t message;
int VADC0;
int VADC1;
int VADC2;
int VADC3;


//#define debug




void setup() {

  pinMode(ledPin, OUTPUT);

  //Start i2c as slave
  Wire.begin(I2C_ADDRESS);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);


#if defined debug
  Serial.begin(9600);
#endif
}

void requestEvent()
{

    Wire.write(highByte(VADC0));
    Wire.write(lowByte(VADC0));
    Wire.write(highByte(VADC1));
    Wire.write(lowByte(VADC1));
    Wire.write(highByte(VADC2));
    Wire.write(lowByte(VADC2));
    Wire.write(highByte(VADC3));
    Wire.write(lowByte(VADC3));

#if defined debug
  Serial.print("ADC0: ");
  Serial.println(VADC0);
  Serial.print("ADC1: ");
  Serial.println(VADC1);
  Serial.print("ADC2: ");
  Serial.println(VADC2);
  Serial.print("ADC2: ");
  Serial.println(VADC2);
#endif
    

}

void loop() {

 
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {

    previousMillis = currentMillis;

    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    digitalWrite(ledPin, ledState);
  }

  VADC0=analogRead(ADC0)<<2;
  VADC1=analogRead(ADC1)<<2;
  VADC2=analogRead(ADC2)<<2;
  VADC3=analogRead(ADC3)<<2;
  
}


void receiveEvent(int bytes) {

  int byteCount = 0;
  uint8_t byte1 = 0;
  uint8_t byte2 = 0;
  uint8_t byte3 = 0;

  while (0 < Wire.available()) {
    
    byte x = Wire.read();

      if (byteCount == 0) {
        byte1 = x;
      }
      if (byteCount == 1) {
        byte2 = x;
      }
      if (byteCount == 2) {
        byte3 = x;
      }

      byteCount = byteCount + 1;

  }

#if defined debug
  Serial.print("Byte1 Status: ");
  Serial.println(byte1);
  Serial.print("Byte2 Status: ");
  Serial.println(byte2);
  Serial.print("Byte3 Status: ");
  Serial.println(byte3);

#endif




}