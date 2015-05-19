#pragma once
#include "ASCMetafile.h"
#include "ASCGraphicsRenderer.h"

// IASCDocumentRenderer
[object, uuid("1B7CCA59-9471-4265-B5E2-2D2B2FF504FE"), dual, pointer_default(unique)]
__interface IASCDocumentRenderer : IASCRenderer
{
	[id(20000)] HRESULT DrawPage([in] LONG lPageNumber, [in] IUnknown* punkRenderer);

	[id(20001), propget] HRESULT PageCount([out, retval] LONG* lPageNumber);
	[id(20002)] HRESULT GetPageSize([in] LONG lPageNumber, [out] double* pfWidth, [out] double* pfHeight);

	[id(20003)] HRESULT LockPage([in] LONG lPageNumber, [out, retval] IUnknown** ppunkPage);

	[id(20004)] HRESULT SetPageInstance([in] IUnknown* punkInstance);
	[id(20005)] HRESULT SetDocumentInstance([in] IUnknown* punkInstance, [in, satype("double")] SAFEARRAY* pPages);

	[id(20006)] HRESULT SetAdditionalCommands([in] BSTR bsXml);
};

[dispinterface, uuid("F2218BB8-6B9E-4de6-9F73-ECF75707C7B0")]
__interface _IASCDocumentRendererEvents
{
	[id(100)] HRESULT OnNewPage(double dWidthMm, double dHeightMm);
	[id(101)] HRESULT OnCompletePage();
};

[coclass, uuid("3DE3B1C2-3D6A-431e-B20B-BBF5BA38B402"), threading(apartment), event_source(com), vi_progid("ASCDocumentRenderer"), progid("ASCDocumentRenderer.1"), version(1.0)]
class ATL_NO_VTABLE CASCDocumentRenderer :	public IASCDocumentRenderer, public IASCRenderer2
{
private:
	class CPageInfo
	{
	public:
		double m_dWidth;
		double m_dHeight;

	public:
		CPageInfo()
		{
			m_dWidth	= 0;
			m_dHeight	= 0;
		}
		CPageInfo& operator=(const CPageInfo& oSrc)
		{
			m_dWidth	= oSrc.m_dWidth;
			m_dHeight	= oSrc.m_dHeight;

			return *this;
		}
		CPageInfo(const CPageInfo& oSrc)
		{
			*this = oSrc;
		}
	};
private:
	double m_fCurrentX;
	double m_fCurrentY;

	LONG m_lPagesCount;
	CAtlArray<IASCMetafile*> m_arPages;
	
	IAVSOfficePages* m_pDocInstance;
	CAtlArray<CPageInfo> m_arInfos;

	BOOL m_bCompleteLastPage;
	
	CRITICAL_SECTION m_oCS;

	double m_dWindowDPIX;
	double m_dWindowDPIY;

public:

	__event __interface _IASCDocumentRendererEvents;

	CASCDocumentRenderer() : m_arPages()
	{
		m_fCurrentX = 0;
		m_fCurrentY = 0;

		m_lPagesCount = 0;

		m_bCompleteLastPage = FALSE;

		m_pDocInstance = NULL;

		InitializeCriticalSection ( &m_oCS );

		m_dWindowDPIX		= -1;
		m_dWindowDPIY		= -1;
	}

	~CASCDocumentRenderer()
	{
		size_t lCount = m_arPages.GetCount();
		for (size_t nIndex = 0; nIndex < lCount; ++nIndex)
		{
			IASCMetafile* pPage = m_arPages[nIndex];
			RELEASEINTERFACE(pPage);
		}
		
		m_arPages.RemoveAll();

		DeleteCriticalSection  ( &m_oCS );
	}

public:

	STDMETHOD(get_Type)(LONG* lType)
	{
		return S_OK;
	}
	//-------- Функции для работы со страницей --------------------------------------------------
	STDMETHOD(NewPage)()
	{
		CTemporaryCS CS ( &m_oCS );

		IASCMetafile* pPage = NULL;
		CoCreateInstance(__uuidof(CASCMetafile), NULL, CLSCTX_INPROC, __uuidof(IASCMetafile), (void**)&pPage);

		pPage->put_Width(0);
		pPage->put_Height(0);

		++m_lPagesCount;
		m_bCompleteLastPage = FALSE;
		
		m_arPages.Add(pPage);

		OnNewPage(0, 0);

		return S_OK;
	}
	STDMETHOD(get_Height)(double* dHeight)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->get_Height(dHeight);
	}
	STDMETHOD(put_Height)(double dHeight)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->put_Height(dHeight);
	}
	STDMETHOD(get_Width)(double* dWidth)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->get_Width(dWidth);
	}
	STDMETHOD(put_Width)(double dWidth)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->put_Width(dWidth);
	}
	STDMETHOD(get_DpiX)(double* dDpiX)
	{
		return S_FALSE;
	}
	STDMETHOD(get_DpiY)(double* dDpiY)
	{
		return S_FALSE;
	}

// pen --------------------------------------------------------------------------------------
	STDMETHOD(SetPen)(BSTR bsXML)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->SetPen(bsXML);
	}
	STDMETHOD(get_PenColor)(LONG* lColor)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->get_PenColor(lColor);
	}
	STDMETHOD(put_PenColor)(LONG lColor)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->put_PenColor(lColor);
	}
	STDMETHOD(get_PenAlpha)(LONG* lAlpha)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->get_PenAlpha(lAlpha);
	}
	STDMETHOD(put_PenAlpha)(LONG lAlpha)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->put_PenAlpha(lAlpha);
	}
	STDMETHOD(get_PenSize)(double* dSize)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->get_PenSize(dSize);
	}
	STDMETHOD(put_PenSize)(double dSize)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->put_PenSize(dSize);
	}
	STDMETHOD(get_PenDashStyle)(BYTE* val)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->get_PenDashStyle(val);
	}
	STDMETHOD(put_PenDashStyle)(BYTE val)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->put_PenDashStyle(val);
	}
	STDMETHOD(get_PenLineStartCap)(BYTE* val)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->get_PenLineStartCap(val);
	}
	STDMETHOD(put_PenLineStartCap)(BYTE val)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->put_PenLineStartCap(val);
	}
	STDMETHOD(get_PenLineEndCap)(BYTE* val)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->get_PenLineEndCap(val);
	}
	STDMETHOD(put_PenLineEndCap)(BYTE val)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->put_PenLineEndCap(val);
	}
	STDMETHOD(get_PenLineJoin)(BYTE* val)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->get_PenLineJoin(val);
	}
	STDMETHOD(put_PenLineJoin)(BYTE val)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->put_PenLineJoin(val);
	}
	STDMETHOD(get_PenDashOffset)(double* val)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->get_PenDashOffset(val);
	}
	STDMETHOD(put_PenDashOffset)(double val)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->put_PenDashOffset(val);
	}
	STDMETHOD(get_PenAlign)(LONG* val)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->get_PenAlign(val);
	}
	STDMETHOD(put_PenAlign)(LONG val)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->put_PenAlign(val);
	}
	STDMETHOD(get_PenMiterLimit)(double* val)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->get_PenMiterLimit(val);
	}
	STDMETHOD(put_PenMiterLimit)(double val)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->put_PenMiterLimit(val);
	}
	STDMETHOD(PenDashPattern)(SAFEARRAY* pPattern)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->PenDashPattern(pPattern);
	}

// brush ------------------------------------------------------------------------------------
	STDMETHOD(SetBrush)(BSTR bsXML)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->SetBrush(bsXML);
	}
	STDMETHOD(get_BrushType)(LONG* lType)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->get_BrushType(lType);
	}
	STDMETHOD(put_BrushType)(LONG lType)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->put_BrushType(lType);
	}
	STDMETHOD(get_BrushColor1)(LONG* lColor)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->get_BrushColor1(lColor);
	}
	STDMETHOD(put_BrushColor1)(LONG lColor)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->put_BrushColor1(lColor);
	}
	STDMETHOD(get_BrushAlpha1)(LONG* lAlpha)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->get_BrushAlpha1(lAlpha);
	}
	STDMETHOD(put_BrushAlpha1)(LONG lAlpha)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->put_BrushAlpha1(lAlpha);
	}
	STDMETHOD(get_BrushColor2)(LONG* lColor)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->get_BrushColor2(lColor);
	}
	STDMETHOD(put_BrushColor2)(LONG lColor)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->put_BrushColor2(lColor);
	}
	STDMETHOD(get_BrushAlpha2)(LONG* lAlpha)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->get_BrushAlpha2(lAlpha);
	}
	STDMETHOD(put_BrushAlpha2)(LONG lAlpha)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->put_BrushAlpha2(lAlpha);
	}
	STDMETHOD(get_BrushTexturePath)(BSTR* bsPath)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->get_BrushTexturePath(bsPath);
	}
	STDMETHOD(put_BrushTexturePath)(BSTR bsPath)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->put_BrushTexturePath(bsPath);
	}
	STDMETHOD(get_BrushTextureMode)(LONG* lMode)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->get_BrushTextureMode(lMode);
	}
	STDMETHOD(put_BrushTextureMode)(LONG lMode)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->put_BrushTextureMode(lMode);
	}
	STDMETHOD(get_BrushTextureAlpha)(LONG* lTxAlpha)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->get_BrushTextureAlpha(lTxAlpha);
	}
	STDMETHOD(put_BrushTextureAlpha)(LONG lTxAlpha)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->put_BrushTextureAlpha(lTxAlpha);
	}
	STDMETHOD(get_BrushLinearAngle)(double* dAngle)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->get_BrushLinearAngle(dAngle);
	}
	STDMETHOD(put_BrushLinearAngle)(double dAngle)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->put_BrushLinearAngle(dAngle);
	}
	STDMETHOD(BrushRect)(BOOL val, double left, double top, double width, double height)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->BrushRect(val, left, top, width, height);
	}

// font -------------------------------------------------------------------------------------
	STDMETHOD(SetFont)(BSTR bsXML)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->SetFont(bsXML);
	}
	STDMETHOD(get_FontName)(BSTR* bsName)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->get_FontName(bsName);
	}
	STDMETHOD(put_FontName)(BSTR bsName)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->put_FontName(bsName);
	}
	STDMETHOD(get_FontPath)(BSTR* bsName)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->get_FontPath(bsName);
	}
	STDMETHOD(put_FontPath)(BSTR bsName)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->put_FontPath(bsName);
	}
	STDMETHOD(get_FontSize)(double* dSize)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->get_FontSize(dSize);
	}
	STDMETHOD(put_FontSize)(double dSize)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->put_FontSize(dSize);
	}
	STDMETHOD(get_FontStyle)(LONG* lStyle)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->get_FontStyle(lStyle);
	}
	STDMETHOD(put_FontStyle)(LONG lStyle)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->put_FontStyle(lStyle);
	}
	STDMETHOD(get_FontStringGID)(BOOL* bGID)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->get_FontStringGID(bGID);
	}
	STDMETHOD(put_FontStringGID)(BOOL bGID)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->put_FontStringGID(bGID);
	}

	STDMETHOD(get_FontCharSpace)(double* dSpace)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->get_FontCharSpace(dSpace);
	}
	STDMETHOD(put_FontCharSpace)(double dSpace)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->put_FontCharSpace(dSpace);
	}

// shadow -----------------------------------------------------------------------------------
	STDMETHOD(SetShadow)(BSTR bsXML)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->SetShadow(bsXML);
	}
	STDMETHOD(get_ShadowDistanceX)(double* val)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->get_ShadowDistanceX(val);
	}
	STDMETHOD(put_ShadowDistanceX)(double val)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->put_ShadowDistanceX(val);
	}
	STDMETHOD(get_ShadowDistanceY)(double* val)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->get_ShadowDistanceY(val);
	}
	STDMETHOD(put_ShadowDistanceY)(double val)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->put_ShadowDistanceY(val);
	}
	STDMETHOD(get_ShadowBlurSize)(double* val)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->get_ShadowBlurSize(val);
	}
	STDMETHOD(put_ShadowBlurSize)(double val)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->put_ShadowBlurSize(val);
	}
	STDMETHOD(get_ShadowColor)(LONG* val)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->get_ShadowColor(val);
	}
	STDMETHOD(put_ShadowColor)(LONG val)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->put_ShadowColor(val);
	}
	STDMETHOD(get_ShadowAlpha)(LONG* val)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->get_ShadowAlpha(val);
	}
	STDMETHOD(put_ShadowAlpha)(LONG val)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->put_ShadowAlpha(val);
	}
	STDMETHOD(get_ShadowVisible)(BOOL* val)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->get_ShadowVisible(val);
	}
	STDMETHOD(put_ShadowVisible)(BOOL val)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->put_ShadowVisible(val);
	}

// edge -------------------------------------------------------------------------------------
	STDMETHOD(SetEdgeText)(BSTR bsXML)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->SetEdgeText(bsXML);
	}
	STDMETHOD(get_EdgeVisible)(LONG* val)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->get_EdgeVisible(val);
	}
	STDMETHOD(put_EdgeVisible)(LONG val)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->put_EdgeVisible(val);
	}
	STDMETHOD(get_EdgeColor)(LONG* val)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->get_EdgeColor(val);
	}
	STDMETHOD(put_EdgeColor)(LONG val)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->put_EdgeColor(val);
	}
	STDMETHOD(get_EdgeAlpha)(LONG* val)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->get_EdgeAlpha(val);
	}
	STDMETHOD(put_EdgeAlpha)(LONG val)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->put_EdgeAlpha(val);
	}
	STDMETHOD(get_EdgeDist)(double* val)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->get_EdgeDist(val);
	}
	STDMETHOD(put_EdgeDist)(double val)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->put_EdgeDist(val);
	}

//-------- Функции для вывода текста --------------------------------------------------------
	STDMETHOD(CommandDrawText)(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->CommandDrawText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);
	}
	STDMETHOD(CommandDrawTextEx)(BSTR bsText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->CommandDrawTextEx(bsText, bsGidText, bsSourceCodeText, fX, fY, fWidth, fHeight, fBaseLineOffset, lFlags);
	}

//-------- Маркеры для команд ---------------------------------------------------------------
	STDMETHOD(BeginCommand)(DWORD lType)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->BeginCommand(lType);
	}
	STDMETHOD(EndCommand)(DWORD lType)
	{
		if (c_nPageType == lType)
		{
			m_bCompleteLastPage = TRUE;

			OnCompletePage();
		}
		else
		{
			LONG lPageIndex = m_lPagesCount - 1;
			if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
				return S_FALSE;

			m_arPages[lPageIndex]->EndCommand(lType);
		}
		
		return S_OK;
	}

//-------- Функции для работы с Graphics Path -----------------------------------------------
	STDMETHOD(PathCommandMoveTo)(double fX, double fY)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->PathCommandMoveTo(fX, fY);
	}
	STDMETHOD(PathCommandLineTo)(double fX, double fY)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->PathCommandLineTo(fX, fY);
	}
	STDMETHOD(PathCommandLinesTo)(SAFEARRAY* pPoints)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->PathCommandLinesTo(pPoints);
	}
	STDMETHOD(PathCommandCurveTo)(double fX1, double fY1, double fX2, double fY2, double fX3, double fY3)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->PathCommandCurveTo(fX1, fY1, fX2, fY2, fX3, fY3);
	}
	STDMETHOD(PathCommandCurvesTo)(SAFEARRAY* pPoints)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->PathCommandCurvesTo(pPoints);
	}
	STDMETHOD(PathCommandArcTo)(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->PathCommandArcTo(fX, fY, fWidth, fHeight, fStartAngle, fSweepAngle);
	}
	STDMETHOD(PathCommandClose)()
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->PathCommandClose();
	}
	STDMETHOD(PathCommandEnd)()
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->PathCommandEnd();
	}
	STDMETHOD(DrawPath)(long nType)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->DrawPath(nType);
	}
	STDMETHOD(PathCommandStart)()
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->PathCommandStart();
	}
	STDMETHOD(PathCommandGetCurrentPoint)(double* fX, double* fY)
	{
		if ((NULL == fX) || (NULL == fY))
			return S_FALSE;

		*fX = m_fCurrentX;
		*fY = m_fCurrentY;

		return S_OK;
	}
	STDMETHOD(PathCommandText)(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->PathCommandText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);
	}
	STDMETHOD(PathCommandTextEx)(BSTR bsText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->PathCommandTextEx(bsText, bsGidText, bsSourceCodeText, fX, fY, fWidth, fHeight, fBaseLineOffset, lFlags);
	}

//-------- Функции для вывода изображений ---------------------------------------------------
	STDMETHOD(DrawImage)(IUnknown* pInterface, double fX, double fY, double fWidth, double fHeight)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->DrawImage(pInterface, fX, fY, fWidth, fHeight);
	}
	STDMETHOD(DrawImageFromFile)(BSTR bstrVal, double fX, double fY, double fWidth, double fHeight)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->DrawImageFromFile(bstrVal, fX, fY, fWidth, fHeight);
		return S_OK;
	}

// transform --------------------------------------------------------------------------------
	STDMETHOD(GetCommandParams)(double* dAngle, double* dLeft, double* dTop, double* dWidth, double* dHeight, DWORD* lFlags)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->GetCommandParams(dAngle, dLeft, dTop, dWidth, dHeight, lFlags);
	}
	STDMETHOD(SetCommandParams)(double dAngle, double dLeft, double dTop, double dWidth, double dHeight, DWORD lFlags)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->SetCommandParams(dAngle, dLeft, dTop, dWidth, dHeight, lFlags);
	}

	STDMETHOD(SetTransform)(double dA, double dB, double dC, double dD, double dE, double dF)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->SetTransform(dA, dB, dC, dD, dE, dF);
	}
	STDMETHOD(GetTransform)(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->GetTransform(pdA, pdB, pdC, pdD, pdE, pdF);
	}
	STDMETHOD(ResetTransform)(void)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->ResetTransform();
	}

// -----------------------------------------------------------------------------------------
	STDMETHOD(get_ClipMode)(LONG* plMode)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->get_ClipMode(plMode);
	}
	STDMETHOD(put_ClipMode)(LONG lMode)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->put_ClipMode(lMode);
	}

// additiaonal params ----------------------------------------------------------------------
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		CString strParamName = (CString)ParamName; 
		
		if (_T("CurrentX") == strParamName)
		{
			m_fCurrentX = ParamValue.fltVal;
			return S_OK;
		}
		if (_T("CurrentY") == strParamName)
		{
			m_fCurrentY = ParamValue.fltVal;
			return S_OK;
		}
		if (_T("DPIX") == strParamName)
		{
			m_dWindowDPIX = ParamValue.dblVal;
			return S_OK;
		}
		if (_T("DPIY") == strParamName)
		{
			m_dWindowDPIY = ParamValue.dblVal;
			return S_OK;
		}
		
		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		if (NULL == ParamValue)
			return S_FALSE;
		
		CString strParamName = (CString)ParamName; 

		if (_T("DPIX") == strParamName)
		{
			ParamValue->vt = VT_R8;
			ParamValue->dblVal = m_dWindowDPIX;
			return S_OK;
		}
		if (_T("DPIY") == strParamName)
		{
			ParamValue->vt = VT_R8;
			ParamValue->dblVal = m_dWindowDPIY;
			return S_OK;
		}
		return S_OK;
	}

	// ------------------------------------
	STDMETHOD(DrawPage)(LONG lPageNumber, IUnknown* punkRenderer)
	{
		CTemporaryCS CS ( &m_oCS );

		if ((0 > lPageNumber) || (lPageNumber >= m_lPagesCount) || (NULL == punkRenderer))
		{
			return S_FALSE;
		}

		//m_arPages[lPageNumber]->Draw(punkRenderer, m_dWindowDPIX, m_dWindowDPIY, NULL);
		m_arPages[lPageNumber]->Draw(punkRenderer, NULL);
		return S_OK;
	}

	STDMETHOD(get_PageCount)(LONG* plPages)
	{
		if (NULL == plPages)
			return S_FALSE;

		CTemporaryCS CS ( &m_oCS );

		if (m_bCompleteLastPage)
		{
			*plPages = m_lPagesCount;
		}
		else
		{
			*plPages = max(0, (m_lPagesCount - 1));
		}

		return S_OK;
	}

	STDMETHOD(GetPageSize)(LONG lPageNumber, double* pfWidth, double* pfHeight)
	{		
		if ((0 > lPageNumber) || (lPageNumber >= m_lPagesCount))
			return S_FALSE;

		m_arPages[lPageNumber]->get_Width(pfWidth);
		m_arPages[lPageNumber]->get_Height(pfHeight);
		
		return S_OK;
	}
	STDMETHOD(LockPage)(LONG lPageNumber, IUnknown** ppunkPage)
	{
		if (lPageNumber < 0 || lPageNumber >= m_arPages.GetCount())
			return S_FALSE;

		if (NULL == ppunkPage)
			return S_FALSE;

		m_arPages[lPageNumber]->QueryInterface(IID_IUnknown, (void**)ppunkPage);

		return S_OK;
	}
	STDMETHOD(SetPageInstance)(IUnknown* punkInstance)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		return m_arPages[lPageIndex]->SetPageInstance(punkInstance, lPageIndex);
	}

	STDMETHOD(SetDocumentInstance)(IUnknown* punkInstance, SAFEARRAY* pPages)
	{
		if ((NULL == punkInstance) || (NULL == pPages))
			return S_FALSE;
		
		double* pInfos		= (double*)pPages->pvData;
		LONG lCountPages	= (LONG)(pPages->rgsabound[0].cElements / 2);

		for (LONG nPage = 0; nPage < lCountPages; ++nPage, pInfos += 2)
		{
			IASCMetafile* pPage = NULL;
			CoCreateInstance(__uuidof(CASCMetafile), NULL, CLSCTX_INPROC, __uuidof(IASCMetafile), (void**)&pPage);

			pPage->put_Width(pInfos[0]);
			pPage->put_Height(pInfos[1]);

			pPage->SetPageInstance(punkInstance, nPage);
		
			m_arPages.Add(pPage);
		}

		m_lPagesCount = lCountPages;
		m_bCompleteLastPage = TRUE;

		OnCompletePage();

		return S_OK;
	}

	STDMETHOD(SetAdditionalCommands)(BSTR bsXML)
	{
		return S_OK;
	}

	STDMETHOD(CommandLong)(LONG lCommand)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		IASCRenderer2* pRend2 = NULL;
		m_arPages[lPageIndex]->QueryInterface(__uuidof(IASCRenderer2), (void**)&pRend2);
		HRESULT hRes = pRend2->CommandLong(lCommand);
		RELEASEINTERFACE(pRend2);
		return hRes;
	}
	STDMETHOD(CommandDouble)(double dCommand)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		IASCRenderer2* pRend2 = NULL;
		m_arPages[lPageIndex]->QueryInterface(__uuidof(IASCRenderer2), (void**)&pRend2);
		HRESULT hRes = pRend2->CommandDouble(dCommand);
		RELEASEINTERFACE(pRend2);
		return hRes;
	}
	STDMETHOD(CommandString)(BSTR bsCommand)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		IASCRenderer2* pRend2 = NULL;
		m_arPages[lPageIndex]->QueryInterface(__uuidof(IASCRenderer2), (void**)&pRend2);
		HRESULT hRes = pRend2->CommandString(bsCommand);
		RELEASEINTERFACE(pRend2);
		return hRes;
	}
	STDMETHOD(CommandVariant)(VARIANT vCommand)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		IASCRenderer2* pRend2 = NULL;
		m_arPages[lPageIndex]->QueryInterface(__uuidof(IASCRenderer2), (void**)&pRend2);
		HRESULT hRes = pRend2->CommandVariant(vCommand);
		RELEASEINTERFACE(pRend2);
		return hRes;
	}

	STDMETHOD(CommandLong2)(LONG lType, LONG lCommand)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		IASCRenderer2* pRend2 = NULL;
		m_arPages[lPageIndex]->QueryInterface(__uuidof(IASCRenderer2), (void**)&pRend2);
		HRESULT hRes = pRend2->CommandLong2(lType, lCommand);
		RELEASEINTERFACE(pRend2);
		return hRes;
	}
	STDMETHOD(CommandDouble2)(LONG lType, double dCommand)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		IASCRenderer2* pRend2 = NULL;
		m_arPages[lPageIndex]->QueryInterface(__uuidof(IASCRenderer2), (void**)&pRend2);
		HRESULT hRes = pRend2->CommandDouble2(lType, dCommand);
		RELEASEINTERFACE(pRend2);
		return hRes;
	}
	STDMETHOD(CommandString2)(LONG lType, BSTR bsCommand)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		IASCRenderer2* pRend2 = NULL;
		m_arPages[lPageIndex]->QueryInterface(__uuidof(IASCRenderer2), (void**)&pRend2);
		HRESULT hRes = pRend2->CommandString2(lType, bsCommand);
		RELEASEINTERFACE(pRend2);
		return hRes;
	}
	STDMETHOD(CommandVariant2)(LONG lType, VARIANT vCommand)
	{
		LONG lPageIndex = m_lPagesCount - 1;
		if (lPageIndex < 0 || lPageIndex >= m_arPages.GetCount())
			return S_FALSE;

		IASCRenderer2* pRend2 = NULL;
		m_arPages[lPageIndex]->QueryInterface(__uuidof(IASCRenderer2), (void**)&pRend2);
		HRESULT hRes = pRend2->CommandVariant2(lType, vCommand);
		RELEASEINTERFACE(pRend2);
		return hRes;
	}
};