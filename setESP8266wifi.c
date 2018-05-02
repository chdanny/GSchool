//
//esp8266測試 輸入at 會回傳 ok 
//
//


#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // Arduino RX:0, TX:1  
void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
     Serial.write("111");
  }
  mySerial.begin(115200);
}
void loop() // run over and over
{
  if (mySerial.available())
    Serial.write(mySerial.read());
  if (Serial.available())
    mySerial.write(Serial.read());
}