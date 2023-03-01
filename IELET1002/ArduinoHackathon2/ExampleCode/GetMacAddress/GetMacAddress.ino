// Code inspired by example on https://randomnerdtutorials.com/get-change-esp32-esp8266-mac-address-arduino/
#include <WiFi.h>

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  delay(5000);

  Serial.println();
  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
}
