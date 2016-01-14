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
    bool m_traite = false;

    //Méthodes internes
    bool Nettoyage(Panoramique* pano1, Panoramique *pano2);
    bool AjoutPoint(int num, float l, float c, float z, Panoramique* pano);
    bool DejaPresent(std::vector<Point*> lstPts, Point* pt);

public:
    Appariement(Panoramique* pano1, Panoramique* pano2);
    ~Appariement();

    //Méthodes
    bool ChargeMesures(XError* error, std::string FileResult, int *nbPoints);
    int NbPointsApp();
    std::vector<Point *> ChoixQuatrePointsAleatoires(std::vector<Point*> points);

};

#endif // APPARIEMENT_H
