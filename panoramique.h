#ifndef PANORAMIQUE_H
#define PANORAMIQUE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <Eigen/Dense>

#include "point.h"
#include "libXBase/Xerror.h"
#include "libXBase/XPt3D.h"

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
    Eigen::Matrix3d m_rotation = Eigen::Matrix3d::Identity();
    Eigen::Vector3d m_translation = Eigen::Vector3d::Zero();
    double m_echelle = 1;
    std::vector<float> m_carteProfondeur;

    //Méthode
    bool calculLargHaut();
    XError* Error();

public:
    Panoramique(Chantier* parent,std::string nom){pChantier = parent;m_strNom = nom;}
    ~Panoramique();

    //Attributs
    Chantier* Chantier() {return pChantier;}
    std::string Nom() {return m_strNom;}
    long Hauteur() {return m_hauteur;}
    long Largeur() {return m_largeur;}
    std::vector<Point*> & tousPointsIm() {return m_tousPointsIm;}
    std::vector<float> & CarteProfondeur() {return m_carteProfondeur;}
    std::vector<Appariement*> & TousAppariementIm() {return m_apparie;}
    Eigen::Matrix3d* Rotation() {return &m_rotation;}
    Eigen::Vector3d* Translation() {return &m_translation;}
    double* Echelle() {return &m_echelle;}
    bool valide = false;

    //Méthodes
    bool Init(XError* error);
    bool ChargeCarteProfondeur();
    bool GetZ(int l, int c, float* z);
    Point* GetPt(float l, float c);
    XPt3D GetPointXPt3D(int num);
    bool AjoutPoint(Point* pt);
    bool EcrireXYZ();
};

#endif // PANORAMIQUE_H
