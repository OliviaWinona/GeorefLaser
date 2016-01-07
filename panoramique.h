#ifndef PANORAMIQUE_H
#define PANORAMIQUE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <Eigen/Dense>

#include "point.h"
#include "libXBase/Xerror.h"

class Appariement;
class Chantier;
class Panoramique
{
protected:
    //Attributs
    std::string m_strNom;
    Chantier* pChantier;
    long largeur;
    long hauteur;
    std::vector<Appariement*> apparie;
    std::vector<Point*> tousPointsIm;
    Eigen::Matrix3f rotation;
    Eigen::Vector3f translation;
    //float carteProfondeur[][];

    //Méthode
    bool calculLargHaut();

public:
    Panoramique(Chantier* parent,std::string nom){pChantier = parent;m_strNom = nom;}
    ~Panoramique();

     //Attributs
    Chantier* Chantier();
    std::string Nom();

    bool Init(XError* error);
};

#endif // PANORAMIQUE_H
