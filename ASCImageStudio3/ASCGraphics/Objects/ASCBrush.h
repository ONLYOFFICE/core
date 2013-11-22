// CASCBrush.h : Declaration of the CAVSBrush

#pragma once
#include "stdafx.h"
#include "resource.h"       // main symbols
#include "Registration.h"

#include "Structures.h"
#include "..\Interfaces\IASCGraphicsPath.h"

// CAVSBrush
[coclass, uuid("76026DDB-9D2A-4462-891C-9BEC13BA049A"), threading(apartment), vi_progid("AVSGraphics.Brush"), progid("AVSGraphics.Brush"), version(1.0), support_error_info(IAVSBrush), registration_script("control.rgs")]
class ATL_NO_VTABLE CAVSBrush
	:	public IAVSBrush
{
	friend class CAVSMetafile;

protected:
	NSStructures::CBrush m_oBrush;

public:

	CAVSBrush() : m_oBrush()
	{
	}

	~CAVSBrush()
	{
	}

public:
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		return S_OK;
	}
	STDMETHOD(CreateDublicate)(IAVSGraphicsBase** ppGraphicsBase)
	{
		if (NULL == ppGraphicsBase)
			return S_FALSE;

		RELEASEINTERFACE((*ppGraphicsBase));
		if (SUCCEEDED(CoCreateInstance(__uuidof(CAVSBrush), NULL, CLSCTX_INPROC, __uuidof(IAVSGraphicsBase), (void**)ppGraphicsBase)))
		{
			((CAVSBrush*)(*ppGraphicsBase))->m_oBrush = m_oBrush;
			return S_OK;
		}
		return S_FALSE;
	}

	STDMETHOD(get_Xml)(BSTR* bstrXml)
	{
		if (NULL == bstrXml)
			return S_FALSE;

		*bstrXml = m_oBrush.ToXmlString().AllocSysString();
		return S_OK;
	}
	STDMETHOD(put_Xml)(BSTR bstrXml)
	{
		if (NULL == bstrXml)
			return S_FALSE;

		m_oBrush.FromXmlString(CString(bstrXml));
		return S_OK;
	}

	STDMETHOD(get_Color1)(LONG* lVal)
	{
		if (NULL == lVal)
			return S_FALSE;

		*lVal = m_oBrush.Color1;
		return S_OK;
	}
	STDMETHOD(put_Color1)(LONG lVal)
	{
		m_oBrush.Color1 = lVal;
		return S_OK;
	}
	STDMETHOD(get_Color2)(LONG* lVal)
	{
		if (NULL == lVal)
			return S_FALSE;

		*lVal = m_oBrush.Color2;
		return S_OK;
	}
	STDMETHOD(put_Color2)(LONG lVal)
	{
		m_oBrush.Color2 = lVal;
		return S_OK;
	}

	STDMETHOD(get_Alpha1)(LONG* lVal)
	{
		if (NULL == lVal)
			return S_FALSE;

		*lVal = m_oBrush.Alpha1;
		return S_OK;
	}
	STDMETHOD(put_Alpha1)(LONG lVal)
	{
		m_oBrush.Alpha1 = lVal;
		return S_OK;
	}
	STDMETHOD(get_Alpha2)(LONG* lVal)
	{
		if (NULL == lVal)
			return S_FALSE;

		*lVal = m_oBrush.Alpha2;
		return S_OK;
	}
	STDMETHOD(put_Alpha2)(LONG lVal)
	{
		m_oBrush.Alpha2 = lVal;
		return S_OK;
	}

	STDMETHOD(get_Type)(LONG* lVal)
	{
		if (NULL == lVal)
			return S_FALSE;

		*lVal = m_oBrush.Type;
		return S_OK;
	}
	STDMETHOD(put_Type)(LONG lVal)
	{
		m_oBrush.Type = lVal;
		return S_OK;
	}
	STDMETHOD(get_TextureAlpha)(LONG* lVal)
	{
		if (NULL == lVal)
			return S_FALSE;

		*lVal = m_oBrush.TextureAlpha;
		return S_OK;
	}
	STDMETHOD(put_TextureAlpha)(LONG lVal)
	{
		m_oBrush.TextureAlpha = lVal;
		return S_OK;
	}
	STDMETHOD(get_TextureMode)(LONG* lVal)
	{
		if (NULL == lVal)
			return S_FALSE;

		*lVal = m_oBrush.TextureMode;
		return S_OK;
	}
	STDMETHOD(put_TextureMode)(LONG lVal)
	{
		m_oBrush.TextureMode = lVal;
		return S_OK;
	}

	STDMETHOD(get_TexturePath)(BSTR* val)
	{
		if (NULL == val)
			return S_FALSE;

		*val = m_oBrush.TexturePath.AllocSysString();
		return S_OK;
	}
	STDMETHOD(put_TexturePath)(BSTR val)
	{
		m_oBrush.TexturePath = (CString)val;
		return S_OK;
	}

	STDMETHOD(SetRect)(double dLeft, double dTop, double dRight, double dBottom)
	{
		m_oBrush.Rect.X			= (Gdiplus::REAL)dLeft;
		m_oBrush.Rect.Y			= (Gdiplus::REAL)dTop;
		m_oBrush.Rect.Width		= (Gdiplus::REAL)(dRight - dLeft);
		m_oBrush.Rect.Height	= (Gdiplus::REAL)(dBottom - dTop);

		return S_OK;
	}
	STDMETHOD(GetRect)(double* dLeft, double* dTop, double* dRight, double* dBottom)
	{
		if (NULL == dLeft || NULL == dTop || NULL == dRight || NULL == dBottom)
			return S_FALSE;

		*dLeft		= (double)m_oBrush.Rect.X;
		*dTop		= (double)m_oBrush.Rect.Y;
		*dRight		= (double)(m_oBrush.Rect.X + m_oBrush.Rect.Width);
		*dBottom	= (double)(m_oBrush.Rect.Y + m_oBrush.Rect.Height);

		return S_OK;
	}
};