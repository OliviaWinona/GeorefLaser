#include "ConsoleError.h"

#ifdef WIN32
	#include <windows.h>
# endif
#include <iostream>
#include <sstream>

namespace ConsoleError
{
	std::string OrigineMessageData(const char* prefixe,const char* origine, const char* mes, const char* data,bool ShowOrigine)
	{ 
		std::stringstream OMD;
		if(strlen(prefixe) !=0)
			OMD <<  prefixe << '\t' ;
		if(ShowOrigine)
			OMD << origine << '\t';
		OMD << mes << '\t' << data << '\n';
		return OMD.str();
	}
	void Error(const char* origine, const char* mes,  const char* data,bool ShowOrigine)
	{
		#ifdef WIN32	
			SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED);
			std::cout <<  OrigineMessageData("ERREUR",origine,mes,data,ShowOrigine);
			SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		#else
			std::cout << RED << "Erreur " << origine << mes << data << DEFAULT_COLOR <<std::endl;
		#endif
	}

	void Alert(const char* origine, const char* mes,  const char* data,bool ShowOrigine)	
	{
		#ifdef WIN32	
			SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED | FOREGROUND_GREEN );
			std::cout <<  OrigineMessageData("ALERTE",origine,mes,data,ShowOrigine);
			SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		#else
			std::cout << YELLOW <<  "Alerte " << origine << mes << data << DEFAULT_COLOR <<std::endl;
		#endif
	}

	void Info(const char* origine, const char* mes,  const char* data,bool ShowOrigine)
	{
		#ifdef WIN32	
			SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN );
			std::cout <<  OrigineMessageData("",origine,mes,data,ShowOrigine);
			SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		#else
			std::cout << GREEN << "Info " << origine << mes << data << DEFAULT_COLOR <<std::endl;
		#endif
	}
	void Commentaire(const char* origine, const char* mes, bool ShowOrigine)
	{
			if(ShowOrigine)
				std::cout << origine <<'\t';
			std::cout << mes <<'\n';
	}
}
