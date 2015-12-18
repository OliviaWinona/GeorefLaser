#ifndef _XMETADATA_H
#define _XMETADATA_H

#include <string>

#include "libXBase/XError.h"

//classe m�re de toutes les m�ta-donn�es
class XMetaData
{
protected :
	XError* m_error;
public:
	XMetaData( XError* error = NULL)
	{
		m_error = error;
		//booleen permettant de cr�er des pointeurs de m�tadonn�es provisoires 
		//en attendant d'impl�menter �� avec des shared pointers
		IsMonoAccess = false;
	}
	~XMetaData(void){;}

	bool IsMonoAccess;

	virtual std::string InfoTexte(){return std::string("Description de m�ta-donn�es non surcharg�e");}

	bool WriteInfoTxt(std::ostream* out) { *out << InfoTexte(); return out->good();}
	virtual bool XmlWrite(std::ostream* out) = 0;
};

#endif //_XMETADATA_H
