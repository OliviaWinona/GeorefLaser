#ifndef APPARIEMENT_H
#define APPARIEMENT_H

#include <iostream>
#include <vector>
#include <Eigen/Dense>
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
    std::vector<Point*> m_listePointsPano1;
    std::vector<Point*> m_listePointsPano2;

    // Méthodes internes
    bool Nettoyage(Panoramique* pano1, Panoramique *pano2);
    bool DejaPresent(std::vector<Point*> lstPts, Point* pt);
    std::vector<Point*> ChoixQuatrePointsAleatoires(std::vector<Point*> points);
    void CalculCoefAxiateur(double a,double b,double c,double theta,Eigen::Matrix3d *R);
    bool Thomson_Shut(XError *error, std::vector<XPt3D> &ptPano1, std::vector<XPt3D> &ptPano2, Eigen::Matrix3d *R, Eigen::Vector3d *T, double *e);
    bool TestRotation();
    bool TestEchelle();
    std::vector<Point> PointsCompense();
    bool TestDistance();

public:
    Appariement(Panoramique* pano1, Panoramique* pano2);
    ~Appariement();

    // Attributs
    Panoramique* Pano1() {return m_image1;}
    Panoramique* Pano2() {return m_image2;}
    std::vector<Point*> & PointsPano1() {return m_listePointsPano1;}
    std::vector<Point*> & PointsPano2() {return m_listePointsPano2;}

    bool traite = false;
    Eigen::Matrix3d rot_app;
    Eigen::Vector3d trans_app;
    double echelle_app;
    bool utilise = false;

    // Méthodes
    int NbPointsApp();
    bool ChargeMesures(XError* error, std::string FileResult, int *nbPoints);
    bool Compensation(XError* error);

};

#endif // APPARIEMENT_H
