#pragma once
#include "resource.h"       // main symbols
#include <string>
#include <./../ASCOfficeODFFile/include/cpdoccore/common/boost_filesystem_version.h>
#include "./../Common/OfficeFileTemplate.h"
#include "./../Common/OfficeFileErrorDescription.h"
#include "OdtEvent.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif


#ifdef _DEBUG
	#import "..\Redist\ASCOfficeUtils.dll" raw_interfaces_only
#else
	#import "..\Redist\ASCOfficeUtils.dll" raw_interfaces_only
#endif

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


// IOdtFile
[
	object,
	uuid("7DEC2313-C482-4039-A5C6-083F1EDD86AB"),
	dual,	helpstring("IOdtFile Interface"),
	pointer_default(unique)
]
__interface IOdtFile : IAVSOfficeFileTemplate
{
};


// COdtFile

[
	coclass,
	event_source(com),
	default(IOdtFile),
	threading(apartment),
	vi_progid("AVSOdtFile.OdtFile"),
	progid("AVSOdtFile.OdtFile.1"),
	version(1.0),
	uuid("04596A11-01C4-48D9-B020-B08E3F443F0F"),
	helpstring("OdtFile Class")
]
class ATL_NO_VTABLE COdtFile : public IOdtFile, public Odt::IOdtEvent
{
public:
	__event __interface _IAVSOfficeFileTemplateEvents2;
	virtual bool Progress(long ID, long Percent);

	STDMETHOD(LoadFromFile)(BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions);
	STDMETHOD(SaveToFile)(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions);

public:
	COdtFile();
	DECLARE_PROTECT_FINAL_CONSTRUCT()
	HRESULT FinalConstruct();
	void FinalRelease();

private:
	HRESULT convert(const std::wstring & srcPath, const std::wstring & dstPath);
	void createOriginDocx(const boost::filesystem::wpath& path) const;
	void createOriginOdt(const boost::filesystem::wpath& path) const;
	const unsigned long LoadFromResource(LPCWSTR lpResName, LPCWSTR lpResType, LPCWSTR fileName) const;
	HRESULT SaveToFileImpl(const std::wstring & srcPath,
											 const std::wstring & srcTempPath,
											 const std::wstring & dstTempPath,
											 const std::wstring & dstFileName);

private:
	AVSOfficeUtils::IOfficeUtils*		m_pOfficeUtils;
    ATL::CComPtr<IAVSOfficeFileTemplate> odfFile_;
};