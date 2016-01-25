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
    for(unsigned int i=0 ; i<m_listePano.size() ; i++)
        delete m_listePano[i];
    for(unsigned int i=0 ; i<m_listeAppariement.size() ; i++)
        delete m_listeAppariement[i];
}
//------------------------------------------------------------
int Chantier::NbPanoramiques() {return m_listePano.size();}
//------------------------------------------------------------
//-----------------Partie Chargement des panos----------------
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
//-----------------Partie Création des .key-------------------
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
//----------------Partie Création des .result-----------------
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
//--------------Partie Chargement des .result-----------------
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
//-------------------Partie Orientation-----------------------
//------------------------------------------------------------
Appariement* Chantier::NonTraite()
{
    Appariement* app = 0;
    for(unsigned int i=0 ; i<m_listeAppariement.size() ; i++)
    {
        if(m_listeAppariement[i]->traite)
            continue;
        app = m_listeAppariement[i];
        break;
    }
    return app; //est un pointeur vers NULL si tout est déjà traité
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
    //cout << "rotation : " << endl << app->rot_app << endl;
    if(!app->TestRotation())
    {
        XErrorAlert(m_error,__FUNCTION__,"pas rotation 2D, on recommence avec de nouveaux points");
        return false;
    }
//    if(!app->TestDistance())
//    {
//        XErrorAlert(m_error,__FUNCTION__,"transformation non valide, on recommence");
//        return false;
//    }
    XErrorInfo(m_error,__FUNCTION__,"Transfo ok !");
    return true;
}
//------------------------------------------------------------
Appariement* Chantier::PlusPointsCommun(std::string nom, std::vector<Panoramique*> pano_prec)
{
    Appariement* meilleur_app = 0;
    for(unsigned int i=0 ; i<m_listeAppariement.size() ; i++)
    {
        if((m_listeAppariement[i]->Pano1()->Nom() != nom) && (m_listeAppariement[i]->Pano2()->Nom() != nom))
            continue;
        if(m_listeAppariement[i]->utilise)
            continue;
        if((meilleur_app == 0) || (m_listeAppariement[i]->NbPointsApp() > meilleur_app->NbPointsApp()))
            meilleur_app = m_listeAppariement[i];
    }
    std::string nom_pano;
    bool tmp = false;
    for(unsigned int i=pano_prec.size() ; i>0 ; i--)
    {
        nom_pano = pano_prec[i-1]->Nom();
        for(unsigned int j=0 ; j<m_listeAppariement.size() ; j++)
        {
            if(m_listeAppariement[j]->utilise)
                continue;
            if((m_listeAppariement[j]->Pano1()->Nom() != nom_pano) && (m_listeAppariement[j]->Pano2()->Nom() != nom_pano))
                continue;
            if(m_listeAppariement[j]->NbPointsApp() > meilleur_app->NbPointsApp())
            {
                meilleur_app = m_listeAppariement[j];
                tmp = true;
            }
        }
        if (tmp)
        {
            pano_prec.erase(pano_prec.end()-pano_prec.size()+i, pano_prec.end());
            break;
        }
    }
    meilleur_app->utilise = true;
    return meilleur_app;
}
//------------------------------------------------------------
bool Chantier::Cheminement()
{
    int nbPano = m_listePano.size();
    Appariement* app;
    Panoramique* pano;
    std::vector<Panoramique*> panos_precedentes;
    for(int i=0 ; i<nbPano ; i++)
    {
        if(i == 0)
        {
            app = PlusPointsCommun(m_listePano[0]->Nom(), panos_precedentes);
            m_listePano[0]->valide = true;
            panos_precedentes.push_back(m_listePano[0]);

            pano = app->Pano2();
//            *pano->Rotation() = app->rot_app;
//            *pano->Translation() = app->trans_app;
//            *pano->Echelle() = app->echelle_app;
//            pano->valide = true;
            continue;
        }
        cout << pano->Nom() << endl;

        int signe = 1;
        for(unsigned int j=panos_precedentes.size() ; j>0 ; j--)
        {
            //Attention au sens de la transformation
            if(app->Pano2()->Nom() == panos_precedentes[j-1]->Nom())
                signe = -1;

            //passage des coordonnées de la pano dans le repère de la pano précédente
            *pano->Rotation() = *pano->Rotation() * signe * app->rot_app;
            *pano->Translation() = *pano->Translation() + signe * app->trans_app;
            *pano->Echelle() = *pano->Echelle() * signe* app->echelle_app;
        }
        app = PlusPointsCommun(pano->Nom(), panos_precedentes);
        if(app->Pano1()->Nom() == pano->Nom())
            pano = app->Pano2();
        else
            pano = app->Pano1();
    }
    return true;
}
//------------------------------------------------------------
bool Chantier::Orientation()
{
    Appariement* app;
    srand(time(NULL));
    char message[1024];
    while(app = NonTraite())
    {
        sprintf(message,"%s et %s ",app->Pano1()->Nom().c_str(), app->Pano2()->Nom().c_str());
        XErrorInfo(m_error,__FUNCTION__,message);
        if(!Compensation(app)) // si un des tests n'est pas bon, on recommence
            continue;
        app->traite = true;
    }
    if(!Cheminement())
        XErrorAlert(m_error,__FUNCTION__,"echec de la complétion des matrices de rotation, translation et echelle");

    return true;
}
