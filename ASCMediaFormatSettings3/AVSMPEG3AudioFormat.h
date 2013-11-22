// AVSMPEG3AudioFormat.h : Declaration of the CAVSMPEG3AudioFormat

#pragma once
#include "resource.h"       // main symbols


// IAVSMPEG3AudioFormat
[
	object,
	uuid("0BF215D8-5DD3-4EBF-B185-DC90F4FFF824"),
	dual,	helpstring("IAVSMPEG3AudioFormat Interface"),
	pointer_default(unique)
]
__interface IAVSMPEG3AudioFormat  : IAVSAudioFormat
{
		[propget, id(113000 + 5)] HRESULT Preset([out, retval] long *pVal);
		[propput, id(113000 + 5)] HRESULT Preset([in] long newVal);

		[propget, id(113000 + 6)] HRESULT AltPreset([out, retval] long *pVal);
		[propput, id(113000 + 6)] HRESULT AltPreset([in] long newVal);

		[propget, id(113000 + 10)] HRESULT VBRBitrate([out, retval] long *pVal);
		[propput, id(113000 + 10)] HRESULT VBRBitrate([in] long newVal);
		[propget, id(113000 + 11)] HRESULT VBRQuality([out, retval] long *pVal);
		[propput, id(113000 + 11)] HRESULT VBRQuality([in] long newVal);
		[propget, id(113000 + 12)] HRESULT VBRCodingMethod([out, retval] long *pVal);
		[propput, id(113000 + 12)] HRESULT VBRCodingMethod([in] long newVal);
		[propget, id(113000 + 13)] HRESULT ABRBitrate([out, retval] long *pVal);
		[propput, id(113000 + 13)] HRESULT ABRBitrate([in] long newVal);
		[propget, id(113000 + 14)] HRESULT TypeStereo([out, retval] long *pVal);
		[propput, id(113000 + 14)] HRESULT TypeStereo([in] long newVal);
		[propget, id(113000 + 15)] HRESULT Quality([out, retval] long *pVal);
		[propput, id(113000 + 15)] HRESULT Quality([in] long newVal);
		[propget, id(113000 + 16)] HRESULT WritingVBRHeader([out, retval] VARIANT_BOOL *pVal);
		[propput, id(113000 + 16)] HRESULT WritingVBRHeader([in] VARIANT_BOOL newVal);
		[propget, id(113000 + 17)] HRESULT BitReservoir([out, retval] VARIANT_BOOL *pVal);
		[propput, id(113000 + 17)] HRESULT BitReservoir([in] VARIANT_BOOL newVal);
		[propget, id(113000 + 18)] HRESULT BlockTypeControl([out, retval] VARIANT_BOOL *pVal);
		[propput, id(113000 + 18)] HRESULT BlockTypeControl([in] VARIANT_BOOL newVal);
		[propget, id(113000 + 19)] HRESULT ShortBlocks([out, retval] VARIANT_BOOL *pVal);
		[propput, id(113000 + 19)] HRESULT ShortBlocks([in] VARIANT_BOOL newVal);
		[propget, id(113000 + 20)] HRESULT Emphasis([out, retval] long *pVal);
		[propput, id(113000 + 20)] HRESULT Emphasis([in] long newVal);
		[propget, id(113000 + 21)] HRESULT StrictISOCompliance([out, retval] VARIANT_BOOL *pVal);
		[propput, id(113000 + 21)] HRESULT StrictISOCompliance([in] VARIANT_BOOL newVal);
		[propget, id(113000 + 22)] HRESULT StrictlyEnforceMinBitrate([out, retval] VARIANT_BOOL *pVal);
		[propput, id(113000 + 22)] HRESULT StrictlyEnforceMinBitrate([in] VARIANT_BOOL newVal);
		[propget, id(113000 + 23)] HRESULT ATHControlMode([out, retval] long *pVal);
		[propput, id(113000 + 23)] HRESULT ATHControlMode([in] long newVal);
		[propget, id(113000 + 24)] HRESULT LowPassFrequency([out, retval] long *pVal);
		[propput, id(113000 + 24)] HRESULT LowPassFrequency([in] long newVal);
		[propget, id(113000 + 25)] HRESULT HighPassFrequency([out, retval] long *pVal);
		[propput, id(113000 + 25)] HRESULT HighPassFrequency([in] long newVal);
		[propget, id(113000 + 26)] HRESULT LowPassWidth([out, retval] long *pVal);
		[propput, id(113000 + 26)] HRESULT LowPassWidth([in] long newVal);
		[propget, id(113000 + 27)] HRESULT HighPassWidth([out, retval] long *pVal);
		[propput, id(113000 + 27)] HRESULT HighPassWidth([in] long newVal);
		[propget, id(113000 + 29)] HRESULT ErrorProtection([out, retval] VARIANT_BOOL *pVal);
		[propput, id(113000 + 29)] HRESULT ErrorProtection([in] VARIANT_BOOL newVal);
};



// CAVSMPEG3AudioFormat
template <class CoT, class T>
class CAVSMPEG3AudioFormatImpl : public CAVSAudioFormatImpl<CoT, T>
{

public:
	CAVSMPEG3AudioFormatImpl()
	{
		m_lMediaType = MEDIAFORMAT_MPEG3AUDIO;
		SetDefaultProperties();
	}
	~CAVSMPEG3AudioFormatImpl()
	{
	}
	STDMETHOD(CreateDuplicate)(IAVSMediaFormat** pMediaFormat)
	{
		CAVSAudioFormatImpl<CoT, T>::CreateDuplicate(pMediaFormat);
		if (NULL==(*pMediaFormat))
			return S_OK;

		IAVSMPEG3AudioFormat* pAVSMPEG3Format=NULL;
		(*pMediaFormat)->QueryInterface(__uuidof(IAVSMPEG3AudioFormat), (void**)&pAVSMPEG3Format);
		if (NULL==pAVSMPEG3Format)
		{
			(*pMediaFormat)->Release();
			*pMediaFormat = NULL;
			return S_OK;
		}
		pAVSMPEG3Format->put_VBRBitrate(VBRBitrate);
		pAVSMPEG3Format->put_Quality(Quality);
		pAVSMPEG3Format->put_VBRQuality(VBRQuality);
		pAVSMPEG3Format->put_VBRCodingMethod(VBRCodingMethod);
		pAVSMPEG3Format->put_ABRBitrate(ABRBitrate);
		pAVSMPEG3Format->put_ErrorProtection(ErrorProtection);
		pAVSMPEG3Format->put_WritingVBRHeader(WritingVBRHeader);
		pAVSMPEG3Format->put_BitReservoir(BitReservoir);
		pAVSMPEG3Format->put_BlockTypeControl(BlockTypeControl);
		pAVSMPEG3Format->put_ShortBlocks(ShortBlocks);
		pAVSMPEG3Format->put_StrictISOCompliance(StrictISOCompliance);
		pAVSMPEG3Format->put_StrictlyEnforceMinBitrate(StrictlyEnforceMinBitrate);			
		pAVSMPEG3Format->put_LowPassFrequency(LowPassFreq);
		pAVSMPEG3Format->put_HighPassFrequency(HighPassFreq);
		pAVSMPEG3Format->put_LowPassWidth(LowPassWidth); 
		pAVSMPEG3Format->put_HighPassWidth(HighPassWidth); 
		pAVSMPEG3Format->put_ATHControlMode(ATHControlMode);
		pAVSMPEG3Format->put_Emphasis(Emphasis);
		pAVSMPEG3Format->put_TypeStereo(TypeStereo);
		pAVSMPEG3Format->Release();

		return S_OK;
	}
	STDMETHOD(SetDefaultProperties)(void)
	{
		CAVSAudioFormatImpl<CoT, T>::SetDefaultProperties();

		m_lFormatTag = WAVE_FORMAT_MPEGLAYER3;
		m_lBitrate=128*1000;

		m_lBitrate=128*1000;
		VBRBitrate=192*1000;
		Quality=5;
		TypeStereo=1;
		VBRQuality=7;
		VBRCodingMethod=-1;
		ABRBitrate=128*1000;
		ErrorProtection=0;
		WritingVBRHeader=VARIANT_TRUE;
		BitReservoir=VARIANT_TRUE;
		BlockTypeControl=0;
		ShortBlocks=VARIANT_TRUE;
		StrictISOCompliance=0;
		StrictlyEnforceMinBitrate=0;			
		LowPassFreq=-1; //-1 - no ; 0 - LameChoose ;...
		HighPassFreq=-1; //-1 - no ; 0 - LameChoose ;...
		LowPassWidth=0; //0 - LameChoose (15%);...
		HighPassWidth=0; //0 - LameChoose (15%);...
		ATHControlMode=0; //0-default ; 1- only , 2 - disable ; 3- only for long blocks
		Emphasis=0;
		Preset=0;
		TypeStereo=1;
		return S_OK;
	}
	STDMETHOD(SetPropertiesByFormat)(IAVSMediaFormat* pMediaFormat)
	{
		CAVSAudioFormatImpl<CoT, T>::SetPropertiesByFormat(pMediaFormat);
		if (pMediaFormat!=NULL)
		{
			IAVSMPEG3AudioFormat* pAVSMPEG3Format=NULL;
			pMediaFormat->QueryInterface(__uuidof(IAVSMPEG3AudioFormat), (void**)&pAVSMPEG3Format);
			if (pAVSMPEG3Format!=NULL)
			{
				pAVSMPEG3Format->get_VBRBitrate(&VBRBitrate);
				pAVSMPEG3Format->get_Quality(&Quality);
				pAVSMPEG3Format->get_VBRQuality(&VBRQuality);
				pAVSMPEG3Format->get_VBRCodingMethod(&VBRCodingMethod);
				pAVSMPEG3Format->get_ABRBitrate(&ABRBitrate);
				pAVSMPEG3Format->get_ErrorProtection(&ErrorProtection);
				pAVSMPEG3Format->get_WritingVBRHeader(&WritingVBRHeader);
				pAVSMPEG3Format->get_BitReservoir(&BitReservoir);
				pAVSMPEG3Format->get_BlockTypeControl(&BlockTypeControl);
				pAVSMPEG3Format->get_ShortBlocks(&ShortBlocks);
				pAVSMPEG3Format->get_StrictISOCompliance(&StrictISOCompliance);
				pAVSMPEG3Format->get_StrictlyEnforceMinBitrate(&StrictlyEnforceMinBitrate);			
				pAVSMPEG3Format->get_LowPassFrequency(&LowPassFreq);
				pAVSMPEG3Format->get_HighPassFrequency(&HighPassFreq);
				pAVSMPEG3Format->get_LowPassWidth(&LowPassWidth); 
				pAVSMPEG3Format->get_HighPassWidth(&HighPassWidth); 
				pAVSMPEG3Format->get_ATHControlMode(&ATHControlMode);
				pAVSMPEG3Format->get_Emphasis(&Emphasis);
				pAVSMPEG3Format->get_TypeStereo(&TypeStereo);
				RELEASEINTERFACE(pAVSMPEG3Format);
			}
		}
		return S_OK;
	}
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		//Здесь проверяем параметры относящиеся к AVSMPEG3AudioFormat
		return CAVSAudioFormatImpl<CoT, T>::SetAdditionalParam(ParamName, ParamValue);
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		//Здесь проверяем параметры относящиеся к AVSMPEG3AudioFormat
		return CAVSAudioFormatImpl<CoT, T>::GetAdditionalParam(ParamName, ParamValue);
	}
	
	STDMETHOD(GetFormatHeader)(BYTE** Data, long* Size)
	{
		*Data = NULL;
		*Size = 0;
	
		if (!m_lChannelsMask)	
			SetChannelsMaskPreset(m_lChannels, (DWORD&)m_lChannelsMask);
		else 
			CheckChannelsMask(m_lChannels, (DWORD&)m_lChannelsMask);
	
		long SpecificationLen=0;
		char* Specification=NULL;

		MPEGLAYER3WAVEFORMAT format;

		format.wfx.cbSize = MPEGLAYER3_WFX_EXTRA_BYTES;
		format.wfx.wBitsPerSample = 0;
		format.wfx.nBlockAlign = 1;
		format.wfx.wFormatTag = WAVE_FORMAT_MPEGLAYER3;
		format.wfx.nChannels = (WORD)m_lChannels;
		if (2 < format.wfx.nChannels)
			format.wfx.nChannels = 2;
		format.wfx.nSamplesPerSec = m_lSampleRate;
		format.wfx.nAvgBytesPerSec = m_lBitrate/8;
		
		format.fdwFlags = 34;//MPEGLAYER3_FLAG_PADDING_OFF;
		format.nBlockSize = (short)(144*m_lBitrate/m_lSampleRate);
		if (32000 > m_lSampleRate)
			format.nBlockSize /= 2;
		format.nCodecDelay = 0;//0x0571;
		format.nFramesPerBlock = 1;
		format.wID = MPEGLAYER3_ID_MPEG;
	
		*Size=sizeof(WAVEFORMATEX) + MPEGLAYER3_WFX_EXTRA_BYTES;
		*Data = (BYTE*)HeapAlloc(GetProcessHeap(), 0, *Size);
		if (*Data)
		{
			memcpy(*Data, (BYTE*)&format, *Size);
		}
		return S_OK;
	}
	
	LINK_PROPERTY_LONG(VBRBitrate,VBRBitrate);
	LINK_PROPERTY_LONG(Quality,Quality);
	LINK_PROPERTY_LONG(VBRQuality,VBRQuality);
	LINK_PROPERTY_LONG(VBRCodingMethod,VBRCodingMethod);
	LINK_PROPERTY_LONG(ABRBitrate,ABRBitrate);
	LINK_PROPERTY_BOOL(ErrorProtection,ErrorProtection);
	LINK_PROPERTY_BOOL(WritingVBRHeader,WritingVBRHeader);
	LINK_PROPERTY_BOOL(BitReservoir,BitReservoir);
	LINK_PROPERTY_BOOL(BlockTypeControl,BlockTypeControl);
	LINK_PROPERTY_BOOL(ShortBlocks,ShortBlocks);
	LINK_PROPERTY_BOOL(StrictISOCompliance,StrictISOCompliance);
	LINK_PROPERTY_BOOL(StrictlyEnforceMinBitrate,StrictlyEnforceMinBitrate);			
	LINK_PROPERTY_LONG(LowPassFrequency,LowPassFreq);
	LINK_PROPERTY_LONG(HighPassFrequency,HighPassFreq);
	LINK_PROPERTY_LONG(LowPassWidth,LowPassWidth); 
	LINK_PROPERTY_LONG(HighPassWidth,HighPassWidth); 
	LINK_PROPERTY_LONG(ATHControlMode,ATHControlMode);
	LINK_PROPERTY_LONG(Emphasis,Emphasis);
	LINK_PROPERTY_LONG(TypeStereo,TypeStereo);

	STDMETHOD (get_Preset)(long *pVal)
	{
		if (Preset>99)*pVal=0;
		else *pVal=Preset;
		return S_OK;
	}

	STDMETHOD (put_Preset)(long newVal)
	{
		if (newVal>0)
			SetParamFromPreset(newVal);
		
		return S_OK;
	}


	STDMETHOD (get_AltPreset)(long *pVal)
	{
		if (Preset<100)*pVal=0;
		else *pVal=Preset-100;
		
		return S_OK;
	}

	STDMETHOD (put_AltPreset)(long newVal)
	{
		if (newVal>0)
			SetParamFromPreset(newVal+100);
		return S_OK;
	}
protected:
	long VBRBitrate;
	long Quality;
	long VBRQuality;
	long VBRCodingMethod;
	long ABRBitrate;
	VARIANT_BOOL ErrorProtection;
	VARIANT_BOOL WritingVBRHeader;
	VARIANT_BOOL BitReservoir;
	VARIANT_BOOL BlockTypeControl;
	VARIANT_BOOL ShortBlocks;
	VARIANT_BOOL StrictISOCompliance;
	VARIANT_BOOL StrictlyEnforceMinBitrate;			
	long LowPassFreq;
	long HighPassFreq;
	long LowPassWidth; 
	long HighPassWidth; 
	long ATHControlMode;
	long Emphasis;
	long Preset;
	long TypeStereo;

	void SetParamFromPreset(long Preset)
	{
		switch(Preset){
		case 1: //Phone
			m_lSampleRate=8000; m_lChannels=1; m_lBitrate=8000;Quality=5;TypeStereo=1;	
			VBRCodingMethod=0;	VBRBitrate=56000;VBRQuality=6;	
			LowPassFreq=3200;	LowPassWidth=1000; 
			HighPassFreq=-1;HighPassWidth=0;Preset=0;
			break;
		case 2: //Sw
			m_lSampleRate=11025; m_lChannels=1; m_lBitrate=8000;Quality=5;TypeStereo=1;	
			VBRCodingMethod=0;	VBRBitrate=64000;VBRQuality=5;	
			LowPassFreq=4800;	LowPassWidth=500; 
			HighPassFreq=-1;HighPassWidth=0;Preset=0;
			break;
		case 3: //Am
			m_lSampleRate=16000; m_lChannels=1; m_lBitrate=24000;Quality=5;TypeStereo=1;	
			VBRCodingMethod=0;	VBRBitrate=160000;VBRQuality=5;	
			LowPassFreq=7200;	LowPassWidth=500; 
			HighPassFreq=-1;HighPassWidth=0;Preset=0;
			break;
		case 4: //Fm
			m_lSampleRate=22050; m_lChannels=2; m_lBitrate=16000;Quality=5;TypeStereo=2;	
			VBRCodingMethod=0;	VBRBitrate=128000;VBRQuality=5;	
			LowPassFreq=9950;	LowPassWidth=880; 
			HighPassFreq=-1;HighPassWidth=0;Preset=0;
			break;
		case 5: //Voice
			m_lSampleRate=32000; m_lChannels=1; m_lBitrate=32000;Quality=5;TypeStereo=2;	
			VBRCodingMethod=0;	VBRBitrate=128000;VBRQuality=5;	
			LowPassFreq=12300;	LowPassWidth=2000; 
			HighPassFreq=-1;HighPassWidth=0;ShortBlocks=0;Preset=0;
			break;
		case 6: //Radio
			m_lSampleRate=44100; m_lChannels=2; m_lBitrate=64000;Quality=5;TypeStereo=2;	
			VBRCodingMethod=0;	VBRBitrate=256000;VBRQuality=4;	
			LowPassFreq=15000;	LowPassWidth=0; 
			HighPassFreq=-1;HighPassWidth=0;Preset=0;
			break;
		case 7: //Tape
			m_lSampleRate=44100; m_lChannels=2; m_lBitrate=96000;Quality=5;TypeStereo=2;	
			VBRCodingMethod=0;	VBRBitrate=320000;VBRQuality=4;	
			LowPassFreq=18500;	LowPassWidth=2000; 
			HighPassFreq=-1;HighPassWidth=0;Preset=0;
			break;
		case 8: //HiFi
			m_lSampleRate=44100; m_lChannels=2; m_lBitrate=112000;Quality=2;TypeStereo=2;	
			VBRCodingMethod=0;	VBRBitrate=320000;VBRQuality=3;	
			LowPassFreq=20240;	LowPassWidth=2200; 
			HighPassFreq=-1;HighPassWidth=0;Preset=0;
			break;
		case 9: //CD
			m_lSampleRate=44100; m_lChannels=2; m_lBitrate=128000;Quality=2;TypeStereo=1;	
			VBRCodingMethod=0;	VBRBitrate=320000;VBRQuality=2;	
			LowPassFreq=-1;	LowPassWidth=0; 
			HighPassFreq=-1;HighPassWidth=0;Preset=0;
			break;
		case 10: //Studio
			m_lSampleRate=44100; m_lChannels=2; m_lBitrate=160000;Quality=2;TypeStereo=1;	
			VBRCodingMethod=0;	VBRBitrate=320000;VBRQuality=0;	
			LowPassFreq=-1;	LowPassWidth=0; 
			HighPassFreq=-1;HighPassWidth=0;Preset=0;
			break;
		case 11: //R3Mix
			m_lSampleRate=44100; m_lChannels=2; m_lBitrate=96000;Quality=2;TypeStereo=2;	
			VBRCodingMethod=VBR_MTRH;	VBRBitrate=320000;VBRQuality=1;	
			LowPassFreq=19500;	LowPassWidth=0; 
			HighPassFreq=-1;HighPassWidth=0;Preset=1;
			break;
		case 101: //FastStandart
			m_lSampleRate=44100; m_lChannels=2; m_lBitrate=128000;Quality=2;TypeStereo=2;	
			VBRCodingMethod=VBR_NEW;	VBRBitrate=320000;VBRQuality=1;	
			LowPassFreq=19000;	LowPassWidth=0; 
			HighPassFreq=-1;HighPassWidth=0;Preset=2;
			break;
		case 102: //Standart
			m_lSampleRate=44100; m_lChannels=2; m_lBitrate=128000;Quality=2;TypeStereo=2;	
			VBRCodingMethod=VBR_OLD;	VBRBitrate=320000;VBRQuality=1;	
			LowPassFreq=19000;	LowPassWidth=0; 
			HighPassFreq=-1;HighPassWidth=0;Preset=2;
			break;
		case 103: //FastExtreme
			m_lSampleRate=44100; m_lChannels=2; m_lBitrate=128000;Quality=2;TypeStereo=2;	
			VBRCodingMethod=VBR_NEW;	VBRBitrate=320000;VBRQuality=1;	
			LowPassFreq=19500;	LowPassWidth=0; 
			HighPassFreq=-1;HighPassWidth=0;Preset=3;
			break;
		case 104: //Extereme
			m_lSampleRate=44100; m_lChannels=2; m_lBitrate=128000;Quality=2;TypeStereo=2;	
			VBRCodingMethod=VBR_OLD;	VBRBitrate=320000;VBRQuality=1;	
			LowPassFreq=19500;	LowPassWidth=0; 
			HighPassFreq=-1;HighPassWidth=0;Preset=3;
			break;
		case 105: //Insane
			m_lSampleRate=44100; m_lChannels=2; m_lBitrate=320000;Quality=2;TypeStereo=2;	
			VBRCodingMethod=VBR_OLD;	VBRBitrate=320000;VBRQuality=1;	
			LowPassFreq=20500;	LowPassWidth=0; 
			HighPassFreq=-1;HighPassWidth=0;Preset=4;
			break;
		case 0:
		case 100:
		default:
			m_lBitrate=128000;	VBRBitrate=128000;	m_lChannels=2;		Quality=5;
			TypeStereo=1;	VBRQuality=7;m_lSampleRate=44100;
			VBRCodingMethod=-1;	ABRBitrate=128000;	ErrorProtection=0;WritingVBRHeader=0;	BitReservoir=1;
			BlockTypeControl=0;	ShortBlocks=1;	StrictISOCompliance=0;
			StrictlyEnforceMinBitrate=0;	LowPassFreq=-1;	HighPassFreq=-1;
			LowPassWidth=0; HighPassWidth=0;	ATHControlMode=0; Emphasis=0;
			Preset=0;
			Preset=0;
			break;
		}
	}
private:

};


[ coclass, uuid("E12FBC48-E7B8-4C0F-ABCC-16198088A1AF"), threading(apartment), vi_progid("AVSMediaFormatSettings3.AVSMPEG3AudioFo"), progid("AVSMediaFormatSettings3.AVSMPEG3Audio.1"), version(1.0)]
class ATL_NO_VTABLE CAVSMPEG3AudioFormat : public CAVSMPEG3AudioFormatImpl<CAVSMPEG3AudioFormat, IAVSMPEG3AudioFormat>
{
};