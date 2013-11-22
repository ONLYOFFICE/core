#pragma once

#include <ATLDefine.h>
#include <MediaFormatDefine.h>
#include <MediaDataDefine.h>
#include <VideoFileErrorDescription.h>
#include "ASCMediaBuffer.h"
	
[object, uuid("C04D2557-D06D-406c-BA7D-27657B1F788C"), dual, pointer_default(unique)]
__interface IAVSMediaData : IDispatch
{
	[id(200000 + 1), propget] HRESULT MediaType([out, retval] long* pVal);
	//см. "MediaDataDefine.h" параметры с префиксом DUBLICATE_TYPE_
	[id(200000 + 2)] HRESULT CreateDuplicate([in] long DublicateType, [out, retval] IAVSMediaData** pMediaData);

	[id(200000 + 3)] HRESULT AllocateBuffer([in] long BufferSize);
	[id(200000 + 4), propget] HRESULT Buffer([out, retval] BYTE** pVal);
	[id(200000 + 5), propget] HRESULT BufferSize([out, retval] long* pVal);

	[id(200000 + 6), propget] HRESULT DataSize([out, retval] long* pVal);
	[id(200000 + 6), propput] HRESULT DataSize([in] long newVal);

	[id(200000 + 7), propget] HRESULT IsDiscontinuity([out, retval] VARIANT_BOOL* pVal);
	[id(200000 + 7), propput] HRESULT IsDiscontinuity([in] VARIANT_BOOL newVal);

	[id(200000 + 8), propget] HRESULT IsTimeStampExist([out, retval] VARIANT_BOOL* pVal);
	[id(200000 + 9), propget] HRESULT TimeStamp([out, retval] double* pVal);
	[id(200000 + 9), propput] HRESULT TimeStamp([in] double newVal);

	[id(200000 + 10), propget] HRESULT IsDurationExist([out, retval] VARIANT_BOOL* pVal);
	[id(200000 + 11), propget] HRESULT Duration([out, retval] double* pVal);
	[id(200000 + 11), propput] HRESULT Duration([in] double newVal);

	[id(200000 + 12)] HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(200000 + 13)] HRESULT GetAdditionalParam([in] BSTR ParamName, [out,retval] VARIANT* ParamValue);
};
	
template <class CoT, class T>
class CAVSMediaDataImpl : public IDispatchImpl<T>
{
protected:
	LONG m_lMediaType;
	VARIANT_BOOL m_bIsDiscontinuity;
	
	VARIANT_BOOL m_bTimeStampExist;
	DOUBLE m_dTimeStamp;

	VARIANT_BOOL m_bDurationExist;
	DOUBLE m_dDuration;

	CMediaBuffer* m_pMediaBuffer;
	
	long m_lDataSize;
public:
	
	CAVSMediaDataImpl()
	{
		m_lMediaType = MEDIAFORMAT_UNKNOWN;

		m_bIsDiscontinuity = VARIANT_FALSE;

		m_bTimeStampExist = VARIANT_FALSE;
		m_dTimeStamp = 0;
		
		m_bDurationExist = VARIANT_FALSE;
		m_dDuration = 0;

		m_lDataSize = 0;

		m_pMediaBuffer = new CMediaBuffer();
	}
	~CAVSMediaDataImpl(void)
	{
		RELEASEINTERFACE(m_pMediaBuffer)
	}
	
	STDMETHOD(get_MediaType)(LONG* pVal)
	{
		*pVal = m_lMediaType;

		return S_OK;
	}

	STDMETHOD(CreateDuplicate)(long DublicateType, IAVSMediaData** pMediaData)
	{
		if (NULL == pMediaData)
			return S_OK;

		*pMediaData = NULL;
		CoCreateInstance(__uuidof(CoT), NULL, CLSCTX_ALL, __uuidof(IAVSMediaData), (void**)pMediaData);
		if (NULL == *pMediaData)
			return S_OK;

		(*pMediaData)->put_IsDiscontinuity(m_bIsDiscontinuity);
		if (m_bTimeStampExist != VARIANT_FALSE)
			(*pMediaData)->put_TimeStamp(m_dTimeStamp);
		if (m_bDurationExist != VARIANT_FALSE)
			(*pMediaData)->put_Duration(m_dDuration);

		CAVSMediaDataImpl* pMediaDataImpl = (CAVSMediaDataImpl*)(*pMediaData);

		RELEASEINTERFACE(pMediaDataImpl->m_pMediaBuffer)

		if (NULL!=m_pMediaBuffer)
		{
			switch (DublicateType)
			{
			case DUBLICATE_TYPE_LINK:
				{
					pMediaDataImpl->m_pMediaBuffer = m_pMediaBuffer;
					pMediaDataImpl->m_pMediaBuffer->AddRef();
				}
				break;
			case DUBLICATE_TYPE_COPY:
				{
					pMediaDataImpl->m_pMediaBuffer = new CMediaBuffer();
					if (NULL!=pMediaDataImpl->m_pMediaBuffer)
					{
						pMediaDataImpl->m_pMediaBuffer->SetBuffer(m_pMediaBuffer->GetBufferSize());
						memcpy(pMediaDataImpl->m_pMediaBuffer->GetBuffer(), m_pMediaBuffer->GetBuffer(), m_pMediaBuffer->GetBufferSize());
					}
				}
				break;
			}
		}

		(*pMediaData)->put_DataSize(m_lDataSize);
		return S_OK;
	}
	
	STDMETHOD(AllocateBuffer)(LONG BufferSize)
	{
		m_pMediaBuffer->SetBuffer(BufferSize);

		m_lDataSize = m_pMediaBuffer->GetBufferSize();

		return S_OK;
	}
	STDMETHOD(get_Buffer)(BYTE** pVal)
	{
		*pVal = 0;
		if (NULL!=m_pMediaBuffer)
			*pVal = m_pMediaBuffer->GetBuffer();
		return S_OK;
	}
	STDMETHOD(get_BufferSize)(LONG* pVal)
	{
		*pVal = 0;
		if (NULL!=m_pMediaBuffer)
			*pVal = m_pMediaBuffer->GetBufferSize();
		return S_OK;
	}
	STDMETHOD(get_DataSize)(long* pVal)
	{
		*pVal = m_lDataSize;

		return S_OK;
	}
	STDMETHOD(put_DataSize)(long newVal)
	{
		if (NULL==m_pMediaBuffer)
			m_lDataSize = 0;
		else
			m_lDataSize = min(newVal, m_pMediaBuffer->GetBufferSize());
		return S_OK;
	}
		
	STDMETHOD(get_IsDiscontinuity)(VARIANT_BOOL* pVal)
	{
		*pVal = m_bIsDiscontinuity;

		return S_OK;
	}
	STDMETHOD(put_IsDiscontinuity)(VARIANT_BOOL newVal)
	{
		m_bIsDiscontinuity = newVal;

		return S_OK;
	}
	
	STDMETHOD(get_IsTimeStampExist)(VARIANT_BOOL* pVal)
	{
		*pVal = m_bTimeStampExist;

		return S_OK;
	}
	STDMETHOD(get_TimeStamp)(DOUBLE* pVal)
	{
		*pVal = m_dTimeStamp;

		return S_OK;
	}
	STDMETHOD(put_TimeStamp)(DOUBLE newVal)
	{
		m_dTimeStamp = newVal;
		m_bTimeStampExist = VARIANT_TRUE;

		return S_OK;
	}
	
	STDMETHOD(get_IsDurationExist)(VARIANT_BOOL* pVal)
	{
		*pVal = m_bDurationExist;

		return S_OK;
	}
	STDMETHOD(get_Duration)(DOUBLE* pVal)
	{
		*pVal = m_dDuration;

		return S_OK;
	}
	STDMETHOD(put_Duration)(DOUBLE newVal)
	{
		m_dDuration = newVal;
		m_bDurationExist = VARIANT_TRUE;

		return S_OK;
	}
	
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		return S_OK;
	}
};
	
[coclass, uuid("142B5713-0AA8-4d89-87CC-0AE636F381E9"), threading(apartment), vi_progid("AVSMediaCore3.AVSMediaData"), progid("AVSMediaCore3.AVSMediaData.1"), version(1.0)]
class ATL_NO_VTABLE CAVSMediaData : public CAVSMediaDataImpl<CAVSMediaData, IAVSMediaData>
{
};
