#include "stdafx.h"
#include "SVGRenderer.h"

CASCSVGRenderer::CASCSVGRenderer()
{
}

HRESULT CASCSVGRenderer::FinalConstruct()
{
	m_oInit.Init();
	
	m_dDpiX = 96.0;
	m_dDpiY	= 96.0;

	m_dWidth	= -1;
	m_dHeight	= -1;

	m_lClipMode			= c_nClipRegionTypeWinding;

	m_pSimpleGraphicsConverter = NULL;
	CoCreateInstance(__uuidof(CASCGraphicSimpleComverter), NULL, CLSCTX_ALL, __uuidof(IASCGraphicSimpleComverter), (void**)&m_pSimpleGraphicsConverter);

	m_pFontManager = NULL;
	
	IUnknown* punkRenderer = NULL;
	this->QueryInterface(IID_IUnknown, (void**)&punkRenderer);

	m_pSimpleGraphicsConverter->put_Renderer(punkRenderer);
	RELEASEINTERFACE(punkRenderer);

	m_pFonts		= NULL;

	m_oInstalledFont.Name = _T("");

	return S_OK;
}

void CASCSVGRenderer::FinalRelease()
{
	RELEASEINTERFACE(m_pSimpleGraphicsConverter);
	RELEASEINTERFACE(m_pFonts);
	RELEASEINTERFACE(m_pFontManager);
}

STDMETHODIMP CASCSVGRenderer::get_Type(LONG* lType)
{
	if (NULL == lType)
		return S_FALSE;

	*lType = c_nSVGRendrerer;
	return S_OK;
}
//-------- Функции для работы со страницей --------------------------------------------------
STDMETHODIMP CASCSVGRenderer::NewPage()
{
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::get_Height(double* dHeight)
{
	*dHeight = m_dHeight;	
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::put_Height(double dHeight)
{
	m_dHeight = dHeight;

	if (-1 != m_dWidth)
		OnBaseMatrixUpdate();

	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::get_Width(double* dWidth)
{
	*dWidth = m_dWidth;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::put_Width(double dWidth)
{
	m_dWidth = dWidth;

	if (-1 != m_dHeight)
		OnBaseMatrixUpdate();

	return S_OK;
}

STDMETHODIMP CASCSVGRenderer::get_DpiX(double* dDpiX)
{
	if (NULL != dDpiX)
		*dDpiX = m_dDpiX;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::get_DpiY(double* dDpiY)
{
	if (NULL != dDpiY)
		*dDpiY = m_dDpiY;
	return S_OK;
}
// pen --------------------------------------------------------------------------------------
STDMETHODIMP CASCSVGRenderer::SetPen(BSTR bsXML)
{
	m_oPen.FromXmlString((CString)bsXML);
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::get_PenColor(LONG* lColor)
{
	*lColor = m_oPen.Color;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::put_PenColor(LONG lColor)
{
	m_oPen.Color = (lColor & 0x00FFFFFF);

	DWORD dwVal = (DWORD)lColor;
	LONG lSchemeIndex = ((BYTE)(dwVal >> 24) - 100);
	if (lSchemeIndex >= 0)
		m_lColorSchemeIndexPen1 = lSchemeIndex;
	else
		m_lColorSchemeIndexPen1 = -1;

	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::get_PenAlpha(LONG* lAlpha)
{
	*lAlpha = m_oPen.Alpha;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::put_PenAlpha(LONG lAlpha)
{
	m_oPen.Alpha = lAlpha;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::get_PenSize(double* dSize)
{
	*dSize = m_oPen.Size;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::put_PenSize(double dSize)
{
	m_oPen.Size = dSize;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::get_PenDashStyle(BYTE* val)
{
	*val = m_oPen.DashStyle;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::put_PenDashStyle(BYTE val)
{
	m_oPen.DashStyle = val;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::get_PenLineStartCap(BYTE* val)
{
	*val = m_oPen.LineStartCap;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::put_PenLineStartCap(BYTE val)
{
	m_oPen.LineStartCap = val;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::get_PenLineEndCap(BYTE* val)
{
	*val = m_oPen.LineEndCap;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::put_PenLineEndCap(BYTE val)
{
	m_oPen.LineEndCap = val;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::get_PenLineJoin(BYTE* val)
{
	*val = m_oPen.LineJoin;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::put_PenLineJoin(BYTE val)
{
	m_oPen.LineJoin = val;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::get_PenDashOffset(double* val)
{
	*val = m_oPen.DashOffset;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::put_PenDashOffset(double val)
{
	m_oPen.DashOffset = val;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::get_PenAlign(LONG* val)
{
	*val = m_oPen.Align;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::put_PenAlign(LONG val)
{
	m_oPen.Align = val;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::get_PenMiterLimit(double* val)
{
	*val = m_oPen.MiterLimit;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::put_PenMiterLimit(double val)
{
	m_oPen.MiterLimit = val;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::PenDashPattern(SAFEARRAY* pPattern)
{
	if (NULL != pPattern)
	{
		m_oPen.SetDashPattern((double*)pPattern->pvData, pPattern->rgsabound[0].cElements);
	}
	return S_OK;
}
// brush ------------------------------------------------------------------------------------
STDMETHODIMP CASCSVGRenderer::SetBrush(BSTR bsXML)
{
	m_oBrush.FromXmlString((CString)bsXML);
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::get_BrushType(LONG* lType)
{
	*lType = m_oBrush.Type;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::put_BrushType(LONG lType)
{
	m_oBrush.Type = lType;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::get_BrushColor1(LONG* lColor)
{
	*lColor = m_oBrush.Color1;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::put_BrushColor1(LONG lColor)
{
	m_oBrush.Color1 = (lColor & 0x00FFFFFF);

	DWORD dwVal = (DWORD)lColor;
	LONG lSchemeIndex = ((BYTE)(dwVal >> 24) - 100);
	if (lSchemeIndex >= 0)
		m_lColorSchemeIndexBrush1 = lSchemeIndex;
	else
		m_lColorSchemeIndexBrush1 = -1;

	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::get_BrushAlpha1(LONG* lAlpha)
{
	*lAlpha = m_oBrush.Alpha1;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::put_BrushAlpha1(LONG lAlpha)
{
	m_oBrush.Alpha1 = lAlpha;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::get_BrushColor2(LONG* lColor)
{
	*lColor = m_oBrush.Color2;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::put_BrushColor2(LONG lColor)
{
	m_oBrush.Color2 = lColor;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::get_BrushAlpha2(LONG* lAlpha)
{
	*lAlpha = m_oBrush.Alpha2;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::put_BrushAlpha2(LONG lAlpha)
{
	m_oBrush.Alpha2 = lAlpha;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::get_BrushTexturePath(BSTR* bsPath)
{
	*bsPath = m_oBrush.TexturePath.AllocSysString();
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::put_BrushTexturePath(BSTR bsPath)
{
	m_oBrush.TexturePath = (CString)bsPath;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::get_BrushTextureMode(LONG* lMode)
{
	*lMode = m_oBrush.TextureMode;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::put_BrushTextureMode(LONG lMode)
{
	m_oBrush.TextureMode = lMode;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::get_BrushTextureAlpha(LONG* lTxAlpha)
{
	*lTxAlpha = m_oBrush.TextureAlpha;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::put_BrushTextureAlpha(LONG lTxAlpha)
{
	m_oBrush.TextureAlpha = lTxAlpha;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::get_BrushLinearAngle(double* dAngle)
{
	*dAngle = m_oBrush.LinearAngle;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::put_BrushLinearAngle(double dAngle)
{
	m_oBrush.LinearAngle = dAngle;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::BrushRect(BOOL val, double left, double top, double width, double height)
{
	m_oBrush.Rectable = val;
	m_oBrush.Rect.X = (float)left;
	m_oBrush.Rect.Y = (float)top;
	m_oBrush.Rect.Width  = (float)width;
	m_oBrush.Rect.Height = (float)height;	

	return S_OK;
}
// font -------------------------------------------------------------------------------------
STDMETHODIMP CASCSVGRenderer::SetFont(BSTR bsXML)
{
	m_oFont.FromXmlString((CString)bsXML);
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::get_FontName(BSTR* bsName)
{
	*bsName = m_oFont.Name.AllocSysString();
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::put_FontName(BSTR bsName)
{
	m_oFont.Name = (CString)bsName;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::get_FontPath(BSTR* bsName)
{
	*bsName = m_oFont.Path.AllocSysString();
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::put_FontPath(BSTR bsName)
{
	m_oFont.Path = bsName;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::get_FontSize(double* dSize)
{
	*dSize = m_oFont.Size;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::put_FontSize(double dSize)
{
	m_oFont.Size = dSize;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::get_FontStyle(LONG* lStyle)
{
	*lStyle = m_oFont.GetStyle();
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::put_FontStyle(LONG lStyle)
{
	m_oFont.SetStyle(lStyle);
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::get_FontStringGID(BOOL* bGID)
{
	*bGID = m_oFont.StringGID;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::put_FontStringGID(BOOL bGID)
{
	m_oFont.StringGID = bGID;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::get_FontCharSpace(double* dSpace)
{
	*dSpace = m_oFont.CharSpace;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::put_FontCharSpace(double dSpace)
{
	m_oFont.CharSpace = dSpace;
	return S_OK;
}
// shadow -----------------------------------------------------------------------------------
STDMETHODIMP CASCSVGRenderer::SetShadow(BSTR bsXML)
{
	m_oShadow.FromXmlString((CString)bsXML);
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::get_ShadowDistanceX(double* val)
{
	*val = m_oShadow.DistanceX;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::put_ShadowDistanceX(double val)
{
	m_oShadow.DistanceX = val;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::get_ShadowDistanceY(double* val)
{
	*val = m_oShadow.DistanceY;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::put_ShadowDistanceY(double val)
{
	m_oShadow.DistanceY = val;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::get_ShadowBlurSize(double* val)
{
	*val = m_oShadow.BlurSize;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::put_ShadowBlurSize(double val)
{
	m_oShadow.BlurSize = val;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::get_ShadowColor(LONG* val)
{
	*val = m_oShadow.Color;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::put_ShadowColor(LONG val)
{
	m_oShadow.Color = val;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::get_ShadowAlpha(LONG* val)
{
	*val = m_oShadow.Alpha;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::put_ShadowAlpha(LONG val)
{
	m_oShadow.Alpha = val;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::get_ShadowVisible(BOOL* val)
{
	*val = m_oShadow.Visible;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::put_ShadowVisible(BOOL val)
{
	m_oShadow.Visible = val;
	return S_OK;
}
// edge -------------------------------------------------------------------------------------
STDMETHODIMP CASCSVGRenderer::SetEdgeText(BSTR bsXML)
{
	m_oEdge.FromXmlString((CString)bsXML);
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::get_EdgeVisible(LONG* val)
{
	*val = m_oEdge.Visible;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::put_EdgeVisible(LONG val)
{
	m_oEdge.Visible = val;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::get_EdgeColor(LONG* val)
{
	*val = m_oEdge.Color;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::put_EdgeColor(LONG val)
{
	m_oEdge.Color = val;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::get_EdgeAlpha(LONG* val)
{
	*val = m_oEdge.Alpha;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::put_EdgeAlpha(LONG val)
{
	m_oEdge.Alpha = val;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::get_EdgeDist(double* val)
{
	*val = m_oEdge.Dist;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::put_EdgeDist(double val)
{
	m_oEdge.Dist = val;
	return S_OK;
}
//-------- Функции для вывода текста --------------------------------------------------------
STDMETHODIMP CASCSVGRenderer::CommandDrawText(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
{
	if (c_nHyperlinkType == m_lCurrentCommandType)
		return S_OK;

	PathCommandStart();
	PathCommandText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);

	//double	dOldPenW = m_oPen.Size;
	//long	lOldPenC = m_oPen.Color;
	//long	lOldPenA = m_oPen.Alpha;

	//m_oPen.Size  = 25.4 / 96;
	//m_oPen.Color = m_oBrush.Color1;
	//m_oPen.Alpha = m_oBrush.Alpha1;

	DrawPath(0x0100);

	//m_oPen.Size  = dOldPenW;
	//m_oPen.Color = lOldPenC;
	//m_oPen.Alpha = lOldPenA;
	
	PathCommandEnd();
	
	//double x = fX;
	//double y = fY;
	//m_oFullTransform.TransformPoint(x, y);

	//double r = fX + fWidth;
	//double b = fY + fHeight;
	//m_oFullTransform.TransformPoint(r, b);

	//m_oWriter.WriteText(bsText, NULL, x, y, r - x, b - y, fBaseLineOffset);

	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::CommandDrawTextEx(BSTR bsText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
{
	if (NULL != bsGidText)
	{
		m_oFont.StringGID = TRUE;
		return CommandDrawText(bsGidText, fX, fY, fWidth, fHeight, fBaseLineOffset);
	}
	
	m_oFont.StringGID = FALSE;
	return CommandDrawText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);
}
//-------- Маркеры для команд ---------------------------------------------------------------
STDMETHODIMP CASCSVGRenderer::BeginCommand(DWORD lType)
{
	if (c_nClipType == lType)
	{
		//m_oWriter.WritePathClip();
	}
	else if (c_nPathType == lType)
	{
		//m_oWriter.WriteBeginPath();
	}
	m_lCurrentCommandType = lType;

	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::EndCommand(DWORD lType)
{
	if (c_nPageType == lType)
	{
		EndPage();
	}
	else if (c_nClipType == lType)
	{
		//m_oWriter.WritePathClipEnd();
	}
	else if (c_nResetClipType == lType)
	{
		//m_oWriter.WritePathResetClip();
	}
	m_lCurrentCommandType = -1;

	return S_OK;
}
//-------- Функции для работы с Graphics Path -----------------------------------------------
STDMETHODIMP CASCSVGRenderer::PathCommandMoveTo(double fX, double fY)
{
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
STDMETHODIMP CASCSVGRenderer::PathCommandLineTo(double fX, double fY)
{
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
STDMETHODIMP CASCSVGRenderer::PathCommandLinesTo(SAFEARRAY* pPoints)
{
	m_pSimpleGraphicsConverter->PathCommandLinesTo(pPoints);
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::PathCommandCurveTo(double fX1, double fY1, double fX2, double fY2, double fX3, double fY3)
{
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
STDMETHODIMP CASCSVGRenderer::PathCommandCurvesTo(SAFEARRAY* pPoints)
{
	m_pSimpleGraphicsConverter->PathCommandCurvesTo(pPoints);
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::PathCommandArcTo(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle)
{
	m_pSimpleGraphicsConverter->PathCommandArcTo(fX, fY, fWidth, fHeight, fStartAngle, fSweepAngle);
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::PathCommandClose()
{
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
STDMETHODIMP CASCSVGRenderer::PathCommandEnd()
{
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
STDMETHODIMP CASCSVGRenderer::DrawPath(long nType)
{
	if (m_oWriterVML.GetCurSize() < 3)
		return S_OK;
	
	//m_oWriter.WriteDrawPath(nType);

	BOOL bStroke	= (0x01 == (0x01 & nType));
	BOOL bFill		= (0x01 < nType);

	if (0 == m_oPen.Alpha)
		bStroke = false;
	if ((0 == m_oBrush.Alpha1) && (c_BrushTypeSolid == m_oBrush.Type))
		bFill = false;
	else if ((0 == m_oBrush.TextureAlpha) && (c_BrushTypeTexture == m_oBrush.Type || c_BrushTypePattern == m_oBrush.Type))
		bFill = false;

	if (!bFill)
	{
		// stroke
		int nColorPen	= NSSVG::ConvertColor(m_oPen.Color);
		int nPenW		= NSSVG::round(m_oPen.Size * m_dDpiY / c_ag_Inch_to_MM);

		CString strStyle = _T("");
		if (-1 == m_lColorSchemeIndexPen1)
			strStyle.Format(g_string_vml_StyleStroke, nColorPen, nPenW, (double)m_oPen.Alpha / 255);
		else 
			strStyle.Format(g_string_vml_StyleStroke_class, m_lColorSchemeIndexPen1, nPenW, (double)m_oPen.Alpha / 255);

		m_oWriter.WriteString(g_bstr_vml_Path);
		m_oWriter.WriteString(strStyle);
		m_oWriter.WriteString(g_bstr_path_d);
		m_oWriter.Write(m_oWriterVML);
		m_oWriter.WriteString(g_bstr_path_d_end);
		m_oWriter.WriteString(g_bstr_nodeClose);

		return S_OK;
	}
	else if (c_BrushTypeTexture == m_oBrush.Type || c_BrushTypePattern == m_oBrush.Type)
	{
		double x = 0;
		double y = 0;
		double w = 0;
		double h = 0;
		m_pSimpleGraphicsConverter->PathCommandGetBounds(&x, &y, &w, &h);

		BSTR bsPath = m_oBrush.TexturePath.AllocSysString();
		DrawImageFromFile(bsPath, x, y, w, h);
		SysFreeString(bsPath);
		return S_OK;
		//double x = 0;
		//double y = 0;
		//double r = 0;
		//double b = 0;

		//m_pSimpleGraphicsConverter->PathCommandGetBounds(&x, &y, &r, &b);
		//r += x;
		//b += y;

		//m_oFullTransform.TransformPoint(x, y);
		//m_oFullTransform.TransformPoint(r, b);

		//CString str = _T("");
		//str.Format(g_string_image, NSSVG::round(x), NSSVG::round(y), NSSVG::round(r - x), NSSVG::round(b - y), m_oBrush.TexturePath);

		//m_oWriter.WriteString(str);
		//return S_OK;
	}
	
	int nColorBrush	= NSSVG::ConvertColor(m_oBrush.Color1);
	CString strMode = _T("nonzero");
	if (nType & c_nEvenOddFillMode)
		strMode = _T("evenodd");
	
	if (!bStroke)
	{
		CString strStyle = _T("");
		if (-1 == m_lColorSchemeIndexBrush1)
			strStyle.Format(g_string_vml_StyleFill, nColorBrush, (double)m_oBrush.Alpha1 / 255, strMode);
		else
			strStyle.Format(g_string_vml_StyleFill_class, m_lColorSchemeIndexBrush1, (double)m_oBrush.Alpha1 / 255, strMode);

		m_oWriter.WriteString(g_bstr_vml_Path);
		m_oWriter.WriteString(strStyle);
		m_oWriter.WriteString(g_bstr_path_d);
		m_oWriter.Write(m_oWriterVML);
		m_oWriter.WriteString(g_bstr_path_d_end);
		m_oWriter.WriteString(g_bstr_nodeClose);
		return S_OK;
	}
	
	int nPenColor = NSSVG::ConvertColor(m_oPen.Color);
	int nPenW		= NSSVG::round(m_oPen.Size * m_dDpiY / c_ag_Inch_to_MM);
	CString strStyle = _T("");
	if (-1 != m_lColorSchemeIndexPen1 && -1 != m_lColorSchemeIndexBrush1)
		strStyle.Format(g_string_vml_Style3, m_lColorSchemeIndexPen1, m_lColorSchemeIndexBrush1, (double)m_oBrush.Alpha1 / 255, strMode, nPenW, (double)m_oPen.Alpha / 255);
	else if (-1 != m_lColorSchemeIndexPen1)
		strStyle.Format(g_string_vml_Style1, m_lColorSchemeIndexPen1, nColorBrush, (double)m_oBrush.Alpha1 / 255, strMode, nPenW, (double)m_oPen.Alpha / 255);
	else if (-1 != m_lColorSchemeIndexBrush1)
		strStyle.Format(g_string_vml_Style2, m_lColorSchemeIndexBrush1, (double)m_oBrush.Alpha1 / 255, strMode, nPenColor, nPenW, (double)m_oPen.Alpha / 255);
	else
		strStyle.Format(g_string_vml_Style, nColorBrush, (double)m_oBrush.Alpha1 / 255, strMode, nPenColor, nPenW, (double)m_oPen.Alpha / 255);

	m_oWriter.WriteString(g_bstr_vml_Path);
	m_oWriter.WriteString(strStyle);
	m_oWriter.WriteString(g_bstr_path_d);
	m_oWriter.Write(m_oWriterVML);
	m_oWriter.WriteString(g_bstr_path_d_end);
	m_oWriter.WriteString(g_bstr_nodeClose);

	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::PathCommandStart()
{
	if (c_nSimpleGraphicType == m_lCurrentCommandType)
	{
		Start();
	}
	else
	{
		m_pSimpleGraphicsConverter->PathCommandStart();
	}

	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::PathCommandGetCurrentPoint(double* fX, double* fY)
{
	m_pSimpleGraphicsConverter->PathCommandGetCurrentPoint(fX, fY);	
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::PathCommandText(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
{
	if (NULL == m_pFontManager)
	{
		CoCreateInstance(__uuidof(CASCFontManager), NULL, CLSCTX_ALL, __uuidof(IASCFontManager), (void**)&m_pFontManager);
		m_pFontManager->Initialize(L"");
	}
	
	_SetFont();
	m_pSimpleGraphicsConverter->PathCommandText(bsText, m_pFontManager, fX, fY, fWidth, fHeight, fBaseLineOffset);
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::PathCommandTextEx(BSTR bsText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
{
	if (NULL != bsGidText)
	{
		m_oFont.StringGID = TRUE;
		return PathCommandText(bsGidText, fX, fY, fWidth, fHeight, fBaseLineOffset);
	}
	
	m_oFont.StringGID = FALSE;
	return PathCommandText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);
}
//-------- Функции для вывода изображений ---------------------------------------------------
STDMETHODIMP CASCSVGRenderer::DrawImage(IUnknown* pInterface, double fX, double fY, double fWidth, double fHeight)
{
	double x = fX;
	double y = fY;
	m_oFullTransform.TransformPoint(x, y);

	double r	= fX + fWidth;
	double b	= fY + fHeight;
	m_oFullTransform.TransformPoint(r, b);

	//m_oWriter.WriteImage(pInterface, x, y, r - x, b - y);

	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::DrawImageFromFile(BSTR bstrVal, double fX, double fY, double fWidth, double fHeight)
{
	double x = fX;
	double y = fY;
	m_oFullTransform.TransformPoint(x, y);

	double r	= fX + fWidth;
	double b	= fY + fHeight;
	m_oFullTransform.TransformPoint(r, b);

	CString strFile = (CString)bstrVal;

	CImageExt oExt;
	LONG lFileType = oExt.GetImageType(strFile);

	if ((1 == lFileType) || (2 == lFileType))
	{
		// метафайл
		oExt.DrawOnRenderer(this, bstrVal, fX, fY, fWidth, fHeight);
		return S_OK;
	}
	
	CString str = _T("");
	str.Format(g_string_image, NSSVG::round(x), NSSVG::round(y), NSSVG::round(r - x), NSSVG::round(b - y), (CString)bstrVal);

	m_oWriter.WriteString(str);
	return S_OK;
}
// transform --------------------------------------------------------------------------------
STDMETHODIMP CASCSVGRenderer::GetCommandParams(double* dAngle, double* dLeft, double* dTop, double* dWidth, double* dHeight, DWORD* lFlags)
{
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::SetCommandParams(double dAngle, double dLeft, double dTop, double dWidth, double dHeight, DWORD lFlags)
{
	CalculateFullTransform();
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::SetTransform(double dA, double dB, double dC, double dD, double dE, double dF)
{
	Aggplus::CMatrix oTrans(dA, dB, dC, dD, dE, dF);
	m_oTransform = oTrans;

	CalculateFullTransform();

	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::GetTransform(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF)
{
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::ResetTransform(void)
{
	m_oTransform.Reset();
	CalculateFullTransform();
	return S_OK;
}
// -----------------------------------------------------------------------------------------
STDMETHODIMP CASCSVGRenderer::get_ClipMode(LONG* plMode)
{
	*plMode = m_lClipMode;
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::put_ClipMode(LONG lMode)
{
	m_lClipMode = lMode;
	return S_OK;
}
// additiaonal params ----------------------------------------------------------------------
STDMETHODIMP CASCSVGRenderer::SetAdditionalParam(BSTR ParamName, VARIANT ParamValue)
{
	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::GetAdditionalParam(BSTR ParamName, VARIANT* ParamValue)
{
	return S_OK;
}
// --------------------------------------------------------------------------------------------

// file
STDMETHODIMP CASCSVGRenderer::CreateOfficeFile(BSTR bsFilePath, LONG lFonts)
{
	m_strDstFile = (CString)bsFilePath;
	m_oWriter.Clear();

	if (0 != lFonts)
	{
		if (NULL == m_pFonts)
		{
			CoCreateInstance(__uuidof(CASCWinFonts), NULL, CLSCTX_ALL, __uuidof(IASCWinFonts), (void**)&m_pFonts);
		}
		if (NULL == m_pFontManager)
		{
			CoCreateInstance(__uuidof(CASCFontManager), NULL, CLSCTX_ALL, __uuidof(IASCFontManager), (void**)&m_pFontManager);

			m_pFontManager->Initialize(L"");
			m_pFontManager->SetDefaultFont(L"Arial");
		}
	}

	double dW = m_dWidth;
	double dH = m_dHeight;
	m_oFullTransform.TransformPoint(dW, dH);

	int w = NSSVG::round(dW);
	int h = NSSVG::round(dH);

	CString strSVG = _T("");
	strSVG.Format(g_string_svgOpen, w, h, w, h);

	m_oWriter.WriteString(strSVG);
	m_oWriter.WriteString(g_bstr_graphicOpen);

	m_lColorSchemeIndexBrush1	= -1;
	m_lColorSchemeIndexPen1		= -1;

	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::CloseFile(LONG lSave)
{
	m_oWriter.WriteString(g_bstr_graphicClose);
	m_oWriter.WriteString(g_bstr_svgClose);

	if (0 == lSave)
		return S_OK;

	NSTextRenderer::CFile oFile;
	
	if (S_OK == oFile.CreateFile(m_strDstFile))
	{
		CString strData = m_oWriter.GetData();
		oFile.WriteFile((void*)strData.GetBuffer(), sizeof(WCHAR) * strData.GetLength());
	}

	return S_OK;
}
STDMETHODIMP CASCSVGRenderer::get_Data(BSTR* bsData)
{
	*bsData = m_oWriter.GetData().AllocSysString();
	return S_OK;
}
