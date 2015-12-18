#ifdef WIN32
    #include "libXFileSystem/XSystemInfo.h"
#else
    #include "XSystemInfoLinux.h"
#endif

#ifdef WIN32
 #if _MSC_VER >= 1500
   #include <Shlobj.h>
 #endif
#endif

#include "libXBase/XStringTools.h"
#include "libXFileSystem/XPath.h"
#include <algorithm>

std::string ws2s(const std::wstring& wstr)
{
    int size_needed = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), int(wstr.length() + 1), 0, 0, 0, 0);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), int(wstr.length() + 1), &strTo[0], size_needed, 0, 0);
    return strTo;
}
//-----------------------------------------------------------------------------
void XSystemInfo::StripDirectoryPath(std::string &input)
{
	std::string str = input;
	std::string::size_type pos;
	std::string::size_type input_pos = 0;
	while( ( pos = str.find_first_of("/"))!=std::string::npos)
	{
		input_pos += pos;
		input[input_pos]='\\';
		str = str.substr(pos+1,str.size());
		input_pos++;
	}
}
//-----------------------------------------------------------------------------
void XSystemInfo::AddPathSep(std::string & path)
{
	if(path.empty())
	{
		path.push_back('.');
		path.push_back(XPath::PathSep);
		return;
	}
	if((*path.rbegin())!= XPath::PathSep)
		path.push_back(XPath::PathSep);
}
//-----------------------------------------------------------------------------
void XSystemInfo::SubPathSep(std::string & path)
{
	if((*path.rbegin())== XPath::PathSep)
		path = path.substr(0,path.size()-1);
}

//-----------------------------------------------------------------------------
// Donne l'espace disque disponible dans un repertoire
//-----------------------------------------------------------------------------
double XSystemInfo::GetDiskFreeSpace(std::string path)
{
	typedef BOOL (CALLBACK* LPFNDLLFUNC1)(LPCTSTR, PULARGE_INTEGER,
				  PULARGE_INTEGER , PULARGE_INTEGER );

	HINSTANCE hDLL;               // Handle sur la DLL
	LPFNDLLFUNC1 lpfnDllFunc1;    // Pointeur de fonction
	double diskSpace = 0.0;

    hDLL = LoadLibrary((LPCWSTR)"Kernel32.dll");
	if (hDLL == NULL)
		return 0;

	lpfnDllFunc1 = (LPFNDLLFUNC1)GetProcAddress(hDLL,"GetDiskFreeSpaceExA");
	if (!lpfnDllFunc1)
	{
		DWORD SectorsPerCluster, BytesPerSector, NumberOfFreeClusters, TotalNumberOfClusters;
        SetCurrentDirectory((LPCWSTR)path.c_str());
		if (::GetDiskFreeSpace(NULL, &SectorsPerCluster, &BytesPerSector,
			&NumberOfFreeClusters,&TotalNumberOfClusters) == TRUE) {
			diskSpace = (double)(NumberOfFreeClusters * BytesPerSector * SectorsPerCluster);
			diskSpace /= 1048576.0;
		}
	} else { //La fonction GetDiskFreeSpaceEx est connue
		ULARGE_INTEGER FreeBytesAvailableToCaller,TotalNumberOfBytes,TotalNumberOfFreeBytes;
        if (lpfnDllFunc1((LPCWSTR)path.c_str(),&FreeBytesAvailableToCaller,
						&TotalNumberOfBytes, &TotalNumberOfFreeBytes ) == TRUE) {
			if (FreeBytesAvailableToCaller.HighPart > 0)
				diskSpace = (double)FreeBytesAvailableToCaller.HighPart * 4096.0 +
				(double)FreeBytesAvailableToCaller.LowPart / 1048576.0;
			else
				diskSpace = (double)FreeBytesAvailableToCaller.LowPart / 1048576.0;
		}
	}
	FreeLibrary(hDLL);
	return diskSpace;
}

//-----------------------------------------------------------------------------
// Permet de tester l'existence d'un fichier
//-----------------------------------------------------------------------------
bool XSystemInfo::FindFile(const char* filename)
{
	WIN32_FIND_DATA findData;
    HANDLE file = FindFirstFile((LPCWSTR)filename, &findData);
	if (file == INVALID_HANDLE_VALUE)
		return false;	// Le fichier n'existe pas
	FindClose(file);
	return true;
}
//-----------------------------------------------------------------------------
// Permet de tester l'existence d'un r�pertoire
//-----------------------------------------------------------------------------
bool XSystemInfo::FindFolder(std::string folder)
{
	if (folder.size() == 0)
		return false;
	SubPathSep(folder);		

	WIN32_FIND_DATA findData;
    HANDLE file = FindFirstFile((LPCWSTR)folder.c_str(), &findData);
	if (file == INVALID_HANDLE_VALUE)// Le repertoire n'existe pas
		return false;
	FindClose(file);

	return true;
}
//-----------------------------------------------------------------------------
bool XSystemInfo::Copy_File(const char* src, const char* dst, bool FailIfExist)
{
    return ::CopyFile((LPCWSTR)src,(LPCWSTR)dst,FailIfExist);
}

//-----------------------------------------------------------------------------
// Retourne la taille d'un fichier dans un double 
//-----------------------------------------------------------------------------
double XSystemInfo::GetFileSize2(const char* src)
{

    HANDLE fileHandle = CreateFile((LPCWSTR)src, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	LARGE_INTEGER fileSize;
	if(!GetFileSizeEx(fileHandle, &fileSize))
		return -1;
	DWORD low = fileSize.LowPart;
	double high = fileSize.HighPart;
	double quad = fileSize.QuadPart;

	CloseHandle( fileHandle);	
	return (double)(__int64)fileSize.QuadPart;


}
//-----------------------------------------------------------------------------
std::string XSystemInfo::FileSizeToString(double fileSizeDbl)
{
	std::string units("octets");

	if (fileSizeDbl > 1024.0)
	{
		fileSizeDbl /= 1024.0;
		units = "Ko";
	}
	if (fileSizeDbl > 1024.0)
	{
		fileSizeDbl /= 1024.0;
		units = "Mo";
	}
	if (fileSizeDbl > 1024.0)
	{
		fileSizeDbl /= 1024.0;
		units = "Go";
	}

	char fileSizeStr[1024];
	sprintf(fileSizeStr,"%.2f %s", fileSizeDbl, units.c_str());

	return std::string(fileSizeStr);
}
//-----------------------------------------------------------------------------
// Compte le nombre de fichier d'un repertoire
//-----------------------------------------------------------------------------
int XSystemInfo::CountFileInFolder(std::string folder, std::string filtre)
{
	AddPathSep(folder);
	folder += filtre;

	HANDLE hFind;
	WIN32_FIND_DATA fd;
	int nCount = 0;

     if ((hFind = ::FindFirstFile ((LPCWSTR)folder.c_str(), &fd)) != INVALID_HANDLE_VALUE) {
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				nCount++;
		}
		else {
			return 0;
		}

	while (::FindNextFile (hFind, &fd)) {
		if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			nCount++;
	}
	::FindClose(hFind);
	return nCount;
}
//+ rapide que CountFileInFolder ( utilis� pour connaitre approximativement le nombre d�l�ments d'un repertoire)
//-----------------------------------------------------------------------------
int XSystemInfo::CountElementsInFolder(std::string folder)
{
	AddPathSep(folder);
	folder += "*.*";

	HANDLE hFind;
	WIN32_FIND_DATA fd;
	int nCount = 0;

     if ((hFind = ::FindFirstFile ((LPCWSTR)folder.c_str(), &fd)) != INVALID_HANDLE_VALUE)
			nCount++;
		else 
			return 0;

	while (::FindNextFile (hFind, &fd))
	{
		nCount++;
	}
	::FindClose(hFind);
	return nCount;
}
//-----------------------------------------------------------------------------
bool XSystemInfo::NumberofElementsInFolderIsGreaterThan(std::string folder, int nbElementsMax)
{
	AddPathSep(folder);
	folder += "*.*";

	HANDLE hFind;
	WIN32_FIND_DATA fd;

    if ((hFind = ::FindFirstFile ((LPCWSTR)folder.c_str(), &fd)) == INVALID_HANDLE_VALUE)
		return false;

	for(int i=0; i<nbElementsMax; i++)
		if(!::FindNextFile (hFind, &fd))
			return false;
	::FindClose(hFind);
	return true;
}

//-----------------------------------------------------------------------------
// Renvoie la liste des fichiers d'un repertoire
//-----------------------------------------------------------------------------
bool XSystemInfo::GetFileListInFolder(std::string folder, std::vector<std::string> &liste, std::string filtre, bool sort)
{
/*	std::string strFolder = folder;
	AddPathSep(strFolder);

	std::string strPath = strFolder + filtre;

	HANDLE hFind;
	WIN32_FIND_DATA fd;

    if ((hFind = ::FindFirstFile ((LPCWSTR)strPath.c_str(), &fd)) != INVALID_HANDLE_VALUE)
	{
		if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
            liste.push_back((LPCWSTR)strFolder + fd.cFileName);
		}
	}
	else
	{
		return false;
	}

	while (::FindNextFile (hFind, &fd))
	{
		if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
            liste.push_back((LPCWSTR)strFolder + fd.cFileName);
		}
	}
	::FindClose(hFind);
	if(sort)
        std::sort(liste.begin(),liste.end());*/

	return true;
}


//-----------------------------------------------------------------------------
std::string XSystemInfo::GetExePath()
{
    wchar_t buf[MAX_PATH];
    //char buf[MAX_PATH];
    DWORD count = ::GetModuleFileNameW(NULL, buf, MAX_PATH);
    std::string test = ws2s(buf) ;
    return test.substr(0, test.rfind('\\'));;
}
//-----------------------------------------------------------------------------
std::string XSystemInfo::GetTempFileName(std::string contexte)
{
/*	if(contexte.empty())
		contexte = "tmp";

	char filename[MAX_PATH], path[MAX_PATH];
	::GetTempPath(MAX_PATH, path);
	::GetTempFileName(path ,contexte.c_str(), 0, filename);
    return std::string(filename);*/
    return std::string();
}
//-----------------------------------------------------------------------------
std::string XSystemInfo::GetTempPath()
{
    std::wstring path;
    ::GetTempPath(MAX_PATH, (LPWSTR)path.c_str());
    return ws2s(path);
}
//-----------------------------------------------------------------------------
// Renvoie la date syst�me sous la forme yyyy:mm:dd
//-----------------------------------------------------------------------------
std::string XSystemInfo::DateSysteme(std::string sep)
{
	SYSTEMTIME systime;
	::GetSystemTime(& systime);
	char date[1024];
	sprintf(date,"%4d%s%02d%s%02d",systime.wYear,sep.c_str(),systime.wMonth,sep.c_str(),systime.wDay);
	return std::string(date);
}
//-----------------------------------------------------------------------------
// Renvoie l'heure syst�me sous la forme hh:mm:ss
//-----------------------------------------------------------------------------
std::string XSystemInfo::HeureSysteme(std::string sep)
{
	SYSTEMTIME systime;
	::GetSystemTime(& systime);
	char heure[1024];
	sprintf(heure,"%02d%s%02d%s%02d",systime.wHour,sep.c_str(),systime.wMinute,sep.c_str(),systime.wSecond);
	return std::string(heure);
}
//-----------------------------------------------------------------------------
void XSystemInfo::System_Time(std::string &strDate, std::string & strHeure)
{
	SYSTEMTIME systime;
	::GetSystemTime(& systime);
	char date[1024];
	sprintf(date,"%4d:%02d:%02d",systime.wYear,systime.wMonth,systime.wDay);
	char heure[1024];
	sprintf(heure,"%02d:%02d:%02d",systime.wHour,systime.wMinute,systime.wSecond);
	strDate = date;
	strHeure = heure;
}

//-----------------------------------------------------------------------------
// Compte le nombre de fichier dans une arborescence
//-----------------------------------------------------------------------------
int XSystemInfo::CountFileInTree(std::string Path)
{
/*	std::string path = Path;
	if (path.size() < 1)
		return 0;

	AddPathSep(path);

	std::string find_path = path + "*.*";

 	HANDLE hFind;
  WIN32_FIND_DATA fd;
  int nCount = 0;
 if ((hFind = ::FindFirstFile (find_path.c_str(), &fd)) != INVALID_HANDLE_VALUE) {
		if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			nCount++;
		else {
			std::string filename = fd.cFileName;
			if ((filename.compare(".") != 0) && (filename.compare("..") != 0))
				nCount += CountFileInTree(path + filename);
		}
	}
	else
		return 0;

	while (::FindNextFile (hFind, &fd)) {
		if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			nCount++;
		else{
			std::string filename = fd.cFileName;
			if ((filename.compare(".") != 0) && (filename.compare("..") != 0))
				nCount += CountFileInTree(path + filename);
		}
	}
	::FindClose(hFind);
    return nCount;*/
    return 0;
}
//-----------------------------------------------------------------------------
bool XSystemInfo::DeleteFolder(std::string folder)
{
	if (folder[folder.length()-1] == '\\')
		folder = folder.substr(0,folder.length()-1);
    return ::RemoveDirectory((LPWSTR)folder.c_str());
}
//-----------------------------------------------------------------------------
bool XSystemInfo::DeleteFichier(std::string fichier)
{
    return ::DeleteFile((LPWSTR)fichier.c_str());
}

//-----------------------------------------------------------------------------
bool XSystemInfo::FullDeleteFolder(std::string folder)
{
	if(FullEmptyFolder(folder) == -1)
		return false;
    return DeleteFolder(folder);
}
//-----------------------------------------------------------------------------
bool XSystemInfo::RenameFolder(std::string folder, std::string newName)
{
	if (rename(folder.c_str(), newName.c_str())==0)
		return true;
	return false;
}

//-----------------------------------------------------------------------------
//D�truit tout les fichiers d'un r�pertoire (pas les sous-r�pertoires !)
//-----------------------------------------------------------------------------
int XSystemInfo::EmptyFolder(std::string folder)
{
    /*if (folder.size() < 1)
		return -1;
	AddPathSep(folder);

	std::string Path = folder + "*.*";
	std::string fileName;

	int nbDelete = 0;

	HANDLE file;
	WIN32_FIND_DATA fd;
	bool pb = false;

  if ((file = ::FindFirstFile (Path.c_str(), &fd)) != INVALID_HANDLE_VALUE) {
		if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
			fileName = folder;
			fileName += fd.cFileName;
			if(::DeleteFile(fileName.c_str()))
				nbDelete++;
			else
				pb = true;
		}
	}
	else
		return 0;

	while (::FindNextFile (file, &fd)) {
		if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
			fileName = folder;
			fileName += fd.cFileName;
			if(::DeleteFile(fileName.c_str()))
				nbDelete++;
			else
				pb = true;
		}
	}
	::FindClose(file);
	if(pb)
		return -1;

    return nbDelete;*/
    return 0;
}
//-----------------------------------------------------------------------------
//D�truit tout le contenu d'un r�pertoire (y compris les sous-r�pertoires !)
//-----------------------------------------------------------------------------
int XSystemInfo::FullEmptyFolder(std::string folder)
{
/*	if (folder.size() < 1)
		return -1;
	if (folder[folder.length()-1] != XPath::PathSep)
		folder += XPath::PathSep;

	std::string Path = folder + "*.*";
	std::string fileName;

	int nbDelete = 0;

	HANDLE file;
	WIN32_FIND_DATA fd;
	bool pb = false;

  if ((file = ::FindFirstFile (Path.c_str(), &fd)) == INVALID_HANDLE_VALUE) 
	  return 0;
  
	fileName = fd.cFileName;
	if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
		if(::DeleteFile((folder+fileName).c_str()))
			nbDelete++;
		else
			pb = true;
	}
	else
	{
		if ((fileName.compare(".") != 0) && (fileName.compare("..") != 0))
		{
			nbDelete+= FullEmptyFolder(folder+fileName);
			if(DeleteFolder(folder+fileName))
				nbDelete++;
			else
				pb = true;
		}
	}
	
	

	while (::FindNextFile (file, &fd)) {
		fileName = fd.cFileName;
		if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
			if(::DeleteFile((folder+fileName).c_str()))
				nbDelete++;
			else
				pb = true;
		}
		else
		{
			if ((fileName.compare(".") != 0) && (fileName.compare("..") != 0))
			{
				nbDelete+= FullEmptyFolder(folder+fileName);
				if(DeleteFolder(folder+fileName))
					nbDelete++;
				else
					pb = true;
			}
		}
	}
	::FindClose(file);
	if(pb)
		return -1;

    return nbDelete;*/
    return 0;
}
//---------------------------------------------------------------------------
bool XSystemInfo::CreateDirectoryIfNotExist(const char* FullDirecotryPath )
{
	std::string path = FullDirecotryPath;
	if(FindFolder(path))
		return true;
    std::string toto1(FullDirecotryPath);
    std::wstring toto2(toto1.begin(),toto1.end());
    return CreateDirectory((LPWSTR)FullDirecotryPath , NULL);
}


//---------------------------------------------------------------------------
bool  XSystemInfo::CreateMultiDirectory(const char* FullDirecotryPath )
{
	std::string path = FullDirecotryPath;
	if(FindFolder(path))
		return true;
	XStringTools st;
	std::vector<std::string> directory = st.Tokenize(path,XPath::PathSep,true);

	path="";
	for(std::vector<std::string>::iterator it=directory.begin();it<directory.end();it++)
	{
		if((*it)[1]==':')//On va eviter de cr�er un disque...
		{
			path+=(*it);
			continue;
		}
		path += XPath::PathSep;
		path+=(*it);
		if( FindFolder(path))
			continue;
        if( CreateDirectory( (LPWSTR)path.c_str() , NULL) != TRUE)
			return false;
	}
    return true;
}
//---------------------------------------------------------------------------
std::string XSystemInfo::ComputerName()
{
	DWORD buf_size = 1024;
	char buffer[1024];
    if (::GetComputerName((LPWSTR)buffer,&buf_size)== FALSE)
		return std::string();
	return std::string(buffer);
}
//R�pertoire de l'executable---------------------------------------------------------------------------
std::string  XSystemInfo::GetCurrentExe()
{
    wchar_t buf[MAX_PATH];
    DWORD count = ::GetModuleFileName(NULL, buf, MAX_PATH);
    return ws2s(buf);
}
//---------------------------------------------------------------------------
std::string XSystemInfo::CreateLogFile(std::ofstream* log, const char * extension)
{
	XPath p;

	std::string AppPath = GetCurrentExe();
	std::string AppName = p.NameNoExt(AppPath.c_str());

	std::string folder = p.Path(AppPath.c_str()) + XPath::PathSep + "Journal_" + AppName;
	if (!CreateDirectoryIfNotExist(folder.c_str()))
		return std::string();
	folder += XPath::PathSep;

	//date
	char buf[1024];
	int nblogToday = 0;
	do {
		sprintf(buf,"%s%s_%s_%s_%d.%s",folder.c_str(),AppName.c_str(),ComputerName().c_str(),DateSysteme(std::string("_")).c_str(),nblogToday,extension);
		nblogToday++;
    } while(::GetFileAttributes((LPWSTR)buf)!=0xFFFFFFFF);//enddo



	log->open(buf, std::ios_base::out);
	if(!log->good())
        return std::string();
	log->setf(std::ios_base::unitbuf);

	return std::string(buf);

}
//---------------------------------------------------------------------------
bool XSystemInfo::CopyFileInFolder(const char* fullFilename, const char* folderDest)
{
	std::string dest = folderDest;
	AddPathSep(dest);

	XPath p;
	std::string newFilename = dest + p.Name(fullFilename);
    return ::CopyFile((LPWSTR)fullFilename,(LPWSTR)newFilename.c_str(),FALSE);
}
//---------------------------------------------------------------------------
bool XSystemInfo::IsMachineFormatLittleEndian()
{
	std::string endian;
	int i = 1;
    char *p = (char *)&i;
    if (p[0] == 1)
		return true;
    
	return false;;
}
//-----------------------------------------------------------------------------
SIZE_T XSystemInfo::GetLargestFreeMemRegion()
{
	LPVOID baseAddr;
	SIZE_T ls = GetLargestFreeMemRegion(&baseAddr);
	return ls;
}

//-----------------------------------------------------------------------------
//Determine la taille maximum d'allocation possible
//-----------------------------------------------------------------------------
size_t XSystemInfo::GetMaxSizeAllocationAvalaible()
{
    size_t step = 0x1000000;
    size_t size = step;
    size_t best = 0;
    while (step > 0)
    {
        void *p = malloc(size);
        if (p) {
            free(p);
            best = size;
        }
        else {
            step /= 0x10;
        }
        size += step;
    }
    return best;
}
//-----------------------------------------------------------------------------
SIZE_T XSystemInfo::GetLargestFreeMemRegion(LPVOID *lpBaseAddr)
{
	SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);
	VOID *p = 0;
	MEMORY_BASIC_INFORMATION mbi;
	SIZE_T largestSize = 0;
	while(p < systemInfo.lpMaximumApplicationAddress)
	{
		SIZE_T dwRet = VirtualQuery(p, &mbi, sizeof(mbi));
		if (dwRet > 0)
		{
	//        if (showMemInfo)
	//          PrintMemInfo(mbi);
		if (mbi.State == MEM_FREE)
		{
			if (largestSize < mbi.RegionSize)
			{
			largestSize = mbi.RegionSize;
			if (lpBaseAddr != NULL)
				*lpBaseAddr = mbi.BaseAddress;
			}
		}
		p = (void*) (((char*)p) + mbi.RegionSize);
		}
		else
		{
	//       if (showMemInfo)
	//         _tprintf(_T(�### VirtualQuery failed (%p)n�), p);
		p = (void*) (((char*)p) + systemInfo.dwPageSize);
		}
	}
	return largestSize;
}
//-----------------------------------------------------------------------------
void XSystemInfo::secondes1970_To_DateTime(time_t secondes1970, unsigned int* date, double* heure)
{
	LONGLONG ll = Int32x32To64(secondes1970, 10000000) + 116444736000000000;
	FILETIME ft;
	ft.dwLowDateTime = (DWORD) ll;
	ft.dwHighDateTime = ll >>32;
	SYSTEMTIME st;
	FileTimeToSystemTime(&ft,&st);

	*date = (int) st.wDay*1000000 + st.wMonth*10000 + st.wYear;
	*heure = st.wHour*10000 + st.wMinute*100 + st.wSecond;
}

//-----------------------------------------------------------------------------
int XSystemInfo::NbProcessors()
{
	SYSTEM_INFO siSysInfo;   
	GetSystemInfo(&siSysInfo); 
	uint16 nbProc = siSysInfo.dwNumberOfProcessors;

	return nbProc;
}
//-----------------------------------------------------------------------------
void XSystemInfo::SleepT(unsigned long milliSeconds)
{
	Sleep(milliSeconds);
	/*
	a impl�menter dans la version unix avec 
	#include <unistd.h>
	 usleep(100*1e3);
	 */

}
//-----------------------------------------------------------------------------
bool XSystemInfo::TestWrintingOnFolder(std::string folder)
{
	XPath P;
	P.AddPathSep(folder);
	std::string testfile = folder + "XSystemInfo.test";
	std::ofstream test(testfile.c_str());
	if(!test)
		return false;
    ::DeleteFile((LPWSTR)testfile.c_str());
	return true;
}
