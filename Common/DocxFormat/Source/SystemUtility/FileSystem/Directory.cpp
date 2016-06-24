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
#include "Directory.h"

#include <winbase.h>
#include <shlobj.h>
#include "Rpc.h"    // 'UuidCreate' functuin, need to link Rpcrt4.lib

#pragma comment(lib,"rpcrt4.lib")

#ifndef FILE_SEPARATOR
	#define FILE_SEPARATOR
	#define FILE_SEPARATOR_CHAR '\\'
	#define FILE_SEPARATOR_STR _T("\\")
#endif

#include "../../../../../DesktopEditor/common/Directory.h"

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
		DWORD dwAttrib = ::GetFileAttributesW(path.c_str());
		if  (dwAttrib != INVALID_FILE_ATTRIBUTES && 0 != (dwAttrib & FILE_ATTRIBUTE_DIRECTORY)) return true;
		
		return CreateDirectory(path.c_str());
    }
    bool Directory::CreateDirectory(String strFolderPathRoot, String strFolderName)
    {
        String strFolder = strFolderPathRoot;
        strFolder += FILE_SEPARATOR_STR;
        strFolder += strFolderName;
        return CreateDirectory(strFolder);
    }
	CString Directory::CreateTempFileWithUniqueName (const CString & strFolderPathRoot, CString Prefix)
	{
        TCHAR pBuffer [MAX_PATH+1];
        memset (pBuffer, 0, sizeof (TCHAR) * (MAX_PATH+1));

		/*unRet = */GetTempFileName( strFolderPathRoot.GetString(), Prefix.GetString(), 0,pBuffer);

		return CString(pBuffer);
	}
	std::wstring Directory::CreateTempFileWithUniqueName (const std::wstring & strFolderPathRoot, std::wstring Prefix)
	{
        TCHAR pBuffer [MAX_PATH+1];
        memset (pBuffer, 0, sizeof (TCHAR) * (MAX_PATH+1));

		/*unRet = */GetTempFileName( strFolderPathRoot.c_str(), Prefix.c_str(), 0,pBuffer);

		return std::wstring(pBuffer);
	}
	CString Directory::GetLongPathName_(const CString& fileName)
	{
		long length = GetLongPathNameW(fileName, NULL, 0);
		if(length < 1 )return _T("");
		wchar_t *buffer = new wchar_t[length];
		if (buffer == NULL)return _T(""); 
		length = GetLongPathName(fileName, buffer, length);
		if(length == 0)
		{
			delete []buffer;
			return _T("");
		}
		CString sPathFileName(buffer);
		delete []buffer;
		return sPathFileName;
	}

	CString Directory::GetTempPath()
	{
        TCHAR pBuffer [MAX_PATH+1];
        memset (pBuffer, 0, sizeof (TCHAR) * (MAX_PATH+1));
		::GetTempPathW (MAX_PATH, pBuffer);
        CString res;
        res.AppendFormat(_T("%ls"), pBuffer);
       
		int nSeparator1Pos = res.ReverseFind(_T('/'));
        if (-1 == nSeparator1Pos)
        {
            nSeparator1Pos = res.ReverseFind(_T('\\'));
        }

        if (-1 == nSeparator1Pos)
            return _T("");

		return res.Mid (0, nSeparator1Pos);
	}
	void Directory::DeleteDirectory(const CString& strDirectory, bool deleteRoot)
	{
		NSDirectory::DeleteDirectory(strDirectory.GetString(), deleteRoot);
	}

    CString Directory::CreateDirectoryWithUniqueName (CString & strFolderPathRoot)
    {
        UUID uuid;
        RPC_WSTR str_uuid;
        UuidCreate (&uuid);
        UuidToString (&uuid, &str_uuid);
        CString pcTemplate = strFolderPathRoot + FILE_SEPARATOR_STR;
        pcTemplate += (TCHAR *) str_uuid;
        RpcStringFree (&str_uuid);

        int attemps = 10;
        while (!CreateDirectory(pcTemplate))
        {
            UuidCreate (&uuid);
            UuidToString (&uuid, &str_uuid);
            pcTemplate = strFolderPathRoot + FILE_SEPARATOR_STR;
            pcTemplate += (TCHAR *) str_uuid;
            RpcStringFree (&str_uuid);
            attemps--;

            if (0 == attemps)
            {
                pcTemplate = _T("");
            }
        }
        return pcTemplate;
    }
	std::wstring Directory::CreateDirectoryWithUniqueName (const std::wstring & strFolderPathRoot)
    {
        UUID uuid;
        RPC_WSTR str_uuid;
        UuidCreate (&uuid);
        UuidToString (&uuid, &str_uuid);
		std::wstring pcTemplate = strFolderPathRoot + FILE_SEPARATOR_STR;
        pcTemplate += (TCHAR *) str_uuid;
        RpcStringFree (&str_uuid);

        int attemps = 10;
        while (!CreateDirectory(pcTemplate))
        {
            UuidCreate (&uuid);
            UuidToString (&uuid, &str_uuid);
            pcTemplate = strFolderPathRoot + FILE_SEPARATOR_STR;
            pcTemplate += (TCHAR *) str_uuid;
            RpcStringFree (&str_uuid);
            attemps--;

            if (0 == attemps)
            {
                pcTemplate = _T("");
            }
        }
        return pcTemplate;
    }
    bool Directory::CreateDirectories(LPCTSTR path) 
	{
		DWORD dwAttrib = ::GetFileAttributesW(path);
		if  (dwAttrib != INVALID_FILE_ATTRIBUTES && 0 != (dwAttrib & FILE_ATTRIBUTE_DIRECTORY)) return true;

		int codeResult = ERROR_SUCCESS;

		SECURITY_ATTRIBUTES sa={};

		codeResult = SHCreateDirectoryEx(NULL, path, &sa);

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
	CString Directory::GetFolderPath(const CString& strFolderPath)
	{
        int n1 = strFolderPath.ReverseFind('\\');
        if (n1 < 0 )
		{
			n1 = strFolderPath.ReverseFind('/');
			if (n1 < 0 )
			{           
				return _T("");
			}
			return strFolderPath.Mid(0,n1);
		}
        return strFolderPath.Mid(0,n1);
	}
#if defined (_WIN32) || defined(_WIN64)
	std::wstring Directory::GetFolderPath(const std::wstring & strFolderPath)
	{
        int n1 = strFolderPath.rfind('\\');
        if (n1 < 0 )
		{
			n1 = strFolderPath.rfind('/');
			if (n1 < 0 )
			{           
				return _T("");
			}
			return strFolderPath.substr(0,n1);
		}
        return strFolderPath.substr(0,n1);
	}
	void Directory::DeleteDirectory(std::wstring & strDirectory, bool deleteRoot)
	{
		NSDirectory::DeleteDirectory(strDirectory, deleteRoot);
	}
#endif
        bool Directory::IsExist(const std::wstring&  strFileName)
        {
			DWORD ftyp = GetFileAttributesW(strFileName.c_str());
			
			if (ftyp == INVALID_FILE_ATTRIBUTES)	return false; 
			if (ftyp & FILE_ATTRIBUTE_DIRECTORY)	return true;  
			if (ftyp & FILE_ATTRIBUTE_NORMAL ||
				ftyp & FILE_ATTRIBUTE_ARCHIVE)		return true;  

			return false;
        }
}
