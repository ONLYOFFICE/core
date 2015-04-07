// OfficePPTFile3.h : Declaration of the CPPTFile3
#pragma once
#include "resource.h"       // main symbols

#include "../../../Common/OfficeFileTemplate.h"
#include "../../../Common/ATLDefine.h"
#include "../../../Common/ASCUtils.h"

#include "../../../Common/VideoFileErrorDescription.h"

#include "../PPTFormatLib/PPTFormatLib.h"

// IAVSOfficePPTFile
[object, uuid("01AB4BA0-7AE9-4539-8B12-608233D307BC"), dual, pointer_default(unique)]
__interface IAVSOfficePPTFile :  IAVSOfficeFileTemplate
{
	[id(100 + 0), propget] HRESULT DrawingXml( [out, retval] BSTR* pbstrXml );
    [id(100 + 1), propget] HRESULT TempDirectory( [out, retval] BSTR* pbstrDir );
    [id(100 + 1), propput] HRESULT TempDirectory( [in] BSTR bstrDir );
	
	[id(1000 + 0)] HRESULT Is_PPTFile([in] BSTR fileName, [out, retval] VARIANT_BOOL* Result);
	
	[id(1000 + 1)] HRESULT GetDVDXml([out, retval] BSTR* bstrPTTXml);
	[id(1000 + 2)] HRESULT GetBluRayXml([out, retval] BSTR* bstrDVDXml);

	[id(2000 + 0)] HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(2001 + 1)] HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);
};


// _IAVSOfficePPTFileEvents
[dispinterface,	uuid("DF3BE363-8404-4173-830A-984BF11790F5")]
__interface _IAVSOfficePPTFileEvents
{
	[id(1)] HRESULT ProcessVideoFile([in] LONG ID, [in] LONG Percent, [in, out, ref] SHORT* Cancel);
};	

// CAVSOfficePPTFile
[coclass, uuid("FC1AD867-FFA6-454a-82A0-1538465A55EB"), event_source(com), threading(apartment), vi_progid("AVSOfficePPTFile.OfficePPTFile"), progid("AVSOfficePPTFile.OfficePPTFile.1"), version(1.0)]
class ATL_NO_VTABLE CAVSOfficePPTFile : 
	public IAVSOfficePPTFile, 
	public CAVSATLError
{
public:
	CAVSOfficePPTFile() : 
	  CAVSATLError(__uuidof(CAVSOfficePPTFile), __uuidof(IAVSOfficePPTFile)), m_hSynchMutex(NULL)
	{
	}

	__event __interface _IAVSOfficePPTFileEvents;
	__event __interface _IAVSOfficeFileTemplateEvents2;

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{	
		m_hSynchMutex = CreateMutex(NULL, FALSE, NULL);

		m_Status = 0;

		return S_OK;
	}

	void FinalRelease()
	{
		RELEASEHANDLE ( m_hSynchMutex );
	}

private:

	COfficePPTFile PPTFile;

	HANDLE m_hSynchMutex;
	LONG m_Status;
	
public:
	STDMETHOD(get_DrawingXml)(BSTR* pbstrXml)
	{
		if (NULL == pbstrXml)
			return S_FALSE;
		return S_OK;
	}
	STDMETHOD(get_TempDirectory)(BSTR* pbstrDir)
	{
		if (NULL == pbstrDir)
			return S_FALSE;

		*pbstrDir = PPTFile.get_TempDirectory().AllocSysString();
		return S_OK;
	}
	STDMETHOD(put_TempDirectory)(BSTR bstrDir)
	{
		PPTFile.put_TempDirectory((CStringW)bstrDir);
		return S_OK;
	}
	STDMETHOD(Is_PPTFile)(BSTR fileName, VARIANT_BOOL* Result)
	{
		CSynchAccess oSynchAccess(m_hSynchMutex);
		return S_OK;
	}

	STDMETHOD(LoadFromFile)(BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions)
	{
		CSynchAccess oSynchAccess(m_hSynchMutex);

		return PPTFile.LoadFromFile(CString(sSrcFileName), CString(sDstPath), CString(sXMLOptions));
	}
	STDMETHOD(SaveToFile)(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions)
	{
		return S_OK;
	}
	
	// для конвертации с поддержкой интерактива...	
	STDMETHOD(GetDVDXml)(BSTR* bstrDVDXml)
	{
		return S_OK;
	}
	STDMETHOD(GetBluRayXml)(BSTR* bstrBluRayXml)
	{
		return S_OK;
	}

	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		if (NULL == ParamValue)
			return S_FALSE;

		PPTFile.GetAdditionalParam(CString(ParamName), ParamValue);

		return S_OK;
	}
};

