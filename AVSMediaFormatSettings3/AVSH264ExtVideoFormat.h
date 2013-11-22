#pragma once
#include "resource.h"       // main symbols
#include "ATLDefine.h"
#include "AVSH264VideoFormat.h"

class CAVSH264ExtVideoFormat;
// IAVSH264ExtVideoFormat
[ object, uuid("3491C3DB-B0AA-46b9-BBE4-F95F7588E4E2"), dual, pointer_default(unique)]
__interface IAVSH264ExtVideoFormat : IAVSH264VideoFormat
{
	[id(117000 + 1), propget] HRESULT Deterministis([out, retval] VARIANT_BOOL* pVal);
	[id(117000 + 1), propput] HRESULT Deterministis([in] VARIANT_BOOL newVal);
	[id(117000 + 2), propget] HRESULT Overscan([out, retval] long* pVal);
	[id(117000 + 2), propput] HRESULT Overscan([in] long newVal);
	[id(117000 + 3), propget] HRESULT VidFormat([out, retval] long* pVal);
	[id(117000 + 3), propput] HRESULT VidFormat([in] long newVal);
	[id(117000 + 4), propget] HRESULT FullRange([out, retval] VARIANT_BOOL* pVal);
	[id(117000 + 4), propput] HRESULT FullRange([in] VARIANT_BOOL newVal);
	[id(117000 + 5), propget] HRESULT ColorPrim([out, retval] long* pVal);
	[id(117000 + 5), propput] HRESULT ColorPrim([in] long newVal);
	[id(117000 + 6), propget] HRESULT Transfer([out, retval] long* pVal);
	[id(117000 + 6), propput] HRESULT Transfer([in] long newVal);
	[id(117000 + 7), propget] HRESULT ColorMatrix([out, retval] long* pVal);
	[id(117000 + 7), propput] HRESULT ColorMatrix([in] long newVal);
	[id(117000 + 8), propget] HRESULT ChromaLoc([out, retval] long* pVal);
	[id(117000 + 8), propput] HRESULT ChromaLoc([in] long newVal);
	[id(117000 + 9), propget] HRESULT FrameReference([out, retval] long* pVal);
	[id(117000 + 9), propput] HRESULT FrameReference([in] long newVal);
	[id(117000 + 10), propget] HRESULT BFrameAdaptive([out, retval] long* pVal);
	[id(117000 + 10), propput] HRESULT BFrameAdaptive([in] long newVal);
	[id(117000 + 11), propget] HRESULT BFramePyramid([out, retval] VARIANT_BOOL* pVal);
	[id(117000 + 11), propput] HRESULT BFramePyramid([in] VARIANT_BOOL newVal);
	[id(117000 + 12), propget] HRESULT CabacInitIdc([out, retval] long* pVal);
	[id(117000 + 12), propput] HRESULT CabacInitIdc([in] long newVal);
	[id(117000 + 13), propget] HRESULT RCRateTolerance([out, retval] double* pVal);
	[id(117000 + 13), propput] HRESULT RCRateTolerance([in] double newVal);
	[id(117000 + 14), propget] HRESULT RCVbvBufferSize([out, retval] long* pVal);
	[id(117000 + 14), propput] HRESULT RCVbvBufferSize([in] long newVal);
	[id(117000 + 15), propget] HRESULT RCVbvBufferInit([out, retval] double* pVal);
	[id(117000 + 15), propput] HRESULT RCVbvBufferInit([in] double newVal);
	[id(117000 + 16), propget] HRESULT RCRfConstant([out, retval] double* pVal);
	[id(117000 + 16), propput] HRESULT RCRfConstant([in] double newVal);
	[id(117000 + 17), propget] HRESULT RCAqMode([out, retval] long* pVal);
	[id(117000 + 17), propput] HRESULT RCAqMode([in] long newVal);
	[id(117000 + 18), propget] HRESULT RCAqStrength([out, retval] double* pVal);
	[id(117000 + 18), propput] HRESULT RCAqStrength([in] double newVal);
	[id(117000 + 19), propget] HRESULT RCQblur([out, retval] double* pVal);
	[id(117000 + 19), propput] HRESULT RCQblur([in] double newVal);
	[id(117000 + 20), propget] HRESULT RCComplexityBlur([out, retval] double* pVal);
	[id(117000 + 20), propput] HRESULT RCComplexityBlur([in] double newVal);
	[id(117000 + 21), propget] HRESULT PsyRd([out, retval] double* pVal);
	[id(117000 + 21), propput] HRESULT PsyRd([in] double newVal);
	[id(117000 + 22), propget] HRESULT PsyTrellis([out, retval] double* pVal);
	[id(117000 + 22), propput] HRESULT PsyTrellis([in] double newVal);
	[id(117000 + 23), propget] HRESULT MVRangeThread([out, retval] long* pVal);
	[id(117000 + 23), propput] HRESULT MVRangeThread([in] long newVal);
	[id(117000 + 24), propget] HRESULT MVRange([out, retval] long* pVal);
	[id(117000 + 24), propput] HRESULT MVRange([in] long newVal);
	[id(117000 + 25), propget] HRESULT Direct8x8Inference([out, retval] long* pVal);
	[id(117000 + 25), propput] HRESULT Direct8x8Inference([in] long newVal);
	[id(117000 + 26), propget] HRESULT ChromaQpOffset([out, retval] long* pVal);
	[id(117000 + 26), propput] HRESULT ChromaQpOffset([in] long newVal);
	[id(117000 + 27), propget] HRESULT FastPSkip([out, retval] VARIANT_BOOL* pVal);
	[id(117000 + 27), propput] HRESULT FastPSkip([in] VARIANT_BOOL newVal);
	[id(117000 + 28), propget] HRESULT DctDecimate([out, retval] VARIANT_BOOL* pVal);
	[id(117000 + 28), propput] HRESULT DctDecimate([in] VARIANT_BOOL newVal);
	[id(117000 + 29), propget] HRESULT DeadZoneInter([out, retval] long* pVal);
	[id(117000 + 29), propput] HRESULT DeadZoneInter([in] long newVal);
	[id(117000 + 30), propget] HRESULT DeadZoneIntra([out, retval] long* pVal);
	[id(117000 + 30), propput] HRESULT DeadZoneIntra([in] long newVal);
	[id(117000 + 31), propget] HRESULT Psnr([out, retval] VARIANT_BOOL* pVal);
	[id(117000 + 31), propput] HRESULT Psnr([in] VARIANT_BOOL newVal);
	[id(117000 + 32), propget] HRESULT Ssim([out, retval] VARIANT_BOOL* pVal);
	[id(117000 + 32), propput] HRESULT Ssim([in] VARIANT_BOOL newVal);
	[id(117000 + 33), propget] HRESULT RepeatHeaders([out, retval] VARIANT_BOOL* pVal);
	[id(117000 + 33), propput] HRESULT RepeatHeaders([in] VARIANT_BOOL newVal);
	[id(117000 + 34), propget] HRESULT GenerateAUD([out, retval] VARIANT_BOOL* pVal);
	[id(117000 + 34), propput] HRESULT GenerateAUD([in] VARIANT_BOOL newVal);
};

template <class CoT, class T>
class CAVSH264ExtVideoFormatImpl : public CAVSH264VideoFormatImpl<CoT, T>
{
public:
	CAVSH264ExtVideoFormatImpl()
	{
		m_lMediaType = MEDIAFORMAT_H264_EXT;
		SetDefaultProperties();
	}
	~CAVSH264ExtVideoFormatImpl()
	{
	}
	STDMETHOD(CreateDuplicate)(IAVSMediaFormat** pMediaFormat)
	{
		CAVSH264VideoFormatImpl<CoT, T>::CreateDuplicate(pMediaFormat);
		if( NULL == (*pMediaFormat) )
			return S_OK;

		IAVSH264ExtVideoFormat* pAVSH264ExtFormat = NULL;
		(*pMediaFormat)->QueryInterface( &pAVSH264ExtFormat );

		if( NULL == pAVSH264ExtFormat )
		{
			(*pMediaFormat)->Release();
			*pMediaFormat = NULL;
			return S_OK;
		}
		pAVSH264ExtFormat->put_Deterministis( m_bDeterministis );
		pAVSH264ExtFormat->put_Overscan( m_lOverscan );
		pAVSH264ExtFormat->put_VidFormat( m_lVidFormat );
		pAVSH264ExtFormat->put_FullRange( m_bFullRange );
		pAVSH264ExtFormat->put_ColorPrim( m_lColorPrim );
		pAVSH264ExtFormat->put_Transfer( m_lTransfer );
		pAVSH264ExtFormat->put_ColorMatrix( m_lColorMatrix );
		pAVSH264ExtFormat->put_ChromaLoc( m_lChromaLoc );

		pAVSH264ExtFormat->put_FrameReference( m_lFrameReference );
		pAVSH264ExtFormat->put_BFrameAdaptive( m_lBFrameAdaptive );
		pAVSH264ExtFormat->put_BFramePyramid( m_bBFramePyramid );

		pAVSH264ExtFormat->put_CabacInitIdc( m_lCabacInitIdc );

		pAVSH264ExtFormat->put_RCRateTolerance( m_dRCRateTolerance );
		pAVSH264ExtFormat->put_RCVbvBufferSize( m_lRCVbvBufferSize );
		pAVSH264ExtFormat->put_RCVbvBufferInit( m_dRCVbvBufferInit );
		pAVSH264ExtFormat->put_RCRfConstant( m_dRCRfConstant );
		pAVSH264ExtFormat->put_RCAqMode( m_lRCAqMode );
		pAVSH264ExtFormat->put_RCAqStrength( m_dRCAqStrength );
		pAVSH264ExtFormat->put_RCQblur( m_dRCQblur );
		pAVSH264ExtFormat->put_RCComplexityBlur( m_dRCComplexityBlur );

		pAVSH264ExtFormat->put_PsyRd( m_dPsyRd );
		pAVSH264ExtFormat->put_PsyTrellis( m_dPsyTrellis );
		pAVSH264ExtFormat->put_MVRangeThread( m_lMVRangeThread );
		pAVSH264ExtFormat->put_MVRange( m_lMVRange );
		pAVSH264ExtFormat->put_Direct8x8Inference( m_lDirect8x8Inference );
		pAVSH264ExtFormat->put_ChromaQpOffset( m_lChromaQpOffset );
		pAVSH264ExtFormat->put_FastPSkip( m_bFastPSkip );
		pAVSH264ExtFormat->put_DctDecimate( m_bDctDecimate );
		pAVSH264ExtFormat->put_DeadZoneInter( m_lDeadZoneInter );
		pAVSH264ExtFormat->put_DeadZoneIntra( m_lDeadZoneIntra );
		pAVSH264ExtFormat->put_Psnr( m_bPsnr );
		pAVSH264ExtFormat->put_Ssim( m_bSsim );

		pAVSH264ExtFormat->put_RepeatHeaders( m_bRepeatHeaders );
		pAVSH264ExtFormat->put_GenerateAUD( m_bAud );

		RELEASEINTERFACE( pAVSH264ExtFormat );

		return S_OK;
	}
	STDMETHOD(SetDefaultProperties)(void)
	{
		CAVSH264VideoFormatImpl<CoT, T>::SetDefaultProperties();

		m_bDeterministis = VARIANT_TRUE;
		m_lOverscan = 0;
		m_lVidFormat = 5;
		m_bFullRange = VARIANT_FALSE;
		m_lColorPrim = 2;
		m_lTransfer = 2;
		m_lColorMatrix = 2;
		m_lChromaLoc = 0;
		
		m_lFrameReference = 1;
		m_lBFrameAdaptive = 1;
		m_bBFramePyramid = VARIANT_FALSE;
		
		m_lCabacInitIdc = 0;
		
		m_dRCRateTolerance = 1.0;
		m_lRCVbvBufferSize = 0;
		m_dRCVbvBufferInit = 0.9;
		m_dRCRfConstant = 0.; 
		m_lRCAqMode = 2;
		m_dRCAqStrength = 1.0;
		m_dRCQblur = 0.5;
		m_dRCComplexityBlur = 20.;
		
		m_dPsyRd = 1.0;
		m_dPsyTrellis = 0.;
		m_lMVRangeThread = -1;
		m_lMVRange = -1;
		m_lDirect8x8Inference = 1;
		m_lChromaQpOffset = 0;
		m_bFastPSkip = VARIANT_TRUE;
		m_bDctDecimate = VARIANT_TRUE;
		m_lDeadZoneInter = 21;
		m_lDeadZoneIntra = 11;
		m_bPsnr = VARIANT_TRUE;
		m_bSsim = VARIANT_TRUE;
		
		m_bRepeatHeaders = VARIANT_TRUE;
		m_bAud = VARIANT_FALSE;

		return S_OK;
	}
	STDMETHOD(SetPropertiesByFormat)(IAVSMediaFormat* pMediaFormat)
	{
		CAVSH264VideoFormatImpl<CoT, T>::SetPropertiesByFormat( pMediaFormat );
		if( pMediaFormat != NULL )
		{
			IAVSH264ExtVideoFormat*	pAVSH264ExtFormat = NULL;
			pMediaFormat->QueryInterface( &pAVSH264ExtFormat );
			if( pAVSH264ExtFormat != NULL )
			{
				pAVSH264ExtFormat->get_Deterministis( &m_bDeterministis );
				pAVSH264ExtFormat->get_Overscan( &m_lOverscan );
				pAVSH264ExtFormat->get_VidFormat( &m_lVidFormat );
				pAVSH264ExtFormat->get_FullRange( &m_bFullRange );
				pAVSH264ExtFormat->get_ColorPrim( &m_lColorPrim );
				pAVSH264ExtFormat->get_Transfer( &m_lTransfer );
				pAVSH264ExtFormat->get_ColorMatrix( &m_lColorMatrix );
				pAVSH264ExtFormat->get_ChromaLoc( &m_lChromaLoc );

				pAVSH264ExtFormat->get_FrameReference( &m_lFrameReference );
				pAVSH264ExtFormat->get_BFrameAdaptive( &m_lBFrameAdaptive );
				pAVSH264ExtFormat->get_BFramePyramid( &m_bBFramePyramid );

				pAVSH264ExtFormat->get_CabacInitIdc( &m_lCabacInitIdc );

				pAVSH264ExtFormat->get_RCRateTolerance( &m_dRCRateTolerance );
				pAVSH264ExtFormat->get_RCVbvBufferSize( &m_lRCVbvBufferSize );
				pAVSH264ExtFormat->get_RCVbvBufferInit( &m_dRCVbvBufferInit );
				pAVSH264ExtFormat->get_RCRfConstant( &m_dRCRfConstant );
				pAVSH264ExtFormat->get_RCAqMode( &m_lRCAqMode );
				pAVSH264ExtFormat->get_RCAqStrength( &m_dRCAqStrength );
				pAVSH264ExtFormat->get_RCQblur( &m_dRCQblur );
				pAVSH264ExtFormat->get_RCComplexityBlur( &m_dRCComplexityBlur );

				pAVSH264ExtFormat->get_PsyRd( &m_dPsyRd );
				pAVSH264ExtFormat->get_PsyTrellis( &m_dPsyTrellis );
				pAVSH264ExtFormat->get_MVRangeThread( &m_lMVRangeThread );
				pAVSH264ExtFormat->get_MVRange( &m_lMVRange );
				pAVSH264ExtFormat->get_Direct8x8Inference( &m_lDirect8x8Inference );
				pAVSH264ExtFormat->get_ChromaQpOffset( &m_lChromaQpOffset );
				pAVSH264ExtFormat->get_FastPSkip( &m_bFastPSkip );
				pAVSH264ExtFormat->get_DctDecimate( &m_bDctDecimate );
				pAVSH264ExtFormat->get_DeadZoneInter( &m_lDeadZoneInter );
				pAVSH264ExtFormat->get_DeadZoneIntra( &m_lDeadZoneIntra );
				pAVSH264ExtFormat->get_Psnr( &m_bPsnr );
				pAVSH264ExtFormat->get_Ssim( &m_bSsim );

				pAVSH264ExtFormat->get_RepeatHeaders( &m_bRepeatHeaders );
				pAVSH264ExtFormat->get_GenerateAUD( &m_bAud );

				RELEASEINTERFACE( pAVSH264ExtFormat );
			}
		}
		return S_OK;
	}
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		//Здесь проверяем параметры относящиеся к AVSH264ExtVideoFormat
		return CAVSH264VideoFormatImpl<CoT, T>::SetAdditionalParam(ParamName, ParamValue);
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		//Здесь проверяем параметры относящиеся к AVSH264ExtVideoFormat
		return CAVSH264VideoFormatImpl<CoT, T>::GetAdditionalParam(ParamName, ParamValue);
	}
	
	LINK_PROPERTY_VARIANT_BOOL( Deterministis, m_bDeterministis );
	LINK_PROPERTY_LONG( Overscan, m_lOverscan );
	LINK_PROPERTY_LONG( VidFormat, m_lVidFormat );
	LINK_PROPERTY_VARIANT_BOOL( FullRange, m_bFullRange );
	LINK_PROPERTY_LONG( ColorPrim, m_lColorPrim );
	LINK_PROPERTY_LONG( Transfer, m_lTransfer );
	LINK_PROPERTY_LONG( ColorMatrix, m_lColorMatrix );
	LINK_PROPERTY_LONG( ChromaLoc, m_lChromaLoc );

	LINK_PROPERTY_LONG( FrameReference, m_lFrameReference );
	LINK_PROPERTY_LONG( BFrameAdaptive, m_lBFrameAdaptive );
	LINK_PROPERTY_VARIANT_BOOL( BFramePyramid, m_bBFramePyramid );

	LINK_PROPERTY_LONG( CabacInitIdc, m_lCabacInitIdc );

	LINK_PROPERTY_DOUBLE( RCRateTolerance, m_dRCRateTolerance );
	LINK_PROPERTY_LONG( RCVbvBufferSize, m_lRCVbvBufferSize );
	LINK_PROPERTY_DOUBLE( RCVbvBufferInit, m_dRCVbvBufferInit );
	LINK_PROPERTY_DOUBLE( RCRfConstant, m_dRCRfConstant );
	LINK_PROPERTY_LONG( RCAqMode, m_lRCAqMode );
	LINK_PROPERTY_DOUBLE( RCAqStrength, m_dRCAqStrength );
	LINK_PROPERTY_DOUBLE( RCQblur, m_dRCQblur );
	LINK_PROPERTY_DOUBLE( RCComplexityBlur, m_dRCComplexityBlur );

	LINK_PROPERTY_DOUBLE( PsyRd, m_dPsyRd );
	LINK_PROPERTY_DOUBLE( PsyTrellis, m_dPsyTrellis );
	LINK_PROPERTY_LONG( MVRangeThread, m_lMVRangeThread );
	LINK_PROPERTY_LONG( MVRange, m_lMVRange );
	LINK_PROPERTY_LONG( Direct8x8Inference, m_lDirect8x8Inference );
	LINK_PROPERTY_LONG( ChromaQpOffset, m_lChromaQpOffset );
	LINK_PROPERTY_VARIANT_BOOL( FastPSkip, m_bFastPSkip );
	LINK_PROPERTY_VARIANT_BOOL( DctDecimate, m_bDctDecimate );
	LINK_PROPERTY_LONG( DeadZoneInter, m_lDeadZoneInter );
	LINK_PROPERTY_LONG( DeadZoneIntra, m_lDeadZoneIntra );
	LINK_PROPERTY_VARIANT_BOOL( Psnr, m_bPsnr );
	LINK_PROPERTY_VARIANT_BOOL( Ssim, m_bSsim );

	LINK_PROPERTY_VARIANT_BOOL( RepeatHeaders, m_bRepeatHeaders );
	LINK_PROPERTY_VARIANT_BOOL( GenerateAUD, m_bAud );

protected:
	VARIANT_BOOL	m_bDeterministis;
	long			m_lOverscan;
	long			m_lVidFormat;
	VARIANT_BOOL	m_bFullRange;
	long			m_lColorPrim;
	long			m_lTransfer;
	long			m_lColorMatrix;
	long			m_lChromaLoc;

	long			m_lFrameReference;
	long			m_lBFrameAdaptive;
	VARIANT_BOOL	m_bBFramePyramid;

	long			m_lCabacInitIdc;

	double			m_dRCRateTolerance;
	long			m_lRCVbvBufferSize;
	double			m_dRCVbvBufferInit;
	double			m_dRCRfConstant;
	long			m_lRCAqMode;
	double			m_dRCAqStrength;
	double			m_dRCQblur;
	double			m_dRCComplexityBlur;

	double			m_dPsyRd;
	double			m_dPsyTrellis;
	long			m_lMVRangeThread;
	long			m_lMVRange;
	long			m_lDirect8x8Inference;
	long			m_lChromaQpOffset;
	VARIANT_BOOL	m_bFastPSkip;
	VARIANT_BOOL	m_bDctDecimate;
	long			m_lDeadZoneInter;
	long			m_lDeadZoneIntra;
	VARIANT_BOOL	m_bPsnr;
	VARIANT_BOOL	m_bSsim;

	VARIANT_BOOL	m_bRepeatHeaders;
	VARIANT_BOOL	m_bAud;
};

// CAVSH264ExtVideoFormat

[coclass, uuid("E71EFE55-7A73-44c7-B78E-AE3F62C82945"), threading(apartment), vi_progid("AVSMediaFormatSettings3.AVSH263VideoFor"), progid("AVSMediaFormatSettings3.AVSH263VideoF.1"), version(1.0)]
class ATL_NO_VTABLE CAVSH264ExtVideoFormat : public CAVSH264ExtVideoFormatImpl<CAVSH264ExtVideoFormat, IAVSH264ExtVideoFormat>
{

};

