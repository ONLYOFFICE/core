// AVSDocumentConverter.h : Declaration of the CAVSDocumentConverter

#pragma once
#include "resource.h"       // main symbols
#include <atlcoll.h>
#include "ConverterDefines.h"
#include "DocumentConverterManager.h"
#include "EventSource.h"
// шифрование XML
#include "../Common/RSA/XMLDecoder.h"

#ifdef _DEBUG
//#define VALIDATE_INPUT_XML
#endif

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif


// IAVSDocumentConverter
[
	object,
	uuid("3A97A431-0C6B-4A1C-83C0-C0C576201DCB"),
	dual,	helpstring("IAVSDocumentConverter Interface"),
	pointer_default(unique)
]
__interface IAVSOfficeUniversalConverter : IDispatch
{
	[id(100), helpstring("method Start")] HRESULT Start([in] BSTR sXMLString);
	[id(110), helpstring("method Stop")] HRESULT Stop(void);

	[id(120), helpstring("method Suspend")] HRESULT Suspend(void);
	[id(130), helpstring("method Resume")] HRESULT Resume(void);

	[propget, id(140), helpstring("property Status")] HRESULT Status([out, retval] LONG* pVal);

	[id(150), helpstring("method SetAdditionalParam")] HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(160), helpstring("method GetAdditionalParam")] HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);

	[id(180), helpstring("method GetEstimatedTime")] HRESULT GetEstimatedTime([out, retval] DOUBLE* pVal);
	[id(190), helpstring("method GetEstimatedTime")] HRESULT GetElapsedTime([out, retval] DOUBLE* pVal);

	[id(200), helpstring("method SetThreadType")] HRESULT SetThreadType([in] LONG NewVal);
	[id(210), helpstring("method SetThreadPriority")] HRESULT SetThreadPriority([in] LONG NewVal);

	[id(50), propget] HRESULT OfficeFilesCache([out, retval] IUnknown** ppunkVal);
	[id(50), propput] HRESULT OfficeFilesCache([in] IUnknown* punkVal);

	[id(60), propget] HRESULT TempDirectory( [out, retval] BSTR* pbstrDir );
	[id(60), propput] HRESULT TempDirectory( [in] BSTR bstrDir );

	[id(10000), hidden]		HRESULT	_OnEvent(void);
};


// _IAVSDocumentConverterEvents
[
	dispinterface,
	uuid("318D9D87-2889-4C6F-9292-852D4FEBABB1"),
	helpstring("_IAVSDocumentConverterEvents Interface")
]
__interface _IAVSOfficeUniversalConverterEvents
{
	//[id(10), helpstring("method OnProcess")] HRESULT OnProcess([in] LONG lPercent);
	//[id(20), helpstring("method OnError")] HRESULT OnError([in] LONG lError,[in] BSTR XmlDescription,[in, out, ref] SHORT* Cancel );
	////[id(25), helpstring("method OnWarning")] HRESULT OnWarning([in] LONG nID,[in] BSTR bstrXmlDescription);
	//[id(30), helpstring("method OnComplete")] HRESULT OnComplete();

	//[id(40), helpstring("method OnDestinationFileStart")] HRESULT OnDestinationFileStart([in] BSTR NewFileName);
	//[id(50), helpstring("method OnDestinationFileComplete")] HRESULT OnDestinationFileComplete([in] BSTR FileName);
	//[id(60), helpstring("method OnProcessFile")] HRESULT OnProcessFile([in] LONG lPercent, [in] BSTR FileName);

	[id(10), helpstring("method OnProcess")] HRESULT OnProcess([in] LONG lPercent);
	[id(20), helpstring("method OnError")] HRESULT OnErrorWide([in] BSTR XmlDescription,[in, out, ref] SHORT* Cancel );
	[id(800), helpstring("method OnError")] HRESULT OnError([in] LONG nError,[in] LONG nFileId);

	//[id(25), helpstring("method OnWarning")] HRESULT OnWarning([in] LONG nID,[in] BSTR bstrXmlDescription);
	[id(30), helpstring("method OnComplete")] HRESULT OnComplete();

	[id(40), helpstring("method OnDestinationFileStart")] HRESULT OnDestinationFileStart([in] LONG nNewFileName);
	[id(50), helpstring("method OnDestinationFileComplete")] HRESULT OnDestinationFileComplete([in] LONG nFileName);
	[id(60), helpstring("method OnProcessFile")] HRESULT OnProcessFile([in] LONG nPercent, [in] LONG nFileName);
};


// CAVSDocumentConverter

[
	coclass,
	default(IAVSOfficeUniversalConverter, _IAVSOfficeUniversalConverterEvents),
	threading(apartment),
	event_source(com),
	vi_progid("AVSOfficeUniversalConverter.AVSDocume"),
	progid("AVSOfficeUniversalConverter.AVSDocu.1"),
	version(1.0),
	uuid("26DB43F6-E73E-4332-94B5-93784EFDBCC0"),
	helpstring("AVSDocumentConverter Class")
]
class ATL_NO_VTABLE CAVSOfficeUniversalConverter : public IAVSOfficeUniversalConverter
	, public CEventSource<CAVSOfficeUniversalConverter, IAVSOfficeUniversalConverter, &__uuidof(IAVSOfficeUniversalConverter)>
{
private:
	CRITICAL_SECTION m_csThread;
	DocumentConverterManager m_oConvertManager;
	LONG m_lLastProcess;
	IAVSOfficeFilesCache* m_piFilesCache;
	CString m_sTempDirectory;
	CXMLDecoder	m_oDecoder;
	bool m_bStop;//чтобы не отправлялись event кроме OnComplete при вызове Stop
public:
	CAVSOfficeUniversalConverter():m_lLastProcess(-1), m_piFilesCache(NULL)
	{
		m_hNeedEvent = NULL;
		m_hNeedEvent = CreateEvent( NULL, FALSE, FALSE, NULL);
	}
	__event __interface _IAVSOfficeUniversalConverterEvents;


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		APIEventsOn( 200 );
		InitializeCriticalSection(&m_csThread);
		return S_OK;
	}

	void FinalRelease()
	{
		APIEventsOff();
		StopAndWait();
		RELEASEHANDLE( m_hNeedEvent );
		RELEASEINTERFACE( m_piFilesCache );
		DeleteCriticalSection(&m_csThread);
	}
	void OnEvent (void)
		{
			_OnEvent();
		}
public:
	STDMETHOD(Start)(BSTR sXMLString);
	STDMETHOD(Stop)(void);
	STDMETHOD(Suspend)(void);
	STDMETHOD(Resume)(void);
	STDMETHOD(get_Status)(LONG* pVal);
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue);
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue);
	STDMETHOD(GetEstimatedTime)(DOUBLE* pVal);
	STDMETHOD(GetElapsedTime)(DOUBLE* pVal);
	STDMETHOD(SetThreadType)(LONG nNewVal);
	STDMETHOD(SetThreadPriority)(LONG nNewVal);
	STDMETHOD(get_OfficeFilesCache)(IUnknown** ppunkVal);
	STDMETHOD(put_OfficeFilesCache)(IUnknown* punkVal);
	STDMETHOD(get_TempDirectory)( BSTR* pbstrDir );
	STDMETHOD(put_TempDirectory)( BSTR bstrDir );
	STDMETHOD(_OnEvent)(void);
private:
	#ifdef VALIDATE_INPUT_XML
	STDMETHOD(ValidateDocumentConverterXML)(BSTR sXMLString);
	#endif
	void CAVSOfficeUniversalConverter::StopAndWait(void);
};