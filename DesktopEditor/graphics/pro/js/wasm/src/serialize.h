#ifndef _WASM_SERIALIZE_H
#define _WASM_SERIALIZE_H

#include <vector>
#include <stack>
#include "../../../../../common/File.h"

#define CLEAR_STACK(stack) while (!(stack).empty()) (stack).pop()

namespace NSWasm
{
	class CData
	{
	protected:
		BYTE* m_pData;
		size_t m_lSize;

		BYTE* m_pDataCur;
		size_t m_lSizeCur;

		std::stack<size_t> m_lStack;

	public:
		CData()
		{
			m_pData = NULL;
			m_lSize = 0;

			m_pDataCur = m_pData;
			m_lSizeCur = m_lSize;
		}
		CData(const CData& other)
		{
			m_lSize = other.m_lSize;
			m_lSizeCur = other.m_lSizeCur;

			m_pData = (BYTE*)malloc(m_lSize * sizeof(BYTE));
			memcpy(m_pData, other.m_pData, other.m_lSizeCur * sizeof(BYTE));
			m_pDataCur = m_pData + m_lSizeCur;

			m_lStack = other.m_lStack;
		}
		CData(CData&& other)
		{
			m_lSize = other.m_lSize;
			m_lSizeCur = other.m_lSizeCur;

			m_pData = other.m_pData;
			m_pDataCur = other.m_pDataCur;

			m_lStack = std::move(other.m_lStack);

			other.ClearWithoutAttack();
		}
		virtual ~CData()
		{
			Clear();
		}
		CData& operator= (const CData& other)
		{
			if (this == &other)
				return *this;

			Clear();

			m_lSize = other.m_lSize;
			m_lSizeCur = other.m_lSizeCur;

			m_pData = (BYTE*)malloc(m_lSize * sizeof(BYTE));
			memcpy(m_pData, other.m_pData, other.m_lSizeCur * sizeof(BYTE));
			m_pDataCur = m_pData + m_lSizeCur;

			m_lStack = other.m_lStack;

			return *this;
		}
		CData& operator= (CData&& other)
		{
			if (this == &other)
				return *this;

			Clear();

			m_lSize = other.m_lSize;
			m_lSizeCur = other.m_lSizeCur;

			m_pData = other.m_pData;
			m_pDataCur = other.m_pDataCur;

			other.ClearWithoutAttack();

			m_lStack = std::move(other.m_lStack);

			return *this;
		}

		inline void AddSize(size_t nSize)
		{
			if (NULL == m_pData)
			{
				m_lSize = 1000;
				if (nSize > m_lSize)
					m_lSize = nSize;

				m_pData = (BYTE*)malloc(m_lSize * sizeof(BYTE));

				m_lSizeCur = 0;
				m_pDataCur = m_pData;
				return;
			}

			if ((m_lSizeCur + nSize) > m_lSize)
			{
				while ((m_lSizeCur + nSize) > m_lSize)
					m_lSize *= 2;

				BYTE* pRealloc = (BYTE*)realloc(m_pData, m_lSize * sizeof(BYTE));
				if (NULL != pRealloc)
				{
					m_pData = pRealloc;
					m_pDataCur = m_pData + m_lSizeCur;
				}
				else
				{
					BYTE* pMalloc = (BYTE*)malloc(m_lSize * sizeof(BYTE));
					memcpy(pMalloc, m_pData, m_lSizeCur * sizeof(BYTE));

					free(m_pData);
					m_pData = pMalloc;
					m_pDataCur = m_pData + m_lSizeCur;
				}
			}
		}

	public:
		void StartRecord(BYTE type)
		{
			AddSize(5); // sizeof (BYTE + unsigned int)
			WriteBYTE(type);
			AddInt(0);
			m_lStack.push(m_lSizeCur);
		}
		void EndRecord()
		{
			size_t start = m_lStack.top();
			unsigned int len = m_lSizeCur - start;
			memcpy(m_pData + start - 4, &len, sizeof(unsigned int));
			m_lStack.pop();
		}
		void AddInt(unsigned int value)
		{
			AddSize(4);
			memcpy(m_pDataCur, &value, sizeof(unsigned int));
			m_pDataCur += 4;
			m_lSizeCur += 4;
		}
		void AddSInt(int value)
		{
			AddSize(4);
			memcpy(m_pDataCur, &value, sizeof(int));
			m_pDataCur += 4;
			m_lSizeCur += 4;
		}
		void AddInt(unsigned int value, size_t pos)
		{
			if (pos < m_lSizeCur)
				memcpy(m_pData + pos, &value, sizeof(unsigned int));
		}
		void AddDouble(double value)
		{
			// такой точности хватит
			int nV = value * 100;
			AddInt(nV);
		}
		void WriteBYTE(BYTE value)
		{
			AddSize(sizeof(BYTE));
			memcpy(m_pDataCur, &value, sizeof(BYTE));
			m_pDataCur += sizeof(BYTE);
			m_lSizeCur += sizeof(BYTE);
		}
		void WriteBool(bool value)
		{
			WriteBYTE(value ? 1 : 0);
		}
		void WriteDouble(double value)
		{
			int nV = value * 10000;
			AddInt(nV);
		}
		void WriteDouble2(double value)
		{
			SHORT lValue = (SHORT)(value * 100);
			AddSize(sizeof(SHORT));
			memcpy(m_pDataCur, &lValue, sizeof(SHORT));
			m_pDataCur += sizeof(SHORT);
			m_lSizeCur += sizeof(SHORT);
		}
		void WriteWCHAR(int value)
		{
			if (value < 0x10000)
				WriteUSHORT(value);
			else
			{
				AddSize(4);
				int code = value - 0x10000;
				USHORT c1 = 0xD800 | ((code >> 10) & 0x03FF);
				USHORT c2 = 0xDC00 | (code & 0x03FF);
				memcpy(m_pDataCur, &c1, sizeof(USHORT));
				memcpy(m_pDataCur + 2, &c2, sizeof(USHORT));
				m_pDataCur += 4;
				m_lSizeCur += 4;
			}
		}
		void WriteUSHORT(USHORT value)
		{
			AddSize(sizeof(USHORT));
			memcpy(m_pDataCur, &value, sizeof(USHORT));
			m_pDataCur += sizeof(USHORT);
			m_lSizeCur += sizeof(USHORT);
		}
		void WriteString(BYTE* value, unsigned int len)
		{
			AddSize(len + 4);
			memcpy(m_pDataCur, &len, sizeof(unsigned int));
			m_pDataCur += 4;
			m_lSizeCur += 4;
			memcpy(m_pDataCur, value, len);
			m_pDataCur += len;
			m_lSizeCur += len;
		}
		void WriteString(const std::string& sStr)
		{
			WriteString((BYTE*)sStr.c_str(), (unsigned int)sStr.length());
		}
		void WriteString(const std::wstring& sStr)
		{
			BYTE* pDataUtf8 = NULL;
			LONG lDataUtf8 = 0;
			NSFile::CUtf8Converter::GetUtf8StringFromUnicode(sStr.c_str(), (LONG)sStr.length(), pDataUtf8, lDataUtf8);
			WriteString(pDataUtf8, (unsigned int)lDataUtf8);
			RELEASEARRAYOBJECTS(pDataUtf8);
		}
		void WriteStringUtf16(const std::wstring& sStr, const bool& isBytes = false)
		{
			unsigned int size = static_cast<unsigned int>(sStr.length());

			int len = 0;
			size_t posLen = m_lSizeCur;

			// len reserve
			AddInt(0);

			if (sizeof(wchar_t) == 4)
			{
				AddSize(4 * size + 2/*'\0'*/);
				NSFile::CUtf8Converter::GetUtf16StringFromUnicode_4bytes(sStr.c_str(), (LONG)size, m_pDataCur, len, false);
			}
			else
			{
				size_t bufferLen = 2 * size;
				AddSize(bufferLen);
				len = (int)(bufferLen);
				memcpy(m_pDataCur, sStr.c_str(), bufferLen);
			}

			m_pDataCur += static_cast<unsigned int>(len);
			m_lSizeCur += static_cast<unsigned int>(len);

			if (!isBytes)
				len /= 2;

			AddInt((unsigned int)len, posLen);
		}
		void Write(BYTE* value, unsigned int len)
		{
			if (!value || len == 0)
				return;
			AddSize(len);
			memcpy(m_pDataCur, value, len);
			m_pDataCur += len;
			m_lSizeCur += len;
		}
		BYTE* GetBuffer()
		{
			return m_pData;
		}

		BYTE* MoveBuffer()
		{
			BYTE* pData = m_pData;
			ClearWithoutAttack();
			return pData;
		}

		void Clear()
		{
			if (m_pData)
				free(m_pData);

			m_pData = NULL;
			m_lSize = 0;

			m_pDataCur = m_pData;
			m_lSizeCur = 0;

			CLEAR_STACK(m_lStack);
		}
		void ClearWithoutAttack()
		{
			m_pData = NULL;
			m_lSize = 0;

			m_pDataCur = m_pData;
			m_lSizeCur = 0;

			CLEAR_STACK(m_lStack);
		}
		void ClearNoAttack()
		{
			m_pDataCur = m_pData;
			m_lSizeCur = 0;

			CLEAR_STACK(m_lStack);
		}
		unsigned int GetSize()
		{
			return (unsigned int)m_lSizeCur;
		}

		void SkipLen()
		{
			AddInt(0);
		}
		void WriteLen()
		{
			unsigned int len = (unsigned int)m_lSizeCur;
			memcpy(m_pData, &len, sizeof(unsigned int));
		}
	};

	struct CHChar
	{
		int unicode; // юникодное значение
		double x; // сдвиг по baseline
		double width; // ширина символа (сдвиг до след буквы)

		CHChar() : unicode(0), x(0), width(0) {}
		CHChar(const CHChar& oSrc) { *this = oSrc; }
		CHChar& operator=(const CHChar& oSrc)
		{
			unicode = oSrc.unicode;
			x = oSrc.x;
			width = oSrc.width;
			return *this;
		}

		inline void Clear()
		{
			unicode = 0;
			x = 0;
			width = 0;
		}
	};

	struct CHLine
	{
		double m_dAscent;
		double m_dDescent;
		double m_dX;
		double m_dY;

		double m_dEndX;
		double m_dEndY;

		// baseline ruler: y = k*x + b
		double m_dK;
		double m_dB;
		double m_ex;
		double m_ey;
		bool m_bIsConstX;

		// symbols
		// не менять на списки. постоянное создание объектов и их удаление -
		// плохо влияет на скорость
		CHChar* m_pChars;
		LONG m_lSizeChars;
		LONG m_lCharsTail;

		bool m_bIsSetUpTransform;
		double m_sx;
		double m_sy;
		double m_shx;
		double m_shy;

		CHLine()
		{
			m_dAscent = 0;
			m_dDescent = 0;
			m_dX = 0;
			m_dY = 0;

			m_dK = 0;
			m_dB = 0;
			m_bIsConstX = false;

			m_ex = 0;
			m_ey = 0;

			m_lSizeChars = 1000;
			m_lCharsTail = 0;
			m_pChars = new CHChar[m_lSizeChars];

			m_bIsSetUpTransform = false;
			m_sx = 1;
			m_sy = 1;
			m_shx = 0;
			m_shy = 0;
		}
		CHLine& operator=(const CHLine& oLine)
		{
			m_dAscent = oLine.m_dAscent;
			m_dDescent = oLine.m_dDescent;
			m_dX = oLine.m_dX;
			m_dY = oLine.m_dY;

			m_dK = oLine.m_dK;
			m_dB = oLine.m_dB;

			m_lSizeChars = oLine.m_lSizeChars;
			m_lCharsTail = oLine.m_lCharsTail;

			RELEASEARRAYOBJECTS(m_pChars);
			m_pChars = new CHChar[m_lSizeChars];

			for (LONG i = 0; i < m_lSizeChars; ++i)
				m_pChars[i] = oLine.m_pChars[i];

			m_bIsSetUpTransform = oLine.m_bIsSetUpTransform;
			m_sx = oLine.m_sx;
			m_sy = oLine.m_sy;
			m_shx = oLine.m_shx;
			m_shy = oLine.m_shy;

			return *this;
		}
		~CHLine()
		{
			RELEASEARRAYOBJECTS(m_pChars);
		}

		inline void Clear()
		{
			m_dAscent = 0;
			m_dDescent = 0;
			m_dX = 0;
			m_dY = 0;

			m_dK = 0;
			m_dB = 0;
			m_bIsConstX = false;

			m_ex = 0;
			m_ey = 0;

			m_lCharsTail = 0;

			m_bIsSetUpTransform = false;
			m_sx = 1;
			m_sy = 1;
			m_shx = 0;
			m_shy = 0;
		}

		inline CHChar* AddTail()
		{
			if (m_lCharsTail >= m_lSizeChars)
			{
				CHChar* pNews = new CHChar[2 * m_lSizeChars];
				for (LONG i = 0; i < m_lSizeChars; ++i)
					pNews[i] = m_pChars[i];

				RELEASEARRAYOBJECTS(m_pChars);
				m_pChars = pNews;
				m_lSizeChars *= 2;
			}

			CHChar* pChar = &m_pChars[m_lCharsTail];
			++m_lCharsTail;
			pChar->Clear();

			return pChar;
		}

		inline CHChar* GetTail()
		{
			if (m_lCharsTail >= m_lSizeChars)
			{
				CHChar* pNews = new CHChar[2 * m_lSizeChars];
				for (LONG i = 0; i < m_lSizeChars; ++i)
					pNews[i] = m_pChars[i];

				RELEASEARRAYOBJECTS(m_pChars);
				m_pChars = pNews;
				m_lSizeChars *= 2;
			}

			return m_lCharsTail ? &m_pChars[m_lCharsTail - 1] : NULL;
		}

		inline LONG GetCountChars()
		{
			return m_lCharsTail;
		}
	};
} // namespace NSWasm

namespace NSWasm
{
	struct CPageLinkItem
	{
		std::string Link;
		double Dest;

		double X;
		double Y;
		double W;
		double H;
	};

	class CPageLink
	{
	public:
		std::vector<CPageLinkItem> m_arLinks;

	public:
		BYTE* Serialize()
		{
			NSWasm::CData oRes;
			oRes.SkipLen();
			for (const CPageLinkItem& link : m_arLinks)
			{
				oRes.WriteString((BYTE*)link.Link.c_str(), link.Link.length());
				oRes.AddDouble(link.Dest);
				oRes.AddDouble(link.X);
				oRes.AddDouble(link.Y);
				oRes.AddDouble(link.W);
				oRes.AddDouble(link.H);
			}
			oRes.WriteLen();

			BYTE* res = oRes.GetBuffer();
			oRes.ClearWithoutAttack();
			return res;
		}
	};
} // namespace NSWasm

#endif // _WASM_SERIALIZE_H
