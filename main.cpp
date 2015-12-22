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

bool InitInternalData()
{
    //XSystemInfo system;
 /*   std::string logFile = system.CreateLogFile(&m_log, "txt");
    if (logFile.size() == 0)
    {
        std::cout <<"erreur de creation du journal";
        return false;
    }
    m_log.precision(3);
    m_log.setf(std::ios::fixed);
    m_error = new XErrorConsoleTxt(&m_log);
   */
    m_error = new XErrorConsole();
   // m_error->LogFileName(logFile);

    XErrorCommentaire(m_error, __FUNCTION__, "Georeferencement de stations laser");

    // Récupération du chemin de l'application
    /*m_strAppPath = system.GetExePath();
    XPath p;
    p.AddPathSep(m_strAppPath);*/

    //m_process = new XConsoleModelProcess(m_error);
    return true;
}


int main(int argc, char **argv)
{
    Chantier projet;
    projet.nomDossier = "D:\\ProjetInfo\\donnees_test";
    InitInternalData();
    //XPt3D test; //Test point3D
    //XErrorAlert(m_error,__FUNCTION__,"alerte test");
    //XErrorError(m_error,__FUNCTION__,"erreur test");

    XErrorInfo(m_error,__FUNCTION__,projet.nomDossier.c_str());
    //projet.CreationListePano(projet.nomDossier, &m_error);
    projet.CreationListePano(projet.nomDossier);
    cout << "FIN" << endl;
    return 0;
}

