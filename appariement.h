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
    Panoramique * image1;
    Panoramique * image2;
    std::vector<Point*> listePoints;

    //Méthodes internes
    int FindPoint(float l, float c, Panoramique *pano);

public:
    Appariement(Panoramique* pano1, Panoramique* pano2);
    ~Appariement();

    //Méthodes
    bool ChargeMesures(XError* error, std::string FileResult, int *nbPoints);

};

#endif // APPARIEMENT_H
