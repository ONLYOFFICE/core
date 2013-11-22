// CAVSPen.h : Declaration of the CAVSPen

#pragma once
#include "stdafx.h"
#include "resource.h"       // main symbols
#include "Registration.h"

#include "Structures.h"
#include "..\Interfaces\IAVSPen.h"

// CAVSPen
[coclass, uuid("FBBDE648-32C0-4b9c-94BA-FB917511B36D"), threading(apartment), vi_progid("AVSGraphics.Pen"), progid("AVSGraphics.Pen"), version(1.0), support_error_info(IAVSGraphics), registration_script("control.rgs")]
class ATL_NO_VTABLE CAVSPen
	:	public IAVSPen
{
	friend class CAVSMetafile;

protected:
	NSStructures::CPen m_oPen;

public:

	CAVSPen() : m_oPen()
	{
	}

	~CAVSPen()
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
		if (SUCCEEDED(CoCreateInstance(__uuidof(CAVSPen), NULL, CLSCTX_INPROC, __uuidof(IAVSGraphicsBase), (void**)ppGraphicsBase)))
		{
			((CAVSPen*)(*ppGraphicsBase))->m_oPen = m_oPen;
			return S_OK;
		}
		return S_FALSE;
	}

	STDMETHOD(get_Xml)(BSTR* bstrXml)
	{
		if (NULL == bstrXml)
			return S_FALSE;

		*bstrXml = m_oPen.ToXmlString().AllocSysString();
		return S_OK;
	}
	STDMETHOD(put_Xml)(BSTR bstrXml)
	{
		if (NULL == bstrXml)
			return S_FALSE;

		m_oPen.FromXmlString(CString(bstrXml));
		return S_OK;
	}

	STDMETHOD(get_Color)(LONG* lVal)
	{
		if (NULL == lVal)
			return S_FALSE;

		*lVal = m_oPen.Color;
		return S_OK;
	}
	STDMETHOD(put_Color)(LONG lVal)
	{
		m_oPen.Color = lVal;
		return S_OK;
	}
	STDMETHOD(get_Alpha)(LONG* lVal)
	{
		if (NULL == lVal)
			return S_FALSE;

		*lVal = m_oPen.Alpha;
		return S_OK;
	}
	STDMETHOD(put_Alpha)(LONG lVal)
	{
		m_oPen.Alpha = lVal;
		return S_OK;
	}
	STDMETHOD(get_Size)(double* lVal)
	{
		if (NULL == lVal)
			return S_FALSE;

		*lVal = m_oPen.Size;
		return S_OK;
	}
	STDMETHOD(put_Size)(double lVal)
	{
		m_oPen.Size = lVal;
		return S_OK;
	}

	STDMETHOD(get_DashStyle)(LONG* lVal)
	{
		if (NULL == lVal)
			return S_FALSE;

		*lVal = (LONG)m_oPen.DashStyle;
		return S_OK;
	}
	STDMETHOD(put_DashStyle)(LONG lVal)
	{
		m_oPen.DashStyle = (byte)lVal;
		return S_OK;
	}

	STDMETHOD(get_DashOffset)(double* dOffset)
	{
		if (NULL == dOffset)
			return S_FALSE;

		*dOffset = (double)m_oPen.DashOffset;
		return S_OK;
	}
	STDMETHOD(put_DashOffset)(double dOffset)
	{
		m_oPen.DashOffset = (float)dOffset;
		return S_OK;
	}

	STDMETHOD(get_DashPattern)(LPSAFEARRAY* lpArray)
	{
		if (NULL == m_oPen.DashPattern)
			return S_FALSE;

		SAFEARRAYBOUND rgsabTypes[1];
		rgsabTypes[0].lLbound = 0;
		rgsabTypes[0].cElements = m_oPen.Count;
		(*lpArray) = SafeArrayCreate(VT_R4, 1, rgsabTypes);
		memcpy((*lpArray)->pvData, m_oPen.DashPattern, sizeof(Gdiplus::REAL) * rgsabTypes[0].cElements);
		return S_OK;
	}
	STDMETHOD(put_DashPattern)(LPSAFEARRAY lpArray)
	{
		LONG lCount = lpArray->rgsabound[0].cElements;
		double* pData = (double*)lpArray->pvData;

		m_oPen.SetDashPattern(pData, lCount);

		return S_OK;
	}

	STDMETHOD(get_DashCap)(LONG* lDashCap)
	{
		return S_OK;
	}
	STDMETHOD(put_DashCap)(LONG lDashCap)
	{
		return S_OK;
	}

	STDMETHOD(get_LineStartCap)(LONG* lVal)
	{
		if (NULL == lVal)
			return S_FALSE;

		*lVal = m_oPen.LineStartCap;
		return S_OK;
	}
	STDMETHOD(put_LineStartCap)(LONG lVal)
	{
		m_oPen.LineStartCap = lVal;
		return S_OK;
	}
	STDMETHOD(get_LineEndCap)(LONG* lVal)
	{
		if (NULL == lVal)
			return S_FALSE;

		*lVal = m_oPen.LineEndCap;
		return S_OK;
	}
	STDMETHOD(put_LineEndCap)(LONG lVal)
	{
		m_oPen.LineEndCap = lVal;
		return S_OK;
	}
	STDMETHOD(get_LineJoin)(LONG* lVal)
	{
		if (NULL == lVal)
			return S_FALSE;

		*lVal = m_oPen.LineJoin;
		return S_OK;
	}
	STDMETHOD(put_LineJoin)(LONG lVal)
	{
		m_oPen.LineJoin = lVal;
		return S_OK;
	}
	STDMETHOD(get_LineMiterLimit)(LONG* lVal)
	{
		return S_OK;
	}
	STDMETHOD(put_LineMiterLimit)(LONG lVal)
	{
		return S_OK;
	}
	STDMETHOD(get_Alignment)(LONG* lVal)
	{
		if (NULL == lVal)
			return S_FALSE;

		*lVal = m_oPen.Align;
		return S_OK;
	}
	STDMETHOD(put_Alignment)(LONG lVal)
	{
		m_oPen.Align = lVal;
		return S_OK;
	}

	STDMETHOD(SetLineCap)(LONG lStartCap, LONG lEndCap, LONG lDashCap)
	{
		m_oPen.LineStartCap = lStartCap;
		m_oPen.LineEndCap	= lEndCap;
		return S_OK;
	}
};