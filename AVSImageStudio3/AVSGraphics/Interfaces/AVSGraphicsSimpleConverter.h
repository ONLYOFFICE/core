#pragma once
#include "..\stdafx.h"
#include "..\agg\AVSWrapper\GraphicsPath.h"

// IAVSGraphicSimpleComverter
[object, uuid("45EC6624-B5E1-4e60-ADD4-8DD310B6CF96"), dual, pointer_default(unique)]
__interface IAVSGraphicSimpleComverter : IDispatch
{
	[id(0), propget]	HRESULT Renderer([out, retval] IUnknown** ppunkRenderer);
	[id(0), propput]	HRESULT Renderer([in] IUnknown* punkRenderer);

	[id(100)]			HRESULT PathCommandMoveTo([in] double fX, [in] double fY);
	[id(101)]			HRESULT PathCommandLineTo([in] double fX, [in] double fY);
	[id(102)]			HRESULT PathCommandLinesTo([in, satype("double")] SAFEARRAY* pPoints);
	[id(103)]			HRESULT PathCommandCurveTo([in] double fX1, [in] double fY1, [in] double fX2, [in] double fY2, [in] double fX3, [in] double fY3);
	[id(104)]			HRESULT PathCommandCurvesTo([in, satype("double")] SAFEARRAY* pPoints);
	[id(105)]			HRESULT PathCommandArcTo([in] double fX, [in] double fY, [in] double fWidth, [in] double fHeight, [in] double fStartAngle, [in] double fSweepAngle);
	[id(106)]			HRESULT PathCommandClose();
	[id(107)]			HRESULT PathCommandEnd();
	[id(108)]			HRESULT PathCommandStart();
	[id(109)]			HRESULT PathCommandGetCurrentPoint([out] double* fX, [out] double* fY);
	[id(110)]			HRESULT PathCommandText([in] BSTR bsText, [in] IAVSFontManager* pManager, 
												[in] double fX, [in] double fY, [in] double fWidth, [in] double fHeight, [in] double fBaseLineOffset);
	[id(111)]			HRESULT PathCommandTextEx([in] BSTR bsText, [in] BSTR bsGidText, [in] BSTR bsSourceCodeText, [in] IAVSFontManager* pManager, 
												  [in] double fX, [in] double fY, [in] double fWidth, [in] double fHeight, [in] double fBaseLineOffset, [in] DWORD lFlags);

	[id(112)]			HRESULT PathCommandGetBounds([out] double* dLeft, [out] double* dTop, [out] double* dWidth, [out] double* dHeight);

	[id(1000)]			HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(1001)]			HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);
};

[dispinterface, uuid("DA3B4916-0C9E-4845-BD8C-FFC9CE6D3DB2")]
__interface _IAVSGraphicSimpleComverterEvents
{
};

// CAVSGraphicSimpleComverter
[coclass, uuid("48AC5FEC-13E9-413b-BCB5-E74B55ACF4D3"), event_source(com), threading(apartment), vi_progid("SimpleComverter"), progid("SimpleComverter.1"), version(1.0)]
class ATL_NO_VTABLE CAVSGraphicSimpleComverter : public IAVSGraphicSimpleComverter
{
protected:
	Aggplus::CGraphicsPathSimpleConverter m_oConverter;
	
public:
	__event __interface _IAVSGraphicSimpleComverterEvents;

	CAVSGraphicSimpleComverter() : m_oConverter()
	{
	}

	~CAVSGraphicSimpleComverter()
	{
	}

public:
	STDMETHOD(PathCommandMoveTo)(double fX, double fY)
	{
		return (true == m_oConverter.PathCommandMoveTo(fX, fY)) ? S_OK : S_FALSE;
	}
	STDMETHOD(PathCommandLineTo)(double fX, double fY)
	{
		return (true == m_oConverter.PathCommandLineTo(fX, fY)) ? S_OK : S_FALSE;
	}
	STDMETHOD(PathCommandLinesTo)(SAFEARRAY* pPoints)
	{
		return (true == m_oConverter.PathCommandLinesTo(pPoints)) ? S_OK : S_FALSE;
	}

	STDMETHOD(PathCommandCurveTo)(double fX1, double fY1, double fX2, double fY2, double fX3, double fY3)
	{
		return (true == m_oConverter.PathCommandCurveTo(fX1, fY1, fX2, fY2, fX3, fY3)) ? S_OK : S_FALSE;
	}
	STDMETHOD(PathCommandCurvesTo)(SAFEARRAY* pPoints)
	{
		return (true == m_oConverter.PathCommandCurvesTo(pPoints)) ? S_OK : S_FALSE;
	}
	STDMETHOD(PathCommandArcTo)(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle)
	{
		return (true == m_oConverter.PathCommandArcTo(fX, fY, fWidth, fHeight, fStartAngle, fSweepAngle)) ? S_OK : S_FALSE;
	}
	STDMETHOD(PathCommandClose)()
	{
		return (true == m_oConverter.PathCommandClose()) ? S_OK : S_FALSE;
	}
	STDMETHOD(PathCommandEnd)()
	{
		return (true == m_oConverter.PathCommandEnd()) ? S_OK : S_FALSE;
	}

	STDMETHOD(PathCommandStart)()
	{
		return (true == m_oConverter.PathCommandStart()) ? S_OK : S_FALSE;
	}
	STDMETHOD(PathCommandGetCurrentPoint)(double* fX, double* fY)
	{
		return (true == m_oConverter.PathCommandGetCurrentPoint(fX, fY)) ? S_OK : S_FALSE;
	}
	STDMETHOD(PathCommandGetBounds)(double* dLeft, double* dTop, double* dWidth, double* dHeight)
	{
		double left		= 0;
		double top		= 0;
		double width	= 0;
		double height	= 0;

		m_oConverter.PathCommandGetBounds(left, top, width, height);

		*dLeft		= left;
		*dTop		= top;
		*dWidth		= width;
		*dHeight	= height;

		return S_OK;
	}

	STDMETHOD(PathCommandText)(BSTR bsText, IAVSFontManager* pManager, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
	{
		return (true == m_oConverter.PathCommandText(bsText, pManager, fX, fY, fWidth, fHeight, fBaseLineOffset)) ? S_OK : S_FALSE;
	}
	STDMETHOD(PathCommandTextEx)(BSTR bsText, BSTR bsGidText, BSTR bsSourceCodeText, IAVSFontManager* pManager, 
												  double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
	{
		return (true == m_oConverter.PathCommandTextEx(bsText, bsGidText, bsSourceCodeText, pManager,
													fX, fY, fWidth, fHeight, fBaseLineOffset, lFlags)) ? S_OK : S_FALSE;
	}

	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		return S_OK;
	}

	STDMETHOD(get_Renderer)(IUnknown** ppunkRenderer)
	{
		if (NULL == ppunkRenderer)
			return S_FALSE;

		IAVSRenderer* pRenderer = m_oConverter.GetRenderer();

		if (NULL == pRenderer)
			return S_FALSE;

		pRenderer->QueryInterface(IID_IUnknown, (void**)ppunkRenderer);
		return S_OK;
	}
	
	STDMETHOD(put_Renderer)(IUnknown* punkRenderer)
	{
		m_oConverter.SetRenderer((IAVSRenderer*)punkRenderer);
		return S_OK;
	}
};