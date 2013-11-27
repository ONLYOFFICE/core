#include "stdafx.h"
#include "ShapeWriter.h"

HRESULT NSPresentationEditor::CShapeWriter::FinalConstruct()
{
	m_dDpiX = 96.0;
	m_dDpiY	= 96.0;

	m_lClipMode = c_nClipRegionTypeWinding;

	m_pSimpleGraphicsConverter = NULL;
	CoCreateInstance(ASCGraphics::CLSID_CASCGraphicSimpleComverter, NULL, CLSCTX_ALL, ASCGraphics::IID_IASCGraphicSimpleComverter, (void**)&m_pSimpleGraphicsConverter);

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

void NSPresentationEditor::CShapeWriter::FinalRelease()
{
	RELEASEINTERFACE(m_pSimpleGraphicsConverter);
}

STDMETHODIMP NSPresentationEditor::CShapeWriter::get_Type(LONG* lType)
{
	if (NULL == lType)
		return S_FALSE;

	*lType = c_nSVGConverter;
	return S_OK;
}
//-------- Функции для работы со страницей --------------------------------------------------
STDMETHODIMP NSPresentationEditor::CShapeWriter::NewPage()
{
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::get_Height(double* dHeight)
{
	if (NULL != dHeight)
		*dHeight = m_dHeight;	
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::put_Height(double dHeight)
{
	m_dHeight = dHeight;

	m_oBaseTransform.Reset();
	double dScaleX = 43200 / max(m_oBounds.GetWidth(), 0.01);
	double dScaleY = 43200 / max(m_oBounds.GetHeight(), 0.01);
	m_oBaseTransform.Translate(-m_oBounds.left, -m_oBounds.top, Aggplus::MatrixOrderAppend);
	m_oBaseTransform.Scale(dScaleX, dScaleY, Aggplus::MatrixOrderAppend);
	
	CalculateFullTransform();

	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::get_Width(double* dWidth)
{
	if (NULL != dWidth)
		*dWidth = m_dWidth;	
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::put_Width(double dWidth)
{
	m_dWidth = dWidth;

	m_oBaseTransform.Reset();
	double dScaleX = 43200 / max(m_oBounds.GetWidth(), 0.01);
	double dScaleY = 43200 / max(m_oBounds.GetHeight(), 0.01);
	m_oBaseTransform.Translate(-m_oBounds.left, -m_oBounds.top, Aggplus::MatrixOrderAppend);
	m_oBaseTransform.Scale(dScaleX, dScaleY, Aggplus::MatrixOrderAppend);

	CalculateFullTransform();

	return S_OK;
}

STDMETHODIMP NSPresentationEditor::CShapeWriter::get_DpiX(double* dDpiX)
{
	if (NULL != dDpiX)
		*dDpiX = m_dDpiX;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::get_DpiY(double* dDpiY)
{
	if (NULL != dDpiY)
		*dDpiY = m_dDpiY;
	return S_OK;
}
// pen --------------------------------------------------------------------------------------
STDMETHODIMP NSPresentationEditor::CShapeWriter::SetPen(BSTR bsXML)
{
	//m_oPen.FromXmlString((CString)bsXML);
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::get_PenColor(LONG* lColor)
{
	*lColor = m_oPen.Color.GetLONG();
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::put_PenColor(LONG lColor)
{
	BYTE lScheme = ((DWORD)(lColor)) >> 24;

	if (0xFF != lScheme)
		m_oPen.Color.SetBGR(lColor);
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::get_PenAlpha(LONG* lAlpha)
{
	*lAlpha = m_oPen.Alpha;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::put_PenAlpha(LONG lAlpha)
{
	m_oPen.Alpha = lAlpha;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::get_PenSize(double* dSize)
{
	*dSize = m_oPen.Size;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::put_PenSize(double dSize)
{
	m_oPen.Size = dSize * 25.4 / 96.0;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::get_PenDashStyle(BYTE* val)
{
	*val = m_oPen.DashStyle;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::put_PenDashStyle(BYTE val)
{
	m_oPen.DashStyle = val;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::get_PenLineStartCap(BYTE* val)
{
	*val = m_oPen.LineStartCap;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::put_PenLineStartCap(BYTE val)
{
	m_oPen.LineStartCap = val;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::get_PenLineEndCap(BYTE* val)
{
	*val = m_oPen.LineEndCap;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::put_PenLineEndCap(BYTE val)
{
	m_oPen.LineEndCap = val;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::get_PenLineJoin(BYTE* val)
{
	*val = m_oPen.LineJoin;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::put_PenLineJoin(BYTE val)
{
	m_oPen.LineJoin = val;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::get_PenDashOffset(double* val)
{
	*val = m_oPen.DashOffset;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::put_PenDashOffset(double val)
{
	m_oPen.DashOffset = val;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::get_PenAlign(LONG* val)
{
	*val = m_oPen.Align;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::put_PenAlign(LONG val)
{
	m_oPen.Align = val;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::get_PenMiterLimit(double* val)
{
	*val = m_oPen.MiterLimit;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::put_PenMiterLimit(double val)
{
	m_oPen.MiterLimit = val;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::PenDashPattern(SAFEARRAY* pPattern)
{
	if (NULL != pPattern)
	{
		m_oPen.SetDashPattern((double*)pPattern->pvData, pPattern->rgsabound[0].cElements);
	}

	return S_OK;
}
// brush ------------------------------------------------------------------------------------
STDMETHODIMP NSPresentationEditor::CShapeWriter::SetBrush(BSTR bsXML)
{
	//m_oBrush.FromXmlString((CString)bsXML);
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::get_BrushType(LONG* lType)
{
	*lType = m_oBrush.Type;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::put_BrushType(LONG lType)
{
	m_oBrush.Type = lType;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::get_BrushColor1(LONG* lColor)
{
	*lColor = m_oBrush.Color1.GetLONG();
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::put_BrushColor1(LONG lColor)
{
	BYTE lScheme = ((DWORD)(lColor)) >> 24;

	if (0xFF != lScheme)
		m_oBrush.Color1.SetBGR(lColor);
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::get_BrushAlpha1(LONG* lAlpha)
{
	*lAlpha = m_oBrush.Alpha1;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::put_BrushAlpha1(LONG lAlpha)
{
	m_oBrush.Alpha1 = lAlpha;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::get_BrushColor2(LONG* lColor)
{
	*lColor = m_oBrush.Color2.GetLONG();
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::put_BrushColor2(LONG lColor)
{
	m_oBrush.Color2.SetBGR(lColor);
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::get_BrushAlpha2(LONG* lAlpha)
{
	*lAlpha = m_oBrush.Alpha2;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::put_BrushAlpha2(LONG lAlpha)
{
	m_oBrush.Alpha2 = lAlpha;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::get_BrushTexturePath(BSTR* bsPath)
{
	*bsPath = m_oBrush.TexturePath.AllocSysString();
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::put_BrushTexturePath(BSTR bsPath)
{
	m_oBrush.TexturePath = (CString)bsPath;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::get_BrushTextureMode(LONG* lMode)
{
	*lMode = m_oBrush.TextureMode;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::put_BrushTextureMode(LONG lMode)
{
	m_oBrush.TextureMode = lMode;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::get_BrushTextureAlpha(LONG* lTxAlpha)
{
	*lTxAlpha = m_oBrush.TextureAlpha;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::put_BrushTextureAlpha(LONG lTxAlpha)
{
	m_oBrush.TextureAlpha = lTxAlpha;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::get_BrushLinearAngle(double* dAngle)
{
	*dAngle = m_oBrush.LinearAngle;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::put_BrushLinearAngle(double dAngle)
{
	m_oBrush.LinearAngle = dAngle;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::BrushRect(BOOL val, double left, double top, double width, double height)
{
	m_oBrush.Rectable = val;
	m_oBrush.Rect.X = (float)left;
	m_oBrush.Rect.Y = (float)top;
	m_oBrush.Rect.Width  = (float)width;
	m_oBrush.Rect.Height = (float)height;	

	return S_OK;
}
// font -------------------------------------------------------------------------------------
STDMETHODIMP NSPresentationEditor::CShapeWriter::SetFont(BSTR bsXML)
{
	//m_oFont.FromXmlString((CString)bsXML);
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::get_FontName(BSTR* bsName)
{
	*bsName = m_oFont.Name.AllocSysString();
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::put_FontName(BSTR bsName)
{
	m_oFont.Name = (CString)bsName;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::get_FontPath(BSTR* bsName)
{
	*bsName = m_oFont.Path.AllocSysString();
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::put_FontPath(BSTR bsName)
{
	m_oFont.Path = bsName;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::get_FontSize(double* dSize)
{
	*dSize = m_oFont.Size;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::put_FontSize(double dSize)
{
	m_oFont.Size = dSize;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::get_FontStyle(LONG* lStyle)
{
	*lStyle = m_oFont.GetStyle();
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::put_FontStyle(LONG lStyle)
{
	m_oFont.SetStyle(lStyle);
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::get_FontStringGID(BOOL* bGID)
{
	*bGID = m_oFont.StringGID;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::put_FontStringGID(BOOL bGID)
{
	m_oFont.StringGID = bGID;
	m_pFontManager->SetStringGID(bGID);
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::get_FontCharSpace(double* dSpace)
{
	*dSpace = m_oFont.CharSpace;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::put_FontCharSpace(double dSpace)
{
	m_oFont.CharSpace = dSpace;
	return S_OK;
}
// shadow -----------------------------------------------------------------------------------
STDMETHODIMP NSPresentationEditor::CShapeWriter::SetShadow(BSTR bsXML)
{
	//m_oShadow.FromXmlString((CString)bsXML);
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::get_ShadowDistanceX(double* val)
{
	*val = m_oShadow.DistanceX;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::put_ShadowDistanceX(double val)
{
	m_oShadow.DistanceX = val;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::get_ShadowDistanceY(double* val)
{
	*val = m_oShadow.DistanceY;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::put_ShadowDistanceY(double val)
{
	m_oShadow.DistanceY = val;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::get_ShadowBlurSize(double* val)
{
	*val = m_oShadow.BlurSize;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::put_ShadowBlurSize(double val)
{
	m_oShadow.BlurSize = val;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::get_ShadowColor(LONG* val)
{
	*val = m_oShadow.Color.GetLONG();
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::put_ShadowColor(LONG val)
{
	m_oShadow.Color.SetBGR(val);
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::get_ShadowAlpha(LONG* val)
{
	*val = m_oShadow.Alpha;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::put_ShadowAlpha(LONG val)
{
	m_oShadow.Alpha = val;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::get_ShadowVisible(BOOL* val)
{
	*val = m_oShadow.Visible;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::put_ShadowVisible(BOOL val)
{
	m_oShadow.Visible = val;
	return S_OK;
}
// edge -------------------------------------------------------------------------------------
STDMETHODIMP NSPresentationEditor::CShapeWriter::SetEdgeText(BSTR bsXML)
{
	//m_oEdge.FromXmlString((CString)bsXML);
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::get_EdgeVisible(LONG* val)
{
	*val = m_oEdge.Visible;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::put_EdgeVisible(LONG val)
{
	m_oEdge.Visible = val;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::get_EdgeColor(LONG* val)
{
	*val = m_oEdge.Color.GetLONG();
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::put_EdgeColor(LONG val)
{
	m_oEdge.Color.SetBGR(val);
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::get_EdgeAlpha(LONG* val)
{
	*val = m_oEdge.Alpha;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::put_EdgeAlpha(LONG val)
{
	m_oEdge.Alpha = val;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::get_EdgeDist(double* val)
{
	*val = m_oEdge.Dist;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::put_EdgeDist(double val)
{
	m_oEdge.Dist = val;
	return S_OK;
}
//-------- Функции для вывода текста --------------------------------------------------------
STDMETHODIMP NSPresentationEditor::CShapeWriter::CommandDrawText(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
{
	if (c_nHyperlinkType == m_lCurrentCommandType)
		return S_OK;

	if (TRUE)
	{
		PathCommandEnd();
		BeginCommand(c_nPathType);
		PathCommandText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);
		DrawPath(c_nWindingFillMode);
		EndCommand(c_nPathType);
		PathCommandEnd();
		return S_OK;
	}

	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::CommandDrawTextEx(BSTR bsText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
{
	if (TRUE)
	{
		PathCommandEnd();
		BeginCommand(c_nPathType);
		PathCommandTextEx(bsText, bsGidText, bsSourceCodeText, fX, fY, fWidth, fHeight, fBaseLineOffset, lFlags);
		DrawPath(c_nWindingFillMode);
		EndCommand(c_nPathType);
		PathCommandEnd();
		return S_OK;
	}
	return S_OK;
}
//-------- Маркеры для команд ---------------------------------------------------------------
STDMETHODIMP NSPresentationEditor::CShapeWriter::BeginCommand(DWORD lType)
{
	if (c_nPathType == lType)
	{
		m_pSimpleGraphicsConverter->PathCommandEnd();
	}

	m_lCurrentCommandType = lType;

	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::EndCommand(DWORD lType)
{
	m_lCurrentCommandType = -1;
	return S_OK;
}
//-------- Функции для работы с Graphics Path -----------------------------------------------
STDMETHODIMP NSPresentationEditor::CShapeWriter::PathCommandMoveTo(double fX, double fY)
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
STDMETHODIMP NSPresentationEditor::CShapeWriter::PathCommandLineTo(double fX, double fY)
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
STDMETHODIMP NSPresentationEditor::CShapeWriter::PathCommandLinesTo(SAFEARRAY* pPoints)
{
	m_pSimpleGraphicsConverter->PathCommandLinesTo(pPoints);
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::PathCommandCurveTo(double fX1, double fY1, double fX2, double fY2, double fX3, double fY3)
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
STDMETHODIMP NSPresentationEditor::CShapeWriter::PathCommandCurvesTo(SAFEARRAY* pPoints)
{
	m_pSimpleGraphicsConverter->PathCommandCurvesTo(pPoints);
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::PathCommandArcTo(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle)
{
	m_pSimpleGraphicsConverter->PathCommandArcTo(fX, fY, fWidth, fHeight, fStartAngle, fSweepAngle);
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::PathCommandClose()
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
STDMETHODIMP NSPresentationEditor::CShapeWriter::PathCommandEnd()
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
STDMETHODIMP NSPresentationEditor::CShapeWriter::DrawPath(long nType)
{
	bool bIsStroke	= ((0x01 == (nType & 0x01)) && (0 != m_oPen.Alpha));
	bool bIsFill	= ((0xFF < nType) && (0 != m_oBrush.Alpha1));

	if (bIsStroke && bIsFill)
	{
		CString str = _T("");
		str.Format(_T("<a:path w=\"43200\" h=\"43200\">"));
		m_oWriterVML.WriteString(str);
		m_oWriterVML.Write(m_oWriterPath);
		m_oWriterVML.WriteString(_T("</a:path>"));
	}
	else if (bIsStroke)
	{
		CString str = _T("");
		str.Format(_T("<a:path w=\"43200\" h=\"43200\" fill=\"none\">"));
		m_oWriterVML.WriteString(str);
		m_oWriterVML.Write(m_oWriterPath);
		m_oWriterVML.WriteString(_T("</a:path>"));
	}
	else if (bIsFill)
	{
		CString str = _T("");
		str.Format(_T("<a:path w=\"43200\" h=\"43200\" stroke=\"0\">"));
		m_oWriterVML.WriteString(str);
		m_oWriterVML.Write(m_oWriterPath);
		m_oWriterVML.WriteString(_T("</a:path>"));
	}

	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::PathCommandStart()
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
STDMETHODIMP NSPresentationEditor::CShapeWriter::PathCommandGetCurrentPoint(double* fX, double* fY)
{
	m_pSimpleGraphicsConverter->PathCommandGetCurrentPoint(fX, fY);	
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::PathCommandText(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
{
	_SetFont();
	m_pSimpleGraphicsConverter->PathCommandText(bsText, m_pFontManager, fX, fY, fWidth, fHeight, fBaseLineOffset);
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::PathCommandTextEx(BSTR bsText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
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
STDMETHODIMP NSPresentationEditor::CShapeWriter::DrawImage(IUnknown* pInterface, double fX, double fY, double fWidth, double fHeight)
{
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::DrawImageFromFile(BSTR bstrVal, double fX, double fY, double fWidth, double fHeight)
{
	return S_OK;
}
// transform --------------------------------------------------------------------------------
STDMETHODIMP NSPresentationEditor::CShapeWriter::GetCommandParams(double* dAngle, double* dLeft, double* dTop, double* dWidth, double* dHeight, DWORD* lFlags)
{
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::SetCommandParams(double dAngle, double dLeft, double dTop, double dWidth, double dHeight, DWORD lFlags)
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
STDMETHODIMP NSPresentationEditor::CShapeWriter::SetTransform(double dA, double dB, double dC, double dD, double dE, double dF)
{
	CMatrix oTrans(dA, dB, dC, dD, dE, dF);
	m_oTransform = oTrans;

	CalculateFullTransform();

	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::GetTransform(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF)
{
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::ResetTransform(void)
{
	m_oTransform.Reset();
	CalculateFullTransform();
	return S_OK;
}
// -----------------------------------------------------------------------------------------
STDMETHODIMP NSPresentationEditor::CShapeWriter::get_ClipMode(LONG* plMode)
{
	*plMode = m_lClipMode;
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::put_ClipMode(LONG lMode)
{
	m_lClipMode = lMode;
	return S_OK;
}
// additiaonal params ----------------------------------------------------------------------
STDMETHODIMP NSPresentationEditor::CShapeWriter::SetAdditionalParam(BSTR ParamName, VARIANT ParamValue)
{
	return S_OK;
}
STDMETHODIMP NSPresentationEditor::CShapeWriter::GetAdditionalParam(BSTR ParamName, VARIANT* ParamValue)
{
	return S_OK;
}
// --------------------------------------------------------------------------------------------