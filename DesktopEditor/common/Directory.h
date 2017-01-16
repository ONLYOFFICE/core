/*
 * (c) Copyright Ascensio System SIA 2010-2016
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#ifndef _BUILD_DIRECTORY_CROSSPLATFORM_H_
#define _BUILD_DIRECTORY_CROSSPLATFORM_H_

#include <stdio.h>
#include <string>
#include "Array.h"
#include "File.h"

#if defined(_WIN32) || defined (_WIN64)
    #include "windows.h"
    #include "windef.h"
    #include <shlobj.h>
    #include <Rpc.h>
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

#ifndef FILE_SEPARATOR
	#if defined(_WIN32) || defined(_WIN64)
		#define FILE_SEPARATOR
		#define FILE_SEPARATOR_CHAR '\\'
        #define FILE_SEPARATOR_STR L"\\"
	#else
		#define FILE_SEPARATOR
		#define FILE_SEPARATOR_CHAR '/'
        #define FILE_SEPARATOR_STR L"/"
	#endif
#endif

namespace NSDirectory
{
#if !defined(_WIN32) && !defined (_WIN64)
    static bool is_directory_exist(char* dir)
    {
        struct stat st;
        bool bRes = (0 == stat(dir, &st)) && S_ISDIR(st.st_mode);
        return bRes;
    }

    static bool _mkdir (const char *dir)
    {
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
                        res = is_directory_exist(tmp);
                        if (!res)
                            res = (0 == mkdir(tmp, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH));
                        *p = '/';
                        if (!res)
                            break;
                }
        if (res)
            res = (0 == mkdir(tmp, S_IRWXU));
        return res;
    }
#endif

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
		
		if (!strDirectory.empty())
		{     
			GetFiles2(strDirectory, oArray, bIsRecursion);
		}
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
        bool bRes = is_directory_exist((char*)pUtf8);
        delete [] pUtf8;
        return bRes;
#elif MAC
        BYTE* pUtf8 = NULL;
        LONG lLen = 0;
        NSFile::CUtf8Converter::GetUtf8StringFromUnicode(strDirectory.c_str(), strDirectory.length(), pUtf8, lLen, false);
        struct stat st;
        bool bRes = is_directory_exist((char*)pUtf8);
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
	static bool CreateDirectories(const std::wstring& strDirectory)
	{
		if (Exists(strDirectory) == true)  return true;

#if defined(_WIN32) || defined (_WIN64)
		DWORD dwAttrib = ::GetFileAttributesW(strDirectory.c_str());
		if  (dwAttrib != INVALID_FILE_ATTRIBUTES && 0 != (dwAttrib & FILE_ATTRIBUTE_DIRECTORY)) return true;

		int codeResult = ERROR_SUCCESS;

		SECURITY_ATTRIBUTES sa={};

		codeResult = SHCreateDirectoryExW(NULL, strDirectory.c_str(), &sa);

		bool created = false;
		if (codeResult == ERROR_SUCCESS)
			created = true;

		return created;
#else
		std::string pathUtf8 = U_TO_UTF8(strDirectory);
		return _mkdir (pathUtf8.c_str());
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
        static std::wstring CreateTempFileWithUniqueName (const std::wstring & strFolderPathRoot, std::wstring Prefix)
        {
#if defined(_WIN32) || defined (_WIN64)
            wchar_t pBuffer [MAX_PATH+1];
            memset (pBuffer, 0, sizeof (wchar_t) * (MAX_PATH+1));

            /*unRet = */GetTempFileNameW( strFolderPathRoot.c_str(), Prefix.c_str(), 0,pBuffer);

            return std::wstring(pBuffer);
#else
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
#endif
        }
        static std::wstring CreateDirectoryWithUniqueName (const std::wstring & strFolderPathRoot)
        {
#if defined(_WIN32) || defined (_WIN64)
            UUID uuid;
            RPC_WSTR str_uuid;
            UuidCreate (&uuid);
            UuidToStringW (&uuid, &str_uuid);
                    std::wstring pcTemplate = strFolderPathRoot + FILE_SEPARATOR_STR;
            pcTemplate += (wchar_t *) str_uuid;
            RpcStringFreeW (&str_uuid);

            int attemps = 10;
            while (!CreateDirectory(pcTemplate))
            {
                UuidCreate (&uuid);
                UuidToStringW (&uuid, &str_uuid);
                pcTemplate = strFolderPathRoot + FILE_SEPARATOR_STR;
                pcTemplate += (wchar_t *) str_uuid;
                RpcStringFreeW (&str_uuid);
                attemps--;

                if (0 == attemps)
                {
                    pcTemplate = L"";
                }
            }
            return pcTemplate;
#else
            std::string pcTemplate = stringWstingToUtf8String (strFolderPathRoot) + "/ascXXXXXX";
            char *pcRes = mkdtemp(const_cast <char *> (pcTemplate.c_str()));
            if (NULL == pcRes)
                return _T("");

            std::string sRes = pcRes;
            return stringUtf8ToWString (sRes);
#endif
        }
        static std::wstring GetTempPath()
        {
#if defined(_WIN32) || defined (_WIN64)
            wchar_t pBuffer [MAX_PATH+1];
            memset (pBuffer, 0, sizeof (wchar_t) * (MAX_PATH+1));
                    ::GetTempPathW (MAX_PATH, pBuffer);
            std::wstring res;
            res += std::wstring(pBuffer);

            int nSeparator1Pos = res.rfind(L"/");
            if (-1 == nSeparator1Pos)
            {
                nSeparator1Pos = res.rfind(L"\\");
            }

            if (-1 == nSeparator1Pos)
                return L"";

            return res.substr (0, nSeparator1Pos);
#else
            char *folder = getenv ("TEMP");

            if (NULL == folder)
                folder = getenv ("TMP");
            if (NULL == folder)
                folder = getenv ("TMPDIR");

            if (NULL == folder)
                folder = "/tmp";

            return stringUtf8ToWString(folder);
#endif
        }

        static int GetFilesCount(const std::wstring& path, const bool& recursive)
        {
#if defined(_WIN32) || defined (_WIN64)
            std::wstring pathMask = path + L"\\*";
            int filesCount = 0;

            WIN32_FIND_DATAW findData;
            HANDLE findResult = FindFirstFileW(pathMask.c_str(), &findData);

            do {
                if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                    if (!recursive)
                        continue;
                    if ((std::wstring) findData.cFileName == L".")
                        continue;
                    if ((std::wstring) findData.cFileName == L"..")
                        continue;
                    std::wstring innerPath = path + L'\\' + (std::wstring) findData.cFileName;
                    filesCount += GetFilesCount(innerPath, recursive);
                }
                else
                    ++filesCount;
            } while (FindNextFileW(findResult, &findData));

            FindClose(findResult);
            return filesCount;
#else
            std::string path_utf8 = stringWstingToUtf8String(path.c_str());

            std::vector<std::string> files;
            listdir (path_utf8.c_str(), recursive, files);

            return files.size()+1;
#endif
        }
#if !defined(_WIN32) && !defined (_WIN64)
        static bool PathIsDirectory(const std::wstring& pathName)
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
#endif
}

#endif //_BUILD_DIRECTORY_CROSSPLATFORM_H_
