#ifndef CHANTIER_H
#define CHANTIER_H
#include <iostream>
#include <vector>
#include <libXBase/XErrorConsole.h>

class Panoramique;
class Chantier
{
public:
    Chantier();
    ~Chantier();

    // Méthodes
    //void CreationListePano(std::string dossier, XErrorConsole &m_error);
    void CreationListePano(std::string dossier);

    // Attributs
    std::string nomDossier;
    int nbIm;
    std::vector<Panoramique*> listePano;

};

#endif // CHANTIER_H
