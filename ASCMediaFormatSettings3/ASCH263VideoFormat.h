// ASCH263VideoFormat.h : Declaration of the CASCH263VideoFormat

#pragma once
#include "resource.h"       // main symbols
#include "ATLDefine.h"
#include "ASCVideoFormat.h"

class CAVSH263VideoFormat;
// IAVSH263VideoFormat
[ object, uuid("1D253F25-E711-4EF9-B907-3D35CBF2B0AE"), dual, pointer_default(unique)]
__interface IAVSH263VideoFormat : IAVSVideoFormat
{
	[id(106000 + 1), propget] HRESULT QuantI([out, retval] LONG* pVal);
	[id(106000 + 1), propput] HRESULT QuantI([in] LONG newVal);
	[id(106000 + 2), propget] HRESULT QuantP([out, retval] LONG* pVal);
	[id(106000 + 2), propput] HRESULT QuantP([in] LONG newVal);
	[id(106000 + 3), propget] HRESULT QuantB([out, retval] LONG* pVal);
	[id(106000 + 3), propput] HRESULT QuantB([in] LONG newVal);
	[id(106000 + 4), propget] HRESULT QuantEI_EP([out, retval] LONG* pVal);
	[id(106000 + 4), propput] HRESULT QuantEI_EP([in] LONG newVal);
		   
	[id(106000 + 5), propget] HRESULT RateControlMethod([out, retval] LONG* pVal);
	[id(106000 + 5), propput] HRESULT RateControlMethod([in] LONG newVal);
	[id(106000 + 6), propget] HRESULT UseGOBSync([out, retval] VARIANT_BOOL* pVal);
	[id(106000 + 6), propput] HRESULT UseGOBSync([in] VARIANT_BOOL newVal);
	[id(106000 + 7), propget] HRESULT Distance_I_I([out, retval] LONG* pVal);
	[id(106000 + 7), propput] HRESULT Distance_I_I([in] LONG newVal);
	[id(106000 + 8), propget] HRESULT Distance_I_P([out, retval] LONG* pVal);
	[id(106000 + 8), propput] HRESULT Distance_I_P([in] LONG newVal);
	[id(106000 + 9), propget] HRESULT Distance_MB([out, retval] LONG* pVal);
	[id(106000 + 9), propput] HRESULT Distance_MB([in] LONG newVal);
		   
	[id(106000 + 10), propget] HRESULT UMVMode([out, retval] LONG* pVal);
	[id(106000 + 10), propput] HRESULT UMVMode([in] LONG newVal);
		   
	[id(106000 + 11), propget] HRESULT UseSintaxArithCoding([out, retval] VARIANT_BOOL* pVal);
	[id(106000 + 11), propput] HRESULT UseSintaxArithCoding([in] VARIANT_BOOL newVal);
	[id(106000 + 12), propget] HRESULT UseAdvancedPrediction([out, retval] VARIANT_BOOL* pVal);
	[id(106000 + 12), propput] HRESULT UseAdvancedPrediction([in] VARIANT_BOOL newVal);
	[id(106000 + 13), propget] HRESULT UsePBFrames([out, retval] VARIANT_BOOL* pVal);
	[id(106000 + 13), propput] HRESULT UsePBFrames([in] VARIANT_BOOL newVal);
		   
	[id(106000 + 14), propget] HRESULT UseAdvancedIntraCoding([out, retval] VARIANT_BOOL* pVal);
	[id(106000 + 14), propput] HRESULT UseAdvancedIntraCoding([in] VARIANT_BOOL newVal);
	[id(106000 + 15), propget] HRESULT UseDeblockingFilter([out, retval] VARIANT_BOOL* pVal);
	[id(106000 + 15), propput] HRESULT UseDeblockingFilter([in] VARIANT_BOOL newVal);
	[id(106000 + 16), propget] HRESULT UseImprovedPBFrames([out, retval] VARIANT_BOOL* pVal);
	[id(106000 + 16), propput] HRESULT UseImprovedPBFrames([in] VARIANT_BOOL newVal);
		   
	[id(106000 + 17), propget] HRESULT UseRefPictureSelectionCode([out, retval] VARIANT_BOOL* pVal);
	[id(106000 + 17), propput] HRESULT UseRefPictureSelectionCode([in] VARIANT_BOOL newVal);
	[id(106000 + 18), propget] HRESULT Threads([out, retval] LONG* pVal);
	[id(106000 + 18), propput] HRESULT Threads([in] LONG newVal);
	[id(106000 + 19), propget] HRESULT FramesPerThread([out, retval] LONG* pVal);
	[id(106000 + 19), propput] HRESULT FramesPerThread([in] LONG newVal);
	[id(106000 + 20), propget] HRESULT SyncGOBsPerFrame([out, retval] LONG* pVal);
	[id(106000 + 20), propput] HRESULT SyncGOBsPerFrame([in] LONG newVal);
		   
	[id(106000 + 21), propget] HRESULT ScalabilityMode([out, retval] LONG* pVal);
	[id(106000 + 21), propput] HRESULT ScalabilityMode([in] LONG newVal);
		   
	[id(106000 + 22), propget] HRESULT UseAlternativeInterVLC([out, retval] VARIANT_BOOL* pVal);
	[id(106000 + 22), propput] HRESULT UseAlternativeInterVLC([in] VARIANT_BOOL newVal);
	[id(106000 + 23), propget] HRESULT UseModifiedQuantization([out, retval] VARIANT_BOOL* pVal);
	[id(106000 + 23), propput] HRESULT UseModifiedQuantization([in] VARIANT_BOOL newVal);
};

template <class CoT, class T>
class CAVSH263VideoFormatImpl : public CAVSVideoFormatImpl<CoT, T>
{
public:
	CAVSH263VideoFormatImpl()
	{
		m_lMediaType = MEDIAFORMAT_H263;
		SetDefaultProperties();
	}
	STDMETHOD(CreateDuplicate)(IAVSMediaFormat** pMediaFormat)
	{
		CAVSVideoFormatImpl<CoT, T>::CreateDuplicate(pMediaFormat);
		if (NULL==(*pMediaFormat))
			return S_OK;
		IAVSH263VideoFormat* pAVSH263Format=NULL;
		(*pMediaFormat)->QueryInterface(__uuidof(IAVSH263VideoFormat), (void**)&pAVSH263Format);
		if (NULL==pAVSH263Format)
		{
			(*pMediaFormat)->Release();
			*pMediaFormat = NULL;
			return S_OK;
		}

		pAVSH263Format->put_QuantI(m_lQuantI);
		pAVSH263Format->put_QuantP(m_lQuantP);
		pAVSH263Format->put_QuantB(m_lQuantB);
		pAVSH263Format->put_QuantEI_EP(m_lQuantEI_EP);
		
		pAVSH263Format->put_RateControlMethod(m_lRateControlMethod);
		pAVSH263Format->put_UseGOBSync(m_bUseGOBSync ? VARIANT_TRUE : VARIANT_FALSE);
		pAVSH263Format->put_Distance_I_I(m_lDistance_I_I);
		pAVSH263Format->put_Distance_I_P(m_lDistance_I_P);
		pAVSH263Format->put_Distance_MB(m_lDistance_MB);
		
		pAVSH263Format->put_UMVMode(m_lUMVMode);
		
		pAVSH263Format->put_UseSintaxArithCoding(m_bUseSintaxArithCoding ? VARIANT_TRUE : VARIANT_FALSE);
		pAVSH263Format->put_UseAdvancedPrediction(m_bUseAdvancedPrediction ? VARIANT_TRUE : VARIANT_FALSE);
		pAVSH263Format->put_UsePBFrames(m_bUsePBFrames ? VARIANT_TRUE : VARIANT_FALSE);
		
		pAVSH263Format->put_UseAdvancedIntraCoding(m_bUseAdvancedIntraCoding ? VARIANT_TRUE : VARIANT_FALSE);
		pAVSH263Format->put_UseDeblockingFilter(m_bUseDeblockingFilter ? VARIANT_TRUE : VARIANT_FALSE);
		pAVSH263Format->put_UseImprovedPBFrames(m_bUseImprovedPBFrames ? VARIANT_TRUE : VARIANT_FALSE);
		
		pAVSH263Format->put_UseRefPictureSelectionCode(m_bUseRefPictureSelectionCode ? VARIANT_TRUE : VARIANT_FALSE);
		pAVSH263Format->put_Threads(m_lThreads);
		pAVSH263Format->put_FramesPerThread(m_lFramesPerThread);
		pAVSH263Format->put_SyncGOBsPerFrame(m_lSyncGOBsPerFrame);
		
		pAVSH263Format->put_ScalabilityMode(m_lScalabilityMode);
		
		pAVSH263Format->put_UseAlternativeInterVLC(m_bUseAlternativeInterVLC ? VARIANT_TRUE : VARIANT_FALSE);
		pAVSH263Format->put_UseModifiedQuantization(m_bUseModifiedQuantization ? VARIANT_TRUE : VARIANT_FALSE);
		pAVSH263Format->Release();
		return S_OK;
	}
	STDMETHOD(SetDefaultProperties)(void)
	{
		CAVSVideoFormatImpl<CoT, T>::SetDefaultProperties();
		m_lQuantI = 2;
		m_lQuantP = 2;
		m_lQuantB = 2;
		m_lQuantEI_EP = 2;
		
		m_lRateControlMethod = 3;
		m_bUseGOBSync = FALSE;
		m_lDistance_I_I = 30;
		m_lDistance_I_P = 0;
		m_lDistance_MB  = 0;
		
		m_lUMVMode = 0;
		
		m_bUseSintaxArithCoding = FALSE;
		m_bUseAdvancedPrediction = FALSE;
		m_bUsePBFrames = FALSE;
		
		m_bUseAdvancedIntraCoding = FALSE;
		m_bUseDeblockingFilter = FALSE;
		m_bUseImprovedPBFrames = FALSE;
		
		m_bUseRefPictureSelectionCode = FALSE;
		m_lThreads = 2;
		m_lFramesPerThread = 3;
		m_lSyncGOBsPerFrame = 1;
		
		m_lScalabilityMode = 0;
		
		m_bUseAlternativeInterVLC = FALSE;
		m_bUseModifiedQuantization = FALSE;
		return S_OK;
	}
	STDMETHOD(SetPropertiesByFormat)(IAVSMediaFormat* pMediaFormat)
	{
		CAVSVideoFormatImpl<CoT, T>::SetPropertiesByFormat(pMediaFormat);
		if (pMediaFormat!=NULL)
		{
			IAVSH263VideoFormat*	pAVSH263Format=NULL;
			pMediaFormat->QueryInterface(__uuidof(IAVSH263VideoFormat), (void**)&pAVSH263Format);
			if (pAVSH263Format!=NULL)
			{
				pAVSH263Format->get_QuantI(&m_lQuantI);
				pAVSH263Format->get_QuantP(&m_lQuantP);
				pAVSH263Format->get_QuantB(&m_lQuantB);
				pAVSH263Format->get_QuantEI_EP(&m_lQuantEI_EP);
				
				pAVSH263Format->get_RateControlMethod(&m_lRateControlMethod);
				pAVSH263Format->get_UseGOBSync(&m_bUseGOBSync);
				pAVSH263Format->get_Distance_I_I(&m_lDistance_I_I);
				pAVSH263Format->get_Distance_I_P(&m_lDistance_I_P);
				pAVSH263Format->get_Distance_MB(&m_lDistance_MB);
				
				pAVSH263Format->get_UMVMode(&m_lUMVMode);
				
				pAVSH263Format->get_UseSintaxArithCoding(&m_bUseSintaxArithCoding);
				pAVSH263Format->get_UseAdvancedPrediction(&m_bUseAdvancedPrediction);
				pAVSH263Format->get_UsePBFrames(&m_bUsePBFrames);
				
				pAVSH263Format->get_UseAdvancedIntraCoding(&m_bUseAdvancedIntraCoding);
				pAVSH263Format->get_UseDeblockingFilter(&m_bUseDeblockingFilter);
				pAVSH263Format->get_UseImprovedPBFrames(&m_bUseImprovedPBFrames);
				
				pAVSH263Format->get_UseRefPictureSelectionCode(&m_bUseRefPictureSelectionCode);
				pAVSH263Format->get_Threads(&m_lThreads);
				pAVSH263Format->get_FramesPerThread(&m_lFramesPerThread);
				pAVSH263Format->get_SyncGOBsPerFrame(&m_lSyncGOBsPerFrame);
				
				pAVSH263Format->get_ScalabilityMode(&m_lScalabilityMode);
				
				pAVSH263Format->get_UseAlternativeInterVLC(&m_bUseAlternativeInterVLC);
				pAVSH263Format->get_UseModifiedQuantization(&m_bUseModifiedQuantization);

				RELEASEINTERFACE(pAVSH263Format);
			}
		}
		return S_OK;
	}
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		//Здесь проверяем параметры относящиеся к AVSH263VideoFormat
		return CAVSVideoFormatImpl<CoT, T>::SetAdditionalParam(ParamName, ParamValue);
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		//Здесь проверяем параметры относящиеся к AVSH263VideoFormat
		return CAVSVideoFormatImpl<CoT, T>::GetAdditionalParam(ParamName, ParamValue);
	}
	//////
	LINK_PROPERTY_LONG(QuantI, m_lQuantI)
	LINK_PROPERTY_LONG(QuantP, m_lQuantP)
	LINK_PROPERTY_LONG(QuantB, m_lQuantB)
	LINK_PROPERTY_LONG(QuantEI_EP, m_lQuantEI_EP)
	
	LINK_PROPERTY_LONG(RateControlMethod, m_lRateControlMethod)
	LINK_PROPERTY_VARIANT_BOOL(UseGOBSync, m_bUseGOBSync)
	LINK_PROPERTY_LONG(Distance_I_I, m_lDistance_I_I)
	LINK_PROPERTY_LONG(Distance_I_P, m_lDistance_I_P)
	LINK_PROPERTY_LONG(Distance_MB, m_lDistance_MB)
	
	LINK_PROPERTY_LONG(UMVMode, m_lUMVMode)
	
	LINK_PROPERTY_VARIANT_BOOL(UseSintaxArithCoding, m_bUseSintaxArithCoding)
	LINK_PROPERTY_VARIANT_BOOL(UseAdvancedPrediction, m_bUseAdvancedPrediction)
	LINK_PROPERTY_VARIANT_BOOL(UsePBFrames, m_bUsePBFrames)
	
	LINK_PROPERTY_VARIANT_BOOL(UseAdvancedIntraCoding, m_bUseAdvancedIntraCoding)
	LINK_PROPERTY_VARIANT_BOOL(UseDeblockingFilter, m_bUseDeblockingFilter)
	LINK_PROPERTY_VARIANT_BOOL(UseImprovedPBFrames, m_bUseImprovedPBFrames)
	
	LINK_PROPERTY_VARIANT_BOOL(UseRefPictureSelectionCode, m_bUseRefPictureSelectionCode)
	LINK_PROPERTY_LONG(Threads, m_lThreads)
	LINK_PROPERTY_LONG(FramesPerThread, m_lFramesPerThread)
	LINK_PROPERTY_LONG(SyncGOBsPerFrame, m_lSyncGOBsPerFrame)
	
	LINK_PROPERTY_LONG(ScalabilityMode, m_lScalabilityMode)
	
	LINK_PROPERTY_VARIANT_BOOL(UseAlternativeInterVLC, m_bUseAlternativeInterVLC)
	LINK_PROPERTY_VARIANT_BOOL(UseModifiedQuantization, m_bUseModifiedQuantization)
protected:
	
	long m_lQuantI;
	long m_lQuantP;
	long m_lQuantB;
	long m_lQuantEI_EP;
	
	long m_lRateControlMethod;
	VARIANT_BOOL m_bUseGOBSync;
	long m_lDistance_I_I;
	long m_lDistance_I_P;
	long m_lDistance_MB;
	
	long m_lUMVMode;
	
	VARIANT_BOOL m_bUseSintaxArithCoding;
	VARIANT_BOOL m_bUseAdvancedPrediction;
	VARIANT_BOOL m_bUsePBFrames;
	
	VARIANT_BOOL m_bUseAdvancedIntraCoding;
	VARIANT_BOOL m_bUseDeblockingFilter;
	VARIANT_BOOL m_bUseImprovedPBFrames;
	
	VARIANT_BOOL m_bUseRefPictureSelectionCode;
	long m_lThreads;
	long m_lFramesPerThread;
	long m_lSyncGOBsPerFrame;
	
	long m_lScalabilityMode;
	
	VARIANT_BOOL m_bUseAlternativeInterVLC;
	VARIANT_BOOL m_bUseModifiedQuantization;
};

// CAVSH263VideoFormat

[coclass, uuid("E43AEFB8-11E0-4256-9E7F-961BD5E17A16"), threading(apartment), vi_progid("AVSMediaFormatSettings3.AVSH263VideoFor"), progid("AVSMediaFormatSettings3.AVSH263VideoF.1"), version(1.0)]
class ATL_NO_VTABLE CAVSH263VideoFormat : public CAVSH263VideoFormatImpl<CAVSH263VideoFormat, IAVSH263VideoFormat>
{

};

