#pragma once

#include <Arduino.h>

#include <NTPClient.h>
#include <WiFiUdp.h>

#if defined(ESP8266)
  #include <ESP8266WiFi.h>

#elif defined(ESP32)
  #include <threadesp32.h>

  #define NTPTASK_NAME        "NTP"
  #define NTPTASK_HEAP        (3 * 1024)
  #define NTPTASK_PRIORITY    TaskParent::th_LOWEST_PRIORITY
  #define NTPTASK_CORE        1

#endif

#define NTP_MONTHENDSUMMERTIME 10
#define LEAPYEAR(year)  (((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) ? true : false)

#if defined(ESP8266)
  class NTPClientExt : public NTPClient{

#elif defined(ESP32)
  class NTPClientExt : public NTPClient, public TaskParent{
#endif

  public:
  
    /* Constructor */
    NTPClientExt(const char* _poolServerName, int _timeOffset, int _updateInterval,bool _daylightSaving = false);
    String getFormattedDate();      
    char*  cGetFormattedDate();

    String getFormattedDateTime();
    char*  cGetFormattedDateTime();

    String getTime();
    String getTimeS();
    char*  cGetTime();

    int  getYear();
    int  getMonth();
    int  getMDay();
    int  getWeekday(int anyo,int mes, int dia);
    int  getWeekday();
    int  getLastSundayOfMonth(int anyo,int mes);
    bool itsSummerTime();
    void update();
    void setDaylightSaving(int _startMonth,int _endMonth);
    void unsetDaylightSaving();
    void setTimeZone(int tz);
    int  getDayOfTheYear();
    const char* getNameOfMonth(int day);
    const char* getNameOfDay(int dia);
    void setLang(const char* _lang);
    struct tm* epoch2tm(time_t epoch);


  protected:
    void loop();
    bool isConnected() { return WiFi.status() == WL_CONNECTED; }
    struct tm ts;
    struct tm tsTemp;
    String buffer;
    char cBuffer[50];
    struct tm* getTS();
    WiFiUDP ntpUDP;

    
  private:
    int _timeZone=0;
    int _tcCentury[6] = {5,3,1,0,-2,-4};
    int _tcMonth[12]  = {6,2,2,5,0,3,5,1,4,6,2,4};
    int _daysOfTheMonth[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    int _monthStartsSummerTime =   3;  // The last Sunday of March (Western Europe)
    int _monthEndsSummerTime = 10;     // The last Sunday in October (Western Europe)
    bool _daylightSaving;
    char _nombreMeses[2][12][5] = {" Ene"," Feb"," Mar"," Abr"," May"," Jun"," Jul"," Ago"," Sep"," Oct"," Nov"," Dic",
                                  " Jan"," Feb"," Mar"," Apr"," May"," Jun"," Jul"," Aug"," Sep"," Oct"," Nov"," Dec"};
    char _nombreDia[2][7][10] = {"Domingo","Lunes","Martes","Miercoles","Jueves","Viernes","Sabado",
                               "Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
    int _lang = 1;

};                              


