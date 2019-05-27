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
#include "WString.h"
#include "Utils.h"

#include "../../DesktopEditor/common/StringExt.h"
#include "../../DesktopEditor/common/Types.h"

#define MAX_STRING_LEN 2147483648 

namespace XPS
{
	class CWStringBuffer
	{
	public:

		CWStringBuffer(const wchar_t* wsString, unsigned int unLen)
		{
			if (unLen)
			{
				m_pBuffer = new wchar_t[unLen + 1];
				m_pBuffer[unLen] = 0x00;
				memcpy(m_pBuffer, wsString, sizeof(wchar_t) * unLen);
			}
			else
			{
				m_pBuffer = NULL;
			}

			m_nRefCount = 1;
		}
		void AddRef()
		{
			m_nRefCount++;
		}
		int  Release()
		{
			return --m_nRefCount;
		}
		void Free()
		{
			RELEASEARRAYOBJECTS(m_pBuffer);
		}
		wchar_t operator[](const unsigned int& unIndex) const
		{
			return m_pBuffer[unIndex];
		}

	private:

		wchar_t* m_pBuffer;
		int      m_nRefCount;

		friend class CWString;
	};
	CWString::CWString()
	{
		m_bOwnBuffer = false;
		m_pBuffer    = NULL;
		m_unLen      = 0;
	}
	CWString::CWString(const wchar_t* wsString)
	{
		m_bOwnBuffer = false;
		m_pBuffer    = NULL;
		m_unLen      = 0;
		create(wsString, false);
	}
    CWString::CWString(const std::wstring& wsString)
    {
        m_bOwnBuffer = false;
        m_pBuffer    = NULL;
        m_unLen      = 0;
        create(wsString, true);
    }
	CWString::CWString(wchar_t* wsString, bool bCopy, int nLen)
	{
		m_bOwnBuffer = false;
		m_pBuffer    = NULL;
		m_unLen      = 0;
		create(wsString, bCopy, nLen);
	}
	CWString::CWString(const CWString& wsString)
	{
		m_unLen      = wsString.m_unLen;
		m_bOwnBuffer = wsString.m_bOwnBuffer;
		m_pBuffer    = wsString.m_pBuffer;
		if (m_bOwnBuffer && m_pBuffer)
			((CWStringBuffer*)m_pBuffer)->AddRef();
	}
	CWString::~CWString()
	{
		clear();
	}
	void CWString::create(const wchar_t* wsString, bool bCopy, int nLen)
	{
		clear();
        unsigned int unLen = -1 == nLen ? std::min((unsigned int)wcslen(wsString), (unsigned int)MAX_STRING_LEN) : (unsigned int)nLen;
		m_unLen = unLen;
		if (bCopy)
		{
			if (unLen)
			{
				m_pBuffer    = (void*)(new CWStringBuffer(wsString, m_unLen));
				m_bOwnBuffer = true;
			}
		}
		else
		{
			m_pBuffer    = (void*)wsString;
			m_bOwnBuffer = false;
		}
	}
    void CWString::create(const std::wstring& sString, bool bCopy)
    {
        // unused bCopy
        create(sString.c_str(), true, (int)sString.length());
    }

	void CWString::clear()
	{
		if (m_bOwnBuffer)
		{
			CWStringBuffer* pWStringBuffer = (CWStringBuffer*)m_pBuffer;
			if (pWStringBuffer && !pWStringBuffer->Release())
				delete pWStringBuffer;
		}

		m_bOwnBuffer = false;
		m_pBuffer    = NULL;
		m_unLen      = 0;
	}
	void CWString::operator=(const wchar_t* wsString)
	{
		clear();
		create(wsString, false);
	}
	void CWString::operator=(const CWString& wsString)
	{
		clear();
		m_unLen      = wsString.m_unLen;
		m_bOwnBuffer = wsString.m_bOwnBuffer;
		m_pBuffer    = wsString.m_pBuffer;
		if (m_bOwnBuffer && m_pBuffer)
			((CWStringBuffer*)m_pBuffer)->AddRef();
	}
	const wchar_t* CWString::c_str() const
	{
		if (m_bOwnBuffer)
		{
			CWStringBuffer* pWStringBuffer = (CWStringBuffer*)m_pBuffer;
			if (pWStringBuffer)
				return pWStringBuffer->m_pBuffer;

			return NULL;
		}

		return (const wchar_t*)m_pBuffer;
	}
    const std::wstring CWString::c_stdstr() const
    {
        const wchar_t* pData = c_str();
        if (NULL == pData)
            return L"";
        return std::wstring(pData);
    }
	bool CWString::operator<(const CWString& wsString) const
	{
		const wchar_t* wsLeft = this->c_str();
		const wchar_t* wsRight = wsString.c_str();

        unsigned int unLen = std::min(m_unLen, wsString.m_unLen);
		for (unsigned int unPos = 0; unPos < unLen; unPos++)
		{
			if (wsLeft[unPos] < wsRight[unPos])
				return true;
			else if (wsLeft[unPos] > wsRight[unPos])
				return false;
		}

		return (m_unLen > wsString.m_unLen);
	}
	bool CWString::operator>(const CWString& wsString) const
	{
		return !operator<(wsString);
	}
	bool CWString::operator==(const CWString& wsString) const
	{
		const wchar_t* wsLeft = this->c_str();
		const wchar_t* wsRight = wsString.c_str();

		if (m_unLen != wsString.m_unLen)
			return false;

		for (unsigned int unPos = 0; unPos < m_unLen; unPos++)
		{
			if (wsLeft[unPos] != wsRight[unPos])
				return false;
		}

		return true;
	}
	bool CWString::operator==(const wchar_t* wsString) const
	{
		const wchar_t* wsLeft = this->c_str();
        unsigned unLen = std::min((unsigned int)wcslen(wsString), (unsigned int)MAX_STRING_LEN);

		if (m_unLen != unLen)
			return false;

		for (unsigned int unPos = 0; unPos < m_unLen; unPos++)
		{
			if (wsLeft[unPos] != wsString[unPos])
				return false;
		}

		return true;
	}
	unsigned int CWString::size() const
	{
		return m_unLen;
	}
	wchar_t CWString::operator[](const unsigned int& unIndex) const
	{
		return this->c_str()[unIndex];
	}
	bool CWString::empty() const
	{
		return 0 == m_unLen;
	}
    int CWString::tointeger() const
    {
        return std::stoi(c_str());
    }
	std::vector<CWString> CWString::split(wchar_t wChar, bool bCopy)
	{
		std::vector<CWString> vResult;

		int nPos = 0;
		int nLen = size();
		int nCharPos = nPos;
		while (nPos < nLen)
		{
			int nCharPos = nPos;
			while (nCharPos < nLen && operator[](nCharPos) != wChar)
				nCharPos++;

			CWString wsElement;
			wsElement.create(this->c_str() + nPos, bCopy, nCharPos - nPos);
			vResult.push_back(wsElement);
			nPos = nCharPos + 1;
		}
		return vResult;
	}
}
