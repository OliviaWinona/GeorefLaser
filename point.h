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
    Point(int num, float l, float c, float z);
    ~Point();

    //Attributs
    int NumPoint() {return m_num;}
    float Ligne() {return m_ligne;} //y
    float Colonne() {return m_colonne;} //x
    float Profondeur() {return m_z;} // z

};

#endif // POINT_H
