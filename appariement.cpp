#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <libXBase/XStringTools.h>
#include "appariement.h"
#include "point.h"
#include "panoramique.h"
#include "chantier.h"

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
bool Appariement::AjoutPoint(int num, float l, float c, float z, Panoramique* pano)
{
    Point* pt = new Point(num, l, c, z);
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
    std::string chemin = m_image1->Chantier()->Dossier() + "\\" + FileResult;
    std::ifstream file;
    XStringTools st;
    char message[1024];
    file.open(chemin);
    if (!file.is_open())
        return XErrorError(error,__FUNCTION__,"Erreur ouverture fichier",chemin.c_str());
    XErrorInfo(error,__FUNCTION__,"Chargement mesures", FileResult.c_str());


    //Vérifier le nombre de points dans le fichier
    int nblignes = 0;
    std::string s;
    while(std::getline(file,s)) ++nblignes;
    if(nblignes < 4)
    {
        sprintf(message,"Trop peu de points : %s, appariement non pris en compte",st.itoa(nblignes).c_str());
        XErrorAlert(error,__FUNCTION__,message);
        return false;
    }
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
    float l1, c1, l2, c2, z1, z2;
    while(!file.eof())
    {
        decoupage.clear();
        getline(file, ligneEnCours);
        if(ligneEnCours != "")
        {
            st.Tokenize(ligneEnCours, decoupage, "\t");
            c1 = atof(decoupage[0].c_str());
            l1 = atof(decoupage[1].c_str());
            c2 = atof(decoupage[2].c_str());
            l2 = atof(decoupage[3].c_str());

            if(!m_image1->GetZ(l1,c1,&z1))
                continue;
            if(!m_image2->GetZ(l2,c2,&z2))
                continue;

            int num1 = m_image1->FindPoint(l1, c1);
            int num2 = m_image2->FindPoint(l2, c2);

            if(num1 != 0) //point déjà existant sur la pano 1
            {
                sprintf(message,"Point %s deja trouve dans pano %s",st.itoa(num1).c_str(),m_image1->Nom().c_str());
                XErrorInfo(error,__FUNCTION__,message);
                AjoutPoint(num1, l2, c2, z2, m_image2);
                continue;
            }

            if(num2 != 0) //point déjà existant sur la pano 2
            {
                sprintf(message,"Point %s deja trouve dans pano %s",st.itoa(num2).c_str(),m_image2->Nom().c_str());
                XErrorInfo(error,__FUNCTION__,message);
                AjoutPoint(num2, l1, c1, z1, m_image1);
                continue;
            }

            //point non existant
            (*nbPoints) = (*nbPoints) + 1;
            AjoutPoint(*nbPoints, l1, c1, z1, m_image1);
            AjoutPoint(*nbPoints, l2, c2, z2, m_image2);
        }
    }
    file.close();

    //Suppression des cartes de profondeurs
    m_image1->CarteProfondeur().clear();
    m_image2->CarteProfondeur().clear();

    if(NbPointsApp() < 4)
    {
        (*nbPoints) = (*nbPoints) - NbPointsApp();
        Nettoyage(m_image1, m_image2);
        XErrorAlert(error,__FUNCTION__,"Nb de points coherent < 4, appariement non pris en compte");
        return false;
    }

    XErrorInfo(error,__FUNCTION__,"Nb points homologues : ",st.itoa(NbPointsApp()).c_str());
    return true;
}
//------------------------------------------------------------
bool Appariement::DejaPresent(std::vector<Point*> lstPts, Point* pt)
{
    for(int i=0 ; i<lstPts.size() ; i++)
    {
        if(lstPts[i]->NumPoint() == pt->NumPoint())
            return true;
    }
    return false;
}
//------------------------------------------------------------
std::vector<Point*> Appariement::ChoixQuatrePointsAleatoires(std::vector<Point*> points)
{
    int rd;
    srand(time(NULL));
    while(points.size() < 4)
    {
        rd = rand() % NbPointsApp() + 1;
        cout << "rand " << rd << endl;
        if(DejaPresent(points, m_listePoints[rd]))
            continue;

        points.push_back(m_listePoints[rd]);
    }

    return points;
}
