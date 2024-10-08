#include <NTPClientExt.hpp>


#if defined(ESP8266)

// Constructor
NTPClientExt::NTPClientExt(const char* poolServerName, int timeOffset, int updateInterval,bool daylightSaving):
      NTPClient(ntpUDP, poolServerName, timeOffset * 3600, updateInterval * 1000 * 60){
      _daylightSaving = daylightSaving;
      _timeZone = timeOffset;

}

#elif defined(ESP32)

// Constructor
NTPClientExt::NTPClientExt(const char* poolServerName, int timeOffset, int updateInterval,bool daylightSaving)
  : NTPClient(ntpUDP, poolServerName, timeOffset * 3600, updateInterval * 1000 * 60), 
    TaskParent(NTPTASK_NAME,NTPTASK_HEAP,NTPTASK_PRIORITY, NTPTASK_CORE){
      _daylightSaving = daylightSaving;
      _timeZone = timeOffset;

}

#endif




// Set the clock to daylight saving time.
void NTPClientExt::setDaylightSaving(int startMonth,int endMonth){
  _monthStartsSummerTime =   startMonth;
  _monthEndsSummerTime = endMonth; 
  _daylightSaving = true;
  if(WiFi.status() == WL_CONNECTED) forceUpdate();
}

// Unset the clock to daylight saving time.
void NTPClientExt::unsetDaylightSaving(){
  _daylightSaving = false;
  if(WiFi.status() == WL_CONNECTED) forceUpdate();  
}

// Returns a string with the date formatted as YYYY-MM-DD
String NTPClientExt::getFormattedDate(){
  getTS();
  strftime(cBuffer, sizeof(cBuffer), "%Y-%m-%d", &ts);
  buffer=String(cBuffer);
  return buffer;
}

// Returns a char* with the date formatted as YYYY-MM-DD
char* NTPClientExt::cGetFormattedDate(){
  getTS();
  strftime(cBuffer, sizeof(cBuffer), "%Y-%m-%d", &ts);
  return cBuffer;
}

// Returns a string with the date formatted as YYYY-MM-DD HH:MM:SS
String NTPClientExt::getFormattedDateTime(){
  getTS();
  strftime(cBuffer, sizeof(cBuffer), "%Y-%m-%d %H:%M:%S", &ts);
  buffer=String(cBuffer);
  return buffer;
}

// Returns a char* with the date formatted as YYYY-MM-DD HH:MM:SS
char* NTPClientExt::cGetFormattedDateTime(){
  getTS();
  strftime(cBuffer, sizeof(cBuffer), "%Y-%m-%d %H:%M:%S", &ts);
  return cBuffer;
}

// Returns a string with the time formatted as HH:MM
String NTPClientExt::getTime(){
  getTS();
  strftime(cBuffer, sizeof(cBuffer), "%H:%M", &ts);
  buffer=String(cBuffer);
  return buffer;
}

// Returns a string with the time formatted as HH:MM:SS
String NTPClientExt::getTimeS(){
  getTS();
  strftime(cBuffer, sizeof(cBuffer), "%H:%M:%S", &ts);
  buffer=String(cBuffer);
  return buffer;
}

// Returns a char* with the time formatted as HH:MM
char* NTPClientExt::cGetTime(){
  getTS();
  strftime(cBuffer, sizeof(cBuffer), "%H:%M", &ts);
  return cBuffer;
}


// Returns the year as an integer
int  NTPClientExt::getYear(){
  return getTS()->tm_year + 1900;
}

// Returns the month as an integer (1-12)
int  NTPClientExt::getMonth(){
  return getTS()->tm_mon + 1;
}

// Returns the number of the day of the month as an integer (1-31)
int  NTPClientExt::getMDay(){
  return getTS()->tm_mday;
}

// Returns the number of the day of the year (1-365 or 1-366 leaps)
int  NTPClientExt::getDayOfTheYear(){
  return getTS()->tm_yday;
}


#if defined(ESP32)

/* Funcion de la tarea */
void NTPClientExt::loop(){
  if(!WiFi.isConnected()){
    delay(1000);
  }
  
  NTPClient::begin();
  while(true){
    if(WiFi.isConnected() ){
      update();
      }
   delay(1000);
  }
}  

#endif



// Update the time, if necessary
void NTPClientExt::update(){
  if(_daylightSaving){
    if(itsSummerTime())
      setTimeOffset((_timeZone + 1) * 3600);   
    else
      setTimeOffset(_timeZone  * 3600);      
    }  
  if(isConnected()){
    NTPClient::update();  
    }

}



// Returns and updates the ts structure
struct tm* NTPClientExt::getTS(){
  time_t epoch = getEpochTime();
  ts = *localtime(&epoch);
  return &ts;
}

// Return tm structure from an arbitrary epoch value
struct tm* NTPClientExt::epoch2tm(time_t epoch){
  tsTemp = *localtime(&epoch);
  return &tsTemp;
}


// Returns the day of the week (0-6)
// Perpetual calendar from 1700 to 2299
// 0=Sunday, 1=Monday ... 6=Saturday
int NTPClientExt::getWeekday(int year,int month, int day){
  
  int cCentury = _tcCentury[(year - 1700) / 100];
  int cYear  = (year % 100) + (year % 100) / 4;
  int cLeapYear = (LEAPYEAR(year) && (month==1 || month==2)) ? -1 : 0; 
  int cMonth = _tcMonth[month - 1];
  
  return (cCentury + cYear + cLeapYear + cMonth + day) % 7;

}

// Returns the day of the week (0-6) of the current date
// Perpetual calendar from 1700 to 2299
// 0=Sunday, 1=Monday ... 6=Saturday
int NTPClientExt::getWeekday(){
  
  int year = getYear();
  int month = getMonth();
  int day = getMDay();

  int cCentury = _tcCentury[(year - 1700) / 100];
  int cYear  = (year % 100) + (year % 100) / 4;
  int cLeapYear = (LEAPYEAR(year) && (month==1 || month==2)) ? -1 : 0; 
  int cMonth = _tcMonth[month - 1];
  
  return (cCentury + cYear + cLeapYear + cMonth + day) % 7;

}


// Returns the day of the last Sunday of the month (to calculate daylight saving time)
int NTPClientExt::getLastSundayOfMonth(int year,int month){
  int dayOfMonth = _daysOfTheMonth[month - 1];
  if(LEAPYEAR(year) && month==2) 
    dayOfMonth++;
  return dayOfMonth - getWeekday(year,month,dayOfMonth);
}

// Returns true if we are in daylight saving time
bool NTPClientExt::itsSummerTime(){
  int year = getYear();
  char frmt[] = "%u-%02u-%02u %02u:00:00";
  char buffer[25];

  sprintf(buffer,frmt,year,_monthStartsSummerTime,getLastSundayOfMonth(year,_monthStartsSummerTime),2);
  String iniSunmmerTime = buffer;
  sprintf(buffer,frmt,year,_monthEndsSummerTime,getLastSundayOfMonth(year,_monthEndsSummerTime),3);
  String endSummerTime = buffer;
  String now = getFormattedDateTime();

  if(now > iniSunmmerTime && now < endSummerTime)
    return true;
  else
    return false;
}

// Returns the name of the month as char*
const char* NTPClientExt::getNameOfMonth(int _month){
  return _nombreMeses[_lang][_month];
}

const char* NTPClientExt::getNameOfDay(int dia){ 
  return _nombreDia[_lang][dia]; 
  }

// Set the language 0 = Spanish, 1 = English
void NTPClientExt::setLang(const char* lang){
  _lang = 0;
  if(strcmp(lang,"ES")==0)
    _lang = 0;
  if(strcmp(lang,"EN")==0)
    _lang = 1;

}

// Set the utc time zone
void NTPClientExt::setTimeZone(int tz){
  _timeZone = tz;
  setTimeOffset(tz * 3600);
  if(WiFi.status() == WL_CONNECTED) forceUpdate();
    
}
