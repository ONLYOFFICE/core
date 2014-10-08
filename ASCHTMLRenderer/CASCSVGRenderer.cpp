#include "CASCSVGRenderer.h"
#include "../DesktopEditor/graphics/GraphicsPath.h"
#include "Writer/StringWriter.h"
#include "CMetafile.h"

//#include "ASCMetafile.h"
//#include "StringWriter.h"
//#include "..\Objects\Structures.h"
//#include "ASCGraphicsSimpleConverter.h"
//#include "..\Objects\ASCWinFonts.h"
//#include "..\Objects\ASCFontManager.h"
//#include "GdiplusEx.h"

namespace NSSVG
{
	inline static double FABS(double dVal)
	{
		return (dVal >= 0) ? dVal : -dVal;
	}
	inline static int round(double dVal)
	{
		return (int)(dVal + 0.5);
	}
	inline static LONG ConvertColor(LONG lBGR)
	{
		return (0x00FFFFFF & (((lBGR & 0xFF) << 16) | (lBGR & 0x0000FF00) | ((lBGR >> 16) & 0xFF)));
	} 
}

namespace NSHtmlRenderer
{
	static CString g_string_svgOpen									= _T("<svg width=\"%dpx\" height=\"%dpx\" viewBox=\"0 0 %d %d\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n");
	static _bstr_t g_bstr_svgClose									= L"</svg>";

	static CString g_string_image									= _T("<image x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" xlink:href=\"%s\" preserveAspectRatio=\"none\"/>\n");

	static _bstr_t g_bstr_graphicOpen								= L"<g>\n";
	static _bstr_t g_bstr_graphicClose								= L"</g>\n";

	static _bstr_t g_bstr_path_d									= L" d=\"";
	static _bstr_t g_bstr_path_d_end								= L"\" ";

	static _bstr_t g_bstr_vml_ClosePath								= L"Z ";
	static CString g_string_vml_MoveTo								= _T("M %d,%d ");
	static CString g_string_vml_LineTo								= _T("L %d,%d ");
	static CString g_string_vml_CurveTo								= _T("C %d,%d %d,%d %d,%d ");
	static _bstr_t g_bstr_vml_Path									= L"<path ";
	static _bstr_t g_bstr_nodeClose									= L" />\n";
	static CString g_string_vml_StyleStroke							= _T("style=\"fill:none;stroke:#%06x;stroke-width:%dpx;stroke-opacity:%.2lf\" ");
	static CString g_string_vml_StyleStroke_class					= _T("class=\"stroke%d\" style=\"fill:none;stroke-width:%dpx;stroke-opacity:%.2lf\" ");
	static CString g_string_vml_StyleFill							= _T("style=\"fill:#%06x;fill-opacity:%.2lf;fill-rule:%s;stroke:none\" ");
	static CString g_string_vml_StyleFill_class						= _T("class=\"fill%d\" style=\"fill-opacity:%.2lf;fill-rule:%s;stroke:none\" ");
	static CString g_string_vml_Style								= _T("style=\"fill:#%06x;fill-opacity:%.2lf;fill-rule:%s;stroke:#%06x;stroke-width:%dpx;stroke-opacity:%.2lf\" ");
	static CString g_string_vml_Style1								= _T("class=\"stroke%d\" style=\"fill:#%06x;fill-opacity:%.2lf;fill-rule:%s;stroke-width:%dpx;stroke-opacity:%.2lf\" ");
	static CString g_string_vml_Style2								= _T("class=\"fill%d\" style=\"fill-opacity:%.2lf;fill-rule:%s;stroke:#%06x;stroke-width:%dpx;stroke-opacity:%.2lf\" ");
	static CString g_string_vml_Style3								= _T("class=\"stroke%d fill%d\" style=\"fill-opacity:%.2lf;fill-rule:%s;stroke-width:%dpx;stroke-opacity:%.2lf\" ");

	CASCSVGRenderer::CASCSVGRenderer()
	{
		m_dDpiX = 96.0;
		m_dDpiY	= 96.0;

		m_dWidth	= -1;
		m_dHeight	= -1;

		m_lClipMode			= c_nClipRegionTypeWinding;

		m_pSimpleGraphicsConverter = new Aggplus::CGraphicsPathSimpleConverter();
		m_pSimpleGraphicsConverter->SetRenderer(this);

		m_pFontManager = NULL;
		m_pPen = new NSStructures::CPen();
		m_pBrush = new NSStructures::CBrush();
		m_pFont = new NSStructures::CFont();
		m_pInstalledFont = new NSStructures::CFont();
		m_pInstalledFont->Name = _T("");

		m_pBaseTransform = new Aggplus::CMatrix();
		m_pTransform = new Aggplus::CMatrix();
		m_pFullTransform = new Aggplus::CMatrix();

		m_pWriter = new NSStrings::CStringWriter();
		m_pWriterVML = new NSStrings::CStringWriter();
	}
	CASCSVGRenderer::~CASCSVGRenderer()
	{
		RELEASEOBJECT(m_pSimpleGraphicsConverter);
		RELEASEOBJECT(m_pFontManager);
		RELEASEOBJECT(m_pPen);
		RELEASEOBJECT(m_pBrush);
		RELEASEOBJECT(m_pFont);
		RELEASEOBJECT(m_pBaseTransform);
		RELEASEOBJECT(m_pTransform);
		RELEASEOBJECT(m_pFullTransform);
		RELEASEOBJECT(m_pWriter);
		RELEASEOBJECT(m_pWriterVML);
	}

	HRESULT CASCSVGRenderer::get_Type(LONG* lType)
	{
		if (NULL == lType)
			return S_FALSE;

		*lType = c_nSVGRendrerer;
		return S_OK;
	}
	//-------- Функции для работы со страницей --------------------------------------------------
	HRESULT CASCSVGRenderer::NewPage()
	{
		return S_OK;
	}
	HRESULT CASCSVGRenderer::get_Height(double* dHeight)
	{
		*dHeight = m_dHeight;	
		return S_OK;
	}
	HRESULT CASCSVGRenderer::put_Height(const double& dHeight)
	{
		m_dHeight = dHeight;

		if (-1 != m_dWidth)
			OnBaseMatrixUpdate();

		return S_OK;
	}
	HRESULT CASCSVGRenderer::get_Width(double* dWidth)
	{
		*dWidth = m_dWidth;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::put_Width(const double& dWidth)
	{
		m_dWidth = dWidth;

		if (-1 != m_dHeight)
			OnBaseMatrixUpdate();

		return S_OK;
	}

	HRESULT CASCSVGRenderer::get_DpiX(double* dDpiX)
	{
		if (NULL != dDpiX)
			*dDpiX = m_dDpiX;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::get_DpiY(double* dDpiY)
	{
		if (NULL != dDpiY)
			*dDpiY = m_dDpiY;
		return S_OK;
	}
	// pen --------------------------------------------------------------------------------------
	HRESULT CASCSVGRenderer::get_PenColor(LONG* lColor)
	{
		*lColor = m_pPen->Color;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::put_PenColor(const LONG& lColor)
	{
		m_pPen->Color = (lColor & 0x00FFFFFF);

		DWORD dwVal = (DWORD)lColor;
		LONG lSchemeIndex = ((BYTE)(dwVal >> 24) - 100);
		if (lSchemeIndex >= 0)
			m_lColorSchemeIndexPen1 = lSchemeIndex;
		else
			m_lColorSchemeIndexPen1 = -1;

		return S_OK;
	}
	HRESULT CASCSVGRenderer::get_PenAlpha(LONG* lAlpha)
	{
		*lAlpha = m_pPen->Alpha;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::put_PenAlpha(const LONG& lAlpha)
	{
		m_pPen->Alpha = lAlpha;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::get_PenSize(double* dSize)
	{
		*dSize = m_pPen->Size;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::put_PenSize(const double& dSize)
	{
		m_pPen->Size = dSize;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::get_PenDashStyle(BYTE* val)
	{
		*val = m_pPen->DashStyle;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::put_PenDashStyle(const BYTE& val)
	{
		m_pPen->DashStyle = val;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::get_PenLineStartCap(BYTE* val)
	{
		*val = m_pPen->LineStartCap;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::put_PenLineStartCap(const BYTE& val)
	{
		m_pPen->LineStartCap = val;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::get_PenLineEndCap(BYTE* val)
	{
		*val = m_pPen->LineEndCap;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::put_PenLineEndCap(const BYTE& val)
	{
		m_pPen->LineEndCap = val;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::get_PenLineJoin(BYTE* val)
	{
		*val = m_pPen->LineJoin;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::put_PenLineJoin(const BYTE& val)
	{
		m_pPen->LineJoin = val;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::get_PenDashOffset(double* val)
	{
		*val = m_pPen->DashOffset;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::put_PenDashOffset(const double& val)
	{
		m_pPen->DashOffset = val;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::get_PenAlign(LONG* val)
	{
		*val = m_pPen->Align;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::put_PenAlign(const LONG& val)
	{
		m_pPen->Align = val;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::get_PenMiterLimit(double* val)
	{
		*val = m_pPen->MiterLimit;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::put_PenMiterLimit(const double& val)
	{
		m_pPen->MiterLimit = val;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::PenDashPattern(double* pPattern, LONG lCount)
	{
		if (NULL != pPattern)
		{
			m_pPen->SetDashPattern(pPattern, lCount);
		}
		return S_OK;
	}
	// brush ------------------------------------------------------------------------------------
	HRESULT CASCSVGRenderer::get_BrushType(LONG* lType)
	{
		*lType = m_pBrush->Type;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::put_BrushType(const LONG& lType)
	{
		m_pBrush->Type = lType;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::get_BrushColor1(LONG* lColor)
	{
		*lColor = m_pBrush->Color1;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::put_BrushColor1(const LONG& lColor)
	{
		m_pBrush->Color1 = (lColor & 0x00FFFFFF);

		DWORD dwVal = (DWORD)lColor;
		LONG lSchemeIndex = ((BYTE)(dwVal >> 24) - 100);
		if (lSchemeIndex >= 0)
			m_lColorSchemeIndexBrush1 = lSchemeIndex;
		else
			m_lColorSchemeIndexBrush1 = -1;

		return S_OK;
	}
	HRESULT CASCSVGRenderer::get_BrushAlpha1(LONG* lAlpha)
	{
		*lAlpha = m_pBrush->Alpha1;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::put_BrushAlpha1(const LONG& lAlpha)
	{
		m_pBrush->Alpha1 = lAlpha;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::get_BrushColor2(LONG* lColor)
	{
		*lColor = m_pBrush->Color2;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::put_BrushColor2(const LONG& lColor)
	{
		m_pBrush->Color2 = lColor;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::get_BrushAlpha2(LONG* lAlpha)
	{
		*lAlpha = m_pBrush->Alpha2;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::put_BrushAlpha2(const LONG& lAlpha)
	{
		m_pBrush->Alpha2 = lAlpha;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::get_BrushTexturePath(std::wstring* bsPath)
	{
		*bsPath = m_pBrush->TexturePath;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::put_BrushTexturePath(const std::wstring& bsPath)
	{
		m_pBrush->TexturePath = bsPath;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::get_BrushTextureMode(LONG* lMode)
	{
		*lMode = m_pBrush->TextureMode;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::put_BrushTextureMode(const LONG& lMode)
	{
		m_pBrush->TextureMode = lMode;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::get_BrushTextureAlpha(LONG* lTxAlpha)
	{
		*lTxAlpha = m_pBrush->TextureAlpha;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::put_BrushTextureAlpha(const LONG& lTxAlpha)
	{
		m_pBrush->TextureAlpha = lTxAlpha;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::get_BrushLinearAngle(double* dAngle)
	{
		*dAngle = m_pBrush->LinearAngle;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::put_BrushLinearAngle(const double& dAngle)
	{
		m_pBrush->LinearAngle = dAngle;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::BrushRect(const INT& val, const double& left, const double& top, const double& width, const double& height)
	{
		m_pBrush->Rectable = val;
		m_pBrush->Rect.X = (float)left;
		m_pBrush->Rect.Y = (float)top;
		m_pBrush->Rect.Width  = (float)width;
		m_pBrush->Rect.Height = (float)height;	

		return S_OK;
	}
	HRESULT CASCSVGRenderer::BrushBounds(const double& left, const double& top, const double& width, const double& height)
	{
		//todo new command
		return S_OK;
	}

	HRESULT CASCSVGRenderer::put_BrushGradientColors(LONG* lColors, double* pPositions, LONG nCount)
	{
		//todo new command
		return S_OK;
	}
	// font -------------------------------------------------------------------------------------
	HRESULT CASCSVGRenderer::get_FontName(std::wstring* bsName)
	{
		*bsName = m_pFont->Name;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::put_FontName(const std::wstring& bsName)
	{
		m_pFont->Name = bsName;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::get_FontPath(std::wstring* bsName)
	{
		*bsName = m_pFont->Path;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::put_FontPath(const std::wstring& bsName)
	{
		m_pFont->Path = bsName;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::get_FontSize(double* dSize)
	{
		*dSize = m_pFont->Size;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::put_FontSize(const double& dSize)
	{
		m_pFont->Size = dSize;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::get_FontStyle(LONG* lStyle)
	{
		*lStyle = m_pFont->GetStyle();
		return S_OK;
	}
	HRESULT CASCSVGRenderer::put_FontStyle(const LONG& lStyle)
	{
		m_pFont->SetStyle(lStyle);
		return S_OK;
	}
	HRESULT CASCSVGRenderer::get_FontStringGID(INT* bGID)
	{
		*bGID = m_pFont->StringGID;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::put_FontStringGID(const INT& bGID)
	{
		m_pFont->StringGID = bGID;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::get_FontCharSpace(double* dSpace)
	{
		*dSpace = m_pFont->CharSpace;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::put_FontCharSpace(const double& dSpace)
	{
		m_pFont->CharSpace = dSpace;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::get_FontFaceIndex(int* lFaceIndex)
	{
		*lFaceIndex = m_pFont->FaceIndex;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::put_FontFaceIndex(const int& lFaceIndex)
	{
		m_pFont->FaceIndex = lFaceIndex;
		return S_OK;
	}
	//-------- Функции для вывода текста --------------------------------------------------------
	HRESULT CASCSVGRenderer::CommandDrawTextCHAR(const LONG& c, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset)
	{
		//todo new command
		return S_OK;
	}
	HRESULT CASCSVGRenderer::CommandDrawText(const std::wstring& bsText, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset)
	{
		if (c_nHyperlinkType == m_lCurrentCommandType)
			return S_OK;

		PathCommandStart();
		PathCommandText(bsText, x, y, w, h, baselineOffset);

		//double	dOldPenW = m_pPen->Size;
		//long	lOldPenC = m_pPen->Color;
		//long	lOldPenA = m_pPen->Alpha;

		//m_pPen->Size  = 25.4 / 96;
		//m_pPen->Color = m_pBrush->Color1;
		//m_pPen->Alpha = m_pBrush->Alpha1;

		DrawPath(0x0100);

		//m_pPen->Size  = dOldPenW;
		//m_pPen->Color = lOldPenC;
		//m_pPen->Alpha = lOldPenA;

		PathCommandEnd();

		//double x = fX;
		//double y = fY;
		//m_pFullTransform->TransformPoint(x, y);

		//double r = fX + fWidth;
		//double b = fY + fHeight;
		//m_pFullTransform->TransformPoint(r, b);

		//m_pWriter->WriteText(bsText, NULL, x, y, r - x, b - y, fBaseLineOffset);

		return S_OK;
	}
	HRESULT CASCSVGRenderer::CommandDrawTextExCHAR(const LONG& c, const LONG& gid, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset, const DWORD& lFlags)
	{
		//todo new command
		return S_OK;
	}
	HRESULT CASCSVGRenderer::CommandDrawTextEx(const std::wstring& bsUnicodeText, const std::wstring& bsGidText, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset, const DWORD& lFlags)
	{
		if (!bsGidText.empty())
		{
			m_pFont->StringGID = TRUE;
			return CommandDrawText(bsGidText, x, y, w, h, baselineOffset);
		}

		m_pFont->StringGID = FALSE;
		return CommandDrawText(bsUnicodeText, x, y, w, h, baselineOffset);
	}
	//-------- Маркеры для команд ---------------------------------------------------------------
	HRESULT CASCSVGRenderer::BeginCommand(const DWORD& lType)
	{
		if (c_nClipType == lType)
		{
			//m_pWriter->WritePathClip();
		}
		else if (c_nPathType == lType)
		{
			//m_pWriter->WriteBeginPath();
		}
		m_lCurrentCommandType = lType;

		return S_OK;
	}
	HRESULT CASCSVGRenderer::EndCommand(const DWORD& lType)
	{
		if (c_nPageType == lType)
		{
			EndPage();
		}
		else if (c_nClipType == lType)
		{
			//m_pWriter->WritePathClipEnd();
		}
		else if (c_nResetClipType == lType)
		{
			//m_pWriter->WritePathResetClip();
		}
		m_lCurrentCommandType = -1;

		return S_OK;
	}
	//-------- Функции для работы с Graphics Path -----------------------------------------------
	HRESULT CASCSVGRenderer::PathCommandMoveTo(const double& fX, const double& fY)
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
	HRESULT CASCSVGRenderer::PathCommandLineTo(const double& fX, const double& fY)
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
	HRESULT CASCSVGRenderer::PathCommandLinesTo(double* points, const int& count)
	{
		m_pSimpleGraphicsConverter->PathCommandLinesTo(points, count);
		return S_OK;
	}
	HRESULT CASCSVGRenderer::PathCommandCurveTo(const double& fX1, const double& fY1, const double& fX2, const double& fY2, const double& fX3, const double& fY3)
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
	HRESULT CASCSVGRenderer::PathCommandCurvesTo(double* points, const int& count)
	{
		m_pSimpleGraphicsConverter->PathCommandCurvesTo(points, count);
		return S_OK;
	}
	HRESULT CASCSVGRenderer::PathCommandArcTo(const double&  fX, const double&  fY, const double&  fWidth, const double&  fHeight, const double&  fStartAngle, const double&  fSweepAngle)
	{
		m_pSimpleGraphicsConverter->PathCommandArcTo(fX, fY, fWidth, fHeight, fStartAngle, fSweepAngle);
		return S_OK;
	}
	HRESULT CASCSVGRenderer::PathCommandClose()
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
	HRESULT CASCSVGRenderer::PathCommandEnd()
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
	HRESULT CASCSVGRenderer::DrawPath(const long& nType)
	{
		if (m_pWriterVML->GetCurSize() < 3)
			return S_OK;

		//m_pWriter->WriteDrawPath(nType);

		BOOL bStroke	= (0x01 == (0x01 & nType));
		BOOL bFill		= (0x01 < nType);

		if (0 == m_pPen->Alpha)
			bStroke = false;
		if ((0 == m_pBrush->Alpha1) && (c_BrushTypeSolid == m_pBrush->Type))
			bFill = false;
		else if ((0 == m_pBrush->TextureAlpha) && (c_BrushTypeTexture == m_pBrush->Type || c_BrushTypePattern == m_pBrush->Type))
			bFill = false;

		if (!bFill)
		{
			// stroke
			int nColorPen	= NSSVG::ConvertColor(m_pPen->Color);
			int nPenW		= NSSVG::round(m_pPen->Size * m_dDpiY / c_ag_Inch_to_MM);

			CString strStyle = _T("");
			if (-1 == m_lColorSchemeIndexPen1)
				strStyle.Format(g_string_vml_StyleStroke, nColorPen, nPenW, (double)m_pPen->Alpha / 255);
			else 
				strStyle.Format(g_string_vml_StyleStroke_class, m_lColorSchemeIndexPen1, nPenW, (double)m_pPen->Alpha / 255);

			m_pWriter->WriteString(g_bstr_vml_Path);
			m_pWriter->WriteString(strStyle);
			m_pWriter->WriteString(g_bstr_path_d);
			m_pWriter->Write(*m_pWriterVML);
			m_pWriter->WriteString(g_bstr_path_d_end);
			m_pWriter->WriteString(g_bstr_nodeClose);

			return S_OK;
		}
		else if (c_BrushTypeTexture == m_pBrush->Type || c_BrushTypePattern == m_pBrush->Type)
		{
			double x = 0;
			double y = 0;
			double w = 0;
			double h = 0;
			m_pSimpleGraphicsConverter->PathCommandGetBounds(x, y, w, h);

			DrawImageFromFile(m_pBrush->TexturePath, x, y, w, h, 255);
			return S_OK;
			//double x = 0;
			//double y = 0;
			//double r = 0;
			//double b = 0;

			//m_pSimpleGraphicsConverter->PathCommandGetBounds(&x, &y, &r, &b);
			//r += x;
			//b += y;

			//m_pFullTransform->TransformPoint(x, y);
			//m_pFullTransform->TransformPoint(r, b);

			//CString str = _T("");
			//str.Format(g_string_image, NSSVG::round(x), NSSVG::round(y), NSSVG::round(r - x), NSSVG::round(b - y), m_pBrush->TexturePath);

			//m_pWriter->WriteString(str);
			//return S_OK;
		}

		int nColorBrush	= NSSVG::ConvertColor(m_pBrush->Color1);
		CString strMode = _T("nonzero");
		if (nType & c_nEvenOddFillMode)
			strMode = _T("evenodd");

		if (!bStroke)
		{
			CString strStyle = _T("");
			if (-1 == m_lColorSchemeIndexBrush1)
				strStyle.Format(g_string_vml_StyleFill, nColorBrush, (double)m_pBrush->Alpha1 / 255, strMode);
			else
				strStyle.Format(g_string_vml_StyleFill_class, m_lColorSchemeIndexBrush1, (double)m_pBrush->Alpha1 / 255, strMode);

			m_pWriter->WriteString(g_bstr_vml_Path);
			m_pWriter->WriteString(strStyle);
			m_pWriter->WriteString(g_bstr_path_d);
			m_pWriter->Write(*m_pWriterVML);
			m_pWriter->WriteString(g_bstr_path_d_end);
			m_pWriter->WriteString(g_bstr_nodeClose);
			return S_OK;
		}

		int nPenColor = NSSVG::ConvertColor(m_pPen->Color);
		int nPenW		= NSSVG::round(m_pPen->Size * m_dDpiY / c_ag_Inch_to_MM);
		CString strStyle = _T("");
		if (-1 != m_lColorSchemeIndexPen1 && -1 != m_lColorSchemeIndexBrush1)
			strStyle.Format(g_string_vml_Style3, m_lColorSchemeIndexPen1, m_lColorSchemeIndexBrush1, (double)m_pBrush->Alpha1 / 255, strMode, nPenW, (double)m_pPen->Alpha / 255);
		else if (-1 != m_lColorSchemeIndexPen1)
			strStyle.Format(g_string_vml_Style1, m_lColorSchemeIndexPen1, nColorBrush, (double)m_pBrush->Alpha1 / 255, strMode, nPenW, (double)m_pPen->Alpha / 255);
		else if (-1 != m_lColorSchemeIndexBrush1)
			strStyle.Format(g_string_vml_Style2, m_lColorSchemeIndexBrush1, (double)m_pBrush->Alpha1 / 255, strMode, nPenColor, nPenW, (double)m_pPen->Alpha / 255);
		else
			strStyle.Format(g_string_vml_Style, nColorBrush, (double)m_pBrush->Alpha1 / 255, strMode, nPenColor, nPenW, (double)m_pPen->Alpha / 255);

		m_pWriter->WriteString(g_bstr_vml_Path);
		m_pWriter->WriteString(strStyle);
		m_pWriter->WriteString(g_bstr_path_d);
		m_pWriter->Write(*m_pWriterVML);
		m_pWriter->WriteString(g_bstr_path_d_end);
		m_pWriter->WriteString(g_bstr_nodeClose);

		return S_OK;
	}
	HRESULT CASCSVGRenderer::PathCommandStart()
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
	HRESULT CASCSVGRenderer::PathCommandGetCurrentPoint(double* fX, double* fY)
	{
		m_pSimpleGraphicsConverter->PathCommandGetCurrentPoint(fX, fY);	
		return S_OK;
	}
	HRESULT CASCSVGRenderer::PathCommandTextCHAR(const LONG& c, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset)
	{
		//todo new command
		return S_OK;
	}
	HRESULT CASCSVGRenderer::PathCommandText(const std::wstring& bsText, const double& fX, const double& fY, const double& fWidth, const double& fHeight, const double& fBaseLineOffset)
	{
		if (NULL == m_pFontManager)
		{
			m_pFontManager = new CFontManager();
			m_pFontManager->Initialize();
		}

		_SetFont();
		m_pSimpleGraphicsConverter->PathCommandText(bsText, m_pFontManager, fX, fY, fWidth, fHeight, fBaseLineOffset);
		return S_OK;
	}
	HRESULT CASCSVGRenderer::PathCommandTextExCHAR(const LONG& c, const LONG& gid, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset, const DWORD& lFlags)
	{
		//todo new command
		return S_OK;
	}
	HRESULT CASCSVGRenderer::PathCommandTextEx(const std::wstring& bsUnicodeText, const std::wstring& bsGidText, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset, const DWORD& lFlags)
	{
		if (!bsGidText.empty())
		{
			m_pFont->StringGID = TRUE;
			return PathCommandText(bsGidText, x, y, w, h, baselineOffset);
		}

		m_pFont->StringGID = FALSE;
		return PathCommandText(bsUnicodeText, x, y, w, h, baselineOffset);
	}
	//-------- Функции для вывода изображений ---------------------------------------------------
	HRESULT CASCSVGRenderer::DrawImage(IGrObject* pImage, const double& fX, const double& fY, const double& fWidth, const double& fHeight)
	{
		double x = fX;
		double y = fY;
		m_pFullTransform->TransformPoint(x, y);

		double r	= fX + fWidth;
		double b	= fY + fHeight;
		m_pFullTransform->TransformPoint(r, b);

		//m_pWriter->WriteImage(pInterface, x, y, r - x, b - y);

		return S_OK;
	}
	HRESULT CASCSVGRenderer::DrawImageFromFile(const std::wstring& sVal, const double& fX, const double& fY, const double& fWidth, const double& fHeight, const BYTE& lAlpha = 255)
	{
		double x = fX;
		double y = fY;
		m_pFullTransform->TransformPoint(x, y);

		double r	= fX + fWidth;
		double b	= fY + fHeight;
		m_pFullTransform->TransformPoint(r, b);

		CImageExt oExt;
		LONG lFileType = oExt.GetImageType(sVal);

		if ((1 == lFileType) || (2 == lFileType))
		{
			// метафайл
			oExt.DrawOnRenderer(this, sVal, fX, fY, fWidth, fHeight, TRUE);
			return S_OK;
		}

		CString str = _T("");
		str.Format(g_string_image, NSSVG::round(x), NSSVG::round(y), NSSVG::round(r - x), NSSVG::round(b - y), sVal.c_str());

		m_pWriter->WriteString(str);
		return S_OK;
	}
	// transform --------------------------------------------------------------------------------
	HRESULT CASCSVGRenderer::SetTransform(const double& dA, const double& dB, const double& dC, const double& dD, const double& dE, const double& dF)
	{
		Aggplus::CMatrix oTrans(dA, dB, dC, dD, dE, dF);
		*m_pTransform = oTrans;

		CalculateFullTransform();

		return S_OK;
	}
	HRESULT CASCSVGRenderer::GetTransform(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF)
	{
		return S_OK;
	}
	HRESULT CASCSVGRenderer::ResetTransform(void)
	{
		m_pTransform->Reset();
		CalculateFullTransform();
		return S_OK;
	}
	// -----------------------------------------------------------------------------------------
	HRESULT CASCSVGRenderer::get_ClipMode(LONG* plMode)
	{
		*plMode = m_lClipMode;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::put_ClipMode(const LONG& lMode)
	{
		m_lClipMode = lMode;
		return S_OK;
	}
	// --------------------------------------------------------------------------------------------
	HRESULT CASCSVGRenderer::CommandLong(const LONG& lType, const LONG& lCommand)
	{
		//todo new command;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::CommandDouble(const LONG& lType, const double& dCommand)
	{
		//todo new command;
		return S_OK;
	}
	HRESULT CASCSVGRenderer::CommandString(const LONG& lType, const std::wstring& sCommand)
	{
		//todo new command;
		return S_OK;
	}
	// file
	HRESULT CASCSVGRenderer::CreateOfficeFile(const std::wstring& bsFilePath, const LONG& lFonts)
	{
		m_strDstFile = bsFilePath;
		m_pWriter->Clear();

		if (0 != lFonts)
		{
			//todo
			//if (NULL == m_pFonts)
			//{
			//	CoCreateInstance(__uuidof(CASCWinFonts), NULL, CLSCTX_ALL, __uuidof(IASCWinFonts), (void**)&m_pFonts);
			//}
			if (NULL == m_pFontManager)
			{
				m_pFontManager = new CFontManager();
				m_pFontManager->Initialize();
				//m_pFontManager->SetDefaultFont(L"Arial");
			}
		}

		double dW = m_dWidth;
		double dH = m_dHeight;
		m_pFullTransform->TransformPoint(dW, dH);

		int w = NSSVG::round(dW);
		int h = NSSVG::round(dH);

		CString strSVG = _T("");
		strSVG.Format(g_string_svgOpen, w, h, w, h);

		m_pWriter->WriteString(strSVG);
		m_pWriter->WriteString(g_bstr_graphicOpen);

		m_lColorSchemeIndexBrush1	= -1;
		m_lColorSchemeIndexPen1		= -1;

		return S_OK;
	}
	HRESULT CASCSVGRenderer::CloseFile(const LONG& lSave)
	{
		m_pWriter->WriteString(g_bstr_graphicClose);
		m_pWriter->WriteString(g_bstr_svgClose);

		if (0 == lSave)
			return S_OK;

		NSFile::CFileBinary oFile;

		if (S_OK == oFile.CreateFile(m_strDstFile))
		{
			CString strData = m_pWriter->GetCString();
			oFile.WriteFile((BYTE*)strData.GetBuffer(), sizeof(WCHAR) * strData.GetLength());
		}

		return S_OK;
	}
	HRESULT CASCSVGRenderer::get_Data(std::wstring* bsData)
	{
		*bsData = std::wstring(m_pWriter->GetCString().GetString());
		return S_OK;
	}
	void CASCSVGRenderer::CalculateFullTransform()
	{
		*m_pFullTransform	= *m_pBaseTransform;
		m_pFullTransform->Multiply(m_pTransform, Aggplus::MatrixOrderPrepend);
	}

	inline void CASCSVGRenderer::MoveTo(const double& dX, const double& dY)
	{
		double x = dX;
		double y = dY;
		m_pFullTransform->TransformPoint(x, y);

		CString str = _T("");
		str.Format(g_string_vml_MoveTo, (int)x, (int)y);
		m_pWriterVML->WriteString(str);
	}
	inline void CASCSVGRenderer::LineTo(const double& dX, const double& dY)
	{
		double x = dX;
		double y = dY;
		m_pFullTransform->TransformPoint(x, y);

		CString str = _T("");
		str.Format(g_string_vml_LineTo, (int)x, (int)y);
		m_pWriterVML->WriteString(str);
	}
	inline void CASCSVGRenderer::CurveTo(const double& x1, const double& y1, const double& x2, const double& y2, const double& x3, const double& y3)
	{
		double _x1 = x1;
		double _y1 = y1;
		m_pFullTransform->TransformPoint(_x1, _y1);

		double _x2 = x2;
		double _y2 = y2;
		m_pFullTransform->TransformPoint(_x2, _y2);

		double _x3 = x3;
		double _y3 = y3;
		m_pFullTransform->TransformPoint(_x3, _y3);

		CString str = _T("");
		str.Format(g_string_vml_CurveTo, (int)_x1, (int)_y1, (int)_x2, (int)_y2, (int)_x3, (int)_y3);
		m_pWriterVML->WriteString(str);
	}
	inline void CASCSVGRenderer::Start()
	{
	}
	void CASCSVGRenderer::End()
	{
		m_pWriterVML->ClearNoAttack();
	}
	void CASCSVGRenderer::Close()
	{
		m_pWriterVML->WriteString(g_bstr_vml_ClosePath);
	}

	void CASCSVGRenderer::StartPage(double& dWidth, double& dHeight)
	{
		m_dWidth	= dWidth;
		m_dHeight	= dHeight;
	}
	void CASCSVGRenderer::EndPage()
	{
	}

	void CASCSVGRenderer::OnBaseMatrixUpdate()
	{
		m_pBaseTransform->Reset();

		double dScaleX = m_dDpiX / c_ag_Inch_to_MM;
		double dScaleY = m_dDpiY / c_ag_Inch_to_MM;

		m_pBaseTransform->Scale(dScaleX, dScaleY, Aggplus::MatrixOrderAppend);

		CalculateFullTransform();
	}

	void CASCSVGRenderer::_SetFont()
	{
		double dPix = m_pFont->CharSpace * m_dDpiX / 25.4;

		if (m_pInstalledFont->IsEqual(m_pFont))
		{
			if (1 < m_dWidth)
			{
				m_pFontManager->SetCharSpacing(dPix);
			}
			return;
		}

		m_pFontManager->SetStringGID(m_pFont->StringGID);
		if (1 < m_dWidth)
		{
			m_pFontManager->SetCharSpacing(dPix);
		}

		if (_T("") == m_pFont->Path)
		{
			m_pFontManager->LoadFontByName(m_pFont->Name, (float)m_pFont->Size, m_pFont->GetStyle(), m_dDpiX, m_dDpiY);
		}
		else
		{
			m_pFontManager->LoadFontFromFile(m_pFont->Path, m_pFont->FaceIndex, (float)m_pFont->Size, m_dDpiX, m_dDpiY);
		}

		*m_pInstalledFont = *m_pFont;
	}
}