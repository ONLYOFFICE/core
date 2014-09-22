#pragma once
#include <atlbase.h>

// тип в DrawPath
const long c_nStroke			= 0x0001;
const long c_nWindingFillMode   = 0x0100;
const long c_nEvenOddFillMode   = 0x0200;

// тип в BeginCommand, EndCommand
const long c_nNone              = 0x0000;
const long c_nPageType			= 0x0001;
const long c_nTextType			= 0x0002;
const long c_nImageType			= 0x0003;
const long c_nPathType			= 0x0004;
const long c_nNumberingMarker	= 0x0005;
const long c_nHyperlinkType		= 0x0008;
const long c_nWatermarkType     = 0x0010;
const long c_nClipType			= 0x0020;
const long c_nResetClipType		= 0x0040;
const long c_nTextGraphicType	= 0x0080;
const long c_nStrokeTextType    = 0x0100;
const long c_nSimpleGraphicType	= 0x0200;
const long c_nParagraphType		= 0x0400;
const long c_nComplexFigureType = 0x0800;
const long c_nPDFGradientType   = 0x1000;
const long c_nTableCell			= 0x2000;

const long c_nPDFTilingFill				= 0x2001;
const long c_nPDFTilingFillIteration	= 0x2002;

const long c_nHeader			= 0xa000;
const long c_nColumnCount		= 0xa001;
const long c_nFootnote			= 0xa002;
const long c_nPageBreak			= 0xa003;

const long c_nShape				= 0xa010;
const long c_nShapeGroup		= 0xa011;
const long c_nShapeWrap			= 0xa012;

const long c_nTable				= 0xa020;
const long c_nTableXml			= 0xa021;
const long c_nTableTop			= 0xa022;

const long c_nBlock					= 0xa030;
const long c_nBlockHeadingLevel		= 0xa031;
const long c_nBlockNumberingLevel	= 0xa032;
const long c_nBlockHorzAlign		= 0xa033;

const long c_nLine				= 0xa040;
const long c_nBaselineShift		= 0xa041;

// типы клипа
const long c_nClipRegionTypeWinding		= 0x0000;
const long c_nClipRegionTypeEvenOdd		= 0x0001;
// тип объединения клипов
const long c_nClipRegionIntersect		= 0x0000;
const long c_nClipRegionUnion			= 0x0100;

// флаги в CommandDrawTextEx
const long c_nFlagNone			= 0x0000;
const long c_nFlagHyperlink		= 0x0001;

// флаги в CommandParams
const long c_nParamFlipX		= 0x0001;
const long c_nParamFlipY		= 0x0002;
const long c_nFlipNextRotate	= 0x0004;

// типы рендерера
const long c_nUnknownRenderer   = 0x0000;
const long c_nPDFWriter         = 0x0001;
const long c_nHtmlRendrerer		= 0x0002;
const long c_nSVGRendrerer		= 0x0003;
const long c_nSWFRenderer       = 0x0004;
const long c_nSVGConverter      = 0x0005;
const long c_nHtmlRendrerer2	= 0x0006;
const long c_nEbookWriter		= 0x0007;
const long c_nDocxWriter		= 0x0008;
const long c_nVifFormat			= 0x0009;


// IASCRenderer
[object, uuid("97F3816A-C2BD-43b2-B480-24711DBE7C8D"), dual, pointer_default(unique)]
__interface IASCRenderer : IDispatch
{
// тип рендерера-----------------------------------------------------------------------------
	[id(1),   propget]	HRESULT Type([out, retval] LONG* lType);
//-------- Функции для работы со страницей --------------------------------------------------
	[id(101)]			HRESULT NewPage();
	[id(102), propget]	HRESULT Height([out, retval] double* dHeight);
	[id(102), propput]	HRESULT Height([in] double dHeight);
	[id(103), propget]	HRESULT Width([out, retval] double* dWidth);
	[id(103), propput]	HRESULT Width([in] double dWidth);
	[id(104), propget]	HRESULT DpiX([out, retval] double* dDpiX);
	[id(105), propget]	HRESULT DpiY([out, retval] double* dDpiY);

// pen --------------------------------------------------------------------------------------
	[id(201)]			HRESULT SetPen([in] BSTR bsXML);
	[id(202), propget]	HRESULT PenColor([out, retval] LONG* lColor);
	[id(202), propput]	HRESULT PenColor([in] LONG lColor);
	[id(203), propget]	HRESULT PenAlpha([out, retval] LONG* lAlpha);
	[id(203), propput]	HRESULT PenAlpha([in] LONG lAlpha);
	[id(204), propget]	HRESULT PenSize([out, retval] double* dSize);
	[id(204), propput]	HRESULT PenSize([in] double dSize);
	[id(205), propget]	HRESULT PenDashStyle([out, retval] BYTE* val);
	[id(205), propput]	HRESULT PenDashStyle([in] BYTE val);
	[id(206), propget]	HRESULT PenLineStartCap([out, retval] BYTE* val);
	[id(206), propput]	HRESULT PenLineStartCap([in] BYTE val);
	[id(207), propget]	HRESULT PenLineEndCap([out, retval] BYTE* val);
	[id(207), propput]	HRESULT PenLineEndCap([in] BYTE val);
	[id(208), propget]	HRESULT PenLineJoin([out, retval] BYTE* val);
	[id(208), propput]	HRESULT PenLineJoin([in] BYTE val);
	[id(209), propget]	HRESULT PenDashOffset([out, retval] double* dOffset);
	[id(209), propput]	HRESULT PenDashOffset([in] double dOffset);
	[id(210), propget]	HRESULT PenAlign([out, retval] LONG* lAlign);
	[id(210), propput]	HRESULT PenAlign([in] LONG lAlign);
	[id(211), propget]	HRESULT PenMiterLimit([out, retval] double* dOffset);
	[id(211), propput]	HRESULT PenMiterLimit([in] double dOffset);
	[id(212)]			HRESULT PenDashPattern([in, satype("double")] SAFEARRAY* pPattern);

// brush ------------------------------------------------------------------------------------
	[id(301)]			HRESULT SetBrush([in]BSTR bsXML);
	[id(302), propget]  HRESULT BrushType([out, retval] LONG* lType);
	[id(302), propput]	HRESULT BrushType([in] LONG lType);
	[id(303), propget]  HRESULT BrushColor1([out, retval] LONG* lColor);
	[id(303), propput]	HRESULT BrushColor1([in] LONG lColor);
	[id(304), propget]  HRESULT BrushAlpha1([out, retval] LONG* lAlpha);
	[id(304), propput]	HRESULT BrushAlpha1([in] LONG lAlpha);
	[id(305), propget]  HRESULT BrushColor2([out, retval] LONG* lColor);
	[id(305), propput]	HRESULT BrushColor2([in] LONG lColor);
	[id(306), propget]  HRESULT BrushAlpha2([out, retval] LONG* lAlpha);
	[id(306), propput]	HRESULT BrushAlpha2([in] LONG lAlpha);
	[id(307), propget]  HRESULT BrushTexturePath([out, retval] BSTR* bsPath); 
	[id(307), propput]  HRESULT BrushTexturePath([in] BSTR bsPath); 
	[id(308), propget]  HRESULT BrushTextureMode([out, retval] LONG* lMode);
	[id(308), propput]	HRESULT BrushTextureMode([in] LONG lMode);
	[id(309), propget]  HRESULT BrushTextureAlpha([out, retval] LONG* lTxAlpha);
	[id(309), propput]	HRESULT BrushTextureAlpha([in] LONG lTxAlpha);
	[id(310), propget]  HRESULT BrushLinearAngle([out, retval] double* dAngle);
	[id(310), propput]	HRESULT BrushLinearAngle([in] double dAngle);
	[id(311)]			HRESULT BrushRect([in] BOOL val, [in] double left, [in] double top, [in] double width, [in] double height);

// font -------------------------------------------------------------------------------------
	[id(401)]			HRESULT SetFont([in]BSTR bsXML);
	[id(402), propget]	HRESULT FontName([out, retval] BSTR* bsName);
	[id(402), propput]	HRESULT FontName([in] BSTR bsName);
	[id(403), propget]	HRESULT FontPath([out, retval] BSTR* bsName);
	[id(403), propput]	HRESULT FontPath([in] BSTR bsName);
	[id(404), propget]  HRESULT FontSize([out, retval] double* dSize);
	[id(404), propput]	HRESULT FontSize([in] double dSize);
	[id(405), propget]  HRESULT FontStyle([out, retval] LONG* lStyle);
	[id(405), propput]	HRESULT FontStyle([in] LONG lStyle);
	[id(406), propget]  HRESULT FontStringGID([out, retval] BOOL* bGID);
	[id(406), propput]	HRESULT FontStringGID([in] BOOL bGID);
	[id(407), propget]  HRESULT FontCharSpace([out, retval] double* dSpace);
	[id(407), propput]	HRESULT FontCharSpace([in] double dSpace);

// shadow -----------------------------------------------------------------------------------
	[id(501)]			HRESULT SetShadow  ([in]BSTR bsXML);
	[id(502), propget]  HRESULT ShadowDistanceX([out, retval] double* val);
	[id(502), propput]	HRESULT ShadowDistanceX([in] double val);
	[id(503), propget]  HRESULT ShadowDistanceY([out, retval] double* val);
	[id(503), propput]	HRESULT ShadowDistanceY([in] double val);
	[id(504), propget]  HRESULT ShadowBlurSize([out, retval] double* val);
	[id(504), propput]	HRESULT ShadowBlurSize([in] double val);
	[id(506), propget]  HRESULT ShadowColor([out, retval] LONG* val);
	[id(506), propput]	HRESULT ShadowColor([in] LONG val);
	[id(507), propget]  HRESULT ShadowAlpha([out, retval] LONG* val);
	[id(507), propput]	HRESULT ShadowAlpha([in] LONG val);
	[id(508), propget]  HRESULT ShadowVisible([out, retval] BOOL* val);
	[id(508), propput]	HRESULT ShadowVisible([in] BOOL val);

// edge -------------------------------------------------------------------------------------
	[id(601)]			HRESULT SetEdgeText([in]BSTR bsXML);
	[id(602), propget]  HRESULT EdgeVisible([out, retval] LONG* val);
	[id(602), propput]	HRESULT EdgeVisible([in] LONG val);
	[id(603), propget]  HRESULT EdgeColor([out, retval] LONG* val);
	[id(603), propput]	HRESULT EdgeColor([in] LONG val);
	[id(604), propget]  HRESULT EdgeAlpha([out, retval] LONG* val);
	[id(604), propput]	HRESULT EdgeAlpha([in] LONG val);
	[id(605), propget]  HRESULT EdgeDist([out, retval] double* val);
	[id(605), propput]	HRESULT EdgeDist([in] double val);

//-------- Функции для вывода текста --------------------------------------------------------
	[id(701)]			HRESULT CommandDrawText  ([in] BSTR bsText, [in] double fX, [in] double fY, [in] double fWidth, [in]double fHeight, [in]double fBaseLineOffset );
	[id(702)]			HRESULT CommandDrawTextEx([in] BSTR bsUnicodeText, [in] BSTR bsGidText, [in] BSTR bsSourceCodeText, [in] double fX, [in] double fY, [in] double fWidth, [in]double fHeight, [in]double fBaseLineOffset, [in] DWORD lFlags);

//-------- Маркеры для команд ---------------------------------------------------------------
	[id(801)]			HRESULT BeginCommand([in] DWORD lType);
	[id(802)]			HRESULT EndCommand	([in] DWORD lType);

//-------- Функции для работы с Graphics Path -----------------------------------------------
	[id(901)]			HRESULT PathCommandMoveTo  ([in] double fX,  [in] double fY);
	[id(902)]			HRESULT PathCommandLineTo  ([in] double fX,  [in] double fY);
	[id(903)]			HRESULT PathCommandLinesTo ([in,  satype("double")] SAFEARRAY* pPoints);
	[id(904)]			HRESULT PathCommandCurveTo ([in] double fX1, [in] double fY1, [in] double fX2, [in] double fY2, [in] double fX3, [in] double fY3);
	[id(905)]			HRESULT PathCommandCurvesTo([in,  satype("double")] SAFEARRAY* pPoints);
	[id(906)]			HRESULT PathCommandArcTo   ([in] double fX,  [in] double fY, [in] double fWidth, [in] double fHeight, [in] double fStartAngle, [in] double fSweepAngle);
	[id(907)]			HRESULT PathCommandClose();
	[id(908)]			HRESULT PathCommandEnd();
	[id(909)]			HRESULT DrawPath([in]long nType);
	[id(910)]			HRESULT PathCommandStart();
	[id(911)]			HRESULT PathCommandGetCurrentPoint([out] double* fX, [out] double* fY);
	[id(912)]			HRESULT PathCommandText([in] BSTR bsText, [in] double fX, [in] double fY, [in] double fWidth, [in] double fHeight, [in] double fBaseLineOffset);
	[id(913)]			HRESULT PathCommandTextEx([in] BSTR bsUnicodeText, [in] BSTR bsGidText, [in] BSTR bsSourceCodeText, [in] double fX, [in] double fY, [in] double fWidth, [in]double fHeight, [in]double fBaseLineOffset, [in] DWORD lFlags);

//-------- Функции для вывода изображений ---------------------------------------------------
	[id(1001)]			HRESULT DrawImage([in]IUnknown* pInterface, [in] double fX, [in] double fY, [in] double fWidth, [in] double fHeight);
	[id(1002)]			HRESULT DrawImageFromFile([in] BSTR bstrVal, [in] double fX, [in] double fY, [in] double fWidth, [in] double fHeight);

// transform --------------------------------------------------------------------------------
	[id(2001)]			HRESULT GetCommandParams([out] double* dAngle, [out] double* dLeft, [out] double* dTop, [out] double* dWidth, [out] double* dHeight, [out] DWORD* lFlags );
	[id(2002)]			HRESULT SetCommandParams([in]  double dAngle,  [in]  double dLeft,  [in]  double dTop,  [in]  double dWidth,  [in]  double dHeight,  [in]  DWORD lFlags );

	[id(2003)]			HRESULT SetTransform([in] double   dA, [in] double   dB, [in] double   dC, [in] double   dD, [in] double   dE, [in] double   dF);
	[id(2004)]			HRESULT GetTransform([out]double *pdA, [out]double *pdB, [out]double *pdC, [out]double *pdD, [out]double *pdE, [out]double *pdF);
	[id(2005)]			HRESULT ResetTransform(void);

// -----------------------------------------------------------------------------------------
	[id(3001), propget] HRESULT ClipMode([out, retval] LONG* plMode);
	[id(3001), propput] HRESULT ClipMode([in] LONG lMode);

// additiaonal params ----------------------------------------------------------------------
	[id(10001)]	HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT	ParamValue);
	[id(10002)]	HRESULT GetAdditionalParam([in] BSTR ParamName, [out] VARIANT *	ParamValue);
};

// IASCRenderer2
[object, uuid("37F42FA1-A7DB-466e-ACFB-0E31F5D4126E"), dual, pointer_default(unique)]
__interface IASCRenderer2 : IDispatch
{
	[id(3010)]			HRESULT CommandLong([in] LONG lCommand);
	[id(3011)]			HRESULT CommandDouble([in] double dCommand);
	[id(3012)]			HRESULT CommandString([in] BSTR bsCommand);
	[id(3013)]			HRESULT CommandVariant([in] VARIANT vCommand);

	[id(3020)]			HRESULT CommandLong2([in] LONG lType, [in] LONG lCommand);
	[id(3021)]			HRESULT CommandDouble2([in] LONG lType, [in] double dCommand);
	[id(3022)]			HRESULT CommandString2([in] LONG lType, [in] BSTR bsCommand);
	[id(3023)]			HRESULT CommandVariant2([in] LONG lType, [in] VARIANT vCommand);
};


#define PROPERTY_RENDERER(NameBase, Name, Type)			\
	STDMETHOD(get_##NameBase##Name)(Type* pVal)			\
	{													\
		if (NULL == pVal)								\
			return S_FALSE;								\
		*pVal =	m_o##NameBase.##Name;					\
		return S_OK;									\
	}													\
	STDMETHOD(put_##NameBase##Name)(Type Val)			\
	{													\
		m_o##NameBase.##Name = Val;						\
		return S_OK;									\
	}			

// exapmle:
// PROPERTY_RENDERER(Pen, Color, LONG)
// PROPERTY_RENDERER(Pen, Alpha, LONG)