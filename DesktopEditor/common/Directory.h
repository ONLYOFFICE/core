#ifndef _BUILD_DIRECTORY_CROSSPLATFORM_H_
#define _BUILD_DIRECTORY_CROSSPLATFORM_H_

#include <stdio.h>
#include <string>
#include "Array.h"
#include "File.h"

#if defined(_WIN32) || defined (_WIN64)
    #include "windows.h"
    #include "windef.h"
#elif __linux__
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <unistd.h>
    #include <dirent.h>
#elif MAC
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <unistd.h>
    #include <dirent.h>
#elif _IOS
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <unistd.h>
    #include <dirent.h>
#endif

namespace NSDirectory
{
#ifdef _IOS
    void GetFiles2_ios(std::wstring strDirectory, CArray<std::wstring>& oArray, bool bIsRecursion);
#endif
	static void GetFiles2(std::wstring strDirectory, CArray<std::wstring>& oArray, bool bIsRecursion = false)
    {
#if defined(_WIN32) || defined (_WIN64)
        WIN32_FIND_DATAW oFD;

        std::wstring sSpec = strDirectory + L"\\*.*";
        HANDLE hRes = FindFirstFileW( sSpec.c_str(), &oFD );
        if( INVALID_HANDLE_VALUE == hRes )
            return;
        do
        {
            sSpec = oFD.cFileName;
            if (sSpec != L"." && sSpec != L"..")
            {
                sSpec = strDirectory + L"\\" + sSpec;
                if( !( oFD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
                {
                    oArray.Add(sSpec);
                }
                else if (bIsRecursion)
                {
                    GetFiles2(sSpec, oArray, bIsRecursion);
                }
            }
        } while( FindNextFileW( hRes, &oFD ) );
        FindClose( hRes );
#endif

#ifdef __linux__
        BYTE* pUtf8 = NULL;
        LONG lLen = 0;
        NSFile::CUtf8Converter::GetUtf8StringFromUnicode(strDirectory.c_str(), strDirectory.length(), pUtf8, lLen, false);
        DIR *dp;
        struct dirent *dirp;
        if((dp  = opendir((char*)pUtf8)) != NULL)
        {
            while ((dirp = readdir(dp)) != NULL)
            {
                int nType = 0;
                if(DT_REG == dirp->d_type)
                    nType = 2;
                else if (DT_DIR == dirp->d_type)
                    nType = 1;
                else if (DT_UNKNOWN == dirp->d_type)
                {
                     // XFS problem
                     struct stat buff;
                     std::string sTmp = std::string((char*)pUtf8) + "/" + std::string(dirp->d_name);
                     stat(sTmp.c_str(), &buff);
                     if (S_ISREG(buff.st_mode))
                        nType = 2;
                     else if (S_ISDIR(buff.st_mode))
                        nType = 1;
                }

                if (2 == nType)
                {
                    std::wstring sName = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)dirp->d_name, strlen(dirp->d_name));
                    oArray.Add(strDirectory + L"/" + sName);
                }

                if (bIsRecursion && (1 == nType))
                {
                    if(dirp->d_name[0] != '.')
                    {
                        std::wstring sName = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)dirp->d_name, strlen(dirp->d_name));
                        GetFiles2(strDirectory + L"/" + sName, oArray, bIsRecursion);
                    }
                }
            }
            closedir(dp);
        }
        delete [] pUtf8;
#endif
        
#if defined(MAC) || defined (_IOS)
        BYTE* pUtf8 = NULL;
        LONG lLen = 0;
        NSFile::CUtf8Converter::GetUtf8StringFromUnicode(strDirectory.c_str(), strDirectory.length(), pUtf8, lLen, false);
        DIR *dp;
        struct dirent *dirp;
        if((dp  = opendir((char*)pUtf8)) != NULL)
        {
            while ((dirp = readdir(dp)) != NULL)
            {
                if(DT_REG == dirp->d_type)
                {
                    std::wstring sName = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)dirp->d_name, strlen(dirp->d_name));
                    oArray.Add(strDirectory + L"/" + sName);
                }
                
                if (bIsRecursion && DT_DIR == dirp->d_type)
                {
                    if(dirp->d_name[0] != '.')
                    {
                        std::wstring sName = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)dirp->d_name, strlen(dirp->d_name));
                        GetFiles2(strDirectory + L"/" + sName, oArray, bIsRecursion);
                    }
                }
            }
            closedir(dp);
        }
        delete [] pUtf8;
        return;
#endif
    }

    static CArray<std::wstring> GetFiles(std::wstring strDirectory, bool bIsRecursion = false)
	{
		CArray<std::wstring> oArray;
        GetFiles2(strDirectory, oArray, bIsRecursion);
		return oArray;
	}

	static CArray<std::wstring> GetDirectories(std::wstring strDirectory)
	{
		CArray<std::wstring> oArray;

#if defined(_WIN32) || defined (_WIN64)
		WIN32_FIND_DATAW oFD; 
		
		std::wstring sSpec = strDirectory + L"\\*";
		HANDLE hRes = FindFirstFileW( sSpec.c_str(), &oFD );
		if( INVALID_HANDLE_VALUE == hRes )
			return oArray;
		do 
		{
			sSpec = oFD.cFileName;
			if (sSpec != L"." && sSpec != L"..")
			{
				sSpec = strDirectory + L"\\" + sSpec;
				if( oFD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) 
				{
					oArray.Add(sSpec);
				}
			}
		} while( FindNextFileW( hRes, &oFD ) );		
		FindClose( hRes );
#elif __linux__
		BYTE* pUtf8 = NULL;
		LONG lLen = 0;
        NSFile::CUtf8Converter::GetUtf8StringFromUnicode(strDirectory.c_str(), strDirectory.length(), pUtf8, lLen, false);
		DIR *dp;
		struct dirent *dirp;
		if((dp  = opendir((char*)pUtf8)) != NULL)
		{
			while ((dirp = readdir(dp)) != NULL)
			{
				bool bIsDir = false;
				if (DT_DIR == dirp->d_type)
					bIsDir = true;
				else if (DT_UNKNOWN == dirp->d_type)
				{
					// XFS problem
					struct stat buff;
					std::string sTmp = std::string((char*)pUtf8) + "/" + std::string(dirp->d_name);
					stat(sTmp.c_str(), &buff);
					if (S_ISDIR(buff.st_mode))
						bIsDir = true;
				}
				if(bIsDir)
				{
					if(dirp->d_name[0] != '.')
					{
						std::wstring sName = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)dirp->d_name, strlen(dirp->d_name));
						oArray.Add(strDirectory + L"/" + sName);
					}
				}
			}
			closedir(dp);
		}
		delete [] pUtf8;
#elif MAC
        BYTE* pUtf8 = NULL;
        LONG lLen = 0;
        NSFile::CUtf8Converter::GetUtf8StringFromUnicode(strDirectory.c_str(), strDirectory.length(), pUtf8, lLen, false);
        DIR *dp;
        struct dirent *dirp;
        if((dp  = opendir((char*)pUtf8)) != NULL)
        {
            while ((dirp = readdir(dp)) != NULL)
            {
                if(DT_DIR == dirp->d_type)
                {
                    if(dirp->d_name[0] != '.')
                    {
                        std::wstring sName = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)dirp->d_name, strlen(dirp->d_name));
                        oArray.Add(strDirectory + L"/" + sName);
                    }
                }
            }
            closedir(dp);
        }
        delete [] pUtf8;
#endif
		return oArray;
	}
	static bool Exists(const std::wstring& strDirectory)
	{
#if defined(_WIN32) || defined (_WIN64)
		DWORD dwAttrib = ::GetFileAttributesW(strDirectory.c_str());
		return (dwAttrib != INVALID_FILE_ATTRIBUTES && 0 != (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
#elif __linux__
        BYTE* pUtf8 = NULL;
		LONG lLen = 0;
        NSFile::CUtf8Converter::GetUtf8StringFromUnicode(strDirectory.c_str(), strDirectory.length(), pUtf8, lLen, false);
		struct stat st;
		bool bRes = (0 == stat((char*)pUtf8, &st)) && S_ISDIR(st.st_mode);
		delete [] pUtf8;
		return bRes;
#elif MAC
        BYTE* pUtf8 = NULL;
        LONG lLen = 0;
        NSFile::CUtf8Converter::GetUtf8StringFromUnicode(strDirectory.c_str(), strDirectory.length(), pUtf8, lLen, false);
        struct stat st;
        bool bRes = (0 == stat((char*)pUtf8, &st)) && S_ISDIR(st.st_mode);
        delete [] pUtf8;
        return bRes;
#endif
        return false;
	}
	static bool CreateDirectory(const std::wstring& strDirectory)
	{
		if (Exists(strDirectory) == true)  return true;

#if defined(_WIN32) || defined (_WIN64)
		return FALSE != ::CreateDirectoryW(strDirectory.c_str(), NULL);
#elif __linux__
		BYTE* pUtf8 = NULL;
		LONG lLen = 0;
        NSFile::CUtf8Converter::GetUtf8StringFromUnicode(strDirectory.c_str(), strDirectory.length(), pUtf8, lLen, false);
		struct stat st;
		int nRes = 0;
		if (stat((char*)pUtf8, &st) == -1) {
			nRes = mkdir((char*)pUtf8, S_IRWXU | S_IRWXG | S_IRWXO);
		}
		delete [] pUtf8;
		return 0 == nRes;
#elif MAC
        BYTE* pUtf8 = NULL;
        LONG lLen = 0;
        NSFile::CUtf8Converter::GetUtf8StringFromUnicode(strDirectory.c_str(), strDirectory.length(), pUtf8, lLen, false);
        struct stat st;
        int nRes = 0;
        if (stat((char*)pUtf8, &st) == -1) {
            nRes = mkdir((char*)pUtf8, S_IRWXU | S_IRWXG | S_IRWXO);
        }
        delete [] pUtf8;
        return 0 == nRes;
#endif
        return false;
	}
	static void DeleteDirectory(const std::wstring& strDirectory, bool deleteRoot = true)
	{
		CArray<std::wstring> aFiles = GetFiles(strDirectory);
		for(int i = 0; i < aFiles.GetCount(); ++i)
		{
			NSFile::CFileBinary::Remove(aFiles[i]);
		}
		CArray<std::wstring> aDirectories = GetDirectories(strDirectory);
		for(int i = 0; i < aDirectories.GetCount(); ++i)
		{
			DeleteDirectory(aDirectories[i]);
		}
#if defined(_WIN32) || defined (_WIN64)
		if (deleteRoot) RemoveDirectoryW(strDirectory.c_str());
#elif __linux__
		BYTE* pUtf8 = NULL;
		LONG lLen = 0;
        NSFile::CUtf8Converter::GetUtf8StringFromUnicode(strDirectory.c_str(), strDirectory.length(), pUtf8, lLen, false);
		rmdir((char*)pUtf8);
		delete [] pUtf8;

		if (deleteRoot = false)CreateDirectory(strDirectory);
#elif MAC
        BYTE* pUtf8 = NULL;
        LONG lLen = 0;
        NSFile::CUtf8Converter::GetUtf8StringFromUnicode(strDirectory.c_str(), strDirectory.length(), pUtf8, lLen, false);
        rmdir((char*)pUtf8);
        delete [] pUtf8;
        
        if (deleteRoot = false)CreateDirectory(strDirectory);
#endif
	}
	static std::wstring GetFolderPath(const std::wstring& wsFolderPath)
	{
		int n1 = wsFolderPath.rfind('\\');
		if (n1 < 0)
		{
			n1 = wsFolderPath.rfind('/');
			if (n1 < 0)
			{
				return L"";
			}
			return wsFolderPath.substr(0, n1);
		}
		return wsFolderPath.substr(0, n1);
	}
}

#endif //_BUILD_DIRECTORY_CROSSPLATFORM_H_
