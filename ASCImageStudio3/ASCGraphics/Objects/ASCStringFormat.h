// CASCStringFormat.h : Declaration of the CAVSStringFormat

#pragma once
#include "stdafx.h"
#include "resource.h"       // main symbols
#include "Registration.h"

#include "..\Interfaces\IASCStringFormat.h"

// CAVSStringFormat
[coclass, uuid("217E68AB-9332-4ef5-B2A6-CE41C60DCE37"), threading(apartment), vi_progid("AVSGraphics.StringFormat"), progid("AVSGraphics.StringFormat"), version(1.0), support_error_info(IAVSStringFormat), registration_script("control.rgs")]
class ATL_NO_VTABLE CAVSStringFormat
	:	public IAVSStringFormat
{
protected:
	LONG m_lAlignmentH;
	LONG m_lAlignmentV;

public:

	CAVSStringFormat()
	{
		m_lAlignmentH = 1;
		m_lAlignmentV = 1;
	}

	~CAVSStringFormat()
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
		if (SUCCEEDED(CoCreateInstance(__uuidof(CAVSStringFormat), NULL, CLSCTX_INPROC, __uuidof(IAVSGraphicsBase), (void**)ppGraphicsBase)))
		{
			((CAVSStringFormat*)(*ppGraphicsBase))->m_lAlignmentH = m_lAlignmentH;
			((CAVSStringFormat*)(*ppGraphicsBase))->m_lAlignmentV = m_lAlignmentV;
			return S_OK;
		}
		return S_FALSE;
	}
	STDMETHOD(get_AlignmentH)(LONG* lVal)
	{
		if (NULL == lVal)
			return S_FALSE;

		*lVal = m_lAlignmentH;
		return S_OK;
	}
	STDMETHOD(put_AlignmentH)(LONG lVal)
	{
		m_lAlignmentH = lVal;
		return S_OK;
	}

	STDMETHOD(get_AlignmentV)(LONG* lVal)
	{
		if (NULL == lVal)
			return S_FALSE;

		*lVal = m_lAlignmentV;
		return S_OK;
	}
	STDMETHOD(put_AlignmentV)(LONG lVal)
	{
		m_lAlignmentV = lVal;
		return S_OK;
	}
};