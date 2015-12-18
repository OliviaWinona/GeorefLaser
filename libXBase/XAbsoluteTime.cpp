#include "XAbsoluteTime.h"
#include <time.h>
#include "libXBase/Xbase.h"
#include "libXGeodesy/XGpsTools.h"


XAbsoluteTime::XAbsoluteTime(void)
{
	year = 0;
	month = 0;
	day = 0;
    m_tps = 0;
	hour = 0;
	min =0;
	sec =0;
}
//-----------------------------------------------------------------------------
XAbsoluteTime::~XAbsoluteTime(void)
{
}
//-----------------------------------------------------------------------------
uint32 XAbsoluteTime::SetTime(uint32 date,uint16 corJour, uint16 corSec)
{
    day = jour(date);
	if(day-corJour < 1)//problème
		std::cout << "ATTENTION CODE TEMPORAIRE PROBLEME SUR LE CALCUL DE LA DATE";
	day= day-corJour;

	month = mois(date);
	year = annee(date);

	struct tm t;
    t.tm_year = year +2000-1900;
    t.tm_mon  = month - 1;
    t.tm_mday = day;
    t.tm_hour = 0;
	t.tm_min  = 0;
	t.tm_sec  = corSec;
	sec = corSec;

    m_tps = mktime(&t);
	//char info[1024];
	//sprintf(info,"Nbre de secondes depuis le 1er Janvier 1970 : %ld\n", tps);
	return m_tps;
}
//-----------------------------------------------------------------------------
uint32 XAbsoluteTime::SetTimeTrajGps(uint32 date)
{
	XGpsTools xgt(date);
	uint16 corSec = xgt.CorrectionUtcToGps();
	uint16 corJour = xgt.WeekDay();

	return SetTime(date,corJour,corSec);
}
//-----------------------------------------------------------------------------
uint32 XAbsoluteTime::SetTimeRxpUtc(uint32 date)
{
	return SetTime(date,0,0);
}
//-----------------------------------------------------------------------------
bool XAbsoluteTime::XmlWrite(std::ostream* out)
{
	*out << "<pivot_ref_gps>\n";
	*out << "<year>" << year+2000 << "</year>\n";
	*out << "<month>"<< month  << "</month>\n";
	*out << "<day>"  << day << "</day>\n";
	*out << "<hour>" << hour << "</hour>\n";
	*out << "<min>"  << min  << "</min>\n";
	*out << "<sec>"  << sec  << "</sec>\n";
	*out << "<sec_1970>" << m_tps << "</sec_1970>\n";
	*out << "</pivot_ref_gps>\n";
	return out->good();
}
