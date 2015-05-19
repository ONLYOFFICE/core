// CASCGlyphImage.h : Declaration of the CGlyphImage

#pragma once
#include "..\stdafx.h"
#include "..\resource.h"       // main symbols

#include "..\Interfaces\IASCGlyphImage.h"

#include "Font\GlyphBitmap.h"

// CGlyphImage
[coclass, uuid("F891A27A-A985-4b85-BCB8-039AAC2A9A76"), threading(apartment), vi_progid("AVSGraphics.GlyphImage"), progid("AVSGraphics.GlyphImage"), version(1.0), support_error_info(IGlyphImage), registration_script("control.rgs")]
class ATL_NO_VTABLE CGlyphImage
	:	public IGlyphImage2
{

public:
	TGlyphBitmap m_oGlyph;

public:

	CGlyphImage() : m_oGlyph()
	{
		m_oGlyph.bFreeData	= false;
		m_oGlyph.pData		= NULL;
	}

	~CGlyphImage()
	{
		Destroy();
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
	STDMETHOD(CreateDublicate)(IASCGraphicsBase** ppGraphicsBase)
	{
		if (NULL == ppGraphicsBase)
			return S_FALSE;

		RELEASEINTERFACE((*ppGraphicsBase));
		if (SUCCEEDED(CoCreateInstance(__uuidof(CGlyphImage), NULL, CLSCTX_INPROC, __uuidof(IASCGraphicsBase), (void**)ppGraphicsBase)))
		{
			((CGlyphImage*)(*ppGraphicsBase))->m_oGlyph = m_oGlyph;
			return S_OK;
		}
		return S_FALSE;
	}

	STDMETHOD(get_Width)(LONG* width)
	{
		if (NULL == width)
			return S_FALSE;

		*width = m_oGlyph.nWidth;
		return S_OK;
	}
	STDMETHOD(get_Height)(LONG* height)
	{
		if (NULL == height)
			return S_FALSE;

		*height = m_oGlyph.nHeight;
		return S_OK;
	}
	STDMETHOD(get_Antialiased)(VARIANT_BOOL* bVal)
	{
		if (NULL == bVal)
			return S_FALSE;

		*bVal = (TRUE == m_oGlyph.bAA) ? VARIANT_TRUE : VARIANT_FALSE; 
		return S_OK;
	}
	STDMETHOD(get_Data)(BYTE** ppBuffer)
	{
		if (NULL == ppBuffer)
			return S_FALSE;

		*ppBuffer = m_oGlyph.pData;
		return S_OK;
	}

	STDMETHOD(Create)(BYTE* pData, LONG lX, LONG lY, LONG lWidth, LONG lHeight, BOOL bIsFreeData)
	{
		m_oGlyph.nX			= lX;
		m_oGlyph.nY			= lY;
		m_oGlyph.nWidth		= lWidth;
		m_oGlyph.nHeight	= lHeight;

		m_oGlyph.bAA		= FALSE;
		m_oGlyph.bFreeData	= FALSE;

		m_oGlyph.pData		= pData;

		//if (FALSE)
		//{
		//	m_oGlyph.bFreeData	= TRUE;
		//	m_oGlyph.pData		= (unsigned char*)malloc(lWidth * lHeight);
		//	memcpy(m_oGlyph.pData, pData, lWidth * lHeight);
		//}

		return S_OK;
	}

public:

	void Create(TGlyphBitmap* pGlyph)
	{
		if (NULL == pGlyph)
			return;
		
		Destroy();

		m_oGlyph.nX			= pGlyph->nX;
		m_oGlyph.nY			= pGlyph->nY;
		m_oGlyph.nWidth		= pGlyph->nWidth;
		m_oGlyph.nHeight	= pGlyph->nHeight;

		m_oGlyph.bAA		= pGlyph->bAA;
		m_oGlyph.bFreeData	= pGlyph->bFreeData;

		m_oGlyph.bFreeData	= true;
		m_oGlyph.pData		= new BYTE[m_oGlyph.nWidth * m_oGlyph.nHeight];
		memcpy(m_oGlyph.pData, pGlyph->pData, m_oGlyph.nWidth * m_oGlyph.nHeight);

		//if (m_oGlyph.bFreeData)
		//{
		//	// надо перевыделить память и скопировать.
		//	// а вообще наверное не надо. просто никогда не релизить (договориться разок и все)
		//}

		//m_oGlyph.pData		= pGlyph->pData;
	}

protected:

	void Destroy()
	{
		if (m_oGlyph.bFreeData)
		{
			RELEASEMEM(m_oGlyph.pData);
		}
	}
};