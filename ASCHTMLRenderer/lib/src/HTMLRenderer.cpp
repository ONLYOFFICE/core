#include "../include/HTMLRenderer.h"
#include "./Common.h"

// TEST
#include "VectorGraphicsWriter2.h"
#include "CanvasWriter.h"
#include "SVGWriter2.h"
#include "FontManager.h"
#include "Text.h"
#include "Document.h"

class CASCHTMLRenderer_Private
{
public:
    NSHtmlRenderer::CDocument		m_oDocument;		// вся информация о документе (только страницы и их размеры)
    NSHtmlRenderer::CThumbnails		m_oThumbnail;		// генератор табнейлов (создает, отрисовывает, сохраняет)
    //NSHtmlRenderer::CWriter			m_oWriter;			// сам вьюер. основной класс

    std::wstring    m_strDstFile;
    LONG            m_lLastSavedPage;

    Aggplus::CGraphicsPathSimpleConverter*	m_pSimpleGraphicsConverter;		// конвертер сложных гафических путей в простые

    CApplicationFonts                       m_oApplicationFonts;
    CFontManager*                           m_pFontManager;					// менеджер шрифтов

    Aggplus::CMatrix                m_oBaseTransform;	// матрица перерасчета координатных осей (здесь: миллиметры -> пикселы)
    Aggplus::CMatrix                m_oTransform;		// текущая матрица преобразований рендерера
    Aggplus::CMatrix                m_oFullTransform;	// полная матрица преобразований (m_oBaseTransform * m_oTransform)

    double							m_dTransformAngle;

    LONG							m_lCurrentCommandType;	// текущая команда
    LONG							m_lCurrentPage;			// текущая страница
    bool							m_bIsMetafileDrawing;
    bool							m_bIsTextGraphicType;

    double							m_dDpiX;
    double							m_dDpiY;

    LONG							m_lClipMode;

    NSStructures::CPen				m_oPen;				// настройки всей графики (скопирован ашник из AVSGraphics)
    NSStructures::CBrush			m_oBrush;
    NSStructures::CFont				m_oFont;
    NSStructures::CShadow			m_oShadow;
    NSStructures::CEdgeText			m_oEdge;

    NSStructures::CFont				m_oInstalledFont;

    bool							m_bPageClosed;
    bool							m_bPageOpened;

    bool							m_bIsThumbnailsCreate;
    bool							m_bIsWEB;
    bool							m_bIsMenu;

public:
};

CASCHTMLRenderer::CASCHTMLRenderer()
{
    m_pInternal->m_lLastSavedPage = 0;
    m_pInternal->m_oDocument.SetUpdater(this);

    m_pInternal->m_dDpiX = 96.0;
    m_pInternal->m_dDpiY	= 96.0;

    m_pInternal->m_lClipMode = c_nClipRegionTypeWinding;

    m_pInternal->m_oWriter.m_dDpiX	= m_dDpiX;
    m_pInternal->m_oWriter.m_dDpiY	= m_dDpiY;

    m_pInternal->m_oWriter.m_pPen	= &m_oPen;
    m_pInternal->m_oWriter.m_pBrush	= &m_oBrush;
    m_pInternal->m_oWriter.m_pFont	= &m_oFont;

    m_pInternal->m_pSimpleGraphicsConverter = new Aggplus::CGraphicsPathSimpleConverter();
    m_pInternal->m_pSimpleGraphicsConverter->SetRenderer(this);

    m_pInternal->m_bPageClosed = TRUE;

    m_pInternal->m_dTransformAngle = 0.0;

    m_pInternal->m_pFontManager = NULL;

    //m_oWriter.SetSimpleConverter(m_pSimpleGraphicsConverter, &m_oFullTransform);
    //m_oWriter.m_oPage.SetSettings(&m_oPen, &m_oBrush, &m_oFont, m_oWriter.GetStyles(), &m_oTransform);

    m_pInternal->m_bIsThumbnailsCreate  = false;
    m_pInternal->m_bIsWEB               = false;
    m_pInternal->m_bIsMenu              = false;

    m_bIsMetafileDrawing = false;
    m_bIsTextGraphicType = false;
}

CASCHTMLRenderer::~CASCHTMLRenderer()
{
    RELEASEOBJECT((m_pInternal->m_pSimpleGraphicsConverter));
}

HRESULT CASCHTMLRenderer::get_Type(LONG* lType)
{
    *lType = c_nHtmlRendrerer;
    return S_OK;
}

//-------- Функции для работы со страницей --------------------------------------------------
HRESULT CASCHTMLRenderer::NewPage()
{
    if (m_pInternal->m_bPageOpened)
    {
        m_pInternal->EndPage();
    }

    m_pInternal->m_oDocument.NewPage();
    m_pInternal->m_bPageClosed = false;
    m_pInternal->m_bPageOpened = true;

    m_pInternal->m_bIsMetafileDrawing = false;
    m_pInternal->m_bIsTextGraphicType = false;
    return S_OK;
}

HRESULT CASCHTMLRenderer::get_Height(double* dHeight)
{
    size_t nCount = m_pInternal->m_oDocument.m_arrPages.size();
    if (nCount > 0)
        *dHeight = m_pInternal->m_oDocument.m_arrPages[nCount - 1].GetHeight();
    return S_OK;
}

HRESULT CASCHTMLRenderer::put_Height(const double& dHeight)
{
    size_t lCount = m_pInternal->m_oDocument.m_arrPages.size();
    if (0 != lCount)
    {
        if (m_pInternal->m_bIsThumbnailsCreate)
        {
            m_pInternal->m_oThumbnail.put_Height(dHeight);
        }
        m_pInternal->m_oDocument.m_arrPages[lCount - 1].SetHeight(dHeight);
    }
}

HRESULT CASCHTMLRenderer::get_Width(double* dWidth)
{
    size_t nCount = m_pInternal->m_oDocument.m_arrPages.size();
    if (nCount > 0)
        *dWidth = m_pInternal->m_oDocument.m_arrPages[nCount - 1].GetWidth();
    return S_OK;
}

HRESULT CASCHTMLRenderer::put_Width(const double& dWidth)
{
    size_t lCount = m_pInternal->m_oDocument.m_arrPages.size();
    if (0 != lCount)
    {
        if (m_bIsThumbnailsCreate)
        {
            m_oThumbnail.put_Width(dWidth);
        }
        m_oDocument.m_arrPages[lCount - 1].SetWidth(dWidth);
    }

    return S_OK;
}

HRESULT CASCHTMLRenderer::get_DpiX(double* dDpiX)
{
    *dDpiX = m_pInternal->m_dDpiX;
    return S_OK;
}

HRESULT CASCHTMLRenderer::get_DpiY(double* dDpiY)
{
    *dDpiY = m_pInternal->m_dDpiY;
}

// pen --------------------------------------------------------------------------------------
HRESULT CASCHTMLRenderer::get_PenColor(LONG* lColor)
{
    *lColor = m_pInternal->m_oPen.Color;
    return S_OK;
}

HRESULT CASCHTMLRenderer::put_PenColor(const LONG& lColor)
{
    if (m_pInternal->m_bIsThumbnailsCreate)
    {
        m_pInternal->m_oThumbnail.put_PenColor(lColor);
    }
    m_pInternal->m_oPen.Color = lColor;
    return S_OK;
}

HRESULT CASCHTMLRenderer::get_PenAlpha(LONG* lAlpha)
{
    *lAlpha = m_pInternal->m_oPen.Alpha;
    return S_OK;
}

HRESULT CASCHTMLRenderer::put_PenAlpha(const LONG& lAlpha)
{
    if (m_pInternal->m_bIsThumbnailsCreate)
    {
        m_pInternal->m_oThumbnail.put_PenAlpha(lAlpha);
    }
    m_pInternal->m_oPen.Alpha = lAlpha;
    return S_OK;
}

HRESULT CASCHTMLRenderer::get_PenSize(double* dSize)
{
    *dSize = m_pInternal->m_oPen.Size;
    return S_OK;
}

HRESULT CASCHTMLRenderer::put_PenSize(const double& dSize)
{
    if (m_pInternal->m_bIsThumbnailsCreate)
    {
        m_pInternal->m_oThumbnail.put_PenSize(dSize);
    }
    m_pInternal->m_oPen.Size = dSize;
    return S_OK;
}

HRESULT CASCHTMLRenderer::get_PenDashStyle(BYTE* val)
{
    *val = m_pInternal->m_oPen.DashStyle;
    return S_OK;
}

HRESULT CASCHTMLRenderer::put_PenDashStyle(const BYTE& val)
{
    if (m_pInternal->m_bIsThumbnailsCreate)
    {
        m_pInternal->m_oThumbnail.put_PenDashStyle(val);
    }
    m_pInternal->m_oPen.DashStyle = val;
    return S_OK;
}

HRESULT CASCHTMLRenderer::get_PenLineStartCap(BYTE* val)
{
    *val = m_pInternal->m_oPen.LineStartCap;
    return S_OK;
}

HRESULT CASCHTMLRenderer::put_PenLineStartCap(const BYTE& val)
{
    if (m_pInternal->m_bIsThumbnailsCreate)
    {
        m_pInternal->m_oThumbnail.put_PenLineStartCap(val);
    }
    m_pInternal->m_oPen.LineStartCap = val;
    return S_OK;
}

HRESULT CASCHTMLRenderer::get_PenLineEndCap(BYTE* val)
{
    *val = m_pInternal->m_oPen.LineEndCap;
    return S_OK;
}

HRESULT CASCHTMLRenderer::put_PenLineEndCap(const BYTE& val)
{
    if (m_pInternal->m_bIsThumbnailsCreate)
    {
        m_pInternal->m_oThumbnail.put_PenLineEndCap(val);
    }
    m_pInternal->m_oPen.LineEndCap = val;
    return S_OK;
}

HRESULT CASCHTMLRenderer::get_PenLineJoin(BYTE* val)
{
    *val = m_pInternal->m_oPen.LineJoin;
    return S_OK;
}

HRESULT CASCHTMLRenderer::put_PenLineJoin(const BYTE& val)
{
    if (m_pInternal->m_bIsThumbnailsCreate)
    {
        m_pInternal->m_oThumbnail.put_PenLineJoin(val);
    }
    m_pInternal->m_oPen.LineJoin = val;
    return S_OK;
}

HRESULT CASCHTMLRenderer::get_PenDashOffset(double* dOffset)
{
    *val = m_pInternal->m_oPen.DashOffset;
    return S_OK;
}

HRESULT CASCHTMLRenderer::put_PenDashOffset(const double& dOffset)
{
    if (m_pInternal->m_bIsThumbnailsCreate)
    {
        m_pInternal->m_oThumbnail.put_PenDashOffset(val);
    }
    m_pInternal->m_oPen.DashOffset = val;
    return S_OK;
}

HRESULT CASCHTMLRenderer::get_PenAlign(LONG* lAlign)
{
    *val = m_pInternal->m_oPen.Align;
    return S_OK;
}

HRESULT CASCHTMLRenderer::put_PenAlign(const LONG& lAlign)
{
    if (m_pInternal->m_bIsThumbnailsCreate)
    {
        m_pInternal->m_oThumbnail.put_PenAlign(val);
    }
    m_pInternal->m_oPen.Align = val;
    return S_OK;
}

HRESULT CASCHTMLRenderer::get_PenMiterLimit(double* dOffset)
{
    *val = m_pInternal->m_oPen.MiterLimit;
    return S_OK;
}

HRESULT CASCHTMLRenderer::put_PenMiterLimit(const double& dOffset)
{
    if (m_pInternal->m_bIsThumbnailsCreate)
    {
        m_pInternal->m_oThumbnail.put_PenMiterLimit(val);
    }
    m_pInternal->m_oPen.MiterLimit = val;
    return S_OK;
}

HRESULT CASCHTMLRenderer::PenDashPattern(double* pPattern, LONG lCount)
{
    if (m_pInternal->m_bIsThumbnailsCreate)
    {
        m_pInternal->m_oThumbnail.PenDashPattern(pPattern);
    }

    if (NULL != pPattern)
    {
        m_pInternal->m_oPen.SetDashPattern(pPattern, lCount);
    }

    return S_OK;
}

// brush ------------------------------------------------------------------------------------
HRESULT CASCHTMLRenderer::get_BrushType(LONG* lType)
{
    *lType = m_pInternal->m_oBrush.Type;
    return S_OK;
}

HRESULT CASCHTMLRenderer::put_BrushType(const LONG& lType)
{
    if (m_pInternal->m_bIsThumbnailsCreate)
    {
        m_pInternal->m_oThumbnail.put_BrushType(lType);
    }
    m_pInternal->m_oBrush.Type = lType;
    return S_OK;
}

HRESULT CASCHTMLRenderer::get_BrushColor1(LONG* lColor)
{
    *lColor = m_pInternal->m_oBrush.Color1;
    return S_OK;
}

HRESULT CASCHTMLRenderer::put_BrushColor1(const LONG& lColor)
{
    if (m_pInternal->m_bIsThumbnailsCreate)
    {
        m_pInternal->m_oThumbnail.put_BrushColor1(lColor);
    }
    m_pInternal->m_oBrush.Color1 = lColor;
    return S_OK;
}

HRESULT CASCHTMLRenderer::get_BrushAlpha1(LONG* lAlpha)
{
    *lAlpha = m_pInternal->m_oBrush.Alpha1;
    return S_OK;
}

HRESULT CASCHTMLRenderer::put_BrushAlpha1(const LONG& lAlpha)
{
    if (m_pInternal->m_bIsThumbnailsCreate)
    {
        m_pInternal->m_oThumbnail.put_BrushAlpha1(lAlpha);
    }
    m_pInternal->m_oBrush.Alpha1 = lAlpha;
    return S_OK;
}

HRESULT CASCHTMLRenderer::get_BrushColor2(LONG* lColor)
{
    *lColor = m_pInternal->m_oBrush.Color2;
    return S_OK;
}

HRESULT CASCHTMLRenderer::put_BrushColor2(const LONG& lColor)
{
    if (m_pInternal->m_bIsThumbnailsCreate)
    {
        m_pInternal->m_oThumbnail.put_BrushColor2(lColor);
    }
    m_pInternal->m_oBrush.Color2 = lColor;
    return S_OK;
}

HRESULT CASCHTMLRenderer::get_BrushAlpha2(LONG* lAlpha)
{
    *lAlpha = m_pInternal->m_oBrush.Alpha2;
    return S_OK;
}

HRESULT CASCHTMLRenderer::put_BrushAlpha2(const LONG& lAlpha)
{
    if (m_pInternal->m_bIsThumbnailsCreate)
    {
        m_pInternal->m_oThumbnail.put_BrushAlpha2(lAlpha);
    }
    m_pInternal->m_oBrush.Alpha2 = lAlpha;
    return S_OK;
}

HRESULT CASCHTMLRenderer::get_BrushTexturePath(std::wstring* bsPath)
{
    *bsPath = m_pInternal->m_oBrush.TexturePath;
    return S_OK;
}

HRESULT CASCHTMLRenderer::put_BrushTexturePath(const std::wstring& bsPath)
{
    if (m_pInternal->m_bIsThumbnailsCreate)
    {
        m_pInternal->m_oThumbnail.put_BrushTexturePath(bsPath);
    }
    m_pInternal->m_oBrush.TexturePath = bsPath;
    return S_OK;
}

HRESULT CASCHTMLRenderer::get_BrushTextureMode(LONG* lMode)
{
    *lMode = m_pInternal->m_oBrush.TextureMode;
    return S_OK;
}

HRESULT CASCHTMLRenderer::put_BrushTextureMode(const LONG& lMode)
{
    if (m_pInternal->m_bIsThumbnailsCreate)
    {
        m_pInternal->m_oThumbnail.put_BrushTextureMode(lMode);
    }
    m_pInternal->m_oBrush.TextureMode = lMode;
    return S_OK;
}

HRESULT CASCHTMLRenderer::get_BrushTextureAlpha(LONG* lTxAlpha)
{
    *lTxAlpha = m_pInternal->m_oBrush.TextureAlpha;
    return S_OK;
}

HRESULT CASCHTMLRenderer::put_BrushTextureAlpha(const LONG& lTxAlpha)
{
    if (m_pInternal->m_bIsThumbnailsCreate)
    {
        m_pInternal->m_oThumbnail.put_BrushTextureAlpha(lTxAlpha);
    }
    m_pInternal->m_oBrush.TextureAlpha = lTxAlpha;
    return S_OK;
}

HRESULT CASCHTMLRenderer::get_BrushLinearAngle(double* dAngle)
{
    *dAngle = m_pInternal->m_oBrush.LinearAngle;
    return S_OK;
}

HRESULT CASCHTMLRenderer::put_BrushLinearAngle(const double& dAngle)
{
    if (m_pInternal->m_bIsThumbnailsCreate)
    {
        m_pInternal->m_oThumbnail.put_BrushLinearAngle(dAngle);
    }
    m_pInternal->m_oBrush.LinearAngle = dAngle;
    return S_OK;
}

HRESULT CASCHTMLRenderer::BrushRect(const INT& val, const double& left, const double& top, const double& width, const double& height)
{
    if (m_pInternal->m_bIsThumbnailsCreate)
    {
        m_pInternal->m_oThumbnail.BrushRect(val, left, top, width, height);
    }

    m_pInternal->m_oBrush.Rectable = val;
    m_pInternal->m_oBrush.Rect.X = (float)left;
    m_pInternal->m_oBrush.Rect.Y = (float)top;
    m_pInternal->m_oBrush.Rect.Width  = (float)width;
    m_pInternal->m_oBrush.Rect.Height = (float)height;

    return S_OK;
}

HRESULT CASCHTMLRenderer::BrushBounds(const double& left, const double& top, const double& width, const double& height)
{
    if (m_pInternal->m_bIsThumbnailsCreate)
    {
        m_pInternal->m_oThumbnail.BrushBounds(left, top, width, height);
    }

    m_pInternal->m_oBrush.Bounds.left	= left;
    m_pInternal->m_oBrush.Bounds.top	= top;
    m_pInternal->m_oBrush.Bounds.right	= left + width;
    m_pInternal->m_oBrush.Bounds.bottom	= top + height;

    return S_OK;
}

HRESULT CASCHTMLRenderer::put_BrushGradientColors(LONG* lColors, double* pPositions, LONG nCount)
{
    if (m_pInternal->m_bIsThumbnailsCreate)
    {
        m_pInternal->m_oThumbnail.put_BrushGradientColors(lColors, pPositions, nCount);
    }

    m_pInternal->m_oBrush.m_arrSubColors.RemoveAll();
    for (LONG i = 0; i < nCount; ++i)
    {
        NSStructures::CBrush::TSubColor color;
        color.color		= lColors[i];
        color.position	= (long)(pPositions[i] * 65536);
        m_pInternal->m_oBrush.m_arrSubColors.Add(color);
    }

    return S_OK;
}

// font -------------------------------------------------------------------------------------
HRESULT CASCHTMLRenderer::get_FontName(std::wstring* bsName)
{
    *bsName = m_pInternal->m_oFont.Name;
    return S_OK;
}

HRESULT CASCHTMLRenderer::put_FontName(const std::wstring& bsName)
{
    if (m_pInternal->m_bIsThumbnailsCreate)
    {
        m_pInternal->m_oThumbnail.put_FontName(bsName);
    }
    m_pInternal->m_oFont.Name = bsName;
    return S_OK;
}

HRESULT CASCHTMLRenderer::get_FontPath(std::wstring* bsName)
{
    *bsName = m_pInternal->m_oFont.Path;
    return S_OK;
}

HRESULT CASCHTMLRenderer::put_FontPath(const std::wstring& bsName)
{
    if (m_pInternal->m_bIsThumbnailsCreate)
    {
        m_pInternal->m_oThumbnail.put_FontPath(bsName);
    }
    m_pInternal->m_oFont.Path = bsName;
    return S_OK;
}

HRESULT CASCHTMLRenderer::get_FontSize(double* dSize)
{
    *dSize = m_pInternal->m_oFont.Size;
    return S_OK;
}

HRESULT CASCHTMLRenderer::put_FontSize(const double& dSize)
{
    if (m_pInternal->m_bIsThumbnailsCreate)
    {
        m_pInternal->m_oThumbnail.put_FontSize(dSize);
    }
    m_pInternal->m_oFont.Size = dSize;
    return S_OK;
}

HRESULT CASCHTMLRenderer::get_FontStyle(LONG* lStyle)
{
    *lStyle = m_pInternal->m_oFont.GetStyle();
    return S_OK;
}

HRESULT CASCHTMLRenderer::put_FontStyle(const LONG& lStyle)
{
    if (m_pInternal->m_bIsThumbnailsCreate)
    {
        m_pInternal->m_oThumbnail.put_FontStyle(lStyle);
    }
    m_pInternal->m_oFont.SetStyle(lStyle);
    return S_OK;
}

HRESULT CASCHTMLRenderer::get_FontStringGID(INT* bGID)
{
    *bGID = m_pInternal->m_oFont.StringGID;
    return S_OK;
}

HRESULT CASCHTMLRenderer::put_FontStringGID(const INT& bGID)
{
    if (m_pInternal->m_bIsThumbnailsCreate)
    {
        m_pInternal->m_oThumbnail.put_FontStringGID(bGID);
    }
    m_pInternal->m_oFont.StringGID = bGID;
    m_pInternal->m_pFontManager->SetStringGID(bGID);
    return S_OK;
}

HRESULT CASCHTMLRenderer::get_FontCharSpace(double* dSpace)
{
    *dSpace = m_pInternal->m_oFont.CharSpace;
    return S_OK;
}

HRESULT CASCHTMLRenderer::put_FontCharSpace(const double& dSpace)
{
    if (m_pInternal->m_bIsThumbnailsCreate)
    {
        m_pInternal->m_oThumbnail.put_FontCharSpace(dSpace);
    }
    m_pInternal->m_oFont.CharSpace = dSpace;
    return S_OK;
}

HRESULT CASCHTMLRenderer::get_FontFaceIndex(int* lFaceIndex)
{
    *lFaceIndex = m_pInternal->m_oFont.FaceIndex;
    return S_OK;
}

HRESULT CASCHTMLRenderer::put_FontFaceIndex(const int& lFaceIndex)
{
    if (m_bIsThumbnailsCreate)
    {
        m_oThumbnail.put_FontCharSpace(dSpace);
    }
    m_oFont.CharSpace = dSpace;
    return S_OK;
}

//-------- Функции для вывода текста --------------------------------------------------------
HRESULT CASCHTMLRenderer::CommandDrawTextCHAR(const LONG& c, const double& x, const double& y, const double& w, const double& h);
HRESULT CASCHTMLRenderer::CommandDrawText(const std::wstring& bsText, const double& x, const double& y, const double& w, const double& h);

HRESULT CASCHTMLRenderer::CommandDrawTextExCHAR(const LONG& c, const LONG& gid, const double& x, const double& y, const double& w, const double& h);
HRESULT CASCHTMLRenderer::CommandDrawTextEx(const std::wstring& bsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& x, const double& y, const double& w, const double& h);

//-------- Маркеры для команд ---------------------------------------------------------------
HRESULT CASCHTMLRenderer::BeginCommand(const DWORD& lType);
HRESULT CASCHTMLRenderer::EndCommand(const DWORD& lType);

//-------- Функции для работы с Graphics Path -----------------------------------------------
HRESULT CASCHTMLRenderer::PathCommandMoveTo(const double& x, const double& y);
HRESULT CASCHTMLRenderer::PathCommandLineTo(const double& x, const double& y);
HRESULT CASCHTMLRenderer::PathCommandLinesTo(double* points, const int& count);
HRESULT CASCHTMLRenderer::PathCommandCurveTo(const double& x1, const double& y1, const double& x2, const double& y2, const double& x3, const double& y3);
HRESULT CASCHTMLRenderer::PathCommandCurvesTo(double* points, const int& count);
HRESULT CASCHTMLRenderer::PathCommandArcTo(const double& x, const double& y, const double& w, const double& h, const double& startAngle, const double& sweepAngle);
HRESULT CASCHTMLRenderer::PathCommandClose();
HRESULT CASCHTMLRenderer::PathCommandEnd();
HRESULT CASCHTMLRenderer::DrawPath(const LONG& nType);
HRESULT CASCHTMLRenderer::PathCommandStart();
HRESULT CASCHTMLRenderer::PathCommandGetCurrentPoint(double* x, double* y);

HRESULT CASCHTMLRenderer::PathCommandTextCHAR(const LONG& c, const double& x, const double& y, const double& w, const double& h);
HRESULT CASCHTMLRenderer::PathCommandText(const std::wstring& bsText, const double& x, const double& y, const double& w, const double& h);

HRESULT CASCHTMLRenderer::PathCommandTextExCHAR(const LONG& c, const LONG& gid, const double& x, const double& y, const double& w, const double& h);
HRESULT CASCHTMLRenderer::PathCommandTextEx(const std::wstring& bsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& x, const double& y, const double& w, const double& h);

//-------- Функции для вывода изображений ---------------------------------------------------
HRESULT CASCHTMLRenderer::DrawImage(IGrObject* pImage, const double& x, const double& y, const double& w, const double& h);
HRESULT CASCHTMLRenderer::DrawImageFromFile(const std::wstring&, const double& x, const double& y, const double& w, const double& h, const BYTE& lAlpha = 255);

// transform --------------------------------------------------------------------------------
HRESULT CASCHTMLRenderer::SetTransform(const double& m1, const double& m2, const double& m3, const double& m4, const double& m5, const double& m6);
HRESULT CASCHTMLRenderer::GetTransform(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF);
HRESULT CASCHTMLRenderer::ResetTransform();

// -----------------------------------------------------------------------------------------
HRESULT CASCHTMLRenderer::get_ClipMode(LONG* plMode);
HRESULT CASCHTMLRenderer::put_ClipMode(const LONG& lMode);

// additiaonal params ----------------------------------------------------------------------
HRESULT CASCHTMLRenderer::CommandLong(const LONG& lType, const LONG& lCommand);
HRESULT CASCHTMLRenderer::CommandDouble(const LONG& lType, const double& dCommand);
HRESULT CASCHTMLRenderer::CommandString(const LONG& lType, const std::wstring& sCommand);

// owner params ----------------------------------------------------------------------
HRESULT CASCHTMLRenderer::get_Mode(LONG *plMode);
HRESULT CASCHTMLRenderer::put_Mode(LONG lMode);

HRESULT CASCHTMLRenderer::CreateOfficeFile(std::wstring bsFileName);
HRESULT CASCHTMLRenderer::CloseFile();

HRESULT CASCHTMLRenderer::SetAdditionalParam(std::string sParamName, int nValue);
