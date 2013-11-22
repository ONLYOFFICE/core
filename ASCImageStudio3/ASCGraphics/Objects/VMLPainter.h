#pragma once
#include "..\..\..\Common\MediaFormatDefine.h"
#include "..\Interfaces\ImageSerializeObjects2.h"
#include "..\Interfaces\ASCGraphicsRenderer.h"

[object, uuid("786E2B0C-27A4-44bd-9FDD-67CDE59DDEB0"), dual, pointer_default(unique)]
__interface IAVSVMLPainter : IDispatch
{
	[id(10)]		HRESULT OpenXml([in] BSTR bstrXml);
	[id(11)]		HRESULT Draw([in] IAVSRenderer* pRenderer, [in] double dFrame);
	[id(12)]		HRESULT DrawOnMediaData([in] IUnknown* punkFrame, [in] double dFrame);
	
	[id(1001)]		HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(1002)]		HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);
};

// CAVSVMLPainter
[coclass, uuid("4102A280-2728-49d5-951C-C008064A8F94"), threading(apartment), vi_progid("AVSGraphics.VMLPainter"), progid("AVSGraphics.VMLPainter"), version(1.0), support_error_info(IAVSVMLPainter), registration_script("control.rgs")]
class ATL_NO_VTABLE CAVSVMLPainter
	:	public IAVSVMLPainter
{
protected:
	ImageStudio::Serialize::Paint::Common::CDrawGraphicPath m_oPath;

public:

	CAVSVMLPainter() : m_oPath()
	{
	}

	~CAVSVMLPainter()
	{
	}

public:
	STDMETHOD(OpenXml)(BSTR bstrXml)
	{
		XmlUtils::CXmlNode oNode;
		if (oNode.FromXmlString((CString)bstrXml))
		{
			m_oPath.InternalFromXmlNode(oNode);
		}

		return S_OK;
	}
	STDMETHOD(Draw)(IAVSRenderer* pRenderer, double dFrame)
	{
		m_oPath.Draw(pRenderer);
		return S_OK;
	}
	STDMETHOD(DrawOnMediaData)(IUnknown* punkFrame, double dFrame)
	{
		IAVSGraphicsRenderer* pRenderer = NULL;
		CoCreateInstance(__uuidof(CAVSGraphicsRenderer), NULL, CLSCTX_ALL, __uuidof(IAVSGraphicsRenderer), (void**)&pRenderer);

		if (NULL != pRenderer)
		{
			pRenderer->CreateFromMediaData(punkFrame, 0, 0, 100, 100);
			
			IAVSRenderer* pRendererBase = NULL;
			pRenderer->QueryInterface(__uuidof(IAVSRenderer), (void**)&pRendererBase);

			m_oPath.Draw(pRendererBase);

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