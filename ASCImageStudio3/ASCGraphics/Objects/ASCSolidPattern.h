// CASCSolidPattern.h : Declaration of the CAVSSolidPattern

#pragma once
#include "..\stdafx.h"
#include "..\resource.h"       // main symbols
#include "Registration.h"

#include "..\..\..\Common\ASCUtils.h"
#include "..\Interfaces\IASCPattern.h"

// CAVSPen
[coclass, uuid("71441146-654B-4c7e-9568-69BE27CA192D"), threading(apartment), vi_progid("AVSGraphics.SolidPattern"), progid("AVSGraphics.SolidPattern"), version(1.0), support_error_info(IAVSPattern), registration_script("control.rgs")]
class ATL_NO_VTABLE CAVSSolidPattern
	:	public IAVSPattern
{
protected:
	LONG m_lColor;

public:

	CAVSSolidPattern()
	{
		m_lColor = 0xFF000000;
	}

	~CAVSSolidPattern()
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
		if (SUCCEEDED(CoCreateInstance(__uuidof(CAVSSolidPattern), NULL, CLSCTX_INPROC, __uuidof(IAVSGraphicsBase), (void**)ppGraphicsBase)))
		{
			((CAVSSolidPattern*)(*ppGraphicsBase))->m_lColor = m_lColor;
			return S_OK;
		}
		return S_FALSE;
	}

	STDMETHOD(get_Type)(LONG* lVal)
	{
		if (NULL == lVal)
			return S_FALSE;

		*lVal = 1;
		return S_OK;
	}

	STDMETHOD(get_Width)(LONG* lVal)
	{
		if (NULL == lVal)
			return S_FALSE;

		*lVal = 0;
		return S_OK;
	}
	STDMETHOD(get_Height)(LONG* lVal)
	{
		if (NULL == lVal)
			return S_FALSE;

		*lVal = 0;
		return S_OK;
	}
	STDMETHOD(GetPixel)(LONG lX, LONG lY, LONG* pColor)
	{
		*pColor = m_lColor;
		return S_OK;
	}
	STDMETHOD(SetColor)(LONG lColor)
	{
		m_lColor = lColor;
		return S_OK;
	}
};