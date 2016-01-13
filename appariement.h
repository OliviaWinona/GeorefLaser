#ifndef APPARIEMENT_H
#define APPARIEMENT_H

#include <iostream>
#include <vector>
#include "libXBase/XError.h"
#include "panoramique.h"
#include "point.h"

class Appariement
{
protected:
    //Attributs
    Panoramique * m_image1;
    Panoramique * m_image2;
    std::vector<Point*> m_listePoints;

    //Méthodes internes
    int FindPoint(float l, float c, Panoramique *pano);
    bool AjoutPoint(int num, float l, float c, Panoramique* pano);
    bool PointCoherent(float l, float c, Panoramique *pano);
    bool Nettoyage(Panoramique* pano1, Panoramique *pano2);

public:
    Appariement(Panoramique* pano1, Panoramique* pano2);
    ~Appariement();

    //Méthodes
    bool ChargeMesures(XError* error, std::string FileResult, int *nbPoints);
    int NbPointsApp();

};

#endif // APPARIEMENT_H
