#include "panoramique.h"

#include <iostream>
#include <fstream>
#include <cmath>
#include <Eigen/Dense>
#include <string>
#include "libXBase/XStringTools.h"
#include "libXBase/XPt3D.h"
#include "appariement.h"
#include "chantier.h"


Panoramique::~Panoramique(){;}
//------------------------------------------------------------
//Chantier* Panoramique::Chantier(){return pChantier;}
//------------------------------------------------------------

//------------------------------------------------------------
long GetFileSize(std::ifstream &Fichier)
{
    //Sauvegarder position courante
    long pos = Fichier.tellg();
    //Se placer en fin de fichier
    Fichier.seekg(0, std::ios_base::end);
    //Récupérer la nouvelle position = taille du fichier
    long size = Fichier.tellg();
    //Restaurer la position initiale du fichier
    Fichier.seekg(pos, std::ios_base::beg);
    return size;
}
//------------------------------------------------------------
long GetFileHauteur(long size)
{
    //Pour fichier binaire sur 4 octets
    return sqrt(size / 8);
}
//------------------------------------------------------------
bool Panoramique::calculLargHaut()
{
    std::string name = pChantier->Dossier() + "\\" + m_strNom + ".bin";
    std::ifstream file(name.c_str(), std::ios::in|std::ios::binary);
    if (!file.is_open())
        return false;

    long size = GetFileSize(file);
    m_hauteur = GetFileHauteur(size);
    m_largeur = m_hauteur * 2;
    file.close();
    return true;
}
//------------------------------------------------------------
bool Panoramique::ChargeCarteProfondeur()
{
    std::string name = pChantier->Dossier() + "\\" + Nom()+".bin";
    std::ifstream file(name.c_str(), std::ios::in|std::ios::binary);
    if(!file.is_open())
        return false;

    m_carteProfondeur.resize(m_largeur * m_hauteur,0);
    file.read((char*)&m_carteProfondeur[0], sizeof(float)*m_largeur*m_hauteur);
    file.close();
    return true;
}
//------------------------------------------------------------
bool Panoramique::Init(XError* error)
{
    if(!calculLargHaut())
        return XErrorError(error,__FUNCTION__,"Erreur de calcul des dimensions ",m_strNom.c_str());

    return true;
}
//------------------------------------------------------------
bool Panoramique::GetZ(float l, float c, float* z)
{
    if(m_carteProfondeur[c+l*m_largeur] == 0)
        return false;
    (*z) = m_carteProfondeur[c+l*m_largeur];
    return true;
}
//------------------------------------------------------------
int Panoramique::GetNum(float l, float c)
{
    for(unsigned int i=0 ; i<m_tousPointsIm.size() ; i++)
    {
        if (m_tousPointsIm[i]->Ligne() == l && m_tousPointsIm[i]->Colonne() == c)
            return m_tousPointsIm[i]->NumPoint();
    }
    return 0;
}
//------------------------------------------------------------
XPt3D Panoramique::GetPoint(int num)
{
    //std::cout << "taille " << m_tousPointsIm.size() << std::endl;
    //std::cout << "num debut fonction" << num << std::endl;
    for(unsigned int i=0 ; i<m_tousPointsIm.size() ; i++)
    {
        //std::cout << "num " << m_tousPointsIm[i]->NumPoint() << std::endl;
        if (m_tousPointsIm[i]->NumPoint() == num)
            //std::cout << "x " << m_tousPointsIm[i]->Colonne() << std::endl;
            return XPt3D((double)m_tousPointsIm[i]->Colonne(), (double)m_tousPointsIm[i]->Ligne(), (double)m_tousPointsIm[i]->Profondeur());
    }
    return NULL;
}
