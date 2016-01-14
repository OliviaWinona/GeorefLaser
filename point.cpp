#include "point.h"

Point::Point(int num, float l, float c, float z)
{
    m_num = num;
    m_ligne = l;
    m_colonne = c;
    m_z = z;
}

Point::~Point()
{
}
