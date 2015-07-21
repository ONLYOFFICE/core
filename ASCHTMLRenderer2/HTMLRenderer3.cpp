#include "stdafx.h"
#include "HTMLRenderer3.h"
#include "Resource.h"

#include "../DesktopEditor/graphics/Image.h"

HRESULT CASCHTMLRenderer3::FinalConstruct()
{
	return S_OK;
}

void CASCHTMLRenderer3::FinalRelease()
{
}

STDMETHODIMP CASCHTMLRenderer3::get_Type(LONG* lType)
{
	return m_oRenderer.get_Type(lType);
}
//-------- Функции для работы со страницей --------------------------------------------------
STDMETHODIMP CASCHTMLRenderer3::NewPage()
{
	return m_oRenderer.NewPage();
}
STDMETHODIMP CASCHTMLRenderer3::get_Height(double* dHeight)
{
	return m_oRenderer.get_Height(dHeight);
}
STDMETHODIMP CASCHTMLRenderer3::put_Height(double dHeight)
{
	return m_oRenderer.put_Height(dHeight);
}
STDMETHODIMP CASCHTMLRenderer3::get_Width(double* dWidth)
{
	return m_oRenderer.get_Width(dWidth);
}
STDMETHODIMP CASCHTMLRenderer3::put_Width(double dWidth)
{
	return m_oRenderer.put_Width(dWidth);
}

STDMETHODIMP CASCHTMLRenderer3::get_DpiX(double* dDpiX)
{
	return m_oRenderer.get_DpiX(dDpiX);
}
STDMETHODIMP CASCHTMLRenderer3::get_DpiY(double* dDpiY)
{
	return m_oRenderer.get_DpiY(dDpiY);
}
// pen --------------------------------------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer3::SetPen(BSTR bsXML)
{	
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_PenColor(LONG* lColor)
{
	return m_oRenderer.get_PenColor(lColor);
}
STDMETHODIMP CASCHTMLRenderer3::put_PenColor(LONG lColor)
{
	return m_oRenderer.put_PenColor(lColor);
}
STDMETHODIMP CASCHTMLRenderer3::get_PenAlpha(LONG* lAlpha)
{
	return m_oRenderer.get_PenAlpha(lAlpha);
}
STDMETHODIMP CASCHTMLRenderer3::put_PenAlpha(LONG lAlpha)
{
	return m_oRenderer.put_PenAlpha(lAlpha);
}
STDMETHODIMP CASCHTMLRenderer3::get_PenSize(double* dSize)
{
	return m_oRenderer.get_PenSize(dSize);
}
STDMETHODIMP CASCHTMLRenderer3::put_PenSize(double dSize)
{
	return m_oRenderer.put_PenSize(dSize);
}
STDMETHODIMP CASCHTMLRenderer3::get_PenDashStyle(BYTE* val)
{
	return m_oRenderer.get_PenDashStyle(val);
}
STDMETHODIMP CASCHTMLRenderer3::put_PenDashStyle(BYTE val)
{
	return m_oRenderer.put_PenDashStyle(val);
}
STDMETHODIMP CASCHTMLRenderer3::get_PenLineStartCap(BYTE* val)
{
	return m_oRenderer.get_PenLineStartCap(val);
}
STDMETHODIMP CASCHTMLRenderer3::put_PenLineStartCap(BYTE val)
{
	return m_oRenderer.put_PenLineStartCap(val);
}
STDMETHODIMP CASCHTMLRenderer3::get_PenLineEndCap(BYTE* val)
{
	return m_oRenderer.get_PenLineEndCap(val);
}
STDMETHODIMP CASCHTMLRenderer3::put_PenLineEndCap(BYTE val)
{
	return m_oRenderer.put_PenLineEndCap(val);
}
STDMETHODIMP CASCHTMLRenderer3::get_PenLineJoin(BYTE* val)
{
	return m_oRenderer.get_PenLineJoin(val);
}
STDMETHODIMP CASCHTMLRenderer3::put_PenLineJoin(BYTE val)
{
	return m_oRenderer.put_PenLineJoin(val);
}
STDMETHODIMP CASCHTMLRenderer3::get_PenDashOffset(double* val)
{
	return m_oRenderer.get_PenDashOffset(val);
}
STDMETHODIMP CASCHTMLRenderer3::put_PenDashOffset(double val)
{
	return m_oRenderer.put_PenDashOffset(val);
}
STDMETHODIMP CASCHTMLRenderer3::get_PenAlign(LONG* val)
{
	return m_oRenderer.get_PenAlign(val);
}
STDMETHODIMP CASCHTMLRenderer3::put_PenAlign(LONG val)
{
	return m_oRenderer.put_PenAlign(val);
}
STDMETHODIMP CASCHTMLRenderer3::get_PenMiterLimit(double* val)
{
	return m_oRenderer.get_PenMiterLimit(val);
}
STDMETHODIMP CASCHTMLRenderer3::put_PenMiterLimit(double val)
{
	return m_oRenderer.put_PenMiterLimit(val);
}
STDMETHODIMP CASCHTMLRenderer3::PenDashPattern(SAFEARRAY* pPattern)
{
	if (NULL != pPattern)
	{
		return m_oRenderer.PenDashPattern((double*)pPattern->pvData, (LONG)pPattern->rgsabound[0].cElements);
	}
	return S_OK;
}
// brush ------------------------------------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer3::SetBrush(BSTR bsXML)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_BrushType(LONG* lType)
{
	return m_oRenderer.get_BrushType(lType);
}
STDMETHODIMP CASCHTMLRenderer3::put_BrushType(LONG lType)
{
	return m_oRenderer.put_BrushType(lType);
}
STDMETHODIMP CASCHTMLRenderer3::get_BrushColor1(LONG* lColor)
{
	return m_oRenderer.get_BrushColor1(lColor);
}
STDMETHODIMP CASCHTMLRenderer3::put_BrushColor1(LONG lColor)
{
	return m_oRenderer.put_BrushColor1(lColor);
}
STDMETHODIMP CASCHTMLRenderer3::get_BrushAlpha1(LONG* lAlpha)
{
	return m_oRenderer.get_BrushAlpha1(lAlpha);
}
STDMETHODIMP CASCHTMLRenderer3::put_BrushAlpha1(LONG lAlpha)
{
	return m_oRenderer.put_BrushAlpha1(lAlpha);
}
STDMETHODIMP CASCHTMLRenderer3::get_BrushColor2(LONG* lColor)
{
	return m_oRenderer.get_BrushColor2(lColor);
}
STDMETHODIMP CASCHTMLRenderer3::put_BrushColor2(LONG lColor)
{
	return m_oRenderer.put_BrushColor2(lColor);
}
STDMETHODIMP CASCHTMLRenderer3::get_BrushAlpha2(LONG* lAlpha)
{
	return m_oRenderer.get_BrushAlpha2(lAlpha);
}
STDMETHODIMP CASCHTMLRenderer3::put_BrushAlpha2(LONG lAlpha)
{
	return m_oRenderer.put_BrushAlpha2(lAlpha);
}
STDMETHODIMP CASCHTMLRenderer3::get_BrushTexturePath(BSTR* bsPath)
{
	std::wstring sPath;
	m_oRenderer.get_BrushTexturePath(&sPath);
	*bsPath = SysAllocString(sPath.c_str());
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_BrushTexturePath(BSTR bsPath)
{
	return m_oRenderer.put_BrushTexturePath(bsPath);
}
STDMETHODIMP CASCHTMLRenderer3::get_BrushTextureMode(LONG* lMode)
{
	return m_oRenderer.get_BrushTextureMode(lMode);
}
STDMETHODIMP CASCHTMLRenderer3::put_BrushTextureMode(LONG lMode)
{
	return m_oRenderer.put_BrushTextureMode(lMode);
}
STDMETHODIMP CASCHTMLRenderer3::get_BrushTextureAlpha(LONG* lTxAlpha)
{
	return m_oRenderer.get_BrushTextureAlpha(lTxAlpha);
}
STDMETHODIMP CASCHTMLRenderer3::put_BrushTextureAlpha(LONG lTxAlpha)
{
	return m_oRenderer.put_BrushTextureAlpha(lTxAlpha);
}
STDMETHODIMP CASCHTMLRenderer3::get_BrushLinearAngle(double* dAngle)
{
	return m_oRenderer.get_BrushLinearAngle(dAngle);
}
STDMETHODIMP CASCHTMLRenderer3::put_BrushLinearAngle(double dAngle)
{
	return m_oRenderer.put_BrushLinearAngle(dAngle);
}
STDMETHODIMP CASCHTMLRenderer3::BrushRect(BOOL val, double left, double top, double width, double height)
{
	return m_oRenderer.BrushRect(val, left, top, width, height);
}
// font -------------------------------------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer3::SetFont(BSTR bsXML)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_FontName(BSTR* bsName)
{
	std::wstring sName;
	m_oRenderer.get_FontName(&sName);
	*bsName = SysAllocString(sName.c_str());
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_FontName(BSTR bsName)
{
	return m_oRenderer.put_FontName(bsName);
}
STDMETHODIMP CASCHTMLRenderer3::get_FontPath(BSTR* bsPath)
{
	std::wstring sPath;
	m_oRenderer.get_FontPath(&sPath);
	*bsPath = SysAllocString(sPath.c_str());
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_FontPath(BSTR bsName)
{
	return m_oRenderer.put_FontPath(bsName);
}
STDMETHODIMP CASCHTMLRenderer3::get_FontSize(double* dSize)
{
	return m_oRenderer.get_FontSize(dSize);
}
STDMETHODIMP CASCHTMLRenderer3::put_FontSize(double dSize)
{
	return m_oRenderer.put_FontSize(dSize);
}
STDMETHODIMP CASCHTMLRenderer3::get_FontStyle(LONG* lStyle)
{
	return m_oRenderer.get_FontStyle(lStyle);
}
STDMETHODIMP CASCHTMLRenderer3::put_FontStyle(LONG lStyle)
{
	return m_oRenderer.put_FontStyle(lStyle);
}
STDMETHODIMP CASCHTMLRenderer3::get_FontStringGID(BOOL* bGID)
{
	return m_oRenderer.get_FontStringGID(bGID);
}
STDMETHODIMP CASCHTMLRenderer3::put_FontStringGID(BOOL bGID)
{
	return m_oRenderer.put_FontStringGID(bGID);
}
STDMETHODIMP CASCHTMLRenderer3::get_FontCharSpace(double* dSpace)
{
	return m_oRenderer.get_FontCharSpace(dSpace);
}
STDMETHODIMP CASCHTMLRenderer3::put_FontCharSpace(double dSpace)
{
	return m_oRenderer.put_FontCharSpace(dSpace);
}
// shadow -----------------------------------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer3::SetShadow(BSTR bsXML)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_ShadowDistanceX(double* val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_ShadowDistanceX(double val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_ShadowDistanceY(double* val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_ShadowDistanceY(double val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_ShadowBlurSize(double* val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_ShadowBlurSize(double val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_ShadowColor(LONG* val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_ShadowColor(LONG val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_ShadowAlpha(LONG* val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_ShadowAlpha(LONG val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_ShadowVisible(BOOL* val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_ShadowVisible(BOOL val)
{
	return S_OK;
}
// edge -------------------------------------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer3::SetEdgeText(BSTR bsXML)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_EdgeVisible(LONG* val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_EdgeVisible(LONG val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_EdgeColor(LONG* val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_EdgeColor(LONG val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_EdgeAlpha(LONG* val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_EdgeAlpha(LONG val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::get_EdgeDist(double* val)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_EdgeDist(double val)
{
	return S_OK;
}
//-------- Функции для вывода текста --------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer3::CommandDrawText(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
{
	return m_oRenderer.CommandDrawText(bsText, fX, fY + fBaseLineOffset, fWidth, fHeight);
}
STDMETHODIMP CASCHTMLRenderer3::CommandDrawTextEx(BSTR bsText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
{	
	int nGidsLen = 0;
	if (NULL != bsGidText)
		nGidsLen = wcslen(bsGidText);

	unsigned int * pGids = NULL;
	if (nGidsLen != 0)
	{
		pGids = new unsigned int[nGidsLen];

		for (int i = 0; i < nGidsLen; ++i)
			pGids[i] = (int)bsGidText[i];
	}

	m_oRenderer.CommandDrawTextEx(bsText, pGids, (unsigned int)nGidsLen, fX, fY + fBaseLineOffset, fWidth, fHeight);

	RELEASEARRAYOBJECTS(pGids);
	return S_OK;
}
//-------- Маркеры для команд ---------------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer3::BeginCommand(DWORD lType)
{
	return m_oRenderer.BeginCommand(lType);
}
STDMETHODIMP CASCHTMLRenderer3::EndCommand(DWORD lType)
{
	return m_oRenderer.EndCommand(lType);
}
//-------- Функции для работы с Graphics Path -----------------------------------------------
STDMETHODIMP CASCHTMLRenderer3::PathCommandMoveTo(double fX, double fY)
{
	return m_oRenderer.PathCommandMoveTo(fX, fY);
}
STDMETHODIMP CASCHTMLRenderer3::PathCommandLineTo(double fX, double fY)
{
	return m_oRenderer.PathCommandLineTo(fX, fY);
}
STDMETHODIMP CASCHTMLRenderer3::PathCommandLinesTo(SAFEARRAY* pPoints)
{
	return m_oRenderer.PathCommandLinesTo((double*)pPoints->pvData, (LONG)pPoints->rgsabound[0].cElements);	
}
STDMETHODIMP CASCHTMLRenderer3::PathCommandCurveTo(double fX1, double fY1, double fX2, double fY2, double fX3, double fY3)
{
	return m_oRenderer.PathCommandCurveTo(fX1, fY1, fX2, fY2, fX3, fY3);
}
STDMETHODIMP CASCHTMLRenderer3::PathCommandCurvesTo(SAFEARRAY* pPoints)
{
	return m_oRenderer.PathCommandCurvesTo((double*)pPoints->pvData, (LONG)pPoints->rgsabound[0].cElements);
}
STDMETHODIMP CASCHTMLRenderer3::PathCommandArcTo(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle)
{
	return m_oRenderer.PathCommandArcTo(fX, fY, fWidth, fHeight, fStartAngle, fSweepAngle);
}
STDMETHODIMP CASCHTMLRenderer3::PathCommandClose()
{
	return m_oRenderer.PathCommandClose();
}
STDMETHODIMP CASCHTMLRenderer3::PathCommandEnd()
{
	return m_oRenderer.PathCommandEnd();
}
STDMETHODIMP CASCHTMLRenderer3::DrawPath(long nType)
{
	return m_oRenderer.DrawPath(nType);
}
STDMETHODIMP CASCHTMLRenderer3::PathCommandStart()
{
	return m_oRenderer.PathCommandStart();
}
STDMETHODIMP CASCHTMLRenderer3::PathCommandGetCurrentPoint(double* fX, double* fY)
{
	return m_oRenderer.PathCommandGetCurrentPoint(fX, fY);
}
STDMETHODIMP CASCHTMLRenderer3::PathCommandText(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
{
	return m_oRenderer.PathCommandText(bsText, fX, fY + fBaseLineOffset, fWidth, fHeight);
}
STDMETHODIMP CASCHTMLRenderer3::PathCommandTextEx(BSTR bsText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
{	
	int nGidsLen = 0;
	if (NULL != bsGidText)
		nGidsLen = wcslen(bsGidText);

	unsigned int * pGids = NULL;
	if (nGidsLen != 0)
	{
		pGids = new unsigned int[nGidsLen];

		for (int i = 0; i < nGidsLen; ++i)
			pGids[i] = (int)bsGidText[i];
	}

	m_oRenderer.PathCommandTextEx(bsText, pGids, (unsigned int)nGidsLen, fX, fY + fBaseLineOffset, fWidth, fHeight);

	RELEASEARRAYOBJECTS(pGids);
	return S_OK;
}
//-------- Функции для вывода изображений ---------------------------------------------------
STDMETHODIMP CASCHTMLRenderer3::DrawImage(IUnknown* pInterface, double fX, double fY, double fWidth, double fHeight)
{
	MediaCore::IAVSUncompressedVideoFrame* pFrame = NULL;
	pInterface->QueryInterface(MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pFrame);

	BYTE* pData = NULL;
	LONG lWidth = 0;
	LONG lHeight = 0;
	LONG lStride = 0;
	pFrame->get_Buffer(&pData);
	pFrame->get_Width(&lWidth);
	pFrame->get_Height(&lHeight);
	pFrame->get_Stride(0, &lStride);

	Aggplus::CImage oImage;
	oImage.Create(pData, (DWORD)lWidth, (DWORD)lHeight, lStride);

	m_oRenderer.DrawImage(&oImage, fX, fY, fWidth, fHeight);

	oImage.Create(NULL, 0, 0, lStride);
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::DrawImageFromFile(BSTR bstrVal, double fX, double fY, double fWidth, double fHeight)
{
	return m_oRenderer.DrawImageFromFile(bstrVal, fX, fY, fWidth, fHeight);
}
// transform --------------------------------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer3::GetCommandParams(double* dAngle, double* dLeft, double* dTop, double* dWidth, double* dHeight, DWORD* lFlags)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::SetCommandParams(double dAngle, double dLeft, double dTop, double dWidth, double dHeight, DWORD lFlags)
{
	return m_oRenderer.SetCommandParams(dAngle, dLeft, dTop, dWidth, dHeight, lFlags);
}
STDMETHODIMP CASCHTMLRenderer3::SetTransform(double dA, double dB, double dC, double dD, double dE, double dF)
{
	return m_oRenderer.SetTransform(dA, dB, dC, dD, dE, dF);
}
STDMETHODIMP CASCHTMLRenderer3::GetTransform(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::ResetTransform(void)
{
	return m_oRenderer.ResetTransform();
}
// -----------------------------------------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer3::get_ClipMode(LONG* plMode)
{
	return m_oRenderer.get_ClipMode(plMode);
}
STDMETHODIMP CASCHTMLRenderer3::put_ClipMode(LONG lMode)
{
	return m_oRenderer.put_ClipMode(lMode);
}
// additiaonal params ----------------------------------------------------------------------
STDMETHODIMP CASCHTMLRenderer3::SetAdditionalParam(BSTR ParamName, VARIANT ParamValue)
{
	CString strParamName = (CString)ParamName; 
		
	if (_T("SourceType") == strParamName)
	{
		return m_oRenderer.SetAdditionalParam("SourceType", (int)ParamValue.lVal);
	}

	if (_T("TilingHtmlPattern") == strParamName)
	{
		std::wstring sXml(ParamValue.bstrVal);
		return m_oRenderer.SetAdditionalParam("TilingHtmlPattern", sXml);
	}

	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::GetAdditionalParam(BSTR ParamName, VARIANT* ParamValue)
{
	// TODO:
	return S_OK;
}
// --------------------------------------------------------------------------------------------

// file
STDMETHODIMP CASCHTMLRenderer3::CreateOfficeFile(BSTR bsFilePath)
{
	return m_oRenderer.CreateOfficeFile(bsFilePath);
}
STDMETHODIMP CASCHTMLRenderer3::CloseFile()
{
	return m_oRenderer.CloseFile();
}

STDMETHODIMP CASCHTMLRenderer3::get_Mode(LONG* plMode)
{
	return S_OK;
}
STDMETHODIMP CASCHTMLRenderer3::put_Mode(LONG lMode)
{
	return S_OK;
}
