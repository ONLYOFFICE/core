#pragma once
#include "resource.h"       // main symbols
#include "MediaFormatXMLConst.h"
#define DONT_USE_GET_DIGIT
#include "XMLUtils.h"

using namespace XmlUtils;

[object, uuid("0F55D9B8-CCFD-47d6-99FD-235217C5C5D5"), dual, pointer_default(unique)]
__interface IAVSMediaFormatSerialize : IDispatch
{
	[id(190000 + 1)] HRESULT SaveToXML([in] IUnknown *pUnknown, [in] BSTR XMLNodeName,  [out, retval] BSTR *pXML);
	[id(190000 + 2)] HRESULT LoadFromXML([in] BSTR XML, [out, retval] IUnknown **ppUnknown);
};




// CAVSAudioFormat
[ coclass, uuid("9B287A8F-0CC2-4e57-A4D7-A30ADD66F431"), threading(apartment), vi_progid("AVSMediaFormatSettings3.AVSAudioFormat"), progid("AVSMediaFormatSettings3.AVSAudioForma.1"), version(1.0)]
class ATL_NO_VTABLE CAVSMediaFormatSerialize
	: public IAVSMediaFormatSerialize
{
public:
	CAVSMediaFormatSerialize()
	{	
	}

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}	
	void FinalRelease() 
	{
	}
public:
	STDMETHOD(SaveToXML)(IUnknown *pUnknown, BSTR XMLNodeName, BSTR *pXML)
	{
		if (NULL==pXML)
			return S_OK;
		
		*pXML = NULL;
		if (NULL==pUnknown)
			return S_OK;
		IAVSMediaFormat *pMediaFormat = NULL;
		pUnknown->QueryInterface(__uuidof(IAVSMediaFormat), (void**)&pMediaFormat);
		if (NULL==pMediaFormat)
			return S_OK;

		CString sXMLNodeName; sXMLNodeName = XMLNodeName;
		XmlUtils::CDispatchXmlWriter oXMLWriter;
		oXMLWriter.WriteNodeBegin(sXMLNodeName);

		//Ни какого лучшего способа получить CLSID не нашел
		CString sTemp = g_cpszCLSIDParamName; 
		BSTR bsParamName = sTemp.AllocSysString();
		VARIANT val;
		pMediaFormat->GetAdditionalParam(bsParamName, &val);
		SysFreeString(bsParamName);
		sTemp = val.bstrVal;
		SysFreeString(val.bstrVal);
				
		((CXmlWriter *)&oXMLWriter)->WriteNode(g_cpszCLSIDNodeName, sTemp);
		
		sTemp = g_cpszXMLSettingParamName; 
		bsParamName = sTemp.AllocSysString();		
		pMediaFormat->GetAdditionalParam(bsParamName, &val);
		SysFreeString(bsParamName);
		if (NULL!=val.bstrVal)
		{
			sTemp = val.bstrVal;
			oXMLWriter.WriteString(sTemp);
			SysFreeString(val.bstrVal);
		}
		oXMLWriter.WriteNodeEnd(sXMLNodeName);
		RELEASEINTERFACE(pMediaFormat);
		(*pXML) = oXMLWriter.GetXmlString().AllocSysString();
		return S_OK;
	}
	STDMETHOD(LoadFromXML)(BSTR XML, IUnknown **ppUnknown)
	{
		if (NULL==ppUnknown)
			return S_OK;
		XmlUtils::CDispatchXmlReader oXMLReader;
		CString sXMLData; sXMLData = XML;
		oXMLReader.SetXmlString(sXMLData);
		CString sCLSID = oXMLReader.ReadNodeValue(g_cpszCLSIDNodeName);
		
		IAVSMediaFormat *pMediaFormat = NULL;
		if (!CreateObject(sCLSID, &pMediaFormat))
			return S_FALSE;
	
		oXMLReader.ReadNodeValue(g_cpszSettingNodeName);
		CString sNodeXML = oXMLReader.ReadNodeXml();

		CString sTemp = g_cpszXMLSettingParamName; 
		BSTR bsParamName = sTemp.AllocSysString();
		VARIANT val;
		val.vt = VT_BSTR;
		val.bstrVal = sNodeXML.AllocSysString();

		pMediaFormat->SetAdditionalParam(bsParamName, val);
		SysFreeString(bsParamName);
		SysFreeString(val.bstrVal);

		pMediaFormat->QueryInterface(__uuidof(IUnknown), (void**)ppUnknown);
		RELEASEINTERFACE(pMediaFormat);
		return S_OK;
	}
protected:
	static BOOL CreateObject(const CString &sCLSID, IAVSMediaFormat **pFormat)
	{
		CStringW swCLSID; swCLSID = sCLSID;
		CLSID clsid;
		HRESULT hr = CLSIDFromString((WCHAR *)swCLSID.GetBuffer(), &clsid);
		if (S_OK!=hr)
			return FALSE;
		hr = CoCreateInstance(clsid, NULL, CLSCTX_ALL, __uuidof(IAVSMediaFormat), (void**)pFormat);
		return (NULL!=(*pFormat));
	}
};

