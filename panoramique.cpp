#include "panoramique.h"

#include <iostream>
#include <fstream>
#include <cmath>
#include <Eigen/Dense>
#include <string>
#include "libXBase/XStringTools.h"
#include "libXBase/XPt3D.h"
#include "appariement.h"
#include "chantier.h"

using namespace std;

Panoramique::~Panoramique()
{
    for(unsigned int i=0 ; i<m_tousPointsIm.size() ; i++)
        delete m_tousPointsIm[i];
}
//------------------------------------------------------------
XError* Panoramique::Error()
{
    if(pChantier == NULL)
        return NULL;
    return pChantier->Error();
}

//------------------------------------------------------------
long GetFileSize(std::ifstream &Fichier)
{
    //Sauvegarder position courante
    long pos = Fichier.tellg();
    //Se placer en fin de fichier
    Fichier.seekg(0, std::ios_base::end);
    //Récupérer la nouvelle position = taille du fichier
    long size = Fichier.tellg();
    //Restaurer la position initiale du fichier
    Fichier.seekg(pos, std::ios_base::beg);
    return size;
}
//------------------------------------------------------------
long GetFileHauteur(long size)
{
    //Pour fichier binaire sur 4 octets
    return sqrt(size / 8);
}
//------------------------------------------------------------
bool Panoramique::calculLargHaut()
{
    std::string name = pChantier->Dossier() + "\\" + m_strNom + ".bin";
    std::ifstream file(name.c_str(), std::ios::in|std::ios::binary);
    if (!file.is_open())
        return false;

    long size = GetFileSize(file);
    m_hauteur = GetFileHauteur(size);
    m_largeur = m_hauteur * 2;
    file.close();
    return true;
}
//------------------------------------------------------------
bool Panoramique::ChargeCarteProfondeur()
{
    std::string name = pChantier->Dossier() + "\\" + Nom()+".bin";
    std::ifstream file(name.c_str(), std::ios::in|std::ios::binary);
    if(!file.is_open())
        return false;

    m_carteProfondeur.resize(m_largeur * m_hauteur,0);
    file.read((char*)&m_carteProfondeur[0], sizeof(float)*m_largeur*m_hauteur);
    file.close();

    //Ecriture de la carte de profondeur dans un fichier texte : Debbug
//    ofstream fichierOut;
//    std::string nomFichierOut = "test"+ m_strNom + ".txt";
//    fichierOut.open(nomFichierOut.c_str());
//    if(!fichierOut.is_open())
//        return false;
//    for(int l=0 ; l<m_hauteur ; l++)
//    {
//        for(int c=0 ; c<m_largeur ; c++)
//            fichierOut << "c" << c << " " <<m_carteProfondeur[c+l*m_largeur] << " ";
//        fichierOut << "\n";
//    }
//    fichierOut.close();

    return true;
}
//------------------------------------------------------------
bool Panoramique::Init(XError* error)
{
    if(!calculLargHaut())
        return XErrorError(error,__FUNCTION__,"Erreur de calcul des dimensions ",m_strNom.c_str());

    return true;
}
//------------------------------------------------------------
bool Panoramique::GetZ(int l, int c, float* z)
{
    char message[1024];
    sprintf(message,"pas de z, %i ligne, %i colonne", l, c);
    if(m_carteProfondeur[c+l*m_largeur] == 0)
    {
        XErrorAlert(Error(),__FUNCTION__,message, m_strNom.c_str());
        return false;
    }
    (*z) = m_carteProfondeur[c+l*m_largeur];
    return true;
}
//------------------------------------------------------------
Point* Panoramique::GetPt(float l, float c)
{
    for(unsigned int i=0 ; i<m_tousPointsIm.size() ; i++)
    {
        if (m_tousPointsIm[i]->Ligne() == l && m_tousPointsIm[i]->Colonne() == c)
            return m_tousPointsIm[i];
    }
    return 0;
}
//------------------------------------------------------------
XPt3D Panoramique::GetPointXPt3D(int num)
{
    for(unsigned int i=0 ; i<m_tousPointsIm.size() ; i++)
    {
        if (m_tousPointsIm[i]->NumPoint() == num)
            return XPt3D((double)m_tousPointsIm[i]->Colonne(), (double)m_tousPointsIm[i]->Ligne(), (double)m_tousPointsIm[i]->Hauteur());
    }
    return NULL;
}
//------------------------------------------------------------
bool Panoramique::AjoutPoint(Point* pt)
{
    tousPointsIm().push_back(pt);
    return true;
}
//------------------------------------------------------------
bool Panoramique::EcrireXYZ()
{
    std::string const emplacement = Chantier()->Dossier() + "\\" + m_strNom + ".XYZ";
    ofstream fichierOut(emplacement);
    if(!fichierOut)
        return false;
    for(unsigned int i=0 ; i<m_tousPointsIm.size() ; i++)
    {
        fichierOut << m_tousPointsIm[i]->NumPoint() << " " << m_tousPointsIm[i]->Colonne() << " " << m_tousPointsIm[i]->Ligne() << " " << m_tousPointsIm[i]->Hauteur() << " 0.05\n";
    }
    fichierOut << "* Station : " << m_strNom << "\n";
    fichierOut << m_translation(0) << " " << m_translation(1) << " " << m_translation(2) << "\n";
    fichierOut << m_rotation(0,0) << " " << m_rotation(0,1) << " " << m_rotation(0,2) << "\n";
    fichierOut << m_rotation(1,0) << " " << m_rotation(1,1) << " " << m_rotation(1,2) << "\n";
    fichierOut << m_rotation(2,0) << " " << m_rotation(2,1) << " " << m_rotation(2,2) << "\n";

    fichierOut.close();
    XErrorInfo(Error(),__FUNCTION__,"ecriture reussie",m_strNom.c_str());
    return true;
}
