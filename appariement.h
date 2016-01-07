#ifndef APPARIEMENT_H
#define APPARIEMENT_H

#include <iostream>
#include<vector>
#include "panoramique.h"
#include "point.h"

class Appariement
{
protected:
    Panoramique * image1;
    Panoramique * image2;
    std::vector<Point*> listePoints;

public:
    Appariement(Panoramique* pano1, Panoramique* pano2);
    ~Appariement();

    bool ChargeMesures();

};

#endif // APPARIEMENT_H
