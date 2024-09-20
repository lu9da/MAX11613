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

/***********************
* 20/092024  
* increased resolution to 12 bit
* by add 16 samples and divide by 4
* as seen in 
* https://forum.arduino.cc/t/converting-analog-values-to-12bits/246579/10
***********************/

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
int VADC0[16];
int VADC1[16];
int VADC2[16];
int VADC3[16];
int sum0 = 0;
int sum1 = 0;
int sum2 = 0;
int sum3 = 0;
int val0 = 0;
int val1 = 0;
int val2 = 0;
int val3 = 0;

byte s = 0;



//#define debug

//int sum = 0;
//for (byte s = 0; s < 16; s++) sum += analogRead(A0);
//int value = sum/4;


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

    Wire.write(highByte(val0));
    Wire.write(lowByte(val0));
    Wire.write(highByte(val1));
    Wire.write(lowByte(val1));
    Wire.write(highByte(val2));
    Wire.write(lowByte(val2));
    Wire.write(highByte(val3));
    Wire.write(lowByte(val3));

#if defined debug

  Serial.print("ADC0: ");
  Serial.println(val0);
  Serial.print("ADC1: ");
  Serial.println(val1);
  Serial.print("ADC2: ");
  Serial.println(val2);
  Serial.print("ADC3: ");
  Serial.println(val3);

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

  VADC0[s] = analogRead(ADC0);
  VADC1[s] = analogRead(ADC1);
  VADC2[s] = analogRead(ADC2);
  VADC3[s] = analogRead(ADC3);

  s++; 

  if (s > 15) {

    s = 0;
  
  }


  sum0 = 0;
  sum1 = 0;
  sum2 = 0;
  sum3 = 0;

  for (byte su = 0; su < 16; su++){

    sum0 = sum0 + VADC0[su];
    sum1 = sum1 + VADC1[su];
    sum2 = sum2 + VADC2[su];
    sum3 = sum3 + VADC3[su];
  
  } 

  val0 = sum0/4;
  val1 = sum1/4;
  val2 = sum2/4;
  val3 = sum3/4;


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
