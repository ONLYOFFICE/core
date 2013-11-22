// CAVSShadow.h : Declaration of the CAVSShadow

#pragma once
#include "stdafx.h"
#include "resource.h"       // main symbols
#include "Registration.h"

#include "Structures.h"
#include "..\Interfaces\IAVSShadow.h"

#include "AVSPen.h"
#include "AVSBrush.h"
#include "AVSGraphicsPath.h"

// CAVSShadow
[coclass, uuid("46E93E51-68B8-4a36-AFFA-F3D8F67C101B"), threading(apartment), vi_progid("AVSGraphics.Shadow"), progid("AVSGraphics.Shadow"), version(1.0), support_error_info(IAVSShadow), registration_script("control.rgs")]
class ATL_NO_VTABLE CAVSShadow
	:	public IAVSShadow
{
	friend class CAVSMetafile;

protected:
	NSStructures::CShadow m_oShadow;

public:

	CAVSShadow() : m_oShadow()
	{
	}

	~CAVSShadow()
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
		if (SUCCEEDED(CoCreateInstance(__uuidof(CAVSShadow), NULL, CLSCTX_INPROC, __uuidof(IAVSGraphicsBase), (void**)ppGraphicsBase)))
		{
			((CAVSShadow*)(*ppGraphicsBase))->m_oShadow = m_oShadow;
			return S_OK;
		}
		return S_FALSE;
	}

	STDMETHOD(get_Xml)(BSTR* bstrXml)
	{
		if (NULL == bstrXml)
			return S_FALSE;

		*bstrXml = m_oShadow.ToXmlString().AllocSysString();
		return S_OK;
	}
	STDMETHOD(put_Xml)(BSTR bstrXml)
	{
		if (NULL == bstrXml)
			return S_FALSE;

		m_oShadow.FromXmlString(CString(bstrXml));
		return S_OK;
	}

	STDMETHOD(get_Visible)(VARIANT_BOOL* lVal)
	{
		if (NULL == lVal)
			return S_FALSE;

		*lVal = m_oShadow.Visible;
		return S_OK;
	}
	STDMETHOD(put_Visible)(VARIANT_BOOL lVal)
	{
		m_oShadow.Visible = lVal;
		return S_OK;
	}
	STDMETHOD(get_Color)(LONG* lVal)
	{
		if (NULL == lVal)
			return S_FALSE;

		*lVal = m_oShadow.Color;
		return S_OK;
	}
	STDMETHOD(put_Color)(LONG lVal)
	{
		m_oShadow.Color = lVal;
		return S_OK;
	}

	STDMETHOD(get_Alpha)(LONG* lVal)
	{
		if (NULL == lVal)
			return S_FALSE;

		*lVal = m_oShadow.Alpha;
		return S_OK;
	}
	STDMETHOD(put_Alpha)(LONG lVal)
	{
		m_oShadow.Alpha = lVal;
		return S_OK;
	}
	STDMETHOD(get_DistanceX)(double* lVal)
	{
		if (NULL == lVal)
			return S_FALSE;

		*lVal = m_oShadow.DistanceX;
		return S_OK;
	}
	STDMETHOD(put_DistanceX)(double lVal)
	{
		m_oShadow.DistanceX = lVal;
		return S_OK;
	}
	STDMETHOD(get_DistanceY)(double* lVal)
	{
		if (NULL == lVal)
			return S_FALSE;

		*lVal = m_oShadow.DistanceY;
		return S_OK;
	}
	STDMETHOD(put_DistanceY)(double lVal)
	{
		m_oShadow.DistanceY = lVal;
		return S_OK;
	}

	STDMETHOD(get_BlurSize)(double* lVal)
	{
		if (NULL == lVal)
			return S_FALSE;

		*lVal = m_oShadow.BlurSize;
		return S_OK;
	}
	STDMETHOD(put_BlurSize)(double lVal)
	{
		m_oShadow.BlurSize = lVal;
		return S_OK;
	}

	STDMETHOD(DrawText)(IAVSGraphics* pGraphics, IAVSFontManager* pFont, BSTR bsText, double dLeft, double dTop, double dRight, double dBottom)
	{
		if ((FALSE == m_oShadow.Visible) || NULL == pGraphics || NULL == pFont)
			return S_OK;

		double x = dLeft;
		double y = dTop;
		double w = dRight - dLeft;
		double h = dBottom - dTop;
		
		x += m_oShadow.DistanceX;
		y += m_oShadow.DistanceY;

		double dBlurSize = m_oShadow.BlurSize;
		
		if (dBlurSize) 
		{
			IAVSBrush* pBrush = NULL;
			CoCreateInstance(__uuidof(CAVSBrush), NULL, CLSCTX_INPROC, __uuidof(IAVSBrush), (void**)&pBrush);

			if (NULL == pBrush)
				return S_FALSE;

			pBrush->put_Type(c_BrushTypeSolid);
			pBrush->put_Color1(m_oShadow.Color);
			pBrush->put_Alpha1(m_oShadow.Alpha);

			double dAlpha = (double)m_oShadow.Alpha;

			for(float i = (float)dBlurSize + 1.0F; i >= 1.0F; i--) 
			{
				int iAlpha = (int)((dAlpha/8/(i)/1.1));
				pBrush->put_Alpha1(iAlpha);
				
				DrawBlurString(i, pGraphics, pFont, pBrush, bsText, x, y, w, h);
			}

			RELEASEINTERFACE(pBrush);
		}
		else
		{
			IAVSBrush* pBrush = NULL;
			CoCreateInstance(__uuidof(CAVSBrush), NULL, CLSCTX_INPROC, __uuidof(IAVSBrush), (void**)&pBrush);

			if (NULL == pBrush)
				return S_FALSE;

			pBrush->put_Type(c_BrushTypeSolid);
			pBrush->put_Color1(m_oShadow.Color);
			pBrush->put_Alpha1(m_oShadow.Alpha);

			pGraphics->DrawString(bsText, pFont, pBrush, x, y);

			RELEASEINTERFACE(pBrush);
		}

		return S_OK;
	}

	STDMETHOD(DrawPath)(IAVSGraphics* pGraphics, IAVSGraphicsPath* pPath)
	{
		IAVSBrush* pBrush = NULL;
		CoCreateInstance(__uuidof(CAVSBrush), NULL, CLSCTX_INPROC, __uuidof(IAVSBrush), (void**)&pBrush);

		if (NULL == pBrush)
			return S_FALSE;

		pBrush->put_Type(c_BrushTypeSolid);
		pBrush->put_Color1(m_oShadow.Color);
		pBrush->put_Alpha1(m_oShadow.Alpha);

		IAVSMatrix* pMatrix = NULL;
		pGraphics->get_Transform(&pMatrix);

		pMatrix->Translate(m_oShadow.DistanceX, m_oShadow.DistanceY, c_ag_MatrixOrderAppend);
		pGraphics->put_Transform(pMatrix);

		pGraphics->FillPath(pBrush, pPath);

		RELEASEINTERFACE(pMatrix);
		RELEASEINTERFACE(pBrush);

		return S_OK;
	}

protected:
	inline void DrawBlurString(int currentBlurSize, IAVSGraphics* Gr, IAVSFontManager* font, IAVSBrush* brush, 
		BSTR Text, double& x, double& y, double& width, double& height)
	{
		for(int j = 0; j < currentBlurSize; j++) 
		{
			x += 1;
			Gr->DrawString(Text, font, brush, x, y);
		}
		for(int j = 0; j < currentBlurSize; j++) 
		{
			y += 1;
			Gr->DrawString(Text, font, brush, x, y);
		}
		for(int j = 0; j < currentBlurSize; j++) 
		{
			x -= 1;
			Gr->DrawString(Text, font, brush, x, y);
		}
		for(int j = 0; j < currentBlurSize; j++) 
		{
			x -= 1;
			Gr->DrawString(Text, font, brush, x, y);
		}
		for(int j = 0; j < currentBlurSize; j++) 
		{
			y -= 1;
			Gr->DrawString(Text, font, brush, x, y);
		}
		for(int j = 0; j < currentBlurSize; j++) 
		{
			y -= 1;
			Gr->DrawString(Text, font, brush, x, y);
		}
		for(int j = 0; j < currentBlurSize; j++) 
		{
			x += 1;
			Gr->DrawString(Text, font, brush, x, y);
		}
		for(int j = 0; j < currentBlurSize; j++) 
		{
			x += 1;
			Gr->DrawString(Text, font, brush, x, y);
		}
		for(int j = 0; j < currentBlurSize; j++) 
		{
			y += 1;
			Gr->DrawString(Text, font, brush, x, y);
		}
	}
};