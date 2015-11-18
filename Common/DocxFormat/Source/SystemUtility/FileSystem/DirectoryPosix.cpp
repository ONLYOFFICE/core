#include "Directory.h"
#include "../../Base/ASCString.h"
#include "../../Base/Base.h"

#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>

#ifndef MAX_PATH
#define MAX_PATH 512
#endif

#include "../../../../../DesktopEditor/common/Directory.h"

namespace FileSystem {

    // recursively make directories by path
    // returns true if dir was created
    static bool _mkdir (const char *dir) {
            char tmp[MAX_PATH];
            char *p = NULL;
            size_t len;
            bool res = true;

            snprintf(tmp, sizeof(tmp),"%s",dir);
            len = strlen(tmp);
            if(tmp[len - 1] == '/')
                    tmp[len - 1] = 0;
            for(p = tmp + 1; *p; p++)
                    if(*p == '/') {
                            *p = 0;
                            res = (0 == mkdir(tmp, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH));
                            *p = '/';
                            if (!res)
                                break;
                    }
            if (res)
                res = (0 == mkdir(tmp, S_IRWXU));
            return res;
    }


    // FIXME: not thread safe (from win32 code, but there are no memory leaks)
    LPCTSTR Directory::GetCurrentDirectory() {
        char spc_current_dir [MAX_PATH];
        static CString wspc_current_dir;

        getcwd (spc_current_dir, MAX_PATH);
        std::string sDir;
        sDir = spc_current_dir;
        wspc_current_dir = stringUtf8ToWString (sDir);

        return wspc_current_dir.c_str();
    }
    String Directory::GetCurrentDirectoryS() {
        std::string sDir;
        char * pc_current_dir = getcwd(NULL, 0);
        if (NULL != pc_current_dir)
        {
            sDir = pc_current_dir;
            free (pc_current_dir);
        }
        else
            return String();

        return stringUtf8ToWString (sDir);
    }
    void Directory::DeleteDirectory(const CString& strDirectory, bool deleteRoot )
	{
        NSDirectory::DeleteDirectory(strDirectory.GetString(), deleteRoot);
	}
	CString Directory::GetTempPath()
	{
        char *folder = getenv ("TEMP");

        if (NULL == folder)
            folder = getenv ("TMP");
        if (NULL == folder)
            folder = getenv ("TMPDIR");

        if (NULL == folder)
            folder = "/tmp";

        return stringUtf8ToWString(folder);
	}
    bool Directory::CreateDirectory (LPCTSTR path)
	{
        bool directoryCreated = false;

        std::wstring wsPath = path;
        std::string sPathUtf8 = stringWstingToUtf8String (wsPath);

        // read/write/search permissions for owner and group, and with read/search permissions for others
        if (0 == mkdir (sPathUtf8.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH))
            directoryCreated = true;

        return directoryCreated;
    }
    bool Directory::CreateDirectory (const String& path)
	{
        return Directory::CreateDirectory(path.c_str());
    }
    CString Directory::CreateTempFileWithUniqueName (const CString & strFolderPathRoot,CString Prefix)
    {
        char pcRes[MAX_PATH];
        if (NULL == pcRes) return _T("");

        Prefix = strFolderPathRoot + FILE_SEPARATOR_STR + Prefix + _T("_XXXXXX");

        std::wstring w_str  = Prefix.GetBuffer();
        std::string a_str   = stringWstingToUtf8String(w_str);

        memcpy(pcRes, a_str.c_str(), a_str.length());
        pcRes[a_str.length()] = '\0';

        int res = mkstemp( pcRes);
        if (-1 != res)
            close(res);

        std::string sRes = pcRes;
        return stringUtf8ToWString (sRes);
    }
    std::wstring Directory::CreateTempFileWithUniqueName (const std::wstring & strFolderPathRoot, std::wstring Prefix)
    {
        char pcRes[MAX_PATH];
        if (NULL == pcRes) return _T("");

        Prefix = strFolderPathRoot + FILE_SEPARATOR_STR + Prefix + _T("_XXXXXX");

        std::wstring w_str  = Prefix.c_str();
        std::string a_str   = stringWstingToUtf8String(w_str);

        memcpy(pcRes, a_str.c_str(), a_str.length());
        pcRes[a_str.length()] = '\0';

        int res = mkstemp( pcRes);
        if (-1 != res)
            close(res);

        std::string sRes = pcRes;
        return stringUtf8ToWString (sRes);
    }
    bool Directory::CreateDirectory (String strFolderPathRoot, String strFolderName)
    {
        String strFolder = strFolderPathRoot;
        strFolder += FILE_SEPARATOR_STR ;
        strFolder += strFolderName;
        return CreateDirectory(strFolder);
    }
    CString Directory::CreateDirectoryWithUniqueName (CString & strFolderPathRoot)
    {
        std::string pcTemplate = stringWstingToUtf8String (strFolderPathRoot) + "/ascXXXXXX";
        char *pcRes = mkdtemp(const_cast <char *> (pcTemplate.c_str()));
        if (NULL == pcRes)
            return _T("");

        std::string sRes = pcRes;
        return stringUtf8ToWString (sRes);
    }
    std::wstring Directory::CreateDirectoryWithUniqueName (const std::wstring & strFolderPathRoot)
    {
        std::string pcTemplate = stringWstingToUtf8String (strFolderPathRoot) + "/ascXXXXXX";
        char *pcRes = mkdtemp(const_cast <char *> (pcTemplate.c_str()));
        if (NULL == pcRes)
            return _T("");

        std::string sRes = pcRes;
        return stringUtf8ToWString (sRes);
    }
    bool Directory::CreateDirectories(LPCTSTR path)
	{
        std::wstring pathWstring;
        pathWstring = path;
        std::string pathUtf8 = stringWstingToUtf8String (pathWstring);
        return _mkdir (pathUtf8.c_str());
    }

    // recursive directory scanning routine
    bool listdir (const char* dirname, const bool recursive, std::vector<std::string>& files)
    {
      DIR* d_fh;
      struct dirent* entry;
      std::string longest_name;

      while ( (d_fh = opendir(dirname)) == NULL)
      {
        // Couldn't open directory
        return false;
      }

      while ((entry=readdir(d_fh)) != NULL) {

        // Don't descend up the tree or include the current directory
        if(strncmp(entry->d_name, "..", 2) != 0 &&
           strncmp(entry->d_name, ".", 1) != 0)
        {
          bool bIsDir = false;
          if (DT_DIR == entry->d_type)
              bIsDir = true;
          else if (DT_UNKNOWN == entry->d_type)
          {
              // XFS problem
              struct stat buff;
              std::string sTmp = std::string((char*)dirname) + "/" + std::string(entry->d_name);
              stat(sTmp.c_str(), &buff);
              if (S_ISDIR(buff.st_mode))
                  bIsDir = true;
          }
          // If it's a directory recurse into it
          if (bIsDir)
          {
              if (recursive)
              {
                    // Prepend the current directory and recurse
                    longest_name = dirname;
                    longest_name += "/";
                    longest_name += entry->d_name;

                    if (!listdir(longest_name.c_str(), recursive, files))
                        return false;
              }
          }
          else
          {
              // it's a file
              std::string sFileName;
              sFileName = dirname;
              sFileName += "/";
              sFileName += entry->d_name;

              files.push_back(sFileName);
          }
        }
      }

      closedir(d_fh);

      return true;
    }


    StringArray Directory::GetFilesInDirectory(LPCTSTR path, const bool& andSubdirectories)
    {
        std::wstring path_wstring = path;
        std::string path_utf8 = stringWstingToUtf8String(path_wstring);

        std::vector<std::string> files;
        listdir (path_utf8.c_str(), andSubdirectories, files);

        StringArray files_wstring;
        std::for_each (std::begin(files), std::end(files), [&] (std::string file)
        {
            std::wstring file_wstring = stringUtf8ToWString (file);
            files_wstring.push_back (file_wstring);
        });

        return files_wstring;
    }
    StringArray Directory::GetFilesInDirectory(const String& path, const bool& andSubdirectories) {
        LPCTSTR pathW = path.c_str();
        return GetFilesInDirectory(pathW, andSubdirectories);
    }

    int Directory::GetFilesCount(const CString& path, const bool& recursive) {

        std::string path_utf8 = stringWstingToUtf8String(path.c_str());

        std::vector<std::string> files;
        listdir (path_utf8.c_str(), recursive, files);

        return files.size()+1;
    }
    CString Directory::GetFolderPath(const CString& strFolderPath)
    {
        int n1 = strFolderPath.rfind(FILE_SEPARATOR_STR);
        if (n1 < 0 )
            return _T("");
        return strFolderPath.substr(0,n1);
    }

    CString Directory::GetLongPathName_(const CString& fileName)
    {
        return fileName;
    }

    bool Directory::PathIsDirectory(const CString& pathName)
    {
        struct stat s;

        std::string sPathNameUtf8 = stringWstingToUtf8String (pathName);
        if (stat(sPathNameUtf8.c_str(), &s) == 0)
        {
            if (s.st_mode & S_IFDIR)return true;
            else return false;
        }

        return false;
    }
    bool Directory::IsExist(const std::wstring&  strFileName)
    {
		struct stat sb;

        std::string sPathNameUtf8 = stringWstingToUtf8String (strFileName);
        if (stat(sPathNameUtf8.c_str(), &sb) == 0)
		{
			if (S_ISDIR(sb.st_mode)) return true;
			if (S_ISREG(sb.st_mode)) return true;
		}
		return false;
    }
}
