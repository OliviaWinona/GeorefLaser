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
    Panoramique(Chantier _monChantier, std::string _nom, long _largeur, long _hauteur, std::initializer_list<Appariement*> _apparie, std::initializer_list<Point*> _tousPointsIm, Eigen::Matrix3f _rotation, Eigen::Vector3f _translation);
    ~Panoramique();

    void calculLargHaut(long &largeur, long &hauteur, std::string nom);
    long GetFileSize(std::ifstream &Fichier);
    long GetFileHauteur(long size);

    //Attributs
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
