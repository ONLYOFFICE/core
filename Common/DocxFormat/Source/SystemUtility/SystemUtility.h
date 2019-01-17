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
#pragma once

#include "../Base/Base.h"

namespace OOX
{
	class CPath
	{
	public:
		std::wstring	m_strFilename;
		//флаг введен, чтобы отличать относительные и абсолютные пути в rels
		bool m_bIsRoot;

	public:
		CPath();
		CPath(const std::wstring&	sName, bool bIsNorm = true);
        CPath(const char*&			sName, bool bIsNorm = true);
        CPath(const wchar_t*&		sName, bool bIsNorm = true);

		CPath(const CPath& oSrc);

		CPath& operator=(const	CPath&			oSrc);
		CPath& operator=(const	std::wstring&	oSrc);
        CPath& operator=(const  char*			oSrc);
        CPath& operator=(const  wchar_t*		oSrc);

		friend CPath operator/(const CPath& path1, const CPath& path2)
		{
			CPath path(path1.m_strFilename + FILE_SEPARATOR_STR + path2.m_strFilename);
			path.Normalize();

			return path;
		}
		friend CPath operator/(const CPath& path1, const std::wstring& path2)
		{
            CPath path(path1.m_strFilename + FILE_SEPARATOR_STR + path2);
			path.Normalize();

			return path;
		}

		friend CPath operator+(const CPath& path1, const CPath& path2)
		{
			CPath path(path1.m_strFilename + path2.m_strFilename);
			path.Normalize();
			return path;
		}
		friend CPath operator+(const CPath& path1, const std::wstring& path2)
		{
			CPath path(path1.m_strFilename + path2);
			path.Normalize();
			return path;
		}
		friend CPath operator+(const std::wstring& path1, const CPath& path2)
		{
			CPath path(path1 + path2.m_strFilename);
			path.Normalize();
			return path;
		}

		bool FileInDirectoryCorrect();

		AVSINLINE std::wstring GetBasename() const
		{
			std::wstring sFilename = GetFilename();
			std::wstring sExt = GetExtention();
			return sFilename.substr(0, sFilename.length() - sExt.length());
		}
        AVSINLINE std::wstring GetExtention(bool bIsPoint = true) const
        {
            int nFind = (int)m_strFilename.rfind('.');
            if (-1 == nFind)
                return L"";

            if (!bIsPoint)
                ++nFind;

            return m_strFilename.substr(nFind);
        }
        AVSINLINE std::wstring GetDirectory(bool bIsSlash = true) const
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
        AVSINLINE std::wstring GetPath() const
        {
            return m_strFilename;
        }
        AVSINLINE std::wstring GetFilename() const
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

        void Normalize();

		void CheckIsRoot()
		{
			if(m_strFilename.length() > 0 && ( '/' == m_strFilename[0] ))
				m_bIsRoot = true;
			else
				m_bIsRoot = false;
		}

		void SetName(std::wstring sName, bool bNormalize)
		{
			m_strFilename = sName;
			CheckIsRoot();
			if(bNormalize)
				Normalize();
		}

		bool GetIsRoot()
		{
			return m_bIsRoot;
		}
	};

	class CSystemUtility
	{
	public:
        static bool			CreateFile			(const	std::wstring&	strFileName);
		static bool			IsFileExist			(const	std::wstring&	strFileName);
		static bool			IsFileExist			(const	CPath&			sPath);
		static std::wstring GetDirectoryName	(const	std::wstring&	strFileName);
        static int			GetFilesCount		(const	std::wstring&	strDirPath, const bool& bRecursive = false);
		static std::wstring GetFileExtention	(const	std::wstring&	strFileName);
		static bool			CreateDirectories	(const	CPath&			oPath);
		static void			ReplaceExtention	(		std::wstring&	strName, std::wstring& str1, std::wstring& str2);
	};
}
