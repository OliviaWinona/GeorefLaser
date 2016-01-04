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
    Appariement(Panoramique _image1, Panoramique _image2, std::initializer_list<Point*> _lstPoints);
    ~Appariement();

    Panoramique *image1;
    Panoramique *image2;
    std::vector<Point*> listePoints;
};

#endif // APPARIEMENT_H
