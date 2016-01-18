#ifndef APPARIEMENT_H
#define APPARIEMENT_H

#include <iostream>
#include <vector>
#include "libXBase/XError.h"
#include "libXBase/XPt3D.h"
#include "panoramique.h"
#include "point.h"

class Appariement
{
protected:
    // Attributs
    Panoramique * m_image1;
    Panoramique * m_image2;
    std::vector<Point*> m_listePoints;

    // Méthodes internes
    bool Nettoyage(Panoramique* pano1, Panoramique *pano2);
    bool AjoutPoint(int num, float l, float c, float z, Panoramique* pano);
    bool DejaPresent(std::vector<Point*> lstPts, Point* pt);
    void CalculCoefAxiateur(double a,double b,double c,double theta,Eigen::Matrix3d *R);

public:
    Appariement(Panoramique* pano1, Panoramique* pano2);
    ~Appariement();

    // Attributs
    Panoramique* Pano1() {return m_image1;}
    Panoramique* Pano2() {return m_image2;}
    bool traite = false;

    // Méthodes
    bool ChargeMesures(XError* error, std::string FileResult, int *nbPoints);
    int NbPointsApp();
    std::vector<Point *> ChoixQuatrePointsAleatoires(std::vector<Point*> points);
    bool Thomson_Shut(XError *error, std::vector<XPt3D> &ptPano1, std::vector<XPt3D> &ptPano2, Eigen::Matrix3d *R, Eigen::Vector3d *T, double *e);

};

#endif // APPARIEMENT_H
