#pragma once
#include "stdafx.h"
#include "resource.h"       // main symbols
#include <string>

#include "..\Common\ASCUtils.h"
#include "..\ASCImageStudio3\ASCGraphics\Interfaces\ASCRenderer.h"
#include "..\Common\MediaFormatDefine.h"
#include "..\Common\TemporaryCS.h"

#include "Writer\Writer3.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

// IASCHTMLRenderer
[ object, uuid("7E973D3E-34ED-45ea-90A4-775E70AEBE0B"), dual, pointer_default(unique) ]
__interface IASCHTMLRenderer2: IASCRenderer
{
	[id(5003), propget] HRESULT Mode([out, retval] LONG* plMode);
	[id(5003), propput] HRESULT Mode([in] LONG lMode);

	[id(5000)] HRESULT CreateOfficeFile([in] BSTR bsFileName);
	[id(5001)] HRESULT CloseFile();
};

// _IASCHTMLRendererEvents
[uuid("9B423367-F5BD-4bfa-8858-EC34049EC248"), dispinterface]
__interface _IASCHTMLRenderer2Events
{
};

// CASCHTMLRenderer
[ coclass, default(IASCHTMLRenderer2), threading(apartment), event_source(com), vi_progid("AVSHTMLRend.Rend2"), progid("AVSHTMLRend.Rend2.1"), version(1.0), uuid("310AD7B8-4A70-47d6-815E-A20A71B2AE59") ]
class ATL_NO_VTABLE CASCHTMLRenderer2 : 
	public IASCHTMLRenderer2,
	public NSHtmlRenderer::IBaseMatrixUpdater
{
private:

public:
	__event __interface _IASCHTMLRenderer2Events;

public:
	DECLARE_PROTECT_FINAL_CONSTRUCT()
	
	CASCHTMLRenderer2()
	{
	}
	~CASCHTMLRenderer2()
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

	STDMETHOD(get_Mode)(LONG* plMode);
	STDMETHOD(put_Mode)(LONG lMode); 
// --------------------------------------------------------------------------------------------

// file
	STDMETHOD(CreateOfficeFile)(BSTR bsFilePath);
	STDMETHOD(CloseFile)();

public:
	HRESULT FinalConstruct();
	void FinalRelease();

public:

	virtual void OnBaseMatrixUpdate(const double& dWidth, const double& dHeight)
	{
		if (m_bPageClosed)
		{
			// значит размеры пришли не в первый раз для текущей страницы
			return;
		}
		m_bPageClosed = TRUE;
		
		CalculateFullTransform();
		StartPage(dWidth, dHeight);
	}

private:
	
	NSHtmlRenderer::CDocument		m_oDocument;		// вся информация о документе (только страницы и их размеры)
	NSHtmlRenderer::CWriter3		m_oWriter;			// сам вьюер. основной класс
	NSHtmlRenderer::CRenderers*		m_pRenderers;

	CString m_strDstFile;
	LONG m_lLastSavedPage;

	Graphics::IASCGraphicSimpleComverter*	m_pSimpleGraphicsConverter;		// конвертер сложных гафических путей в простые
	Graphics::IASCFontManager*				m_pFontManager;					// менеджер шрифтов

	NSHtmlRenderer::CMatrix			m_oTransform;		// текущая матрица преобразований рендерера
	double							m_dTransformAngle;

	bool m_bIsPathTextEx;

	LONG							m_lCurrentCommandType;	// текущая команда
	LONG							m_lCurrentPage;			// текущая страница
	bool							m_bIsMetafileDrawing;
	bool							m_bIsTextGraphicType;

	LONG							m_lClipMode;

	NSStructures::CPen				m_oPen;				// настройки всей графики (скопирован ашник из AVSGraphics)
	NSStructures::CBrush			m_oBrush;
	NSStructures::CFont				m_oFont;
	NSStructures::CShadow			m_oShadow;
	NSStructures::CEdgeText			m_oEdge;

	NSStructures::CFont				m_oInstalledFont;

	BOOL							m_bPageClosed;
	BOOL							m_bPageOpened;

	Graphics::IASCWinFonts*		m_pFonts;

protected:
	void CalculateFullTransform()
	{
		m_dTransformAngle	= m_oTransform.z_Rotation();
	}

	inline void MoveTo(const double& dX, const double& dY)
	{
		double x = dX;
		double y = dY;
		m_oTransform.TransformPoint(x, y);

		m_oWriter.WritePathMoveTo(x, y);
	}
	inline void LineTo(const double& dX, const double& dY)
	{
		double x = dX;
		double y = dY;
		m_oTransform.TransformPoint(x, y);

		m_oWriter.WritePathLineTo(x, y);
	}
	inline void CurveTo(const double& x1, const double& y1, const double& x2, const double& y2, const double& x3, const double& y3)
	{
		double _x1 = x1;
		double _y1 = y1;
		m_oTransform.TransformPoint(_x1, _y1);

		double _x2 = x2;
		double _y2 = y2;
		m_oTransform.TransformPoint(_x2, _y2);

		double _x3 = x3;
		double _y3 = y3;
		m_oTransform.TransformPoint(_x3, _y3);

		m_oWriter.WritePathCurveTo(_x1, _y1, _x2, _y2, _x3, _y3);
	}
	void Start()
	{
		m_oWriter.WriteBeginPath();
	}
	void End()
	{
		m_oWriter.WriteEndPath();
	}
	void Close()
	{
		m_oWriter.WritePathClose();
	}

	void StartPage(const double& dWidth, const double& dHeight)
	{
		++m_lCurrentPage;
		//m_oThumbnail.Create(dWidth, dHeight);
		m_oWriter.NewPage(dWidth, dHeight);
	}
	void EndPage()
	{
		m_oWriter.EndPage();
		m_bPageOpened = FALSE;
	}

	void _SetFont()
	{
		if (NULL == m_pFontManager)
		{
			CoCreateInstance(__uuidof(Graphics::CASCFontManager), NULL, CLSCTX_ALL, __uuidof(Graphics::IASCFontManager), (void**)&m_pFontManager);
			m_pFontManager->Initialize(L"");
		}

		double dPix = m_oFont.CharSpace * 96 / 25.4;
		
		if (m_oInstalledFont.IsEqual(&m_oFont))
		{
			if (1 < m_oWriter.m_dWidth)
			{
				m_pFontManager->SetCharSpacing(dPix);
			}
			return;
		}

		m_pFontManager->SetStringGID(m_oFont.StringGID);
		if (1 < m_oWriter.m_dWidth)
		{
			m_pFontManager->SetCharSpacing(dPix);
		}

		if (_T("") == m_oFont.Path)
		{
			BSTR bsName = m_oFont.Name.AllocSysString();
			m_pFontManager->LoadFontByName(bsName, (float)m_oFont.Size, m_oFont.GetStyle(), 96, 96);
			SysFreeString(bsName);
		}
		else
		{
			BSTR bsName = m_oFont.Path.AllocSysString();
			m_pFontManager->LoadFontFromFile(bsName, (float)m_oFont.Size, 96, 96, 0);
			SysFreeString(bsName);
		}

		m_oInstalledFont = m_oFont;
	}
};