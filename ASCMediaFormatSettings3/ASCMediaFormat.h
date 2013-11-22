// ASCMediaFormat.h : Declaration of the CASCMediaFormat

#pragma once
#include "resource.h"       // main symbols
#include "MediaFormatDefine.h"
#include "MediaFormatXMLConst.h"
#define DONT_USE_GET_DIGIT
#include "XMLUtils.h"
#include "ATLDefine.h"

// IAVSMediaFormat
[object, uuid("097AB95D-451B-45F7-A8DD-9F8CAA861AF6"), dual, pointer_default(unique)]
__interface IAVSMediaFormat : IDispatch
{
	//см. "MediaFormatDefine.h" define с префиксом MEDIAFORMAT_
	[id(100000 + 1), propget]		HRESULT MediaType([out, retval] LONG* pVal);
	[id(100000 + 2)]				HRESULT CreateDuplicate([out,retval] IAVSMediaFormat** pMediaFormat);

	[id(100000 + 3)]				HRESULT SetDefaultProperties(void);

	[id(100000 + 4)]				HRESULT SetPropertiesByFormat([in] IAVSMediaFormat* pMediaFormat);

	[id(100000 + 5)]				HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(100000 + 6)]				HRESULT GetAdditionalParam([in] BSTR ParamName, [out,retval] VARIANT* ParamValue);
};


template <class CoT, class T>
class CAVSMediaFormatImpl : public IDispatchImpl<T>
{
public:
	CAVSMediaFormatImpl()
		: m_lMediaType(MEDIAFORMAT_UNKNOWN)
	{
		
	}
	STDMETHOD(get_MediaType)(long *pVal)
	{
		*pVal = m_lMediaType;
		return S_OK;
	}
	STDMETHOD(CreateDuplicate)(IAVSMediaFormat** pMediaFormat)
	{
		if (NULL==pMediaFormat)
			return S_OK;
		HRESULT	hResult = CoCreateInstance(__uuidof(CoT), NULL, CLSCTX_ALL, __uuidof(IAVSMediaFormat), (void**)pMediaFormat);
		return S_OK;
	}
	STDMETHOD(SetDefaultProperties)(void)
	{
		return S_OK;
	}
	STDMETHOD(SetPropertiesByFormat)(IAVSMediaFormat* pMediaFormat)
	{
		return S_OK;
	}
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		CString sParamName; sParamName = ParamName;
		if (g_cpszXMLSettingParamName==sParamName)
		{
			CString sXML; sXML = ParamValue.bstrVal;
			LoadFromXML(sXML);
		}
		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		CString sParamName; sParamName = ParamName;
		if (g_cpszXMLSettingParamName==sParamName)
		{
			CString sXML; 
			SaveToXML(sXML);
			ParamValue->vt = VT_BSTR;
			ParamValue->bstrVal = sXML.AllocSysString();
		}
		else if (g_cpszCLSIDParamName==sParamName)
		{
			LPOLESTR lpswzCLSID = NULL;
			HRESULT hr = StringFromCLSID(__uuidof(CoT), &lpswzCLSID);
			if (S_OK!=hr)
				return S_FALSE;

			CStringW swCLSID = lpswzCLSID;
			ParamValue->vt = VT_BSTR;
			ParamValue->bstrVal = swCLSID.AllocSysString();
		}				
		return S_OK;
	}

protected:
	long m_lMediaType;

	void SaveToXML(CString &sXMLData)
	{
		XmlUtils::CDispatchXmlWriter oXMLWriter;
		oXMLWriter.WriteNodeBegin(g_cpszSettingNodeName);
		
		oXMLWriter.WriteNodeBegin(g_cpszStandardSettingNodeName);
		XmlUtils::CDispatchPropertyList arProp;
		XmlUtils::GetPropertyList(this, arProp, INVOKE_PROPERTYGET);
		oXMLWriter.WritePropertyList(arProp);
		oXMLWriter.WriteNodeEnd(g_cpszStandardSettingNodeName);

		CString sExtSetting;
		SaveExtendedSettingToXML(sExtSetting);
		oXMLWriter.WriteString(sExtSetting);
		oXMLWriter.WriteNodeEnd(g_cpszSettingNodeName);
		
		sXMLData = oXMLWriter.GetXmlString();
		sXMLData.Replace(_T("><"), _T(">\r\n<"));
	}
	void LoadFromXML(const CString &sXMLData)
	{
		XmlUtils::CDispatchXmlReader oXmlReader;
		oXmlReader.SetXmlString(sXMLData);
		oXmlReader.ReadNode(g_cpszSettingNodeName);

		XmlUtils::CDispatchPropertyList arProp;
		oXmlReader.GetPropertyList(g_cpszStandardSettingNodeName, arProp);
		IDispatch *pdispThis = NULL;

		this->QueryInterface(__uuidof(IDispatch), (void**)&pdispThis);
		XmlUtils::ApplyPropertyList(pdispThis, arProp);
		RELEASEINTERFACE(pdispThis);

		oXmlReader.ReadNode(g_cpszExtendedSettingNodeName);
		LoadExtendedSettingFromXML(oXmlReader.ReadNodeXml());
	}
	//два следующих метода можно переопределить в порождаемых классах, чтобы реализовать
	//сохранение и загрузку параметров, которые не получиться сохранить автоматически из IDispatch
	virtual void SaveExtendedSettingToXML(CString &sXMLData)
	{
		XmlUtils::CXmlWriter oXMLWriter;
		oXMLWriter.WriteNodeBegin(g_cpszExtendedSettingNodeName);
		oXMLWriter.WriteNodeEnd(g_cpszExtendedSettingNodeName);
		sXMLData = oXMLWriter.GetXmlString();
	}
	virtual void LoadExtendedSettingFromXML(const CString &sXMLData)
	{
		XmlUtils::CXmlReader oXmlReader;
		oXmlReader.SetXmlString(sXMLData);
	}
};

