#ifndef CHANTIER_H
#define CHANTIER_H
#include <iostream>
#include <vector>

class Panoramique;
class Chantier
{
public:
    Chantier();
    ~Chantier();

    std::string nomDossier;
    int nbIm;
    std::vector<Panoramique*> listePano;

};

#endif // CHANTIER_H
