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

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

#include "../../DesktopEditor/graphics/TemporaryCS.h"
#include "../../DesktopEditor/common/File.h"
#include "Utils.h"
#include "../../OfficeUtils/src/ZipFolder.h"
#include "../../DesktopEditor/graphics/pro/Fonts.h"

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
        void Check(const std::wstring& wsName, const std::wstring& wsFontPath, IFolder* pFolder)
		{
			m_oCS.Enter();
			if (!Find(wsName))
			{
				Add(wsName);

				unsigned char sKey[16];
				GetFontKey(wsName, sKey);

                // Нужно подменить первые 32 байта файла
                if (IFolder::iftFolder == pFolder->getType())
                {
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
                else if (IFolder::iftZip == pFolder->getType())
                {
                    IFolder::CBuffer* buffer = NULL;
                    pFolder->read(wsFontPath, buffer);

                    if (buffer->Size >= 32)
                    {
                        unsigned char* sFontData = buffer->Buffer;
                        for (int nIndex = 0; nIndex < 32; nIndex++)
                            sFontData[nIndex] ^= sKey[nIndex % 16];
                    }

                    if (NSFonts::NSApplicationFontStream::GetGlobalMemoryStorage())
                        NSFonts::NSApplicationFontStream::GetGlobalMemoryStorage()->Add(wsFontPath, buffer->Buffer, buffer->Size);

                    RELEASEOBJECT(buffer);
                }
			}
			m_oCS.Leave();
		}
		int  GetFontId(const std::wstring& wsName)
		{
			std::vector<std::wstring>::iterator oIter = std::find(m_mList.begin(), m_mList.end(), wsName);
			if (oIter != m_mList.end())
				return std::distance(m_mList.begin(), oIter);
			return -1;
		}
	private:

		bool Find(const std::wstring& wsName)
		{
			std::vector<std::wstring>::iterator oIter = std::find(m_mList.begin(), m_mList.end(), wsName);
			return oIter != m_mList.end();
		}
		void Add(const std::wstring& wsName)
		{
			m_mList.push_back(wsName);
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
		std::vector<std::wstring>         m_mList;
	};
}

#endif // _XPS_XPSLIB_FONTLIST_H
