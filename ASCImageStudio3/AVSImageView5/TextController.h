#pragma once

#include "ITextController.h"
#include <TrackTextRotable.h>

namespace TextControllerCommands
{
	static const unsigned long	SetBrush					=	0x10001000;
	static const unsigned long	SetFont						=	0x10001010;
	static const unsigned long	SetEdgeText					=	0x10001020;
	static const unsigned long	SetPen						=	0x10001030;
	static const unsigned long	SetShadow					=	0x10001040;

	static const unsigned long	SetFontSize					=	0x10001050;
	static const unsigned long	SetFontType					=	0x10001051;
	static const unsigned long	SetFontName					=	0x10001052;

	static const unsigned long	BeginEdit					=	0x10002010;
	static const unsigned long	EndEdit						=	0x10002020;
	static const unsigned long	SelectTextAll				=	0x10002000;

	static const unsigned long	NotAutoFitText				=	0x10003010;	
	static const unsigned long	ResizeShapeToFitText		=	0x10003020;
	static const unsigned long	ScrollingText				=	0x10003030;
};

[
	coclass,
	threading(apartment),
	default(ITextController),
	event_source(com),
	vi_progid("AVSImageView5.TextTrackController"),
	progid("AVSImageView5.TextTrackController.1"),
	version(1.0),
	uuid("BED9DB52-42BA-469b-9D51-AF4A4FAFB51B"),
	helpstring("CTextController Class")
]

class ATL_NO_VTABLE CTextController : public ITextController
{
public:

	__event __interface _ITextControllerEvents;
	
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct ()
	{
		return S_OK;
	}

	VOID FinalRelease ()
	{

	}


public:
	
	CTextController ()
	{
		m_pTracking = NULL;

	}
	~CTextController ()
	{

	}


	
	inline void SetTracking ( Tracking::CTrackObject* pTracking )
	{
		if ( NULL != pTracking )
			m_pTracking		=	static_cast<Tracking::CTrackTextRotatable*> ( pTracking );
	}

	STDMETHOD ( ITextController::Command )		( LONG Command, VARIANT Value, VARIANT* pGetOut, VARIANT_BOOL* Status )
	{
		if ( NULL != m_pTracking )
		{
			switch ( Command )
			{
			case TextControllerCommands::SetBrush:
				{
					m_pTracking->SetBrush ( CString ( W2BSTR ( Value.bstrVal ) ) );
					m_pTracking->Update ( );
				}
				break;
			
			case TextControllerCommands::SetFont:
				{
					m_pTracking->SetFont ( CString ( W2BSTR ( Value.bstrVal ) ) );
					m_pTracking->Update ( );
				}
				break;
				
			case TextControllerCommands::SetEdgeText:
				{
					m_pTracking->SetEdgeText ( CString ( W2BSTR ( Value.bstrVal ) ) );
					m_pTracking->Update ( );
				}
				break;

			case TextControllerCommands::NotAutoFitText:
				{
					m_pTracking->SetNotAutoFitText ( true );
					m_pTracking->Update ( );
				}
				break;
				
			case TextControllerCommands::ResizeShapeToFitText:
				{
					m_pTracking->SetResizeShapeToFitText ( true );
					m_pTracking->Update ( );
				}
				break;

			case TextControllerCommands::ScrollingText:
				{
					m_pTracking->SetScrollingText ( true );
					m_pTracking->Update ( );
				}
				break;

			case TextControllerCommands::SetFontName:
				{
					m_pTracking->SetFontName ( CString ( W2BSTR ( Value.bstrVal ) ) );
					m_pTracking->Update ( );
				}
				break;

			case TextControllerCommands::SetFontSize:
				{
					m_pTracking->SetFontSize ( Value.lVal );
					m_pTracking->Update ( );
				}
				break;
		
			case TextControllerCommands::SetFontType:
				{
					m_pTracking->SetFontType ( Value.lVal );
					m_pTracking->Update ( );
				}
				break;

			case TextControllerCommands::SelectTextAll: 
				{
					m_pTracking->SelectTextAll ( );
					m_pTracking->Update ( );
				}
				break;
		
			case TextControllerCommands::BeginEdit: 
				{
					m_pTracking->BeginEditText ( );
				}
				break;
			
			case TextControllerCommands::EndEdit: 
				{
					m_pTracking->EndEditText ( );
				}
				break;
			}
		}

		pGetOut->vt			=	VT_UNKNOWN;
		pGetOut->punkVal	=	NULL;

		*Status	= VARIANT_TRUE;
	
		return S_OK;
	}


	//
	STDMETHOD ( ITextController::BeginEdit )	( VARIANT_BOOL* Status )
	{
		*Status	=	VARIANT_FALSE;

		if ( NULL != m_pTracking )
		{
			m_pTracking->BeginEditText ();

			*Status	=	VARIANT_TRUE;
		}
		return S_OK;
	}

	STDMETHOD ( ITextController::EndEdit )		( VARIANT_BOOL* Status )
	{
		*Status	=	VARIANT_FALSE;

		if ( NULL != m_pTracking )
		{
			m_pTracking->EndEditText ();
			*Status	=	VARIANT_TRUE;
		}

		return S_OK;
	}


	//
	STDMETHOD ( ITextController::SetParam )		( BSTR ParamName, VARIANT ParamValue )
	{
		return S_OK;
	}
	STDMETHOD ( ITextController::GetParam )		( BSTR ParamName, VARIANT* ParamValue )
	{
		return S_OK;
	}

private:

	Tracking::CTrackTextRotatable*	m_pTracking;
};
