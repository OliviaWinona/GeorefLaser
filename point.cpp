#include <Eigen/Dense>
#include <iostream>
#include "point.h"
#include "libXBase/XPt3D.h"

Point::Point(int num, float l, float c, float z)
{
    m_num = num;
    m_ligne = l;
    m_colonne = c;
    m_z = z;
}
//------------------------------------------------------------
Point::~Point()
{
}
//------------------------------------------------------------
Point Point::TransfPoint(Eigen::Matrix3d r, Eigen::Vector3d t, double e)
{
    Eigen::Vector3d transfo;
    Eigen::Vector3d origine(m_colonne,m_ligne,m_z);
    transfo = t + e*r*origine;
    Point pt(m_num, transfo(1),transfo(0),transfo(2));
    return pt;
}
//------------------------------------------------------------
double distance3D(Point pt1, Point pt2) //FONCTION A MODIF QUAND REMPLI COORDX ET COORDY
{
    XPt3D A((double)pt1.Colonne(), (double)pt1.Ligne(), (double)pt1.Hauteur());
    XPt3D B((double)pt2.Colonne(), (double)pt2.Ligne(), (double)pt2.Hauteur());
    return dist(A,B);
}
//------------------------------------------------------------
void AffichePoint(Point pt)
{
    std::cout << "num : " << pt.NumPoint() << ", colonne (x) : " << pt.Colonne() << ", ligne (y) : " << pt.Ligne() << ", profondeur (z) : " << pt.Hauteur() << std::endl;
}
