#pragma once
#include "VMLPainter.h"

[object, uuid("B3F59373-AD80-4932-8B9F-F16F99FE7D87"), dual, pointer_default(unique)]
__interface IAVSImagePainter : IDispatch
{
	[id(10)]		HRESULT OpenXml([in] BSTR bstrXml);
	[id(11)]		HRESULT Draw([in] IAVSRenderer* pRenderer, [in] double dFrame);
	[id(12)]		HRESULT DrawOnMediaData([in] IUnknown* punkFrame, [in] double dFrame);
	
	[id(1001)]		HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(1002)]		HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);
};

// CAVSImagePainter
[coclass, uuid("0734E1BC-BF41-471c-B551-1E4DA1071FD0"), threading(apartment), vi_progid("AVSGraphics.ImagePainter"), progid("AVSGraphics.ImagePainter"), version(1.0), support_error_info(IAVSImagePainter), registration_script("control.rgs")]
class ATL_NO_VTABLE CAVSImagePainter
	:	public IAVSImagePainter
{
protected:
	CString m_strFilePath;

	double m_dLeft;
	double m_dTop;
	double m_dWidth;
	double m_dHeight;

	// кроп??

	double m_dWidthMM;
	double m_dHeightMM;

public:

	CAVSImagePainter()
	{
	}

	~CAVSImagePainter()
	{
	}

public:
	STDMETHOD(OpenXml)(BSTR bstrXml)
	{
		XmlUtils::CXmlNode oNode;
		if (oNode.FromXmlString((CString)bstrXml))
		{
			m_oMeasurer.InternalFromXmlNode(oNode);

			m_strFilePath = oNode.GetAttributeOrValue(_T("filepath"), _T(""));

			m_dWidthMM	= XmlUtils::GetDouble(oNode.GetAttributeOrValue(_T("widthmetric"), _T("100")));
			m_dHeightMM = XmlUtils::GetDouble(oNode.GetAttributeOrValue(_T("heightmetric"), _T("100")));
			
			m_dLeft		= XmlUtils::GetDouble(oNode.GetAttributeOrValue(_T("left"), _T("100")));
			m_dTop		= XmlUtils::GetDouble(oNode.GetAttributeOrValue(_T("top"), _T("100")));
			m_dWidth	= XmlUtils::GetDouble(oNode.GetAttributeOrValue(_T("right"), _T("100")));
			m_dHeight	= XmlUtils::GetDouble(oNode.GetAttributeOrValue(_T("bottom"), _T("100")));

			m_dWidth	-= m_dLeft;
			m_dHeight	-= m_dTop;
		}

		return S_OK;
	}
	STDMETHOD(Draw)(IAVSRenderer* pRenderer, double dFrame)
	{
		if (NULL != pRenderer)
		{
			pRenderer->put_Width(m_dWidthMM);
			pRenderer->put_Height(m_dHeightMM);

			BSTR bsFile = m_strFilePath.AllocSysString();
			pRenderer->DrawImageFromFile(bsFile, m_dLeft, m_dTop, m_dWidth, m_dHeight);
			SysFreeString(bsFile);
		}
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

			pRendererBase->put_Width(m_dWidthMM);
			pRendererBase->put_Height(m_dHeightMM);

			BSTR bsFile = m_strFilePath.AllocSysString();
			pRendererBase->DrawImageFromFile(bsFile, m_dLeft, m_dTop, m_dWidth, m_dHeight);
			SysFreeString(bsFile);

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