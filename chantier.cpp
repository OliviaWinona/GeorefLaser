#include "chantier.h"

#include <iostream>
#include <vector>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <time.h>

#include "libXFileSystem/XSystemInfo.h"
#include "libXFileSystem/XPath.h"
#include "libXBase/XPt3D.h"

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
    for(int i=0; i< NbPanoramiques(); i++)
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

    for (unsigned int i=0; i < contenuDossier.size(); i++){
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
bool Chantier::CreationKey()
{
    char commande[1024];
    std::string chemin = m_strNomDossier+"\\";
    for(unsigned int i=0 ; i<m_listePano.size() ; i++)
    {
        XErrorInfo(m_error,__FUNCTION__,"Creation du .key pour la pano : ", m_listePano[i]->Nom().c_str());
        sprintf(commande,"cd /d %s && siftpp_tgi.exe %s.tif [--first-octave 0] --verbose", chemin.c_str(), m_listePano[i]->Nom().c_str());
        XErrorInfo(m_error,__FUNCTION__, commande);
        std::system(commande);
    }
    return true;
}
//------------------------------------------------------------
//------------------------------------------------------------
bool Chantier::CreationResult()
{
    char message[1024];
    char commande[1024];
    std::string chemin = m_strNomDossier+"\\";
    for(unsigned int i=0; i<m_listePano.size() ; i++)
    {
        for(unsigned int j=i ; j<m_listePano.size() ; j++)
        {
            if(i==j)
                continue;
            sprintf(message,"Creation du .result : %s.-.%s.result", m_listePano[i]->Nom().c_str(), m_listePano[j]->Nom().c_str());
            XErrorInfo(m_error,__FUNCTION__,message);
            sprintf(commande,"cd /d %s && ann_lowe.exe --kp1 %s.key --kp2 %s.key --verbose", chemin.c_str(), m_listePano[i]->Nom().c_str(), m_listePano[j]->Nom().c_str());
            std::system(commande);
        }
    }
    return true;
}
//------------------------------------------------------------
//------------------------------------------------------------
Panoramique* Chantier::FindPano(std::string nom)
{
    for (unsigned int i=0 ; i<m_listePano.size() ; i++)
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
    app->Pano1()->TousAppariementIm().push_back(app);
    app->Pano2()->TousAppariementIm().push_back(app);
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

    for (unsigned int i=0; i < contenuDossier.size(); i++)
        if(P.Extension(contenuDossier[i].c_str()) == "result")
            AddResult(contenuDossier[i]);

    if (m_listeAppariement.size() == 0)
        return XErrorError(m_error,__FUNCTION__,"pas de result valide dans ce dossier ",dossier.c_str());

    XErrorInfo(m_error,__FUNCTION__,"Nb d'appariements : ",st.itoa(m_listeAppariement.size()).c_str());
    return true;
}
//------------------------------------------------------------
//------------------------------------------------------------
Appariement* Chantier::PlusPointsCommun()
{
    Appariement* meilleur_app = 0; //pas encore de meilleur
    for(unsigned int i=0 ; i<m_listeAppariement.size() ; i++)
    {
        if(m_listeAppariement[i]->traite)
            continue;
        if((meilleur_app==0)||(m_listeAppariement[i]->NbPointsApp() > meilleur_app->NbPointsApp()))
            meilleur_app = m_listeAppariement[i];
    }
    return meilleur_app; //est un pointeur vers NULL si tout est déjà traité
}
//------------------------------------------------------------
bool Chantier::Compensation(Appariement* app)
{
    std::vector<Point*> pointsAleatoires;
    pointsAleatoires = app->ChoixQuatrePointsAleatoires(pointsAleatoires);
    std::vector<XPt3D> pointsPano1, pointsPano2;
    for(unsigned int i=0 ; i<pointsAleatoires.size() ; i++)
    {
        pointsPano1.push_back(app->Pano1()->GetPointXPt3D(pointsAleatoires[i]->NumPoint()));
        pointsPano2.push_back(app->Pano2()->GetPointXPt3D(pointsAleatoires[i]->NumPoint()));
    }
    app->Thomson_Shut(m_error, pointsPano1, pointsPano2, &app->rot_app, &app->trans_app, &app->echelle_app);
    if(!app->TestEchelle())
    {
        XErrorAlert(m_error,__FUNCTION__,"echelle non valide, on recommence avec de nouveaux points");
        return false;
    }
    cout << "rotation : " << endl << app->rot_app << endl;
    if(!app->TestRotation())
    {
        XErrorAlert(m_error,__FUNCTION__,"pas rotation 2D, on recommence avec de nouveaux points");
        return false;
    }
    std::vector<Point> ptsCompenses;
    ptsCompenses = app->PointsCompense();
    if(!app->TestDistance(ptsCompenses))
    {
        XErrorAlert(m_error,__FUNCTION__,"transformation non valide, on recommence");
        return false;
    }
    return true;
}
//------------------------------------------------------------
bool Chantier::Orientation()
{
    Appariement* app;
    srand(time(NULL));
    char message[1024];
    while(app = PlusPointsCommun())
    {
        sprintf(message,"%s et %s : ",app->Pano1()->Nom().c_str(), app->Pano2()->Nom().c_str());
        XErrorInfo(m_error,__FUNCTION__,message);
        if(!Compensation(app)) // si un des tests n'est pas bon, on recommence
            continue;
        app->traite = true;
    }
    return true;
}
