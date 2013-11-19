#pragma once
#include "resource.h"       // main symbols
#include "..\\..\\Common\\OfficeFileTemplate.h"
#include "..\\..\\Common\\OfficeFileFormats.h"
#include "..\\..\\Common\\OfficeDefines.h"
#include "..\\..\\..\\..\\..\\Common\\XmlUtils.h"
#include "..\\..\\..\\..\\..\\Common\\ATLDefine.h"
#include "..\\..\\..\\AVSVideoStudio3\\Common\\MediaFormatDefine.h"
#include <boost/filesystem.hpp>
#include "./../../Common/OfficeFileErrorDescription.h"


#define ONE_INCH 2.54

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif


#ifdef _DEBUG
#import ".\..\..\AVSOfficeUtils\AVSOfficeUtils\Debug\AVSOfficeUtils.dll" raw_interfaces_only
#else
#import ".\..\..\..\..\Redist\AVSOfficeStudio\AVSOfficeUtils.dll" raw_interfaces_only
#endif



// IImageFile
[
	object,
	uuid("702E7704-B36E-484D-8398-EB46EA675E00"),
	dual,	helpstring("IImageFile Interface"),
	pointer_default(unique)
]
__interface IOfficeImageFile : IAVSOfficeFileTemplate
{
	[id(101), propget] HRESULT CommandRenderer([out, retval] IUnknown** ppunkRend);
	[id(101), propput] HRESULT CommandRenderer([in] IUnknown* punkRend);
	[id(110)] HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(120)] HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);
};



// CImageFile

[
	coclass,
	default(IOfficeImageFile,_IAVSOfficeFileTemplateEvents),
	threading(apartment),
	event_source(com),
	vi_progid("AVSOfficeImageFile.ImageFile"),
	progid("AVSOfficeImageFile.ImageFile.1"),
	version(1.0),
	uuid("B5657647-50CC-4AF4-8723-354A0F680C94"),
	helpstring("ImageFile Class")
]
class ATL_NO_VTABLE COfficeImageFile : public IOfficeImageFile
{
public:
	COfficeImageFile();
	__event __interface _IAVSOfficeFileTemplateEvents;
	__event __interface _IAVSOfficeFileTemplateEvents2;
	DECLARE_PROTECT_FINAL_CONSTRUCT()
	HRESULT FinalConstruct();
	void FinalRelease();

public:
	STDMETHOD(LoadFromFile)(BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions);
	STDMETHOD(SaveToFile)(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions);

	STDMETHOD(get_CommandRenderer)(IUnknown** ppunkRend);
	STDMETHOD(put_CommandRenderer)(IUnknown* ppunkRend);
	STDMETHOD(SetAdditionalParam)( BSTR ParamName, VARIANT ParamValue);
	STDMETHOD(GetAdditionalParam)( BSTR ParamName, VARIANT* ParamValue);
private:
	void UpdateGdiPlusRenderer(double dWidthMm, double dHeightMm,AVSMediaCore3::IAVSUncompressedVideoFrame** piImage,AVSGraphics::IAVSGraphicsRenderer** piRend);

private:
	void createOriginDocx(const boost::filesystem::wpath& path) const;
	const unsigned long LoadFromResource(LPCWSTR lpResName, LPCWSTR lpResType, LPCWSTR fileName) const;

private:
	AVSGraphics::IAVSEffectPainter        *m_piEffectPainter;
	AVSGraphics::IAVSDocumentRenderer *	m_piCommandsRenderer;
	AVSOfficeUtils::IOfficeUtils*						m_pOfficeUtils;
	int m_nSaveMode;
	//нужно сохран€ть m_piImageFile, чтобы была возможность сохран€ть в один файл несколько source многокрытным вызовом SaveToFile
	AVSImageFile3::IImageFile3Ptr m_piImageFile;
	int m_nCounter;
	bool m_bSaveRendererStopped;
};