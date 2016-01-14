#include "chantier.h"

#include <iostream>
#include <vector>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>

#include "libXFileSystem/XSystemInfo.h"
#include "libXFileSystem/XPath.h"

#include "panoramique.h"
#include "appariement.h"

using namespace std;

Chantier::Chantier(XError* error) : m_strNomDossier("")
{
    m_error = error;
}
//------------------------------------------------------------
Chantier::~Chantier()
{
}
//------------------------------------------------------------
int Chantier::NbPanoramiques() {return m_listePano.size();}
//------------------------------------------------------------
bool Chantier::InitPanos()
{
    bool ok;
    for(uint32 i=0; i< NbPanoramiques(); i++)
    {
        ok = m_listePano[i]->Init(m_error);
    }
    return ok;
}
//------------------------------------------------------------
bool Chantier::ChargePano(std::string dossier)
{
    XErrorInfo(m_error,__FUNCTION__,"Analyse du dossier",dossier.c_str());
    std::vector<std::string> contenuDossier;
    DIR * rep = opendir(dossier.c_str());
    if (rep != NULL)
    {
        struct dirent * ent;
        while ((ent = readdir(rep)) != NULL)
        {
            contenuDossier.push_back(ent->d_name);
        }
        closedir(rep);
    }

    char message[1024];
    sprintf(message,"%d fichiers",contenuDossier.size());
    XErrorInfo(m_error, __FUNCTION__,message,dossier.c_str());

    for (int i=0; i < contenuDossier.size(); i++){
        if(P.Extension(contenuDossier[i].c_str()) == "tif"){
            std::string nomPano = P.NameNoExt(contenuDossier[i].c_str());
            std::string nomCarteProfondeur = nomPano+".bin";
            if(std::find(contenuDossier.begin(), contenuDossier.end(), nomCarteProfondeur) == contenuDossier.end())
            {
                XErrorAlert(m_error,__FUNCTION__,"manque la carte de profondeur pour l'image ",nomPano.c_str());
                continue;
            }
            m_listePano.push_back(new Panoramique(this,nomPano));
            sprintf(message,"Creation de la pano : %s",nomPano.c_str());
            XErrorInfo(m_error,__FUNCTION__,message);
        }
    }
    if (m_listePano.size() == 0)
        return XErrorError(m_error,__FUNCTION__,"pas d'image dans ce dossier ",dossier.c_str());

    m_strNomDossier = dossier;
    XErrorInfo(m_error,__FUNCTION__,"NB images",st.itoa(m_listePano.size()).c_str());
    if (m_listePano.size() < 2)
        return XErrorError(m_error,__FUNCTION__,"Il n'y a pas assez d'images");
    return InitPanos();
}
//------------------------------------------------------------
//------------------------------------------------------------
Panoramique* Chantier::FindPano(std::string nom)
{
    for (int i=0 ; i<m_listePano.size() ; i++)
    {
        if (m_listePano[i]->Nom() == nom)
            return m_listePano[i];
    }
    return NULL;
}
//------------------------------------------------------------
bool Chantier::AddResult(std::string fileResult)
{
    std::string name = fileResult.substr(0, fileResult.size()-7);
    //decoupage du nom
    std::vector<std::string> decoupage;
    std::string delimiteur = ".-.";
    st.Tokenize(name, decoupage, delimiteur);
    std::string nom1 = decoupage[0];
    std::string nom2 = decoupage[1];

    Panoramique* pano1 = FindPano(nom1);
    if(pano1 == NULL)
        return XErrorError(m_error,__FUNCTION__,"Pano non trouve ",nom1.c_str());
    Panoramique* pano2 = FindPano(nom2);
    if(pano2 == NULL)
        return XErrorError(m_error,__FUNCTION__,"Pano non trouve ",nom2.c_str());
    Appariement* app = new Appariement(pano1,pano2);

    if(!app->ChargeMesures(m_error, fileResult, &m_nbPoints))
    {
        delete app;
        return true;
    }
    m_listeAppariement.push_back(app);
    return true;
}
//------------------------------------------------------------
bool Chantier::ChargeResult(std::string dossier)
{
    std::vector<std::string> contenuDossier;
    DIR * rep = opendir(dossier.c_str());
    if (rep != NULL)
    {
        struct dirent * ent;
        while ((ent = readdir(rep)) != NULL)
        {
            contenuDossier.push_back(ent->d_name);
        }
        closedir(rep);
    }

    for (int i=0; i < contenuDossier.size(); i++)
        if(P.Extension(contenuDossier[i].c_str()) == "result")
            AddResult(contenuDossier[i]);

    if (m_listeAppariement.size() == 0)
        return XErrorError(m_error,__FUNCTION__,"pas de result valide dans ce dossier ",dossier.c_str());

    return true;
}
//------------------------------------------------------------
//------------------------------------------------------------
Appariement* Chantier::PlusPointsCommum()
{
    Appariement* app = m_listeAppariement[0];
    for(int i=0 ; i<m_listeAppariement.size() ; i++)
    {
        if(m_listeAppariement[i]->NbPointsApp() > app->NbPointsApp())
            app = m_listeAppariement[i];
    }
    return app;
}
//------------------------------------------------------------
bool Chantier::Orientation()
{
    XErrorInfo(m_error,__FUNCTION__,"Nb d'appariements : ",st.itoa(m_listeAppariement.size()).c_str());
    Appariement* app = PlusPointsCommum();

    std::vector<Point*> pointsAleatoires;
    pointsAleatoires = app->ChoixQuatrePointsAleatoires(pointsAleatoires);
    cout << pointsAleatoires.size() << endl;

    return true;
}
