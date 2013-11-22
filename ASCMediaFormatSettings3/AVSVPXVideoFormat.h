// AVSVPXVideoFormat.h : Declaration of the CAVSVPXVideoFormat

#pragma once
#include "resource.h"       // main symbols
#include "ATLDefine.h"
#include "AVSVideoFormat.h"

class CAVSVPXVideoFormat;
// IAVSVPXVideoFormat
[
	object,
	uuid("71559154-7E8B-4A11-9852-9C7ADD4F6612"),
	dual,	helpstring("IAVSVPXVideoFormat Interface"),
	pointer_default(unique)
]
__interface IAVSVPXVideoFormat : IAVSVideoFormat
{
	[propget, id(119000 + 1), helpstring("property EncodingTime")] HRESULT EncodingTime([out, retval] LONG* pVal);
	[propput, id(119000 + 1), helpstring("property EncodingTime")] HRESULT EncodingTime([in] LONG newVal);
	[propget, id(119000 + 2), helpstring("property ThreadCount")] HRESULT ThreadCount([out, retval] LONG* pVal);
	[propput, id(119000 + 2), helpstring("property ThreadCount")] HRESULT ThreadCount([in] LONG newVal);
	[propget, id(119000 + 3), helpstring("property Profile")] HRESULT Profile([out, retval] LONG* pVal);
	[propput, id(119000 + 3), helpstring("property Profile")] HRESULT Profile([in] LONG newVal);
	[propget, id(119000 + 4), helpstring("property ErrorResilient")] HRESULT ErrorResilient([out, retval] LONG* pVal);
	[propput, id(119000 + 4), helpstring("property ErrorResilient")] HRESULT ErrorResilient([in] LONG newVal);
	[propget, id(119000 + 5), helpstring("property MultiPassMode")] HRESULT MultiPassMode([out, retval] LONG* pVal);
	[propput, id(119000 + 5), helpstring("property MultiPassMode")] HRESULT MultiPassMode([in] LONG newVal);
	[propget, id(119000 + 6), helpstring("property LagInFrames")] HRESULT LagInFrames([out, retval] LONG* pVal);
	[propput, id(119000 + 6), helpstring("property LagInFrames")] HRESULT LagInFrames([in] LONG newVal);
	[propget, id(119000 + 7), helpstring("property RCDropFrameThresh")] HRESULT RCDropFrameThresh([out, retval] LONG* pVal);
	[propput, id(119000 + 7), helpstring("property RCDropFrameThresh")] HRESULT RCDropFrameThresh([in] LONG newVal);
	[propget, id(119000 + 8), helpstring("property RCResizeAllowed")] HRESULT RCResizeAllowed([out, retval] LONG* pVal);
	[propput, id(119000 + 8), helpstring("property RCResizeAllowed")] HRESULT RCResizeAllowed([in] LONG newVal);
	[propget, id(119000 + 9), helpstring("property RCResizeUpThresh")] HRESULT RCResizeUpThresh([out, retval] LONG* pVal);
	[propput, id(119000 + 9), helpstring("property RCResizeUpThresh")] HRESULT RCResizeUpThresh([in] LONG newVal);
	[propget, id(119000 + 10), helpstring("property RCResizeDownThresh")] HRESULT RCResizeDownThresh([out, retval] LONG* pVal);
	[propput, id(119000 + 10), helpstring("property RCResizeDownThresh")] HRESULT RCResizeDownThresh([in] LONG newVal);
	[propget, id(119000 + 11), helpstring("property RCEndUsage")] HRESULT RCEndUsage([out, retval] LONG* pVal);
	[propput, id(119000 + 11), helpstring("property RCEndUsage")] HRESULT RCEndUsage([in] LONG newVal);
	[propget, id(119000 + 12), helpstring("property RCStatFileName")] HRESULT RCStatFileName([out, retval] BSTR* pVal);
	[propput, id(119000 + 12), helpstring("property RCStatFileName")] HRESULT RCStatFileName([in] BSTR newVal);
	[propget, id(119000 + 13), helpstring("property RCMinQuantizer")] HRESULT RCMinQuantizer([out, retval] LONG* pVal);
	[propput, id(119000 + 13), helpstring("property RCMinQuantizer")] HRESULT RCMinQuantizer([in] LONG newVal);
	[propget, id(119000 + 14), helpstring("property RCMaxQuantizer")] HRESULT RCMaxQuantizer([out, retval] LONG* pVal);
	[propput, id(119000 + 14), helpstring("property RCMaxQuantizer")] HRESULT RCMaxQuantizer([in] LONG newVal);
	[propget, id(119000 + 15), helpstring("property RCUndershootPct")] HRESULT RCUndershootPct([out, retval] LONG* pVal);
	[propput, id(119000 + 15), helpstring("property RCUndershootPct")] HRESULT RCUndershootPct([in] LONG newVal);
	[propget, id(119000 + 16), helpstring("property RCOvershootPct")] HRESULT RCOvershootPct([out, retval] LONG* pVal);
	[propput, id(119000 + 16), helpstring("property RCOvershootPct")] HRESULT RCOvershootPct([in] LONG newVal);
	[propget, id(119000 + 17), helpstring("property RCBufferSize")] HRESULT RCBufferSize([out, retval] LONG* pVal);
	[propput, id(119000 + 17), helpstring("property RCBufferSize")] HRESULT RCBufferSize([in] LONG newVal);
	[propget, id(119000 + 18), helpstring("property RCBufferInitSize")] HRESULT RCBufferInitSize([out, retval] LONG* pVal);
	[propput, id(119000 + 18), helpstring("property RCBufferInitSize")] HRESULT RCBufferInitSize([in] LONG newVal);
	[propget, id(119000 + 19), helpstring("property RCBufferOptSize")] HRESULT RCBufferOptSize([out, retval] LONG* pVal);
	[propput, id(119000 + 19), helpstring("property RCBufferOptSize")] HRESULT RCBufferOptSize([in] LONG newVal);
	[propget, id(119000 + 20), helpstring("property RC2PassVBRBiasPct")] HRESULT RC2PassVBRBiasPct([out, retval] LONG* pVal);
	[propput, id(119000 + 20), helpstring("property RC2PassVBRBiasPct")] HRESULT RC2PassVBRBiasPct([in] LONG newVal);
	[propget, id(119000 + 21), helpstring("property RC2PassVBRMinsectionPct")] HRESULT RC2PassVBRMinsectionPct([out, retval] LONG* pVal);
	[propput, id(119000 + 21), helpstring("property RC2PassVBRMinsectionPct")] HRESULT RC2PassVBRMinsectionPct([in] LONG newVal);
	[propget, id(119000 + 22), helpstring("property RC2PassVBRMaxsectionPct")] HRESULT RC2PassVBRMaxsectionPct([out, retval] LONG* pVal);
	[propput, id(119000 + 22), helpstring("property RC2PassVBRMaxsectionPct")] HRESULT RC2PassVBRMaxsectionPct([in] LONG newVal);
	[propget, id(119000 + 23), helpstring("property KeyFrameMode")] HRESULT KeyFrameMode([out, retval] LONG* pVal);
	[propput, id(119000 + 23), helpstring("property KeyFrameMode")] HRESULT KeyFrameMode([in] LONG newVal);
	[propget, id(119000 + 24), helpstring("property KeyFrameMinDist")] HRESULT KeyFrameMinDist([out, retval] LONG* pVal);
	[propput, id(119000 + 24), helpstring("property KeyFrameMinDist")] HRESULT KeyFrameMinDist([in] LONG newVal);
	[propget, id(119000 + 25), helpstring("property KeyFrameMaxDist")] HRESULT KeyFrameMaxDist([out, retval] LONG* pVal);
	[propput, id(119000 + 25), helpstring("property KeyFrameMaxDist")] HRESULT KeyFrameMaxDist([in] LONG newVal);

	[propget, id(103000 + 26), helpstring("property ExtraDate")] HRESULT ExtraData([out, retval, satype("BYTE")] SAFEARRAY** pVal);
	[propput, id(103000 + 26), helpstring("property ExtraDate")] HRESULT ExtraData([in, satype("BYTE")] SAFEARRAY* newVal);
};




template <class CoT, class T>

class CAVSVPXVideoFormatImpl : public CAVSVideoFormatImpl<CoT, T>
{
public:
	CAVSVPXVideoFormatImpl()
	{
		m_lMediaType = MEDIAFORMAT_VPX;
		m_parExtraData = NULL;
		SetDefaultProperties();
	}
	~CAVSVPXVideoFormatImpl()
	{
		RELEASEARRAY(m_parExtraData);
	}
	STDMETHOD(CreateDuplicate)(IAVSMediaFormat** pMediaFormat)
	{
		CAVSVideoFormatImpl<CoT, T>::CreateDuplicate(pMediaFormat);
		if (NULL==(*pMediaFormat))
			return S_OK;
		IAVSVPXVideoFormat* pAVSVPXFormat=NULL;
		(*pMediaFormat)->QueryInterface(__uuidof(IAVSVPXVideoFormat), (void**)&pAVSVPXFormat);
		if (NULL==pAVSVPXFormat)
		{
			(*pMediaFormat)->Release();
			*pMediaFormat = NULL;
			return S_OK;
		}

		pAVSVPXFormat->put_EncodingTime( m_lEncodingTime );
		pAVSVPXFormat->put_ThreadCount( m_lThreadCount );
		pAVSVPXFormat->put_Profile( m_lProfile );
		pAVSVPXFormat->put_ErrorResilient( m_lErrorResilient );
		pAVSVPXFormat->put_MultiPassMode( m_lMultiPassMode );
		pAVSVPXFormat->put_LagInFrames( m_lLagInFrames );
		pAVSVPXFormat->put_RCDropFrameThresh( m_lRCDropFrameThresh );
		pAVSVPXFormat->put_RCResizeAllowed( m_lRCResizeAllowed );
		pAVSVPXFormat->put_RCResizeUpThresh( m_lRCResizeUpThresh );
		pAVSVPXFormat->put_RCResizeDownThresh( m_lRCResizeDownThresh );
		pAVSVPXFormat->put_RCEndUsage( m_lRCEndUsage );
		
		BSTR bsTemp = m_sRCStatFileName.AllocSysString();
		pAVSVPXFormat->put_RCStatFileName( bsTemp );
		SysFreeString( bsTemp );

		pAVSVPXFormat->put_RCMinQuantizer( m_lRCMinQuantizer );
		pAVSVPXFormat->put_RCMaxQuantizer( m_lRCMaxQuantizer );
		pAVSVPXFormat->put_RCUndershootPct( m_lRCUndershootPct );
		pAVSVPXFormat->put_RCOvershootPct( m_lRCOvershootPct );
		pAVSVPXFormat->put_RCBufferSize( m_lRCBufferSize );
		pAVSVPXFormat->put_RCBufferInitSize( m_lRCBufferInitSize );
		pAVSVPXFormat->put_RCBufferOptSize( m_lRCBufferOptSize );
		pAVSVPXFormat->put_RC2PassVBRBiasPct( m_lRC2PassVBRBiasPct );
		pAVSVPXFormat->put_RC2PassVBRMinsectionPct( m_lRC2PassVBRMinsectionPct );
		pAVSVPXFormat->put_RC2PassVBRMaxsectionPct( m_lRC2PassVBRMaxsectionPct );
		pAVSVPXFormat->put_KeyFrameMode( m_lKeyFrameMode );
		pAVSVPXFormat->put_KeyFrameMinDist( m_lKeyFrameMinDist );
		pAVSVPXFormat->put_KeyFrameMaxDist( m_lKeyFrameMaxDist );

		pAVSVPXFormat->Release();
		return S_OK;
	}
	STDMETHOD(SetDefaultProperties)(void)
	{
		CAVSVideoFormatImpl<CoT, T>::SetDefaultProperties();

		m_lEncodingTime = 1000000;
		m_lThreadCount = 1;
		m_lProfile = 0;
		m_lErrorResilient = 0;
		m_lMultiPassMode = 0;
		m_lLagInFrames = 0;
		m_lRCDropFrameThresh = 70;
		m_lRCResizeAllowed = 0;
		m_lRCResizeUpThresh = 60;
		m_lRCResizeDownThresh = 30;
		m_lRCEndUsage = 0;
		m_sRCStatFileName = "c:/avsvpxpass.log";
		m_lRCMinQuantizer = 4;
		m_lRCMaxQuantizer = 63;
		m_lRCUndershootPct = 95;
		m_lRCOvershootPct = 200;
		m_lRCBufferSize = 6000;
		m_lRCBufferInitSize = 4000;
		m_lRCBufferOptSize = 5000;
		m_lRC2PassVBRBiasPct = 50;
		m_lRC2PassVBRMinsectionPct = 0;
		m_lRC2PassVBRMaxsectionPct = 400;
		m_lKeyFrameMode = 1;
		m_lKeyFrameMinDist = 0;
		m_lKeyFrameMaxDist = 9999;

		return S_OK;
	}
	STDMETHOD(SetPropertiesByFormat)(IAVSMediaFormat* pMediaFormat)
	{
		CAVSVideoFormatImpl<CoT, T>::SetPropertiesByFormat( pMediaFormat);
		if (pMediaFormat!=NULL)
		{
			IAVSVPXVideoFormat*	pAVSVPXFormat = NULL;
			pMediaFormat->QueryInterface(__uuidof(IAVSVPXVideoFormat), (void**)&pAVSVPXFormat);
			if (pAVSVPXFormat!=NULL)
			{
				pAVSVPXFormat->get_EncodingTime( &m_lEncodingTime );
				pAVSVPXFormat->get_ThreadCount( &m_lThreadCount );
				pAVSVPXFormat->get_Profile( &m_lProfile );
				pAVSVPXFormat->get_ErrorResilient( &m_lErrorResilient );
				pAVSVPXFormat->get_MultiPassMode( &m_lMultiPassMode );
				pAVSVPXFormat->get_LagInFrames( &m_lLagInFrames );
				pAVSVPXFormat->get_RCDropFrameThresh( &m_lRCDropFrameThresh );
				pAVSVPXFormat->get_RCResizeAllowed( &m_lRCResizeAllowed );
				pAVSVPXFormat->get_RCResizeUpThresh( &m_lRCResizeUpThresh );
				pAVSVPXFormat->get_RCResizeDownThresh( &m_lRCResizeDownThresh );
				pAVSVPXFormat->get_RCEndUsage( &m_lRCEndUsage );
				
				BSTR sStatFileName = NULL;
				pAVSVPXFormat->get_RCStatFileName( &sStatFileName );
				m_sRCStatFileName = sStatFileName;
				RELEASESYSSTRING( sStatFileName );

				pAVSVPXFormat->get_RCMinQuantizer( &m_lRCMinQuantizer );
				pAVSVPXFormat->get_RCMaxQuantizer( &m_lRCMaxQuantizer );
				pAVSVPXFormat->get_RCUndershootPct( &m_lRCUndershootPct );
				pAVSVPXFormat->get_RCOvershootPct( &m_lRCOvershootPct );
				pAVSVPXFormat->get_RCBufferSize( &m_lRCBufferSize );
				pAVSVPXFormat->get_RCBufferInitSize( &m_lRCBufferInitSize );
				pAVSVPXFormat->get_RCBufferOptSize( &m_lRCBufferOptSize );
				pAVSVPXFormat->get_RC2PassVBRBiasPct( &m_lRC2PassVBRBiasPct );
				pAVSVPXFormat->get_RC2PassVBRMinsectionPct( &m_lRC2PassVBRMinsectionPct );
				pAVSVPXFormat->get_RC2PassVBRMaxsectionPct( &m_lRC2PassVBRMaxsectionPct );
				pAVSVPXFormat->get_KeyFrameMode( &m_lKeyFrameMode );
				pAVSVPXFormat->get_KeyFrameMinDist( &m_lKeyFrameMinDist );
				pAVSVPXFormat->get_KeyFrameMaxDist( &m_lKeyFrameMaxDist );

				RELEASEARRAY(m_parExtraData);
				pAVSVPXFormat->get_ExtraData( &m_parExtraData );

				RELEASEINTERFACE( pAVSVPXFormat );
			}
		}
		return S_OK;
	}
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		//Здесь проверяем параметры относящиеся к AVSVPXVideoFormat
		return CAVSVideoFormatImpl<CoT, T>::SetAdditionalParam(ParamName, ParamValue);
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		//Здесь проверяем параметры относящиеся к AVSVPXVideoFormat
		return CAVSVideoFormatImpl<CoT, T>::GetAdditionalParam(ParamName, ParamValue);
	}

	LINK_PROPERTY_LONG( EncodingTime, m_lEncodingTime )
	LINK_PROPERTY_LONG( ThreadCount, m_lThreadCount )
	LINK_PROPERTY_LONG( Profile, m_lProfile )
	LINK_PROPERTY_LONG( ErrorResilient, m_lErrorResilient )
	LINK_PROPERTY_LONG( MultiPassMode, m_lMultiPassMode )
	LINK_PROPERTY_LONG( LagInFrames, m_lLagInFrames )
	LINK_PROPERTY_LONG(	RCDropFrameThresh, m_lRCDropFrameThresh )
	LINK_PROPERTY_LONG(	RCResizeAllowed, m_lRCResizeAllowed )
	LINK_PROPERTY_LONG(	RCResizeUpThresh, m_lRCResizeUpThresh )
	LINK_PROPERTY_LONG(	RCResizeDownThresh, m_lRCResizeDownThresh )
	LINK_PROPERTY_LONG(	RCEndUsage, m_lRCEndUsage )
	LINK_PROPERTY_BSTR(	RCStatFileName, m_sRCStatFileName )
	LINK_PROPERTY_LONG(	RCMinQuantizer, m_lRCMinQuantizer )
	LINK_PROPERTY_LONG(	RCMaxQuantizer, m_lRCMaxQuantizer )
	LINK_PROPERTY_LONG(	RCUndershootPct, m_lRCUndershootPct )
	LINK_PROPERTY_LONG(	RCOvershootPct, m_lRCOvershootPct )
	LINK_PROPERTY_LONG(	RCBufferSize, m_lRCBufferSize )
	LINK_PROPERTY_LONG(	RCBufferInitSize, m_lRCBufferInitSize )
	LINK_PROPERTY_LONG(	RCBufferOptSize, m_lRCBufferOptSize )
	LINK_PROPERTY_LONG(	RC2PassVBRBiasPct, m_lRC2PassVBRBiasPct )
	LINK_PROPERTY_LONG(	RC2PassVBRMinsectionPct, m_lRC2PassVBRMinsectionPct )
	LINK_PROPERTY_LONG(	RC2PassVBRMaxsectionPct, m_lRC2PassVBRMaxsectionPct )
	LINK_PROPERTY_LONG(	KeyFrameMode, m_lKeyFrameMode )
	LINK_PROPERTY_LONG(	KeyFrameMinDist, m_lKeyFrameMinDist )
	LINK_PROPERTY_LONG(	KeyFrameMaxDist, m_lKeyFrameMaxDist )

	LINK_PROPERTY_SAFEARRAY(ExtraData, m_parExtraData)

protected:

	LONG m_lEncodingTime;
	LONG m_lThreadCount;
	LONG m_lProfile;
	LONG m_lErrorResilient;
	LONG m_lMultiPassMode;
	LONG m_lLagInFrames;
	LONG m_lRCDropFrameThresh;
	LONG m_lRCResizeAllowed;
	LONG m_lRCResizeUpThresh;
	LONG m_lRCResizeDownThresh;
	LONG m_lRCEndUsage;
	CString m_sRCStatFileName;
	LONG m_lRCMinQuantizer;
	LONG m_lRCMaxQuantizer;
	LONG m_lRCUndershootPct;
	LONG m_lRCOvershootPct;
	LONG m_lRCBufferSize;
	LONG m_lRCBufferInitSize;
	LONG m_lRCBufferOptSize;
	LONG m_lRC2PassVBRBiasPct;
	LONG m_lRC2PassVBRMinsectionPct;
	LONG m_lRC2PassVBRMaxsectionPct;
	LONG m_lKeyFrameMode;
	LONG m_lKeyFrameMinDist;
	LONG m_lKeyFrameMaxDist;

	LPSAFEARRAY m_parExtraData;
};

// CAVSVPXVideoFormat

[
	coclass,
	default(IAVSVPXVideoFormat),
	threading(apartment),
	vi_progid("AVSMediaFormatSettings3.AVSVPXVideoForm"),
	progid("AVSMediaFormatSettings3.AVSVPXVideoFo.1"),
	version(1.0),
	uuid("68C609AF-BC6B-4247-ACF8-D1083175D5A3"),
	helpstring("AVSVPXVideoFormat Class")
]

class ATL_NO_VTABLE CAVSVPXVideoFormat : public CAVSVPXVideoFormatImpl<CAVSVPXVideoFormat, IAVSVPXVideoFormat>
{

};