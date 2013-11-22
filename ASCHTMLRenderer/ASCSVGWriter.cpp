#include "stdafx.h"
#include "./ASCSVGWriter.h"

CASCSVGWriter::CASCSVGWriter()
{
}

HRESULT CASCSVGWriter::FinalConstruct()
{
	m_dDpiX = 96.0;
	m_dDpiY	= 96.0;

	m_dWidth = 100;
	m_dHeight = 100;
	m_bIsRaster = false;

	m_lClipMode = c_nClipRegionTypeWinding;

	m_pSimpleGraphicsConverter = NULL;
	CoCreateInstance(Graphics::CLSID_CASCGraphicSimpleComverter, NULL, CLSCTX_ALL, Graphics::IID_IASCGraphicSimpleComverter, (void**)&m_pSimpleGraphicsConverter);

	IUnknown* punkRenderer = NULL;
	this->QueryInterface(IID_IUnknown, (void**)&punkRenderer);

	m_pSimpleGraphicsConverter->put_Renderer(punkRenderer);
	RELEASEINTERFACE(punkRenderer);

	m_dTransformAngle = 0.0;

	m_pFonts = NULL;
	m_pFontManager = NULL;

	CoCreateInstance(__uuidof(Graphics::CASCFontManager), NULL, CLSCTX_ALL, __uuidof(Graphics::IASCFontManager), (void**)&m_pFontManager);
	m_pFontManager->Initialize(L"");

	m_oVectorWriter.m_pSimpleConverter  = m_pSimpleGraphicsConverter;
	m_oVectorWriter.m_pFullTransform	= &m_oFullTransform;
	m_oVectorWriter.SetSettings(&m_oPen, &m_oBrush, m_pSimpleGraphicsConverter);

	return S_OK;
}

void CASCSVGWriter::FinalRelease()
{
	RELEASEINTERFACE(m_pSimpleGraphicsConverter);
	RELEASEINTERFACE(m_pFonts);
	RELEASEINTERFACE(m_pFontManager);
}

STDMETHODIMP CASCSVGWriter::get_Type(LONG* lType)
{
	if (NULL == lType)
		return S_FALSE;

	*lType = c_nSVGRendrerer;
	return S_OK;
}
//-------- Функции для работы со страницей --------------------------------------------------
STDMETHODIMP CASCSVGWriter::NewPage()
{	
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::get_Height(double* dHeight)
{
	*dHeight = m_dHeight;	
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::put_Height(double dHeight)
{
	m_dHeight = dHeight;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::get_Width(double* dWidth)
{
	*dWidth = m_dWidth;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::put_Width(double dWidth)
{
	m_dWidth = dWidth;
	return S_OK;
}

STDMETHODIMP CASCSVGWriter::get_DpiX(double* dDpiX)
{
	if (NULL != dDpiX)
		*dDpiX = m_dDpiX;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::get_DpiY(double* dDpiY)
{
	if (NULL != dDpiY)
		*dDpiY = m_dDpiY;
	return S_OK;
}
// pen --------------------------------------------------------------------------------------
STDMETHODIMP CASCSVGWriter::SetPen(BSTR bsXML)
{
	m_oPen.FromXmlString((CString)bsXML);
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::get_PenColor(LONG* lColor)
{
	*lColor = m_oPen.Color;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::put_PenColor(LONG lColor)
{
	m_oPen.Color = lColor;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::get_PenAlpha(LONG* lAlpha)
{
	*lAlpha = m_oPen.Alpha;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::put_PenAlpha(LONG lAlpha)
{
	m_oPen.Alpha = lAlpha;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::get_PenSize(double* dSize)
{
	*dSize = m_oPen.Size;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::put_PenSize(double dSize)
{
	m_oPen.Size = dSize;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::get_PenDashStyle(BYTE* val)
{
	*val = m_oPen.DashStyle;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::put_PenDashStyle(BYTE val)
{
	m_oPen.DashStyle = val;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::get_PenLineStartCap(BYTE* val)
{
	*val = m_oPen.LineStartCap;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::put_PenLineStartCap(BYTE val)
{
	m_oPen.LineStartCap = val;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::get_PenLineEndCap(BYTE* val)
{
	*val = m_oPen.LineEndCap;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::put_PenLineEndCap(BYTE val)
{
	m_oPen.LineEndCap = val;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::get_PenLineJoin(BYTE* val)
{
	*val = m_oPen.LineJoin;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::put_PenLineJoin(BYTE val)
{
	m_oPen.LineJoin = val;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::get_PenDashOffset(double* val)
{
	*val = m_oPen.DashOffset;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::put_PenDashOffset(double val)
{
	m_oPen.DashOffset = val;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::get_PenAlign(LONG* val)
{
	*val = m_oPen.Align;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::put_PenAlign(LONG val)
{
	m_oPen.Align = val;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::get_PenMiterLimit(double* val)
{
	*val = m_oPen.MiterLimit;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::put_PenMiterLimit(double val)
{
	m_oPen.MiterLimit = val;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::PenDashPattern(SAFEARRAY* pPattern)
{
	if (NULL != pPattern)
	{
		m_oPen.SetDashPattern((double*)pPattern->pvData, pPattern->rgsabound[0].cElements);
	}

	return S_OK;
}
// brush ------------------------------------------------------------------------------------
STDMETHODIMP CASCSVGWriter::SetBrush(BSTR bsXML)
{
	m_oBrush.FromXmlString((CString)bsXML);
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::get_BrushType(LONG* lType)
{
	*lType = m_oBrush.Type;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::put_BrushType(LONG lType)
{
	m_oBrush.Type = lType;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::get_BrushColor1(LONG* lColor)
{
	*lColor = m_oBrush.Color1;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::put_BrushColor1(LONG lColor)
{
	m_oBrush.Color1 = lColor;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::get_BrushAlpha1(LONG* lAlpha)
{
	*lAlpha = m_oBrush.Alpha1;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::put_BrushAlpha1(LONG lAlpha)
{
	m_oBrush.Alpha1 = lAlpha;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::get_BrushColor2(LONG* lColor)
{
	*lColor = m_oBrush.Color2;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::put_BrushColor2(LONG lColor)
{
	m_oBrush.Color2 = lColor;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::get_BrushAlpha2(LONG* lAlpha)
{
	*lAlpha = m_oBrush.Alpha2;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::put_BrushAlpha2(LONG lAlpha)
{
	m_oBrush.Alpha2 = lAlpha;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::get_BrushTexturePath(BSTR* bsPath)
{
	*bsPath = m_oBrush.TexturePath.AllocSysString();
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::put_BrushTexturePath(BSTR bsPath)
{
	m_oBrush.TexturePath = (CString)bsPath;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::get_BrushTextureMode(LONG* lMode)
{
	*lMode = m_oBrush.TextureMode;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::put_BrushTextureMode(LONG lMode)
{
	m_oBrush.TextureMode = lMode;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::get_BrushTextureAlpha(LONG* lTxAlpha)
{
	*lTxAlpha = m_oBrush.TextureAlpha;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::put_BrushTextureAlpha(LONG lTxAlpha)
{
	m_oBrush.TextureAlpha = lTxAlpha;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::get_BrushLinearAngle(double* dAngle)
{
	*dAngle = m_oBrush.LinearAngle;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::put_BrushLinearAngle(double dAngle)
{
	m_oBrush.LinearAngle = dAngle;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::BrushRect(BOOL val, double left, double top, double width, double height)
{
	m_oBrush.Rectable = val;
	m_oBrush.Rect.X = (float)left;
	m_oBrush.Rect.Y = (float)top;
	m_oBrush.Rect.Width  = (float)width;
	m_oBrush.Rect.Height = (float)height;	

	return S_OK;
}
// font -------------------------------------------------------------------------------------
STDMETHODIMP CASCSVGWriter::SetFont(BSTR bsXML)
{
	m_oFont.FromXmlString((CString)bsXML);
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::get_FontName(BSTR* bsName)
{
	*bsName = m_oFont.Name.AllocSysString();
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::put_FontName(BSTR bsName)
{
	m_oFont.Name = (CString)bsName;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::get_FontPath(BSTR* bsName)
{
	*bsName = m_oFont.Path.AllocSysString();
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::put_FontPath(BSTR bsName)
{
	m_oFont.Path = bsName;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::get_FontSize(double* dSize)
{
	*dSize = m_oFont.Size;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::put_FontSize(double dSize)
{
	m_oFont.Size = dSize;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::get_FontStyle(LONG* lStyle)
{
	*lStyle = m_oFont.GetStyle();
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::put_FontStyle(LONG lStyle)
{
	m_oFont.SetStyle(lStyle);
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::get_FontStringGID(BOOL* bGID)
{
	*bGID = m_oFont.StringGID;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::put_FontStringGID(BOOL bGID)
{
	m_oFont.StringGID = bGID;
	m_pFontManager->SetStringGID(bGID);
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::get_FontCharSpace(double* dSpace)
{
	*dSpace = m_oFont.CharSpace;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::put_FontCharSpace(double dSpace)
{
	m_oFont.CharSpace = dSpace;
	return S_OK;
}
// shadow -----------------------------------------------------------------------------------
STDMETHODIMP CASCSVGWriter::SetShadow(BSTR bsXML)
{
	m_oShadow.FromXmlString((CString)bsXML);
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::get_ShadowDistanceX(double* val)
{
	*val = m_oShadow.DistanceX;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::put_ShadowDistanceX(double val)
{
	m_oShadow.DistanceX = val;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::get_ShadowDistanceY(double* val)
{
	*val = m_oShadow.DistanceY;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::put_ShadowDistanceY(double val)
{
	m_oShadow.DistanceY = val;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::get_ShadowBlurSize(double* val)
{
	*val = m_oShadow.BlurSize;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::put_ShadowBlurSize(double val)
{
	m_oShadow.BlurSize = val;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::get_ShadowColor(LONG* val)
{
	*val = m_oShadow.Color;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::put_ShadowColor(LONG val)
{
	m_oShadow.Color = val;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::get_ShadowAlpha(LONG* val)
{
	*val = m_oShadow.Alpha;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::put_ShadowAlpha(LONG val)
{
	m_oShadow.Alpha = val;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::get_ShadowVisible(BOOL* val)
{
	*val = m_oShadow.Visible;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::put_ShadowVisible(BOOL val)
{
	m_oShadow.Visible = val;
	return S_OK;
}
// edge -------------------------------------------------------------------------------------
STDMETHODIMP CASCSVGWriter::SetEdgeText(BSTR bsXML)
{
	m_oEdge.FromXmlString((CString)bsXML);
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::get_EdgeVisible(LONG* val)
{
	*val = m_oEdge.Visible;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::put_EdgeVisible(LONG val)
{
	m_oEdge.Visible = val;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::get_EdgeColor(LONG* val)
{
	*val = m_oEdge.Color;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::put_EdgeColor(LONG val)
{
	m_oEdge.Color = val;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::get_EdgeAlpha(LONG* val)
{
	*val = m_oEdge.Alpha;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::put_EdgeAlpha(LONG val)
{
	m_oEdge.Alpha = val;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::get_EdgeDist(double* val)
{
	*val = m_oEdge.Dist;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::put_EdgeDist(double val)
{
	m_oEdge.Dist = val;
	return S_OK;
}
//-------- Функции для вывода текста --------------------------------------------------------
STDMETHODIMP CASCSVGWriter::CommandDrawText(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
{
	if (c_nHyperlinkType == m_lCurrentCommandType)
		return S_OK;

	if (m_bIsRaster)
		return S_OK;

	PathCommandEnd();
	BeginCommand(c_nPathType);
	PathCommandText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);
	DrawPath(c_nWindingFillMode);
	EndCommand(c_nPathType);
	PathCommandEnd();
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::CommandDrawTextEx(BSTR bsText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
{
	if (m_bIsRaster)
		return S_OK;

	PathCommandEnd();
	BeginCommand(c_nPathType);
	PathCommandTextEx(bsText, bsGidText, bsSourceCodeText, fX, fY, fWidth, fHeight, fBaseLineOffset, lFlags);
	DrawPath(c_nWindingFillMode);
	EndCommand(c_nPathType);
	PathCommandEnd();
	return S_OK;	
}
//-------- Маркеры для команд ---------------------------------------------------------------
STDMETHODIMP CASCSVGWriter::BeginCommand(DWORD lType)
{
	if (m_bIsRaster)
		return S_OK;

	if (c_nClipType == lType)
	{
		m_oVectorWriter.WritePathClip();
	}
	else if (c_nPathType == lType)
	{
		m_oVectorWriter.WritePathStart();
		m_pSimpleGraphicsConverter->PathCommandEnd();
	}	
	m_lCurrentCommandType = lType;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::EndCommand(DWORD lType)
{
	if (m_bIsRaster)
		return S_OK;

	if (c_nClipType == lType)
	{
		m_oVectorWriter.WritePathClipEnd();
	}
	else if (c_nResetClipType == lType)
	{
		m_oVectorWriter.WritePathResetClip();
	}	
	m_lCurrentCommandType = -1;
	return S_OK;
}
//-------- Функции для работы с Graphics Path -----------------------------------------------
STDMETHODIMP CASCSVGWriter::PathCommandMoveTo(double fX, double fY)
{
	if (m_bIsRaster)
		return S_OK;

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
STDMETHODIMP CASCSVGWriter::PathCommandLineTo(double fX, double fY)
{
	if (m_bIsRaster)
		return S_OK;

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
STDMETHODIMP CASCSVGWriter::PathCommandLinesTo(SAFEARRAY* pPoints)
{
	if (m_bIsRaster)
		return S_OK;

	m_pSimpleGraphicsConverter->PathCommandLinesTo(pPoints);
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::PathCommandCurveTo(double fX1, double fY1, double fX2, double fY2, double fX3, double fY3)
{
	if (m_bIsRaster)
		return S_OK;

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
STDMETHODIMP CASCSVGWriter::PathCommandCurvesTo(SAFEARRAY* pPoints)
{
	if (m_bIsRaster)
		return S_OK;

	m_pSimpleGraphicsConverter->PathCommandCurvesTo(pPoints);
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::PathCommandArcTo(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle)
{
	if (m_bIsRaster)
		return S_OK;

	m_pSimpleGraphicsConverter->PathCommandArcTo(fX, fY, fWidth, fHeight, fStartAngle, fSweepAngle);
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::PathCommandClose()
{
	if (m_bIsRaster)
		return S_OK;

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
STDMETHODIMP CASCSVGWriter::PathCommandEnd()
{
	if (m_bIsRaster)
		return S_OK;

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
STDMETHODIMP CASCSVGWriter::DrawPath(long nType)
{
	if (m_oBrush.Type == c_BrushTypeTexture)
		m_bIsRaster = true;

	if (m_bIsRaster)
		return S_OK;

	m_oVectorWriter.WriteDrawPath(nType);
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::PathCommandStart()
{
	if (m_bIsRaster)
		return S_OK;

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
STDMETHODIMP CASCSVGWriter::PathCommandGetCurrentPoint(double* fX, double* fY)
{
	m_pSimpleGraphicsConverter->PathCommandGetCurrentPoint(fX, fY);	
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::PathCommandText(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
{
	if (m_bIsRaster)
		return S_OK;

	_SetFont();
	m_pSimpleGraphicsConverter->PathCommandText(bsText, m_pFontManager, fX, fY, fWidth, fHeight, fBaseLineOffset);
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::PathCommandTextEx(BSTR bsText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
{
	if (m_bIsRaster)
		return S_OK;

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
STDMETHODIMP CASCSVGWriter::DrawImage(IUnknown* pInterface, double fX, double fY, double fWidth, double fHeight)
{
	m_bIsRaster = true;
	/*
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
	*/

	return S_OK;
}
STDMETHODIMP CASCSVGWriter::DrawImageFromFile(BSTR bstrVal, double fX, double fY, double fWidth, double fHeight)
{
	m_bIsRaster = true;
	/*
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
	*/

	return S_OK;
}
// transform --------------------------------------------------------------------------------
STDMETHODIMP CASCSVGWriter::GetCommandParams(double* dAngle, double* dLeft, double* dTop, double* dWidth, double* dHeight, DWORD* lFlags)
{
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::SetCommandParams(double dAngle, double dLeft, double dTop, double dWidth, double dHeight, DWORD lFlags)
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
	CalculateFullTransform();

	return S_OK;
}
STDMETHODIMP CASCSVGWriter::SetTransform(double dA, double dB, double dC, double dD, double dE, double dF)
{
	NSHtmlRenderer::CMatrix oTrans(dA, dB, dC, dD, dE, dF);
	m_oTransform = oTrans;

	CalculateFullTransform();

	return S_OK;
}
STDMETHODIMP CASCSVGWriter::GetTransform(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF)
{
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::ResetTransform(void)
{
	m_oTransform.Reset();
	CalculateFullTransform();
	return S_OK;
}
// -----------------------------------------------------------------------------------------
STDMETHODIMP CASCSVGWriter::get_ClipMode(LONG* plMode)
{
	*plMode = m_lClipMode;
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::put_ClipMode(LONG lMode)
{
	m_lClipMode = lMode;
	m_oVectorWriter.SetClipMode(m_lClipMode);
	return S_OK;
}
// additiaonal params ----------------------------------------------------------------------
STDMETHODIMP CASCSVGWriter::SetAdditionalParam(BSTR ParamName, VARIANT ParamValue)
{
	CString sParamName = ParamName;
	if ( _T("SaveFile") == sParamName && ParamValue.vt == VT_BSTR)
	{
		CString strFileSave = (CString)ParamValue.bstrVal;
		CFile oFile;
		if (oFile.CreateFile(strFileSave) == S_OK)
		{
			LONG lW = (LONG)m_dWidth;
			LONG lH = (LONG)m_dHeight;
			CString strStart = _T("");
			strStart.Format(NSHtmlRenderer::g_svg_string_svgOpen, (LONG)(m_dWidth / 100), (LONG)(m_dHeight / 100), lW, lH);

			oFile.WriteStringUTF8(strStart);

			CString s = m_oVectorWriter.m_oSVG.m_oDocument.GetCString();
			oFile.WriteStringUTF8(s);

			CString strEnd = _T("</svg>");
			oFile.WriteStringUTF8(strEnd);
		}
		oFile.CloseFile();
	}
	else if (_T("ReInit") == sParamName)
	{
		m_dDpiX = 96.0;
		m_dDpiY	= 96.0;

		m_dWidth = 100;
		m_dHeight = 100;
		m_bIsRaster = false;

		m_lClipMode = c_nClipRegionTypeWinding;

		RELEASEINTERFACE(m_pSimpleGraphicsConverter);
		CoCreateInstance(Graphics::CLSID_CASCGraphicSimpleComverter, NULL, CLSCTX_ALL, Graphics::IID_IASCGraphicSimpleComverter, (void**)&m_pSimpleGraphicsConverter);

		IUnknown* punkRenderer = NULL;
		this->QueryInterface(IID_IUnknown, (void**)&punkRenderer);

		m_pSimpleGraphicsConverter->put_Renderer(punkRenderer);
		RELEASEINTERFACE(punkRenderer);

		m_dTransformAngle = 0.0;

		m_pFonts = NULL;
		
		m_oVectorWriter.m_pSimpleConverter  = m_pSimpleGraphicsConverter;
		m_oVectorWriter.m_pFullTransform	= &m_oFullTransform;
		m_oVectorWriter.SetSettings(&m_oPen, &m_oBrush, m_pSimpleGraphicsConverter);

		m_oPen.SetDefaultParams();
		m_oBrush.SetDefaultParams();
		m_oFont.SetDefaultParams();

		m_oBaseTransform.Reset();
		m_oTransform.Reset();
		m_oFullTransform.Reset();

		m_oVectorWriter.m_oSVG.ReInit();
	}
	return S_OK;
}
STDMETHODIMP CASCSVGWriter::GetAdditionalParam(BSTR ParamName, VARIANT* ParamValue)
{
	CString sParamName = ParamName;
	if ( _T("IsRaster") == sParamName )
	{
		ParamValue->vt		= VT_BOOL;
		ParamValue->boolVal	= m_bIsRaster ? VARIANT_TRUE : VARIANT_FALSE;
	}
	
	return S_OK;
}
// --------------------------------------------------------------------------------------------