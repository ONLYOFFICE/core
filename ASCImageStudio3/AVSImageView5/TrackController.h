#pragma once

#include "ITrackController.h"
#include "Tracking.h"

[
	coclass,
	threading(single),
	default(ITrackController),
	vi_progid("AVSImageView5.TrackController"),
	progid("AVSImageView5.TrackController.1"),
	version(1.0),
	uuid("3A072816-9A83-4c75-B4CE-53D55C0D01CB"),
	helpstring("CTrackController Class")
]

class ATL_NO_VTABLE CTrackController : public ITrackController
{
public:

	DECLARE_PROTECT_FINAL_CONSTRUCT()
	HRESULT FinalConstruct ()
	{
		return S_OK;
	}

	VOID FinalRelease ()
	{

	}


public:

	CTrackController ()
	{
		m_pTracking		=	NULL;
	}


	//
	STDMETHOD ( ITrackController::get_PointsCount )		( LONG* pValue )
	{
		*pValue	=	0;

		if ( NULL != m_pTracking )
		{
			*pValue	=	m_pTracking->GetCount();
		}

		return S_OK;
	}

	STDMETHOD ( ITrackController::get_PointX )			( LONG Index, DOUBLE* pValue )
	{
		*pValue	=	0;

		if ( NULL != m_pTracking )
		{
			*pValue	=	 AspectXAdd ( m_pTracking->GetAt(Index).dX );
		}

		return S_OK;
	}

	STDMETHOD ( ITrackController::put_PointX )			( LONG Index, DOUBLE Value )
	{
		if ( NULL != m_pTracking )
		{
			m_pTracking->operator [] (Index).dX = Value;
		}

		return S_OK;
	}

	STDMETHOD ( ITrackController::get_PointY )			( LONG Index, DOUBLE* pValue )
	{
		*pValue	=	0;

		if ( NULL != m_pTracking )
		{
			*pValue	=	 AspectYAdd ( m_pTracking->GetAt(Index).dY );
		}

		return S_OK;
	}
	STDMETHOD ( ITrackController::put_PointY )			( LONG Index, DOUBLE Value )
	{
		if ( NULL != m_pTracking )
		{
			m_pTracking->operator [] (Index).dY = Value;
		}

		return S_OK;
	}
	STDMETHOD ( ITrackController::SetAdditionalParam )	( BSTR ParamName, VARIANT ParamValue )
	{
		return S_OK;
	}

	STDMETHOD ( ITrackController::GetAdditionalParam )	( BSTR ParamName, VARIANT* ParamValue )
	{
		return S_OK;
	}



	//
	inline void SetTracking ( Tracking::CTrackObject* pTracking )
	{
		m_pTracking		=	pTracking;
	}

	inline void SetSizeParams	( double nAspect, int nAspectWidth, int nAspectHeight, int nWidth, int nHeight  )
	{
		m_nAspect		= nAspect;
		m_nAspectWidth	= nAspectWidth;
		m_nAspectHeight	= nAspectHeight;
		m_nWidth		= nWidth;
		m_nHeight		= nHeight;
	}

private:
	//
	inline double AspectXAdd	( double x )
	{
		if ( m_nAspect < 0.001 )
			return x;

		return m_nAspectWidth * x / m_nWidth;
	}

	inline double AspectYAdd	( double y )
	{
		if ( m_nAspect < 0.001 )
			return y;

		return m_nAspectHeight * y / m_nHeight;
	}

private:

	Tracking::CTrackPoints*	m_pTracking;

	double					m_nAspect;
	int						m_nAspectWidth;
	int						m_nAspectHeight;
	int						m_nWidth;
	int						m_nHeight;
};
