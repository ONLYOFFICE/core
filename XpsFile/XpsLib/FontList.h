/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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

				// Need to replace the first 32 bytes of the file
				if (IFolder::iftFolder == pFolder->getType())
				{
					if (NSDirectory::Exists(wsFontPath))
					{
						IFolder::CBuffer* buffer = NULL;;
						pFolder->readFileWithChunks(wsFontPath, buffer);
						if (buffer)
							delete buffer;
					}

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
					pFolder->readFileWithChunks(wsFontPath, buffer);

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
		int GetFontId(const std::wstring& wsName)
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
		int GetIntegerFromHex(const std::wstring& wsString)
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
		std::vector<std::wstring> m_mList;
	};
}

#endif // _XPS_XPSLIB_FONTLIST_H
