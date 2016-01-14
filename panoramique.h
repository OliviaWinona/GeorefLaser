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

public:
    Panoramique(Chantier* parent,std::string nom){pChantier = parent;m_strNom = nom;}
    ~Panoramique();

    //Attributs
    Chantier* Chantier() {return pChantier;}
    std::string Nom() {return m_strNom;}
    long Hauteur() {return m_hauteur;}
    long Largeur() {return m_largeur;}
    std::vector<Point*> tousPointsIm() {return m_tousPointsIm;}
    std::vector<float> CarteProfondeur() {return m_carteProfondeur;}

    //Méthodes
    bool Init(XError* error);
    bool ChargeCarteProfondeur();
    bool GetZ(float l, float c, float* z);
    int FindPoint(float l, float c);
};

#endif // PANORAMIQUE_H
