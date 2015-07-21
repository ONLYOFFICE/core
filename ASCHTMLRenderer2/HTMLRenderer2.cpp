#include "stdafx.h"
#include "HTMLRenderer2.h"
#include "Resource.h"

HRESULT CASCHTMLRenderer2::FinalConstruct()
{
	return S_OK;
}

void CASCHTMLRenderer2::FinalRelease()
{
}

STDMETHODIMP CASCHTMLRenderer2::get_Type(LONG* lType)
{
	return S_OK;
}
//-------- Функции для работы со страницей --------------------------------------------------
STDMETHODIMP CASCHTMLRenderer2::NewPage()
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::get_Height(double* dHeight)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::put_Height(double dHeight)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::get_Width(double* dWidth)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::put_Width(double dWidth)
{
	return S_OK;
}

STDMETHODIMP CASCHTMLRenderer2::get_DpiX(double* dDpiX)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::get_DpiY(double* dDpiY)
{
	return S_OK;
}
// pen --------------------------------------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer2::SetPen(BSTR bsXML)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::get_PenColor(LONG* lColor)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::put_PenColor(LONG lColor)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::get_PenAlpha(LONG* lAlpha)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::put_PenAlpha(LONG lAlpha)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::get_PenSize(double* dSize)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::put_PenSize(double dSize)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::get_PenDashStyle(BYTE* val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::put_PenDashStyle(BYTE val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::get_PenLineStartCap(BYTE* val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::put_PenLineStartCap(BYTE val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::get_PenLineEndCap(BYTE* val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::put_PenLineEndCap(BYTE val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::get_PenLineJoin(BYTE* val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::put_PenLineJoin(BYTE val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::get_PenDashOffset(double* val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::put_PenDashOffset(double val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::get_PenAlign(LONG* val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::put_PenAlign(LONG val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::get_PenMiterLimit(double* val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::put_PenMiterLimit(double val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::PenDashPattern(SAFEARRAY* pPattern)
{
	return S_OK;
}
// brush ------------------------------------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer2::SetBrush(BSTR bsXML)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::get_BrushType(LONG* lType)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::put_BrushType(LONG lType)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::get_BrushColor1(LONG* lColor)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::put_BrushColor1(LONG lColor)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::get_BrushAlpha1(LONG* lAlpha)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::put_BrushAlpha1(LONG lAlpha)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::get_BrushColor2(LONG* lColor)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::put_BrushColor2(LONG lColor)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::get_BrushAlpha2(LONG* lAlpha)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::put_BrushAlpha2(LONG lAlpha)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::get_BrushTexturePath(BSTR* bsPath)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::put_BrushTexturePath(BSTR bsPath)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::get_BrushTextureMode(LONG* lMode)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::put_BrushTextureMode(LONG lMode)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::get_BrushTextureAlpha(LONG* lTxAlpha)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::put_BrushTextureAlpha(LONG lTxAlpha)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::get_BrushLinearAngle(double* dAngle)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::put_BrushLinearAngle(double dAngle)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::BrushRect(BOOL val, double left, double top, double width, double height)
{
	return S_OK;
}
// font -------------------------------------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer2::SetFont(BSTR bsXML)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::get_FontName(BSTR* bsName)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::put_FontName(BSTR bsName)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::get_FontPath(BSTR* bsName)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::put_FontPath(BSTR bsName)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::get_FontSize(double* dSize)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::put_FontSize(double dSize)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::get_FontStyle(LONG* lStyle)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::put_FontStyle(LONG lStyle)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::get_FontStringGID(BOOL* bGID)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::put_FontStringGID(BOOL bGID)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::get_FontCharSpace(double* dSpace)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::put_FontCharSpace(double dSpace)
{
	return S_OK;
}
// shadow -----------------------------------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer2::SetShadow(BSTR bsXML)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::get_ShadowDistanceX(double* val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::put_ShadowDistanceX(double val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::get_ShadowDistanceY(double* val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::put_ShadowDistanceY(double val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::get_ShadowBlurSize(double* val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::put_ShadowBlurSize(double val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::get_ShadowColor(LONG* val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::put_ShadowColor(LONG val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::get_ShadowAlpha(LONG* val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::put_ShadowAlpha(LONG val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::get_ShadowVisible(BOOL* val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::put_ShadowVisible(BOOL val)
{
	return S_OK;
}
// edge -------------------------------------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer2::SetEdgeText(BSTR bsXML)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::get_EdgeVisible(LONG* val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::put_EdgeVisible(LONG val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::get_EdgeColor(LONG* val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::put_EdgeColor(LONG val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::get_EdgeAlpha(LONG* val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::put_EdgeAlpha(LONG val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::get_EdgeDist(double* val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::put_EdgeDist(double val)
{
	return S_OK;
}
//-------- Функции для вывода текста --------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer2::CommandDrawText(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::CommandDrawTextEx(BSTR bsText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
{	
	return S_OK;
}
//-------- Маркеры для команд ---------------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer2::BeginCommand(DWORD lType)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::EndCommand(DWORD lType)
{
	return S_OK;
}
//-------- Функции для работы с Graphics Path -----------------------------------------------
STDMETHODIMP CASCHTMLRenderer2::PathCommandMoveTo(double fX, double fY)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::PathCommandLineTo(double fX, double fY)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::PathCommandLinesTo(SAFEARRAY* pPoints)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::PathCommandCurveTo(double fX1, double fY1, double fX2, double fY2, double fX3, double fY3)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::PathCommandCurvesTo(SAFEARRAY* pPoints)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::PathCommandArcTo(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::PathCommandClose()
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::PathCommandEnd()
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::DrawPath(long nType)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::PathCommandStart()
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::PathCommandGetCurrentPoint(double* fX, double* fY)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::PathCommandText(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::PathCommandTextEx(BSTR bsText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
{
	return S_OK;
}
//-------- Функции для вывода изображений ---------------------------------------------------
STDMETHODIMP CASCHTMLRenderer2::DrawImage(IUnknown* pInterface, double fX, double fY, double fWidth, double fHeight)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::DrawImageFromFile(BSTR bstrVal, double fX, double fY, double fWidth, double fHeight)
{
	return S_OK;
}
// transform --------------------------------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer2::GetCommandParams(double* dAngle, double* dLeft, double* dTop, double* dWidth, double* dHeight, DWORD* lFlags)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::SetCommandParams(double dAngle, double dLeft, double dTop, double dWidth, double dHeight, DWORD lFlags)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::SetTransform(double dA, double dB, double dC, double dD, double dE, double dF)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::GetTransform(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::ResetTransform(void)
{
	return S_OK;
}
// -----------------------------------------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer2::get_ClipMode(LONG* plMode)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::put_ClipMode(LONG lMode)
{
	return S_OK;
}
// additiaonal params ----------------------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer2::SetAdditionalParam(BSTR ParamName, VARIANT ParamValue)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::GetAdditionalParam(BSTR ParamName, VARIANT* ParamValue)
{	
	return S_OK;
}
// --------------------------------------------------------------------------------------------

// file
STDMETHODIMP CASCHTMLRenderer2::CreateOfficeFile(BSTR bsFilePath)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::CloseFile()
{
	return S_OK;
}

STDMETHODIMP CASCHTMLRenderer2::get_Mode(LONG* plMode)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer2::put_Mode(LONG lMode)
{
	return S_OK;
}
