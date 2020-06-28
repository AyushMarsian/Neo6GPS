/*  Copyright to AyushMarsian(Ankit Ghevariya)
 *  Licence GNU GPL V3
 *  
 *  Example for read location and time from GPS data and correct the time according to timezone
 *  Written by Ankit Ghevariya, public domain
 *  
 *  You have to connect pin as written below
 *  (Neo6M) --------- (Arduino)
 *   Vcc    --------> 5v
 *   TxD    --------> Digital 2
 *   RxD    --------> Digital 3
 *   Gnd    --------> Gnd
 */

#include <Neo6GPS.h>
#include <SoftwareSerial.h>

SoftwareSerial gSerial(2, 3); // RX, TX
Neo6GPS gps;

struct gpsData g; // Structure in which data of GPS will be store
char buffer[50]={0};

void setup()
{
  Serial.begin(9600); 
  gSerial.begin(9600);      // Software serial port for GPS interfacing
  gps.begin(gSerial);       // Pass serial port where GPS is connected
  gps.setTimeZone(-4,0);    // Pass hours and minute in function. Ex. timezone of Ontario,Canada is -04:00
  //gps.setTimeZone(5,30);  // Pass hours and minute in function. Ex. timezone of India is +05:30
}

void loop() 
{ 
	if(gps.readData(&g)) // Will return 0 in case if GPS not responding
	{
		if(g.status) // GPS will take time to find correct location, status will be 1 when GSP finds the location
		{
			Serial.print("Latitude:");
			Serial.println(g.latitude,6);
			Serial.print("Longitude:");
			Serial.println(g.longitude,6);
			sprintf(buffer,"Time:%d/%d/%d %d:%d:%d",g.date,g.month,g.year,g.hour,g.min,g.sec);
			Serial.println(buffer);
		}
		else
		{
			Serial.println("GPS not ready");
		}
	}
	else
	{
		Serial.println("Error while reading data from GPS");
	}
}

