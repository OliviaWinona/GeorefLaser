#include "panoramique.h"

#include <iostream>
#include <fstream>
#include <cmath>
#include <Eigen/Dense>
#include <string>
#include <libXBase/XStringTools.h>
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
}
//------------------------------------------------------------
bool Panoramique::Init(XError* error)
{
    if(!calculLargHaut())
        return XErrorError(error,__FUNCTION__,"Erreur de calcul des dimensions ",m_strNom.c_str());

    return true;
}
