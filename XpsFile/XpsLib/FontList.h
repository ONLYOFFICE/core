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
#ifndef _XPS_XPSLIB_FONTLIST_H
#define _XPS_XPSLIB_FONTLIST_H

#include <map>
#include <string>
#include <sstream>

#include "../../DesktopEditor/graphics/TemporaryCS.h"
#include "../../DesktopEditor/common/File.h"
#include "Utils.h"

namespace XPS
{
	class CFontList
	{
	public:

		CFontList()
		{
			m_oCS.InitializeCriticalSection();
		}
		~CFontList()
		{
			m_oCS.DeleteCriticalSection();
		}
		void Clear()
		{
			m_mList.clear();
		}
		void Check(const std::wstring& wsName, const std::wstring& wsFontPath)
		{
			m_oCS.Enter();
			if (!Find(wsName))
			{
				Add(wsName);

				unsigned char sKey[16];
				GetFontKey(wsName, sKey);

				NSFile::CFileBinary oFile;
				oFile.OpenFile(wsFontPath, true);

				unsigned char sFontData[32];
				DWORD dwBytesRead;
				oFile.ReadFile(sFontData, 32, dwBytesRead);

				for (int nIndex = 0; nIndex < 32; nIndex++)
					sFontData[nIndex] ^= sKey[nIndex % 16];

				FILE* pFile = oFile.GetFileNative();
				if (pFile)
				{
					fseek(pFile, 0, SEEK_SET);
					fwrite(sFontData, 1, 32, pFile);
				}

				oFile.CloseFile();
			}
			m_oCS.Leave();
		}
	private:

		bool Find(const std::wstring& wsName)
		{
			std::map<std::wstring, bool>::iterator oIter = m_mList.find(wsName);
			if (oIter != m_mList.end())
				return oIter->second;

			return false;
		}
		void Add(const std::wstring& wsName)
		{
			m_mList.insert(std::pair<std::wstring, bool>(wsName, true));
		}
		void GetFontKey(const std::wstring& wsName, unsigned char* sKey)
		{
			int k = 0;
			for (int i = wsName.length() - 1; i >= 0; i--)
			{
				if ('-' != wsName[i])
				{
					sKey[k] = (unsigned char)GetIntegerFromHex(wsName.substr(i - 1, 2));
					i--;
					k++;
				}
			}
		}
		int  GetIntegerFromHex(const std::wstring& wsString)
		{
			if (0 == wsString.size())
				return 0;

			std::wistringstream wiss(wsString);

			int nValue = 0;
			wiss >> std::hex >> nValue;
			return nValue;
		}

	private:

		NSCriticalSection::CRITICAL_SECTION m_oCS;
		std::map<std::wstring, bool>        m_mList;
	};
}

#endif // _XPS_XPSLIB_FONTLIST_H
