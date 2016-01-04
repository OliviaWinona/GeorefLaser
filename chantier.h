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
    Chantier(std::string _nomDossier, int _nbIm, std::initializer_list<Panoramique*> _lstPano);
    ~Chantier();

    // Méthodes
    //void CreationListePano(std::string dossier, XErrorConsole &m_error);
    bool CreationListePano(std::string dossier);

    // Attributs
    std::string nomDossier;
    int nbIm = 0;
    std::vector<Panoramique*> listePano;

};

#endif // CHANTIER_H
