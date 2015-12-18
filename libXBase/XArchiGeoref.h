#ifndef _X_ARCHI_GEOREF_H_
#define _X_ARCHI_GEOREF_H_

#include "libXBase/XPt3D.h"
#include "libXBase/XMat3D.h"

//Donn�es relatives au g�or�f�rencement
class XArchiGeoref
{
protected ://donn�es membres
	XPt3D			m_Translation;	// Rattachement dans le r�f�rentiel v�hicule
	double       m_dEchelle; //inutilis� pour l'instant !

	//donn�es calcul�es
	//Les donn�es sont exprim�es dans le sens : rep�re capteur --> rep�re v�hicule
	XMat3D m_Rotation;
public :
	XArchiGeoref(XPt3D T = XPt3D(), XMat3D M= XMat3D::Identite())
	{
		m_Translation = T;
		m_Rotation = M;
		m_dEchelle = 1.;
	}
	void Translation(XPt3D p){m_Translation = p;}
	XPt3D Translation(){return m_Translation;}

	XMat3D Rotation(){return m_Rotation;}
	void Rotation(XMat3D M){m_Rotation = M;}

	XPt3D Applique_transfo(XPt3D Pt);//rep�re capteur --> rep�re v�hicule
	XPt3D Applique_inverse_transfo(XPt3D Pt);//rep�re v�hicule --> rep�re capteur

	XMat3D Applique_transfo(XMat3D M);
	XMat3D Applique_inverse_transfo(XMat3D M);

	XArchiGeoref Applique_transfo(XArchiGeoref G);
	XArchiGeoref Applique_inverse_transfo(XArchiGeoref G);

	std::string InfoTexte();
	bool XmlWrite(std::ostream* out);

};

#endif
