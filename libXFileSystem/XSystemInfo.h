//Fran�ois Becirspahic   Projet Cam�ra Num�rique
//J.P.Papelard - SR MATIS

#ifndef __X_SYSTEMINFO_H
#define __X_SYSTEMINFO_H

#ifdef WIN32
 #if _MSC_VER < 1500
   #include <shlobj.h>
 #else
  #include <windows.h>
 #endif
#endif

#include <fstream>
#include <string>
#include <vector>
#include <list>
#ifndef WIN32
 #ifndef _int64
  typedef long long _int64;
  typedef long long __int64;
 #endif // _int64
#endif // WIN32



class XSystemInfo 
{
public:
        bool GetFileListInFolder(std::string folder, std::vector<std::string> &liste, std::string filtre = "*.*", bool sort= true);
        //std::vector<std::string> GetSubFolderListInFolder(std::string folder, bool multiLevel = true);
        long long GetFileSize1(const char* src);
        double GetFileSize2(const char* src);
		std::string FileSizeToString(double fileSizeDbl);

        bool FindFile(const char* filename);
        bool FindFolder(std::string folder);

		bool Copy_File(const char* src, const char* dst, bool FailIfExist);
        //bool CopyDirContent(const char* src, const char* dst, bool FailIfExist = false);
        bool CopyFileInFolder(const char* fullFilename, const char* folderDest);
        bool CreateMultiDirectory(const char* FullDirectoryPath );
        bool CreateDirectoryIfNotExist(const char* FullDirectoryPath );

        std::string DateSysteme(std::string sep = ":");
        std::string HeureSysteme(std::string sep = ":");
        void System_Time(std::string &strDate, std::string & strHeure);
        long long GetLocalTimeStamp();

        std::string GetExePath();//renvoie le r�pertoire de l'executable courant
	
#ifdef WIN32

protected:
	SIZE_T GetLargestFreeMemRegion(LPVOID *lpBaseAddr);//valable uniquement sous XP ?

public:

    //void GetFileListInAllSubFolder(std::string folder, std::vector<std::string> &liste, std::string filtre = "*.*");

	void StripDirectoryPath(std::string &input);
	void AddPathSep(std::string & path);
	void SubPathSep(std::string & path);

	//informations sur le syst�me
    //double GetDiskFreeSpace(std::string path);
    bool IsMachineFormatLittleEndian();

    //std::string BrowseForFolder(HWND hOwner,LPCSTR lpszTitle, UINT nFlags = 0x0040);
    //int BrowseForFolderDefault(HWND hOwner,LPCSTR pszTitle, char* pszDirSel, bool CreateNewFolder = true );

	int  CountFileInFolder(std::string folder, std::string filtre = "*.*");
	int  CountElementsInFolder(std::string folder);
	bool NumberofElementsInFolderIsGreaterThan(std::string folder, int nbElementsMax);
    //int CountFileInTree(std::string Path);

    //int EmptyFolder(std::string folder);
    //int FullEmptyFolder(std::string folder);
	bool DeleteFolder(std::string folder);
	bool DeleteFichier(std::string fichier);
    //bool FullDeleteFolder(std::string folder);
	bool RenameFolder(std::string folder, std::string newName);

	std::string GetTempFileName(std::string contexte);
	std::string GetTempPath();

	std::string GetCurrentExe();//Renvoie le nom et le r�pertoire de l'executable courant
	std::string ComputerName();
	int NbProcessors();//Nombre de processeurs

	std::string CreateLogFile(std::ofstream* log, const char* extension);

	SIZE_T GetLargestFreeMemRegion();//valable uniquement sous XP ?
	size_t GetMaxSizeAllocationAvalaible();

	void secondes1970_To_DateTime(time_t secondes1970, unsigned int* date, double* heure);

	void SleepT(unsigned long milliSeconds);
	bool TestWrintingOnFolder(std::string folder);

#endif // WIN32
};

#endif //__SYSTEMINFO_H

