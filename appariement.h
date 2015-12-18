#ifndef APPARIEMENT_H
#define APPARIEMENT_H
#include <iostream>
#include<vector>
#include "panoramique.h"
#include "point.h"

class Appariement
{
public:
    Appariement();
    ~Appariement();

    Panoramique *image1;
    Panoramique *image2;
    std::vector<Point*> listePoints;
};

#endif // APPARIEMENT_H
