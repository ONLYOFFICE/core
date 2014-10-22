#include "Directory.h"

#include <shlobj.h>


namespace FileSystem 
{
    LPCTSTR Directory::GetCurrentDirectory() 
	{
        static const int bufferSize = MAX_PATH;
        LPTSTR directory = new TCHAR[bufferSize];

        DWORD lenght = ::GetCurrentDirectory(bufferSize, directory);
        if (lenght == 0) {
            delete[] directory;
            directory = NULL;
        }

        return directory;
    }
    String Directory::GetCurrentDirectoryS() 
	{
        LPCTSTR directory = GetCurrentDirectory();
        if (directory == NULL)
            return String();

        return String(directory);
    }

    bool Directory::CreateDirectory(LPCTSTR path)
	{
        bool directoryCreated = false;
        if (::CreateDirectory(path, NULL) == TRUE)
            directoryCreated = true;
        return directoryCreated;
    }
    bool Directory::CreateDirectory(const String& path) 
	{
        return CreateDirectory(path.c_str());
    }
    bool Directory::CreateDirectory(String strFolderPathRoot, String strFolderName)
    {
        String strFolder = strFolderPathRoot;
        strFolder += _T("/");
        strFolder += strFolderName;
        return CreateDirectory(strFolder);
    }
    bool Directory::CreateDirectories(LPCTSTR path) 
	{
		int codeResult = ERROR_SUCCESS;

		codeResult = SHCreateDirectory(NULL, path);

        bool created = false;
        if (codeResult == ERROR_SUCCESS)
            created = true;

        return created;
    }

    StringArray Directory::GetFilesInDirectory(LPCTSTR path, const bool& andSubdirectories) {
        size_t pathLength = 0;
        StringCchLength(path, MAX_PATH, &pathLength);
        ++pathLength;
        size_t pathToFilesLength = pathLength + 3;
        LPTSTR pathToFiles = new TCHAR[pathToFilesLength];

        StringCchCopy(pathToFiles, pathLength, path);
        StringCchCat(pathToFiles, pathToFilesLength, TEXT("\\*"));

        WIN32_FIND_DATA findData;
        HANDLE findResult = FindFirstFile(pathToFiles, &findData);
        delete[] pathToFiles;

        if (findResult == INVALID_HANDLE_VALUE)
            return StringArray();

        StringArray files;
        do {
            if (andSubdirectories || !(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                String file = findData.cFileName;
                files.insert(files.end(), file);
            }
        } while (FindNextFile(findResult, &findData));

        FindClose(findResult);

        return files;
    }
    StringArray Directory::GetFilesInDirectory(const String& path, const bool& andSubdirectories) {
        LPCTSTR pathW = path.c_str();
        return GetFilesInDirectory(pathW, andSubdirectories);
    }

    int Directory::GetFilesCount(const CString& path, const bool& recursive) {
        CString pathMask = path + _T("\\*");

        WIN32_FIND_DATA findData;
        HANDLE findResult = FindFirstFile(pathMask, &findData);

        int filesCount = 0;
        do {
            if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                if (!recursive)
                    continue;
                if ((CString) findData.cFileName == _T("."))
                    continue;
                if ((CString) findData.cFileName == _T(".."))
                    continue;
                CString innerPath = path + _T('\\') + (CString) findData.cFileName;
                filesCount += GetFilesCount(innerPath, recursive);
            }
            else
                ++filesCount;
        } while (FindNextFile(findResult, &findData));

        FindClose(findResult);

        return filesCount;
    }

}
