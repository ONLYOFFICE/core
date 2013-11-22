// ASCVideoFormat.h : Declaration of the CASCVideoFormat

#pragma once
#include "resource.h"       // main symbols
#include "ASCMediaFormat.h"       // main symbols

#include "../Common/FourCCDefines.h"

class CAVSVideoFormat;
// IAVSVideoFormat
[ object, uuid("AC948866-8408-4F7F-85FE-FF6CFD1F1264"), dual, pointer_default(unique)]
__interface IAVSVideoFormat : IAVSMediaFormat
{
	[id(102000 + 1), propget] HRESULT fccHandler([out, retval] ULONG* pVal);
	[id(102000 + 1), propput] HRESULT fccHandler([in] ULONG newVal);
		
	[id(102000 + 2), propget] HRESULT Bitrate([out, retval] long* pVal);
	[id(102000 + 2), propput] HRESULT Bitrate([in] long newVal);
	[id(102000 + 3), propget] HRESULT FrameRate([out, retval] double* pVal);
	[id(102000 + 3), propput] HRESULT FrameRate([in] double newVal);
		
	[id(102000 + 4), propget] HRESULT Width([out, retval] long* pVal);
	[id(102000 + 4), propput] HRESULT Width([in] long newVal);
	[id(102000 + 5), propget] HRESULT Height([out, retval] long* pVal);
	[id(102000 + 5), propput] HRESULT Height([in] long newVal);
	//см. "MediaFormatDefine.h" define с префиксом CSP_
	[id(102000 + 6), propget] HRESULT ColorSpace([out, retval] long* pVal);
	[id(102000 + 6), propput] HRESULT ColorSpace([in] long newVal);
	[id(102000 + 7), propget] HRESULT AspectRatioX([out, retval] long* pVal);
	[id(102000 + 7), propput] HRESULT AspectRatioX([in] long newVal);
	[id(102000 + 8), propget] HRESULT AspectRatioY([out, retval] long* pVal);
	[id(102000 + 8), propput] HRESULT AspectRatioY([in] long newVal);
		
	[id(102000 + 9), propget] HRESULT CodecName([out, retval] BSTR* pVal);

	[id(102000 + 10), propget] HRESULT InternalCodecType([out, retval] long* pVal);
	[id(102000 + 10), propput] HRESULT InternalCodecType([in] long newVal);
};

template <class CoT, class T>
class CAVSVideoFormatImpl : public CAVSMediaFormatImpl<CoT, T>
{
public:
	CAVSVideoFormatImpl()
		: m_bIsInterlaced(FALSE)
	{
		m_lMediaType = MEDIAFORMAT_VIDEO;
		SetDefaultProperties();
	}

	STDMETHOD(CreateDuplicate)(IAVSMediaFormat** pMediaFormat)
	{
		CAVSMediaFormatImpl<CoT, T>::CreateDuplicate(pMediaFormat);
		if (NULL==(*pMediaFormat))
			return S_OK;
		IAVSVideoFormat * pAVSVideoFormat=NULL;
		(*pMediaFormat)->QueryInterface(__uuidof(IAVSVideoFormat), (void**)&pAVSVideoFormat);
		if (NULL==pAVSVideoFormat)
		{
			(*pMediaFormat)->Release();
			*pMediaFormat = NULL;
			return S_OK;
		}

		pAVSVideoFormat->put_fccHandler(m_ulfccHandler);
		pAVSVideoFormat->put_Bitrate(m_lBitrate);
		pAVSVideoFormat->put_FrameRate(m_dFrameRate);
		
		pAVSVideoFormat->put_Width(m_lWidth);
		pAVSVideoFormat->put_Height(m_lHeight);
		pAVSVideoFormat->put_ColorSpace(m_lColorSpace);
		pAVSVideoFormat->put_AspectRatioX(m_lAspectRatioX);
		pAVSVideoFormat->put_AspectRatioY(m_lAspectRatioY);
		pAVSVideoFormat->put_InternalCodecType(m_lInternalCodecType);

		pAVSVideoFormat->Release();
		return S_OK;
	}
	STDMETHOD(SetDefaultProperties)(void)
	{
		m_ulfccHandler = 0xFFFFFFFF;
		m_lInternalCodecType = 0;
		m_lBitrate = 2500000;
		m_dFrameRate = 30.0;

		m_lWidth = 352;
		m_lHeight = 288;
		m_lColorSpace = CSP_BGRA|CSP_VFLIP;
		m_lAspectRatioX = 0;
		m_lAspectRatioY = 1;

		return S_OK;
	}
	STDMETHOD(SetPropertiesByFormat)(IAVSMediaFormat* pMediaFormat)
	{
		CAVSMediaFormatImpl<CoT, T>::SetPropertiesByFormat(pMediaFormat);
		if (pMediaFormat!=NULL)
		{
			IAVSVideoFormat * pAVSVideoFormat=NULL;
			pMediaFormat->QueryInterface(__uuidof(IAVSVideoFormat), (void**)&pAVSVideoFormat);
			if (pAVSVideoFormat!=NULL)
			{
				pAVSVideoFormat->get_fccHandler(&m_ulfccHandler);
				pAVSVideoFormat->get_Bitrate(&m_lBitrate);
				pAVSVideoFormat->get_FrameRate(&m_dFrameRate);
				
				pAVSVideoFormat->get_Width(&m_lWidth);
				pAVSVideoFormat->get_Height(&m_lHeight);
				pAVSVideoFormat->get_ColorSpace(&m_lColorSpace);
				pAVSVideoFormat->get_AspectRatioX(&m_lAspectRatioX);
				pAVSVideoFormat->get_AspectRatioY(&m_lAspectRatioY);
				pAVSVideoFormat->get_InternalCodecType(&m_lInternalCodecType);

				RELEASEINTERFACE(pAVSVideoFormat);
			}
		}
		return S_OK;
	}
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		CString sParamName; sParamName = ParamName;		
		if (_T("IsInterlaced") == sParamName)
		{//17.07.2009 Добавлено по указанию С.А.
			m_bIsInterlaced = (VARIANT_FALSE != ParamValue.boolVal);
		}
		//Здесь проверяем параметры относящиеся к VideoFormat
		return CAVSMediaFormatImpl<CoT, T>::SetAdditionalParam(ParamName, ParamValue);
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		CString sParamName; sParamName = ParamName;
		
		if (_T("IsInterlaced") == sParamName)
		{//17.07.2009 Добавлено по указанию С.А.
			if (NULL==ParamValue)
				return S_OK;
			ParamValue->vt = VT_BOOL;
			ParamValue->boolVal = m_bIsInterlaced ? VARIANT_TRUE : VARIANT_FALSE;
			return S_OK;
		}
		//Здесь проверяем параметры относящиеся к VideoFormat
		return CAVSMediaFormatImpl<CoT, T>::GetAdditionalParam(ParamName, ParamValue);
	}

	
	STDMETHOD(get_fccHandler)(ULONG* pVal)
	{
		*pVal = m_ulfccHandler;
		return S_OK;
	}
	STDMETHOD(put_fccHandler)(ULONG newVal)
	{
		m_ulfccHandler = newVal;
		return S_OK;
	}
			
	STDMETHOD(get_Bitrate)(long* pVal)
	{
		*pVal = m_lBitrate;
		return S_OK;
	}
	STDMETHOD(put_Bitrate)(long newVal)
	{
		m_lBitrate = newVal;
		return S_OK;
	}
	STDMETHOD(get_FrameRate)(double* pVal)
	{
		*pVal = m_dFrameRate;
		return S_OK;
	}
	STDMETHOD(put_FrameRate)(double newVal)
	{
		m_dFrameRate = newVal;
		return S_OK;
	}
	
	STDMETHOD(get_Width)(long* pVal)
	{
		*pVal = m_lWidth;
		return S_OK;
	}
	STDMETHOD(put_Width)(long newVal)
	{
		m_lWidth = newVal;
		return S_OK;
	}
	STDMETHOD(get_Height)(long* pVal)
	{
		*pVal = m_lHeight;
		return S_OK;
	}
	STDMETHOD(put_Height)(long newVal)
	{
		m_lHeight = newVal;
		return S_OK;
	}
	STDMETHOD(get_ColorSpace)(long* pVal)
	{
		*pVal = m_lColorSpace;
		return S_OK;
	}
	STDMETHOD(put_ColorSpace)(long newVal)
	{
		m_lColorSpace = newVal;
		return S_OK;
	}
	STDMETHOD(get_AspectRatioX)(long* pVal)
	{
		*pVal = m_lAspectRatioX;
		return S_OK;

	}
	STDMETHOD(put_AspectRatioX)(long newVal)
	{
		m_lAspectRatioX = newVal;
		return S_OK;
	}
	STDMETHOD(get_AspectRatioY)(long* pVal)
	{
		*pVal = m_lAspectRatioY;
		return S_OK;
	}
	STDMETHOD(put_AspectRatioY)(long newVal)
	{
		m_lAspectRatioY = newVal;
		return S_OK;
	}
	
	STDMETHOD(get_CodecName)(BSTR* pVal)
	{
		CString	sCodecName;
		if (m_lInternalCodecType==0)
		{
			sCodecName = GetVideoCodecByFourCC(m_ulfccHandler);
//			if (0!=m_lInternalCodecType)
//				sCodecName += _T(" (Internal)");

		} else
		{
			switch (m_lInternalCodecType)
			{
			case XVID_INTERNALCODEC:
				sCodecName = _T("MPEG4 (DivX/Xvid compatible)");
				break;
			case INTELMEDIA_H264_INTERNALCODEC:
				sCodecName = _T("H.264/AVC (Intel)");
				break;
			case H264_INTERNALCODEC:
			case FF_H264_INTERNALCODEC:
				sCodecName = _T("H.264/AVC (Advanced Video Coding)");
				break;
			case FF_FLV_INTERNALCODEC:
				sCodecName = _T("H.263 FLV Video");
				break;
			case RV7_INTERNALCODEC:
				sCodecName = _T("Real Video 7");
				break;
			case RV8_INTERNALCODEC:
				sCodecName = _T("Real Video 8");
				break;
			case RV9_INTERNALCODEC:
				sCodecName = _T("Real Video 9");
				break;
			case RV10_INTERNALCODEC:
				sCodecName = _T("Real Video 10");
				break;
			case FF_SVQ1_INTERNALCODEC:
				sCodecName = _T("Sorenson Video 1");
				break;
			case FF_SVQ3_INTERNALCODEC:			
				sCodecName = _T("Sorenson Video 3");
				break;
			case FF_DV_INTERNALCODEC:			
				sCodecName = _T("Digital Video");
				break;
			case FF_MJPEG_INTERNALCODEC:		
				sCodecName = _T("IBM Motion JPEG");
				break;
			case FF_MJPEGB_INTERNALCODEC:		
				sCodecName = _T("Motion JPEG Codec");
				break;
			case FF_SP5X_INTERNALCODEC:			
				sCodecName = _T("SunPlus Aiptek MegaCam Codec");
				break;
			case FF_LJPEG_INTERNALCODEC:		
				sCodecName = _T("Lossless JPEG compression");
				break;
			case FF_MSMPEG4_INTERNALCODEC:		
				sCodecName = _T("Microsoft MPEG-4");
				break;
			case FF_MSMPEG4V1_INTERNALCODEC:	
				sCodecName = _T("Microsoft MPEG-4 V1");
				break;
			case FF_MSMPEG4V2_INTERNALCODEC:	
				sCodecName = _T("Microsoft MPEG-4 V2");
				break;
			case FF_MSMPEG4V3_INTERNALCODEC:	
				sCodecName = _T("Microsoft MPEG-4 V3");
				break;
			case FF_CINEPAK_INTERNALCODEC:		
				sCodecName = _T("Cinepak by Radius");
				break;
			case FF_INDEO2_INTERNALCODEC:		
				sCodecName = _T("Intel Indeo Video 2");
				break;
			case FF_INDEO3_INTERNALCODEC:		
				sCodecName = _T("Intel Indeo Video 3");
				break;
			case FF_HUFFMAN_INTERNALCODEC:		
				sCodecName = _T("Huffman Lossless Codec");
				break;
			case FF_8BPS_INTERNALCODEC:			
				sCodecName = _T("Planar RGB");
				break;
			case FF_VC1_INTERNALCODEC:			
				sCodecName = _T("Microsoft Video Codec 1");
				break;
			case FF_RLE_INTERNALCODEC:			
				sCodecName = _T("Apple Animation");
				break;
			case FF_SMC_INTERNALCODEC:			
				sCodecName = _T("Apple Graphics");
				break;
			case FF_RPZA_INTERNALCODEC:			
				sCodecName = _T("Apple Video");
				break;
			case FF_QDRW_INTERNALCODEC:			
				sCodecName = _T("Apple Quick draw");
				break;
			case FF_CYUV_INTERNALCODEC:			
				sCodecName = _T("Creative YUV");
				break;
			case FF_H261_INTERNALCODEC:			
				sCodecName = _T("Intel ITU H.261");
				break;
			case FF_ASUSV1_INTERNALCODEC:		
				sCodecName = _T("Asus Video V1");
				break;
			case FF_ASUSV2_INTERNALCODEC:		
				sCodecName = _T("Asus Video V2");
				break;
			case FF_QPEG_INTERNALCODEC:			
				sCodecName = _T("Q-Team QPEG");
				break;
			case FF_ON2VP6F_INTERNALCODEC:		
				sCodecName = _T("On2 VP6 flipped");
				break;
			case FF_ON2VP3_INTERNALCODEC:		
				sCodecName = _T("On2 VP3");
				break;
			case FF_ON2VP5_INTERNALCODEC:		
				sCodecName = _T("On2 VP5");
				break;
			case FF_ON2VP6_INTERNALCODEC:		
				sCodecName = _T("On2 VP6");
				break;
			case FF_FRAPS_INTERNALCODEC:
				sCodecName = _T("Fraps");
				break;
			default:
				sCodecName = GetVideoCodecByFourCC(m_ulfccHandler);
			}
		}
		*pVal = sCodecName.AllocSysString();
		return S_OK;
	}
	
	STDMETHOD(get_InternalCodecType)(long* pVal)
	{
		*pVal = m_lInternalCodecType;
		return S_OK;
	}
	STDMETHOD(put_InternalCodecType)(long newVal)
	{
		m_lInternalCodecType = newVal;
		return S_OK;
	}

protected:	
	ULONG m_ulfccHandler;
	long m_lInternalCodecType;
	long m_lBitrate;
	double m_dFrameRate;
	long m_lWidth;
	long m_lHeight;

	long m_lColorSpace;
	long m_lAspectRatioX;
	long m_lAspectRatioY;

	BOOL m_bIsInterlaced;

	const static CString GetVideoCodecByFourCC(DWORD fccHandler)
	{
		char	fcc[5];
		fcc[0]=((BYTE*)&fccHandler)[0];
		fcc[1]=((BYTE*)&fccHandler)[1];
		fcc[2]=((BYTE*)&fccHandler)[2];
		fcc[3]=((BYTE*)&fccHandler)[3];
		fcc[4]=0;

		fccHandler = UPPERCASEFOURCC(fccHandler);

		CString	sCodecName((char *)fcc);
		BOOL	bNotFound = FALSE;
		while (!bNotFound)
		{
			switch (fccHandler)
			{
			case _3IV0Handler:
				sCodecName = _T("MPEG4-based Codec 3ivx");
				break;
			case _3IV1Handler:
				sCodecName = _T("MPEG4-based Codec 3ivx");
				break;
			case _3IV2Handler:
				sCodecName = _T("MPEG4-based Codec 3ivx");
				break;
			case _3IVDHandler:
				sCodecName = _T("FFmpeg DivX ;-) (MS MPEG-4 v3)");
				break;
			case _3IVXHandler:
				sCodecName = _T("MPEG4-based Codec 3ivx");
				break;
			case 0:
			case _8BPSHandler:
				sCodecName = _T("Uncompressed RGB");	//Planar
				break;
			case _SGIHandler:
				sCodecName = _T("SGI");
				break;
			case AAS4Handler:
				sCodecName = _T("Autodesk Animator Codec (RLE)");
				break;
			case ASV1Handler:
				sCodecName = _T("Asus Video V1");
				break;
			case ASV2Handler:
				sCodecName = _T("Asus Video V2");
				break;
			case AASCHandler:
				sCodecName = _T("Autodesk Animator Codec (RLE)");
				break;
			case ABYRHandler:
				sCodecName = _T("Kensington Codec");
				break;
			case ADV1Handler:
				sCodecName = _T("Loronix WaveCodec");	//(used in various CCTV products)
				break;
			case ADVJHandler:
				sCodecName = _T("Avid M-JPEG Avid Technology Also known as AVRn");
				break;
			case AEMIHandler:
				sCodecName = _T("Array VideoONE MPEG1-I Capture");
				break;
			case AFLCHandler:
				sCodecName = _T("Autodesk Animator FLC");	//(256 color)
				break;
			case AFLIHandler:
				sCodecName = _T("Autodesk Animator FLI");	//(256 color)
				break;
			case ALPHHandler:	//Not specified
				bNotFound = TRUE;
				break;
			case AMPGHandler:
				sCodecName = _T("Array VideoONE MPEG");
				break;
			case ANIMHandler:
				sCodecName = _T("Intel RDX");
				break;
			case AP41Handler:
				sCodecName = _T("AngelPotion Definitive");	//(hack MS MP43)
				break;
			case ASVXHandler:
				sCodecName = _T("Asus Video 2.0");
				break;
			case AUR2Handler:
				sCodecName = _T("AuraVision Aura 2 Codec");
				break;
			case AURAHandler:
				sCodecName = _T("AuraVision Aura 1 Codec");
				break;
			case AUVXHandler:
				sCodecName = _T("AUVX video Codec");
				break;
			case AVC1Handler:
				sCodecName = _T("H.264 Video");
				break;
			case AVDJHandler:
				sCodecName = _T("Avid Motion JPEG");
				break;
			case AVI1Handler:
				sCodecName = _T("MainConcept Motion JPEG Codec");
				break;
			case AVI2Handler:
				sCodecName = _T("MainConcept Motion JPEG Codec");
				break;
			case AVRHandler:
				sCodecName = _T("AVR JPEG");
				break;
			case AVRNHandler:
				sCodecName = _T("Avid Motion JPEG");
				break;
			case AZPRHandler:
				sCodecName = _T("Quicktime Apple Video");
				break;
			case B16GHandler:
				sCodecName = _T("Grayscale uncompressed");	//16Gray
				break;
			case B32AHandler:
				sCodecName = _T("Grayscale uncompressed");	//32AlphaGray
				break;
			case B48RHandler:
				sCodecName = _T("Uncompressed RGB48");	//48RGB
				break;
			case B64AHandler:
				sCodecName = _T("Uncompressed RGB64");	//64ARGB
				break;
			case BASEHandler:
				sCodecName = _T("Base");
				break;
			case BGRHandler:
				sCodecName = _T("Uncompressed BGR32");	//8:8:8:8
				break;
			case BINKHandler:
				sCodecName = _T("Bink Video");	//(RAD Game Tools) (256 color)
				break;
			case BITMHandler:
				sCodecName = _T("Microsoft H.261");
				break;
			case BLZ0Handler:
				sCodecName = _T("FFmpeg MPEG-4");
				break;
			case BT20Handler:
				sCodecName = _T("Brooktree MediaStream Codec");	//(ex Brooktree)
				break;
			case BTCVHandler:
				sCodecName = _T("Brooktree composite video Codec");
				break;
			case BTVCHandler:
				sCodecName = _T("Conexant Composite Video");
				break;
			case BW10Handler:
				sCodecName = _T("Data Translation Broadway MPEG Capture/Compression");
				break;
			case CC12Handler:
				sCodecName = _T("Intel YUV12 Codec");
				break;
			case CDVCHandler:
				sCodecName = _T("Canopus DV Codec");
				break;
			case CFCCHandler:
				sCodecName = _T("Conkrete DPS Perception Motion JPEG");
				break;
			case CGDIHandler:
				sCodecName = _T("Microsoft CamCorder in Office 97");
				break;
			case CHAMHandler:
				sCodecName = _T("WinNow Caviara Champagne");
				break;
			case CJPGHandler:
				sCodecName = _T("Creative Video Blaster Webcam Go JPEG");
				break;
			case CLJRHandler:
				sCodecName = _T("Cirrus Logic YUV 4:1:1");
				break;
			case CLOUHandler:
				sCodecName = _T("Cloud");
				break;
			case CLPLHandler:
				sCodecName = _T("Format similar to YV12 but including a level of indirection.");
				break;
			case CM10Handler:
				sCodecName = _T("MediaShow 1.0");
				break;
			case CMYKHandler:
				sCodecName = _T("Common Data Format in Printing");
				break;
			case COL0Handler:
				sCodecName = _T("FFmpeg DivX ;-) (MS MPEG-4 v3)");
				break;
			case COL1Handler:
				sCodecName = _T("FFmpeg DivX ;-) (MS MPEG-4 v3)");
				break;
			case CPLAHandler:
				sCodecName = _T("Weitek YUV 4:2:0 Planar");
				break;
			case CRAMHandler:
				sCodecName = _T("Microsoft Video 1");
				break;
			case CT10Handler:
				sCodecName = _T("TalkingShow 1.0");
				break;
			case CVIDHandler:
				sCodecName = _T("Cinepak by Radius");	//YUV 4:2:2
				break;
			case CWLTHandler:
				sCodecName = _T("Microsoft Color WLT DIB");
				break;
			case CYUVHandler:
				sCodecName = _T("Creative Labs YUV");
				break;
			case CYUYHandler:
				sCodecName = _T("ATI Technologies YUV");
				break;
			case D261Handler:
				sCodecName = _T("DEC H.261");
				break;
			case D263Handler:
				sCodecName = _T("DEC H.263");
				break;
			case DCAPHandler:
				sCodecName = _T("Pinnacle DV25 Codec");
				break;
			case DCMJHandler:
				sCodecName = _T("dcmj Video V3.00a");
				break;
			case DIBHandler:
				sCodecName = _T("Uncompressed");
				break;
			case DIV1Handler:
				sCodecName = _T("FFmpeg-4 V1");	//(hacked MS MPEG-4 V1)
				break;
			case DIV2Handler:
				sCodecName = _T("MS MPEG-4 V2");
				break;
			case DIV3Handler:
				sCodecName = _T("Low motion DivX MPEG-4");
				break;
			case DIV4Handler:
				sCodecName = _T("Fast motion DivX MPEG-4");
				break;
			case DIV5Handler:
				sCodecName = _T("DivX MPEG-4");
				break;
			case DIV6Handler:
				sCodecName = _T("DivX MPEG-4");
				break;
			case DIVXHandler:
				sCodecName = _T("DivX MPEG-4");	//OpenDivX (DivX 4.0 and later)
				break;
			case DJPGHandler:
				sCodecName = _T("Broadway 101 Motion JPEG Codec");
				break;
			case DMB1Handler:
				sCodecName = _T("Matrox Rainbow Runner hardware compression");	//(Motion JPEG)
				break;
			case DMB2Handler:
				sCodecName = _T("Motion JPEG Codec used by Paradigm");
				break;
			case DP02Handler:
				sCodecName = _T("DynaPel MPEG-4");
				break;
			case DP16Handler:
				sCodecName = _T("YUV411 with DPCM 6-bit compression");
				break;
			case DP18Handler:
				sCodecName = _T("YUV411 with DPCM 8-bit compression");
				break;
			case DP26Handler:
				sCodecName = _T("YUV422 with DPCM 6-bit compression");
				break;
			case DP28Handler:
				sCodecName = _T("YUV422 with DPCM 8-bit compression");
				break;
			case DP96Handler:
				sCodecName = _T("YVU9 with DPCM 6-bit compression");
				break;
			case DP98Handler:
				sCodecName = _T("YVU9 with DPCM 8-bit compression");
				break;
			case DP9LHandler:
				sCodecName = _T("YVU9 with DPCM 6-bit compression and thinned-out");
				break;
			case DPS0Handler:
				sCodecName = _T("DPS Reality Motion JPEG");
				break;
			case DPSCHandler:
				sCodecName = _T("DPS PAR Motion JPEG");
				break;
			case DRWXHandler:
				sCodecName = _T("Pinnacle DV25 Codec");
				break;
			case DSVDHandler:
				sCodecName = _T("Microsoft DirectShow DV");
				break;
			case DUCKHandler:
				sCodecName = _T("TRUE Motion 1.0");
				break;
			case DV1NHandler:
				sCodecName = _T("DVC Pro100 NTSC");
				break;
			case DV1PHandler:
				sCodecName = _T("DVC Pro100 PAL");
				break;
			case DV25Handler:
				sCodecName = _T("Matrox DVCPRO Codec");	//SMPTE 314M 25Mb/s compressed
				break;
			case DV50Handler:
				sCodecName = _T("Matrox DVCPRO50 Codec");	//SMPTE 314M 50Mb/s compressed
				break;
			case DV5NHandler:
				sCodecName = _T("DVC Pro50 NTSC");
				break;
			case DV5PHandler:
				sCodecName = _T("DVC Pro50 PAL");
				break;
			case DVCHandler:
				sCodecName = _T("MainConcept DV Codec");
				break;
			case DVCPHandler:
				sCodecName = _T("Digital Video");	//Sony (DV)
				break;
			case DVCSHandler:
				sCodecName = _T("MainConcept DV Codec");
				break;
			case DVE2Handler:
				sCodecName = _T("DVE-2 Videoconferencing Codec");
				break;
			case DVH1Handler:
				sCodecName = _T("DVC Pro HD");
				break;
			case DVH5Handler:
				sCodecName = _T("DVC PRO HD1080i 50");
				break;
			case DVH6Handler:
				sCodecName = _T("DVC PRO HD1080i 60");
				break;
			case DVHDHandler:
				sCodecName = _T("Digital Video");	//1125 lines at 30.00 Hz or 1250 lines at 25.00 Hz
				break;
			case DVHPHandler:
				sCodecName = _T("DVC PRO HD720p");
				break;
			case DVNMHandler:
				bNotFound=TRUE;
				break;
			case DVMAHandler:
				sCodecName = _T("Darim Vision DVMPEG");	//(dummy for MPEG compressor)
				break;
			case DVPPHandler:
				sCodecName = _T("DVC Pro PAL");
				break;
			case DVSDHandler:
				sCodecName = _T("Digital Video");	//Sony (DV) 525 lines at 29.97 Hz or 625 lines at 25.00 Hz
				break;
			case DVSLHandler:
				sCodecName = _T("Digital Video compressed in SD");	//(SDL)
				break;
			case DVX1Handler:
				sCodecName = _T("Lucent DVX1000SP Video");	//Decoder
				break;
			case DVX2Handler:
				sCodecName = _T("Lucent DVX2000S Video");	//Decoder
				break;
			case DVX3Handler:
				sCodecName = _T("Lucent DVX3000S Video");	//Decoder
				break;
			case DX50Handler:
				sCodecName = _T("DivX 5.0 Codec");
				break;
			case DXT0Handler:
			case DXT1Handler:
			case DXT2Handler:
			case DXT3Handler:
			case DXT4Handler:
			case DXT5Handler:
			case DXT6Handler:
			case DXT7Handler:
			case DXT8Handler:
			case DXT9Handler:
			case DXTAHandler:
			case DXTBHandler:
			case DXTCHandler:
				sCodecName = _T("DirectX Texture Compression");
				break;
			case DXTDHandler:
			case DXTEHandler:
			case DXTFHandler:
			case DXTGHandler:
			case DXTHHandler:
			case DXTIHandler:
			case DXTJHandler:
			case DXTKHandler:
			case DXTLHandler:
			case DXTMHandler:
			case DXTNHandler:
			case DXTOHandler:
			case DXTPHandler:
			case DXTQHandler:
			case DXTRHandler:
			case DXTSHandler:
			case DXTTHandler:
			case DXTUHandler:
			case DXTVHandler:
			case DXTWHandler:
			case DXTXHandler:
			case DXTYHandler:
			case DXTZHandler:
				sCodecName = _T("DirectX Compressed Texture");
				break;
			case EKQ0Handler:
				sCodecName = _T("Elsa graphics card quick codec");
				break;
			case ELK0Handler:
				sCodecName = _T("Elsa graphics card codec");
				break;
			case EM2VHandler:
				sCodecName = _T("Etymonix MPEG-2 I-frame");
				break;
			case ESCPHandler:
				sCodecName = _T("Eidos Technologies Escape Codec");
				break;
			case ETV1Handler:
				sCodecName = _T("eTreppid Video Codec");
				break;
			case ETV2Handler:
				sCodecName = _T("eTreppid Video Codec");
				break;
			case ETVCHandler:
				sCodecName = _T("eTreppid Video Codec");
				break;
			case FIREHandler:
				sCodecName = _T("Fire");
				break;
			case FLICHandler:
				sCodecName = _T("Autodesk FLI/FLC Animation");
				break;
			case FLJPHandler:
				sCodecName = _T("Field Encoded Motion JPEG");	//(Targa emulation)
				break;
			case FRWAHandler:
				sCodecName = _T("Darim Vision Forward Motion JPEG");	//with Alpha-channel
				break;
			case FRWDHandler:
				sCodecName = _T("Darim Vision Forward Motion JPEG");
				break;
			case FRWTHandler:
				sCodecName = _T("Darim Vision Forward Motion JPEG");
				break;
			case FRWUHandler:
				sCodecName = _T("Darim Vision Forward Uncompressed");
				break;
			case FVF1Handler:
				sCodecName = _T("Iterated Systems Fractal Video Frame");
				break;
			case FVFWHandler:
				sCodecName = _T("ff MPEG-4 based on Xvid Codec");
				break;
			case FXT1Handler:
				bNotFound=TRUE;
				break;
			case GEPJHandler:
				sCodecName = _T("White Pine Motion JPEG Codec");	//(ex Paradigm Matrix)
				break;
			case GIFHandler:
				sCodecName = _T("Gif image");
				break;
			case GLZWHandler:
				sCodecName = _T("Motion LZW");	//by gabest@freemail.hu
				break;
			case GPEGHandler:
				sCodecName = _T("Motion JPEG");	//by gabest@freemail.hu (with floating point)
				break;
			case GPJMHandler:
				sCodecName = _T("Pinnacle ReelTime MJPEG Codec");
				break;
			case GREYHandler:
				sCodecName = _T("Apparently a duplicate of Y800");
				break;
			case GWLTHandler:
				sCodecName = _T("Microsoft Greyscale WLT DIB");
				break;
			case H260Handler:
				sCodecName = _T("Intel ITU H.260");
				break;
			case H261Handler:
				sCodecName = _T("Intel ITU H.261");
				break;
			case H262Handler:
				sCodecName = _T("Intel ITU H.262");
				break;
			case H263Handler:
				sCodecName = _T("Intel ITU H.263");
				break;
			case H264Handler:
				sCodecName = _T("Intel ITU H.264");
				break;
			case H265Handler:
				sCodecName = _T("Intel ITU H.265");
				break;
			case H266Handler:
				sCodecName = _T("Intel ITU H.266");
				break;
			case H267Handler:
				sCodecName = _T("Intel ITU H.267");
				break;
			case H268Handler:
				sCodecName = _T("Intel ITU H.268");
				break;
			case H269Handler:
				sCodecName = _T("Intel ITU H.263");	//for POTS-based videoconferencing
				break;
			case HFYUHandler:
				sCodecName = _T("Huffman Lossless Codec");	//YUV and RGB formats (with Alpha-channel)
				break;
			case HMCRHandler:
				sCodecName = _T("Rendition Motion Compensation Format");
				break;
			case HMRRHandler:
				sCodecName = _T("Rendition Motion Compensation Format");
				break;
			case I263Handler:
				sCodecName = _T("Intel ITU H.263");
				break;
			case I420Handler:
				sCodecName = _T("Intel Indeo 4 H.263");
				break;
			case IANHandler:
				sCodecName = _T("Indeo 4 (RDX) Codec");
				break;
			case ICLBHandler:
				sCodecName = _T("InSoft CellB Videoconferencing Codec");
				break;
			case IF09Handler:
				sCodecName = _T("Microsoft H.261");	//Intel intermediate YUV9
				break;
			case IGORHandler:
				sCodecName = _T("Power DVD");
				break;
			case IJPGHandler:
				sCodecName = _T("Intergraph JPEG");
				break;
			case ILVCHandler:
				sCodecName = _T("Intel Layered Video");
				break;
			case ILVRHandler:
				sCodecName = _T("ITU H.263+ Codec");
				break;
			case IMACHandler:
				sCodecName = _T("Intel hardware motion compensation");
				break;
			case IMC1Handler:
				sCodecName = _T("Uncompressed YVU12 4:2:0 Planar");	//As YV12, except the U and V planes each have the same stride as the Y plane
				break;
			case IMC2Handler:
				sCodecName = _T("Uncompressed YVU12 4:2:0 Planar");	//Similar to IMC1, except that the U and V lines are interleaved at half stride boundaries
				break;
			case IMC3Handler:
				sCodecName = _T("Uncompressed YUV12 4:2:0 Planar");	//As IMC1, except that U and V are swapped
				break;
			case IMC4Handler:
				sCodecName = _T("Uncompressed YUV12 4:2:0 Planar");	//As IMC2, except that U and V are swapped
				break;
			case IPDVHandler:
				sCodecName = _T("Giga AVI DV Codec");
				break;
			case IR21Handler:
				sCodecName = _T("Intel Indeo 2.1");
				break;
			case IRAWHandler:
				sCodecName = _T("Intel YUV Uncompressed");
				break;
			case ISMEHandler:
				sCodecName = _T("Intel's next-generation video codec");
				break;
			case IUYVHandler:
				sCodecName = _T("Interlaced UYVY Uncompressed");	//(line order 0,2,4 then 1,3,5 etc)
				break;
			case IV30Handler:
				sCodecName = _T("Intel Indeo Video 3");
				break;
			case IV31Handler:
				sCodecName = _T("Intel Indeo Video 3.1");
				break;
			case IV32Handler:
				sCodecName = _T("Intel Indeo Video 3.2");
				break;
			case IV33Handler:
				sCodecName = _T("Intel Indeo Video 3.3");
				break;
			case IV34Handler:
				sCodecName = _T("Intel Indeo Video 3.4");
				break;
			case IV35Handler:
				sCodecName = _T("Intel Indeo Video 3.5");
				break;
			case IV36Handler:
				sCodecName = _T("Intel Indeo Video 3.6");
				break;
			case IV37Handler:
				sCodecName = _T("Intel Indeo Video 3.7");
				break;
			case IV38Handler:
				sCodecName = _T("Intel Indeo Video 3.8");
				break;
			case IV39Handler:
				sCodecName = _T("Intel Indeo Video 3.9");
				break;
			case IV40Handler:
				sCodecName = _T("Intel Indeo Video 4.0");
				break;
			case IV41Handler:
				sCodecName = _T("Intel Indeo Video 4.1");
				break;
			case IV42Handler:
				sCodecName = _T("Intel Indeo Video 4.2");
				break;
			case IV43Handler:
				sCodecName = _T("Intel Indeo Video 4.3");
				break;
			case IV44Handler:
				sCodecName = _T("Intel Indeo Video 4.4");
				break;
			case IV45Handler:
				sCodecName = _T("Intel Indeo Video 4.5");
				break;
			case IV46Handler:
				sCodecName = _T("Intel Indeo Video 4.6");
				break;
			case IV47Handler:
				sCodecName = _T("Intel Indeo Video 4.7");
				break;
			case IV48Handler:
				sCodecName = _T("Intel Indeo Video 4.8");
				break;
			case IV49Handler:
				sCodecName = _T("Intel Indeo Video 4.9");
				break;
			case IV50Handler:
				sCodecName = _T("Intel Indeo Video 5.0 Wavelet");
				break;
			case IY41Handler:
				sCodecName = _T("Interlaced Conexant YUV 4:1:1 Raw");	//(line order 0,2,4,...,1,3,5...)
				break;
			case IYU1Handler:
				sCodecName = _T("12 bit format");	//used in mode 2 of the IEEE 1394 Digital Camera 1.04 spec
				break;
			case IYU2Handler:
				sCodecName = _T("24 bit format");	//used in mode 2 of the IEEE 1394 Digital Camera 1.04 spec
				break;
			case IYUVHandler:
				sCodecName = _T("Intel Indeo iYUV 4:2:0");
				break;
			case JBYRHandler:
				sCodecName = _T("Kensington Video Codec");
				break;
			case JFIFHandler:
				sCodecName = _T("Motion JPEG (FFmpeg)");
				break;
			case JPEGHandler:
				sCodecName = _T("Still Image JPEG DIB");
				break;
			case JPGLHandler:
				sCodecName = _T("DIVIO JPEG Light for WebCams (Pegasus Lossless JPEG)");
				break;
			case KMVCHandler:
				sCodecName = _T("Karl Morton Video Codec");
				break;
			case KPCDHandler:
				sCodecName = _T("Photo CD");
				break;
			case L261Handler:
				sCodecName = _T("Lead Technologies H.261");
				break;
			case L263Handler:
				sCodecName = _T("Lead Technologies H.263");
				break;
			case LCMWHandler:
				sCodecName = _T("Lead Technologies Motion CMW Codec");
				break;
			case LEADHandler:
				sCodecName = _T("Proprietary MCMP compression");
				break;
			case LGRYHandler:
				sCodecName = _T("Lead Technologies Grayscale Image");
				break;
			case LIA1Handler:
				sCodecName = _T("Liafail");
				break;
			case LJPGHandler:
				sCodecName = _T("Lossless JPEG compression");
				break;
			case LSV0Handler:
				bNotFound=TRUE;
				break;
			case LSVCHandler:
				sCodecName = _T("Infinop Lightning Strike Codec");	//Infinop Lightning Strike constant bit rate video codec
				break;
			case LSVMHandler:
				sCodecName = _T("Vianet Lighting Strike Vmail");	//(Streaming)
				break;
			case LSVWHandler:
				sCodecName = _T("Infinop Lightning Strike Codec");	//Infinop Lightning Strike multiple bit rate video codec
				break;
			case LZO1Handler:
				sCodecName = _T("LZO compressed");	//(lossless codec)
				break;
			case M101Handler:
				sCodecName = _T("Uncompressed field-based YUY2");
				break;
			case M261Handler:
				sCodecName = _T("Microsoft H.261");
				break;
			case M263Handler:
				sCodecName = _T("Microsoft H.263");
				break;
			case M4S2Handler:
				sCodecName = _T("Microsoft MPEG-4");	//(hacked MS MPEG-4)
				break;
			case MC12Handler:
				sCodecName = _T("ATI Motion Compensation Format");
				break;
			case MC24Handler:
				sCodecName = _T("MainConcept Motion JPEG Codec");
				break;
			case MCAMHandler:
				sCodecName = _T("ATI Motion Compensation Format");
				break;
			case MDVDHandler:
				sCodecName = _T("Alex MicroDVD Video");	//(hacked MS MPEG-4)
				break;
			case MJ2CHandler:
				sCodecName = _T("Morgan Multimedia JPEG2000 Compression");
				break;
			case MJP2Handler:
				sCodecName = _T("JPEG2000 Compression");
				break;
			case MJPAHandler:
				sCodecName = _T("Pinnacle Motion JPEG");	//with Alpha-channel
				break;
			case MJPBHandler:
				sCodecName = _T("Motion JPEG Codec");
				break;
			case MJPGHandler:
				sCodecName = _T("IBM Motion JPEG");	//including Huffman Tables
				break;
			case MMESHandler:
				sCodecName = _T("Matrox MPEG-2");
				break;
			case MMIFHandler:
				sCodecName = _T("Matrox MPEG-2 I-frame");
				break;
			case MP2AHandler:
				sCodecName = _T("Media Excel MPEG-2 Audio");
				break;
			case MP2THandler:
				sCodecName = _T("Media Excel MPEG-2 Transport Stream");
				break;
			case MP2VHandler:
				sCodecName = _T("Media Excel MPEG-2 Video");
				break;
			case MP41Handler:
				sCodecName = _T("Microsoft MPEG-4 V1");	//(enhansed H263)
				break;
			case MP42Handler:
				sCodecName = _T("Microsoft MPEG-4 V2");
				break;
			case MP43Handler:
				sCodecName = _T("Microsoft MPEG-4 V3");
				break;
			case MP4AHandler:
				sCodecName = _T("Media Excel MPEG-4 Audio");
				break;
			case MP4SHandler:
				sCodecName = _T("Microsoft MPEG-4");	//(Windows Media 7.0)
				break;
			case MP4THandler:
				sCodecName = _T("Media Excel MPEG-4 Transport Stream");
				break;
			case MP4VHandler:
				sCodecName = _T("Microsoft MPEG-4");	//(hacked MS MPEG-4)
				break;
			case MPEGHandler:
				sCodecName = _T("MPEG Video");	//Chromatic MPEG-1 Video I Frame
				break;
			case MPG1Handler:
				sCodecName = _T("MPEG-1");
				break;
			case MPG2Handler:
				sCodecName = _T("MPEG-2");
				break;
			case MPG3Handler:
				sCodecName = _T("Lowmotion DivX MPEG-4");	//Same as
				break;
			case MPG4Handler:
				sCodecName = _T("Microsoft MPEG-4 V1");
				break;
			case MPGIHandler:
				sCodecName = _T("Sigma Design MPEG-1 I-frame");
				break;
			case MPNGHandler:
				sCodecName = _T("Motion PNG Codec");
				break;
			case MRCAHandler:
				sCodecName = _T("FAST Multimedia MR Codec");
				break;
			case MRLEHandler:
				sCodecName = _T("Microsoft Run Length Encoding");
				break;
		//	case MSxxHandler:	//Reserved
			case MSS1Handler:
				sCodecName = _T("Windows Media Screen V7");
				break;
			case MSS2Handler:
				sCodecName = _T("Windows Media Video 9 Screen");
				break;
			case MSV1Handler:
				sCodecName = _T("Microsoft Video Codec V1");
				break;
			case MSVCHandler:
				sCodecName = _T("Microsoft Video 1");
				break;
			case MSZHHandler:
				sCodecName = _T("Lossless Codec");	//(ZIP compression)
				break;
			case MTGAHandler:
				sCodecName = _T("Motion TGA images");	//(24, 32 bpp)
				break;
			case MTX1Handler:
			case MTX2Handler:
			case MTX3Handler:
			case MTX4Handler:
			case MTX5Handler:
			case MTX6Handler:
			case MTX7Handler:
			case MTX8Handler:
			case MTX9Handler:
				sCodecName = _T("Matrox Motion-JPEG Codec");
				break;
			case MV10Handler:
			case MV11Handler:
			case MV12Handler:
				sCodecName = _T("Nokia MVC Video Codec");
				break;
			case MV99Handler:
				sCodecName = _T("Nokia MVC Video Codec");
				break;
			case MVC1Handler:
			case MVC2Handler:
				sCodecName = _T("Nokia MVC Video Codec");
				break;
			case MVC9Handler:
				sCodecName = _T("Nokia MVC Video Codec");
				break;
			case MWV1Handler:
				sCodecName = _T("Aware Motion Wavelets");
				break;
			case MYUVHandler:
				sCodecName = _T("MPEG YUV420");
				break;
			case NAVIHandler:
				sCodecName = _T("nAVI Video Codec");	//(hacked MS MPEG-4)
				break;
			case NHVUHandler:
				sCodecName = _T("NVidia Texture Format");	//(GEForce 3)
				break;
			case NT00Handler:
				sCodecName = _T("NewTek LigtWave HDTV YUV with Alpha-channel");
				break;
			case NTN1Handler:
				sCodecName = _T("Nogatech Video Compression 1");
				break;
			case NUV1Handler:
				sCodecName = _T("NuppelVideo");
				break;
			case NV12Handler:
				sCodecName = _T("Uncompressed YUV planar");	//8-bit Y plane followed by an interleaved U/V plane with 2x2 subsampling
				break;
			case NV21Handler:
				sCodecName = _T("Uncompressed YUV planar");	//As NV12 with U and V reversed in the interleaved plane
				break;
			case NVDSHandler:
				sCodecName = _T("nVidia Texture Format");
				break;
			case NVHSHandler:
				sCodecName = _T("NVidia Texture Format");	//(GEForce 3)
				break;
			case NVHUHandler:
				sCodecName = _T("NVidia Texture Format");
				break;
			case NVS0Handler:
			case NVS1Handler:
			case NVS2Handler:
			case NVS3Handler:
			case NVS4Handler:
			case NVS5Handler:
			case NVS6Handler:
			case NVS7Handler:
			case NVS8Handler:
			case NVS9Handler:
				sCodecName = _T("nVidia Texture Compression Format");
				break;
			case NVT0Handler:
			case NVT1Handler:
			case NVT2Handler:
			case NVT3Handler:
			case NVT4Handler:
			case NVT5Handler:
			case NVT6Handler:
			case NVT7Handler:
			case NVT8Handler:
			case NVT9Handler:
				sCodecName = _T("nVidia Texture Compression Format");
				break;
			case NY12Handler:
				sCodecName = _T("Nogatech YUV 12 format");
				break;
			case NYUVHandler:
				sCodecName = _T("Nogatech YUV 422 format");
				break;
			case PATHHandler:
				sCodecName = _T("Vector");
				break;
			case PCL2Handler:
				sCodecName = _T("Pinnacle RL Video Codec");
				break;
			case PCLEHandler:
				sCodecName = _T("Studio 400 Video Codec");
				break;
			case PDVCHandler:
				sCodecName = _T("Panasonic DV Codec");
				break;
			case PGVVHandler:
				sCodecName = _T("Radius Video Vision Telecast");	//(adaptive JPEG)
				break;
			case PHMOHandler:
				sCodecName = _T("IBM Photomotion");
				break;
			case PIM1Handler:
				sCodecName = _T("Pegasus Imaging MPEG-1 Codec");
				break;
			case PIM2Handler:
				sCodecName = _T("Pegasus Imaging Codec");
				break;
			case PIMJHandler:
				sCodecName = _T("Pegasus Imaging PICvideo Lossless JPEG");
				break;
			case PIXLHandler:
				sCodecName = _T("MiroVideo XL");	//(Motion JPEG)
				break;
			case PNGHandler:
				sCodecName = _T("Png image");
				break;
			case PNTGHandler:
				sCodecName = _T("Mac paint");
				break;
			case PVEZHandler:
				sCodecName = _T("Horizons Technology PowerEZ Codec");
				break;
			case PVMMHandler:
				sCodecName = _T("PacketVideo Corporation MPEG-4");
				break;
			case PVW2Handler:
				sCodecName = _T("Pegasus Imaging Wavelet 2000");
				break;
			case PXLTHandler:
				sCodecName = _T("Pixlet");
				break;
			case Q1_0Handler:
				sCodecName = _T("Q-Team QPEG 1.0");
				break;
			case QDRWHandler:
				sCodecName = _T("Quick draw");
				break;
			case QDGXHandler:
				sCodecName = _T("Quick draw GX");
				break;
			case QPEGHandler:
				sCodecName = _T("Q-Team QPEG 1.1");
				break;
			case QPEQHandler:
				sCodecName = _T("Q-Team QPEG 1.1");
				break;
			case R408Handler:
				sCodecName = _T("Uncompressed YCbCrA");	//Component Y'CbCrA 8-bit 4:4:4:4, rendering format. full range alpha, zero biased yuv (4444YpCbCrA8R)
				break;
			case RAWHandler:
				sCodecName = _T("Uncompressed");
				break;
			case RGBHandler:
				sCodecName = _T("Uncompressed");
				break;
			case RGB1Handler:
				sCodecName = _T("Uncompressed");	//RGB332 3:3:2
				break;
			case RGBAHandler:
				sCodecName = _T("Raw RGB with alpha");
				break;
			case RGBOHandler:
				sCodecName = _T("Uncompressed");	//RGB555 5:5:5
				break;
			case RGBPHandler:
				sCodecName = _T("Uncompressed");	//RGB565 5:6:5
				break;
			case RGBQHandler:
				sCodecName = _T("Uncompressed");	//RGB555X 5:5:5 BE
				break;
			case RGBRHandler:
				sCodecName = _T("Uncompressed");	//RGB565X 5:6:5 BE
				break;
			case RGBTHandler:
				sCodecName = _T("Uncompressed RGB");	//with transparency
				break;
			case RIPLHandler:
				sCodecName = _T("Water ripple");
				break;
			case RIVAHandler:
				sCodecName = _T("Swizzled texture format");
				break;
			case RPZAHandler:	//Video
				bNotFound=TRUE;
				break;
			case RLE4Handler:
				sCodecName = _T("Raw RGB image");	//Run length encoded 4bpp
				break;
			case RLE8Handler:
				sCodecName = _T("Raw RGB image");	//Run length encoded 8bpp
				break;
			case RLEHandler:
				sCodecName = _T("Raw RGB");	//with arbitrary sample packing within a pixel
				break;
			case RLNDHandler:
				bNotFound=TRUE;
				break;
			case RT21Handler:
				sCodecName = _T("Intel Indeo 2.1");
				break;
			case RVXHandler:
				sCodecName = _T("Intel RDX");
				break;
			case RMP4Handler:
				sCodecName = _T("REALmagic MPEG-4 Video Codec");	//(Sigma Design, built on XviD)
				break;
			case ROQVHandler:
				sCodecName = _T("Id RoQ File Video Decoder");
				break;
			case S263Handler:
				sCodecName = _T("Sorenson Vision H.263");
				break;
			case SMCHandler:
				sCodecName = _T("Graphics");
				break;
			case SMYKHandler:
				sCodecName = _T("CMYK");
				break;
			case SP53Handler:
			case SP54Handler:
			case SP55Handler:
			case SP56Handler:
			case SP57Handler:
			case SP58Handler:
				sCodecName = _T("SunPlus Aiptek MegaCam Codec");
				break;
			case SPIGHandler:
				sCodecName = _T("Radius Spigot");
				break;
			case SPLCHandler:
				sCodecName = _T("Splash Studios ACM Audio Codec");
				break;
			case SQZ2Handler:
				sCodecName = _T("Microsoft VXTreme Video Codec V2");
				break;
			case STVAHandler:
				sCodecName = _T("ST CMOS Imager Data");	//(Bayer)
				break;
			case STVBHandler:
				sCodecName = _T("ST CMOS Imager Data");	//(Nudged Bayer)
				break;
			case STVCHandler:
				sCodecName = _T("ST CMOS Imager Data");	//(Bunched)
				break;
			case STVXHandler:
				sCodecName = _T("ST CMOS Imager Data");	//(Extended)
				break;
			case STVYHandler:
				sCodecName = _T("ST CMOS Imager Data");	//(Extended with Correction Data)
				break;
			case SV10Handler:
				sCodecName = _T("Sorenson Media Video R1");
				break;
			case SVQ1Handler:
				sCodecName = _T("Sorenson Video");	//(Apple Quicktime 3)
				break;
			case SVQ3Handler:
				sCodecName = _T("Sorenson Video 3");	//(Apple Quicktime 5)
				break;
			case SWC1Handler:
				sCodecName = _T("MainConcept Motion JPEG Codec");
				break;
			case SYV9Handler:
				sCodecName = _T("Sorenson YUV9");
				break;
			case T420Handler:
				sCodecName = _T("Toshiba YUV 4:2:0");
				break;
			case TGAHandler:
				sCodecName = _T("Targa image");
				break;
			case TIFFHandler:
				sCodecName = _T("Tiff image");
				break;
			case TIM2Handler:
				sCodecName = _T("Pinnacle RAL DVI");
				break;
			case TLMSHandler:
				sCodecName = _T("TeraLogic Motion Infraframe Codec A");
				break;
			case TLSTHandler:
				sCodecName = _T("TeraLogic Motion Infraframe Codec B");
				break;
			case TM20Handler:
				sCodecName = _T("Duck TRUEMotion 2.0");
				break;
			case TM2AHandler:
				sCodecName = _T("Duck TRUEMotion Archiver 2.0");
				break;
			case TM2XHandler:
				sCodecName = _T("Duck TRUEMotion 2X");
				break;
			case TMICHandler:
				sCodecName = _T("TeraLogic Motion Intraframe Codec 2");
				break;
			case TMOTHandler:
				sCodecName = _T("TRUEMotion Video Compression");
				break;
			case TR20Handler:
				sCodecName = _T("Duck TRUEMotion RT 2.0");
				break;
			case TSCCHandler:
				sCodecName = _T("TechSmith Screen Capture Codec");
				break;
			case TV10Handler:
				sCodecName = _T("Tecomac Low-Bit Rate Codec");
				break;
			case TVJPHandler:
				sCodecName = _T("TRUEVision Field Encoded Motion JPEG");	//(Targa emulation)
				break;
			case TVMJHandler:
				sCodecName = _T("TRUEVision Field Encoded Motion JPEG");	//(Targa emulation)
				break;
			case TY0NHandler:
				sCodecName = _T("Trident Decompression Driver");
				break;
			case TY2CHandler:
				sCodecName = _T("Trident Decompression Driver");
				break;
			case TY2NHandler:
				sCodecName = _T("Trident Decompression Driver");
				break;
			case U263Handler:
				sCodecName = _T("UB Video StreamForce H.263");
				break;
			case UCODHandler:
				sCodecName = _T("ClearVideo");	//(fractal compression-based codec)
				break;
			case ULTIHandler:
				sCodecName = _T("IBM Corp. Ultimotion");
				break;
			case UMP4Handler:
				sCodecName = _T("UB Video MPEG 4");
				break;
			case UYNVHandler:
				sCodecName = _T("Uncompressed YUV 4:2:2");	//A direct copy of UYVY registered by nVidiato work around problems in some old codecs which did not like hardware that offered more than 2 UYVY surfaces
				break;
			case UYVPHandler:
				sCodecName = _T("Uncompressed YCbCr 4:2:2");	//YCbCr 4:2:2 extended precision 10-bits per component in U0Y0V0Y1 order
				break;
			case UYVYHandler:
				sCodecName = _T("Uncompressed YUV 4:2:2");	//(Y sample at every pixel, U and V sampled at every second pixel horizontally on each line)
				break;
			case V210Handler:
				sCodecName = _T("Uncompressed YCbCr");	//Component Y'CbCr 10-bit 4:2:2 (422YpCbCr10)
				break;
			case V216Handler:
				sCodecName = _T("Uncompressed YCbCr");	//Component Y'CbCr 10,12,14,16-bit 4:2:2 (422YpCbCr16)
				break;
			case V261Handler:
				sCodecName = _T("Lucent VX3000S Video Codec");
				break;
			case V308Handler:
				sCodecName = _T("Uncompressed YCbCr");	//Component Y'CbCr 8-bit 4:4:4 (444YpCbCr8)
				break;
			case V408Handler:
				sCodecName = _T("Uncompressed YCbCr");	//Component Y'CbCrA 8-bit 4:4:4:4 (4444YpCbCrA8)
				break;
			case V410Handler:
				sCodecName = _T("Uncompressed YCbCr");	//Component Y'CbCr 10-bit 4:4:4 (444YpCbCr10)
				break;
			case V422Handler:
				sCodecName = _T("Vitec Multimedia YUV 4:2:2");	//as for UYVY, but with different component ordering within the u_int32 macropixel
				break;
			case V655Handler:
				sCodecName = _T("Vitec Multimedia YUV 4:2:2");	//16 bit (6:5:5) format 
				break;
			case VCR1Handler:
				sCodecName = _T("ATI VCR 1.0");
				break;
			case VCR2Handler:
				sCodecName = _T("ATI VCR 2.0");	//(MPEG YV12)
				break;
			case VCR3Handler:
				sCodecName = _T("ATI VCR 3.0");
				break;
			case VCR4Handler:
				sCodecName = _T("ATI VCR 4.0");
				break;
			case VCR5Handler:
				sCodecName = _T("ATI VCR 5.0");
				break;
			case VCR6Handler:
				sCodecName = _T("ATI VCR 6.0");
				break;
			case VCR7Handler:
				sCodecName = _T("ATI VCR 7.0");
				break;
			case VCR8Handler:
				sCodecName = _T("ATI VCR 8.0");
				break;
			case VCR9Handler:
				sCodecName = _T("ATI VCR 9.0");
				break;
			case VDCTHandler:
				sCodecName = _T("Video Maker Pro DIB");
				break;
			case VDOMHandler:
				sCodecName = _T("VDOnet VDOWave");
				break;
			case VDOWHandler:
				sCodecName = _T("VDOLive (H.263)");
				break;
			case VDSTHandler:
				sCodecName = _T("VirtualDub remote frameclient ICM driver");
				break;
			case VDTZHandler:
				sCodecName = _T("Darim Vision VideoTizer YUV");
				break;
			case VGPXHandler:
				sCodecName = _T("Alaris VideoGramPixel Codec");
				break;
			case VIDSHandler:
				sCodecName = _T("Vitec Multimedia YUV 4:2:2 Codec");
				break;
			case VIFPHandler:
				sCodecName = _T("Virtual Frame API Codec");	//(VFAPI)
				break;
			case VIV1Handler:
				sCodecName = _T("Vivo H.263");
				break;
			case VIV2Handler:
				sCodecName = _T("Vivo H.263");
				break;
			case VIVOHandler:
				sCodecName = _T("Vivo H.263");
				break;
			case VIXLHandler:
				sCodecName = _T("MiroVideo XL");	//(Motion JPEG)
				break;
			case VLV1Handler:
				sCodecName = _T("VideoLogic Codec");
				break;
			case VP30Handler:
				sCodecName = _T("VP3@ Compressor");	//On2 (ex Duck TRUEMotion)
				break;
			case VP31Handler:
				sCodecName = _T("VP31@ Compressor");	//On2 (ex Duck TRUEMotion)
				break;
			case VP60Handler:
				sCodecName = _T("VP60@ Simple Profile");	//On2
				break;
			case VP61Handler:
				sCodecName = _T("VP61@ Advanced Profile");	//On2
				break;
			case VP62Handler:
				sCodecName = _T("VP62@ Heightened Sharpness Profile");	//On2
				break;
			case VQC1Handler:
			case VQC2Handler:
				bNotFound=TRUE;
				break;
			case VQJPHandler:
				sCodecName = _T("VQ630 dual-mode digital camera");
				break;
			case VQS4Handler:
				sCodecName = _T("VQ110 digital video camera");
				break;
			case VSSVHandler:
				sCodecName = _T("Vanguard Software Solutions Video Codec");
				break;
			case VTC1Handler:
			case VTC2Handler:
			case VTC3Handler:
			case VTC4Handler:
			case VTC5Handler:
			case VTC6Handler:
			case VTC7Handler:
			case VTC8Handler:
			case VTC9Handler:
				bNotFound=TRUE;
				break;
			case VTLPHandler:
				sCodecName = _T("Alaris VideoGramPixel Codec");
				break;
			case VUY2Handler:
				sCodecName = _T("Uncompressed YCbCr");	//Component Y'CbCr 8-bit 4:2:2 (422YpCbCr8)
				break;
			case VX1KHandler:
				sCodecName = _T("Lucent VX1000S Video Codec");
				break;
			case VX2KHandler:
				sCodecName = _T("Lucent VX2000S Video Codec");
				break;
			case VXSPHandler:
				sCodecName = _T("Lucent VX1000SP Video Codec");
				break;
			case VYU9Handler:
				sCodecName = _T("ATI Technologies YUV");
				break;
			case VYUYHandler:
				sCodecName = _T("ATI Packed YUV Data");
				break;
			case WBVCHandler:
				sCodecName = _T("Winbond W9960 Codec");
				break;
			case WHAMHandler:
				sCodecName = _T("Microsoft Video 1");
				break;
			case WINXHandler:
				sCodecName = _T("Winnov Software Compression");
				break;
			case WJPGHandler:
				sCodecName = _T("Winbond JPEG");	//(AverMedia USB devices)
				break;
			case WMS2Handler:
				bNotFound=TRUE;
				break;
			case WMV1Handler:
				sCodecName = _T("Windows Media Video V7");
				break;
			case WMV2Handler:
				sCodecName = _T("Windows Media Video V8");
				break;
			case WMV3Handler:
				sCodecName = _T("Windows Media Video 9");
				break;
			case WMVAHandler:
				sCodecName = _T("Windows Media Video 9 Advanced Profile");
				break;
			case WNV1Handler:
				sCodecName = _T("WinNow Videum Hardware Compression");
				break;
			case WNVAHandler:
				sCodecName = _T("Winnov hw compress");
				break;
			case WPY2Handler:
				bNotFound=TRUE;
				break;
			case WRAWHandler:
				sCodecName = _T("Windows Raw");
				break;
			case WRLEHandler:
				sCodecName = _T("BMP image");
				break;
			case WRPRHandler:
				sCodecName = _T("VideoTools VideoServer Client Codec");	//(wrapper for AviSynth)
				break;
			case WVC1Handler:
				sCodecName = _T("Windows Media Video 9 Advanced Profile");
				break;
			case WMVPHandler:
				sCodecName = _T("Windows Media Video 9 Image");
				break;
			case WVP1Handler:
				sCodecName = _T("Windows Media Video 9 Image v1");
				break;
			case WVP2Handler:
				sCodecName = _T("Windows Media Video 9 Image v2");
				break;
		//	case WSxxHandler:
			case WZCDHandler:
				sCodecName = _T("iScan");
				break;
			case WZDCHandler:
				sCodecName = _T("iSnap");
				break;
			case X263Handler:
				sCodecName = _T("Xirlink H.263");
				break;
			case X264Handler:
				sCodecName = _T("H.264 Video");
				break;
			case XLV0Handler:
				sCodecName = _T("NetXL Inc. XL Video Decoder");
				break;
			case XJPGHandler:
				sCodecName = _T("Xirlink JPEG");	//-like compressor
				break;
			case XMPGHandler:
				sCodecName = _T("XING MPEG (I frame only)");
				break;
			case XVIDHandler:
				sCodecName = _T("Xvid MPEG-4 Codec");
				break;
			case XVIXHandler:
				sCodecName = _T("Xvid MPEG-4 Codec");	//Based on
				break;
			case XWV0Handler:
			case XWV1Handler:
			case XWV2Handler:
				sCodecName = _T("XiWave Video Codec");
				break;
			case XWV3Handler:
				sCodecName = _T("XiWave Video Codec (Xi-3 Video)");
				break;
			case XWV4Handler:
			case XWV5Handler:
			case XWV6Handler:
			case XWV7Handler:
			case XWV8Handler:
			case XWV9Handler:
				sCodecName = _T("XiWave Video Codec");
				break;
			case XXANHandler:
				sCodecName = _T("Origin Video Codec");	//(used in Wing Commander 3 and 4)
				break;
			case Y211Handler:
				sCodecName = _T("Packed YUV format");	//with Y sampled at every second pixel across each line and U and V sampled at every fourth pixel
				break;
			case Y411Handler:
				sCodecName = _T("Uncompressed YUV 4:1:1 Packed");
				break;
			case Y41BHandler:
				sCodecName = _T("Uncompressed YUV 4:1:1 Planar");
				break;
			case Y41PHandler:
				sCodecName = _T("Conexant YUV 4:1:1 Raw");	//(ex Brooktree)
				break;
			case Y41THandler:
				sCodecName = _T("Conexant YUV 4:1:1 Raw");	//but the lsb of each Y component is used to signal pixel transparency
				break;
			case Y420Handler:
				sCodecName = _T("Uncompressed YUV 4:2:0");
				break;
			case Y422Handler:
				sCodecName = _T("Uncompressed YUV 4:2:2");	//Direct copy of UYVY as used by ADS Technologies Pyro WebCam firewire camera
				break;
			case Y42BHandler:
				sCodecName = _T("Uncompressed YUV 4:2:2 Planar");
				break;
			case Y42THandler:
				sCodecName = _T("Uncompressed YUV 4:2:2");	//Format as for UYVY, but the lsb of each Y component is used to signal pixel transparency
				break;
			case Y800Handler:
				sCodecName = _T("Simple grayscale video");
				break;
			case Y8Handler:
				sCodecName = _T("Simple grayscale video");
				break;
			case YC12Handler:
				sCodecName = _T("Intel YUV12 Codec");
				break;
			case YCCKHandler:
				sCodecName = _T("Uncompressed YCbCr video");	//with key data
				break;
			case YU92Handler:
				sCodecName = _T("YUV format");
				break;
			case YUNVHandler:
				sCodecName = _T("Uncompressed YUV 4:2:2");	//A direct copy of YUY2 registered by nVidia to work around problems in some old codecs which did not like hardware that offered more than 2 YUY2 surfaces
				break;
			case YUV2Handler:
				sCodecName = _T("Uncompressed YUV");	//Component
				break;
			case YUV8Handler:
				sCodecName = _T("Winnov Caviar YUV8");
				break;
			case YUV9Handler:
				sCodecName = _T("Intel YUV9");
				break;
			case YUVPHandler:
				sCodecName = _T("Uncompressed YCbCr 4:2:2");	//extended precision 10-bits per component in Y0U0Y1V0 order
				break;
			case YUY2Handler:
				sCodecName = _T("Uncompressed YUV 4:2:2");	//as for UYVY but with different component ordering within the u_int32 macropixel
				break;
			case YUVSHandler:
				sCodecName = _T("Uncompressed YUV");	//Component unsigned
				break;
			case YUVUHandler:
				sCodecName = _T("Uncompressed YUV");	//Component signed
				break;
			case YUYVHandler:
				sCodecName = _T("Canopus YUV format");
				break;
			case YV12Handler:
				sCodecName = _T("ATI YVU12 4:2:0 Planar");
				break;
			case YVU9Handler:
				sCodecName = _T("Brooktree YVU9 Raw");	//(YVU9 Planar)
				break;
			case YVYUHandler:
				sCodecName = _T("Uncompressed YUV 4:2:2");	//as for UYVY but with different component ordering within the u_int32 macropixel
				break;
			case ZLIBHandler:
				sCodecName = _T("Lossless Codec");	//(ZIP compression)
				break;
			case ZPEGHandler:
				sCodecName = _T("Metheus Video Zipper");
				break;
			case ZPG1Handler:
			case ZPG2Handler:
			case ZPG3Handler:
			case ZPG4Handler:
				bNotFound=TRUE;
				break;
			case ZYGOHandler:
				sCodecName = _T("ZyGo Video Codec");
				break;
	//////////////////////////////////////////////////////////////
			case RV10Handler:
				sCodecName = _T("Real Video 1.0");
				break;
			case RV20Handler:
				sCodecName = _T("Real Video 2.0");
				break;
			case RV30Handler:
				sCodecName = _T("Real Video 3.0");
				break;
			case RV40Handler:
				sCodecName = _T("Real Video 4.0");
				break;
			case FPS1Handler:
				sCodecName = _T("Fraps");
				break;
			default:
				bNotFound=TRUE;
				break;
			}

			if (bNotFound)
			{
				DWORD	fccHandler2 = LOWERCASEFOURCC(fccHandler);
				bNotFound = (fccHandler==fccHandler2);
				fccHandler = fccHandler2;
			} 
			else
				break;
		}

		return sCodecName;
	}


};


// CAVSVideoFormat
[coclass, uuid("6FA90777-E844-4B62-845D-3849168BF8D4"), threading(apartment), vi_progid("AVSMediaFormatSettings3.AVSVideoFormat"), progid("AVSMediaFormatSettings3.AVSVideoForma.1"), version(1.0)]
class ATL_NO_VTABLE CAVSVideoFormat : public CAVSVideoFormatImpl<CAVSVideoFormat, IAVSVideoFormat>
{
};

