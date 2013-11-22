// AVSMPEG4VideoFormat.h : Declaration of the CAVSMPEG4VideoFormat

#pragma once
#include "resource.h"       // main symbols
#include <atlcoll.h>
#include "ATLDefine.h"
#include "AVSVideoFormat.h"

class CAVSMPEG4VideoFormat;
// IAVSMPEG4VideoFormat
[ object, uuid("BD0D4FF6-F690-4184-B37A-48D383D13D9D"), dual, pointer_default(unique)]
__interface IAVSMPEG4VideoFormat : IAVSVideoFormat
{
	[id(105000 + 1), propget] HRESULT CPUFlags([out, retval] long* pVal);
	[id(105000 + 1), propput] HRESULT CPUFlags([in] long newVal);
	[id(105000 + 2), propget] HRESULT ProfileLevel([out, retval] long* pVal);
	[id(105000 + 2), propput] HRESULT ProfileLevel([in] long newVal);
		   	
	[id(105000 + 3), propget] HRESULT Plugin([out, retval] long* pVal);
	[id(105000 + 3), propput] HRESULT Plugin([in] long newVal);
	[id(105000 + 4), propget] HRESULT BitrateEnabled([out, retval] VARIANT_BOOL* pVal);
	[id(105000 + 4), propput] HRESULT BitrateEnabled([in] VARIANT_BOOL newVal);
	[id(105000 + 5), propget] HRESULT DesiredQuant([out, retval] long* pVal);
	[id(105000 + 5), propput] HRESULT DesiredQuant([in] long newVal);
	[id(105000 + 6), propget] HRESULT DesiredSize([out, retval] long* pVal);
	[id(105000 + 6), propput] HRESULT DesiredSize([in] long newVal);
		   	
	[id(105000 + 7), propget] HRESULT QuantizationType([out, retval] long* pVal);
	[id(105000 + 7), propput] HRESULT QuantizationType([in] long newVal);
	[id(105000 + 8), propget] HRESULT LumimaskingEnabled([out, retval] VARIANT_BOOL* pVal);
	[id(105000 + 8), propput] HRESULT LumimaskingEnabled([in] VARIANT_BOOL newVal);
	[id(105000 + 9), propget] HRESULT InterlacingEnabled([out, retval] VARIANT_BOOL* pVal);
	[id(105000 + 9), propput] HRESULT InterlacingEnabled([in] VARIANT_BOOL newVal);
	[id(105000 + 10), propget] HRESULT TffEnabled([out, retval] VARIANT_BOOL* pVal);
	[id(105000 + 10), propput] HRESULT TffEnabled([in] VARIANT_BOOL newVal);
	[id(105000 + 11), propget] HRESULT QuarterPixel([out, retval] VARIANT_BOOL* pVal);
	[id(105000 + 11), propput] HRESULT QuarterPixel([in] VARIANT_BOOL newVal);
	[id(105000 + 12), propget] HRESULT GlobalMotionCompensation([out, retval] VARIANT_BOOL* pVal);
	[id(105000 + 12), propput] HRESULT GlobalMotionCompensation([in] VARIANT_BOOL newVal);
		   	
	[id(105000 + 13), propget] HRESULT UseBVOP([out, retval] VARIANT_BOOL* pVal);
	[id(105000 + 13), propput] HRESULT UseBVOP([in] VARIANT_BOOL newVal);
	[id(105000 + 14), propget] HRESULT BVOPMaxFrames([out, retval] long* pVal);
	[id(105000 + 14), propput] HRESULT BVOPMaxFrames([in] long newVal);
	[id(105000 + 15), propget] HRESULT BVOPQuantRatio([out, retval] long* pVal);
	[id(105000 + 15), propput] HRESULT BVOPQuantRatio([in] long newVal);
	[id(105000 + 16), propget] HRESULT BVOPQuantOffset([out, retval] long* pVal);
	[id(105000 + 16), propput] HRESULT BVOPQuantOffset([in] long newVal);
	[id(105000 + 17), propget] HRESULT BVOPGlobalPacket([out, retval] VARIANT_BOOL* pVal);
	[id(105000 + 17), propput] HRESULT BVOPGlobalPacket([in] VARIANT_BOOL newVal);
		   	
	[id(105000 + 18), propget] HRESULT PixelAspectRatioEnabled([out, retval] VARIANT_BOOL* pVal);
	[id(105000 + 18), propput] HRESULT PixelAspectRatioEnabled([in] VARIANT_BOOL newVal);
	[id(105000 + 19), propget] HRESULT DisplayAspectRatio([out, retval] long* pVal);
	[id(105000 + 19), propput] HRESULT DisplayAspectRatio([in] long newVal);
	[id(105000 + 20), propget] HRESULT PARX([out, retval] long* pVal);
	[id(105000 + 20), propput] HRESULT PARX([in] long newVal);
	[id(105000 + 21), propget] HRESULT PARY([out, retval] long* pVal);
	[id(105000 + 21), propput] HRESULT PARY([in] long newVal);
	[id(105000 + 22), propget] HRESULT ARX([out, retval] long* pVal);
	[id(105000 + 22), propput] HRESULT ARX([in] long newVal);
	[id(105000 + 23), propget] HRESULT ARY([out, retval] long* pVal);
	[id(105000 + 23), propput] HRESULT ARY([in] long newVal);

	[id(105000 + 24), propget] HRESULT ZonesCount([out, retval] long* pVal);
	[id(105000 + 24), propput] HRESULT ZonesCount([in] long newVal);
	[id(105000 + 26), propget] HRESULT ZoneFrame([in] long Index, [out, retval] long* pVal);
	[id(105000 + 26), propput] HRESULT ZoneFrame([in] long Index, [in] long newVal);
	[id(105000 + 27), propget] HRESULT ZoneMode([in] long Index, [out, retval] long* pVal);
	[id(105000 + 27), propput] HRESULT ZoneMode([in] long Index, [in] long newVal);
	[id(105000 + 28), propget] HRESULT ZoneWeight([in] long Index, [out, retval] long* pVal);
	[id(105000 + 28), propput] HRESULT ZoneWeight([in] long Index, [in] long newVal);
	[id(105000 + 29), propget] HRESULT ZoneQuant([in] long Index, [out, retval] long* pVal);
	[id(105000 + 29), propput] HRESULT ZoneQuant([in] long Index, [in] long newVal);
	[id(105000 + 30), propget] HRESULT ZoneFrameType([in] long Index, [out, retval] long* pVal);
	[id(105000 + 30), propput] HRESULT ZoneFrameType([in] long Index, [in] long newVal);
	[id(105000 + 31), propget] HRESULT ZoneGrayscaleEnabled([in] long Index, [out, retval] VARIANT_BOOL* pVal);
	[id(105000 + 31), propput] HRESULT ZoneGrayscaleEnabled([in] long Index, [in] VARIANT_BOOL newVal);
	[id(105000 + 32), propget] HRESULT ZoneChromaOptimizerEnabled([in] long Index, [out, retval] VARIANT_BOOL* pVal);
	[id(105000 + 32), propput] HRESULT ZoneChromaOptimizerEnabled([in] long Index, [in] VARIANT_BOOL newVal);
	[id(105000 + 33), propget] HRESULT ZoneBVOPThreshold([in] long Index, [out, retval] long* pVal);
	[id(105000 + 33), propput] HRESULT ZoneBVOPThreshold([in] long Index, [in] long newVal);
	[id(105000 + 34), propget] HRESULT ZoneCartoonModeEnabled([in] long Index, [out, retval] VARIANT_BOOL* pVal);
	[id(105000 + 34), propput] HRESULT ZoneCartoonModeEnabled([in] long Index, [in] VARIANT_BOOL newVal);
				
	[id(105000 + 35), propget] HRESULT SingleRCDelayFactor([out, retval] long* pVal);
	[id(105000 + 35), propput] HRESULT SingleRCDelayFactor([in] long newVal);
	[id(105000 + 36), propget] HRESULT SingleRCAveragingPeriod([out, retval] long* pVal);
	[id(105000 + 36), propput] HRESULT SingleRCAveragingPeriod([in] long newVal);
	[id(105000 + 37), propget] HRESULT SingleRCBuffer([out, retval] long* pVal);
	[id(105000 + 37), propput] HRESULT SingleRCBuffer([in] long newVal);
				
	[id(105000 + 38), propget] HRESULT Pass1FileName([out, retval] BSTR* pVal);
	[id(105000 + 38), propput] HRESULT Pass1FileName([in] BSTR newVal);
	[id(105000 + 39), propget] HRESULT Full1Pass([out, retval] VARIANT_BOOL* pVal);
	[id(105000 + 39), propput] HRESULT Full1Pass([in] VARIANT_BOOL newVal);
				
	[id(105000 + 40), propget] HRESULT Pass2FileName([out, retval] BSTR* pVal);
	[id(105000 + 40), propput] HRESULT Pass2FileName([in] BSTR newVal);
	[id(105000 + 41), propget] HRESULT Pass2KeyframeBoost([out, retval] long* pVal);
	[id(105000 + 41), propput] HRESULT Pass2KeyframeBoost([in] long newVal);
	[id(105000 + 42), propget] HRESULT Pass2KfReduction([out, retval] long* pVal);
	[id(105000 + 42), propput] HRESULT Pass2KfReduction([in] long newVal);
	[id(105000 + 43), propget] HRESULT Pass2KfThreshold([out, retval] long* pVal);
	[id(105000 + 43), propput] HRESULT Pass2KfThreshold([in] long newVal);
	[id(105000 + 44), propget] HRESULT Pass2CurveCompressionHigh([out, retval] long* pVal);
	[id(105000 + 44), propput] HRESULT Pass2CurveCompressionHigh([in] long newVal);
	[id(105000 + 45), propget] HRESULT Pass2CurveCompressionLow([out, retval] long* pVal);
	[id(105000 + 45), propput] HRESULT Pass2CurveCompressionLow([in] long newVal);
	[id(105000 + 46), propget] HRESULT Pass2OverflowControlStrength([out, retval] long* pVal);
	[id(105000 + 46), propput] HRESULT Pass2OverflowControlStrength([in] long newVal);
	[id(105000 + 47), propget] HRESULT Pass2MaxOverflowImprovement([out, retval] long* pVal);
	[id(105000 + 47), propput] HRESULT Pass2MaxOverflowImprovement([in] long newVal);
	[id(105000 + 48), propget] HRESULT Pass2MaxOverflowDegradation([out, retval] long* pVal);
	[id(105000 + 48), propput] HRESULT Pass2MaxOverflowDegradation([in] long newVal);
	[id(105000 + 49), propget] HRESULT Pass2ContainerFrameOverhead([out, retval] long* pVal);
	[id(105000 + 49), propput] HRESULT Pass2ContainerFrameOverhead([in] long newVal);
				
	[id(105000 + 50), propget] HRESULT MotionSearchType([out, retval] long* pVal);
	[id(105000 + 50), propput] HRESULT MotionSearchType([in] long newVal);
	[id(105000 + 51), propget] HRESULT VHQMode([out, retval] long* pVal);
	[id(105000 + 51), propput] HRESULT VHQMode([in] long newVal);
	[id(105000 + 52), propget] HRESULT VHQBMode([out, retval] VARIANT_BOOL* pVal);
	[id(105000 + 52), propput] HRESULT VHQBMode([in] VARIANT_BOOL newVal);
	[id(105000 + 53), propget] HRESULT ChromaMotion([out, retval] VARIANT_BOOL* pVal);
	[id(105000 + 53), propput] HRESULT ChromaMotion([in] VARIANT_BOOL newVal);
	[id(105000 + 54), propget] HRESULT TurboMode([out, retval] VARIANT_BOOL* pVal);
	[id(105000 + 54), propput] HRESULT TurboMode([in] VARIANT_BOOL newVal);
	[id(105000 + 55), propget] HRESULT MaxKeyInterval([out, retval] long* pVal);
	[id(105000 + 55), propput] HRESULT MaxKeyInterval([in] long newVal);
	[id(105000 + 56), propget] HRESULT FrameDropRatio([out, retval] long* pVal);
	[id(105000 + 56), propput] HRESULT FrameDropRatio([in] long newVal);
				
	[id(105000 + 57), propget] HRESULT MinIQuantizer([out, retval] long* pVal);
	[id(105000 + 57), propput] HRESULT MinIQuantizer([in] long newVal);
	[id(105000 + 58), propget] HRESULT MaxIQuantizer([out, retval] long* pVal);
	[id(105000 + 58), propput] HRESULT MaxIQuantizer([in] long newVal);
	[id(105000 + 59), propget] HRESULT MinPQuantizer([out, retval] long* pVal);
	[id(105000 + 59), propput] HRESULT MinPQuantizer([in] long newVal);
	[id(105000 + 60), propget] HRESULT MaxPQuantizer([out, retval] long* pVal);
	[id(105000 + 60), propput] HRESULT MaxPQuantizer([in] long newVal);
	[id(105000 + 61), propget] HRESULT MinBQuantizer([out, retval] long* pVal);
	[id(105000 + 61), propput] HRESULT MinBQuantizer([in] long newVal);
	[id(105000 + 62), propget] HRESULT MaxBQuantizer([out, retval] long* pVal);
	[id(105000 + 62), propput] HRESULT MaxBQuantizer([in] long newVal);
	[id(105000 + 63), propget] HRESULT TrellisQuantization([out, retval] VARIANT_BOOL* pVal);
	[id(105000 + 63), propput] HRESULT TrellisQuantization([in] VARIANT_BOOL newVal);
				
	[id(105000 + 64), propget] HRESULT DeblockYEnabled([out, retval] VARIANT_BOOL* pVal);
	[id(105000 + 64), propput] HRESULT DeblockYEnabled([in] VARIANT_BOOL newVal);
	[id(105000 + 65), propget] HRESULT DeblockUVEnabled([out, retval] VARIANT_BOOL* pVal);
	[id(105000 + 65), propput] HRESULT DeblockUVEnabled([in] VARIANT_BOOL newVal);
	[id(105000 + 66), propget] HRESULT DeringYEnabled([out, retval] VARIANT_BOOL* pVal);
	[id(105000 + 66), propput] HRESULT DeringYEnabled([in] VARIANT_BOOL newVal);
	[id(105000 + 67), propget] HRESULT DeringUVEnabled([out, retval] VARIANT_BOOL* pVal);
	[id(105000 + 67), propput] HRESULT DeringUVEnabled([in] VARIANT_BOOL newVal);
	[id(105000 + 68), propget] HRESULT FilmEffectEnabled([out, retval] VARIANT_BOOL* pVal);
	[id(105000 + 68), propput] HRESULT FilmEffectEnabled([in] VARIANT_BOOL newVal);
	[id(105000 + 69), propget] HRESULT Brightness([out, retval] long* pVal);
	[id(105000 + 69), propput] HRESULT Brightness([in] long newVal);
};

#define RC_ZONE_WEIGHT		0
#define RC_ZONE_QUANT		1

#define XVID_TYPE_AUTO		0 /* encoder: automatically determine coding type */
#define XVID_TYPE_IVOP		1 /* intra frame */
#define XVID_TYPE_PVOP		2 /* predicted frame */
#define XVID_TYPE_BVOP		3 /* bidirectionally encoded */
#define XVID_TYPE_SVOP		4 /* predicted+sprite frame */

struct Zone
{
	Zone()
	{
		SetDefaultProperties();
	}
	void SetDefaultProperties()
	{
		m_lFrame = 0;
		m_lMode	 = RC_ZONE_WEIGHT;
		m_lWeight = 100;
		m_lQuant = 400;
		m_lFrameType = XVID_TYPE_AUTO;
		m_bGrayscaleEnabled = FALSE;
		m_bChromaOptimizerEnabled = FALSE;
		m_lBVOPThreshold = 0; 
		m_bCartoonModeEnabled = FALSE;
	}
	long m_lFrame;
	long m_lMode;
	long m_lWeight;
	long m_lQuant;
	long m_lFrameType;
	VARIANT_BOOL m_bGrayscaleEnabled;
	VARIANT_BOOL m_bChromaOptimizerEnabled;
	long m_lBVOPThreshold;
	VARIANT_BOOL m_bCartoonModeEnabled;
};
template <class CoT, class T>
class CAVSMPEG4VideoFormatImpl : public CAVSVideoFormatImpl<CoT, T>
{
public:
	CAVSMPEG4VideoFormatImpl()
	{
		m_lMediaType = MEDIAFORMAT_MPEG4;
		SetDefaultProperties();
	}
	STDMETHOD(CreateDuplicate)(IAVSMediaFormat** pMediaFormat)
	{
		if (NULL==pMediaFormat)
			return S_OK;

		CAVSVideoFormatImpl<CoT, T>::CreateDuplicate(pMediaFormat);
		if (NULL==(*pMediaFormat))
			return S_OK;
		IAVSMPEG4VideoFormat* pAVSMPEG4Format=NULL;
		(*pMediaFormat)->QueryInterface(__uuidof(IAVSMPEG4VideoFormat), (void**)&pAVSMPEG4Format);
		if (NULL==pAVSMPEG4Format)
		{
			(*pMediaFormat)->Release();
			*pMediaFormat = NULL;
			return S_OK;
		}

		DublicateZones(pAVSMPEG4Format);
		pAVSMPEG4Format->put_CPUFlags(m_lCPUFlags);
		pAVSMPEG4Format->put_ProfileLevel(m_lProfileLevel);
		
		pAVSMPEG4Format->put_Plugin(m_lPlugin);
		pAVSMPEG4Format->put_BitrateEnabled(m_bBitrateEnabled ? VARIANT_TRUE : VARIANT_FALSE);
		pAVSMPEG4Format->put_DesiredQuant(m_lDesiredQuant);
		pAVSMPEG4Format->put_DesiredSize(m_lDesiredSize);

		pAVSMPEG4Format->put_QuantizationType(m_lQuantizationType);
		pAVSMPEG4Format->put_LumimaskingEnabled(m_bLumimaskingEnabled ? VARIANT_TRUE : VARIANT_FALSE);
		pAVSMPEG4Format->put_InterlacingEnabled(m_bInterlacingEnabled ? VARIANT_TRUE : VARIANT_FALSE);
		pAVSMPEG4Format->put_TffEnabled(m_bTffEnabled ? VARIANT_TRUE : VARIANT_FALSE);
		pAVSMPEG4Format->put_QuarterPixel(m_bQuarterPixel ? VARIANT_TRUE : VARIANT_FALSE);
		pAVSMPEG4Format->put_GlobalMotionCompensation(m_bGlobalMotionCompensation ? VARIANT_TRUE : VARIANT_FALSE);

		pAVSMPEG4Format->put_UseBVOP(m_bUseBVOP ? VARIANT_TRUE : VARIANT_FALSE);
		pAVSMPEG4Format->put_BVOPMaxFrames(m_lBVOPMaxFrames);
		pAVSMPEG4Format->put_BVOPQuantRatio(m_lBVOPQuantRatio);
		pAVSMPEG4Format->put_BVOPQuantOffset(m_lBVOPQuantOffset);
		pAVSMPEG4Format->put_BVOPGlobalPacket(m_bBVOPGlobalPacket ? VARIANT_TRUE : VARIANT_FALSE);

		pAVSMPEG4Format->put_PixelAspectRatioEnabled(m_bPixelAspectRatioEnabled ? VARIANT_TRUE : VARIANT_FALSE);
		pAVSMPEG4Format->put_DisplayAspectRatio(m_lDisplayAspectRatio);
		pAVSMPEG4Format->put_PARX(m_lPARX);
		pAVSMPEG4Format->put_PARY(m_lPARY);
		pAVSMPEG4Format->put_ARX(m_lARX);
		pAVSMPEG4Format->put_ARY(m_lARY);

		pAVSMPEG4Format->put_SingleRCDelayFactor(m_lSingleRCDelayFactor);
		pAVSMPEG4Format->put_SingleRCAveragingPeriod(m_lSingleRCAveragingPeriod);
		pAVSMPEG4Format->put_SingleRCBuffer(m_lSingleRCBuffer);

		BSTR bsTemp = m_sPass1FileName.AllocSysString();
		pAVSMPEG4Format->put_Pass1FileName(bsTemp);
		SysFreeString(bsTemp);

		pAVSMPEG4Format->put_Full1Pass(m_bFull1Pass ? VARIANT_TRUE : VARIANT_FALSE);

		bsTemp = m_sPass1FileName.AllocSysString();
		pAVSMPEG4Format->put_Pass2FileName(bsTemp);
		SysFreeString(bsTemp);

		pAVSMPEG4Format->put_Pass2KeyframeBoost(m_lPass2KeyframeBoost);
		pAVSMPEG4Format->put_Pass2KfReduction(m_lPass2KfReduction);
		pAVSMPEG4Format->put_Pass2KfThreshold(m_lPass2KfThreshold);
		pAVSMPEG4Format->put_Pass2CurveCompressionHigh(m_lPass2CurveCompressionHigh);
		pAVSMPEG4Format->put_Pass2CurveCompressionLow(m_lPass2CurveCompressionLow);
		pAVSMPEG4Format->put_Pass2OverflowControlStrength(m_lPass2OverflowControlStrength);
		pAVSMPEG4Format->put_Pass2MaxOverflowImprovement(m_lPass2MaxOverflowImprovement);
		pAVSMPEG4Format->put_Pass2MaxOverflowDegradation(m_lPass2MaxOverflowDegradation);
		pAVSMPEG4Format->put_Pass2ContainerFrameOverhead(m_lPass2ContainerFrameOverhead);

		pAVSMPEG4Format->put_MotionSearchType(m_lMotionSearchType);
		pAVSMPEG4Format->put_VHQMode(m_lVHQMode);
		pAVSMPEG4Format->put_VHQBMode(m_bVHQBMode ? VARIANT_TRUE : VARIANT_FALSE);
		pAVSMPEG4Format->put_ChromaMotion(m_bChromaMotion ? VARIANT_TRUE : VARIANT_FALSE);
		pAVSMPEG4Format->put_TurboMode(m_bTurboMode ? VARIANT_TRUE : VARIANT_FALSE);
		pAVSMPEG4Format->put_MaxKeyInterval(m_lMaxKeyInterval);
		pAVSMPEG4Format->put_FrameDropRatio(m_lFrameDropRatio);

		pAVSMPEG4Format->put_MinIQuantizer(m_lMinIQuantizer);
		pAVSMPEG4Format->put_MaxIQuantizer(m_lMaxIQuantizer);
		pAVSMPEG4Format->put_MinPQuantizer(m_lMinPQuantizer);
		pAVSMPEG4Format->put_MaxPQuantizer(m_lMaxPQuantizer);
		pAVSMPEG4Format->put_MinBQuantizer(m_lMinBQuantizer);
		pAVSMPEG4Format->put_MaxBQuantizer(m_lMaxBQuantizer);
		pAVSMPEG4Format->put_TrellisQuantization(m_bTrellisQuantization ? VARIANT_TRUE : VARIANT_FALSE);

		pAVSMPEG4Format->put_DeblockYEnabled(m_bDeblockYEnabled ? VARIANT_TRUE : VARIANT_FALSE);
		pAVSMPEG4Format->put_DeblockUVEnabled(m_bDeblockUVEnabled ? VARIANT_TRUE : VARIANT_FALSE);
		pAVSMPEG4Format->put_DeringYEnabled(m_bDeringYEnabled ? VARIANT_TRUE : VARIANT_FALSE);
		pAVSMPEG4Format->put_DeringUVEnabled(m_bDeringUVEnabled ? VARIANT_TRUE : VARIANT_FALSE);
		pAVSMPEG4Format->put_FilmEffectEnabled(m_bFilmEffectEnabled ? VARIANT_TRUE : VARIANT_FALSE);
		pAVSMPEG4Format->put_Brightness(m_lBrightness);

		pAVSMPEG4Format->Release();

		return S_OK;
	}
	STDMETHOD(SetDefaultProperties)(void)
	{
		CAVSVideoFormatImpl<CoT, T>::SetDefaultProperties();
		m_ulfccHandler = XVIDHandler;

		m_arZones.SetCount(1);
		
		m_lCPUFlags	= 0;
		m_lProfileLevel = 13;	

		m_lPlugin = 1;
		m_bBitrateEnabled = VARIANT_TRUE;
		m_lDesiredQuant = MPEG4_DEFAULT_QUANT;
		m_lDesiredSize = 570000;

		m_lQuantizationType = 0;
		m_bLumimaskingEnabled		= FALSE;
		m_bInterlacingEnabled		= FALSE;
		m_bTffEnabled				= FALSE;
		m_bQuarterPixel				= FALSE;
		m_bGlobalMotionCompensation	= FALSE;

		m_bUseBVOP = FALSE;
		m_lBVOPMaxFrames = 2;
		m_lBVOPQuantRatio = 150;	
		m_lBVOPQuantOffset = 100;
		m_bBVOPGlobalPacket = VARIANT_TRUE;

		m_bPixelAspectRatioEnabled = FALSE;
		m_lDisplayAspectRatio = 0;
		m_lPARX = 1;
		m_lPARY = 1;
		m_lARX = 4;
		m_lARY = 3;

		m_lSingleRCDelayFactor = 16;
		m_lSingleRCAveragingPeriod = 100;
		m_lSingleRCBuffer = 100;

		m_sPass1FileName = "c:/xvid.pass";
		m_bFull1Pass = FALSE;
		m_sPass2FileName = "c:/xvid.pass";

		m_lPass2KeyframeBoost = 10;
		m_lPass2KfReduction = 20;
		m_lPass2KfThreshold = 1;
		m_lPass2CurveCompressionHigh = 0;
		m_lPass2CurveCompressionLow = 0;
		m_lPass2OverflowControlStrength = 5;
		m_lPass2MaxOverflowImprovement = 5;
		m_lPass2MaxOverflowDegradation = 5;
		m_lPass2ContainerFrameOverhead = 24;

		m_lMotionSearchType = 6;
		m_lVHQMode = 1;
		m_bVHQBMode = FALSE;
		m_bChromaMotion = VARIANT_TRUE;
		m_bTurboMode = FALSE;
		m_lMaxKeyInterval = 300;
		m_lFrameDropRatio = 0;

		m_lMinIQuantizer = 1;
		m_lMaxIQuantizer = 31;
		m_lMinPQuantizer = 1;
		m_lMaxPQuantizer = 31;
		m_lMinBQuantizer = 1;
		m_lMaxBQuantizer = 31;
		m_bTrellisQuantization = VARIANT_TRUE;

		m_bDeblockYEnabled = FALSE;
		m_bDeblockUVEnabled = FALSE;
		m_bDeringYEnabled = FALSE;
		m_bDeringUVEnabled = FALSE;
		m_bFilmEffectEnabled = FALSE;		
		m_lBrightness = 0;
		return S_OK;
	}
	STDMETHOD(SetPropertiesByFormat)(IAVSMediaFormat* pMediaFormat)
	{
		CAVSVideoFormatImpl<CoT, T>::SetPropertiesByFormat(pMediaFormat);
		if (pMediaFormat!=NULL)
		{
			IAVSMPEG4VideoFormat*	pAVSMPEG4Format=NULL;
			pMediaFormat->QueryInterface(__uuidof(IAVSMPEG4VideoFormat), (void**)&pAVSMPEG4Format);
			if (pAVSMPEG4Format!=NULL)
			{
				m_arZones.RemoveAll();

				LONG	ZonesCount;
				pAVSMPEG4Format->get_ZonesCount(&ZonesCount);
				for (int i=0; i<ZonesCount; i++)
				{
					Zone	temp;
					pAVSMPEG4Format->get_ZonesCount(&temp.m_lFrame);
					pAVSMPEG4Format->get_ZoneMode(i, &temp.m_lMode);
					pAVSMPEG4Format->get_ZoneWeight(i, &temp.m_lWeight);
					pAVSMPEG4Format->get_ZoneQuant(i, &temp.m_lQuant);
					pAVSMPEG4Format->get_ZoneFrameType(i, &temp.m_lFrameType);
					pAVSMPEG4Format->get_ZoneGrayscaleEnabled(i, &temp.m_bGrayscaleEnabled);
					pAVSMPEG4Format->get_ZoneChromaOptimizerEnabled(i, &temp.m_bChromaOptimizerEnabled);
					pAVSMPEG4Format->get_ZoneBVOPThreshold(i, &temp.m_lBVOPThreshold);
					pAVSMPEG4Format->get_ZoneCartoonModeEnabled(i, &temp.m_bCartoonModeEnabled);
					m_arZones.Add(temp);
				}

				pAVSMPEG4Format->get_CPUFlags(&m_lCPUFlags);
				pAVSMPEG4Format->get_ProfileLevel(&m_lProfileLevel);
				
				pAVSMPEG4Format->get_Plugin(&m_lPlugin);
				pAVSMPEG4Format->get_BitrateEnabled(&m_bBitrateEnabled);
				pAVSMPEG4Format->get_DesiredQuant(&m_lDesiredQuant);
				pAVSMPEG4Format->get_DesiredSize(&m_lDesiredSize);

				pAVSMPEG4Format->get_QuantizationType(&m_lQuantizationType);
				pAVSMPEG4Format->get_LumimaskingEnabled(&m_bLumimaskingEnabled);
				pAVSMPEG4Format->get_InterlacingEnabled(&m_bInterlacingEnabled);
				pAVSMPEG4Format->get_TffEnabled(&m_bTffEnabled);
				pAVSMPEG4Format->get_QuarterPixel(&m_bQuarterPixel);
				pAVSMPEG4Format->get_GlobalMotionCompensation(&m_bGlobalMotionCompensation);

				pAVSMPEG4Format->get_UseBVOP(&m_bUseBVOP);
				pAVSMPEG4Format->get_BVOPMaxFrames(&m_lBVOPMaxFrames);
				pAVSMPEG4Format->get_BVOPQuantRatio(&m_lBVOPQuantRatio);
				pAVSMPEG4Format->get_BVOPQuantOffset(&m_lBVOPQuantOffset);
				pAVSMPEG4Format->get_BVOPGlobalPacket(&m_bBVOPGlobalPacket);

				pAVSMPEG4Format->get_PixelAspectRatioEnabled(&m_bPixelAspectRatioEnabled);
				pAVSMPEG4Format->get_DisplayAspectRatio(&m_lDisplayAspectRatio);
				pAVSMPEG4Format->get_PARX(&m_lPARX);
				pAVSMPEG4Format->get_PARY(&m_lPARY);
				pAVSMPEG4Format->get_ARX(&m_lARX);
				pAVSMPEG4Format->get_ARY(&m_lARY);

				pAVSMPEG4Format->get_SingleRCDelayFactor(&m_lSingleRCDelayFactor);
				pAVSMPEG4Format->get_SingleRCAveragingPeriod(&m_lSingleRCAveragingPeriod);
				pAVSMPEG4Format->get_SingleRCBuffer(&m_lSingleRCBuffer);

				BSTR	bsTemp = NULL;
				pAVSMPEG4Format->get_Pass1FileName(&bsTemp);
				m_sPass1FileName=bsTemp;
				RELEASESYSSTRING(bsTemp);

				pAVSMPEG4Format->get_Full1Pass(&m_bFull1Pass);

				pAVSMPEG4Format->get_Pass2FileName(&bsTemp);
				m_sPass2FileName=bsTemp;
				RELEASESYSSTRING(bsTemp);

				pAVSMPEG4Format->get_Pass2KeyframeBoost(&m_lPass2KeyframeBoost);
				pAVSMPEG4Format->get_Pass2KfReduction(&m_lPass2KfReduction);
				pAVSMPEG4Format->get_Pass2KfThreshold(&m_lPass2KfThreshold);
				pAVSMPEG4Format->get_Pass2CurveCompressionHigh(&m_lPass2CurveCompressionHigh);
				pAVSMPEG4Format->get_Pass2CurveCompressionLow(&m_lPass2CurveCompressionLow);
				pAVSMPEG4Format->get_Pass2OverflowControlStrength(&m_lPass2OverflowControlStrength);
				pAVSMPEG4Format->get_Pass2MaxOverflowImprovement(&m_lPass2MaxOverflowImprovement);
				pAVSMPEG4Format->get_Pass2MaxOverflowDegradation(&m_lPass2MaxOverflowDegradation);
				pAVSMPEG4Format->get_Pass2ContainerFrameOverhead(&m_lPass2ContainerFrameOverhead);

				pAVSMPEG4Format->get_MotionSearchType(&m_lMotionSearchType);
				pAVSMPEG4Format->get_VHQMode(&m_lVHQMode);
				pAVSMPEG4Format->get_VHQBMode(&m_bVHQBMode);
				pAVSMPEG4Format->get_ChromaMotion(&m_bChromaMotion);
				pAVSMPEG4Format->get_TurboMode(&m_bTurboMode);
				pAVSMPEG4Format->get_MaxKeyInterval(&m_lMaxKeyInterval);
				pAVSMPEG4Format->get_FrameDropRatio(&m_lFrameDropRatio);

				pAVSMPEG4Format->get_MinIQuantizer(&m_lMinIQuantizer);
				pAVSMPEG4Format->get_MaxIQuantizer(&m_lMaxIQuantizer);
				pAVSMPEG4Format->get_MinPQuantizer(&m_lMinPQuantizer);
				pAVSMPEG4Format->get_MaxPQuantizer(&m_lMaxPQuantizer);
				pAVSMPEG4Format->get_MinBQuantizer(&m_lMinBQuantizer);
				pAVSMPEG4Format->get_MaxBQuantizer(&m_lMaxBQuantizer);
				pAVSMPEG4Format->get_TrellisQuantization(&m_bTrellisQuantization);

				pAVSMPEG4Format->get_DeblockYEnabled(&m_bDeblockYEnabled);
				pAVSMPEG4Format->get_DeblockUVEnabled(&m_bDeblockUVEnabled);
				pAVSMPEG4Format->get_DeringYEnabled(&m_bDeringYEnabled);
				pAVSMPEG4Format->get_DeringUVEnabled(&m_bDeringUVEnabled);
				pAVSMPEG4Format->get_FilmEffectEnabled(&m_bFilmEffectEnabled);
				pAVSMPEG4Format->get_Brightness(&m_lBrightness);

				RELEASEINTERFACE(pAVSMPEG4Format);
			}
		}
		return S_OK;
	}
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		//Здесь проверяем параметры относящиеся к AVSMPEG4VideoFormat
		return CAVSVideoFormatImpl<CoT, T>::SetAdditionalParam(ParamName, ParamValue);
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		//Здесь проверяем параметры относящиеся к AVSMPEG4VideoFormat
		return CAVSVideoFormatImpl<CoT, T>::GetAdditionalParam(ParamName, ParamValue);
	}
	//////////////////////////////////////
	LINK_PROPERTY_LONG(CPUFlags,m_lCPUFlags)
	LINK_PROPERTY_LONG(ProfileLevel,m_lProfileLevel)

	LINK_PROPERTY_LONG(Plugin,m_lPlugin)
	LINK_PROPERTY_VARIANT_BOOL(BitrateEnabled, m_bBitrateEnabled)
	LINK_PROPERTY_LONG(DesiredQuant, m_lDesiredQuant)
	LINK_PROPERTY_LONG(DesiredSize, m_lDesiredSize)

	LINK_PROPERTY_LONG(QuantizationType, m_lQuantizationType)
	LINK_PROPERTY_VARIANT_BOOL(LumimaskingEnabled, m_bLumimaskingEnabled)
	LINK_PROPERTY_VARIANT_BOOL(InterlacingEnabled, m_bInterlacingEnabled)
	LINK_PROPERTY_VARIANT_BOOL(TffEnabled, m_bTffEnabled)
	LINK_PROPERTY_VARIANT_BOOL(QuarterPixel, m_bQuarterPixel)
	LINK_PROPERTY_VARIANT_BOOL(GlobalMotionCompensation, m_bGlobalMotionCompensation)

	LINK_PROPERTY_VARIANT_BOOL(UseBVOP, m_bUseBVOP)
	LINK_PROPERTY_LONG(BVOPMaxFrames, m_lBVOPMaxFrames)
	LINK_PROPERTY_LONG(BVOPQuantRatio, m_lBVOPQuantRatio)
	LINK_PROPERTY_LONG(BVOPQuantOffset, m_lBVOPQuantOffset)
	LINK_PROPERTY_VARIANT_BOOL(BVOPGlobalPacket, m_bBVOPGlobalPacket)

	LINK_PROPERTY_VARIANT_BOOL(PixelAspectRatioEnabled, m_bPixelAspectRatioEnabled)
	LINK_PROPERTY_LONG(DisplayAspectRatio, m_lDisplayAspectRatio)
	LINK_PROPERTY_LONG(PARX, m_lPARX)
	LINK_PROPERTY_LONG(PARY, m_lPARY)
	LINK_PROPERTY_LONG(ARX, m_lARX)
	LINK_PROPERTY_LONG(ARY, m_lARY)	

	//
	STDMETHOD(get_ZonesCount)(long* pVal)
	{
		*pVal = (long)m_arZones.GetCount();
		return S_OK;
	}
	STDMETHOD(put_ZonesCount)(long newVal)
	{
		if (MPEG4_MAX_ZONES<newVal)
			newVal = MPEG4_MAX_ZONES;

		m_arZones.SetCount(newVal);
		return S_OK;
	}

	LINK_PROPERTY_ARRAY_LONG(ZoneFrame, m_arZones, m_lFrame)
	LINK_PROPERTY_ARRAY_LONG(ZoneMode, m_arZones, m_lMode)
	LINK_PROPERTY_ARRAY_LONG(ZoneWeight, m_arZones, m_lWeight)
	LINK_PROPERTY_ARRAY_LONG(ZoneQuant, m_arZones, m_lQuant)
	LINK_PROPERTY_ARRAY_LONG(ZoneFrameType, m_arZones, m_lFrameType)
	LINK_PROPERTY_ARRAY_BOOL(ZoneGrayscaleEnabled, m_arZones, m_bGrayscaleEnabled)
	LINK_PROPERTY_ARRAY_BOOL(ZoneChromaOptimizerEnabled, m_arZones, m_bChromaOptimizerEnabled)
	LINK_PROPERTY_ARRAY_LONG(ZoneBVOPThreshold, m_arZones, m_lBVOPThreshold)
	LINK_PROPERTY_ARRAY_BOOL(ZoneCartoonModeEnabled, m_arZones, m_bCartoonModeEnabled)
	//
	LINK_PROPERTY_LONG(SingleRCDelayFactor	 , m_lSingleRCDelayFactor)
	LINK_PROPERTY_LONG(SingleRCAveragingPeriod, m_lSingleRCAveragingPeriod)
	LINK_PROPERTY_LONG(SingleRCBuffer		 , m_lSingleRCBuffer)

	LINK_PROPERTY_BSTR(Pass1FileName, m_sPass1FileName)
	LINK_PROPERTY_VARIANT_BOOL(Full1Pass, m_bFull1Pass)
	LINK_PROPERTY_BSTR(Pass2FileName, m_sPass2FileName)
	
	LINK_PROPERTY_LONG(Pass2KeyframeBoost, m_lPass2KeyframeBoost)
	LINK_PROPERTY_LONG(Pass2KfReduction, m_lPass2KfReduction)
	LINK_PROPERTY_LONG(Pass2KfThreshold, m_lPass2KfThreshold)
	LINK_PROPERTY_LONG(Pass2CurveCompressionHigh, m_lPass2CurveCompressionHigh)
	LINK_PROPERTY_LONG(Pass2CurveCompressionLow, m_lPass2CurveCompressionLow)
	LINK_PROPERTY_LONG(Pass2OverflowControlStrength, m_lPass2OverflowControlStrength)
	LINK_PROPERTY_LONG(Pass2MaxOverflowImprovement, m_lPass2MaxOverflowImprovement)
	LINK_PROPERTY_LONG(Pass2MaxOverflowDegradation, m_lPass2MaxOverflowDegradation)
	LINK_PROPERTY_LONG(Pass2ContainerFrameOverhead, m_lPass2ContainerFrameOverhead)
	
	LINK_PROPERTY_LONG(MotionSearchType, m_lMotionSearchType)
	LINK_PROPERTY_LONG(VHQMode, m_lVHQMode)
	LINK_PROPERTY_VARIANT_BOOL(VHQBMode, m_bVHQBMode)
	LINK_PROPERTY_VARIANT_BOOL(ChromaMotion, m_bChromaMotion)
	LINK_PROPERTY_VARIANT_BOOL(TurboMode, m_bTurboMode)
	LINK_PROPERTY_LONG(MaxKeyInterval, m_lMaxKeyInterval)
	LINK_PROPERTY_LONG(FrameDropRatio, m_lFrameDropRatio)

		
	LINK_PROPERTY_LONG(MinIQuantizer, m_lMinIQuantizer)
	LINK_PROPERTY_LONG(MaxIQuantizer, m_lMaxIQuantizer)
	LINK_PROPERTY_LONG(MinPQuantizer, m_lMinPQuantizer)
	LINK_PROPERTY_LONG(MaxPQuantizer, m_lMaxPQuantizer)
	LINK_PROPERTY_LONG(MinBQuantizer, m_lMinBQuantizer)
	LINK_PROPERTY_LONG(MaxBQuantizer, m_lMaxBQuantizer)
	LINK_PROPERTY_VARIANT_BOOL(TrellisQuantization, m_bTrellisQuantization)
		
	LINK_PROPERTY_VARIANT_BOOL(DeblockYEnabled,  m_bDeblockYEnabled)
	LINK_PROPERTY_VARIANT_BOOL(DeblockUVEnabled, m_bDeblockUVEnabled)
	LINK_PROPERTY_VARIANT_BOOL(DeringYEnabled, m_bDeringYEnabled)
	LINK_PROPERTY_VARIANT_BOOL(DeringUVEnabled, m_bDeringUVEnabled)
	LINK_PROPERTY_VARIANT_BOOL(FilmEffectEnabled, m_bFilmEffectEnabled)
	LINK_PROPERTY_LONG(Brightness, m_lBrightness)
protected:
	void DublicateZones(IAVSMPEG4VideoFormat *pAVSMPEG4Format)
	{
		if (NULL==pAVSMPEG4Format)
			return;
		long lCount = (long)m_arZones.GetCount();
		pAVSMPEG4Format->put_ZonesCount(lCount);
		for (long lInd = 0; lInd<lCount; lInd++)
		{
			pAVSMPEG4Format->put_ZoneFrame(lInd, m_arZones[lInd].m_lFrame);
			pAVSMPEG4Format->put_ZoneMode(lInd, m_arZones[lInd].m_lMode);
			pAVSMPEG4Format->put_ZoneWeight(lInd, m_arZones[lInd].m_lWeight);
			pAVSMPEG4Format->put_ZoneQuant(lInd, m_arZones[lInd].m_lQuant);
			pAVSMPEG4Format->put_ZoneFrameType(lInd, m_arZones[lInd].m_lFrameType);
			pAVSMPEG4Format->put_ZoneGrayscaleEnabled(lInd, m_arZones[lInd].m_bGrayscaleEnabled ? VARIANT_TRUE : VARIANT_FALSE);
			pAVSMPEG4Format->put_ZoneChromaOptimizerEnabled(lInd, m_arZones[lInd].m_bChromaOptimizerEnabled ? VARIANT_TRUE : VARIANT_FALSE);
			pAVSMPEG4Format->put_ZoneBVOPThreshold(lInd, m_arZones[lInd].m_lBVOPThreshold);
			pAVSMPEG4Format->put_ZoneCartoonModeEnabled(lInd, m_arZones[lInd].m_bCartoonModeEnabled ? VARIANT_TRUE : VARIANT_FALSE);
		}
	}
	CAtlArray<Zone> m_arZones;

	long m_lCPUFlags;
	long m_lProfileLevel;	

	long m_lPlugin;
	VARIANT_BOOL m_bBitrateEnabled;
	long m_lDesiredQuant;
	long m_lDesiredSize;

	long m_lQuantizationType;
	VARIANT_BOOL m_bLumimaskingEnabled;
	VARIANT_BOOL m_bInterlacingEnabled;
	VARIANT_BOOL m_bTffEnabled;
	VARIANT_BOOL m_bQuarterPixel;
	VARIANT_BOOL m_bGlobalMotionCompensation;

	VARIANT_BOOL m_bUseBVOP;
	long m_lBVOPMaxFrames;
	long m_lBVOPQuantRatio;	
	long m_lBVOPQuantOffset;
	VARIANT_BOOL m_bBVOPGlobalPacket;

	VARIANT_BOOL m_bPixelAspectRatioEnabled;
	long m_lDisplayAspectRatio;
	long m_lPARX;
	long m_lPARY;
	long m_lARX;
	long m_lARY;

	long m_lSingleRCDelayFactor;
	long m_lSingleRCAveragingPeriod;
	long m_lSingleRCBuffer;

	CString m_sPass1FileName;
	VARIANT_BOOL m_bFull1Pass;
	CString m_sPass2FileName;
	long m_lPass2KeyframeBoost;
	long m_lPass2KfReduction;
	long m_lPass2KfThreshold;
	long m_lPass2CurveCompressionHigh;
	long m_lPass2CurveCompressionLow;
	long m_lPass2OverflowControlStrength;
	long m_lPass2MaxOverflowImprovement;
	long m_lPass2MaxOverflowDegradation;
	long m_lPass2ContainerFrameOverhead;

	long m_lMotionSearchType;
	long m_lVHQMode;
	VARIANT_BOOL m_bVHQBMode;
	VARIANT_BOOL m_bChromaMotion;
	VARIANT_BOOL m_bTurboMode;
	long m_lMaxKeyInterval;
	long m_lFrameDropRatio;

	long m_lMinIQuantizer;
	long m_lMaxIQuantizer;
	long m_lMinPQuantizer;
	long m_lMaxPQuantizer;
	long m_lMinBQuantizer;
	long m_lMaxBQuantizer;
	VARIANT_BOOL m_bTrellisQuantization;

	VARIANT_BOOL m_bDeblockYEnabled;
	VARIANT_BOOL m_bDeblockUVEnabled;
	VARIANT_BOOL m_bDeringYEnabled;
	VARIANT_BOOL m_bDeringUVEnabled;
	VARIANT_BOOL m_bFilmEffectEnabled;
	long m_lBrightness;

	//два следующих метода можно переопределить в порождаемых классах, чтобы реализовать
	//сохранение и загрузку параметров, которые не получиться сохранить автоматически из IDispatch
	virtual void SaveExtendedSettingToXML(CString &sXMLData)
	{
		XmlUtils::CXmlWriter oXMLWriter;
		oXMLWriter.WriteNodeBegin(g_cpszExtendedSettingNodeName);

		oXMLWriter.WriteNodeBegin(g_cpszZonesNodeName);
		size_t nCount = m_arZones.GetCount();
		oXMLWriter.WriteNode(g_cpszZonesCountNodeName, (long)nCount);
		for (size_t nIndex = 0; nIndex<nCount; nIndex++)
		{
			CString sZoneTitle;
			sZoneTitle.Format(g_cpszZoneTitleFormatNodeName, nIndex);
			oXMLWriter.WriteNodeBegin(sZoneTitle);
			const Zone &oZone = m_arZones[nIndex];
			oXMLWriter.WriteNode(g_cpszZoneFrameNodeName, oZone.m_lFrame);
			oXMLWriter.WriteNode(g_cpszZoneModeNodeName, oZone.m_lMode);
			oXMLWriter.WriteNode(g_cpszZoneWeightNodeName, oZone.m_lWeight);
			oXMLWriter.WriteNode(g_cpszZoneQuantNodeName, oZone.m_lQuant);
			oXMLWriter.WriteNode(g_cpszZoneFrameTypeNodeName, oZone.m_lFrameType);
			oXMLWriter.WriteNode(g_cpszZoneGrayscaleEnabledNodeName, (oZone.m_bGrayscaleEnabled ? 1 : 0));
			oXMLWriter.WriteNode(g_cpszZoneChromaOptimizerEnabledNodeName, (oZone.m_bChromaOptimizerEnabled ? 1 : 0));
			oXMLWriter.WriteNode(g_cpszZoneBVOPThresholdNodeName, oZone.m_lBVOPThreshold);
			oXMLWriter.WriteNode(g_cpszZoneCartoonModeEnabledNodeName, (oZone.m_bCartoonModeEnabled ? 1 : 0));

			oXMLWriter.WriteNodeEnd(sZoneTitle);
		}
		oXMLWriter.WriteNodeEnd(g_cpszZonesNodeName);

		oXMLWriter.WriteNodeEnd(g_cpszExtendedSettingNodeName);
		sXMLData = oXMLWriter.GetXmlString();
	}
	virtual void LoadExtendedSettingFromXML(const CString &sXMLData)
	{
		m_arZones.RemoveAll();
		XmlUtils::CXmlReader oXmlReader;
		oXmlReader.SetXmlString(sXMLData);

		oXmlReader.ReadNode(g_cpszZonesNodeName);
		size_t nCount = _ttoi(oXmlReader.ReadNodeValue(g_cpszZonesCountNodeName));
		for (size_t nIndex = 0; nIndex<nCount; nIndex++)
		{
			CString sZoneTitle;
			sZoneTitle.Format(g_cpszZoneTitleFormatNodeName, nIndex);
			if (!oXmlReader.ReadNode(sZoneTitle))
				continue;

			Zone oZone;
			oZone.m_lFrame					= _ttoi(oXmlReader.ReadNodeValue(g_cpszZoneFrameNodeName));
			oZone.m_lMode					= _ttoi(oXmlReader.ReadNodeValue(g_cpszZoneModeNodeName));
			oZone.m_lWeight					= _ttoi(oXmlReader.ReadNodeValue(g_cpszZoneWeightNodeName));
			oZone.m_lQuant					= _ttoi(oXmlReader.ReadNodeValue(g_cpszZoneQuantNodeName));
			oZone.m_lFrameType				= _ttoi(oXmlReader.ReadNodeValue(g_cpszZoneFrameTypeNodeName));
			oZone.m_bGrayscaleEnabled		= (0!=_ttoi(oXmlReader.ReadNodeValue(g_cpszZoneGrayscaleEnabledNodeName)));
			oZone.m_bChromaOptimizerEnabled = (0!=_ttoi(oXmlReader.ReadNodeValue(g_cpszZoneChromaOptimizerEnabledNodeName)));
			oZone.m_lBVOPThreshold			= _ttoi(oXmlReader.ReadNodeValue(g_cpszZoneBVOPThresholdNodeName));
			oZone.m_bCartoonModeEnabled		= (0!=_ttoi(oXmlReader.ReadNodeValue(g_cpszZoneCartoonModeEnabledNodeName)));
			m_arZones.Add(oZone);
		}
		if (0==m_arZones.GetCount())
			m_arZones.SetCount(1);
	}
};




// CAVSMPEG4VideoFormat
[ coclass, uuid("2F7C6CC1-5685-4D96-AF12-39C52017731A"), threading(apartment), vi_progid("AVSMediaFormatSettings3.AVSMPEG4VideoFo"), progid("AVSMediaFormatSettings3.AVSMPEG4Video.1"), version(1.0)]
class ATL_NO_VTABLE CAVSMPEG4VideoFormat : 	public CAVSMPEG4VideoFormatImpl<CAVSMPEG4VideoFormat, IAVSMPEG4VideoFormat>
{
};

