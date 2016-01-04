#include <iostream>
#include <vector>
#include <dirent.h>
#include <stdio.h>
#include <libXFileSystem/XSystemInfo.h>
#include <libXBase/XErrorConsole.h>

#include "chantier.h"
#include "panoramique.h"

using namespace std;

Chantier::Chantier() : nomDossier(""),
                       nbIm(0),
                       listePano()
{
    cout << "constructeur par défaut de Chantier" << endl;
}

Chantier::Chantier(std::string _nomDossier, int _nbIm, std::initializer_list<Panoramique*> _lstPano) :
                    nomDossier(_nomDossier),
                    nbIm(_nbIm),
                    listePano(_lstPano)
{
    cout << "constructeur chantier" << endl;
}

Chantier::~Chantier()
{
}

//void Chantier::CreationListePano(std::string dossier, XErrorConsole *m_error)
bool Chantier::CreationListePano(std::string dossier)
{
    //Liste le contenu du dossier
    std::vector<std::string> contenuDossier;
    DIR * rep = opendir(dossier.c_str());
    if (rep != NULL){
       struct dirent * ent;
       while ((ent = readdir(rep)) != NULL){
           contenuDossier.push_back(ent->d_name);
       }
       closedir(rep);
    }

    //Recherche les images en .tif
    std::string extention_tif = ".tif";
    for (int i=0; i < contenuDossier.size(); i++){
        if (std::equal(extention_tif.rbegin(), extention_tif.rend(), contenuDossier[i].rbegin())){
            nbIm ++;
            std::string nomPano = contenuDossier[i].substr(0, contenuDossier[i].size()-4);
            cout << "Nom de l'image : " << nomPano << endl;
            //Vérifier si elle a bien une carte de profondeur associée
            std::string nomCarteProfondeur = nomPano+".bin";
            if (std::find(contenuDossier.begin(), contenuDossier.end(), nomCarteProfondeur) != contenuDossier.end()){
                cout << "ok" << endl;
                //Création par défaut de pano avec juste le chantier et le nom de la pano
                //pano = new Panoramique(dossier, );
            }else{
                cout << "Il manque la carte de profondeur pour l'image " << nomPano << ".tif" << endl;
            }
        }
    }
    if (nbIm == 0){
        cout << "Il n'y a pas d'image dans ce dossier" << endl;
        return false;
    }
    cout << nbIm << endl;
    return true;
}
