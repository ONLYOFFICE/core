#pragma once

#include <wmsdk.h>
#include "AudioDefines.h"
#include "ATLDefine.h"

//#define LOG_PROFILE_WMA

typedef	HRESULT  (_stdcall *WMCREATEPROFILEMANAGER) (IWMProfileManager ** Manager);

static const DWORD g_carSampleRateTable[] = 
{
	8000, 11025, 12000, 16000, 24000, 32000, 44100, 48000, 88200, 96000
};

static const WORD g_carSampleSizeTable[] = 
{
	8, 16, 24, 32
};
static const WORD g_carChannelsTable[] =
{
	1, 2
};
static const DWORD g_carChannelsMaskTable[] = 
{
	1, 3
};


struct SWMACodecData
{
	SWMACodecData()
		: m_dwCodecIndex(0)
		, m_dwFormatIndex(0)
		, m_bVBR(FALSE)
		, m_lNumPasses(0)
	
		, m_sCodecName(_T(""))
		, m_sFormatName(_T(""))
		, m_pFormatData(NULL)

		, m_dwSampleRate(0)
		, m_wSampleSize(0)

		, m_wChannels(0)
		, m_dwChannelsMask(0)

		, m_dwBitrate(0)
		, m_wFormatTag(0)

		, m_pFormatEx(0)
	{		
	}
	SWMACodecData(const SWMACodecData &oSrc)
	{
		*this = oSrc;
	}
		
	~SWMACodecData()
	{
		RELEASEINTERFACE(m_pFormatData);
		if (NULL!=m_pFormatEx)
			::HeapFree(GetProcessHeap(), NULL, m_pFormatEx);
	}
		
	SWMACodecData &operator =(const SWMACodecData &oSrc)
	{
		m_dwCodecIndex		= oSrc.m_dwCodecIndex;
		m_dwFormatIndex		= oSrc.m_dwFormatIndex;
		m_bVBR				= oSrc.m_bVBR;
		m_lNumPasses		= oSrc.m_lNumPasses;
		
		m_sCodecName		= oSrc.m_sCodecName;
		m_sFormatName		= oSrc.m_sFormatName;
		
		m_pFormatData		= oSrc.m_pFormatData;
		ADDREFINTERFACE(m_pFormatData);

		m_dwSampleRate		= oSrc.m_dwSampleRate;
		m_wSampleSize		= oSrc.m_wSampleSize;

		m_wChannels			= oSrc.m_wChannels;
		m_dwChannelsMask	= oSrc.m_dwChannelsMask;

		m_dwBitrate			= oSrc.m_dwBitrate;
		m_wFormatTag		= oSrc.m_wFormatTag;

		InitFormatEx(oSrc.m_pFormatEx);			
		
		return (*this);
	}
		
	void InitFormatEx(WAVEFORMATEX *pFormatEx)
	{
		if (NULL==pFormatEx)
			return;
		if (NULL!=m_pFormatEx)
			::HeapFree(GetProcessHeap(), 0, m_pFormatEx);

		m_pFormatEx = (WAVEFORMATEX *)::HeapAlloc(GetProcessHeap(), 0, sizeof(WAVEFORMATEX) + pFormatEx->cbSize);
		if (NULL==m_pFormatEx)
			return;
		*m_pFormatEx = *pFormatEx;
		if (0!=pFormatEx->cbSize)
			memcpy((LPBYTE)m_pFormatEx + sizeof(WAVEFORMATEX), (LPBYTE)pFormatEx + sizeof(WAVEFORMATEX), pFormatEx->cbSize);
	}
		
	const WAVEFORMATEX *GetFormatEx() const
	{
		return m_pFormatEx;
	}
		
	BOOL IsFormatDataValid() const
	{
		return (NULL!=m_pFormatData);
	}
		
	void SetFormatData(IWMStreamConfig* pFormatData)
	{
		RELEASEINTERFACE(m_pFormatData);
		m_pFormatData = pFormatData;
		ADDREFINTERFACE(m_pFormatData);
	}
	void GetFormatData(IWMStreamConfig** ppFormatData) const
	{
		*ppFormatData = NULL;
		if (NULL==m_pFormatData)
			return;
		*ppFormatData = m_pFormatData;
		ADDREFINTERFACE((*ppFormatData));
	}
		
	DWORD m_dwCodecIndex;
	DWORD m_dwFormatIndex;
	BOOL m_bVBR;
	long m_lNumPasses;
	
	CString m_sCodecName;
	CString m_sFormatName;

	DWORD m_dwSampleRate;
	WORD m_wSampleSize;

	WORD m_wChannels;
	DWORD m_dwChannelsMask;

	DWORD m_dwBitrate;
	WORD m_wFormatTag;
protected:
	IWMStreamConfig* m_pFormatData;
	//у нас говенное решение, мы реализуем отложенную загрузку, соответственно снаружи
	//вначале получаем SWMACodecData потом провер€ем загружен ли m_pFormatData и если нет
	//вызываем CWMACodecsManager::InitFormatData
	WAVEFORMATEX *m_pFormatEx;
	//поскольку может быть расширенный формат, и тогда должна быть выделена непрерывна€ область пам€ти
	//вначале которой будет WAVEFORMATEX, а дальше расширение делаем указателем.
};
	
class CWMACodecsManager
{
public:
	CWMACodecsManager()
		: m_hDLL(NULL)
		, m_bEnabled(FALSE)
		, m_pCodecInfo(NULL)
		, m_pCodecInfo2(NULL)
		, m_pCodecInfo3(NULL)
		, m_pProfileManager(NULL)
	{
		LoadCodecsPCM();

		WMCreateProfileManager();
		if (NULL==m_pProfileManager)
			return;

		if (!InitCodecInfo())
			return;

		m_bEnabled = TRUE;

		LoadCodecsWMA();

#ifdef LOG_PROFILE_WMA
		SaveProfileWMAToFile();
#endif
	}
	virtual ~CWMACodecsManager(void)
	{
		RELEASEINTERFACE(m_pCodecInfo);
		RELEASEINTERFACE(m_pCodecInfo2);
		RELEASEINTERFACE(m_pCodecInfo3);
		RELEASEINTERFACE(m_pProfileManager);		
		if (NULL!=m_hDLL)
			FreeLibrary(m_hDLL);	
	}

		
	size_t GetCodecsCount() const
	{
		return m_arCodecs.GetCount();
	}

		
	const SWMACodecData& GetAt(size_t nIndex) const
	{
		return m_arCodecs.GetAt(nIndex);
	}
	SWMACodecData& GetAt(size_t nIndex)
	{
		return m_arCodecs.GetAt(nIndex);
	}

		
	const SWMACodecData& operator[](size_t nIndex) const
	{
		return m_arCodecs[nIndex];
	}
	SWMACodecData& operator[](size_t nIndex)
	{
		return m_arCodecs[nIndex];
	}	
		
	void InitFormatData(size_t nIndex)
	{
		if (nIndex >= m_arCodecs.GetCount())
			return;
		if (m_arCodecs[nIndex].IsFormatDataValid())
			return;
		if (!m_bEnabled)
			return;


		IWMProfile* pProfile = NULL;
		m_pProfileManager->CreateEmptyProfile(WMT_VER_9_0, &pProfile);
		
		if (NULL!=pProfile)
		{
			IWMProfile3* pProfile3 = NULL;
			pProfile->QueryInterface(IID_IWMProfile3, (void**)&pProfile3);

			if (NULL!=pProfile3)
			{
				IWMStreamConfig* pNewStream = NULL;
				pProfile3->CreateNewStream(WMMEDIATYPE_Audio, &pNewStream);

				if (NULL!=pNewStream)
				{
					IWMMediaProps* pMediaProps = NULL;
					pNewStream->QueryInterface( IID_IWMMediaProps, (void**) &pMediaProps);

					if (NULL != pMediaProps)
					{
						WM_MEDIA_TYPE* pMediaType = NULL;
						if (WAVE_FORMAT_PCM == m_arCodecs[nIndex].m_wFormatTag)
							GetMediaTypePCM(nIndex, &pMediaType);
						else 
							GetMediaTypeWMA(nIndex, &pMediaType);
						if (NULL!=pMediaType)
						{
							pMediaProps->SetMediaType(pMediaType);					
							::HeapFree(GetProcessHeap(), 0, pMediaType);
						}

						pMediaProps->Release();
					}

					pNewStream->SetBitrate(m_arCodecs[nIndex].m_dwBitrate);
					pNewStream->SetBufferWindow(3000); 

					IWMPropertyVault* pPropertyVault = NULL;
					pNewStream->QueryInterface(IID_IWMPropertyVault, (void**)&pPropertyVault);
					if (NULL!=pPropertyVault)
					{
						pPropertyVault->SetProperty(g_wszVBREnabled, WMT_TYPE_BOOL, (BYTE*)&m_arCodecs[nIndex].m_bVBR, sizeof(BOOL));
						pPropertyVault->Release();
					}

					pNewStream->SetStreamNumber(1);
					pNewStream->SetStreamName(L"");
					
					m_arCodecs[nIndex].SetFormatData(pNewStream);
					RELEASEINTERFACE(pNewStream);
				}
				pProfile3->Release();
			}
			pProfile->Release();
		}
	}
protected:
	HMODULE m_hDLL;
	BOOL m_bEnabled;
	IWMProfileManager* m_pProfileManager;
	IWMCodecInfo*  m_pCodecInfo;
	IWMCodecInfo2* m_pCodecInfo2;
	IWMCodecInfo3* m_pCodecInfo3;

	CAtlArray<SWMACodecData> m_arCodecs;
		
	HRESULT WMCreateProfileManager()
	{		
		CString sSysDir;
		::GetSystemDirectory(sSysDir.GetBuffer(MAX_PATH + 1), MAX_PATH);
		sSysDir.ReleaseBuffer();
		sSysDir += _T("\\wmvcore.dll");
		m_hDLL = ::LoadLibrary(sSysDir);//system dir
		//hDLL = LoadLibrary("D:\\Work\\WM\\8\\WMVCORE.DLL");
		//hDLL = LoadLibrary("D:\\Work\\WM\\9\\WMVCORE.DLL");
		//hDLL = LoadLibrary("D:\\Work\\WM\\10\\WMVCORE.DLL");
		if (NULL==m_hDLL)
			return S_FALSE;

		WMCREATEPROFILEMANAGER WMCreateProfileManagerDll = (WMCREATEPROFILEMANAGER)GetProcAddress(m_hDLL, "WMCreateProfileManager");
		if (NULL == WMCreateProfileManagerDll)
			return S_FALSE;

		return WMCreateProfileManagerDll(&m_pProfileManager);
	}

	BOOL InitCodecInfo()
	{
		m_pProfileManager->QueryInterface(IID_IWMCodecInfo, (void**)&m_pCodecInfo);
		if (NULL == m_pCodecInfo)
			return FALSE;		
		m_pProfileManager->QueryInterface(IID_IWMCodecInfo2, (void**)&m_pCodecInfo2);
		m_pProfileManager->QueryInterface(IID_IWMCodecInfo3, (void**)&m_pCodecInfo3);
		return TRUE;
	}

			
	void GetMediaTypePCM(size_t nIndex, WM_MEDIA_TYPE** ppType)
	{
		*ppType = NULL;
		if (nIndex >= m_arCodecs.GetCount())
			return;

		const size_t cnSize = sizeof(WM_MEDIA_TYPE) + sizeof(WAVEFORMATEX);

		WM_MEDIA_TYPE *pTempType = (WM_MEDIA_TYPE*)::HeapAlloc(GetProcessHeap(), 0, cnSize);
		*ppType = pTempType;
		if (NULL == pTempType)
			return;

		ZeroMemory(pTempType, cnSize);
	    
		pTempType->majortype			= WMMEDIATYPE_Audio;
		pTempType->subtype				= WMMEDIASUBTYPE_PCM;
		pTempType->bFixedSizeSamples	= TRUE;
		pTempType->bTemporalCompression	= FALSE;
		pTempType->lSampleSize			= m_arCodecs[nIndex].m_wChannels * m_arCodecs[nIndex].m_wSampleSize / 8;
		pTempType->formattype			= WMFORMAT_WaveFormatEx;
		pTempType->pUnk					= NULL;
		pTempType->cbFormat				= sizeof(WAVEFORMATEX);
		pTempType->pbFormat				= (LPBYTE)pTempType + sizeof(WM_MEDIA_TYPE);

		const WAVEFORMATEX *pFormatEx = m_arCodecs[nIndex].GetFormatEx();
		*((WAVEFORMATEX *)pTempType->pbFormat) = *pFormatEx;		
	}
			
	void GetMediaTypeWMA(size_t nIndex, WM_MEDIA_TYPE** ppType)
	{
		*ppType = NULL;
		if (nIndex >= m_arCodecs.GetCount())
			return;

		if (NULL!=m_pCodecInfo3)
			m_pCodecInfo3->SetCodecEnumerationSetting(WMMEDIATYPE_Audio, m_arCodecs[nIndex].m_dwCodecIndex, g_wszVBREnabled, WMT_TYPE_BOOL, (BYTE*)&m_arCodecs[nIndex].m_bVBR, sizeof(BOOL));

		IWMStreamConfig* pStream = NULL;		
		m_pCodecInfo2->GetCodecFormatDesc(WMMEDIATYPE_Audio, m_arCodecs[nIndex].m_dwCodecIndex, m_arCodecs[nIndex].m_dwFormatIndex, &pStream, NULL, NULL);
		if (NULL==pStream)
			return;

		IWMMediaProps* pMediaProps = NULL;
		pStream->QueryInterface(IID_IWMMediaProps, (void**)&pMediaProps);
		pStream->Release();
		if (NULL == pMediaProps)
			return;
		
		DWORD dwSize = 0;
		pMediaProps->GetMediaType(NULL, &dwSize);
		*ppType= (WM_MEDIA_TYPE* )::HeapAlloc(GetProcessHeap(), 0, dwSize);
		if (NULL == (*ppType))
			return;
		pMediaProps->GetMediaType(*ppType, &dwSize);
		pMediaProps->Release();
	}
		
	void LoadCodecsWMA()
	{	
		if ((!m_bEnabled) || (NULL==m_pCodecInfo2))
			return;
			
		long lVBRPresent = 0;		
		
		if (NULL != m_pCodecInfo3) 
			lVBRPresent++;

		DWORD dwCodecCount = 0;
		HRESULT hr = m_pCodecInfo->GetCodecInfoCount(WMMEDIATYPE_Audio, &dwCodecCount);
		if (S_OK!=hr)
			return;

		DWORD dwPasses = 1;
		
		for (DWORD dwCodecIndex = 0; dwCodecIndex < dwCodecCount; dwCodecIndex++) 
		{		
			if (NULL != m_pCodecInfo3)
			{
				dwPasses = 1;//29.07.2009 исправлено с dwPasses = 2; по просьбе јллы.
				m_pCodecInfo3->SetCodecEnumerationSetting(WMMEDIATYPE_Audio, dwCodecIndex, g_wszNumPasses, WMT_TYPE_DWORD, (BYTE*)&dwPasses, sizeof(DWORD));
			}
			for (long lVBR = 0; lVBR <= lVBRPresent; lVBR++)
			{
				BOOL bVBR = (0!=lVBR);
				
				HRESULT hr = S_OK;
				if (NULL != m_pCodecInfo3)
					hr = m_pCodecInfo3->SetCodecEnumerationSetting(WMMEDIATYPE_Audio, dwCodecIndex, g_wszVBREnabled, WMT_TYPE_BOOL, (BYTE*)&bVBR, sizeof(BOOL));
				
				if ((S_OK != hr) && (0!=lVBR))
					continue;
				
				DWORD dwFormatsCount = 0;
				hr = m_pCodecInfo->GetCodecFormatCount(WMMEDIATYPE_Audio, dwCodecIndex, &dwFormatsCount);
								
				SWMACodecData oCodec;
				if (NULL != m_pCodecInfo2)
				{
					DWORD dwNameLength = 0;
					m_pCodecInfo2->GetCodecName(WMMEDIATYPE_Audio, dwCodecIndex, NULL, &dwNameLength);
					if (dwNameLength > 0)
					{
						CStringW swName;
						m_pCodecInfo2->GetCodecName(WMMEDIATYPE_Audio, dwCodecIndex, swName.GetBuffer(dwNameLength + 1), &dwNameLength);
						swName.ReleaseBuffer();
						oCodec.m_sCodecName = swName;
					}
				}
				oCodec.m_bVBR = bVBR;
				oCodec.m_dwCodecIndex = dwCodecIndex;
				oCodec.m_lNumPasses = 1;

				if (NULL != m_pCodecInfo3)
				{
					dwPasses = 1;//29.07.2009 исправлено с dwPasses = 2; по просьбе јллы.
					hr = m_pCodecInfo3->SetCodecEnumerationSetting(WMMEDIATYPE_Audio, dwCodecIndex, g_wszNumPasses, WMT_TYPE_DWORD, (BYTE*)&dwPasses, sizeof(DWORD));
					if ((S_OK == hr) &&  (1 < dwPasses))
						oCodec.m_lNumPasses = dwPasses;
				}

				for (DWORD dwFormatIndex = 0; dwFormatIndex < dwFormatsCount; dwFormatIndex++)
				{
					if (NULL==m_pCodecInfo2)
						continue;

					DWORD dwSize = 0;
					hr = m_pCodecInfo2->GetCodecFormatDesc(WMMEDIATYPE_Audio, dwCodecIndex, dwFormatIndex, NULL, NULL, &dwSize);	
					if ((S_OK!=hr) || (dwSize<1))
						continue;

					CStringW swFormatDesc;
					IWMStreamConfig* pStream = NULL;
					m_pCodecInfo2->GetCodecFormatDesc(WMMEDIATYPE_Audio, dwCodecIndex, dwFormatIndex, &pStream, swFormatDesc.GetBuffer(dwSize + 1), &dwSize);
					swFormatDesc.ReleaseBuffer();					

					oCodec.m_sFormatName = swFormatDesc;

					if (NULL!=pStream)
					{
						oCodec.m_dwFormatIndex = dwFormatIndex;
							
						IWMMediaProps* pMediaProps_out = NULL;
						hr = pStream->QueryInterface(IID_IWMMediaProps, (void**)&pMediaProps_out);

						if (NULL!=pMediaProps_out)
						{
							pMediaProps_out->GetMediaType(NULL, &dwSize);

							WM_MEDIA_TYPE *pType = (WM_MEDIA_TYPE *)::HeapAlloc(GetProcessHeap(), 0, dwSize);
							hr = pMediaProps_out->GetMediaType(pType, &dwSize);

							if ((NULL!=pType) && (dwSize>0))
							{
								WAVEFORMATEX *pFormatEx = (WAVEFORMATEX*)pType->pbFormat;

								oCodec.m_wSampleSize	= pFormatEx->wBitsPerSample;
								oCodec.m_dwSampleRate	= pFormatEx->nSamplesPerSec;
								oCodec.m_wChannels		= pFormatEx->nChannels;
								
								if ((0 < pFormatEx->cbSize) && (2 < oCodec.m_wChannels))
									oCodec.m_dwChannelsMask = ((WAVEFORMATEXTENSIBLE*)pFormatEx)->dwChannelMask;
								else
									oCodec.m_dwChannelsMask = 3;

								oCodec.m_wFormatTag = pFormatEx->wFormatTag;

								oCodec.InitFormatEx(pFormatEx);
								pStream->GetBitrate(&oCodec.m_dwBitrate);							

								if (oCodec.m_wSampleSize < 8)
									oCodec.m_wSampleSize = 16;//

								m_arCodecs.Add(oCodec);
							}
							if (NULL!=pType)
								HeapFree(GetProcessHeap(), 0, pType);
							pMediaProps_out->Release();
						}						
						pStream->Release();
					}
				}
			}
		}
	}
		
	void LoadCodecsPCM()
	{
		SWMACodecData oCodec;
		
		oCodec.m_dwCodecIndex = -1;
		oCodec.m_bVBR = FALSE;
		oCodec.m_dwFormatIndex = -1;

		oCodec.m_sCodecName	= _T("PCM");
		oCodec.m_wFormatTag	= WAVE_FORMAT_PCM;

		WAVEFORMATEX oFormatEx;
		oFormatEx.cbSize = 0;
		oFormatEx.wFormatTag = WAVE_FORMAT_PCM;

		for (long lSampleRateIndex = 0; lSampleRateIndex < sizeof(g_carSampleRateTable)/sizeof(DWORD); lSampleRateIndex++)
		{
			for (long lChannelIndex = 0; lChannelIndex < sizeof(g_carChannelsTable)/sizeof(WORD); lChannelIndex++)
			{
				for (long lSampleSizeIndex = 0; lSampleSizeIndex < sizeof(g_carSampleSizeTable)/sizeof(WORD); lSampleSizeIndex++)
				{
					
					oFormatEx.wBitsPerSample	= oCodec.m_wSampleSize	= g_carSampleSizeTable[lSampleSizeIndex];
					oFormatEx.nSamplesPerSec	= oCodec.m_dwSampleRate	= g_carSampleRateTable[lSampleRateIndex];
					oFormatEx.nChannels			= oCodec.m_wChannels	= g_carChannelsTable[lChannelIndex];
					oCodec.m_dwChannelsMask		= g_carChannelsMaskTable[lChannelIndex];
					
					oFormatEx.nBlockAlign		= oFormatEx.wBitsPerSample/8 * oFormatEx.nChannels;

					oFormatEx.nAvgBytesPerSec	= oFormatEx.nBlockAlign * oFormatEx.nSamplesPerSec;
					oCodec.m_dwBitrate			= oFormatEx.nAvgBytesPerSec * 8;

					oCodec.m_sFormatName  = GetSampleRateStr(oCodec.m_dwSampleRate).m_str;
					oCodec.m_sFormatName += GetChannelsStr(oCodec.m_wChannels, oCodec.m_dwChannelsMask).m_str;
					oCodec.m_sFormatName += GetBitrateStr(oCodec.m_dwBitrate/1000, oCodec.m_dwBitrate/1000).m_str;
					oCodec.m_sFormatName += GetSampleSizeStr(oCodec.m_wSampleSize).m_str;
					//переписывать функции получени€ текста формата заменив ComBSTR на CString хочетс€, но нельз€, потому что
					//они используютс€ в других местах

					oCodec.InitFormatEx(&oFormatEx);

					m_arCodecs.Add(oCodec);
				}
			}
		}
	}
	void SaveProfileWMAToFile()
	{
		if (m_arCodecs.GetCount()<1)return;

		FILE*	fLog = fopen("c:\\avslog_wmprofile.log","wt");
		CStringA tmp;

		for (long i=0;i<m_arCodecs.GetCount() && fLog;i++)
		{
			//fprintf(fLog,"[%d ; %d]\n",m_arCodecs[i].m_dwCodecIndex,m_arCodecs[i].m_dwFormatIndex);
		
			tmp=m_arCodecs[i].m_sCodecName;
			fprintf(fLog,"%s (%d)\t%4d-%1d-%2d\n",tmp.GetBuffer(),m_arCodecs[i].m_wFormatTag,
								m_arCodecs[i].m_dwSampleRate,m_arCodecs[i].m_wChannels,m_arCodecs[i].m_wSampleSize);
		
			fprintf(fLog,"\t(%d; %d; %d; %d)\n",m_arCodecs[i].m_lNumPasses,m_arCodecs[i].m_dwBitrate,m_arCodecs[i].m_dwChannelsMask,m_arCodecs[i].m_bVBR);
		
			fprintf(fLog,"-------------------------------------------------------------------------\n\n");
			
			fflush(fLog);
		}
		fclose(fLog);
	}

};
