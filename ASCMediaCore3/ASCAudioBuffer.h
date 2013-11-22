// ASCAudioBuffer.h : Declaration of the CASCAudioBuffer

#pragma once
#include "resource.h"       // main symbols
#include "ASCMediaData.h"
#include "ASCAudioData.h"

#include "MediaDataDefine.h"
#include "ASCUtils.h"

// IAVSAudioBuffer
[object, uuid("1CB6CE9F-0E75-4B17-8512-668FF3B10745"), dual, pointer_default(unique)]
__interface IAVSAudioBuffer : IDispatch
{
	[propget, id(206000 + 1), helpstring("property Mode")] HRESULT Mode([out, retval] LONG* pVal);
	[propput, id(206000 + 1), helpstring("property Mode")] HRESULT Mode([in] LONG newVal);
	[propget, id(206000 + 2), helpstring("property MaxBufferSize")] HRESULT MaxBufferSize([out, retval] LONG* pVal);
	[propput, id(206000 + 2), helpstring("property MaxBufferSize")] HRESULT MaxBufferSize([in] LONG newVal);
	[propget, id(206000 + 3), helpstring("property BufferSize")] HRESULT BufferSize([out, retval] LONG* pVal);
	[propget, id(206000 + 4), helpstring("property CurrentTimeStamp")] HRESULT CurrentTimeStamp([out, retval] DOUBLE* pVal);

	[id(206000 + 10), helpstring("method SetArrayData")] HRESULT SetArrayData([in, satype("byte")] SAFEARRAY* pAudio);
	[id(206000 + 11), helpstring("method SetData")] HRESULT SetData([in] IAVSAudioData* pAudio);
	[id(206000 + 12), helpstring("method GetData")] HRESULT GetData(LONG nSamples, [out,retval] IAVSAudioData** pAudio);
	[id(206000 + 13), helpstring("method ClearBuffer")] HRESULT ClearBuffer(void);
};


//макисмальное время тишины, которая вставляется при 
//BufferMode == USETIMESTAMPALWAYS
//BufferMode == USETIMESTAMP
static const long g_clMaxAudioGap = 5*60;//5 минут

template <class CoT, class T>
class CAVSAudioBufferImpl : public IDispatchImpl<T>
{
private:
	HANDLE	hSynchMutex;

	LONG	m_BufferMode;
	LONG	m_MaxBufferSize;
	LONG	m_CurrentBufferSize;

	DOUBLE	m_CurrentTimeStamp;
	DOUBLE	m_LastTimeStamp;

	LONG	m_LastOffset;

	CIPtrAtlArray<IAVSMediaData*>	m_AudioBuffer;
	long	m_nSilenceAudioCompensate;

public:
	CAVSAudioBufferImpl()
	{
		hSynchMutex=NULL;
		m_BufferMode=INGNORETIMESTAMP;
		m_MaxBufferSize=0x7FFFFFFF;
		m_nSilenceAudioCompensate=0;

		hSynchMutex=CreateMutex(NULL, FALSE, NULL);

		ClearBuffer();
	}

	~CAVSAudioBufferImpl()
	{
		ClearBuffer();
		RELEASEHANDLE(hSynchMutex);
	}

	STDMETHOD(get_Mode)(LONG* pVal)
	{
		CSynchAccess	SynchAccess=hSynchMutex;
		*pVal = m_BufferMode;
		return S_OK;
	}
	STDMETHOD(put_Mode)(LONG newVal)
	{
		CSynchAccess	SynchAccess=hSynchMutex;
		if ((newVal<0)||(newVal>=AUDIO_BUFFER_MODE_COUNT))
			return S_OK;
		m_BufferMode = newVal;
		return S_OK;
	}
	STDMETHOD(get_MaxBufferSize)(LONG* pVal)
	{
		CSynchAccess	SynchAccess=hSynchMutex;
		*pVal = m_MaxBufferSize;
		return S_OK;
	}
	STDMETHOD(put_MaxBufferSize)(LONG newVal)
	{
		CSynchAccess	SynchAccess=hSynchMutex;
		m_MaxBufferSize = newVal;
		return S_OK;
	}
	STDMETHOD(get_BufferSize)(LONG* pVal)
	{
		CSynchAccess	SynchAccess=hSynchMutex;
		*pVal = m_CurrentBufferSize;
		return S_OK;
	}
	STDMETHOD(get_CurrentTimeStamp)(DOUBLE* pVal)
	{
		CSynchAccess	SynchAccess=hSynchMutex;
		*pVal = m_CurrentTimeStamp;
		return S_OK;
	}

	STDMETHOD(SetArrayData)(SAFEARRAY* pVal)
	{
		CSynchAccess	SynchAccess=hSynchMutex;

		LONG		BufferSize;
		SafeArrayGetUBound(pVal,1,&BufferSize); BufferSize++;

		IAVSAudioData*	AVSAudioData=NULL;
		CoCreateInstance(__uuidof(CAVSAudioData), NULL, CLSCTX_ALL, __uuidof(IAVSAudioData), (void**)&AVSAudioData);
		BYTE*		MemPtr;
		AVSAudioData->AllocateBuffer(BufferSize);
		AVSAudioData->get_Buffer(&MemPtr);
		memcpy(MemPtr, pVal->pvData, BufferSize);
		SetData(AVSAudioData);
		RELEASEINTERFACE(AVSAudioData);

		return S_OK;
	}
	STDMETHOD(SetData)(IAVSAudioData* pVal)
	{
		CSynchAccess	SynchAccess=hSynchMutex;

		if (-1==m_CurrentTimeStamp)
			pVal->get_TimeStamp(&m_CurrentTimeStamp);

		BYTE*			pMemPtr = NULL;		
		long			lDataSize = 0;
		pVal->get_Buffer(&pMemPtr);
		pVal->get_DataSize(&lDataSize);
		if (NULL==pMemPtr)
			return S_OK;//вопрос не надо ли догенерить тишины до TimeStamp пришедшей AudioData
		WAVEFORMATEX*	pWaveFormat = (WAVEFORMATEX*)pMemPtr;
		lDataSize -= (sizeof(WAVEFORMATEX) + pWaveFormat->cbSize);
		if (0>=lDataSize)
			return S_OK;
		m_CurrentBufferSize += 8*lDataSize/(pWaveFormat->nChannels*pWaveFormat->wBitsPerSample);

		if ((m_MaxBufferSize>0)&&(m_CurrentBufferSize>m_MaxBufferSize))
		{
			IAVSAudioData* pAudio=NULL;
			GetData((m_CurrentBufferSize-m_MaxBufferSize), &pAudio);
			RELEASEINTERFACE(pAudio);
		}

		switch (m_BufferMode)
		{
			case TIMESTAMPCOMPENSATE:		//	TIMESTAMPCOMPENSATE	-	компенсируем тишиной 
			{
				const double g_MaxAudioWriteSilences		=	10;

				VARIANT_BOOL	vbIsTimeStampExist;
				double			dTimeStamp;
				pVal->get_IsTimeStampExist(&vbIsTimeStampExist);
				pVal->get_TimeStamp(&dTimeStamp);
				double Duration = 0.0;
				pVal->get_Duration ( &Duration );

				if (VARIANT_FALSE!=vbIsTimeStampExist)
				{
					if (-1!=m_LastTimeStamp)
					{
						long lDelta = (long)((dTimeStamp-m_LastTimeStamp)*pWaveFormat->nSamplesPerSec/1000.0);
						if (g_clMaxAudioGap*(long)pWaveFormat->nSamplesPerSec<=lDelta)
						{
							ATLASSERT(FALSE);
							lDelta = g_clMaxAudioGap*pWaveFormat->nSamplesPerSec;
						}

						m_nSilenceAudioCompensate  += lDelta; 
						 
						if ( m_nSilenceAudioCompensate >= g_MaxAudioWriteSilences * (long)pWaveFormat->nSamplesPerSec )
						{
							m_nSilenceAudioCompensate	=	0;

							IAVSAudioData*	pAudioData = NULL;
							CoCreateInstance(__uuidof(CAVSAudioData), NULL, CLSCTX_ALL, __uuidof(IAVSAudioData), (void**)&pAudioData);
							if (NULL!=pAudioData)
							{
								long lBytesNeed = (m_nSilenceAudioCompensate * pWaveFormat->nChannels * pWaveFormat->wBitsPerSample)/8;
								pAudioData->AllocateBuffer(lBytesNeed + sizeof(WAVEFORMATEX) + pWaveFormat->cbSize);

								LPBYTE	pMemPtrDst = NULL;
								pAudioData->get_Buffer(&pMemPtrDst);
								if (NULL!=pMemPtrDst)
								{
									memcpy(pMemPtrDst, pMemPtr, sizeof(WAVEFORMATEX) + pWaveFormat->cbSize);

									pMemPtrDst += sizeof(WAVEFORMATEX) + pWaveFormat->cbSize;
									memset(pMemPtrDst, 0x00, lBytesNeed);
									m_CurrentBufferSize += m_nSilenceAudioCompensate; 
									m_AudioBuffer.Add(pAudioData);
								}
							}
						}
					}
					m_LastTimeStamp = dTimeStamp + 1000.0*lDataSize/(pWaveFormat->nChannels*pWaveFormat->wBitsPerSample/8)/pWaveFormat->nSamplesPerSec;
				} 
				else
				{
					if (m_LastTimeStamp!=-1)
						m_LastTimeStamp+=1000.0*lDataSize/(pWaveFormat->nChannels*pWaveFormat->wBitsPerSample/8)/pWaveFormat->nSamplesPerSec;
				}
			}
			break;

		case USETIMESTAMPALWAYS:
			{
				VARIANT_BOOL	vbIsTimeStampExist;
				double			dTimeStamp;
				pVal->get_IsTimeStampExist(&vbIsTimeStampExist);
				pVal->get_TimeStamp(&dTimeStamp);

				if (VARIANT_FALSE!=vbIsTimeStampExist)
				{
					if (-1!=m_LastTimeStamp)
					{
						long lDelta = (long)((dTimeStamp-m_LastTimeStamp)*pWaveFormat->nSamplesPerSec/1000.0);
						if (g_clMaxAudioGap*(long)pWaveFormat->nSamplesPerSec<=lDelta)
						{
							ATLASSERT(FALSE);
							lDelta = g_clMaxAudioGap*pWaveFormat->nSamplesPerSec;
						}
						if (0<lDelta)
						{
							IAVSAudioData*	pAudioData = NULL;
							CoCreateInstance(__uuidof(CAVSAudioData), NULL, CLSCTX_ALL, __uuidof(IAVSAudioData), (void**)&pAudioData);
							if (NULL!=pAudioData)
							{
								long lBytesNeed = (lDelta * pWaveFormat->nChannels * pWaveFormat->wBitsPerSample)/8;
								pAudioData->AllocateBuffer(lBytesNeed + sizeof(WAVEFORMATEX) + pWaveFormat->cbSize);

								LPBYTE	pMemPtrDst = NULL;
								pAudioData->get_Buffer(&pMemPtrDst);
								if (NULL!=pMemPtrDst)
								{
									memcpy(pMemPtrDst, pMemPtr, sizeof(WAVEFORMATEX) + pWaveFormat->cbSize);

									pMemPtrDst += sizeof(WAVEFORMATEX) + pWaveFormat->cbSize;
									memset(pMemPtrDst, 0x00, lBytesNeed);
									m_CurrentBufferSize += lDelta; 
									m_AudioBuffer.Add(pAudioData);
								}
							}
						}
					}
					m_LastTimeStamp = dTimeStamp + 1000.0*lDataSize/(pWaveFormat->nChannels*pWaveFormat->wBitsPerSample/8)/pWaveFormat->nSamplesPerSec;
				} 
				else
				{
					if (m_LastTimeStamp!=-1)
						m_LastTimeStamp+=1000.0*lDataSize/(pWaveFormat->nChannels*pWaveFormat->wBitsPerSample/8)/pWaveFormat->nSamplesPerSec;
				}
			}
			break;
		case USETIMESTAMP:
			{
				VARIANT_BOOL	IsDiscontinuity;
				VARIANT_BOOL	IsTimeStampExist;
				DOUBLE			TimeStamp;
				pVal->get_IsDiscontinuity(&IsDiscontinuity);
				pVal->get_IsTimeStampExist(&IsTimeStampExist);
				pVal->get_TimeStamp(&TimeStamp);

				if (IsDiscontinuity==VARIANT_FALSE)
				{
					if (IsTimeStampExist!=VARIANT_FALSE)
					{
						if (m_LastTimeStamp!=-1)
						{
							long lDelta=(long)((TimeStamp-m_LastTimeStamp)*pWaveFormat->nSamplesPerSec/1000);
							if (g_clMaxAudioGap*(long)pWaveFormat->nSamplesPerSec<=lDelta)
							{
								ATLASSERT(FALSE);
								lDelta = g_clMaxAudioGap*pWaveFormat->nSamplesPerSec;
							}
							if (lDelta>0)
							{
								IAVSAudioData*	pAudioData=NULL;
								CoCreateInstance(__uuidof(CAVSAudioData), NULL, CLSCTX_ALL, __uuidof(IAVSAudioData), (void**)&pAudioData);
								if (pAudioData!=NULL)
								{
									LONG	nBytesNeed=(lDelta*pWaveFormat->nChannels*pWaveFormat->wBitsPerSample)/8;
									pAudioData->AllocateBuffer(nBytesNeed+sizeof(WAVEFORMATEX)+pWaveFormat->cbSize);

									BYTE*	MemPtrDst=NULL;
									pAudioData->get_Buffer(&MemPtrDst);
									memcpy(MemPtrDst, pMemPtr, sizeof(WAVEFORMATEX)+pWaveFormat->cbSize);

									MemPtrDst+=sizeof(WAVEFORMATEX)+pWaveFormat->cbSize;
									memset(MemPtrDst, 0x00, nBytesNeed);
									m_CurrentBufferSize += lDelta; 
									m_AudioBuffer.Add(pAudioData);
								}
							}
						}
						m_LastTimeStamp=TimeStamp+1000.0*lDataSize/(pWaveFormat->nChannels*pWaveFormat->wBitsPerSample/8)/pWaveFormat->nSamplesPerSec;
					} 
					else
					{
						if (m_LastTimeStamp!=-1)
							m_LastTimeStamp+=1000.0*lDataSize/(pWaveFormat->nChannels*pWaveFormat->wBitsPerSample/8)/pWaveFormat->nSamplesPerSec;
					}
				}
				else
				{
					if (IsTimeStampExist!=VARIANT_FALSE)
					{
						m_LastTimeStamp=TimeStamp+1000.0*lDataSize/(pWaveFormat->nChannels*pWaveFormat->wBitsPerSample/8)/pWaveFormat->nSamplesPerSec;
					} 
					else
						m_LastTimeStamp=-1;
				}
			}
		};

		pVal->AddRef();
		m_AudioBuffer.Add(pVal);

		return S_OK;
	}
	STDMETHOD(GetData)(LONG nSamples, IAVSAudioData** pAudio)
	{
		CSynchAccess	SynchAccess=hSynchMutex;

		*pAudio=NULL;
		if (nSamples>m_CurrentBufferSize)
			return S_OK;
		if (m_AudioBuffer.GetCount()>0)	// always true
		{
			IAVSMediaData*	audio=m_AudioBuffer[0];
			BYTE*			MemPtr=NULL;
			WAVEFORMATEX*	wave_format;
			LONG			BufferSize;
			audio->get_Buffer(&MemPtr);
			audio->get_BufferSize(&BufferSize);
			wave_format=(WAVEFORMATEX*)MemPtr;
			LONG			nBytesNeed=(nSamples*wave_format->nChannels*wave_format->wBitsPerSample)/8;

			long lFrequency = wave_format->nSamplesPerSec;

			IAVSAudioData*	pMediaData=NULL;
			CoCreateInstance(__uuidof(CAVSAudioData), NULL, CLSCTX_ALL, __uuidof(IAVSAudioData), (void**)&pMediaData);
			if (pMediaData==NULL)
				return MEMORY;

			BYTE*			MemPtrDst=NULL;
			pMediaData->AllocateBuffer(nBytesNeed+sizeof(WAVEFORMATEX)+wave_format->cbSize);
			pMediaData->get_Buffer(&MemPtrDst);
			memcpy(MemPtrDst, MemPtr, sizeof(WAVEFORMATEX)+wave_format->cbSize);
			MemPtrDst+=sizeof(WAVEFORMATEX)+wave_format->cbSize;
			
			while (m_AudioBuffer.GetCount()>0)
			{
				audio=m_AudioBuffer[0];
				audio->get_Buffer(&MemPtr);
				audio->get_BufferSize(&BufferSize);
				wave_format=(WAVEFORMATEX*)MemPtr;

				MemPtr+=sizeof(WAVEFORMATEX)+wave_format->cbSize+m_LastOffset;
				BufferSize-=sizeof(WAVEFORMATEX)+wave_format->cbSize+m_LastOffset;

				LONG	size=min(nBytesNeed, BufferSize);
				memcpy(MemPtrDst, MemPtr, size);

				BufferSize-=size;
				nBytesNeed-=size;
				m_LastOffset+=size;
				MemPtrDst+=size;

				if (BufferSize==0)
				{
					m_LastOffset=0;
					m_AudioBuffer.RemoveAt(0);
					RELEASEINTERFACE(audio);
				}
				if (nBytesNeed==0)
					break;
			}
			*pAudio = pMediaData;
			m_CurrentBufferSize-=nSamples;
			if (0!=lFrequency)
			{
				double dDuration = 1000.0*(double)nSamples/(double)lFrequency;
				pMediaData->put_TimeStamp(m_CurrentTimeStamp);
				pMediaData->put_Duration(dDuration);
				m_CurrentTimeStamp += dDuration;
			}
		}
		return S_OK;
	}
	STDMETHOD(ClearBuffer)()
	{
		CSynchAccess	SynchAccess=hSynchMutex;

		m_CurrentBufferSize=0;
		m_CurrentTimeStamp=-1;
		m_LastTimeStamp=-1;
		m_LastOffset=0;
		m_AudioBuffer.FreeAll();
		return S_OK;
	}
};

// CAVSAudioBuffer
[coclass, uuid("7F8D7AC7-C05E-4986-9C31-DFEF2B7298CC"), threading(apartment), vi_progid("AVSMediaCore3.AVSAudioBuffer"), progid("AVSMediaCore3.AVSAudioBuffer.1"), version(1.0)]
class ATL_NO_VTABLE CAVSAudioBuffer : public CAVSAudioBufferImpl<CAVSAudioBuffer, IAVSAudioBuffer>
{
};
