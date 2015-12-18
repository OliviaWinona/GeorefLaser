#ifndef XCONSOLEMODELPROCESS_H
#define XCONSOLEMODELPROCESS_H
#include <libXBase/XError.h>

class XConsoleModelParams
{
public:
    std::string input_path;
    std::string output_path;

    enum eTypPtsProcess {ConsoleModelProcess_Undef};
    eTypPtsProcess TypProcess;

public:
    XConsoleModelParams(void){
        TypProcess = ConsoleModelProcess_Undef;
    }
    ~XConsoleModelParams(void){;}
    bool SetTypProcess(std::string name);
    std::string StrTypProcess();

    bool  CheckValid(XError* error);
    bool XmlWrite(std::ofstream* out);

};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

class XConsoleModelProcess
{
protected:
    XError* m_error;

    static XConsoleModelParams m_params;		// Paramètres et Options de traitement
    static std::ofstream* out;
    static uint32 count;

    bool ProcessFile(std::string InputFilename);

public:
    XConsoleModelProcess(XError* error);
    ~XConsoleModelProcess(void);

    XConsoleModelParams* Params(){return &m_params;}

    std::string OutFileName(std::string InputFilename);

    bool InitProcess();
    bool Process();

};

#endif // XCONSOLEMODELPROCESS_H
