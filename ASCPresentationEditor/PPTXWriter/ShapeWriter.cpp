#include "ShapeWriter.h"

NSPresentationEditor::CShapeWriter::CShapeWriter()
{
	m_pRels			= NULL;
	m_lNextShapeID	= 1000;

	m_bIsWriteGeom = true;

//////////////////////////////////////////////////////////////////////
	m_dDpiX = 96.0;
	m_dDpiY	= 96.0;

	m_lClipMode = c_nClipRegionTypeWinding;

	m_pSimpleGraphicsConverter = new Aggplus::CGraphicsPathSimpleConverter();
	m_pSimpleGraphicsConverter->SetRenderer(this);

	m_dTransformAngle = 0.0;

	m_dWidth	= 100;
	m_dHeight	= 100;

	m_pFontManager = NULL;
}


HRESULT NSPresentationEditor::CShapeWriter::get_Type(LONG* lType)
{
	if (NULL == lType)
		return S_FALSE;

	*lType = c_nSVGConverter;
	return S_OK;
}
//-------- Функции для работы со страницей --------------------------------------------------
HRESULT NSPresentationEditor::CShapeWriter::NewPage()
{
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_Height(double* dHeight)
{
	if (NULL != dHeight)
		*dHeight = m_dHeight;	
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_Height(const double& dHeight)
{
	m_dHeight = dHeight;

	m_oBaseTransform.Reset();
    double dScaleX = 43200 / (std::max)(m_oBounds.GetWidth(), 0.01);
    double dScaleY = 43200 / (std::max)(m_oBounds.GetHeight(), 0.01);
	m_oBaseTransform.Translate(-m_oBounds.left, -m_oBounds.top, Aggplus::MatrixOrderAppend);
	m_oBaseTransform.Scale(dScaleX, dScaleY, Aggplus::MatrixOrderAppend);
	
	CalculateFullTransform();

	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_Width(double* dWidth)
{
	if (NULL != dWidth)
		*dWidth = m_dWidth;	
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_Width(const double& dWidth)
{
	m_dWidth = dWidth;

	m_oBaseTransform.Reset();
    double dScaleX = 43200 / (std::max)(m_oBounds.GetWidth(), 0.01);
    double dScaleY = 43200 / (std::max)(m_oBounds.GetHeight(), 0.01);
	m_oBaseTransform.Translate(-m_oBounds.left, -m_oBounds.top, Aggplus::MatrixOrderAppend);
	m_oBaseTransform.Scale(dScaleX, dScaleY, Aggplus::MatrixOrderAppend);

	CalculateFullTransform();

	return S_OK;
}

HRESULT NSPresentationEditor::CShapeWriter::get_DpiX(double* dDpiX)
{
	if (NULL != dDpiX)
		*dDpiX = m_dDpiX;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_DpiY(double* dDpiY)
{
	if (NULL != dDpiY)
		*dDpiY = m_dDpiY;
	return S_OK;
}
// pen --------------------------------------------------------------------------------------
HRESULT NSPresentationEditor::CShapeWriter::SetPen(BSTR bsXML)
{
	//m_oPen.FromXmlString((CString)bsXML);
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_PenColor(LONG* lColor)
{
	*lColor = m_oPen.Color.GetLONG();
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_PenColor(const LONG&  lColor)
{
	BYTE lScheme = ((DWORD)(lColor)) >> 24;

	if (0xFF != lScheme)
		m_oPen.Color.SetBGR(lColor);
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_PenAlpha(LONG* lAlpha)
{
	*lAlpha = m_oPen.Alpha;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_PenAlpha(const LONG& lAlpha)
{
	m_oPen.Alpha = lAlpha;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_PenSize(double* dSize)
{
	*dSize = m_oPen.Size;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_PenSize(const double& dSize)
{
	m_oPen.Size = dSize * 25.4 / 96.0;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_PenDashStyle(BYTE* val)
{
	*val = m_oPen.DashStyle;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_PenDashStyle(const BYTE& val)
{
	m_oPen.DashStyle = val;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_PenLineStartCap(BYTE* val)
{
	*val = m_oPen.LineStartCap;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_PenLineStartCap(const BYTE& val)
{
	m_oPen.LineStartCap = val;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_PenLineEndCap(BYTE* val)
{
	*val = m_oPen.LineEndCap;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_PenLineEndCap(const BYTE& val)
{
	m_oPen.LineEndCap = val;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_PenLineJoin(BYTE* val)
{
	*val = m_oPen.LineJoin;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_PenLineJoin(const BYTE& val)
{
	m_oPen.LineJoin = val;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_PenDashOffset(double* val)
{
	*val = m_oPen.DashOffset;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_PenDashOffset(const double&  val)
{
	m_oPen.DashOffset = val;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_PenAlign(LONG* val)
{
	*val = m_oPen.Align;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_PenAlign(const LONG& val)
{
	m_oPen.Align = val;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_PenMiterLimit(double* val)
{
	*val = m_oPen.MiterLimit;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_PenMiterLimit(const double& val)
{
	m_oPen.MiterLimit = val;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::PenDashPattern(double* pPattern, LONG lCount)
{
	if (NULL != pPattern)
	{
		m_oPen.SetDashPattern(pPattern, lCount);
	}

	return S_OK;
}
// brush ------------------------------------------------------------------------------------
HRESULT NSPresentationEditor::CShapeWriter::SetBrush(BSTR bsXML)
{
	//m_oBrush.FromXmlString((CString)bsXML);
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_BrushType(LONG* lType)
{
	*lType = m_oBrush.Type;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_BrushType(const LONG& lType)
{
	m_oBrush.Type = lType;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_BrushColor1(LONG* lColor)
{
	*lColor = m_oBrush.Color1.GetLONG();
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_BrushColor1(const LONG& lColor)
{
	BYTE lScheme = ((DWORD)(lColor)) >> 24;

	if (0xFF != lScheme)
		m_oBrush.Color1.SetBGR(lColor);
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_BrushAlpha1(LONG* lAlpha)
{
	*lAlpha = m_oBrush.Alpha1;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_BrushAlpha1(const LONG& lAlpha)
{
	m_oBrush.Alpha1 = lAlpha;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_BrushColor2(LONG* lColor)
{
	*lColor = m_oBrush.Color2.GetLONG();
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_BrushColor2(const LONG& lColor)
{
	m_oBrush.Color2.SetBGR(lColor);
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_BrushAlpha2(LONG* lAlpha)
{
	*lAlpha = m_oBrush.Alpha2;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_BrushAlpha2(const LONG& lAlpha)
{
	m_oBrush.Alpha2 = lAlpha;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_BrushTexturePath(std::wstring* bsPath)
{
	if (bsPath == NULL) return S_OK;
	*bsPath = string2std_string(m_oBrush.TexturePath);
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_BrushTexturePath(const std::wstring& bsPath)
{
	m_oBrush.TexturePath = std_string2string(bsPath);
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_BrushTextureMode(LONG* lMode)
{
	*lMode = m_oBrush.TextureMode;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_BrushTextureMode(const LONG& lMode)
{
	m_oBrush.TextureMode = lMode;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_BrushTextureAlpha(LONG* lTxAlpha)
{
	*lTxAlpha = m_oBrush.TextureAlpha;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_BrushTextureAlpha(const LONG& lTxAlpha)
{
	m_oBrush.TextureAlpha = lTxAlpha;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_BrushLinearAngle(double* dAngle)
{
	*dAngle = m_oBrush.LinearAngle;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_BrushLinearAngle(const double& dAngle)
{
	m_oBrush.LinearAngle = dAngle;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::BrushRect(const INT& val, const double& left, const double& top, const double& width, const double& height)
{
	m_oBrush.Rectable = val;
	m_oBrush.Rect.X = (float)left;
	m_oBrush.Rect.Y = (float)top;
	m_oBrush.Rect.Width  = (float)width;
	m_oBrush.Rect.Height = (float)height;	

	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::BrushBounds(const double& left, const double& top, const double& width, const double& height)
{
	return S_OK;
}

HRESULT NSPresentationEditor::CShapeWriter::put_BrushGradientColors(LONG* lColors, double* pPositions, LONG nCount)
{

	return S_OK;
}
// font -------------------------------------------------------------------------------------
HRESULT NSPresentationEditor::CShapeWriter::SetFont(BSTR bsXML)
{
	//m_oFont.FromXmlString((CString)bsXML);
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_FontName(std::wstring* bsName)
{
	if (bsName == NULL) return S_OK;
	*bsName = string2std_string(m_oFont.Name);
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_FontName(const std::wstring& bsName)
{
	m_oFont.Name = std_string2string(bsName);
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_FontPath(std::wstring* bsName)
{
	if (bsName == NULL) return S_OK;
	*bsName = string2std_string(m_oFont.Path);
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_FontPath(const std::wstring& bsName)
{
	m_oFont.Path = std_string2string(bsName);
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_FontSize(double* dSize)
{
	*dSize = m_oFont.Size;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_FontSize(const double& dSize)
{
	m_oFont.Size = dSize;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_FontStyle(LONG* lStyle)
{
	*lStyle = m_oFont.GetStyle();
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_FontStyle(const LONG& lStyle)
{
	m_oFont.SetStyle(lStyle);
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_FontStringGID(INT* bGID)
{
	*bGID = m_oFont.StringGID;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_FontStringGID(const INT& bGID)
{
	m_oFont.StringGID = bGID;
	m_pFontManager->SetStringGID(bGID);
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_FontCharSpace(double* dSpace)
{
	*dSpace = m_oFont.CharSpace;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_FontCharSpace(const double& dSpace)
{
	m_oFont.CharSpace = dSpace;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_FontFaceIndex(int* lFaceIndex)
{
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_FontFaceIndex(const int& lFaceIndex)
{
	return S_OK;
}
// shadow -----------------------------------------------------------------------------------
HRESULT NSPresentationEditor::CShapeWriter::SetShadow(BSTR bsXML)
{
	//m_oShadow.FromXmlString((CString)bsXML);
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_ShadowDistanceX(double* val)
{
	*val = m_oShadow.DistanceX;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_ShadowDistanceX(double val)
{
	m_oShadow.DistanceX = val;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_ShadowDistanceY(double* val)
{
	*val = m_oShadow.DistanceY;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_ShadowDistanceY(double val)
{
	m_oShadow.DistanceY = val;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_ShadowBlurSize(double* val)
{
	*val = m_oShadow.BlurSize;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_ShadowBlurSize(double val)
{
	m_oShadow.BlurSize = val;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_ShadowColor(LONG* val)
{
	*val = m_oShadow.Color.GetLONG();
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_ShadowColor(LONG val)
{
	m_oShadow.Color.SetBGR(val);
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_ShadowAlpha(LONG* val)
{
	*val = m_oShadow.Alpha;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_ShadowAlpha(LONG val)
{
	m_oShadow.Alpha = val;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_ShadowVisible(BOOL* val)
{
	*val = m_oShadow.Visible;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_ShadowVisible(BOOL val)
{
	m_oShadow.Visible = val;
	return S_OK;
}
// edge -------------------------------------------------------------------------------------
HRESULT NSPresentationEditor::CShapeWriter::SetEdgeText(BSTR bsXML)
{
	//m_oEdge.FromXmlString((CString)bsXML);
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_EdgeVisible(LONG* val)
{
	*val = m_oEdge.Visible;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_EdgeVisible(LONG val)
{
	m_oEdge.Visible = val;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_EdgeColor(LONG* val)
{
	*val = m_oEdge.Color.GetLONG();
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_EdgeColor(LONG val)
{
	m_oEdge.Color.SetBGR(val);
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_EdgeAlpha(LONG* val)
{
	*val = m_oEdge.Alpha;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_EdgeAlpha(LONG val)
{
	m_oEdge.Alpha = val;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::get_EdgeDist(double* val)
{
	*val = m_oEdge.Dist;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_EdgeDist(double val)
{
	m_oEdge.Dist = val;
	return S_OK;
}
//-------- Функции для вывода текста --------------------------------------------------------
HRESULT NSPresentationEditor::CShapeWriter::CommandDrawText(const std::wstring& bsText, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset)
{
	if (c_nHyperlinkType == m_lCurrentCommandType)
		return S_OK;

	if (TRUE)
	{
		PathCommandEnd();
		BeginCommand(c_nPathType);
		PathCommandText(bsText, x, y, w, h, baselineOffset);
		DrawPath(c_nWindingFillMode);
		EndCommand(c_nPathType);
		PathCommandEnd();
		return S_OK;
	}

	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::CommandDrawTextCHAR(const LONG& c, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset)
{

	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::CommandDrawTextExCHAR(const LONG& c, const LONG& gid, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset, const DWORD& lFlags)
{

	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::PathCommandTextCHAR(const LONG& c, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset)
{

	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::PathCommandTextExCHAR(const LONG& c, const LONG& gid, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset, const DWORD& lFlags)
{

	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::CommandLong(const LONG& lType, const LONG& lCommand)
{

	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::CommandDouble(const LONG& lType, const double& dCommand)
{

	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::CommandString(const LONG& lType, const std::wstring& sCommand)
{

	return S_OK;
}

HRESULT NSPresentationEditor::CShapeWriter::CommandDrawTextEx(const std::wstring& bsUnicodeText, const std::wstring& bsGidText, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset, const DWORD& lFlags)
{
	if (TRUE)
	{
		PathCommandEnd();
		BeginCommand(c_nPathType);
		PathCommandTextEx(bsUnicodeText, bsGidText,x, y, w, h, baselineOffset, lFlags);
		DrawPath(c_nWindingFillMode);
		EndCommand(c_nPathType);
		PathCommandEnd();
		return S_OK;
	}
	return S_OK;
}
//-------- Маркеры для команд ---------------------------------------------------------------
HRESULT NSPresentationEditor::CShapeWriter::BeginCommand(const DWORD& lType)
{
	if (c_nPathType == lType)
	{
		m_pSimpleGraphicsConverter->PathCommandEnd();
	}

	m_lCurrentCommandType = lType;

	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::EndCommand(const DWORD& lType)
{
	m_lCurrentCommandType = -1;
	return S_OK;
}
//-------- Функции для работы с Graphics Path -----------------------------------------------
HRESULT NSPresentationEditor::CShapeWriter::PathCommandMoveTo(const double& x, const double& y)
{
	if (c_nSimpleGraphicType == m_lCurrentCommandType)
	{
		MoveTo(x, y);
	}
	else
	{
		m_pSimpleGraphicsConverter->PathCommandMoveTo(x, y);
	}

	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::PathCommandLineTo(const double& x, const double& y)
{
	if (c_nSimpleGraphicType == m_lCurrentCommandType)
	{
		LineTo(x, y);
	}
	else
	{
		m_pSimpleGraphicsConverter->PathCommandLineTo(x, y);
	}

	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::PathCommandLinesTo(double* points, const int& count)
{
	m_pSimpleGraphicsConverter->PathCommandLinesTo(points, count);
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::PathCommandCurveTo(const double& x1, const double& y1, const double& x2, const double& y2, const double& x3, const double& y3)
{
	if (c_nSimpleGraphicType == m_lCurrentCommandType)
	{
		CurveTo(x1, y1, x2, y2, x3, y3);
	}
	else
	{
		m_pSimpleGraphicsConverter->PathCommandCurveTo(x1, y1, x2, y2, x3, y3);
	}

	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::PathCommandCurvesTo(double* points, const int& count)
{
	m_pSimpleGraphicsConverter->PathCommandCurvesTo(points, count);
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::PathCommandArcTo(const double& x, const double& y, const double& w, const double& h, const double& startAngle, const double& sweepAngle)
{
	m_pSimpleGraphicsConverter->PathCommandArcTo(x, y, w, h, startAngle, sweepAngle);
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::PathCommandClose()
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
HRESULT NSPresentationEditor::CShapeWriter::PathCommandEnd()
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
HRESULT NSPresentationEditor::CShapeWriter::DrawPath(const LONG& nType)
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
HRESULT NSPresentationEditor::CShapeWriter::PathCommandStart()
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
HRESULT NSPresentationEditor::CShapeWriter::PathCommandGetCurrentPoint(double* fX, double* fY)
{
	m_pSimpleGraphicsConverter->PathCommandGetCurrentPoint(fX, fY);	
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::PathCommandText(const std::wstring& bsText, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset)
{
	_SetFont();
	m_pSimpleGraphicsConverter->PathCommandText(bsText, m_pFontManager, x, y, w, h, baselineOffset);
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::PathCommandTextEx(const std::wstring& bsUnicodeText, const std::wstring& bsGidText, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset, const DWORD& lFlags)
{
	BOOL bGid = m_oFont.StringGID;
	if (NULL != bsGidText)
	{
		m_oFont.StringGID = TRUE;
		PathCommandText(bsUnicodeText, x, y, w, h, baselineOffset);
	}
	
	m_oFont.StringGID = FALSE;
	PathCommandText(bsUnicodeText, x, y, w, h, baselineOffset);

	m_oFont.StringGID = bGid;
	return S_OK;
}
//-------- Функции для вывода изображений ---------------------------------------------------
HRESULT NSPresentationEditor::CShapeWriter::DrawImage(IGrObject* pImage, const double& x, const double& y, const double& w, const double& h)
{
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::DrawImageFromFile(const std::wstring&, const double& x, const double& y, const double& w, const double& h, const BYTE& lAlpha)
{
	return S_OK;
}
// transform --------------------------------------------------------------------------------
HRESULT NSPresentationEditor::CShapeWriter::GetCommandParams(double* dAngle, double* dLeft, double* dTop, double* dWidth, double* dHeight, DWORD* lFlags)
{
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::SetCommandParams(double dAngle, double dLeft, double dTop, double dWidth, double dHeight, DWORD lFlags)
{
	if ((dWidth <= 1) || (dHeight <= 1))
		lFlags = 0;

	BOOL bFlipX = (0 != (c_nParamFlipX & lFlags));
	BOOL bFlipY = (0 != (c_nParamFlipY & lFlags));
	
	double m11 = bFlipX ? -1.0 : 1.0;
	double m22 = bFlipY ? -1.0 : 1.0;
	
	Aggplus::CMatrix oMatrix(1, 0, 0, 1, 0, 0);

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
HRESULT NSPresentationEditor::CShapeWriter::SetTransform(const double& m1, const double& m2, const double& m3, const double& m4, const double& m5, const double& m6)
{
	Aggplus::CMatrix oTrans(m1, m2, m3, m4, m5, m6);
	m_oTransform = oTrans;

	CalculateFullTransform();

	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::GetTransform(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF)
{
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::ResetTransform(void)
{
	m_oTransform.Reset();
	CalculateFullTransform();
	return S_OK;
}
// -----------------------------------------------------------------------------------------
HRESULT NSPresentationEditor::CShapeWriter::get_ClipMode(LONG* plMode)
{
	*plMode = m_lClipMode;
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::put_ClipMode(const LONG& lMode)
{
	m_lClipMode = lMode;
	return S_OK;
}
// additiaonal params ----------------------------------------------------------------------
HRESULT NSPresentationEditor::CShapeWriter::SetAdditionalParam(BSTR ParamName, VARIANT ParamValue)
{
	return S_OK;
}
HRESULT NSPresentationEditor::CShapeWriter::GetAdditionalParam(BSTR ParamName, VARIANT* ParamValue)
{
	return S_OK;
}
// --------------------------------------------------------------------------------------------
