#pragma once
#include "IASCGraphicsRenderer.h"
#include "GdiplusRenderer.h"

[coclass, uuid("A6145D77-5C72-4d61-A2D7-7D8DB41E8260"), threading(apartment), vi_progid("GraphicsRenderer"), progid("GraphicsRenderer.1"), version(1.0)]
class ATL_NO_VTABLE CASCGdiplusRenderer :	public IASCGraphicsRenderer
{
private:
	CGdiplusRenderer m_oRenderer;

public:
	CASCGdiplusRenderer() : m_oRenderer()
	{
	}

	~CASCGdiplusRenderer()
	{
	}

public:

	STDMETHOD(get_Type)(LONG* lType)
	{
		return S_OK;
	}
	//-------- Функции для работы со страницей --------------------------------------------------
	STDMETHOD(NewPage)()
	{
		return m_oRenderer.NewPage();
	}
	STDMETHOD(get_Height)(double* dHeight)
	{
		return m_oRenderer.get_Height(dHeight);
	}
	STDMETHOD(put_Height)(double dHeight)
	{
		return m_oRenderer.put_Height(dHeight);
	}
	STDMETHOD(get_Width)(double* dWidth)
	{
		return m_oRenderer.get_Width(dWidth);
	}
	STDMETHOD(put_Width)(double dWidth)
	{
		return m_oRenderer.put_Width(dWidth);
	}
	STDMETHOD(get_DpiX)(double* dDpiX)
	{
		return m_oRenderer.get_DpiX(dDpiX);
	}
	STDMETHOD(get_DpiY)(double* dDpiY)
	{
		return m_oRenderer.get_DpiY(dDpiY);
	}

// pen --------------------------------------------------------------------------------------
	STDMETHOD(SetPen)(BSTR bsXML)
	{
		return m_oRenderer.SetPen(bsXML);
	}
	STDMETHOD(get_PenColor)(LONG* lColor)
	{
		return m_oRenderer.get_PenColor(lColor);
	}
	STDMETHOD(put_PenColor)(LONG lColor)
	{
		return m_oRenderer.put_PenColor(lColor);
	}
	STDMETHOD(get_PenAlpha)(LONG* lAlpha)
	{
		return m_oRenderer.get_PenAlpha(lAlpha);
	}
	STDMETHOD(put_PenAlpha)(LONG lAlpha)
	{
		return m_oRenderer.put_PenAlpha(lAlpha);
	}
	STDMETHOD(get_PenSize)(double* dSize)
	{
		return m_oRenderer.get_PenSize(dSize);
	}
	STDMETHOD(put_PenSize)(double dSize)
	{
		return m_oRenderer.put_PenSize(dSize);
	}
	STDMETHOD(get_PenDashStyle)(BYTE* val)
	{
		return m_oRenderer.get_PenDashStyle(val);
	}
	STDMETHOD(put_PenDashStyle)(BYTE val)
	{
		return m_oRenderer.put_PenDashStyle(val);
	}
	STDMETHOD(get_PenLineStartCap)(BYTE* val)
	{
		return m_oRenderer.get_PenLineStartCap(val);
	}
	STDMETHOD(put_PenLineStartCap)(BYTE val)
	{
		return m_oRenderer.put_PenLineStartCap(val);
	}
	STDMETHOD(get_PenLineEndCap)(BYTE* val)
	{
		return m_oRenderer.get_PenLineEndCap(val);
	}
	STDMETHOD(put_PenLineEndCap)(BYTE val)
	{
		return m_oRenderer.put_PenLineEndCap(val);
	}
	STDMETHOD(get_PenLineJoin)(BYTE* val)
	{
		return m_oRenderer.get_PenLineJoin(val);
	}
	STDMETHOD(put_PenLineJoin)(BYTE val)
	{
		return m_oRenderer.put_PenLineJoin(val);
	}
	STDMETHOD(get_PenDashOffset)(double* val)
	{
		return m_oRenderer.get_PenDashOffset(val);
	}
	STDMETHOD(put_PenDashOffset)(double val)
	{
		return m_oRenderer.put_PenDashOffset(val);
	}
	STDMETHOD(get_PenAlign)(LONG* val)
	{
		return m_oRenderer.get_PenAlign(val);
	}
	STDMETHOD(put_PenAlign)(LONG val)
	{
		return m_oRenderer.put_PenAlign(val);
	}
	STDMETHOD(get_PenMiterLimit)(double* val)
	{
		return m_oRenderer.get_PenMiterLimit(val);
	}
	STDMETHOD(put_PenMiterLimit)(double val)
	{
		return m_oRenderer.put_PenMiterLimit(val);
	}
	STDMETHOD(PenDashPattern)(SAFEARRAY* pPattern)
	{
		return m_oRenderer.PenDashPattern(pPattern);
	}

// brush ------------------------------------------------------------------------------------
	STDMETHOD(SetBrush)(BSTR bsXML)
	{
		return m_oRenderer.SetBrush(bsXML);
	}
	STDMETHOD(get_BrushType)(LONG* lType)
	{
		return m_oRenderer.get_BrushType(lType);
	}
	STDMETHOD(put_BrushType)(LONG lType)
	{
		return m_oRenderer.put_BrushType(lType);
	}
	STDMETHOD(get_BrushColor1)(LONG* lColor)
	{
		return m_oRenderer.get_BrushColor1(lColor);
	}
	STDMETHOD(put_BrushColor1)(LONG lColor)
	{
		return m_oRenderer.put_BrushColor1(lColor);
	}
	STDMETHOD(get_BrushAlpha1)(LONG* lAlpha)
	{
		return m_oRenderer.get_BrushAlpha1(lAlpha);
	}
	STDMETHOD(put_BrushAlpha1)(LONG lAlpha)
	{
		return m_oRenderer.put_BrushAlpha1(lAlpha);
	}
	STDMETHOD(get_BrushColor2)(LONG* lColor)
	{
		return m_oRenderer.get_BrushColor2(lColor);
	}
	STDMETHOD(put_BrushColor2)(LONG lColor)
	{
		return m_oRenderer.put_BrushColor2(lColor);
	}
	STDMETHOD(get_BrushAlpha2)(LONG* lAlpha)
	{
		return m_oRenderer.get_BrushAlpha2(lAlpha);
	}
	STDMETHOD(put_BrushAlpha2)(LONG lAlpha)
	{
		return m_oRenderer.put_BrushAlpha2(lAlpha);
	}
	STDMETHOD(get_BrushTexturePath)(BSTR* bsPath)
	{
		return m_oRenderer.get_BrushTexturePath(bsPath);
	}
	STDMETHOD(put_BrushTexturePath)(BSTR bsPath)
	{
		return m_oRenderer.put_BrushTexturePath(bsPath);
	}
	STDMETHOD(get_BrushTextureMode)(LONG* lMode)
	{
		return m_oRenderer.get_BrushTextureMode(lMode);
	}
	STDMETHOD(put_BrushTextureMode)(LONG lMode)
	{
		return m_oRenderer.put_BrushTextureMode(lMode);
	}
	STDMETHOD(get_BrushTextureAlpha)(LONG* lTxAlpha)
	{
		return m_oRenderer.get_BrushTextureAlpha(lTxAlpha);
	}
	STDMETHOD(put_BrushTextureAlpha)(LONG lTxAlpha)
	{
		return m_oRenderer.put_BrushTextureAlpha(lTxAlpha);
	}
	STDMETHOD(get_BrushLinearAngle)(double* dAngle)
	{
		return m_oRenderer.get_BrushLinearAngle(dAngle);
	}
	STDMETHOD(put_BrushLinearAngle)(double dAngle)
	{
		return m_oRenderer.put_BrushLinearAngle(dAngle);
	}
	STDMETHOD(BrushRect)(BOOL val, double left, double top, double width, double height)
	{
		return m_oRenderer.BrushRect(val, left, top, width, height);
	}

// font -------------------------------------------------------------------------------------
	STDMETHOD(SetFont)(BSTR bsXML)
	{
		return m_oRenderer.SetFont(bsXML);
	}
	STDMETHOD(get_FontName)(BSTR* bsName)
	{
		return m_oRenderer.get_FontName(bsName);
	}
	STDMETHOD(put_FontName)(BSTR bsName)
	{
		return m_oRenderer.put_FontName(bsName);
	}
	STDMETHOD(get_FontPath)(BSTR* bsName)
	{
		return m_oRenderer.get_FontPath(bsName);
	}
	STDMETHOD(put_FontPath)(BSTR bsName)
	{
		return m_oRenderer.put_FontPath(bsName);
	}
	STDMETHOD(get_FontSize)(double* dSize)
	{
		return m_oRenderer.get_FontSize(dSize);
	}
	STDMETHOD(put_FontSize)(double dSize)
	{
		return m_oRenderer.put_FontSize(dSize);
	}
	STDMETHOD(get_FontStyle)(LONG* lStyle)
	{
		return m_oRenderer.get_FontStyle(lStyle);
	}
	STDMETHOD(put_FontStyle)(LONG lStyle)
	{
		return m_oRenderer.put_FontStyle(lStyle);
	}
	STDMETHOD(get_FontStringGID)(BOOL* bGID)
	{
		return m_oRenderer.get_FontStringGID(bGID);
	}
	STDMETHOD(put_FontStringGID)(BOOL bGID)
	{
		return m_oRenderer.put_FontStringGID(bGID);
	}
	STDMETHOD(get_FontCharSpace)(double* dSpace)
	{
		return m_oRenderer.get_FontCharSpace(dSpace);
	}
	STDMETHOD(put_FontCharSpace)(double dSpace)
	{
		return m_oRenderer.put_FontCharSpace(dSpace);
	}

// shadow -----------------------------------------------------------------------------------
	STDMETHOD(SetShadow)(BSTR bsXML)
	{
		return m_oRenderer.SetShadow(bsXML);
	}
	STDMETHOD(get_ShadowDistanceX)(double* val)
	{
		return m_oRenderer.get_ShadowDistanceX(val);
	}
	STDMETHOD(put_ShadowDistanceX)(double val)
	{
		return m_oRenderer.put_ShadowDistanceX(val);
	}
	STDMETHOD(get_ShadowDistanceY)(double* val)
	{
		return m_oRenderer.get_ShadowDistanceY(val);
	}
	STDMETHOD(put_ShadowDistanceY)(double val)
	{
		return m_oRenderer.put_ShadowDistanceY(val);
	}
	STDMETHOD(get_ShadowBlurSize)(double* val)
	{
		return m_oRenderer.get_ShadowBlurSize(val);
	}
	STDMETHOD(put_ShadowBlurSize)(double val)
	{
		return m_oRenderer.put_ShadowBlurSize(val);
	}
	STDMETHOD(get_ShadowColor)(LONG* val)
	{
		return m_oRenderer.get_ShadowColor(val);
	}
	STDMETHOD(put_ShadowColor)(LONG val)
	{
		return m_oRenderer.put_ShadowColor(val);
	}
	STDMETHOD(get_ShadowAlpha)(LONG* val)
	{
		return m_oRenderer.get_ShadowAlpha(val);
	}
	STDMETHOD(put_ShadowAlpha)(LONG val)
	{
		return m_oRenderer.put_ShadowAlpha(val);
	}
	STDMETHOD(get_ShadowVisible)(BOOL* val)
	{
		return m_oRenderer.get_ShadowVisible(val);
	}
	STDMETHOD(put_ShadowVisible)(BOOL val)
	{
		return m_oRenderer.put_ShadowVisible(val);
	}

// edge -------------------------------------------------------------------------------------
	STDMETHOD(SetEdgeText)(BSTR bsXML)
	{
		return m_oRenderer.SetEdgeText(bsXML);
	}
	STDMETHOD(get_EdgeVisible)(LONG* val)
	{
		return m_oRenderer.get_EdgeVisible(val);
	}
	STDMETHOD(put_EdgeVisible)(LONG val)
	{
		return m_oRenderer.put_EdgeVisible(val);
	}
	STDMETHOD(get_EdgeColor)(LONG* val)
	{
		return m_oRenderer.get_EdgeColor(val);
	}
	STDMETHOD(put_EdgeColor)(LONG val)
	{
		return m_oRenderer.put_EdgeColor(val);
	}
	STDMETHOD(get_EdgeAlpha)(LONG* val)
	{
		return m_oRenderer.get_EdgeAlpha(val);
	}
	STDMETHOD(put_EdgeAlpha)(LONG val)
	{
		return m_oRenderer.put_EdgeAlpha(val);
	}
	STDMETHOD(get_EdgeDist)(double* val)
	{
		return m_oRenderer.get_EdgeDist(val);
	}
	STDMETHOD(put_EdgeDist)(double val)
	{
		return m_oRenderer.put_EdgeDist(val);
	}

//-------- Функции для вывода текста --------------------------------------------------------
	STDMETHOD(CommandDrawText)(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
	{
		return m_oRenderer.CommandDrawText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);
	}
	STDMETHOD(CommandDrawTextEx)(BSTR bsText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
	{
		return m_oRenderer.CommandDrawTextEx(bsText, bsGidText, bsSourceCodeText, fX, fY, fWidth, fHeight, fBaseLineOffset, lFlags);
	}

//-------- Маркеры для команд ---------------------------------------------------------------
	STDMETHOD(BeginCommand)(DWORD lType)
	{
		return m_oRenderer.BeginCommand(lType);
	}
	STDMETHOD(EndCommand)(DWORD lType)
	{
		return m_oRenderer.EndCommand(lType);
	}

//-------- Функции для работы с Graphics Path -----------------------------------------------
	STDMETHOD(PathCommandMoveTo)(double fX, double fY)
	{
		return m_oRenderer.PathCommandMoveTo(fX, fY);
	}
	STDMETHOD(PathCommandLineTo)(double fX, double fY)
	{
		return m_oRenderer.PathCommandLineTo(fX, fY);
	}
	STDMETHOD(PathCommandLinesTo)(SAFEARRAY* pPoints)
	{
		return m_oRenderer.PathCommandLinesTo(pPoints);
	}
	STDMETHOD(PathCommandCurveTo)(double fX1, double fY1, double fX2, double fY2, double fX3, double fY3)
	{
		return m_oRenderer.PathCommandCurveTo(fX1, fY1, fX2, fY2, fX3, fY3);
	}
	STDMETHOD(PathCommandCurvesTo)(SAFEARRAY* pPoints)
	{
		return m_oRenderer.PathCommandCurvesTo(pPoints);
	}
	STDMETHOD(PathCommandArcTo)(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle)
	{
		return m_oRenderer.PathCommandArcTo(fX, fY, fWidth, fHeight, fStartAngle, fSweepAngle);
	}
	STDMETHOD(PathCommandClose)()
	{
		return m_oRenderer.PathCommandClose();
	}
	STDMETHOD(PathCommandEnd)()
	{
		return m_oRenderer.PathCommandEnd();
	}
	STDMETHOD(DrawPath)(long nType)
	{
		return m_oRenderer.DrawPath(nType);
	}
	STDMETHOD(PathCommandStart)()
	{
		return m_oRenderer.PathCommandStart();
	}
	STDMETHOD(PathCommandGetCurrentPoint)(double* fX, double* fY)
	{
		return m_oRenderer.PathCommandGetCurrentPoint(fX, fY);
	}
	STDMETHOD(PathCommandText)(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
	{
		return m_oRenderer.PathCommandText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);
	}
	STDMETHOD(PathCommandTextEx)(BSTR bsText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
	{
		return m_oRenderer.PathCommandTextEx(bsText, bsGidText, bsSourceCodeText, fX, fY, fWidth, fHeight, fBaseLineOffset, lFlags);
	}

//-------- Функции для вывода изображений ---------------------------------------------------
	STDMETHOD(DrawImage)(IUnknown* pInterface, double fX, double fY, double fWidth, double fHeight)
	{
		return m_oRenderer.DrawImage(pInterface, fX, fY, fWidth, fHeight);
	}
	STDMETHOD(DrawImageFromFile)(BSTR bstrVal, double fX, double fY, double fWidth, double fHeight)
	{
		return m_oRenderer.DrawImageFromFile(bstrVal, fX, fY, fWidth, fHeight);
	}

// transform --------------------------------------------------------------------------------
	STDMETHOD(GetCommandParams)(double* dAngle, double* dLeft, double* dTop, double* dWidth, double* dHeight, DWORD* lFlags)
	{
		return m_oRenderer.GetCommandParams(dAngle, dLeft, dTop, dWidth, dHeight, lFlags);
	}
	STDMETHOD(SetCommandParams)(double dAngle, double dLeft, double dTop, double dWidth, double dHeight, DWORD lFlags)
	{
		return m_oRenderer.SetCommandParams(dAngle, dLeft, dTop, dWidth, dHeight, lFlags);
	}

	STDMETHOD(SetTransform)(double dA, double dB, double dC, double dD, double dE, double dF)
	{
		return m_oRenderer.SetTransform(dA, dB, dC, dD, dE, dF);
	}
	STDMETHOD(GetTransform)(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF)
	{
		return m_oRenderer.GetTransform(pdA, pdB, pdC, pdD, pdE, pdF);
	}
	STDMETHOD(ResetTransform)(void)
	{
		return m_oRenderer.ResetTransform();
	}

// -----------------------------------------------------------------------------------------
	STDMETHOD(get_ClipMode)(LONG* plMode)
	{
		return m_oRenderer.get_ClipMode(plMode);
	}
	STDMETHOD(put_ClipMode)(LONG lMode)
	{
		return m_oRenderer.put_ClipMode(lMode);
	}

// additiaonal params ----------------------------------------------------------------------
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		return S_OK;
	}

// ASCGraphicsRenderer
	STDMETHOD(CreateFromMediaData)(IUnknown* punkMediaData, double dLeft, double dTop, double dWidth, double dHeight)
	{
		return S_OK;
	}
	STDMETHOD(CreateFromHDC)(LONG* pHDC, IUnknown* punkFontManager, double dWidthMM, double dHeightMM,
		double dLeft, double dTop, double dWidth, double dHeight, double dAngle)
	{
		return S_OK;
	}
	STDMETHOD(BitBlt)(LONG* pHDC)
	{
		return S_OK;
	}
};