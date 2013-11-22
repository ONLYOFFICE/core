#include "stdafx.h"
#include "HTMLRenderer3.h"
#include "Resource.h"
#include "math.h"

HRESULT CASCHTMLRenderer3::FinalConstruct()
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

	m_bIsChangedFontParamBetweenDrawText = true;
	m_bIsGraphicsDumperMode = false;

	return S_OK;
}

void CASCHTMLRenderer3::FinalRelease()
{
	RELEASEINTERFACE(m_pFontManager);
	RELEASEINTERFACE(m_pSimpleGraphicsConverter);
	RELEASEINTERFACE(m_pFonts);
}

STDMETHODIMP CASCHTMLRenderer3::get_Type(LONG* lType)
{
	if (NULL == lType)
		return S_FALSE;

	*lType = c_nHtmlRendrerer2;
	return S_OK;
}
//-------- Функции для работы со страницей --------------------------------------------------
STDMETHODIMP CASCHTMLRenderer3::NewPage()
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
STDMETHODIMP CASCHTMLRenderer3::get_Height(double* dHeight)
{
	size_t nCount = m_oDocument.m_arrPages.GetCount();
	if ((nCount > 0) && (NULL != dHeight))
	{
		*dHeight = m_oDocument.m_arrPages[nCount - 1].GetHeight();	
	}
	
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_Height(double dHeight)
{
	LONG lCount = (LONG)m_oDocument.m_arrPages.GetCount();
	if (0 != lCount)
	{
		m_oDocument.m_arrPages[lCount - 1].SetHeight(dHeight);
	}

	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_Width(double* dWidth)
{
	size_t nCount = m_oDocument.m_arrPages.GetCount();
	if ((nCount > 0) && (NULL != dWidth))
	{
		*dWidth = m_oDocument.m_arrPages[nCount - 1].GetWidth();	
	}

	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_Width(double dWidth)
{
	LONG lCount = (LONG)m_oDocument.m_arrPages.GetCount();
	if (0 != lCount)
	{
		m_oDocument.m_arrPages[lCount - 1].SetWidth(dWidth);
	}

	return S_OK;
}

STDMETHODIMP CASCHTMLRenderer3::get_DpiX(double* dDpiX)
{
	if (NULL != dDpiX)
		*dDpiX = 96;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_DpiY(double* dDpiY)
{
	if (NULL != dDpiY)
		*dDpiY = 96;
	return S_OK;
}
// pen --------------------------------------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer3::SetPen(BSTR bsXML)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.SetPen(bsXML);

	m_oPen.FromXmlString((CString)bsXML);
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_PenColor(LONG* lColor)
{
	*lColor = m_oPen.Color;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_PenColor(LONG lColor)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.put_PenColor(lColor);

	m_oPen.Color = lColor;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_PenAlpha(LONG* lAlpha)
{
	*lAlpha = m_oPen.Alpha;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_PenAlpha(LONG lAlpha)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.put_PenAlpha(lAlpha);

	m_oPen.Alpha = lAlpha;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_PenSize(double* dSize)
{
	*dSize = m_oPen.Size;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_PenSize(double dSize)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.put_PenSize(dSize);

	m_oPen.Size = dSize;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_PenDashStyle(BYTE* val)
{
	*val = m_oPen.DashStyle;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_PenDashStyle(BYTE val)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.put_PenDashStyle(val);

	m_oPen.DashStyle = val;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_PenLineStartCap(BYTE* val)
{
	*val = m_oPen.LineStartCap;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_PenLineStartCap(BYTE val)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.put_PenLineStartCap(val);

	m_oPen.LineStartCap = val;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_PenLineEndCap(BYTE* val)
{
	*val = m_oPen.LineEndCap;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_PenLineEndCap(BYTE val)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.put_PenLineEndCap(val);

	m_oPen.LineEndCap = val;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_PenLineJoin(BYTE* val)
{
	*val = m_oPen.LineJoin;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_PenLineJoin(BYTE val)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.put_PenLineJoin(val);

	m_oPen.LineJoin = val;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_PenDashOffset(double* val)
{
	*val = m_oPen.DashOffset;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_PenDashOffset(double val)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.put_PenDashOffset(val);

	m_oPen.DashOffset = val;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_PenAlign(LONG* val)
{
	*val = m_oPen.Align;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_PenAlign(LONG val)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.put_PenAlign(val);

	m_oPen.Align = val;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_PenMiterLimit(double* val)
{
	*val = m_oPen.MiterLimit;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_PenMiterLimit(double val)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.put_PenMiterLimit(val);

	m_oPen.MiterLimit = val;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::PenDashPattern(SAFEARRAY* pPattern)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.PenDashPattern(pPattern);

	if (NULL != pPattern)
	{
		m_oPen.SetDashPattern((double*)pPattern->pvData, pPattern->rgsabound[0].cElements);
	}
	return S_OK;
}
// brush ------------------------------------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer3::SetBrush(BSTR bsXML)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.SetBrush(bsXML);

	m_oBrush.FromXmlString((CString)bsXML);
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_BrushType(LONG* lType)
{
	*lType = m_oBrush.Type;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_BrushType(LONG lType)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.put_BrushType(lType);

	m_oBrush.Type = lType;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_BrushColor1(LONG* lColor)
{
	*lColor = m_oBrush.Color1;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_BrushColor1(LONG lColor)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.put_BrushColor1(lColor);

	m_oBrush.Color1 = lColor;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_BrushAlpha1(LONG* lAlpha)
{
	*lAlpha = m_oBrush.Alpha1;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_BrushAlpha1(LONG lAlpha)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.put_BrushAlpha1(lAlpha);

	m_oBrush.Alpha1 = lAlpha;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_BrushColor2(LONG* lColor)
{
	*lColor = m_oBrush.Color2;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_BrushColor2(LONG lColor)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.put_BrushColor2(lColor);

	m_oBrush.Color2 = lColor;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_BrushAlpha2(LONG* lAlpha)
{
	*lAlpha = m_oBrush.Alpha2;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_BrushAlpha2(LONG lAlpha)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.put_BrushAlpha2(lAlpha);

	m_oBrush.Alpha2 = lAlpha;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_BrushTexturePath(BSTR* bsPath)
{
	*bsPath = m_oBrush.TexturePath.AllocSysString();
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_BrushTexturePath(BSTR bsPath)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.put_BrushTexturePath(bsPath);

	m_oBrush.TexturePath = (CString)bsPath;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_BrushTextureMode(LONG* lMode)
{
	*lMode = m_oBrush.TextureMode;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_BrushTextureMode(LONG lMode)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.put_BrushTextureMode(lMode);

	m_oBrush.TextureMode = lMode;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_BrushTextureAlpha(LONG* lTxAlpha)
{
	*lTxAlpha = m_oBrush.TextureAlpha;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_BrushTextureAlpha(LONG lTxAlpha)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.put_BrushTextureAlpha(lTxAlpha);

	m_oBrush.TextureAlpha = lTxAlpha;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_BrushLinearAngle(double* dAngle)
{
	*dAngle = m_oBrush.LinearAngle;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_BrushLinearAngle(double dAngle)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.put_BrushLinearAngle(dAngle);

	m_oBrush.LinearAngle = dAngle;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::BrushRect(BOOL val, double left, double top, double width, double height)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.BrushRect(val, left, top, width, height);

	m_oBrush.Rectable = val;
	m_oBrush.Rect.X = (float)left;
	m_oBrush.Rect.Y = (float)top;
	m_oBrush.Rect.Width  = (float)width;
	m_oBrush.Rect.Height = (float)height;
	return S_OK;
}
// font -------------------------------------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer3::SetFont(BSTR bsXML)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.SetFont(bsXML);

	m_oFont.FromXmlString((CString)bsXML);
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_FontName(BSTR* bsName)
{
	*bsName = m_oFont.Name.AllocSysString();
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_FontName(BSTR bsName)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.put_FontName(bsName);

	m_oFont.Name = bsName;
	m_bIsChangedFontParamBetweenDrawText = true;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_FontPath(BSTR* bsName)
{
	*bsName = m_oFont.Path.AllocSysString();
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_FontPath(BSTR bsName)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.put_FontPath(bsName);

	m_oFont.Path = bsName;
	m_bIsChangedFontParamBetweenDrawText = true;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_FontSize(double* dSize)
{
	*dSize = m_oFont.Size;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_FontSize(double dSize)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.put_FontSize(dSize);

	if (m_oFont.Size != dSize)
	{
		m_oFont.Size = dSize;
		m_bIsChangedFontParamBetweenDrawText = true;
	}
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_FontStyle(LONG* lStyle)
{
	*lStyle = m_oFont.GetStyle();
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_FontStyle(LONG lStyle)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.put_FontStyle(lStyle);

	LONG lOld = m_oFont.GetStyle();
	if (lOld != lStyle)
	{
		m_oFont.SetStyle(lStyle);
		m_bIsChangedFontParamBetweenDrawText = true;
	}
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_FontStringGID(BOOL* bGID)
{
	*bGID = m_oFont.StringGID;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_FontStringGID(BOOL bGID)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.put_FontStringGID(bGID);

	m_oFont.StringGID = bGID;
	m_pFontManager->SetStringGID(bGID);
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_FontCharSpace(double* dSpace)
{
	*dSpace = m_oFont.CharSpace;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_FontCharSpace(double dSpace)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.put_FontCharSpace(dSpace);

	m_oFont.CharSpace = dSpace;
	return S_OK;
}
// shadow -----------------------------------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer3::SetShadow(BSTR bsXML)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.SetShadow(bsXML);

	m_oShadow.FromXmlString((CString)bsXML);
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_ShadowDistanceX(double* val)
{
	*val = m_oShadow.DistanceX;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_ShadowDistanceX(double val)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.put_ShadowDistanceX(val);

	m_oShadow.DistanceX = val;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_ShadowDistanceY(double* val)
{
	*val = m_oShadow.DistanceY;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_ShadowDistanceY(double val)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.put_ShadowDistanceY(val);

	m_oShadow.DistanceY = val;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_ShadowBlurSize(double* val)
{
	*val = m_oShadow.BlurSize;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_ShadowBlurSize(double val)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.put_ShadowBlurSize(val);

	m_oShadow.BlurSize = val;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_ShadowColor(LONG* val)
{
	*val = m_oShadow.Color;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_ShadowColor(LONG val)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.put_ShadowColor(val);

	m_oShadow.Color = val;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_ShadowAlpha(LONG* val)
{
	*val = m_oShadow.Alpha;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_ShadowAlpha(LONG val)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.put_ShadowAlpha(val);

	m_oShadow.Alpha = val;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_ShadowVisible(BOOL* val)
{
	*val = m_oShadow.Visible;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_ShadowVisible(BOOL val)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.put_ShadowVisible(val);

	m_oShadow.Visible = val;
	return S_OK;
}
// edge -------------------------------------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer3::SetEdgeText(BSTR bsXML)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.SetEdgeText(bsXML);

	m_oEdge.FromXmlString((CString)bsXML);
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_EdgeVisible(LONG* val)
{
	*val = m_oEdge.Visible;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_EdgeVisible(LONG val)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.put_EdgeVisible(val);

	m_oEdge.Visible = val;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_EdgeColor(LONG* val)
{
	*val = m_oEdge.Color;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_EdgeColor(LONG val)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.put_EdgeColor(val);

	m_oEdge.Color = val;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_EdgeAlpha(LONG* val)
{
	*val = m_oEdge.Alpha;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_EdgeAlpha(LONG val)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.put_EdgeAlpha(val);

	m_oEdge.Alpha = val;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_EdgeDist(double* val)
{
	*val = m_oEdge.Dist;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_EdgeDist(double val)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.put_EdgeDist(val);

	m_oEdge.Dist = val;
	return S_OK;
}
//-------- Функции для вывода текста --------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer3::CommandDrawText(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.CommandDrawText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);

	if (c_nHyperlinkType == m_lCurrentCommandType)
		return S_OK;

	m_oWriter.WriteText(bsText, NULL, fX, fY + fBaseLineOffset, fWidth, fHeight, fBaseLineOffset, m_bIsChangedFontParamBetweenDrawText);
	m_bIsChangedFontParamBetweenDrawText = false;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::CommandDrawTextEx(BSTR bsText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
{	
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.CommandDrawTextEx(bsText, bsGidText, bsSourceCodeText, fX, fY, fWidth, fHeight, fBaseLineOffset, lFlags);

	m_oWriter.WriteText(bsText, bsGidText, fX, fY + fBaseLineOffset, fWidth, fHeight, fBaseLineOffset, m_bIsChangedFontParamBetweenDrawText);
	m_bIsChangedFontParamBetweenDrawText = false;
	return S_OK;
}
//-------- Маркеры для команд ---------------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer3::BeginCommand(DWORD lType)
{
	if (m_bIsGraphicsDumperMode && lType != c_nPDFTilingFill)
		return m_oDumper.BeginCommand(lType);

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
	else if (c_nPDFTilingFill == lType)
	{
		m_oWriter.m_lTilingCounter++;
	}
	m_lCurrentCommandType = lType;

	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::EndCommand(DWORD lType)
{
	if (m_bIsGraphicsDumperMode && lType != c_nPDFTilingFill)
		return m_oDumper.EndCommand(lType);

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
	else if (c_nPDFTilingFill == lType)
	{
		m_oWriter.m_lTilingCounter--;

		if (0 == m_oWriter.m_lTilingCounter)
		{
			m_bIsGraphicsDumperMode = false;

			// dump Graphics
			IUnknown* pPattern = m_oDumper.ConvertVectorGraphics();
			m_oWriter.WritePattern(pPattern, m_oDumper.m_oTile);			
			RELEASEINTERFACE(pPattern);
		}
	}
	//else if (c_nParagraphType == lType)
	//{
	//	m_oWriter.m_oPage.m_oText.EndParagraph();
	//}
	m_lCurrentCommandType = -1;

	return S_OK;
}
//-------- Функции для работы с Graphics Path -----------------------------------------------
STDMETHODIMP CASCHTMLRenderer3::PathCommandMoveTo(double fX, double fY)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.PathCommandMoveTo(fX, fY);

	if (c_nSimpleGraphicType == m_lCurrentCommandType)
	{
		MoveTo(fX, fY);
	}
	else
	{
		m_pSimpleGraphicsConverter->PathCommandMoveTo(fX, fY);
	}

	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::PathCommandLineTo(double fX, double fY)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.PathCommandLineTo(fX, fY);

	if (c_nSimpleGraphicType == m_lCurrentCommandType)
	{
		LineTo(fX, fY);
	}
	else
	{
		m_pSimpleGraphicsConverter->PathCommandLineTo(fX, fY);
	}

	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::PathCommandLinesTo(SAFEARRAY* pPoints)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.PathCommandLinesTo(pPoints);

	m_pSimpleGraphicsConverter->PathCommandLinesTo(pPoints);
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::PathCommandCurveTo(double fX1, double fY1, double fX2, double fY2, double fX3, double fY3)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.PathCommandCurveTo(fX1, fY1, fX2, fY2, fX3, fY3);

	if (c_nSimpleGraphicType == m_lCurrentCommandType)
	{
		CurveTo(fX1, fY1, fX2, fY2, fX3, fY3);
	}
	else
	{
		m_pSimpleGraphicsConverter->PathCommandCurveTo(fX1, fY1, fX2, fY2, fX3, fY3);
	}

	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::PathCommandCurvesTo(SAFEARRAY* pPoints)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.PathCommandCurvesTo(pPoints);

	m_pSimpleGraphicsConverter->PathCommandCurvesTo(pPoints);
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::PathCommandArcTo(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.PathCommandArcTo(fX, fY, fWidth, fHeight, fStartAngle, fSweepAngle);

	m_pSimpleGraphicsConverter->PathCommandArcTo(fX, fY, fWidth, fHeight, fStartAngle, fSweepAngle);
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::PathCommandClose()
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.PathCommandClose();

	if (c_nSimpleGraphicType == m_lCurrentCommandType)
	{
		Close();
	}
	else
	{
		m_pSimpleGraphicsConverter->PathCommandClose();
	}

	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::PathCommandEnd()
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.PathCommandEnd();

	if (c_nSimpleGraphicType == m_lCurrentCommandType)
	{
		End();
	}
	else
	{
		m_pSimpleGraphicsConverter->PathCommandEnd();
	}

	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::DrawPath(long nType)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.DrawPath(nType);

	m_oWriter.WriteDrawPath(nType);
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::PathCommandStart()
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.PathCommandStart();

	if (c_nSimpleGraphicType == m_lCurrentCommandType)
	{
		Start();
	}
	else
	{
		m_pSimpleGraphicsConverter->PathCommandEnd();
		m_pSimpleGraphicsConverter->PathCommandStart();
	}

	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::PathCommandGetCurrentPoint(double* fX, double* fY)
{
	m_pSimpleGraphicsConverter->PathCommandGetCurrentPoint(fX, fY);	
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::PathCommandText(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.PathCommandText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);

	_SetFont();
	m_pSimpleGraphicsConverter->PathCommandText(bsText, m_pFontManager, fX, fY, fWidth, fHeight, fBaseLineOffset);
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::PathCommandTextEx(BSTR bsText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
{	
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.PathCommandTextEx(bsText, bsGidText, bsSourceCodeText, fX, fY, fWidth, fHeight, fBaseLineOffset, lFlags);

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
STDMETHODIMP CASCHTMLRenderer3::DrawImage(IUnknown* pInterface, double fX, double fY, double fWidth, double fHeight)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.DrawImage(pInterface, fX, fY, fWidth, fHeight);

	if (NULL == pInterface)
		return S_OK;	

	m_oWriter.WriteImage(pInterface, fX, fY, fWidth, fHeight);
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::DrawImageFromFile(BSTR bstrVal, double fX, double fY, double fWidth, double fHeight)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.DrawImageFromFile(bstrVal, fX, fY, fWidth, fHeight);

	m_oWriter.WriteImage((CString)bstrVal, fX, fY, fWidth, fHeight);
	return S_OK;
}
// transform --------------------------------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer3::GetCommandParams(double* dAngle, double* dLeft, double* dTop, double* dWidth, double* dHeight, DWORD* lFlags)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::SetCommandParams(double dAngle, double dLeft, double dTop, double dWidth, double dHeight, DWORD lFlags)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.SetCommandParams(dAngle, dLeft, dTop, dWidth, dHeight, lFlags);

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
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::SetTransform(double dA, double dB, double dC, double dD, double dE, double dF)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.SetTransform(dA, dB, dC, dD, dE, dF);

	agg::trans_affine* paff = &m_oTransform.m_agg_mtx;
	paff->sx = dA;
	paff->shy = dB;
	paff->shx = dC;
	paff->sy = dD;
	paff->tx = dE;
	paff->ty = dF;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::GetTransform(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::ResetTransform(void)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.ResetTransform();

	m_oTransform.Reset();
	return S_OK;
}
// -----------------------------------------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer3::get_ClipMode(LONG* plMode)
{
	*plMode = m_lClipMode;
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_ClipMode(LONG lMode)
{
	if (m_bIsGraphicsDumperMode)
		return m_oDumper.put_ClipMode(lMode);

	m_lClipMode = lMode;
	m_oWriter.m_oSVGWriter.m_lClipMode = lMode;
	return S_OK;
}
// additiaonal params ----------------------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer3::SetAdditionalParam(BSTR ParamName, VARIANT ParamValue)
{
	CString strParamName = (CString)ParamName; 
		
	if (_T("SourceType") == strParamName)
	{		
		m_oWriter.m_lSrcFileType = ParamValue.lVal;
		return S_OK;
	}

	if (_T("TilingHtmlPattern") == strParamName)
	{
		if (1 == m_oWriter.m_lTilingCounter)
		{
			m_bIsGraphicsDumperMode = true;
			m_oDumper.m_oTile.LoadFromXml((CString)ParamValue.bstrVal);

			m_oDumper.NewPage(m_oWriter.m_dWidth, m_oWriter.m_dHeight);

			m_oDumper.m_oTile.bbox_x = 0;
			m_oDumper.m_oTile.bbox_y = 0;
			m_oDumper.m_oTile.bbox_r = m_oDumper.m_lWidthPix - 1;
			m_oDumper.m_oTile.bbox_b = m_oDumper.m_lHeightPix - 1;

			m_oDumper.m_oBounds.left = 0;
			m_oDumper.m_oBounds.top = 0;
			m_oDumper.m_oBounds.right = m_oDumper.m_lWidthPix - 1;
			m_oDumper.m_oBounds.bottom = m_oDumper.m_lHeightPix - 1;
		}		
	}

	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::GetAdditionalParam(BSTR ParamName, VARIANT* ParamValue)
{
	CString sParamName = ParamName;
	if ( _T("MaxImageSize") == sParamName )
	{
		ParamValue->vt		= VT_I4;
		ParamValue->lVal	= 800;
	}
	if (_T("TilingHtmlPattern") == sParamName)
	{
		if (0 == m_oWriter.m_lTilingCounter)
		{
			// иначе придут команды для отрисовщика (CGraphicsDumper)
			ParamValue->vt		= VT_BOOL;
			ParamValue->boolVal = VARIANT_TRUE;
		}
	}
	return S_OK;
}
// --------------------------------------------------------------------------------------------

// file
STDMETHODIMP CASCHTMLRenderer3::CreateOfficeFile(BSTR bsFilePath)
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

	m_oWriter.m_oSmartText.m_oFontManager.Init(_T(""));

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

	CoCreateInstance(Graphics::CLSID_CASCGraphicSimpleComverter, NULL, CLSCTX_ALL, Graphics::IID_IASCGraphicSimpleComverter, (void**)&m_pSimpleGraphicsConverter);
	m_oWriter.m_pSimpleConverter = m_pSimpleGraphicsConverter;

	IUnknown* punkRenderer = NULL;
	this->QueryInterface(IID_IUnknown, (void**)&punkRenderer);

	m_pSimpleGraphicsConverter->put_Renderer(punkRenderer);
	RELEASEINTERFACE(punkRenderer);

	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::CloseFile()
{
	if (m_bPageOpened)
	{
		EndPage();
	}
	
	m_oWriter.WriteEndDocument(m_oDocument);
	RELEASEINTERFACE(m_pSimpleGraphicsConverter);
	return S_OK;
}

STDMETHODIMP CASCHTMLRenderer3::get_Mode(LONG* plMode)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_Mode(LONG lMode)
{
	return S_OK;
}
