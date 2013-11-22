#pragma once
#include "VMLPainter.h"

[object, uuid("A34D4812-AAE9-4441-91B6-F73E19659DE0"), dual, pointer_default(unique)]
__interface IAVSTextPainter : IDispatch
{
	[id(10)]		HRESULT OpenXml([in] BSTR bstrXml);
	[id(11)]		HRESULT Draw([in] IAVSRenderer* pRenderer, [in] double dFrame);
	[id(12)]		HRESULT DrawOnMediaData([in] IUnknown* punkFrame, [in] double dFrame);
	
	[id(1001)]		HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(1002)]		HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);
};

// CAVSTextPainter
[coclass, uuid("F762C2A1-E655-46d3-A9D9-D31E0DBD9797"), threading(apartment), vi_progid("AVSGraphics.TextPainter"), progid("AVSGraphics.TextPainter"), version(1.0), support_error_info(IAVSTextPainter), registration_script("control.rgs")]
class ATL_NO_VTABLE CAVSTextPainter
	:	public IAVSTextPainter
{
protected:
	ImageStudio::Serialize::Paint::Common::CTextMeasurer m_oMeasurer;
	double m_dWidthMM;
	double m_dHeightMM;

public:

	CAVSTextPainter() : m_oMeasurer()
	{
	}

	~CAVSTextPainter()
	{
	}

public:
	STDMETHOD(OpenXml)(BSTR bstrXml)
	{
		XmlUtils::CXmlNode oNode;
		if (oNode.FromXmlString((CString)bstrXml))
		{
			m_oMeasurer.InternalFromXmlNode(oNode);

			m_dWidthMM	= XmlUtils::GetDouble(oNode.GetAttributeOrValue(_T("widthmm"), _T("100")));
			m_dHeightMM = XmlUtils::GetDouble(oNode.GetAttributeOrValue(_T("heightmm"), _T("100")));
		}

		return S_OK;
	}
	STDMETHOD(Draw)(IAVSRenderer* pRenderer, double dFrame)
	{
		if (NULL != pRenderer)
		{
			pRenderer->put_Width(m_dWidthMM);
			pRenderer->put_Height(m_dHeightMM);
		}
		m_oMeasurer.DrawText(pRenderer, (float)dFrame);
		return S_OK;
	}
	STDMETHOD(DrawOnMediaData)(IUnknown* punkFrame, double dFrame)
	{
		IAVSGraphicsRenderer* pRenderer = NULL;
		CoCreateInstance(__uuidof(CAVSGraphicsRenderer), NULL, CLSCTX_ALL, __uuidof(IAVSGraphicsRenderer), (void**)&pRenderer);

		if (NULL != pRenderer)
		{
			pRenderer->CreateFromMediaData(punkFrame, 0, 0, 100, 100);
			((CAVSGraphicsRenderer*)pRenderer)->SetFontManager(NULL);
			
			IAVSRenderer* pRendererBase = NULL;
			pRenderer->QueryInterface(__uuidof(IAVSRenderer), (void**)&pRendererBase);

			pRenderer->put_Width(m_dWidthMM);
			pRenderer->put_Height(m_dHeightMM);

			m_oMeasurer.DrawText(pRendererBase, (float)dFrame);

			RELEASEINTERFACE(pRendererBase);
			RELEASEINTERFACE(pRenderer);

			return S_OK;
		}
				
		return S_FALSE;
	}

	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		return S_OK;
	}
};