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
int Appariement::FindPoint(float l, float c, Panoramique* pano)
{
    for(int i=0 ; i<pano->tousPointsIm().size() ; i++)
    {
        if (pano->tousPointsIm()[i]->ligne() == l && pano->tousPointsIm()[i]->colonne() == c)
            return pano->tousPointsIm()[i]->numPoint();
    }
    return 0;
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
    float x1, y1, x2, y2;
    while(!file.eof())
    {
        getline(file, ligneEnCours);
        st.Tokenize(ligneEnCours, decoupage, "\t");
        x1 = atof(decoupage[0].c_str());
        y1 = atof(decoupage[1].c_str());
        x2 = atof(decoupage[2].c_str());
        y2 = atof(decoupage[3].c_str());
        int num1 = FindPoint(x1, y1, image1);
        int num2 = FindPoint(x2, y2, image2);

        if(num1 == 0 && num2 == 0) //point non existant
        {
            (*nbPoints) = (*nbPoints) + 1;
            Point* pt1 = new Point((int)nbPoints, x1, y1);
            Point* pt2 = new Point((int)nbPoints, x2, y2);
            listePoints.push_back(pt1);
            listePoints.push_back(pt2);

            image1->tousPointsIm().push_back(pt1);
            image2->tousPointsIm().push_back(pt2);
        }

        if(num1 != 0) //point déjà existant sur la pano 1
        {
            Point* pt2 = new Point(num1, x2, y2);
            listePoints.push_back(pt2);
            image2->tousPointsIm().push_back(pt2);
        }

        if(num2 != 0) //point déjà existant sur la pano 2
        {
            Point* pt1 = new Point(num2, x1, y1);
            listePoints.push_back(pt1);
            image1->tousPointsIm().push_back(pt1);
        }

        decoupage.clear();
    }

    file.close();
    return true;
}
