#pragma once
#include "AVSRenderer.h"
#include "StringWriter.h"
#include "..\Objects\Structures.h"
#include "AVSGraphicsSimpleConverter.h"
#include "..\Objects\AVSWinFonts.h"
#include "..\Objects\AVSFontManager.h"
#include "GdiplusEx.h"

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

// IAVSSVGRenderer
[ object, uuid("B6C464AA-20C6-4054-AF62-BCF0F61BE70B"), dual, pointer_default(unique) ]
__interface IAVSSVGRenderer: IAVSRenderer
{
	[id(5000)]			HRESULT CreateOfficeFile([in] BSTR bsFileName, [in] LONG lFonts);
	[id(5001)]			HRESULT CloseFile([in] LONG lSave);

	[id(6000), propget] HRESULT Data([out, retval] BSTR* pbstrSVG); 
};

// _IAVSSVGRendererEvents
[uuid("C52A6CF5-83F2-4085-8CF2-5387A0790634"), dispinterface]
__interface _IAVSSVGRendererEvents
{
};

// CAVSSVGRenderer
[ coclass, default(IAVSSVGRenderer), threading(apartment), event_source(com), vi_progid("SVGRenderer.Rend"), progid("SVGRenderer.Rend.1"), version(1.0), uuid("877EADD9-87FE-4c82-BB7A-CAC90BE1B22C") ]
class ATL_NO_VTABLE CAVSSVGRenderer : 
	public IAVSSVGRenderer
{
public:
	__event __interface _IAVSSVGRendererEvents;

public:
	DECLARE_PROTECT_FINAL_CONSTRUCT()
	
	CAVSSVGRenderer();
	~CAVSSVGRenderer()
	{
	}

public:
	// ------------------------------------ renderer --------------------------------------------
	STDMETHOD(get_Type)(LONG* lType);
	//-------- Функции для работы со страницей --------------------------------------------------
	STDMETHOD(NewPage)();
	STDMETHOD(get_Height)(double* dHeight);
	STDMETHOD(put_Height)(double dHeight);
	STDMETHOD(get_Width)(double* dWidth);
	STDMETHOD(put_Width)(double dWidth);

	STDMETHOD(get_DpiX)(double* dDpiX);
	STDMETHOD(get_DpiY)(double* dDpiY);
// pen --------------------------------------------------------------------------------------
	STDMETHOD(SetPen)(BSTR bsXML);
	STDMETHOD(get_PenColor)(LONG* lColor);
	STDMETHOD(put_PenColor)(LONG lColor);
	STDMETHOD(get_PenAlpha)(LONG* lAlpha);
	STDMETHOD(put_PenAlpha)(LONG lAlpha);
	STDMETHOD(get_PenSize)(double* dSize);
	STDMETHOD(put_PenSize)(double dSize);
	STDMETHOD(get_PenDashStyle)(BYTE* val);
	STDMETHOD(put_PenDashStyle)(BYTE val);
	STDMETHOD(get_PenLineStartCap)(BYTE* val);
	STDMETHOD(put_PenLineStartCap)(BYTE val);
	STDMETHOD(get_PenLineEndCap)(BYTE* val);
	STDMETHOD(put_PenLineEndCap)(BYTE val);
	STDMETHOD(get_PenLineJoin)(BYTE* val);
	STDMETHOD(put_PenLineJoin)(BYTE val);
	STDMETHOD(get_PenDashOffset)(double* val);
	STDMETHOD(put_PenDashOffset)(double val);
	STDMETHOD(get_PenAlign)(LONG* val);
	STDMETHOD(put_PenAlign)(LONG val);
	STDMETHOD(get_PenMiterLimit)(double* val);
	STDMETHOD(put_PenMiterLimit)(double val);
	STDMETHOD(PenDashPattern)(SAFEARRAY* pPattern);
// brush ------------------------------------------------------------------------------------
	STDMETHOD(SetBrush)(BSTR bsXML);
	STDMETHOD(get_BrushType)(LONG* lType);
	STDMETHOD(put_BrushType)(LONG lType);
	STDMETHOD(get_BrushColor1)(LONG* lColor);
	STDMETHOD(put_BrushColor1)(LONG lColor);
	STDMETHOD(get_BrushAlpha1)(LONG* lAlpha);
	STDMETHOD(put_BrushAlpha1)(LONG lAlpha);
	STDMETHOD(get_BrushColor2)(LONG* lColor);
	STDMETHOD(put_BrushColor2)(LONG lColor);
	STDMETHOD(get_BrushAlpha2)(LONG* lAlpha);
	STDMETHOD(put_BrushAlpha2)(LONG lAlpha);
	STDMETHOD(get_BrushTexturePath)(BSTR* bsPath);
	STDMETHOD(put_BrushTexturePath)(BSTR bsPath);
	STDMETHOD(get_BrushTextureMode)(LONG* lMode);
	STDMETHOD(put_BrushTextureMode)(LONG lMode);
	STDMETHOD(get_BrushTextureAlpha)(LONG* lTxAlpha);
	STDMETHOD(put_BrushTextureAlpha)(LONG lTxAlpha);
	STDMETHOD(get_BrushLinearAngle)(double* dAngle);
	STDMETHOD(put_BrushLinearAngle)(double dAngle);
	STDMETHOD(BrushRect)(BOOL val, double left, double top, double width, double height);
// font -------------------------------------------------------------------------------------
	STDMETHOD(SetFont)(BSTR bsXML);
	STDMETHOD(get_FontName)(BSTR* bsName);
	STDMETHOD(put_FontName)(BSTR bsName);
	STDMETHOD(get_FontPath)(BSTR* bsName);
	STDMETHOD(put_FontPath)(BSTR bsName);
	STDMETHOD(get_FontSize)(double* dSize);
	STDMETHOD(put_FontSize)(double dSize);
	STDMETHOD(get_FontStyle)(LONG* lStyle);
	STDMETHOD(put_FontStyle)(LONG lStyle);
	STDMETHOD(get_FontStringGID)(BOOL* bGID);
	STDMETHOD(put_FontStringGID)(BOOL bGID);
	STDMETHOD(get_FontCharSpace)(double* dSpace);
	STDMETHOD(put_FontCharSpace)(double dSpace);
// shadow -----------------------------------------------------------------------------------
	STDMETHOD(SetShadow)(BSTR bsXML);
	STDMETHOD(get_ShadowDistanceX)(double* val);
	STDMETHOD(put_ShadowDistanceX)(double val);
	STDMETHOD(get_ShadowDistanceY)(double* val);
	STDMETHOD(put_ShadowDistanceY)(double val);
	STDMETHOD(get_ShadowBlurSize)(double* val);
	STDMETHOD(put_ShadowBlurSize)(double val);
	STDMETHOD(get_ShadowColor)(LONG* val);
	STDMETHOD(put_ShadowColor)(LONG val);
	STDMETHOD(get_ShadowAlpha)(LONG* val);
	STDMETHOD(put_ShadowAlpha)(LONG val);
	STDMETHOD(get_ShadowVisible)(BOOL* val);
	STDMETHOD(put_ShadowVisible)(BOOL val);
// edge -------------------------------------------------------------------------------------
	STDMETHOD(SetEdgeText)(BSTR bsXML);
	STDMETHOD(get_EdgeVisible)(LONG* val);
	STDMETHOD(put_EdgeVisible)(LONG val);
	STDMETHOD(get_EdgeColor)(LONG* val);
	STDMETHOD(put_EdgeColor)(LONG val);
	STDMETHOD(get_EdgeAlpha)(LONG* val);
	STDMETHOD(put_EdgeAlpha)(LONG val);
	STDMETHOD(get_EdgeDist)(double* val);
	STDMETHOD(put_EdgeDist)(double val);
//-------- Функции для вывода текста --------------------------------------------------------
	STDMETHOD(CommandDrawText)(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset);
	STDMETHOD(CommandDrawTextEx)(BSTR bsText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags);
//-------- Маркеры для команд ---------------------------------------------------------------
	STDMETHOD(BeginCommand)(DWORD lType);
	STDMETHOD(EndCommand)(DWORD lType);
//-------- Функции для работы с Graphics Path -----------------------------------------------
	STDMETHOD(PathCommandMoveTo)(double fX, double fY);
	STDMETHOD(PathCommandLineTo)(double fX, double fY);
	STDMETHOD(PathCommandLinesTo)(SAFEARRAY* pPoints);
	STDMETHOD(PathCommandCurveTo)(double fX1, double fY1, double fX2, double fY2, double fX3, double fY3);
	STDMETHOD(PathCommandCurvesTo)(SAFEARRAY* pPoints);
	STDMETHOD(PathCommandArcTo)(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle);
	STDMETHOD(PathCommandClose)();
	STDMETHOD(PathCommandEnd)();
	STDMETHOD(DrawPath)(long nType);
	STDMETHOD(PathCommandStart)();
	STDMETHOD(PathCommandGetCurrentPoint)(double* fX, double* fY);
	STDMETHOD(PathCommandText)(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset);
	STDMETHOD(PathCommandTextEx)(BSTR bsText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags);
//-------- Функции для вывода изображений ---------------------------------------------------
	STDMETHOD(DrawImage)(IUnknown* pInterface, double fX, double fY, double fWidth, double fHeight);
	STDMETHOD(DrawImageFromFile)(BSTR bstrVal, double fX, double fY, double fWidth, double fHeight);
// transform --------------------------------------------------------------------------------
	STDMETHOD(GetCommandParams)(double* dAngle, double* dLeft, double* dTop, double* dWidth, double* dHeight, DWORD* lFlags);
	STDMETHOD(SetCommandParams)(double dAngle, double dLeft, double dTop, double dWidth, double dHeight, DWORD lFlags);
	STDMETHOD(SetTransform)(double dA, double dB, double dC, double dD, double dE, double dF);
	STDMETHOD(GetTransform)(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF);
	STDMETHOD(ResetTransform)(void);
// -----------------------------------------------------------------------------------------
	STDMETHOD(get_ClipMode)(LONG* plMode);
	STDMETHOD(put_ClipMode)(LONG lMode);
// additiaonal params ----------------------------------------------------------------------
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue);
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue);
// --------------------------------------------------------------------------------------------

// file
	STDMETHOD(CreateOfficeFile)(BSTR bsFilePath, LONG lFonts);
	STDMETHOD(CloseFile)(LONG lSave);
	STDMETHOD(get_Data)(BSTR* bstrVal);

public:
	HRESULT FinalConstruct();
	void FinalRelease();

private:
	
	NSTextRenderer::CStringWriter	m_oWriter;
	NSTextRenderer::CStringWriter	m_oWriterVML;
	CString							m_strDstFile;

	IAVSGraphicSimpleComverter*		m_pSimpleGraphicsConverter;		// конвертер сложных гафических путей в простые
	IAVSFontManager*				m_pFontManager;					// менеджер шрифтов

	Aggplus::CMatrix				m_oBaseTransform;	// матрица перерасчета координатных осей (здесь: миллиметры -> пикселы)
	Aggplus::CMatrix				m_oTransform;		// текущая матрица преобразований рендерера
	Aggplus::CMatrix				m_oFullTransform;	// полная матрица преобразований (m_oBaseTransform * m_oTransform)

	LONG							m_lCurrentCommandType;	// текущая команда

	double							m_dDpiX;				
	double							m_dDpiY;

	double							m_dWidth;
	double							m_dHeight;

	LONG							m_lClipMode;

	LONG							m_lColorSchemeIndexBrush1;
	LONG							m_lColorSchemeIndexPen1;

	NSStructures::CPen				m_oPen;				// настройки всей графики (скопирован ашник из AVSGraphics)
	NSStructures::CBrush			m_oBrush;
	NSStructures::CFont				m_oFont;
	NSStructures::CShadow			m_oShadow;
	NSStructures::CEdgeText			m_oEdge;

	NSStructures::CFont				m_oInstalledFont;

	IAVSWinFonts*					m_pFonts;

	CGdiPlusInit					m_oInit;			// инициализация gdi+

protected:
	void CalculateFullTransform()
	{
		m_oFullTransform	= m_oBaseTransform;
		m_oFullTransform.Multiply(&m_oTransform, Aggplus::MatrixOrderPrepend);
	}

	inline void MoveTo(const double& dX, const double& dY)
	{
		double x = dX;
		double y = dY;
		m_oFullTransform.TransformPoint(x, y);

		CString str = _T("");
		str.Format(g_string_vml_MoveTo, (int)x, (int)y);
		m_oWriterVML.WriteString(str);
	}
	inline void LineTo(const double& dX, const double& dY)
	{
		double x = dX;
		double y = dY;
		m_oFullTransform.TransformPoint(x, y);

		CString str = _T("");
		str.Format(g_string_vml_LineTo, (int)x, (int)y);
		m_oWriterVML.WriteString(str);
	}
	inline void CurveTo(const double& x1, const double& y1, const double& x2, const double& y2, const double& x3, const double& y3)
	{
		double _x1 = x1;
		double _y1 = y1;
		m_oFullTransform.TransformPoint(_x1, _y1);

		double _x2 = x2;
		double _y2 = y2;
		m_oFullTransform.TransformPoint(_x2, _y2);

		double _x3 = x3;
		double _y3 = y3;
		m_oFullTransform.TransformPoint(_x3, _y3);

		CString str = _T("");
		str.Format(g_string_vml_CurveTo, (int)_x1, (int)_y1, (int)_x2, (int)_y2, (int)_x3, (int)_y3);
		m_oWriterVML.WriteString(str);
	}
	inline void Start()
	{
	}
	void End()
	{
		m_oWriterVML.ClearNoAttack();
	}
	void Close()
	{
		m_oWriterVML.WriteString(g_bstr_vml_ClosePath);
	}

	void StartPage(double& dWidth, double& dHeight)
	{
		m_dWidth	= dWidth;
		m_dHeight	= dHeight;
	}
	void EndPage()
	{
	}

	void OnBaseMatrixUpdate()
	{
		m_oBaseTransform.Reset();

		double dScaleX = m_dDpiX / c_ag_Inch_to_MM;
		double dScaleY = m_dDpiY / c_ag_Inch_to_MM;

		m_oBaseTransform.Scale(dScaleX, dScaleY, Aggplus::MatrixOrderAppend);

		CalculateFullTransform();
	}

	void _SetFont()
	{
		double dPix = m_oFont.CharSpace * m_dDpiX / 25.4;
		
		if (m_oInstalledFont.IsEqual(&m_oFont))
		{
			if (1 < m_dWidth)
			{
				m_pFontManager->SetCharSpacing(dPix);
			}
			return;
		}

		m_pFontManager->SetStringGID(m_oFont.StringGID);
		if (1 < m_dWidth)
		{
			m_pFontManager->SetCharSpacing(dPix);
		}

		if (_T("") == m_oFont.Path)
		{
			BSTR bsName = m_oFont.Name.AllocSysString();
			m_pFontManager->LoadFontByName(bsName, (float)m_oFont.Size, m_oFont.GetStyle(), m_dDpiX, m_dDpiY);
			SysFreeString(bsName);
		}
		else
		{
			BSTR bsName = m_oFont.Path.AllocSysString();
			m_pFontManager->LoadFontFromFile(bsName, (float)m_oFont.Size, m_dDpiX, m_dDpiY);
			SysFreeString(bsName);
		}

		m_oInstalledFont = m_oFont;
	}
};