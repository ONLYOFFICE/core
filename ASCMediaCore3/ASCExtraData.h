// ASCExtraData.h : Declaration of the CASCExtraData

#pragma once
#include "resource.h"       // main symbols
#include "../Common/AudioDefines.h"       // main symbols

// IASCExtraData
[
	object,
	uuid("F04698FD-7956-4BCF-B579-512CBC1ECB0F"),
	dual,	helpstring("IASCExtraData Interface"),
	pointer_default(unique)
]
__interface IAVSExtraData : IDispatch
{
	[propget, id(208000 + 1)] HRESULT Album([out, retval] BSTR *pVal);
	[propput, id(208000 + 1)] HRESULT Album([in] BSTR newVal);
	[propget, id(208000 + 2)] HRESULT Artist([out, retval] BSTR *pVal);
	[propput, id(208000 + 2)] HRESULT Artist([in] BSTR newVal);
	[propget, id(208000 + 3)] HRESULT Comments([out, retval] BSTR *pVal);
	[propput, id(208000 + 3)] HRESULT Comments([in] BSTR newVal);
	[propget, id(208000 + 4)] HRESULT Copyright([out, retval] BSTR *pVal);
	[propput, id(208000 + 4)] HRESULT Copyright([in] BSTR newVal);
	[propget, id(208000 + 6)] HRESULT Title([out, retval] BSTR *pVal);
	[propput, id(208000 + 6)] HRESULT Title([in] BSTR newVal);
	[propget, id(208000 + 7)] HRESULT Year([out, retval] long *pVal);
	[propput, id(208000 + 7)] HRESULT Year([in] long newVal);
	[propget, id(208000 + 8)] HRESULT URL([out, retval] BSTR *pVal);
	[propput, id(208000 + 8)] HRESULT URL([in] BSTR newVal);
	[propget, id(208000 + 9)] HRESULT Track([out, retval] long *pVal);
	[propput, id(208000 + 9)] HRESULT Track([in] long newVal);
	[propget, id(208000 + 10)] HRESULT EncodedBy([out, retval] BSTR *pVal);
	[propput, id(208000 + 10)] HRESULT EncodedBy([in] BSTR newVal);
	[propget, id(208000 + 11)] HRESULT Composer([out, retval] BSTR *pVal);
	[propput, id(208000 + 11)] HRESULT Composer([in] BSTR newVal);
	[propget, id(208000 + 12)] HRESULT Genre([out, retval] BSTR *pVal);
	[propput, id(208000 + 12)] HRESULT Genre([in] BSTR newVal);
	[propget, id(208000 + 13)] HRESULT OriginalArtist([out, retval] BSTR* pVal);
	[propput, id(208000 + 13)] HRESULT OriginalArtist([in] BSTR newVal);
	
	[propget, id(208000 + 14)] HRESULT Picture([out, retval] IPictureDisp** pVal);
	[propput, id(208000 + 14)] HRESULT Picture([in] IPictureDisp* newVal);

	[propget, id(208000 + 20)]	HRESULT AdditionalCount([out, retval] long* pVal);
	[id(208000 + 21)]			HRESULT GetAdditional([in]long Index,[out] BSTR* Name,[out]BSTR* Value);
	[id(208000 + 22)]			HRESULT SetAdditional([in]long Index,[in] BSTR Name,[in]BSTR Value);
	[id(208000 + 23)]			HRESULT DeleteAdditional([in]long Index);

	[propget, id(208000 + 30)]	HRESULT MarkerCount([out, retval] long* pVal);
	[id(208000 + 31)]			HRESULT GetMarker([in]long Index,[out]double* Position,[out]double* Length,[out]BSTR* Label,[out]BSTR* Note);
	[id(208000 + 32)]			HRESULT SetMarker([in]long Index,[in]double Position,[in]double Length,[in]BSTR Label,[in]BSTR Note);
	[id(208000 + 33)]			HRESULT DeleteMarker([in]long Index);


	[id(208000 + 50)] HRESULT CreateDuplicate([out, retval] IAVSExtraData** pMediaData);

};



// CAVSExtraData
[
	coclass,
	threading(apartment),
	vi_progid("AVSMediaCore3.AVSExtraData"),
	progid("AVSMediaCore3.AVSExtraData.1"),
	version(1.0),
	uuid("174EA295-730D-4132-A830-9C71ED225496"),
	helpstring("AVSExtraData Class")
]
class ATL_NO_VTABLE CAVSExtraData : 
	public IAVSExtraData
{
protected:
public:
	
	CAVSExtraData()
	{
		ExtraInfo.Clear();
	}
	~CAVSExtraData(void)
	{
		ExtraInfo.Clear();
	}
	

	STDMETHOD(CreateDuplicate)(IAVSExtraData** pExtraData)
	{
		if (NULL == pExtraData)
			return S_OK;

		*pExtraData = NULL;
		CoCreateInstance(__uuidof(CAVSExtraData), NULL, CLSCTX_ALL, __uuidof(IAVSExtraData), (void**)pExtraData);
		if (NULL == *pExtraData)
			return S_OK;

		return S_OK;
	}
	
    STDMETHOD(get_Year)(long *pVal);
	STDMETHOD(put_Year)(long newVal);
	STDMETHOD(get_Title)(BSTR *pVal);
	STDMETHOD(put_Title)(BSTR newVal);
	STDMETHOD(get_Copyright)(BSTR *pVal);
	STDMETHOD(put_Copyright)(BSTR newVal);
	STDMETHOD(get_Comments)(BSTR *pVal);
	STDMETHOD(put_Comments)(BSTR newVal);
	STDMETHOD(get_Artist)(BSTR *pVal);
	STDMETHOD(put_Artist)(BSTR newVal);
	STDMETHOD(get_Album)(BSTR *pVal);
	STDMETHOD(put_Album)(BSTR newVal);
	STDMETHOD(get_Genre)(BSTR *pVal);
	STDMETHOD(put_Genre)(BSTR newVal);
	STDMETHOD(get_Composer)(BSTR *pVal);
	STDMETHOD(put_Composer)(BSTR newVal);
	STDMETHOD(get_EncodedBy)(BSTR *pVal);
	STDMETHOD(put_EncodedBy)(BSTR newVal);
	STDMETHOD(get_Track)(long *pVal);
	STDMETHOD(put_Track)(long newVal);
	STDMETHOD(get_URL)(BSTR *pVal);
	STDMETHOD(put_URL)(BSTR newVal);
	STDMETHOD(get_OriginalArtist)(BSTR* pVal);
	STDMETHOD(put_OriginalArtist)(BSTR newVal);

	STDMETHOD(get_Picture)( IPictureDisp** pVal);
	STDMETHOD(put_Picture)( IPictureDisp* newVal);

	STDMETHOD(get_AdditionalCount)( long* pVal);
	STDMETHOD(GetAdditional)(long Index, BSTR* Name,BSTR* Value);
	STDMETHOD(SetAdditional)(long Index, BSTR Name,BSTR Value);
	STDMETHOD(DeleteAdditional)(long Index);

	STDMETHOD(get_MarkerCount)( long* pVal);
	STDMETHOD(GetMarker)(long Index,double* TimeStamp,double* Duration,BSTR* Label,BSTR* Note);
	STDMETHOD(SetMarker)(long Index,double TimeStamp,double Duration,BSTR Label,BSTR Note);
	STDMETHOD(DeleteMarker)(long Index);

private:
	_ExtraInfo ExtraInfo;
};
	
