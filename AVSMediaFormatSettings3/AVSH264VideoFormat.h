#pragma once
#include "resource.h"       // main symbols
#include "ATLDefine.h"
#include "AVSVideoFormat.h"

class CAVSH264VideoFormat;
// IAVSH264VideoFormat
[ object, uuid("AB130AA7-AACE-4fdd-BC44-97455FB77522"), dual, pointer_default(unique)]
__interface IAVSH264VideoFormat : IAVSVideoFormat
{
	[id(107000 + 1), propget] HRESULT DesiredQuant([out, retval] long* pVal);
	[id(107000 + 1), propput] HRESULT DesiredQuant([in] long newVal);
	[id(107000 + 2), propget] HRESULT NumThreads([out, retval] long* pVal);
	[id(107000 + 2), propput] HRESULT NumThreads([in] long newVal);
	[id(107000 + 3), propget] HRESULT NoiseReduction([out, retval] long* pVal);
	[id(107000 + 3), propput] HRESULT NoiseReduction([in] long newVal);

	[id(107000 + 4), propget] HRESULT RefMax([out, retval] long* pVal);
	[id(107000 + 4), propput] HRESULT RefMax([in] long newVal);
	[id(107000 + 5), propget] HRESULT MinKeyInterval([out, retval] long* pVal);
	[id(107000 + 5), propput] HRESULT MinKeyInterval([in] long newVal);
	[id(107000 + 6), propget] HRESULT MaxKeyInterval([out, retval] long* pVal);
	[id(107000 + 6), propput] HRESULT MaxKeyInterval([in] long newVal);
	[id(107000 + 7), propget] HRESULT SceneCutThreshold([out, retval] long* pVal);
	[id(107000 + 7), propput] HRESULT SceneCutThreshold([in] long newVal);
	[id(107000 + 8), propget] HRESULT QPMin([out, retval] long* pVal);
	[id(107000 + 8), propput] HRESULT QPMin([in] long newVal);
	[id(107000 + 9), propget] HRESULT QPMax([out, retval] long* pVal);
	[id(107000 + 9), propput] HRESULT QPMax([in] long newVal);
	[id(107000 + 10), propget] HRESULT QPStep([out, retval] long* pVal);
	[id(107000 + 10), propput] HRESULT QPStep([in] long newVal);
	
	[id(107000 + 11), propget] HRESULT DeblockingFilter([out, retval] VARIANT_BOOL* pVal);
	[id(107000 + 11), propput] HRESULT DeblockingFilter([in] VARIANT_BOOL newVal);
	[id(107000 + 12), propget] HRESULT Strength([out, retval] long* pVal);
	[id(107000 + 12), propput] HRESULT Strength([in] long newVal);
	[id(107000 + 13), propget] HRESULT Threshold([out, retval] long* pVal);
	[id(107000 + 13), propput] HRESULT Threshold([in] long newVal);

	[id(107000 + 14), propget] HRESULT Cabac([out, retval] VARIANT_BOOL* pVal);
	[id(107000 + 14), propput] HRESULT Cabac([in] VARIANT_BOOL newVal);
	[id(107000 + 15), propget] HRESULT Trellis([out, retval] VARIANT_BOOL* pVal);
	[id(107000 + 15), propput] HRESULT Trellis([in] VARIANT_BOOL newVal);

	[id(107000 + 16), propget] HRESULT ChromaME([out, retval] VARIANT_BOOL* pVal);
	[id(107000 + 16), propput] HRESULT ChromaME([in] VARIANT_BOOL newVal);
	[id(107000 + 17), propget] HRESULT KeyframeBoost([out, retval] long* pVal);
	[id(107000 + 17), propput] HRESULT KeyframeBoost([in] long newVal);
	[id(107000 + 18), propget] HRESULT BFrameReduction([out, retval] long* pVal);
	[id(107000 + 18), propput] HRESULT BFrameReduction([in] long newVal);
	[id(107000 + 19), propget] HRESULT CurveComp([out, retval] long* pVal);
	[id(107000 + 19), propput] HRESULT CurveComp([in] long newVal);

	[id(107000 + 20), propget] HRESULT SARX([out, retval] long* pVal);
	[id(107000 + 20), propput] HRESULT SARX([in] long newVal);
	[id(107000 + 21), propget] HRESULT SARY([out, retval] long* pVal);
	[id(107000 + 21), propput] HRESULT SARY([in] long newVal);

	[id(107000 + 22), propget] HRESULT BVOPMaxFrames([out, retval] long* pVal);
	[id(107000 + 22), propput] HRESULT BVOPMaxFrames([in] long newVal);
	[id(107000 + 23), propget] HRESULT BVOPWeightedBipredictional([out, retval] VARIANT_BOOL* pVal);
	[id(107000 + 23), propput] HRESULT BVOPWeightedBipredictional([in] VARIANT_BOOL newVal);
	[id(107000 + 24), propget] HRESULT BVOPReference([out, retval] VARIANT_BOOL* pVal);
	[id(107000 + 24), propput] HRESULT BVOPReference([in] VARIANT_BOOL newVal);
	[id(107000 + 25), propget] HRESULT BVOPAdaptive([out, retval] VARIANT_BOOL* pVal);
	[id(107000 + 25), propput] HRESULT BVOPAdaptive([in] VARIANT_BOOL newVal);
	[id(107000 + 26), propget] HRESULT BVOPBidirectionalME([out, retval] VARIANT_BOOL* pVal);
	[id(107000 + 26), propput] HRESULT BVOPBidirectionalME([in] VARIANT_BOOL newVal);
	[id(107000 + 27), propget] HRESULT BVOPBias([out, retval] long* pVal);
	[id(107000 + 27), propput] HRESULT BVOPBias([in] long newVal);

	[id(107000 + 28), propget] HRESULT PartitionDecision([out, retval] long* pVal);
	[id(107000 + 28), propput] HRESULT PartitionDecision([in] long newVal);
	[id(107000 + 29), propget] HRESULT MEMethod([out, retval] long* pVal);
	[id(107000 + 29), propput] HRESULT MEMethod([in] long newVal);
	[id(107000 + 30), propget] HRESULT MERange([out, retval] long* pVal);
	[id(107000 + 30), propput] HRESULT MERange([in] long newVal);

	[id(107000 + 31), propget] HRESULT DirectMode([out, retval] long* pVal);
	[id(107000 + 31), propput] HRESULT DirectMode([in] long newVal);
	[id(107000 + 32), propget] HRESULT DCT8x8([out, retval] VARIANT_BOOL* pVal);
	[id(107000 + 32), propput] HRESULT DCT8x8([in] VARIANT_BOOL newVal);
	[id(107000 + 33), propget] HRESULT PSub16x16([out, retval] VARIANT_BOOL* pVal);
	[id(107000 + 33), propput] HRESULT PSub16x16([in] VARIANT_BOOL newVal);
	[id(107000 + 34), propget] HRESULT BSub16x16([out, retval] VARIANT_BOOL* pVal);
	[id(107000 + 34), propput] HRESULT BSub16x16([in] VARIANT_BOOL newVal);
	[id(107000 + 35), propget] HRESULT PSub8x8([out, retval] VARIANT_BOOL* pVal);
	[id(107000 + 35), propput] HRESULT PSub8x8([in] VARIANT_BOOL newVal);
	[id(107000 + 36), propget] HRESULT I8x8([out, retval] VARIANT_BOOL* pVal);
	[id(107000 + 36), propput] HRESULT I8x8([in] VARIANT_BOOL newVal);
	[id(107000 + 37), propget] HRESULT I4x4([out, retval] VARIANT_BOOL* pVal);
	[id(107000 + 37), propput] HRESULT I4x4([in] VARIANT_BOOL newVal);

	[id(107000 + 38), propget] HRESULT MixedRef([out, retval] VARIANT_BOOL* pVal);
	[id(107000 + 38), propput] HRESULT MixedRef([in] VARIANT_BOOL newVal);

	[id(107000 + 39), propget] HRESULT EncodingType([out, retval] long* pVal);
	[id(107000 + 39), propput] HRESULT EncodingType([in] long newVal);
	[id(107000 + 40), propget] HRESULT NumPass([out, retval] long* pVal);
	[id(107000 + 40), propput] HRESULT NumPass([in] long newVal);
	[id(107000 + 41), propget] HRESULT Fast1Pass([out, retval] VARIANT_BOOL* pVal);
	[id(107000 + 41), propput] HRESULT Fast1Pass([in] VARIANT_BOOL newVal);
	[id(107000 + 42), propget] HRESULT UpdateStats([out, retval] VARIANT_BOOL* pVal);
	[id(107000 + 42), propput] HRESULT UpdateStats([in] VARIANT_BOOL newVal);

	[id(107000 + 43), propget] HRESULT LevelIDC([out, retval] long* pVal);
	[id(107000 + 43), propput] HRESULT LevelIDC([in] long newVal);
	[id(107000 + 44), propget] HRESULT VBVMaxBitrate([out, retval] long* pVal);
	[id(107000 + 44), propput] HRESULT VBVMaxBitrate([in] long newVal);

	[id(107000 + 45), propget] HRESULT StatFileName([out, retval] BSTR* pVal);
	[id(107000 + 45), propput] HRESULT StatFileName([in] BSTR newVal);

	[id(107000 + 46), propget] HRESULT AppleCompatible([out, retval] VARIANT_BOOL* pVal);
	[id(107000 + 46), propput] HRESULT AppleCompatible([in] VARIANT_BOOL newVal);

	[id(103000 + 47), propget] HRESULT ExtraData([out, retval, satype("BYTE")] SAFEARRAY** pVal);
	[id(103000 + 47), propput] HRESULT ExtraData([in, satype("BYTE")] SAFEARRAY* newVal);
};

template <class CoT, class T>
class CAVSH264VideoFormatImpl : public CAVSVideoFormatImpl<CoT, T>
{
public:
	CAVSH264VideoFormatImpl()
	{
		m_lMediaType = MEDIAFORMAT_H264;
		m_parExtraData = NULL;
		SetDefaultProperties();
	}
	~CAVSH264VideoFormatImpl()
	{
		RELEASEARRAY(m_parExtraData);
	}
	STDMETHOD(CreateDuplicate)(IAVSMediaFormat** pMediaFormat)
	{
		CAVSVideoFormatImpl<CoT, T>::CreateDuplicate(pMediaFormat);
		if (NULL==(*pMediaFormat))
			return S_OK;
		IAVSH264VideoFormat* pAVSH264Format=NULL;
		(*pMediaFormat)->QueryInterface(__uuidof(IAVSH264VideoFormat), (void**)&pAVSH264Format);
		if (NULL==pAVSH264Format)
		{
			(*pMediaFormat)->Release();
			*pMediaFormat = NULL;
			return S_OK;
		}

		pAVSH264Format->put_DesiredQuant(m_lDesiredQuant);
		pAVSH264Format->put_NumThreads(m_lNumThreads);
		pAVSH264Format->put_NoiseReduction(m_lNoiseReduction);

		pAVSH264Format->put_RefMax(m_lRefMax);
		pAVSH264Format->put_MinKeyInterval(m_lMinKeyInterval);
		pAVSH264Format->put_MaxKeyInterval(m_lMaxKeyInterval);
		pAVSH264Format->put_SceneCutThreshold(m_lSceneCutThreshold);
		pAVSH264Format->put_QPMin(m_lQPMin);
		pAVSH264Format->put_QPMax(m_lQPMax);
		pAVSH264Format->put_QPStep(m_lQPStep);

		pAVSH264Format->put_DeblockingFilter((m_bDeblockingFilter ? VARIANT_TRUE : VARIANT_FALSE));
		pAVSH264Format->put_Strength(m_lStrength);
		pAVSH264Format->put_Threshold(m_lThreshold);

		pAVSH264Format->put_Cabac((m_bCabac ? VARIANT_TRUE : VARIANT_FALSE));
		pAVSH264Format->put_Trellis((m_bTrellis ? VARIANT_TRUE : VARIANT_FALSE));

		pAVSH264Format->put_ChromaME((m_bChromaME ? VARIANT_TRUE : VARIANT_FALSE));
		pAVSH264Format->put_KeyframeBoost(m_lKeyframeBoost);
		pAVSH264Format->put_BFrameReduction(m_lBFrameReduction);
		pAVSH264Format->put_CurveComp(m_lCurveComp);

		pAVSH264Format->put_SARX(m_lSARX);
		pAVSH264Format->put_SARY(m_lSARY);

		pAVSH264Format->put_BVOPMaxFrames(m_lBVOPMaxFrames);
		pAVSH264Format->put_BVOPWeightedBipredictional((m_bBVOPWeightedBipredictional ? VARIANT_TRUE : VARIANT_FALSE));
		pAVSH264Format->put_BVOPReference((m_bBVOPReference ? VARIANT_TRUE : VARIANT_FALSE));
		pAVSH264Format->put_BVOPAdaptive((m_bBVOPAdaptive ? VARIANT_TRUE : VARIANT_FALSE));
		pAVSH264Format->put_BVOPBidirectionalME((m_bBVOPBidirectionalME ? VARIANT_TRUE : VARIANT_FALSE));
		pAVSH264Format->put_BVOPBias(m_lBVOPBias);

		pAVSH264Format->put_PartitionDecision(m_lPartitionDecision);
		pAVSH264Format->put_MEMethod(m_lMEMethod);
		pAVSH264Format->put_MERange(m_lMERange);

		pAVSH264Format->put_DirectMode(m_lDirectMode);
		pAVSH264Format->put_DCT8x8((m_bDCT8x8 ? VARIANT_TRUE : VARIANT_FALSE));
		pAVSH264Format->put_PSub16x16((m_bPSub16x16 ? VARIANT_TRUE : VARIANT_FALSE));
		pAVSH264Format->put_BSub16x16((m_bBSub16x16 ? VARIANT_TRUE : VARIANT_FALSE));
		pAVSH264Format->put_PSub8x8((m_bPSub8x8 ? VARIANT_TRUE : VARIANT_FALSE));
		pAVSH264Format->put_I8x8((m_bI8x8 ? VARIANT_TRUE : VARIANT_FALSE));
		pAVSH264Format->put_I4x4((m_bI4x4 ? VARIANT_TRUE : VARIANT_FALSE));

		pAVSH264Format->put_MixedRef((m_bMixedRef ? VARIANT_TRUE : VARIANT_FALSE));

		pAVSH264Format->put_EncodingType(m_lEncodingType);
		pAVSH264Format->put_NumPass(m_lNumPass);
		pAVSH264Format->put_Fast1Pass((m_bFast1Pass ? VARIANT_TRUE : VARIANT_FALSE));
		pAVSH264Format->put_UpdateStats((m_bUpdateStats ? VARIANT_TRUE : VARIANT_FALSE));

		pAVSH264Format->put_LevelIDC(m_lLevelIDC);
		pAVSH264Format->put_VBVMaxBitrate(m_lVBVMaxBitrate);

		BSTR bsTemp = m_sStatFileName.AllocSysString();
		pAVSH264Format->put_StatFileName(bsTemp);
		SysFreeString(bsTemp);

		pAVSH264Format->put_AppleCompatible((m_bAppleCompatible ? VARIANT_TRUE : VARIANT_FALSE));

		pAVSH264Format->put_ExtraData(m_parExtraData);

		pAVSH264Format->Release();
		return S_OK;
	}
	STDMETHOD(SetDefaultProperties)(void)
	{
		CAVSVideoFormatImpl<CoT, T>::SetDefaultProperties();
		m_lDesiredQuant = 26;
		m_lNumThreads = 1;
		m_lNoiseReduction = 0;

		m_lRefMax = 1;
		m_lMinKeyInterval = 25;
		m_lMaxKeyInterval = 250;
		m_lSceneCutThreshold = 40;
		m_lQPMin = 10;
		m_lQPMax = 51;
		m_lQPStep = 4;

		m_bDeblockingFilter = VARIANT_TRUE;
		m_lStrength = 0;
		m_lThreshold = 0;

		m_bCabac = VARIANT_TRUE;
		m_bTrellis = VARIANT_TRUE;

		m_bChromaME = VARIANT_TRUE;
		m_lKeyframeBoost = 40;
		m_lBFrameReduction = 30;
		m_lCurveComp = 60;

		m_lSARX = 1;
		m_lSARY = 1;

		m_lBVOPMaxFrames = 2;
		m_bBVOPWeightedBipredictional = VARIANT_TRUE;
		m_bBVOPReference = FALSE;
		m_bBVOPAdaptive = VARIANT_TRUE;
		m_bBVOPBidirectionalME = FALSE;
		m_lBVOPBias = 0;

		m_lPartitionDecision = 4;
		m_lMEMethod = 1;
		m_lMERange = 16;

		m_lDirectMode = 1;
		m_bDCT8x8 = VARIANT_TRUE;
		m_bPSub16x16 = VARIANT_TRUE;
		m_bBSub16x16 = VARIANT_TRUE;
		m_bPSub8x8 = FALSE;
		m_bI8x8 = VARIANT_TRUE;
		m_bI4x4 = VARIANT_TRUE;

		m_bMixedRef = FALSE;

		m_lEncodingType = 0;
		m_lNumPass = 1;
		m_bFast1Pass = FALSE;
		m_bUpdateStats = VARIANT_TRUE;

		m_lLevelIDC = 51;
		m_lVBVMaxBitrate = 0;

		m_sStatFileName = "c:/h264.pass";

		m_bAppleCompatible = FALSE;
		RELEASEARRAY(m_parExtraData);

		return S_OK;
	}
	STDMETHOD(SetPropertiesByFormat)(IAVSMediaFormat* pMediaFormat)
	{
		CAVSVideoFormatImpl<CoT, T>::SetPropertiesByFormat(pMediaFormat);
		if (pMediaFormat!=NULL)
		{
			IAVSH264VideoFormat*	pAVSH264Format=NULL;
			pMediaFormat->QueryInterface(__uuidof(IAVSH264VideoFormat), (void**)&pAVSH264Format);
			if (pAVSH264Format!=NULL)
			{
				pAVSH264Format->get_DesiredQuant(&m_lDesiredQuant);
				pAVSH264Format->get_NumThreads(&m_lNumThreads);
				pAVSH264Format->get_NoiseReduction(&m_lNoiseReduction);

				pAVSH264Format->get_RefMax(&m_lRefMax);
				pAVSH264Format->get_MinKeyInterval(&m_lMinKeyInterval);
				pAVSH264Format->get_MaxKeyInterval(&m_lMaxKeyInterval);
				pAVSH264Format->get_SceneCutThreshold(&m_lSceneCutThreshold);
				pAVSH264Format->get_QPMin(&m_lQPMin);
				pAVSH264Format->get_QPMax(&m_lQPMax);
				pAVSH264Format->get_QPStep(&m_lQPStep);

				pAVSH264Format->get_DeblockingFilter(&m_bDeblockingFilter);
				pAVSH264Format->get_Strength(&m_lStrength);
				pAVSH264Format->get_Threshold(&m_lThreshold);

				pAVSH264Format->get_Cabac(&m_bCabac);
				pAVSH264Format->get_Trellis(&m_bTrellis);

				pAVSH264Format->get_ChromaME(&m_bChromaME);
				pAVSH264Format->get_KeyframeBoost(&m_lKeyframeBoost);
				pAVSH264Format->get_BFrameReduction(&m_lBFrameReduction);
				pAVSH264Format->get_CurveComp(&m_lCurveComp);

				pAVSH264Format->get_SARX(&m_lSARX);
				pAVSH264Format->get_SARY(&m_lSARY);

				pAVSH264Format->get_BVOPMaxFrames(&m_lBVOPMaxFrames);
				pAVSH264Format->get_BVOPWeightedBipredictional(&m_bBVOPWeightedBipredictional);
				pAVSH264Format->get_BVOPReference(&m_bBVOPReference);
				pAVSH264Format->get_BVOPAdaptive(&m_bBVOPAdaptive);
				pAVSH264Format->get_BVOPBidirectionalME(&m_bBVOPBidirectionalME);
				pAVSH264Format->get_BVOPBias(&m_lBVOPBias);

				pAVSH264Format->get_PartitionDecision(&m_lPartitionDecision);
				pAVSH264Format->get_MEMethod(&m_lMEMethod);
				pAVSH264Format->get_MERange(&m_lMERange);

				pAVSH264Format->get_DirectMode(&m_lDirectMode);
				pAVSH264Format->get_DCT8x8(&m_bDCT8x8);
				pAVSH264Format->get_PSub16x16(&m_bPSub16x16);
				pAVSH264Format->get_BSub16x16(&m_bBSub16x16);
				pAVSH264Format->get_PSub8x8(&m_bPSub8x8);
				pAVSH264Format->get_I8x8(&m_bI8x8);
				pAVSH264Format->get_I4x4(&m_bI4x4);

				pAVSH264Format->get_MixedRef(&m_bMixedRef);

				pAVSH264Format->get_EncodingType(&m_lEncodingType);
				pAVSH264Format->get_NumPass(&m_lNumPass);
				pAVSH264Format->get_Fast1Pass(&m_bFast1Pass);
				pAVSH264Format->get_UpdateStats(&m_bUpdateStats);

				pAVSH264Format->get_LevelIDC(&m_lLevelIDC);
				pAVSH264Format->get_VBVMaxBitrate(&m_lVBVMaxBitrate);

				BSTR	sStatFileName=NULL;
				pAVSH264Format->get_StatFileName(&sStatFileName);
				m_sStatFileName=sStatFileName;
				RELEASESYSSTRING(sStatFileName);

				pAVSH264Format->get_AppleCompatible(&m_bAppleCompatible);

				RELEASEARRAY(m_parExtraData);
				pAVSH264Format->get_ExtraData(&m_parExtraData);
				RELEASEINTERFACE(pAVSH264Format);
			}
		}
		return S_OK;
	}
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		//Здесь проверяем параметры относящиеся к AVSH264VideoFormat
		return CAVSVideoFormatImpl<CoT, T>::SetAdditionalParam(ParamName, ParamValue);
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		//Здесь проверяем параметры относящиеся к AVSH264VideoFormat
		return CAVSVideoFormatImpl<CoT, T>::GetAdditionalParam(ParamName, ParamValue);
	}
	//////
	LINK_PROPERTY_LONG(DesiredQuant, m_lDesiredQuant)
	LINK_PROPERTY_LONG(NumThreads, m_lNumThreads)
	LINK_PROPERTY_LONG(NoiseReduction, m_lNoiseReduction)

	LINK_PROPERTY_LONG(RefMax, m_lRefMax)	
	LINK_PROPERTY_LONG(MinKeyInterval, m_lMinKeyInterval)
	LINK_PROPERTY_LONG(MaxKeyInterval, m_lMaxKeyInterval)
	LINK_PROPERTY_LONG(SceneCutThreshold, m_lSceneCutThreshold)
	LINK_PROPERTY_LONG(QPMin, m_lQPMin)
	LINK_PROPERTY_LONG(QPMax, m_lQPMax)
	LINK_PROPERTY_LONG(QPStep, m_lQPStep)

	LINK_PROPERTY_VARIANT_BOOL(DeblockingFilter, m_bDeblockingFilter)
	LINK_PROPERTY_LONG(Strength, m_lStrength)
	LINK_PROPERTY_LONG(Threshold, m_lThreshold)

	LINK_PROPERTY_VARIANT_BOOL(Cabac, m_bCabac)
	LINK_PROPERTY_VARIANT_BOOL(Trellis, m_bTrellis)

	LINK_PROPERTY_VARIANT_BOOL(ChromaME, m_bChromaME)
	LINK_PROPERTY_LONG(KeyframeBoost, m_lKeyframeBoost)
	LINK_PROPERTY_LONG(BFrameReduction, m_lBFrameReduction)
	LINK_PROPERTY_LONG(CurveComp, m_lCurveComp)

	LINK_PROPERTY_LONG(SARX, m_lSARX)
	LINK_PROPERTY_LONG(SARY, m_lSARY)

	LINK_PROPERTY_LONG(BVOPMaxFrames, m_lBVOPMaxFrames)
	LINK_PROPERTY_VARIANT_BOOL(BVOPWeightedBipredictional, m_bBVOPWeightedBipredictional)
	LINK_PROPERTY_VARIANT_BOOL(BVOPReference, m_bBVOPReference)
	LINK_PROPERTY_VARIANT_BOOL(BVOPAdaptive, m_bBVOPAdaptive)
	LINK_PROPERTY_VARIANT_BOOL(BVOPBidirectionalME, m_bBVOPBidirectionalME)
	LINK_PROPERTY_LONG(BVOPBias, m_lBVOPBias)

	LINK_PROPERTY_LONG(PartitionDecision, m_lPartitionDecision)
	LINK_PROPERTY_LONG(MEMethod, m_lMEMethod)
	LINK_PROPERTY_LONG(MERange, m_lMERange)

	LINK_PROPERTY_LONG(DirectMode, m_lDirectMode)
	LINK_PROPERTY_VARIANT_BOOL(DCT8x8, m_bDCT8x8)
	LINK_PROPERTY_VARIANT_BOOL(PSub16x16, m_bPSub16x16)
	LINK_PROPERTY_VARIANT_BOOL(BSub16x16, m_bBSub16x16)
	LINK_PROPERTY_VARIANT_BOOL(PSub8x8, m_bPSub8x8)
	LINK_PROPERTY_VARIANT_BOOL(I8x8, m_bI8x8)
	LINK_PROPERTY_VARIANT_BOOL(I4x4, m_bI4x4)

	LINK_PROPERTY_VARIANT_BOOL(MixedRef, m_bMixedRef)

	LINK_PROPERTY_LONG(EncodingType, m_lEncodingType)
	LINK_PROPERTY_LONG(NumPass, m_lNumPass)
	LINK_PROPERTY_VARIANT_BOOL(Fast1Pass, m_bFast1Pass)
	LINK_PROPERTY_VARIANT_BOOL(UpdateStats, m_bUpdateStats)

	LINK_PROPERTY_LONG(LevelIDC, m_lLevelIDC)
	LINK_PROPERTY_LONG(VBVMaxBitrate, m_lVBVMaxBitrate)

	LINK_PROPERTY_BSTR(StatFileName, m_sStatFileName)

	LINK_PROPERTY_VARIANT_BOOL(AppleCompatible, m_bAppleCompatible)

	LINK_PROPERTY_SAFEARRAY(ExtraData, m_parExtraData);

protected:
	long m_lDesiredQuant;
	long m_lNumThreads;
	long m_lNoiseReduction;

	long m_lRefMax;
	long m_lMinKeyInterval;
	long m_lMaxKeyInterval;
	long m_lSceneCutThreshold;
	long m_lQPMin;
	long m_lQPMax;
	long m_lQPStep;

	VARIANT_BOOL m_bDeblockingFilter;
	long m_lStrength;
	long m_lThreshold;

	
	VARIANT_BOOL m_bCabac;
	VARIANT_BOOL m_bTrellis;

	VARIANT_BOOL m_bChromaME;
	long m_lKeyframeBoost;
	long m_lBFrameReduction;
	long m_lCurveComp;

	long m_lSARX;
	long m_lSARY;

	long m_lBVOPMaxFrames;
	VARIANT_BOOL m_bBVOPWeightedBipredictional;
	VARIANT_BOOL m_bBVOPReference;
	VARIANT_BOOL m_bBVOPAdaptive;
	VARIANT_BOOL m_bBVOPBidirectionalME;
	long m_lBVOPBias;

	long m_lPartitionDecision;
	long m_lMEMethod;
	long m_lMERange;

	long m_lDirectMode;
	VARIANT_BOOL m_bDCT8x8;
	VARIANT_BOOL m_bPSub16x16;
	VARIANT_BOOL m_bBSub16x16;
	VARIANT_BOOL m_bPSub8x8;
	VARIANT_BOOL m_bI8x8;
	VARIANT_BOOL m_bI4x4;

	VARIANT_BOOL m_bMixedRef;

	long m_lEncodingType;
	long m_lNumPass;
	VARIANT_BOOL m_bFast1Pass;
	VARIANT_BOOL m_bUpdateStats;

	long m_lLevelIDC;
	long m_lVBVMaxBitrate;

	CString m_sStatFileName;

	VARIANT_BOOL m_bAppleCompatible;

	LPSAFEARRAY m_parExtraData;
};

// CAVSH264VideoFormat

[coclass, uuid("26CBBFE8-A446-4643-9FA2-42510D5DFFBC"), threading(apartment), vi_progid("AVSMediaFormatSettings3.AVSH263VideoFor"), progid("AVSMediaFormatSettings3.AVSH263VideoF.1"), version(1.0)]
class ATL_NO_VTABLE CAVSH264VideoFormat : public CAVSH264VideoFormatImpl<CAVSH264VideoFormat, IAVSH264VideoFormat>
{

};

