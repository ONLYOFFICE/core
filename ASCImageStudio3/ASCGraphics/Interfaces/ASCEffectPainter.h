#pragma once

#include "..\stdafx.h"
#include "..\resource.h"       // main symbols
#include "Registration.h"

#include "..\DocumentViewer\Viewer.h"

// шифрование XML
#include "..\..\..\..\Common\RSA\XMLDecoder.h"

using namespace ImageStudio::Serialize::Paint::Common;

// IAVSEffectPainter
[object, uuid("99F1BCD5-6188-472c-B29A-F56E1F23F0EA"), dual, pointer_default(unique)]
__interface IAVSEffectPainter : IDispatch
{
	[id(0)]				HRESULT SetXml([in] BSTR bstrXml);
	[id(1)]				HRESULT Draw([in] IUnknown* punkRenderer);

	[id(2000)]			HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(2001)]			HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);
};

[object, uuid("351A75D3-4FDD-48b5-9027-03CE0AB0D4F6"), dual, pointer_default(unique)]
__interface IAVSEffectPainter2 : IDispatch
{
	[id(3000)]			HRESULT SaveImage([in] IUnknown* punkFrame, [in] BSTR bstrFile, [in] LONG lType);
};

// CAVSEffectPainter
[coclass, uuid("98F3D2DD-7373-4c5a-BF3E-5B66670FE7F4"), threading(apartment), vi_progid("AVSEffectPainter"), progid("AVSEffectPainter.1"), version(1.0)]
class ATL_NO_VTABLE CAVSEffectPainter :	public IAVSEffectPainter, public IAVSEffectPainter2
{
protected:

	CAtlArray<NSDocumentViewer::CWatermark> m_arrWatermarks;

public:

	CAVSEffectPainter() : m_arrWatermarks()
	{
	}

	~CAVSEffectPainter()
	{
	}

public:
	STDMETHOD(SetXml)(BSTR bstrXml)
	{
		m_arrWatermarks.RemoveAll();

		XmlUtils::CXmlNode oNode;
		if (oNode.FromXmlString((CString)bstrXml))
		{
			XmlUtils::CXmlNodes oNodes;
			if (oNode.GetNodes(_T("Watermark"), oNodes))
			{
				int nCount = oNodes.GetCount();
				size_t nWatermarks = 0;
				for (int i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode oWatermark;
					oNodes.GetAt(i, oWatermark);

					m_arrWatermarks.Add();
					m_arrWatermarks[nWatermarks].LoadFromXML(oWatermark);
					++nWatermarks;
				}
			}
		}
		
		return S_OK;
	}
	STDMETHOD(Draw)(IUnknown* punkRenderer)
	{	
		if (NULL == punkRenderer)
			return S_FALSE;
		
		IAVSRenderer* pRenderer = NULL;
		punkRenderer->QueryInterface(__uuidof(IAVSRenderer), (void**)&pRenderer);

		if (NULL == pRenderer)
			return S_FALSE;

		NSDocumentViewer::CDoubleRect oRect;
		oRect.left = 0;
		oRect.top = 0;

		pRenderer->get_Width(&oRect.right);
		pRenderer->get_Height(&oRect.bottom);

		size_t nCount = m_arrWatermarks.GetCount();
		for (size_t i = 0; i < nCount; ++i)
		{
			m_arrWatermarks[i].DrawOnRenderer(pRenderer, oRect);
		}

		RELEASEINTERFACE(pRenderer);
		return S_OK;
	}

	
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		return S_OK;
	}

	STDMETHOD(SaveImage)(IUnknown* punkFrame, BSTR bstrFile, LONG lType)
	{
		ImageStudio::IImageTransforms* pTransform = NULL;
		CoCreateInstance(ImageStudio::CLSID_ImageTransforms, NULL ,CLSCTX_INPROC_SERVER, ImageStudio::IID_IImageTransforms, (void**)&pTransform);

		VARIANT var;
		var.vt = VT_UNKNOWN;
		var.punkVal = punkFrame;
		pTransform->SetSource(0, var);

		CString strXml = _T("<transforms><ImageFile-SaveAsPng destinationpath=\"") + (CString)bstrFile + _T("\" format=\"888\"/></transforms>");
		
		VARIANT_BOOL vbSuccess = VARIANT_FALSE;
		BSTR bsXml = strXml.AllocSysString();
		pTransform->SetXml(bsXml, &vbSuccess);
		SysFreeString(bsXml);

		pTransform->Transform(&vbSuccess);

		RELEASEINTERFACE(pTransform);

		return S_OK;
	}
};
