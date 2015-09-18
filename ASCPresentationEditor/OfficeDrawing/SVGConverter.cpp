
#include "SVGConverter.h"

#include "../../DesktopEditor/graphics/GraphicsPath.h"

NSPresentationEditor::CSVGConverter::CSVGConverter()
{
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
void NSPresentationEditor::CSVGConverter::_SetFont()
{
	if (NULL == m_pFontManager)
	{
		m_pFontManager = new CFontManager();
		m_pFontManager->Initialize();
	}

	m_pFontManager->SetStringGID(m_oFont.StringGID);

	if (_T("") == m_oFont.Path)
	{
		m_pFontManager->LoadFontByName(m_oFont.Name, (float)m_oFont.Size, m_oFont.GetStyle(), m_dDpiX, m_dDpiY);

    }
	else
	{
        m_pFontManager->LoadFontFromFile(m_oFont.Path, (float)m_oFont.Size, m_dDpiX, m_dDpiY, 0);
    }

	m_oInstalledFont = m_oFont;
}

NSPresentationEditor::CSVGConverter::~CSVGConverter()
{
	RELEASEOBJECT(m_pSimpleGraphicsConverter);
	RELEASEOBJECT(m_pFontManager);
}

HRESULT NSPresentationEditor::CSVGConverter::get_Type(LONG* lType)
{
	if (NULL == lType)
        return S_FALSE;

	*lType = c_nSVGConverter;
	return S_OK;
}
//-------- Функции для работы со страницей --------------------------------------------------
HRESULT NSPresentationEditor::CSVGConverter::NewPage()
{
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::get_Height(double* dHeight)
{
	if (NULL != dHeight)
		*dHeight = m_dHeight;	
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::put_Height(const double &dHeight)
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
HRESULT NSPresentationEditor::CSVGConverter::get_Width(double* dWidth)
{
	if (NULL != dWidth)
		*dWidth = m_dWidth;	
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::put_Width(const double& dWidth)
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

HRESULT NSPresentationEditor::CSVGConverter::get_DpiX(double* dDpiX)
{
	if (NULL != dDpiX)
		*dDpiX = m_dDpiX;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::get_DpiY(double* dDpiY)
{
	if (NULL != dDpiY)
		*dDpiY = m_dDpiY;
	return S_OK;
}
// pen --------------------------------------------------------------------------------------

HRESULT NSPresentationEditor::CSVGConverter::get_PenColor(LONG* lColor)
{
	*lColor = m_oPen.Color.GetLONG();
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::put_PenColor(const LONG& lColor)
{
	BYTE lScheme = (BYTE)(((DWORD)(lColor)) >> 24);

	if (0xFF != lScheme)
		m_oPen.Color.SetBGR(lColor);
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::get_PenAlpha(LONG* lAlpha)
{
	*lAlpha = m_oPen.Alpha;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::put_PenAlpha(const LONG& lAlpha)
{
	m_oPen.Alpha = lAlpha;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::get_PenSize(double* dSize)
{
	*dSize = m_oPen.Size;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::put_PenSize(const double& dSize)
{
	m_oPen.Size = dSize * 25.4 / 96.0;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::get_PenDashStyle(BYTE* val)
{
	*val = m_oPen.DashStyle;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::put_PenDashStyle(const BYTE&  val)
{
	m_oPen.DashStyle = val;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::get_PenLineStartCap(BYTE* val)
{
	*val = m_oPen.LineStartCap;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::put_PenLineStartCap(const BYTE&  val)
{
	m_oPen.LineStartCap = val;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::get_PenLineEndCap(BYTE* val)
{
	*val = m_oPen.LineEndCap;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::put_PenLineEndCap(const BYTE& val)
{
	m_oPen.LineEndCap = val;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::get_PenLineJoin(BYTE* val)
{
	*val = m_oPen.LineJoin;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::put_PenLineJoin(const BYTE& val)
{
	m_oPen.LineJoin = val;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::get_PenDashOffset(double* val)
{
	*val = m_oPen.DashOffset;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::put_PenDashOffset(const double& val)
{
	m_oPen.DashOffset = val;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::get_PenAlign(LONG* val)
{
	*val = m_oPen.Align;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::put_PenAlign(const LONG& val)
{
	m_oPen.Align = val;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::get_PenMiterLimit(double* val)
{
	*val = m_oPen.MiterLimit;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::put_PenMiterLimit(const double& val)
{
	m_oPen.MiterLimit = val;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::PenDashPattern(double* pPattern, LONG lCount)
{
	if (NULL != pPattern)
	{
		m_oPen.SetDashPattern(pPattern, lCount);
	}

	return S_OK;
}
// brush ------------------------------------------------------------------------------------
HRESULT NSPresentationEditor::CSVGConverter::get_BrushType(LONG* lType)
{
	*lType = m_oBrush.Type;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::put_BrushType(const LONG& lType)
{
	m_oBrush.Type = lType;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::get_BrushColor1(LONG* lColor)
{
	*lColor = m_oBrush.Color1.GetLONG();
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::put_BrushColor1(const LONG& lColor)
{
	BYTE lScheme = (BYTE)(((DWORD)(lColor)) >> 24);

	if (0xFF != lScheme)
		m_oBrush.Color1.SetBGR(lColor);
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::get_BrushAlpha1(LONG* lAlpha)
{
	*lAlpha = m_oBrush.Alpha1;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::put_BrushAlpha1(const LONG& lAlpha)
{
	m_oBrush.Alpha1 = lAlpha;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::get_BrushColor2(LONG* lColor)
{
	*lColor = m_oBrush.Color2.GetLONG();
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::put_BrushColor2(const LONG& lColor)
{
	m_oBrush.Color2.SetBGR(lColor);
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::get_BrushAlpha2(LONG* lAlpha)
{
	*lAlpha = m_oBrush.Alpha2;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::put_BrushAlpha2(const LONG& lAlpha)
{
	m_oBrush.Alpha2 = lAlpha;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::get_BrushTexturePath(std::wstring* bsPath)
{
    if (bsPath == NULL) return S_FALSE;
	*bsPath = m_oBrush.TexturePath;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::put_BrushTexturePath(const std::wstring& bsPath)
{
	m_oBrush.TexturePath = bsPath;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::get_BrushTextureMode(LONG* lMode)
{
	*lMode = m_oBrush.TextureMode;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::put_BrushTextureMode(const LONG& lMode)
{
	m_oBrush.TextureMode = lMode;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::get_BrushTextureAlpha(LONG* lTxAlpha)
{
	*lTxAlpha = m_oBrush.TextureAlpha;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::put_BrushTextureAlpha(const LONG& lTxAlpha)
{
	m_oBrush.TextureAlpha = lTxAlpha;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::get_BrushLinearAngle(double* dAngle)
{
	*dAngle = m_oBrush.LinearAngle;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::put_BrushLinearAngle(const double& dAngle)
{
	m_oBrush.LinearAngle = dAngle;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::BrushRect(const INT& val, const double& left, const double& top, const double& width, const double& height)
{
	m_oBrush.Rectable = val;
	m_oBrush.Rect.X = (float)left;
	m_oBrush.Rect.Y = (float)top;
	m_oBrush.Rect.Width  = (float)width;
	m_oBrush.Rect.Height = (float)height;	

	return S_OK;
}
// font -------------------------------------------------------------------------------------

HRESULT NSPresentationEditor::CSVGConverter::get_FontName(std::wstring* bsName)
{
	*bsName = m_oFont.Name;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::put_FontName(const std::wstring& bsName)
{
	m_oFont.Name = bsName;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::get_FontPath(std::wstring* bsName)
{
	*bsName = m_oFont.Path;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::put_FontPath(const std::wstring& bsName)
{
	m_oFont.Path = bsName;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::get_FontSize(double* dSize)
{
	*dSize = m_oFont.Size;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::put_FontSize(const double& dSize)
{
	m_oFont.Size = dSize;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::get_FontStyle(LONG* lStyle)
{
	*lStyle = m_oFont.GetStyle();
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::put_FontStyle(const LONG& lStyle)
{
	m_oFont.SetStyle(lStyle);
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::get_FontStringGID(INT* bGID)
{
	*bGID = m_oFont.StringGID;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::put_FontStringGID(const INT& bGID)
{
	m_oFont.StringGID = bGID;
	if (m_pFontManager)
		m_pFontManager->SetStringGID(bGID);
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::get_FontCharSpace(double* dSpace)
{
	*dSpace = m_oFont.CharSpace;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::put_FontCharSpace(const double& dSpace)
{
	m_oFont.CharSpace = dSpace;
	return S_OK;
}
// shadow -----------------------------------------------------------------------------------

HRESULT NSPresentationEditor::CSVGConverter::get_ShadowDistanceX(double* val)
{
	*val = m_oShadow.DistanceX;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::put_ShadowDistanceX(const double& val)
{
	m_oShadow.DistanceX = val;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::get_ShadowDistanceY(double* val)
{
	*val = m_oShadow.DistanceY;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::put_ShadowDistanceY(const double& val)
{
	m_oShadow.DistanceY = val;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::get_ShadowBlurSize(double* val)
{
	*val = m_oShadow.BlurSize;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::put_ShadowBlurSize(const double& val)
{
	m_oShadow.BlurSize = val;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::get_ShadowColor(LONG* val)
{
	*val = m_oShadow.Color.GetLONG();
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::put_ShadowColor(const LONG& val)
{
	m_oShadow.Color.SetBGR(val);
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::get_ShadowAlpha(LONG* val)
{
	*val = m_oShadow.Alpha;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::put_ShadowAlpha(const LONG& val)
{
	m_oShadow.Alpha = val;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::get_ShadowVisible(INT* val)
{
	*val = m_oShadow.Visible;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::put_ShadowVisible(const INT& val)
{
	m_oShadow.Visible = val;
	return S_OK;
}
// edge -------------------------------------------------------------------------------------
HRESULT NSPresentationEditor::CSVGConverter::get_EdgeVisible(LONG* val)
{
	*val = m_oEdge.Visible;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::put_EdgeVisible(const LONG& val)
{
	m_oEdge.Visible = val;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::get_EdgeColor(LONG* val)
{
	*val = m_oEdge.Color.GetLONG();
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::put_EdgeColor(const LONG& val)
{
	m_oEdge.Color.SetBGR(val);
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::get_EdgeAlpha(LONG* val)
{
	*val = m_oEdge.Alpha;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::put_EdgeAlpha(const LONG& val)
{
	m_oEdge.Alpha = val;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::get_EdgeDist(double* val)
{
	*val = m_oEdge.Dist;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::put_EdgeDist(const double& val)
{
	m_oEdge.Dist = val;
	return S_OK;
}
//-------- Функции для вывода текста --------------------------------------------------------
HRESULT NSPresentationEditor::CSVGConverter::CommandDrawText(const std::wstring& bsText, const double& fX, const double& fY, const double& fWidth, const double& fHeight)
{
	if (c_nHyperlinkType == m_lCurrentCommandType)
		return S_OK;

	if (abs(m_dTransformAngle) > 1)
	{
		PathCommandEnd();
		BeginCommand(c_nPathType);
		PathCommandText(bsText, fX, fY, fWidth, fHeight);
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
HRESULT NSPresentationEditor::CSVGConverter::CommandDrawTextEx(const std::wstring& bsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& x, const double& y, const double& w, const double& h)
{
	if (abs(m_dTransformAngle) > 1)
	{
		PathCommandEnd();
		BeginCommand(c_nPathType);
		PathCommandTextEx(bsUnicodeText,  pGids, nGidsCount, x, y, w, h);
		DrawPath(c_nWindingFillMode);
		EndCommand(c_nPathType);
		PathCommandEnd();
		return S_OK;
	}

	double x_ = x;
	double y_ = y;
	m_oFullTransform.TransformPoint(x_, y_);

	double r = x + w;
	double b = y + h;
	m_oFullTransform.TransformPoint(r, b);

	return S_OK;
}
//-------- Маркеры для команд ---------------------------------------------------------------
HRESULT NSPresentationEditor::CSVGConverter::BeginCommand(const DWORD& lType)
{
	if (c_nPathType == lType)
	{
		m_pSimpleGraphicsConverter->PathCommandEnd();
	}

	m_lCurrentCommandType = lType;

	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::EndCommand(const DWORD& lType)
{
	m_lCurrentCommandType = -1;
	return S_OK;
}
//-------- Функции для работы с Graphics Path -----------------------------------------------
HRESULT NSPresentationEditor::CSVGConverter::PathCommandMoveTo(const double& fX, const double& fY)
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
HRESULT NSPresentationEditor::CSVGConverter::PathCommandLineTo(const double& fX, const double& fY)
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
HRESULT NSPresentationEditor::CSVGConverter::PathCommandLinesTo(double* points, const int& count)
{
	m_pSimpleGraphicsConverter->PathCommandLinesTo(points, count);
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::PathCommandCurveTo(const double& fX1, const double& fY1, const double& fX2, const double& fY2, const double& fX3, const double& fY3)
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
HRESULT NSPresentationEditor::CSVGConverter::PathCommandCurvesTo(double* points, const int& count)
{
	m_pSimpleGraphicsConverter->PathCommandCurvesTo(points, count);
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::PathCommandArcTo(const double& x, const double& y, const double& w, const double& h, const double& startAngle, const double& sweepAngle)
{
	m_pSimpleGraphicsConverter->PathCommandArcTo(x, y, w, h, startAngle, sweepAngle);
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::PathCommandClose()
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
HRESULT NSPresentationEditor::CSVGConverter::PathCommandEnd()
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
HRESULT NSPresentationEditor::CSVGConverter::DrawPath(const LONG& nType)
{
	bool bIsStroke	= ((0x01 == (nType & 0x01)) && (0 != m_oPen.Alpha));
	bool bIsFill	= ((0xFF < nType) && (0 != m_oBrush.Alpha1));

	m_oSvgPen		= m_oPen;
	m_oSvgBrush		= m_oBrush;

	int lIndexPath = m_oSvgPath.m_arParts.size() - 1;
	if (lIndexPath < 0)
		return S_OK;

	m_oSvgPath.m_arParts[lIndexPath].m_bFill	= bIsFill;
	m_oSvgPath.m_arParts[lIndexPath].m_bStroke	= bIsStroke;

	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::PathCommandStart()
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
HRESULT NSPresentationEditor::CSVGConverter::PathCommandGetCurrentPoint(double* fX, double* fY)
{
	m_pSimpleGraphicsConverter->PathCommandGetCurrentPoint(fX, fY);	
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::PathCommandText(const std::wstring& bsText, const double& x, const double& y, const double& w, const double& h)
{
	_SetFont();
	m_pSimpleGraphicsConverter->PathCommandText(bsText, m_pFontManager, x, y, w, h, 0);
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::PathCommandTextEx(const std::wstring& sText, const unsigned int* pGids, const unsigned int nGidsCount, const double& x, const double& y, const double& w, const double& h)
{
    bool bGid = m_oFont.StringGID;
	//TODOOO
	//if (NULL != bsGidText)
	//{
 //       m_oFont.StringGID = true;
	//	PathCommandText(bsGidText, x, y, w, h);
	//}
	
    m_oFont.StringGID = false;
	PathCommandText(sText, x, y, w, h);

	m_oFont.StringGID = bGid;
	return S_OK;
}
//-------- Функции для вывода изображений ---------------------------------------------------
HRESULT NSPresentationEditor::CSVGConverter::DrawImage(IGrObject* pImage, const double& x, const double& y, const double& w, const double& h)
{
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::DrawImageFromFile(const std::wstring& fn, const double& x, const double& y, const double& w, const double& h, const BYTE& lAlpha)
{
	m_oPen.Alpha = 0;
	m_oSvgBrush.Type = c_BrushTypeTexture;
	m_oSvgBrush.TexturePath = std_string2string(fn);
	return S_OK;
}
// transform --------------------------------------------------------------------------------
HRESULT NSPresentationEditor::CSVGConverter::GetCommandParams(double* dAngle, double* dLeft, double* dTop, double* dWidth, double* dHeight, DWORD* lFlags)
{
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::SetCommandParams(double dAngle, double dLeft, double dTop, double dWidth, double dHeight, DWORD lFlags)
{
	if ((dWidth <= 1) || (dHeight <= 1))
		lFlags = 0;

    bool bFlipX = (0 != (c_nParamFlipX & lFlags));
    bool bFlipY = (0 != (c_nParamFlipY & lFlags));
	
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
HRESULT NSPresentationEditor::CSVGConverter::SetTransform(const double& dA, const double& dB, const double& dC, const double& dD, const double& dE, const double& dF)
{
	Aggplus::CMatrix oTrans(dA, dB, dC, dD, dE, dF);
	m_oTransform = oTrans;

	CalculateFullTransform();

	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::GetTransform(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF)
{
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::ResetTransform(void)
{
	m_oTransform.Reset();
	CalculateFullTransform();
	return S_OK;
}
// -----------------------------------------------------------------------------------------
HRESULT NSPresentationEditor::CSVGConverter::get_ClipMode(LONG* plMode)
{
	*plMode = m_lClipMode;
	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::put_ClipMode(const LONG& lMode)
{
	m_lClipMode = lMode;
	return S_OK;
}
// additiaonal params ----------------------------------------------------------------------
HRESULT NSPresentationEditor::CSVGConverter::SetAdditionalParam(BSTR ParamName, VARIANT ParamValue)
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
HRESULT NSPresentationEditor::CSVGConverter::GetAdditionalParam(BSTR ParamName, VARIANT* ParamValue)
{
	return S_OK;
}
// --------------------------------------------------------------------------------------------

// file
HRESULT NSPresentationEditor::CSVGConverter::OpenFile(BSTR bsFilePath)
{
	if (NULL == m_pFontManager)
	{
		m_pFontManager = new CFontManager();

		m_pFontManager->Initialize();
		//m_pFontManager->SetDefaultFont(L"Arial");
	}

	m_oFont.SetDefaultParams();
	m_oInstalledFont.SetDefaultParams();

	return S_OK;
}
HRESULT NSPresentationEditor::CSVGConverter::CloseFile()
{
	m_oSvgPath.m_arParts.clear();
	return S_OK;
}
