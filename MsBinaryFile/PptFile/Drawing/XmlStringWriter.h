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
#include "../../../OOXML/SystemUtility/File.h"

namespace PPT
{
    static std::wstring	g_bstr_nodeopen				= L"<";
    static std::wstring	g_bstr_nodeclose			= L">";
    static std::wstring	g_bstr_nodeopen_slash		= L"</";
    static std::wstring	g_bstr_nodeclose_slash		= L"/>";
    static std::wstring	g_bstr_node_space			= L" ";
    static std::wstring	g_bstr_node_equal			= L"=";
    static std::wstring	g_bstr_node_quote			= L"\"";
    static std::wstring	g_bstr_boolean_true			= L"true";
    static std::wstring	g_bstr_boolean_false		= L"false";
	
	inline static double FABS(double dVal)
	{
		return (dVal >= 0) ? dVal : -dVal;
	}
	inline static int round(double dVal)
	{
		return (int)(dVal + 0.5);
	}

	class CStringWriter
	{
	private:
		wchar_t*	m_pData;
		size_t		m_lSize;

		wchar_t*	m_pDataCur;
		size_t		m_lSizeCur;

	public:
		CStringWriter()
		{
			m_pData = NULL;
			m_lSize = 0;

			m_pDataCur	= m_pData;
			m_lSizeCur	= m_lSize;
		}
		~CStringWriter()
		{
			RELEASEMEM(m_pData);
		}

        void AddSize(const size_t nSize)
		{
			if (NULL == m_pData)
			{
                m_lSize = (std::max)(nSize, (size_t) 1000);
				m_pData = (wchar_t*)malloc(m_lSize * sizeof(wchar_t));
				memset(m_pData, 0, m_lSize * sizeof(wchar_t));
				
				m_lSizeCur = 0;
				m_pDataCur = m_pData;
				return;
			}

			if ((m_lSizeCur + nSize) > m_lSize)
			{
				while ((m_lSizeCur + nSize) > m_lSize)
				{
					m_lSize *= 2;
				}

				wchar_t* pRealloc = (wchar_t*)realloc(m_pData, m_lSize * sizeof(wchar_t));
				if (NULL != pRealloc)
				{
					// реаллок сработал
			
					m_pData		= pRealloc;
					m_pDataCur	= m_pData + m_lSizeCur;
				}
				else
				{
					wchar_t* pMalloc = (wchar_t*)malloc(m_lSize * sizeof(wchar_t));
					memset(pMalloc, 0, m_lSize * sizeof(wchar_t));
					
					memcpy(pMalloc, m_pData, m_lSizeCur * sizeof(wchar_t));

					free(m_pData);
					m_pData		= pMalloc;
					m_pDataCur	= m_pData + m_lSizeCur;
				}
			}
		}

	public:
		
        void WriteString(const wchar_t* pString, size_t& nLen)
		{
			AddSize(nLen);
			// memcpy(m_pDataCur, pString, nLen * sizeof(wchar_t));
			// wstring has 4 bytes length (not 2 in WIN32) in linux/macos
			memcpy(m_pDataCur, pString, (2 == sizeof (wchar_t)) ? (nLen << 1) : (nLen << 2));
			
			m_pDataCur += nLen;
			m_lSizeCur += nLen;
		}
		
		void WriteStringXML(const std::wstring & wString)
		{
			std::wstring buffer;
			buffer.reserve(wString.size());
			for(size_t pos = 0; pos != wString.size(); ++pos)
			{
				switch(wString[pos])
				{
                                case '&':  buffer.append((L"&amp;"));      break;
                                case '\"': buffer.append((L"&quot;"));     break;
                                case '\'': buffer.append((L"&apos;"));     break;
                                case '<':  buffer.append((L"&lt;"));       break;
                                case '>':  buffer.append((L"&gt;"));       break;
				default:   buffer.append(&wString[pos], 1);	break;
				}
			}
			WriteString(buffer);
		}

        void WriteString(const std::wstring & wString)
		{
			size_t nLen = wString.length();
			WriteString(wString.c_str(), nLen);
		}

        // not works
        void WriteStringFront(const wchar_t* pString, size_t& nLen)
        {
            AddSize(nLen);
            /// shift data to back
            std::rotate(m_pData, m_pData + nLen+1, m_pDataCur + nLen);
            // memcpy(m_pDataCur, pString, nLen * sizeof(wchar_t));
            // wstring has 4 bytes length (not 2 in WIN32) in linux/macos
            memcpy(m_pData, pString, sizeof (wchar_t) * nLen);

            m_pDataCur += nLen;
            m_lSizeCur += nLen;
        }

        // not works
        void WriteStringFront(const std::wstring & wString)
        {
            size_t nLen = wString.length();
            WriteStringFront(wString.c_str(), nLen);
        }

        size_t GetCurSize()
		{
			return m_lSizeCur;
		}

		void Write(CStringWriter& oWriter)
		{
			WriteString(oWriter.m_pData, oWriter.m_lSizeCur);
		}

		inline void Clear()
		{
			RELEASEMEM(m_pData);
			
			m_pData = NULL;
			m_lSize = 0;

			m_pDataCur	= m_pData;
			m_lSizeCur	= 0;
		}
		inline void ClearNoAttack()
		{
			m_pDataCur	= m_pData;

			if (m_pData) m_pData[0] = 0;
			m_lSizeCur	= 0;
		}

                std::wstring GetData() const
		{
			return std::wstring(m_pData, (int)m_lSizeCur);
		}

		void CorrectUnicodeString()
		{
			BYTE m_mapUnicode[0x10000];
			memset(m_mapUnicode, 0, 0x10000);

			m_mapUnicode[0x0009] = 1;
			m_mapUnicode[0x000A] = 1;
			m_mapUnicode[0x000D] = 1;

			memset(m_mapUnicode + 0x0020, 1, 0xD7FF - 0x0020 + 1);
			memset(m_mapUnicode + 0xE000, 1, 0xFFFD - 0xE000 + 1);

			wchar_t* pMemory = m_pData;
			while (pMemory < m_pDataCur)
			{
				if (!m_mapUnicode[*pMemory])
					*pMemory = wchar_t(' ');
				++pMemory;
			}
		}
	};
}
