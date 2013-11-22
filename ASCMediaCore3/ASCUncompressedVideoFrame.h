#pragma once
#include "ASCMediaData.h"
#include "MediaDataStruct.h"
#include "atlenc.h"
	
[object, uuid("B78DE084-9D29-431c-B76B-B996D02F5F02"), dual, pointer_default(unique)]
__interface IAVSUncompressedVideoFrame : IAVSMediaData
{
	[id(202000 + 1), propget] HRESULT Plane([in] long Index, [out, retval] BYTE** pVal);
	[id(202000 + 1), propput] HRESULT Plane([in] long Index, [in] BYTE* newVal);
	[id(202000 + 2), propget] HRESULT Stride([in] long Index, [out, retval] long* pVal);
	[id(202000 + 2), propput] HRESULT Stride([in] long Index, [in] long newVal);
	[id(202000 + 3), propget] HRESULT ColorSpace([out, retval] long* pVal);
	[id(202000 + 3), propput] HRESULT ColorSpace([in] long newVal);

	[id(202000 + 4)] HRESULT SetDefaultStrides(void);

	[id(202000 + 5), propget] HRESULT Width([out, retval] long* pVal);
	[id(202000 + 5), propput] HRESULT Width([in] long newVal);
	[id(202000 + 6), propget] HRESULT Height([out, retval] long* pVal);
	[id(202000 + 6), propput] HRESULT Height([in] long newVal);
	[id(202000 + 7), propget] HRESULT AspectRatioX([out, retval] long* pVal);
	[id(202000 + 7), propput] HRESULT AspectRatioX([in] long newVal);
	[id(202000 + 8), propget] HRESULT AspectRatioY([out, retval] long* pVal);
	[id(202000 + 8), propput] HRESULT AspectRatioY([in] long newVal);
	[id(202000 + 9), propget] HRESULT Interlaced([out, retval] VARIANT_BOOL* pVal);
	[id(202000 + 9), propput] HRESULT Interlaced([in] VARIANT_BOOL newVal);

	//[id(202000 + 10)] HRESULT GetAVSSafeArray([out, retval, satype("BYTE")] SAFEARRAY** pArray);
	//[id(202000 + 10)] HRESULT SetAVSSafeArray([in, satype("BYTE")] SAFEARRAY** pArray);

	
};

[object, uuid("89063236-01B8-4b21-9A8D-4B82C819D32B"), dual, pointer_default(unique)]
__interface IAVSUncompressedVideoFrameSerialize : IAVSUncompressedVideoFrame
{
	[id(203000 + 1)] HRESULT ToSafeArray ([out, retval, satype("BYTE")] SAFEARRAY** pArray);
	[id(203000 + 2)] HRESULT FromSafeArray ([in, satype("BYTE")] SAFEARRAY* pArray);
	[id(203000 + 3)] HRESULT ToBASE64 ([out, retval] BSTR* pStr);
	[id(203000 + 4)] HRESULT FromBASE64 ([in] BSTR pStr);
};
	
template <class CoT, class T>
class CAVSUncompressedVideoFrameImpl : public CAVSMediaDataImpl<CoT, T>
{
protected:
	struct SFrameSerailizationParams	// for Serialization
	{
		// MediaData Params
		VARIANT_BOOL m_bIsDiscontinuity;
		VARIANT_BOOL m_bTimeStampExist;
		DOUBLE m_dTimeStamp;
		VARIANT_BOOL m_bDurationExist;
		DOUBLE m_dDuration;

		// Uncompressed Frame Params
		SUncompressedVideoFrame sBaseParams;
	};
public:
	CAVSUncompressedVideoFrameImpl()
	{
		m_lMediaType = MEDIAFORMAT_VIDEO_UNCOMPRESSED;
	}
	
	STDMETHOD(CreateDuplicate)(long DublicateType, IAVSMediaData** pMediaData)
	{
		CAVSMediaDataImpl<CoT, T>::CreateDuplicate(DublicateType, pMediaData);
		if (NULL == *pMediaData)
			return S_OK;

		IAVSUncompressedVideoFrame* pAVSVideoData = NULL;
		(*pMediaData)->QueryInterface(__uuidof(IAVSUncompressedVideoFrame), (void**)(&pAVSVideoData));
		if (NULL == pAVSVideoData)
		{
			(*pMediaData)->Release();
			*pMediaData = NULL;
			return S_OK;
		}


		CAVSUncompressedVideoFrameImpl* pAVSVideoDataImpl = (CAVSUncompressedVideoFrameImpl*)pAVSVideoData;
		pAVSVideoDataImpl->m_oVideoFrame = m_oVideoFrame;
		for (int i=0;i<PLANE_COUNT;i++)
		{
			if (NULL!=m_oVideoFrame.Plane[i])
			{
				pAVSVideoDataImpl->m_oVideoFrame.Plane[i] = reinterpret_cast<BYTE*>(
					reinterpret_cast<ULONG64>(pAVSVideoDataImpl->m_pMediaBuffer->GetBuffer()) +
					reinterpret_cast<ULONG64>(m_oVideoFrame.Plane[i]) -
					reinterpret_cast<ULONG64>(m_pMediaBuffer->GetBuffer()));
			}
		}
		
		RELEASEINTERFACE(pAVSVideoData)

		return S_OK;
	}
	STDMETHOD(AllocateBuffer)(long lBufferSize)
	{
		if (lBufferSize<=0)
		{
			//lBufferSize -  не используется
			LONG lDataSize = m_oVideoFrame.Stride[0]*m_oVideoFrame.Height;
			ATLASSERT(0!=lDataSize);
			
			switch (m_oVideoFrame.ColorSpace & CSP_COLOR_MASK)
			{
			case CSP_I420:
			case CSP_YV12:

				m_pMediaBuffer->SetBuffer(lDataSize + (m_oVideoFrame.Stride[1] + m_oVideoFrame.Stride[2])*m_oVideoFrame.Height/2);
				m_oVideoFrame.Plane[0] = m_pMediaBuffer->GetBuffer();
				m_oVideoFrame.Plane[1] = m_oVideoFrame.Plane[0] + m_oVideoFrame.Stride[0]*m_oVideoFrame.Height;
				m_oVideoFrame.Plane[2] = m_oVideoFrame.Plane[0] + m_oVideoFrame.Stride[0]*m_oVideoFrame.Height + m_oVideoFrame.Stride[1]*m_oVideoFrame.Height/2;
				break;

			//case CSP_YV12:

			//	m_pMediaBuffer->SetBuffer(lDataSize + (m_oVideoFrame.Stride[1] + m_oVideoFrame.Stride[2])*m_oVideoFrame.Height/2);
			//	m_oVideoFrame.Plane[0] = m_pMediaBuffer->GetBuffer();
			//	m_oVideoFrame.Plane[1] = m_oVideoFrame.Plane[0] + m_oVideoFrame.Stride[0]*m_oVideoFrame.Height + m_oVideoFrame.Stride[1]*m_oVideoFrame.Height/2;
			//	m_oVideoFrame.Plane[2] = m_oVideoFrame.Plane[0] + m_oVideoFrame.Stride[0]*m_oVideoFrame.Height;
			//	break;

			case CSP_YUY2:
			case CSP_UYVY:
			case CSP_YVYU:

			case CSP_BGRA:
			case CSP_ABGR:
			case CSP_RGBA:
			case CSP_ARGB:

			case CSP_BGR:
			case CSP_RGB555:
			case CSP_RGB565:

			case CSP_RGB8:

				m_pMediaBuffer->SetBuffer(lDataSize);
				m_oVideoFrame.Plane[0] = m_pMediaBuffer->GetBuffer();
				break;
			}
		}
		else
		{
			CAVSMediaDataImpl<CoT, T>::AllocateBuffer(lBufferSize);
			m_oVideoFrame.Plane[0] = m_pMediaBuffer->GetBuffer();
		}
		ATLASSERT(0!=m_oVideoFrame.Plane[0]);
		return S_OK;
	}
	
	STDMETHOD(get_Plane)(LONG Index, BYTE** pVal)
	{
		*pVal = m_oVideoFrame.Plane[Index];

		return S_OK;
	}
	STDMETHOD(put_Plane)(LONG Index, BYTE* newVal)
	{
		m_oVideoFrame.Plane[Index] = newVal;
		return S_OK;
	}
	STDMETHOD(get_Stride)(LONG Index, LONG* pVal)
	{
		*pVal = m_oVideoFrame.Stride[Index];
		return S_OK;
	}
	STDMETHOD(put_Stride)(LONG Index, LONG newVal)
	{
		m_oVideoFrame.Stride[Index] = newVal;
		return S_OK;
	}
	STDMETHOD(get_ColorSpace)(LONG* pVal)
	{
		*pVal = m_oVideoFrame.ColorSpace;

		return S_OK;
	}
	STDMETHOD(put_ColorSpace)(LONG newVal)
	{
		m_oVideoFrame.ColorSpace = newVal;

		return S_OK;
	}
	
	STDMETHOD(SetDefaultStrides)(void)
	{
		switch (m_oVideoFrame.ColorSpace & CSP_COLOR_MASK)
		{
		case CSP_I420:
		case CSP_YV12:
			m_oVideoFrame.Stride[0] = m_oVideoFrame.Width;
			m_oVideoFrame.Stride[1] = m_oVideoFrame.Width/2;
			m_oVideoFrame.Stride[2] = m_oVideoFrame.Width/2;
			break;
		case CSP_YUY2:
		case CSP_UYVY:
		case CSP_YVYU:

		case CSP_RGB555:
		case CSP_RGB565:
			m_oVideoFrame.Stride[0] = 2*m_oVideoFrame.Width;
			break;
		case CSP_BGRA:
		case CSP_ABGR:
		case CSP_RGBA:
		case CSP_ARGB: 
			m_oVideoFrame.Stride[0] = 4*m_oVideoFrame.Width;
			break;
		case CSP_BGR:
			m_oVideoFrame.Stride[0] = 3*m_oVideoFrame.Width;
			break;
		}

		return S_OK;
	}
	STDMETHOD(get_Width)(LONG* pVal)
	{
		*pVal = m_oVideoFrame.Width;

		return S_OK;
	}
	STDMETHOD(put_Width)(LONG newVal)
	{
		m_oVideoFrame.Width = newVal;

		return S_OK;
	}
	STDMETHOD(get_Height)(LONG* pVal)
	{
		*pVal = m_oVideoFrame.Height;

		return S_OK;
	}
	STDMETHOD(put_Height)(LONG newVal)
	{
		m_oVideoFrame.Height = newVal;
		
		return S_OK;
	}
	STDMETHOD(get_AspectRatioX)(LONG* pVal)
	{
		*pVal = m_oVideoFrame.AspectX;

		return S_OK;
	}
	STDMETHOD(put_AspectRatioX)(LONG newVal)
	{
		m_oVideoFrame.AspectX = newVal;

		return S_OK;
	}
	STDMETHOD(get_AspectRatioY)(LONG* pVal)
	{
		*pVal = m_oVideoFrame.AspectY;

		return S_OK;
	}
	STDMETHOD(put_AspectRatioY)(LONG newVal)
	{
		m_oVideoFrame.AspectY = newVal;

		return S_OK;
	}
	STDMETHOD(get_Interlaced)(VARIANT_BOOL* pVal)
	{
		*pVal = m_oVideoFrame.Interlaced ? VARIANT_TRUE : VARIANT_FALSE;

		return S_OK;
	}
	STDMETHOD(put_Interlaced)(VARIANT_BOOL newVal)
	{
		m_oVideoFrame.Interlaced = (newVal != VARIANT_FALSE) ? TRUE : FALSE;

		return S_OK;
	}
	
	STDMETHOD (ToSafeArray) (SAFEARRAY** pArray)
	{
		//копирование каждый раз заново
		if (NULL == pArray)
			return S_OK;

		*pArray = NULL;
		if (NULL == m_pMediaBuffer->GetBuffer())
			return INVALIDOPERATION;

		BYTE* SrcPtr = NULL;
		BYTE* DstPtr = NULL;

		SFrameSerailizationParams sSerializationParam;
		sSerializationParam.sBaseParams = m_oVideoFrame;
		sSerializationParam.m_bDurationExist = m_bDurationExist;
		sSerializationParam.m_bIsDiscontinuity = m_bIsDiscontinuity;
		sSerializationParam.m_bTimeStampExist = m_bTimeStampExist;
		sSerializationParam.m_dDuration = m_dDuration;
		sSerializationParam.m_dTimeStamp = m_dTimeStamp;

		switch (m_oVideoFrame.ColorSpace & CSP_COLOR_MASK)
		{
		case CSP_I420:
		case CSP_YV12:

			*pArray = SafeArrayCreateVector (VT_UI1, 0, sizeof (sSerializationParam) + 2 * m_oVideoFrame.Width * m_oVideoFrame.Height);

			SafeArrayAccessData (*pArray, (void **) &DstPtr);

			// Копируем данные о фрейме
			memcpy (DstPtr, &sSerializationParam, sizeof (sSerializationParam));
			DstPtr += sizeof (sSerializationParam);

			// Копируем сам фрейм
			SrcPtr = m_oVideoFrame.Plane[0];
			for (int i = 0; i < m_oVideoFrame.Height; ++i)
			{
				memcpy(DstPtr, SrcPtr, m_oVideoFrame.Width);
				SrcPtr += m_oVideoFrame.Stride[0];
				DstPtr += m_oVideoFrame.Width;
			}
			SrcPtr = m_oVideoFrame.Plane[1];
			for (int i = 0; i < m_oVideoFrame.Height/2; ++i)
			{
				memcpy(DstPtr, SrcPtr, m_oVideoFrame.Width/2);
				SrcPtr += m_oVideoFrame.Stride[1];
				DstPtr += m_oVideoFrame.Width/2;
			}
			SrcPtr = m_oVideoFrame.Plane[2];
			for (int i = 0; i < m_oVideoFrame.Height/2; ++i)
			{
				memcpy(DstPtr, SrcPtr, m_oVideoFrame.Width/2);
				SrcPtr += m_oVideoFrame.Stride[2];
				DstPtr += m_oVideoFrame.Width/2;
			}
			SafeArrayUnaccessData(*pArray);
			break;

		case CSP_YUY2:
		case CSP_UYVY:
		case CSP_YVYU:

		case CSP_RGB555:
		case CSP_RGB565:

			*pArray = SafeArrayCreateVector (VT_UI1, 0, sizeof (sSerializationParam) + 2 * m_oVideoFrame.Width * m_oVideoFrame.Height);

			SafeArrayAccessData (*pArray, (void **) &DstPtr);

			// Копируем данные о фрейме
			memcpy (DstPtr, &sSerializationParam, sizeof (sSerializationParam));
			DstPtr += sizeof (sSerializationParam);

			// Копируем сам фрейм
			SrcPtr = m_oVideoFrame.Plane[0];
			for (int i = 0; i < m_oVideoFrame.Height; ++i)
			{
				memcpy(DstPtr, SrcPtr, m_oVideoFrame.Width*2);
				SrcPtr += m_oVideoFrame.Stride[0];
				DstPtr += m_oVideoFrame.Width * 2;
			}
			SafeArrayUnaccessData(*pArray);
			break;

		case CSP_BGR:

			*pArray = SafeArrayCreateVector (VT_UI1, 0, sizeof (sSerializationParam) + 3 * m_oVideoFrame.Width * m_oVideoFrame.Height);

			SafeArrayAccessData (*pArray, (void **) &DstPtr);

			// Копируем данные о фрейме
			memcpy (DstPtr, &sSerializationParam, sizeof (sSerializationParam));
			DstPtr += sizeof (sSerializationParam);

			// Копируем сам фрейм
			SrcPtr = m_oVideoFrame.Plane[0];
			for (int i = 0; i < m_oVideoFrame.Height; ++i)
			{
				memcpy(DstPtr, SrcPtr, m_oVideoFrame.Width*3);
				SrcPtr += m_oVideoFrame.Stride[0];
				DstPtr += m_oVideoFrame.Width * 3;
			}
			SafeArrayUnaccessData (*pArray);
			break;

		case CSP_BGRA:
		case CSP_ABGR:
		case CSP_RGBA:
		case CSP_ARGB:

			*pArray = SafeArrayCreateVector (VT_UI1, 0, sizeof (sSerializationParam) + 4 * m_oVideoFrame.Width * m_oVideoFrame.Height);

			SafeArrayAccessData (*pArray, (void **) &DstPtr);
			
			// Копируем данные о фрейме
			memcpy (DstPtr, &sSerializationParam, sizeof (sSerializationParam));
			DstPtr += sizeof (sSerializationParam);

			// Копируем сам фрейм
			SrcPtr = m_oVideoFrame.Plane[0];
			for (int i = 0; i < m_oVideoFrame.Height; ++i)
			{
				memcpy(DstPtr, SrcPtr, m_oVideoFrame.Width*4);
				SrcPtr += m_oVideoFrame.Stride[0];
				DstPtr += m_oVideoFrame.Width*4;
			}
			SafeArrayUnaccessData(*pArray);

			break;
		}

		return S_OK;
	}

	STDMETHOD(FromSafeArray)(SAFEARRAY* pArray)
	{
		if (NULL == pArray)
			return S_OK;

		SFrameSerailizationParams sSerializationParam;
		long nArraySize (0);
		SafeArrayGetUBound (pArray, 1, &nArraySize);
		nArraySize++;

		if (nArraySize < sizeof (sSerializationParam))
			return MEMORY;

		BYTE *SrcPtr (NULL), *FramePtrBase (NULL);
		BYTE *DstPtr (NULL);

		HRESULT hRes = SafeArrayAccessData(pArray, (void **) &SrcPtr); 
		FramePtrBase = SrcPtr + sizeof (sSerializationParam);		// Начало данных фрейма

		//
		if (S_OK != hRes)
			return MEMORY;

		// Копируем данные о фрейме
		memcpy (&sSerializationParam, SrcPtr, sizeof (sSerializationParam));
		SrcPtr += sizeof (sSerializationParam);	

		m_oVideoFrame = sSerializationParam.sBaseParams;
		m_bDurationExist = sSerializationParam.m_bDurationExist;
		m_bIsDiscontinuity = sSerializationParam.m_bIsDiscontinuity;
		m_bTimeStampExist = sSerializationParam.m_bTimeStampExist;
		m_dDuration = sSerializationParam.m_dDuration;
		m_dTimeStamp = sSerializationParam.m_dTimeStamp;

		// отводим ровно столько памяти, сколько нужно для картинки с выбранным типом
		SetDefaultStrides();
		AllocateBuffer(-1);

		switch (m_oVideoFrame.ColorSpace & CSP_COLOR_MASK)
		{
		case CSP_I420:
		case CSP_YV12:

			DstPtr = m_oVideoFrame.Plane[0];
			for (int i = 0; i < m_oVideoFrame.Height; ++i)
			{
				memcpy(DstPtr, SrcPtr, m_oVideoFrame.Width);
				SrcPtr += m_oVideoFrame.Width;
				DstPtr += m_oVideoFrame.Stride[0];
			}
			SrcPtr = FramePtrBase + m_oVideoFrame.Stride[0] * m_oVideoFrame.Height;
			DstPtr = m_oVideoFrame.Plane[1];
			for (int i = 0; i < m_oVideoFrame.Height/2; ++i)
			{
				memcpy(DstPtr, SrcPtr, m_oVideoFrame.Width/2);
				SrcPtr += m_oVideoFrame.Width/2;
				DstPtr += m_oVideoFrame.Stride[1];
			}
			SrcPtr = FramePtrBase + (m_oVideoFrame.Stride[0] * m_oVideoFrame.Height) + (m_oVideoFrame.Stride[1] * m_oVideoFrame.Height / 2);
			DstPtr = m_oVideoFrame.Plane[2];
			for (int i = 0; i < m_oVideoFrame.Height/2; ++i)
			{
				memcpy(DstPtr, SrcPtr, m_oVideoFrame.Width/2);
				SrcPtr += m_oVideoFrame.Width/2;
				DstPtr += m_oVideoFrame.Stride[2];
			}
			break;

		case CSP_YUY2:
		case CSP_UYVY:
		case CSP_YVYU:

		case CSP_RGB555:
		case CSP_RGB565:
			DstPtr = m_oVideoFrame.Plane[0];
			for (int i = 0; i < m_oVideoFrame.Height; ++i)
			{
				memcpy (DstPtr, SrcPtr, m_oVideoFrame.Width * 2);
				SrcPtr += m_oVideoFrame.Width * 2;
				DstPtr += m_oVideoFrame.Stride [0];
			}
			break;

		case CSP_BGR:
			DstPtr = m_oVideoFrame.Plane[0];
			for (int i = 0; i < m_oVideoFrame.Height; ++i)
			{
				memcpy (DstPtr, SrcPtr, m_oVideoFrame.Width * 3);
				SrcPtr += m_oVideoFrame.Width * 3;
				DstPtr += m_oVideoFrame.Stride [0];
			}
			break;

		case CSP_BGRA:
		case CSP_ABGR:
		case CSP_RGBA:
		case CSP_ARGB:
			DstPtr = m_oVideoFrame.Plane[0];
			for (int i = 0; i < m_oVideoFrame.Height; ++i)
			{
				memcpy (DstPtr, SrcPtr, m_oVideoFrame.Width * 4);
				SrcPtr += m_oVideoFrame.Width * 4;
				DstPtr += m_oVideoFrame.Stride [0];
			}
			break;
		}

		SafeArrayUnaccessData (pArray);

		return S_OK;
	}
	STDMETHOD (ToBASE64) (BSTR* pStr)
	{
		// Сначала гоним в SafeArray, 
		SAFEARRAY *pSA = NULL;
		HRESULT hRes = ToSafeArray(&pSA);
		if (S_OK != hRes)
			return hRes;
		
		// Теперь в BASE64
		long nBufferSize (0);
		LPBYTE pData = NULL;
		SafeArrayGetUBound(pSA, 1, &nBufferSize);
		nBufferSize++;


		hRes = SafeArrayAccessData (pSA, (void **) &pData);
		if (S_OK != hRes)
			return S_OK;
		
		// 
		int nBase64Size = Base64EncodeGetRequiredLength ((int)nBufferSize);
		LPSTR pStrData = new char [nBase64Size + 1];
		BOOL bSuccess = Base64Encode(pData, (int)nBufferSize, pStrData, &nBase64Size);
		pStrData[nBase64Size] = '\0';

		CStringW sResult;
		sResult = pStrData;
		delete [] pStrData;
		*pStr = sResult.AllocSysString();
		
		SafeArrayUnaccessData (pSA);
		SafeArrayDestroy (pSA);
		return S_OK;
	}
	STDMETHOD (FromBASE64) (BSTR pStr)
	{
		// Сначала гоним в SafeArray
		CStringA sInStr;
		sInStr = pStr;
		int nEncSize;
		Base64Decode(sInStr.GetBuffer(), sInStr.GetLength(), NULL, &nEncSize);

		SAFEARRAY *pSA = SafeArrayCreateVector (VT_UI1, 0, nEncSize);
		if (NULL == pSA)
			return MEMORY;

		LPBYTE pBuffer = NULL;
		SafeArrayAccessData (pSA, (void **) &pBuffer);
	
		if (!Base64Decode(sInStr.GetBuffer(), sInStr.GetLength(), pBuffer, &nEncSize))
		{
			SafeArrayUnaccessData (pSA);
			SafeArrayDestroy (pSA);
			return S_FALSE;
		}
		
		SafeArrayUnaccessData (pSA);

		// Загружаем из SafeArray
		HRESULT hRes = FromSafeArray(pSA);
		SafeArrayDestroy (pSA);

		return hRes;
	}
public:
	//public, потому что Transform обращается напрямую к этим данным
	SUncompressedVideoFrame m_oVideoFrame;
};
	
[coclass, uuid("AA742621-AE1E-41ba-B577-4A1F3D1B3417"), threading(apartment), vi_progid("AVSMediaCore3.AVSVideoFrameUnc"), progid("AVSMediaCore3.AVSVideoFrameUnc.1"), version(1.0)]
class ATL_NO_VTABLE CAVSUncompressedVideoFrame : public CAVSUncompressedVideoFrameImpl<CAVSUncompressedVideoFrame, IAVSUncompressedVideoFrameSerialize>
{
};

