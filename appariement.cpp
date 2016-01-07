#include <iostream>
#include "appariement.h"

using namespace std;

Appariement::Appariement(Panoramique* pano1, Panoramique* pano2)
{
    image1 = pano1;
    image2 = pano2;
}


Appariement::~Appariement()
{
}
