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
#include "StringBuilder.h"
#include "File.h"

namespace NSStringUtils
{
	const wchar_t g_hex_values[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

	CStringBuilderA::CStringBuilderA()
	{
		m_pData = NULL;
		m_lSize = 0;

		m_pDataCur = m_pData;
		m_lSizeCur = m_lSize;
	}
	CStringBuilderA::~CStringBuilderA()
	{
		if (NULL != m_pData)
			free(m_pData);
		m_pData = NULL;
	}

	void CStringBuilderA::AddSize(size_t nSize)
	{
		if (NULL == m_pData)
		{
			m_lSize = (std::max)((int)nSize, 1000);
			m_pData = (char*)malloc(m_lSize * sizeof(char));

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

			char* pRealloc = (char*)realloc(m_pData, m_lSize * sizeof(char));
			if (NULL != pRealloc)
			{
				// реаллок сработал
				m_pData = pRealloc;
				m_pDataCur = m_pData + m_lSizeCur;
			}
			else
			{
				char* pMalloc = (char*)malloc(m_lSize * sizeof(char));
				memcpy(pMalloc, m_pData, m_lSizeCur * sizeof(char));

				free(m_pData);
				m_pData = pMalloc;
				m_pDataCur = m_pData + m_lSizeCur;
			}
		}
	}

	void CStringBuilderA::SetText(const std::string& sText)
	{
		ClearNoAttack();
		WriteString(sText);
	}
	void CStringBuilderA::WriteString(const std::string& str)
	{
		WriteString(str.c_str(), str.length());
	}

	void CStringBuilderA::WriteStringNoSafe(const char* pString, size_t nLen)
	{
		memcpy(m_pDataCur, pString, nLen * sizeof(char));

		m_pDataCur += nLen;
		m_lSizeCur += nLen;
	}
	void CStringBuilderA::WriteString(const char* pString, size_t nLen)
	{
		AddSize(nLen);
		WriteStringNoSafe(pString, nLen);
	}

	void CStringBuilderA::AddCharNoSafe(const char& _c)
	{
		*m_pDataCur++ = _c;
		++m_lSizeCur;
	}
	void CStringBuilderA::AddCharSafe(const char& _c)
	{
		AddSize(1);
		*m_pDataCur++ = _c;
		++m_lSizeCur;
	}

	size_t CStringBuilderA::GetCurSize()
	{
		return m_lSizeCur;
	}
	void CStringBuilderA::SetCurSize(size_t lCurSize)
	{
		m_lSizeCur = lCurSize;
		m_pDataCur = m_pData + m_lSizeCur;
	}
	size_t CStringBuilderA::GetSize()
	{
		return m_lSize;
	}

	void CStringBuilderA::Clear()
	{
		if (NULL != m_pData)
			free(m_pData);
		m_pData = NULL;

		m_pData = NULL;
		m_lSize = 0;

		m_pDataCur = m_pData;
		m_lSizeCur = 0;
	}
	void CStringBuilderA::ClearNoAttack()
	{
		m_pDataCur = m_pData;
		m_lSizeCur = 0;
	}

	std::string CStringBuilderA::GetData()
	{
		std::string str(m_pData, (int)m_lSizeCur);
		return str;
	}
	char* CStringBuilderA::GetBuffer()
	{
		return m_pData;
	}
}

namespace NSStringUtils
{
	CStringBuilder::CStringBuilder()
	{
		m_pData = NULL;
		m_lSize = 0;

		m_pDataCur = m_pData;
		m_lSizeCur = m_lSize;
	}
	CStringBuilder::~CStringBuilder()
	{
		if (NULL != m_pData)
			free(m_pData);
		m_pData = NULL;
	}

	void CStringBuilder::AddSize(size_t nSize)
	{
		if (NULL == m_pData)
		{
			m_lSize = (std::max)((int)nSize, 1000);
			m_pData = (wchar_t*)malloc(m_lSize * sizeof(wchar_t));

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
				m_pData = pRealloc;
				m_pDataCur = m_pData + m_lSizeCur;
			}
			else
			{
				wchar_t* pMalloc = (wchar_t*)malloc(m_lSize * sizeof(wchar_t));
				memcpy(pMalloc, m_pData, m_lSizeCur * sizeof(wchar_t));

				free(m_pData);
				m_pData = pMalloc;
				m_pDataCur = m_pData + m_lSizeCur;
			}
		}
	}

	void CStringBuilder::SetText(const std::wstring& bsText)
	{
		ClearNoAttack();
		WriteString(bsText);

					for (size_t i = 0; i < m_lSizeCur; ++i)
		{
			if (WCHAR(8233) == m_pData[i])
				m_pData[i] = WCHAR(' ');
		}
	}

    void CStringBuilder::operator+=(const std::wstring& oTemp)
    {
        WriteString(oTemp.c_str(), oTemp.length());
    }

	void CStringBuilder::WriteStringNoSafe(const wchar_t* pString, size_t nLen)
	{
		memcpy(m_pDataCur, pString, nLen * sizeof(wchar_t));

		m_pDataCur += nLen;
		m_lSizeCur += nLen;
	}
	void CStringBuilder::WriteString(const wchar_t* pString, size_t nLen)
	{
		if (-1 == nLen)
		{
			nLen = wcslen(pString);
		}
		AddSize(nLen);
		WriteStringNoSafe(pString, nLen);
	}

	void CStringBuilder::WriteString(const std::wstring& sString)
	{
		this->WriteString(sString.c_str(), sString.length());
	}

	void CStringBuilder::AddCharNoSafe(const wchar_t& _c)
	{
		*m_pDataCur++ = _c;
		++m_lSizeCur;
	}
	void CStringBuilder::AddCharNoCheck(const wchar_t& _c)
	{
		*m_pDataCur++ = _c;
		++m_lSizeCur;
	}
	void CStringBuilder::AddSpaceNoCheck()
	{
		*m_pDataCur++ = ' ';
		++m_lSizeCur;
	}
	void CStringBuilder::AddCharSafe(const wchar_t& _c)
	{
		AddSize(1);
		*m_pDataCur++ = _c;
		++m_lSizeCur;
	}
	void CStringBuilder::AddChar2Safe(const wchar_t _c1, const wchar_t& _c2)
	{
		AddSize(2);
		*m_pDataCur++ = _c1;
		*m_pDataCur++ = _c2;
		m_lSizeCur += 2;
	}

	void CStringBuilder::WriteEncodeXmlString(const std::wstring& sString)
	{
		WriteEncodeXmlString(sString.c_str(), (int)sString.length());
	}

	void CStringBuilder::WriteEncodeXmlString(const wchar_t* pString, int nCount)
	{
		if (sizeof(wchar_t) == 2)
			WriteEncodeXmlString_2bytes(pString, nCount);
		else
			WriteEncodeXmlString_4bytes(pString, nCount);
	}
	void CStringBuilder::WriteEncodeXmlStringHHHH(const std::wstring& sString)
	{
		WriteEncodeXmlStringHHHH(sString.c_str(), (int)sString.length());
	}

	void CStringBuilder::WriteEncodeXmlStringHHHH(const wchar_t* pString, int nCount)
	{
		if (sizeof(wchar_t) == 2)
			WriteEncodeXmlStringHHHH_2bytes(pString, nCount);
		else
			WriteEncodeXmlStringHHHH_4bytes(pString, nCount);
	}
	inline void CStringBuilder::WriteEncodeXmlString_4bytes(const wchar_t* pString, int nCount)
	{
		const wchar_t* pData = pString;
		int nCounter = 0;
		unsigned int code;
		while (*pData != 0)
		{
			code = (unsigned int)*pData;
			WriteEncodeXmlChar(*pData, CheckXmlCode(code));

			++pData;
			if (-1 != nCount)
			{
				++nCounter;
				if (nCounter >= nCount)
					break;
			}
		}
	}
	inline void CStringBuilder::WriteEncodeXmlString_2bytes(const wchar_t* pString, int nCount)
	{
		const wchar_t* pData = pString;
		int nCounter = 0;
		unsigned int code;
		BYTE type;
		while (*pData != 0)
		{
			code = (unsigned int)*pData;
			if (code >= 0xD800 && code <= 0xDFFF && *(pData + 1) != 0)
			{
				code = 0x10000 + (((code & 0x3FF) << 10) | (0x03FF & *(pData + 1)));
				type = CheckXmlCode(code);
				if(0 != type)
				{
					WriteEncodeXmlChar(*pData, type);
					++pData;
				}
			}
			else
			{
				type = CheckXmlCode(code);
			}
			WriteEncodeXmlChar(*pData, type);

			++pData;
			if (-1 != nCount)
			{
				++nCounter;
				if (nCounter >= nCount)
					break;
			}
		}
	}
	inline void CStringBuilder::WriteEncodeXmlStringHHHH_4bytes(const wchar_t* pString, int nCount)
	{
		const wchar_t* pData = pString;
		int nCounter = 0;
		unsigned int code;
		while (*pData != 0)
		{
			code = (unsigned int)*pData;
			WriteEncodeXmlChar(*pData, CheckXmlCodeHHHH(code, pData));

			++pData;
			if (-1 != nCount)
			{
				++nCounter;
				if (nCounter >= nCount)
					break;
			}
		}
	}
	inline void CStringBuilder::WriteEncodeXmlStringHHHH_2bytes(const wchar_t* pString, int nCount)
	{
		const wchar_t* pData = pString;
		int nCounter = 0;
		unsigned int code;
		BYTE type;
		while (*pData != 0)
		{
			code = (unsigned int)*pData;
			if (code >= 0xD800 && code <= 0xDFFF && *(pData + 1) != 0)
			{
				code = 0x10000 + (((code & 0x3FF) << 10) | (0x03FF & *(pData + 1)));
				type = CheckXmlCodeHHHH(code, pData);
				if(0 != type)
				{
					WriteEncodeXmlChar(*pData, type);
					++pData;
				}
			}
			else
			{
				type = CheckXmlCodeHHHH(code, pData);
			}
			WriteEncodeXmlChar(*pData, type);

			++pData;
			if (-1 != nCount)
			{
				++nCounter;
				if (nCounter >= nCount)
					break;
			}
		}
	}
	inline void CStringBuilder::WriteEncodeXmlChar(wchar_t code, BYTE type)
	{
		switch (type)
		{
		case 1:
			AddCharSafe(code);
			break;
		case 0:
			AddCharSafe((wchar_t)' ');
			break;
		case 2:
			AddSize(5);
			*m_pDataCur++ = (wchar_t)('&');
			*m_pDataCur++ = (wchar_t)('a');
			*m_pDataCur++ = (wchar_t)('m');
			*m_pDataCur++ = (wchar_t)('p');
			*m_pDataCur++ = (wchar_t)(';');
			m_lSizeCur += 5;
			break;
		case 3:
			AddSize(6);
			*m_pDataCur++ = (wchar_t)('&');
			*m_pDataCur++ = (wchar_t)('a');
			*m_pDataCur++ = (wchar_t)('p');
			*m_pDataCur++ = (wchar_t)('o');
			*m_pDataCur++ = (wchar_t)('s');
			*m_pDataCur++ = (wchar_t)(';');
			m_lSizeCur += 6;
			break;
		case 4:
			AddSize(4);
			*m_pDataCur++ = (wchar_t)('&');
			*m_pDataCur++ = (wchar_t)('l');
			*m_pDataCur++ = (wchar_t)('t');
			*m_pDataCur++ = (wchar_t)(';');
			m_lSizeCur += 4;
			break;
		case 5:
			AddSize(4);
			*m_pDataCur++ = (wchar_t)('&');
			*m_pDataCur++ = (wchar_t)('g');
			*m_pDataCur++ = (wchar_t)('t');
			*m_pDataCur++ = (wchar_t)(';');
			m_lSizeCur += 4;
			break;
		case 6:
			AddSize(6);
			*m_pDataCur++ = (wchar_t)('&');
			*m_pDataCur++ = (wchar_t)('q');
			*m_pDataCur++ = (wchar_t)('u');
			*m_pDataCur++ = (wchar_t)('o');
			*m_pDataCur++ = (wchar_t)('t');
			*m_pDataCur++ = (wchar_t)(';');
			m_lSizeCur += 6;
			break;
		case 7:
			AddSize(5);
			*m_pDataCur++ = (wchar_t)('&');
			*m_pDataCur++ = (wchar_t)('#');
			*m_pDataCur++ = (wchar_t)('x');
			*m_pDataCur++ = (wchar_t)('A');
			*m_pDataCur++ = (wchar_t)(';');
			m_lSizeCur += 5;
			break;
		case 8:
			AddSize(5);
			*m_pDataCur++ = (wchar_t)('&');
			*m_pDataCur++ = (wchar_t)('#');
			*m_pDataCur++ = (wchar_t)('x');
			*m_pDataCur++ = (wchar_t)('D');
			*m_pDataCur++ = (wchar_t)(';');
			m_lSizeCur += 5;
			break;
		case 9:
			AddSize(5);
			*m_pDataCur++ = (wchar_t)('&');
			*m_pDataCur++ = (wchar_t)('#');
			*m_pDataCur++ = (wchar_t)('x');
			*m_pDataCur++ = (wchar_t)('9');
			*m_pDataCur++ = (wchar_t)(';');
			m_lSizeCur += 5;
			break;
		case 10:
			AddSize(7);
			*m_pDataCur++ = (wchar_t)('_');
			*m_pDataCur++ = (wchar_t)('x');
			*m_pDataCur++ = (wchar_t)('0');
			*m_pDataCur++ = (wchar_t)('0');
			*m_pDataCur++ = (wchar_t)('5');
			*m_pDataCur++ = (wchar_t)('F');
			*m_pDataCur++ = (wchar_t)('_');
			m_lSizeCur += 7;
			break;
		case 11:
			AddSize(7);
			*m_pDataCur++ = (wchar_t)('_');
			*m_pDataCur++ = (wchar_t)('x');
			m_lSizeCur += 2;
			WriteHexByteNoSafe((code >> 8) & 0xFF);
			WriteHexByteNoSafe(code & 0xFF);
			*m_pDataCur++ = (wchar_t)('_');
			++m_lSizeCur;
			break;
		default:
			break;
		}
	}

	size_t CStringBuilder::GetCurSize()
	{
		return m_lSizeCur;
	}
	void CStringBuilder::SetCurSize(size_t lCurSize)
	{
		m_lSizeCur = lCurSize;
		m_pDataCur = m_pData + m_lSizeCur;
	}
	size_t CStringBuilder::GetSize()
	{
		return m_lSize;
	}

	void CStringBuilder::Write(CStringBuilder& oWriter, const size_t& offset)
	{
		WriteString(oWriter.m_pData + offset, oWriter.m_lSizeCur - offset);
	}

	void CStringBuilder::Clear()
	{
		if (NULL != m_pData)
			free(m_pData);
		m_pData = NULL;

		m_pData = NULL;
		m_lSize = 0;

		m_pDataCur = m_pData;
		m_lSizeCur = 0;
	}
	void CStringBuilder::ClearNoAttack()
	{
		m_pDataCur = m_pData;
		m_lSizeCur = 0;
	}

	std::wstring CStringBuilder::GetData()
	{
		std::wstring str(m_pData, (int)m_lSizeCur);
		return str;
	}
	wchar_t* CStringBuilder::GetBuffer()
	{
		return m_pData;
	}

	void CStringBuilder::RemoveLastSpaces()
	{
		wchar_t* pMemory = m_pDataCur - 1;
		while ((pMemory > m_pData) && (wchar_t(' ') == *pMemory))
		{
			--pMemory;
			--m_lSizeCur;
			--m_pDataCur;
		}

	}
	bool CStringBuilder::IsSpace()
	{
		if (1 != m_lSizeCur)
			return false;
		return (wchar_t(' ') == *m_pData);
	}

	void CStringBuilder::AddInt(int val)
	{
		AddSize(10);
		AddIntNoCheck(val);
	}
	void CStringBuilder::AddIntDel10(int val)
	{
		AddSize(11);
		AddIntNoCheckDel10(val);
	}
	void CStringBuilder::AddIntDel100(int val)
	{
		AddSize(11);
		AddIntNoCheckDel100(val);
	}
	void CStringBuilder::AddInt64(__int64 val)
	{
		//todo AddIntNoCheck
		WriteString(std::to_wstring(val));
	}

	void CStringBuilder::AddIntNoCheck(int val)
	{
		if (0 == val)
		{
							*m_pDataCur++ = (wchar_t)'0';
			++m_lSizeCur;
			return;
		}
		if (val < 0)
		{
			val = -val;
							*m_pDataCur++ = (wchar_t)'-';
			++m_lSizeCur;
		}

		int len = 0;
		int oval = val;
		while (oval > 0)
		{
			oval /= 10;
			++len;
		}

		oval = 1;
		while (val > 0)
		{
							m_pDataCur[len - oval] = (wchar_t)('0' + (val % 10));
			++oval;
			val /= 10;
		}

		m_pDataCur += len;
		m_lSizeCur += len;
	}

	void CStringBuilder::AddIntNoCheckDel10(int val)
	{
		if (0 == val)
		{
							*m_pDataCur++ = (wchar_t)'0';
			++m_lSizeCur;
			return;
		}
		if (val < 0)
		{
			val = -val;
							*m_pDataCur++ = (wchar_t)'-';
			++m_lSizeCur;
		}

		int len = 0;
		int oval = val;
		while (oval > 0)
		{
			oval /= 10;
			++len;
		}

		oval = 1;
		int nLastS = (val % 10);
		if (0 != nLastS)
		{
			++len;
							m_pDataCur[len - oval] = (wchar_t)('0' + nLastS);
			++oval;
							m_pDataCur[len - oval] = (wchar_t)('.');
			++oval;
			val /= 10;
		}
		else
		{
			--len;
			val /= 10;
		}

		while (val > 0)
		{
							m_pDataCur[len - oval] = (wchar_t)('0' + (val % 10));
			++oval;
			val /= 10;
		}

		m_pDataCur += len;
		m_lSizeCur += len;
	}
	void CStringBuilder::AddIntNoCheckDel100(int val)
	{
		if (0 == val)
		{
			*m_pDataCur++ = (wchar_t)'0';
			++m_lSizeCur;
			return;
		}
		if (val < 0)
		{
			val = -val;
			*m_pDataCur++ = (wchar_t)'-';
			++m_lSizeCur;
		}

		int len = 0;
		int oval = val;
		while (oval > 0)
		{
			oval /= 10;
			++len;
		}

		oval = 1;
		int nLastS = (val % 10);
		if (0 != nLastS)
		{
			++len;
			m_pDataCur[len - oval] = (wchar_t)('0' + nLastS);
			++oval;
			m_pDataCur[len - oval] = (wchar_t)('.');
			++oval;
			val /= 10;
		}
		else
		{
			--len;
			val /= 10;
		}

		while (val > 0)
		{
			m_pDataCur[len - oval] = (wchar_t)('0' + (val % 10));
			++oval;
			val /= 10;
		}

		m_pDataCur += len;
		m_lSizeCur += len;
	}
	void CStringBuilder::AddDouble(double val, int count)
	{
		std::wstring s = std::to_wstring(val);

		if (count != -1)
		{
			size_t nSize = s.length();
			std::wstring::size_type pos1 = s.find(wchar_t('.'));
			if (pos1 != std::wstring::npos)
			{
				size_t nCountNeed = pos1 + 1 + count;
				if (nCountNeed < nSize)
					s = s.substr(0, nCountNeed);
			}
			std::wstring::size_type pos2 = s.find(wchar_t(','));
			if (pos2 != std::wstring::npos)
			{
				size_t nCountNeed = pos2 + 1 + count;
				if (nCountNeed < nSize)
					s = s.substr(0, nCountNeed);
			}
		}

		WriteString(s);
	}

	void CStringBuilder::WriteHexByteNoSafe(const unsigned char& value)
	{
		*m_pDataCur++ = g_hex_values[(value >> 4) & 0x0F];
		++m_lSizeCur;
		*m_pDataCur++ = g_hex_values[value & 0x0F];
		++m_lSizeCur;
	}
	void CStringBuilder::WriteHexByte(const unsigned char& value)
	{
		AddSize(2);
		WriteHexByteNoSafe(value);
	}
	void CStringBuilder::WriteHexInt3(const unsigned int& value)
	{
		AddSize(6);
		WriteHexByteNoSafe((value >> 16) & 0xFF);
		WriteHexByteNoSafe((value >> 8) & 0xFF);
		WriteHexByteNoSafe(value & 0xFF);
	}
	void CStringBuilder::WriteHexColor3(const unsigned char& r, const unsigned char& g, const unsigned char& b)
	{
		AddSize(7);
		*m_pDataCur++ = (wchar_t)'#';
		++m_lSizeCur;
		WriteHexByteNoSafe(r);
		WriteHexByteNoSafe(g);
		WriteHexByteNoSafe(b);
	}
	void CStringBuilder::WriteHexColor3(const unsigned int& value)
	{
		AddSize(7);
		*m_pDataCur++ = (wchar_t)'#';
		++m_lSizeCur;
					WriteHexByteNoSafe(value & 0xFF);
					WriteHexByteNoSafe((value >> 8) & 0xFF);
					WriteHexByteNoSafe((value >> 16) & 0xFF);
	}

    void CStringBuilder::Skip(int nSkip)
    {
        m_pDataCur += nSkip;
        m_lSizeCur += nSkip;
    }

	unsigned char CStringBuilder::CheckXmlCode(unsigned int c)
	{
		if ('&' == c)
			return 2;
		if ('\'' == c)
			return 3;
		if ('<' == c)
			return 4;
		if ('>' == c)
			return 5;
		if ('\"' == c)
			return 6;
		if ('\n' == c)//when reading from the attributes is replaced by a space.
			return 7;
		if ('\r' == c)//when reading from the attributes is replaced by a space.
			return 8;
		if ('\t' == c)//when reading from the attributes is replaced by a space.
			return 9;

		//xml 1.0 Character Range https://www.w3.org/TR/xml/#charsets
		if ((0x20 <= c && c <= 0xD7FF) || (0xE000 <= c && c <= 0xFFFD) || (0x10000 <= c && c <= 0x10FFFF))
			return 1;

		return 0;
	}
	unsigned char CStringBuilder::CheckXmlCodeHHHH(unsigned int c, const wchar_t* pData)
	{
		if ('&' == c)
			return 2;
		if ('\'' == c)
			return 3;
		if ('<' == c)
			return 4;
		if ('>' == c)
			return 5;
		if ('\"' == c)
			return 6;
		if ('\n' == c)//when reading from the attributes is replaced by a space.
			return 7;
		if ('\r' == c)//when reading from the attributes is replaced by a space.
			return 8;
		if ('\t' == c)//when reading from the attributes is replaced by a space.
			return 9;
		if (NSFile::CUtf8Converter::CheckHHHHChar(pData) >= 0)
			return 10;

		//xml 1.0 Character Range https://www.w3.org/TR/xml/#charsets
		if ((0x20 <= c && c <= 0xD7FF) || (0xE000 <= c && c <= 0xFFFD) || (0x10000 <= c && c <= 0x10FFFF))
			return 1;
		else if(c <= 0xFFFF)
			return 11;

		return 0;
	}

    void string_replace(std::wstring& text, const std::wstring& replaceFrom, const std::wstring& replaceTo)
    {
        size_t posn = 0;
        while (std::wstring::npos != (posn = text.find(replaceFrom, posn)))
        {
            text.replace(posn, replaceFrom.length(), replaceTo);
            posn += replaceTo.length();
        }
    }
    void string_replaceA(std::string& text, const std::string& replaceFrom, const std::string& replaceTo)
    {
        size_t posn = 0;
        while (std::string::npos != (posn = text.find(replaceFrom, posn)))
        {
            text.replace(posn, replaceFrom.length(), replaceTo);
            posn += replaceTo.length();
        }
    }
}
