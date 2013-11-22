// ASCAudioFormat.h : Declaration of the CASCAudioFormat

#pragma once
#include "resource.h"       // main symbols
#include "ATLDefine.h"
#include "ASCMediaFormat.h"       // main symbols
#include "AudioDefines.h"
#include "ASCUtils.h"

class CAVSAudioFormat;
// IAVSAudioFormat
[object, uuid("67879F80-A4C8-4907-84FB-C79BE09C7D87"), dual, pointer_default(unique)]
__interface IAVSAudioFormat : IAVSMediaFormat
{
	[id(101000 + 1),propget] HRESULT FormatTag([out, retval] long* pVal);
	[id(101000 + 1),propput] HRESULT FormatTag([in] long newVal);
	 
	[id(101000 + 2),propget] HRESULT SampleRate([out, retval] long* pVal);
	[id(101000 + 2),propput] HRESULT SampleRate([in] long newVal);
	[id(101000 + 3),propget] HRESULT Bitrate([out, retval] long* pVal);
	[id(101000 + 3),propput] HRESULT Bitrate([in] long newVal);
	[id(101000 + 4),propget] HRESULT Channels([out, retval] long* pVal);
	[id(101000 + 4),propput] HRESULT Channels([in] long newVal);
	[id(101000 + 5),propget] HRESULT SampleSize([out, retval] long* pVal);
	[id(101000 + 5),propput] HRESULT SampleSize([in] long newVal);
	 
	[id(101000 + 6),propget] HRESULT BlockAlign([out, retval] long* pVal);
	[id(101000 + 6),propput] HRESULT BlockAlign([in] long newVal);
	[id(101000 + 7),propget] HRESULT CompressRate([out, retval] double* pVal);
	[id(101000 + 8),propget] HRESULT ChannelsMask([out, retval] long* pVal);
	[id(101000 + 8),propput] HRESULT ChannelsMask([in] long newVal);
	 
	[id(101000 + 9),propget] HRESULT CodecName([out, retval] BSTR* pVal);
	
	[id(101000 + 11)] HRESULT GetFormatHeader([out] BYTE** Data, [out] LONG* Size);
	[id(101000 + 12)] HRESULT SetFormatHeader([in] BYTE* Data, [in] LONG Size);

	[id(101000 + 13), propget] HRESULT ExtraData([out, retval, satype("byte")] SAFEARRAY **pExtraData);
	[id(101000 + 13), propput] HRESULT ExtraData([in, satype("byte")] SAFEARRAY **pExtraData);
};

template <class CoT, class T>
class CAVSAudioFormatImpl : public CAVSMediaFormatImpl<CoT, T>
{
public:
	CAVSAudioFormatImpl()
		: m_pExtraData(NULL)
	{
		 m_lMediaType = MEDIAFORMAT_AUDIO;
		SetDefaultProperties();
	}
	virtual ~CAVSAudioFormatImpl()
	{
		RELEASEARRAY(m_pExtraData);
	}
	STDMETHOD(CreateDuplicate)(IAVSMediaFormat** pMediaFormat)
	{
		CAVSMediaFormatImpl<CoT, T>::CreateDuplicate(pMediaFormat);
		if ((*pMediaFormat)!=NULL)
		{
			IAVSAudioFormat * pAVSAudioFormat=NULL;
			(*pMediaFormat)->QueryInterface(__uuidof(IAVSAudioFormat), (void**)&pAVSAudioFormat);
			if (NULL==pAVSAudioFormat)
			{
				RELEASEINTERFACE((*pMediaFormat));
				return S_OK;
			}
			pAVSAudioFormat->put_FormatTag(m_lFormatTag);
			pAVSAudioFormat->put_SampleRate(m_lSampleRate);
			pAVSAudioFormat->put_Bitrate(m_lBitrate);
			pAVSAudioFormat->put_Channels(m_lChannels);
			pAVSAudioFormat->put_SampleSize(m_lSampleSize);

			pAVSAudioFormat->put_BlockAlign(m_lBlockAlign);
			pAVSAudioFormat->put_ChannelsMask(m_lChannelsMask);
			if (NULL==m_pExtraData)
				pAVSAudioFormat->put_ExtraData(NULL);
			else
				pAVSAudioFormat->put_ExtraData(&m_pExtraData);

			pAVSAudioFormat->Release();
		}
		return S_OK;
	}
	STDMETHOD(SetDefaultProperties)(void)
	{
		CAVSMediaFormatImpl<CoT, T>::SetDefaultProperties();

		m_lFormatTag = WAVE_FORMAT_PCM;
		m_lSampleRate = 44100;
		m_lChannelsMask = 0x03;
		m_lChannels = 2;		
		m_lSampleSize = 16;

		SetDefaultPropertiesByFormat();

		return S_OK;
	}

	HRESULT SetDefaultPropertiesByFormat(void)
	{
		RELEASEARRAY(m_pExtraData);
/*
		m_lSampleRate = 44100;
		m_lChannelsMask = 0x03;
		m_lChannels = 2;		
		m_lSampleSize = 16;
*/
		switch (m_lFormatTag)
		{
			case WAVE_FORMAT_IEEE_FLOAT:
			case WAVE_FORMAT_EXTENSIBLE:
			case WAVE_FORMAT_PCM:	
			case WAVE_FORMAT_LPCM:	
			case WAVE_APPLE_LOSSLESS:
			case WAVE_FORMAT_FLAC:
				m_lBlockAlign = m_lChannels*m_lSampleSize/8;
				m_lBitrate = m_lSampleRate*m_lBlockAlign*8;	
				break;
			case WAVE_FORMAT_MPEGLAYER3:
			case WAVE_FORMAT_MPEG:
			case WAVE_FORMAT_MPC: 
			case WAVE_FORMAT_AAC:
			case WAVE_FORMAT_OGG:
				m_lBitrate = 128000;	
				m_lBlockAlign = 1;
				break;	
			case WAVE_FORMAT_DOLBY_AC3_SPDIF: 
			case WAVE_FORMAT_DVM: //Ac3
			case WAVE_FORMAT_DTS: 
			case WAVE_FORMAT_DTS_DS:
				m_lBitrate = 384000;	
				m_lBlockAlign = 1;
				break;	
			case WAVE_FORMAT_DIALOGIC_OKI_ADPCM: //VOX
			case WAVE_FORMAT_G721_ADPCM: //G721
			case WAVE_FORMAT_G723_ADPCM: //G723
			case WAVE_FORMAT_G726_ADPCM: //G726
			case WAVE_FORMAT_SPG726:	//G726:
			case WAVE_FORMAT_MSG723:	//G723.1
			case WAVE_FORMAT_G729:
			case WAVE_FORMAT_G729A:
			case WAVE_FORMAT_QUALCOMM_PUREVOICE:
				m_lSampleRate = 8000;	
				m_lBitrate = 32000;	
				m_lSampleSize = 4;
				m_lBlockAlign = 1;
				m_lChannels = 1;
				break;
			case WAVE_FORMAT_AMR_AMV:
				m_lSampleRate = 8000;	
				m_lBitrate = 12200;	
				m_lSampleSize = 4;
				m_lBlockAlign = 1;
				m_lChannels = 1;
				break;
			case WAVE_FORMAT_ADPCM: 
			case WAVE_FORMAT_IMA_ADPCM: 
			case WAVE_FORMAT_IMA_ADPCM_QT: 
			case WAVE_FORMAT_IMA_ADPCM_AMV:
			case WAVE_FORMAT_IMA_ADPCM_FLV:
				m_lSampleSize = 4;
				m_lBlockAlign = 2;
				m_lBitrate = m_lSampleRate*m_lChannels*4;
				break;			
			case WAVE_FORMAT_ALAW:
			case WAVE_FORMAT_MULAW:
				m_lSampleSize = 4;
				m_lBlockAlign = 2;
				m_lBitrate = m_lSampleRate*m_lChannels*8;
				break;
			case WAVE_FORMAT_GSM610:
				m_lChannels=1;
				m_lSampleSize = 4;
				m_lBlockAlign = 1;
                m_lBitrate = static_cast<long>(1.625 * m_lSampleRate * m_lChannels );
				break;
			case WAVE_APPLE_ANY:
				break;
		}
		return S_OK;
	}
	STDMETHOD(SetPropertiesByFormat)(IAVSMediaFormat* pMediaFormat)
	{
		CAVSMediaFormatImpl<CoT, T>::SetPropertiesByFormat(pMediaFormat);
		if (pMediaFormat!=NULL)
		{
			IAVSAudioFormat*	AVSAudioFormat=NULL;
			pMediaFormat->QueryInterface(__uuidof(IAVSAudioFormat), (void**)&AVSAudioFormat);
			if (AVSAudioFormat!=NULL)
			{
				AVSAudioFormat->get_FormatTag(&m_lFormatTag);
				AVSAudioFormat->get_SampleRate(&m_lSampleRate);
				AVSAudioFormat->get_Bitrate(&m_lBitrate);
				AVSAudioFormat->get_Channels(&m_lChannels);
				AVSAudioFormat->get_SampleSize(&m_lSampleSize);

				AVSAudioFormat->get_BlockAlign(&m_lBlockAlign);
				AVSAudioFormat->get_ChannelsMask(&m_lChannelsMask);

				RELEASEARRAY(m_pExtraData);
				AVSAudioFormat->get_ExtraData(&m_pExtraData);

				RELEASEINTERFACE(AVSAudioFormat);
			}
		}
		return S_OK;
	}
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		//Здесь проверяем параметры относящиеся к AudioFormat
		return CAVSMediaFormatImpl<CoT, T>::SetAdditionalParam(ParamName, ParamValue);
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		//Здесь проверяем параметры относящиеся к AudioFormat
		return CAVSMediaFormatImpl<CoT, T>::GetAdditionalParam(ParamName, ParamValue);
	}

	STDMETHOD(get_FormatTag)(long* pVal)
	{
		*pVal = m_lFormatTag;
		return S_OK;
	}
	STDMETHOD(put_FormatTag)(long newVal)
	{
		m_lFormatTag = newVal;		
		return SetDefaultPropertiesByFormat();
	}
	STDMETHOD(get_SampleRate)(long* pVal)
	{
		*pVal = m_lSampleRate;
		return S_OK;
	}
	STDMETHOD(put_SampleRate)(long newVal)
	{
		m_lSampleRate = newVal;
		GetDefaultBitrate();
		return S_OK;
	}
	STDMETHOD(get_Bitrate)(long* pVal)
	{
		*pVal = m_lBitrate;
		return S_OK;
	}
	STDMETHOD(put_Bitrate)(long newVal)
	{
		switch (m_lFormatTag)
		{
			case WAVE_FORMAT_IEEE_FLOAT:
			case WAVE_FORMAT_EXTENSIBLE:
			case WAVE_FORMAT_PCM:	
			case 0x674d://LPCM			
			case WAVE_FORMAT_DIALOGIC_OKI_ADPCM: //VOX
			case WAVE_FORMAT_ADPCM: //Ms Adpcm
			case WAVE_FORMAT_IMA_ADPCM: //Ima Adpcm MS
			case WAVE_FORMAT_IMA_ADPCM_QT: //Ima Adpcm QT
			case WAVE_FORMAT_ALAW: //A-law
			case WAVE_FORMAT_MULAW: //u-law
			case WAVE_FORMAT_GSM610: //Gsm
			case WAVE_FORMAT_FLAC:
				break;
			default:
				m_lBitrate = newVal;
		}
		
		return S_OK;
	}
	STDMETHOD(get_Channels)(long* pVal)
	{
		*pVal = m_lChannels;
		return S_OK;
	}
	STDMETHOD(put_Channels)(long newVal)
	{
		m_lChannels = newVal;
		GetDefaultBitrate();
		return S_OK;
	}
	STDMETHOD(get_SampleSize)(long* pVal)
	{
		*pVal = m_lSampleSize;
		return S_OK;
	}
	STDMETHOD(put_SampleSize)(long newVal)
	{
		m_lSampleSize =  newVal;
		GetDefaultBitrate();
		return S_OK;
	}
	
	STDMETHOD(get_BlockAlign)(long* pVal)
	{
		*pVal = m_lBlockAlign;
		return S_OK;
	}
	STDMETHOD(put_BlockAlign)(long newVal)
	{
		m_lBlockAlign =  newVal;
		GetDefaultBitrate();
		return S_OK;
	}
	STDMETHOD(get_CompressRate)(double* pVal)
	{
		*pVal = 1; 
		if( abs( m_lBitrate ) > 0  )
			*pVal=((double)m_lSampleRate*m_lChannels*(m_lSampleSize))/(double)m_lBitrate;
		return S_OK;
	}
	STDMETHOD(get_ChannelsMask)(long* pVal)
	{
		*pVal = m_lChannelsMask;
		return S_OK;
	}
	STDMETHOD(put_ChannelsMask)(long newVal)
	{
		m_lChannelsMask = newVal;
		return S_OK;
	}
		
	STDMETHOD(get_CodecName)(BSTR* pVal)
	{
		CString sCodecName = _T("");
		switch (m_lFormatTag)
		{
			case WAVE_FORMAT_IEEE_FLOAT:
			case WAVE_FORMAT_EXTENSIBLE:
			case WAVE_FORMAT_PCM:	
				sCodecName = _T("PCM");
				break;
			case WAVE_FORMAT_LPCM:
				sCodecName = _T("LPCM");
				break;
            case WAVE_FORMAT_BPCM:
                sCodecName = _T("BPCM");
                break;
//////////ПРАВИЛЬНЫЙ вариант .. не для "простого" пользователя
			//case WAVE_FORMAT_MPEGLAYER3://MP3
			//	if (m_lSampleRate<32000)
			//		sCodecName="MPEG 2.0 layer-3";				
			//	else 
			//		sCodecName="MPEG 2.5 layer-3";				
			//	break;
			//case WAVE_FORMAT_MPEG://MP2
			//	sCodecName="MPEG layer-2";
			//	break;
//////////////////////////////////////////////////////////////
///////для "тупых" пользователей
			case WAVE_FORMAT_MPEGLAYER3://MP3
				sCodecName = _T("MP3");				
				break;
			case WAVE_FORMAT_MPEG://MP2
				sCodecName = _T("MP2");
				break;
/////////////////////////////////////////////////////////////
			case WAVE_FORMAT_DIALOGIC_OKI_ADPCM: //VOX
				sCodecName = _T("Dialogic ADPCM (vox)");
				break;
			case WAVE_FORMAT_G721_ADPCM: //G721
				sCodecName = _T("CCITT G.721 ADPCM");
				break;
			case WAVE_FORMAT_G723_ADPCM: //G723
				sCodecName = _T("CCITT G.723 ADPCM");
				break;
			case WAVE_FORMAT_G726_ADPCM: //G726
			case 0x0045://WAVE_FORMAT_SPG726:
				sCodecName = _T("CCITT G.726 ADPCM");
				break;
			case WAVE_FORMAT_MSG723: //G723.1
				sCodecName = _T("Microsoft G.723.1 ADPCM");
				break;
			case 0x674f: //OGG
				sCodecName = _T("Ogg Vorbis");
				break;
			case 0x777: //G729
			case WAVE_FORMAT_G729A:
				sCodecName = _T("CCITT G.729 ADPCM");
				break;
			case 0x888: //Mp+
				sCodecName = _T("MP Plus SV-8");
				break;
			case WAVE_FORMAT_ADPCM: //Ms Adpcm
				sCodecName = _T("Microsoft ADPCM");
				break;
			case WAVE_FORMAT_IMA_ADPCM: //ms ima
			case WAVE_FORMAT_IMA_ADPCM_QT: //apple ima
			case WAVE_FORMAT_IMA_ADPCM_AMV: //amv ima
			case WAVE_FORMAT_IMA_ADPCM_FLV: // flv ima
				sCodecName = _T("IMA ADPCM");
				break;
			case WAVE_FORMAT_ALAW: //A-law
				sCodecName = _T("CCITT A-law");
				break;
			case WAVE_FORMAT_MULAW: //u-law
				sCodecName = _T("CCITT u-law");
				break;
			case WAVE_FORMAT_GSM610: //Gsm
				sCodecName = _T("MS GSM 6.10");
				break;
			case WAVE_FORMAT_DOLBY_AC3_SPDIF: //Ac3
			case WAVE_FORMAT_DVM: //0x2000 //Ac3
				sCodecName = _T("AC3");
				break;
			case WAVE_FORMAT_DTS: //DTS
			case 0x0190://WAVE_FORMAT_DTS_DS:
			case 0x2001:
				sCodecName = _T("DTS");
				break;
			case 0xA106:
				sCodecName = _T("MPEG-2/4 Audio");
				break;
			case 0xA107:
				sCodecName = _T("MPEG-2/4 Audio LATM");
				break;
			case WAVE_FORMAT_MLP:
				sCodecName = _T("Meridian Lossless");
				break;
			case WAVE_FORMAT_TRUEHD:
				sCodecName = _T("TrueHD");
				break;
			case WAVE_FORMAT_AC3COREHD:
				sCodecName = _T("AC3Core/TrueHD");
				break;
			case 0x0136:
				if (m_lSampleRate==16000)
					sCodecName = _T("AWB ACELP"); //WideBand 
				else
					sCodecName = _T("AMR ACELP"); //NarrowBand
				break;
			case WAVE_FORMAT_QUALCOMM_PUREVOICE:
				sCodecName = _T("Qualcomm PureVoice");
				break;
			case 4001: //QT
				sCodecName = _T("QT"); //сторонний кодек Apple
				//SubTypes
				if ((NULL!=m_pExtraData) && (4 < m_pExtraData->rgsabound[0].cElements))
				{
					DWORD* pdwData = (DWORD*)m_pExtraData->pvData;
					DWORD SubType = pdwData[1];
					switch (SubType)
					{
						case mmioFOURCC('2','M','D','Q'):
							sCodecName = _T("QDesign Music 2");
							break;
					}
				}
				break;
			case 0x0160://WAVE_FORMAT_MSAUDIO1: 
				sCodecName = _T("WMA ver.1");
				break;
			case 0x0161://WAVE_FORMAT_MSAUDIO2: 
				sCodecName = _T("WMA");
				break;
			case 0x0162://WAVE_FORMAT_MSAUDIO3: 
				sCodecName = _T("WMA Pro");
				break;
			case 0x0163://WAVE_FORMAT_MSAUDIO4: 
				sCodecName = _T("WMA Lossless");
				break;
			case 0x0164://WAVE_FORMAT_MSAUDIO4: 
				sCodecName = _T("WMA SPDIF");
				break;
			case 0x0130://WAVE_FORMAT_SIPROLAB_ACELPNET:
				sCodecName = _T("Acelp.NET");
				break;
			case 0x00A://WAVE_FORMAT_WMSP1:
				sCodecName = _T("WM Speech");
				break;
			case WAVE_FORMAT_QDESIGN_MUSIC:
				sCodecName = _T("QDesign Music");
				break;
			case WAVE_FORMAT_MSNAUDIO:
				sCodecName = _T("MSN Audio");
				break;
			case '41'://'1', '4', '_', '4'
				sCodecName = _T("Win32 RealAudio 1.0");
				break;
			case '82'://'2', '8', '_', '8'
				sCodecName = _T("Win32 RealAudio 2.0");
				break;
			case 'oc'://'c', 'o', 'o', 'k'
				sCodecName = _T("Win32 RealAudio COOK");
				break;
			case 'is'://'s', 'i', 'p', 'r'
				sCodecName = _T("Win32 RealAudio Sipro");
				break;
			case 'ta'://'a', 't', 'r', 'c'
				sCodecName = _T("Win32 RealAudio ATRAC3");
				break;
			case 'nd'://'d', 'n', 'e', 't'
				sCodecName = _T("Win32 RealAudio AC3");
				break;
			case 'ar':
				sCodecName = _T("Win32 RealAudio RALF");		//'r', 'a', 'l', 'f'
				sCodecName = _T("Win32 RealAudio AAC");		//'r', 'a', 'a', 'c'
				break;
			case WAVE_FORMAT_FLAC:
				sCodecName = _T("FLAC");
				break;
			case WAVE_APPLE_LOSSLESS:
				sCodecName = _T("Apple Lossless");
				break;
			case WAVE_FORMAT_VOXWARE_BYTE_ALIGNED:
			case WAVE_FORMAT_VOXWARE_AC8: 
			case WAVE_FORMAT_VOXWARE_AC10: 
			case WAVE_FORMAT_VOXWARE_AC16: 
			case WAVE_FORMAT_VOXWARE_AC20: 
			case WAVE_FORMAT_VOXWARE_RT24: 
			case WAVE_FORMAT_VOXWARE_RT29: 
			case WAVE_FORMAT_VOXWARE_RT29HW: 
			case WAVE_FORMAT_VOXWARE_VR12: 
			case WAVE_FORMAT_VOXWARE_VR18: 
			case WAVE_FORMAT_VOXWARE_TQ40: 
			case 0x007A://WAVE_FORMAT_VOXWARE_SC3: 
			case 0x007B://WAVE_FORMAT_VOXWARE_SC3: 
				sCodecName = _T("Voxware Audio Codec");
				break;
			default:
				sCodecName = _T("Unknown name");
				break;
		}
		*pVal = sCodecName.AllocSysString();
		return S_OK;
	}

	STDMETHOD(GetFormatHeader)(BYTE** Data, LONG* Size)
	{
		if (NULL==Data)
			return S_FALSE;

		*Data = NULL;
		*Size = 0;

		if (0!=m_lChannelsMask)	
			SetChannelsMaskPreset(m_lChannels, (DWORD&)m_lChannelsMask);
		else 
			CheckChannelsMask(m_lChannels,(DWORD&)m_lChannelsMask);

		HRESULT hr;
		switch (m_lFormatTag)
		{
			case WAVE_FORMAT_IEEE_FLOAT:
			case WAVE_FORMAT_EXTENSIBLE:
			case WAVE_FORMAT_PCM:	
				hr = FillFormatPCM(Data, *Size);
				break;
			case WAVE_FORMAT_LPCM://LPCM			
				hr = FillFormatLPCM(Data, *Size);
				break;
			case WAVE_FORMAT_MPEGLAYER3://MP3
				hr = FillFormatMP3(Data, *Size);
				break;;				
			case WAVE_FORMAT_MPEG://MP2
				hr = FillFormatMP2(Data, *Size);
				break;
			case WAVE_FORMAT_DIALOGIC_OKI_ADPCM: //VOX
				hr = FillFormatVox(Data, *Size);
				break;
			case WAVE_FORMAT_G721_ADPCM: //G721
				hr = FillFormatG721(Data, *Size);
				break;
			case WAVE_FORMAT_G723_ADPCM: //G723
				hr = FillFormatG723(Data, *Size);
				break;
			case WAVE_FORMAT_MSG723: //G723.1
				hr = FillFormatMSG723(Data, *Size);
				break;
			case WAVE_FORMAT_G726_ADPCM: //G726
			case WAVE_FORMAT_SPG726:
				hr = FillFormatG726(Data, *Size);
				break;
			case WAVE_FORMAT_G729: //G729
			case WAVE_FORMAT_G729A:
				hr = FillFormatG729(Data, *Size);
				break;
			case WAVE_FORMAT_OGG: //OGG
				hr = FillFormatOggVorbis(Data, *Size);
				break;
			case WAVE_FORMAT_MPC: //Mp+
				hr = FillFormatMPC(Data, *Size);
				break;
			case WAVE_FORMAT_ADPCM: //Ms ADPCM
				hr = FillFormatMsADPCM(Data, *Size);
				break;
			case WAVE_FORMAT_IMA_ADPCM: //Ima Adpcm MS
			case WAVE_FORMAT_IMA_ADPCM_QT: //Ima Adpcm QT
				hr = FillFormatImaADPCM(Data, *Size);
				break;
			case WAVE_FORMAT_ALAW: //A-law
			case WAVE_FORMAT_MULAW: //u-law
				hr = FillFormatLaw(Data, *Size);
				break;
			case WAVE_FORMAT_GSM610: //Gsm
				hr = FillFormatGsm(Data, *Size);
				break;
			case WAVE_FORMAT_DOLBY_AC3_SPDIF: //Ac3
			case WAVE_FORMAT_DVM: //Ac3
			case WAVE_FORMAT_DTS: //DTS
			case WAVE_FORMAT_DTS_DS://0x0190://WAVE_FORMAT_DTS_DS:
				hr = FillFormatAC3(Data, *Size);
				break;
			case WAVE_FORMAT_AAC:
				hr = FillFormatAAC(Data, *Size);
				break;
			case WAVE_FORMAT_AMR_AMV:
				hr = FillFormatAMR(Data, *Size);
				break;
			case WAVE_FORMAT_FLAC://Flac
				hr = FillFormatFLAC(Data, *Size);
				break;
			case WAVE_APPLE_LOSSLESS:
				hr = FillFormatAppleLossless(Data, *Size);
				break;
			case WAVE_FORMAT_AC3COREHD:
			case WAVE_FORMAT_TRUEHD:
			case WAVE_FORMAT_MLP:
				hr = FillFormatAC3(Data, *Size, true);
				break;
/*
			case WAVE_FORMAT_QUALCOMM_PUREVOICE:
				//FillFormatQCELP(format);
				break;
			case WAVE_FORMAT_MSAUDIO1://0x0160
			case WAVE_FORMAT_MSAUDIO1 + 1://0x0161
			case WAVE_FORMAT_MSAUDIO1 + 2://0x0162
			case WAVE_FORMAT_MSAUDIO1 + 3://0x0163
			case WAVE_FORMAT_MSAUDIO1 + 4://0x0164
			case 0x0130://WAVE_FORMAT_SIPROLAB_ACELPNET
			case 0x000A://WAVE_FORMAT_WMSP1
				//FillFormatWMA(format);
				break;
			case WAVE_FORMAT_QDESIGN_MUSIC:
				//FillFormatQDesign(format);
				break;
			case WAVE_FORMAT_MSNAUDIO:
				//FillFormatVox(format);
				break;
*/
			default:
				hr = FillFormatUnknown(Data, *Size);
				ATLASSERT(FALSE);
				break;
		}
		_ASSERTE(SUCCEEDED(hr));
		return hr;
	}
	STDMETHOD(SetFormatHeader)(BYTE* Data, LONG Size)
	{
		WAVEFORMATEX* Format= (WAVEFORMATEX*)Data;
	
		m_lFormatTag = Format->wFormatTag;
		SetDefaultPropertiesByFormat();

		m_lChannels = Format->nChannels;
		m_lSampleRate = Format->nSamplesPerSec;
		m_lSampleSize = Format->wBitsPerSample;
		m_lBitrate = Format->nAvgBytesPerSec*8;

		if (m_lSampleSize < 8)
			m_lSampleSize = 16;

		switch(m_lFormatTag)
		{
		case WAVE_FORMAT_EXTENSIBLE:
			{
				if (22 == Format->cbSize)
				{
					WAVEFORMATEXTENSIBLE *pFormatExt = (WAVEFORMATEXTENSIBLE*)Format;
					m_lChannelsMask = pFormatExt->dwChannelMask;
				}
			}
			break;
		case WAVE_FORMAT_AAC:
			{
				if (8 < Format->cbSize)
				{
					WAVEFORMATEX_AAC *pFormatAAC = (WAVEFORMATEX_AAC*)Format;
					m_lChannelsMask = pFormatAAC->dwChannelMask;
			
					RELEASEARRAY(m_pExtraData);
					m_pExtraData = SafeArrayCreateVector(VT_UI1, pFormatAAC->wfx.cbSize - 8);
					memcpy((char*)m_pExtraData->pvData, pFormatAAC->arnExtraData, pFormatAAC->wfx.cbSize - 8);
				}
			}
			break;
		case WAVE_FORMAT_DTS:
		case WAVE_FORMAT_DOLBY_AC3_SPDIF:
			{
				if (4 == Format->cbSize)
				{
					WAVEFORMATEX_AC3 *pFormatAC3 = (WAVEFORMATEX_AC3 *)Format;
					m_lChannelsMask = pFormatAC3->dwChannelMask;
				}
			}
			break;
		case WAVE_FORMAT_OGG:
			if (12 == Format->cbSize)
			{
				OGGWAVEFORMAT *pFormatOGG = (OGGWAVEFORMAT *)Format;
				m_lChannelsMask = pFormatOGG->dwChannelsMask;
			}
			break;
		case WAVE_FORMAT_FLAC:
			if (4 == Format->cbSize)
			{
				WAVEFORMAT_FLAC *pFormatFLAC = (WAVEFORMAT_FLAC *)Format;
				m_lChannelsMask = pFormatFLAC->dwChannelsMask;
			}
			break;
		case WAVE_APPLE_LOSSLESS:
			{
				WAVEFORMATEX_APPLE_LOSSLESS *pFormatALAC = (WAVEFORMATEX_APPLE_LOSSLESS *)Format;
				if (4 >= Format->cbSize)
				{
					m_lChannelsMask =pFormatALAC->dwChannelsMask;

					if (Format->cbSize >4)
					{
						RELEASEARRAY(m_pExtraData);
						m_pExtraData = SafeArrayCreateVector(VT_UI1, Format->cbSize - 4);
						memcpy((char*)m_pExtraData->pvData, pFormatALAC->arnExtraData, Format->cbSize - 4);
					}
				}
			}
			break;
		default:
			CheckChannelsMask(m_lChannels,(DWORD&)m_lChannelsMask);
			break;
		}		
		return S_OK;
	}

	STDMETHOD(get_ExtraData)(SAFEARRAY **pExtraData)
	{
		if (NULL==pExtraData)
			return S_OK;

		SafeArrayCopy(m_pExtraData, pExtraData);
		return S_OK;
	}
	STDMETHOD(put_ExtraData)(SAFEARRAY **pExtraData)
	{
		if (NULL==pExtraData)
			return S_OK;

		RELEASEARRAY(m_pExtraData);
		if (NULL!=(*pExtraData))
			SafeArrayCopy(*pExtraData, &m_pExtraData);
		return S_OK;
	}
protected:
	long m_lFormatTag;
	long m_lSampleRate;
	
	long m_lChannels;
	long m_lSampleSize;

	long m_lBlockAlign;
	long m_lBitrate;
	long m_lChannelsMask;
	LPSAFEARRAY m_pExtraData;

private:
	void GetDefaultBitrate()
	{
		switch (m_lFormatTag)
		{
			case WAVE_FORMAT_IEEE_FLOAT:
			case WAVE_FORMAT_EXTENSIBLE:
			case WAVE_FORMAT_PCM:	
			case 0x674d://LPCM	
			case WAVE_FORMAT_FLAC:
				m_lBitrate = m_lSampleRate*m_lChannels*m_lSampleSize/8*8;
				break;
			case WAVE_FORMAT_DIALOGIC_OKI_ADPCM: //VOX
			case WAVE_FORMAT_ADPCM: //Ms Adpcm
			case WAVE_FORMAT_IMA_ADPCM: //Ima Adpcm MS
			case WAVE_FORMAT_IMA_ADPCM_QT: //Ima Adpcm QT
				m_lBitrate = m_lSampleRate*m_lChannels*4;
				break;
			case WAVE_FORMAT_ALAW: //A-law
			case WAVE_FORMAT_MULAW: //u-law
				m_lBitrate = m_lSampleRate*m_lChannels*8;
				break;
			case WAVE_FORMAT_GSM610: //Gsm
				m_lBitrate = static_cast<long>(1.625 * m_lSampleRate * m_lChannels );
				break;
		}
	}


	
	static void AutoFillFormatSetting(WAVEFORMATEX &oFormat)
	{
		oFormat.nBlockAlign		= (WORD)(oFormat.nChannels * oFormat.wBitsPerSample / 8);
		oFormat.nAvgBytesPerSec	= oFormat.nBlockAlign * oFormat.nSamplesPerSec;
	}
	//
	
	HRESULT FillFormatUnknown(BYTE **ppData, long &lSize)
	{
		lSize = sizeof(WAVEFORMATEX);
		if (NULL!=m_pExtraData)
			lSize += m_pExtraData->rgsabound[0].cElements;

		*ppData = (BYTE*)HeapAlloc(GetProcessHeap(), 0, lSize);
		if (NULL == *ppData)
		{
			lSize = 0;
			return E_OUTOFMEMORY;
		}
		WAVEFORMATEX* pFormat = (WAVEFORMATEX*)(*ppData);

		pFormat->cbSize				= lSize - sizeof(WAVEFORMATEX);
		pFormat->wFormatTag			= (WORD)m_lFormatTag;
		pFormat->nSamplesPerSec		= m_lSampleRate;
		pFormat->wBitsPerSample		= (WORD)m_lSampleSize;
		pFormat->nChannels			= (WORD)m_lChannels;
		
		AutoFillFormatSetting(*pFormat);
						
		if ((NULL!=m_pExtraData) && (0 < m_pExtraData->rgsabound[0].cElements))
			memcpy((*ppData) + sizeof(WAVEFORMATEX), m_pExtraData->pvData, m_pExtraData->rgsabound[0].cElements);	

		return S_OK;
	}
	HRESULT FillFormatPCM(BYTE **ppData, long &lSize)
	{
		if ((2>=m_lChannels) && (3>=m_lChannelsMask))
		{
			lSize = sizeof(WAVEFORMATEX);
			*ppData = (BYTE*)HeapAlloc(GetProcessHeap(), 0, lSize);
			if (NULL == (*ppData))
			{
				lSize = 0;
				return E_OUTOFMEMORY;
			}
			WAVEFORMATEX *pFormat = (WAVEFORMATEX *)(*ppData);
			
			pFormat->cbSize				= 0;
			pFormat->wFormatTag			= WAVE_FORMAT_PCM;
			pFormat->nSamplesPerSec		= m_lSampleRate;
			pFormat->wBitsPerSample		= (WORD)m_lSampleSize;
			pFormat->nChannels			= (WORD)m_lChannels;
			
			AutoFillFormatSetting(*pFormat);
		}
		else
		{
			lSize = sizeof(WAVEFORMATEXTENSIBLE);			
			*ppData = (BYTE*)HeapAlloc(GetProcessHeap(), 0, lSize);
			if (NULL == (*ppData))
			{
				lSize = 0;
				return E_OUTOFMEMORY;
			}
			WAVEFORMATEXTENSIBLE *pFormatEx = (WAVEFORMATEXTENSIBLE *)(*ppData);
			
			WAVEFORMATEX &oFormat	= pFormatEx->Format;

			oFormat.cbSize			= (WORD)(sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX));
			oFormat.wFormatTag		= WAVE_FORMAT_EXTENSIBLE;
			oFormat.nSamplesPerSec	= m_lSampleRate;
			oFormat.wBitsPerSample	= (WORD)m_lSampleSize;
			oFormat.nChannels		= (WORD)m_lChannels;

			AutoFillFormatSetting(oFormat);

			pFormatEx->dwChannelMask				= m_lChannelsMask;
			pFormatEx->SubFormat					= KSDATAFORMAT_SUBTYPE_PCM;
			pFormatEx->Samples.wValidBitsPerSample	= 0;

		}
		return S_OK;
	}

	HRESULT FillFormatLPCM(BYTE **ppData, long &lSize)
	{
		lSize = sizeof(WAVEFORMATEX_LPCM);
		*ppData = (BYTE*)HeapAlloc(GetProcessHeap(), 0, lSize);
		if (NULL == (*ppData))
		{
			lSize = 0;
			return E_OUTOFMEMORY;
		}
		WAVEFORMATEX_LPCM *pFormatEx = (WAVEFORMATEX_LPCM *)(*ppData);

		WAVEFORMATEX &oFormat	= pFormatEx->format;

		oFormat.cbSize			= (WORD)(sizeof(WAVEFORMATEX_LPCM) - sizeof(WAVEFORMATEX));
		oFormat.wFormatTag		= (WORD)m_lFormatTag;
		oFormat.nChannels		= (WORD)m_lChannels;
		oFormat.nSamplesPerSec	= m_lSampleRate;
		oFormat.wBitsPerSample	= (WORD)m_lSampleSize;

		AutoFillFormatSetting(oFormat);
		
		
		pFormatEx->ChannelsMask1 = 0;//ChannelsMask1;
		pFormatEx->ChannelsMask2 = 0;//ChannelsMask2;
		pFormatEx->Group2Enabled = 0;//Group2Enabled;
		pFormatEx->OffsetGroup   = 0;
		return S_OK;
	}

	HRESULT FillFormatMP3(BYTE **ppData, long &lSize) 
	{
		lSize = sizeof(MPEGLAYER3WAVEFORMAT);
		*ppData = (BYTE*)HeapAlloc(GetProcessHeap(), 0, lSize);
		if (NULL == (*ppData))
		{
			lSize = 0;
			return E_OUTOFMEMORY;
		}
		MPEGLAYER3WAVEFORMAT *pFormatEx = (MPEGLAYER3WAVEFORMAT *)(*ppData);

		WAVEFORMATEX &oFormat	= pFormatEx->wfx;
		oFormat.cbSize			= MPEGLAYER3_WFX_EXTRA_BYTES;//TEST		
		oFormat.wFormatTag		= WAVE_FORMAT_MPEGLAYER3;
		oFormat.wBitsPerSample	= 0;
		oFormat.nBlockAlign		= 1;
		oFormat.nChannels		= (WORD)m_lChannels;
		if (2 < oFormat.nChannels)
			oFormat.nChannels = 2;
		oFormat.nSamplesPerSec	= m_lSampleRate;
		oFormat.nAvgBytesPerSec	= m_lBitrate/8;
		
		pFormatEx->fdwFlags		= 34;//MPEGLAYER3_FLAG_PADDING_OFF;
		if (0 != m_lSampleRate)
			pFormatEx->nBlockSize	= (short)(144 * m_lBitrate/m_lSampleRate);
		else
			return E_FAIL;

		if (m_lSampleRate < 32000)
			pFormatEx->nBlockSize /= 2;
		pFormatEx->nCodecDelay		= 0;//0x0571;
		pFormatEx->nFramesPerBlock	= 1;
		pFormatEx->wID				= MPEGLAYER3_ID_MPEG;
		return S_OK;
	}

	HRESULT FillFormatMP2(BYTE **ppData, long &lSize)
	{
		lSize = sizeof(MPEG1WAVEFORMAT);
		*ppData = (BYTE*)HeapAlloc(GetProcessHeap(), 0, lSize);
		if (NULL == (*ppData))
		{
			lSize = 0;
			return E_OUTOFMEMORY;
		}
		MPEG1WAVEFORMAT* pFormatEx = (MPEG1WAVEFORMAT*)(*ppData);

		WAVEFORMATEX &oFormat	= pFormatEx->wfx;
		oFormat.cbSize			= (WORD)(sizeof(MPEG1WAVEFORMAT) - sizeof(WAVEFORMATEX));//22?
		oFormat.wFormatTag = WAVE_FORMAT_MPEG;
		oFormat.wBitsPerSample	= 0;
		oFormat.nBlockAlign		= 1;
		oFormat.nChannels = (WORD)m_lChannels;
		if (2 < oFormat.nChannels)
			oFormat.nChannels = 2;
		oFormat.nSamplesPerSec	= m_lSampleRate;
		oFormat.nAvgBytesPerSec	= m_lBitrate/8;
		
		pFormatEx->dwHeadBitrate = m_lBitrate;
		pFormatEx->dwPTSHigh = 0;
		pFormatEx->dwPTSLow = 0;
		pFormatEx->fwHeadFlags = 0;	
		if (2 == oFormat.nChannels)
			pFormatEx->fwHeadLayer = ACM_MPEG_LAYER2;
		else 
			pFormatEx->fwHeadLayer = ACM_MPEG_SINGLECHANNEL;
		pFormatEx->fwHeadMode = ACM_MPEG_STEREO;
		pFormatEx->fwHeadModeExt = 1;
		pFormatEx->wHeadEmphasis = 1;
		return S_OK;
	}
	HRESULT FillFormatVox(BYTE **ppData, long &lSize)
	{
		lSize = sizeof(WAVEFORMATEX);
		*ppData = (BYTE*)HeapAlloc(GetProcessHeap(), 0, lSize);
		if (NULL == (*ppData))
		{
			lSize = 0;
			return E_OUTOFMEMORY;
		}
		WAVEFORMATEX *pFormat = (WAVEFORMATEX *)(*ppData);
		
		pFormat->cbSize				= 0;
		pFormat->wFormatTag			= WAVE_FORMAT_DIALOGIC_OKI_ADPCM;
		pFormat->nChannels			= 1;	
		pFormat->wBitsPerSample		= 4;
		pFormat->nSamplesPerSec		= m_lSampleRate;
		pFormat->nBlockAlign		= 4;
		pFormat->nAvgBytesPerSec	= pFormat->nBlockAlign * pFormat->nSamplesPerSec;
		return S_OK;
	}
	HRESULT FillFormatG721(BYTE **ppData, long &lSize)
	{
		lSize = sizeof(G721_ADPCMWAVEFORMAT);
		*ppData = (BYTE*)HeapAlloc(GetProcessHeap(), 0, lSize);
		if (NULL == (*ppData))
		{
			lSize = 0;
			return E_OUTOFMEMORY;
		}
		G721_ADPCMWAVEFORMAT* pFormatEx = (G721_ADPCMWAVEFORMAT*)(*ppData);

		WAVEFORMATEX &oFormat	= pFormatEx->wfx;
		oFormat.cbSize			= (WORD)(sizeof(G721_ADPCMWAVEFORMAT) - sizeof(WAVEFORMATEX));//2?
		oFormat.wFormatTag		= WAVE_FORMAT_G721_ADPCM;
		oFormat.nChannels		= 1;	
		oFormat.wBitsPerSample	= 4;
		oFormat.nSamplesPerSec	= m_lSampleRate;
		oFormat.nBlockAlign		= 4;
		oFormat.nAvgBytesPerSec = oFormat.nBlockAlign * oFormat.nSamplesPerSec;

		pFormatEx->nAuxBlockSize = 4;
		return S_OK;
	}

	HRESULT FillFormatG723(BYTE **ppData, long &lSize)
	{
		lSize = sizeof(G723_ADPCMWAVEFORMAT);
		*ppData = (BYTE*)HeapAlloc(GetProcessHeap(), 0, lSize);
		if (NULL == (*ppData))
		{
			lSize = 0;
			return E_OUTOFMEMORY;
		}
		G723_ADPCMWAVEFORMAT* pFormatEx = (G723_ADPCMWAVEFORMAT*)(*ppData);

		WAVEFORMATEX &oFormat	= pFormatEx->wfx;
		oFormat.cbSize			= (WORD)(sizeof(G723_ADPCMWAVEFORMAT) - sizeof(WAVEFORMATEX));//4?
		oFormat.wFormatTag		= WAVE_FORMAT_G723_ADPCM;
		oFormat.nChannels		= 1;	
		oFormat.wBitsPerSample	= (WORD)m_lSampleSize;
		oFormat.nSamplesPerSec	= m_lSampleRate;
		oFormat.nBlockAlign		= (WORD)m_lSampleSize*4;
		oFormat.nAvgBytesPerSec = oFormat.nBlockAlign * oFormat.nSamplesPerSec;

		pFormatEx->nAuxBlockSize = (WORD)m_lSampleSize;
		pFormatEx->cbExtraSize	 = 0;
		return S_OK;
	}
	HRESULT FillFormatMSG723(BYTE **ppData, long &lSize)
	{
		lSize = sizeof(WAVEFORMATEX);
		*ppData = (BYTE*)HeapAlloc(GetProcessHeap(), 0, lSize);
		if (NULL == (*ppData))
		{
			lSize = 0;
			return E_OUTOFMEMORY;
		}
		WAVEFORMATEX *pFormat = (WAVEFORMATEX *)(*ppData);
		
		pFormat->cbSize				= 0;
		pFormat->wFormatTag			= WAVE_FORMAT_MSG723;
		pFormat->nSamplesPerSec		= 8000;
		pFormat->nChannels			= 1;
		pFormat->wBitsPerSample		= 16;
		pFormat->nBlockAlign		= 2;
		pFormat->nAvgBytesPerSec	= pFormat->nSamplesPerSec * pFormat->nBlockAlign;
		return S_OK;
	}	
	HRESULT FillFormatG726(BYTE **ppData, long &lSize)
	{
		//надо объединить с FillFormatG723, отличаются только oFormat.wFormatTag
		lSize = sizeof(G723_ADPCMWAVEFORMAT);
		*ppData = (BYTE*)HeapAlloc(GetProcessHeap(), 0, lSize);
		if (NULL == (*ppData))
		{
			lSize = 0;
			return E_OUTOFMEMORY;
		}
		G723_ADPCMWAVEFORMAT* pFormatEx = (G723_ADPCMWAVEFORMAT*)(*ppData);

		WAVEFORMATEX &oFormat	= pFormatEx->wfx;
		oFormat.cbSize			= (WORD)(sizeof(G723_ADPCMWAVEFORMAT) - sizeof(WAVEFORMATEX));//4?
		oFormat.wFormatTag		= WAVE_FORMAT_G726_ADPCM;
		oFormat.nChannels		= 1;	
		oFormat.wBitsPerSample	= (WORD)m_lSampleSize;
		oFormat.nSamplesPerSec	= m_lSampleRate;
		oFormat.nBlockAlign		= (WORD)m_lSampleSize*4;
		oFormat.nAvgBytesPerSec = oFormat.nBlockAlign * oFormat.nSamplesPerSec;

		pFormatEx->nAuxBlockSize = (WORD)m_lSampleSize;
		pFormatEx->cbExtraSize	 = 0;
		return S_OK;
	}

	HRESULT FillFormatG729(BYTE **ppData, long &lSize)
	{
		lSize = sizeof(WAVEFORMATEX);
		*ppData = (BYTE*)HeapAlloc(GetProcessHeap(), 0, lSize);
		if (NULL == (*ppData))
		{
			lSize = 0;
			return E_OUTOFMEMORY;
		}
		WAVEFORMATEX *pFormat = (WAVEFORMATEX *)(*ppData);
		
		pFormat->cbSize				= 0;
		pFormat->wFormatTag			= WAVE_FORMAT_G729;
		pFormat->nSamplesPerSec		= m_lSampleRate;
		pFormat->nChannels			= 1;
		pFormat->wBitsPerSample		= 16;
		pFormat->nBlockAlign		= 2;
		pFormat->nAvgBytesPerSec	= pFormat->nSamplesPerSec * pFormat->nBlockAlign;
		return S_OK;
	}
	HRESULT FillFormatOggVorbis(BYTE **ppData, long &lSize)
	{
		lSize = sizeof(OGGWAVEFORMAT);
		*ppData = (BYTE*)HeapAlloc(GetProcessHeap(), 0, lSize);
		if (NULL == *ppData)
		{
			lSize = 0;
			return E_OUTOFMEMORY;
		}
		OGGWAVEFORMAT* pFormatEx = (OGGWAVEFORMAT*)(*ppData);

		WAVEFORMATEX &oFormat	= pFormatEx->wfx;
		oFormat.cbSize			= sizeof(OGGWAVEFORMAT) - sizeof(WAVEFORMATEX);
		oFormat.wFormatTag		= WAVE_FORMAT_OGG;
		oFormat.nChannels		= (WORD)m_lChannels;
		oFormat.wBitsPerSample	= 16;
		oFormat.nSamplesPerSec	= m_lSampleRate;
		oFormat.nBlockAlign		= 4;
		oFormat.nAvgBytesPerSec = m_lBitrate/8;

		pFormatEx->dwVorbisACMVersion	= VORBISACM_VERSION;
		pFormatEx->dwLibVorbisVersion	= LIBVORBIS_VERSION;
		pFormatEx->dwChannelsMask		= m_lChannelsMask;
		return S_OK;
	}
	HRESULT FillFormatMPC(BYTE **ppData, long &lSize)
	{
		lSize = sizeof(WAVEFORMATEX);
		*ppData = (BYTE*)HeapAlloc(GetProcessHeap(), 0, lSize);
		if (NULL == (*ppData))
		{
			lSize = 0;
			return E_OUTOFMEMORY;
		}
		WAVEFORMATEX *pFormat = (WAVEFORMATEX *)(*ppData);
		
		pFormat->cbSize				= 0;
		pFormat->wFormatTag			= WAVE_FORMAT_MPC;
		pFormat->nSamplesPerSec		= m_lSampleRate;
		pFormat->nChannels			= (WORD)m_lChannels;
		pFormat->wBitsPerSample		= 16;
		pFormat->nBlockAlign		= 4;
		pFormat->nAvgBytesPerSec	= m_lBitrate/8;
		return S_OK;
	}
	HRESULT FillFormatMsADPCM(BYTE **ppData, long &lSize)
	{
		const WORD cwADPCMCoefCount = 7;
		lSize = sizeof(ADPCMWAVEFORMAT) + cwADPCMCoefCount * sizeof(ADPCMCOEFSET);
		*ppData = (BYTE*)HeapAlloc(GetProcessHeap(), 0, lSize);
		if (NULL == (*ppData))
		{
			lSize = 0;
			return E_OUTOFMEMORY;
		}
		ADPCMWAVEFORMAT *pFormatEx = (ADPCMWAVEFORMAT *)(*ppData);
		
		WAVEFORMATEX &oFormat	= pFormatEx->wfx;
		oFormat.cbSize			= (WORD)(lSize - sizeof(WAVEFORMATEX));//32 ????
		oFormat.wFormatTag		= WAVE_FORMAT_ADPCM;
		oFormat.nSamplesPerSec	= m_lSampleRate;
		oFormat.nChannels		= (WORD)m_lChannels;
		oFormat.wBitsPerSample	= 4;		

		long lBlockSize = 1024;
		//Б.В. вот здесь http://icculus.org/SDL_sound/downloads/external_documentation/wavecomp.htm
		//сравнивают не частоту, а nSamplesPerSec x Channels
		//и там только 4 вариант значения этого произведения
		if (m_lSampleRate<12000)
		{
			lBlockSize = 256;
			oFormat.nBlockAlign			= 256*(short)m_lChannels;
			pFormatEx->wSamplesPerBlock = 500;
		}
		else if ((m_lSampleRate>=12000) && (m_lSampleRate<23000))
		{
			lBlockSize = 512;
			oFormat.nBlockAlign			= 512*(short)m_lChannels;
			pFormatEx->wSamplesPerBlock = 1012;
		}
		else if (m_lSampleRate>=23000)
		{
			lBlockSize = 1024;
			oFormat.nBlockAlign			= 1024*(short)m_lChannels;
			pFormatEx->wSamplesPerBlock = 2036;
		}

		lBlockSize *= m_lChannels;
		long lBlockSizePCM		= m_lSampleRate * m_lChannels * 2;
		long lBlockSizeADPCM	= 2 * m_lChannels + 4 * (lBlockSize - 7*m_lChannels);
		
		oFormat.nAvgBytesPerSec = MulDiv(lBlockSizePCM, lBlockSize, lBlockSizeADPCM);

		pFormatEx->wNumCoef = cwADPCMCoefCount;

		ADPCMCOEFSET *pCoef = pFormatEx->aCoef;
		pFormatEx->aCoef[0].iCoef1 = 256;
		pFormatEx->aCoef[0].iCoef2 = 0;

		pFormatEx->aCoef[1].iCoef1 = 512;
		pFormatEx->aCoef[1].iCoef2 = -256;

		pFormatEx->aCoef[2].iCoef1 = 0;
		pFormatEx->aCoef[2].iCoef2 = 0;

		pFormatEx->aCoef[3].iCoef1 = 192;
		pFormatEx->aCoef[3].iCoef2 = 64;

		pFormatEx->aCoef[4].iCoef1 = 240;
		pFormatEx->aCoef[4].iCoef2 = 0;

		pFormatEx->aCoef[5].iCoef1 = 460;
		pFormatEx->aCoef[5].iCoef2 = -208;

		pFormatEx->aCoef[6].iCoef1 = 392;
		pFormatEx->aCoef[6].iCoef2 = -232;
		return S_OK;
	}

	HRESULT FillFormatImaADPCM(BYTE **ppData, long &lSize)
	{
		lSize = sizeof(DVIADPCMWAVEFORMAT);
		*ppData = (BYTE*)HeapAlloc(GetProcessHeap(), 0, lSize);
		if (NULL == *ppData)
		{
			lSize = 0;
			return E_OUTOFMEMORY;
		}
		DVIADPCMWAVEFORMAT* pFormatEx = (DVIADPCMWAVEFORMAT*)(*ppData);

		WAVEFORMATEX &oFormat	= pFormatEx->wfx;
		oFormat.cbSize			= sizeof(DVIADPCMWAVEFORMAT) - sizeof(WAVEFORMATEX);
		oFormat.wFormatTag		= (WORD)m_lFormatTag;
		oFormat.nChannels		= (WORD)m_lChannels;
		oFormat.wBitsPerSample	= 4;
		oFormat.nBlockAlign		= 0;
		oFormat.nAvgBytesPerSec = 0;

		oFormat.nSamplesPerSec	= m_lSampleRate;

		if (WAVE_FORMAT_IMA_ADPCM_QT == m_lFormatTag)//Qt
		{
			long lBlockSize			= 34 * m_lChannels;
			long lBlockSizeADPCM	= 128 * m_lChannels;
			long lBlockSizePCM		= m_lChannels * m_lSampleRate * 2;

			oFormat.nBlockAlign		= (WORD)lBlockSize;
			if (0 != lBlockSizeADPCM)
				oFormat.nAvgBytesPerSec	= MulDiv(lBlockSizePCM, lBlockSize, lBlockSizeADPCM);
			else
				return E_FAIL;
			if (0 != m_lChannels)
				pFormatEx->wSamplesPerBlock		= (WORD)(lBlockSizeADPCM / 2 / m_lChannels + 1);
			else
				return E_FAIL;
		}
		else if (WAVE_FORMAT_IMA_ADPCM == m_lFormatTag)//Ms
		{
			long lBlockAlign	= 256 * m_lChannels;
			if (22050 <= m_lSampleRate)
				lBlockAlign *= 2;
			if (44100 <= m_lSampleRate)
				lBlockAlign *= 2;

			long lBlockAlignOut;
			if (0 != m_lChannels)
				lBlockAlignOut = (8 * (lBlockAlign - 4*m_lChannels) / (m_lChannels*4) + 1);
			else
				return E_FAIL;

			double dVSP;
			if (0 != lBlockAlignOut)
				dVSP = m_lSampleRate * lBlockAlign / (double)lBlockAlignOut;
			else
				return E_FAIL;

			oFormat.nBlockAlign		= (WORD)lBlockAlign;
			oFormat.nAvgBytesPerSec	= (long)dVSP;
			pFormatEx->wSamplesPerBlock		= (WORD)lBlockAlignOut;
		}
		return S_OK;
	}	
	HRESULT FillFormatLaw(BYTE **ppData, long &lSize)
	{
		lSize = sizeof(WAVEFORMATEX);
		*ppData = (BYTE*)HeapAlloc(GetProcessHeap(), 0, lSize);
		if (NULL == (*ppData))
		{
			lSize = 0;
			return E_OUTOFMEMORY;
		}
		WAVEFORMATEX *pFormat = (WAVEFORMATEX *)(*ppData);
		
		pFormat->cbSize				= 0;
		pFormat->wFormatTag			= (WORD)m_lFormatTag;
		pFormat->nSamplesPerSec		= m_lSampleRate;
		pFormat->nChannels			= (WORD)m_lChannels;
		pFormat->wBitsPerSample		= 8;
		pFormat->nBlockAlign		= pFormat->nChannels;
		pFormat->nAvgBytesPerSec	= pFormat->nBlockAlign * pFormat->nSamplesPerSec;
		return S_OK;
	}
	HRESULT FillFormatGsm(BYTE **ppData, long &lSize)
	{
		lSize = sizeof(GSM610WAVEFORMAT);
		*ppData = (BYTE*)HeapAlloc(GetProcessHeap(), 0, lSize);
		if (NULL == *ppData)
		{
			lSize = 0;
			return E_OUTOFMEMORY;
		}
		GSM610WAVEFORMAT* pFormatEx = (GSM610WAVEFORMAT*)(*ppData);

		WAVEFORMATEX &oFormat	= pFormatEx->wfx;
		oFormat.cbSize			= sizeof(GSM610WAVEFORMAT) - sizeof(WAVEFORMATEX);
		oFormat.wFormatTag		= WAVE_FORMAT_GSM610;
		oFormat.nChannels		= 1;
		oFormat.wBitsPerSample	= 0;
		oFormat.nSamplesPerSec	= m_lSampleRate;
		oFormat.nBlockAlign		= 65;

		switch(oFormat.nSamplesPerSec)
		{
		case 8000:
			oFormat.nAvgBytesPerSec = 1625;
			break;
		case 11025:
			oFormat.nAvgBytesPerSec = 2239;
			break;
		case 16000:
			oFormat.nAvgBytesPerSec = 3250;
			break;
		case 22050:
			oFormat.nAvgBytesPerSec = 4478;
			break;
		case 32000:
			oFormat.nAvgBytesPerSec = 6500;
			break;
		case 44100:
		default:
			oFormat.nAvgBytesPerSec = 8957;
			break;
		}

		pFormatEx->wSamplesPerBlock = 320;
		return S_OK;
	}
	HRESULT FillFormatAC3(BYTE **ppData, long &lSize,bool useBits=false)
	{
		lSize = sizeof(WAVEFORMATEX_AC3);
		*ppData = (BYTE*)HeapAlloc(GetProcessHeap(), 0, lSize);
		if (NULL == *ppData)
		{
			lSize = 0;
			return E_OUTOFMEMORY;
		}
		WAVEFORMATEX_AC3* pFormatEx = (WAVEFORMATEX_AC3*)(*ppData);

		WAVEFORMATEX &oFormat	= pFormatEx->wfx;
		oFormat.cbSize			= sizeof(WAVEFORMATEX_AC3) - sizeof(WAVEFORMATEX);
		oFormat.wFormatTag		= (WORD)m_lFormatTag;
		oFormat.nChannels		= (WORD)m_lChannels;
		oFormat.nSamplesPerSec	= m_lSampleRate;
		if (useBits)
			oFormat.wBitsPerSample	= m_lSampleSize;
		else
			oFormat.wBitsPerSample	= 16;
		oFormat.nBlockAlign		= 1;
		oFormat.nAvgBytesPerSec = m_lBitrate/8;
		
		pFormatEx->dwChannelMask = m_lChannelsMask;
		return S_OK;
	}
	HRESULT FillFormatAAC(BYTE **ppData, long &lSize)
	{
		lSize = sizeof(WAVEFORMATEX)+8;
		if (NULL!=m_pExtraData)
			lSize += m_pExtraData->rgsabound[0].cElements;

		*ppData = (BYTE*)HeapAlloc(GetProcessHeap(), 0, lSize);
		if (NULL == *ppData)
		{
			lSize = 0;
			return E_OUTOFMEMORY;
		}
		WAVEFORMATEX* pFormatEx = (WAVEFORMATEX*)(*ppData);

		pFormatEx->cbSize			= (WORD)(lSize - sizeof(WAVEFORMATEX));
		pFormatEx->wFormatTag		= WAVE_FORMAT_AAC;
		pFormatEx->nChannels		= (WORD)m_lChannels;
		pFormatEx->nSamplesPerSec	= m_lSampleRate;
		pFormatEx->wBitsPerSample	= 16;
		pFormatEx->nBlockAlign		= 1;
		pFormatEx->nAvgBytesPerSec = m_lBitrate/8;

		((WORD*)(pFormatEx+1))[0]	= 0;//mp4
		((WORD*)(pFormatEx+1))[1]	= 2;//lc
		((DWORD*)(pFormatEx+1))[1]	= m_lChannelsMask;
						
		if ((NULL!=m_pExtraData) && (0 < m_pExtraData->rgsabound[0].cElements))
			memcpy(((char*)(pFormatEx+1))+8, m_pExtraData->pvData, m_pExtraData->rgsabound[0].cElements);	
		return S_OK;
	}		
	HRESULT FillFormatAMR(BYTE **ppData, long &lSize)
	{
		long lMode = 0;
		switch (m_lSampleRate)
		{
		case 8000:
			{
				long lSize = sizeof(g_carlModeAMR) / sizeof(long);
				for (lMode = 0; lMode < lSize; lMode++)
				{
					if (g_carlModeAMR[lMode] == m_lBitrate)
						break;
				}
			}
			break;
		case 16000:
			{
				long lSize = sizeof(g_carlModeAWB) / sizeof(long);
				for (lMode = 0; lMode < lSize; lMode++)
				{
					if (g_carlModeAWB[lMode] == m_lBitrate)
						break;
				}
			}
			break;
		}

		lSize = sizeof(WAVEFORMATEX) + 2;
		*ppData = (BYTE*)HeapAlloc(GetProcessHeap(), 0, lSize);
		if (NULL == (*ppData))
		{
			lSize = 0;
			return E_OUTOFMEMORY;
		}

		WAVEFORMATEX *pFormat = (WAVEFORMATEX *)(*ppData);

		pFormat->cbSize				= 2;
		pFormat->wFormatTag			= WAVE_FORMAT_AMR_AMV;
		pFormat->wBitsPerSample		= 16;
		pFormat->nBlockAlign		= 1;
		pFormat->nChannels			= 1;
		pFormat->nSamplesPerSec		= (WORD)m_lSampleRate;
		pFormat->nAvgBytesPerSec	= (DWORD)(m_lBitrate/8);

		*((WORD *)((*ppData) + sizeof(WAVEFORMATEX))) = (WORD)lMode;
		return S_OK;
	}


	HRESULT FillFormatFLAC(BYTE **ppData, long &lSize)
	{
		lSize = sizeof(WAVEFORMATEX);
		*ppData = (BYTE*)HeapAlloc(GetProcessHeap(), 0, lSize);
		if (NULL == (*ppData))
		{
			lSize = 0;
			return E_OUTOFMEMORY;
		}
		WAVEFORMATEX *pFormat = (WAVEFORMATEX *)(*ppData);
		
		pFormat->cbSize				= 0;
		pFormat->wFormatTag			= WAVE_FORMAT_FLAC;
		pFormat->nSamplesPerSec		= m_lSampleRate;
		pFormat->nChannels			= (WORD)m_lChannels;
		pFormat->wBitsPerSample		= (WORD)m_lSampleSize;
		pFormat->nBlockAlign		= 1;
		pFormat->nAvgBytesPerSec	= pFormat->nSamplesPerSec;
		return S_OK;
	}
	HRESULT FillFormatAppleLossless(BYTE **ppData, long &lSize) 
	{
		lSize = sizeof(WAVEFORMATEX)+4;
		if (NULL!=m_pExtraData)
			lSize += m_pExtraData->rgsabound[0].cElements;

		*ppData = (BYTE*)HeapAlloc(GetProcessHeap(), 0, lSize);
		if (NULL == (*ppData))
		{
			lSize = 0;
			return E_OUTOFMEMORY;
		}
		WAVEFORMATEX *pFormatEx = (WAVEFORMATEX *)(*ppData);
		
		pFormatEx->cbSize			= (WORD)(lSize - sizeof(WAVEFORMATEX));
		pFormatEx->wFormatTag		= WAVE_APPLE_LOSSLESS;
		pFormatEx->nChannels		= (WORD)m_lChannels;
		pFormatEx->nSamplesPerSec	= m_lSampleRate;
		pFormatEx->wBitsPerSample	= m_lSampleSize;
		pFormatEx->nBlockAlign		= (WORD)m_lChannels;
		pFormatEx->nAvgBytesPerSec = m_lSampleRate*m_lChannels;

		((DWORD*)(pFormatEx+1))[0] = m_lChannelsMask;

		if ((NULL!=m_pExtraData) && (0 < m_pExtraData->rgsabound[0].cElements))
			memcpy(((char*)(pFormatEx+1))+4, m_pExtraData->pvData, m_pExtraData->rgsabound[0].cElements);
		return S_OK;
	}

};

// CAVSAudioFormat
[ coclass, uuid("573912EE-8BC2-4099-B598-971635E2F602"), threading(apartment), vi_progid("AVSMediaFormatSettings3.AVSAudioFormat"), progid("AVSMediaFormatSettings3.AVSAudioForma.1"), version(1.0)]
class ATL_NO_VTABLE CAVSAudioFormat : public CAVSAudioFormatImpl<CAVSAudioFormat, IAVSAudioFormat>
{
};

