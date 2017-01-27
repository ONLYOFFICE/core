// AVSDocxRenderer.cpp : Implementation of CAVSDocxRenderer

#include "stdafx.h"
#include "ASCDocxRenderer.h"


// CAVSDocxRenderer

STDMETHODIMP CAVSDocxRenderer::get_Type(LONG* lType)
{
	return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::NewPage()
{
	return m_oDocument.NewPage();
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_Height(double* dHeight)
{
	return m_oDocument.get_Height(dHeight);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_Height(double dHeight)
{
	return m_oDocument.put_Height(dHeight);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_Width(double* dWidth)
{
	return m_oDocument.get_Width(dWidth);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_Width(double dWidth)
{
	return m_oDocument.put_Width(dWidth);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_DpiX(double* dDpiX)
{
	return m_oDocument.get_DpiX(dDpiX);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_DpiY(double* dDpiY)
{
	return m_oDocument.get_DpiY(dDpiY);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::SetPen(BSTR bsXML)
{
	return m_oDocument.SetPen(bsXML);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_PenColor(LONG* lColor)
{
	return m_oDocument.get_PenColor(lColor);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_PenColor(LONG lColor)
{
	return m_oDocument.put_PenColor(lColor);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_PenAlpha(LONG* lAlpha)
{
	return m_oDocument.get_PenAlpha(lAlpha);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_PenAlpha(LONG lAlpha)
{
	return m_oDocument.put_PenAlpha(lAlpha);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_PenSize(double* dSize)
{
	return m_oDocument.get_PenSize(dSize);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_PenSize(double dSize)
{
	return m_oDocument.put_PenSize(dSize);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_PenDashStyle(BYTE* val)
{
	return m_oDocument.get_PenDashStyle(val);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_PenDashStyle(BYTE val)
{
	return m_oDocument.put_PenDashStyle(val);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_PenLineStartCap(BYTE* val)
{
	return m_oDocument.get_PenLineStartCap(val);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_PenLineStartCap(BYTE val)
{
	return m_oDocument.put_PenLineStartCap(val);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_PenLineEndCap(BYTE* val)
{
	return m_oDocument.get_PenLineEndCap(val);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_PenLineEndCap(BYTE val)
{
	return m_oDocument.put_PenLineEndCap(val);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_PenLineJoin(BYTE* val)
{
	return m_oDocument.get_PenLineJoin(val);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_PenLineJoin(BYTE val)
{
	return m_oDocument.put_PenLineJoin(val);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_PenDashOffset(double* val)
{
	return m_oDocument.get_PenDashOffset(val);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_PenDashOffset(double val)
{
	return m_oDocument.put_PenDashOffset(val);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_PenAlign(LONG* val)
{
	return m_oDocument.get_PenAlign(val);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_PenAlign(LONG val)
{
	return m_oDocument.put_PenAlign(val);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_PenMiterLimit(double* val)
{
	return m_oDocument.get_PenMiterLimit(val);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_PenMiterLimit(double val)
{
	return m_oDocument.put_PenMiterLimit(val);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::PenDashPattern(SAFEARRAY* pPattern)
{
	return m_oDocument.PenDashPattern(pPattern);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::SetBrush(BSTR bsXML)
{
	return m_oDocument.SetBrush(bsXML);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_BrushType(LONG* lType)
{
	return m_oDocument.get_BrushType(lType);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_BrushType(LONG lType)
{
	return m_oDocument.put_BrushType(lType);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_BrushColor1(LONG* lColor)
{
	return m_oDocument.get_BrushColor1(lColor);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_BrushColor1(LONG lColor)
{
	return m_oDocument.put_BrushColor1(lColor);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_BrushAlpha1(LONG* lAlpha)
{
	return m_oDocument.get_BrushAlpha1(lAlpha);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_BrushAlpha1(LONG lAlpha)
{
	return m_oDocument.put_BrushAlpha1(lAlpha);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_BrushColor2(LONG* lColor)
{
	return m_oDocument.get_BrushColor2(lColor);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_BrushColor2(LONG lColor)
{
	return m_oDocument.put_BrushColor2(lColor);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_BrushAlpha2(LONG* lAlpha)
{
	return m_oDocument.get_BrushAlpha2(lAlpha);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_BrushAlpha2(LONG lAlpha)
{
	return m_oDocument.put_BrushAlpha2(lAlpha);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_BrushTexturePath(BSTR* bsPath)
{
	return m_oDocument.get_BrushTexturePath(bsPath);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_BrushTexturePath(BSTR bsPath)
{
	return m_oDocument.put_BrushTexturePath(bsPath);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_BrushTextureMode(LONG* lMode)
{
	return m_oDocument.get_BrushTextureMode(lMode);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_BrushTextureMode(LONG lMode)
{
	return m_oDocument.put_BrushTextureMode(lMode);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_BrushTextureAlpha(LONG* lTxAlpha)
{
	return m_oDocument.get_BrushTextureAlpha(lTxAlpha);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_BrushTextureAlpha(LONG lTxAlpha)
{
	return m_oDocument.put_BrushTextureAlpha(lTxAlpha);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_BrushLinearAngle(double* dAngle)
{
	return m_oDocument.get_BrushLinearAngle(dAngle);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_BrushLinearAngle(double dAngle)
{
	return m_oDocument.put_BrushLinearAngle(dAngle);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::BrushRect(BOOL val, double left, double top, double width, double height)
{
	return m_oDocument.BrushRect(val, left, top, width, height);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::SetFont(BSTR bsXML)
{
	return m_oDocument.SetFont(bsXML);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_FontName(BSTR* bsName)
{
	return m_oDocument.get_FontName(bsName);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_FontName(BSTR bsName)
{
	return m_oDocument.put_FontName(bsName);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_FontPath(BSTR* bsName)
{
	return m_oDocument.get_FontPath(bsName);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_FontPath(BSTR bsName)
{
	return m_oDocument.put_FontPath(bsName);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_FontSize(double* dSize)
{
	return m_oDocument.get_FontSize(dSize);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_FontSize(double dSize)
{
	return m_oDocument.put_FontSize(dSize);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_FontStyle(LONG* lStyle)
{
	return m_oDocument.get_FontStyle(lStyle);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_FontStyle(LONG lStyle)
{
	return m_oDocument.put_FontStyle(lStyle);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_FontStringGID(BOOL* bGID)
{
	return m_oDocument.get_FontStringGID(bGID);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_FontStringGID(BOOL bGID)
{
	return m_oDocument.put_FontStringGID(bGID);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_FontCharSpace(double* dSpace)
{
	return m_oDocument.get_FontCharSpace(dSpace);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_FontCharSpace(double dSpace)
{
	return m_oDocument.put_FontCharSpace(dSpace);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::SetShadow(BSTR bsXML)
{
	return m_oDocument.SetShadow(bsXML);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_ShadowDistanceX(double* val)
{
	return m_oDocument.get_ShadowDistanceX(val);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_ShadowDistanceX(double val)
{
	return m_oDocument.put_ShadowDistanceX(val);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_ShadowDistanceY(double* val)
{
	return m_oDocument.get_ShadowDistanceY(val);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_ShadowDistanceY(double val)
{
	return m_oDocument.put_ShadowDistanceY(val);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_ShadowBlurSize(double* val)
{
	return m_oDocument.get_ShadowBlurSize(val);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_ShadowBlurSize(double val)
{
	return m_oDocument.put_ShadowBlurSize(val);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_ShadowColor(LONG* val)
{
	return m_oDocument.get_ShadowColor(val);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_ShadowColor(LONG val)
{
	return m_oDocument.put_ShadowColor(val);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_ShadowAlpha(LONG* val)
{
	return m_oDocument.get_ShadowAlpha(val);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_ShadowAlpha(LONG val)
{
	return m_oDocument.put_ShadowAlpha(val);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_ShadowVisible(BOOL* val)
{
	return m_oDocument.get_ShadowVisible(val);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_ShadowVisible(BOOL val)
{
	return m_oDocument.put_ShadowVisible(val);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::SetEdgeText(BSTR bsXML)
{
	return m_oDocument.SetEdgeText(bsXML);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_EdgeVisible(LONG* val)
{
	return m_oDocument.get_EdgeVisible(val);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_EdgeVisible(LONG val)
{
	return m_oDocument.put_EdgeVisible(val);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_EdgeColor(LONG* val)
{
	return m_oDocument.get_EdgeColor(val);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_EdgeColor(LONG val)
{
	return m_oDocument.put_EdgeColor(val);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_EdgeAlpha(LONG* val)
{
	return m_oDocument.get_EdgeAlpha(val);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_EdgeAlpha(LONG val)
{
	return m_oDocument.put_EdgeAlpha(val);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_EdgeDist(double* val)
{
	return m_oDocument.get_EdgeDist(val);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_EdgeDist(double val)
{
	return m_oDocument.put_EdgeDist(val);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::CommandDrawText(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
{
	return m_oDocument.CommandDrawText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::CommandDrawTextEx(BSTR bsUnicodeText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
{
	return m_oDocument.CommandDrawTextEx(bsUnicodeText, bsGidText, bsSourceCodeText, fX, fY, fWidth, fHeight, fBaseLineOffset, lFlags);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::BeginCommand(DWORD lType)
{
	return m_oDocument.BeginCommand(lType);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::EndCommand(DWORD lType)
{
	return m_oDocument.EndCommand(lType);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::PathCommandMoveTo(double fX, double fY)
{
	return m_oDocument.PathCommandMoveTo(fX, fY);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::PathCommandLineTo(double fX, double fY)
{
	return m_oDocument.PathCommandLineTo(fX, fY);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::PathCommandLinesTo(SAFEARRAY* pPoints)
{
	return m_oDocument.PathCommandLinesTo(pPoints);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::PathCommandCurveTo(double fX1, double fY1, double fX2, double fY2, double fX3, double fY3)
{
	return m_oDocument.PathCommandCurveTo(fX1, fY1, fX2, fY2, fX3, fY3);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::PathCommandCurvesTo(SAFEARRAY* pPoints)
{
	return m_oDocument.PathCommandCurvesTo(pPoints);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::PathCommandArcTo(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle)
{
	return m_oDocument.PathCommandArcTo(fX, fY, fWidth, fHeight, fStartAngle, fSweepAngle);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::PathCommandClose()
{
	return m_oDocument.PathCommandClose();
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::PathCommandEnd()
{
	return m_oDocument.PathCommandEnd();
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::DrawPath(long nType)
{
	return m_oDocument.DrawPath(nType);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::PathCommandStart()
{
	return m_oDocument.PathCommandStart();
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::PathCommandGetCurrentPoint(double* fX, double* fY)
{
	return m_oDocument.PathCommandGetCurrentPoint( fX, fY );
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::GetCommandParams(double* dAngle, double* dLeft, double* dTop, double* dWidth, double* dHeight, DWORD* lFlags)
{
	return m_oDocument.GetCommandParams( dAngle, dLeft, dTop, dWidth, dHeight, lFlags );
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::SetCommandParams(double dAngle, double dLeft, double dTop, double dWidth, double dHeight, DWORD lFlags)
{
	return m_oDocument.SetCommandParams( dAngle, dLeft, dTop, dWidth, dHeight, lFlags );
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::DrawImage(IUnknown* pInterface, double fX, double fY, double fWidth, double fHeight)
{
	return m_oDocument.DrawImage( pInterface, fX, fY, fWidth, fHeight);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::DrawImageFromFile(BSTR bstrVal, double fX, double fY, double fWidth, double fHeight)
{
	return m_oDocument.DrawImageFromFile( bstrVal, fX, fY, fWidth, fHeight);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::SetAdditionalParam(BSTR ParamName, VARIANT ParamValue)
{
	CString sParamName = ParamName;
	if ( _T("SourceRendererType") == sParamName && VT_I4 == ParamValue.vt )
	{
		m_oDocument.m_bIsNeedPDFTextAnalyzer = ( ParamValue.lVal == c_nPDFWriter ? true : false );
	}
	return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::GetAdditionalParam(BSTR ParamName, VARIANT* ParamValue)
{
	return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::SetTransform(double dA, double dB, double dC, double dD, double dE, double dF)
{
	return m_oDocument.SetTransform( dA, dB, dC, dD, dE, dF );
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::GetTransform(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF)
{
	return m_oDocument.GetTransform( pdA, pdB, pdC, pdD, pdE, pdF );
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::ResetTransform(void)
{
	return m_oDocument.ResetTransform();
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::get_ClipMode(LONG* plMode)
{
	return m_oDocument.get_ClipMode(plMode);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::put_ClipMode(LONG lMode)
{
	return m_oDocument.put_ClipMode(lMode);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::PathCommandText(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
{
	return m_oDocument.PathCommandText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::PathCommandTextEx(BSTR bsUnicodeText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
{
	return m_oDocument.PathCommandTextEx( bsUnicodeText, bsGidText, bsSourceCodeText, fX, fY, fWidth, fHeight, fBaseLineOffset, lFlags );
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::Initialize(BSTR bsXMLOptions)
{
	BaseInitialize(bsXMLOptions);

	IUnknown* punkRenderer = NULL;
	this->QueryInterface(IID_IUnknown, (void**)&punkRenderer);

	m_oDocument.CreateDocument(punkRenderer, m_strTempFileDir);

	RELEASEINTERFACE(punkRenderer);

	return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::Save()
{
	m_oDocument.Close();
	return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::CreateOfficeFile(BSTR bsXMLOptions)
{
	BaseInitialize(bsXMLOptions);

	IUnknown* punkRenderer = NULL;
	this->QueryInterface(IID_IUnknown, (void**)&punkRenderer);

	m_oDocument.CreateDocument(punkRenderer, m_strTempFileDir);

	RELEASEINTERFACE(punkRenderer);

	return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP CAVSDocxRenderer::CloseFile()
{
	m_oDocument.Close();
	return S_OK;
}

/*========================================================================================================*/