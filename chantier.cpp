#include <iostream>
#include <vector>
#include <libXFileSystem/XSystemInfo.h>
#include <libXBase/XErrorConsole.h>
#include "XConsoleModelProcess.h"

#include "chantier.h"

using namespace std;

Chantier::Chantier()
{
}

Chantier::~Chantier()
{
}

//void CreationListePano(std::string dossier, XErrorConsole *m_error)
void CreationListePano(std::string dossier)
{
    XSystemInfo system;
    bool ok;
    //XErrorInfo(m_error,__FUNCTION__,dossier);
    cout << dossier << endl;
    ok = system.FindFolder(dossier);
    cout << ok << endl;
    /*if (ok){
        XErrorInfo(m_error,__FUNCTION__,ok);
    }else{
        XErrorAlert(m_error,__FUNCTION__,ok);
    }*/
}
