#pragma once
#include "resource.h"       // main symbols
#include <string>
#include <boost/filesystem.hpp>
#include ".\..\Common\OfficeFileTemplate.h"
#include ".\..\Common\OfficeFileErrorDescription.h"
#include ".\..\Common\OfficeDefines.h"
#include "TxtEvent.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif



#import "..\Redist\ASCOfficeUtils.dll" raw_interfaces_only



#define ZIP_NO_COMPRESSION         0
#define ZIP_BEST_SPEED             1
#define ZIP_BEST_COMPRESSION       9
#define ZIP_DEFAULT_COMPRESSION  (-1)



// ITxtFile
[
	object,
	uuid("313EFCD4-D6D2-4E5D-A99A-47890DA1FAAA"),
	dual,	helpstring("ITxtFile Interface"),
	pointer_default(unique)
]
__interface ITxtFile : IAVSOfficeFileTemplate
{
};


// CTxtFile

[
	coclass,
	event_source(com),
	default(ITxtFile),
	threading(apartment),
	vi_progid("AVSTxtFile.TxtFile"),
	progid("AVSTxtFile.TxtFile.1"),
	version(1.0),
	uuid("91D835EB-A37E-4AF7-8B53-F56D353E2161"),
	helpstring("TxtFile Class")
]
class ATL_NO_VTABLE CTxtFile : public ITxtFile, public Txt::ITxtEvent
{
public:
	__event __interface _IAVSOfficeFileTemplateEvents2;
	virtual bool Progress(long ID, long Percent);

	STDMETHOD(LoadFromFile)(BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions);
	STDMETHOD(SaveToFile)(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions);

public:
	CTxtFile();
	DECLARE_PROTECT_FINAL_CONSTRUCT()
	HRESULT FinalConstruct();
	void FinalRelease();

private:
	void createOriginDocx(const boost::filesystem::wpath& path) const;
	const unsigned long LoadFromResource(LPCWSTR lpResName, LPCWSTR lpResType, LPCWSTR fileName) const;

private:
		ASCOfficeUtils::IOfficeUtils*		m_pOfficeUtils;
};