// CAVSEdgeText.h : Declaration of the CAVSEdgeText

#pragma once
#include "stdafx.h"
#include "resource.h"       // main symbols
#include "Registration.h"

#include "Structures.h"
#include "..\Interfaces\IAVSEdgeText.h"

#include "AVSPen.h"
#include "AVSBrush.h"
#include "AVSGraphicsPath.h"

#include "..\Common\Test.h"

// CAVSEdgeText
[coclass, uuid("0806EEB2-027F-400f-A2E7-B18E8F5CC0F3"), threading(apartment), vi_progid("AVSGraphics.Edge"), progid("AVSGraphics.Edge"), version(1.0), support_error_info(IAVSEdgeText), registration_script("control.rgs")]
class ATL_NO_VTABLE CAVSEdgeText
	:	public IAVSEdgeText
{
	friend class CAVSMetafile;

protected:
	NSStructures::CEdgeText m_oEdge;

public:

	CAVSEdgeText() : m_oEdge()
	{
		CTester oTester;
		oTester.Test();
	}

	~CAVSEdgeText()
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
		if (SUCCEEDED(CoCreateInstance(__uuidof(CAVSEdgeText), NULL, CLSCTX_INPROC, __uuidof(IAVSGraphicsBase), (void**)ppGraphicsBase)))
		{
			((CAVSEdgeText*)(*ppGraphicsBase))->m_oEdge = m_oEdge;
			return S_OK;
		}
		return S_FALSE;
	}

	STDMETHOD(get_Xml)(BSTR* bstrXml)
	{
		if (NULL == bstrXml)
			return S_FALSE;

		*bstrXml = m_oEdge.ToXmlString().AllocSysString();
		return S_OK;
	}
	STDMETHOD(put_Xml)(BSTR bstrXml)
	{
		if (NULL == bstrXml)
			return S_FALSE;

		m_oEdge.FromXmlString(CString(bstrXml));
		return S_OK;
	}

	STDMETHOD(get_Visible)(LONG* lVal)
	{
		if (NULL == lVal)
			return S_FALSE;

		*lVal = m_oEdge.Visible;
		return S_OK;
	}
	STDMETHOD(put_Visible)(LONG lVal)
	{
		m_oEdge.Visible = lVal;
		return S_OK;
	}
	STDMETHOD(get_Color)(LONG* lVal)
	{
		if (NULL == lVal)
			return S_FALSE;

		*lVal = m_oEdge.Color;
		return S_OK;
	}
	STDMETHOD(put_Color)(LONG lVal)
	{
		m_oEdge.Color = lVal;
		return S_OK;
	}

	STDMETHOD(get_Alpha)(LONG* lVal)
	{
		if (NULL == lVal)
			return S_FALSE;

		*lVal = m_oEdge.Alpha;
		return S_OK;
	}
	STDMETHOD(put_Alpha)(LONG lVal)
	{
		m_oEdge.Alpha = lVal;
		return S_OK;
	}
	STDMETHOD(get_Distance)(double* lVal)
	{
		if (NULL == lVal)
			return S_FALSE;

		*lVal = m_oEdge.Dist;
		return S_OK;
	}
	STDMETHOD(put_Distance)(double lVal)
	{
		m_oEdge.Dist = lVal;
		return S_OK;
	}

	STDMETHOD(DrawText)(IAVSGraphics* pGraphics, IAVSFontManager* pFont, BSTR bsText, double dLeft, double dTop, double dRight, double dBottom)
	{
		if ((0 == m_oEdge.Visible) || NULL == pGraphics || NULL == pFont)
			return S_OK;

		double x = dLeft;
		double y = dTop;
		double w = dRight - dLeft;
		double h = dBottom - dTop;
		
		if (m_oEdge.Visible == -1)
		{
			IAVSBrush* pBrush = NULL;
			CoCreateInstance(__uuidof(CAVSBrush), NULL, CLSCTX_INPROC, __uuidof(IAVSBrush), (void**)&pBrush);

			if (NULL == pBrush)
				return S_FALSE;

			pBrush->put_Type(c_BrushTypeSolid);
			pBrush->put_Color1(m_oEdge.Color);
			pBrush->put_Alpha1(m_oEdge.Alpha);

			int offset = int(m_oEdge.Dist);
			double dAlpha = (double)m_oEdge.Alpha;

			for(int i = offset; i >= 1; i--) 
			{
				int iAlpha = (int)((dAlpha/8/(i)/1.1));
				pBrush->put_Alpha1(iAlpha);
				
				DrawBlurString(i, pGraphics, pFont, pBrush, bsText, x, y, w, h);
			}

			RELEASEINTERFACE(pBrush);
		}
		else if (m_oEdge.Visible == 1)
		{
			//pGraphics->put_SmoothingMode((LONG)SmoothingModeHighQuality);

			IAVSGraphicsPath* pPath = NULL;
			CoCreateInstance(__uuidof(CAVSGraphicsPath), NULL, CLSCTX_INPROC, __uuidof(IAVSGraphicsPath), (void**)&pPath);

			IAVSPen* pPen = NULL;
			CoCreateInstance(__uuidof(CAVSPen), NULL, CLSCTX_INPROC, __uuidof(IAVSPen), (void**)&pPen);

			if (NULL == pPath || NULL == pPen)
			{
				RELEASEINTERFACE(pPath);
				RELEASEINTERFACE(pPen);
			}

			pPath->AddString(bsText, pFont, x, y);
			
			pPen->put_Color(m_oEdge.Color);
			pPen->SetLineCap(c_ag_LineCapRoundAnchor, c_ag_LineCapRoundAnchor, c_ag_DashCapRound);

			double dAlpha = (double)m_oEdge.Alpha;

			// draw text
			int offset = int(m_oEdge.Dist);
			for (int i = 1; i <= offset; i++)
			{
				pPen->put_Size(2*(offset - i + 1));
				pPen->put_Alpha((LONG)dAlpha*i/offset);
				
				pGraphics->DrawPath(pPen, pPath);
			}
		}
		else if (m_oEdge.Visible == 2)
		{
			//pGraphics->put_SmoothingMode((LONG)SmoothingModeHighQuality);

			// еще клип нужно сделать

			IAVSGraphicsPath* pPath = NULL;
			CoCreateInstance(__uuidof(CAVSGraphicsPath), NULL, CLSCTX_INPROC, __uuidof(IAVSGraphicsPath), (void**)&pPath);

			IAVSPen* pPen = NULL;
			CoCreateInstance(__uuidof(CAVSPen), NULL, CLSCTX_INPROC, __uuidof(IAVSPen), (void**)&pPen);

			if (NULL == pPath || NULL == pPen)
			{
				RELEASEINTERFACE(pPath);
				RELEASEINTERFACE(pPen);
			}

			pPath->AddString(bsText, pFont, x, y);
			
			pPen->put_Color(m_oEdge.Color);
			pPen->SetLineCap(c_ag_LineCapRoundAnchor, c_ag_LineCapRoundAnchor, c_ag_DashCapRound);

			double dAlpha = (double)m_oEdge.Alpha;

			// draw text
			int offset = int(m_oEdge.Dist);
			for (int i = 1; i <= offset; i++)
			{
				pPen->put_Size(2*(offset - i + 1));
				pPen->put_Alpha((LONG)dAlpha*i/offset);
				
				pGraphics->DrawPath(pPen, pPath);
			}
		}

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