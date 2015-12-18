#pragma once
#include "libXBase/Xbase.h"

class XAbsoluteTime
{
public:
	int year;
	int month;
	int day;
	int hour;
	int min;
	int sec;

    time_t m_tps;
public:
	XAbsoluteTime(void);
	~XAbsoluteTime(void);

	uint32 SetTime(uint32 date, uint16 corJour =0, uint16 corSec = 0);
	uint32 SetTimeTrajGps(uint32 date);
	uint32 SetTimeRxpUtc(uint32 date);
	bool XmlWrite(std::ostream* out);

	uint32 Pivot(){return m_tps;}

};
