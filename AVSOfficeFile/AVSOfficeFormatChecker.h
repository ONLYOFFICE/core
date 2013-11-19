#pragma once
#include "resource.h" 
#include "..\Common\OfficeFileFormats.h"
#include "TemporaryCS.h"

[
	object,
	uuid("498741e0-b8a1-11de-8a39-0800200c9a66"),
	dual,	helpstring("IAVSDocumentConverter Interface"),
	pointer_default(unique)
]
__interface IAVSOfficeFormatChecker : IDispatch
{
	[id(10), helpstring("method copy")] HRESULT GetFileFormat([in] BSTR bstrFilename, [out,retval] LONG* pnFormat);
	[id(30), helpstring("method copy")] HRESULT CanConvert([in] LONG nSource, [in] LONG nDestination ,[out,retval] VARIANT_BOOL* pbResult);
	[id(40), helpstring("method copy")] HRESULT CanView([in] LONG nFileFormat, [out,retval] VARIANT_BOOL* pbResult);
};

[
	coclass,
	default(IAVSOfficeFormatChecker),
	event_receiver(native),
	threading(apartment),
	vi_progid("AVSOfficeUniversalConverter.AVSDocume"),
	progid("AVSOfficeUniversalConverter.AVSDocu.1"),
	version(1.0),
	uuid("58bf6fc0-b8a1-11de-8a39-0800200c9a66"),
	helpstring("AVSOfficeFormatChecker Class")
]
class ATL_NO_VTABLE CAVSOfficeFormatChecker : public IAVSOfficeFormatChecker
{
private:CRITICAL_SECTION m_csThread;
public:
	CAVSOfficeFormatChecker()
	{
	}

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		InitializeCriticalSection(&m_csThread);
		return S_OK;
	}

	void FinalRelease()
	{
		DeleteCriticalSection(&m_csThread);
	}
public:
	STDMETHOD(GetFileFormat)( BSTR bstrFilename, LONG* pnFormat)
	{
		AVSOfficeFormatChecker avsOfficeFormatChecker;
		CTemporaryCS oCS(&m_csThread);
		if( NULL == pnFormat )
			return S_FALSE;
		(*pnFormat) =avsOfficeFormatChecker.GetFileFormat( CString(bstrFilename) );
		return S_OK;
	}
	STDMETHOD(CanConvert)( LONG nSource, LONG nDestination , VARIANT_BOOL* bResult)
	{
		CTemporaryCS oCS(&m_csThread);
		AVSOfficeFormatChecker avsOfficeFormatChecker;
		if( NULL == bResult )
			return S_FALSE;
		if( false == avsOfficeFormatChecker.CanConvert( nSource, nDestination ) )
			(*bResult) = VARIANT_FALSE;
		else
			(*bResult) = VARIANT_TRUE;
		return S_OK;
	}
	STDMETHOD(CanView)( LONG nFileFormat , VARIANT_BOOL* bResult)
	{
		AVSOfficeFormatChecker avsOfficeFormatChecker;
		CTemporaryCS oCS(&m_csThread);
		if( NULL == bResult )
			return S_FALSE;
		if( false == avsOfficeFormatChecker.CanView( nFileFormat ) )
			(*bResult) = VARIANT_FALSE;
		else
			(*bResult) = VARIANT_TRUE;
		return S_OK;
	}
};