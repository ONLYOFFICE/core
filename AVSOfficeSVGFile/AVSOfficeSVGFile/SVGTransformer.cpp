#include "stdafx.h"

#include "SVGTransformer.h"

HRESULT SVGTransformer::LoadFile (BSTR bFile)
{
	m_oStorage.Clear();

	if (0 == m_oParser.LoadFromFile (CString (bFile), &m_oStorage))
		return AVS_ERROR_FILEFORMAT;

	return S_OK;
}

HRESULT SVGTransformer::Load (BSTR bStr)
{
	m_oStorage.Clear();

	CString strXml (bStr);
	if (strXml.GetLength())
	{
		StringHelpers::RemoveTagHTML (strXml, CString(L"<!ENTITY"), CString(L">"));
		StringHelpers::RemoveTagHTML (strXml, CString(L"<!DOCTYPE"), CString(L">"));
		StringHelpers::RemoveComments (strXml);
	}

	if (0 == m_oParser.LoadFromString (strXml, &m_oStorage))
		return AVS_ERROR_FILEFORMAT;

	return S_OK;
}

HRESULT SVGTransformer::Draw ( IUnknown* pUnk, double dX, double dY, double dWidth, double dHeight )
{
	if ( NULL == pUnk )
		return S_FALSE;

	IAVSRenderer* render = NULL;
	pUnk->QueryInterface ( &render );
	pUnk->Release ();

	if ( NULL == render )
		return S_FALSE;

	m_oRender.Draw ( &m_oStorage, render, m_oParser.GetUnitSystem () );

	return S_OK;
}

HRESULT SVGTransformer::get_Width ( LONG* Width )
{
	*Width	=	 m_oParser.GetWidth();

	return S_OK;
}

HRESULT SVGTransformer::get_Height ( LONG* Height )
{
	*Height	=	 m_oParser.GetHeight();

	return S_OK;
}

HRESULT SVGTransformer::get_Metrics ( LONG* Metrics )
{
	*Metrics	=	m_oParser.GetMetics();

	return S_OK;
}

HRESULT SVGTransformer::SetAdditionalParam ( BSTR ParamName, VARIANT ParamValue)
{
	return S_OK;
}

HRESULT SVGTransformer::GetAdditionalParam ( BSTR ParamName, VARIANT* ParamValue)
{
	return S_OK;
}

