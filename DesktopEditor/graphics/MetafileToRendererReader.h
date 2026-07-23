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
#ifndef _BUILD_METAFILE_TO_READER_H_
#define _BUILD_METAFILE_TO_READER_H_

#pragma once
#include <string>
#include "./IRenderer.h"

#if !defined(_WIN32) && !defined(_WIN64)
#include "../common/StringExt.h"
#endif

class IMetafileToRenderter;

namespace NSOnlineOfficeBinToPdf
{
	class CBufferReader
	{
	private:
		unsigned char* m_buffer;
		unsigned char* m_buffer_end;
		unsigned char* m_cur;
		int m_len;

	public:
		CBufferReader(unsigned char* data, int len)
		{
			m_buffer = data;
			m_cur = m_buffer;
			m_len = len;
			m_buffer_end = m_cur + len;
		}

	public:
		inline bool Check()
		{
			return (m_cur < m_buffer_end) ? true : false;
		}
		inline unsigned char* GetCurrentBuffer()
		{
			return m_cur;
		}
		inline void SetCurrentBuffer(unsigned char* cur)
		{
			m_cur = cur;
		}

	public:
		inline unsigned char ReadByte()
		{
			return *m_cur++;
		}
		inline bool ReadBool()
		{
			return (1 == ReadByte()) ? true : false;
		}
		inline unsigned short ReadUShort()
		{
#ifdef _ARM_ALIGN_
			unsigned short ret = 0;
			memcpy(&ret, m_cur, sizeof(unsigned short));
#else
			unsigned short ret = *((unsigned short*)m_cur);
#endif
			m_cur += 2;
			return ret;
		}
		inline int ReadInt()
		{
#ifdef _ARM_ALIGN_
			int ret = 0;
			memcpy(&ret, m_cur, sizeof(int));
#else
			int ret = *((int*)m_cur);
#endif
			m_cur += 4;
			return ret;
		}
		inline double ReadDouble()
		{
			return ReadInt() / 100000.0;
		}

		inline void Skip(int count)
		{
			m_cur += count;
		}
		inline void SkipUShort(int count = 1)
		{
			m_cur += (count << 1);
		}
		inline void SkipInt(int count = 1)
		{
			m_cur += (count << 2);
		}
		inline void SkipDouble(int count = 1)
		{
			m_cur += (count << 2);
		}

		inline std::wstring ReadString16(int nLen)
		{
			std::wstring wsTempString;
#ifdef _ARM_ALIGN_

			if (sizeof(wchar_t) == 4)
			{
#if !defined(_WIN32) && !defined(_WIN64)
				int len = nLen / 2;
				unsigned short* buf = new unsigned short[len];
				memcpy(buf, m_cur, nLen);
				wsTempString = NSStringExt::CConverter::GetUnicodeFromUTF16((unsigned short*)buf, len);
				RELEASEARRAYOBJECTS(buf);
#endif
			}
			else
			{
				int len = nLen / 2;
				wchar_t* buf = new wchar_t[len];
				memcpy(buf, m_cur, nLen);
				wsTempString = std::wstring(buf, len);
				RELEASEARRAYOBJECTS(buf);
			}

#else

			if (sizeof(wchar_t) == 4)
			{
#if !defined(_WIN32) && !defined(_WIN64)
				wsTempString = NSStringExt::CConverter::GetUnicodeFromUTF16((unsigned short*)m_cur, nLen / 2);
#endif
			}
			else
			{
				wsTempString = std::wstring((wchar_t*)m_cur, nLen / 2);
			}

#endif
			m_cur += nLen;
			return wsTempString;
		}
		inline std::wstring ReadString()
		{
			int len = 2 * ReadUShort();
			return ReadString16(len);
		}
		inline std::string ReadStringA()
		{
			int len = ReadInt();
			std::string sRes = std::string((char*)m_cur, len);
			m_cur += len;
			return sRes;
		}
		inline void SkipString16(int len)
		{
			Skip(len);
		}
		inline void SkipString()
		{
			int len = 2 * ReadUShort();
			SkipString16(len);
		}
		inline int Tell()
		{
			return (int)(m_cur - m_buffer);
		}

		IAdvancedCommand* Read(int type, IMetafileToRenderter* pCorrector, int nLen = 0);
	};
}

#endif // _BUILD_METAFILE_TO_READER_H_
