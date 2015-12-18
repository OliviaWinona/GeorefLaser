#include <iostream>
#include <fstream>
#include <cmath>
#include "panoramique.h"
#include "appariement.h"
#include "chantier.h"

using namespace std;

Panoramique::Panoramique()
{
}

Panoramique::~Panoramique()
{
}

long Panoramique::GetFileSize(std::ifstream &Fichier)
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

long Panoramique::GetFileHauteur(long size)
{
    //Pour fichier binaire sur 4 octets
    return sqrt(size / 8);
}

void Panoramique::calculLargHaut(long &largeur, long &hauteur, std::string filename)
{

    std::string name = monChantier->nomDossier + "\\" + filename + ".bin";
    std::ifstream file(name.c_str(), std::ios::in|std::ios::binary);
    if (file.is_open())
    {
        long size = GetFileSize(file);
        hauteur = GetFileHauteur(size);
        largeur = hauteur * 2;
    }
    else
    {
        cout<<"erreur ouverture"<<endl;
    }
}
