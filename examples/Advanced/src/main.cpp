#include <Arduino.h>
#include <NTPClientExt.hpp>

#define WIFISSID "ssid"
#define WIFIPASS "pass"

NTPClientExt* NTPtime;

#define TIME_ZONE +1  // Madrid
#define TIME_UPDATE (5) // 5 minutes


void setup() {
  int contadorConexion;

  Serial.begin(115200);
  /* Western europe zone (+1) with dayling saving */
  NTPtime = new NTPClientExt("pool.ntp.org",TIME_ZONE,TIME_UPDATE,true);
  NTPtime->setDaylightSaving(3,10);  //Daylight saving time begins on the last Sunday of March (3) and ends on the last Sunday of October (10)

  WiFi.begin(WIFISSID,WIFIPASS);
  Serial.print("Conectando");
  contadorConexion = 0;
  while(WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    contadorConexion++;
    if(contadorConexion > 60){
      ESP.restart();
      }
    }
Serial.println("\r\nConectado.");


}

void loop() {

  static bool ch = false;
  
  if(!ch){
    Serial.println("Local time: Western europe (Madrid)");
  }

  Serial.println("------------------ getFormattedDateTime() ----------------");
  for(int n=0;n<5;n++){
    Serial.println(NTPtime->getFormattedDateTime());
    delay(1000);
  }

  Serial.println("------------------ getFormattedDate()    ----------------");
  for(int n=0;n<5;n++){
    Serial.println(NTPtime->getFormattedDate());
    delay(1000);
  }

  Serial.println("------------------ getFormattedTime()    ----------------");
  for(int n=0;n<5;n++){
    Serial.println(NTPtime->getFormattedTime());
    delay(1000);
  }

  Serial.println("------------------ getTime()            ----------------");
  for(int n=0;n<5;n++){
    Serial.println(NTPtime->getTime());
    delay(1000);
  }

  Serial.println("------------------ getYear()            ----------------");
  for(int n=0;n<5;n++){
    Serial.println(NTPtime->getYear());
    delay(1000);
  }


  Serial.println("------------------ getMonth()           ----------------");
  for(int n=0;n<5;n++){
    Serial.println(NTPtime->getMonth());
    delay(1000);
  }

  Serial.println("------------------ getNameOfMonth()      ---------------");
  for(int n=0;n<5;n++){
    Serial.println(NTPtime->getNameOfMonth(NTPtime->getMonth()));
    delay(1000);
  }


  Serial.println("------------------ getMDay()            ----------------");
  for(int n=0;n<5;n++){
    Serial.println(NTPtime->getMDay());
    delay(1000);
  }

  Serial.println("------------------ getDayOfTheYear()    ----------------");
  for(int n=0;n<5;n++){
    Serial.println(NTPtime->getDayOfTheYear());
    delay(1000);
  }

  Serial.println("------------------ getWeekday()        ----------------");
  Serial.println("------- // 0=Sunday, 1=Monday ... 6=Saturday    -------");
  for(int n=0;n<5;n++){
    Serial.println(NTPtime->getWeekday());
    delay(1000);
  }

  Serial.println("------------------ getNameOfDay()        ----------------");
  for(int n=0;n<5;n++){
    Serial.println(NTPtime->getNameOfDay(NTPtime->getWeekday()));
    delay(1000);
  }

 
  if(!ch){
    ch = true;
    Serial.println("Local time: Caracas (Venezuela)");
    NTPtime->setTimeZone(-4);
    NTPtime->unsetDaylightSaving();
    }


}

