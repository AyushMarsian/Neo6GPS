#include "Neo6GPS.h"

////////////////////////////////////////////////////PRIVATE DEFINITION////////////////////////////////////////////////////

String Neo6GPS::_readSerial()
{
	_timeout=0;
	while(!_serial->available() && _timeout<(TIMEOUT*100)) 
	{
		delay(10);
		yield();
		_timeout++;
	}
	if(_serial->available())
	{
		return _serial->readStringUntil('\r');
	}

	return "";
}

void Neo6GPS::_clearSerial()
{
	while(_serial->available())
	{
		_serial->read();
	}
}

void Neo6GPS::_parsGPRMC(struct gpsData *ldata)
{
	_serialBuffer = _serialBuffer.substring(_serialBuffer.indexOf("$GPRMC"),_serialBuffer.indexOf('*'));
	
	_commaIndex=_serialBuffer.indexOf(',');

	ldata->hour = _serialBuffer.substring(_commaIndex+1,_commaIndex+3).toInt();
	_commaIndex=_commaIndex+3;
	ldata->min = _serialBuffer.substring(_commaIndex,_commaIndex+2).toInt();
	_commaIndex=_commaIndex+2;
	ldata->sec = _serialBuffer.substring(_commaIndex,_commaIndex+2).toInt();
	
	_commaIndex=_serialBuffer.indexOf(',',_commaIndex);

	if(_serialBuffer.substring(_commaIndex+1,_commaIndex+2) == "A")
	{
		ldata->status = 1;
	}
	else
	{
		ldata->status = 0;
	}

	_commaIndex=_serialBuffer.indexOf(',',_commaIndex+1);

	tempValue = _serialBuffer.substring(_commaIndex+1,_commaIndex+3).toInt();
	commaIndex=_commaIndex+3;
	_commaIndex=_serialBuffer.indexOf(',',commaIndex);
	
	ldata->latitude = tempValue + (((_serialBuffer.substring(commaIndex,_commaIndex)).toFloat())/60.0);
	
	commaIndex=_commaIndex+1;
	_commaIndex=_serialBuffer.indexOf(',',commaIndex);

	if(_serialBuffer.substring(commaIndex,_commaIndex)=="S")
	{
		ldata->latitude = 0-ldata->latitude;
	}

	commaIndex=_commaIndex+1;
	tempValue = _serialBuffer.substring(commaIndex,commaIndex+3).toInt();
	commaIndex=commaIndex+3;
	_commaIndex=_serialBuffer.indexOf(',',commaIndex);
	
	ldata->longitude = tempValue + (((_serialBuffer.substring(commaIndex,_commaIndex)).toFloat())/60.0);

	commaIndex=_commaIndex+1;
	_commaIndex=_serialBuffer.indexOf(',',commaIndex);

	if(_serialBuffer.substring(commaIndex,_commaIndex)=="W")
	{
		ldata->longitude = 0-ldata->longitude;
	}

	commaIndex=_commaIndex+1;
	_commaIndex=_serialBuffer.indexOf(',',commaIndex);

#ifdef ALL_DATA
	ldata->speedOG = _serialBuffer.substring(commaIndex,_commaIndex).toFloat();
#endif 

	commaIndex=_commaIndex+1;
	_commaIndex=_serialBuffer.indexOf(',',commaIndex);

#ifdef ALL_DATA
	ldata->courseOG = _serialBuffer.substring(commaIndex,_commaIndex).toFloat();
#endif 

	ldata->date = _serialBuffer.substring(_commaIndex+1,_commaIndex+3).toInt();
	_commaIndex=_commaIndex+3;
	ldata->month = _serialBuffer.substring(_commaIndex,_commaIndex+2).toInt();
	_commaIndex=_commaIndex+2;
	ldata->year = _serialBuffer.substring(_commaIndex,_commaIndex+2).toInt();
}

/////////////////////////////////////////////////INSTANT/INIT DEFINITION/////////////////////////////////////////////////

Neo6GPS::Neo6GPS(void)
{
  _serialBuffer.reserve(255); //reserve memory to prevent fragmention

}

void Neo6GPS::begin(Stream &serial) // begin Definition with Serial port assignment
{
	_serial = &serial;
	yield();
}
////////////////////////////////////////////////////PUBLIC DEFINITION////////////////////////////////////////////////////

bool Neo6GPS::readData(struct gpsData *ldata)
{
	_clearSerial();
	delay(500);
	for(reTry=0;reTry<11;reTry++)
	{
		_serialBuffer=_readSerial();
		if((_serialBuffer.indexOf("$GPRMC")) !=-1 )
		{
			_parsGPRMC(ldata);

			tempValue = (ldata->hour*60)+ ldata->min + timeZone;
			ldata->hour = tempValue/60;
			ldata->min = tempValue-(ldata->hour*60);

			return true;
		}
	}
	
	return false;
}

void Neo6GPS::setTimeZone(int8_t hrs,int8_t mins)
{
	timeZone=(hrs*60)+mins;
}

