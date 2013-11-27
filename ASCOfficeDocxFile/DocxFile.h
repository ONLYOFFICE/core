#pragma once
#include "resource.h"
#include "../Common/OfficeFileTemplate.h"
#include "OfficeUtilsEvents.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif


#import "..\Redist\ASCOfficeUtils.dll" raw_interfaces_only


using namespace ASCOfficeUtils;

/*
Compression levels:

-1 - Default.
0 - No compression.
1 - Best speed.
...
9 - Best compression.
*/

#define ZIP_NO_COMPRESSION         0
#define ZIP_BEST_SPEED             1
#define ZIP_BEST_COMPRESSION       9
#define ZIP_DEFAULT_COMPRESSION  (-1)

// IOfficeDocxFile
[
	object,
	uuid("0FE32AEF-E7C5-42BD-A752-AC7F809D5E20"),
	dual,	helpstring("IOfficeDocxFile Interface"),
	pointer_default(unique)
]
__interface IOfficeDocxFile : IAVSOfficeFileTemplate
{
	[id(20), helpstring("method AddObjects")] HRESULT AddObjects([in] BSTR sSrcPath, [in]BSTR sXMLOptions);	
};


// CDocxFile

[
	coclass,
	default(IOfficeDocxFile, _IAVSOfficeFileTemplateEvents),
	threading(apartment),
	event_source(com),
	vi_progid("AVSOfficeDocxFile.DocxFile"),
	progid("AVSOfficeDocxFile.DocxFile.1"),
	version(1.0),
	uuid("CF346C1A-8384-4516-9DD4-11219FB23799"),
	helpstring("OfficeDocxFile Class")
]
class ATL_NO_VTABLE COfficeDocxFile :	public IOfficeDocxFile
{
public:
	COfficeDocxFile();

    __event __interface _IAVSOfficeFileTemplateEvents;
	__event __interface _IAVSOfficeFileTemplateEvents2;

public:
	DECLARE_PROTECT_FINAL_CONSTRUCT()
	HRESULT FinalConstruct();
	void FinalRelease();

public:
	STDMETHOD(LoadFromFile)(BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions);
	STDMETHOD(SaveToFile)(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions);
	STDMETHOD(AddObjects)(BSTR sSrcPath, BSTR sXMLOptions);

private:
	IOfficeUtils* m_pOfficeUtils;
};