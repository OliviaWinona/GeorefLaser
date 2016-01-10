#include <iostream>
#include <fstream>
#include "appariement.h"
#include "point.h"
#include "panoramique.h"

using namespace std;

Appariement::Appariement(Panoramique* pano1, Panoramique* pano2)
{
    image1 = pano1;
    image2 = pano2;
}


Appariement::~Appariement()
{
}
//------------------------------------------------------------
bool Appariement::ChargeMesures(XError* error, std::string FileResult)
{
    std::string nomResult = image1->Nom() + ".-." + image2->Nom() + ".result";
    XErrorInfo(error,__FUNCTION__,"Chargement mesures", nomResult.c_str());
    std::ifstream file;
    file.open(FileResult);
    if (!file.is_open())
        return XErrorError(error,__FUNCTION__,"Erreur ouverture fichier",FileResult.c_str());

    std::string ligneEnCours;
    file >> ligneEnCours;
    cout << ligneEnCours << endl;

    file.close();
    return true;
}
