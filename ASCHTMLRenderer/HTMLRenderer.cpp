#include "stdafx.h"
#include "HTMLRenderer.h"
#include "Resource.h"
#include "math.h"
#include "..\Common\TimeMeasurer.h"

CASCHTMLRenderer::CASCHTMLRenderer()
{
}

HRESULT CASCHTMLRenderer::FinalConstruct()
{
	m_lLastSavedPage = 0;
	m_oDocument.SetUpdater(this);

	m_dDpiX = 96.0;
	m_dDpiY	= 96.0;

	m_lClipMode = c_nClipRegionTypeWinding;

	m_oWriter.m_dDpiX	= m_dDpiX;
	m_oWriter.m_dDpiY	= m_dDpiY;

	m_oWriter.m_pPen	= &m_oPen;
	m_oWriter.m_pBrush	= &m_oBrush;
	m_oWriter.m_pFont	= &m_oFont;

	m_pSimpleGraphicsConverter = NULL;
	CoCreateInstance(Graphics::CLSID_CASCGraphicSimpleComverter, NULL, CLSCTX_ALL, Graphics::IID_IASCGraphicSimpleComverter, (void**)&m_pSimpleGraphicsConverter);

	IUnknown* punkRenderer = NULL;
	this->QueryInterface(IID_IUnknown, (void**)&punkRenderer);

	m_pSimpleGraphicsConverter->put_Renderer(punkRenderer);
	RELEASEINTERFACE(punkRenderer);

	m_bPageClosed = TRUE;

	m_dTransformAngle = 0.0;

	m_pFonts = NULL;
	m_pFontManager = NULL;

	m_oWriter.SetSimpleConverter(m_pSimpleGraphicsConverter, &m_oFullTransform);
	m_oWriter.m_oPage.SetSettings(&m_oPen, &m_oBrush, &m_oFont, m_oWriter.GetStyles(), &m_oTransform);

	m_bIsThumbnailsCreate = FALSE;
	m_bIsWEB			  = FALSE;
	m_bIsMenu			  = FALSE;

	m_bIsMetafileDrawing = false;
	m_bIsTextGraphicType = false;

#ifdef BUILD_SUPPORT_UNREGISTERED_WATERMARK
	m_pEffectPainter	= NULL;
#endif

	return S_OK;
}

void CASCHTMLRenderer::FinalRelease()
{
	RELEASEINTERFACE(m_pSimpleGraphicsConverter);
	RELEASEINTERFACE(m_pFonts);

#ifdef BUILD_SUPPORT_UNREGISTERED_WATERMARK
	RELEASEINTERFACE(m_pEffectPainter);
#endif
}

STDMETHODIMP CASCHTMLRenderer::get_Type(LONG* lType)
{
	if (NULL == lType)
		return S_FALSE;

	*lType = c_nHtmlRendrerer;
	return S_OK;
}
//-------- Функции для работы со страницей --------------------------------------------------
STDMETHODIMP CASCHTMLRenderer::NewPage()
{
	if (m_bPageOpened)
	{
		EndPage();
	}

	m_oDocument.NewPage();
	m_bPageClosed = FALSE;
	m_bPageOpened = TRUE;

	m_bIsMetafileDrawing = false;
	m_bIsTextGraphicType = false;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_Height(double* dHeight)
{
	size_t nCount = m_oDocument.m_arrPages.GetCount();
	if ((nCount > 0) && (NULL != dHeight))
	{
		*dHeight = m_oDocument.m_arrPages[nCount - 1].GetHeight();	
	}
	
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_Height(double dHeight)
{
	LONG lCount = (LONG)m_oDocument.m_arrPages.GetCount();
	if (0 != lCount)
	{
		if (m_bIsThumbnailsCreate)
		{
			m_oThumbnail.put_Height(dHeight);
		}
		m_oDocument.m_arrPages[lCount - 1].SetHeight(dHeight);
	}

	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_Width(double* dWidth)
{
	size_t nCount = m_oDocument.m_arrPages.GetCount();
	if ((nCount > 0) && (NULL != dWidth))
	{
		*dWidth = m_oDocument.m_arrPages[nCount - 1].GetWidth();	
	}

	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_Width(double dWidth)
{
	LONG lCount = (LONG)m_oDocument.m_arrPages.GetCount();
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

STDMETHODIMP CASCHTMLRenderer::get_DpiX(double* dDpiX)
{
	if (NULL != dDpiX)
		*dDpiX = m_dDpiX;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_DpiY(double* dDpiY)
{
	if (NULL != dDpiY)
		*dDpiY = m_dDpiY;
	return S_OK;
}
// pen --------------------------------------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer::SetPen(BSTR bsXML)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.SetPen(bsXML);
	}
	m_oPen.FromXmlString((CString)bsXML);
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_PenColor(LONG* lColor)
{
	*lColor = m_oPen.Color;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_PenColor(LONG lColor)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.put_PenColor(lColor);
	}
	m_oPen.Color = lColor;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_PenAlpha(LONG* lAlpha)
{
	*lAlpha = m_oPen.Alpha;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_PenAlpha(LONG lAlpha)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.put_PenAlpha(lAlpha);
	}
	m_oPen.Alpha = lAlpha;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_PenSize(double* dSize)
{
	*dSize = m_oPen.Size;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_PenSize(double dSize)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.put_PenSize(dSize);
	}
	m_oPen.Size = dSize;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_PenDashStyle(BYTE* val)
{
	*val = m_oPen.DashStyle;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_PenDashStyle(BYTE val)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.put_PenDashStyle(val);
	}
	m_oPen.DashStyle = val;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_PenLineStartCap(BYTE* val)
{
	*val = m_oPen.LineStartCap;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_PenLineStartCap(BYTE val)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.put_PenLineStartCap(val);
	}
	m_oPen.LineStartCap = val;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_PenLineEndCap(BYTE* val)
{
	*val = m_oPen.LineEndCap;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_PenLineEndCap(BYTE val)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.put_PenLineEndCap(val);
	}
	m_oPen.LineEndCap = val;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_PenLineJoin(BYTE* val)
{
	*val = m_oPen.LineJoin;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_PenLineJoin(BYTE val)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.put_PenLineJoin(val);
	}
	m_oPen.LineJoin = val;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_PenDashOffset(double* val)
{
	*val = m_oPen.DashOffset;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_PenDashOffset(double val)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.put_PenDashOffset(val);
	}
	m_oPen.DashOffset = val;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_PenAlign(LONG* val)
{
	*val = m_oPen.Align;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_PenAlign(LONG val)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.put_PenAlign(val);
	}
	m_oPen.Align = val;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_PenMiterLimit(double* val)
{
	*val = m_oPen.MiterLimit;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_PenMiterLimit(double val)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.put_PenMiterLimit(val);
	}
	m_oPen.MiterLimit = val;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::PenDashPattern(SAFEARRAY* pPattern)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.PenDashPattern(pPattern);
	}
	
	if (NULL != pPattern)
	{
		m_oPen.SetDashPattern((double*)pPattern->pvData, pPattern->rgsabound[0].cElements);
	}

	return S_OK;
}
// brush ------------------------------------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer::SetBrush(BSTR bsXML)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.SetBrush(bsXML);
	}
	m_oBrush.FromXmlString((CString)bsXML);
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_BrushType(LONG* lType)
{
	*lType = m_oBrush.Type;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_BrushType(LONG lType)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.put_BrushType(lType);
	}
	m_oBrush.Type = lType;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_BrushColor1(LONG* lColor)
{
	*lColor = m_oBrush.Color1;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_BrushColor1(LONG lColor)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.put_BrushColor1(lColor);
	}
	m_oBrush.Color1 = lColor;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_BrushAlpha1(LONG* lAlpha)
{
	*lAlpha = m_oBrush.Alpha1;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_BrushAlpha1(LONG lAlpha)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.put_BrushAlpha1(lAlpha);
	}
	m_oBrush.Alpha1 = lAlpha;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_BrushColor2(LONG* lColor)
{
	*lColor = m_oBrush.Color2;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_BrushColor2(LONG lColor)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.put_BrushColor2(lColor);
	}
	m_oBrush.Color2 = lColor;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_BrushAlpha2(LONG* lAlpha)
{
	*lAlpha = m_oBrush.Alpha2;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_BrushAlpha2(LONG lAlpha)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.put_BrushAlpha2(lAlpha);
	}
	m_oBrush.Alpha2 = lAlpha;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_BrushTexturePath(BSTR* bsPath)
{
	*bsPath = m_oBrush.TexturePath.AllocSysString();
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_BrushTexturePath(BSTR bsPath)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.put_BrushTexturePath(bsPath);
	}
	m_oBrush.TexturePath = (CString)bsPath;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_BrushTextureMode(LONG* lMode)
{
	*lMode = m_oBrush.TextureMode;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_BrushTextureMode(LONG lMode)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.put_BrushTextureMode(lMode);
	}
	m_oBrush.TextureMode = lMode;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_BrushTextureAlpha(LONG* lTxAlpha)
{
	*lTxAlpha = m_oBrush.TextureAlpha;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_BrushTextureAlpha(LONG lTxAlpha)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.put_BrushTextureAlpha(lTxAlpha);
	}
	m_oBrush.TextureAlpha = lTxAlpha;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_BrushLinearAngle(double* dAngle)
{
	*dAngle = m_oBrush.LinearAngle;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_BrushLinearAngle(double dAngle)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.put_BrushLinearAngle(dAngle);
	}
	m_oBrush.LinearAngle = dAngle;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::BrushRect(BOOL val, double left, double top, double width, double height)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.BrushRect(val, left, top, width, height);
	}
	
	m_oBrush.Rectable = val;
	m_oBrush.Rect.X = (float)left;
	m_oBrush.Rect.Y = (float)top;
	m_oBrush.Rect.Width  = (float)width;
	m_oBrush.Rect.Height = (float)height;	

	return S_OK;
}
// font -------------------------------------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer::SetFont(BSTR bsXML)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.SetFont(bsXML);
	}
	m_oFont.FromXmlString((CString)bsXML);
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_FontName(BSTR* bsName)
{
	*bsName = m_oFont.Name.AllocSysString();
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_FontName(BSTR bsName)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.put_FontName(bsName);
	}
	m_oFont.Name = (CString)bsName;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_FontPath(BSTR* bsName)
{
	*bsName = m_oFont.Path.AllocSysString();
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_FontPath(BSTR bsName)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.put_FontPath(bsName);
	}
	m_oFont.Path = bsName;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_FontSize(double* dSize)
{
	*dSize = m_oFont.Size;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_FontSize(double dSize)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.put_FontSize(dSize);
	}
	m_oFont.Size = dSize;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_FontStyle(LONG* lStyle)
{
	*lStyle = m_oFont.GetStyle();
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_FontStyle(LONG lStyle)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.put_FontStyle(lStyle);
	}
	m_oFont.SetStyle(lStyle);
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_FontStringGID(BOOL* bGID)
{
	*bGID = m_oFont.StringGID;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_FontStringGID(BOOL bGID)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.put_FontStringGID(bGID);
	}
	m_oFont.StringGID = bGID;
	m_pFontManager->SetStringGID(bGID);
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_FontCharSpace(double* dSpace)
{
	*dSpace = m_oFont.CharSpace;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_FontCharSpace(double dSpace)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.put_FontCharSpace(dSpace);
	}
	m_oFont.CharSpace = dSpace;
	return S_OK;
}
// shadow -----------------------------------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer::SetShadow(BSTR bsXML)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.SetShadow(bsXML);
	}
	m_oShadow.FromXmlString((CString)bsXML);
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_ShadowDistanceX(double* val)
{
	*val = m_oShadow.DistanceX;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_ShadowDistanceX(double val)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.put_ShadowDistanceX(val);
	}
	m_oShadow.DistanceX = val;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_ShadowDistanceY(double* val)
{
	*val = m_oShadow.DistanceY;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_ShadowDistanceY(double val)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.put_ShadowDistanceY(val);
	}
	m_oShadow.DistanceY = val;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_ShadowBlurSize(double* val)
{
	*val = m_oShadow.BlurSize;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_ShadowBlurSize(double val)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.put_ShadowBlurSize(val);
	}
	m_oShadow.BlurSize = val;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_ShadowColor(LONG* val)
{
	*val = m_oShadow.Color;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_ShadowColor(LONG val)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.put_ShadowColor(val);
	}
	m_oShadow.Color = val;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_ShadowAlpha(LONG* val)
{
	*val = m_oShadow.Alpha;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_ShadowAlpha(LONG val)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.put_ShadowAlpha(val);
	}
	m_oShadow.Alpha = val;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_ShadowVisible(BOOL* val)
{
	*val = m_oShadow.Visible;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_ShadowVisible(BOOL val)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.put_ShadowVisible(val);
	}
	m_oShadow.Visible = val;
	return S_OK;
}
// edge -------------------------------------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer::SetEdgeText(BSTR bsXML)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.SetEdgeText(bsXML);
	}
	m_oEdge.FromXmlString((CString)bsXML);
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_EdgeVisible(LONG* val)
{
	*val = m_oEdge.Visible;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_EdgeVisible(LONG val)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.put_EdgeVisible(val);
	}
	m_oEdge.Visible = val;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_EdgeColor(LONG* val)
{
	*val = m_oEdge.Color;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_EdgeColor(LONG val)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.put_EdgeColor(val);
	}
	m_oEdge.Color = val;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_EdgeAlpha(LONG* val)
{
	*val = m_oEdge.Alpha;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_EdgeAlpha(LONG val)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.put_EdgeAlpha(val);
	}
	m_oEdge.Alpha = val;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_EdgeDist(double* val)
{
	*val = m_oEdge.Dist;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_EdgeDist(double val)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.put_EdgeDist(val);
	}
	m_oEdge.Dist = val;
	return S_OK;
}
//-------- Функции для вывода текста --------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer::CommandDrawText(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
{
	if (c_nHyperlinkType == m_lCurrentCommandType)
		return S_OK;

	if ((abs(m_dTransformAngle) > 1) || (m_bIsMetafileDrawing == true) || m_bIsWatermarkEnabled)
	{
		PathCommandEnd();
		BeginCommand(c_nPathType);
		PathCommandText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);
		DrawPath(c_nWindingFillMode);
		EndCommand(c_nPathType);
		PathCommandEnd();
		return S_OK;
	}
	
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.CommandDrawText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);
	}

	double x = fX;
	double y = fY;
	m_oFullTransform.TransformPoint(x, y);

	double r = fX + fWidth;
	double b = fY + fHeight;
	m_oFullTransform.TransformPoint(r, b);

	m_oWriter.WriteText(bsText, NULL, x, y, r - x, b - y, fBaseLineOffset);

	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::CommandDrawTextEx(BSTR bsText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
{
	if ((abs(m_dTransformAngle) > 1) || (m_bIsMetafileDrawing == true) || m_bIsWatermarkEnabled)
	{
		PathCommandEnd();
		BeginCommand(c_nPathType);
		PathCommandTextEx(bsText, bsGidText, bsSourceCodeText, fX, fY, fWidth, fHeight, fBaseLineOffset, lFlags);
		DrawPath(c_nWindingFillMode);
		EndCommand(c_nPathType);
		PathCommandEnd();
		return S_OK;
	}

	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.CommandDrawTextEx(bsText, bsGidText, bsSourceCodeText, fX, fY, fWidth, fHeight, fBaseLineOffset, lFlags);
	}

	double x = fX;
	double y = fY;
	m_oFullTransform.TransformPoint(x, y);

	double r = fX + fWidth;
	double b = fY + fHeight;
	m_oFullTransform.TransformPoint(r, b);

	m_oWriter.WriteText(bsText, bsGidText, x, y, r - x, b - y, fBaseLineOffset);
	return S_OK;
}
//-------- Маркеры для команд ---------------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer::BeginCommand(DWORD lType)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.BeginCommand(lType);
	}

	if (c_nClipType == lType)
	{
		m_oWriter.WritePathClip();
	}
	else if (c_nPathType == lType)
	{
		m_oWriter.WriteBeginPath();
		m_pSimpleGraphicsConverter->PathCommandEnd();
	}
	else if (c_nTableCell == lType)
	{
		m_oWriter.m_oPage.WriteNewTableBox();
	}
	else if (c_nImageType == lType)
	{
		m_bIsMetafileDrawing = true;
	}
	else if (c_nTextGraphicType == lType)
	{
		m_bIsTextGraphicType = true;
	}
	//else if (c_nParagraphType == lType)
	//{
	//	m_oWriter.m_oPage.m_oText.BeginParagraph();
	//}
	m_lCurrentCommandType = lType;

	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::EndCommand(DWORD lType)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.EndCommand(lType);
	}

	if (c_nPageType == lType)
	{
		EndPage();
	}
	else if (c_nClipType == lType)
	{
		m_oWriter.WritePathClipEnd();
	}
	else if (c_nResetClipType == lType)
	{
		m_oWriter.WritePathResetClip();
	}
	else if (c_nTableCell == lType)
	{
		m_oWriter.m_oPage.WriteNewTableBox();
	}
	//else if (c_nPathType == lType)
	//{
	//	PathCommandEnd();
	//}
	else if (c_nImageType == lType)
	{
		m_bIsMetafileDrawing = false;
	}
	else if (c_nTextGraphicType == lType)
	{
		m_bIsTextGraphicType = false;
	}
	else if (c_nNumberingMarker == lType)
	{
		// чтобы отступ точно сработал.
		m_oWriter.m_oPage.m_oText.m_bIsNewLine = true;
	}
	//else if (c_nParagraphType == lType)
	//{
	//	m_oWriter.m_oPage.m_oText.EndParagraph();
	//}
	m_lCurrentCommandType = -1;

	return S_OK;
}
//-------- Функции для работы с Graphics Path -----------------------------------------------
STDMETHODIMP CASCHTMLRenderer::PathCommandMoveTo(double fX, double fY)
{
	if (c_nSimpleGraphicType == m_lCurrentCommandType)
	{
		MoveTo(fX, fY);
	}
	else
	{
		m_pSimpleGraphicsConverter->PathCommandMoveTo(fX, fY);
		
		if (m_bIsThumbnailsCreate)
		{
			m_oThumbnail.PathCommandMoveTo(fX, fY);
		}
	}

	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::PathCommandLineTo(double fX, double fY)
{
	if (c_nSimpleGraphicType == m_lCurrentCommandType)
	{
		LineTo(fX, fY);
	}
	else
	{
		m_pSimpleGraphicsConverter->PathCommandLineTo(fX, fY);
		
		if (m_bIsThumbnailsCreate)
		{
			m_oThumbnail.PathCommandLineTo(fX, fY);
		}
	}

	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::PathCommandLinesTo(SAFEARRAY* pPoints)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.PathCommandLinesTo(pPoints);
	}
	m_pSimpleGraphicsConverter->PathCommandLinesTo(pPoints);
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::PathCommandCurveTo(double fX1, double fY1, double fX2, double fY2, double fX3, double fY3)
{
	if (c_nSimpleGraphicType == m_lCurrentCommandType)
	{
		CurveTo(fX1, fY1, fX2, fY2, fX3, fY3);
	}
	else
	{
		m_pSimpleGraphicsConverter->PathCommandCurveTo(fX1, fY1, fX2, fY2, fX3, fY3);
		
		if (m_bIsThumbnailsCreate)
		{
			m_oThumbnail.PathCommandCurveTo(fX1, fY1, fX2, fY2, fX3, fY3);
		}
	}

	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::PathCommandCurvesTo(SAFEARRAY* pPoints)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.PathCommandCurvesTo(pPoints);
	}

	m_pSimpleGraphicsConverter->PathCommandCurvesTo(pPoints);
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::PathCommandArcTo(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.PathCommandArcTo(fX, fY, fWidth, fHeight, fStartAngle, fSweepAngle);
	}

	m_pSimpleGraphicsConverter->PathCommandArcTo(fX, fY, fWidth, fHeight, fStartAngle, fSweepAngle);
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::PathCommandClose()
{
	if (c_nSimpleGraphicType == m_lCurrentCommandType)
	{
		Close();
	}
	else
	{
		m_pSimpleGraphicsConverter->PathCommandClose();
		
		if (m_bIsThumbnailsCreate)
		{
			m_oThumbnail.PathCommandClose();
		}
	}

	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::PathCommandEnd()
{
	if (c_nSimpleGraphicType == m_lCurrentCommandType)
	{
		End();
	}
	else
	{
		m_pSimpleGraphicsConverter->PathCommandEnd();
		
		if (m_bIsThumbnailsCreate)
		{
			m_oThumbnail.PathCommandEnd();
		}
	}

	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::DrawPath(long nType)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.DrawPath(nType);
	}
	if (!m_bIsTextGraphicType || 3 < m_oFont.GetTextDecorationStyle())
	{
		m_oWriter.WriteDrawPath(nType, m_dTransformAngle);
	}

	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::PathCommandStart()
{
	if (c_nSimpleGraphicType == m_lCurrentCommandType)
	{
		Start();
	}
	else
	{
		m_pSimpleGraphicsConverter->PathCommandEnd();
		m_pSimpleGraphicsConverter->PathCommandStart();
		
		if (m_bIsThumbnailsCreate)
		{
			m_oThumbnail.PathCommandStart();
		}
	}

	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::PathCommandGetCurrentPoint(double* fX, double* fY)
{
	m_pSimpleGraphicsConverter->PathCommandGetCurrentPoint(fX, fY);	
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::PathCommandText(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
{
	_SetFont();
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.PathCommandText(bsText, fY, fY, fWidth, fHeight, fBaseLineOffset);
	}
	m_pSimpleGraphicsConverter->PathCommandText(bsText, m_pFontManager, fX, fY, fWidth, fHeight, fBaseLineOffset);
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::PathCommandTextEx(BSTR bsText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
{
	BOOL bGid = m_oFont.StringGID;
	if (NULL != bsGidText)
	{
		m_oFont.StringGID = TRUE;
		PathCommandText(bsGidText, fX, fY, fWidth, fHeight, fBaseLineOffset);
	}
	
	m_oFont.StringGID = FALSE;
	PathCommandText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);

	m_oFont.StringGID = bGid;
	return S_OK;
}
//-------- Функции для вывода изображений ---------------------------------------------------
STDMETHODIMP CASCHTMLRenderer::DrawImage(IUnknown* pInterface, double fX, double fY, double fWidth, double fHeight)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.DrawImage(pInterface, fX, fY, fWidth, fHeight);
	}

	double x = fX;
	double y = fY;

	double r = fX + fWidth;
	double b = fY + fHeight;

	if (0 == m_dTransformAngle)
	{
		m_oFullTransform.TransformPoint(x, y);
		m_oFullTransform.TransformPoint(r, b);
	}
	else
	{
		NSHtmlRenderer::CMatrix oTemp  = m_oFullTransform;
		
		double dCx = (x + r) / 2;
		double dCy = (y + b) / 2;
		m_oFullTransform.TransformPoint(dCx, dCy);
		oTemp.RotateAt(-m_dTransformAngle, dCx, dCy, Aggplus::MatrixOrderAppend);

		oTemp.TransformPoint(x, y);
		oTemp.TransformPoint(r, b);
	}

	m_oWriter.WriteImage(pInterface, x, y, r - x, b - y, m_dTransformAngle);

	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::DrawImageFromFile(BSTR bstrVal, double fX, double fY, double fWidth, double fHeight)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.DrawImageFromFile(bstrVal, fX, fY, fWidth, fHeight);
	}

	double x = fX;
	double y = fY;

	double r = fX + fWidth;
	double b = fY + fHeight;

	if (0 == m_dTransformAngle)
	{
		m_oFullTransform.TransformPoint(x, y);
		m_oFullTransform.TransformPoint(r, b);
	}
	else
	{
		NSHtmlRenderer::CMatrix oTemp  = m_oFullTransform;
		
		double dCx = (x + r) / 2;
		double dCy = (y + b) / 2;
		m_oFullTransform.TransformPoint(dCx, dCy);
		oTemp.RotateAt(-m_dTransformAngle, dCx, dCy, Aggplus::MatrixOrderAppend);

		oTemp.TransformPoint(x, y);
		oTemp.TransformPoint(r, b);
	}

	m_oWriter.WriteImage((CString)bstrVal, x, y, r - x, b - y, m_oTransform.z_Rotation());

	return S_OK;
}
// transform --------------------------------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer::GetCommandParams(double* dAngle, double* dLeft, double* dTop, double* dWidth, double* dHeight, DWORD* lFlags)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::SetCommandParams(double dAngle, double dLeft, double dTop, double dWidth, double dHeight, DWORD lFlags)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.SetCommandParams(dAngle, dLeft, dTop, dWidth, dHeight, lFlags);
	}

	if ((dWidth <= 1) || (dHeight <= 1))
		lFlags = 0;

	BOOL bFlipX = (0 != (c_nParamFlipX & lFlags));
	BOOL bFlipY = (0 != (c_nParamFlipY & lFlags));
	
	double m11 = bFlipX ? -1.0 : 1.0;
	double m22 = bFlipY ? -1.0 : 1.0;
	
	NSHtmlRenderer::CMatrix oMatrix(1, 0, 0, 1, 0, 0);

	if ((0 != dAngle) || (0 != lFlags))
	{
		double dCentreX = (dLeft + dWidth / 2.0);
		double dCentreY = (dTop + dHeight / 2.0);
		
		oMatrix.Translate(-dCentreX, -dCentreY	, Aggplus::MatrixOrderAppend);
		
		oMatrix.Rotate(dAngle					, Aggplus::MatrixOrderAppend);
		oMatrix.Scale(m11, m22					, Aggplus::MatrixOrderAppend);
		
		oMatrix.Translate(dCentreX, dCentreY	, Aggplus::MatrixOrderAppend);
	}

	m_oTransform = oMatrix;
	CalculateFullTransform();

	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::SetTransform(double dA, double dB, double dC, double dD, double dE, double dF)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.SetTransform(dA, dB, dC, dD, dE, dF);
	}
	
	NSHtmlRenderer::CMatrix oTrans(dA, dB, dC, dD, dE, dF);
	m_oTransform = oTrans;

	CalculateFullTransform();

	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::GetTransform(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::ResetTransform(void)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.ResetTransform();
	}

	m_oTransform.Reset();

	CalculateFullTransform();

	return S_OK;
}
// -----------------------------------------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer::get_ClipMode(LONG* plMode)
{
	*plMode = m_lClipMode;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_ClipMode(LONG lMode)
{
	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.put_ClipMode(lMode);
	}
	m_lClipMode = lMode;
	m_oWriter.m_oPage.m_oVectorWriter.SetClipMode(m_lClipMode);
	return S_OK;
}
// additiaonal params ----------------------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer::SetAdditionalParam(BSTR ParamName, VARIANT ParamValue)
{
	//CString strParamName = (CString)ParamName; 
	//	
	//if (_T("ParagraphMetrics") == strParamName)
	//{
	//	CString strXml = ParamValue.bstrVal;
	//	m_oWriter.m_oPage.m_oText.SetParInfo(strXml);
	//	return S_OK;
	//}
	CString sParamName = ParamName;
	if ( _T("SourceRendererType") == sParamName && VT_I4 == ParamValue.vt )
	{
		m_oWriter.m_oPage.m_oText.m_bIsPDFTextStyle = ( ParamValue.lVal == c_nPDFWriter ? true : false );
	}
	else if (_T("UseFontWidthForPick") == sParamName && VT_BOOL == ParamValue.vt)
	{
		m_oWriter.m_oPage.m_oText.m_oFontManager.m_bIsUseFontWidth = ((VARIANT_TRUE == ParamValue.boolVal) ? true : false);
	}
	else if (_T("EffectPainter") == sParamName && VT_UNKNOWN == ParamValue.vt)
	{
#ifdef BUILD_SUPPORT_UNREGISTERED_WATERMARK
		RELEASEINTERFACE(m_pEffectPainter);
		
		if (NULL != ParamValue.punkVal)
		{
			ParamValue.punkVal->QueryInterface(Graphics::IID_IAVSEffectPainter, (void**)&m_pEffectPainter);
		}
#endif
	}

	if (m_bIsThumbnailsCreate)
	{
		m_oThumbnail.SetAdditionalParam(ParamName, ParamValue);
	}
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::GetAdditionalParam(BSTR ParamName, VARIANT* ParamValue)
{
	CString sParamName = ParamName;
	if ( _T("MaxImageSize") == sParamName )
	{
		ParamValue->vt		= VT_I4;
		ParamValue->lVal	= 800;
	}
	return S_OK;
}
// --------------------------------------------------------------------------------------------

// file
STDMETHODIMP CASCHTMLRenderer::CreateOfficeFile(BSTR bsFilePath)
{
	m_strDstFile = (CString)bsFilePath;

	int nIndexLastSlash = m_strDstFile.ReverseFind(TCHAR('\\'));
	if (-1 == nIndexLastSlash)
		return S_FALSE;

	CString strDir  = m_strDstFile.Mid(0, nIndexLastSlash);

	int nIndexExt	= m_strDstFile.ReverseFind(TCHAR('.'));

	if ((-1 == nIndexExt) || nIndexExt <= nIndexLastSlash)
		nIndexExt = m_strDstFile.GetLength();

	CString strName = m_strDstFile.Mid(nIndexLastSlash + 1, nIndexExt - nIndexLastSlash - 1);

	m_oWriter.CreateFile(strDir, strName);
	m_oWriter.WriteStartDocument();

	m_lCurrentPage = -1;
	m_bPageOpened = FALSE;

	if (NULL == m_pFonts)
	{
		CoCreateInstance(Graphics::CLSID_CASCWinFonts, NULL, CLSCTX_ALL, Graphics::IID_IASCWinFonts, (void**)&m_pFonts);
	}
	if (NULL == m_pFontManager)
	{
		CoCreateInstance(Graphics::CLSID_CASCFontManager, NULL, CLSCTX_ALL, Graphics::IID_IASCFontManager, (void**)&m_pFontManager);

		m_pFontManager->Initialize(L"");
		m_pFontManager->SetDefaultFont(L"Arial");
	}

	m_oFont.SetDefaultParams();
	m_oInstalledFont.SetDefaultParams();

	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::CloseFile()
{
	if (m_bPageOpened)
	{
		EndPage();
	}
	
	if (!m_bIsWEB)
		m_oWriter.WriteEndDocument(m_oDocument);
	else
	{
		if (!m_bIsMenu)
			m_oWriter.WriteEndDocument2(m_oDocument);
		else
			m_oWriter.WriteEndDocument3(m_oDocument);
	}

#ifdef BUILD_SUPPORT_UNREGISTERED_WATERMARK
	RELEASEINTERFACE(m_pEffectPainter);
#endif
	return S_OK;
}

STDMETHODIMP CASCHTMLRenderer::get_Mode(LONG* plMode)
{
	LONG lMode = 0;
	if (m_oWriter.m_bIsThumbnails)
		lMode |= 0x01;
	if (m_oWriter.m_bIsMenu)
		lMode |= 0x02;
	
	*plMode = lMode;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_Mode(LONG lMode)
{
	m_oWriter.m_bIsThumbnails	= (0x01 == (lMode & 0x01));
	m_oWriter.m_bIsMenu			= (0x02 == (lMode & 0x02));

	m_bIsWEB					= (0x04 == (lMode & 0x04));
	m_bIsMenu					= FALSE;
	if (m_bIsWEB)
	{
		m_bIsMenu				= (0x08 == (lMode & 0x08));
	}

	m_oWriter.m_oDstInfo.m_bIsWeb = (0x10 == (lMode & 0x10));

	m_bIsThumbnailsCreate		= (true == m_oWriter.m_bIsThumbnails) ? TRUE : FALSE;
	return S_OK;
}
