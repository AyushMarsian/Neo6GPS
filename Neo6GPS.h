#ifndef NEO6GPS_h
#define NEO6GPS_h
#define __NEO6GPS_VERSION__ 1.0.0
#include "Arduino.h"

#ifndef ESP32
	#include <WiFi.h>
#elif ESP8266
	#include <ESP8266WiFi.h>
#endif


#define TIMEOUT 3 // Wait for 60 Seconds

struct gpsData 
{
	bool status;         /* data is valid or not */
    uint8_t sec;         /* seconds */
    uint8_t min;         /* minutes */
    uint8_t hour;        /* hours */
    uint8_t date;        /* date of the month */
    uint8_t month;       /* month */
    uint8_t year;        /* year */
    float latitude;		 /* latitude */
    float longitude;	     /* longitude */

#ifdef ALL_DATA
    float speedOG;       /* Speed over ground */
    float courseOG;      /* course over ground */
#endif 
};

class Neo6GPS		
{									
  private:
	int _timeout;
    uint8_t reTry=0;
	Stream* _serial;
	String _serialBuffer="";
    char tempBuff[20]={0};
    uint8_t _commaIndex=0;
    uint8_t commaIndex=0;
    uint16_t tempValue=0;
    int16_t timeZone=0;
	String _readSerial();
	void _clearSerial();
    void _parsGPRMC(struct gpsData *ldata);
  public:

  	Neo6GPS();
    void begin(Stream &serial);
    bool readData(struct gpsData *ldata);
    void setTimeZone(int8_t hrs,int8_t mins);
};

#endif 