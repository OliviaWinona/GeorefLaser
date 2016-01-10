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
    Panoramique * image1;
    Panoramique * image2;
    std::vector<Point*> listePoints;

public:
    Appariement(Panoramique* pano1, Panoramique* pano2);
    ~Appariement();

    bool ChargeMesures(XError* error, std::string FileResult);

};

#endif // APPARIEMENT_H
