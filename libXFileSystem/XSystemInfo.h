//François Becirspahic   Projet Caméra Numérique
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



class XSystemInfo 
{
public:
        bool GetFileListInFolder(std::string folder, std::vector<std::string> &liste, std::string filtre = "*.*", bool sort= true);
        double GetFileSize2(const char* src);
		std::string FileSizeToString(double fileSizeDbl);

        bool FindFile(const char* filename);
        bool FindFolder(std::string folder);

		bool Copy_File(const char* src, const char* dst, bool FailIfExist);
        bool CopyFileInFolder(const char* fullFilename, const char* folderDest);
        bool CreateMultiDirectory(const char* FullDirectoryPath );
        bool CreateDirectoryIfNotExist(const char* FullDirectoryPath );

        std::string DateSysteme(std::string sep = ":");
        std::string HeureSysteme(std::string sep = ":");
        void System_Time(std::string &strDate, std::string & strHeure);

        std::string GetExePath();//renvoie le répertoire de l'executable courant
	
#ifdef WIN32

protected:
	SIZE_T GetLargestFreeMemRegion(LPVOID *lpBaseAddr);//valable uniquement sous XP ?

public:

	void StripDirectoryPath(std::string &input);
	void AddPathSep(std::string & path);
	void SubPathSep(std::string & path);

	//informations sur le système
	double GetDiskFreeSpace(std::string path);
    bool IsMachineFormatLittleEndian();

	int  CountFileInFolder(std::string folder, std::string filtre = "*.*");
	int  CountElementsInFolder(std::string folder);
	bool NumberofElementsInFolderIsGreaterThan(std::string folder, int nbElementsMax);
	int CountFileInTree(std::string Path);

	int EmptyFolder(std::string folder);
	int FullEmptyFolder(std::string folder);
	bool DeleteFolder(std::string folder);
	bool DeleteFichier(std::string fichier);
	bool FullDeleteFolder(std::string folder);
	bool RenameFolder(std::string folder, std::string newName);

	std::string GetTempFileName(std::string contexte);
	std::string GetTempPath();

	std::string GetCurrentExe();//Renvoie le nom et le répertoire de l'executable courant
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

