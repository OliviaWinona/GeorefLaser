#include <iostream>
#include "appariement.h"

using namespace std;

Appariement::Appariement() : image1(new Panoramique),
                             image2(new Panoramique),
                             listePoints()
{
    cout << "Constructeur par défaut de Appariement" << endl;
}

Appariement::Appariement(Panoramique _image1, Panoramique _image2, std::initializer_list<Point*> _lstPoints) :
                        image1(new Panoramique(_image1)),
                        image2(new Panoramique(_image2)),
                        listePoints(_lstPoints)
{
    cout << "Constructeur de Appariement" << endl;
}

Appariement::~Appariement()
{
}
