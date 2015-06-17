#include "PdfRenderer.h"

#include "Src/Document.h"
#include "Src/Pages.h"

#define MM_2_PT(X) (X * 25.4 / 72.0)
#define PT_2_MM(X) (X * 72.0 / 25.4)

using namespace PdfWriter;

CPdfRenderer::CPdfRenderer()
{
	m_pDocument = new CDocument();
	if (!m_pDocument || !m_pDocument->CreateNew())
	{
		SetError();
		return;
	}

	m_dPageHeight = 297;
	m_dPageWidth  = 210;
	m_pPage       = NULL;
}
CPdfRenderer::~CPdfRenderer()
{
	if (m_pDocument)
		delete m_pDocument;
}
//----------------------------------------------------------------------------------------
// Тип рендерера
//----------------------------------------------------------------------------------------
HRESULT CPdfRenderer::get_Type(LONG* lType)
{
	*lType = c_nPDFWriter;
	return S_OK;
}
//----------------------------------------------------------------------------------------
// Функции для работы со страницей
//----------------------------------------------------------------------------------------
HRESULT CPdfRenderer::NewPage()
{
	if (!IsValid())
		return S_FALSE;

	m_pPage = m_pDocument->AddPage();

	if (!m_pPage)
	{
		SetError();
		return S_FALSE;
	}

	m_pPage->SetWidth(m_dPageWidth);
	m_pPage->SetHeight(m_dPageHeight);

	m_oPen.Reset();

	return S_OK;
}
HRESULT CPdfRenderer::get_Height(double* dHeight)
{
	*dHeight = m_dPageHeight;
	return S_OK;
}
HRESULT CPdfRenderer::put_Height(const double& dHeight)
{
	if (!IsValid() || !m_pPage)
		return S_FALSE;

	m_dPageHeight = dHeight;
	m_pPage->SetHeight(MM_2_PT(dHeight));
	return S_OK;
}
HRESULT CPdfRenderer::get_Width(double* dWidth)
{
	*dWidth = m_dPageWidth;
	return S_OK;
}
HRESULT CPdfRenderer::put_Width(const double& dWidth)
{
	if (!IsValid() || !m_pPage)
		return S_FALSE;

	m_dPageWidth = dWidth;
	m_pPage->SetWidth(MM_2_PT(dWidth));
	return S_OK;
}
HRESULT CPdfRenderer::get_DpiX(double* dDpiX)
{
	*dDpiX = 72;
	return S_OK;
}
HRESULT CPdfRenderer::get_DpiY(double* dDpiY)
{
	*dDpiY = 72;
	return S_OK;
}
//----------------------------------------------------------------------------------------
// Функции для работы с Pen
//----------------------------------------------------------------------------------------
HRESULT CPdfRenderer::get_PenColor(LONG* lColor)
{
	*lColor = m_oPen.GetColor();
	return S_OK;
}
HRESULT CPdfRenderer::put_PenColor(const LONG& lColor)
{
	m_oPen.SetColor(lColor);
	return S_OK;
}
HRESULT CPdfRenderer::get_PenAlpha(LONG* lAlpha)
{
	*lAlpha = m_oPen.GetAlpha();
	return S_OK;
}
HRESULT CPdfRenderer::put_PenAlpha(const LONG& lAlpha)
{
	m_oPen.SetAlpha(lAlpha);
	return S_OK;
}
HRESULT CPdfRenderer::get_PenSize(double* dSize)
{
	*dSize = m_oPen.GetSize();
	return S_OK;
}
HRESULT CPdfRenderer::put_PenSize(const double& dSize)
{
	m_oPen.SetSize(dSize);
	return S_OK;
}
HRESULT CPdfRenderer::get_PenDashStyle(BYTE* nDashStyle)
{
	*nDashStyle = m_oPen.GetDashStyle();
	return S_OK;
}
HRESULT CPdfRenderer::put_PenDashStyle(const BYTE& nDashStyle)
{
	m_oPen.SetDashStyle(nDashStyle);
	return S_OK;
}
HRESULT CPdfRenderer::get_PenLineStartCap(BYTE* nCapStyle)
{
	*nCapStyle = m_oPen.GetStartCapStyle();
	return S_OK;
}
HRESULT CPdfRenderer::put_PenLineStartCap(const BYTE& nCapStyle)
{
	m_oPen.SetStartCapStyle(nCapStyle);
	return S_OK;
}
HRESULT CPdfRenderer::get_PenLineEndCap(BYTE* nCapStyle)
{
	*nCapStyle = m_oPen.GetEndCapStyle();
	return S_OK;
}
HRESULT CPdfRenderer::put_PenLineEndCap(const BYTE& nCapStyle)
{
	m_oPen.SetEndCapStyle(nCapStyle);
	return S_OK;
}
HRESULT CPdfRenderer::get_PenLineJoin(BYTE* nJoinStyle)
{
	*nJoinStyle = m_oPen.GetJoinStyle();
	return S_OK;
}
HRESULT CPdfRenderer::put_PenLineJoin(const BYTE& nJoinStyle)
{
	m_oPen.SetJoinStyle(nJoinStyle);
	return S_OK;
}
HRESULT CPdfRenderer::get_PenDashOffset(double* dOffset)
{
	*dOffset = m_oPen.GetDashOffset();
	return S_OK;
}
HRESULT CPdfRenderer::put_PenDashOffset(const double& dOffset)
{
	m_oPen.SetDashOffset(dOffset);
	return S_OK;
}
HRESULT CPdfRenderer::get_PenAlign(LONG* lAlign)
{
	*lAlign = m_oPen.GetAlign();
	return S_OK;
}
HRESULT CPdfRenderer::put_PenAlign(const LONG& lAlign)
{
	m_oPen.SetAlign(lAlign);
	return S_OK;
}
HRESULT CPdfRenderer::get_PenMiterLimit(double* dMiter)
{
	*dMiter = m_oPen.GetMiter();
	return S_OK;
}
HRESULT CPdfRenderer::put_PenMiterLimit(const double& dMiter)
{
	m_oPen.SetMiter(dMiter);
	return S_OK;
}
HRESULT CPdfRenderer::PenDashPattern(double* pPattern, LONG lCount)
{
	m_oPen.SetDashPattern(pPattern, lCount);
	return S_OK;
}
//----------------------------------------------------------------------------------------
// Функции для работы с Brush
//----------------------------------------------------------------------------------------
HRESULT CPdfRenderer::get_BrushType(LONG* lType)
{
	return S_OK;
}
HRESULT CPdfRenderer::put_BrushType(const LONG& lType)					= 0;
HRESULT CPdfRenderer::get_BrushColor1(LONG* lColor)						= 0;
HRESULT CPdfRenderer::put_BrushColor1(const LONG& lColor)					= 0;
HRESULT CPdfRenderer::get_BrushAlpha1(LONG* lAlpha)						= 0;
HRESULT CPdfRenderer::put_BrushAlpha1(const LONG& lAlpha)					= 0;
HRESULT CPdfRenderer::get_BrushColor2(LONG* lColor)						= 0;
HRESULT CPdfRenderer::put_BrushColor2(const LONG& lColor)					= 0;
HRESULT CPdfRenderer::get_BrushAlpha2(LONG* lAlpha)						= 0;
HRESULT CPdfRenderer::put_BrushAlpha2(const LONG& lAlpha)					= 0;
HRESULT CPdfRenderer::get_BrushTexturePath(std::wstring* bsPath)			= 0;
HRESULT CPdfRenderer::put_BrushTexturePath(const std::wstring& bsPath)	= 0;
HRESULT CPdfRenderer::get_BrushTextureMode(LONG* lMode)					= 0;
HRESULT CPdfRenderer::put_BrushTextureMode(const LONG& lMode)				= 0;
HRESULT CPdfRenderer::get_BrushTextureAlpha(LONG* lTxAlpha)				= 0;
HRESULT CPdfRenderer::put_BrushTextureAlpha(const LONG& lTxAlpha)			= 0;
HRESULT CPdfRenderer::get_BrushLinearAngle(double* dAngle)				= 0;
HRESULT CPdfRenderer::put_BrushLinearAngle(const double& dAngle)			= 0;
HRESULT CPdfRenderer::BrushRect(const INT& val, const double& left, const double& top, const double& width, const double& height) = 0;
HRESULT CPdfRenderer::BrushBounds(const double& left, const double& top, const double& width, const double& height) = 0;
HRESULT CPdfRenderer::put_BrushGradientColors(LONG* lColors, double* pPositions, LONG nCount) = 0;
