#include "XConsoleModelProcess.h"
#include "libXFileSystem/XSystemInfo.h"
#include "libXFileSystem/XPath.h"

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
bool XConsoleModelParams::SetTypProcess(std::string name)
{
    TypProcess = ConsoleModelProcess_Undef;
    if(name.compare("Undef") == 0)
    {
        TypProcess = ConsoleModelProcess_Undef;
        return true;
    }

    return false;
}
//-----------------------------------------------------------------------------
std::string XConsoleModelParams::StrTypProcess()
{
    if(TypProcess == ConsoleModelProcess_Undef)
        return std::string("Undef");

    return std::string("Undef");
}

//-----------------------------------------------------------------------------
bool XConsoleModelParams::CheckValid(XError* error)
{
    XPath P;
    P.AddPathSep(input_path);
    P.AddPathSep(output_path);
    if(input_path == output_path)
        XErrorAlert(error,__FUNCTION__,"ATTENTION : Repertoires entree/sortie identiques ");
    return true;
}

//-----------------------------------------------------------------------------
bool XConsoleModelParams::XmlWrite(std::ofstream* out)
{
    *out << "<ConsoleModel_params>\n";

    *out << "<input_path>" << input_path << "</input_path>\n";
    *out << "<output_path>" << output_path << "</output_path>\n";

    *out << "</ConsoleModel_params>\n";

    return true;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
XConsoleModelProcess::XConsoleModelProcess(XError* error)
{
    m_error = error;
}
//-----------------------------------------------------------------------------
XConsoleModelProcess::~XConsoleModelProcess(void)
{
}

//-----------------------------------------------------------------------------
bool XConsoleModelProcess::InitProcess()
{
    char message[1024];
    sprintf(message,"Repertoire en entree : %s",m_params.input_path.c_str());
    XErrorCommentaire(m_error,__FUNCTION__,message);
    sprintf(message,"Repertoire de sortie : %s",m_params.output_path.c_str());
    XErrorCommentaire(m_error,__FUNCTION__,message);

    XSystemInfo system;
    return system.CreateMultiDirectory(m_params.output_path.c_str());
}
//-----------------------------------------------------------------------------
//STATIC
std::ofstream* XConsoleModelProcess::out;
uint32 XConsoleModelProcess::count;
XConsoleModelParams XConsoleModelProcess::m_params;


//-----------------------------------------------------------------------------
std::string XConsoleModelProcess::OutFileName(std::string InputFilename)
{
    XPath P;
    std::string outfilename = P.Name(InputFilename.c_str());

    if(m_params.TypProcess == XConsoleModelParams::ConsoleModelProcess_Undef)
    {
        outfilename = m_params.output_path + P.InsertBeforeExt(outfilename.c_str(),"_Undef");
    }
    return outfilename;
}

//-----------------------------------------------------------------------------
bool XConsoleModelProcess::Process()
{
    XSystemInfo system;
    std::vector<std::string> listPts;
    system.GetFileListInFolder(Params()->input_path,listPts,"*.ply");
    if(listPts.empty())
        return XErrorInfo(m_error,__FUNCTION__,"aucun fichier ply dans le repertoire entrée" ,Params()->input_path.c_str());

    char message[1024];
    sprintf(message,"%d fichiers à traiter",listPts.size());
    XErrorInfo(m_error,__FUNCTION__,message);

    XPath P;
    XErrorInfo(m_error,__FUNCTION__,"Type de traitement : ",m_params.StrTypProcess().c_str());
    for(uint32 i=0; i< listPts.size(); i++)
    {
        sprintf(message,"\nTraitement %d/%d : ",i+1,listPts.size());
        XErrorInfo(m_error,__FUNCTION__,message,P.Name(listPts[i].c_str()).c_str());
        ProcessFile(listPts[i]);
    }

    return true;
}
//-----------------------------------------------------------------------------
bool XConsoleModelProcess::ProcessFile(std::string filename)
{
    std::string outfile = OutFileName(filename);

    return true;
}
