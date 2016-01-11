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
    Point(int num, int l, int c);
    ~Point();

    //Attributs
    int NumPoint() {return m_num;}

};

#endif // POINT_H
