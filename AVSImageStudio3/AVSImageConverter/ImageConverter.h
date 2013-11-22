// AVSXMLImageConverter.h : Declaration of the CAVSXMLImageConverter

#pragma once
#include "resource.h"       // main symbols
#include "ImageSaverManager.h"
#include "Timer.h"
#include "ImageSource.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

//врем€ срабатывани€ таймера
#define RESPONSIVENESS 40

// IAVSXMLImageConverter
[
	object,
	uuid("E1E170C6-FB1E-4CA3-8FEB-0E0D63F6225D"),
	dual,	helpstring("IAVSXMLImageConverter Interface"),
	pointer_default(unique)
]
__interface IAVSXMLImageConverter : IDispatch
{
	[id(100), helpstring("method Start")] HRESULT Start([in] BSTR sXMLString);
	[id(110), helpstring("method Stop")] HRESULT Stop(void);
	[id(120), helpstring("method Suspend")] HRESULT Suspend(void);
	[id(130), helpstring("method Resume")] HRESULT Resume(void);
	[propget, id(140), helpstring("property Status")] HRESULT Status([out, retval] LONG* pVal);
	[id(150), helpstring("method SetAdditionalParam")] HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(160), helpstring("method GetAdditionalParam")] HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);
	[id(170), helpstring("method GetCurrentPicture")] HRESULT GetCurrentPicture([in] LONG lWidth,[in] LONG lHeight,[out, retval] IUnknown** ppVal);
	[id(180), helpstring("method GetEstimatedTime")] HRESULT GetEstimatedTime([out, retval] FLOAT* pVal);
	[id(190), helpstring("method GetEstimatedTime")] HRESULT GetElapsedTime([out, retval] FLOAT* pVal);
	[id(200), helpstring("method SetThreadType")] HRESULT SetThreadType([in] LONG NewVal);
	[id(210), helpstring("method SetThreadPriority")] HRESULT SetThreadPriority([in] LONG NewVal);
	[propget, id(220), helpstring("property IAVSImageFilesCache2")] HRESULT FilesCache([out, retval] IUnknown** ppVal);
	[propput, id(220), helpstring("property IAVSImageFilesCache2")] HRESULT FilesCache([in] IUnknown* pNewVal);
	[id(10000), hidden]		HRESULT	_OnTimer(void);
};

// _IAVSXMLImageConverterEvents
[
	dispinterface,
	uuid("F0B0E4DF-C356-49FD-8074-C3A75C0C5C02"),
	helpstring("_IAVSXMLImageConverterEvents Interface")
]
__interface _IAVSXMLImageConverterEvents
{
	[id(10), helpstring("method OnProcess")] HRESULT OnProcess([in] LONG lPercent);
	[id(20), helpstring("method OnError")] HRESULT OnError([in] LONG lError);
	[id(30), helpstring("method OnComplete")] HRESULT OnComplete();
	[id(40), helpstring("method OnComplete")] HRESULT OnCompleteFile( [in] BSTR bstrCompleteFile );
};


// CAVSXMLImageConverter

[
	coclass,
	default(IAVSXMLImageConverter, _IAVSXMLImageConverterEvents),
	threading(apartment),
	event_source(com),
	vi_progid("AVSImageConverter.AVSXMLImageConverter"),
	progid("AVSImageConverter.AVSXMLImageConverte.1"),
	version(1.0),
	uuid("55634944-3091-4D54-9BC2-090148A97A9D"),
	helpstring("AVSXMLImageConverter Class")
]
class ATL_NO_VTABLE CAVSXMLImageConverter :
	public IAVSXMLImageConverter, public CTimer<CAVSXMLImageConverter, IAVSXMLImageConverter, &__uuidof(IAVSXMLImageConverter)>
{
	//»спользуетс€ только в GetCurImage()
	//ѕри конвертации не используетс€, т.к. "опыт показал", что картинки 2000*2000
	//занимают много места и никто их в кеш не хранит, и если кеш использовать при конвертации,
	//то они все равно будут открыватьс€ и только загруз€т пам€ть
	ImageStudio::IAVSImageCache* m_piFilesCache;
	MediaCore::IAVSUncompressedVideoFrame* m_piLogoImage;
	IAVSXMLImageSource* m_piImageSource;
	INT32 m_nLogoParam;
	bool m_bAddWM;

	CRITICAL_SECTION m_csThread;
	ImageSaverManager m_oConvertManager;
	LONG m_lLastProcess;
	CErrorItem m_oCurError; // доступен в обработчике OnError
public:
	CAVSXMLImageConverter(): m_piFilesCache( NULL ), m_piLogoImage( NULL ),m_piImageSource( NULL ),m_lLastProcess(-1),m_bAddWM(false)
	{
		m_nLogoParam = IMAGECONVERTER_LOGOPARAM_VERT_CENTER | IMAGECONVERTER_LOGOPARAM_HOR_CENTER; 
		CoCreateInstance( __uuidof(CAVSXMLImageSource) , NULL, CLSCTX_INPROC, __uuidof(IAVSXMLImageSource), (void**)(&m_piImageSource));

	}

	__event __interface _IAVSXMLImageConverterEvents;


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		InitializeCriticalSection(&m_csThread);
		APITimerOn( RESPONSIVENESS );
		return S_OK;
	}

	void FinalRelease()
	{
		APITimerOff();
		Stop();
		DeleteCriticalSection(&m_csThread);
		RELEASEINTERFACE(m_piLogoImage);
		RELEASEINTERFACE(m_piFilesCache);
		RELEASEINTERFACE(m_piImageSource);
	}
	virtual	void OnTimer()
	{
		_OnTimer();
	}
public:
	STDMETHOD(Start)(BSTR sXMLString);
	STDMETHOD(Stop)(void);
	STDMETHOD(Suspend)(void);
	STDMETHOD(Resume)(void);
	STDMETHOD(get_Status)(LONG* pVal);
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue);
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue);
	STDMETHOD(GetCurrentPicture)( LONG lWidth, LONG lHeight, IUnknown** ppVal);
	STDMETHOD(GetEstimatedTime)(FLOAT* pVal);
	STDMETHOD(GetElapsedTime)(FLOAT* pVal);
	STDMETHOD(SetThreadType)(LONG NewVal);
	STDMETHOD(SetThreadPriority)(LONG NewVal);
	STDMETHOD(get_FilesCache)( IUnknown** ppVal);
	STDMETHOD(put_FilesCache)( IUnknown* pNewVal);
	STDMETHOD(_OnTimer)(void);
};

