// AVSMPEGsVideoFormat.h : Declaration of the CAVSMPEGsVideoFormat

#pragma once
#include "resource.h"       // main symbols
//#include "ATLDefine.h"
#include "AVSVideoFormat.h"

class CAVSMPEGsVideoFormat;
// IAVSMPEGsVideoFormat
[ object, uuid("CE81DD2C-89C0-4156-BB30-1D541E26428C"), dual, pointer_default(unique)]
__interface IAVSMPEGsVideoFormat : IAVSVideoFormat
{
	[id(103000 + 1), propget] HRESULT BitrateTolerance([out, retval] long* pVal);
	[id(103000 + 1), propput] HRESULT BitrateTolerance([in] long newVal);
	[id(103000 + 2), propget] HRESULT QScale([out, retval] double* pVal);
	[id(103000 + 2), propput] HRESULT QScale([in] double newVal);
	[id(103000 + 3), propget] HRESULT QMin([out, retval] long* pVal);
	[id(103000 + 3), propput] HRESULT QMin([in] long newVal);
	[id(103000 + 4), propget] HRESULT QMax([out, retval] long* pVal);
	[id(103000 + 4), propput] HRESULT QMax([in] long newVal);
	[id(103000 + 5), propget] HRESULT LMin([out, retval] long* pVal);
	[id(103000 + 5), propput] HRESULT LMin([in] long newVal);
	[id(103000 + 6), propget] HRESULT LMax([out, retval] long* pVal);
	[id(103000 + 6), propput] HRESULT LMax([in] long newVal);
	[id(103000 + 7), propget] HRESULT MBQMin([out, retval] long* pVal);
	[id(103000 + 7), propput] HRESULT MBQMin([in] long newVal);
	[id(103000 + 8), propget] HRESULT MBQMax([out, retval] long* pVal);
	[id(103000 + 8), propput] HRESULT MBQMax([in] long newVal);
		   
	[id(103000 + 9), propget] HRESULT QDiff([out, retval] long* pVal);
	[id(103000 + 9), propput] HRESULT QDiff([in] long newVal);
	[id(103000 + 10), propget] HRESULT QBlur([out, retval] double* pVal);
	[id(103000 + 10), propput] HRESULT QBlur([in] double newVal);
	[id(103000 + 11), propget] HRESULT QComp([out, retval] double* pVal);
	[id(103000 + 11), propput] HRESULT QComp([in] double newVal);
		   
	[id(103000 + 12), propget] HRESULT RCOverrideString([out, retval] BSTR* pVal);
	[id(103000 + 12), propput] HRESULT RCOverrideString([in] BSTR newVal);
	[id(103000 + 13), propget] HRESULT RCEq([out, retval] BSTR* pVal);
	[id(103000 + 13), propput] HRESULT RCEq([in] BSTR newVal);
		   
	[id(103000 + 14), propget] HRESULT RCBufferSize([out, retval] long* pVal);
	[id(103000 + 14), propput] HRESULT RCBufferSize([in] long newVal);
	[id(103000 + 15), propget] HRESULT RCBufferAggressivity([out, retval] double* pVal);
	[id(103000 + 15), propput] HRESULT RCBufferAggressivity([in] double newVal);
	[id(103000 + 16), propget] HRESULT RCMaxRate([out, retval] long* pVal);
	[id(103000 + 16), propput] HRESULT RCMaxRate([in] long newVal);
	[id(103000 + 17), propget] HRESULT RCMinRate([out, retval] long* pVal);
	[id(103000 + 17), propput] HRESULT RCMinRate([in] long newVal);
	[id(103000 + 18), propget] HRESULT RCInitialCplx([out, retval] double* pVal);
	[id(103000 + 18), propput] HRESULT RCInitialCplx([in] double newVal);
		   
	[id(103000 + 19), propget] HRESULT BQFactor([out, retval] double* pVal);
	[id(103000 + 19), propput] HRESULT BQFactor([in] double newVal);
	[id(103000 + 20), propget] HRESULT BQOffset([out, retval] double* pVal);
	[id(103000 + 20), propput] HRESULT BQOffset([in] double newVal);
	[id(103000 + 21), propget] HRESULT IQFactor([out, retval] double* pVal);
	[id(103000 + 21), propput] HRESULT IQFactor([in] double newVal);
	[id(103000 + 22), propget] HRESULT IQOffset([out, retval] double* pVal);
	[id(103000 + 22), propput] HRESULT IQOffset([in] double newVal);
	[id(103000 + 23), propget] HRESULT IntraQuantBias([out, retval] long* pVal);
	[id(103000 + 23), propput] HRESULT IntraQuantBias([in] long newVal);
	[id(103000 + 24), propget] HRESULT InterQuantBias([out, retval] long* pVal);
	[id(103000 + 24), propput] HRESULT InterQuantBias([in] long newVal);
		   
	[id(103000 + 25), propget] HRESULT MEMethod([out, retval] long* pVal);
	[id(103000 + 25), propput] HRESULT MEMethod([in] long newVal);
	[id(103000 + 26), propget] HRESULT MBDecision([out, retval] long* pVal);
	[id(103000 + 26), propput] HRESULT MBDecision([in] long newVal);
	[id(103000 + 27), propget] HRESULT BFrames([out, retval] long* pVal);
	[id(103000 + 27), propput] HRESULT BFrames([in] long newVal);
	[id(103000 + 28), propget] HRESULT IldctCmp([out, retval] long* pVal);
	[id(103000 + 28), propput] HRESULT IldctCmp([in] long newVal);
	[id(103000 + 29), propget] HRESULT MBCmp([out, retval] long* pVal);
	[id(103000 + 29), propput] HRESULT MBCmp([in] long newVal);
	[id(103000 + 30), propget] HRESULT SubCmp([out, retval] long* pVal);
	[id(103000 + 30), propput] HRESULT SubCmp([in] long newVal);
	[id(103000 + 31), propget] HRESULT Cmp([out, retval] long* pVal);
	[id(103000 + 31), propput] HRESULT Cmp([in] long newVal);
	[id(103000 + 32), propget] HRESULT PreCmp([out, retval] long* pVal);
	[id(103000 + 32), propput] HRESULT PreCmp([in] long newVal);
	[id(103000 + 33), propget] HRESULT PreMe([out, retval] long* pVal);
	[id(103000 + 33), propput] HRESULT PreMe([in] long newVal);
		   
	[id(103000 + 34), propget] HRESULT LumiMask([out, retval] double* pVal);
	[id(103000 + 34), propput] HRESULT LumiMask([in] double newVal);
	[id(103000 + 35), propget] HRESULT DarkMask([out, retval] double* pVal);
	[id(103000 + 35), propput] HRESULT DarkMask([in] double newVal);
	[id(103000 + 36), propget] HRESULT ScplxMask([out, retval] double* pVal);
	[id(103000 + 36), propput] HRESULT ScplxMask([in] double newVal);
	[id(103000 + 37), propget] HRESULT TcplxMask([out, retval] double* pVal);
	[id(103000 + 37), propput] HRESULT TcplxMask([in] double newVal);
	[id(103000 + 38), propget] HRESULT PMask([out, retval] double* pVal);
	[id(103000 + 38), propput] HRESULT PMask([in] double newVal);
	[id(103000 + 39), propget] HRESULT Qns([out, retval] long* pVal);
	[id(103000 + 39), propput] HRESULT Qns([in] long newVal);
		   
	[id(103000 + 40), propget] HRESULT Use4mv([out, retval] VARIANT_BOOL* pVal);
	[id(103000 + 40), propput] HRESULT Use4mv([in] VARIANT_BOOL newVal);
	[id(103000 + 41), propget] HRESULT UseObmc([out, retval] VARIANT_BOOL* pVal);
	[id(103000 + 41), propput] HRESULT UseObmc([in] VARIANT_BOOL newVal);
	[id(103000 + 42), propget] HRESULT UseLoop([out, retval] VARIANT_BOOL* pVal);
	[id(103000 + 42), propput] HRESULT UseLoop([in] VARIANT_BOOL newVal);
	[id(103000 + 43), propget] HRESULT UseAic([out, retval] VARIANT_BOOL* pVal);
	[id(103000 + 43), propput] HRESULT UseAic([in] VARIANT_BOOL newVal);
	[id(103000 + 44), propget] HRESULT UseAiv([out, retval] VARIANT_BOOL* pVal);
	[id(103000 + 44), propput] HRESULT UseAiv([in] VARIANT_BOOL newVal);
	[id(103000 + 45), propget] HRESULT UseUmv([out, retval] VARIANT_BOOL* pVal);
	[id(103000 + 45), propput] HRESULT UseUmv([in] VARIANT_BOOL newVal);
	[id(103000 + 46), propget] HRESULT UseSs([out, retval] VARIANT_BOOL* pVal);
	[id(103000 + 46), propput] HRESULT UseSs([in] VARIANT_BOOL newVal);
	[id(103000 + 47), propget] HRESULT UseAltScan([out, retval] VARIANT_BOOL* pVal);
	[id(103000 + 47), propput] HRESULT UseAltScan([in] VARIANT_BOOL newVal);
	[id(103000 + 48), propget] HRESULT UseTrell([out, retval] VARIANT_BOOL* pVal);
	[id(103000 + 48), propput] HRESULT UseTrell([in] VARIANT_BOOL newVal);
	[id(103000 + 49), propget] HRESULT UseScanOffset([out, retval] VARIANT_BOOL* pVal);
	[id(103000 + 49), propput] HRESULT UseScanOffset([in] VARIANT_BOOL newVal);
	[id(103000 + 50), propget] HRESULT UseQpel([out, retval] VARIANT_BOOL* pVal);
	[id(103000 + 50), propput] HRESULT UseQpel([in] VARIANT_BOOL newVal);
	[id(103000 + 51), propget] HRESULT UseQprd([out, retval] VARIANT_BOOL* pVal);
	[id(103000 + 51), propput] HRESULT UseQprd([in] VARIANT_BOOL newVal);
	[id(103000 + 52), propget] HRESULT UseCbprd([out, retval] VARIANT_BOOL* pVal);
	[id(103000 + 52), propput] HRESULT UseCbprd([in] VARIANT_BOOL newVal);
	[id(103000 + 53), propget] HRESULT ClosedGop([out, retval] VARIANT_BOOL* pVal);
	[id(103000 + 53), propput] HRESULT ClosedGop([in] VARIANT_BOOL newVal);
	[id(103000 + 54), propget] HRESULT DoInterlaceDct([out, retval] VARIANT_BOOL* pVal);
	[id(103000 + 54), propput] HRESULT DoInterlaceDct([in] VARIANT_BOOL newVal);
	[id(103000 + 55), propget] HRESULT DoInterlaceME([out, retval] VARIANT_BOOL* pVal);
	[id(103000 + 55), propput] HRESULT DoInterlaceME([in] VARIANT_BOOL newVal);
	[id(103000 + 56), propget] HRESULT UsePart([out, retval] VARIANT_BOOL* pVal);
	[id(103000 + 56), propput] HRESULT UsePart([in] VARIANT_BOOL newVal);
			
	[id(103000 + 57), propget] HRESULT DctAlgo([out, retval] long* pVal);
	[id(103000 + 57), propput] HRESULT DctAlgo([in] long newVal);
	[id(103000 + 58), propget] HRESULT IdctAlgo([out, retval] long* pVal);
	[id(103000 + 58), propput] HRESULT IdctAlgo([in] long newVal);
	[id(103000 + 59), propget] HRESULT PacketSize([out, retval] long* pVal);
	[id(103000 + 59), propput] HRESULT PacketSize([in] long newVal);
	[id(103000 + 60), propget] HRESULT ErrorRate([out, retval] long* pVal);
	[id(103000 + 60), propput] HRESULT ErrorRate([in] long newVal);
	[id(103000 + 61), propget] HRESULT Strict([out, retval] long* pVal);
	[id(103000 + 61), propput] HRESULT Strict([in] long newVal);
	[id(103000 + 62), propget] HRESULT NoiseReduction([out, retval] long* pVal);
	[id(103000 + 62), propput] HRESULT NoiseReduction([in] long newVal);
	[id(103000 + 63), propget] HRESULT ScThreshold([out, retval] long* pVal);
	[id(103000 + 63), propput] HRESULT ScThreshold([in] long newVal);
	[id(103000 + 64), propget] HRESULT METhreshold([out, retval] long* pVal);
	[id(103000 + 64), propput] HRESULT METhreshold([in] long newVal);
	[id(103000 + 65), propget] HRESULT MBThreshold([out, retval] long* pVal);
	[id(103000 + 65), propput] HRESULT MBThreshold([in] long newVal);
	[id(103000 + 66), propget] HRESULT IntraDcPrecision([out, retval] long* pVal);
	[id(103000 + 66), propput] HRESULT IntraDcPrecision([in] long newVal);
	[id(103000 + 67), propget] HRESULT Coder([out, retval] long* pVal);
	[id(103000 + 67), propput] HRESULT Coder([in] long newVal);
	[id(103000 + 68), propget] HRESULT Context([out, retval] long* pVal);
	[id(103000 + 68), propput] HRESULT Context([in] long newVal);
	[id(103000 + 69), propget] HRESULT Predictor([out, retval] long* pVal);
	[id(103000 + 69), propput] HRESULT Predictor([in] long newVal);
	[id(103000 + 70), propget] HRESULT MERange([out, retval] long* pVal);
	[id(103000 + 70), propput] HRESULT MERange([in] long newVal);
			
	[id(103000 + 71), propget] HRESULT GopSize([out, retval] long* pVal);
	[id(103000 + 71), propput] HRESULT GopSize([in] long newVal);
	[id(103000 + 72), propget] HRESULT ThreadCount([out, retval] long* pVal);
	[id(103000 + 72), propput] HRESULT ThreadCount([in] long newVal);
			
	[id(103000 + 73), propget] HRESULT DoPass([out, retval] long* pVal);
	[id(103000 + 73), propput] HRESULT DoPass([in] long newVal);
	[id(103000 + 74), propget] HRESULT Bitexact([out, retval] VARIANT_BOOL* pVal);
	[id(103000 + 74), propput] HRESULT Bitexact([in] VARIANT_BOOL newVal);
	[id(103000 + 75), propget] HRESULT PassLogfilename([out, retval] BSTR* pVal);
	[id(103000 + 75), propput] HRESULT PassLogfilename([in] BSTR newVal);
			
	[id(103000 + 76), propget] HRESULT ExtraData([out, retval, satype("BYTE")] SAFEARRAY** pVal);
	[id(103000 + 76), propput] HRESULT ExtraData([in, satype("BYTE")] SAFEARRAY* newVal);
};

template <class CoT, class T>
class CAVSMPEGsVideoFormatImpl : public CAVSVideoFormatImpl<CoT, T>
{
public:
	CAVSMPEGsVideoFormatImpl()
	{
		m_lMediaType = MEDIAFORMAT_MPEGs;
		m_parExtraData = NULL;
		m_BitsPerSample=24;
		SetDefaultProperties();
	}
	~CAVSMPEGsVideoFormatImpl()
	{
		RELEASEARRAY(m_parExtraData);
	}
	STDMETHOD(CreateDuplicate)(IAVSMediaFormat** pMediaFormat)
	{
		CAVSVideoFormatImpl<CoT, T>::CreateDuplicate(pMediaFormat);
		if (NULL==(*pMediaFormat))
			return S_OK;

		IAVSMPEGsVideoFormat* pAVSMPEGsFormat=NULL;
		(*pMediaFormat)->QueryInterface(__uuidof(IAVSMPEGsVideoFormat), (void**)&pAVSMPEGsFormat);
		if (NULL==pAVSMPEGsFormat)
		{
			(*pMediaFormat)->Release();
			*pMediaFormat = NULL;
			return S_OK;
		}

		pAVSMPEGsFormat->put_BitrateTolerance(m_lBitrateTolerance);
		pAVSMPEGsFormat->put_QScale(m_dQScale);
		pAVSMPEGsFormat->put_QMin(m_lQMin);
		pAVSMPEGsFormat->put_QMax(m_lQMax);
		pAVSMPEGsFormat->put_LMin(m_lLMin);
		pAVSMPEGsFormat->put_LMax(m_lLMax);
		pAVSMPEGsFormat->put_MBQMin(m_lMBQMin);
		pAVSMPEGsFormat->put_MBQMax(m_lMBQMax);
		
		pAVSMPEGsFormat->put_QDiff(m_lQDiff);
		pAVSMPEGsFormat->put_QBlur(m_dQBlur);
		pAVSMPEGsFormat->put_QComp(m_dQComp);
		
		BSTR bsTemp = m_sRCOverrideString.AllocSysString();
		pAVSMPEGsFormat->put_RCOverrideString(bsTemp);
		SysFreeString(bsTemp);

		bsTemp = m_sRCEq.AllocSysString();
		pAVSMPEGsFormat->put_RCOverrideString(bsTemp);
		SysFreeString(bsTemp);

		pAVSMPEGsFormat->put_RCBufferSize(m_lRCBufferSize);
		pAVSMPEGsFormat->put_RCBufferAggressivity(m_dRCBufferAggressivity);
		pAVSMPEGsFormat->put_RCMaxRate(m_lRCMaxRate);
		pAVSMPEGsFormat->put_RCMinRate(m_lRCMinRate);
		pAVSMPEGsFormat->put_RCInitialCplx(m_dRCInitialCplx);
		
		pAVSMPEGsFormat->put_BQFactor(m_dBQFactor);
		pAVSMPEGsFormat->put_BQOffset(m_dBQOffset);
		pAVSMPEGsFormat->put_IQFactor(m_dIQFactor);
		pAVSMPEGsFormat->put_IQOffset(m_dIQOffset);
		pAVSMPEGsFormat->put_IntraQuantBias(m_lIntraQuantBias);
		pAVSMPEGsFormat->put_InterQuantBias(m_lInterQuantBias);

		pAVSMPEGsFormat->put_MEMethod(m_lMEMethod);
		pAVSMPEGsFormat->put_MBDecision(m_lMBDecision);
		pAVSMPEGsFormat->put_BFrames(m_lBFrames);
		pAVSMPEGsFormat->put_IldctCmp(m_lIldctCmp);
		pAVSMPEGsFormat->put_MBCmp(m_lMBCmp);
		pAVSMPEGsFormat->put_SubCmp(m_lSubCmp);
		pAVSMPEGsFormat->put_Cmp(m_lCmp);
		pAVSMPEGsFormat->put_PreCmp(m_lPreCmp);
		pAVSMPEGsFormat->put_PreMe(m_lPreMe);
		
		pAVSMPEGsFormat->put_LumiMask(m_dLumiMask);
		pAVSMPEGsFormat->put_DarkMask(m_dDarkMask);
		pAVSMPEGsFormat->put_ScplxMask(m_dScplxMask);
		pAVSMPEGsFormat->put_TcplxMask(m_dTcplxMask);
		pAVSMPEGsFormat->put_PMask(m_dPMask);
		pAVSMPEGsFormat->put_Qns(m_lQns);
		
		pAVSMPEGsFormat->put_Use4mv((m_bUse4mv ? VARIANT_TRUE : VARIANT_FALSE));
		pAVSMPEGsFormat->put_UseObmc((m_bUseObmc ? VARIANT_TRUE : VARIANT_FALSE));
		pAVSMPEGsFormat->put_UseLoop((m_bUseLoop ? VARIANT_TRUE : VARIANT_FALSE));
		pAVSMPEGsFormat->put_UseAic((m_bUseAic ? VARIANT_TRUE : VARIANT_FALSE));
		pAVSMPEGsFormat->put_UseAiv((m_bUseAiv ? VARIANT_TRUE : VARIANT_FALSE));
		pAVSMPEGsFormat->put_UseUmv((m_bUseUmv ? VARIANT_TRUE : VARIANT_FALSE));
		pAVSMPEGsFormat->put_UseSs((m_bUseSs ? VARIANT_TRUE : VARIANT_FALSE));
		pAVSMPEGsFormat->put_UseAltScan((m_bUseAltScan ? VARIANT_TRUE : VARIANT_FALSE));
		pAVSMPEGsFormat->put_UseTrell((m_bUseTrell ? VARIANT_TRUE : VARIANT_FALSE));
		pAVSMPEGsFormat->put_UseScanOffset((m_bUseScanOffset ? VARIANT_TRUE : VARIANT_FALSE));
		pAVSMPEGsFormat->put_UseQpel((m_bUseQpel ? VARIANT_TRUE : VARIANT_FALSE));
		pAVSMPEGsFormat->put_UseQprd((m_bUseQprd ? VARIANT_TRUE : VARIANT_FALSE));
		pAVSMPEGsFormat->put_UseCbprd((m_bUseCbprd ? VARIANT_TRUE : VARIANT_FALSE));
		pAVSMPEGsFormat->put_ClosedGop((m_bClosedGop ? VARIANT_TRUE : VARIANT_FALSE));
		pAVSMPEGsFormat->put_DoInterlaceDct((m_bDoInterlaceDct ? VARIANT_TRUE : VARIANT_FALSE));
		pAVSMPEGsFormat->put_DoInterlaceME((m_bDoInterlaceME ? VARIANT_TRUE : VARIANT_FALSE));
		pAVSMPEGsFormat->put_UsePart((m_bUsePart ? VARIANT_TRUE : VARIANT_FALSE));

		pAVSMPEGsFormat->put_DctAlgo(m_lDctAlgo);
		pAVSMPEGsFormat->put_IdctAlgo(m_lIdctAlgo);
		pAVSMPEGsFormat->put_PacketSize(m_lPacketSize);
		pAVSMPEGsFormat->put_ErrorRate(m_lErrorRate);
		pAVSMPEGsFormat->put_Strict(m_lStrict);
		pAVSMPEGsFormat->put_NoiseReduction(m_lNoiseReduction);
		pAVSMPEGsFormat->put_ScThreshold(m_lScThreshold);
		pAVSMPEGsFormat->put_METhreshold(m_lMEThreshold);
		pAVSMPEGsFormat->put_MBThreshold(m_lMBThreshold);
		pAVSMPEGsFormat->put_IntraDcPrecision(m_lIntraDcPrecision);
		pAVSMPEGsFormat->put_Coder(m_lCoder);
		pAVSMPEGsFormat->put_Context(m_lContext);
		pAVSMPEGsFormat->put_Predictor(m_lPredictor);
		pAVSMPEGsFormat->put_MERange(m_lMERange);

		pAVSMPEGsFormat->put_GopSize(m_lGopSize);
		pAVSMPEGsFormat->put_ThreadCount(m_lThreadCount);
		pAVSMPEGsFormat->put_DoPass(m_lDoPass);
		pAVSMPEGsFormat->put_Bitexact((m_bBitexact ? VARIANT_TRUE : VARIANT_FALSE));

		bsTemp = m_sPassLogfilename.AllocSysString();
		pAVSMPEGsFormat->put_PassLogfilename(bsTemp);
		SysFreeString(bsTemp);
		
		pAVSMPEGsFormat->put_ExtraData(m_parExtraData);

		VARIANT var;
		var.vt=VT_I4;
		var.lVal=m_BitsPerSample;
		pAVSMPEGsFormat->SetAdditionalParam(L"BitsPerSample",var);

		pAVSMPEGsFormat->Release();
		return S_OK;
	}
	STDMETHOD(SetDefaultProperties)(void)
	{
		CAVSVideoFormatImpl<CoT, T>::SetDefaultProperties();
		
		m_lBitrateTolerance = 4000;
		m_dQScale = 0;
		m_lQMin = 2;
		m_lQMax = 31;
		m_lLMin = 2;
		m_lLMax = 31;
		m_lMBQMin = 2;
		m_lMBQMax = 31;
		
		m_lQDiff = 3;
		m_dQBlur = 0.5;
		m_dQComp = 0.5;
		
		m_sRCOverrideString = "";
		m_sRCEq = "tex^qComp";
		
		m_lRCBufferSize = 0;
		m_dRCBufferAggressivity = 1.0;
		m_lRCMaxRate = 0;
		m_lRCMinRate = 0;
		m_dRCInitialCplx = 0.0;
		
		m_dBQFactor = 1.25;
		m_dBQOffset = 1.25;
		m_dIQFactor = -0.8;
		m_dIQOffset = 0.0;
		m_lIntraQuantBias = 999999;
		m_lInterQuantBias = 999999;

		m_lMEMethod = 5;
		m_lMBDecision = 0;
		m_lBFrames = 0;
		m_lIldctCmp = 8;
		m_lMBCmp = 0;
		m_lSubCmp = 0;
		m_lCmp = 0;
		m_lPreCmp = 0;
		m_lPreMe = 0;
		
		m_dLumiMask = 0.0;
		m_dDarkMask = 0.0;
		m_dScplxMask = 0.0;
		m_dTcplxMask = 0.0;
		m_dPMask = 0.0;
		m_lQns = 0;
		
		m_bUse4mv = FALSE;
		m_bUseObmc = FALSE;
		m_bUseLoop = FALSE;
		m_bUseAic = FALSE;
		m_bUseAiv = FALSE;
		m_bUseUmv = FALSE;
		m_bUseSs = FALSE;
		m_bUseAltScan = FALSE;
		m_bUseTrell = FALSE;
		m_bUseScanOffset = FALSE;
		m_bUseQpel = FALSE;
		m_bUseQprd = FALSE;
		m_bUseCbprd = FALSE;
		m_bClosedGop = FALSE;
		m_bDoInterlaceDct = FALSE;
		m_bDoInterlaceME = FALSE;
		m_bUsePart = FALSE;

		m_lDctAlgo = 0;
		m_lIdctAlgo = 0;
		m_lPacketSize = 0;
		m_lErrorRate = 0;
		m_lStrict = 0;
		m_lNoiseReduction = 0;
		m_lScThreshold = 0;
		m_lMEThreshold = 0;
		m_lMBThreshold = 0;
		m_lIntraDcPrecision = 8;
		m_lCoder = 0;
		m_lContext = 0;
		m_lPredictor = 0;
		m_lMERange = 0;

		m_lGopSize  = 30;
		m_lThreadCount = 1;
		m_lDoPass = 0;
		m_bBitexact = FALSE;
		m_sPassLogfilename = "c:/avspass.log";
		
		RELEASEARRAY(m_parExtraData);
		return S_OK;
	}
	STDMETHOD(SetPropertiesByFormat)(IAVSMediaFormat* pMediaFormat)
	{
		CAVSVideoFormatImpl<CoT, T>::SetPropertiesByFormat(pMediaFormat);
		if (pMediaFormat!=NULL)
		{
			IAVSMPEGsVideoFormat*	pAVSMPEGsFormat=NULL;
			pMediaFormat->QueryInterface(__uuidof(IAVSMPEGsVideoFormat), (void**)&pAVSMPEGsFormat);
			if (pAVSMPEGsFormat!=NULL)
			{
				pAVSMPEGsFormat->get_BitrateTolerance(&m_lBitrateTolerance);
				pAVSMPEGsFormat->get_QScale(&m_dQScale);
				pAVSMPEGsFormat->get_QMin(&m_lQMin);
				pAVSMPEGsFormat->get_QMax(&m_lQMax);
				pAVSMPEGsFormat->get_LMin(&m_lLMin);
				pAVSMPEGsFormat->get_LMax(&m_lLMax);
				pAVSMPEGsFormat->get_MBQMin(&m_lMBQMin);
				pAVSMPEGsFormat->get_MBQMax(&m_lMBQMax);
				
				pAVSMPEGsFormat->get_QDiff(&m_lQDiff);
				pAVSMPEGsFormat->get_QBlur(&m_dQBlur);
				pAVSMPEGsFormat->get_QComp(&m_dQComp);
				
				BSTR	bsTemp = NULL;
				pAVSMPEGsFormat->get_RCOverrideString(&bsTemp);
				m_sRCOverrideString=bsTemp;
				RELEASESYSSTRING(bsTemp);

				pAVSMPEGsFormat->get_RCOverrideString(&bsTemp);
				m_sRCEq=bsTemp;
				RELEASESYSSTRING(bsTemp);

				pAVSMPEGsFormat->get_RCBufferSize(&m_lRCBufferSize);
				pAVSMPEGsFormat->get_RCBufferAggressivity(&m_dRCBufferAggressivity);
				pAVSMPEGsFormat->get_RCMaxRate(&m_lRCMaxRate);
				pAVSMPEGsFormat->get_RCMinRate(&m_lRCMinRate);
				pAVSMPEGsFormat->get_RCInitialCplx(&m_dRCInitialCplx);
				
				pAVSMPEGsFormat->get_BQFactor(&m_dBQFactor);
				pAVSMPEGsFormat->get_BQOffset(&m_dBQOffset);
				pAVSMPEGsFormat->get_IQFactor(&m_dIQFactor);
				pAVSMPEGsFormat->get_IQOffset(&m_dIQOffset);
				pAVSMPEGsFormat->get_IntraQuantBias(&m_lIntraQuantBias);
				pAVSMPEGsFormat->get_InterQuantBias(&m_lInterQuantBias);

				pAVSMPEGsFormat->get_MEMethod(&m_lMEMethod);
				pAVSMPEGsFormat->get_MBDecision(&m_lMBDecision);
				pAVSMPEGsFormat->get_BFrames(&m_lBFrames);
				pAVSMPEGsFormat->get_IldctCmp(&m_lIldctCmp);
				pAVSMPEGsFormat->get_MBCmp(&m_lMBCmp);
				pAVSMPEGsFormat->get_SubCmp(&m_lSubCmp);
				pAVSMPEGsFormat->get_Cmp(&m_lCmp);
				pAVSMPEGsFormat->get_PreCmp(&m_lPreCmp);
				pAVSMPEGsFormat->get_PreMe(&m_lPreMe);
				
				pAVSMPEGsFormat->get_LumiMask(&m_dLumiMask);
				pAVSMPEGsFormat->get_DarkMask(&m_dDarkMask);
				pAVSMPEGsFormat->get_ScplxMask(&m_dScplxMask);
				pAVSMPEGsFormat->get_TcplxMask(&m_dTcplxMask);
				pAVSMPEGsFormat->get_PMask(&m_dPMask);
				pAVSMPEGsFormat->get_Qns(&m_lQns);
				
				pAVSMPEGsFormat->get_Use4mv(&m_bUse4mv);
				pAVSMPEGsFormat->get_UseObmc(&m_bUseObmc);
				pAVSMPEGsFormat->get_UseLoop(&m_bUseLoop);
				pAVSMPEGsFormat->get_UseAic(&m_bUseAic);
				pAVSMPEGsFormat->get_UseAiv(&m_bUseAiv);
				pAVSMPEGsFormat->get_UseUmv(&m_bUseUmv);
				pAVSMPEGsFormat->get_UseSs(&m_bUseSs);
				pAVSMPEGsFormat->get_UseAltScan(&m_bUseAltScan);
				pAVSMPEGsFormat->get_UseTrell(&m_bUseTrell);
				pAVSMPEGsFormat->get_UseScanOffset(&m_bUseScanOffset);
				pAVSMPEGsFormat->get_UseQpel(&m_bUseQpel);
				pAVSMPEGsFormat->get_UseQprd(&m_bUseQprd);
				pAVSMPEGsFormat->get_UseCbprd(&m_bUseCbprd);
				pAVSMPEGsFormat->get_ClosedGop(&m_bClosedGop);
				pAVSMPEGsFormat->get_DoInterlaceDct(&m_bDoInterlaceDct);
				pAVSMPEGsFormat->get_DoInterlaceME(&m_bDoInterlaceME);
				pAVSMPEGsFormat->get_UsePart(&m_bUsePart);

				pAVSMPEGsFormat->get_DctAlgo(&m_lDctAlgo);
				pAVSMPEGsFormat->get_IdctAlgo(&m_lIdctAlgo);
				pAVSMPEGsFormat->get_PacketSize(&m_lPacketSize);
				pAVSMPEGsFormat->get_ErrorRate(&m_lErrorRate);
				pAVSMPEGsFormat->get_Strict(&m_lStrict);
				pAVSMPEGsFormat->get_NoiseReduction(&m_lNoiseReduction);
				pAVSMPEGsFormat->get_ScThreshold(&m_lScThreshold);
				pAVSMPEGsFormat->get_METhreshold(&m_lMEThreshold);
				pAVSMPEGsFormat->get_MBThreshold(&m_lMBThreshold);
				pAVSMPEGsFormat->get_IntraDcPrecision(&m_lIntraDcPrecision);
				pAVSMPEGsFormat->get_Coder(&m_lCoder);
				pAVSMPEGsFormat->get_Context(&m_lContext);
				pAVSMPEGsFormat->get_Predictor(&m_lPredictor);
				pAVSMPEGsFormat->get_MERange(&m_lMERange);

				pAVSMPEGsFormat->get_GopSize(&m_lGopSize);
				pAVSMPEGsFormat->get_ThreadCount(&m_lThreadCount);
				pAVSMPEGsFormat->get_DoPass(&m_lDoPass);
				pAVSMPEGsFormat->get_Bitexact(&m_bBitexact);

				pAVSMPEGsFormat->get_PassLogfilename(&bsTemp);
				m_sPassLogfilename=bsTemp;
				RELEASESYSSTRING(bsTemp);
				
				RELEASEARRAY(m_parExtraData);
				pAVSMPEGsFormat->get_ExtraData(&m_parExtraData);

				RELEASEINTERFACE(pAVSMPEGsFormat);
			}
			else if( MJPGHandler == m_ulfccHandler )
			{
				if( ( m_lBitrate / m_dFrameRate ) > m_lBitrateTolerance )
				{
					m_lBitrateTolerance = m_lBitrate / 1000;
				}
			}
		}
		return S_OK;
	}
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		CString Param( ParamName );
		if ( L"BitsPerSample" == Param )
		{
			m_BitsPerSample = ParamValue.lVal;
		}

		return CAVSVideoFormatImpl<CoT, T>::SetAdditionalParam(ParamName, ParamValue);
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		CString Param( ParamName );
		if ( L"BitsPerSample" == Param )
		{
			ParamValue->vt = VT_I4;
			ParamValue->lVal = m_BitsPerSample;
		}
		return CAVSVideoFormatImpl<CoT, T>::GetAdditionalParam(ParamName, ParamValue);
	}
	//////	
	LINK_PROPERTY_LONG(BitrateTolerance, m_lBitrateTolerance);
	LINK_PROPERTY_DOUBLE(QScale, m_dQScale);
	LINK_PROPERTY_LONG(QMin, m_lQMin);
	LINK_PROPERTY_LONG(QMax, m_lQMax);
	LINK_PROPERTY_LONG(LMin, m_lLMin);
	LINK_PROPERTY_LONG(LMax, m_lLMax);
	LINK_PROPERTY_LONG(MBQMin, m_lMBQMin);
	LINK_PROPERTY_LONG(MBQMax, m_lMBQMax);
	
	LINK_PROPERTY_LONG(QDiff, m_lQDiff);
	LINK_PROPERTY_DOUBLE(QBlur, m_dQBlur);
	LINK_PROPERTY_DOUBLE(QComp, m_dQComp);
	
	LINK_PROPERTY_BSTR(RCOverrideString, m_sRCOverrideString);
	LINK_PROPERTY_BSTR(RCEq, m_sRCEq);
	
	LINK_PROPERTY_LONG(RCBufferSize, m_lRCBufferSize);
	LINK_PROPERTY_DOUBLE(RCBufferAggressivity, m_dRCBufferAggressivity);
	LINK_PROPERTY_LONG(RCMaxRate, m_lRCMaxRate);
	LINK_PROPERTY_LONG(RCMinRate, m_lRCMinRate);
	LINK_PROPERTY_DOUBLE(RCInitialCplx, m_dRCInitialCplx);
	
	LINK_PROPERTY_DOUBLE(BQFactor, m_dBQFactor);
	LINK_PROPERTY_DOUBLE(BQOffset, m_dBQOffset);
	LINK_PROPERTY_DOUBLE(IQFactor, m_dIQFactor);
	LINK_PROPERTY_DOUBLE(IQOffset, m_dIQOffset);
	LINK_PROPERTY_LONG(IntraQuantBias, m_lIntraQuantBias);
	LINK_PROPERTY_LONG(InterQuantBias, m_lInterQuantBias);

	LINK_PROPERTY_LONG(MEMethod, m_lMEMethod);
	LINK_PROPERTY_LONG(MBDecision, m_lMBDecision);
	LINK_PROPERTY_LONG(BFrames, m_lBFrames);
	LINK_PROPERTY_LONG(IldctCmp, m_lIldctCmp);
	LINK_PROPERTY_LONG(MBCmp, m_lMBCmp);
	LINK_PROPERTY_LONG(SubCmp, m_lSubCmp);
	LINK_PROPERTY_LONG(Cmp, m_lCmp);
	LINK_PROPERTY_LONG(PreCmp, m_lPreCmp);
	LINK_PROPERTY_LONG(PreMe, m_lPreMe);
	
	LINK_PROPERTY_DOUBLE(LumiMask, m_dLumiMask);
	LINK_PROPERTY_DOUBLE(DarkMask, m_dDarkMask);
	LINK_PROPERTY_DOUBLE(ScplxMask, m_dScplxMask);
	LINK_PROPERTY_DOUBLE(TcplxMask, m_dTcplxMask);
	LINK_PROPERTY_DOUBLE(PMask, m_dPMask);
	LINK_PROPERTY_LONG(Qns, m_lQns);
	
	LINK_PROPERTY_VARIANT_BOOL(Use4mv, m_bUse4mv);
	LINK_PROPERTY_VARIANT_BOOL(UseObmc, m_bUseObmc);
	LINK_PROPERTY_VARIANT_BOOL(UseLoop, m_bUseLoop);
	LINK_PROPERTY_VARIANT_BOOL(UseAic, m_bUseAic);
	LINK_PROPERTY_VARIANT_BOOL(UseAiv, m_bUseAiv);
	LINK_PROPERTY_VARIANT_BOOL(UseUmv, m_bUseUmv);
	LINK_PROPERTY_VARIANT_BOOL(UseSs, m_bUseSs);
	LINK_PROPERTY_VARIANT_BOOL(UseAltScan, m_bUseAltScan);
	LINK_PROPERTY_VARIANT_BOOL(UseTrell, m_bUseTrell);
	LINK_PROPERTY_VARIANT_BOOL(UseScanOffset, m_bUseScanOffset);
	LINK_PROPERTY_VARIANT_BOOL(UseQpel, m_bUseQpel);
	LINK_PROPERTY_VARIANT_BOOL(UseQprd, m_bUseQprd);
	LINK_PROPERTY_VARIANT_BOOL(UseCbprd, m_bUseCbprd);
	LINK_PROPERTY_VARIANT_BOOL(ClosedGop, m_bClosedGop);
	LINK_PROPERTY_VARIANT_BOOL(DoInterlaceDct, m_bDoInterlaceDct);
	LINK_PROPERTY_VARIANT_BOOL(DoInterlaceME, m_bDoInterlaceME);
	LINK_PROPERTY_VARIANT_BOOL(UsePart, m_bUsePart);

	LINK_PROPERTY_LONG(DctAlgo, m_lDctAlgo);
	LINK_PROPERTY_LONG(IdctAlgo, m_lIdctAlgo);
	LINK_PROPERTY_LONG(PacketSize, m_lPacketSize);
	LINK_PROPERTY_LONG(ErrorRate, m_lErrorRate);
	LINK_PROPERTY_LONG(Strict, m_lStrict);
	LINK_PROPERTY_LONG(NoiseReduction, m_lNoiseReduction);
	LINK_PROPERTY_LONG(ScThreshold, m_lScThreshold);
	LINK_PROPERTY_LONG(METhreshold, m_lMEThreshold);
	LINK_PROPERTY_LONG(MBThreshold, m_lMBThreshold);
	LINK_PROPERTY_LONG(IntraDcPrecision, m_lIntraDcPrecision);
	LINK_PROPERTY_LONG(Coder, m_lCoder);
	LINK_PROPERTY_LONG(Context, m_lContext);
	LINK_PROPERTY_LONG(Predictor, m_lPredictor);
	LINK_PROPERTY_LONG(MERange, m_lMERange);

	LINK_PROPERTY_LONG(GopSize, m_lGopSize);
	LINK_PROPERTY_LONG(ThreadCount, m_lThreadCount);
	LINK_PROPERTY_LONG(DoPass, m_lDoPass);
	LINK_PROPERTY_VARIANT_BOOL(Bitexact, m_bBitexact);
	LINK_PROPERTY_BSTR(PassLogfilename, m_sPassLogfilename);
	
	LINK_PROPERTY_SAFEARRAY(ExtraData, m_parExtraData);

protected:
	
	long m_lBitrateTolerance;
	double m_dQScale;
	long m_lQMin;
	long m_lQMax;
	long m_lLMin;
	long m_lLMax;
	long m_lMBQMin;
	long m_lMBQMax;
	
	long m_lQDiff;
	double m_dQBlur;
	double m_dQComp;
	
	CString m_sRCOverrideString;
	CString m_sRCEq;
	
	long m_lRCBufferSize;
	double m_dRCBufferAggressivity;
	long m_lRCMaxRate;
	long m_lRCMinRate;
	double m_dRCInitialCplx;
	
	double m_dBQFactor;
	double m_dBQOffset;
	double m_dIQFactor;
	double m_dIQOffset;
	long m_lIntraQuantBias;
	long m_lInterQuantBias;

	long m_lMEMethod;
	long m_lMBDecision;
	long m_lBFrames;
	long m_lIldctCmp;
	long m_lMBCmp;
	long m_lSubCmp;
	long m_lCmp;
	long m_lPreCmp;
	long m_lPreMe;
	
	double m_dLumiMask;
	double m_dDarkMask;
	double m_dScplxMask;
	double m_dTcplxMask;
	double m_dPMask;
	long m_lQns;
	
	VARIANT_BOOL m_bUse4mv;
	VARIANT_BOOL m_bUseObmc;
	VARIANT_BOOL m_bUseLoop;
	VARIANT_BOOL m_bUseAic;
	VARIANT_BOOL m_bUseAiv;
	VARIANT_BOOL m_bUseUmv;
	VARIANT_BOOL m_bUseSs;
	VARIANT_BOOL m_bUseAltScan;
	VARIANT_BOOL m_bUseTrell;
	VARIANT_BOOL m_bUseScanOffset;
	VARIANT_BOOL m_bUseQpel;
	VARIANT_BOOL m_bUseQprd;
	VARIANT_BOOL m_bUseCbprd;
	VARIANT_BOOL m_bClosedGop;
	VARIANT_BOOL m_bDoInterlaceDct;
	VARIANT_BOOL m_bDoInterlaceME;
	VARIANT_BOOL m_bUsePart;
//
	long m_BitsPerSample;
	long m_lDctAlgo;
	long m_lIdctAlgo;
	long m_lPacketSize;
	long m_lErrorRate;
	long m_lStrict;
	long m_lNoiseReduction;
	long m_lScThreshold;
	long m_lMEThreshold;
	long m_lMBThreshold;
	long m_lIntraDcPrecision;
	long m_lCoder;
	long m_lContext;
	long m_lPredictor;
	long m_lMERange;

	long m_lGopSize;
	long m_lThreadCount;
	long m_lDoPass;
	VARIANT_BOOL m_bBitexact;
	CString m_sPassLogfilename;
	
	LPSAFEARRAY m_parExtraData;
};


// CAVSMPEGsVideoFormat

[coclass, uuid("7CB4F1BD-4FA2-498A-ABD2-AA7B0F560718"), threading(apartment), vi_progid("AVSMediaFormatSettings3.AVSMPEGsVideoFo"), progid("AVSMediaFormatSettings3.AVSMPEGsVideo.1"), version(1.0)]
class ATL_NO_VTABLE CAVSMPEGsVideoFormat : public CAVSMPEGsVideoFormatImpl<CAVSMPEGsVideoFormat, IAVSMPEGsVideoFormat>
{};

