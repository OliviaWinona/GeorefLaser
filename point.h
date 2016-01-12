#ifndef POINT_H
#define POINT_H
#include <iostream>
#include <vector>

class Point
{
protected:
    //Attribut
    int m_num;
    float m_ligne;
    float m_colonne;
    float m_z;

public:
    Point(int num, float l, float c);
    ~Point();

    //Attributs
    int numPoint() {return m_num;}
    float ligne() {return m_ligne;}
    float colonne() {return m_colonne;}

};

#endif // POINT_H
