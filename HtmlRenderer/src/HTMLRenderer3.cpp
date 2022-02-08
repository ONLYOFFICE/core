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
#include "../include/HTMLRenderer3.h"
#include "./Common.h"
#include "./Document.h"
#include "./Writer.h"

namespace NSHtmlRenderer
{
    class CASCHTMLRenderer3_Private : public NSHtmlRenderer::IBaseMatrixUpdater
    {
    public:
        NSHtmlRenderer::CDocument		m_oDocument;		// вся информация о документе (только страницы и их размеры)
        NSHtmlRenderer::CWriter         m_oWriter;			// сам вьюер. основной класс

        NSHtmlRenderer::CGraphicsDumper	m_oDumper;
        bool							m_bIsGraphicsDumperMode;

        std::wstring    m_strDstFile;
        int             m_lLastSavedPage;

        Aggplus::CGraphicsPathSimpleConverter 	m_oSimpleGraphicsConverter;		// конвертер сложных гафических путей в простые

        NSFonts::IApplicationFonts* m_pApplicationFonts;
        NSFonts::IFontManager*      m_pFontManager;                                     // менеджер шрифтов

        Aggplus::CMatrix			m_oTransform;		// текущая матрица преобразований рендерера
        double						m_dTransformAngle;

        int 						m_lCurrentCommandType;	// текущая команда
        int 						m_lCurrentPage;			// текущая страница
        bool						m_bIsMetafileDrawing;
        bool						m_bIsTextGraphicType;

        int                         m_lClipMode;

        NSStructures::CPen			m_oPen;				// настройки всей графики (скопирован ашник из AVSGraphics)
        NSStructures::CBrush		m_oBrush;
        NSStructures::CFont			m_oFont;

        NSStructures::CFont			m_oInstalledFont;

        bool						m_bPageClosed;
        bool						m_bPageOpened;

        bool						m_bIsChangedFontParamBetweenDrawText;

    public:
        int*                        m_pTempUnicodes;
        int                         m_nTempUnicodesAlloc;
        int                         m_nTempUnicodesLen;

        bool                        m_bIsOnlyTextMode;
        bool                        m_bDisablePageEnd;

    public:
        CASCHTMLRenderer3_Private()
        {
            // initialize move to CreateOfficeFile (custom directory support)
            //m_oApplicationFonts.Initialize();
            m_pApplicationFonts = NSFonts::NSApplication::Create();
            m_pApplicationFonts->GetCache()->SetCacheSize(16);

            m_lLastSavedPage = 0;
            m_oDocument.SetUpdater(this);

            m_oWriter.m_pPen	= &m_oPen;
            m_oWriter.m_pBrush	= &m_oBrush;
            m_oWriter.m_pFont	= &m_oFont;

            m_bPageClosed = true;

            m_dTransformAngle = 0.0;

            m_pFontManager = NULL;

            m_oWriter.SetSimpleConverter(&m_oSimpleGraphicsConverter, &m_oTransform);
            m_oWriter.SetApplicationFonts(m_pApplicationFonts);

            m_bIsMetafileDrawing = false;
            m_bIsTextGraphicType = false;

            m_bIsChangedFontParamBetweenDrawText = true;
            m_bIsGraphicsDumperMode = false;

            m_nTempUnicodesAlloc = 100;
            m_pTempUnicodes = new int[m_nTempUnicodesAlloc];
            m_nTempUnicodesLen = 0;

            m_bIsOnlyTextMode = false;
            m_bPageOpened = false;

            m_bDisablePageEnd = false;
        }
        ~CASCHTMLRenderer3_Private()
        {
            RELEASEOBJECT(m_pApplicationFonts);
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

    public:
        virtual void OnBaseMatrixUpdate(const double& dWidth, const double& dHeight)
        {
            if (m_bPageClosed)
            {
                // значит размеры пришли не в первый раз для текущей страницы
                return;
            }
            m_bPageClosed = true;
            StartPage(dWidth, dHeight);
        }

        void CalculateFullTransform()
        {
            m_dTransformAngle	= m_oTransform.z_Rotation();
        }

        inline void MoveTo(const double& dX, const double& dY)
        {
            m_oWriter.WritePathMoveTo(dX, dY);
        }
        inline void LineTo(const double& dX, const double& dY)
        {
            m_oWriter.WritePathLineTo(dX, dY);
        }
        inline void CurveTo(const double& x1, const double& y1, const double& x2, const double& y2, const double& x3, const double& y3)
        {
            m_oWriter.WritePathCurveTo(x1, y1, x2, y2, x3, y3);
        }
        void Start()
        {
            m_oWriter.WriteBeginPath();
        }
        void End()
        {
            m_oWriter.WriteEndPath();
        }
        void Close()
        {
            m_oWriter.WritePathClose();
        }

        void StartPage(const double& dWidth, const double& dHeight)
        {
            ++m_lCurrentPage;
            m_oWriter.NewPage(dWidth, dHeight);
        }
        void EndPage()
        {
            m_oWriter.EndPage();
            m_bPageOpened = false;
        }

        void _SetFont()
        {
            if (NULL == m_pFontManager)
            {
                m_pFontManager = m_pApplicationFonts->GenerateFontManager();
            }

            double dPix = m_oFont.CharSpace * 96 / 25.4;

            if (m_oInstalledFont.IsEqual(&m_oFont))
            {
                if (1 < m_oWriter.m_dWidth)
                {
                    m_pFontManager->SetCharSpacing(dPix);
                }
                return;
            }

            m_pFontManager->SetStringGID(m_oFont.StringGID);
            if (1 < m_oWriter.m_dWidth)
            {
                m_pFontManager->SetCharSpacing(dPix);
            }

            if (m_oFont.Path.empty())
            {
                m_pFontManager->LoadFontByName(m_oFont.Name, m_oFont.Size, m_oFont.GetStyle(), 96, 96);
            }
            else
            {
                m_pFontManager->LoadFontFromFile(m_oFont.Path, m_oFont.FaceIndex, m_oFont.Size, 96, 96);
            }

            m_oInstalledFont = m_oFont;
        }
    };

    CASCHTMLRenderer3::CASCHTMLRenderer3()
    {
        m_pInternal = new CASCHTMLRenderer3_Private();
    }
    CASCHTMLRenderer3::~CASCHTMLRenderer3()
    {
        RELEASEOBJECT(m_pInternal);
    }

    HRESULT CASCHTMLRenderer3::get_Type(LONG* lType)
    {
        *lType = c_nHtmlRendrerer2;
        return S_OK;
    }

    //-------- Функции для работы со страницей --------------------------------------------------
    HRESULT CASCHTMLRenderer3::NewPage()
    {
        if (m_pInternal->m_bPageOpened)
        {
            m_pInternal->EndPage();
        }

        m_pInternal->m_oPen.SetDefaultParams();
        m_pInternal->m_oBrush.SetDefaultParams();
        m_pInternal->m_oFont.SetDefaultParams();

        m_pInternal->m_oDocument.NewPage();
        m_pInternal->m_bPageClosed = false;
        m_pInternal->m_bPageOpened = true;

        m_pInternal->m_bIsMetafileDrawing = false;
        m_pInternal->m_bIsTextGraphicType = false;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::get_Height(double* dHeight)
    {
        size_t nCount = m_pInternal->m_oDocument.m_arrPages.size();
        if ((nCount > 0) && (NULL != dHeight))
            *dHeight = m_pInternal->m_oDocument.m_arrPages[nCount - 1].GetHeight();
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::put_Height(const double& dHeight)
    {
        size_t nCount = m_pInternal->m_oDocument.m_arrPages.size();
        if (nCount > 0)
            m_pInternal->m_oDocument.m_arrPages[nCount - 1].SetHeight(dHeight);
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::get_Width(double* dWidth)
    {
        size_t nCount = m_pInternal->m_oDocument.m_arrPages.size();
        if ((nCount > 0) && (NULL != dWidth))
            *dWidth = m_pInternal->m_oDocument.m_arrPages[nCount - 1].GetWidth();
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::put_Width(const double& dWidth)
    {
        size_t nCount = m_pInternal->m_oDocument.m_arrPages.size();
        if (nCount > 0)
            m_pInternal->m_oDocument.m_arrPages[nCount - 1].SetWidth(dWidth);
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::get_DpiX(double* dDpiX)
    {
        *dDpiX = 96;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::get_DpiY(double* dDpiY)
    {
        *dDpiY = 96;
        return S_OK;
    }

    // pen --------------------------------------------------------------------------------------
    HRESULT CASCHTMLRenderer3::get_PenColor(LONG* lColor)
    {
        *lColor = m_pInternal->m_oPen.Color;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::put_PenColor(const LONG& lColor)
    {
        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.put_PenColor(lColor);

        m_pInternal->m_oPen.Color = lColor;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::get_PenAlpha(LONG* lAlpha)
    {
        *lAlpha = m_pInternal->m_oPen.Alpha;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::put_PenAlpha(const LONG& lAlpha)
    {
        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.put_PenAlpha(lAlpha);

        m_pInternal->m_oPen.Alpha = lAlpha;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::get_PenSize(double* dSize)
    {
        *dSize = m_pInternal->m_oPen.Size;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::put_PenSize(const double& dSize)
    {
        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.put_PenSize(dSize);

        m_pInternal->m_oPen.Size = dSize;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::get_PenDashStyle(BYTE* val)
    {
        *val = m_pInternal->m_oPen.DashStyle;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::put_PenDashStyle(const BYTE& val)
    {
        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.put_PenDashStyle(val);

        m_pInternal->m_oPen.DashStyle = val;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::get_PenLineStartCap(BYTE* val)
    {
        *val = m_pInternal->m_oPen.LineStartCap;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::put_PenLineStartCap(const BYTE& val)
    {
        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.put_PenLineStartCap(val);

        m_pInternal->m_oPen.LineStartCap = val;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::get_PenLineEndCap(BYTE* val)
    {
        *val = m_pInternal->m_oPen.LineEndCap;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::put_PenLineEndCap(const BYTE& val)
    {
        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.put_PenLineEndCap(val);

        m_pInternal->m_oPen.LineEndCap = val;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::get_PenLineJoin(BYTE* val)
    {
        *val = m_pInternal->m_oPen.LineJoin;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::put_PenLineJoin(const BYTE& val)
    {
        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.put_PenLineJoin(val);

        m_pInternal->m_oPen.LineJoin = val;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::get_PenDashOffset(double* dOffset)
    {
        *dOffset = m_pInternal->m_oPen.DashOffset;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::put_PenDashOffset(const double& dOffset)
    {
        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.put_PenDashOffset(dOffset);

        m_pInternal->m_oPen.DashOffset = dOffset;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::get_PenAlign(LONG* lAlign)
    {
        *lAlign = m_pInternal->m_oPen.Align;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::put_PenAlign(const LONG& lAlign)
    {
        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.put_PenAlign(lAlign);

        m_pInternal->m_oPen.Align = lAlign;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::get_PenMiterLimit(double* dOffset)
    {
        *dOffset = m_pInternal->m_oPen.MiterLimit;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::put_PenMiterLimit(const double& dOffset)
    {
        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.put_PenMiterLimit(dOffset);

        m_pInternal->m_oPen.MiterLimit = dOffset;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::PenDashPattern(double* pPattern, LONG lCount)
    {
        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.PenDashPattern(pPattern, lCount);

        m_pInternal->m_oPen.SetDashPattern(pPattern, lCount);
        return S_OK;
    }

    // brush ------------------------------------------------------------------------------------
    HRESULT CASCHTMLRenderer3::get_BrushType(LONG* lType)
    {
        *lType = m_pInternal->m_oBrush.Type;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::put_BrushType(const LONG& lType)
    {
        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.put_BrushType(lType);

        m_pInternal->m_oBrush.Type = lType;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::get_BrushColor1(LONG* lColor)
    {
        *lColor = m_pInternal->m_oBrush.Color1;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::put_BrushColor1(const LONG& lColor)
    {
        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.put_BrushColor1(lColor);

        m_pInternal->m_oBrush.Color1 = lColor;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::get_BrushAlpha1(LONG* lAlpha)
    {
        *lAlpha = m_pInternal->m_oBrush.Alpha1;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::put_BrushAlpha1(const LONG& lAlpha)
    {
        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.put_BrushAlpha1(lAlpha);

        m_pInternal->m_oBrush.Alpha1 = lAlpha;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::get_BrushColor2(LONG* lColor)
    {
        *lColor = m_pInternal->m_oBrush.Color2;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::put_BrushColor2(const LONG& lColor)
    {
        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.put_BrushColor2(lColor);

        m_pInternal->m_oBrush.Color2 = lColor;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::get_BrushAlpha2(LONG* lAlpha)
    {
        *lAlpha = m_pInternal->m_oBrush.Alpha2;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::put_BrushAlpha2(const LONG& lAlpha)
    {
        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.put_BrushAlpha2(lAlpha);

        m_pInternal->m_oBrush.Alpha2 = lAlpha;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::get_BrushTexturePath(std::wstring* bsPath)
    {
        *bsPath = m_pInternal->m_oBrush.TexturePath;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::put_BrushTexturePath(const std::wstring& bsPath)
    {
        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.put_BrushTexturePath(bsPath);

        m_pInternal->m_oBrush.TexturePath = bsPath;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::get_BrushTextureMode(LONG* lMode)
    {
        *lMode = m_pInternal->m_oBrush.TextureMode;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::put_BrushTextureMode(const LONG& lMode)
    {
        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.put_BrushTextureMode(lMode);

        m_pInternal->m_oBrush.TextureMode = lMode;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::get_BrushTextureAlpha(LONG* lTxAlpha)
    {
        *lTxAlpha = m_pInternal->m_oBrush.TextureAlpha;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::put_BrushTextureAlpha(const LONG& lTxAlpha)
    {
        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.put_BrushTextureAlpha(lTxAlpha);

        m_pInternal->m_oBrush.TextureAlpha = lTxAlpha;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::get_BrushLinearAngle(double* dAngle)
    {
        *dAngle = m_pInternal->m_oBrush.LinearAngle;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::put_BrushLinearAngle(const double& dAngle)
    {
        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.put_BrushLinearAngle(dAngle);

        m_pInternal->m_oBrush.LinearAngle = dAngle;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::BrushRect(const INT& val, const double& left, const double& top, const double& width, const double& height)
    {
        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.BrushRect(val, left, top, width, height);

        m_pInternal->m_oBrush.Rectable = val;
        m_pInternal->m_oBrush.Rect.X = (float)left;
        m_pInternal->m_oBrush.Rect.Y = (float)top;
        m_pInternal->m_oBrush.Rect.Width  = (float)width;
        m_pInternal->m_oBrush.Rect.Height = (float)height;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::BrushBounds(const double& left, const double& top, const double& width, const double& height)
    {
        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.BrushBounds(left, top, width, height);

        m_pInternal->m_oBrush.Bounds.left = left;
        m_pInternal->m_oBrush.Bounds.top = top;
        m_pInternal->m_oBrush.Bounds.right = left + width;
        m_pInternal->m_oBrush.Bounds.bottom = top + height;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::put_BrushGradientColors(LONG* lColors, double* pPositions, LONG nCount)
    {
        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.put_BrushGradientColors(lColors, pPositions, nCount);

        m_pInternal->m_oBrush.m_arrSubColors.clear();
        for (LONG i = 0; i < nCount; ++i)
        {
            NSStructures::CBrush::TSubColor color;
            color.color		= lColors[i];
            color.position	= (long)(pPositions[i] * 65536);
            m_pInternal->m_oBrush.m_arrSubColors.push_back(color);
        }
        return S_OK;
    }

    // font -------------------------------------------------------------------------------------
    HRESULT CASCHTMLRenderer3::get_FontName(std::wstring* bsName)
    {
        *bsName = m_pInternal->m_oFont.Name;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::put_FontName(const std::wstring& bsName)
    {
        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.put_FontName(bsName);

        m_pInternal->m_oFont.Name = bsName;
        m_pInternal->m_bIsChangedFontParamBetweenDrawText = true;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::get_FontPath(std::wstring* bsName)
    {
        *bsName = m_pInternal->m_oFont.Path;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::put_FontPath(const std::wstring& bsName)
    {
        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.put_FontPath(bsName);

        m_pInternal->m_oFont.Path = bsName;
        m_pInternal->m_bIsChangedFontParamBetweenDrawText = true;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::get_FontSize(double* dSize)
    {
        *dSize = m_pInternal->m_oFont.Size;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::put_FontSize(const double& dSize)
    {
        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.put_FontSize(dSize);

        if (m_pInternal->m_oFont.Size != dSize)
        {
            m_pInternal->m_oFont.Size = dSize;
            m_pInternal->m_bIsChangedFontParamBetweenDrawText = true;
        }
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::get_FontStyle(LONG* lStyle)
    {
        *lStyle = m_pInternal->m_oFont.GetStyle();
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::put_FontStyle(const LONG& lStyle)
    {
        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.put_FontStyle(lStyle);

        LONG lOld = m_pInternal->m_oFont.GetStyle();
        if (lOld != lStyle)
        {
            m_pInternal->m_oFont.SetStyle(lStyle);
            m_pInternal->m_bIsChangedFontParamBetweenDrawText = true;
        }
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::get_FontStringGID(INT* bGID)
    {
        *bGID = m_pInternal->m_oFont.StringGID;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::put_FontStringGID(const INT& bGID)
    {
        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.put_FontStringGID(bGID);

        m_pInternal->m_oFont.StringGID = bGID;
        m_pInternal->m_pFontManager->SetStringGID(bGID);
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::get_FontCharSpace(double* dSpace)
    {
        *dSpace = m_pInternal->m_oFont.CharSpace;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::put_FontCharSpace(const double& dSpace)
    {
        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.put_FontCharSpace(dSpace);

        m_pInternal->m_oFont.CharSpace = dSpace;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::get_FontFaceIndex(int* lFaceIndex)
    {
        *lFaceIndex = m_pInternal->m_oFont.FaceIndex;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::put_FontFaceIndex(const int& lFaceIndex)
    {
        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.put_FontFaceIndex(lFaceIndex);

        m_pInternal->m_oFont.FaceIndex = lFaceIndex;
        return S_OK;
    }

    //-------- Функции для вывода текста --------------------------------------------------------
    HRESULT CASCHTMLRenderer3::CommandDrawTextCHAR(const LONG& c, const double& x, const double& y, const double& w, const double& h)
    {
        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.CommandDrawTextCHAR(c, x, y, w, h);

        if (c_nHyperlinkType == m_pInternal->m_lCurrentCommandType)
            return S_OK;

        int _c = (int)c;
        m_pInternal->m_oWriter.WriteText(&_c, NULL, 1, x, y, w, h, m_pInternal->m_bIsChangedFontParamBetweenDrawText);
        m_pInternal->m_bIsChangedFontParamBetweenDrawText = false;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::CommandDrawText(const std::wstring& bsText, const double& x, const double& y, const double& w, const double& h)
    {
        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.CommandDrawText(bsText, x, y, w, h);

        if (c_nHyperlinkType == m_pInternal->m_lCurrentCommandType)
            return S_OK;

        m_pInternal->GetUnicodes(bsText);
        m_pInternal->m_oWriter.WriteText(m_pInternal->m_pTempUnicodes, NULL, m_pInternal->m_nTempUnicodesLen,
                                         x, y, w, h, m_pInternal->m_bIsChangedFontParamBetweenDrawText);
        m_pInternal->m_bIsChangedFontParamBetweenDrawText = false;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::CommandDrawTextExCHAR(const LONG& c, const LONG& gid, const double& x, const double& y, const double& w, const double& h)
    {
        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.CommandDrawTextExCHAR(c, gid, x, y, w, h);

        if (c_nHyperlinkType == m_pInternal->m_lCurrentCommandType)
            return S_OK;

        int _c = (int)c;
        int _g = (int)gid;

        m_pInternal->m_oWriter.WriteText(&_c, &_g, 1,
                                         x, y, w, h, m_pInternal->m_bIsChangedFontParamBetweenDrawText);
        m_pInternal->m_bIsChangedFontParamBetweenDrawText = false;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::CommandDrawTextEx(const std::wstring& bsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& x, const double& y, const double& w, const double& h)
    {
        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.CommandDrawText(bsUnicodeText, x, y, w, h);

        if (c_nHyperlinkType == m_pInternal->m_lCurrentCommandType)
            return S_OK;

        m_pInternal->GetUnicodes(bsUnicodeText);
        m_pInternal->m_oWriter.WriteText(m_pInternal->m_pTempUnicodes, (const int*)pGids, m_pInternal->m_nTempUnicodesLen,
                                         x, y, w, h, m_pInternal->m_bIsChangedFontParamBetweenDrawText);
        m_pInternal->m_bIsChangedFontParamBetweenDrawText = false;
        return S_OK;
    }

    //-------- Маркеры для команд ---------------------------------------------------------------
    HRESULT CASCHTMLRenderer3::BeginCommand(const DWORD& lType)
    {
        if (m_pInternal->m_bIsOnlyTextMode)
        {
            m_pInternal->m_lCurrentCommandType = lType;
            return S_OK;
        }

        if (m_pInternal->m_bIsGraphicsDumperMode && lType != c_nPDFTilingFill)
            return m_pInternal->m_oDumper.BeginCommand(lType);

        if (c_nClipType == lType)
        {
            m_pInternal->m_oWriter.WritePathClip();
        }
        else if (c_nPathType == lType)
        {
            m_pInternal->m_oWriter.WriteBeginPath();
            m_pInternal->m_oSimpleGraphicsConverter.PathCommandEnd();
        }
        else if (c_nImageType == lType)
        {
            m_pInternal->m_bIsMetafileDrawing = true;
        }
        else if (c_nTextGraphicType == lType)
        {
            m_pInternal->m_bIsTextGraphicType = true;
        }
        else if (c_nPDFTilingFill == lType)
        {
            m_pInternal->m_oWriter.m_lTilingCounter++;
        }
        m_pInternal->m_lCurrentCommandType = lType;

        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::EndCommand(const DWORD& lType)
    {
        if (m_pInternal->m_bIsOnlyTextMode)
        {
            if (c_nPageType == lType && !m_pInternal->m_bDisablePageEnd)
            {
                m_pInternal->EndPage();
            }
            m_pInternal->m_lCurrentCommandType = -1;
            return S_OK;
        }

        if (m_pInternal->m_bIsGraphicsDumperMode && lType != c_nPDFTilingFill)
            return m_pInternal->m_oDumper.EndCommand(lType);

        if (c_nPageType == lType && !m_pInternal->m_bDisablePageEnd)
        {
            m_pInternal->EndPage();
        }
        else if (c_nClipType == lType)
        {
            m_pInternal->m_oWriter.WritePathClipEnd();
        }
        else if (c_nResetClipType == lType)
        {
            m_pInternal->m_oWriter.WritePathResetClip();
        }
        //else if (c_nPathType == lType)
        //{
        //	PathCommandEnd();
        //}
        else if (c_nImageType == lType)
        {
            m_pInternal->m_bIsMetafileDrawing = false;
        }
        else if (c_nTextGraphicType == lType)
        {
            m_pInternal->m_bIsTextGraphicType = false;
        }
        else if (c_nPDFTilingFill == lType)
        {
            m_pInternal->m_oWriter.m_lTilingCounter--;

            if (0 == m_pInternal->m_oWriter.m_lTilingCounter)
            {
                m_pInternal->m_bIsGraphicsDumperMode = false;

                // dump Graphics
                CBgraFrame* pFrame = m_pInternal->m_oDumper.ConvertVectorGraphics();
                m_pInternal->m_oWriter.WritePattern(pFrame, m_pInternal->m_oDumper.m_oTile);
                RELEASEOBJECT(pFrame);
            }
        }
        //else if (c_nParagraphType == lType)
        //{
        //	m_oWriter.m_oPage.m_oText.EndParagraph();
        //}
        m_pInternal->m_lCurrentCommandType = -1;

        return S_OK;
    }

    //-------- Функции для работы с Graphics Path -----------------------------------------------
    HRESULT CASCHTMLRenderer3::PathCommandMoveTo(const double& x, const double& y)
    {
        if (m_pInternal->m_bIsOnlyTextMode)
            return S_OK;

        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.PathCommandMoveTo(x, y);

        if (c_nSimpleGraphicType == m_pInternal->m_lCurrentCommandType)
        {
            m_pInternal->MoveTo(x, y);
        }
        else
        {
            m_pInternal->m_oSimpleGraphicsConverter.PathCommandMoveTo(x, y);
        }

        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::PathCommandLineTo(const double& x, const double& y)
    {
        if (m_pInternal->m_bIsOnlyTextMode)
            return S_OK;

        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.PathCommandLineTo(x, y);

        if (c_nSimpleGraphicType == m_pInternal->m_lCurrentCommandType)
        {
            m_pInternal->LineTo(x, y);
        }
        else
        {
            m_pInternal->m_oSimpleGraphicsConverter.PathCommandLineTo(x, y);
        }

        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::PathCommandLinesTo(double* points, const int& count)
    {
        if (m_pInternal->m_bIsOnlyTextMode)
            return S_OK;

        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.PathCommandLinesTo(points, count);

        m_pInternal->m_oSimpleGraphicsConverter.PathCommandLinesTo(points, count);
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::PathCommandCurveTo(const double& x1, const double& y1, const double& x2, const double& y2, const double& x3, const double& y3)
    {
        if (m_pInternal->m_bIsOnlyTextMode)
            return S_OK;

        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.PathCommandCurveTo(x1, y1, x2, y2, x3, y3);

        if (c_nSimpleGraphicType == m_pInternal->m_lCurrentCommandType)
        {
            m_pInternal->CurveTo(x1, y1, x2, y2, x3, y3);
        }
        else
        {
            m_pInternal->m_oSimpleGraphicsConverter.PathCommandCurveTo(x1, y1, x2, y2, x3, y3);
        }

        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::PathCommandCurvesTo(double* points, const int& count)
    {
        if (m_pInternal->m_bIsOnlyTextMode)
            return S_OK;

        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.PathCommandCurvesTo(points, count);

        m_pInternal->m_oSimpleGraphicsConverter.PathCommandCurvesTo(points, count);
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::PathCommandArcTo(const double& x, const double& y, const double& w, const double& h, const double& startAngle, const double& sweepAngle)
    {
        if (m_pInternal->m_bIsOnlyTextMode)
            return S_OK;

        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.PathCommandArcTo(x, y, w, h, startAngle, sweepAngle);

        m_pInternal->m_oSimpleGraphicsConverter.PathCommandArcTo(x, y, w, h, startAngle, sweepAngle);
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::PathCommandClose()
    {
        if (m_pInternal->m_bIsOnlyTextMode)
            return S_OK;

        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.PathCommandClose();

        if (c_nSimpleGraphicType == m_pInternal->m_lCurrentCommandType)
        {
            m_pInternal->Close();
        }
        else
        {
            m_pInternal->m_oSimpleGraphicsConverter.PathCommandClose();
        }

        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::PathCommandEnd()
    {
        if (m_pInternal->m_bIsOnlyTextMode)
            return S_OK;

        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.PathCommandEnd();

        if (c_nSimpleGraphicType == m_pInternal->m_lCurrentCommandType)
        {
            m_pInternal->End();
        }
        else
        {
            m_pInternal->m_oSimpleGraphicsConverter.PathCommandEnd();
        }

        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::DrawPath(const LONG& nType)
    {
        if (m_pInternal->m_bIsOnlyTextMode)
            return S_OK;

        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.DrawPath(nType);

        m_pInternal->m_oWriter.WriteDrawPath(nType);
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::PathCommandStart()
    {
        if (m_pInternal->m_bIsOnlyTextMode)
            return S_OK;

        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.PathCommandStart();

        if (c_nSimpleGraphicType == m_pInternal->m_lCurrentCommandType)
        {
            m_pInternal->Start();
        }
        else
        {
            m_pInternal->m_oSimpleGraphicsConverter.PathCommandEnd();
            m_pInternal->m_oSimpleGraphicsConverter.PathCommandStart();
        }

        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::PathCommandGetCurrentPoint(double* x, double* y)
    {
        if (m_pInternal->m_bIsOnlyTextMode)
            return S_OK;

        m_pInternal->m_oSimpleGraphicsConverter.PathCommandGetCurrentPoint(x, y);
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::PathCommandTextCHAR(const LONG& c, const double& x, const double& y, const double& w, const double& h)
    {
        if (m_pInternal->m_bIsOnlyTextMode)
            return S_OK;

        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.PathCommandTextCHAR(c, x, y, w, h);

        int _c = (int)c;

        m_pInternal->_SetFont();
        m_pInternal->m_oSimpleGraphicsConverter.PathCommandText2(&_c, NULL, 1, m_pInternal->m_pFontManager, x, y, w, h);
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::PathCommandText(const std::wstring& bsText, const double& x, const double& y, const double& w, const double& h)
    {
        if (m_pInternal->m_bIsOnlyTextMode)
            return S_OK;

        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.PathCommandText(bsText, x, y, w, h);

        m_pInternal->GetUnicodes(bsText);

        m_pInternal->_SetFont();
        m_pInternal->m_oSimpleGraphicsConverter.PathCommandText2(m_pInternal->m_pTempUnicodes, NULL, m_pInternal->m_nTempUnicodesLen,
                                                                 m_pInternal->m_pFontManager, x, y, w, h);
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::PathCommandTextExCHAR(const LONG& c, const LONG& gid, const double& x, const double& y, const double& w, const double& h)
    {
        if (m_pInternal->m_bIsOnlyTextMode)
            return S_OK;

        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.PathCommandTextExCHAR(c, gid, x, y, w, h);

        int _c = (int)c;
        int _g = (int)gid;

        m_pInternal->_SetFont();
        m_pInternal->m_oSimpleGraphicsConverter.PathCommandText2(&_c, &_g, 1,
                                                                 m_pInternal->m_pFontManager, x, y, w, h);
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::PathCommandTextEx(const std::wstring& bsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& x, const double& y, const double& w, const double& h)
    {
        if (m_pInternal->m_bIsOnlyTextMode)
            return S_OK;

        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.PathCommandTextEx(bsUnicodeText, pGids, nGidsCount, x, y, w, h);

        m_pInternal->_SetFont();
        m_pInternal->m_oSimpleGraphicsConverter.PathCommandText2(NULL, (const int*)pGids, nGidsCount,
                                                                 m_pInternal->m_pFontManager, x, y, w, h);
        return S_OK;
    }

    //-------- Функции для вывода изображений ---------------------------------------------------
    HRESULT CASCHTMLRenderer3::DrawImage(IGrObject* pImage, const double& x, const double& y, const double& w, const double& h)
    {
        if (m_pInternal->m_bIsOnlyTextMode)
            return S_OK;

        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.DrawImage(pImage, x, y, w, h);

        if (NULL == pImage)
            return S_OK;

        m_pInternal->m_oWriter.WriteImage(pImage, x, y, w, h);
        return S_OK;
    }

    HRESULT CASCHTMLRenderer3::DrawImageFromFile(const std::wstring& sPath, const double& x, const double& y, const double& w, const double& h, const BYTE& lAlpha)
    {
        if (m_pInternal->m_bIsOnlyTextMode)
            return S_OK;

        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.DrawImageFromFile(sPath, x, y, w, h, lAlpha);

        m_pInternal->m_oWriter.WriteImage(sPath, x, y, w, h);
        return S_OK;
    }

    // transform --------------------------------------------------------------------------------
    HRESULT CASCHTMLRenderer3::SetTransform(const double& m1, const double& m2, const double& m3, const double& m4, const double& m5, const double& m6)
    {
        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.SetTransform(m1, m2, m3, m4, m5, m6);

        m_pInternal->m_oTransform.SetElements(m1, m2, m3, m4, m5, m6);
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::GetTransform(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF)
    {
        *pdA = m_pInternal->m_oTransform.sx();
        *pdB = m_pInternal->m_oTransform.shy();
        *pdC = m_pInternal->m_oTransform.shx();
        *pdD = m_pInternal->m_oTransform.sy();
        *pdE = m_pInternal->m_oTransform.tx();
        *pdF = m_pInternal->m_oTransform.ty();
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::ResetTransform()
    {
        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.ResetTransform();

        m_pInternal->m_oTransform.Reset();
        return S_OK;
    }

    // -----------------------------------------------------------------------------------------
    HRESULT CASCHTMLRenderer3::get_ClipMode(LONG* plMode)
    {
        *plMode = m_pInternal->m_lClipMode;
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::put_ClipMode(const LONG& lMode)
    {
        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.put_ClipMode(lMode);

        m_pInternal->m_lClipMode = lMode;
        m_pInternal->m_oWriter.m_oSVGWriter.m_lClipMode = lMode;
        return S_OK;
    }

    // additiaonal params ----------------------------------------------------------------------
    HRESULT CASCHTMLRenderer3::CommandLong(const LONG& lType, const LONG& lCommand)
    {
        if (c_nCommandLongTypeOnlyText == lType)
        {
            return GetOnlyTextMode() ? S_OK : S_FALSE;
        }
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::CommandDouble(const LONG& lType, const double& dCommand)
    {
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::CommandString(const LONG& lType, const std::wstring& sCommand)
    {
        return S_OK;
    }

    HRESULT CASCHTMLRenderer3::StartConvertCoordsToIdentity()
    {
        m_bUseTransformCoordsToIdentity = true;
        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.StartConvertCoordsToIdentity();
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::EndConvertCoordsToIdentity()
    {
        m_bUseTransformCoordsToIdentity = false;
        if (m_pInternal->m_bIsGraphicsDumperMode)
            return m_pInternal->m_oDumper.EndConvertCoordsToIdentity();
        return S_OK;
    }

    // owner params ----------------------------------------------------------------------
    HRESULT CASCHTMLRenderer3::get_Mode(LONG *plMode)
    {
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::put_Mode(LONG lMode)
    {
        return S_OK;
    }

    HRESULT CASCHTMLRenderer3::CreateOfficeFile(std::wstring bsFileName, const std::wstring& fontsDir)
    {
        if (fontsDir.empty())
            m_pInternal->m_pApplicationFonts->Initialize();
        else
            m_pInternal->m_pApplicationFonts->InitializeFromFolder(fontsDir);

        m_pInternal->m_strDstFile = bsFileName;

        size_t pos1 = m_pInternal->m_strDstFile.find_last_of(wchar_t('/'));
        size_t pos2 = m_pInternal->m_strDstFile.find_last_of(wchar_t('\\'));

        size_t pos = std::wstring::npos;
        if (pos1 != std::wstring::npos)
            pos = pos1;

        if (pos2 != std::wstring::npos)
        {
            if (pos == std::wstring::npos)
                pos = pos2;
            else if (pos2 > pos)
                pos = pos2;
        }

        if (pos == std::wstring::npos)
            return S_FALSE;

        std::wstring strDir = m_pInternal->m_strDstFile.substr(0, pos);

        size_t nIndexExt = m_pInternal->m_strDstFile.find_last_of(wchar_t('.'));
        if (nIndexExt == std::wstring::npos || nIndexExt <= pos)
            nIndexExt = pos;

        std::wstring strName = m_pInternal->m_strDstFile.substr(pos + 1, nIndexExt - pos - 1);

        m_pInternal->m_oWriter.m_pTransform = &m_pInternal->m_oTransform;
        m_pInternal->m_oWriter.CreateFile(strDir, strName);
        m_pInternal->m_oWriter.WriteStartDocument();

        m_pInternal->m_oWriter.m_oSmartText.m_oFontManager.Init(m_pInternal->m_pApplicationFonts);

        m_pInternal->m_lCurrentPage = -1;
        m_pInternal->m_bPageOpened = false;

        m_pInternal->m_pFontManager = m_pInternal->m_pApplicationFonts->GenerateFontManager();
        NSFonts::IFontsCache* pGraphicsFontCache = NSFonts::NSFontCache::Create();
        pGraphicsFontCache->SetStreams(m_pInternal->m_pApplicationFonts->GetStreams());
        pGraphicsFontCache->SetCacheSize(16);
        m_pInternal->m_pFontManager->SetOwnerCache(pGraphicsFontCache);

        m_pInternal->m_oFont.SetDefaultParams();
        m_pInternal->m_oInstalledFont.SetDefaultParams();

        m_pInternal->m_oSimpleGraphicsConverter.SetRenderer(this);
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::CloseFile(bool bIsNoBase64)
    {
        if (m_pInternal->m_bPageOpened)
        {
            m_pInternal->EndPage();
        }

        m_pInternal->m_oWriter.WriteEndDocument(m_pInternal->m_oDocument, bIsNoBase64);
        return S_OK;
    }

    HRESULT CASCHTMLRenderer3::SetAdditionalParam(std::string sParamName, int nValue)
    {
        if ("SourceType" == sParamName)
        {
            m_pInternal->m_oWriter.m_lSrcFileType = nValue;
            return S_OK;
        }
        return S_OK;
    }
    HRESULT CASCHTMLRenderer3::SetAdditionalParam(std::string sParamName, const std::wstring& sParam)
    {
        if ("DisablePageEnd" == sParamName)
        {
            m_pInternal->m_bDisablePageEnd = L"yes" == sParam;
        }

        if (m_pInternal->m_bIsOnlyTextMode)
            return S_OK;

        if ("TilingHtmlPattern" == sParamName)
        {
            if (1 == m_pInternal->m_oWriter.m_lTilingCounter)
            {
                m_pInternal->m_bIsGraphicsDumperMode = true;
                m_pInternal->m_oDumper.m_oTile.LoadFromXml(sParam);

                m_pInternal->m_oDumper.NewPage(m_pInternal->m_oWriter.m_dWidth, m_pInternal->m_oWriter.m_dHeight);

                m_pInternal->m_oDumper.m_oTile.bbox_x = 0;
                m_pInternal->m_oDumper.m_oTile.bbox_y = 0;
                m_pInternal->m_oDumper.m_oTile.bbox_r = m_pInternal->m_oDumper.m_lWidthPix - 1;
                m_pInternal->m_oDumper.m_oTile.bbox_b = m_pInternal->m_oDumper.m_lHeightPix - 1;

                m_pInternal->m_oDumper.m_oBounds.left = 0;
                m_pInternal->m_oDumper.m_oBounds.top = 0;
                m_pInternal->m_oDumper.m_oBounds.right = m_pInternal->m_oDumper.m_lWidthPix - 1;
                m_pInternal->m_oDumper.m_oBounds.bottom = m_pInternal->m_oDumper.m_lHeightPix - 1;
            }
        }
        return S_OK;
    }

    bool CASCHTMLRenderer3::GetOnlyTextMode()
    {
        return m_pInternal->m_bIsOnlyTextMode;
    }

    void CASCHTMLRenderer3::SetOnlyTextMode(const bool& enabled)
    {
        m_pInternal->m_oWriter.m_bIsOnlyTextMode = enabled;
        m_pInternal->m_bIsOnlyTextMode = enabled;
    }

    void CASCHTMLRenderer3::GetLastPageInfo(int& paragraphs, int& words, int& symbols, int& spaces, std::string& sBase64Data)
    {
        m_pInternal->m_oWriter.GetLastPageInfo(paragraphs, words, symbols, spaces, sBase64Data);
    }
}
