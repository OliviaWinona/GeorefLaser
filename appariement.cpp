#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <Eigen/Dense>

#include "libXBase/XStringTools.h"
#include "libXBase/XPt3D.h"
#include "appariement.h"
#include "point.h"
#include "panoramique.h"
#include "chantier.h"

using namespace std;

Appariement::Appariement(Panoramique* pano1, Panoramique* pano2)
{
    m_image1 = pano1;
    m_image2 = pano2;
}


Appariement::~Appariement()
{
}
//------------------------------------------------------------
int Appariement::NbPointsApp() {return m_listePoints.size()/2;}
//------------------------------------------------------------
bool Appariement::AjoutPoint(int num, float l, float c, float z, Panoramique* pano)
{
    Point* pt = new Point(num, l, c, z);
    m_listePoints.push_back(pt);
    pano->tousPointsIm().push_back(pt);
    return true;
}
//------------------------------------------------------------
bool Appariement::Nettoyage(Panoramique* pano1, Panoramique* pano2)
{
    int nb = m_listePoints.size()/2;
    pano1->tousPointsIm().erase(pano1->tousPointsIm().end()-nb, pano1->tousPointsIm().end());
    pano2->tousPointsIm().erase(pano2->tousPointsIm().end()-nb, pano2->tousPointsIm().end());
    m_listePoints.clear();
    return true;
}
//------------------------------------------------------------
bool Appariement::ChargeMesures(XError* error, std::string FileResult, int* nbPoints)
{
    std::string chemin = m_image1->Chantier()->Dossier() + "\\" + FileResult;
    std::ifstream file;
    XStringTools st;
    char message[1024];
    file.open(chemin);
    if (!file.is_open())
        return XErrorError(error,__FUNCTION__,"Erreur ouverture fichier",chemin.c_str());
    XErrorInfo(error,__FUNCTION__,"Chargement mesures", FileResult.c_str());


    //Vérifier le nombre de points dans le fichier
    int nblignes = 0;
    std::string s;
    while(std::getline(file,s)) ++nblignes;
    if(nblignes < 4)
    {
        sprintf(message,"Trop peu de points : %s, appariement non pris en compte",st.itoa(nblignes).c_str());
        XErrorAlert(error,__FUNCTION__,message);
        return false;
    }
    file.clear();
    file.seekg(0, std::ios_base::beg);


    //Chargement des cartes de profondeur
    if(!m_image1->ChargeCarteProfondeur())
        return XErrorError(error,__FUNCTION__,"Erreur de chargement de la carte de profondeur",m_image1->Nom().c_str());
    if(!m_image2->ChargeCarteProfondeur())
        return XErrorError(error,__FUNCTION__,"Erreur de chargement de la carte de profondeur",m_image2->Nom().c_str());


    //Création des Points
    std::string ligneEnCours;
    std::vector<std::string>decoupage;
    float l1, c1, l2, c2, z1, z2;
    while(!file.eof())
    {
        decoupage.clear();
        getline(file, ligneEnCours);
        if(ligneEnCours != "")
        {
            st.Tokenize(ligneEnCours, decoupage, "\t");
            c1 = atof(decoupage[0].c_str());
            l1 = atof(decoupage[1].c_str());
            c2 = atof(decoupage[2].c_str());
            l2 = atof(decoupage[3].c_str());

            if(!m_image1->GetZ(l1,c1,&z1))
                continue;
            if(!m_image2->GetZ(l2,c2,&z2))
                continue;

            int num1 = m_image1->GetNum(l1, c1);
            int num2 = m_image2->GetNum(l2, c2);

            if(num1 != 0) //point déjà existant sur la pano 1
            {
                sprintf(message,"Point %s deja trouve dans pano %s",st.itoa(num1).c_str(),m_image1->Nom().c_str());
                XErrorInfo(error,__FUNCTION__,message);
                AjoutPoint(num1, l2, c2, z2, m_image2);
                continue;
            }

            if(num2 != 0) //point déjà existant sur la pano 2
            {
                sprintf(message,"Point %s deja trouve dans pano %s",st.itoa(num2).c_str(),m_image2->Nom().c_str());
                XErrorInfo(error,__FUNCTION__,message);
                AjoutPoint(num2, l1, c1, z1, m_image1);
                continue;
            }

            //point non existant
            (*nbPoints) = (*nbPoints) + 1;
            AjoutPoint(*nbPoints, l1, c1, z1, m_image1);
            AjoutPoint(*nbPoints, l2, c2, z2, m_image2);
        }
    }
    file.close();

    //Suppression des cartes de profondeurs
    m_image1->CarteProfondeur().clear();
    m_image2->CarteProfondeur().clear();

    if(NbPointsApp() < 4)
    {
        (*nbPoints) = (*nbPoints) - NbPointsApp();
        Nettoyage(m_image1, m_image2);
        XErrorAlert(error,__FUNCTION__,"Nb de points coherent < 4, appariement non pris en compte");
        return false;
    }

    XErrorInfo(error,__FUNCTION__,"Nb points homologues : ",st.itoa(NbPointsApp()).c_str());
    return true;
}
//------------------------------------------------------------
bool Appariement::DejaPresent(std::vector<Point*> lstPts, Point* pt)
{
    for(unsigned int i=0 ; i<lstPts.size() ; i++)
    {
        if(lstPts[i]->NumPoint() == pt->NumPoint())
            return true;
    }
    return false;
}
//------------------------------------------------------------
std::vector<Point*> Appariement::ChoixQuatrePointsAleatoires(std::vector<Point*> points)
{
    int rd;
    srand(time(NULL));
    while(points.size() < 4)
    {
        rd = rand() % NbPointsApp() + 1;
        //cout << "pt " << m_listePoints[rd]->NumPoint() << " & rand " << rd << endl;
        if(DejaPresent(points, m_listePoints[rd]))
            continue;

        points.push_back(m_listePoints[rd]);
    }

    return points;
}
//------------------------------------------------------------
void Appariement::CalculCoefAxiateur(double a, double b, double c, double theta, Eigen::Matrix3d *R)
{
     double cost1 = 1 - cos(theta);
     double sint = sin(theta);
     double d = sqrt(a*a + b*b + c*c);
     double u = a/d;
     double v = b/d;
     double w = c/d;
     (*R)(0,0)= 1 -(1 - u*u) * cost1;
     (*R)(0,1)= -w*sint + u*v*cost1;
     (*R)(0,2)= v*sint + u*w*cost1;
     (*R)(1,0)= w*sint + u*v*cost1;
     (*R)(1,1)= 1 -(1 - v*v) * cost1;
     (*R)(1,2)= -u*sint + v*w*cost1;
     (*R)(2,0)= -v*sint + u*w*cost1;
     (*R)(2,1)= u*sint + v*w*cost1;
     (*R)(2,2)= 1 -(1 - w*w) * cost1;
}

//------------------------------------------------------------
bool Appariement::Thomson_Shut(XError* error, std::vector<XPt3D> &ptPano1,std::vector<XPt3D> &ptPano2, Eigen::Matrix3d *R, Eigen::Vector3d *T, double *e)
{
    if(ptPano1.size() != ptPano2.size())
        return XErrorError(error,__FUNCTION__,"pas le meme nb de pt entre les 2 stations pour la similitude");
    if(ptPano1.size() <=3)
        return XErrorError(error,__FUNCTION__,"pas assez de points");

    // Passage en coordonnées barycentrique
    XPt3D G1 = XPt3D(0,0,0);
    XPt3D G2 = XPt3D(0,0,0);

    // Moyenne des coordonnées des points des 4 points de la première station
    for(auto &s : ptPano1)
        G1 += s;
    G1 /= ptPano1.size();
    // Moyenne des coordonnées des points des 4 points de la deuxième station
    for(auto &s : ptPano2)
        G2 += s;
    G2 /= ptPano2.size();

    double dist1 = 0;
    double dist2 = 0;

    for(auto &s : ptPano1)
        dist1 += std::pow((s.X-G2.X),2) + std::pow((s.Y-G2.Y),2) + std::pow((s.Z-G2.Z),2);
    for(auto &s : ptPano2)
        dist2 += std::pow((s.X-G1.X),2) + std::pow((s.Y-G1.Y),2) + std::pow((s.Z-G1.Z),2);

    (*e) = sqrt(dist1/dist2);

    XPt3D X1,X2;
    Eigen::MatrixXd A(ptPano1.size()*3,3);
    Eigen::VectorXd B(ptPano1.size()*3);

    for(unsigned int i=0 ; i<ptPano2.size() ; i++)
    {
        // On enlève le facteur d'échelle pour ramener la pano2 à la même échelle que la pano1
        // On retire le barycentre au point
        X1 = ptPano1[i] - G1;
        X1 *= (*e);
        X2 = ptPano2[i] - G2;

        // Remplissage de la matrice et du vecteur pour le calcul de la transformation
        // Première colonne
        A(i*3,0) = 0;
        A(i*3,1) = -(X1.Z + X2.Z);
        A(i*3,2) = X1.Y + X2.Y;
        B(i*3) = X1.X - X2.X;

        // Deuxième colonne
        A(i*3+1,0) = X1.Z + X2.Z;
        A(i*3+1,1) = 0;
        A(i*3+1,2) = -(X1.X + X2.X);
        B(i*3+1) = X1.Y - X2.Y;

        // Troisième colonne
        A(i*3+2,0) = -(X1.Y + X2.Y);
        A(i*3+2,1) = X1.X + X2.X;
        A(i*3+2,2) = 0;
        B(i*3+2) = X1.Z - X2.Z;
    }

    // Calcul de la matrice de rotation
    Eigen::MatrixXd AtA = (A.transpose())*A;
    Eigen::MatrixXd AtB = (A.transpose())*B;
    Eigen::Vector3d soluc = AtA.colPivHouseholderQr().solve(AtB);

    double N = soluc.norm();

    if(N<1e-30)
    {
        soluc(0) = 0;
        soluc(1) = 0;
        soluc(2) = 1;
        N = 0;
    }else
        soluc = soluc / N;

    N = 2 * atan(N);
    CalculCoefAxiateur(soluc[0],soluc[1],soluc[2],N,R);

    Eigen::Vector3d vectG1, vectG2;
    vectG1<<G1.X,G1.Y,G1.Z;
    vectG2<<G2.X,G2.Y,G2.Z;

    // Calcul de la translation
    (*T) = vectG2 - (*R)*vectG1*(*e);

    return true;
}
