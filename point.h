#ifndef POINT_H
#define POINT_H
#include <iostream>
#include <vector>
#include <Eigen/Dense>

class Point
{
protected:
    //Attribut
    int m_num;
    float m_ligne;
    float m_colonne;
    float m_x;
    float m_y;
    float m_z;

public:
    Point(int num, float l, float c, float z);
    ~Point();

    //Attributs
    int NumPoint() {return m_num;}
    float Ligne() {return m_ligne;} //y
    float Colonne() {return m_colonne;} //x
    float CoordX() {return m_x;}
    float CoordY() {return m_y;}
    float Hauteur() {return m_z;} // z

    //Méthodes
    Point TransfPoint(Eigen::Matrix3d r, Eigen::Vector3d t, double e);


};

#endif // POINT_H

void AffichePoint(Point pt);
double distance3D(Point pt1, Point pt2);
