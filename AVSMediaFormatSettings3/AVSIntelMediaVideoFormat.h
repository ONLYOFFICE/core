// AVSIntelMediaVideoFormat.h : Declaration of the CAVSIntelMediaVideoFormat

#pragma once
#include "resource.h"       // main symbols
#include "AVSVideoFormat.h"
#include "ATLDefine.h"
#include "VideoFileErrorDescription.h"

#include "AVSH264VideoFormat.h"
#include "..\Common\mfxstructures.h"

#include <math.h>

class CAVSIntelMediaVideoFormat;

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif


// IAVSIntelMediaVideoFormat
[
	object,
	uuid("B163D893-DB02-4777-A4D7-4626C9831B71"),
	dual,	helpstring("IAVSIntelMediaVideoFormat Interface"),
	pointer_default(unique)
]
__interface IAVSIntelMediaVideoFormat : IAVSVideoFormat
{
	[id(120000 + 3), propget] HRESULT AsyncDepth([out,retval] USHORT *pVal);
	[id(120000 + 3), propput] HRESULT AsyncDepth([in] USHORT newVal);
	[id(120000 + 6), propget] HRESULT FourCC([out,retval] ULONG *pVal);
	[id(120000 + 6), propput] HRESULT FourCC([in] ULONG newVal);
	[id(120000 + 18), propget] HRESULT PicStruct([out,retval] USHORT *pVal);
	[id(120000 + 18), propput] HRESULT PicStruct([in] USHORT newVal);
	[id(120000 + 19), propget] HRESULT ChromaFormat([out,retval] USHORT *pVal);
	[id(120000 + 19), propput] HRESULT ChromaFormat([in] USHORT newVal);
	[id(120000 + 21), propget] HRESULT CodecId([out,retval] ULONG *pVal);
	[id(120000 + 21), propput] HRESULT CodecId([in] ULONG newVal);
	[id(120000 + 22), propget] HRESULT CodecProfile([out,retval] USHORT *pVal);
	[id(120000 + 22), propput] HRESULT CodecProfile([in] USHORT newVal);
	[id(120000 + 23), propget] HRESULT CodecLevel([out,retval] USHORT *pVal);
	[id(120000 + 23), propput] HRESULT CodecLevel([in] USHORT newVal);
	[id(120000 + 24), propget] HRESULT NumThread([out,retval] USHORT *pVal);
	[id(120000 + 24), propput] HRESULT NumThread([in] USHORT newVal);
	[id(120000 + 25), propget] HRESULT TargetUsage([out,retval] USHORT *pVal);
	[id(120000 + 25), propput] HRESULT TargetUsage([in] USHORT newVal);
	[id(120000 + 26), propget] HRESULT GopPicSize([out,retval] USHORT *pVal);
	[id(120000 + 26), propput] HRESULT GopPicSize([in] USHORT newVal);
	[id(120000 + 27), propget] HRESULT GopRefDist([out,retval] USHORT *pVal);
	[id(120000 + 27), propput] HRESULT GopRefDist([in] USHORT newVal);
	[id(120000 + 28), propget] HRESULT GopOptFlag([out,retval] USHORT *pVal);
	[id(120000 + 28), propput] HRESULT GopOptFlag([in] USHORT newVal);
	[id(120000 + 29), propget] HRESULT IdrInterval([out,retval] USHORT *pVal);
	[id(120000 + 29), propput] HRESULT IdrInterval([in] USHORT newVal);
	[id(120000 + 30), propget] HRESULT RateControlMethod([out,retval] USHORT *pVal);
	[id(120000 + 30), propput] HRESULT RateControlMethod([in] USHORT newVal);
	[id(120000 + 31), propget] HRESULT InitialDelayInKB([out,retval] USHORT *pVal);
	[id(120000 + 31), propput] HRESULT InitialDelayInKB([in] USHORT newVal);
	[id(120000 + 32), propget] HRESULT QPI([out,retval] USHORT *pVal);
	[id(120000 + 32), propput] HRESULT QPI([in] USHORT newVal);
	[id(120000 + 33), propget] HRESULT BufferSizeInKB([out,retval] USHORT *pVal);
	[id(120000 + 33), propput] HRESULT BufferSizeInKB([in] USHORT newVal);
	[id(120000 + 35), propget] HRESULT QPP([out,retval] USHORT *pVal);
	[id(120000 + 35), propput] HRESULT QPP([in] USHORT newVal);
	[id(120000 + 36), propget] HRESULT MaxKbps([out,retval] USHORT *pVal);
	[id(120000 + 36), propput] HRESULT MaxKbps([in] USHORT newVal);
	[id(120000 + 37), propget] HRESULT QPB([out,retval] USHORT *pVal);
	[id(120000 + 37), propput] HRESULT QPB([in] USHORT newVal);
	[id(120000 + 38), propget] HRESULT NumSlice([out,retval] USHORT *pVal);
	[id(120000 + 38), propput] HRESULT NumSlice([in] USHORT newVal);
	[id(120000 + 39), propget] HRESULT NumRefFrame([out,retval] USHORT *pVal);
	[id(120000 + 39), propput] HRESULT NumRefFrame([in] USHORT newVal);
	[id(120000 + 40), propget] HRESULT EncodedOrder([out,retval] USHORT *pVal);
	[id(120000 + 40), propput] HRESULT EncodedOrder([in] USHORT newVal);
	[id(120000 + 41), propget] HRESULT DecodedOrder([out,retval] USHORT *pVal);
	[id(120000 + 41), propput] HRESULT DecodedOrder([in] USHORT newVal);
	[id(120000 + 43), propget] HRESULT Protected([out,retval] USHORT *pVal);
	[id(120000 + 43), propput] HRESULT Protected([in] USHORT newVal);
	[id(120000 + 44), propget] HRESULT IOPattern([out,retval] USHORT *pVal);
	[id(120000 + 44), propput] HRESULT IOPattern([in] USHORT newVal);
	[id(120000 + 48), propget] HRESULT RateDistortionOpt([out,retval] USHORT *pVal);
	[id(120000 + 48), propput] HRESULT RateDistortionOpt([in] USHORT newVal);
	[id(120000 + 49), propget] HRESULT MECostType([out,retval] USHORT *pVal);
	[id(120000 + 49), propput] HRESULT MECostType([in] USHORT newVal);
	[id(120000 + 50), propget] HRESULT MESearchType([out,retval] USHORT *pVal);
	[id(120000 + 50), propput] HRESULT MESearchType([in] USHORT newVal);
	[id(120000 + 51), propget] HRESULT MVSearchWindowX([out,retval] SHORT *pVal);
	[id(120000 + 51), propput] HRESULT MVSearchWindowX([in] SHORT newVal);
	[id(120000 + 52), propget] HRESULT MVSearchWindowY([out,retval] SHORT *pVal);
	[id(120000 + 52), propput] HRESULT MVSearchWindowY([in] SHORT newVal);
	[id(120000 + 53), propget] HRESULT EndOfSequence([out,retval] USHORT *pVal);
	[id(120000 + 53), propput] HRESULT EndOfSequence([in] USHORT newVal);
	[id(120000 + 54), propget] HRESULT FramePicture([out,retval] USHORT *pVal);
	[id(120000 + 54), propput] HRESULT FramePicture([in] USHORT newVal);
	[id(120000 + 55), propget] HRESULT CAVLC([out,retval] USHORT *pVal);
	[id(120000 + 55), propput] HRESULT CAVLC([in] USHORT newVal);
	[id(120000 + 57), propget] HRESULT RefPicListReordering([out,retval] USHORT *pVal);
	[id(120000 + 57), propput] HRESULT RefPicListReordering([in] USHORT newVal);
	[id(120000 + 58), propget] HRESULT ResetRefList([out,retval] USHORT *pVal);
	[id(120000 + 58), propput] HRESULT ResetRefList([in] USHORT newVal);
	[id(120000 + 60), propget] HRESULT IntraPredBlockSize([out,retval] USHORT *pVal);
	[id(120000 + 60), propput] HRESULT IntraPredBlockSize([in] USHORT newVal);
	[id(120000 + 61), propget] HRESULT InterPredBlockSize([out,retval] USHORT *pVal);
	[id(120000 + 61), propput] HRESULT InterPredBlockSize([in] USHORT newVal);
	[id(120000 + 62), propget] HRESULT MVPrecision([out,retval] USHORT *pVal);
	[id(120000 + 62), propput] HRESULT MVPrecision([in] USHORT newVal);
	[id(120000 + 63), propget] HRESULT MaxDecFrameBuffering([out,retval] USHORT *pVal);
	[id(120000 + 63), propput] HRESULT MaxDecFrameBuffering([in] USHORT newVal);
	[id(120000 + 64), propget] HRESULT AUDelimiter([out,retval] USHORT *pVal);
	[id(120000 + 64), propput] HRESULT AUDelimiter([in] USHORT newVal);
	[id(120000 + 65), propget] HRESULT EndOfStream([out,retval] USHORT *pVal);
	[id(120000 + 65), propput] HRESULT EndOfStream([in] USHORT newVal);
	[id(120000 + 66), propget] HRESULT PicTimingSEI([out,retval] USHORT *pVal);
	[id(120000 + 66), propput] HRESULT PicTimingSEI([in] USHORT newVal);
	[id(120000 + 67), propget] HRESULT VuiNalHrdParameters([out,retval] USHORT *pVal);
	[id(120000 + 67), propput] HRESULT VuiNalHrdParameters([in] USHORT newVal);
	[id(120000 + 68), propget] HRESULT NumExtParam([out,retval] USHORT *pVal);
	[id(120000 + 68), propput] HRESULT NumExtParam([in] USHORT newVal);

	[id(120000 + 70), propget] HRESULT AppleCompatible([out,retval] VARIANT_BOOL *pVal);
	[id(120000 + 70), propput] HRESULT AppleCompatible([in] VARIANT_BOOL newVal);
	[id(120000 + 71), propget] HRESULT ExtraData([out,retval, satype("BYTE")] SAFEARRAY **pVal);
	[id(120000 + 71), propput] HRESULT ExtraData([in, satype("BYTE")] SAFEARRAY *newVal);
};

template <class CoT, class T>
class CAVSIntelMediaVideoFormatImpl : public CAVSVideoFormatImpl<CoT, T>
{
public:
	CAVSIntelMediaVideoFormatImpl()
	{
		m_lMediaType = MEDIAFORMAT_INTELMEDIA;
		m_psaExtraData = NULL;
		SetDefaultProperties();
	}
	
	~CAVSIntelMediaVideoFormatImpl()
	{
	}

	STDMETHOD(CreateDuplicate)(IAVSMediaFormat** pMediaFormat)
	{
		if (NULL==pMediaFormat)
			return S_OK;

		CAVSVideoFormatImpl<CoT,T>::CreateDuplicate(pMediaFormat);
		if (NULL==(*pMediaFormat))
			return S_OK;
		IAVSIntelMediaVideoFormat* pAVSIntelMediaVideoFormat=NULL;
		(*pMediaFormat)->QueryInterface(__uuidof(IAVSIntelMediaVideoFormat), (void**)&pAVSIntelMediaVideoFormat);
		if ( NULL == pAVSIntelMediaVideoFormat )
		{
			(*pMediaFormat)->Release();
			*pMediaFormat = NULL;
			return S_OK;
		}

		pAVSIntelMediaVideoFormat->put_AsyncDepth( m_nAsyncDepth );
		pAVSIntelMediaVideoFormat->put_FourCC( m_nFourCC );
		pAVSIntelMediaVideoFormat->put_PicStruct( m_nPicStruct );
		pAVSIntelMediaVideoFormat->put_ChromaFormat( m_nChromaFormat );
		pAVSIntelMediaVideoFormat->put_CodecId( m_nCodecId );
		pAVSIntelMediaVideoFormat->put_CodecProfile( m_nCodecProfile );
		pAVSIntelMediaVideoFormat->put_CodecLevel( m_nCodecLevel );
		pAVSIntelMediaVideoFormat->put_NumThread( m_nNumThread );
		pAVSIntelMediaVideoFormat->put_TargetUsage( m_nTargetUsage );
		pAVSIntelMediaVideoFormat->put_GopPicSize( m_nGopPicSize );
		pAVSIntelMediaVideoFormat->put_GopRefDist( m_nGopRefDist );
		pAVSIntelMediaVideoFormat->put_GopOptFlag( m_nGopOptFlag );
		pAVSIntelMediaVideoFormat->put_IdrInterval( m_nIdrInterval );
		pAVSIntelMediaVideoFormat->put_RateControlMethod( m_nRateControlMethod );
		pAVSIntelMediaVideoFormat->put_InitialDelayInKB( m_nInitialDelayInKB );
		pAVSIntelMediaVideoFormat->put_QPI( m_nQPI );
		pAVSIntelMediaVideoFormat->put_BufferSizeInKB( m_nBufferSizeInKB );
		pAVSIntelMediaVideoFormat->put_QPP( m_nQPP );
		pAVSIntelMediaVideoFormat->put_MaxKbps( m_nMaxKbps );
		pAVSIntelMediaVideoFormat->put_QPB( m_nQPB );
		pAVSIntelMediaVideoFormat->put_NumSlice( m_nNumSlice );
		pAVSIntelMediaVideoFormat->put_NumRefFrame( m_nNumRefFrame );
		pAVSIntelMediaVideoFormat->put_EncodedOrder( m_nEncodedOrder );
		pAVSIntelMediaVideoFormat->put_DecodedOrder( m_nDecodedOrder );
		pAVSIntelMediaVideoFormat->put_Protected( m_nProtected );
		pAVSIntelMediaVideoFormat->put_IOPattern( m_nIOPattern );
		pAVSIntelMediaVideoFormat->put_RateDistortionOpt( m_nRateDistortionOpt );
		pAVSIntelMediaVideoFormat->put_MECostType( m_nMECostType );
		pAVSIntelMediaVideoFormat->put_MESearchType( m_nMESearchType );
		pAVSIntelMediaVideoFormat->put_MVSearchWindowX( m_nMVSearchWindowX );
		pAVSIntelMediaVideoFormat->put_MVSearchWindowY( m_nMVSearchWindowY );
		pAVSIntelMediaVideoFormat->put_EndOfSequence( m_nEndOfSequence );
		pAVSIntelMediaVideoFormat->put_FramePicture( m_nFramePicture );
		pAVSIntelMediaVideoFormat->put_CAVLC( m_nCAVLC );
		pAVSIntelMediaVideoFormat->put_RefPicListReordering( m_nRefPicListReordering );
		pAVSIntelMediaVideoFormat->put_ResetRefList( m_nResetRefList );
		pAVSIntelMediaVideoFormat->put_IntraPredBlockSize( m_nIntraPredBlockSize );
		pAVSIntelMediaVideoFormat->put_InterPredBlockSize( m_nInterPredBlockSize );
		pAVSIntelMediaVideoFormat->put_MVPrecision( m_nMVPrecision );
		pAVSIntelMediaVideoFormat->put_MaxDecFrameBuffering( m_nMaxDecFrameBuffering );
		pAVSIntelMediaVideoFormat->put_AUDelimiter( m_nAUDelimiter );
		pAVSIntelMediaVideoFormat->put_EndOfStream( m_nEndOfStream );
		pAVSIntelMediaVideoFormat->put_PicTimingSEI( m_nPicTimingSEI );
		pAVSIntelMediaVideoFormat->put_VuiNalHrdParameters( m_nVuiNalHrdParameters );
		pAVSIntelMediaVideoFormat->put_NumExtParam( m_nNumExtParam );
		pAVSIntelMediaVideoFormat->put_AppleCompatible( m_nAppleCompatible );
		pAVSIntelMediaVideoFormat->put_ExtraData( m_psaExtraData );

		pAVSIntelMediaVideoFormat->Release();
		return S_OK;
	}
	STDMETHOD(SetDefaultProperties)(void)
	{
		/*m_nAsyncDepth = 0xFF;
		m_nFourCC = 0xFF;
		m_nPicStruct = 0xFF;
		m_nChromaFormat = 0xFF;
		m_nCodecId = 0xFF;
		m_nCodecProfile = 0xFF;
		m_nCodecLevel = 0xFF;
		m_nNumThread = 0xFF;
		m_nTargetUsage = 0xFF;
		m_nGopPicSize = 0xFF;
		m_nGopRefDist = 0xFF;
		m_nGopOptFlag = 0xFF;
		m_nIdrInterval = 0xFF;
		m_nRateControlMethod = 0xFF;
		m_nInitialDelayInKB = 0xFF;
		m_nQPI = 0xFF;
		m_nBufferSizeInKB = 0xFF;
		m_nQPP = 0xFF;
		m_nMaxKbps = 0xFF;
		m_nQPB = 0xFF;
		m_nNumSlice = 0xFF;
		m_nNumRefFrame = 0xFF;
		m_nEncodedOrder = 0xFF;
		m_nDecodedOrder = 0xFF;
		m_nProtected = 0xFF;
		m_nIOPattern = 0xFF;
		m_nRateDistortionOpt = 0xFF;
		m_nMECostType = 0xFF;
		m_nMESearchType = 0xFF;
		m_nMVSearchWindowX = 0xFF;
		m_nMVSearchWindowY = 0xFF;
		m_nEndOfSequence = 0xFF;
		m_nFramePicture = 0xFF;
		m_nCAVLC = 0xFF;
		m_nRefPicListReordering = 0xFF;
		m_nResetRefList = 0xFF;
		m_nIntraPredBlockSize = 0xFF;
		m_nInterPredBlockSize = 0xFF;
		m_nMVPrecision = 0xFF;
		m_nMaxDecFrameBuffering = 0xFF;
		m_nAUDelimiter = 0xFF;
		m_nEndOfStream = 0xFF;
		m_nPicTimingSEI = 0xFF;
		m_nVuiNalHrdParameters = 0xFF;
		m_nNumExtParam = 0xFF;
		m_nAppleCompatible = 0xFF;*/

		m_nAsyncDepth = 0;
		m_nFourCC = MFX_FOURCC_NV12; // Mandated
		m_nPicStruct = MFX_PICSTRUCT_PROGRESSIVE; // Optional
		m_nChromaFormat = MFX_CHROMAFORMAT_YUV420; // Mandated
		m_nCodecId = 0; // Mandated, but it should be set from outside
		m_nCodecProfile = 0;
		m_nCodecLevel = 0;
		m_nNumThread = 0;
		m_nTargetUsage = 0; // Optional //MFX_TARGETUSAGE_BALANCED;
		m_nGopPicSize = 0;
		m_nGopRefDist = 0;
		m_nGopOptFlag = 0;
		m_nIdrInterval = 0;
		m_nRateControlMethod = MFX_RATECONTROL_CBR; // Optional TODO - can be 0?
		m_nInitialDelayInKB = 0; // Mandated, but it should be set from outside
		m_nQPI = 0;
		m_nBufferSizeInKB = 0;
		m_nQPP = 0;
		m_nMaxKbps = 0;
		m_nQPB = 0;
		m_nNumSlice = 0;
		m_nNumRefFrame = 0;
		m_nEncodedOrder = 0; // Mandated
		m_nDecodedOrder = 0;
		m_nProtected = 0;
		m_nIOPattern = MFX_IOPATTERN_IN_SYSTEM_MEMORY; // Mandated
		m_nRateDistortionOpt = 0;
		m_nMECostType = 0;
		m_nMESearchType = 0;
		m_nMVSearchWindowX = 0;
		m_nMVSearchWindowY = 0;
		m_nEndOfSequence = 0;
		m_nFramePicture = 0;
		m_nCAVLC = 0;
		m_nRefPicListReordering = 0;
		m_nResetRefList = 0;
		m_nIntraPredBlockSize = 0;
		m_nInterPredBlockSize = 0;
		m_nMVPrecision = 0;
		m_nMaxDecFrameBuffering = 0;
		m_nAUDelimiter = 0;
		m_nEndOfStream = 0;
		m_nPicTimingSEI = 0;
		m_nVuiNalHrdParameters = 0;
		m_nNumExtParam = 0;
		m_nAppleCompatible = VARIANT_FALSE;
		RELEASEARRAY(m_psaExtraData);

		return S_OK;
	}

	STDMETHOD(SetPropertiesByFormat)(IAVSMediaFormat* pMediaFormat)
	{
		CAVSVideoFormatImpl<CoT, T>::SetPropertiesByFormat(pMediaFormat);
		if (pMediaFormat!=NULL)
		{
			IAVSIntelMediaVideoFormat*	pAVSIntelMediaVideoFormat=NULL;
			pMediaFormat->QueryInterface(__uuidof(IAVSIntelMediaVideoFormat), (void**)&pAVSIntelMediaVideoFormat);
			if (pAVSIntelMediaVideoFormat!=NULL)
			{
				pAVSIntelMediaVideoFormat->get_AsyncDepth( &m_nAsyncDepth );
				pAVSIntelMediaVideoFormat->get_FourCC( &m_nFourCC );
				pAVSIntelMediaVideoFormat->get_PicStruct( &m_nPicStruct );
				pAVSIntelMediaVideoFormat->get_ChromaFormat( &m_nChromaFormat );
				pAVSIntelMediaVideoFormat->get_CodecId( &m_nCodecId );
				pAVSIntelMediaVideoFormat->get_CodecProfile( &m_nCodecProfile );
				pAVSIntelMediaVideoFormat->get_CodecLevel( &m_nCodecLevel );
				pAVSIntelMediaVideoFormat->get_NumThread( &m_nNumThread );
				pAVSIntelMediaVideoFormat->get_TargetUsage( &m_nTargetUsage );
				pAVSIntelMediaVideoFormat->get_GopPicSize( &m_nGopPicSize );
				pAVSIntelMediaVideoFormat->get_GopRefDist( &m_nGopRefDist );
				pAVSIntelMediaVideoFormat->get_GopOptFlag( &m_nGopOptFlag );
				pAVSIntelMediaVideoFormat->get_IdrInterval( &m_nIdrInterval );
				pAVSIntelMediaVideoFormat->get_RateControlMethod( &m_nRateControlMethod );
				pAVSIntelMediaVideoFormat->get_InitialDelayInKB( &m_nInitialDelayInKB );
				pAVSIntelMediaVideoFormat->get_QPI( &m_nQPI );
				pAVSIntelMediaVideoFormat->get_BufferSizeInKB( &m_nBufferSizeInKB );
				pAVSIntelMediaVideoFormat->get_QPP( &m_nQPP );
				pAVSIntelMediaVideoFormat->get_MaxKbps( &m_nMaxKbps );
				pAVSIntelMediaVideoFormat->get_QPB( &m_nQPB );
				pAVSIntelMediaVideoFormat->get_NumSlice( &m_nNumSlice );
				pAVSIntelMediaVideoFormat->get_NumRefFrame( &m_nNumRefFrame );
				pAVSIntelMediaVideoFormat->get_EncodedOrder( &m_nEncodedOrder );
				pAVSIntelMediaVideoFormat->get_DecodedOrder( &m_nDecodedOrder );
				pAVSIntelMediaVideoFormat->get_Protected( &m_nProtected );
				pAVSIntelMediaVideoFormat->get_IOPattern( &m_nIOPattern );
				pAVSIntelMediaVideoFormat->get_RateDistortionOpt( &m_nRateDistortionOpt );
				pAVSIntelMediaVideoFormat->get_MECostType( &m_nMECostType );
				pAVSIntelMediaVideoFormat->get_MESearchType( &m_nMESearchType );
				pAVSIntelMediaVideoFormat->get_MVSearchWindowX( &m_nMVSearchWindowX );
				pAVSIntelMediaVideoFormat->get_MVSearchWindowY( &m_nMVSearchWindowY );
				pAVSIntelMediaVideoFormat->get_EndOfSequence( &m_nEndOfSequence );
				pAVSIntelMediaVideoFormat->get_FramePicture( &m_nFramePicture );
				pAVSIntelMediaVideoFormat->get_CAVLC( &m_nCAVLC );
				pAVSIntelMediaVideoFormat->get_RefPicListReordering( &m_nRefPicListReordering );
				pAVSIntelMediaVideoFormat->get_ResetRefList( &m_nResetRefList );
				pAVSIntelMediaVideoFormat->get_IntraPredBlockSize( &m_nIntraPredBlockSize );
				pAVSIntelMediaVideoFormat->get_InterPredBlockSize( &m_nInterPredBlockSize );
				pAVSIntelMediaVideoFormat->get_MVPrecision( &m_nMVPrecision );
				pAVSIntelMediaVideoFormat->get_MaxDecFrameBuffering( &m_nMaxDecFrameBuffering );
				pAVSIntelMediaVideoFormat->get_AUDelimiter( &m_nAUDelimiter );
				pAVSIntelMediaVideoFormat->get_EndOfStream( &m_nEndOfStream );
				pAVSIntelMediaVideoFormat->get_PicTimingSEI( &m_nPicTimingSEI );
				pAVSIntelMediaVideoFormat->get_VuiNalHrdParameters( &m_nVuiNalHrdParameters );
				pAVSIntelMediaVideoFormat->get_NumExtParam( &m_nNumExtParam );
				pAVSIntelMediaVideoFormat->get_AppleCompatible( &m_nAppleCompatible );

				RELEASEARRAY(m_psaExtraData);
				pAVSIntelMediaVideoFormat->get_ExtraData( &m_psaExtraData );

				RELEASEINTERFACE(pAVSIntelMediaVideoFormat);
				return S_OK;
			}

			// if not returned yet let's try IAVSH264VideoFormat
			IAVSH264VideoFormat*	pAVSH264VideoFormat=NULL;
			pMediaFormat->QueryInterface(__uuidof(IAVSH264VideoFormat), (void**)&pAVSH264VideoFormat);
			if ( pAVSH264VideoFormat != NULL )
			{
				//TODO
				RELEASEINTERFACE(pAVSIntelMediaVideoFormat);
				return S_OK;
			}
		}
		return INVALIDARGUMENT;
	}

	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		//Здесь проверяем параметры относящиеся к AVSIntelMediaVideoFormat
		return CAVSVideoFormatImpl<CoT, T>::SetAdditionalParam(ParamName, ParamValue);
	}

	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		//Здесь проверяем параметры относящиеся к AVSIntelMediaVideoFormat
		return CAVSVideoFormatImpl<CoT, T>::GetAdditionalParam(ParamName, ParamValue);
	}

	//////////////////////////////////////
	LINK_PROPERTY_USHORT(AsyncDepth, m_nAsyncDepth);
	LINK_PROPERTY_ULONG(FourCC, m_nFourCC);
	LINK_PROPERTY_USHORT(PicStruct, m_nPicStruct);
	LINK_PROPERTY_USHORT(ChromaFormat, m_nChromaFormat);
	LINK_PROPERTY_ULONG(CodecId, m_nCodecId);
	LINK_PROPERTY_USHORT(CodecProfile, m_nCodecProfile);
	LINK_PROPERTY_USHORT(CodecLevel, m_nCodecLevel);
	LINK_PROPERTY_USHORT(NumThread, m_nNumThread);
	LINK_PROPERTY_USHORT(TargetUsage, m_nTargetUsage);
	LINK_PROPERTY_USHORT(GopPicSize, m_nGopPicSize);
	LINK_PROPERTY_USHORT(GopRefDist, m_nGopRefDist);
	LINK_PROPERTY_USHORT(GopOptFlag, m_nGopOptFlag);
	LINK_PROPERTY_USHORT(IdrInterval, m_nIdrInterval);
	LINK_PROPERTY_USHORT(RateControlMethod, m_nRateControlMethod);
	LINK_PROPERTY_USHORT(InitialDelayInKB, m_nInitialDelayInKB);
	LINK_PROPERTY_USHORT(QPI, m_nQPI);
	LINK_PROPERTY_USHORT(BufferSizeInKB, m_nBufferSizeInKB);
	LINK_PROPERTY_USHORT(QPP, m_nQPP);
	LINK_PROPERTY_USHORT(MaxKbps, m_nMaxKbps);
	LINK_PROPERTY_USHORT(QPB, m_nQPB);
	LINK_PROPERTY_USHORT(NumSlice, m_nNumSlice);
	LINK_PROPERTY_USHORT(NumRefFrame, m_nNumRefFrame);
	LINK_PROPERTY_USHORT(EncodedOrder, m_nEncodedOrder);
	LINK_PROPERTY_USHORT(DecodedOrder, m_nDecodedOrder);
	LINK_PROPERTY_USHORT(Protected, m_nProtected);
	LINK_PROPERTY_USHORT(IOPattern, m_nIOPattern);
	LINK_PROPERTY_USHORT(RateDistortionOpt, m_nRateDistortionOpt);
	LINK_PROPERTY_USHORT(MECostType, m_nMECostType);
	LINK_PROPERTY_USHORT(MESearchType, m_nMESearchType);
	LINK_PROPERTY_SHORT(MVSearchWindowX, m_nMVSearchWindowX);
	LINK_PROPERTY_SHORT(MVSearchWindowY, m_nMVSearchWindowY);
	LINK_PROPERTY_USHORT(EndOfSequence, m_nEndOfSequence);
	LINK_PROPERTY_USHORT(FramePicture, m_nFramePicture);
	LINK_PROPERTY_USHORT(CAVLC, m_nCAVLC);
	LINK_PROPERTY_USHORT(RefPicListReordering, m_nRefPicListReordering);
	LINK_PROPERTY_USHORT(ResetRefList, m_nResetRefList);
	LINK_PROPERTY_USHORT(IntraPredBlockSize, m_nIntraPredBlockSize);
	LINK_PROPERTY_USHORT(InterPredBlockSize, m_nInterPredBlockSize);
	LINK_PROPERTY_USHORT(MVPrecision, m_nMVPrecision);
	LINK_PROPERTY_USHORT(MaxDecFrameBuffering, m_nMaxDecFrameBuffering);
	LINK_PROPERTY_USHORT(AUDelimiter, m_nAUDelimiter);
	LINK_PROPERTY_USHORT(EndOfStream, m_nEndOfStream);
	LINK_PROPERTY_USHORT(PicTimingSEI, m_nPicTimingSEI);
	LINK_PROPERTY_USHORT(VuiNalHrdParameters, m_nVuiNalHrdParameters);
	LINK_PROPERTY_USHORT(NumExtParam, m_nNumExtParam);
	LINK_PROPERTY_VARIANT_BOOL(AppleCompatible, m_nAppleCompatible);
	LINK_PROPERTY_SAFEARRAY(ExtraData, m_psaExtraData);

protected:
	USHORT	m_nAsyncDepth;
	ULONG	m_nFourCC;
	USHORT	m_nPicStruct;
	USHORT	m_nChromaFormat;
	ULONG	m_nCodecId;
	USHORT	m_nCodecProfile;
	USHORT	m_nCodecLevel;
	USHORT	m_nNumThread;
	USHORT	m_nTargetUsage;
	USHORT	m_nGopPicSize;
	USHORT	m_nGopRefDist;
	USHORT	m_nGopOptFlag;
	USHORT	m_nIdrInterval;
	USHORT	m_nRateControlMethod;
	USHORT	m_nInitialDelayInKB;
	USHORT	m_nQPI;
	USHORT	m_nBufferSizeInKB;
	USHORT	m_nQPP;
	USHORT	m_nMaxKbps;
	USHORT	m_nQPB;
	USHORT	m_nNumSlice;
	USHORT	m_nNumRefFrame;
	USHORT	m_nEncodedOrder;
	USHORT	m_nDecodedOrder;
	USHORT	m_nProtected;
	USHORT	m_nIOPattern;
	USHORT	m_nRateDistortionOpt;
	USHORT	m_nMECostType;
	USHORT	m_nMESearchType;
	SHORT	m_nMVSearchWindowX;
	SHORT	m_nMVSearchWindowY;
	USHORT	m_nEndOfSequence;
	USHORT	m_nFramePicture;
	USHORT	m_nCAVLC;
	USHORT	m_nRefPicListReordering;
	USHORT	m_nResetRefList;
	USHORT	m_nIntraPredBlockSize;
	USHORT	m_nInterPredBlockSize;
	USHORT	m_nMVPrecision;
	USHORT	m_nMaxDecFrameBuffering;
	USHORT	m_nAUDelimiter;
	USHORT	m_nEndOfStream;
	USHORT	m_nPicTimingSEI;
	USHORT	m_nVuiNalHrdParameters;
	USHORT	m_nNumExtParam;
	VARIANT_BOOL	m_nAppleCompatible;
	LPSAFEARRAY	m_psaExtraData;
};

// CAVSIntelMediaVideoFormat

[coclass, uuid("54A60D4B-44B7-4927-9CDA-9DBED567CDDC"), threading(apartment), vi_progid("AVSMediaFormatSettings3.AVSIntelMediaVi"), progid("AVSMediaFormatSettings3.AVSIntelMedia.1"), version(1.0)]
class ATL_NO_VTABLE CAVSIntelMediaVideoFormat : public CAVSIntelMediaVideoFormatImpl<CAVSIntelMediaVideoFormat, IAVSIntelMediaVideoFormat>
{
};

