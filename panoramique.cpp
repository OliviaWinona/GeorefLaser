#include "panoramique.h"

#include <iostream>
#include <fstream>
#include <cmath>
#include <Eigen/Dense>
#include <string>
#include "appariement.h"
#include "chantier.h"


Panoramique::~Panoramique(){;}
//------------------------------------------------------------
std::string Panoramique::Nom(){return m_strNom;}
//------------------------------------------------------------
Chantier* Panoramique::Chantier(){return pChantier;}
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
     hauteur = GetFileHauteur(size);
     largeur = hauteur * 2;
    return true;
}
//------------------------------------------------------------
bool Panoramique::Init(XError* error)
{
    if(!calculLargHaut())
        return XErrorError(error,__FUNCTION__,"Erreur de calcul des dimensions ",m_strNom.c_str());
    return true;
}
