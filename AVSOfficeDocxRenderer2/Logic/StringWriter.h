#pragma once
#include "File.h"

const double c_ag_Inch_to_MM	= 25.4;
const double c_ag_1pxWidth		= 25.4 / 96;

namespace NSDocxRenderer
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
	};

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

		__forceinline void AddSize(size_t nSize, const size_t nSizeMin = 1000)
		{
			if (NULL == m_pData)
			{
				m_lSize = max(nSize, nSizeMin);				
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

	public:
		
		inline void WriteString(wchar_t* pString, size_t& nLen)
		{
			AddSize(nLen);
			//memcpy(m_pDataCur, pString, nLen * sizeof(wchar_t));
			memcpy(m_pDataCur, pString, nLen << 1);
			m_pDataCur += nLen;
			m_lSizeCur += nLen;
		}
		inline void WriteString(_bstr_t& bsString)
		{
			size_t nLen = bsString.length();
			WriteString(bsString.GetBSTR(), nLen);
		}
		inline void WriteString(CString& sString)
		{
			size_t nLen = (size_t)sString.GetLength();

			#ifdef _UNICODE
			WriteString(sString.GetBuffer(), nLen);
			#else
			CStringW str = (CStringW)sString;
			WriteString(str.GetBuffer(), nLen);
			#endif
		}

		inline size_t GetCurSize()
		{
			return m_lSizeCur;
		}

		inline void Write(CStringWriter& oWriter)
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
			m_lSizeCur	= 0;
		}

		CString GetData()
		{
			CString str(m_pData, (int)m_lSizeCur);
			return str;
		}
	};
}