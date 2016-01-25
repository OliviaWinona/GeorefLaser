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
    int m_nbPoints = 0;

    //Outils
    XStringTools st;
    XPath P;

    //Méthodes internes
    bool InitPanos();
    bool AddResult(std::string fileResult);
    Panoramique* FindPano(std::string nom);
    Appariement* PlusPointsCommun(std::string nom, std::vector<Panoramique*> pano_prec);
    Appariement* NonTraite();
    bool Compensation(Appariement *app);
    bool TestDistance(std::vector<Point*> pts1, std::vector<Point> pts2);
    bool Cheminement();

public:
    Chantier(XError* error);
    ~Chantier();

    // Attributs
    std::string Dossier(){return m_strNomDossier;}
    int NbPoints() {return m_nbPoints;}
    XError* Error() {return m_error;}

    // Méthodes
    bool ChargePano(std::string dossier);
    bool CreationKey();
    bool CreationResult();
    bool ChargeResult(std::string dossier);
    int NbPanoramiques();
    bool Orientation();
};

#endif // CHANTIER_H
