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
    m_image1 = pano1;
    m_image2 = pano2;
}


Appariement::~Appariement()
{
}
//------------------------------------------------------------
int Appariement::NbPointsApp() {return m_listePoints.size()/2;}
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
bool Appariement::PointCoherent(float l, float c, Panoramique* pano)
{
    if(pano->CarteProfondeur()[c+l*pano->Largeur()] == 0)
        return false;
    return true;
}

//------------------------------------------------------------
bool Appariement::AjoutPoint(int num, float l, float c, Panoramique* pano)
{
    Point* pt = new Point(num, l, c);
    m_listePoints.push_back(pt);
    pano->tousPointsIm().push_back(pt);
    return true;
}
//------------------------------------------------------------
bool Appariement::Nettoyage(Panoramique* pano1, Panoramique* pano2)
{
    int nb = m_listePoints.size()/2;
    pano1->tousPointsIm().erase(pano1->tousPointsIm().end()-nb, pano1->tousPointsIm().end());
    pano2->tousPointsIm().erase(pano2->tousPointsIm().end()-nb, pano2->tousPointsIm().end());
    m_listePoints.clear();
    return true;
}

//------------------------------------------------------------
bool Appariement::ChargeMesures(XError* error, std::string FileResult, int* nbPoints)
{
    std::string nomResult = m_image1->Nom() + ".-." + m_image2->Nom() + ".result";
    XErrorInfo(error,__FUNCTION__,"Chargement mesures", nomResult.c_str());
    std::ifstream file;
    XStringTools st;
    file.open(FileResult);
    if (!file.is_open())
        return XErrorError(error,__FUNCTION__,"Erreur ouverture fichier",FileResult.c_str());


    //Vérifier le nombre de points dans le fichier
    int nblignes = 0;
    std::string s;
    while(std::getline(file,s)) ++nblignes;
    if(nblignes < 4)
        return XErrorAlert(error,__FUNCTION__,"Trop peu de points", st.itoa(nblignes).c_str());

    file.clear();
    file.seekg(0, std::ios_base::beg);


    //Chargement des cartes de profondeur
    if(!m_image1->ChargeCarteProfondeur())
        return XErrorError(error,__FUNCTION__,"Erreur de chargement de la carte de profondeur",m_image1->Nom().c_str());
    if(!m_image2->ChargeCarteProfondeur())
        return XErrorError(error,__FUNCTION__,"Erreur de chargement de la carte de profondeur",m_image2->Nom().c_str());


    //Création des Points
    std::string ligneEnCours;
    std::vector<std::string>decoupage;
    float l1, c1, l2, c2;
    while(!file.eof())
    {
        getline(file, ligneEnCours);
        if(ligneEnCours != "")
        {
            st.Tokenize(ligneEnCours, decoupage, "\t");
            c1 = atof(decoupage[0].c_str());
            l1 = atof(decoupage[1].c_str());
            c2 = atof(decoupage[2].c_str());
            l2 = atof(decoupage[3].c_str());
            int num1 = FindPoint(l1, c1, m_image1);
            int num2 = FindPoint(l2, c2, m_image2);

            if(PointCoherent(l1,c1,m_image1) && PointCoherent(l2,c2,m_image2))
            {
                if(num1 == 0 && num2 == 0) //point non existant
                {
                    (*nbPoints) = (*nbPoints) + 1;
                    AjoutPoint(*nbPoints, l1, c1, m_image1);
                    AjoutPoint(*nbPoints, l2, c2, m_image2);
                }

                if(num1 != 0) //point déjà existant sur la pano 1
                    AjoutPoint(num1, l2, c2, m_image2);

                if(num2 != 0) //point déjà existant sur la pano 2
                    AjoutPoint(num2, l1, c1, m_image1);
            }
        }
        decoupage.clear();
    }
    file.close();

    //Suppression des cartes de profondeurs
    m_image1->CarteProfondeur().clear();
    m_image2->CarteProfondeur().clear();

    if(NbPointsApp() < 4)
    {
        (*nbPoints) = (*nbPoints) - NbPointsApp();
        Nettoyage(m_image1, m_image2);
        return XErrorAlert(error,__FUNCTION__,"Nb de points coherent < 4");
    }

    XErrorInfo(error,__FUNCTION__,"Nb points homologues : ",st.itoa(NbPointsApp()).c_str());
    return true;
}
