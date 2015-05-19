#pragma once

#include "XmlTrackingSerialize.h"
#include "TrackCubicBezierCurve.h"
#include "TrackExternal.h"
#include "Tracking.h"

using namespace Tracking;
using namespace Structures;

namespace Core
{
	class CTrackManagerImp : public ITracking
	{
	public:
		CTrackManagerImp(void)
		{
			m_pReceiver		=	NULL;
			m_ExternalTrack	=	NULL;

			m_nAspect		=	0.0;
			m_nAspectWidth	=	0;
			m_nAspectHeight	=	0;
			m_nWidth		=	0;
			m_nHeight		=	0;
		}

		~CTrackManagerImp(void)
		{
			Clear ();
		
			ClearPainters ();
		}

	public:
		
		inline bool BeginTracks ( BSTR Xml )
		{
			return ReadXmlTracks ( Xml );
		}

		inline bool EndTracks ()
		{
			Clear ();

			return true;
		}

		
		inline bool AddTrack ( int Index, BSTR Xml )
		{
			Serialize::CTrackXmlReader oReader ( m_nAspect, m_nAspectWidth, m_nAspectHeight, m_nWidth, m_nHeight );

			Tracking::CTrackObject* pTrackObject = NULL;
			if ( oReader.ReadXml ( &pTrackObject, CString ( Xml ) ) )
			{
				if ( NULL != pTrackObject )
				{
					pTrackObject->SetReceiver ( m_pReceiver );

					if ( Index < (int)m_arrTracks.GetCount () && Index >= 0 )
						m_arrTracks.InsertAt ( Index, pTrackObject );
					else
						m_arrTracks.Add ( pTrackObject );

					UpdateID ();

					return true;
				}
			}

			return false;
		}

		inline bool DeleteTrack ( int Index )
		{
			if ( false == IsEnabled () )
				return false;

			if ( Index < (int)m_arrTracks.GetCount () && Index >= 0 )
			{
				RELEASEOBJECT ( m_arrTracks[Index] );
				m_arrTracks.RemoveAt ( Index );
				
				UpdateID ();

				return true;
			}

			return false;
		}

		inline bool ReplaceTrack ( int Index, BSTR Xml )
		{
			return ReadXmlTrack ( Index, Xml );
		}
		
		inline bool ReadXmlTracks ( BSTR Xml )
		{
			EndTracks ();
			
			if ( m_oXmlSerialize.ReadXml ( CString ( Xml ), m_arrTracks, m_nAspect, m_nAspectWidth, m_nAspectHeight, m_nWidth, m_nHeight ) )
			{
				for ( size_t i = 0; i < m_arrTracks.GetCount (); ++i )
					m_arrTracks[i]->SetReceiver ( m_pReceiver );

				UpdateID ();

				return true;
			}

			return false;
		}

		inline bool ReadXmlTrack ( int Index, BSTR Xml )
		{
			if ( Index < (int)m_arrTracks.GetCount () && Index >= 0 )
			{
				Serialize::CTrackXmlReader oReader ( m_nAspect, m_nAspectWidth, m_nAspectHeight, m_nWidth, m_nHeight );

				Tracking::CTrackObject* pTrackObject = NULL;
				if ( oReader.ReadXml ( &pTrackObject, CString ( Xml ) ) )
				{
					if ( NULL != pTrackObject )
					{
						RELEASEOBJECT ( m_arrTracks[Index] );
						m_arrTracks[Index] = pTrackObject;
						m_arrTracks[Index]->SetReceiver ( m_pReceiver );

						UpdateID ();

						return true;
					}
				}
			}
			return false;
		}

		
		inline bool WriteXmlTracks ( BSTR* Xml )
		{
			if ( m_oXmlSerialize.WriteXml ( Xml, m_arrTracks, m_nAspect, m_nAspectWidth, m_nAspectHeight, m_nWidth, m_nHeight) )
				return true;

			*Xml = CString(_TEXT("")).AllocSysString();
			return false;
		}

		inline bool WriteXmlTrack ( int Index, BSTR* Xml )
		{
			if ( Index < (int)m_arrTracks.GetCount () && Index >= 0 )
			{
				Serialize::CTrackXmlWriter oWriter ( m_nAspect, m_nAspectWidth, m_nAspectHeight, m_nWidth, m_nHeight );

				CString TrackXml;
				if ( oWriter.WriteXml ( m_arrTracks[Index], TrackXml ) )
				{
					*Xml = TrackXml.AllocSysString();
					return false;
				}
			}

			*Xml = CString(_TEXT("")).AllocSysString();
			return false;
		}

		
		inline bool ReadXmlSettings ( BSTR Xml )
		{
			ClearPainters ();

			if ( m_oXmlSettings.ReadXml ( CString ( Xml ), m_oTrackingPainter, m_arrPainters ) )
				return true;

			return false;
		}

		inline bool WriteXmlSettings ( BSTR* Xml )
		{
			if ( m_oXmlSettings.WriteXml ( Xml, m_oTrackingPainter, m_arrPainters ) )
				return true;

			return false;
		}

		
		inline bool IsEnabled ()
		{
			if ( 0 != m_arrTracks.GetCount () )
				return true;

			return false;
		}
		
		inline void SetViewParametres ( double nAspect, int nAspectWidth, int nAspectHeight, int nWidth, int nHeight )
		{
			m_nAspect		= nAspect;
			m_nAspectWidth	= nAspectWidth;
			m_nAspectHeight	= nAspectHeight;
			m_nWidth		= nWidth;
			m_nHeight		= nHeight;
		}
		
		// ITracking
		virtual bool ITracking::MoveTrack ( double dX, double dY )
		{
			if ( false == IsEnabled () )
				return false;

			for ( size_t i = 0; i < m_arrTracks.GetCount (); ++i )
				m_arrTracks[i]->MoveTrack ( dX, dY );

			return true;
		}

		virtual void ITracking::SetReceiver ( CMessageReceiver* pReceiver )
		{
			m_pReceiver = pReceiver;
		}

		virtual bool ITracking::OnPaint ( HDC hDC, RECTI& rectWindow, RECTD& rectData )
		{
			if ( false == IsEnabled () )
				return false;

			if ( m_arrPainters.GetCount () > 0 && m_arrPainters.GetCount () >= m_arrTracks.GetCount () )
			{
				for ( size_t i = 0; i < m_arrTracks.GetCount (); ++i )
				{
					m_arrPainters[i]->Create ( hDC );
					m_arrTracks[i]->OnPaint ( *m_arrPainters[i], rectWindow, rectData );
					m_arrPainters[i]->Destroy ();
				}
			}
			else
			{
				m_oTrackingPainter.Create ( hDC );

				for ( size_t i = 0; i < m_arrTracks.GetCount (); ++i )
					m_arrTracks[i]->OnPaint ( m_oTrackingPainter, rectWindow, rectData );

				m_oTrackingPainter.Destroy ();
			}

			return true;
		}


		virtual bool ITracking::OnSetCursor ( int nX, int nY, int& nPointStatus, int& nChangeStatus )
		{
			if ( false == IsEnabled () )
				return false;

			// первый снизу и т.д. установка курсора через реверс
			for ( int i = (int)m_arrTracks.GetCount () - 1; i >= 0; --i )
			{
				m_arrTracks[i]->OnSetCursor ( nX, nY, nPointStatus, nChangeStatus );
				
				if ( nPointStatus != c_nTrackPointStatusNone && nChangeStatus != c_nTrackChangeStatusNone )
					break;
			}

			return false;
		}
		virtual bool ITracking::OnMessages ( UINT nMessage, WPARAM wParam, LPARAM lParam )
		{
			if ( false == IsEnabled () )
				return false;

			/*
			switch ( nMessage)
			{
			case WM_LBUTTONDOWN:
				CTrackManagerImp::OnMouseMove ( GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) );
				break;
			case WM_RBUTTONDOWN:
				CTrackManagerImp::OnMouseMove ( GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) );
				break;
			case WM_MBUTTONDOWN:
				CTrackManagerImp::OnMouseMove ( GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) );
				break;
			case WM_LBUTTONUP: 
				CTrackManagerImp::OnMouseMove ( GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) );
				break;
			case WM_RBUTTONUP: 
				CTrackManagerImp::OnMouseMove ( GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) );
				break;
			case WM_MBUTTONUP:
				CTrackManagerImp::OnMouseMove ( GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) );
				break;
			case WM_LBUTTONDBLCLK:
				CTrackManagerImp::OnMouseMove ( GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) );
				break;
			case WM_RBUTTONDBLCLK: 
				CTrackManagerImp::OnMouseMove ( GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) );
				break;
			case WM_MBUTTONDBLCLK:
				CTrackManagerImp::OnMouseMove ( GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) );
				break;
			case WM_MOUSEMOVE:
				CTrackManagerImp::OnMouseMove ( GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) );
				break;
			default:
				CTrackManagerImp::OnDeafultMessageHandler ( nMessage, wParam, lParam );
				break;
			}
			*/

			// обходим треки в реверс
			for ( int i = (int)m_arrTracks.GetCount () - 1; i >= 0; --i )
			{
				if ( nMessage == WM_LBUTTONDOWN )
				{
					// проверяем попадаение в трек с данными кординатами 
					if ( -1 != m_arrTracks[i]->GetPointUnderCursor ( GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) ) )
					{
						// отработать захват объекта (трека), можем для одного объекта 
						m_arrTracks[i]->OnMessages ( nMessage, wParam, lParam );
						break;
					}
				}
				else
				{
					m_arrTracks[i]->OnMessages ( nMessage, wParam, lParam );
				}
			}

			return true;
		}

		virtual bool ITracking::Command ( BSTR CommandXml, VARIANT* pParamValue )
		{
			return Core::Serialize::Command::Execute ( CommandXml, pParamValue, m_arrTracks );
		}

		
		// IMouseButtonHandlers
		virtual bool IMouseButtonHandlers::OnMouseMove ( int MouseX, int MouseY )
		{
			return false;
		}


		virtual bool IMouseButtonHandlers::OnLButtonDown ( int MouseX, int MouseY )
		{
			return false;
		}


		virtual bool IMouseButtonHandlers::OnLButtonUp ( int MouseX, int MouseY )
		{
			return false;
		}


		virtual bool IMouseButtonHandlers::OnRButtonDown ( int MouseX, int MouseY )
		{
			return false;
		}


		virtual bool IMouseButtonHandlers::OnRButtonUp ( int MouseX, int MouseY )
		{
			return false;
		}


		virtual bool IMouseButtonHandlers::OnMButtonDown ( int MouseX, int MouseY )
		{
			return false;
		}


		virtual bool IMouseButtonHandlers::OnMButtonUp ( int MouseX, int MouseY )
		{
			return false;
		}
	
		virtual bool OnDefaultMessageHandler ( UINT nMessage, WPARAM wParam, LPARAM lParam )
		{
			return false;
		}


		CTrackObject*	GetTrack ( long Index )
		{
			if ( false == IsEnabled () )
				return false;
			
			if ( Index < (int)m_arrTracks.GetCount () && Index >= 0 )
				return m_arrTracks[Index];

			return NULL;
		}


		inline void		SetHWND ( HWND hWnd )
		{
			for ( int i = 0; i < (int)m_arrTracks.GetCount (); ++i )
			{
				if ( m_arrTracks [ i ]->IsTrackText () )
				{
					((Tracking::CTrackObjectText*)m_arrTracks [ i ])->GetIMEUI().Initialize ( hWnd );
				}
				
				if ( m_arrTracks [ i ]->IsTrackTextRotatable () )
				{
					((Tracking::CTrackObjectTextRotatable*)m_arrTracks [ i ])->GetIMEUI().Initialize ( hWnd );
				}
			}
		}

	private:
		
		inline void Clear ()
		{
			for ( int i = 0; i < (int)m_arrTracks.GetCount (); ++i )
				RELEASEOBJECT ( m_arrTracks[i] );

			m_arrTracks.RemoveAll ();
 
			//ClearPainters ();
		}

		inline void ClearPainters ()
		{
			for ( int i = 0; i < (int)m_arrPainters.GetCount (); ++i )
			{
				RELEASEOBJECT ( m_arrPainters[i] );
			}

			m_arrPainters.RemoveAll ();
		}

		inline void UpdateID ()
		{
			for ( int i = 0; i < (int)m_arrTracks.GetCount (); ++i )
				 m_arrTracks[i]->SetTrackID ( (int)i );
		}
	
	private:

		CAtlArray<CTrackObject*>			m_arrTracks;
		CTrackExternal*						m_ExternalTrack;

		CAtlArray<CTrackPainter*>			m_arrPainters;

		Tracking::CTrackPainter				m_oTrackingPainter;
		

		CMessageReceiver*					m_pReceiver;

		double								m_nAspect;
		int									m_nAspectWidth;
		int									m_nAspectHeight;
		int									m_nWidth;
		int									m_nHeight;
		
		Core::Serialize::CTracksXml			m_oXmlSerialize;
		Core::Serialize::CTracksXmlSettings	m_oXmlSettings;
	};
}
