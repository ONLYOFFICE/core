// AVSSubtitleData.h : Declaration of the CAVSSubtitleData

#pragma once
#include "resource.h"       // main symbols
#include "ASCMediaData.h"
#include "MemoryUtils.h"
#include <atlcoll.h>

// AVSSubtitleData
[object, uuid("96FDBD8F-33B1-4092-B731-005A41579FAA"), dual, pointer_default(unique)]
__interface IAVSSubtitleData : IAVSMediaData
{
	[id(211000 + 1), propget] HRESULT TextBox([out, retval] RECT* pVal);
	[id(211000 + 1), propput] HRESULT TextBox([in] RECT newVal);
	
	[id(211000 + 2), propget] HRESULT TypeScroll([out, retval] long* pVal);
	[id(211000 + 2), propput] HRESULT TypeScroll([in] long newVal);

	[id(211000 + 3), propget] HRESULT ContinuousScroll([out, retval] VARIANT_BOOL* pVal);
	[id(211000 + 3), propput] HRESULT ContinuousScroll([in] VARIANT_BOOL newVal);

	[id(211000 + 4), propget] HRESULT TextOverlay([out, retval] VARIANT_BOOL* pVal);
	[id(211000 + 4), propput] HRESULT TextOverlay([in] VARIANT_BOOL newVal);

	[id(211000 + 5), propget] HRESULT AutoScale([out, retval] VARIANT_BOOL* pVal);
	[id(211000 + 5), propput] HRESULT AutoScale([in] VARIANT_BOOL newVal);
};

template <class CoT, class T>
class CAVSSubtitleDataImpl : public CAVSMediaDataImpl<CoT, T>
{
public:
	CAVSSubtitleDataImpl()
	{
		m_lMediaType = MEDIAFORMAT_TEXT;
		
		m_TextBox.top=0;
		m_TextBox.left=0;
		m_TextBox.right=-1;//max by frame
		m_TextBox.bottom=-1;//max by frame

		m_TypeScroll=0;

		m_ContinuousScroll=FALSE;
		m_TextOverlay=FALSE;
		m_AutoScale=-1;
	}
	STDMETHOD(CreateDuplicate)(long DublicateType, IAVSMediaData** pMediaData)
	{
		CAVSMediaDataImpl<CoT, T>::CreateDuplicate(DublicateType, pMediaData);
		if (NULL == *pMediaData)
			return S_OK;

		IAVSSubtitleData* pAVSSubTitle = NULL;
		(*pMediaData)->QueryInterface(__uuidof(IAVSSubtitleData), (void**)(&pAVSSubTitle));
		if (NULL == pAVSSubTitle)
		{
			(*pMediaData)->Release();
			*pMediaData = NULL;
			return S_OK;
		}

		RELEASEINTERFACE(pAVSSubTitle)
		return S_OK;
	}
	STDMETHOD(get_TextBox)(RECT* pVal)
	{
		*pVal = m_TextBox;
		return S_OK;
	}
	STDMETHOD(put_TextBox)(RECT newVal)
	{
		m_TextBox = newVal;
		return S_OK;
	}
	STDMETHOD(get_TypeScroll)(long* pVal)
	{
		*pVal = m_TypeScroll;
		return S_OK;
	}
	STDMETHOD(put_TypeScroll)(long newVal)
	{
		//0 - not
		//1 - horizontal
		//2 - vertical
		m_TypeScroll = newVal;
		return S_OK;
	}
	STDMETHOD(get_ContinuousScroll)(VARIANT_BOOL* pVal)
	{
		*pVal = m_ContinuousScroll;
		return S_OK;
	}
	STDMETHOD(put_ContinuousScroll)(VARIANT_BOOL newVal)
	{
		//бегущая строка
		m_ContinuousScroll = newVal;
		return S_OK;
	}
	STDMETHOD(get_TextOverlay)(VARIANT_BOOL* pVal)
	{
		*pVal = m_TextOverlay;
		return S_OK;
	}
	STDMETHOD(put_TextOverlay)(VARIANT_BOOL newVal)
	{
		m_TextOverlay = newVal;
		return S_OK;
	}
	STDMETHOD(get_AutoScale)(VARIANT_BOOL* pVal)
	{
		*pVal = m_AutoScale;
		return S_OK;
	}
	STDMETHOD(put_AutoScale)(VARIANT_BOOL newVal)
	{
		m_AutoScale = newVal;
		return S_OK;
	}
protected:
	RECT m_TextBox;	
	long m_TypeScroll;
	VARIANT_BOOL m_ContinuousScroll;
	VARIANT_BOOL m_TextOverlay;
	VARIANT_BOOL m_AutoScale;
};

// CAVSSubtitleData

[coclass, uuid("3E4433F7-1D47-49eb-AA85-402B7EF50432"), threading(apartment), vi_progid("AVSMediaCore3.AVSSubtitleData"), progid("AVSMediaCore3.AVSDVDSubpictureFram.1"), version(1.0)]
class ATL_NO_VTABLE CAVSSubtitleData : public CAVSSubtitleDataImpl<CAVSSubtitleData, IAVSSubtitleData>
{
};
