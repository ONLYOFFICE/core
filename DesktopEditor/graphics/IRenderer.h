#ifndef _BUILD_IRENDERER_H_
#define _BUILD_IRENDERER_H_

#include "../common/Types.h"
#include "Matrix.h"
#include <string>

class IGrObject
{
protected:
	ULONG m_lRef;

public:
	IGrObject()
	{
		m_lRef = 1;
	}

	virtual ~IGrObject()
	{
	}

	virtual ULONG AddRef() 
	{ 
		++m_lRef;
		return m_lRef;
	}
    virtual ULONG Release()
	{
		ULONG ret = --m_lRef;
		if (0 == m_lRef)
			delete this;
		return ret;
	}
};

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
const long c_nGrRenderer		= 0x0010;


// IRenderer
class IRenderer : public IGrObject
{
public:
// тип рендерера-----------------------------------------------------------------------------
	virtual HRESULT get_Type(LONG* lType)				= 0;
//-------- Функции для работы со страницей --------------------------------------------------
	virtual HRESULT NewPage()							= 0;
	virtual HRESULT get_Height(double* dHeight)			= 0;
	virtual HRESULT put_Height(const double& dHeight)	= 0;
	virtual HRESULT get_Width(double* dWidth)			= 0;
	virtual HRESULT put_Width(const double& dWidth)		= 0;
	virtual HRESULT get_DpiX(double* dDpiX)				= 0;
	virtual HRESULT get_DpiY(double* dDpiY)				= 0;

// pen --------------------------------------------------------------------------------------
	virtual HRESULT get_PenColor(LONG* lColor)					= 0;
	virtual HRESULT put_PenColor(const LONG& lColor)			= 0;
	virtual HRESULT get_PenAlpha(LONG* lAlpha)					= 0;
	virtual HRESULT put_PenAlpha(const LONG& lAlpha)			= 0;
	virtual HRESULT get_PenSize(double* dSize)					= 0;
	virtual HRESULT put_PenSize(const double& dSize)			= 0;
	virtual HRESULT get_PenDashStyle(BYTE* val)					= 0;
	virtual HRESULT put_PenDashStyle(const BYTE& val)			= 0;
	virtual HRESULT get_PenLineStartCap(BYTE* val)				= 0;
	virtual HRESULT put_PenLineStartCap(const BYTE& val)		= 0;
	virtual HRESULT get_PenLineEndCap(BYTE* val)				= 0;
	virtual HRESULT put_PenLineEndCap(const BYTE& val)			= 0;
	virtual HRESULT get_PenLineJoin(BYTE* val)					= 0;
	virtual HRESULT put_PenLineJoin(const BYTE& val)			= 0;
	virtual HRESULT get_PenDashOffset(double* dOffset)			= 0;
	virtual HRESULT put_PenDashOffset(const double& dOffset)	= 0;
	virtual HRESULT get_PenAlign(LONG* lAlign)					= 0;
	virtual HRESULT put_PenAlign(const LONG& lAlign)			= 0;
	virtual HRESULT get_PenMiterLimit(double* dOffset)			= 0;
	virtual HRESULT put_PenMiterLimit(const double& dOffset)	= 0;
	virtual HRESULT PenDashPattern(double* pPattern, LONG lCount)= 0;

// brush ------------------------------------------------------------------------------------
	virtual HRESULT get_BrushType(LONG* lType)							= 0;
	virtual HRESULT put_BrushType(const LONG& lType)					= 0;
	virtual HRESULT get_BrushColor1(LONG* lColor)						= 0;
	virtual HRESULT put_BrushColor1(const LONG& lColor)					= 0;
	virtual HRESULT get_BrushAlpha1(LONG* lAlpha)						= 0;
	virtual HRESULT put_BrushAlpha1(const LONG& lAlpha)					= 0;
	virtual HRESULT get_BrushColor2(LONG* lColor)						= 0;
	virtual HRESULT put_BrushColor2(const LONG& lColor)					= 0;
	virtual HRESULT get_BrushAlpha2(LONG* lAlpha)						= 0;
	virtual HRESULT put_BrushAlpha2(const LONG& lAlpha)					= 0;
	virtual HRESULT get_BrushTexturePath(std::wstring* bsPath)			= 0; 
	virtual HRESULT put_BrushTexturePath(const std::wstring& bsPath)	= 0; 
	virtual HRESULT get_BrushTextureMode(LONG* lMode)					= 0;
	virtual HRESULT put_BrushTextureMode(const LONG& lMode)				= 0;
	virtual HRESULT get_BrushTextureAlpha(LONG* lTxAlpha)				= 0;
	virtual HRESULT put_BrushTextureAlpha(const LONG& lTxAlpha)			= 0;
	virtual HRESULT get_BrushLinearAngle(double* dAngle)				= 0;
	virtual HRESULT put_BrushLinearAngle(const double& dAngle)			= 0;
	virtual HRESULT BrushRect(const INT& val, const double& left, const double& top, const double& width, const double& height) = 0;
	virtual HRESULT BrushBounds(const double& left, const double& top, const double& width, const double& height) = 0;

	virtual HRESULT put_BrushGradientColors(LONG* lColors, double* pPositions, LONG nCount) = 0;

// font -------------------------------------------------------------------------------------
	virtual HRESULT get_FontName(std::wstring* bsName)			= 0;
	virtual HRESULT put_FontName(const std::wstring& bsName)	= 0;
	virtual HRESULT get_FontPath(std::wstring* bsName)			= 0;
	virtual HRESULT put_FontPath(const std::wstring& bsName)	= 0;
	virtual HRESULT get_FontSize(double* dSize)					= 0;
	virtual HRESULT put_FontSize(const double& dSize)			= 0;
	virtual HRESULT get_FontStyle(LONG* lStyle)					= 0;
	virtual HRESULT put_FontStyle(const LONG& lStyle)			= 0;
	virtual HRESULT get_FontStringGID(INT* bGID)				= 0;
	virtual HRESULT put_FontStringGID(const INT& bGID)			= 0;
	virtual HRESULT get_FontCharSpace(double* dSpace)			= 0;
	virtual HRESULT put_FontCharSpace(const double& dSpace)		= 0;
	virtual HRESULT get_FontFaceIndex(int* lFaceIndex)			= 0;
	virtual HRESULT put_FontFaceIndex(const int& lFaceIndex)	= 0;

//-------- Функции для вывода текста --------------------------------------------------------
	virtual HRESULT CommandDrawTextCHAR(const LONG& c, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset) = 0;
	virtual HRESULT CommandDrawText(const std::wstring& bsText, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset) = 0;
	
	virtual HRESULT CommandDrawTextExCHAR(const LONG& c, const LONG& gid, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset, const DWORD& lFlags) = 0;
	virtual HRESULT CommandDrawTextEx(const std::wstring& bsUnicodeText, const std::wstring& bsGidText, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset, const DWORD& lFlags) = 0;

//-------- Маркеры для команд ---------------------------------------------------------------
	virtual HRESULT BeginCommand(const DWORD& lType)	= 0;
	virtual HRESULT EndCommand(const DWORD& lType)		= 0;

//-------- Функции для работы с Graphics Path -----------------------------------------------
	virtual HRESULT PathCommandMoveTo(const double& x, const double& y)			= 0;
	virtual HRESULT PathCommandLineTo(const double& x, const double& y)			= 0;
	virtual HRESULT PathCommandLinesTo(double* points, const int& count)		= 0;
	virtual HRESULT PathCommandCurveTo(const double& x1, const double& y1, const double& x2, const double& y2, const double& x3, const double& y3)	 = 0;
	virtual HRESULT PathCommandCurvesTo(double* points, const int& count)		= 0;
	virtual HRESULT PathCommandArcTo(const double& x, const double& y, const double& w, const double& h, const double& startAngle, const double& sweepAngle) = 0;
	virtual HRESULT PathCommandClose()											= 0;
	virtual HRESULT PathCommandEnd()											= 0;
	virtual HRESULT DrawPath(const LONG& nType)									= 0;
	virtual HRESULT PathCommandStart()											= 0;
	virtual HRESULT PathCommandGetCurrentPoint(double* x, double* y)			= 0;

	virtual HRESULT PathCommandTextCHAR(const LONG& c, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset) = 0;
	virtual HRESULT PathCommandText(const std::wstring& bsText, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset) = 0;
	
	virtual HRESULT PathCommandTextExCHAR(const LONG& c, const LONG& gid, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset, const DWORD& lFlags) = 0;
	virtual HRESULT PathCommandTextEx(const std::wstring& bsUnicodeText, const std::wstring& bsGidText, const double& x, const double& y, const double& w, const double& h, const double& baselineOffset, const DWORD& lFlags) = 0;

//-------- Функции для вывода изображений ---------------------------------------------------
	virtual HRESULT DrawImage(IGrObject* pImage, const double& x, const double& y, const double& w, const double& h)		= 0;
	virtual HRESULT DrawImageFromFile(const std::wstring&, const double& x, const double& y, const double& w, const double& h, const BYTE& lAlpha = 255)	= 0;	

// transform --------------------------------------------------------------------------------
	virtual HRESULT GetCommandParams(double* dAngle, double* dLeft, double* dTop, double* dWidth, double* dHeight, DWORD* lFlags)
	{
		return S_OK;
	}
	virtual HRESULT SetCommandParams(double dAngle, double dLeft, double dTop, double dWidth, double dHeight, DWORD lFlags)
	{
		if ((dWidth <= 1) || (dHeight <= 1))
			lFlags = 0;

		INT bFlipX = (0 != (c_nParamFlipX & lFlags));
		INT bFlipY = (0 != (c_nParamFlipY & lFlags));

		REAL m11 = bFlipX ? -1.0f : 1.0f;
		REAL m22 = bFlipY ? -1.0f : 1.0f;

		Aggplus::CMatrix oMatrix(1, 0, 0, 1, 0, 0);

		if ((0 != dAngle) || (0 != lFlags))
		{
			REAL dCentreX = (REAL)(dLeft + dWidth / 2.0);
			REAL dCentreY = (REAL)(dTop + dHeight / 2.0);

			oMatrix.Translate(-dCentreX, -dCentreY	, Aggplus::MatrixOrderAppend);

			oMatrix.Rotate((REAL)dAngle	, Aggplus::MatrixOrderAppend);
			oMatrix.Scale(m11, m22					, Aggplus::MatrixOrderAppend);

			oMatrix.Translate(dCentreX, dCentreY	, Aggplus::MatrixOrderAppend);
		}
		double mass[6];
		oMatrix.GetElements(mass);
		SetTransform(mass[0], mass[1], mass[2], mass[3], mass[4], mass[5]);
		return S_OK;
	}
	virtual HRESULT SetTransform(const double& m1, const double& m2, const double& m3, const double& m4, const double& m5, const double& m6) = 0;
	virtual HRESULT GetTransform(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF)	= 0;
	virtual HRESULT ResetTransform() = 0;

// -----------------------------------------------------------------------------------------
	virtual HRESULT get_ClipMode(LONG* plMode)			= 0;
	virtual HRESULT put_ClipMode(const LONG& lMode)		= 0;

// additiaonal params ----------------------------------------------------------------------
	virtual HRESULT CommandLong(const LONG& lType, const LONG& lCommand)			= 0;
	virtual HRESULT CommandDouble(const LONG& lType, const double& dCommand)		= 0;
	virtual HRESULT CommandString(const LONG& lType, const std::wstring& sCommand)	= 0;	
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

#endif // _BUILD_IRENDERER_H_