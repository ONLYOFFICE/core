/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
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

#include "../Base/Base.h"
#include "../Base/Unit.h"
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Directory.h"
#include "SystemUtility.h"

namespace OOX
{
	CPath::CPath() : m_strFilename(L"")
	{
		CheckIsRoot();
	}

	CPath::CPath(const std::wstring& sName, bool bIsNorm) : m_strFilename(sName)
	{
		CheckIsRoot();
		if (bIsNorm)
			Normalize();
	}
	CPath::CPath(const char*& sName, bool bIsNorm)
	{
		std::string s(sName);
		m_strFilename = std::wstring(s.begin(), s.end());
		CheckIsRoot();
		if (bIsNorm)
			Normalize();
	}
	CPath::CPath(const wchar_t*& sName, bool bIsNorm) : m_strFilename(sName)
	{
		CheckIsRoot();
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
		m_bIsRoot = oSrc.m_bIsRoot;
		//считаем вызывать ли Normalize решает oSrc
		//Normalize();
		return *this;
	}
	CPath& CPath::operator=(const std::wstring& oSrc)
	{
		m_strFilename = oSrc;
		Normalize();
		return *this;
	}
	CPath& CPath::operator=(const char* oSrc)
	{
		std::string s(oSrc);
		m_strFilename = std::wstring(s.begin(), s.end());
		Normalize();
		return *this;
	}
	CPath& CPath::operator=(const wchar_t* oSrc)
	{
		m_strFilename = oSrc;
		Normalize();
		return *this;
	}
	void CPath::Normalize()
	{
		if (0 == m_strFilename.length())
			return;

		//todooo оптимизировать
		XmlUtils::replace_all(m_strFilename, L"/", FILE_SEPARATOR_STR);
		XmlUtils::replace_all(m_strFilename, L"\\", FILE_SEPARATOR_STR);

		const wchar_t*  pData   = m_strFilename.c_str();
		int             nLen    = (int) m_strFilename.length();

		wchar_t*    pDataNorm       = new wchar_t   [nLen + 1];
		int*        pSlashPoints    = new int       [nLen + 1];

		int nStart          = 0;
		int nCurrent        = 0;
		int nCurrentSlash   = -1;
		int nCurrentW       = 0;
		bool bIsUp          = false;

#if defined(_WIN32) || defined (_WIN64)
		std::wstring prepending = std::wstring(FILE_SEPARATOR_STR) + FILE_SEPARATOR_STR + L"?";
		if (0 == m_strFilename.find(prepending))
		{
			pDataNorm[nCurrentW++] = FILE_SEPARATOR_CHAR;
			pDataNorm[nCurrentW++] = FILE_SEPARATOR_CHAR;
			pDataNorm[nCurrentW++] = L'?';
			pDataNorm[nCurrentW++] = FILE_SEPARATOR_CHAR;

			nStart = nCurrent = 3;
		}
#else
		if (pData[nCurrent] == FILE_SEPARATOR_CHAR)
			pDataNorm[nCurrentW++] = pData[nCurrent];
#endif
		while (nCurrent < nLen)
		{
			if (pData[nCurrent] == FILE_SEPARATOR_CHAR)
			{
				if (nStart < nCurrent)
				{
					bIsUp = false;
					if ((nCurrent - nStart) == 2)
					{
						if (pData[nStart] == (wchar_t)'.' && pData[nStart + 1] == (wchar_t)'.')
						{
							if (nCurrentSlash > 0)
							{
								--nCurrentSlash;
								nCurrentW = pSlashPoints[nCurrentSlash];
								bIsUp = true;
							}
						}
					}
					if (!bIsUp)
					{
						pDataNorm[nCurrentW++] = (wchar_t) FILE_SEPARATOR_CHAR;
						++nCurrentSlash;
						pSlashPoints[nCurrentSlash] = nCurrentW;
					}
				}
				nStart = nCurrent + 1;
				++nCurrent;
				continue;
			}
			pDataNorm[nCurrentW++] = pData[nCurrent];
			++nCurrent;
		}

		pDataNorm[nCurrentW] = (wchar_t)'\0';

		m_strFilename = std::wstring(pDataNorm, nCurrentW);

		delete []pSlashPoints;
		delete []pDataNorm;
	}


	bool CPath::FileInDirectoryCorrect()
	{
		std::wstring fileDirectory = GetDirectory(false);

		std::wstring lowerFileName = XmlUtils::GetLower(m_strFilename);

		std::vector<std::wstring> trueArray;

		NSDirectory::GetFiles2(fileDirectory, trueArray);

		for (size_t i = 0; i < trueArray.size(); i++)
		{
			std::wstring lowerTest = XmlUtils::GetLower(trueArray[i]);

			if (lowerTest == lowerFileName)
			{
				m_strFilename = trueArray[i];
				return true;
			}
		}
		return false;
	}

	CPath operator/(const CPath& path1, const CPath& path2)
	{
		CPath path(path1.m_strFilename + FILE_SEPARATOR_STR + path2.m_strFilename);
		path.Normalize();

		return path;
	}
	CPath operator/(const CPath& path1, const std::wstring& path2)
	{
		CPath path(path1.m_strFilename + FILE_SEPARATOR_STR + path2);
		path.Normalize();

		return path;
	}

	CPath operator+(const CPath& path1, const CPath& path2)
	{
		CPath path(path1.m_strFilename + path2.m_strFilename);
		path.Normalize();
		return path;
	}
	CPath operator+(const CPath& path1, const std::wstring& path2)
	{
		CPath path(path1.m_strFilename + path2);
		path.Normalize();
		return path;
	}
	CPath operator+(const std::wstring& path1, const CPath& path2)
	{
		CPath path(path1 + path2.m_strFilename);
		path.Normalize();
		return path;
	}

	std::wstring CPath::GetBasename() const
	{
		std::wstring sFilename = GetFilename();
		std::wstring sExt = GetExtention();
		return sFilename.substr(0, sFilename.length() - sExt.length());
	}
	std::wstring CPath::GetExtention(bool bIsPoint) const
	{
		int nFind = (int)m_strFilename.rfind('.');
		if (-1 == nFind)
			return L"";

		if (!bIsPoint)
			++nFind;

		return m_strFilename.substr(nFind);
	}
	std::wstring CPath::GetDirectory(bool bIsSlash) const
	{
		int nPos = (int)m_strFilename.rfind(FILE_SEPARATOR_CHAR);
		if (-1 == nPos)
		{
			return m_strFilename;
		}
		else
		{
			if (bIsSlash)
				++nPos;
			return m_strFilename.substr(0, nPos);
		}
	}
	std::wstring CPath::GetPath() const
	{
		return m_strFilename;
	}
	std::wstring CPath::GetFilename() const
	{
		int nPos = (int)m_strFilename.rfind(FILE_SEPARATOR_CHAR);
		if (-1 == nPos)
		{
			return m_strFilename;
		}
		else
		{
			int nLast = (int) m_strFilename.length();
			return m_strFilename.substr(nPos + 1, nLast);
		}
	}

	void CPath::CheckIsRoot()
	{
		if(m_strFilename.length() > 0 && ( '/' == m_strFilename[0] ))
			m_bIsRoot = true;
		else
			m_bIsRoot = false;
	}

	void CPath::SetName(std::wstring sName, bool bNormalize)
	{
		m_strFilename = sName;
		CheckIsRoot();
		if(bNormalize)
			Normalize();
	}

	bool CPath::GetIsRoot()
	{
		return m_bIsRoot;
	}
}

namespace OOX
{
	bool CSystemUtility::IsFileExist(const std::wstring& strFileName)
	{
		return NSFile::CFileBinary::Exists(strFileName);
	}
	bool CSystemUtility::IsFileExist(const CPath& oPath)
	{
		return IsFileExist(oPath.GetPath());
	}

	std::wstring CSystemUtility::GetDirectoryName(const std::wstring& strFileName)
	{
		CPath oPath(strFileName);
		return oPath.GetDirectory();
	}

	int CSystemUtility::GetFilesCount(const std::wstring& strDirPath, const bool& bRecursive)
	{
		return NSDirectory::GetFilesCount(strDirPath, bRecursive);
	}

	std::wstring CSystemUtility::GetFileExtention(const std::wstring& strFileName)
	{
		CPath oPath(strFileName);
		return oPath.GetExtention();
	}

	bool CSystemUtility::CreateDirectories(const CPath& oPath)
	{
		return NSDirectory::CreateDirectory(oPath.GetPath());
	}

	void CSystemUtility::ReplaceExtention(std::wstring& strName, std::wstring& str1, std::wstring& str2)
	{
		return;
	}
}
