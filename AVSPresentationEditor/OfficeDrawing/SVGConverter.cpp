#include "stdafx.h"
#include "SVGConverter.h"

NSPresentationEditor::CSVGConverter::CSVGConverter()
{
}

HRESULT NSPresentationEditor::CSVGConverter::FinalConstruct()
{
	m_dDpiX = 96.0;
	m_dDpiY	= 96.0;

	m_lClipMode = c_nClipRegionTypeWinding;

	m_pSimpleGraphicsConverter = NULL;
	CoCreateInstance(AVSGraphics::CLSID_CAVSGraphicSimpleComverter, NULL, CLSCTX_ALL, AVSGraphics::IID_IAVSGraphicSimpleComverter, (void**)&m_pSimpleGraphicsConverter);

	IUnknown* punkRenderer = NULL;
	this->QueryInterface(IID_IUnknown, (void**)&punkRenderer);

	m_pSimpleGraphicsConverter->put_Renderer(punkRenderer);
	RELEASEINTERFACE(punkRenderer);

	m_dTransformAngle = 0.0;

	m_dWidth	= 100;
	m_dHeight	= 100;

	m_pFontManager = NULL;
	return S_OK;
}

void NSPresentationEditor::CSVGConverter::FinalRelease()
{
	RELEASEINTERFACE(m_pSimpleGraphicsConverter);
}

STDMETHODIMP NSPresentationEditor::CSVGConverter::get_Type(LONG* lType)
{
	if (NULL == lType)
		return S_FALSE;

	*lType = c_nSVGConverter;
	return S_OK;
}
//-------- Функции для работы со страницей --------------------------------------------------
STDMETHODIMP NSPresentationEditor::CSVGConverter::NewPage()
{
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::get_Height(double* dHeight)
{
	if (NULL != dHeight)
		*dHeight = m_dHeight;	
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::put_Height(double dHeight)
{
	m_dHeight = dHeight;

	m_oBaseTransform.Reset();
	double dKoefBase = 96.0 / 25.4;
	double dScaleX = 43200 / m_dOffsetW;
	double dScaleY = 43200 / m_dOffsetH;
	m_oBaseTransform.Translate(-m_dOffsetX, -m_dOffsetY, Aggplus::MatrixOrderAppend);
	m_oBaseTransform.Scale(dScaleX, dScaleY, Aggplus::MatrixOrderAppend);
	
	CalculateFullTransform();

	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::get_Width(double* dWidth)
{
	if (NULL != dWidth)
		*dWidth = m_dWidth;	
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::put_Width(double dWidth)
{
	m_dWidth = dWidth;

	m_oBaseTransform.Reset();
	double dScaleX = 43200 / m_dOffsetW;
	double dScaleY = 43200 / m_dOffsetH;
	m_oBaseTransform.Translate(-m_dOffsetX, -m_dOffsetY, Aggplus::MatrixOrderAppend);
	m_oBaseTransform.Scale(dScaleX, dScaleY, Aggplus::MatrixOrderAppend);

	CalculateFullTransform();

	return S_OK;
}

STDMETHODIMP NSPresentationEditor::CSVGConverter::get_DpiX(double* dDpiX)
{
	if (NULL != dDpiX)
		*dDpiX = m_dDpiX;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::get_DpiY(double* dDpiY)
{
	if (NULL != dDpiY)
		*dDpiY = m_dDpiY;
	return S_OK;
}
// pen --------------------------------------------------------------------------------------
STDMETHODIMP NSPresentationEditor::CSVGConverter::SetPen(BSTR bsXML)
{
	//m_oPen.FromXmlString((CString)bsXML);
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::get_PenColor(LONG* lColor)
{
	*lColor = m_oPen.Color.GetLONG();
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::put_PenColor(LONG lColor)
{
	BYTE lScheme = ((DWORD)(lColor)) >> 24;

	if (0xFF != lScheme)
		m_oPen.Color.SetBGR(lColor);
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::get_PenAlpha(LONG* lAlpha)
{
	*lAlpha = m_oPen.Alpha;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::put_PenAlpha(LONG lAlpha)
{
	m_oPen.Alpha = lAlpha;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::get_PenSize(double* dSize)
{
	*dSize = m_oPen.Size;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::put_PenSize(double dSize)
{
	m_oPen.Size = dSize * 25.4 / 96.0;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::get_PenDashStyle(BYTE* val)
{
	*val = m_oPen.DashStyle;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::put_PenDashStyle(BYTE val)
{
	m_oPen.DashStyle = val;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::get_PenLineStartCap(BYTE* val)
{
	*val = m_oPen.LineStartCap;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::put_PenLineStartCap(BYTE val)
{
	m_oPen.LineStartCap = val;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::get_PenLineEndCap(BYTE* val)
{
	*val = m_oPen.LineEndCap;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::put_PenLineEndCap(BYTE val)
{
	m_oPen.LineEndCap = val;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::get_PenLineJoin(BYTE* val)
{
	*val = m_oPen.LineJoin;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::put_PenLineJoin(BYTE val)
{
	m_oPen.LineJoin = val;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::get_PenDashOffset(double* val)
{
	*val = m_oPen.DashOffset;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::put_PenDashOffset(double val)
{
	m_oPen.DashOffset = val;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::get_PenAlign(LONG* val)
{
	*val = m_oPen.Align;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::put_PenAlign(LONG val)
{
	m_oPen.Align = val;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::get_PenMiterLimit(double* val)
{
	*val = m_oPen.MiterLimit;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::put_PenMiterLimit(double val)
{
	m_oPen.MiterLimit = val;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::PenDashPattern(SAFEARRAY* pPattern)
{
	if (NULL != pPattern)
	{
		m_oPen.SetDashPattern((double*)pPattern->pvData, pPattern->rgsabound[0].cElements);
	}

	return S_OK;
}
// brush ------------------------------------------------------------------------------------
STDMETHODIMP NSPresentationEditor::CSVGConverter::SetBrush(BSTR bsXML)
{
	//m_oBrush.FromXmlString((CString)bsXML);
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::get_BrushType(LONG* lType)
{
	*lType = m_oBrush.Type;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::put_BrushType(LONG lType)
{
	m_oBrush.Type = lType;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::get_BrushColor1(LONG* lColor)
{
	*lColor = m_oBrush.Color1.GetLONG();
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::put_BrushColor1(LONG lColor)
{
	BYTE lScheme = ((DWORD)(lColor)) >> 24;

	if (0xFF != lScheme)
		m_oBrush.Color1.SetBGR(lColor);
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::get_BrushAlpha1(LONG* lAlpha)
{
	*lAlpha = m_oBrush.Alpha1;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::put_BrushAlpha1(LONG lAlpha)
{
	m_oBrush.Alpha1 = lAlpha;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::get_BrushColor2(LONG* lColor)
{
	*lColor = m_oBrush.Color2.GetLONG();
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::put_BrushColor2(LONG lColor)
{
	m_oBrush.Color2.SetBGR(lColor);
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::get_BrushAlpha2(LONG* lAlpha)
{
	*lAlpha = m_oBrush.Alpha2;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::put_BrushAlpha2(LONG lAlpha)
{
	m_oBrush.Alpha2 = lAlpha;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::get_BrushTexturePath(BSTR* bsPath)
{
	*bsPath = m_oBrush.TexturePath.AllocSysString();
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::put_BrushTexturePath(BSTR bsPath)
{
	m_oBrush.TexturePath = (CString)bsPath;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::get_BrushTextureMode(LONG* lMode)
{
	*lMode = m_oBrush.TextureMode;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::put_BrushTextureMode(LONG lMode)
{
	m_oBrush.TextureMode = lMode;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::get_BrushTextureAlpha(LONG* lTxAlpha)
{
	*lTxAlpha = m_oBrush.TextureAlpha;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::put_BrushTextureAlpha(LONG lTxAlpha)
{
	m_oBrush.TextureAlpha = lTxAlpha;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::get_BrushLinearAngle(double* dAngle)
{
	*dAngle = m_oBrush.LinearAngle;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::put_BrushLinearAngle(double dAngle)
{
	m_oBrush.LinearAngle = dAngle;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::BrushRect(BOOL val, double left, double top, double width, double height)
{
	m_oBrush.Rectable = val;
	m_oBrush.Rect.X = (float)left;
	m_oBrush.Rect.Y = (float)top;
	m_oBrush.Rect.Width  = (float)width;
	m_oBrush.Rect.Height = (float)height;	

	return S_OK;
}
// font -------------------------------------------------------------------------------------
STDMETHODIMP NSPresentationEditor::CSVGConverter::SetFont(BSTR bsXML)
{
	//m_oFont.FromXmlString((CString)bsXML);
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::get_FontName(BSTR* bsName)
{
	*bsName = m_oFont.Name.AllocSysString();
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::put_FontName(BSTR bsName)
{
	m_oFont.Name = (CString)bsName;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::get_FontPath(BSTR* bsName)
{
	*bsName = m_oFont.Path.AllocSysString();
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::put_FontPath(BSTR bsName)
{
	m_oFont.Path = bsName;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::get_FontSize(double* dSize)
{
	*dSize = m_oFont.Size;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::put_FontSize(double dSize)
{
	m_oFont.Size = dSize;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::get_FontStyle(LONG* lStyle)
{
	*lStyle = m_oFont.GetStyle();
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::put_FontStyle(LONG lStyle)
{
	m_oFont.SetStyle(lStyle);
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::get_FontStringGID(BOOL* bGID)
{
	*bGID = m_oFont.StringGID;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::put_FontStringGID(BOOL bGID)
{
	m_oFont.StringGID = bGID;
	m_pFontManager->SetStringGID(bGID);
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::get_FontCharSpace(double* dSpace)
{
	*dSpace = m_oFont.CharSpace;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::put_FontCharSpace(double dSpace)
{
	m_oFont.CharSpace = dSpace;
	return S_OK;
}
// shadow -----------------------------------------------------------------------------------
STDMETHODIMP NSPresentationEditor::CSVGConverter::SetShadow(BSTR bsXML)
{
	//m_oShadow.FromXmlString((CString)bsXML);
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::get_ShadowDistanceX(double* val)
{
	*val = m_oShadow.DistanceX;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::put_ShadowDistanceX(double val)
{
	m_oShadow.DistanceX = val;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::get_ShadowDistanceY(double* val)
{
	*val = m_oShadow.DistanceY;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::put_ShadowDistanceY(double val)
{
	m_oShadow.DistanceY = val;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::get_ShadowBlurSize(double* val)
{
	*val = m_oShadow.BlurSize;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::put_ShadowBlurSize(double val)
{
	m_oShadow.BlurSize = val;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::get_ShadowColor(LONG* val)
{
	*val = m_oShadow.Color.GetLONG();
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::put_ShadowColor(LONG val)
{
	m_oShadow.Color.SetBGR(val);
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::get_ShadowAlpha(LONG* val)
{
	*val = m_oShadow.Alpha;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::put_ShadowAlpha(LONG val)
{
	m_oShadow.Alpha = val;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::get_ShadowVisible(BOOL* val)
{
	*val = m_oShadow.Visible;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::put_ShadowVisible(BOOL val)
{
	m_oShadow.Visible = val;
	return S_OK;
}
// edge -------------------------------------------------------------------------------------
STDMETHODIMP NSPresentationEditor::CSVGConverter::SetEdgeText(BSTR bsXML)
{
	//m_oEdge.FromXmlString((CString)bsXML);
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::get_EdgeVisible(LONG* val)
{
	*val = m_oEdge.Visible;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::put_EdgeVisible(LONG val)
{
	m_oEdge.Visible = val;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::get_EdgeColor(LONG* val)
{
	*val = m_oEdge.Color.GetLONG();
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::put_EdgeColor(LONG val)
{
	m_oEdge.Color.SetBGR(val);
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::get_EdgeAlpha(LONG* val)
{
	*val = m_oEdge.Alpha;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::put_EdgeAlpha(LONG val)
{
	m_oEdge.Alpha = val;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::get_EdgeDist(double* val)
{
	*val = m_oEdge.Dist;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::put_EdgeDist(double val)
{
	m_oEdge.Dist = val;
	return S_OK;
}
//-------- Функции для вывода текста --------------------------------------------------------
STDMETHODIMP NSPresentationEditor::CSVGConverter::CommandDrawText(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
{
	if (c_nHyperlinkType == m_lCurrentCommandType)
		return S_OK;

	if (abs(m_dTransformAngle) > 1)
	{
		PathCommandEnd();
		BeginCommand(c_nPathType);
		PathCommandText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);
		DrawPath(c_nWindingFillMode);
		EndCommand(c_nPathType);
		PathCommandEnd();
		return S_OK;
	}

	double x = fX;
	double y = fY;
	m_oFullTransform.TransformPoint(x, y);

	double r = fX + fWidth;
	double b = fY + fHeight;
	m_oFullTransform.TransformPoint(r, b);

	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::CommandDrawTextEx(BSTR bsText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
{
	if (abs(m_dTransformAngle) > 1)
	{
		PathCommandEnd();
		BeginCommand(c_nPathType);
		PathCommandTextEx(bsText, bsGidText, bsSourceCodeText, fX, fY, fWidth, fHeight, fBaseLineOffset, lFlags);
		DrawPath(c_nWindingFillMode);
		EndCommand(c_nPathType);
		PathCommandEnd();
		return S_OK;
	}

	double x = fX;
	double y = fY;
	m_oFullTransform.TransformPoint(x, y);

	double r = fX + fWidth;
	double b = fY + fHeight;
	m_oFullTransform.TransformPoint(r, b);

	return S_OK;
}
//-------- Маркеры для команд ---------------------------------------------------------------
STDMETHODIMP NSPresentationEditor::CSVGConverter::BeginCommand(DWORD lType)
{
	if (c_nPathType == lType)
	{
		m_pSimpleGraphicsConverter->PathCommandEnd();
	}

	m_lCurrentCommandType = lType;

	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::EndCommand(DWORD lType)
{
	m_lCurrentCommandType = -1;
	return S_OK;
}
//-------- Функции для работы с Graphics Path -----------------------------------------------
STDMETHODIMP NSPresentationEditor::CSVGConverter::PathCommandMoveTo(double fX, double fY)
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
STDMETHODIMP NSPresentationEditor::CSVGConverter::PathCommandLineTo(double fX, double fY)
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
STDMETHODIMP NSPresentationEditor::CSVGConverter::PathCommandLinesTo(SAFEARRAY* pPoints)
{
	m_pSimpleGraphicsConverter->PathCommandLinesTo(pPoints);
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::PathCommandCurveTo(double fX1, double fY1, double fX2, double fY2, double fX3, double fY3)
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
STDMETHODIMP NSPresentationEditor::CSVGConverter::PathCommandCurvesTo(SAFEARRAY* pPoints)
{
	m_pSimpleGraphicsConverter->PathCommandCurvesTo(pPoints);
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::PathCommandArcTo(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle)
{
	m_pSimpleGraphicsConverter->PathCommandArcTo(fX, fY, fWidth, fHeight, fStartAngle, fSweepAngle);
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::PathCommandClose()
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
STDMETHODIMP NSPresentationEditor::CSVGConverter::PathCommandEnd()
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
STDMETHODIMP NSPresentationEditor::CSVGConverter::DrawPath(long nType)
{
	bool bIsStroke	= ((0x01 == (nType & 0x01)) && (0 != m_oPen.Alpha));
	bool bIsFill	= ((0xFF < nType) && (0 != m_oBrush.Alpha1));

	m_oSvgPen		= m_oPen;
	m_oSvgBrush		= m_oBrush;

	int lIndexPath = m_oSvgPath.m_arParts.GetSize() - 1;
	if (lIndexPath < 0)
		return S_OK;

	m_oSvgPath.m_arParts[lIndexPath].m_bFill	= bIsFill;
	m_oSvgPath.m_arParts[lIndexPath].m_bStroke	= bIsStroke;

	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::PathCommandStart()
{
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
STDMETHODIMP NSPresentationEditor::CSVGConverter::PathCommandGetCurrentPoint(double* fX, double* fY)
{
	m_pSimpleGraphicsConverter->PathCommandGetCurrentPoint(fX, fY);	
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::PathCommandText(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
{
	_SetFont();
	m_pSimpleGraphicsConverter->PathCommandText(bsText, m_pFontManager, fX, fY, fWidth, fHeight, fBaseLineOffset);
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::PathCommandTextEx(BSTR bsText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
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
STDMETHODIMP NSPresentationEditor::CSVGConverter::DrawImage(IUnknown* pInterface, double fX, double fY, double fWidth, double fHeight)
{
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::DrawImageFromFile(BSTR bstrVal, double fX, double fY, double fWidth, double fHeight)
{
	m_oPen.Alpha = 0;
	m_oSvgBrush.Type = c_BrushTypeTexture;
	m_oSvgBrush.TexturePath = (CString)bstrVal;
	return S_OK;
}
// transform --------------------------------------------------------------------------------
STDMETHODIMP NSPresentationEditor::CSVGConverter::GetCommandParams(double* dAngle, double* dLeft, double* dTop, double* dWidth, double* dHeight, DWORD* lFlags)
{
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::SetCommandParams(double dAngle, double dLeft, double dTop, double dWidth, double dHeight, DWORD lFlags)
{
	if ((dWidth <= 1) || (dHeight <= 1))
		lFlags = 0;

	BOOL bFlipX = (0 != (c_nParamFlipX & lFlags));
	BOOL bFlipY = (0 != (c_nParamFlipY & lFlags));
	
	double m11 = bFlipX ? -1.0 : 1.0;
	double m22 = bFlipY ? -1.0 : 1.0;
	
	CMatrix oMatrix(1, 0, 0, 1, 0, 0);

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
STDMETHODIMP NSPresentationEditor::CSVGConverter::SetTransform(double dA, double dB, double dC, double dD, double dE, double dF)
{
	CMatrix oTrans(dA, dB, dC, dD, dE, dF);
	m_oTransform = oTrans;

	CalculateFullTransform();

	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::GetTransform(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF)
{
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::ResetTransform(void)
{
	m_oTransform.Reset();
	CalculateFullTransform();
	return S_OK;
}
// -----------------------------------------------------------------------------------------
STDMETHODIMP NSPresentationEditor::CSVGConverter::get_ClipMode(LONG* plMode)
{
	*plMode = m_lClipMode;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::put_ClipMode(LONG lMode)
{
	m_lClipMode = lMode;
	return S_OK;
}
// additiaonal params ----------------------------------------------------------------------
STDMETHODIMP NSPresentationEditor::CSVGConverter::SetAdditionalParam(BSTR ParamName, VARIANT ParamValue)
{
	CString sParamName = ParamName;
	if ( _T("AttributeClassName") == sParamName && VT_BSTR == ParamValue.vt )
	{
		// скидывать не надо, так как может быть несколько путей
		//m_oBrush.Color1.m_lSchemeIndex = -1;
		//m_oPen.Color.m_lSchemeIndex = -1;

		CString strClass = (CString)ParamValue.bstrVal;
		int nLen = strClass.GetLength();
		TCHAR* pData = strClass.GetBuffer();
		TCHAR* pDataEnd = pData + nLen;

		int nIndexStroke = strClass.Find(_T("stroke"));
		int nIndexFill = strClass.Find(_T("fill"));

		if (-1 != nIndexStroke)
		{
			nIndexStroke += 6;
			int nColorScheme = 0;

			TCHAR* pDataMem = pData + nIndexStroke;
			while ((pDataMem < pDataEnd) && XmlUtils::GetDigit(*pDataMem))
			{
				nColorScheme *= 10;
				nColorScheme += (int)(*pDataMem - TCHAR('0'));
				++pDataMem;
			}

			m_oPen.Color.m_lSchemeIndex = nColorScheme - 1;
			m_bIsStrokeScheme = true;
		}

		if (-1 != nIndexFill)
		{
			nIndexFill += 4;
			int nColorScheme = 0;

			TCHAR* pDataMem = pData + nIndexFill;
			while ((pDataMem < pDataEnd) && XmlUtils::GetDigit(*pDataMem))
			{
				nColorScheme *= 10;
				nColorScheme += (int)(*pDataMem - TCHAR('0'));
				++pDataMem;
			}

			m_oBrush.Color1.m_lSchemeIndex = nColorScheme - 1;
			m_bIsFillScheme = true;
		}

	}
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::GetAdditionalParam(BSTR ParamName, VARIANT* ParamValue)
{
	return S_OK;
}
// --------------------------------------------------------------------------------------------

// file
STDMETHODIMP NSPresentationEditor::CSVGConverter::OpenFile(BSTR bsFilePath)
{
	if (NULL == m_pFontManager)
	{
		CoCreateInstance(AVSGraphics::CLSID_CAVSFontManager, NULL, CLSCTX_ALL, AVSGraphics::IID_IAVSFontManager, (void**)&m_pFontManager);

		m_pFontManager->Initialize(L"");
		m_pFontManager->SetDefaultFont(L"Arial");
	}

	m_oFont.SetDefaultParams();
	m_oInstalledFont.SetDefaultParams();

	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CSVGConverter::CloseFile()
{
	m_oSvgPath.m_arParts.RemoveAll();
	return S_OK;
}
