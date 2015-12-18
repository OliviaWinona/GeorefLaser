#ifndef _XMETADATA_H
#define _XMETADATA_H

#include <string>

#include "libXBase/XError.h"

//classe mère de toutes les méta-données
class XMetaData
{
protected :
	XError* m_error;
public:
	XMetaData( XError* error = NULL)
	{
		m_error = error;
		//booleen permettant de créer des pointeurs de métadonnées provisoires 
		//en attendant d'implémenter çà avec des shared pointers
		IsMonoAccess = false;
	}
	~XMetaData(void){;}

	bool IsMonoAccess;

	virtual std::string InfoTexte(){return std::string("Description de méta-données non surchargée");}

	bool WriteInfoTxt(std::ostream* out) { *out << InfoTexte(); return out->good();}
	virtual bool XmlWrite(std::ostream* out) = 0;
};

#endif //_XMETADATA_H
