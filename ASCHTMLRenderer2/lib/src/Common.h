#ifndef _ASC_HTMLRENDERER_COMMON_H_
#define _ASC_HTMLRENDERER_COMMON_H_

#include "../../../DesktopEditor/common/Types.h"
#include "../../../DesktopEditor/common/File.h"
#include "../../../DesktopEditor/common/Directory.h"
#include "../../../DesktopEditor/raster/BgraFrame.h"
#include "../../../DesktopEditor/graphics/Matrix.h"
#include "../../../DesktopEditor/graphics/structures.h"
#include "../../../DesktopEditor/fontengine/ApplicationFonts.h"
#include "../../../DesktopEditor/common/StringBuilder.h"
#include "../../../DesktopEditor/graphics/IRenderer.h"
#include "../../../DesktopEditor/xml/include/xmlutils.h"

namespace NSHtmlRenderer
{
    class IBaseMatrixUpdater
    {
    public:
        virtual void OnBaseMatrixUpdate(const double& dWidth, const double& dHeight) = 0;
    };

    class CDstInfo
    {
    public:
        std::wstring m_strDstFilePath;
        std::wstring m_strAdditionalPath;
        std::wstring m_strDstMedia;
        bool	m_bIsWeb;

    public:
        CDstInfo()
        {
            m_strDstFilePath	= L"";
            m_strAdditionalPath = L"";
            m_strDstMedia		= L"";
            m_bIsWeb			= false;
        }
        CDstInfo(const CDstInfo& oInfo)
        {
            *this = oInfo;
        }
        CDstInfo& operator=(const CDstInfo& oSrc)
        {
            m_strDstFilePath	= oSrc.m_strDstFilePath;
            m_strAdditionalPath = oSrc.m_strAdditionalPath;
            m_strDstMedia		= oSrc.m_strDstMedia;
            m_bIsWeb			= oSrc.m_bIsWeb;
            return *this;
        }
    };
}

namespace NSHtmlRenderer
{
    inline LONG ConvertColor(const LONG& lBGR)
	{
		return (0x00FFFFFF & (((lBGR & 0xFF) << 16) | (lBGR & 0x0000FF00) | ((lBGR >> 16) & 0xFF)));
	}

    inline void WriteIntsToStringBuilder(const int& n1, const int& n2, NSStringUtils::CStringBuilder* pBuilder)
    {
        pBuilder->AddSize(21);
        pBuilder->AddIntNoCheck(n1);
        pBuilder->AddCharNoSafe(',');
        pBuilder->AddIntNoCheck(n2);
    }
    inline void WriteIntsToStringBuilder(const int& n1, const int& n2,
                                    const int& n3, const int& n4,
                                    NSStringUtils::CStringBuilder* pBuilder)
    {
        pBuilder->AddSize(65);
        pBuilder->AddIntNoCheck(n1);
        pBuilder->AddCharNoSafe(',');
        pBuilder->AddIntNoCheck(n2);
        pBuilder->AddCharNoSafe(',');
        pBuilder->AddIntNoCheck(n3);
        pBuilder->AddCharNoSafe(',');
        pBuilder->AddIntNoCheck(n4);
    }
    inline void WriteIntsToStringBuilder(const int& n1, const int& n2,
                                    const int& n3, const int& n4,
                                    const int& n5,
                                    NSStringUtils::CStringBuilder* pBuilder)
    {
        pBuilder->AddSize(65);
        pBuilder->AddIntNoCheck(n1);
        pBuilder->AddCharNoSafe(',');
        pBuilder->AddIntNoCheck(n2);
        pBuilder->AddCharNoSafe(',');
        pBuilder->AddIntNoCheck(n3);
        pBuilder->AddCharNoSafe(',');
        pBuilder->AddIntNoCheck(n4);
        pBuilder->AddCharNoSafe(',');
        pBuilder->AddIntNoCheck(n5);
    }
    inline void WriteIntsToStringBuilder(const int& n1, const int& n2,
                                    const int& n3, const int& n4,
                                    const int& n5, const int& n6,
                                    NSStringUtils::CStringBuilder* pBuilder)
    {
        pBuilder->AddSize(65);
        pBuilder->AddIntNoCheck(n1);
        pBuilder->AddCharNoSafe(',');
        pBuilder->AddIntNoCheck(n2);
        pBuilder->AddCharNoSafe(',');
        pBuilder->AddIntNoCheck(n3);
        pBuilder->AddCharNoSafe(',');
        pBuilder->AddIntNoCheck(n4);
        pBuilder->AddCharNoSafe(',');
        pBuilder->AddIntNoCheck(n5);
        pBuilder->AddCharNoSafe(',');
        pBuilder->AddIntNoCheck(n6);
    }

    inline void WriteFormatted(const wchar_t* s1, const int& n1,
                        const wchar_t* s2,
                        NSStringUtils::CStringBuilder* pBuilder)
    {
        pBuilder->WriteString(s1, wcslen(s1));
        pBuilder->AddInt(n1);
        pBuilder->WriteString(s2, wcslen(s2));
    }
    inline void WriteFormatted(const wchar_t* s1, const int& n1,
                        const wchar_t* s2, const int& n2,
                        const wchar_t* s3,
                        NSStringUtils::CStringBuilder* pBuilder)
    {
        pBuilder->WriteString(s1, wcslen(s1));
        pBuilder->AddInt(n1);
        pBuilder->WriteString(s2, wcslen(s2));
        pBuilder->AddInt(n2);
        pBuilder->WriteString(s3, wcslen(s3));
    }
    inline void WriteFormatted(const wchar_t* s1, const int& n1,
                        const wchar_t* s2, const int& n2,
                        const wchar_t* s3, const int& n3,
                        const wchar_t* s4,
                        NSStringUtils::CStringBuilder* pBuilder)
    {
        pBuilder->WriteString(s1, wcslen(s1));
        pBuilder->AddInt(n1);
        pBuilder->WriteString(s2, wcslen(s2));
        pBuilder->AddInt(n2);
        pBuilder->WriteString(s3, wcslen(s3));
        pBuilder->AddInt(n3);
        pBuilder->WriteString(s4, wcslen(s4));
    }
    inline void WriteFormatted(const wchar_t* s1, const int& n1,
                        const wchar_t* s2, const int& n2,
                        const wchar_t* s3, const int& n3,
                        const wchar_t* s4, const int& n4,
                        const wchar_t* s5,
                        NSStringUtils::CStringBuilder* pBuilder)
    {
        pBuilder->WriteString(s1, wcslen(s1));
        pBuilder->AddInt(n1);
        pBuilder->WriteString(s2, wcslen(s2));
        pBuilder->AddInt(n2);
        pBuilder->WriteString(s3, wcslen(s3));
        pBuilder->AddInt(n3);
        pBuilder->WriteString(s4, wcslen(s4));
        pBuilder->AddInt(n4);
        pBuilder->WriteString(s5, wcslen(s5));
    }

    inline void SetStringColor(LONG lBGR, NSStringUtils::CStringBuilder* pBuilder)
    {
        BYTE R = (BYTE)(lBGR & 0xFF);
        BYTE G = (BYTE)((lBGR >> 8) & 0xFF);
        BYTE B = (BYTE)((lBGR >> 16) & 0xFF);

        pBuilder->AddSize(50);
        pBuilder->WriteString(L"rgb(", 4);
        pBuilder->AddIntNoCheck(R);
        pBuilder->AddCharNoSafe(',');
        pBuilder->AddIntNoCheck(G);
        pBuilder->AddCharNoSafe(',');
        pBuilder->AddIntNoCheck(B);
        pBuilder->AddCharSafe(')');
    }
    inline void SetStrokeColor(LONG lBGR, LONG lA, NSStringUtils::CStringBuilder* pBuilder)
    {
        BYTE R = (BYTE)(lBGR & 0xFF);
        BYTE G = (BYTE)((lBGR >> 8) & 0xFF);
        BYTE B = (BYTE)((lBGR >> 16) & 0xFF);

        pBuilder->WriteString(L"c.strokeStyle = \"rgba(", 22);
        pBuilder->AddSize(70);
        pBuilder->AddIntNoCheck(R);
        pBuilder->AddCharNoSafe(',');
        pBuilder->AddIntNoCheck(G);
        pBuilder->AddCharNoSafe(',');
        pBuilder->AddIntNoCheck(B);
        pBuilder->AddCharNoSafe(',');
        pBuilder->AddIntNoCheckDel100(100 * lA / 255);
        pBuilder->WriteString(L")\";\n", 4);
    }
    inline void SetFillColor(LONG lBGR, LONG lA, NSStringUtils::CStringBuilder* pBuilder)
    {
        BYTE R = (BYTE)(lBGR & 0xFF);
        BYTE G = (BYTE)((lBGR >> 8) & 0xFF);
        BYTE B = (BYTE)((lBGR >> 16) & 0xFF);

        pBuilder->WriteString(L"c.fillStyle = \"rgba(", 20);
        pBuilder->AddSize(70);
        pBuilder->AddIntNoCheck(R);
        pBuilder->AddCharNoSafe(',');
        pBuilder->AddIntNoCheck(G);
        pBuilder->AddCharNoSafe(',');
        pBuilder->AddIntNoCheck(B);
        pBuilder->AddCharNoSafe(',');
        pBuilder->AddIntNoCheckDel100(100 * lA / 255);
        pBuilder->WriteString(L")\";\n", 4);
    }

    struct RECT
    {
        LONG left;
        LONG top;
        LONG right;
        LONG bottom;
    };

    inline bool IsEqualMain(const Aggplus::CMatrix* pMatrix, const Aggplus::CMatrix* pMatrix2)
    {
        if (fabs(pMatrix->m_agg_mtx.sx  - pMatrix2->m_agg_mtx.sx) < 0.001 &&
            fabs(pMatrix->m_agg_mtx.sy  - pMatrix2->m_agg_mtx.sy) < 0.001 &&
            fabs(pMatrix->m_agg_mtx.shx - pMatrix2->m_agg_mtx.shx) < 0.001 &&
            fabs(pMatrix->m_agg_mtx.shy - pMatrix2->m_agg_mtx.shy) < 0.001)
            return true;
        return false;
    }

    static RECT GetImageBounds(CBgraFrame* pFrame)
    {
        BYTE* pBuffer = pFrame->get_Data();
        LONG lWidth = (LONG)pFrame->get_Width();
        LONG lHeight = (LONG)pFrame->get_Height();

        RECT rect;
        rect.left	= 0;
        rect.top	= 0;
        rect.right	= lWidth - 1;
        rect.bottom = lHeight - 1;

        // top
        unsigned int* pData = (unsigned int*)pBuffer;
        for (; rect.top < lHeight; rect.top++)
        {
            bool bIsBreak = false;
            for (LONG i = 0; i < lWidth; ++i, ++pData)
            {
                if (*pData != 0x00)
                {
                    bIsBreak = true;
                    break;
                }
            }
            if (bIsBreak)
                break;
        }
        if (rect.top >= lHeight)
            rect.top = (lHeight - 1);

        // bottom
        for (; rect.bottom >= rect.top; rect.bottom--)
        {
            pData = (unsigned int*)pBuffer;
            pData += (lWidth * rect.bottom);

            bool bIsBreak = false;
            for (LONG i = 0; i < lWidth; ++i, ++pData)
            {
                if (*pData != 0x00)
                {
                    bIsBreak = true;
                    break;
                }
            }
            if (bIsBreak)
                break;
        }
        if (rect.bottom < rect.top)
            rect.bottom = rect.top;

        LONG lDelta = rect.bottom - rect.top + 1;

        // left
        for (; rect.left < lWidth; rect.left++)
        {
            pData = (unsigned int*)(pBuffer + 4 * lWidth * rect.top);
            pData += rect.left;

            bool bIsBreak = false;
            for (LONG i = 0; i < lDelta; ++i, pData += lWidth)
            {
                if (*pData != 0x00)
                {
                    bIsBreak = true;
                    break;
                }
            }
            if (bIsBreak)
                break;
        }
        if (rect.left >= lWidth)
            rect.left = lWidth - 1;

        // right
        for (; rect.right >= rect.left; rect.right--)
        {
            pData = (unsigned int*)(pBuffer + 4 * lWidth * rect.top);
            pData += rect.right;

            bool bIsBreak = false;
            for (LONG i = 0; i < lDelta; ++i, pData += lWidth)
            {
                if (*pData != 0x00)
                {
                    bIsBreak = true;
                    break;
                }
            }
            if (bIsBreak)
                break;
        }
        if (rect.right < rect.left)
            rect.right = rect.left;

        return rect;
    }

    static RECT GetImageBounds2(CBgraFrame* pFrame, BYTE* pCache)
    {
        BYTE* pBuffer = pFrame->get_Data();
        LONG lWidth = (LONG)pFrame->get_Width();
        LONG lHeight = (LONG)pFrame->get_Height();

        RECT rect;
        rect.left	= 0;
        rect.top	= 0;
        rect.right	= lWidth - 1;
        rect.bottom = lHeight - 1;

        // top
        unsigned int** pData = (unsigned int**)pBuffer;
        unsigned int** pDataSrc = (unsigned int**)pCache;
        for (; rect.top < lHeight; rect.top++)
        {
            if (0 != memcmp((void*)pData, (void*)pDataSrc, 4 * lWidth))
                break;

            pData += lWidth;
            pDataSrc += lWidth;
        }
        if (rect.top >= lHeight)
            rect.top = (lHeight - 1);

        // bottom
        for (; rect.bottom >= rect.top; rect.bottom--)
        {
            pData = (unsigned int**)pBuffer;
            pData += (lWidth * rect.bottom);
            pDataSrc = (unsigned int**)pCache;
            pDataSrc += (lWidth * rect.bottom);

            if (0 != memcmp((void*)pData, (void*)pDataSrc, 4 * lWidth))
                break;
        }
        if (rect.bottom < rect.top)
            rect.bottom = rect.top;

        LONG lDelta = rect.bottom - rect.top + 1;

        // left
        for (; rect.left < lWidth; rect.left++)
        {
            pData = (unsigned int**)(pBuffer + 4 * lWidth * rect.top);
            pData += rect.left;
            pDataSrc = (unsigned int**)(pCache + 4 * lWidth * rect.top);
            pDataSrc += rect.left;

            bool bIsBreak = false;
            for (LONG i = 0; i < lDelta; ++i, pData += lWidth, pDataSrc += lWidth)
            {
                if (*pData != *pDataSrc)
                {
                    bIsBreak = true;
                    break;
                }
            }
            if (bIsBreak)
                break;
        }
        if (rect.left >= lWidth)
            rect.left = lWidth - 1;

        // right
        for (; rect.right >= rect.left; rect.right--)
        {
            pData = (unsigned int**)(pBuffer + 4 * lWidth * rect.top);
            pData += rect.right;

            pDataSrc = (unsigned int**)(pCache + 4 * lWidth * rect.top);
            pDataSrc += rect.right;

            bool bIsBreak = false;
            for (LONG i = 0; i < lDelta; ++i, pData += lWidth)
            {
                if (*pData != *pDataSrc)
                {
                    bIsBreak = true;
                    break;
                }
            }
            if (bIsBreak)
                break;
        }
        if (rect.right < rect.left)
            rect.right = rect.left;

        return rect;
    }

	const double c_ag_Inch_to_MM	= 25.4;
	const double c_ag_1pxWidth		= 25.4 / 96;

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

    inline double FABS(double dVal)
	{
		return (dVal >= 0) ? dVal : -dVal;
	}
    inline int round(double dVal)
	{
		return (int)(dVal + 0.5);
    }
}

namespace NSHtmlRenderer
{
    class CMetafile
    {
    public:
        enum CommandType
        {
            // pen
            ctPenXML						= 0,
            ctPenColor						= 1,
            ctPenAlpha						= 2,
            ctPenSize						= 3,
            ctPenDashStyle					= 4,
            ctPenLineStartCap				= 5,
            ctPenLineEndCap					= 6,
            ctPenLineJoin					= 7,
            ctPenDashPatern					= 8,
            ctPenDashPatternCount			= 9,
            ctPenDashOffset					= 10,
            ctPenAlign						= 11,
            ctPenMiterLimit					= 12,

            // brush
            ctBrushXML						= 20,
            ctBrushType						= 21,
            ctBrushColor1					= 22,
            ctBrushColor2					= 23,
            ctBrushAlpha1					= 24,
            ctBrushAlpha2					= 25,
            ctBrushTexturePath				= 26,
            ctBrushTextureAlpha				= 27,
            ctBrushTextureMode				= 28,
            ctBrushRectable					= 29,
            ctBrushAngle                    = 30,
            ctBrushSubColors                = 31,

            // font
            ctFontXML						= 40,
            ctFontName						= 41,
            ctFontSize						= 42,
            ctFontStyle						= 43,
            ctFontPath						= 44,
            ctFontGID						= 45,
            ctFontCharSpace					= 46,

            // shadow
            ctShadowXML						= 50,
            ctShadowVisible					= 51,
            ctShadowDistanceX				= 52,
            ctShadowDistanceY				= 53,
            ctShadowBlurSize				= 54,
            ctShadowColor					= 55,
            ctShadowAlpha					= 56,

            // edge
            ctEdgeXML						= 70,
            ctEdgeVisible					= 71,
            ctEdgeDistance					= 72,
            ctEdgeColor						= 73,
            ctEdgeAlpha						= 74,

            // text
            ctDrawText						= 80,
            ctDrawTextEx					= 81,

            // pathcommands
            ctPathCommandMoveTo				= 91,
            ctPathCommandLineTo				= 92,
            ctPathCommandLinesTo			= 93,
            ctPathCommandCurveTo			= 94,
            ctPathCommandCurvesTo			= 95,
            ctPathCommandArcTo				= 96,
            ctPathCommandClose				= 97,
            ctPathCommandEnd				= 98,
            ctDrawPath						= 99,
            ctPathCommandStart				= 100,
            ctPathCommandGetCurrentPoint	= 101,
            ctPathCommandText				= 102,
            ctPathCommandTextEx				= 103,

            // image
            ctDrawImage						= 110,
            ctDrawImageFromFile				= 111,

            ctSetParams						= 120,

            ctBeginCommand					= 121,
            ctEndCommand					= 122,

            ctSetTransform					= 130,
            ctResetTransform				= 131,

            ctClipMode						= 140,

            ctCommandLong1					= 150,
            ctCommandDouble1				= 151,
            ctCommandString1				= 152,
            ctCommandLong2					= 153,
            ctCommandDouble2				= 154,
            ctCommandString2				= 155,

            ctCommandTextLine				= 160,
            ctCommandTextTransform			= 161,
            ctCommandTextLineEnd			= 162,

            ctCommandTextClipRectReset		= 163,
            ctCommandTextClipRect			= 164,

            ctError							= 255
        };

    private:

        // сам метафайл
        BYTE* m_pBuffer;
        BYTE* m_pBufferMem;

        size_t m_lPosition;
        size_t m_lSize;

        LONG m_lSizeofDouble;
        LONG m_lSizeofFloat;
        LONG m_lSizeofLONG;
        LONG m_lSizeofBYTE;

    public:
        double m_fWidth;
        double m_fHeight;

    public:
        CMetafile()
        {
            Clear();

            m_lSizeofDouble	= sizeof(double);
            m_lSizeofFloat 	= sizeof(float);
            m_lSizeofLONG	= sizeof(int);
            m_lSizeofBYTE   = sizeof(BYTE);
        }
        ~CMetafile()
        {
            RELEASEARRAYOBJECTS(m_pBuffer);
        }

    public:

        inline LONG GetPosition()
        {
            return (LONG)m_lPosition;
        }
        inline BYTE* GetData()
        {
            return m_pBuffer;
        }
        inline void ClearNoAttack()
        {
            m_lPosition = 0;
            m_pBufferMem = m_pBuffer;
        }

        inline void Clear()
        {
            m_lSize		= 0;
            m_lPosition = 0;

            m_pBuffer = NULL;
            m_pBufferMem = NULL;
        }
        inline void Seek(LONG lPos)
        {
            m_lPosition = (size_t)lPos;
            m_pBufferMem = m_pBuffer + m_lPosition;
        }

        inline void CheckBufferSize(size_t lPlus)
        {
            if (NULL != m_pBuffer)
            {
                size_t nNewSize = m_lPosition + lPlus;

                if (nNewSize >= m_lSize)
                {
                    while (nNewSize >= m_lSize)
                    {
                        m_lSize *= 2;
                    }

                    BYTE* pNew = new BYTE[m_lSize];
                    memcpy(pNew, m_pBuffer, m_lPosition);

                    RELEASEARRAYOBJECTS(m_pBuffer);
                    m_pBuffer = pNew;
                }
            }
            else
            {
                m_lSize		= 1000;
                m_pBuffer	= new BYTE[m_lSize];

                CheckBufferSize(lPlus);
            }
        }

        inline void WriteCommandType(const CommandType& eType)
        {
            CheckBufferSize(m_lSizeofBYTE);

            *(m_pBuffer + m_lPosition)	= (BYTE)eType;
            m_lPosition += m_lSizeofBYTE;
        }

        //
        inline void WriteBYTE_nocheck(const BYTE& lValue)
        {
            *(m_pBuffer + m_lPosition)	= lValue;
            m_lPosition += m_lSizeofBYTE;
        }
        inline void WriteLONG_nocheck(const int& lValue)
        {
            *((int*)(m_pBuffer + m_lPosition))	= lValue;
            m_lPosition += m_lSizeofLONG;
        }
        inline void WriteUSHORT_nocheck(const USHORT& lValue)
        {
            *((USHORT*)(m_pBuffer + m_lPosition)) = lValue;
            m_lPosition += sizeof(USHORT);
        }
        inline void WriteWCHAR_nocheck(const WCHAR& lValue)
        {
            *((WCHAR*)(m_pBuffer + m_lPosition))	= lValue;
            m_lPosition += sizeof(WCHAR);
        }
        inline void WriteWCHAR_nocheck2(const int& lValue)
        {
            if (lValue < 0x10000)
            {
                *((USHORT*)(m_pBuffer + m_lPosition)) = lValue;
                m_lPosition += 2;
            }
            else
            {
                int code = lValue - 0x10000;
                *((USHORT*)(m_pBuffer + m_lPosition))      = 0xD800 | ((code >> 10) & 0x03FF);
                *((USHORT*)(m_pBuffer + m_lPosition + 2))  = 0xDC00 | (code & 0x03FF);
                m_lPosition += 4;
            }
        }
        inline void WriteDouble_nocheck(const double& dValue)
        {
            // здесь никаких даблов. Сплошные округления
            LONG lValue = (LONG)(dValue * 10000);
            WriteLONG_nocheck(lValue);
            return;

            CheckBufferSize(m_lSizeofDouble);

            *((double*)(m_pBuffer + m_lPosition)) = dValue;
            m_lPosition += m_lSizeofDouble;
        }
        inline void WriteDouble2_nocheck(const double& dValue)
        {
            // здесь никаких даблов. Сплошные округления
            SHORT lValue = (SHORT)(dValue * 100);
            *((SHORT*)(m_pBuffer + m_lPosition)) = lValue;
            m_lPosition += sizeof(SHORT);
        }
        //
        inline void WriteBYTE(const BYTE& lValue)
        {
            CheckBufferSize(m_lSizeofBYTE);

            *(m_pBuffer + m_lPosition)	= lValue;
            m_lPosition += m_lSizeofBYTE;
        }
        inline void WriteLONG(const int& lValue)
        {
            CheckBufferSize(m_lSizeofLONG);

            *((int*)(m_pBuffer + m_lPosition))	= lValue;
            m_lPosition += m_lSizeofLONG;
        }
        inline void WriteUSHORT(const USHORT& lValue)
        {
            CheckBufferSize(sizeof(USHORT));

            *((USHORT*)(m_pBuffer + m_lPosition)) = lValue;
            m_lPosition += sizeof(USHORT);
        }
        inline void WriteWCHAR(const WCHAR& lValue)
        {
            CheckBufferSize(sizeof(WCHAR));

            *((WCHAR*)(m_pBuffer + m_lPosition))	= lValue;
            m_lPosition += sizeof(WCHAR);
        }
        inline void WriteDouble(const double& dValue)
        {
            // здесь никаких даблов. Сплошные округления
            int lValue = (int)(dValue * 10000);
            WriteLONG(lValue);
            return;

            CheckBufferSize(m_lSizeofDouble);

            *((double*)(m_pBuffer + m_lPosition)) = dValue;
            m_lPosition += m_lSizeofDouble;
        }
        inline void WriteDouble2(const double& dValue)
        {
            // здесь никаких даблов. Сплошные округления
            SHORT lValue = (SHORT)(dValue * 100);
            CheckBufferSize(sizeof(SHORT));

            *((SHORT*)(m_pBuffer + m_lPosition)) = lValue;
            m_lPosition += sizeof(SHORT);
        }
        inline void WriteFloat(const float& fValue)
        {
            CheckBufferSize(m_lSizeofFloat);

            *((float*)(m_pBuffer + m_lPosition))	= fValue;
            m_lPosition += m_lSizeofFloat;
        }
        inline void WriteString(wchar_t* bstrValue)
        {
            int lSize = (int)wcslen(bstrValue) + 1;

            int lSizeMem = lSize * sizeof(wchar_t);

            CheckBufferSize(m_lSizeofLONG + lSizeMem);

            *((int*)(m_pBuffer + m_lPosition))	= lSizeMem;
            m_lPosition += m_lSizeofLONG;

            memcpy(m_pBuffer + m_lPosition, bstrValue, lSizeMem);
            m_lPosition += lSizeMem;
        }

        inline void WriteBYTE(const CommandType& eType, const BYTE& lValue)
        {
            CheckBufferSize(2 * m_lSizeofBYTE);

            *(m_pBuffer + m_lPosition)	= (BYTE)eType;
            m_lPosition += m_lSizeofBYTE;

            *(m_pBuffer + m_lPosition)	= lValue;
            m_lPosition += m_lSizeofBYTE;
        }
        inline void WriteLONG(const CommandType& eType, const int& lValue)
        {
            CheckBufferSize(m_lSizeofBYTE + m_lSizeofLONG);

            *(m_pBuffer + m_lPosition)	= (BYTE)eType;
            m_lPosition += m_lSizeofBYTE;

            *((int*)(m_pBuffer + m_lPosition))	= lValue;
            m_lPosition += m_lSizeofLONG;
        }
        inline void WriteDouble(const CommandType& eType, const double& dValue)
        {
            CheckBufferSize(m_lSizeofBYTE + m_lSizeofDouble);

            *(m_pBuffer + m_lPosition)	= (BYTE)eType;
            m_lPosition += m_lSizeofBYTE;

            *((double*)(m_pBuffer + m_lPosition)) = dValue;
            m_lPosition += m_lSizeofDouble;
        }
        inline void WriteFloat(const CommandType& eType, const float& fValue)
        {
            CheckBufferSize(m_lSizeofBYTE + m_lSizeofFloat);

            *(m_pBuffer + m_lPosition)	= (BYTE)eType;
            m_lPosition += m_lSizeofBYTE;

            *((float*)(m_pBuffer + m_lPosition))	= fValue;
            m_lPosition += m_lSizeofFloat;
        }
        inline void WriteString(const CommandType& eType, wchar_t* bstrValue)
        {
            int lSize = (int)wcslen(bstrValue) + 1;

            int lSizeMem = lSize * sizeof(wchar_t);

            CheckBufferSize(m_lSizeofBYTE + m_lSizeofLONG + lSizeMem);

            *(m_pBuffer + m_lPosition)	= (BYTE)eType;
            m_lPosition += m_lSizeofBYTE;

            *((int*)(m_pBuffer + m_lPosition))	= lSizeMem;
            m_lPosition += m_lSizeofLONG;

            memcpy(m_pBuffer + m_lPosition, bstrValue, lSizeMem);
            m_lPosition += lSizeMem;
        }

        inline void Write(const BYTE* pData, const LONG& lLen)
        {
            CheckBufferSize((size_t)lLen);
            memcpy(m_pBuffer + m_lPosition, pData, lLen);
            m_lPosition += lLen;
        }

        inline void Write(const CommandType& eCommand, const double& f1, const double& f2)
        {
            size_t lMem = m_lSizeofBYTE + 2 * m_lSizeofDouble;

            CheckBufferSize(lMem);

            *(m_pBuffer + m_lPosition)			= (BYTE)eCommand;	m_lPosition += m_lSizeofBYTE;
            *((double*)(m_pBuffer + m_lPosition))= f1;				m_lPosition += m_lSizeofDouble;
            *((double*)(m_pBuffer + m_lPosition))= f2;				m_lPosition += m_lSizeofDouble;
        }

        inline void Write(const CommandType& eCommand, const double& f1, const double& f2, const double& f3, const double& f4, const double& f5, const double& f6)
        {
            size_t lMem = m_lSizeofBYTE + 6 * m_lSizeofDouble;

            CheckBufferSize(lMem);

            *(m_pBuffer + m_lPosition)	= (BYTE)eCommand;	m_lPosition += m_lSizeofBYTE;
            *((double*)(m_pBuffer + m_lPosition))= f1;		m_lPosition += m_lSizeofDouble;
            *((double*)(m_pBuffer + m_lPosition))= f2;		m_lPosition += m_lSizeofDouble;
            *((double*)(m_pBuffer + m_lPosition))= f3;		m_lPosition += m_lSizeofDouble;
            *((double*)(m_pBuffer + m_lPosition))= f4;		m_lPosition += m_lSizeofDouble;
            *((double*)(m_pBuffer + m_lPosition))= f5;		m_lPosition += m_lSizeofDouble;
            *((double*)(m_pBuffer + m_lPosition))= f6;		m_lPosition += m_lSizeofDouble;
        }

        inline void Write(const CommandType& eCommand, const int& lCount, float* pData)
        {
            size_t lFloats = lCount * m_lSizeofFloat;
            size_t lMem = m_lSizeofBYTE + m_lSizeofLONG + lFloats;

            CheckBufferSize(lMem);

            *(m_pBuffer + m_lPosition)	= (BYTE)eCommand;	m_lPosition += m_lSizeofBYTE;
            *((int*)(m_pBuffer + m_lPosition))	= lCount;	m_lPosition += m_lSizeofLONG;

            memcpy(m_pBuffer + m_lPosition, pData, lFloats);
            m_lPosition += lFloats;
        }
        inline void Write(const CommandType& eCommand, const int& lCount, double* pData)
        {
            size_t lFloats = lCount * m_lSizeofDouble;
            size_t lMem = m_lSizeofBYTE + m_lSizeofLONG + lFloats;

            CheckBufferSize(lMem);

            *(m_pBuffer + m_lPosition)	= (BYTE)eCommand;	m_lPosition += m_lSizeofBYTE;
            *((int*)(m_pBuffer + m_lPosition))	= lCount;	m_lPosition += m_lSizeofLONG;

            memcpy(m_pBuffer + m_lPosition, pData, lFloats);
            m_lPosition += lFloats;
        }

        inline void Write(CMetafile& oMeta)
        {
            LONG lPos = oMeta.GetPosition();
            CheckBufferSize(lPos);

            memcpy(m_pBuffer + m_lPosition, oMeta.GetData(), lPos);
            m_lPosition += lPos;
        }
    };
}

#endif // _ASC_HTMLRENDERER_COMMON_H_
