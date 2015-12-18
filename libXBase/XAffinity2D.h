//-----------------------------------------------------------------------------
//								XAffinity2D.h
//								=============
//
// Auteur : F.Becirspahic - Projet Camera Numerique
//
// 06/10/00
//
// Calcul d'une affinite en dimension 2 pour realiser une orientation
// interne par exemple.
//-----------------------------------------------------------------------------

#ifndef _XAFFINITY_H
#define _XAFFINITY_H

#include "libXBase/XPt2D.h"

class XAffinity2D {
	double Tx;		// Calcul par :
	double Ty;		
	double R11;		//	U		Tx	+ R11.X + R12.Y
	double R12;		//		=
	double R21;		//	V		Ty	+ R21.X + R22.Y
	double R22;

public:
	XAffinity2D(void) : Tx(0), Ty(0), R11(0), R12(0), R21(0), R22(0) {}
	XAffinity2D(double a,double b,double c,double d,double e,double f) :
						Tx(a), Ty(b), R11(c), R12(d), R21(e), R22(f) {}
	// Constructeur avec un jeu de mesures
	XAffinity2D(int nbPt, XPt2D* U, XPt2D* X);
	
	double tx() const { return Tx;}
	double ty() const { return Ty;}
	double r11() const { return R11;}
	double r12() const { return R12;}
	double r21() const { return R21;}
	double r22() const { return R22;}
	
	// Calcul dans le sens direct (c.a.d (X ; Y) vers (U ; V)
	XPt2D Direct(XPt2D) const;
	// Calcul dans le sens indirect (c.a.d (U ; V) vers (X ; Y)
	XPt2D Indirect(XPt2D) const;
	
	// Calcul du residu : valeur calculee - valeur theorique
	XPt2D Residu(XPt2D U, XPt2D X) const;
	
	// Calcul du rmq pour un jeu de mesures
	XPt2D Rmq(int nbPt, XPt2D* U, XPt2D* X) const;

	// Import/Export au format XML
	//virtual bool XmlRead(XParserXML* parser, uint32 num = 0, XError* error = NULL);
	virtual bool XmlWrite(std::ostream* out);
};

#endif //_XAFFINITY_H