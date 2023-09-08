#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <threadesp32.h>

#define NTPTASK_NAME        "NTP"
#define NTPTASK_HEAP        (4 * 1024)
#define NTPTASK_PRIORITY    TaskParent::th_LOWEST_PRIORITY
#define NTPTASK_CORE        1

#define NTP_MONTHSTARTSUMMERTIME 3
#define NTP_MONTHENDSUMMERTIME 10

#define LEAPYEAR(year)  (((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) ? true : false)


class NTPClientExt : public NTPClient, public TaskParent{

  public:
  
    /* Constructor */
    NTPClientExt(const char* _poolServerName, long _timeOffset, int _updateInterval);
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
    const char* getNameOfDay(int dia){ return nombreDia[lang][dia]; }
    int  getDayOfTheYear();
    int  getWeekDay(int anyo,int mes, int dia);
    int  getLastSundayOfMonth(int anyo,int mes);
    bool itsSummerTime();
    char* getNameOfMonth(int day);

    void update();
    struct tm* epoch2tm(time_t epoch);


  protected:
    void loop();
    bool isConnected() { return WiFi.status() == WL_CONNECTED; }
    
    struct tm ts;
    struct tm tsTemp;
    String buffer;
    char cBuffer[50];
    struct tm* getTS();
    void setLang(const char* _lang);
    WiFiUDP ntpUDP;

    
  private:
    int timeZone=0;
    int tcCentury[6] = {5,3,1,0,-2,-4};
    int tcMonth[12]  = {6,2,2,5,0,3,5,1,4,6,2,4};
    int daysOfTheMonth[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    int monthStartsSummerTime;
    int monthEndsSummerTime;
    char nombreMeses[2][12][5] = {" Ene"," Feb"," Mar"," Abr"," May"," Jun"," Jul"," Ago"," Sep"," Oct"," Nov"," Dic",
                                  " Jan"," Feb"," Mar"," Apr"," May"," Jun"," Jul"," Aug"," Sep"," Oct"," Nov"," Dec"};
    char nombreDia[2][7][10] = {"Domingo","Lunes","Martes","Miercoles","Jueves","Viernes","Sabado",
                               "Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
    int lang = 0;

};                              


