#ifndef _WASM_SERIALIZE_H
#define _WASM_SERIALIZE_H

#include "../../../../../graphics/IRenderer.h"
#include "../../../../../common/StringExt.h"
#include "../../../../../common/StringUTF32.h"
#include "../../../../../graphics/pro/Fonts.h"

namespace NSWasm
{
    class CData
    {
    protected:
        BYTE* m_pData;
        size_t m_lSize;

        BYTE* m_pDataCur;
        size_t m_lSizeCur;

    public:
        CData()
        {
            m_pData = NULL;
            m_lSize = 0;

            m_pDataCur = m_pData;
            m_lSizeCur = m_lSize;
        }
        virtual ~CData()
        {
            Clear();
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
                    m_pData    = pRealloc;
                    m_pDataCur = m_pData + m_lSizeCur;
                }
                else
                {
                    BYTE* pMalloc = (BYTE*)malloc(m_lSize * sizeof(BYTE));
                    memcpy(pMalloc, m_pData, m_lSizeCur * sizeof(BYTE));

                    free(m_pData);
                    m_pData    = pMalloc;
                    m_pDataCur = m_pData + m_lSizeCur;
                }
            }
        }

    public:
        void AddInt(unsigned int value)
        {
            AddSize(4);
            memcpy(m_pDataCur, &value, sizeof(unsigned int));
            m_pDataCur += 4;
            m_lSizeCur += 4;
        }
        void AddDouble(double value)
        {
            // такой точности хватит
            AddInt(value * 100);
        }
        void WriteBYTE(BYTE value)
        {
            AddSize(sizeof(BYTE));
            memcpy(m_pDataCur, &value, sizeof(BYTE));
            m_pDataCur += sizeof(BYTE);
            m_lSizeCur += sizeof(BYTE);
        }
        void WriteDouble(double value)
        {
            AddInt(value * 10000);
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
                memcpy(m_pDataCur,     &c1, sizeof(USHORT));
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
        void Write(BYTE* value, unsigned int len)
        {
            AddSize(len);
            memcpy(m_pDataCur, value, len);
            m_pDataCur += len;
            m_lSizeCur += len;
        }
        BYTE* GetBuffer()
        {
            return m_pData;
        }

        void Clear()
        {
            if (m_pData) free(m_pData);

            m_pData = NULL;
            m_lSize = 0;

            m_pDataCur = m_pData;
            m_lSizeCur = 0;
        }
        void ClearWithoutAttack()
        {
            m_pData = NULL;
            m_lSize = 0;

            m_pDataCur = m_pData;
            m_lSizeCur = 0;
        }
        void ClearNoAttack()
        {
            m_pDataCur = m_pData;
            m_lSizeCur = 0;
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

    class CHChar
    {
    public:
        int 	unicode;	// юникодное значение
        int 	gid;		// индекс глифа в файле
        double	x;			// сдвиг по baseline
        double	width;		// ширина символа (сдвиг до след буквы)
        double* matrix;		// матрица преобразования (!!! без сдвига)

    public:
        CHChar()
        {
            unicode = 0;
            gid = 0;
            width = 0;
            matrix = NULL;
        }
        CHChar(const CHChar& oSrc)
        {
            *this = oSrc;
        }
        CHChar& operator=(const CHChar& oSrc)
        {
            unicode	= oSrc.unicode;
            gid		= oSrc.gid;
            width	= oSrc.width;
            matrix	= NULL;
            if (NULL != oSrc.matrix)
            {
                matrix = new double[4];
                memcpy(matrix, oSrc.matrix, 4 * sizeof(double));
            }
            return *this;
        }
        ~CHChar()
        {
            RELEASEARRAYOBJECTS(matrix);
        }

        inline void Clear()
        {
            unicode = 0;
            gid = 0;
            width = 0;

            RELEASEARRAYOBJECTS(matrix);
        }
    };

    class CHLine
    {
    public:
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
        CHChar*	m_pChars;
        LONG m_lSizeChars;
        LONG m_lCharsTail;

        bool m_bIsSetUpTransform;
        double m_sx;
        double m_sy;
        double m_shx;
        double m_shy;

    public:
        CHLine()
        {
            m_dAscent	= 0;
            m_dDescent	= 0;
            m_dX		= 0;
            m_dY		= 0;

            m_dK		= 0;
            m_dB		= 0;
            m_bIsConstX	= false;

            m_ex		= 0;
            m_ey		= 0;

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
            m_dAscent	= oLine.m_dAscent;
            m_dDescent	= oLine.m_dDescent;
            m_dX		= oLine.m_dX;
            m_dY		= oLine.m_dY;

            m_dK		= oLine.m_dK;
            m_dB		= oLine.m_dB;

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
            m_dAscent	= 0;
            m_dDescent	= 0;
            m_dX		= 0;
            m_dY		= 0;

            m_dK		= 0;
            m_dB		= 0;
            m_bIsConstX	= false;

            m_ex		= 0;
            m_ey		= 0;

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
                {
                    pNews[i] = m_pChars[i];
                }

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
            if (0 == m_lCharsTail)
                return NULL;

            return &m_pChars[m_lCharsTail - 1];
        }

        inline LONG GetCountChars()
        {
            return m_lCharsTail;
        }
    };
}

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
}

#endif // _WASM_SERIALIZE_H
