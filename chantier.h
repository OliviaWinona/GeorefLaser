#ifndef CHANTIER_H
#define CHANTIER_H

#include <iostream>
#include <vector>
#include <libXBase/XError.h>
#include <libXBase/XStringTools.h>
#include <libXFileSystem/XPath.h>
#include "appariement.h"

class Panoramique;
class Chantier
{
protected:
    //Attributs
    XError* m_error;
    std::string m_strNomDossier;
    std::vector<Panoramique*> m_listePano;
    std::vector<Appariement*> m_listeAppariement;
    int m_nbPoints;

    //Outils
    XStringTools st;
    XPath P;

    //Méthodes internes
    bool InitPanos();
    bool AddResult(std::string fileResult);
    Panoramique* FindPano(std::string nom);


public:
    Chantier(XError* error);
    ~Chantier();

    // Méthodes
    bool ChargePano(std::string dossier);
    bool ChargeResult(std::string dossier);

    // Attributs
    int NbPanoramiques() ;
    std::string Dossier(){return m_strNomDossier;}
    int NbPoints() {return m_nbPoints;}

};

#endif // CHANTIER_H
