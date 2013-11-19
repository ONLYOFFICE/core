#include "stdafx.h"
#include "HTMLRenderer2.h"
#include "Resource.h"
#include "math.h"

HRESULT CAVSHTMLRenderer2::FinalConstruct()
{
	m_lLastSavedPage = 0;
	m_oDocument.SetUpdater(this);

	m_oWriter.m_pPen	= &m_oPen;
	m_oWriter.m_pBrush	= &m_oBrush;
	m_oWriter.m_pFont	= &m_oFont;

	m_pSimpleGraphicsConverter = NULL;
	
	m_bPageClosed = TRUE;

	m_dTransformAngle = 0.0;

	m_pFonts = NULL;
	m_pFontManager = NULL;

	m_oWriter.SetSimpleConverter(m_pSimpleGraphicsConverter, &m_oTransform);

	m_bIsMetafileDrawing = false;
	m_bIsTextGraphicType = false;

	m_pRenderers = &m_oWriter.m_oRenderers;

	m_bIsPathTextEx = false;

	return S_OK;
}

void CAVSHTMLRenderer2::FinalRelease()
{
	RELEASEINTERFACE(m_pFontManager);
	RELEASEINTERFACE(m_pSimpleGraphicsConverter);
	RELEASEINTERFACE(m_pFonts);
}

STDMETHODIMP CAVSHTMLRenderer2::get_Type(LONG* lType)
{
	if (NULL == lType)
		return S_FALSE;

	*lType = c_nHtmlRendrerer2;
	return S_OK;
}
//-------- Функции для работы со страницей --------------------------------------------------
STDMETHODIMP CAVSHTMLRenderer2::NewPage()
{
	if (m_bPageOpened)
	{
		EndPage();
	}

	m_oPen.SetDefaultParams();
	m_oBrush.SetDefaultParams();
	m_oFont.SetDefaultParams();

	m_oDocument.NewPage();
	m_bPageClosed = FALSE;
	m_bPageOpened = TRUE;

	m_bIsMetafileDrawing = false;
	m_bIsTextGraphicType = false;
	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::get_Height(double* dHeight)
{
	size_t nCount = m_oDocument.m_arrPages.GetCount();
	if ((nCount > 0) && (NULL != dHeight))
	{
		*dHeight = m_oDocument.m_arrPages[nCount - 1].GetHeight();	
	}
	
	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::put_Height(double dHeight)
{
	LONG lCount = (LONG)m_oDocument.m_arrPages.GetCount();
	if (0 != lCount)
	{
		m_oDocument.m_arrPages[lCount - 1].SetHeight(dHeight);
	}

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::get_Width(double* dWidth)
{
	size_t nCount = m_oDocument.m_arrPages.GetCount();
	if ((nCount > 0) && (NULL != dWidth))
	{
		*dWidth = m_oDocument.m_arrPages[nCount - 1].GetWidth();	
	}

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::put_Width(double dWidth)
{
	LONG lCount = (LONG)m_oDocument.m_arrPages.GetCount();
	if (0 != lCount)
	{
		m_oDocument.m_arrPages[lCount - 1].SetWidth(dWidth);
	}

	return S_OK;
}

STDMETHODIMP CAVSHTMLRenderer2::get_DpiX(double* dDpiX)
{
	if (NULL != dDpiX)
		*dDpiX = 96;
	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::get_DpiY(double* dDpiY)
{
	if (NULL != dDpiY)
		*dDpiY = 96;
	return S_OK;
}
// pen --------------------------------------------------------------------------------------
STDMETHODIMP CAVSHTMLRenderer2::SetPen(BSTR bsXML)
{
	m_oPen.FromXmlString((CString)bsXML);

	if (NULL != m_pRenderers)
		m_pRenderers->SetPen(bsXML);

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::get_PenColor(LONG* lColor)
{
	*lColor = m_oPen.Color;
	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::put_PenColor(LONG lColor)
{
	m_oPen.Color = lColor;

	if (NULL != m_pRenderers)
		m_pRenderers->put_PenColor(lColor);

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::get_PenAlpha(LONG* lAlpha)
{
	*lAlpha = m_oPen.Alpha;
	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::put_PenAlpha(LONG lAlpha)
{
	m_oPen.Alpha = lAlpha;

	if (NULL != m_pRenderers)
		m_pRenderers->put_PenAlpha(lAlpha);

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::get_PenSize(double* dSize)
{
	*dSize = m_oPen.Size;
	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::put_PenSize(double dSize)
{
	m_oPen.Size = dSize;

	if (NULL != m_pRenderers)
		m_pRenderers->put_PenSize(dSize);

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::get_PenDashStyle(BYTE* val)
{
	*val = m_oPen.DashStyle;
	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::put_PenDashStyle(BYTE val)
{
	m_oPen.DashStyle = val;

	if (NULL != m_pRenderers)
		m_pRenderers->put_PenDashStyle(val);

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::get_PenLineStartCap(BYTE* val)
{
	*val = m_oPen.LineStartCap;
	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::put_PenLineStartCap(BYTE val)
{
	m_oPen.LineStartCap = val;

	if (NULL != m_pRenderers)
		m_pRenderers->put_PenLineStartCap(val);

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::get_PenLineEndCap(BYTE* val)
{
	*val = m_oPen.LineEndCap;
	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::put_PenLineEndCap(BYTE val)
{
	m_oPen.LineEndCap = val;

	if (NULL != m_pRenderers)
		m_pRenderers->put_PenLineEndCap(val);

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::get_PenLineJoin(BYTE* val)
{
	*val = m_oPen.LineJoin;
	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::put_PenLineJoin(BYTE val)
{
	m_oPen.LineJoin = val;

	if (NULL != m_pRenderers)
		m_pRenderers->put_PenLineJoin(val);

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::get_PenDashOffset(double* val)
{
	*val = m_oPen.DashOffset;
	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::put_PenDashOffset(double val)
{
	m_oPen.DashOffset = val;

	if (NULL != m_pRenderers)
		m_pRenderers->put_PenDashOffset(val);

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::get_PenAlign(LONG* val)
{
	*val = m_oPen.Align;
	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::put_PenAlign(LONG val)
{
	m_oPen.Align = val;

	if (NULL != m_pRenderers)
		m_pRenderers->put_PenAlign(val);

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::get_PenMiterLimit(double* val)
{
	*val = m_oPen.MiterLimit;
	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::put_PenMiterLimit(double val)
{
	m_oPen.MiterLimit = val;

	if (NULL != m_pRenderers)
		m_pRenderers->put_PenMiterLimit(val);

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::PenDashPattern(SAFEARRAY* pPattern)
{
	if (NULL != pPattern)
	{
		m_oPen.SetDashPattern((double*)pPattern->pvData, pPattern->rgsabound[0].cElements);
	}

	if (NULL != m_pRenderers)
		m_pRenderers->PenDashPattern(pPattern);

	return S_OK;
}
// brush ------------------------------------------------------------------------------------
STDMETHODIMP CAVSHTMLRenderer2::SetBrush(BSTR bsXML)
{
	m_oBrush.FromXmlString((CString)bsXML);

	if (NULL != m_pRenderers)
		m_pRenderers->SetBrush(bsXML);

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::get_BrushType(LONG* lType)
{
	*lType = m_oBrush.Type;
	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::put_BrushType(LONG lType)
{
	m_oBrush.Type = lType;

	if (NULL != m_pRenderers)
		m_pRenderers->put_BrushType(lType);

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::get_BrushColor1(LONG* lColor)
{
	*lColor = m_oBrush.Color1;
	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::put_BrushColor1(LONG lColor)
{
	m_oBrush.Color1 = lColor;

	if (NULL != m_pRenderers)
		m_pRenderers->put_BrushColor1(lColor);

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::get_BrushAlpha1(LONG* lAlpha)
{
	*lAlpha = m_oBrush.Alpha1;
	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::put_BrushAlpha1(LONG lAlpha)
{
	m_oBrush.Alpha1 = lAlpha;

	if (NULL != m_pRenderers)
		m_pRenderers->put_BrushAlpha1(lAlpha);

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::get_BrushColor2(LONG* lColor)
{
	*lColor = m_oBrush.Color2;
	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::put_BrushColor2(LONG lColor)
{
	m_oBrush.Color2 = lColor;

	if (NULL != m_pRenderers)
		m_pRenderers->put_BrushColor2(lColor);

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::get_BrushAlpha2(LONG* lAlpha)
{
	*lAlpha = m_oBrush.Alpha2;
	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::put_BrushAlpha2(LONG lAlpha)
{
	m_oBrush.Alpha2 = lAlpha;

	if (NULL != m_pRenderers)
		m_pRenderers->put_BrushAlpha2(lAlpha);

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::get_BrushTexturePath(BSTR* bsPath)
{
	*bsPath = m_oBrush.TexturePath.AllocSysString();
	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::put_BrushTexturePath(BSTR bsPath)
{
	m_oBrush.TexturePath = (CString)bsPath;

	if (NULL != m_pRenderers)
		m_pRenderers->put_BrushTexturePath(bsPath);

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::get_BrushTextureMode(LONG* lMode)
{
	*lMode = m_oBrush.TextureMode;
	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::put_BrushTextureMode(LONG lMode)
{
	m_oBrush.TextureMode = lMode;

	if (NULL != m_pRenderers)
		m_pRenderers->put_BrushTextureMode(lMode);

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::get_BrushTextureAlpha(LONG* lTxAlpha)
{
	*lTxAlpha = m_oBrush.TextureAlpha;
	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::put_BrushTextureAlpha(LONG lTxAlpha)
{
	m_oBrush.TextureAlpha = lTxAlpha;

	if (NULL != m_pRenderers)
		m_pRenderers->put_BrushTextureAlpha(lTxAlpha);

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::get_BrushLinearAngle(double* dAngle)
{
	*dAngle = m_oBrush.LinearAngle;
	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::put_BrushLinearAngle(double dAngle)
{
	m_oBrush.LinearAngle = dAngle;

	if (NULL != m_pRenderers)
		m_pRenderers->put_BrushLinearAngle(dAngle);

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::BrushRect(BOOL val, double left, double top, double width, double height)
{
	m_oBrush.Rectable = val;
	m_oBrush.Rect.X = (float)left;
	m_oBrush.Rect.Y = (float)top;
	m_oBrush.Rect.Width  = (float)width;
	m_oBrush.Rect.Height = (float)height;

	if (NULL != m_pRenderers)
		m_pRenderers->BrushRect(val, left, top, width, height);

	return S_OK;
}
// font -------------------------------------------------------------------------------------
STDMETHODIMP CAVSHTMLRenderer2::SetFont(BSTR bsXML)
{
	m_oFont.FromXmlString((CString)bsXML);

	if (NULL != m_pRenderers)
		m_pRenderers->SetFont(bsXML);

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::get_FontName(BSTR* bsName)
{
	*bsName = m_oFont.Name.AllocSysString();
	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::put_FontName(BSTR bsName)
{
	m_oFont.Name = (CString)bsName;

	if (NULL != m_pRenderers)
		m_pRenderers->put_FontName(bsName);

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::get_FontPath(BSTR* bsName)
{
	*bsName = m_oFont.Path.AllocSysString();
	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::put_FontPath(BSTR bsName)
{
	m_oFont.Path = bsName;

	if (NULL != m_pRenderers)
		m_pRenderers->put_FontPath(bsName);

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::get_FontSize(double* dSize)
{
	*dSize = m_oFont.Size;
	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::put_FontSize(double dSize)
{
	m_oFont.Size = dSize;

	if (NULL != m_pRenderers)
		m_pRenderers->put_FontSize(dSize);

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::get_FontStyle(LONG* lStyle)
{
	*lStyle = m_oFont.GetStyle();
	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::put_FontStyle(LONG lStyle)
{
	m_oFont.SetStyle(lStyle);

	if (NULL != m_pRenderers)
		m_pRenderers->put_FontStyle(lStyle);

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::get_FontStringGID(BOOL* bGID)
{
	*bGID = m_oFont.StringGID;
	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::put_FontStringGID(BOOL bGID)
{
	m_oFont.StringGID = bGID;
	m_pFontManager->SetStringGID(bGID);

	if (NULL != m_pRenderers)
		m_pRenderers->put_FontStringGID(bGID);

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::get_FontCharSpace(double* dSpace)
{
	*dSpace = m_oFont.CharSpace;
	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::put_FontCharSpace(double dSpace)
{
	m_oFont.CharSpace = dSpace;

	if (NULL != m_pRenderers)
		m_pRenderers->put_FontCharSpace(dSpace);

	return S_OK;
}
// shadow -----------------------------------------------------------------------------------
STDMETHODIMP CAVSHTMLRenderer2::SetShadow(BSTR bsXML)
{
	m_oShadow.FromXmlString((CString)bsXML);

	if (NULL != m_pRenderers)
		m_pRenderers->SetShadow(bsXML);

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::get_ShadowDistanceX(double* val)
{
	*val = m_oShadow.DistanceX;
	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::put_ShadowDistanceX(double val)
{
	m_oShadow.DistanceX = val;

	if (NULL != m_pRenderers)
		m_pRenderers->put_ShadowDistanceX(val);

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::get_ShadowDistanceY(double* val)
{
	*val = m_oShadow.DistanceY;
	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::put_ShadowDistanceY(double val)
{
	m_oShadow.DistanceY = val;

	if (NULL != m_pRenderers)
		m_pRenderers->put_ShadowDistanceY(val);

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::get_ShadowBlurSize(double* val)
{
	*val = m_oShadow.BlurSize;
	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::put_ShadowBlurSize(double val)
{
	m_oShadow.BlurSize = val;

	if (NULL != m_pRenderers)
		m_pRenderers->put_ShadowBlurSize(val);

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::get_ShadowColor(LONG* val)
{
	*val = m_oShadow.Color;
	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::put_ShadowColor(LONG val)
{
	m_oShadow.Color = val;

	if (NULL != m_pRenderers)
		m_pRenderers->put_ShadowColor(val);

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::get_ShadowAlpha(LONG* val)
{
	*val = m_oShadow.Alpha;
	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::put_ShadowAlpha(LONG val)
{
	m_oShadow.Alpha = val;

	if (NULL != m_pRenderers)
		m_pRenderers->put_ShadowAlpha(val);

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::get_ShadowVisible(BOOL* val)
{
	*val = m_oShadow.Visible;
	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::put_ShadowVisible(BOOL val)
{
	m_oShadow.Visible = val;

	if (NULL != m_pRenderers)
		m_pRenderers->put_ShadowVisible(val);

	return S_OK;
}
// edge -------------------------------------------------------------------------------------
STDMETHODIMP CAVSHTMLRenderer2::SetEdgeText(BSTR bsXML)
{
	m_oEdge.FromXmlString((CString)bsXML);

	if (NULL != m_pRenderers)
		m_pRenderers->SetEdgeText(bsXML);

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::get_EdgeVisible(LONG* val)
{
	*val = m_oEdge.Visible;
	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::put_EdgeVisible(LONG val)
{
	m_oEdge.Visible = val;

	if (NULL != m_pRenderers)
		m_pRenderers->put_EdgeVisible(val);

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::get_EdgeColor(LONG* val)
{
	*val = m_oEdge.Color;
	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::put_EdgeColor(LONG val)
{
	m_oEdge.Color = val;

	if (NULL != m_pRenderers)
		m_pRenderers->put_EdgeColor(val);

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::get_EdgeAlpha(LONG* val)
{
	*val = m_oEdge.Alpha;
	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::put_EdgeAlpha(LONG val)
{
	m_oEdge.Alpha = val;

	if (NULL != m_pRenderers)
		m_pRenderers->put_EdgeAlpha(val);

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::get_EdgeDist(double* val)
{
	*val = m_oEdge.Dist;
	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::put_EdgeDist(double val)
{
	m_oEdge.Dist = val;

	if (NULL != m_pRenderers)
		m_pRenderers->put_EdgeDist(val);

	return S_OK;
}
//-------- Функции для вывода текста --------------------------------------------------------
STDMETHODIMP CAVSHTMLRenderer2::CommandDrawText(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
{
	if (c_nHyperlinkType == m_lCurrentCommandType)
		return S_OK;

	m_oWriter.WriteText(bsText, NULL, fX, fY + fBaseLineOffset, fWidth, fHeight, fBaseLineOffset);

	if (NULL != m_pRenderers)
		m_pRenderers->CommandDrawText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::CommandDrawTextEx(BSTR bsText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
{	
	m_oWriter.WriteText(bsText, bsGidText, fX, fY + fBaseLineOffset, fWidth, fHeight, fBaseLineOffset);

	if (NULL != m_pRenderers)
		m_pRenderers->CommandDrawTextEx(bsText, bsGidText, bsSourceCodeText, fX, fY, fWidth, fHeight, fBaseLineOffset, lFlags);

	return S_OK;
}
//-------- Маркеры для команд ---------------------------------------------------------------
STDMETHODIMP CAVSHTMLRenderer2::BeginCommand(DWORD lType)
{
	if (NULL != m_pRenderers)
		m_pRenderers->BeginCommand(lType);

	if (c_nClipType == lType)
	{
		m_oWriter.WritePathClip();
	}
	else if (c_nPathType == lType)
	{
		m_oWriter.WriteBeginPath();
		m_pSimpleGraphicsConverter->PathCommandEnd();
	}
	else if (c_nImageType == lType)
	{
		m_bIsMetafileDrawing = true;
	}
	else if (c_nTextGraphicType == lType)
	{
		m_bIsTextGraphicType = true;
	}
	m_lCurrentCommandType = lType;

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::EndCommand(DWORD lType)
{
	if (NULL != m_pRenderers)
		m_pRenderers->EndCommand(lType);

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
	//else if (c_nParagraphType == lType)
	//{
	//	m_oWriter.m_oPage.m_oText.EndParagraph();
	//}
	m_lCurrentCommandType = -1;

	return S_OK;
}
//-------- Функции для работы с Graphics Path -----------------------------------------------
STDMETHODIMP CAVSHTMLRenderer2::PathCommandMoveTo(double fX, double fY)
{
	if (c_nSimpleGraphicType == m_lCurrentCommandType)
	{
		MoveTo(fX, fY);
	}
	else
	{
		m_pSimpleGraphicsConverter->PathCommandMoveTo(fX, fY);

		if (NULL != m_pRenderers)
			m_pRenderers->PathCommandMoveTo(fX, fY);
	}

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::PathCommandLineTo(double fX, double fY)
{
	if (c_nSimpleGraphicType == m_lCurrentCommandType)
	{
		LineTo(fX, fY);
	}
	else
	{
		m_pSimpleGraphicsConverter->PathCommandLineTo(fX, fY);

		if (NULL != m_pRenderers)
			m_pRenderers->PathCommandLineTo(fX, fY);
	}

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::PathCommandLinesTo(SAFEARRAY* pPoints)
{
	m_pSimpleGraphicsConverter->PathCommandLinesTo(pPoints);

	if (NULL != m_pRenderers)
		m_pRenderers->PathCommandLinesTo(pPoints);

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::PathCommandCurveTo(double fX1, double fY1, double fX2, double fY2, double fX3, double fY3)
{
	if (c_nSimpleGraphicType == m_lCurrentCommandType)
	{
		CurveTo(fX1, fY1, fX2, fY2, fX3, fY3);
	}
	else
	{
		m_pSimpleGraphicsConverter->PathCommandCurveTo(fX1, fY1, fX2, fY2, fX3, fY3);
		
		if (NULL != m_pRenderers)
			m_pRenderers->PathCommandCurveTo(fX1, fY1, fX2, fY2, fX3, fY3);
	}

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::PathCommandCurvesTo(SAFEARRAY* pPoints)
{
	m_pSimpleGraphicsConverter->PathCommandCurvesTo(pPoints);

	if (NULL != m_pRenderers)
		m_pRenderers->PathCommandCurvesTo(pPoints);

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::PathCommandArcTo(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle)
{
	m_pSimpleGraphicsConverter->PathCommandArcTo(fX, fY, fWidth, fHeight, fStartAngle, fSweepAngle);

	if (NULL != m_pRenderers)
		m_pRenderers->PathCommandArcTo(fX, fY, fWidth, fHeight, fStartAngle, fSweepAngle);

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::PathCommandClose()
{
	if (c_nSimpleGraphicType == m_lCurrentCommandType)
	{
		Close();
	}
	else
	{
		m_pSimpleGraphicsConverter->PathCommandClose();

		if (NULL != m_pRenderers)
			m_pRenderers->PathCommandClose();
	}

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::PathCommandEnd()
{
	if (c_nSimpleGraphicType == m_lCurrentCommandType)
	{
		End();
	}
	else
	{
		m_pSimpleGraphicsConverter->PathCommandEnd();

		if (NULL != m_pRenderers)
			m_pRenderers->PathCommandEnd();
	}

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::DrawPath(long nType)
{
	m_oWriter.WriteDrawPath(nType, m_dTransformAngle);
	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::PathCommandStart()
{
	if (c_nSimpleGraphicType == m_lCurrentCommandType)
	{
		Start();
	}
	else
	{
		m_pSimpleGraphicsConverter->PathCommandEnd();
		m_pSimpleGraphicsConverter->PathCommandStart();

		if (NULL != m_pRenderers)
			m_pRenderers->PathCommandStart();
	}

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::PathCommandGetCurrentPoint(double* fX, double* fY)
{
	m_pSimpleGraphicsConverter->PathCommandGetCurrentPoint(fX, fY);	
	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::PathCommandText(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
{
	_SetFont();
	m_pSimpleGraphicsConverter->PathCommandText(bsText, m_pFontManager, fX, fY, fWidth, fHeight, fBaseLineOffset);

	if (!m_bIsPathTextEx)
	{
		if (NULL != m_pRenderers)
			m_pRenderers->PathCommandText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);
	}
	else
	{
		m_bIsPathTextEx = false;
	}

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::PathCommandTextEx(BSTR bsText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
{
	if (NULL != m_pRenderers)
		m_pRenderers->PathCommandTextEx(bsText, bsGidText, bsSourceCodeText, fX, fY, fWidth, fHeight, fBaseLineOffset, lFlags);

	m_bIsPathTextEx = true;

	BOOL bGid = m_oFont.StringGID;
	if (NULL != bsGidText)
	{
		m_oFont.StringGID = TRUE;
		PathCommandText(bsGidText, fX, fY, fWidth, fHeight, fBaseLineOffset);
	}
	else
	{	
		m_oFont.StringGID = FALSE;
		PathCommandText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);
	}

	m_oFont.StringGID = bGid;
	return S_OK;
}
//-------- Функции для вывода изображений ---------------------------------------------------
STDMETHODIMP CAVSHTMLRenderer2::DrawImage(IUnknown* pInterface, double fX, double fY, double fWidth, double fHeight)
{
	if (NULL == pInterface)
		return S_OK;	

	m_oWriter.WriteImage(pInterface, fX, fY, fWidth, fHeight, m_dTransformAngle);

	if (NULL != m_pRenderers)
		m_pRenderers->DrawImage(pInterface, fX, fY, fWidth, fHeight);

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::DrawImageFromFile(BSTR bstrVal, double fX, double fY, double fWidth, double fHeight)
{
	m_oWriter.WriteImage((CString)bstrVal, fX, fY, fWidth, fHeight, m_oTransform.z_Rotation());

	if (NULL != m_pRenderers)
		m_pRenderers->DrawImageFromFile(bstrVal, fX, fY, fWidth, fHeight);

	return S_OK;
}
// transform --------------------------------------------------------------------------------
STDMETHODIMP CAVSHTMLRenderer2::GetCommandParams(double* dAngle, double* dLeft, double* dTop, double* dWidth, double* dHeight, DWORD* lFlags)
{
	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::SetCommandParams(double dAngle, double dLeft, double dTop, double dWidth, double dHeight, DWORD lFlags)
{
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

	agg::trans_affine* mm = &m_oTransform.m_agg_mtx;
	if (NULL != m_pRenderers)
		m_pRenderers->SetTransform(mm->sx, mm->shy, mm->shx, mm->sy, mm->tx, mm->ty);

	CalculateFullTransform();

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::SetTransform(double dA, double dB, double dC, double dD, double dE, double dF)
{
	NSHtmlRenderer::CMatrix oTrans(dA, dB, dC, dD, dE, dF);
	m_oTransform = oTrans;

	if (NULL != m_pRenderers)
		m_pRenderers->SetTransform(dA, dB, dC, dD, dE, dF);

	CalculateFullTransform();

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::GetTransform(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF)
{
	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::ResetTransform(void)
{
	m_oTransform.Reset();

	if (NULL != m_pRenderers)
		m_pRenderers->ResetTransform();

	CalculateFullTransform();

	return S_OK;
}
// -----------------------------------------------------------------------------------------
STDMETHODIMP CAVSHTMLRenderer2::get_ClipMode(LONG* plMode)
{
	*plMode = m_lClipMode;
	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::put_ClipMode(LONG lMode)
{
	m_lClipMode = lMode;

	if (NULL != m_pRenderers)
		m_pRenderers->put_ClipMode(lMode);

	return S_OK;
}
// additiaonal params ----------------------------------------------------------------------
STDMETHODIMP CAVSHTMLRenderer2::SetAdditionalParam(BSTR ParamName, VARIANT ParamValue)
{
	if (NULL != m_pRenderers)
		m_pRenderers->SetAdditionalParam(ParamName, ParamValue);

	CString strParamName = (CString)ParamName; 
		
	if (_T("SourceType") == strParamName)
	{		
		m_oWriter.m_lSrcFileType = ParamValue.lVal;
		return S_OK;
	}
	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::GetAdditionalParam(BSTR ParamName, VARIANT* ParamValue)
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
STDMETHODIMP CAVSHTMLRenderer2::CreateOfficeFile(BSTR bsFilePath)
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

	m_oWriter.m_pTransform = &m_oTransform;
	m_oWriter.CreateFile(strDir, strName);
	m_oWriter.WriteStartDocument();

	if (m_oWriter.m_lSrcFileType == AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU)
		m_pRenderers = NULL;

	m_oWriter.m_oSmartText.m_oFontManager.Init(_T(""));

	m_lCurrentPage = -1;
	m_bPageOpened = FALSE;

	if (NULL == m_pFonts)
	{
		CoCreateInstance(AVSGraphics::CLSID_CAVSWinFonts, NULL, CLSCTX_ALL, AVSGraphics::IID_IAVSWinFonts, (void**)&m_pFonts);
	}
	if (NULL == m_pFontManager)
	{
		CoCreateInstance(AVSGraphics::CLSID_CAVSFontManager, NULL, CLSCTX_ALL, AVSGraphics::IID_IAVSFontManager, (void**)&m_pFontManager);

		m_pFontManager->Initialize(L"");
		m_pFontManager->SetDefaultFont(L"Arial");
	}

	m_oFont.SetDefaultParams();
	m_oInstalledFont.SetDefaultParams();

	CoCreateInstance(AVSGraphics::CLSID_CAVSGraphicSimpleComverter, NULL, CLSCTX_ALL, AVSGraphics::IID_IAVSGraphicSimpleComverter, (void**)&m_pSimpleGraphicsConverter);

	IUnknown* punkRenderer = NULL;
	this->QueryInterface(IID_IUnknown, (void**)&punkRenderer);

	m_pSimpleGraphicsConverter->put_Renderer(punkRenderer);
	RELEASEINTERFACE(punkRenderer);

	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::CloseFile()
{
	if (m_bPageOpened)
	{
		EndPage();
	}
	
	m_oWriter.WriteEndDocument(m_oDocument);
	RELEASEINTERFACE(m_pSimpleGraphicsConverter);
	return S_OK;
}

STDMETHODIMP CAVSHTMLRenderer2::get_Mode(LONG* plMode)
{
	return S_OK;
}
STDMETHODIMP CAVSHTMLRenderer2::put_Mode(LONG lMode)
{
	return S_OK;
}
