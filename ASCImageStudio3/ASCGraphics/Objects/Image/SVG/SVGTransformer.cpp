#include "stdafx.h"

#include "SVGTransformer.h"

HRESULT SVGTransformer::LoadFile (BSTR bstrFile)
{
	m_oStorage.Clear();

	m_oXmlParser.SetWorkingDirectory(FileUtils::FilePath(CString(bstrFile)));
	m_oRender.SetWorkingDirectory(FileUtils::FilePath(CString(bstrFile)));
	m_oStorage.SetWorkingDirectory(FileUtils::FilePath(CString(bstrFile)));

	if (0 == m_oXmlParser.LoadFromFile (CString(bstrFile), &m_oStorage))
		return AVS_ERROR_FILEFORMAT;

	return S_OK;
}

HRESULT SVGTransformer::Load (BSTR bStr)
{
	m_oStorage.Clear();

	CString strXml (bStr);
	if (0 == m_oXmlParser.LoadFromString (strXml, &m_oStorage))
		return AVS_ERROR_FILEFORMAT;

	return S_OK;
}

HRESULT SVGTransformer::Draw (IUnknown* pUnk, double dX, double dY, double dWidth, double dHeight)
{
	if (NULL == pUnk)
		return S_FALSE;

	IASCRenderer* render = NULL;
	pUnk->QueryInterface (&render);
	pUnk->Release();

	if (NULL == render)
		return S_FALSE;

	m_oRender.SetCSS(m_oXmlParser.GetCSS());
	m_oRender.Draw(&m_oStorage, render, m_oXmlParser.GetUnitSystem());
	return S_OK;
}

HRESULT SVGTransformer::get_Width (LONG* pWidth)
{
	*pWidth = m_oXmlParser.GetWidth();

	return S_OK;
}

HRESULT SVGTransformer::get_Height (LONG* pHeight)
{
	*pHeight = m_oXmlParser.GetHeight();

	return S_OK;
}

HRESULT SVGTransformer::get_Metrics (LONG* pMetrics)
{
	*pMetrics = m_oXmlParser.GetMetics();

	return S_OK;
}

HRESULT SVGTransformer::SetAdditionalParam (BSTR ParamName, VARIANT ParamValue)
{
	if (CString(L"DefaultWidth") == CComBSTR (ParamName))
	{
		m_oXmlParser.SetDefaultWidth(ParamValue.lVal);
		return S_OK;
	}
	
	if (CString(L"DefaultHeight") == CComBSTR (ParamName))
	{
		m_oXmlParser.SetDefaultHeight(ParamValue.lVal);
		return S_OK;
	}

	if (CString (L"WorkingDirectory") == ParamName)
	{
		m_oXmlParser.SetWorkingDirectory((CString)ParamValue.bstrVal);
		m_oRender.SetWorkingDirectory((CString)ParamValue.bstrVal);
		m_oStorage.SetWorkingDirectory((CString)ParamValue.bstrVal);

		return S_OK;
	}

	return S_OK;
}

HRESULT SVGTransformer::GetAdditionalParam (BSTR ParamName, VARIANT* ParamValue)
{
	return S_OK;
}