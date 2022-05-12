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
#include "../include/HTMLRendererText.h"
#include "./Text.h"

namespace NSHtmlRenderer
{
    class CHTMLRendererText_Private
    {
    public:
        double m_dWidth;
        double m_dHeght;

        NSStructures::CBrush m_oBrush;
        NSStructures::CBrush m_oLastBrush;

        NSStructures::CFont m_oFont;
        NSStructures::CFont* m_pFont;
        NSStructures::CFont	m_oInstalledFont;
        LONG m_lCurrentFont;
        double m_dCurrentFontSize;

        Aggplus::CMatrix m_oTransform;
        Aggplus::CMatrix m_oLastTransform;

        bool m_bIsChangedFontParamBetweenDrawText;
        LONG m_lCurrentCommandType;
        LONG m_lSrcFileType;

        CHText m_oSmartText;
        CMetafile m_oPage;

        int* m_pTempUnicodes;
        int  m_nTempUnicodesAlloc;
        int  m_nTempUnicodesLen;

        bool m_bIsFontsInit;

    public:
        CHTMLRendererText_Private()
        {
            m_bIsChangedFontParamBetweenDrawText = true;
            m_lSrcFileType = 0;
            m_pTempUnicodes = NULL;
            m_nTempUnicodesLen = 0;
            m_nTempUnicodesAlloc = 0;

            m_bIsFontsInit = false;

            m_lCurrentFont = 0;
            m_dCurrentFontSize = 0;
            m_pFont = &m_oFont;
        }
        ~CHTMLRendererText_Private()
        {
            RELEASEARRAYOBJECTS(m_pTempUnicodes);
        }

    public:
        void GetUnicodes(const std::wstring& sText)
        {
            int nLen = (int)sText.length();
            if (nLen > m_nTempUnicodesAlloc)
            {
                RELEASEARRAYOBJECTS(m_pTempUnicodes);
                m_nTempUnicodesAlloc = nLen;
                m_pTempUnicodes = new int[m_nTempUnicodesAlloc];
            }

            m_nTempUnicodesLen = 0;
            const wchar_t* pWchars = sText.c_str();

            if (sizeof(wchar_t) == 2)
            {
                for (int nIndex = 0, nGlyphIndex = 0; nIndex < nLen; ++nIndex, ++nGlyphIndex)
                {
                    int code = (int)pWchars[nIndex];
                    if (code >= 0xD800 && code <= 0xDFFF && (nIndex + 1) < nLen)
                    {
                        ++nIndex;
                        code = 0x10000 + (((code & 0x3FF) << 10) | (0x03FF & pWchars[nIndex]));
                    }

                    m_pTempUnicodes[m_nTempUnicodesLen++] = code;
                }
            }
            else
            {
                for ( int nIndex = 0; nIndex < nLen; ++nIndex )
                {
                    m_pTempUnicodes[m_nTempUnicodesLen++] = (int)pWchars[nIndex];
                }
            }
        }

        void WriteText(const int* pUnicodes, const int* pGids, const int& nCount, const double& x, const double& y,
                      const double& width, const double& height, const bool& bIsChangedFontParamBetweenDrawText)
        {
            bool bIsDumpFont = false;
            if (!m_oInstalledFont.IsEqual(&m_oFont))
            {
                m_oInstalledFont = m_oFont;
                bIsDumpFont = true;

                m_dCurrentFontSize = m_oInstalledFont.Size;
            }

            m_oSmartText.CommandText(pUnicodes, pGids, nCount, x, y, width, height, bIsDumpFont, this);
        }
    };

    CHTMLRendererText::CHTMLRendererText()
    {
        m_pInternal = new CHTMLRendererText_Private();
    }
    CHTMLRendererText::~CHTMLRendererText()
    {
        RELEASEOBJECT(m_pInternal);
    }

    void CHTMLRendererText::Init(IOfficeDrawingFile* pFile, int nCacheSize)
    {
        m_pInternal->m_oBrush.SetDefaultParams();
        m_pInternal->m_oLastBrush.SetDefaultParams();

        m_pInternal->m_oFont.SetDefaultParams();
        m_pInternal->m_oInstalledFont.SetDefaultParams();
        m_pInternal->m_oInstalledFont.Name = L"";

        m_pInternal->m_oTransform.Reset();
        m_pInternal->m_oLastTransform.Reset();

        m_pInternal->m_oLastBrush.Color1 = -1;
        m_pInternal->m_dCurrentFontSize	= 0.0;

        m_pInternal->m_bIsChangedFontParamBetweenDrawText = false;
        m_pInternal->m_lCurrentCommandType = -1;

        m_pInternal->m_oSmartText.NewPage();

        if (!m_pInternal->m_bIsFontsInit)
        {
            m_pInternal->m_oSmartText.m_oFontManager.m_pFont = &m_pInternal->m_oFont;

            m_pInternal->m_oSmartText.m_pLastBrush = &m_pInternal->m_oLastBrush;
            m_pInternal->m_oSmartText.m_pBrush = &m_pInternal->m_oBrush;

            m_pInternal->m_oSmartText.m_pFont = &m_pInternal->m_oFont;

            m_pInternal->m_oSmartText.m_pTransform = &m_pInternal->m_oTransform;
            m_pInternal->m_oSmartText.m_pLastTransform = &m_pInternal->m_oLastTransform;

            m_pInternal->m_oSmartText.m_pPageMeta = &m_pInternal->m_oPage;

            OfficeDrawingFileType eType = pFile->GetType();
            switch (eType)
            {
                case odftPDF:
                {
                    m_pInternal->m_lSrcFileType = AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_PDF;
                    m_pInternal->m_oSmartText.m_dTextSpaceEps = 0.1;
                    break;
                }
                case odftDJVU:
                {
                    m_pInternal->m_lSrcFileType = AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU;
                    m_pInternal->m_oSmartText.m_dTextSpaceEps = 0.1;
                    break;
                }
                case odftXPS:
                {
                    m_pInternal->m_lSrcFileType = AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_XPS;
                    m_pInternal->m_oSmartText.m_dTextSpaceEps = 0.1;
                    break;
                }
            default:
                break;
            }

            m_pInternal->m_bIsFontsInit = true;
            m_pInternal->m_oSmartText.Init(pFile->GetFonts(), nCacheSize);
        }

        m_pInternal->m_oPage.ClearNoAttack();
        m_pInternal->m_oPage.WriteLONG(0);

        // статистика
        m_pInternal->m_oPage.WriteLONG(0);
        m_pInternal->m_oPage.WriteLONG(0);
        m_pInternal->m_oPage.WriteLONG(0);
        m_pInternal->m_oPage.WriteLONG(0);

    }

    BYTE* CHTMLRendererText::GetBuffer()
    {
        m_pInternal->m_oSmartText.ClosePage();
        LONG lPos = m_pInternal->m_oPage.GetPosition();
        m_pInternal->m_oPage.Seek(0);
        // len
        m_pInternal->m_oPage.WriteLONG(lPos);
        // stat
        m_pInternal->m_oPage.WriteLONG(m_pInternal->m_oSmartText.m_lCountParagraphs);
        m_pInternal->m_oPage.WriteLONG(m_pInternal->m_oSmartText.m_lCountWords);
        m_pInternal->m_oPage.WriteLONG(m_pInternal->m_oSmartText.m_lCountSymbols);
        m_pInternal->m_oPage.WriteLONG(m_pInternal->m_oSmartText.m_lCountSpaces);
        // seek to end
        m_pInternal->m_oPage.Seek(lPos);

        m_pInternal->m_oSmartText.ClearStatistics();
        return m_pInternal->m_oPage.GetData();
    }

    HRESULT CHTMLRendererText::get_Type(LONG* lType)
    {
        *lType = c_nHtmlRendrererText;
        return S_OK;
    }

    //-------- Функции для работы со страницей --------------------------------------------------
    HRESULT CHTMLRendererText::NewPage()
    {
        return S_OK;
    }
    HRESULT CHTMLRendererText::get_Height(double* dHeight)
    {
        *dHeight = m_pInternal->m_dHeght;
        return S_OK;
    }
    HRESULT CHTMLRendererText::put_Height(const double& dHeight)
    {
        m_pInternal->m_dHeght = dHeight;
        return S_OK;
    }
    HRESULT CHTMLRendererText::get_Width(double* dWidth)
    {
        *dWidth = m_pInternal->m_dWidth;
        return S_OK;
    }
    HRESULT CHTMLRendererText::put_Width(const double& dWidth)
    {
        m_pInternal->m_dWidth = dWidth;
        return S_OK;
    }
    HRESULT CHTMLRendererText::get_DpiX(double* dDpiX)
    {
        *dDpiX = 96;
        return S_OK;
    }
    HRESULT CHTMLRendererText::get_DpiY(double* dDpiY)
    {
        *dDpiY = 96;
        return S_OK;
    }

    // pen --------------------------------------------------------------------------------------
    HRESULT CHTMLRendererText::get_PenColor(LONG* lColor) { return S_OK; }
    HRESULT CHTMLRendererText::put_PenColor(const LONG& lColor) { return S_OK; }
    HRESULT CHTMLRendererText::get_PenAlpha(LONG* lAlpha) { return S_OK; }
    HRESULT CHTMLRendererText::put_PenAlpha(const LONG& lAlpha) { return S_OK; }
    HRESULT CHTMLRendererText::get_PenSize(double* dSize) { return S_OK; }
    HRESULT CHTMLRendererText::put_PenSize(const double& dSize) { return S_OK; }
    HRESULT CHTMLRendererText::get_PenDashStyle(BYTE* val) { return S_OK; }
    HRESULT CHTMLRendererText::put_PenDashStyle(const BYTE& val) { return S_OK; }
    HRESULT CHTMLRendererText::get_PenLineStartCap(BYTE* val) { return S_OK; }
    HRESULT CHTMLRendererText::put_PenLineStartCap(const BYTE& val) { return S_OK; }
    HRESULT CHTMLRendererText::get_PenLineEndCap(BYTE* val) { return S_OK; }
    HRESULT CHTMLRendererText::put_PenLineEndCap(const BYTE& val) { return S_OK; }
    HRESULT CHTMLRendererText::get_PenLineJoin(BYTE* val) { return S_OK; }
    HRESULT CHTMLRendererText::put_PenLineJoin(const BYTE& val) { return S_OK; }
    HRESULT CHTMLRendererText::get_PenDashOffset(double* dOffset) { return S_OK; }
    HRESULT CHTMLRendererText::put_PenDashOffset(const double& dOffset) { return S_OK; }
    HRESULT CHTMLRendererText::get_PenAlign(LONG* lAlign) { return S_OK; }
    HRESULT CHTMLRendererText::put_PenAlign(const LONG& lAlign) { return S_OK; }
    HRESULT CHTMLRendererText::get_PenMiterLimit(double* dOffset) { return S_OK; }
    HRESULT CHTMLRendererText::put_PenMiterLimit(const double& dOffset) { return S_OK; }
    HRESULT CHTMLRendererText::PenDashPattern(double* pPattern, LONG lCount) { return S_OK; }

    // brush ------------------------------------------------------------------------------------
    HRESULT CHTMLRendererText::get_BrushType(LONG* lType)
    {
        *lType = m_pInternal->m_oBrush.Type;
        return S_OK;
    }
    HRESULT CHTMLRendererText::put_BrushType(const LONG& lType)
    {
        m_pInternal->m_oBrush.Type = lType;
        return S_OK;
    }
    HRESULT CHTMLRendererText::get_BrushColor1(LONG* lColor)
    {
        *lColor = m_pInternal->m_oBrush.Color1;
        return S_OK;
    }
    HRESULT CHTMLRendererText::put_BrushColor1(const LONG& lColor)
    {
        m_pInternal->m_oBrush.Color1 = lColor;
        return S_OK;
    }
    HRESULT CHTMLRendererText::get_BrushAlpha1(LONG* lAlpha)
    {
        *lAlpha = m_pInternal->m_oBrush.Alpha1;
        return S_OK;
    }
    HRESULT CHTMLRendererText::put_BrushAlpha1(const LONG& lAlpha)
    {
        m_pInternal->m_oBrush.Alpha1 = lAlpha;
        return S_OK;
    }
    HRESULT CHTMLRendererText::get_BrushColor2(LONG* lColor) { return S_OK; }
    HRESULT CHTMLRendererText::put_BrushColor2(const LONG& lColor) { return S_OK; }
    HRESULT CHTMLRendererText::get_BrushAlpha2(LONG* lAlpha) { return S_OK; }
    HRESULT CHTMLRendererText::put_BrushAlpha2(const LONG& lAlpha) { return S_OK; }
    HRESULT CHTMLRendererText::get_BrushTexturePath(std::wstring* bsPath) { return S_OK; }
    HRESULT CHTMLRendererText::put_BrushTexturePath(const std::wstring& bsPath) { return S_OK; }
    HRESULT CHTMLRendererText::get_BrushTextureMode(LONG* lMode) { return S_OK; }
    HRESULT CHTMLRendererText::put_BrushTextureMode(const LONG& lMode) { return S_OK; }
    HRESULT CHTMLRendererText::get_BrushTextureAlpha(LONG* lTxAlpha) { return S_OK; }
    HRESULT CHTMLRendererText::put_BrushTextureAlpha(const LONG& lTxAlpha) { return S_OK; }
    HRESULT CHTMLRendererText::get_BrushLinearAngle(double* dAngle) { return S_OK; }
    HRESULT CHTMLRendererText::put_BrushLinearAngle(const double& dAngle) { return S_OK; }
    HRESULT CHTMLRendererText::BrushRect(const INT& val, const double& left, const double& top, const double& width, const double& height) { return S_OK; }
    HRESULT CHTMLRendererText::BrushBounds(const double& left, const double& top, const double& width, const double& height) { return S_OK; }
    HRESULT CHTMLRendererText::put_BrushGradientColors(LONG* lColors, double* pPositions, LONG nCount) { return S_OK; }

    // font -------------------------------------------------------------------------------------
    HRESULT CHTMLRendererText::get_FontName(std::wstring* bsName)
    {
        *bsName = m_pInternal->m_oFont.Name;
        return S_OK;
    }
    HRESULT CHTMLRendererText::put_FontName(const std::wstring& bsName)
    {
        m_pInternal->m_oFont.Name = bsName;
        m_pInternal->m_bIsChangedFontParamBetweenDrawText = true;
        return S_OK;
    }
    HRESULT CHTMLRendererText::get_FontPath(std::wstring* bsName)
    {
        *bsName = m_pInternal->m_oFont.Path;
        return S_OK;
    }
    HRESULT CHTMLRendererText::put_FontPath(const std::wstring& bsName)
    {
        m_pInternal->m_oFont.Path = bsName;
        m_pInternal->m_bIsChangedFontParamBetweenDrawText = true;
        return S_OK;
    }
    HRESULT CHTMLRendererText::get_FontSize(double* dSize)
    {
        *dSize = m_pInternal->m_oFont.Size;
        return S_OK;
    }
    HRESULT CHTMLRendererText::put_FontSize(const double& dSize)
    {
        if (m_pInternal->m_oFont.Size != dSize)
        {
            m_pInternal->m_oFont.Size = dSize;
            m_pInternal->m_bIsChangedFontParamBetweenDrawText = true;
        }
        return S_OK;
    }
    HRESULT CHTMLRendererText::get_FontStyle(LONG* lStyle)
    {
        *lStyle = m_pInternal->m_oFont.GetStyle();
        return S_OK;
    }
    HRESULT CHTMLRendererText::put_FontStyle(const LONG& lStyle)
    {
        LONG lOld = m_pInternal->m_oFont.GetStyle();
        if (lOld != lStyle)
        {
            m_pInternal->m_oFont.SetStyle(lStyle);
            m_pInternal->m_bIsChangedFontParamBetweenDrawText = true;
        }
        return S_OK;
    }
    HRESULT CHTMLRendererText::get_FontStringGID(INT* bGID)
    {
        *bGID = m_pInternal->m_oFont.StringGID;
        return S_OK;
    }
    HRESULT CHTMLRendererText::put_FontStringGID(const INT& bGID)
    {
        m_pInternal->m_oFont.StringGID = bGID;
        return S_OK;
    }
    HRESULT CHTMLRendererText::get_FontCharSpace(double* dSpace)
    {
        *dSpace = m_pInternal->m_oFont.CharSpace;
        return S_OK;
    }
    HRESULT CHTMLRendererText::put_FontCharSpace(const double& dSpace)
    {
        m_pInternal->m_oFont.CharSpace = dSpace;
        return S_OK;
    }
    HRESULT CHTMLRendererText::get_FontFaceIndex(int* lFaceIndex)
    {
        *lFaceIndex = m_pInternal->m_oFont.FaceIndex;
        return S_OK;
    }
    HRESULT CHTMLRendererText::put_FontFaceIndex(const int& lFaceIndex)
    {
        m_pInternal->m_oFont.FaceIndex = lFaceIndex;
        return S_OK;
    }

    //-------- Функции для вывода текста --------------------------------------------------------
    HRESULT CHTMLRendererText::CommandDrawTextCHAR(const LONG& c, const double& x, const double& y, const double& w, const double& h)
    {
        if (c_nHyperlinkType == m_pInternal->m_lCurrentCommandType)
            return S_OK;

        int _c = (int)c;
        m_pInternal->WriteText(&_c, NULL, 1, x, y, w, h, m_pInternal->m_bIsChangedFontParamBetweenDrawText);
        m_pInternal->m_bIsChangedFontParamBetweenDrawText = false;
        return S_OK;
    }
    HRESULT CHTMLRendererText::CommandDrawText(const std::wstring& bsText, const double& x, const double& y, const double& w, const double& h)
    {
        if (c_nHyperlinkType == m_pInternal->m_lCurrentCommandType)
            return S_OK;

        m_pInternal->GetUnicodes(bsText);
        m_pInternal->WriteText(m_pInternal->m_pTempUnicodes, NULL, m_pInternal->m_nTempUnicodesLen, x, y, w, h, m_pInternal->m_bIsChangedFontParamBetweenDrawText);
        m_pInternal->m_bIsChangedFontParamBetweenDrawText = false;
        return S_OK;
    }
    HRESULT CHTMLRendererText::CommandDrawTextExCHAR(const LONG& c, const LONG& gid, const double& x, const double& y, const double& w, const double& h)
    {
        if (c_nHyperlinkType == m_pInternal->m_lCurrentCommandType)
            return S_OK;

        int _c = (int)c;
        int _g = (int)gid;

        m_pInternal->WriteText(&_c, &_g, 1, x, y, w, h, m_pInternal->m_bIsChangedFontParamBetweenDrawText);
        m_pInternal->m_bIsChangedFontParamBetweenDrawText = false;
        return S_OK;
    }
    HRESULT CHTMLRendererText::CommandDrawTextEx(const std::wstring& bsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& x, const double& y, const double& w, const double& h)
    {
        if (c_nHyperlinkType == m_pInternal->m_lCurrentCommandType)
            return S_OK;

        m_pInternal->GetUnicodes(bsUnicodeText);
        m_pInternal->WriteText(m_pInternal->m_pTempUnicodes, (const int*)pGids, m_pInternal->m_nTempUnicodesLen, x, y, w, h, m_pInternal->m_bIsChangedFontParamBetweenDrawText);
        m_pInternal->m_bIsChangedFontParamBetweenDrawText = false;
        return S_OK;
    }

    //-------- Маркеры для команд ---------------------------------------------------------------
    HRESULT CHTMLRendererText::BeginCommand(const DWORD& lType)
    {
        m_pInternal->m_lCurrentCommandType = lType;
        return S_OK;
    }
    HRESULT CHTMLRendererText::EndCommand(const DWORD& lType)
    {
        m_pInternal->m_lCurrentCommandType = -1;
        return S_OK;
    }

    //-------- Функции для работы с Graphics Path -----------------------------------------------
    HRESULT CHTMLRendererText::PathCommandMoveTo(const double& x, const double& y) { return S_OK; }
    HRESULT CHTMLRendererText::PathCommandLineTo(const double& x, const double& y) { return S_OK; }
    HRESULT CHTMLRendererText::PathCommandLinesTo(double* points, const int& count) { return S_OK; }
    HRESULT CHTMLRendererText::PathCommandCurveTo(const double& x1, const double& y1, const double& x2, const double& y2, const double& x3, const double& y3) { return S_OK; }
    HRESULT CHTMLRendererText::PathCommandCurvesTo(double* points, const int& count) { return S_OK; }
    HRESULT CHTMLRendererText::PathCommandArcTo(const double& x, const double& y, const double& w, const double& h, const double& startAngle, const double& sweepAngle) { return S_OK; }
    HRESULT CHTMLRendererText::PathCommandClose() { return S_OK; }
    HRESULT CHTMLRendererText::PathCommandEnd() { return S_OK; }
    HRESULT CHTMLRendererText::DrawPath(const LONG& nType) { return S_OK; }
    HRESULT CHTMLRendererText::PathCommandStart() { return S_OK; }
    HRESULT CHTMLRendererText::PathCommandGetCurrentPoint(double* x, double* y) { return S_OK; }
    HRESULT CHTMLRendererText::PathCommandTextCHAR(const LONG& c, const double& x, const double& y, const double& w, const double& h) { return S_OK; }
    HRESULT CHTMLRendererText::PathCommandText(const std::wstring& bsText, const double& x, const double& y, const double& w, const double& h) { return S_OK; }
    HRESULT CHTMLRendererText::PathCommandTextExCHAR(const LONG& c, const LONG& gid, const double& x, const double& y, const double& w, const double& h) { return S_OK; }
    HRESULT CHTMLRendererText::PathCommandTextEx(const std::wstring& bsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& x, const double& y, const double& w, const double& h) { return S_OK; }

    //-------- Функции для вывода изображений ---------------------------------------------------
    HRESULT CHTMLRendererText::DrawImage(IGrObject* pImage, const double& x, const double& y, const double& w, const double& h) { return S_OK; }

    HRESULT CHTMLRendererText::DrawImageFromFile(const std::wstring& sPath, const double& x, const double& y, const double& w, const double& h, const BYTE& lAlpha) { return S_OK; }

    // transform --------------------------------------------------------------------------------
    HRESULT CHTMLRendererText::SetTransform(const double& m1, const double& m2, const double& m3, const double& m4, const double& m5, const double& m6)
    {
        m_pInternal->m_oTransform.SetElements(m1, m2, m3, m4, m5, m6);
        return S_OK;
    }
    HRESULT CHTMLRendererText::GetTransform(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF)
    {
        *pdA = m_pInternal->m_oTransform.sx();
        *pdB = m_pInternal->m_oTransform.shy();
        *pdC = m_pInternal->m_oTransform.shx();
        *pdD = m_pInternal->m_oTransform.sy();
        *pdE = m_pInternal->m_oTransform.tx();
        *pdF = m_pInternal->m_oTransform.ty();
        return S_OK;
    }
    HRESULT CHTMLRendererText::ResetTransform()
    {
        m_pInternal->m_oTransform.Reset();
        return S_OK;
    }

    // -----------------------------------------------------------------------------------------
    HRESULT CHTMLRendererText::get_ClipMode(LONG* plMode) { return S_OK; }
    HRESULT CHTMLRendererText::put_ClipMode(const LONG& lMode) { return S_OK; }

    // additiaonal params ----------------------------------------------------------------------
    HRESULT CHTMLRendererText::CommandLong(const LONG& lType, const LONG& lCommand) { return S_OK; }
    HRESULT CHTMLRendererText::CommandDouble(const LONG& lType, const double& dCommand) { return S_OK; }
    HRESULT CHTMLRendererText::CommandString(const LONG& lType, const std::wstring& sCommand) { return S_OK; }
}
