#include "stdafx.h"
#include "HTMLRenderer.h"
#include "Resource.h"

CASCHTMLRenderer::CASCHTMLRenderer()
{
}

HRESULT CASCHTMLRenderer::FinalConstruct()
{
	return S_OK;
}

void CASCHTMLRenderer::FinalRelease()
{
}

STDMETHODIMP CASCHTMLRenderer::get_Type(LONG* lType)
{
	return S_OK;
}
//-------- Функции для работы со страницей --------------------------------------------------
STDMETHODIMP CASCHTMLRenderer::NewPage()
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_Height(double* dHeight)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_Height(double dHeight)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_Width(double* dWidth)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_Width(double dWidth)
{
	return S_OK;
}

STDMETHODIMP CASCHTMLRenderer::get_DpiX(double* dDpiX)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_DpiY(double* dDpiY)
{
	return S_OK;
}
// pen --------------------------------------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer::SetPen(BSTR bsXML)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_PenColor(LONG* lColor)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_PenColor(LONG lColor)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_PenAlpha(LONG* lAlpha)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_PenAlpha(LONG lAlpha)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_PenSize(double* dSize)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_PenSize(double dSize)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_PenDashStyle(BYTE* val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_PenDashStyle(BYTE val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_PenLineStartCap(BYTE* val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_PenLineStartCap(BYTE val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_PenLineEndCap(BYTE* val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_PenLineEndCap(BYTE val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_PenLineJoin(BYTE* val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_PenLineJoin(BYTE val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_PenDashOffset(double* val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_PenDashOffset(double val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_PenAlign(LONG* val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_PenAlign(LONG val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_PenMiterLimit(double* val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_PenMiterLimit(double val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::PenDashPattern(SAFEARRAY* pPattern)
{
	return S_OK;
}
// brush ------------------------------------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer::SetBrush(BSTR bsXML)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_BrushType(LONG* lType)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_BrushType(LONG lType)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_BrushColor1(LONG* lColor)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_BrushColor1(LONG lColor)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_BrushAlpha1(LONG* lAlpha)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_BrushAlpha1(LONG lAlpha)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_BrushColor2(LONG* lColor)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_BrushColor2(LONG lColor)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_BrushAlpha2(LONG* lAlpha)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_BrushAlpha2(LONG lAlpha)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_BrushTexturePath(BSTR* bsPath)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_BrushTexturePath(BSTR bsPath)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_BrushTextureMode(LONG* lMode)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_BrushTextureMode(LONG lMode)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_BrushTextureAlpha(LONG* lTxAlpha)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_BrushTextureAlpha(LONG lTxAlpha)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_BrushLinearAngle(double* dAngle)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_BrushLinearAngle(double dAngle)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::BrushRect(BOOL val, double left, double top, double width, double height)
{
	return S_OK;
}
// font -------------------------------------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer::SetFont(BSTR bsXML)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_FontName(BSTR* bsName)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_FontName(BSTR bsName)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_FontPath(BSTR* bsName)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_FontPath(BSTR bsName)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_FontSize(double* dSize)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_FontSize(double dSize)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_FontStyle(LONG* lStyle)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_FontStyle(LONG lStyle)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_FontStringGID(BOOL* bGID)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_FontStringGID(BOOL bGID)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_FontCharSpace(double* dSpace)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_FontCharSpace(double dSpace)
{
	return S_OK;
}
// shadow -----------------------------------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer::SetShadow(BSTR bsXML)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_ShadowDistanceX(double* val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_ShadowDistanceX(double val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_ShadowDistanceY(double* val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_ShadowDistanceY(double val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_ShadowBlurSize(double* val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_ShadowBlurSize(double val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_ShadowColor(LONG* val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_ShadowColor(LONG val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_ShadowAlpha(LONG* val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_ShadowAlpha(LONG val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_ShadowVisible(BOOL* val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_ShadowVisible(BOOL val)
{
	return S_OK;
}
// edge -------------------------------------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer::SetEdgeText(BSTR bsXML)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_EdgeVisible(LONG* val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_EdgeVisible(LONG val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_EdgeColor(LONG* val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_EdgeColor(LONG val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_EdgeAlpha(LONG* val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_EdgeAlpha(LONG val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::get_EdgeDist(double* val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_EdgeDist(double val)
{
	return S_OK;
}
//-------- Функции для вывода текста --------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer::CommandDrawText(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::CommandDrawTextEx(BSTR bsText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
{
	return S_OK;
}
//-------- Маркеры для команд ---------------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer::BeginCommand(DWORD lType)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::EndCommand(DWORD lType)
{
	return S_OK;
}
//-------- Функции для работы с Graphics Path -----------------------------------------------
STDMETHODIMP CASCHTMLRenderer::PathCommandMoveTo(double fX, double fY)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::PathCommandLineTo(double fX, double fY)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::PathCommandLinesTo(SAFEARRAY* pPoints)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::PathCommandCurveTo(double fX1, double fY1, double fX2, double fY2, double fX3, double fY3)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::PathCommandCurvesTo(SAFEARRAY* pPoints)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::PathCommandArcTo(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::PathCommandClose()
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::PathCommandEnd()
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::DrawPath(long nType)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::PathCommandStart()
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::PathCommandGetCurrentPoint(double* fX, double* fY)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::PathCommandText(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::PathCommandTextEx(BSTR bsText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
{
	return S_OK;
}
//-------- Функции для вывода изображений ---------------------------------------------------
STDMETHODIMP CASCHTMLRenderer::DrawImage(IUnknown* pInterface, double fX, double fY, double fWidth, double fHeight)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::DrawImageFromFile(BSTR bstrVal, double fX, double fY, double fWidth, double fHeight)
{
	return S_OK;
}
// transform --------------------------------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer::GetCommandParams(double* dAngle, double* dLeft, double* dTop, double* dWidth, double* dHeight, DWORD* lFlags)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::SetCommandParams(double dAngle, double dLeft, double dTop, double dWidth, double dHeight, DWORD lFlags)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::SetTransform(double dA, double dB, double dC, double dD, double dE, double dF)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::GetTransform(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::ResetTransform(void)
{
	return S_OK;
}
// -----------------------------------------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer::get_ClipMode(LONG* plMode)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_ClipMode(LONG lMode)
{
	return S_OK;
}
// additiaonal params ----------------------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer::SetAdditionalParam(BSTR ParamName, VARIANT ParamValue)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::GetAdditionalParam(BSTR ParamName, VARIANT* ParamValue)
{
	return S_OK;
}
// --------------------------------------------------------------------------------------------

// file
STDMETHODIMP CASCHTMLRenderer::CreateOfficeFile(BSTR bsFilePath)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::CloseFile()
{
	return S_OK;
}

STDMETHODIMP CASCHTMLRenderer::get_Mode(LONG* plMode)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer::put_Mode(LONG lMode)
{
	return S_OK;
}
