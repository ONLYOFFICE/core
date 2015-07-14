#ifndef _BUILD_STRING_BUILDER_CROSSPLATFORM_H_
#define _BUILD_STRING_BUILDER_CROSSPLATFORM_H_

#include <string>
#include <algorithm>

namespace NSStringUtils
{
	class CStringBuilder
	{
	private:
		wchar_t*	m_pData;
		size_t		m_lSize;

		wchar_t*	m_pDataCur;
		size_t		m_lSizeCur;

	public:
		CStringBuilder()
		{
			m_pData = NULL;
			m_lSize = 0;

			m_pDataCur = m_pData;
			m_lSizeCur = m_lSize;
		}
		~CStringBuilder()
		{
			if (NULL != m_pData)
				free(m_pData);
			m_pData = NULL;
		}

		inline void AddSize(size_t nSize)
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

	public:

		inline void WriteString(const wchar_t* pString, size_t nLen)
		{
			AddSize(nLen);

			memcpy(m_pDataCur, pString, nLen * sizeof(wchar_t));

			m_pDataCur += nLen;
			m_lSizeCur += nLen;
		}

		inline void WriteString(const std::wstring& sString)
		{
			this->WriteString(sString.c_str(), sString.length());
		}

		inline void AddCharSafe(const wchar_t& _c)
		{
			AddSize(1);
			*m_pDataCur++ = _c;
			++m_lSizeCur;
		}
		inline void AddChar2Safe(const wchar_t _c1, const wchar_t& _c2)
		{
			AddSize(2);
			*m_pDataCur++ = _c1;
			*m_pDataCur++ = _c2;
			m_lSizeCur += 2;
		}

		inline void WriteEncodeXmlString(const wchar_t* pString)
		{
			const wchar_t* pData = pString;
			while (*pData != 0)
			{
				BYTE _code = CheckCode(*pData);

				switch (_code)
				{
				case 1:
					AddCharSafe(*pData);
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
				default:
					break;
				}

				++pData;
			}

		}

		inline size_t GetCurSize()
		{
			return m_lSizeCur;
		}

		inline void Write(CStringBuilder& oWriter)
		{
			WriteString(oWriter.m_pData, oWriter.m_lSizeCur);
		}

		inline void Clear()
		{
			if (NULL != m_pData)
				free(m_pData);
			m_pData = NULL;

			m_pData = NULL;
			m_lSize = 0;

			m_pDataCur = m_pData;
			m_lSizeCur = 0;
		}
		inline void ClearNoAttack()
		{
			m_pDataCur = m_pData;
			m_lSizeCur = 0;
		}

		std::wstring GetData()
		{
			std::wstring str(m_pData, (int)m_lSizeCur);
			return str;
		}

	protected:
		inline unsigned char CheckCode(const wchar_t& c)
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

			return 1;			
		}
	};
}

#endif // _BUILD_STRING_BUILDER_CROSSPLATFORM_H_