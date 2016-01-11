#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <libXBase/XStringTools.h>
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
Point* Appariement::FindPoint(int numero, Panoramique* pano)
{
    for(int i=0 ; i<pano->tousPointsIm().size() ; i++)
    {
        if (pano->tousPointsIm()[i]->NumPoint() == numero)
            return pano->tousPointsIm()[i];
    }
    return NULL;
}

//------------------------------------------------------------
bool Appariement::ChargeMesures(XError* error, std::string FileResult, int* nbPoints)
{
    std::string nomResult = image1->Nom() + ".-." + image2->Nom() + ".result";
    XErrorInfo(error,__FUNCTION__,"Chargement mesures", nomResult.c_str());
    std::ifstream file;
    file.open(FileResult);
    if (!file.is_open())
        return XErrorError(error,__FUNCTION__,"Erreur ouverture fichier",FileResult.c_str());

    std::string ligneEnCours;
    XStringTools st;
    std::vector<std::string>decoupage;
    while(!file.eof())
    {
        getline(file, ligneEnCours);
        st.Tokenize(ligneEnCours, decoupage, "\t");
        //cout << decoupage[0] << endl;

        decoupage.clear();
    }

    file.close();
    return true;
}
