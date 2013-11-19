#pragma once
#include "resource.h"       // main symbols
#include <string>
#include <boost/filesystem.hpp>
#include ".\..\Common\OfficeFileTemplate.h"
#include ".\..\Common\OfficeFileErrorDescription.h"
#include ".\..\Common\OfficeDefines.h"
#include "XmlEvent.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif


#ifdef _DEBUG
//#import "..\AVSOfficeUtils\AVSOfficeUtils\Debug\AVSOfficeUtils.dll" raw_interfaces_only
#import "..\..\..\Redist\AVSOfficeStudio\AVSOfficeUtils.dll" raw_interfaces_only
#else
#import "..\..\..\Redist\AVSOfficeStudio\AVSOfficeUtils.dll" raw_interfaces_only
#endif


#define ZIP_NO_COMPRESSION         0
#define ZIP_BEST_SPEED             1
#define ZIP_BEST_COMPRESSION       9
#define ZIP_DEFAULT_COMPRESSION  (-1)



// IXmlFile
[
	object,
	//uuid("313EFCD4-D6D2-4E5D-A99A-47890DA1FAAA"),
	uuid("AA15FAE9-AB71-4f01-B3A9-59FC4D12AB34"),
	dual,	helpstring("IXmlFile Interface"),
	pointer_default(unique)
]
__interface IXmlFile : IAVSOfficeFileTemplate
{
};


// CXmlFile

[
	coclass,
	event_source(com),
	default(IXmlFile),
	threading(apartment),
	vi_progid("AVSXmlFile.XmlFile"),
	progid("AVSXmlFile.XmlFile.1"),
	version(1.0),
	//uuid("91D835EB-A37E-4AF7-8B53-F56D353E2161"),
	uuid("DD887DF2-E1A4-492A-9D87-05E0E0BC485D"),
	helpstring("XmlFile Class")
]
class ATL_NO_VTABLE CXmlFile : public IXmlFile, public XmlFormat::IXmlEvent
{
public:
	__event __interface _IAVSOfficeFileTemplateEvents2;
	virtual bool Progress(long ID, long Percent);

	STDMETHOD(LoadFromFile)(BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions);
	STDMETHOD(SaveToFile)(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions);

public:
	CXmlFile();
	DECLARE_PROTECT_FINAL_CONSTRUCT()
	HRESULT FinalConstruct();
	void FinalRelease();

private:
	void createOriginDocx(const boost::filesystem::wpath& path) const;
	const unsigned long LoadFromResource(LPCWSTR lpResName, LPCWSTR lpResType, LPCWSTR fileName) const;

private:
		AVSOfficeUtils::IOfficeUtils*		m_pOfficeUtils;
};