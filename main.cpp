#include <iostream>
#include <libXBase/XErrorConsole.h>
#include <libXFileSystem/XPath.h>
#include <libXFileSystem/XSystemInfo.h>
#include "XConsoleModelProcess.h"
#include "shellapi.h"
#include "libXBase/XPt3D.h"

#include "chantier.h"
#include "fonctions.h"
#define WIN32

using namespace std;

//XErrorConsoleTxt* m_error;
XErrorConsole* m_error;

std::ofstream m_log;
std::string m_strAppPath;
XConsoleModelProcess* m_process;
Chantier* projet;

bool InitInternalData()
{
   XSystemInfo system;
//    std::string logFile = system.CreateLogFile(&m_log, "txt");
//    if (logFile.size() == 0)
//    {
//        std::cout <<"erreur de creation du journal";
//        return false;
//    }
    m_log.precision(3);
    m_log.setf(std::ios::fixed);
   // m_error = new XErrorConsoleTxt(&m_log);
    m_error = new XErrorConsole();

   // m_error->LogFileName(logFile);

    XErrorCommentaire(m_error, __FUNCTION__, "Georeferencement de stations laser");

    // Récupération du chemin de l'application
    m_strAppPath = system.GetExePath();
    XPath p;
    p.AddPathSep(m_strAppPath);

    projet = new Chantier(m_error);
    return true;
}


int main(int argc, char **argv)
{
    InitInternalData();

    std::string nomDossier = "D:\\ProjetInfo\\donnees_test";

    XErrorCommentaire(m_error,__FUNCTION__,"\n Charge les panoramiques");
    if(!projet->ChargePano(nomDossier))
    {
        getchar();
        return 0;
    }

//    XErrorCommentaire(m_error,__FUNCTION__,"\n Creation des .key");
//    if(!projet->CreationKey())
//    {
//        getchar();
//        return 0;
//    }

//    XErrorCommentaire(m_error,__FUNCTION__,"\n Creation des .result");
//    if(!projet->CreationResult())
//    {
//        getchar();
//        return 0;
//    }

    XErrorCommentaire(m_error,__FUNCTION__,"\n Charge les .result");
    if(!projet->ChargeResult())
    {
        getchar();
        return 0;
    }

    XErrorCommentaire(m_error,__FUNCTION__,"\n Phase d'orientation");
    if(!projet->Orientation())
    {
        getchar();
        return 0;
    }

    delete projet;
    XErrorCommentaire(m_error,__FUNCTION__, "FIN");
    return 0;
}
