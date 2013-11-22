// ASCOggVorbisAudioFormat.h : Declaration of the CASCOggVorbisAudioFormat

#pragma once
#include "resource.h"       // main symbols
#include "ASCAudioFormat.h" 


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

#define VORBISACM_VERSION			0x20020201
#define LIBVORBIS_VERSION			0x20020717	

// IAVSOggVorbisAudioFormat
[
	object,
	uuid("55EBA440-9382-4FCE-8146-8607D9C625CA"),
	dual,	helpstring("IAVSOggVorbisAudioFormat Interface"),
	pointer_default(unique)
]
__interface IAVSOggVorbisAudioFormat : IAVSAudioFormat
{
	[id(116000 + 0), propget] HRESULT MinBitrate([out, retval] long* pVal);
	[id(116000 + 0), propput] HRESULT MinBitrate([in] long newVal);

	[id(116000 + 1), propget] HRESULT MaxBitrate([out, retval] long* pVal);
	[id(116000 + 1), propput] HRESULT MaxBitrate([in] long newVal);

	[id(116000 + 2), propget] HRESULT Quality([out, retval] long* pVal); //-200 (low) < 1000 
	[id(116000 + 2), propput] HRESULT Quality([in] long newVal);

	[id(116000 + 3), propget] HRESULT VBRCodingMode([out, retval] long* pVal);//1 - Quality; 2- CBR; 3- VBR; 4- ABR 
	[id(116000 + 3), propput] HRESULT VBRCodingMode([in] long newVal);

	[id(116000 + 10),propget] HRESULT LowPassFrequency([out, retval] long *pVal); //def 0 Hz  2000 < 99000 Hz
	[id(116000 + 10),propput] HRESULT LowPassFrequency([in] long newVal);

	[id(116000 + 11),propget] HRESULT ImpulseNoiseTune([out, retval] double *pVal);//def 0.0 -15.0 < 0.0
	[id(116000 + 11),propput] HRESULT ImpulseNoiseTune([in] double newVal);

	[id(116000 + 12),propget] HRESULT ScaleFactor([out, retval] double *pVal); // def 100%   volume amplify
	[id(116000 + 12),propput] HRESULT ScaleFactor([in] double newVal);

};

static DWORD _OggChannelsMask[9]=
{
	0,
	SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT,
	SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT,
	SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | SPEAKER_FRONT_CENTER, //3.0
	SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT, //2+2
	SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | SPEAKER_FRONT_CENTER | SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT, //5.0
	SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | SPEAKER_FRONT_CENTER | SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT | SPEAKER_LOW_FREQUENCY, //5.1
	SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | SPEAKER_FRONT_CENTER | SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT | SPEAKER_BACK_CENTER | SPEAKER_LOW_FREQUENCY, //6.1,
	SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | SPEAKER_FRONT_CENTER | SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT | SPEAKER_SIDE_LEFT | SPEAKER_SIDE_RIGHT | SPEAKER_LOW_FREQUENCY, //6.1,};
};
// CAVSOggVorbisAudioFormat
template <class CoT, class T>
class CAVSOggVorbisAudioFormatImpl : public CAVSAudioFormatImpl<CoT, T>
{

public:
	CAVSOggVorbisAudioFormatImpl()
	{
		m_lMediaType = MEDIAFORMAT_OGGVORBISAUDIO;
		SetDefaultProperties();
	}
	~CAVSOggVorbisAudioFormatImpl()
	{
	}
	STDMETHOD(CreateDuplicate)(IAVSMediaFormat** pMediaFormat)
	{
		CAVSAudioFormatImpl<CoT, T>::CreateDuplicate(pMediaFormat);
		if (NULL==(*pMediaFormat))
			return S_OK;

		IAVSOggVorbisAudioFormat* pAVSOggVorbisFormat=NULL;
		(*pMediaFormat)->QueryInterface(__uuidof(IAVSOggVorbisAudioFormat), (void**)&pAVSOggVorbisFormat);
		if (NULL==pAVSOggVorbisFormat)
		{
			(*pMediaFormat)->Release();
			*pMediaFormat = NULL;
			return S_OK;
		}
		pAVSOggVorbisFormat->put_MaxBitrate(m_lMinBitrate);
		pAVSOggVorbisFormat->put_MinBitrate(m_lMaxBitrate);
		pAVSOggVorbisFormat->put_Quality(m_lQuality);
		pAVSOggVorbisFormat->put_VBRCodingMode(m_lVBRCodingMode);
		pAVSOggVorbisFormat->put_LowPassFrequency(m_lLowPassFrequency);
		pAVSOggVorbisFormat->put_ImpulseNoiseTune(m_dImpulseNoiseTune);
		pAVSOggVorbisFormat->put_ScaleFactor(m_dScaleFactor);
		pAVSOggVorbisFormat->Release();

		return S_OK;
	}
	STDMETHOD(SetDefaultProperties)(void)
	{
		CAVSAudioFormatImpl<CoT, T>::SetDefaultProperties();

		m_lFormatTag = 0x674f;
		m_lBitrate=128*1000;

		m_lMinBitrate=0;
		m_lMaxBitrate=0;
		m_lQuality=400;	
		m_lVBRCodingMode=2;
		m_lLowPassFrequency=0;
		m_dImpulseNoiseTune=0;
		m_dScaleFactor=100;
		return S_OK;
	}

	STDMETHOD(SetPropertiesByFormat)(IAVSMediaFormat* pMediaFormat)
	{
		CAVSAudioFormatImpl<CoT, T>::SetPropertiesByFormat(pMediaFormat);
		if (pMediaFormat!=NULL)
		{
			IAVSOggVorbisAudioFormat* pAVSOggVorbisFormat=NULL;
			pMediaFormat->QueryInterface(__uuidof(IAVSOggVorbisAudioFormat), (void**)&pAVSOggVorbisFormat);
			if (pAVSOggVorbisFormat!=NULL)
			{
				pAVSOggVorbisFormat->get_MinBitrate(&m_lMinBitrate);
				pAVSOggVorbisFormat->get_MaxBitrate(&m_lMaxBitrate);
				pAVSOggVorbisFormat->get_Quality(&m_lQuality);	
				pAVSOggVorbisFormat->get_ImpulseNoiseTune(&m_dImpulseNoiseTune);
				pAVSOggVorbisFormat->get_LowPassFrequency(&m_lLowPassFrequency);
				pAVSOggVorbisFormat->get_ScaleFactor(&m_dScaleFactor);
				pAVSOggVorbisFormat->get_VBRCodingMode(&m_lVBRCodingMode);
				RELEASEINTERFACE(pAVSOggVorbisFormat);
			}
		}
		return S_OK;
	}
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		//Здесь проверяем параметры относящиеся к AVSOggVorbisAudioFormat
		return CAVSAudioFormatImpl<CoT, T>::SetAdditionalParam(ParamName, ParamValue);
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		//Здесь проверяем параметры относящиеся к AVSOggVorbisAudioFormat
		return CAVSAudioFormatImpl<CoT, T>::GetAdditionalParam(ParamName, ParamValue);
	}
	
	STDMETHOD(GetFormatHeader)(BYTE** Data, long* Size)
	{
		*Data = NULL;
		*Size = 0;
	
		if (!m_lChannelsMask)	
			SetChannelsMaskPreset(m_lChannels,(DWORD&)m_lChannelsMask);
		else
		{
			m_lChannelsMask=_OggChannelsMask[m_lChannels];
		}

		OGGWAVEFORMAT format;

		format.wfx.cbSize=12;
		format.wfx.wBitsPerSample=16;
		format.wfx.nBlockAlign=1;
		format.wfx.wFormatTag=WAVE_FORMAT_OGG;
		format.wfx.nChannels=(WORD)m_lChannels;
		if (format.wfx.nChannels>2)format.wfx.nChannels=2;
		format.wfx.nSamplesPerSec=m_lSampleRate;
		format.wfx.nAvgBytesPerSec=m_lBitrate/8;
		
		format.dwVorbisACMVersion=VORBISACM_VERSION;
		format.dwLibVorbisVersion=LIBVORBIS_VERSION;
		format.dwChannelsMask=m_lChannelsMask;

		*Size=sizeof(WAVEFORMATEX)+12;
		*Data = (BYTE*)HeapAlloc(GetProcessHeap(),0,*Size);
		if (*Data)
		{
			memcpy(*Data,(BYTE*)&format,*Size);
		}
		return S_OK;
	}
	STDMETHOD(put_Channels)(long newVal)
	{
		CAVSAudioFormatImpl<CoT, T>::put_Channels(newVal);

		m_lChannelsMask=_OggChannelsMask[m_lChannels];
		return S_OK;
	}

	LINK_PROPERTY_LONG(MinBitrate,m_lMinBitrate);
	LINK_PROPERTY_LONG(MaxBitrate,m_lMaxBitrate);
	LINK_PROPERTY_LONG(Quality,m_lQuality);
	LINK_PROPERTY_LONG(VBRCodingMode,m_lVBRCodingMode);
	LINK_PROPERTY_LONG(LowPassFrequency,m_lLowPassFrequency);
	LINK_PROPERTY_DOUBLE(ImpulseNoiseTune,m_dImpulseNoiseTune);
	LINK_PROPERTY_DOUBLE(ScaleFactor,m_dScaleFactor);

private:
	long m_lMinBitrate;
	long m_lMaxBitrate;
	long m_lQuality;		
	long m_lVBRCodingMode;
	long m_lLowPassFrequency;
	double m_dImpulseNoiseTune;
	double m_dScaleFactor;
};


[
	coclass,
	default(IAVSOggVorbisAudioFormat),
	threading(apartment),
	vi_progid("AVSMediaFormatSettings3.AVSOggVorbisAud"),
	progid("AVSMediaFormatSettings3.AVSOggVorbisA.1"),
	version(1.0),
	uuid("49C5B8D7-3EB7-4AFD-AAED-2E18DB705610"),
	helpstring("AVSOggVorbisAudioFormat Class")
]
class ATL_NO_VTABLE CAVSOggVorbisAudioFormat : public CAVSOggVorbisAudioFormatImpl<CAVSOggVorbisAudioFormat, IAVSOggVorbisAudioFormat>
{
};
