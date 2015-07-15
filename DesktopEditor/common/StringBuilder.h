#ifndef _BUILD_STRING_BUILDER_CROSSPLATFORM_H_
#define _BUILD_STRING_BUILDER_CROSSPLATFORM_H_

#include <string>
#include <algorithm>

namespace NSStringUtils
{
	const wchar_t g_hex_values[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
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

		inline void WriteStringNoSafe(const wchar_t* pString, size_t nLen)
		{
			memcpy(m_pDataCur, pString, nLen * sizeof(wchar_t));

			m_pDataCur += nLen;
			m_lSizeCur += nLen;
		}
		inline void WriteString(const wchar_t* pString, size_t nLen)
		{
			AddSize(nLen);
			WriteStringNoSafe(pString, nLen);
		}

		inline void WriteString(const std::wstring& sString)
		{
			this->WriteString(sString.c_str(), sString.length());
		}

		inline void AddCharNoSafe(const wchar_t& _c)
		{			
			*m_pDataCur++ = _c;
			++m_lSizeCur;
		}
		inline void AddCharNoCheck(const wchar_t& _c)
		{
			*m_pDataCur++ = _c;
			++m_lSizeCur;
		}
		inline void AddSpaceNoCheck()
		{
			*m_pDataCur++ = ' ';
			++m_lSizeCur;
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
		inline void SetCurSize(size_t lCurSize)
		{
			m_lSizeCur = lCurSize;
			m_pDataCur = m_pData + m_lSizeCur;
		}
		inline size_t GetSize()
		{
			return m_lSize;
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
		wchar_t* GetBuffer()
		{
			return m_pData;
		}

		inline void RemoveLastSpaces()
		{
			wchar_t* pMemory = m_pDataCur - 1;
			while ((pMemory > m_pData) && (wchar_t(' ') == *pMemory))
			{
				--pMemory;
				--m_lSizeCur;
				--m_pDataCur;
			}

		}
		inline bool IsSpace()
		{
			if (1 != m_lSizeCur)
				return false;
			return (wchar_t(' ') == *m_pData);
		}

		void AddInt(int val)
		{
			AddSize(10);
			AddIntNoCheck(val);
		}
		void AddIntDel10(int val)
		{
			AddSize(11);
			AddIntNoCheckDel10(val);
		}
		void AddIntDel100(int val)
		{
			AddSize(11);
			AddIntNoCheckDel100(val);
		}

		void AddIntNoCheck(int val)
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

		void AddIntNoCheckDel10(int val)
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
		void AddIntNoCheckDel100(int val)
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
		void AddDouble(double val, int count)
		{
			// TODO:
		}

		inline void WriteHexByteNoSafe(const unsigned char& value)
		{
			*m_pDataCur++ = g_hex_values[(value >> 4) & 0x0F];
			++m_lSizeCur;
			*m_pDataCur++ = g_hex_values[value & 0x0F];
			++m_lSizeCur;
		}
		inline void WriteHexByte(const unsigned char& value)
		{
			AddSize(2);
			WriteHexByteNoSafe(value);
		}
		inline void WriteHexInt3(const unsigned int& value)
		{
			AddSize(6);
			WriteHexByteNoSafe((value >> 16) & 0xFF);
			WriteHexByteNoSafe((value >> 8) & 0xFF);
			WriteHexByteNoSafe(value & 0xFF);
		}
		inline void WriteHexColor3(const unsigned char& r, const unsigned char& g, const unsigned char& b)
		{
			AddSize(7);
			*m_pDataCur++ = (wchar_t)'#';
			++m_lSizeCur;
			WriteHexByteNoSafe(r);
			WriteHexByteNoSafe(g);
			WriteHexByteNoSafe(b);
		}
		inline void WriteHexColor3(const unsigned int& value)
		{
			AddSize(7);
			*m_pDataCur++ = (wchar_t)'#';
			++m_lSizeCur;
                        WriteHexByteNoSafe(value & 0xFF);
                        WriteHexByteNoSafe((value >> 8) & 0xFF);
                        WriteHexByteNoSafe((value >> 16) & 0xFF);
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
