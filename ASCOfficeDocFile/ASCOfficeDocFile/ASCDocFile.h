#pragma once

#include "resource.h" 

#include "..\DocDocxConverter\Converter.h"
#include "..\..\Common\OfficeFileTemplate.h"
#include "..\DocxDocConverter\FileConverter.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

// IOfficeDocFile
[
	object,
	uuid("C694C5ED-201C-46BA-B159-1D7C907FA023"),
	dual,	helpstring("IOfficeDocFile Interface"),
	pointer_default(unique)
]
__interface IOfficeDocFile : IAVSOfficeFileTemplate
{
};

// COfficeDocFile

[
	coclass,
	default(IOfficeDocFile, _IAVSOfficeFileTemplateEvents),
	threading(apartment),
	event_source(com),
	vi_progid("AVSOfficeDocFile.DocFile"),
	progid("AVSOfficeDocFile.DocFile.1"),
	version(1.0),
	uuid("CFABE3C9-A61B-4C20-86F4-A19C8FA1546C"),
	helpstring("OfficeDocFile Class")
]

class ATL_NO_VTABLE COfficeDocFile: public IOfficeDocFile
{
public:

	COfficeDocFile()
	{

	}

	__event __interface _IAVSOfficeFileTemplateEvents;
	__event __interface _IAVSOfficeFileTemplateEvents2;

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

	// OfficeFileTemplate Methods
public:

	STDMETHOD(LoadFromFile)(BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions);
	STDMETHOD(SaveToFile)(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions);

protected:

	static void OnProgressFunc (LPVOID lpParam, long nID, long nPercent);
	static void OnProgressExFunc (LPVOID lpParam, long nID, long nPercent, short* pStop);
};