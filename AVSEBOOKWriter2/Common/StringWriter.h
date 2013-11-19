#pragma once
#include "File.h"

const double c_ag_Inch_to_MM	= 25.4;
const double c_ag_1pxWidth		= 25.4 / 96;

static wchar_t g_wc_amp		= wchar_t('&');
static wchar_t g_wc_apos	= wchar_t('\'');
static wchar_t g_wc_lt		= wchar_t('<');
static wchar_t g_wc_qt		= wchar_t('>');
static wchar_t g_wc_quot	= wchar_t('\"');

static _bstr_t g_bstr_amp	= L"&amp;";
static _bstr_t g_bstr_apos	= L"&apos;";
static _bstr_t g_bstr_lt	= L"&lt;";
static _bstr_t g_bstr_qt	= L"&gt;";
static _bstr_t g_bstr_quot	= L"\"";
static _bstr_t g_bstr_mdash	= L"&mdash;";

namespace NSCommon
{
	enum ImageType
	{
		itJPG	= 0,
		itPNG	= 1
	};
	class CImageInfo
	{
	public:
		ImageType	m_eType;
		LONG		m_lID;

		CImageInfo()
		{
			m_eType	= itJPG;
			m_lID	= -1;
		}
		CImageInfo(const CImageInfo& oSrc)
		{
			*this = oSrc;
		}
		CImageInfo& operator=(const CImageInfo& oSrc)
		{
			m_eType		= oSrc.m_eType;
			m_lID		= oSrc.m_lID;

			return *this;
		}

		AVSINLINE CString GetPath(const CString& strMedia)
		{
			CString strExt = _T("");
			strExt.Format(_T("\\image%d.%s"), m_lID, (itJPG == m_eType) ? _T("jpg") : _T("png"));
			return strMedia + strExt;
		}
	};

	inline static double FABS(double dVal)
	{
		return (dVal >= 0) ? dVal : -dVal;
	}
	inline static int round(double dVal)
	{
		return (int)(dVal + 0.5);
	}

	class CBufferedText
	{
	protected:
		BYTE*	m_pData;
		size_t	m_lSize;

		BYTE*	m_pDataCur;
		size_t	m_lSizeCur;

	public:
		CBufferedText()
		{
			m_pData = NULL;
			m_lSize = 0;

			m_pDataCur	= m_pData;
			m_lSizeCur	= m_lSize;
		}
		CBufferedText(const CBufferedText& oSrc)
		{
			m_pData = NULL;
			*this = oSrc;
		}
		CBufferedText& operator=(const CBufferedText& oSrc)
		{
			RELEASEMEM(m_pData);

			m_lSize		= oSrc.m_lSize;
			m_lSizeCur	= oSrc.m_lSizeCur;
			m_pData		= (BYTE*)malloc(m_lSize * sizeof(BYTE));

			memcpy(m_pData, oSrc.m_pData, m_lSizeCur * sizeof(BYTE));
							
			m_pDataCur = m_pData + m_lSizeCur;

			return *this;
		}

		CBufferedText(const size_t& nLen)
		{
			m_lSize = nLen;
			m_pData = (BYTE*)malloc(m_lSize * sizeof(BYTE));
				
			m_lSizeCur = 0;
			m_pDataCur = m_pData;
		}
		virtual ~CBufferedText()
		{
			RELEASEMEM(m_pData);
		}

		AVSINLINE void AddSize(const size_t& nSize)
		{
			if (NULL == m_pData)
			{
				m_lSize = max(nSize, 1000);				
				m_pData = (BYTE*)malloc(m_lSize * sizeof(BYTE));
				
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

				BYTE* pRealloc = (BYTE*)realloc(m_pData, m_lSize * sizeof(BYTE));
				if (NULL != pRealloc)
				{
					// реаллок сработал
					m_pData		= pRealloc;
					m_pDataCur	= m_pData + m_lSizeCur;
				}
				else
				{
					BYTE* pMalloc = (BYTE*)malloc(m_lSize * sizeof(BYTE));
					memcpy(pMalloc, m_pData, m_lSizeCur * sizeof(BYTE));

					free(m_pData);
					m_pData		= pMalloc;
					m_pDataCur	= m_pData + m_lSizeCur;
				}
			}
		}

	public:
		
		AVSINLINE BYTE operator[](const size_t& nIndex)
		{
			if (nIndex < m_lSizeCur)
				return m_pData[nIndex];

			return 0;
		}

	public:
		AVSINLINE void WriteData(BYTE* pString, const size_t& nLen)
		{
			AddSize(nLen);
			memcpy(m_pDataCur, pString, nLen);
			m_pDataCur += nLen;
			m_lSizeCur += nLen;
		}
		AVSINLINE BYTE* GetData()
		{
			return m_pData;
		}
		AVSINLINE size_t GetCurSize()
		{
			return m_lSizeCur;
		}
		AVSINLINE size_t GetSize()
		{
			return m_lSize;
		}
		AVSINLINE void Clear()
		{
			RELEASEMEM(m_pData);
			
			m_pData = NULL;
			m_lSize = 0;

			m_pDataCur	= m_pData;
			m_lSizeCur	= 0;
		}
		AVSINLINE void ClearNoAttack()
		{
			m_pDataCur	= m_pData;
			m_lSizeCur	= 0;
		}
	};

	class CTextItem
	{
	protected:
		wchar_t*	m_pData;
		size_t		m_lSize;

		wchar_t*	m_pDataCur;
		size_t		m_lSizeCur;

	public:
		CTextItem()
		{
			m_pData = NULL;
			m_lSize = 0;

			m_pDataCur	= m_pData;
			m_lSizeCur	= m_lSize;
		}
		CTextItem(const CTextItem& oSrc)
		{
			m_pData = NULL;
			*this = oSrc;
		}
		CTextItem& operator=(const CTextItem& oSrc)
		{
			RELEASEMEM(m_pData);

			m_lSize		= oSrc.m_lSize;
			m_lSizeCur	= oSrc.m_lSizeCur;
			m_pData		= (wchar_t*)malloc(m_lSize * sizeof(wchar_t));

			memcpy(m_pData, oSrc.m_pData, m_lSizeCur * sizeof(wchar_t));
							
			m_pDataCur = m_pData + m_lSizeCur;

			return *this;
		}

		CTextItem(const size_t& nLen)
		{
			m_lSize = nLen;
			m_pData = (wchar_t*)malloc(m_lSize * sizeof(wchar_t));
				
			m_lSizeCur = 0;
			m_pDataCur = m_pData;
		}
		CTextItem(wchar_t* pData, const size_t& nLen)
		{
			m_lSize = nLen;
			m_pData = (wchar_t*)malloc(m_lSize * sizeof(wchar_t));

			memcpy(m_pData, pData, m_lSize * sizeof(wchar_t));
				
			m_lSizeCur = m_lSize;
			m_pDataCur = m_pData + m_lSize;
		}
		CTextItem(wchar_t* pData, BYTE* pUnicodeChecker = NULL)
		{
			size_t nLen = GetStringLen(pData);

			m_lSize = nLen;
			m_pData = (wchar_t*)malloc(m_lSize * sizeof(wchar_t));

			memcpy(m_pData, pData, m_lSize * sizeof(wchar_t));
				
			m_lSizeCur = m_lSize;
			m_pDataCur = m_pData + m_lSize;

			if (NULL != pUnicodeChecker)
			{
				wchar_t* pMemory = m_pData;
				while (pMemory < m_pDataCur)
				{
					if (!pUnicodeChecker[*pMemory])
						*pMemory = wchar_t(' ');
					++pMemory;
				}
			}
		}
		virtual ~CTextItem()
		{
			RELEASEMEM(m_pData);
		}

		AVSINLINE void AddSize(const size_t& nSize)
		{
			if (NULL == m_pData)
			{
				m_lSize = max(nSize, 1000);				
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
					m_pData		= pRealloc;
					m_pDataCur	= m_pData + m_lSizeCur;
				}
				else
				{
					wchar_t* pMalloc = (wchar_t*)malloc(m_lSize * sizeof(wchar_t));
					memcpy(pMalloc, m_pData, m_lSizeCur * sizeof(wchar_t));

					free(m_pData);
					m_pData		= pMalloc;
					m_pDataCur	= m_pData + m_lSizeCur;
				}
			}
		}

		AVSINLINE wchar_t* GetData()
		{
			return m_pData;
		}
		AVSINLINE size_t GetCurSize()
		{
			return m_lSizeCur;
		}

	public:
		
		AVSINLINE void operator+=(const CTextItem& oTemp)
		{
			WriteString(oTemp.m_pData, oTemp.m_lSizeCur);
		}
		AVSINLINE void operator+=(_bstr_t& oTemp)
		{
			size_t nLen = oTemp.length();
			WriteString(oTemp.GetBSTR(), nLen);
		}
		AVSINLINE void operator+=(CString& oTemp)
		{
			size_t nLen = (size_t)oTemp.GetLength();

			#ifdef _UNICODE
			WriteString(oTemp.GetBuffer(), nLen);
			#else
			CStringW str = (CStringW)oTemp;
			WriteString(str.GetBuffer(), nLen);
			#endif
		}
		AVSINLINE wchar_t& operator[](const size_t& nIndex)
		{
			return m_pData[nIndex];
		}

		AVSINLINE void SetText(BSTR& bsText)
		{
			ClearNoAttack();
			size_t nLen = GetStringLen(bsText);

			WriteString(bsText, nLen);
		}
		AVSINLINE void AddSpace()
		{
			AddSize(1);
			*m_pDataCur = wchar_t(' ');

			++m_lSizeCur;
			++m_pDataCur;
		}
		AVSINLINE void AddSpaceFirst()
		{
			AddSize(1);

			wchar_t* pMemory = new wchar_t[m_lSizeCur];
			memcpy(pMemory, m_pData, m_lSizeCur * sizeof(wchar_t));
			memcpy(m_pData + 1, pMemory, m_lSizeCur * sizeof(wchar_t));
			RELEASEARRAYOBJECTS(pMemory);

			*m_pData = wchar_t(' ');

			++m_lSizeCur;
			++m_pDataCur;
		}
		AVSINLINE void CorrectUnicode(const BYTE* pUnicodeChecker)
		{
			if (NULL != pUnicodeChecker)
			{
				wchar_t* pMemory = m_pData;
				while (pMemory < m_pDataCur)
				{
					if (!pUnicodeChecker[*pMemory])
						*pMemory = wchar_t(' ');
					++pMemory;
				}
			}
		}
		AVSINLINE void RemoveLastSpaces()
		{
			wchar_t* pMemory = m_pDataCur - 1;
			while ((pMemory > m_pData) && (wchar_t(' ') == *pMemory))
			{
				--pMemory;
				--m_lSizeCur;
				--m_pDataCur;
			}

		}
		AVSINLINE bool IsSpace()
		{
			if (1 != m_lSizeCur)
				return false;
			return (wchar_t(' ') == *m_pData);
		}
		AVSINLINE void CheckLastSpanLine()
		{
			if (0 == m_lSizeCur)
				return;

			if ((wchar_t(' ') == m_pData[m_lSizeCur - 1]) || (wchar_t('-') == m_pData[m_lSizeCur - 1]))
				return;

			AddSpace();			
		}

	public:
		AVSINLINE void WriteString(wchar_t* pString, const size_t& nLen)
		{
			AddSize(nLen);
			//memcpy(m_pDataCur, pString, nLen * sizeof(wchar_t));
			memcpy(m_pDataCur, pString, nLen << 1);
			m_pDataCur += nLen;
			m_lSizeCur += nLen;
		}
		AVSINLINE size_t GetSize()
		{
			return m_lSize;
		}
		AVSINLINE void Clear()
		{
			RELEASEMEM(m_pData);
			
			m_pData = NULL;
			m_lSize = 0;

			m_pDataCur	= m_pData;
			m_lSizeCur	= 0;
		}
		AVSINLINE void ClearNoAttack()
		{
			m_pDataCur	= m_pData;
			m_lSizeCur	= 0;
		}

		AVSINLINE size_t GetStringLen(wchar_t* pData)
		{
			wchar_t* s = pData;
			for (; *s != 0; ++s);
			return (size_t)(s - pData);
		}

		AVSINLINE CString GetCString()
		{
			CString str(m_pData, (int)m_lSizeCur);
			return str;
		}
		AVSINLINE wchar_t* GetBuffer()
		{
			return m_pData;
		}
	};

	class CStringWriter : public CTextItem
	{
	public:
		CStringWriter() : CTextItem()
		{
		}
		virtual ~CStringWriter()
		{
		}

	public:
		
		AVSINLINE void WriteStringB(_bstr_t& bsString)
		{
			size_t nLen = bsString.length();
			CTextItem::WriteString(bsString.GetBSTR(), nLen);
		}
		AVSINLINE void WriteString(CString sString)
		{
			size_t nLen = (size_t)sString.GetLength();

			#ifdef _UNICODE
			CTextItem::WriteString(sString.GetBuffer(), nLen);
			#else
			CStringW str = (CStringW)sString;
			WriteString(str.GetBuffer(), nLen);
			#endif
		}
		AVSINLINE void WriteStringC(const CString& sString)
		{
			size_t nLen = (size_t)sString.GetLength();

			CString* pStr = const_cast<CString*>(&sString);

			#ifdef _UNICODE
			CTextItem::WriteString(pStr->GetBuffer(), nLen);
			#else
			CStringW str = (CStringW)sString;
			WriteString(str.GetBuffer(), nLen);
			#endif
		}
		AVSINLINE void Write(CStringWriter& oWriter)
		{
			CTextItem::WriteString(oWriter.m_pData, oWriter.m_lSizeCur);
		}
		AVSINLINE void WriteI(CTextItem& oItem)
		{
			CTextItem::WriteString(oItem.GetData(), oItem.GetCurSize());
		}

		AVSINLINE void WriteString(wchar_t* pString, const size_t& nLen)
		{
			CTextItem::AddSize(nLen);
			//memcpy(m_pDataCur, pString, nLen * sizeof(wchar_t));
			memcpy(m_pDataCur, pString, nLen << 1);
			m_pDataCur += nLen;
			m_lSizeCur += nLen;
		}

		void WriteTextHTML(CTextItem& oItem)
		{
			size_t nCurrent = 0;
			size_t nCount	= oItem.GetCurSize();

			size_t nCurrentOld = nCurrent;
			wchar_t* pData = oItem.GetData();
			wchar_t* pStartData = pData;

			while (nCurrent < nCount)
			{
				wchar_t c = *pData++;

				if (g_wc_amp == c)
				{
					if (nCurrentOld != nCurrent)
						WriteString(pStartData, nCurrent - nCurrentOld);

					WriteStringB(g_bstr_amp);
					
					++nCurrent;
					nCurrentOld = nCurrent;
					pStartData = pData;
				}
				/*else if (g_wc_apos == c)
				{
					if (nCurrentOld != nCurrent)
						WriteString(pStartData, nCurrent - nCurrentOld);

					WriteStringB(g_bstr_apos);
					
					++nCurrent;
					nCurrentOld = nCurrent;
					pStartData = pData;
				}*/
				else if (g_wc_lt == c)
				{
					if (nCurrentOld != nCurrent)
						WriteString(pStartData, nCurrent - nCurrentOld);

					WriteStringB(g_bstr_lt);
					
					++nCurrent;
					nCurrentOld = nCurrent;
					pStartData = pData;
				}
				else if (g_wc_qt == c)
				{
					if (nCurrentOld != nCurrent)
						WriteString(pStartData, nCurrent - nCurrentOld);

					WriteStringB(g_bstr_qt);
					
					++nCurrent;
					nCurrentOld = nCurrent;
					pStartData = pData;
				}
				else if (g_wc_quot == c)
				{
					if (nCurrentOld != nCurrent)
						WriteString(pStartData, nCurrent - nCurrentOld);

					WriteStringB(g_bstr_quot);
					
					++nCurrent;
					nCurrentOld = nCurrent;
					pStartData = pData;
				}
				else if (8212 == (USHORT)c)
				{
					if (nCurrentOld != nCurrent)
						WriteString(pStartData, nCurrent - nCurrentOld);

					WriteStringB(g_bstr_mdash);
					
					++nCurrent;
					nCurrentOld = nCurrent;
					pStartData = pData;
				}
				else
				{
					++nCurrent;
				}
			}

			if (nCurrentOld != nCurrent)
				WriteString(pStartData, nCurrent - nCurrentOld);			
		}

		void WriteTextXML(CTextItem& oItem)
		{
			size_t nCurrent = 0;
			size_t nCount	= oItem.GetCurSize();

			size_t nCurrentOld = nCurrent;
			wchar_t* pData = oItem.GetData();
			wchar_t* pStartData = pData;

			while (nCurrent < nCount)
			{
				wchar_t c = *pData++;

				if (g_wc_amp == c)
				{
					if (nCurrentOld != nCurrent)
						WriteString(pStartData, nCurrent - nCurrentOld);

					WriteStringB(g_bstr_amp);
					
					++nCurrent;
					nCurrentOld = nCurrent;
					pStartData = pData;
				}
				/*else if (g_wc_apos == c)
				{
					if (nCurrentOld != nCurrent)
						WriteString(pStartData, nCurrent - nCurrentOld);

					WriteStringB(g_bstr_apos);
					
					++nCurrent;
					nCurrentOld = nCurrent;
					pStartData = pData;
				}*/
				else if (g_wc_lt == c)
				{
					if (nCurrentOld != nCurrent)
						WriteString(pStartData, nCurrent - nCurrentOld);

					WriteStringB(g_bstr_lt);
					
					++nCurrent;
					nCurrentOld = nCurrent;
					pStartData = pData;
				}
				else if (g_wc_qt == c)
				{
					if (nCurrentOld != nCurrent)
						WriteString(pStartData, nCurrent - nCurrentOld);

					WriteStringB(g_bstr_qt);
					
					++nCurrent;
					nCurrentOld = nCurrent;
					pStartData = pData;
				}
				else if (g_wc_quot == c)
				{
					if (nCurrentOld != nCurrent)
						WriteString(pStartData, nCurrent - nCurrentOld);

					WriteStringB(g_bstr_quot);
					
					++nCurrent;
					nCurrentOld = nCurrent;
					pStartData = pData;
				}
				else
				{
					++nCurrent;
				}
			}

			if (nCurrentOld != nCurrent)
				WriteString(pStartData, nCurrent - nCurrentOld);			
		}
	};
}