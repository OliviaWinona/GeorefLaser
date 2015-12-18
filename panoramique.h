#ifndef PANORAMIQUE_H
#define PANORAMIQUE_H
#include <iostream>
#include <fstream>
#include <vector>
#include <Eigen/Dense>
#include "point.h"


class Appariement;
class Chantier;
class Panoramique
{
public:
    Panoramique();
    ~Panoramique();

    void calculLargHaut(long &largeur, long &hauteur, std::string nom);
    long GetFileSize(std::ifstream &Fichier);
    long GetFileHauteur(long size);


    Chantier* monChantier;
    std::string nom;
    long largeur;
    long hauteur;
    //float carteProfondeur[][];
    std::vector<Appariement*> apparie;
    std::vector<Point*> tousPointsIm;
    Eigen::Matrix3f rotation;
    Eigen::Vector3f translation;
};

#endif // PANORAMIQUE_H
