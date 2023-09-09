#include <Arduino.h>
#include <NTPClientExt.hpp>

#define WIFISSID "wifi"
#define WIFIPASS "pass"

/* Western europe (+1) */

NTPClientExt* NTPtime;

#define TIME_ZONE +1  // western europe
#define TIME_UPDATE (5) // 5 minutes


void setup() {
  int contadorConexion;

  Serial.begin(115200);
  /* Western europe zone +1 with dayling saving */
  NTPtime = new NTPClientExt("pool.ntp.org",TIME_ZONE,TIME_UPDATE,true);

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

 Serial.println(NTPtime->getFormattedDateTime());
 delay(1000);



}

