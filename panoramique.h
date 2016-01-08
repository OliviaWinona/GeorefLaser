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
    long m_largeur;
    long m_hauteur;
    std::vector<Appariement*> m_apparie;
    std::vector<Point*> m_tousPointsIm;
    Eigen::Matrix3f m_rotation;
    Eigen::Vector3f m_translation;
    std::vector<float> m_carteProfondeur;

    //Méthode
    bool calculLargHaut();
    bool ChargeCarteProfondeur();

public:
    Panoramique(Chantier* parent,std::string nom){pChantier = parent;m_strNom = nom;}
    ~Panoramique();

    // Surcharge
    //bool operator==(Panoramique & P2){return m_strNom == P2.Nom();}

    //Attributs
    Chantier* Chantier();
    std::string Nom() {return m_strNom;}
    long Hauteur() {return m_hauteur;}
    long Largeur() {return m_largeur;}

    //Méthodes
    bool Init(XError* error);
};

#endif // PANORAMIQUE_H
