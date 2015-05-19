// ImageSource.h : Declaration of the CAVSXMLImageSource

#pragma once
#include "resource.h"       // main symbols
#include <atldefine.h>

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

// IAVSXMLImageSource
[
	object,
	uuid("3F44C0DE-5FEA-4D5C-B5AB-C5C53D1FB8D4"),
	dual,	helpstring("IAVSXMLImageSource Interface"),
	pointer_default(unique)
]
__interface IAVSXMLImageSource : IDispatch
{
	[id(200), helpstring("method Open")] HRESULT LoadImage([in] BSTR sXMLString, [in] LONG nWidth, LONG nHeight, [out,retval] IUnknown** ppImage);
	[propget, id(210), helpstring("property IAVSImageFilesCache2")] HRESULT FilesCache([out, retval] IUnknown** ppVal);
	[propput, id(210), helpstring("property IAVSImageFilesCache2")] HRESULT FilesCache([in] IUnknown* pNewVal);
	[id(220), helpstring("method SetAdditionalParam")] HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(230), helpstring("method GetAdditionalParam")] HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);
};


// _IAVSXMLImageSourceEvents
[
	dispinterface,
	uuid("8C817913-75EB-4819-9361-F86665E01CFC"),
	helpstring("_IAVSXMLImageSourceEvents Interface")
]
__interface _IAVSXMLImageSourceEvents
{
};


// CAVSXMLImageSource

[
	coclass,
	default(IAVSXMLImageSource, _IAVSXMLImageSourceEvents),
	threading(apartment),
	event_source(com),
	vi_progid("AVSImageConverter.AVSXMLImageSource"),
	progid("AVSImageConverter.AVSXMLImageSource.1"),
	version(1.0),
	uuid("71ABA05D-1926-40CB-927E-794CD8E54351"),
	helpstring("AVSXMLImageSource Class")
]
class ATL_NO_VTABLE CAVSXMLImageSource :
	public IAVSXMLImageSource
{
	ImageStudio::IAVSImageCache* m_piFilesCache;
	ImageStudio::IImageTransforms* m_piTransform;
	ImageFile::IImageFile3* m_piImageFile;
	ImageVideoFile::IImageVideoFile4* m_piImageVideoFile;
    CString m_sCurApngFile;
    long m_nLastFrame;
	CRITICAL_SECTION m_csThread;
	INT32 m_nMinWidth;
	INT32 m_nMinHeight;
public:
	CAVSXMLImageSource():m_piFilesCache(NULL),m_piTransform(NULL),m_piImageFile(NULL),m_piImageVideoFile(NULL),m_nMinWidth(-1),m_nMinHeight(-1)
	{
		CoCreateInstance(ImageStudio::CLSID_ImageTransforms, NULL, CLSCTX_INPROC, ImageStudio::IID_IImageTransforms, (void**)(&m_piTransform));
		CoCreateInstance(ImageFile::CLSID_ImageFile3, NULL, CLSCTX_INPROC, ImageFile::IID_IImageFile3, (void**)(&m_piImageFile));
		CoCreateInstance(ImageVideoFile::CLSID_CImageVideoFile3, NULL, CLSCTX_INPROC, ImageVideoFile::IID_IImageVideoFile4, (void**)(&m_piImageVideoFile));
		if( NULL != m_piImageVideoFile )
		{
			VARIANT vtResizeType;
			vtResizeType.vt = VT_I4;
			vtResizeType.lVal = 0; //не применять resize на открытие
			m_piImageVideoFile->SetAdditionalParam( _T("resizetype"), vtResizeType );
			VARIANT vtWithAlpha;
			vtWithAlpha.vt = VT_BOOL;
			vtWithAlpha.lVal = VARIANT_TRUE; //оставлять прозрачность
			m_piImageVideoFile->SetAdditionalParam( _T("drawwithalpha"), vtWithAlpha );
		}
		m_sCurApngFile = _T("");
		m_nLastFrame = -1;
	}

	__event __interface _IAVSXMLImageSourceEvents;


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		InitializeCriticalSection(&m_csThread);
		return S_OK;
	}

	void FinalRelease()
	{
		DeleteCriticalSection(&m_csThread);
		RELEASEINTERFACE(m_piTransform);
		RELEASEINTERFACE(m_piImageFile);
		if( NULL != m_piImageVideoFile )
			m_piImageVideoFile->CloseFile();
		RELEASEINTERFACE(m_piImageVideoFile);
		RELEASEINTERFACE(m_piFilesCache);
	}

public:
	STDMETHOD(LoadImage)( BSTR sXMLString, LONG nWidth, LONG nHeight, IUnknown** ppImage);
	STDMETHOD(get_FilesCache)( IUnknown** ppVal);
	STDMETHOD(put_FilesCache)( IUnknown* pNewVal);
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue);
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue);
};

