#include "SystemUtility.h"

#ifdef _WIN32
#include <windows.h>
#else
#include "../Base/ASCString.h"
#endif




#include "FileSystem/FileSystem.h"

//#include "AVSUtils.h"
//#include "File.h"

namespace OOX
{
    CPath::CPath() : m_strFilename(L"") 
    {
    }
    CPath::CPath(const CString& sName, bool bIsNorm) : m_strFilename(sName)
    {
		if (bIsNorm)
			Normalize();
    }
    CPath::CPath(LPCSTR& sName, bool bIsNorm) : m_strFilename(sName)
    {
		if (bIsNorm)
			Normalize();
    }
    CPath::CPath(LPCWSTR& sName, bool bIsNorm) : m_strFilename(sName)
    {
		if (bIsNorm)
			Normalize();
    }

    CPath::CPath(const CPath& oSrc)
    {
        *this = oSrc;
		//считаем вызывать ли Normalize решает oSrc
		//Normalize();
    }

    CPath& CPath::operator=(const CPath& oSrc)
    {
        m_strFilename = oSrc.m_strFilename;
		//считаем вызывать ли Normalize решает oSrc
		//Normalize();
        return *this;
    }
	CPath& CPath::operator=(const CString& oSrc)
    {
        m_strFilename = oSrc;
		Normalize();
        return *this;
    }
    CPath& CPath::operator=(LPCSTR oSrc)
    {
        m_strFilename = oSrc;
        Normalize();
        return *this;
    }
    CPath& CPath::operator=(LPCWSTR oSrc)
    {
        m_strFilename = oSrc;
        Normalize();
        return *this;
    }

	AVSINLINE CString CPath::GetExtention(bool bIsPoint) const
    {
        int nFind = m_strFilename.ReverseFind('.');
        if (-1 == nFind)
            return _T("");

        if (!bIsPoint)
            ++nFind;

        return m_strFilename.Mid(nFind);
    }
    AVSINLINE CString CPath::GetDirectory(bool bIsSlash) const
    {
        int nPos = m_strFilename.ReverseFind('\\');
        if (-1 == nPos)
        {
            return m_strFilename;
        }
        else
        {
            if (bIsSlash)
                ++nPos;
            return m_strFilename.Mid(0, nPos);
        }
    }
    AVSINLINE CString CPath::GetPath() const
    {
        return m_strFilename;
    }
}

namespace OOX
{
    bool CSystemUtility::CreateFile(const CString& strFileName)
    {
#ifdef _WIN32
		BSTR strPath = strFileName.AllocSysString();
        HANDLE hResult = ::CreateFile(strPath, GENERIC_READ, 0, NULL, 
            CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        SysFreeString(strPath);

        if (hResult == INVALID_HANDLE_VALUE)
            return false;
        if (!CloseHandle(hResult))
            return false;
        
        return true;
#else
        std::string path_string = stringWstingToUtf8String(strFileName);
        FILE * pFile = fopen (path_string.c_str(), "wb");
        if (NULL != pFile)
        {
            fclose (pFile);
            return true;
        }

        return false;
#endif
    }

    bool CSystemUtility::IsFileExist(const CString& strFileName)
    {
        return FileSystem::File::Exists(strFileName);
    }
    bool CSystemUtility::IsFileExist(const CPath& oPath)
    {
        return IsFileExist(oPath.GetPath());
    }

    CString CSystemUtility::GetDirectoryName(const CString& strFileName)
    {
        CPath oPath(strFileName);
        return oPath.GetDirectory();
    }

    int CSystemUtility::GetFilesCount(const CString& strDirPath, const bool& bRecursive)
    {
        return FileSystem::Directory::GetFilesCount(strDirPath, bRecursive);
    }

    CString CSystemUtility::GetFileExtention(const CString& strFileName)
    {
        CPath oPath(strFileName);
        return oPath.GetExtention();
    }

    bool CSystemUtility::CreateDirectories(const CPath& oPath)
    {
        return FileSystem::Directory::CreateDirectories(oPath.m_strFilename);
    }

    void CSystemUtility::ReplaceExtention(CString& strName, CString& str1, CString& str2)
    {
        return;
    }
}
