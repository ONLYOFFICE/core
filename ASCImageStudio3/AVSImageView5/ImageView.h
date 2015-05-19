#pragma once

#define	_USE_NEW_SCHEME_RENDER_

#include <windef.h>
#include <windowsx.h>
#include <AtlDefine.h>

#include <stdio.h>
#include <AtlCtl.h>
#include <Messages.h>
#include <Tracking.h>
//#include <windowsex.h>
#include <TimeMeasurer.h>
#include <Registration.h>
#include <ImageStudioUtils.h>

#include "Common.h"
#include "resource.h"
#include "Constants.h"
#include "TrackManager.h"
#include "TimeMeasurer.h"

#include "ImageView5.h"
//#include "TrackController.h"
//#include "TextController.h"

#ifdef _USE_NEW_SCHEME_RENDER_
	#include "X:\AVS\Sources\AVSImageStudio3\AVSImageEditor\AVSImageEditor\ViewManager.h"
	#include "XmlViewManagerSettings.h"
#else
	#include "x:\Common\WindowsEx.h"
#endif

#include "Tracking\WindowsEx.h"
#include "ImageStudioTextRender.h"

// ImageView5
[
	coclass,
	control,
	default(IImageView5),
	threading(apartment),
	vi_progid("AVSImageView5.ImageView5"),
	progid("AVSImageView5.ImageView5.1"),
	version(1.0),
	event_source(com),
	uuid("9391A367-EAC5-4D50-BC04-0FA2A3EAEF9A"),
	helpstring("ImageView5 Class"),
	support_error_info(IImageView5),
	registration_script("control.rgs")
]
class ATL_NO_VTABLE ImageView5 :
	public IImageView5,
	public IPersistStreamInitImpl<ImageView5>,
	public IOleControlImpl<ImageView5>,
	public IOleObjectImpl<ImageView5>,
	public IOleInPlaceActiveObjectImpl<ImageView5>,
	public IViewObjectExImpl<ImageView5>,
	public IOleInPlaceObjectWindowlessImpl<ImageView5>,
	public IPersistStorageImpl<ImageView5>,
	public ISpecifyPropertyPagesImpl<ImageView5>,
	public IQuickActivateImpl<ImageView5>,
	public IDataObjectImpl<ImageView5>,
	public IProvideClassInfo2Impl<&__uuidof(ImageView5), NULL>,
	public CComControl<ImageView5>,
	public Messages::CMessageReceiver
{
	__event __interface _ImageView5Events;

public:

	ImageView5()
	{
#ifdef _DEBUG
		int flag = _CrtSetDbgFlag ( _CRTDBG_REPORT_FLAG );	// Get current flag
		flag |= _CRTDBG_LEAK_CHECK_DF;						// Turn on leak-checking bit
		flag |= _CRTDBG_CHECK_ALWAYS_DF;					// Turn on CrtCheckMemory
		//	flag |= _CRTDBG_DELAY_FREE_MEM_DF;
		//_CrtSetBreakAlloc ( 760 );
		_CrtSetDbgFlag(flag);								// Set flag to the new value
#endif
		// initialize gdi+
		m_oGdiPlusInit.Init();

		// setup invalid image parameters
		m_nWidth					=	0;
		m_nHeight					=	0;
		m_pPixels					=	0;
		m_pMask						=	0;

		// setup additional helpers
		m_bAutoRefresh				=	TRUE;

		m_bShowBorder				=	TRUE;
		m_nWindowStyle				=	c_WindowStyleClient;

		m_nMaskStyle				=	c_MaskStyleDotsDense;
		m_nMaskColor				=	RGB(255, 0, 0);

		m_nBackStyle				=	c_BackStyleSolid;
		m_nBackColor1				=	ImageStudioUtils::GetColorLight(GetSysColor(COLOR_3DFACE), 16);
		m_nBackColor2				=	ImageStudioUtils::GetColorDark(GetSysColor(COLOR_3DFACE), 16);

		// default preview image offsets and zoomings
		m_dCenterX					=	0.0;
		m_dCenterY					=	0.0;
		m_dZoom						=	1.0;
		m_dAspect					=	-1.0;
		m_nAspectWidth				=	0;
		m_nAspectHeight				=	0;

		// disable mouse capturing flag - mouse unlocked by default
		m_nPointer					=	c_MousePointerDefault;

		// setup tracking object
		m_nTrackingColorScheme		=	c_TrackColorSchemeInverse;
		m_pTracking					=	0;

		// disallow windowless activation
		m_bWindowOnly				=	TRUE;

		m_oTrackManager.SetReceiver ( this );

		m_bUseExternalRender		=	false;

		m_nInterpolationMode		=	Core::PixelsOperations::sc_nInterpolationModeNearestNeighbor;

		//m_oCheckFeatures.Analyze ();

		m_pImageTracking			=	NULL;
		m_nImageTrackingAlpha		=	255L;

		m_pBitmap					=	NULL;
		m_pGrahics					=	NULL;

		m_bImageTrackingRebuild		=	false;
		m_bEnableOnPaintBlock		=	false;
		
		m_bEnableDrawText			=	true;
		m_bResizeShapeToFitText		=	false;
		m_bUseStudioTextRender		=	true;	// использовать дл€ отрисовки ImageStudio

#ifdef _USE_NEW_SCHEME_RENDER_
		
		m_oRenderManager.SetEnableGrid						( false );
		m_oRenderManager.SetBackClearColor					( 0xFF000000 );
		m_oRenderManager.SetZoomToFitOffsetPX				( 0.0 );

		m_oRenderManager.SetRenderMode ( ImageRendering::Constants::sc_RenderModeUseCopyImage,						FALSE );
		m_oRenderManager.SetRenderMode ( ImageRendering::Constants::sc_RenderModeUseAlphaLayer,						TRUE );
		m_oRenderManager.SetRenderMode ( ImageRendering::Constants::sc_RenderModeEnableImageSurface,				FALSE );
		m_oRenderManager.SetRenderMode ( ImageRendering::Constants::sc_RenderModeDisabledZoomToFitForSmallImages,	FALSE );
		m_oRenderManager.SetRenderMode ( ImageRendering::Constants::sc_RenderModeFastCopyBuffersWithBlackFone,		FALSE );

		m_oRenderManager.GetRullers ().SetRullerUp		( false );
		m_oRenderManager.GetRullers ().SetRullerDown	( false );
		m_oRenderManager.GetRullers ().SetRullerRight	( false );
		m_oRenderManager.GetRullers ().SetRullerLeft	( false );

		m_bEnableCustomizeScheme	=	false;
#endif
		
		m_nLMButtonState		=	0;
		m_nRMButtonState		=	0;
		m_nMMButtonState		=	0;

		/*
		
		if ( FAILED ( m_pTrackController.CoCreateInstance ( __uuidof ( CTrackController ) ) ) )
		{
			// ::MessageBox ( NULL, _T("Cannot create CTrackController COM object"), _T("Error"),MB_OK );
		}
	
		if ( FAILED ( m_pTextController.CoCreateInstance ( __uuidof( CTextController ) ) ) )
		{
			// ::MessageBox ( NULL, _T("Cannot create IViewSettings COM object"), _T("Error"),MB_OK );
		}
		
		*/

		m_bSetNoResizeRFAT	=	FALSE;
		m_bSizesResizeRFAT	=	FALSE;
	}

	~ImageView5()
	{
		Clear();
		
		RELEASEOBJECT		( m_pTracking );
		RELEASEINTERFACE	( m_pImageTracking );
		RELEASEOBJECT		( m_pBitmap );
		// RELEASEOBJECT	( m_pGrahics );
	}


	BOOL IsValid()
	{
		if (!m_pPixels || !m_nWidth || !m_nHeight || !IsWindow())
			return FALSE;

		return TRUE;
	}

	void Clear()
	{
		// destroy the image
		if ( m_pPixels )
		{
			delete[] m_pPixels;
			m_pPixels	=	NULL;
		}

		if ( m_pMask )
		{
			delete[] m_pMask;
			m_pMask		=	NULL;
		}

		//RELEASEOBJECT ( m_pTracking );

		m_nWidth = 0;
		m_nHeight = 0;
	}


	//HWND CreateControlWindow(HWND hWndParent, RECT& rcPos)
	//{
	//	//T* pT = static_cast<T*>(this);
	//	//return pT->Create(hWndParent, rcPos);
	//	m_hWndTimer = CComControl::CreateControlWindow ( hWndParent, rcPos );

	//	::SetTimer(	m_hWndTimer,        // handle to main window 
	//		IDT_TIMER1,         // timer identifier 
	//		1,               // 1-second interval 
	//		(TIMERPROC) NULL ); // no timer callback 

	//	return hWndParent;
	//}
public:

	DECLARE_VIEW_STATUS(0)
	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE | OLEMISC_CANTLINKINSIDE | OLEMISC_INSIDEOUT | OLEMISC_ACTIVATEWHENVISIBLE | OLEMISC_SETCLIENTSITEFIRST)

	BEGIN_PROP_MAP(ImageView5)
		PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
		PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	END_PROP_MAP()

	BEGIN_MSG_MAP(ImageView5)
		MESSAGE_RANGE_HANDLER(0, 0xFFFFFFFF, OnMessages)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

	// IViewObjectEx
	//DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

public:

	LRESULT OnMessages ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
	{
		// reset handling flag
		bHandled = FALSE;

		switch ( uMsg )
		{
		case WM_SETCURSOR:
			{
				bHandled	=	OnSetCursor ();
			}
			break;

		case WM_MOUSEMOVE:
			{
				// initialize track mouse event 
				tme.cbSize		= sizeof( TRACKMOUSEEVENT );
				tme.dwFlags		= TME_LEAVE;
				tme.hwndTrack	= m_hWnd;

				TrackMouseEvent( &tme );

				OnMouseEvent ( uMsg );
			}
			break;

		case WM_MOUSEWHEEL:
			{
				if ( IsWindow() )
				{
					SetFocus();
				}
				//LONG MouseX = LOWORD ( lParam );
				//LONG MouseY = GET_Y_LPARAM ( lParam );

				int Delta = GET_WHEEL_DELTA_WPARAM ( wParam );

				MouseMove ( WM_MOUSEWHEEL, -1, (double)wParam, Delta );
			}
			break;

		case WM_LBUTTONDBLCLK:
			{
				if ( IsWindow() )
				{
					SetFocus();
				}
			
				OnMouseEvent ( uMsg );
			}
			break;

		case WM_ERASEBKGND:
			{
				bHandled	=	TRUE;

				return 0;
			}
			break;

		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = ::BeginPaint ( m_hWnd, &ps );
				OnPaint ( c_SenderDraw, hdc );
				EndPaint ( NULL );

				bHandled	=	TRUE; 

				return TRUE;
			}
			break;

		case WM_PRINT:
			{
				HDC hDC = (HDC)wParam;
				if ( NULL != hDC )
				{
					OnPrint ( c_SenderDraw, hDC );
				}

				return 0;
			}
			break;

		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONUP: 	
		case WM_MBUTTONUP: 
		case WM_RBUTTONDBLCLK:
		case WM_MBUTTONDBLCLK:
			{
				OnMouseEvent ( uMsg );
			}
			break;
		}

		if ( m_pTracking )
			m_pTracking->OnMessages ( uMsg, wParam, lParam );
		else
			m_oTrackManager.OnMessages ( uMsg, wParam, lParam );

		///*
		// setup mouse cursor if 
		//if (uMsg == WM_SETCURSOR)
		//	bHandled = OnSetCursor();

		//if (uMsg == WM_MOUSEMOVE)
		//{
		//	// initialize track mouse event 
		//	tme.cbSize		= sizeof( TRACKMOUSEEVENT );
		//	tme.dwFlags		= TME_LEAVE;
		//	tme.hwndTrack	= m_hWnd;

		//	TrackMouseEvent( &tme );			
		//}
		//
		//if (IsWindow())
		//{
		//	if (uMsg == WM_LBUTTONDBLCLK)
		//	{
		//		SetFocus();
		//	}
		//}

		// handle mouse events - send necessary event to parent
		//if ( uMsg == WM_LBUTTONDOWN || uMsg == WM_RBUTTONDOWN || uMsg == WM_MBUTTONDOWN ||
		//	uMsg == WM_LBUTTONUP || uMsg == WM_RBUTTONUP || uMsg == WM_MBUTTONUP ||
		//	// uMsg == WM_LBUTTONDBLCLK ||
		//	uMsg == WM_RBUTTONDBLCLK || uMsg == WM_MBUTTONDBLCLK 
		//	//||	uMsg == WM_MOUSEMOVE
		//	)
		//	OnMouseEvent(uMsg);

		// allow trackers to handle its own events
		//if ( m_pTracking )
		//	m_pTracking->OnMessages ( uMsg, wParam, lParam );
		//else
		//	m_oTrackManager.OnMessages ( uMsg, wParam, lParam );

		//// handle painting messages
		//if ( uMsg == WM_ERASEBKGND )
		//{
		//	bHandled = TRUE;
		//}
		//else if ( uMsg == WM_PAINT )
		//{
		//	HDC hdc = BeginPaint (  NULL );
		//	OnPaint ( c_SenderDraw, hdc );
		//	EndPaint ( NULL );

		//	bHandled	=	FALSE; // ыD БEБEть - иначБEБEдвисаеБEъь БEстБEыLьK painting'БE
		//}

		return 0;
	}

	BOOL OnPaint ( int nSender, HDC hdc = NULL )
	{
		// check for autorefresh flag
		if (!m_bAutoRefresh && nSender != c_SenderRefresh)
			return FALSE;

		if (!IsWindow())
			return FALSE;

#ifdef _USE_NEW_SCHEME_RENDER_

		if ( m_bEnableCustomizeScheme )
		{
			RenderCustomizeScheme ( nSender, hdc );

			return TRUE;
		}
#endif
		// retrieve window rectangle and check for valid size
		RECT rect; GetClientRect(&rect);
		if (rect.right - rect.left < 1 || rect.bottom - rect.top < 1)
			return FALSE;
		
		HDC	hPreviewDC = ( hdc == NULL ) ? GetDC () : hdc;

		m_oDoubleBuffer.Create(hPreviewDC, rect, TRUE);

		int nWidth  = m_oDoubleBuffer.GetRect().GetWidth();
		int nHeight = m_oDoubleBuffer.GetRect().GetHeight();

		// extract last frame (skip mouse tracking case)
		BOOL bScreenValid = IsValid();
		if (nSender != c_SenderTrack)
		{
			BYTE* pPixels = m_oDoubleBuffer.GetBackBitmapPixels();

			if ( m_nInterpolationMode == Core::PixelsOperations::sc_nInterpolationModeBicubic )
			{
				if( FALSE == IPP_ExtractRect(nWidth, nHeight, 4, pPixels) )
					bScreenValid = ExtractRect ( nWidth, nHeight, 4, pPixels );
				else
					bScreenValid = TRUE;
			}
			else
			{
				if( FALSE == IPP_ExtractRect(nWidth, nHeight, 4, pPixels) )
					bScreenValid = UpdateBackGroundPixels ( nWidth, nHeight, 4, pPixels );
				else
					bScreenValid = TRUE;
			}
		}

		// draw the screen
		if (bScreenValid)
		{
			m_oDoubleBuffer.RenderBackBitmapPixels();
		
			DrawTrackingImage ();

			if ( false == m_bEnableOnPaintBlock )
				ShowTracking ( m_oDoubleBuffer.GetBackDC(), rect );
		}
		else
		{
			Rendering::Gdi::FillRectangle(m_oDoubleBuffer.GetBackDC(), m_nBackColor1, rect);
		}

		ChangeBackBuffer( (INT_PTR)m_oDoubleBuffer.GetBackDC() );

		// show window borders
		ShowWindowBorders ( m_oDoubleBuffer.GetBackDC(), rect );
		
		// flip back buffer to the screen
		m_oDoubleBuffer.Render();

		ReleaseDC(hPreviewDC);

		return TRUE;
	}

	BOOL OnPrint ( int nSender, HDC hDC )
	{
#ifdef _USE_NEW_SCHEME_RENDER_

		if ( m_bEnableCustomizeScheme )
		{
			BOOL Source = FALSE;

			RECT rect = { 0, 0, 0, 0 }; 

			HBITMAP hBitmap		=	(HBITMAP)GetCurrentObject ( hDC, OBJ_BITMAP );
			BITMAP bitmap;
			if ( NULL != GetObjectW ( hBitmap, sizeof(bitmap), (LPVOID)&bitmap ) )
			{
				rect.right			=	bitmap.bmWidth;		
				rect.bottom			=	bitmap.bmHeight;
				Source				=	TRUE;
			}

			if ( FALSE == Source )
			{
				HWND hWnd			=	::WindowFromDC ( hDC );
				if ( NULL == hWnd )
					return FALSE;
				if ( FALSE == ::GetClientRect ( hWnd, &rect ) )
					return FALSE;
			}

			if ( rect.right - rect.left < 1 || rect.bottom - rect.top < 1 )
				return FALSE;
			
			if ( FALSE == ::GetClientRect ( m_hWnd, &rect ) )
				return FALSE;

			m_oDoubleBuffer.Create ( hDC, rect, TRUE );

			// extract last frame (skip mouse tracking case)
			BOOL bScreenValid = IsValid();
			if (nSender != c_SenderTrack)
			{
				m_oRenderManager.SetHWND ( m_hWnd );
				
				m_oRenderManager.BeginPaint ( m_oDoubleBuffer.GetBackDC(), rect, m_oDoubleBuffer.GetBackBitmapPixels () );

				m_oRenderManager.EndPaint ();
			}

			// draw the screen
			if ( bScreenValid )
			{
				RenderBackBitmapPixels ( m_oDoubleBuffer.GetRect().GetWidth(), m_oDoubleBuffer.GetRect ().GetHeight (), m_oDoubleBuffer.GetBackBitmapPixels (), m_oDoubleBuffer.GetBackDC () );

				DrawTrackingImage ();

				if ( false == m_bEnableOnPaintBlock )
				{
					if ( m_oRenderManager.GetZoomToWindow () )
					{
						m_oRenderManager.ZoomToWindow ();

						m_dZoom		=	m_oRenderManager.GetZoom ();
						m_dCenterX	=	m_oRenderManager.GetCenterX ();
						m_dCenterY	=	m_oRenderManager.GetCenterY ();
					}

					ShowTracking ( m_oDoubleBuffer.GetBackDC(), rect );
				}
			}
			else
			{
				Rendering::Gdi::FillRectangle(m_oDoubleBuffer.GetBackDC(), m_nBackColor1, rect);
			}

			ChangeBackBuffer ( (INT_PTR)m_oDoubleBuffer.GetBackDC() );

			// show window borders
			ShowWindowBorders ( m_oDoubleBuffer.GetBackDC(), rect );

			// flip back buffer to the screen
			m_oDoubleBuffer.Render();
		}
#endif
		return TRUE;
	}
	BOOL OnSetCursor()
	{
		// variables
		POINT point; GetCursorPos(&point); ::ScreenToClient(m_hWnd, &point);

		// compute cursor style
		int nPointer = m_nPointer;

		if (m_nPointer == c_MousePointerDefault)
		{
			//if (!m_pTracking)
			//	return FALSE;

			int nPointStatus			= Tracking::c_nTrackPointStatusNone;
			int nTrackChangeStatus		= Tracking::c_nTrackChangeStatusNone;

			if ( NULL != m_pTracking )
				m_pTracking->OnSetCursor(point.x, point.y, nPointStatus, nTrackChangeStatus);
			else
				m_oTrackManager.OnSetCursor ( point.x, point.y, nPointStatus, nTrackChangeStatus );

			if (nPointStatus == Tracking::c_nTrackPointStatusPoint) // hand arrow
				nPointer = c_MousePointerHand;
			else if (nPointStatus == Tracking::c_nTrackPointStatusHorizontal) // - size
				nPointer = c_MousePointerSizeWE;
			else if (nPointStatus == Tracking::c_nTrackPointStatusVertical) // | size
				nPointer = c_MousePointerSizeNS;
			else if (nPointStatus == Tracking::c_nTrackPointStatusSize1) // \ size
				nPointer = c_MousePointerSizeNWSE;
			else if (nPointStatus == Tracking::c_nTrackPointStatusSize2) // / size
				nPointer = c_MousePointerSizeNESW;
			else if (nPointStatus == Tracking::c_nTrackPointStatusSizeAll) // + size
				nPointer = c_MousePointerSizeAll;
			else if (nPointStatus == Tracking::c_nTrackPointStatusCanAdd) // pick cross
				nPointer = c_MousePointerCross;
			else if (nPointStatus == Tracking::c_nTrackPointStatusRotation) // rotation cross
				nPointer = c_MousePointerRotation;
			else if (nPointStatus == Tracking::c_nTrackPointStatusIBeam) // edit text
				nPointer = c_MousePointerIBeam;
			else if (nPointStatus == Tracking::c_nTrackPointStatusSizeForRotation) // size for rotation
				nPointer = c_MousePointerSizeForRotation;
			else
				return FALSE;
		}

		// setup mouse pointer
		switch (nPointer)
		{
		case c_MousePointerArrow:			return (BOOL)SetCursor(LoadCursor(NULL, IDC_ARROW));
		case c_MousePointerCross:			return (BOOL)SetCursor(LoadCursor(NULL, IDC_CROSS));
		case c_MousePointerIBeam:			return (BOOL)SetCursor(LoadCursor(NULL, IDC_IBEAM));
		case c_MousePointerIcon:			return (BOOL)SetCursor(LoadCursor(NULL, IDC_ICON));
		case c_MousePointerSize:			return (BOOL)SetCursor(LoadCursor(NULL, IDC_SIZEALL));
		case c_MousePointerSizeNESW:		return (BOOL)SetCursor(LoadCursor(NULL, IDC_SIZENESW));
		case c_MousePointerSizeNS:			return (BOOL)SetCursor(LoadCursor(NULL, IDC_SIZENS));
		case c_MousePointerSizeNWSE:		return (BOOL)SetCursor(LoadCursor(NULL, IDC_SIZENWSE));
		case c_MousePointerSizeWE:			return (BOOL)SetCursor(LoadCursor(NULL, IDC_SIZEWE));
		case c_MousePointerUpArrow:			return (BOOL)SetCursor(LoadCursor(NULL, IDC_UPARROW));
		case c_MousePointerHourglass:		return (BOOL)SetCursor(LoadCursor(NULL, IDC_WAIT));
		case c_MousePointerNoDrop:			return (BOOL)SetCursor(LoadCursor(NULL, IDC_NO));
		case c_MousePointerArrowHourglass:	return (BOOL)SetCursor(LoadCursor(NULL, IDC_APPSTARTING));
		case c_MousePointerArrowQuestion:	return (BOOL)SetCursor(LoadCursor(NULL, IDC_HELP));
		case c_MousePointerSizeAll:			return (BOOL)SetCursor(LoadCursor(NULL, IDC_SIZEALL));
		case c_MousePointerHand:			return (BOOL)SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(32649)));
		case c_MousePointerRotation:		return (BOOL)SetCursor(LoadCursor(_AtlComModule.m_hInstTypeLib, MAKEINTRESOURCE(IDC_CURSOR2)));
		case c_MousePointerSizeForRotation:	return (BOOL)SetCursor(LoadCursor(_AtlComModule.m_hInstTypeLib, MAKEINTRESOURCE(IDC_CURSOR1)));
		}

		// invalid pointer id
		return FALSE;
	}

	BOOL OnMouseEvent(int nMessage)
	{
		// variables
		int nButton, nShift;
		double dX, dY;

		// retrieve current mouse state
		GetMouseState(nButton, nShift, dX, dY);	

		// fire the event
		if (nMessage == WM_LBUTTONDOWN || nMessage == WM_RBUTTONDOWN || nMessage == WM_MBUTTONDOWN)
		{
			SetCapture();
			MouseDown(nButton, nShift, AspectXAdd(dX), AspectYAdd(dY));
		}
		else if ( nMessage == WM_LBUTTONUP || nMessage == WM_RBUTTONUP || nMessage == WM_MBUTTONUP)
		{
			if ( WM_LBUTTONUP == nMessage )
				nButton		=	VK_LBUTTON;
			if ( WM_RBUTTONUP == nMessage )
				nButton		=	VK_RBUTTON;
			if ( WM_MBUTTONUP == nMessage )
				nButton		=	VK_MBUTTON;

			ReleaseCapture();
			MouseUp ( nButton, nShift, AspectXAdd(dX), AspectYAdd(dY) );
		}
		else if (nMessage == WM_LBUTTONDBLCLK || nMessage == WM_RBUTTONDBLCLK || nMessage == WM_MBUTTONDBLCLK)
		{
			MouseDblClick(nButton, nShift, AspectXAdd(dX), AspectYAdd(dY));
		}
		else if (nMessage == WM_MOUSEMOVE)
		{
			MouseMove(nButton, nShift, AspectXAdd(dX), AspectYAdd(dY));
		}

		return TRUE;
	}

	virtual void ReceiveMessage(Messages::CMessage* pMessage)
	{
		if ( NULL == pMessage )
			return;

		if ( m_oTrackManager.IsEnabled () )
		{
			// сообщение трека об изменени€х передаем TrackID
			Tracking::CTrackObject*	pTracking	=	m_oTrackManager.GetTrack ( pMessage->GetIntegerValues()[1] );

			if ( NULL == pTracking )
			{
				if ( -1 == pMessage->GetIntegerValues()[1] )
				{
					pTracking	=	m_oTrackManager.GetTrack ( 0 );
				}
			}
			
			if ( NULL != pTracking )
			{
				if ( pTracking->IsTracksTypeTrackEffectKenBurns () )
				{
					/*
					
					LONG Type = -1;
					
					if ( pMessage->GetType() == Tracking::Constants::sc_nMessageTrackPointBegin )
						Type = 0;
					else if ( pMessage->GetType() == Tracking::Constants::sc_nMessageTrackPointProcess )
						Type = 1;
					else if ( pMessage->GetType() == Tracking::Constants::sc_nMessageTrackPointProcessAll )
						Type = 1;
					else if ( pMessage->GetType() == Tracking::Constants::sc_nMessageTrackPointEnd )
						Type = 2;
					else if ( pMessage->GetType() == Tracking::Constants::sc_nMessageTrackTextEnd )
						Type = 4;
					else if ( pMessage->GetType() == Tracking::Constants::sc_nMessageTrackTextBegin )
						Type = 5;
					else if ( pMessage->GetType() == Tracking::Constants::sc_nMessageTrackTextChange )
						Type = 6;
					
					*/
					
					TrackChange ( pMessage->GetIntegerValues()[0], pMessage->GetType() );
				}
				else
				{
					TrackChange ( pMessage->GetIntegerValues()[1], pMessage->GetType() );
				}

				/*
				if ( pTracking->IsTracksTypeTrackEffectKenBurns () )
				{
					LONG Type = -1;

					if ( pMessage->GetType() == Tracking::Constants::sc_nMessageTrackPointBegin )
						Type = 0;
					else if ( pMessage->GetType() == Tracking::Constants::sc_nMessageTrackPointProcess )
						Type = 1;
					else if ( pMessage->GetType() == Tracking::Constants::sc_nMessageTrackPointProcessAll )
						Type = 1;
					else if ( pMessage->GetType() == Tracking::Constants::sc_nMessageTrackPointEnd )
						Type = 2;
					else if ( pMessage->GetType() == Tracking::Constants::sc_nMessageTrackTextEnd )
						Type = 4;
					else if ( pMessage->GetType() == Tracking::Constants::sc_nMessageTrackTextBegin )
						Type = 5;
					else if ( pMessage->GetType() == Tracking::Constants::sc_nMessageTrackTextChange )
						Type = 6;

					CTrackEffectKenBurns* pEKBTrack = static_cast<CTrackEffectKenBurns*> ( pTracking );
					if ( pEKBTrack )
					{
						Structures::RECTD rectD = pEKBTrack->GetCurTrack ().GetDoubleBoundingRect(0, 3);

						rectD.Normalize();

						TrackRectangle ( pTracking->GetType (), AspectXAdd(rectD.left), AspectYAdd(rectD.top), AspectXAdd(rectD.right), AspectYAdd(rectD.bottom) );
					}
				}
				*/

				if ( pTracking->GetCount() >= 1 )
				{

					if ( pTracking->IsTrackRectangleRotatableFixedAspect () )
					{
						LONG Type = -1;

						if ( pMessage->GetType() == Tracking::Constants::sc_nMessageTrackPointBegin )
							Type = 0;
						else if ( pMessage->GetType() == Tracking::Constants::sc_nMessageTrackPointProcess )
							Type = 1;
						else if ( pMessage->GetType() == Tracking::Constants::sc_nMessageTrackPointProcessAll )
							Type = 1;
						else if ( pMessage->GetType() == Tracking::Constants::sc_nMessageTrackPointEnd )
							Type = 2;
						else if ( pMessage->GetType() == Tracking::Constants::sc_nMessageTrackTextEnd )
							Type = 4;
						else if ( pMessage->GetType() == Tracking::Constants::sc_nMessageTrackTextBegin )
							Type = 5;
						else if ( pMessage->GetType() == Tracking::Constants::sc_nMessageTrackTextChange )
							Type = 6;

						double Angle	=	Geometry::GetAngle ( pTracking->GetAt(5).dX, pTracking->GetAt(5).dY, pTracking->GetAt(4).dX, pTracking->GetAt(4).dY) + M_PI_2;

						Structures::RECTD RectangleRC = pTracking->GetUnrotatableRect ( Angle, 0, 2, 5 );	

						Angle			=	grad ( ( ( Angle <= 0 ) ? Angle + 2.0 * M_PI: Angle ) );

						TrackRectangleRotatableFixedAspect (
							Type, 
							AspectXAdd ( RectangleRC.left ),
							AspectYAdd ( RectangleRC.top ), 
							AspectXAdd ( RectangleRC.right ),
							AspectYAdd ( RectangleRC.bottom ),
							Angle );
					}
				}
			}
		}
		else
		{
			if ( NULL == m_pTracking )
				return;

			if ( m_pTracking->GetCount() < 1 )
				return;

			if ( pMessage->GetType() == Tracking::Constants::sc_nMessageTrackPointBegin )
				FireEvent(0);
			else if ( pMessage->GetType() == Tracking::Constants::sc_nMessageTrackPointProcess )
				FireEvent(1);
			else if ( pMessage->GetType() == Tracking::Constants::sc_nMessageTrackPointProcessAll )
				FireEvent(1);
			else if ( pMessage->GetType() == Tracking::Constants::sc_nMessageTrackPointEnd )
				FireEvent(2);
			else if ( pMessage->GetType() == Tracking::Constants::sc_nMessageTrackTextEnd )
				FireEvent(4);
			else if ( pMessage->GetType() == Tracking::Constants::sc_nMessageTrackTextBegin )
				FireEvent(5);
			else if ( pMessage->GetType() == Tracking::Constants::sc_nMessageTrackTextChange )
				FireEvent(6);
		}

		OnPaint ( c_SenderTrack );
	}

public:

	BOOL IPP_ExtractRect( int nWidth, int nHeight, int nChannels, BYTE* pPixels )
	{
		return FALSE;

		// check for valid preview image
		if (!pPixels || !nWidth || !nHeight || nChannels != 4)
			return FALSE;

		// создаЄБEбуфеБEдл€ БEьKежутъHыZБEъ@ераций
		BYTE* pBuffer = NULL;
		BYTE* pImage  = m_pPixels;
		DWORD nImageSize = m_nWidth*m_nHeight*4;

		// ъCрисовываем БEскБE
		if( m_pMask && m_nMaskStyle != c_MaskStyleHidden )
		{
			pBuffer = new Ipp8u[nImageSize];
			if( pBuffer )
			{
				COLORREF nSelection;   OleTranslateColor(m_nMaskColor,  NULL, &nSelection);
				COLORREF nBackground1; OleTranslateColor(m_nBackColor1, NULL, &nBackground1);
				COLORREF nBackground2; OleTranslateColor(m_nBackColor2, NULL, &nBackground2);

				int nSelectionRed	= GetRValue(nSelection);
				int nSelectionGreen = GetGValue(nSelection);
				int nSelectionBlue  = GetBValue(nSelection);

				memcpy( pBuffer, m_pPixels, nImageSize );

				BYTE* pMaskPixel   = m_pMask;
				BYTE* pBufferPixel = pBuffer;

				if( c_MaskStyleSolid == m_nMaskStyle )
				{
					for( int nCountPixels = nWidth*nHeight; nCountPixels > 0; --nCountPixels, pMaskPixel += 1, pBufferPixel += 4 )
					{
						if( *pMaskPixel )
						{
							pBufferPixel[0] = BYTE((pBufferPixel[0] + nSelectionBlue)  >> 1);
							pBufferPixel[1] = BYTE((pBufferPixel[1] + nSelectionGreen) >> 1);
							pBufferPixel[2] = BYTE((pBufferPixel[2] + nSelectionRed)   >> 1);
						}
					}
				}
				else if( m_nMaskStyle > c_MaskStyleBorder && m_nMaskStyle <= c_MaskStyleThatches )
				{
					const BYTE* pPattern = &c_Patterns[m_nMaskStyle - c_MaskStyleBorder][0][0];

					for( int nRow = 0; nRow < m_nHeight; ++nRow )
					{
						for( int nPos = 0; nPos < m_nWidth; ++nPos, ++m_pMask )
						{
							if( pPattern[(nRow & 7) << 3 + (nPos & 7)] )
							{
								pBufferPixel[0] = BYTE((pBufferPixel[0] + nSelectionBlue)  >> 1);
								pBufferPixel[1] = BYTE((pBufferPixel[1] + nSelectionGreen) >> 1);
								pBufferPixel[2] = BYTE((pBufferPixel[2] + nSelectionRed)   >> 1);
							}
						}
					}
				}

				pImage = pBuffer;
			}
		}

		int nDstStride = ImageStudioUtils::Align32(nWidth * nChannels);

		// retrieve selection color components
		COLORREF nBackground1; OleTranslateColor(m_nBackColor1, NULL, &nBackground1);

		int nBackgroundRed1	  = GetRValue(nBackground1);
		int nBackgroundGreen1 = GetGValue(nBackground1);
		int nBackgroundBlue1  = GetBValue(nBackground1);

		// ъCрисовываем фоБE
		IppiSize DstSize = {nWidth, nHeight};
		Ipp8u BackColor[4] = {
			GetBValue(nBackground1),
			GetGValue(nBackground1),
			GetRValue(nBackground1),
			255
		};

		ippiSet_8u_C4R( BackColor, pPixels, nDstStride, DstSize);

		if( m_nBackStyle > c_BackStyleSolid && m_nBackStyle < c_BackStyleThatches )
		{
			COLORREF nBackground2; OleTranslateColor(m_nBackColor2, NULL, &nBackground2);

			nBackground2 |= 0xff000000;

			DWORD* pCurrentPixel = (DWORD*)pPixels;
			const  BYTE* pPattern = &c_Patterns[m_nBackStyle - c_BackStyleSolid][0][0];

			for( int nRow = 0; nRow < nHeight; ++nRow )
			{
				for( int nPos = 0; nPos < nWidth; ++nPos, pCurrentPixel += 1 )
				{
					if( pPattern[(nRow & 7) << 3 + (nPos & 7)] )
					{
						pCurrentPixel[0] = nBackground2;
					}
				}
			}
		}

		// БEБEруем ъCь@ражаемБE БEртинБE
		IppiSize SrcSize = {m_nWidth, m_nHeight};
		IppiRect SrcRect = {0, 0, m_nWidth, m_nHeight};
		IppiRect OutRect = {0, 0, nWidth, nHeight};

		double dX = (nWidth  - m_nWidth  * m_dZoom)*0.5;
		double dY = (nHeight - m_nHeight * m_dZoom)*0.5;
		dX += dX > 0 ? 0.5 : -0.5;
		dY += dY > 0 ? 0.5 : -0.5;
		dX = int(dX);
		dY = int(dY);

		if( dX > 0 )
		{
			OutRect.x = int(dX);
			OutRect.width -= 2 * OutRect.x;
		}
		if( dY > 0 )
		{
			OutRect.y = int(dY);
			OutRect.height -= 2 * OutRect.y;
		}

		dX += m_dCenterX * m_dZoom - nWidth * 0.5;
		dY += m_dCenterY * m_dZoom - nHeight * 0.5;

		int nLeft   = OutRect.x - int(dX);
		int nRight  = nLeft + OutRect.width;
		int nTop    = OutRect.y - int(dY);
		int nBottom = nTop + OutRect.height;

		OutRect.x -= int(dX);
		OutRect.y += int(dY);

		if( OutRect.x < 0 )
		{
			pImage -= int(OutRect.x / m_dZoom) * 4;
			SrcSize.width += int(OutRect.x / m_dZoom);

			OutRect.x = 0;
		}
		else if( OutRect.x > nWidth - OutRect.width )
		{
			OutRect.width = nWidth - OutRect.x;
		}

		if( OutRect.y < 0 )
		{
			pImage -= int(OutRect.y / m_dZoom) * m_nWidth * 4;
			SrcSize.height += int(OutRect.y / m_dZoom);

			OutRect.y = 0;
		}
		else if( OutRect.y > nHeight - OutRect.height )
		{
			OutRect.height = nHeight - OutRect.y;
		}

		BYTE* pDstPixels = pPixels + OutRect.y * nDstStride + OutRect.x * 4;
		DstSize.width    = OutRect.width;
		DstSize.height   = OutRect.height;

		ippiResize_8u_C4R( pImage, SrcSize, m_nWidth*4, SrcRect, pDstPixels, nDstStride, DstSize, m_dZoom, m_dZoom, IPPI_INTER_SUPER );

		// show border (if present)
		if(m_bShowBorder)
		{
			// show vertical sides
			for( int nY = nTop; nY <= nBottom; ++nY )
			{
				// check for valid y coordinate
				if (nY < 0 || nY >= nHeight)
					continue;

				if (nLeft >= 0 && nLeft < nWidth)
				{
					int nIndex = nChannels*nLeft + (nHeight - 1 - nY)*nDstStride;

					pPixels[nIndex + 0] = 0;
					pPixels[nIndex + 1] = 0;
					pPixels[nIndex + 2] = 0;
				}
				if (nRight >= 0 && nRight < nWidth)
				{
					int nIndex = nChannels*nRight + (nHeight - 1 - nY)*nDstStride;

					pPixels[nIndex + 0] = 0;
					pPixels[nIndex + 1] = 0;
					pPixels[nIndex + 2] = 0;
				}
			}

			// show horizontal sides
			for( int nX = nLeft; nX <= nRight; ++nX )
			{
				// check for valid x coordinate
				if (nX < 0 || nX >= nWidth)
					continue;

				if (nTop >= 0 && nTop < nHeight)
				{
					int nIndex = nChannels*nX + (nHeight - 1 - nTop)*nDstStride;

					pPixels[nIndex + 0] = 0;
					pPixels[nIndex + 1] = 0;
					pPixels[nIndex + 2] = 0;
				}
				if (nBottom >= 0 && nBottom < nHeight)
				{
					int nIndex = nChannels*nX + (nHeight - 1 - nBottom)*nDstStride;

					pPixels[nIndex + 0] = 0;
					pPixels[nIndex + 1] = 0;
					pPixels[nIndex + 2] = 0;
				}
			}
		}

		if( pBuffer )
			delete [] pBuffer;

		return TRUE;
	}

	BOOL ExtractRect(int nWidth, int nHeight, int nChannels, BYTE* pPixels)
	{
		// check for valid preview image
		if (!pPixels || !nWidth || !nHeight)
			return FALSE;

		// variables
		int nSourceX, nSourceY;
		int nLineSize = ImageStudioUtils::Align32(nWidth * nChannels);
		int nX, nY, nOffset, nIndex, nNewIndex;
		int nPatternX, nPatternY;

		// retrieve selection color components
		COLORREF nSelection; OleTranslateColor(m_nMaskColor, NULL, &nSelection);

		int nSelectionRed	= GetRValue(nSelection);
		int nSelectionGreen = GetGValue(nSelection);
		int nSelectionBlue  = GetBValue(nSelection);

		// retrieve background color components
		COLORREF nBackground1; OleTranslateColor(m_nBackColor1, NULL, &nBackground1);
		COLORREF nBackground2; OleTranslateColor(m_nBackColor2, NULL, &nBackground2);

		int nBackgroundRed1	 = GetRValue(nBackground1);
		int nBackgroundGreen1 = GetGValue(nBackground1);
		int nBackgroundBlue1  = GetBValue(nBackground1);

		int nBackgroundRed2	 = GetRValue(nBackground2);
		int nBackgroundGreen2 = GetGValue(nBackground2);
		int nBackgroundBlue2  = GetBValue(nBackground2);

		// check for simpliest case
		if (m_nBackStyle == c_BackStyleNone) ZeroMemory(pPixels, nLineSize*nHeight);

		// preview rectangle variables
		double dPreviewLeft, dPreviewTop, dPreviewRight, dPreviewBottom;

		// compute preview rectangle coordinates
		ComputeRectangle(dPreviewLeft, dPreviewTop, dPreviewRight, dPreviewBottom);

		// compute preview rectangle extensions
		double dPreviewWidth  = dPreviewRight - dPreviewLeft;
		double dPreviewHeight = dPreviewBottom - dPreviewTop;

		// compute preview rectangle steps
		double dSourceX, dSourceY;
		double dStepX = dPreviewWidth/(nWidth - 1);
		double dStepY = dPreviewHeight/(nHeight - 1);

		// extract sub-image pixels
		for (nY = 0, nIndex = 0, dSourceY = dPreviewBottom; nY < nHeight; ++nY, dSourceY -= dStepY, nIndex += nLineSize)
		{
			// compute integer y coordinate
			nSourceY = m_nHeight - 1 - (int)floor(dSourceY);

			// compute pettern y coordinate
			nPatternY = nY % 8;

			// row y coordinate is valid - copy row information
			for (nX = 0, nNewIndex = nIndex, dSourceX = dPreviewLeft; nX < nWidth; ++nX, nNewIndex += nChannels, dSourceX += dStepX)
			{
				// compute integer x coordinate
				nSourceX = (int)floor(dSourceX);

				// compute pettern x coordinate
				nPatternX = nX % 8;

				// check for valid pixel coordinate
				if (m_pPixels && nSourceY >= 0 && nSourceY < m_nHeight && nSourceX >= 0 && nSourceX < m_nWidth)
				{
					// compute offset
					nOffset = 4*(nSourceX + nSourceY*m_nWidth);

					// column x coordinate is valid - copy pixel information
					pPixels[nNewIndex + 0] = m_pPixels[nOffset + 0];
					pPixels[nNewIndex + 1] = m_pPixels[nOffset + 1];
					pPixels[nNewIndex + 2] = m_pPixels[nOffset + 2];

					// check for pixel mask 
					if (m_pMask && m_pMask[nOffset/4] && m_nMaskStyle != c_MaskStyleHidden)
					{
						// check for solid selection style
						if (m_nMaskStyle == c_MaskStyleSolid)
						{
							// update pixel color
							pPixels[nNewIndex + 0] = (BYTE)(0.5*(nSelectionBlue + m_pPixels[nOffset + 0]));
							pPixels[nNewIndex + 1] = (BYTE)(0.5*(nSelectionGreen + m_pPixels[nOffset + 1]));
							pPixels[nNewIndex + 2] = (BYTE)(0.5*(nSelectionRed + m_pPixels[nOffset + 2]));
						}
						else
						{
							// compute nearest pixels coordinates
							int nX1 = (int)( dSourceX - dStepX );
							int nX2 = nSourceX;
							int nX3 = (int)( dSourceX + dStepX );
							int nY1 = m_nHeight - 1 - (int)( dSourceY - dStepY );
							int nY2 = nSourceY;
							int nY3 = m_nHeight - 1 - (int)( dSourceY + dStepY );

							// correct pixels
							if (nX1 < 0)			nX1 = nX2;
							if (nX1 >= m_nWidth)	nX1 = nX2;
							if (nX3 < 0)			nX3 = nX2;
							if (nX3 >= m_nWidth)	nX3 = nX2;

							if (nY1 < 0)			nY1 = nY2;
							if (nY1 >= m_nHeight)	nY1 = nY2;
							if (nY3 < 0)			nY3 = nY2;
							if (nY3 >= m_nHeight)	nY3 = nY2;

							// check whether pixel is on mask border
							if (!m_pMask[nX1 + nY1*m_nWidth] || !m_pMask[nX2 + nY1*m_nWidth] || !m_pMask[nX3 + nY1*m_nWidth] ||
								!m_pMask[nX1 + nY2*m_nWidth] || !m_pMask[nX2 + nY2*m_nWidth] || !m_pMask[nX3 + nY2*m_nWidth] ||
								!m_pMask[nX1 + nY3*m_nWidth] || !m_pMask[nX2 + nY3*m_nWidth] || !m_pMask[nX3 + nY3*m_nWidth])
							{
								// update pixel color
								pPixels[nNewIndex + 0] = (BYTE)(0.5*(nSelectionBlue + m_pPixels[nOffset + 0]));
								pPixels[nNewIndex + 1] = (BYTE)(0.5*(nSelectionGreen + m_pPixels[nOffset + 1]));
								pPixels[nNewIndex + 2] = (BYTE)(0.5*(nSelectionRed + m_pPixels[nOffset + 2]));

								// proceed to next pixel
								continue;
							}

							// apply other pixel masking
							if (m_nMaskStyle > c_MaskStyleBorder && m_nMaskStyle < c_MaskStyleThatches)
							{
								if (c_Patterns[m_nMaskStyle - c_MaskStyleBorder][nPatternY][nPatternX])
								{
									pPixels[nNewIndex + 0] = (BYTE)(0.5*(nSelectionBlue + m_pPixels[nOffset + 0]));
									pPixels[nNewIndex + 1] = (BYTE)(0.5*(nSelectionGreen + m_pPixels[nOffset + 1]));
									pPixels[nNewIndex + 2] = (BYTE)(0.5*(nSelectionRed + m_pPixels[nOffset + 2]));
								}
							}
						}
					}

					// proceed to next pixel
					continue;
				}

				// fill pixel with background color
				if (m_nBackStyle == c_BackStyleSolid)
				{
					pPixels[nNewIndex + 0] = nBackgroundBlue1;
					pPixels[nNewIndex + 1] = nBackgroundGreen1;
					pPixels[nNewIndex + 2] = nBackgroundRed1;
				}
				else if (m_nBackStyle > c_BackStyleSolid && m_nBackStyle < c_BackStyleThatches)
				{
					if (c_Patterns[m_nBackStyle][nPatternY][nPatternX])
					{
						pPixels[nNewIndex + 0] = nBackgroundBlue2;
						pPixels[nNewIndex + 1] = nBackgroundGreen2;
						pPixels[nNewIndex + 2] = nBackgroundRed2;
					}
					else
					{
						pPixels[nNewIndex + 0] = nBackgroundBlue1;
						pPixels[nNewIndex + 1] = nBackgroundGreen1;
						pPixels[nNewIndex + 2] = nBackgroundRed1;
					}
				}
			}
		}

		// show border (if present)
		if(m_bShowBorder)
		{
			// show vertical line
			int nLeft	= (int)floor(nWidth*(0 - dPreviewLeft)/dPreviewWidth);
			int nRight	= (int)floor(nWidth*(m_nWidth - dPreviewLeft)/dPreviewWidth);
			int nTop	= (int)floor(nHeight*(0 - dPreviewTop)/dPreviewHeight);
			int nBottom	= (int)floor(nHeight*(m_nHeight - dPreviewTop)/dPreviewHeight);

			// show vertical sides
			for (nY = nTop; nY <= nBottom; ++nY)
			{
				// check for valid y coordinate
				if (nY < 0 || nY >= nHeight)
					continue;

				if (nLeft >= 0 && nLeft < nWidth)
				{
					nIndex = nChannels*nLeft + (nHeight - 1 - nY)*nLineSize;

					pPixels[nIndex + 0] = 0;
					pPixels[nIndex + 1] = 0;
					pPixels[nIndex + 2] = 0;
				}
				if (nRight >= 0 && nRight < nWidth)
				{
					nIndex = nChannels*nRight + (nHeight - 1 - nY)*nLineSize;

					pPixels[nIndex + 0] = 0;
					pPixels[nIndex + 1] = 0;
					pPixels[nIndex + 2] = 0;
				}
			}

			// show horizontal sides
			for (nX = nLeft; nX <= nRight; ++nX)
			{
				// check for valid x coordinate
				if (nX < 0 || nX >= nWidth)
					continue;

				if (nTop >= 0 && nTop < nHeight)
				{
					nIndex = nChannels*nX + (nHeight - 1 - nTop)*nLineSize;

					pPixels[nIndex + 0] = 0;
					pPixels[nIndex + 1] = 0;
					pPixels[nIndex + 2] = 0;
				}
				if (nBottom >= 0 && nBottom < nHeight)
				{
					nIndex = nChannels*nX + (nHeight - 1 - nBottom)*nLineSize;

					pPixels[nIndex + 0] = 0;
					pPixels[nIndex + 1] = 0;
					pPixels[nIndex + 2] = 0;
				}
			}
		}

		// all ok
		return TRUE;
	}


	BOOL UpdateBackGroundPixels ( int nWidth, int nHeight, int nChannels, BYTE* pPixels )
	{
		//CTemporaryCS Blocking ( &m_cs );

		// check for valid preview image
		if (!pPixels || !nWidth || !nHeight || m_pPixels == NULL )
			return FALSE;

		// variables
		int nLineSize = ImageStudioUtils::Align32(nWidth * nChannels);
		//int nSourceX, nSourceY;
		//int nX, nY, nOffset, nIndex, nNewIndex;
		//int nPatternX, nPatternY;

		//// retrieve selection color components
		//COLORREF nSelection; OleTranslateColor(m_nMaskColor, NULL, &nSelection);

		//int nSelectionRed		= GetRValue(nSelection);
		//int nSelectionGreen	= GetGValue(nSelection);
		//int nSelectionBlue	= GetBValue(nSelection);

		// retrieve background color components
		COLORREF nBackground1; OleTranslateColor(m_nBackColor1, NULL, &nBackground1);
		COLORREF nBackground2; OleTranslateColor(m_nBackColor2, NULL, &nBackground2);

		BYTE nBackgroundRed1	=	GetRValue(nBackground1);
		BYTE nBackgroundGreen1	=	GetGValue(nBackground1);
		BYTE nBackgroundBlue1	=	GetBValue(nBackground1);

		BYTE nBackgroundRed2	=	GetRValue(nBackground2);
		BYTE nBackgroundGreen2	=	GetGValue(nBackground2);
		BYTE nBackgroundBlue2	=	GetBValue(nBackground2);
	
		// check for simpliest case
		if ( m_nBackStyle == c_BackStyleNone )
		{
			ZeroMemory ( pPixels, nLineSize * nHeight );
		}
		else
		{
			DWORD Color = 0xFF000000|(nBackgroundRed1<<16)|(nBackgroundGreen1<<8)|(nBackgroundBlue1);
			Core::PixelsOperations::MemsetDW ( (DWORD*) pPixels, nWidth * nHeight, Color );
		}

		/*
		#ifdef _DEBUG
		unsigned __int64 TicksCount			= __rdtsc();
		#endif

		#ifdef _DEBUG
		TicksCount		= __rdtsc();
		//ATLTRACE("Core::Memset : %f percent\n", (double) TicksCountFunc02 / (double)TicksCountFunc01 * 100.0 );
		#endif
		*/
		

		int Left						=	nWidth * 0.5	- m_nWidth * m_dZoom * 0.5;
		int Bottom						=	nHeight * 0.5	- m_nHeight * m_dZoom * 0.5;
		int Right						=	nWidth * 0.5	+ m_nWidth * m_dZoom * 0.5;
		int Top							=	nHeight * 0.5	+ m_nHeight * m_dZoom * 0.5;

		double WriteWidth				=	Right - Left;
		double WriteHeight				=	Top - Bottom;

		double InvertZoom				=	1.0 / m_dZoom;

		double OffsetX					=	nWidth * 0.5 - ( m_dCenterX ) * m_dZoom;
		double OffsetY					=	nHeight * 0.5 - ( m_nHeight - m_dCenterY ) * m_dZoom;

		long StartDX					=	OffsetX;
		if ( StartDX < 0 )				
			StartDX = 0;

		long StartDY					=	OffsetY;
		if ( StartDY < 0 )				
			StartDY = 0;

		long EndDX						=	OffsetX + WriteWidth;
		if ( EndDX > nWidth )
			EndDX = nWidth;

		long EndDY						=	OffsetY + WriteHeight;
		if ( EndDY > nHeight )
			EndDY = nHeight;

		if ( Core::PixelsOperations::sc_nInterpolationModeNearestNeighbor == m_nInterpolationMode )
		{
			unsigned long* pSourceRGBA	=	(DWORD*) m_pPixels;
			unsigned long* pPixelsRGBA	=	(DWORD*) pPixels;
//#ifdef _DEBUG
//			unsigned __int64 TicksCountFunc01	= 0;
//			unsigned __int64 TicksCountFunc02	= 0;
//			unsigned __int64 TicksCount			= __rdtsc();
//#endif
			//if ( false == m_oCheckFeatures.FeatureSSE2 () )
			{
				Core::PixelsOperations::NearestResizeImageDW ( pSourceRGBA, pPixelsRGBA, 
					m_nWidth, m_nHeight, nWidth, nHeight,
					0.0, 0.0,							// Source offset
					-(long)OffsetX, -(long)OffsetY,		// Destination offset
					InvertZoom, InvertZoom,				// Source step
					StartDX, EndDX, StartDY, EndDY );	// Destination write start~end pixels
			}
//#ifdef _DEBUG
//			TicksCountFunc01	=	__rdtsc() - TicksCount;
//			TicksCount			=	__rdtsc();
//#endif
			//if ( m_oCheckFeatures.FeatureSSE2 () )	//  + 15%
			//if ( 0 )
			//{

			//	Core::PixelsOperations::NearestResizeImageDW_SSE2 ( pSourceRGBA, pPixelsRGBA, 
			//		m_nWidth, m_nHeight, nWidth, nHeight,
			//		0.0, 0.0,							// Source offset
			//		-(long)OffsetX, -(long)OffsetY,		// Destination offset
			//		InvertZoom, InvertZoom,				// Source step
			//		StartDX, EndDX, StartDY, EndDY );	// Destination write start~end pixels
			//}
//#ifdef _DEBUG
//			TicksCountFunc02	=	__rdtsc() - TicksCount;
//			TicksCount			=	__rdtsc();
//
//			//ATLTRACE("NearestResizeImage : %f percent\n", (double) TicksCountFunc02 / (double)TicksCountFunc01 * 100.0 );
//#endif
		}

		if ( Core::PixelsOperations::sc_nInterpolationModeBilinear == m_nInterpolationMode )
		{		
			//unsigned long* pSourceRGBA	=	(DWORD*) m_pPixels;
			//unsigned long* pPixelsRGBA	=	(DWORD*) pPixels;
			//Core::PixelsOperations::BilinearResizeImageDW ( pSourceRGBA, pPixelsRGBA, 
			Core::PixelsOperations::BilinearResizeImageBT ( m_pPixels, pPixels,
				m_nWidth, m_nHeight, nWidth, nHeight,
				0.0, 0.0,								// Source offset
				-(long)OffsetX, -(long)OffsetY,		// Destination offset
				InvertZoom, InvertZoom,				// Source step
				StartDX, EndDX, StartDY, EndDY );	// Destination write start~end pixels
		}

		// extract sub-image pixels
		//for (nY = 0, nIndex = 0, dSourceY = dPreviewBottom; nY < nHeight; ++nY, dSourceY -= dStepY, nIndex += nLineSize)
		//{
		//	// compute integer y coordinate
		//	nSourceY = m_nHeight - 1 - (int)floor(dSourceY);

		//	if ( nSourceY < 0 || nSourceY >= m_nHeight )
		//		continue;

		//	// compute pettern y coordinate
		//	//nPatternY = nY % 8;

		//	// row y coordinate is valid - copy row information
		//	for (nX = 0, nNewIndex = nIndex, dSourceX = dPreviewLeft; nX < nWidth; ++nX, nNewIndex += nChannels, dSourceX += dStepX)
		//	{
		//		// compute integer x coordinate
		//		nSourceX = (int)floor(dSourceX);

		//		// check for valid pixel coordinate
		//		if ( nSourceX < 0 || nSourceX >= m_nWidth)
		//			continue;

		//		// compute pettern x coordinate
		//		//nPatternX = nX % 8;

		//		//if ( nSourceY >= 0 && nSourceY < m_nHeight && nSourceX >= 0 && nSourceX < m_nWidth)
		//		{
		//			// compute offset
		//			nOffset = 4*(nSourceX + nSourceY*m_nWidth);

		//			// column x coordinate is valid - copy pixel information
		//			//pPixels[nNewIndex + 0] = m_pPixels[nOffset + 0];
		//			//pPixels[nNewIndex + 1] = m_pPixels[nOffset + 1];
		//			//pPixels[nNewIndex + 2] = m_pPixels[nOffset + 2];

		//			memcpy ( pPixels + nNewIndex, m_pPixels + nOffset, sizeof ( BYTE ) * 3 );

		//			// check for pixel mask 
		//			if (m_pMask && m_pMask[nOffset/4] && m_nMaskStyle != c_MaskStyleHidden)
		//			{
		//				// check for solid selection style
		//				if (m_nMaskStyle == c_MaskStyleSolid)
		//				{
		//					// update pixel color
		//					pPixels[nNewIndex + 0] = (BYTE)(0.5*(nSelectionBlue + m_pPixels[nOffset + 0]));
		//					pPixels[nNewIndex + 1] = (BYTE)(0.5*(nSelectionGreen + m_pPixels[nOffset + 1]));
		//					pPixels[nNewIndex + 2] = (BYTE)(0.5*(nSelectionRed + m_pPixels[nOffset + 2]));
		//				}
		//				else
		//				{
		//					// compute nearest pixels coordinates
		//					int nX1 = (int)( dSourceX - dStepX );
		//					int nX2 = nSourceX;
		//					int nX3 = (int)( dSourceX + dStepX );
		//					int nY1 = m_nHeight - 1 - (int)( dSourceY - dStepY );
		//					int nY2 = nSourceY;
		//					int nY3 = m_nHeight - 1 - (int)( dSourceY + dStepY );

		//					// correct pixels
		//					if (nX1 < 0)			nX1 = nX2;
		//					if (nX1 >= m_nWidth)	nX1 = nX2;
		//					if (nX3 < 0)			nX3 = nX2;
		//					if (nX3 >= m_nWidth)	nX3 = nX2;

		//					if (nY1 < 0)			nY1 = nY2;
		//					if (nY1 >= m_nHeight)	nY1 = nY2;
		//					if (nY3 < 0)			nY3 = nY2;
		//					if (nY3 >= m_nHeight)	nY3 = nY2;

		//					// check whether pixel is on mask border
		//					if (!m_pMask[nX1 + nY1*m_nWidth] || !m_pMask[nX2 + nY1*m_nWidth] || !m_pMask[nX3 + nY1*m_nWidth] ||
		//						!m_pMask[nX1 + nY2*m_nWidth] || !m_pMask[nX2 + nY2*m_nWidth] || !m_pMask[nX3 + nY2*m_nWidth] ||
		//						!m_pMask[nX1 + nY3*m_nWidth] || !m_pMask[nX2 + nY3*m_nWidth] || !m_pMask[nX3 + nY3*m_nWidth])
		//					{
		//						// update pixel color
		//						pPixels[nNewIndex + 0] = (BYTE)(0.5*(nSelectionBlue + m_pPixels[nOffset + 0]));
		//						pPixels[nNewIndex + 1] = (BYTE)(0.5*(nSelectionGreen + m_pPixels[nOffset + 1]));
		//						pPixels[nNewIndex + 2] = (BYTE)(0.5*(nSelectionRed + m_pPixels[nOffset + 2]));

		//						// proceed to next pixel
		//						continue;
		//					}

		//					// apply other pixel masking
		//					if (m_nMaskStyle > c_MaskStyleBorder && m_nMaskStyle < c_MaskStyleThatches)
		//					{
		//						if (c_Patterns[m_nMaskStyle - c_MaskStyleBorder][nPatternY][nPatternX])
		//						{
		//							pPixels[nNewIndex + 0] = (BYTE)(0.5*(nSelectionBlue + m_pPixels[nOffset + 0]));
		//							pPixels[nNewIndex + 1] = (BYTE)(0.5*(nSelectionGreen + m_pPixels[nOffset + 1]));
		//							pPixels[nNewIndex + 2] = (BYTE)(0.5*(nSelectionRed + m_pPixels[nOffset + 2]));
		//						}
		//					}
		//				}
		//			}

		//			// proceed to next pixel

		//			continue;
		//		}

		//		// fill pixel with background color
		//		if (m_nBackStyle == c_BackStyleSolid)
		//		{
		//			pPixels[nNewIndex + 0] = nBackgroundBlue1;
		//			pPixels[nNewIndex + 1] = nBackgroundGreen1;
		//			pPixels[nNewIndex + 2] = nBackgroundRed1;
		//		}
		//		else if (m_nBackStyle > c_BackStyleSolid && m_nBackStyle < c_BackStyleThatches)
		//		{
		//			if (c_Patterns[m_nBackStyle][nPatternY][nPatternX])
		//			{
		//				pPixels[nNewIndex + 0] = nBackgroundBlue2;
		//				pPixels[nNewIndex + 1] = nBackgroundGreen2;
		//				pPixels[nNewIndex + 2] = nBackgroundRed2;
		//			}
		//			else
		//			{
		//				pPixels[nNewIndex + 0] = nBackgroundBlue1;
		//				pPixels[nNewIndex + 1] = nBackgroundGreen1;
		//				pPixels[nNewIndex + 2] = nBackgroundRed1;
		//			}
		//		}
		//	}
		//}

		// show border (if present)
		//if(m_bShowBorder)
		//{
		//	// show vertical line
		//	int nLeft	= (int)floor(nWidth*(0 - dPreviewLeft)/dPreviewWidth);
		//	int nRight	= (int)floor(nWidth*(m_nWidth - dPreviewLeft)/dPreviewWidth);
		//	int nTop	= (int)floor(nHeight*(0 - dPreviewTop)/dPreviewHeight);
		//	int nBottom	= (int)floor(nHeight*(m_nHeight - dPreviewTop)/dPreviewHeight);

		//	// show vertical sides
		//	for (nY = nTop; nY <= nBottom; ++nY)
		//	{
		//		// check for valid y coordinate
		//		if (nY < 0 || nY >= nHeight)
		//			continue;

		//		if (nLeft >= 0 && nLeft < nWidth)
		//		{
		//			nIndex = nChannels*nLeft + (nHeight - 1 - nY)*nLineSize;

		//			pPixels[nIndex + 0] = 0;
		//			pPixels[nIndex + 1] = 0;
		//			pPixels[nIndex + 2] = 0;
		//		}
		//		if (nRight >= 0 && nRight < nWidth)
		//		{
		//			nIndex = nChannels*nRight + (nHeight - 1 - nY)*nLineSize;

		//			pPixels[nIndex + 0] = 0;
		//			pPixels[nIndex + 1] = 0;
		//			pPixels[nIndex + 2] = 0;
		//		}
		//	}

		//	// show horizontal sides
		//	for (nX = nLeft; nX <= nRight; ++nX)
		//	{
		//		// check for valid x coordinate
		//		if (nX < 0 || nX >= nWidth)
		//			continue;

		//		if (nTop >= 0 && nTop < nHeight)
		//		{
		//			nIndex = nChannels*nX + (nHeight - 1 - nTop)*nLineSize;

		//			pPixels[nIndex + 0] = 0;
		//			pPixels[nIndex + 1] = 0;
		//			pPixels[nIndex + 2] = 0;
		//		}
		//		if (nBottom >= 0 && nBottom < nHeight)
		//		{
		//			nIndex = nChannels*nX + (nHeight - 1 - nBottom)*nLineSize;

		//			pPixels[nIndex + 0] = 0;
		//			pPixels[nIndex + 1] = 0;
		//			pPixels[nIndex + 2] = 0;
		//		}
		//	}
		//}

		// all ok
		return TRUE;
	}

	void ShowWindowBorders(HDC hDC, RECT rect)
	{
		RECT rect1, rect2;

		rect1.left = rect.left;
		rect1.top = rect.top;
		rect1.right = rect.right;
		rect1.bottom = rect.bottom;

		rect2.left = rect.left + 1;
		rect2.top = rect.top + 1;
		rect2.right = rect.right - 1;
		rect2.bottom = rect.bottom - 1;

		// show stylized window borders
		if (m_nWindowStyle == c_WindowStyleClient)
		{
			Rendering::Gdi::DrawRectangle3D(hDC, GetSysColor(COLOR_3DSHADOW), GetSysColor(COLOR_3DLIGHT), rect1);
			Rendering::Gdi::DrawRectangle3D(hDC, GetSysColor(COLOR_3DDKSHADOW), GetSysColor(COLOR_3DHILIGHT), rect2);
		}
		else if (m_nWindowStyle == c_WindowStyleModal)
		{
			Rendering::Gdi::DrawRectangle3D(hDC, GetSysColor(COLOR_3DLIGHT), GetSysColor(COLOR_3DSHADOW), rect1);
			Rendering::Gdi::DrawRectangle3D(hDC, GetSysColor(COLOR_3DHILIGHT), GetSysColor(COLOR_3DDKSHADOW), rect2);
		}
		else if (m_nWindowStyle == c_WindowStyleStatic)
		{
			Rendering::Gdi::DrawRectangle3D(hDC, GetSysColor(COLOR_3DLIGHT), GetSysColor(COLOR_3DSHADOW), rect1);
			Rendering::Gdi::DrawRectangle3D(hDC, GetSysColor(COLOR_3DDKSHADOW), GetSysColor(COLOR_3DHILIGHT), rect2);
		}
	}
	void ShowTracking(HDC hDC, RECT rect)
	{
		//if (!m_pTracking)
		//	return;

		// compute preview rectangle coordinates
		double dPreviewLeft, dPreviewTop, dPreviewRight, dPreviewBottom;
		ComputeRectangle(dPreviewLeft, dPreviewTop, dPreviewRight, dPreviewBottom);

		Structures::RECTI rectWindow(rect);
		Structures::RECTD rectData(dPreviewLeft, dPreviewTop, dPreviewRight, dPreviewBottom);

		if ( NULL != m_pTracking )
		{
			m_oTrackingPainter.Create(hDC);
			m_pTracking->OnPaint(m_oTrackingPainter, rectWindow, rectData);
		}
		else
		{
			m_oTrackManager.OnPaint ( hDC, rectWindow, rectData );
		}
	}

	void ComputeRectangle(double& dLeft, double& dTop, double& dRight, double& dBottom)
	{
		// retrieve cursor position and preview window rectanlge
		RECT rect; GetWindowRect(&rect);

		// compute preview image size
		int nWidth = abs(rect.right - rect.left + 1);
		int nHeight = abs(rect.bottom - rect.top + 1);

		// check for valid simpliest case when image size is invalid
		if (!nWidth || !nHeight)
			return;

		// compute preview rectangle coordinates
		dLeft = m_dCenterX - 0.5 * nWidth / m_dZoom;
		dRight = m_dCenterX + 0.5 * nWidth / m_dZoom;
		dTop = m_dCenterY - 0.5 * nHeight / m_dZoom;
		dBottom = m_dCenterY + 0.5 * nHeight / m_dZoom;
	}
	void GetMouseState(int& nButton, int& nShift, double& dX, double& dY)
	{
		// retrieve cursor position and preview window rectanlge
		RECT rect; GetWindowRect(&rect);
		POINT point; GetCursorPos(&point);

		// correct mouse coordinates
		point.x = min(max(point.x, rect.left), rect.right);
		point.y = min(max(point.y, rect.top), rect.bottom);

		// compute preview image size
		int nWidth = abs(rect.right - rect.left);
		int nHeight = abs(rect.bottom - rect.top);

		// check for valid simpliest case when image size is invalid
		if (nWidth < 2) nWidth = 2;
		if (nHeight < 2) nHeight = 2;

		// compute preview rectangle coordinates
		double dPreviewLeft, dPreviewTop, dPreviewRight, dPreviewBottom;
		ComputeRectangle(dPreviewLeft, dPreviewTop, dPreviewRight, dPreviewBottom);

		// compute click point
		dX = dPreviewLeft + (dPreviewRight - dPreviewLeft)*(point.x - rect.left)/nWidth;
		dY = dPreviewTop + (dPreviewBottom - dPreviewTop)*(point.y - rect.top)/nHeight;

		int nLMButtonState	=	GetKeyState(VK_LBUTTON);
		int nRMButtonState	=	GetKeyState(VK_RBUTTON);
		int nMMButtonState	=	GetKeyState(VK_MBUTTON);

		if ( nLMButtonState != m_nLMButtonState && nRMButtonState == m_nRMButtonState && nMMButtonState == m_nMMButtonState )
		{
			nButton			=	1;
		}
		if ( nLMButtonState == m_nLMButtonState && nRMButtonState != m_nRMButtonState && nMMButtonState == m_nMMButtonState )
		{
			nButton			=	2;
		}
		if ( nLMButtonState == m_nLMButtonState && nRMButtonState == m_nRMButtonState && nMMButtonState != m_nMMButtonState )
		{
			nButton			=	4;
		}

		m_nLMButtonState	=	nLMButtonState;
		m_nRMButtonState	=	nRMButtonState;
		m_nMMButtonState	=	nMMButtonState;

		// ATLTRACE ( _T("LEFT : %d, RIGHT : %d, MIDLE : %d\n"), m_nLMButtonState, m_nRMButtonState, m_nMMButtonState );

		// compute mouse button state
		//nButton		=	GetKeyState(VK_LBUTTON) < 0 ? 1 : GetKeyState(VK_RBUTTON) < 0 ? 2 : GetKeyState(VK_MBUTTON) < 0 ? 4 : 0;

		nShift		=	(GetKeyState(VK_SHIFT) < 0 ? 1 : 0) + (GetKeyState(VK_CONTROL) < 0 ? 2 : 0) + (GetKeyState(VK_MENU) < 0 ? 4 : 0);
	}
	void GetTrackArray(SAFEARRAY** pArray, BOOL bAspectAdd)
	{
		// check for valid safe array
		if (!pArray || !m_pTracking) return;

		// compute SAFEARRAY bounds
		SAFEARRAYBOUND rgsaBound[1];
		rgsaBound[0].lLbound = 0;
		rgsaBound[0].cElements = 2*m_pTracking->GetCount();

		// create array
		*pArray = SafeArrayCreate(VT_R8, 1, rgsaBound);

		// retrieve data pointer
		double* pData = (double*)((*pArray)->pvData);

		// fill the array
		for (int nIndex = 0; nIndex <  m_pTracking->GetCount(); ++nIndex)
		{
			if (bAspectAdd)
			{
				pData[2*nIndex + 0] = AspectXAdd(m_pTracking->GetAt(nIndex).dX);
				pData[2*nIndex + 1] = AspectYAdd(m_pTracking->GetAt(nIndex).dY);
			}
			else
			{
				pData[2*nIndex + 0] = m_pTracking->GetAt(nIndex).dX;
				pData[2*nIndex + 1] = m_pTracking->GetAt(nIndex).dY;
			}
		}
	}

	BOOL SetTrackArray(SAFEARRAY** pArray, BOOL bAspectRemove)
	{
		// check for valid input parameters
		if (!pArray || !(*pArray))
			return FALSE;

		// variables
		VARTYPE type;
		long lBound, uBound;

		// check for valid pixel array size
		if (SafeArrayGetDim(*pArray) != 1)
			return FALSE;

		// check for valid array element type
		if (FAILED(SafeArrayGetVartype(*pArray, &type)) || type != VT_R8 || SafeArrayGetElemsize(*pArray) != 8)
			return FALSE;

		// retrieve image attributes
		if (FAILED(SafeArrayGetLBound(*pArray, 1, &lBound)) || FAILED(SafeArrayGetUBound(*pArray, 1, &uBound)))
			return FALSE;

		// compute image attributes
		int nSize = 1 + uBound - lBound;

		// check that the array contains 2*n doubles
		if (nSize % 2)
			return FALSE;

		m_pTracking->SetSize(nSize/2);
		double* pData = (double*)(*pArray)->pvData; 

		// fill the array
		for (int nIndex = 0; nIndex < m_pTracking->GetCount(); ++nIndex)
		{
			if (bAspectRemove)
			{
				m_pTracking->GetAt(nIndex).dX = AspectXRemove(pData[2*nIndex + 0]);
				m_pTracking->GetAt(nIndex).dY = AspectYRemove(pData[2*nIndex + 1]);
			}
			else
			{
				m_pTracking->GetAt(nIndex).dX = pData[2*nIndex + 0];
				m_pTracking->GetAt(nIndex).dY = pData[2*nIndex + 1];
			}
		}

		return TRUE;
	}

	BOOL SafeArrayToPoints(SAFEARRAY** pArray, BOOL bAspectRemove, Structures::POINTD*& pPoints, int& nCount)
	{
		// check for valid input parameters
		if (!pArray || !(*pArray))
			return FALSE;

		// variables
		VARTYPE type;
		long lBound, uBound;

		// check for valid pixel array size
		if (SafeArrayGetDim(*pArray) != 1)
			return FALSE;

		// check for valid array element type
		if (FAILED(SafeArrayGetVartype(*pArray, &type)) || type != VT_R8 || SafeArrayGetElemsize(*pArray) != 8)
			return FALSE;

		// retrieve image attributes
		if (FAILED(SafeArrayGetLBound(*pArray, 1, &lBound)) || FAILED(SafeArrayGetUBound(*pArray, 1, &uBound)))
			return FALSE;

		// compute image attributes
		int nSize = 1 + uBound - lBound;

		// check that the array contains 2*n doubles
		if ((nSize % 2) || (nSize/2 >= 127) )
			return FALSE;

		nCount = nSize/2;
		pPoints = new Structures::POINTD[nCount];
		double* pData = (double*)(*pArray)->pvData; 

		// fill the array
		for (int index = 0; index < nCount; ++index)
		{
			if (bAspectRemove)
			{
				pPoints[index].x = AspectXRemove(pData[2*index + 0]);
				pPoints[index].y = AspectYRemove(pData[2*index + 1]);
			}
			else
			{
				pPoints[index].x = pData[2*index + 0];
				pPoints[index].y = pData[2*index + 1];
			}
		}

		return TRUE;
	}

	void BeginTrack(Tracking::CTrackObject* pTracking)
	{
		// destroy old tracking object
		if (m_pTracking)
		{
			delete m_pTracking;
			m_pTracking = 0;
		}

		// check for valid new tracking object
		m_pTracking = pTracking;

		// setup message receiver for tracking object
		m_pTracking->SetReceiver(this);

		// update preview
		OnPaint(c_SenderTrack);
	}

	
	double AspectXAdd(double x)
	{
		if ( m_dAspect < 0.001 || 0 == m_nWidth )
			return x;

		return m_nAspectWidth*x/m_nWidth;
	}
	double AspectYAdd(double y)
	{
		if (m_dAspect < 0.001 || 0 == m_nHeight )
			return y;

		return m_nAspectHeight*y/m_nHeight;
	}
	double AspectXRemove(double x)
	{
		if (m_dAspect < 0.001 || 0 == m_nAspectWidth)
			return x;

		return m_nWidth*x/m_nAspectWidth;
	}
	double AspectYRemove(double y)
	{
		if (m_dAspect < 0.001 || 0 == m_nAspectHeight )
			return y;

		return m_nHeight*y/m_nAspectHeight;
	}

	
	void BGRA_Flip(BYTE* pBGRA, int nWidth, int nHeight)
	{
		/* @desc: 
		int BGRA_Flip(BYTE* pBGRA, int nWidth, int nHeight)
		{
		ъBуществБEетБE вертикалБEьD ъCражеыGБEБEртинБE (pBGRA, nWidth, nHeight) 
		}
		*/

		if (NULL == pBGRA)
			return;

		int nLineSize = 4*nWidth;

		BYTE* pBGRALineCopy = new BYTE[nLineSize];
		if (NULL == pBGRALineCopy)
			return;

		BYTE* pBGRALineTop = pBGRA;
		BYTE* pBGRALineBottom = pBGRA + nLineSize*(nHeight - 1);

		for (int nY = 0; nY < nHeight/2; ++nY)
		{
			memcpy(pBGRALineCopy, pBGRALineTop, nLineSize);
			memcpy(pBGRALineTop, pBGRALineBottom, nLineSize);
			memcpy(pBGRALineBottom, pBGRALineCopy, nLineSize);

			pBGRALineTop += nLineSize;
			pBGRALineBottom -= nLineSize;
		}

		delete[] pBGRALineCopy;
	}


	
	inline void DrawTrackingImage ()
	{
		if ( NULL != m_pImageTracking )
		{
			Gdiplus::Bitmap oBitmap ( m_oDoubleBuffer.GetRect().GetWidth(), m_oDoubleBuffer.GetRect ().GetHeight (), 
				4 * m_oDoubleBuffer.GetRect().GetWidth(), PixelFormat32bppARGB, m_oDoubleBuffer.GetBackBitmapPixels () );
			
			Gdiplus::Graphics oGraphics ( &oBitmap );
			if ( Gdiplus::Ok != oGraphics.GetLastStatus () )
				return;

			m_pGrahics = &oGraphics;

			if ( Gdiplus::Ok == m_pGrahics->GetLastStatus () )
			{
				Tracking::CTrackObject*	pTracking = m_pTracking;
				if ( m_oTrackManager.IsEnabled () )
					pTracking = m_oTrackManager.GetTrack ( 0 );		// TODO : БEБE тоБEБE ьCин трекинБE

				if ( pTracking )
				{
					if ( pTracking->IsTrackRectangle () || pTracking->IsTrackRectangleFixedAspect () || pTracking->IsTrackRectangleFixedSize () )
					{
						if ( pTracking->GetCount() < 1 )
							return;

						Structures::RECTI rectBounds = pTracking->GetIntegerBoundingRect(0, 3);

						int ImageWidth		= ( m_pBitmap == NULL ) ? -1 : m_pBitmap->GetWidth ();
						int ImageHeight		= ( m_pBitmap == NULL ) ? -1 : m_pBitmap->GetHeight ();
						int NewImageWidth	= rectBounds.right	- rectBounds.left;
						int NewImageHeight	= rectBounds.bottom	- rectBounds.top;

						if ( NewImageWidth != ImageWidth || NewImageHeight != ImageHeight || m_bImageTrackingRebuild )
						{
							RELEASEOBJECT ( m_pBitmap );

							m_pBitmap = new Gdiplus::Bitmap ( NewImageWidth, NewImageHeight, PixelFormat32bppARGB );
							Gdiplus::Graphics ResizeGraphics ( m_pBitmap );
							if ( Gdiplus::Ok == ResizeGraphics.GetLastStatus () )
							{
								long ImageWidth		=	m_pImageTracking->Width;
								Gdiplus::Bitmap SourceBitmap ( m_pImageTracking->Width, m_pImageTracking->Height, m_pImageTracking->Width * 4, PixelFormat32bppARGB, m_pImageTracking->Buffer );
								if ( Gdiplus::Ok == SourceBitmap.GetLastStatus () )
								{
									ResizeGraphics.DrawImage ( &SourceBitmap, 0, 0, NewImageWidth, NewImageHeight );
									
									if ( m_pImageTracking->ColorSpace & CSP_VFLIP )
									{
										m_pBitmap->RotateFlip ( RotateNoneFlipY );
									}
								}
							}

							m_bImageTrackingRebuild = false;
						}

						if ( NULL != m_pBitmap )
							m_pGrahics->DrawImage ( m_pBitmap, rectBounds.left, rectBounds.top, m_pBitmap->GetWidth (), m_pBitmap->GetHeight() );
					}

					else if ( pTracking->IsTrackRectangleRotatable () )
					{
						//------------------------------------------------------------------------------------------------------------------
						//       8	
						//       |
						//   0---4---1
						//   |       |
						//   7   9   5
						//   |       |
						//   3---6---2
						//------------------------------------------------------------------------------------------------------------------

						if ( pTracking->GetCount() < 1 )
							return;

						double Angle = Geometry::GetAngle ( pTracking->GetAt(9).dX, pTracking->GetAt(9).dY, pTracking->GetAt(8).dX, pTracking->GetAt(8).dY ) + M_PI / 2.0;

						Structures::RECTI rectBounds = pTracking->GetIntegerBoundingRect ( 0, 3 );

						double WidthW	= Geometry::GetLength ( pTracking->GetAt(0).dX, pTracking->GetAt(0).dY, pTracking->GetAt(1).dX, pTracking->GetAt(1).dY );
						double HeightW	= Geometry::GetLength ( pTracking->GetAt(0).dX, pTracking->GetAt(0).dY, pTracking->GetAt(3).dX, pTracking->GetAt(3).dY );

						int ImageWidth		= ( m_pBitmap == NULL ) ? -1 : m_pBitmap->GetWidth ();
						int ImageHeight		= ( m_pBitmap == NULL ) ? -1 : m_pBitmap->GetHeight ();
						int NewImageWidth	= WidthW;
						int NewImageHeight	= HeightW;

						if ( NewImageWidth != ImageWidth || NewImageHeight != ImageHeight || m_bImageTrackingRebuild )
						{
							RELEASEOBJECT ( m_pBitmap );

							m_pBitmap = new Gdiplus::Bitmap ( NewImageWidth, NewImageHeight, PixelFormat32bppARGB );
							Gdiplus::Graphics ResizeGraphics ( m_pBitmap );
							if ( Gdiplus::Ok == ResizeGraphics.GetLastStatus () )
							{
								long ImageWidth		=	m_pImageTracking->Width;
								Gdiplus::Bitmap SourceBitmap ( m_pImageTracking->Width, m_pImageTracking->Height, m_pImageTracking->Width * 4, PixelFormat32bppARGB, m_pImageTracking->Buffer );
								if ( Gdiplus::Ok == SourceBitmap.GetLastStatus () )
								{
									ResizeGraphics.DrawImage ( &SourceBitmap, 0, 0, NewImageWidth, NewImageHeight );
									
									if ( m_pImageTracking->ColorSpace & CSP_VFLIP )
									{
										m_pBitmap->RotateFlip ( RotateNoneFlipY );
									}
								}
							}

							m_bImageTrackingRebuild = false;
						}

						if ( NULL != m_pBitmap )
						{
							double DX = pTracking->GetAt ( 9 ).dX;
							double DY = pTracking->GetAt ( 9 ).dY;

							double DX00 = pTracking->GetAt ( 0 ).dX;
							double DY00 = pTracking->GetAt ( 0 ).dY;
							double DX03 = pTracking->GetAt ( 3 ).dX;
							double DY03 = pTracking->GetAt ( 3 ).dY;
							double DX01 = pTracking->GetAt ( 1 ).dX;
							double DY01 = pTracking->GetAt ( 1 ).dY;

							TransformCoordToControl ( &DX01, &DY01 );
							TransformCoordToControl ( &DX03, &DY03 );
							TransformCoordToControl ( &DX00, &DY00 );

							double WidthW2	= Geometry::GetLength ( DX00, DY00, DX01, DY01 );
							double HeightW2	= Geometry::GetLength ( DX00, DY00, DX03, DY03 );

							TransformCoordToControl ( &DX, &DY );
							Gdiplus::Matrix oMatrix;
							//oMatrix.Translate ( NewImageWidth * 0.5f, NewImageHeight * 0.5f );
							oMatrix.Translate ( DX , DY  );
							oMatrix.Rotate ( Angle * 180 / M_PI );
							//oMatrix.Translate ( WidthW, -HeightW );

							m_pGrahics->SetTransform ( &oMatrix );

							m_pGrahics->DrawImage ( m_pBitmap, 
								(REAL)-WidthW2 * 0.5f,
								(REAL)-HeightW2 * 0.5f,
								(REAL)WidthW2,
								(REAL)HeightW2 );

							m_pGrahics->SetTransform ( &Gdiplus::Matrix() );
						}
					}
					else if ( pTracking->IsTrackRectangleRotatableFixedAspect () )
					{
						//------------------------------------------------------------------------------------------------------------------
						//       4	
						//       |
						//   0-------1
						//   |       |
						//   |   5   |
						//   |       |
						//   3-------2
						//------------------------------------------------------------------------------------------------------------------

						if ( pTracking->GetCount() < 1 )
							return;

						double Angle = Geometry::GetAngle ( pTracking->GetAt(5).dX, pTracking->GetAt(5).dY, pTracking->GetAt(4).dX, pTracking->GetAt(4).dY ) + M_PI / 2.0;

						Structures::RECTI rectBounds = pTracking->GetIntegerBoundingRect ( 0, 3 );

						double WidthW	= Geometry::GetLength ( pTracking->GetAt(0).dX, pTracking->GetAt(0).dY, pTracking->GetAt(1).dX, pTracking->GetAt(1).dY );
						double HeightW	= Geometry::GetLength ( pTracking->GetAt(0).dX, pTracking->GetAt(0).dY, pTracking->GetAt(3).dX, pTracking->GetAt(3).dY );

						int ImageWidth		= ( m_pBitmap == NULL ) ? -1 : m_pBitmap->GetWidth ();
						int ImageHeight		= ( m_pBitmap == NULL ) ? -1 : m_pBitmap->GetHeight ();
						int NewImageWidth	= WidthW;
						int NewImageHeight	= HeightW;

						if ( NewImageWidth != ImageWidth || NewImageHeight != ImageHeight || m_bImageTrackingRebuild )
						{
							RELEASEOBJECT ( m_pBitmap );

							m_pBitmap = new Gdiplus::Bitmap ( NewImageWidth, NewImageHeight, PixelFormat32bppARGB );
							Gdiplus::Graphics ResizeGraphics ( m_pBitmap );
							if ( Gdiplus::Ok == ResizeGraphics.GetLastStatus () )
							{
								long ImageWidth		=	m_pImageTracking->Width;
								Gdiplus::Bitmap SourceBitmap ( m_pImageTracking->Width, m_pImageTracking->Height, m_pImageTracking->Width * 4, PixelFormat32bppARGB, m_pImageTracking->Buffer );
								if ( Gdiplus::Ok == SourceBitmap.GetLastStatus () )
								{
									ResizeGraphics.DrawImage ( &SourceBitmap, 0, 0, NewImageWidth, NewImageHeight );
									
									if ( m_pImageTracking->ColorSpace & CSP_VFLIP )
									{
										m_pBitmap->RotateFlip ( RotateNoneFlipY );
									}
								}
							}

							m_bImageTrackingRebuild = false;
						}

						if ( NULL != m_pBitmap )
						{
							double DX = pTracking->GetAt ( 5 ).dX;
							double DY = pTracking->GetAt ( 5 ).dY;

							double DX00 = pTracking->GetAt ( 0 ).dX;
							double DY00 = pTracking->GetAt ( 0 ).dY;
							double DX03 = pTracking->GetAt ( 3 ).dX;
							double DY03 = pTracking->GetAt ( 3 ).dY;
							double DX01 = pTracking->GetAt ( 1 ).dX;
							double DY01 = pTracking->GetAt ( 1 ).dY;

							TransformCoordToControl ( &DX01, &DY01 );
							TransformCoordToControl ( &DX03, &DY03 );
							TransformCoordToControl ( &DX00, &DY00 );

							double WidthW2	= Geometry::GetLength ( DX00, DY00, DX01, DY01 );
							double HeightW2	= Geometry::GetLength ( DX00, DY00, DX03, DY03 );

							TransformCoordToControl ( &DX, &DY );
							Gdiplus::Matrix oMatrix;
							//oMatrix.Translate ( NewImageWidth * 0.5f, NewImageHeight * 0.5f );
							oMatrix.Translate ( DX , DY  );
							oMatrix.Rotate ( Angle * 180 / M_PI );
							//oMatrix.Translate ( WidthW, -HeightW );

							m_pGrahics->SetTransform ( &oMatrix );

							m_pGrahics->DrawImage ( m_pBitmap, 
								(REAL)-WidthW2 * 0.5f,
								(REAL)-HeightW2 * 0.5f,
								(REAL)WidthW2,
								(REAL)HeightW2 );

							m_pGrahics->SetTransform ( &Gdiplus::Matrix() );
						}
					}
				}
			}
		}
	}




	inline void RenderCustomizeScheme ( int nSender, HDC hdc = NULL )
	{
		bool UseImageStudioTextRender = false;

		// retrieve window rectangle and check for valid size
		RECT rect; 
		GetClientRect(&rect);
		if ( rect.right - rect.left < 1 || rect.bottom - rect.top < 1 )
			return;

		HDC	hPreviewDC = ( hdc == NULL ) ? GetDC () : hdc;

		m_oDoubleBuffer.Create ( hPreviewDC, rect, TRUE );

		// extract last frame (skip mouse tracking case)
		BOOL bScreenValid = IsValid();
		if (nSender != c_SenderTrack)
		{
			m_oRenderManager.SetHWND ( m_hWnd );
			m_oRenderManager.SetRefresh ( ImageRendering::Constants::sc_nRefreshForRebuild );
			m_oRenderManager.BeginPaint ( m_oDoubleBuffer.GetBackDC(), rect, m_oDoubleBuffer.GetBackBitmapPixels () );
	
			m_oRenderManager.EndPaint ();
		}
	
		m_oSaveSurface.SetSource ( m_oDoubleBuffer.GetSurface () );
	
		// draw the screen
		if ( bScreenValid )
		{
			if ( m_pImageTracking )
			{
				m_oSaveSurface.Copy ();
			}

			DrawTrackingImage ();

#ifdef IMAGE_STUDIO_TEXT_RENDER

			Tracking::CTrackObject*	pTracking = m_pTracking;
			if ( NULL == pTracking )
			{
				if ( m_oTrackManager.IsEnabled () )
				{
					pTracking = m_oTrackManager.GetTrack ( 0 );
				}
			}

			Tracking::CTrackObjectTextRotatable* pText = dynamic_cast < Tracking::CTrackObjectTextRotatable* > ( pTracking );
			if ( NULL != pText )
			{
				if ( pText->IsEditingModeEnabled () )
				{
					UseImageStudioTextRender	=	true;

					double dAngle = Geometry::GetAngle( pText->GetAt(9).dX, pText->GetAt(9).dY, pText->GetAt(8).dX, pText->GetAt(8).dY) + M_PI_2;

					Structures::RECTD rectD = pText->GetUnrotatableRect ( dAngle );	

					dAngle = grad ( ((dAngle <= 0 )? dAngle + 2.0 * M_PI: dAngle));

					double CX	=	pText->GetAt ( 9 ).dX;
					double CY	=	pText->GetAt ( 9 ).dY;

					double DX00	=	pText->GetAt ( 0 ).dX;
					double DY00	=	pText->GetAt ( 0 ).dY;
					double DX03	=	pText->GetAt ( 3 ).dX;
					double DY03	=	pText->GetAt ( 3 ).dY;
					double DX01	=	pText->GetAt ( 1 ).dX;
					double DY01	=	pText->GetAt ( 1 ).dY;

					TransformCoordToControl ( &DX01, &DY01 );
					TransformCoordToControl ( &DX03, &DY03 );
					TransformCoordToControl ( &DX00, &DY00 );

					double WidthW	= Geometry::GetLength ( DX00, DY00, DX01, DY01 );
					double HeightW	= Geometry::GetLength ( DX00, DY00, DX03, DY03 );

					TransformCoordToControl ( &CX, &CY );

					m_oStudioTextRender.SetTracking ( pText );
					m_oStudioTextRender.SetBounding ( CX - WidthW / 2, CY - HeightW / 2, CX + WidthW / 2, CY + HeightW / 2,	dAngle, m_dZoom );
					m_oStudioTextRender.SetSource ( m_oDoubleBuffer.GetSurface () );
					m_oStudioTextRender.DrawXmlEffectText ( );
				}
			}
#endif
			if ( m_oStudioTextEffect.IsEffect () )
			{
				double X0 = 0, Y0 = 0, X1 = 0, Y1 = 0;
				m_oStudioTextEffect.GetNoneNormalizedBounding ( X0, Y0, X1, Y1 );
				
				TransformCoordToControl ( &X0, &Y0 );
				TransformCoordToControl ( &X1, &Y1 );

				m_oStudioTextEffect.SetBounding ( X0, Y0, X1, Y1, m_dZoom );

				m_oStudioTextEffect.SetSource ( m_oDoubleBuffer.GetSurface () );
				m_oStudioTextEffect.DrawXmlEffectText ( );
			}

			RenderBackBitmapPixels ( 
				m_oDoubleBuffer.GetRect().GetWidth(),
				m_oDoubleBuffer.GetRect ().GetHeight (),
				m_oDoubleBuffer.GetBackBitmapPixels (), 
				m_oDoubleBuffer.GetBackDC () );

			if ( false == m_bEnableOnPaintBlock )
			{
				if ( m_oRenderManager.GetZoomToWindow () )
				{
					m_oRenderManager.ZoomToWindow ();

					m_dZoom		=	m_oRenderManager.GetZoom ();
					m_dCenterX	=	m_oRenderManager.GetCenterX ();
					m_dCenterY	=	m_oRenderManager.GetCenterY ();
				}


				ShowTracking ( m_oDoubleBuffer.GetBackDC(), rect );
			}
		}
		else
		{
			Rendering::Gdi::FillRectangle(m_oDoubleBuffer.GetBackDC(), m_nBackColor1, rect);
		}

		ChangeBackBuffer ( (INT_PTR)m_oDoubleBuffer.GetBackDC() );

		// show window borders
		ShowWindowBorders ( m_oDoubleBuffer.GetBackDC(), rect );

		// flip back buffer to the screen
		m_oDoubleBuffer.Render();

#ifdef IMAGE_STUDIO_TEXT_RENDER

		if ( m_pImageTracking )
		{
			if ( m_oSaveSurface.GetSurface () )
			{
				memcpy ( m_oDoubleBuffer.GetBackBitmapPixels (), m_oSaveSurface.GetSurface (), m_oDoubleBuffer.GetRect().GetWidth() * m_oDoubleBuffer.GetRect ().GetHeight () * 4 );
			}
		}
		else
		{
			if ( m_oStudioTextEffect.IsEffect () )
			{
				if ( m_oStudioTextEffect.GetSurface () )
				{
					memcpy ( m_oDoubleBuffer.GetBackBitmapPixels (), m_oStudioTextEffect.GetSurface (), m_oDoubleBuffer.GetRect().GetWidth() * m_oDoubleBuffer.GetRect ().GetHeight () * 4 );
				}
			}

			if ( UseImageStudioTextRender )
			{
				if ( m_oStudioTextRender.GetSurface () )
				{
					memcpy ( m_oDoubleBuffer.GetBackBitmapPixels (), m_oStudioTextRender.GetSurface (), m_oDoubleBuffer.GetRect().GetWidth() * m_oDoubleBuffer.GetRect ().GetHeight () * 4 );
				}
			}
		}

#endif

		ReleaseDC(hPreviewDC);
	}

public:

	// event wrappers with coordinates corrections
	void FireEvent ( int Type, int TrackID = 0 )
	{
		//if (!m_pTracking || m_pTracking->GetCount() < 1)
		//	return;

		// ATLTRACE (_T("FireEvent : %d\n"), Type);

		if ( NULL == m_pTracking )
		{
			//if ( m_oTrackManager.IsEnabled () )
			{
				TrackChange ( TrackID, Type );
			}
		}
		else
		{
			if ( m_pTracking->GetCount() < 1 )
				return;

			// check for event type
			if (m_pTracking->IsTrackPoint())
			{
				TrackPoint(Type, AspectXAdd(m_pTracking->GetAt(0).dX), AspectYAdd(m_pTracking->GetAt(0).dY));
			}
			else if (m_pTracking->IsTrackRectangleFixedSize())
			{
				TrackRectangleFixedSize(Type, AspectXAdd(m_pTracking->GetAt(0).dX), AspectYAdd(m_pTracking->GetAt(0).dY));
			}
			else if (m_pTracking->IsTrackRectangleFixedAspect())
			{
				TrackRectangleFixedAspect(Type, AspectXAdd(m_pTracking->GetAt(0).dX), AspectYAdd(m_pTracking->GetAt(0).dY), AspectXAdd(m_pTracking->GetAt(2).dX), AspectYAdd(m_pTracking->GetAt(2).dY));
			}
			else if (m_pTracking->IsTrackLine())
			{
				TrackLine(Type, AspectXAdd(m_pTracking->GetAt(0).dX), AspectYAdd(m_pTracking->GetAt(0).dY), AspectXAdd(m_pTracking->GetAt(1).dX), AspectYAdd(m_pTracking->GetAt(1).dY));
			}
			else if (m_pTracking->IsTrackEllipse())
			{
				Structures::RECTD rectD = m_pTracking->GetDoubleBoundingRect(0, 3);

				rectD.Normalize();

				TrackEllipse(Type, AspectXAdd(rectD.left), AspectYAdd(rectD.top), AspectXAdd(rectD.right), AspectYAdd(rectD.bottom));
			}
			else if (m_pTracking->IsTrackRectangle())
			{
				Structures::RECTD rectD = m_pTracking->GetDoubleBoundingRect(0, 3);

				rectD.Normalize();

				TrackRectangle(Type, AspectXAdd(rectD.left), AspectYAdd(rectD.top), AspectXAdd(rectD.right), AspectYAdd(rectD.bottom));
			}
			else if (m_pTracking->IsTrackRectangleRotatable() )
			{
				double dAngle = Geometry::GetAngle( m_pTracking->GetAt(9).dX, m_pTracking->GetAt(9).dY, m_pTracking->GetAt(8).dX, m_pTracking->GetAt(8).dY) + M_PI_2;

				Structures::RECTD rectD = m_pTracking->GetUnrotatableRect( dAngle );	

				dAngle = grad( ((dAngle <= 0 )? dAngle + 2.0 * M_PI: dAngle));

				TrackRectangleRotatable( Type, AspectXAdd( rectD.left ), AspectYAdd( rectD.top ), AspectXAdd( rectD.right ), AspectYAdd( rectD.bottom ), dAngle);
			}
			else if (m_pTracking->IsTrackRectangleRotatableFixedAspect() )
			{
				double dAngle = Geometry::GetAngle( m_pTracking->GetAt(5).dX, m_pTracking->GetAt(5).dY, m_pTracking->GetAt(4).dX, m_pTracking->GetAt(4).dY) + M_PI_2;

				Structures::RECTD rectD = m_pTracking->GetUnrotatableRect( dAngle, 0, 2, 5 );	

				dAngle = grad( ((dAngle <= 0 )? dAngle + 2.0 * M_PI: dAngle));

				TrackRectangleRotatableFixedAspect( Type, AspectXAdd( rectD.left ), AspectYAdd( rectD.top ), AspectXAdd( rectD.right ), AspectYAdd( rectD.bottom ), dAngle );
			}
			else if (m_pTracking->IsTrackPolyline())
			{
				SAFEARRAY* pArray = 0; GetTrackArray(&pArray, TRUE);

				TrackPolyline(Type, (BYTE*)pArray);

				SafeArrayDestroy(pArray);
			}
			else if (m_pTracking->IsTrackPolygon())
			{
				SAFEARRAY* pArray = 0; GetTrackArray(&pArray, TRUE);

				TrackPolygon(Type, (BYTE*)pArray);

				SafeArrayDestroy(pArray);
			}
			else if (m_pTracking->IsTrackPie())
			{
				Structures::RECTD rectD = m_pTracking->GetDoubleBoundingRect(0, 3);
				rectD.Normalize();

				// get center and angles
				double dCenterX = rectD.GetCenter().x;
				double dCenterY = rectD.GetCenter().y;

				double dAngle1 = grad(Geometry::GetAngle(AspectXAdd ( dCenterX ), AspectYAdd ( dCenterY ), AspectXAdd ( m_pTracking->GetAt(5).dX ), AspectYAdd ( m_pTracking->GetAt(5).dY)));
				double dAngle2 = grad(Geometry::GetAngle(AspectXAdd ( dCenterX ), AspectYAdd ( dCenterY ), AspectXAdd ( m_pTracking->GetAt(4).dX ), AspectYAdd ( m_pTracking->GetAt(4).dY)));
				dAngle1 = (dAngle1 >= 0)? dAngle1 : 360 + dAngle1;
				dAngle2 = (dAngle2 >= 0)? dAngle2 : 360 + dAngle2;
				double SweepAngle = dAngle2 - dAngle1;
				SweepAngle = (SweepAngle >= 0)? SweepAngle : 360 + SweepAngle;

				TrackPie(Type, AspectXAdd(rectD.left), AspectYAdd(rectD.top), AspectXAdd(rectD.right), AspectYAdd(rectD.bottom), dAngle1, SweepAngle);
			}
			else if (m_pTracking->IsTrackPieSimple())
			{
				// get center, radiuses and angles
				double dCenterX = m_pTracking->GetAt(2).dX;
				double dCenterY = m_pTracking->GetAt(2).dY;

				double dAngle1 = grad(Geometry::GetAngle( AspectXAdd ( dCenterX ), AspectYAdd ( dCenterY ),  AspectXAdd ( m_pTracking->GetAt(1).dX ), AspectYAdd ( m_pTracking->GetAt(1).dY)));
				double dAngle2 = grad(Geometry::GetAngle( AspectXAdd ( dCenterX ), AspectYAdd ( dCenterY ),  AspectXAdd ( m_pTracking->GetAt(0).dX ), AspectYAdd ( m_pTracking->GetAt(0).dY)));
				dAngle1 = (dAngle1 >= 0)? dAngle1 : 360 + dAngle1;
				dAngle2 = (dAngle2 >= 0)? dAngle2 : 360 + dAngle2;
				double SweepAngle = dAngle2 - dAngle1;
				SweepAngle = (SweepAngle >= 0)? SweepAngle : 360 + SweepAngle;

				// apply aspect correction
				dCenterX = AspectXAdd(dCenterX);
				dCenterY = AspectYAdd(dCenterY);

				// get radius
				double dRadius = fabs(dCenterX - AspectXAdd(m_pTracking->GetAt(3).dX));

				// event track simple pie
				TrackSimplePie(Type, dCenterX, dCenterY, dRadius, dAngle1, SweepAngle);
			}
			else if (m_pTracking->IsTrackSector())
			{
				Structures::RECTD rectD = m_pTracking->GetDoubleBoundingRect(0, 3);
				rectD.Normalize();

				// get center, radiuses and angles
				double dCenterX = rectD.GetCenter().x;
				double dCenterY = rectD.GetCenter().y;

				double dAngle1 = grad(Geometry::GetAngle ( AspectXAdd ( dCenterX ), AspectYAdd ( dCenterY ), AspectXAdd ( m_pTracking->GetAt(7).dX ), AspectYAdd ( m_pTracking->GetAt(7).dY ) ) );
				double dAngle2 = grad(Geometry::GetAngle ( AspectXAdd ( dCenterX ), AspectYAdd ( dCenterY ), AspectXAdd ( m_pTracking->GetAt(6).dX ), AspectYAdd ( m_pTracking->GetAt(6).dY ) ) );
				dAngle1 = (dAngle1 >= 0)? dAngle1 : 360 + dAngle1;
				dAngle2 = (dAngle2 >= 0)? dAngle2 : 360 + dAngle2;
				double SweepAngle = dAngle2 - dAngle1;
				SweepAngle = (SweepAngle >= 0)? SweepAngle : 360 + SweepAngle;

				double dRadius1	= Geometry::GetLength ( AspectXAdd ( dCenterX ), AspectYAdd ( dCenterY ),  AspectXAdd ( m_pTracking->GetAt(6).dX ), AspectYAdd ( m_pTracking->GetAt(6).dY ) );
				double dRadius2 = Geometry::GetLength ( AspectXAdd ( dCenterX ), AspectYAdd ( dCenterY ),  AspectXAdd ( m_pTracking->GetAt(4).dX ), AspectYAdd ( m_pTracking->GetAt(4).dY ) );
				double dRatio = (dRadius1 < 0.001)? 1. : dRadius2/dRadius1;

				TrackSector(Type, AspectXAdd(rectD.left), AspectYAdd(rectD.top), AspectXAdd(rectD.right), AspectYAdd(rectD.bottom), dAngle1, SweepAngle, dRatio);
			}
			else if (m_pTracking->IsTrackSectorSimple())
			{
				// get center
				double dCenterX = m_pTracking->GetAt(4).dX;
				double dCenterY = m_pTracking->GetAt(4).dY;

				// get angles
				double dAngle1 = grad(Geometry::GetAngle ( AspectXAdd ( dCenterX ), AspectYAdd ( dCenterY ),  AspectXAdd ( m_pTracking->GetAt(1).dX ), AspectYAdd ( m_pTracking->GetAt(1).dY ) ) );
				double dAngle2 = grad(Geometry::GetAngle ( AspectXAdd ( dCenterX ), AspectYAdd ( dCenterY ),  AspectXAdd ( m_pTracking->GetAt(0).dX ), AspectYAdd ( m_pTracking->GetAt(0).dY ) ) );
				dAngle1 = (dAngle1 >= 0)? dAngle1 : 360 + dAngle1;
				dAngle2 = (dAngle2 >= 0)? dAngle2 : 360 + dAngle2;
				double SweepAngle = dAngle2 - dAngle1;
				SweepAngle = (SweepAngle >= 0)? SweepAngle : 360 + SweepAngle;

				// set aspect for center
				dCenterX = AspectXAdd(dCenterX);
				dCenterY = AspectYAdd(dCenterY);

				// get radiuses
				double dRadius1 = fabs(dCenterX - AspectXAdd(m_pTracking->GetAt(5).dX));
				double dRadius2 = fabs(dCenterX - AspectXAdd(m_pTracking->GetAt(9).dX));

				// event track simple sector
				TrackSimpleSector(Type, dCenterX, dCenterY, dRadius1, dRadius2, dAngle1, SweepAngle);
			}
			else if (m_pTracking->IsTrackCircle())
			{
				Structures::RECTD rectD = m_pTracking->GetDoubleBoundingRect(0, 3);

				rectD.Normalize();

				// get center and angles
				double dCenterX = rectD.GetCenter().x;
				double dCenterY = rectD.GetCenter().y;

				// apply aspect correction
				dCenterX = AspectXAdd(dCenterX);
				dCenterY = AspectYAdd(dCenterY);

				// get radius
				double dRadius = fabs(dCenterX - AspectXAdd(m_pTracking->GetAt(0).dX));

				TrackCircle(Type, dCenterX, dCenterY, dRadius);
			}
			else if (m_pTracking->IsTrackEllipseInRectangle())
			{
				Structures::RECTD rectD = m_pTracking->GetDoubleBoundingRect(0, 3);

				rectD.Normalize();

				TrackEllipseInRectangle(Type, AspectXAdd(rectD.left), AspectYAdd(rectD.top), AspectXAdd(rectD.right), AspectYAdd(rectD.bottom));
			}
			else if (m_pTracking->IsTrackTextRotatable())
			{
				double dAngle = Geometry::GetAngle( m_pTracking->GetAt(9).dX, m_pTracking->GetAt(9).dY, m_pTracking->GetAt(8).dX, m_pTracking->GetAt(8).dY) + M_PI_2;

				Structures::RECTD rectD = m_pTracking->GetUnrotatableRect( dAngle );	

				dAngle = grad( ((dAngle <= 0 )? dAngle + 2.0 * M_PI: dAngle));

				Tracking::CTrackObjectTextRotatable* pTrackingTextRotatable = dynamic_cast <Tracking::CTrackObjectTextRotatable*> (m_pTracking);
				//Tracking::CTrackTextRotatable* pTrackingTextRotatable	=	dynamic_cast <Tracking::CTrackTextRotatable*> ( m_pTracking );
				
				CStringW SourceText		=	L"";
				if ( NULL != pTrackingTextRotatable )
				{
					SourceText = pTrackingTextRotatable->GetString(); 
				}

				TrackTextRotatable ( Type, AspectXAdd( rectD.left ), AspectYAdd( rectD.top ), AspectXAdd( rectD.right ), AspectYAdd( rectD.bottom ), dAngle, SourceText.AllocSysString() );
			}
			else if (m_pTracking->IsTrackText())
			{
				Structures::RECTD rectD	=	m_pTracking->GetDoubleBoundingRect(0, 3);

				rectD.Normalize();

				Tracking::CTrackObjectText *pTrackingText = dynamic_cast <Tracking::CTrackObjectText*> (m_pTracking);
				
				CStringW SourceText		=	L"";
				if ( NULL != pTrackingText )
				{
					SourceText = pTrackingText->GetString();
				}

				TrackText(Type, AspectXAdd(rectD.left), AspectYAdd(rectD.top), AspectXAdd(rectD.right), AspectYAdd(rectD.bottom), SourceText.AllocSysString() );
			}
			else if (m_pTracking->IsTrackTrajectoryPolyline())
			{
				SAFEARRAY* pArray = 0; GetTrackArray(&pArray, TRUE);

				TrackTrajectoryPolyline(Type, (BYTE*)pArray);

				SafeArrayDestroy(pArray);
			}
		}
	}

public:

	STDMETHOD(SetMediaData)(IUnknown* pInterface)
	{
		if ( NULL == pInterface )
			return S_OK;

		if ( FAILED ( SetImage ( pInterface ) )  )
		{
			return S_OK;
		}

		if ( m_bAutoRefresh )
		{
			// update window
			return Refresh();
		}
		
		return S_OK;
	}

	STDMETHOD(SetMask)(SAFEARRAY** Image)
	{
		// check for valid image
		if (!IsValid())
			return S_OK;

		// destroy previous mask
		if (m_pMask)
		{
			delete[] m_pMask;
			m_pMask = 0;
		}

		// variables
		int nWidth = 0;
		int nHeight = 0;
		int nChannels = 0;
		BYTE* pPixels = 0;

		// extract image from the array
		pPixels = ImageStudioUtils::ExtractImage(Image, nWidth, nHeight, nChannels);

		// check that the image is valid
		if (!pPixels || nWidth != m_nWidth || nHeight != m_nHeight || nChannels != 1)
			return S_OK;

		// allocate memory for new mask
		m_pMask = new BYTE[m_nWidth*m_nHeight];

		// check for allocated memory
		if (!m_pMask)
			return S_OK;

		// copy information about mask
		memcpy(m_pMask, pPixels, m_nWidth*m_nHeight);

		// update window
		return Refresh();
	}
	STDMETHOD(Refresh)()
	{
		// repaint window
		OnPaint(c_SenderRefresh);

		// all ok
		return S_OK;
	}

	STDMETHOD(GetScreen)(SAFEARRAY** Image, VARIANT_BOOL* Success)
	{
		*Success = VARIANT_FALSE;

		if (!Image || !IsWindow())
			return S_OK;

		// retrieve window rectangle
		RECT rect; GetClientRect(&rect);
		int nWidth = abs(rect.right - rect.left);
		int nHeight = abs(rect.bottom - rect.top);

		// check whether window is valid
		if (!nWidth && !nHeight)
			return S_OK;

		// variables
		SAFEARRAYBOUND rgsaBound[3];
		rgsaBound[0].lLbound = 0;
		rgsaBound[0].cElements = 4;
		rgsaBound[1].lLbound = 0;
		rgsaBound[1].cElements = nWidth;
		rgsaBound[2].lLbound = 0;
		rgsaBound[2].cElements = nHeight;

		*Image = SafeArrayCreate(VT_UI1, 3, rgsaBound);

		// extract image pixels
		if (!ExtractRect(nWidth, nHeight, 4, (BYTE*)((*Image)->pvData)))
			return S_OK;

		*Success = VARIANT_TRUE;

		return S_OK;
	}

	STDMETHOD(get_AutoRefresh)(VARIANT_BOOL* Allow)
	{
		*Allow = m_bAutoRefresh ? VARIANT_TRUE : VARIANT_FALSE; 

		return S_OK;
	}
	STDMETHOD(put_AutoRefresh)(VARIANT_BOOL Allow)
	{
		m_bAutoRefresh = (Allow == VARIANT_TRUE) ? TRUE : FALSE; 

		OnPaint(c_SenderDraw); 

		return S_OK;
	}


	// zommings
	STDMETHOD(ZoomIn)()
	{
		m_dZoom *= c_dImageFactorZoomIn;

#ifdef _USE_NEW_SCHEME_RENDER_
	
		m_oRenderManager.SetZoomToWindow ( false );
		m_oRenderManager.SetZoom ( m_dZoom, false );

#endif

		OnPaint(c_SenderDraw);

		return S_OK;
	}
	STDMETHOD(ZoomOut)()
	{
//		CTemporaryCS Blocking ( &m_cs );

		m_dZoom *= c_dImageFactorZoomOut;
		
#ifdef _USE_NEW_SCHEME_RENDER_
	
		m_oRenderManager.SetZoomToWindow ( false );
		m_oRenderManager.SetZoom ( m_dZoom, false );

#endif

		OnPaint(c_SenderDraw);

		return S_OK;
	}
	STDMETHOD(ZoomToFit)()
	{
		// check for valid image
		if (!IsValid())
			return S_OK;

#ifdef _USE_NEW_SCHEME_RENDER_
		
		if ( m_bEnableCustomizeScheme )
		{
			m_oRenderManager.SetZoomToWindow ( true );
			m_oRenderManager.ZoomToWindow ();

			if ( IsWindow ( ) )
				InvalidateRect ( NULL, TRUE );
			
			m_dZoom		=	m_oRenderManager.GetZoom ();
			m_dCenterX	=	m_oRenderManager.GetCenterX ();
			m_dCenterY	=	m_oRenderManager.GetCenterY ();

			return S_OK;
		}
#endif

		// convert size in inches to size in pixels
		RECT rect; GetClientRect(&rect);

		// correct zoom factor to shrink image in window
		m_dZoom		=	min ( ( rect.right - rect.left ) / ( double ) m_nWidth, ( rect.bottom - rect.top ) / ( double ) m_nHeight );

		// reinitialize center values
		m_dCenterX	=	0.5 * m_nWidth;
		m_dCenterY	=	0.5 * m_nHeight;

		// update window
		OnPaint(c_SenderDraw);

		// all ok
		return S_OK;
	}

	STDMETHOD(ShrinkToFit)()
	{
		// check for valid image
		if (!IsValid())
			return S_OK;

		// convert size in inches to size in pixels
		RECT rect; GetClientRect(&rect);

		// correct zoom factor to shrink image in window
		m_dZoom = min((rect.right - rect.left)/(double)m_nWidth, (rect.bottom - rect.top)/(double)m_nHeight);

		// correct zoom factor
		m_dZoom = min(1.0, m_dZoom);

		// reinitialize center values
		m_dCenterX = 0.5*m_nWidth;
		m_dCenterY = 0.5*m_nHeight;

#ifdef _USE_NEW_SCHEME_RENDER_
		m_oRenderManager.SetZoom ( m_dZoom );
	
		m_oRenderManager.SetCenterX ( m_dCenterX );
		m_oRenderManager.SetCenterY ( m_dCenterY );
#endif
		// update window
		OnPaint(c_SenderDraw);

		// all ok
		return S_OK;
	}

	// view settings
	STDMETHOD(get_viewCenterX)(double* CenterX)
	{
		*CenterX = m_dCenterX;

		return S_OK;
	}
	STDMETHOD(put_viewCenterX)(double CenterX)
	{
		m_dCenterX = CenterX;
	
#ifdef _USE_NEW_SCHEME_RENDER_
		m_oRenderManager.SetZoomToWindow ( false );
		m_oRenderManager.SetCenterX ( m_dCenterX );
#endif
		OnPaint(c_SenderDraw);

		return S_OK;
	}
	STDMETHOD(get_viewCenterY)(double* CenterY)
	{
		*CenterY = m_dCenterY;

		return S_OK;
	}
	STDMETHOD(put_viewCenterY)(double CenterY)
	{
		m_dCenterY = CenterY;
	
#ifdef _USE_NEW_SCHEME_RENDER_
		m_oRenderManager.SetZoomToWindow ( false );
		m_oRenderManager.SetCenterY ( m_dCenterY );
#endif
		OnPaint(c_SenderDraw);

		return S_OK;
	}
	STDMETHOD(get_viewZoom)(double* Zoom)
	{
		*Zoom = m_dZoom;

		return S_OK;
	}
	STDMETHOD(put_viewZoom)(double Zoom)
	{
		m_dZoom = Zoom;

#ifdef _USE_NEW_SCHEME_RENDER_
		m_oRenderManager.SetZoomToWindow ( false );
		m_oRenderManager.SetZoom ( m_dZoom, false );
#endif

		OnPaint(c_SenderDraw);

		return S_OK;
	}
	STDMETHOD(get_viewAspect)(double* Aspect)
	{
		*Aspect = m_dAspect;

		return S_OK;
	}
	STDMETHOD(put_viewAspect)(double Aspect)
	{
		m_dAspect = Aspect;

		OnPaint(c_SenderDraw);

		return S_OK;
	}

	// formatting
	STDMETHOD(get_formatShowBorder)(VARIANT_BOOL* Show)
	{
		*Show = m_bShowBorder ? VARIANT_TRUE : VARIANT_FALSE;

		return S_OK;
	}
	STDMETHOD(put_formatShowBorder)(VARIANT_BOOL Show)
	{
		m_bShowBorder = (Show == VARIANT_TRUE) ? TRUE : FALSE;

		OnPaint(c_SenderDraw);

		return S_OK;
	}
	STDMETHOD(get_formatWindowStyle)(long* Style)
	{
		*Style = m_nWindowStyle;

		return S_OK;
	}
	STDMETHOD(put_formatWindowStyle)(long Style)
	{
		m_nWindowStyle = Style;

		OnPaint(c_SenderDraw);

		return S_OK;
	}
	STDMETHOD(get_formatMousePointer)(long* Pointer)
	{
		*Pointer = m_nPointer;

		return S_OK;
	}
	STDMETHOD(put_formatMousePointer)(long Pointer)
	{
		m_nPointer = Pointer;

		return S_OK;
	}

	STDMETHOD(get_formatMaskStyle)(long* Style)
	{
		*Style = m_nMaskStyle;

		return S_OK;
	}
	STDMETHOD(put_formatMaskStyle)(long Style)
	{
		m_nMaskStyle = Style;

		OnPaint(c_SenderDraw);

		return S_OK;
	}
	STDMETHOD(get_formatMaskColor)(OLE_COLOR* Color)
	{
		*Color = m_nMaskColor;

		return S_OK;
	}
	STDMETHOD(put_formatMaskColor)(OLE_COLOR Color)
	{
		m_nMaskColor = Color;

		OnPaint(c_SenderDraw);

		return S_OK;
	}

	STDMETHOD(get_formatBackStyle)(long* Style)
	{
		*Style = m_nBackStyle;

		return S_OK;
	}
	STDMETHOD(put_formatBackStyle)(long Style)
	{
		m_nBackStyle = Style;

		OnPaint(c_SenderDraw);

		return S_OK;
	}
	STDMETHOD(get_formatBackColor1)(OLE_COLOR* Color)
	{
		*Color = m_nBackColor1;

		return S_OK;
	}
	STDMETHOD(put_formatBackColor1)(OLE_COLOR Color)
	{
		m_nBackColor1 = Color;

#ifdef _USE_NEW_SCHEME_RENDER_

		COLORREF ClearColor;  
		OleTranslateColor ( Color,  NULL, &ClearColor );
		Gdiplus::Color color; color.SetFromCOLORREF ( ClearColor );
		m_oRenderManager.SetBackClearColor ( 0xFF000000 | color.GetValue () );

#endif
		OnPaint(c_SenderDraw);

		return S_OK;
	}
	STDMETHOD(get_formatBackColor2)(OLE_COLOR* Color)
	{
		*Color = m_nBackColor2;

		return S_OK;
	}
	STDMETHOD(put_formatBackColor2)(OLE_COLOR Color)
	{
		m_nBackColor2 = Color;

		OnPaint(c_SenderDraw);

		return S_OK;
	}

	STDMETHOD(get_trackType)(long* Type)
	{
		if (m_pTracking)
			*Type = m_pTracking->GetType();
		else
			*Type = Tracking::Constants::sc_nTrackTypeNone;

		return S_OK;
	}
	STDMETHOD(get_trackColorScheme)(long* ColorScheme)
	{
		*ColorScheme = m_nTrackingColorScheme;

		return S_OK;
	}
	STDMETHOD(put_trackColorScheme)(long ColorScheme)
	{
		m_nTrackingColorScheme = ColorScheme;

		if (m_nTrackingColorScheme == c_TrackColorSchemeInverse)			m_oTrackingPainter.SetColors(-2, -1, -2, -2);
		else if (m_nTrackingColorScheme == c_TrackColorSchemeBlackWhite)	m_oTrackingPainter.SetColors(0, RGB(255, 255, 255), RGB(255, 255, 255), 0);
		else if (m_nTrackingColorScheme == c_TrackColorSchemeWhiteBlack)	m_oTrackingPainter.SetColors(0, RGB(255, 255, 255), 0, RGB(255, 255, 255));
		else if (m_nTrackingColorScheme == c_TrackColorSchemeWhiteOrange)	m_oTrackingPainter.SetColors(RGB(255, 128, 64), RGB(255, 255, 255), RGB(255, 128, 64), RGB(255, 255, 255));
		else if (m_nTrackingColorScheme == c_TrackColorSchemeBlueYellow)	m_oTrackingPainter.SetColors(RGB(255, 255, 128), RGB(0, 0, 128), RGB(255, 255, 128), RGB(0, 0, 128));
		else if (m_nTrackingColorScheme == c_TrackColorSchemeBlackGreen)	m_oTrackingPainter.SetColors(RGB(0, 255, 128), 0, RGB(0, 255, 128), 0);

		OnPaint(c_SenderDraw);

		return S_OK;
	}

	STDMETHOD(BeginTracks)(BSTR XML)
	{
		RELEASEOBJECT ( m_pTracking );

		m_oTrackManager.SetViewParametres ( m_dAspect, m_nAspectWidth, m_nAspectHeight, m_nWidth, m_nHeight );

		if ( m_oTrackManager.BeginTracks ( XML ) )
		{
			m_oTrackManager.SetHWND ( m_hWnd );

			OnPaint ( c_SenderTrack );

			FireEvent ( Constants::sc_nMessageTracksCreate );

			return S_OK;
		}

		return S_FALSE;
	}

	STDMETHOD(EndTrack)()
	{
#ifdef _DEBUG
		// ATLTRACE ( _T("END-TRACK") );
#endif

		// check for valid image
		if (!IsValid())
			return S_OK;

		if ( NULL == m_pTracking )
		{
			if ( m_oTrackManager.IsEnabled () )
			{
				m_oTrackManager.EndTracks ();

				FireEvent ( Constants::sc_nMessageTracksDelete );
			}
		}
		else
		{
			// old style 

			// check for no tracking
			if (!m_pTracking || m_pTracking->IsTrackNone())
				return S_OK;

			// fire final event
			FireEvent(3);

			// clear point list
			RELEASEOBJECT ( m_pTracking );

			//delete m_pTracking;
			//m_pTracking = 0;
		}

		// update preview
		OnPaint(c_SenderTrack);

		// all ok
		return S_OK;
	}




	STDMETHOD(get_Serialize)(BSTR* XML)
	{
		if ( NULL != m_pTracking )
		{
			CString TrackXml;

			Core::Serialize::CTrackXmlWriter oWriter ( m_dAspect, m_nAspectWidth, m_nAspectHeight, m_nWidth, m_nHeight );
			oWriter.WriteXml ( m_pTracking, TrackXml );
			*XML = TrackXml.AllocSysString();

			return S_OK;
		}

		m_oTrackManager.SetViewParametres ( m_dAspect, m_nAspectWidth, m_nAspectHeight, m_nWidth, m_nHeight );

		if ( m_oTrackManager.WriteXmlTracks ( XML ) )
			return S_OK;

		return S_OK;
	}

	STDMETHOD(put_Serialize)(BSTR XML)
	{
		return BeginTracks ( XML );
	}

	STDMETHOD(get_Settings)(BSTR* XML)
	{
		m_oTrackManager.WriteXmlSettings ( XML );

		return S_OK;
	}

	STDMETHOD(put_Settings)(BSTR XML)
	{
		m_oTrackManager.ReadXmlSettings ( XML );

		return S_OK;
	}

	//
	STDMETHOD(BeginTrackPoint)(double X, double Y)
	{
		if (!IsValid())
			return S_OK;

		double dX = AspectXRemove(X);
		double dY = AspectYRemove(Y);

		Tracking::CTrackObjectPoints* pTracking = new Tracking::CTrackObjectPoints();

		pTracking->Create(dX, dY);

		BeginTrack(pTracking);

		return S_OK;
	}

	STDMETHOD(BeginTrackLine)(double X1, double Y1, double X2, double Y2)
	{
		if (!IsValid())
			return S_OK;

		double dX1 = AspectXRemove(X1);
		double dY1 = AspectYRemove(Y1);
		double dX2 = AspectXRemove(X2);
		double dY2 = AspectYRemove(Y2);

		Tracking::CTrackObjectLine* pTracking = new Tracking::CTrackObjectLine();

		pTracking->Create(dX1, dY1, dX2, dY2);

		BeginTrack(pTracking);

		return S_OK;
	}

	STDMETHOD(BeginTrackEllipse)(double Left, double Top, double Right, double Bottom)
	{
		if (!IsValid())
			return S_OK;

		double dLeft = AspectXRemove(Left);
		double dTop = AspectYRemove(Top);
		double dRight = AspectXRemove(Right);
		double dBottom = AspectYRemove(Bottom);

		Tracking::CTrackObjectEllipse* pTracking = new Tracking::CTrackObjectEllipse();

		pTracking->Create(dLeft, dTop, dRight, dBottom);

		BeginTrack(pTracking);

		return S_OK;
	}

	STDMETHOD(BeginTrackRectangle)(double Left, double Top, double Right, double Bottom)
	{
		if (!IsValid())
			return S_OK;

		double dLeft = AspectXRemove(Left);
		double dTop = AspectYRemove(Top);
		double dRight = AspectXRemove(Right);
		double dBottom = AspectYRemove(Bottom);

		Tracking::CTrackObjectRectangle* pTracking = new Tracking::CTrackObjectRectangle();

		pTracking->Create(dLeft, dTop, dRight, dBottom);
		pTracking->GetTrackSticking ()	=	m_oTrackSticking;

		BeginTrack(pTracking);
#ifdef _DEBUG
		//ATLTRACE ( _T("BeginTrackRectangle-TRACK") );
#endif

		return S_OK;
	}

	STDMETHOD(BeginTrackRectangleFixedSize)(double Left, double Top, double Width, double Height)
	{
		if (!IsValid())
			return S_OK;

		double dLeft = AspectXRemove(Left);
		double dTop = AspectYRemove(Top);
		double dRight = AspectXRemove(Left + Width);
		double dBottom = AspectYRemove(Top + Height);

		Tracking::CTrackObjectRectangleFixedSize* pTracking = new Tracking::CTrackObjectRectangleFixedSize();

		pTracking->Create(dLeft, dTop, dRight, dBottom);

		BeginTrack(pTracking);

		return S_OK;
	}
	STDMETHOD(BeginTrackRectangleFixedAspect)(double Left, double Top, double Width, double Height)
	{
		if (!IsValid())
			return S_OK;

		double dLeft = AspectXRemove(Left);
		double dTop = AspectYRemove(Top);
		double dRight = AspectXRemove(Left + Width);
		double dBottom = AspectYRemove(Top + Height);

		Tracking::CTrackObjectRectangleFixedAspect* pTracking = new Tracking::CTrackObjectRectangleFixedAspect();

		pTracking->Create(dLeft, dTop, dRight, dBottom);

		BeginTrack(pTracking);

		return S_OK;
	}

	STDMETHOD(BeginTrackPolyline)(SAFEARRAY** Points)
	{
		if (!IsValid())
			return S_OK;

		Structures::POINTD* pPoints = 0;
		int nPointsCount = 0;

		if (!SafeArrayToPoints(Points, TRUE, pPoints, nPointsCount))
			return S_OK;

		Tracking::CTrackObjectPolyline* pTracking = new Tracking::CTrackObjectPolyline();

		pTracking->Create(pPoints, nPointsCount);

		delete[] pPoints;

		BeginTrack(pTracking);

		return S_OK;
	}
	STDMETHOD(BeginTrackPolygon)(SAFEARRAY** Points)
	{
		if (!IsValid())
			return S_OK;

		Structures::POINTD* pPoints = 0;
		int nPointsCount = 0;

		if (!SafeArrayToPoints(Points, TRUE, pPoints, nPointsCount))
			return S_OK;

		Tracking::CTrackObjectPolygon* pTracking = new Tracking::CTrackObjectPolygon();

		pTracking->Create(pPoints, nPointsCount);

		delete[] pPoints;

		BeginTrack(pTracking);

		return S_OK;
	}	
	STDMETHOD(BeginTrackPie)(double Left, double Top, double Right, double Bottom, double StartAngle, double SweepAngle)
	{
		if (!IsValid())
			return S_OK;

		double dLeft = AspectXRemove(Left);
		double dTop = AspectYRemove(Top);
		double dRight = AspectXRemove(Right);
		double dBottom = AspectYRemove(Bottom);

		Tracking::CTrackObjectPie* pTracking = new Tracking::CTrackObjectPie();

		pTracking->Create(dLeft, dTop, dRight, dBottom, StartAngle, SweepAngle);
	
		Structures::RECTD rectD = pTracking->GetDoubleBoundingRect(0, 3);
		rectD.Normalize();

		// get center and angles
		double dCenterX = rectD.GetCenter().x;
		double dCenterY = rectD.GetCenter().y;

		double dAngle1 = grad(Geometry::GetAngle ( AspectXRemove ( dCenterX ), AspectYRemove ( dCenterY ), AspectXRemove ( pTracking->GetAt(5).dX ), AspectYRemove ( pTracking->GetAt(5).dY)));
		double dAngle2 = grad(Geometry::GetAngle ( AspectXRemove ( dCenterX ), AspectYRemove ( dCenterY ), AspectXRemove ( pTracking->GetAt(4).dX ), AspectYRemove ( pTracking->GetAt(4).dY)));
		dAngle1 = (dAngle1 >= 0)? dAngle1 : 360 + dAngle1;
		dAngle2 = (dAngle2 >= 0)? dAngle2 : 360 + dAngle2;
		SweepAngle = dAngle2 - dAngle1;
		SweepAngle = (SweepAngle >= 0)? SweepAngle : 360 + SweepAngle;

		pTracking->Create(dLeft, dTop, dRight, dBottom, dAngle1, SweepAngle);

		BeginTrack(pTracking);

		return S_OK;
	}

	STDMETHOD(BeginTrackSimplePie)(double CenterX, double CenterY, double Radius, double StartAngle, double SweepAngle)
	{
		if (!IsValid())
			return S_OK;

		// create rect
		double Left = CenterX - Radius;
		double Right = CenterX + Radius;
		double Top = CenterY - Radius;
		double Bottom = CenterY + Radius;

		// set aspect
		double dLeft = AspectXRemove(Left);
		double dTop = AspectYRemove(Top);
		double dRight = AspectXRemove(Right);
		double dBottom = AspectYRemove(Bottom);

		Tracking::CTrackObjectPieSimple* pTracking = new Tracking::CTrackObjectPieSimple();

		pTracking->Create(dLeft, dTop, dRight, dBottom, StartAngle, SweepAngle);

		// get center, radiuses and angles
		double dCenterX = pTracking->GetAt(2).dX;
		double dCenterY = pTracking->GetAt(2).dY;

		double dAngle1 = grad(Geometry::GetAngle ( AspectXRemove ( dCenterX ), AspectYRemove ( dCenterY ), AspectXRemove ( pTracking->GetAt(1).dX ), AspectYRemove ( pTracking->GetAt(1).dY)));
		double dAngle2 = grad(Geometry::GetAngle ( AspectXRemove ( dCenterX ), AspectYRemove ( dCenterY ), AspectXRemove ( pTracking->GetAt(0).dX ), AspectYRemove ( pTracking->GetAt(0).dY)));
		dAngle1 = (dAngle1 >= 0)? dAngle1 : 360 + dAngle1;
		dAngle2 = (dAngle2 >= 0)? dAngle2 : 360 + dAngle2;
		SweepAngle = dAngle2 - dAngle1;
		SweepAngle = (SweepAngle >= 0)? SweepAngle : 360 + SweepAngle;

		// apply aspect correction
		dCenterX = AspectXAdd(dCenterX);
		dCenterY = AspectYAdd(dCenterY);

		// get radius
		double dRadius = fabs(dCenterX - AspectXAdd(pTracking->GetAt(3).dX));
		
		pTracking->Create(dLeft, dTop, dRight, dBottom, dAngle1, SweepAngle);

		// event track simple pie
		//TrackSimplePie(Type, dCenterX, dCenterY, dRadius, dAngle1, SweepAngle);

		BeginTrack(pTracking);

		return S_OK;
	}
	STDMETHOD(BeginTrackSector)(double Left, double Top, double Right, double Bottom, double StartAngle, double SweepAngle, double Ratio)
	{
		if (!IsValid())
			return S_OK;

		double dLeft	=	AspectXRemove(Left);
		double dTop		=	AspectYRemove(Top);
		double dRight	=	AspectXRemove(Right);
		double dBottom	=	AspectYRemove(Bottom);

		Tracking::CTrackObjectSector* pTracking = new Tracking::CTrackObjectSector();

		pTracking->Create(dLeft, dTop, dRight, dBottom, StartAngle, SweepAngle, Ratio);

		Structures::RECTD rectD = pTracking->GetDoubleBoundingRect(0, 3);
		rectD.Normalize();

		// get center, radiuses and angles
		double dCenterX	=	rectD.GetCenter().x;
		double dCenterY	=	rectD.GetCenter().y;

		double dAngle1	=	grad ( Geometry::GetAngle(AspectXRemove ( dCenterX ), AspectYRemove ( dCenterY ), AspectXRemove ( pTracking->GetAt(7).dX), AspectYRemove(pTracking->GetAt(7).dY)));
		double dAngle2	=	grad ( Geometry::GetAngle(AspectXRemove ( dCenterX ), AspectYRemove ( dCenterY ), AspectXRemove ( pTracking->GetAt(6).dX), AspectYRemove(pTracking->GetAt(6).dY)));
		dAngle1			=	( dAngle1 >= 0 )	?	dAngle1 : 360 + dAngle1;
		dAngle2			=	( dAngle2 >= 0 )	?	dAngle2 : 360 + dAngle2;
		SweepAngle		=	dAngle2 - dAngle1;
		SweepAngle		=	( SweepAngle >= 0 ) ?	SweepAngle : 360 + SweepAngle;

		double dRadius1 = Geometry::GetLength( AspectXRemove ( dCenterX ),  AspectYRemove ( dCenterY ),  AspectXRemove ( pTracking->GetAt(6).dX ), AspectYRemove(pTracking->GetAt(6).dY ) );
		double dRadius2 = Geometry::GetLength( AspectXRemove ( dCenterX ),  AspectYRemove ( dCenterY ),  AspectXRemove ( pTracking->GetAt(4).dX ), AspectYRemove(pTracking->GetAt(4).dY ) );
		double dRatio = (dRadius1 < 0.001)? 1. : dRadius2/dRadius1;

		pTracking->Create(dLeft, dTop, dRight, dBottom, dAngle1, SweepAngle, dRatio );

		BeginTrack(pTracking);

		return S_OK;
	}
	STDMETHOD(BeginTrackSimpleSector)(double CenterX, double CenterY, double Radius1, double Radius2, double StartAngle, double SweepAngle)
	{		
		if (!IsValid())
			return S_OK;

		// create rect
		double Left = CenterX - Radius1;
		double Right = CenterX + Radius1;
		double Top = CenterY - Radius1;
		double Bottom = CenterY + Radius1;

		// set aspect
		double dLeft = AspectXRemove(Left);
		double dTop = AspectYRemove(Top);
		double dRight = AspectXRemove(Right);
		double dBottom = AspectYRemove(Bottom);

		double Ratio = Radius2/Radius1;

		Tracking::CTrackObjectSectorSimple* pTracking = new Tracking::CTrackObjectSectorSimple();

		pTracking->Create(dLeft, dTop, dRight, dBottom, StartAngle, SweepAngle, Ratio);

		//Structures::RECTD rectD = pTracking->GetDoubleBoundingRect(0, 3);
		//rectD.Normalize();

		//double dCenterX	=	rectD.GetCenter().x;
		//double dCenterY	=	rectD.GetCenter().y;

		double dCenterX = pTracking->GetAt(4).dX;
		double dCenterY = pTracking->GetAt(4).dY;

		// get angles
		double dAngle1 = grad(Geometry::GetAngle ( AspectXRemove ( dCenterX ), AspectYRemove ( dCenterY ), AspectXRemove ( pTracking->GetAt(1).dX ), AspectYRemove ( pTracking->GetAt(1).dY ) ) );
		double dAngle2 = grad(Geometry::GetAngle ( AspectXRemove ( dCenterX ), AspectYRemove ( dCenterY ), AspectXRemove ( pTracking->GetAt(0).dX ), AspectYRemove ( pTracking->GetAt(0).dY ) ) );
		dAngle1 = (dAngle1 >= 0)? dAngle1 : 360 + dAngle1;
		dAngle2 = (dAngle2 >= 0)? dAngle2 : 360 + dAngle2;
		SweepAngle = dAngle2 - dAngle1;
		SweepAngle = (SweepAngle >= 0)? SweepAngle : 360 + SweepAngle;

		pTracking->Create(dLeft, dTop, dRight, dBottom, dAngle1, SweepAngle, Ratio);

		BeginTrack(pTracking);
		
		return S_OK;
	}
	STDMETHOD(BeginTrackEllipseInRectangle)(double Left, double Top, double Right, double Bottom)
	{
		if (!IsValid())
			return S_OK;

		double dLeft = AspectXRemove(Left);
		double dTop = AspectYRemove(Top);
		double dRight = AspectXRemove(Right);
		double dBottom = AspectYRemove(Bottom);

		Tracking::CTrackObjectEllipseInRectangle* pTracking = new Tracking::CTrackObjectEllipseInRectangle();

		pTracking->Create(dLeft, dTop, dRight, dBottom);

		pTracking->GetTrackSticking ()	=	m_oTrackSticking;

		BeginTrack(pTracking);

		return S_OK;
	}
	STDMETHOD(BeginTrackCircle)(double CenterX, double CenterY, double Radius)
	{
		if (!IsValid())
			return S_OK;

		// set aspect
		double dLeft = AspectXRemove(CenterX - Radius);
		double dTop = AspectYRemove(CenterY - Radius);
		double dRight = AspectXRemove(CenterX + Radius);
		double dBottom = AspectYRemove(CenterY + Radius);

		Tracking::CTrackObjectCircle* pTracking = new Tracking::CTrackObjectCircle();

		pTracking->Create(dLeft, dTop, dRight, dBottom);

		BeginTrack(pTracking);

		return S_OK;
	}

	STDMETHOD(BeginTrackRectangleRotatable)(double Left, double Top, double Right, double Bottom, double Angle, double StickAngle)
	{
		if (!IsValid())
			return S_OK;

		double dLeft	= AspectXRemove(Left);
		double dTop		= AspectYRemove(Top);
		double dRight	= AspectXRemove(Right);
		double dBottom	= AspectYRemove(Bottom);

		Tracking::CTrackObjectRectangleRotatable* pTracking = new Tracking::CTrackObjectRectangleRotatable();

		pTracking->Create(dLeft, dTop, dRight, dBottom, Angle, StickAngle );

		BeginTrack(pTracking);

		return S_OK;
	}

	STDMETHOD(BeginTrackRectangleRotatableFixedAspect)(double Left, double Top, double Right, double Bottom, double Angle, double StickAngle)
	{
		if (!IsValid())
			return S_OK;

		double dLeft	= AspectXRemove(Left);
		double dTop		= AspectYRemove(Top);
		double dRight	= AspectXRemove(Right);
		double dBottom	= AspectYRemove(Bottom);

		Tracking::CTrackObjectRectangleRotatableFixedAspect* pTracking = new Tracking::CTrackObjectRectangleRotatableFixedAspect();

		pTracking->Create( dLeft, dTop, dRight, dBottom, Angle, StickAngle );

		BeginTrack(pTracking);

		return S_OK;
	}
	//
	STDMETHOD(BeginTrackText)(double Left, double Top, double Right, double Bottom, BSTR inText, BSTR FontName, int FontSize, int FontType, int FontColor)
	{
		if (!IsValid())
			return S_OK;

		double dLeft	= AspectXRemove(Left);
		double dTop		= AspectYRemove(Top);
		double dRight	= AspectXRemove(Right);
		double dBottom	= AspectYRemove(Bottom);

		Tracking::CTrackObjectText* pTracking = new Tracking::CTrackObjectText();

		pTracking->Create ( dLeft, dTop, dRight, dBottom, inText, FontName, FontSize, FontType, FontColor );
		pTracking->SetEnableDrawText ( m_bEnableDrawText );
		pTracking->GetIMEUI().Initialize ( m_hWnd );

		BeginTrack ( pTracking );

		return S_OK;
	}

	STDMETHOD(BeginTrackTextRotatable)(double Left, double Top, double Right, double Bottom, double Angle, double StickAngle, BSTR inText, BSTR FontName, int FontSize, int FontType, int FontColor)
	{
		if (!IsValid())
			return S_OK;

		double dLeft	= AspectXRemove(Left);
		double dTop		= AspectYRemove(Top);
		double dRight	= AspectXRemove(Right);
		double dBottom	= AspectYRemove(Bottom);

		Tracking::CTrackObjectTextRotatable* pTracking = new Tracking::CTrackObjectTextRotatable();

		pTracking->SetResizeShapeToFitText ( m_bResizeShapeToFitText );
		pTracking->Create ( dLeft, dTop, dRight, dBottom, Angle, StickAngle, inText, FontName, FontSize, FontType, FontColor );
		pTracking->SetEnableDrawText ( m_bEnableDrawText );
		pTracking->GetIMEUI().Initialize ( m_hWnd );

		BeginTrack(pTracking);

		/*

		if ( FALSE )
		{
			Tracking::CTrackTextRotatable* pTracking = new Tracking::CTrackTextRotatable();

			pTracking->SetResizeShapeToFitText ( m_bResizeShapeToFitText );
			pTracking->Create ( dLeft, dTop, dRight, dBottom, Angle, StickAngle, inText, FontName, FontSize, FontType, FontColor );
			pTracking->SetEnableDrawText ( m_bEnableDrawText );
			pTracking->GetIMEUI().Initialize ( m_hWnd );

			BeginTrack ( pTracking );
		}
		else
		{
			Tracking::CTrackObjectTextRotatable* pTracking = new Tracking::CTrackObjectTextRotatable();

			pTracking->SetResizeShapeToFitText ( m_bResizeShapeToFitText );
			pTracking->Create ( dLeft, dTop, dRight, dBottom, Angle, StickAngle, inText, FontName, FontSize, FontType, FontColor );
			pTracking->SetEnableDrawText ( m_bEnableDrawText );
			pTracking->GetIMEUI().Initialize ( m_hWnd );

			BeginTrack(pTracking);
		}
		*/

		return S_OK;
	}

	//
	STDMETHOD(MoveTrack)(double dX, double dY)
	{
		if (!IsValid())
			return S_OK;

		if ( NULL != m_pTracking )
		{
			m_pTracking->MoveTrack(dX, dY);
			OnPaint(c_SenderTrack);
		}
		else
		{
			if ( m_oTrackManager.MoveTrack ( dX, dY ) )
				OnPaint(c_SenderTrack);
		}

		return S_OK;
	}
	STDMETHOD(TransformCoordToBitmap)(double* X, double* Y)
	{
		// retrieve cursor position and preview window rectanlge
		RECT rect; GetWindowRect(&rect);

		// compute preview image size
		int nWidth  = abs( rect.right - rect.left );
		int nHeight = abs( rect.bottom - rect.top );
		// check for valid simpliest case when image size is invalid
		if (!nWidth || !nHeight)
			return S_FALSE;

		*X = ( *X - 0.5 * nWidth  + m_dCenterX * m_dZoom ) / m_dZoom;
		*Y = ( *Y - 0.5 * nHeight + m_dCenterY * m_dZoom ) / m_dZoom; 

		return S_OK;
	}
	STDMETHOD(TransformCoordToControl)(double* X, double* Y)
	{
		// retrieve cursor position and preview window rectanlge
		RECT rect; GetWindowRect(&rect);

		// compute preview image size
		int nWidth  = abs( rect.right - rect.left );
		int nHeight = abs( rect.bottom - rect.top );

		// check for valid simpliest case when image size is invalid
		if (!nWidth || !nHeight)
			return S_FALSE;

		*X = *X * m_dZoom + 0.5 * nWidth  - m_dCenterX * m_dZoom;
		*Y = *Y * m_dZoom + 0.5 * nHeight - m_dCenterY * m_dZoom; 	

		return S_OK;
	}

	STDMETHOD(BeginTrackTrajectoryPolyline)(SAFEARRAY** Points)
	{
		if (!IsValid())
			return S_OK;

		Structures::POINTD* pPoints = 0;
		int nPointsCount = 0;

		if (!SafeArrayToPoints(Points, TRUE, pPoints, nPointsCount))
			return S_OK;

		Tracking::CTrackObjectTrajectoryPolyline* pTracking = new Tracking::CTrackObjectTrajectoryPolyline();

		pTracking->Create(pPoints, nPointsCount);

		delete[] pPoints;

		BeginTrack(pTracking);

		return S_OK;
	}

	STDMETHOD(get_InterpolationMode)(long* Mode)
	{
		*Mode = m_nInterpolationMode;

		return S_OK;
	}

	STDMETHOD(put_InterpolationMode)(long Mode)
	{
		 if ( Mode > 1 )
			Mode = 0;

		m_nInterpolationMode = Mode;

#ifdef _USE_NEW_SCHEME_RENDER_

		m_oRenderManager.SetInterpolationMode ( Mode  );
#endif

		OnPaint ( c_SenderDraw );

		return S_OK;
	}
		
	STDMETHOD ( IImageView5::Command )				( BSTR COMMAND,		VARIANT* ParamValue)
	{
		if ( false == m_oTrackManager.Command ( COMMAND, ParamValue ) )
			return S_FALSE;

		return S_OK;
	}

	STDMETHOD ( IImageView5::SetAdditionalParam )	( BSTR ParamName,	VARIANT ParamValue )
	{
		CComVariant ParamVal	=	ParamValue;
		CComBSTR ParamStr		=	ParamName;
	
		if ( CComBSTR ( _T("SetText") ) == ParamName )
		{
			m_oStudioTextEffect.SetText ( ParamValue.bstrVal );

			return S_OK;
		}

		if ( CComBSTR ( _T("SetXMLEffect") ) == ParamName )
		{
			m_bEnableDrawText	=	false;
			
			m_oStudioTextRender.SetLayoutSizes ( m_nWidth, m_nHeight );
			m_oStudioTextRender.SetEffectXml ( ParamValue.bstrVal );

			return S_OK;
		}
		
		if ( CComBSTR ( _T("SetTextXmlEffect") ) == ParamName )	// внешний текст не трек
		{
			m_oStudioTextEffect.SetBounding ( 0.0, 0.0, 0.0, 0.0, m_dZoom );
			m_oStudioTextEffect.SetEffectXml ( ParamValue.bstrVal );

			return S_OK;
		}
			
		if ( CComBSTR ( _T("SetImage" ) ) == ParamName )
		{
			SetImage ( ParamValue.punkVal );

			return S_OK;
		}

		if ( CComBSTR ( _T("SetImageNULL") ) == ParamName )
		{
			Clear ();

			m_nWidth	=	0;
			m_nHeight	=	0;

#ifdef _USE_NEW_SCHEME_RENDER_
		
			if ( m_bEnableCustomizeScheme )
			{
				m_oRenderManager.SetImage ( NULL, 0, 0 );
			}
#endif
			if ( IsWindow ( ) )
			{
				InvalidateRect ( NULL, TRUE );
			}

			return S_OK;
		}

		if ( CComBSTR ( _T("SetImageTracking") ) == ParamName )
		{
			RELEASEINTERFACE ( m_pImageTracking );

			if ( NULL != ParamVal.punkVal )
			{
				MediaCore::IAVSMediaData* pDublicateCopy	=	NULL;
				((MediaCore::IAVSUncompressedVideoFrame*)ParamVal.punkVal)->raw_CreateDuplicate ( 2, &pDublicateCopy );
				if ( NULL != pDublicateCopy )
				{
					pDublicateCopy->QueryInterface ( &m_pImageTracking );
					pDublicateCopy->Release ();

					m_bImageTrackingRebuild		=	true;
					m_nImageTrackingAlpha		=	255L;

					/*
					if ( m_pImageTracking )
					{
						m_nImageTrackingAlpha	=	128;//ParamValue.lVal;

						m_nImageTrackingAlpha	=	__min ( m_nImageTrackingAlpha, 255L );
						m_nImageTrackingAlpha	=	__max ( m_nImageTrackingAlpha, 0L );

						LONG BufferSize			=	m_pImageTracking->Width * m_pImageTracking->Height;
						DWORD* pSrc				=	(DWORD*) ( m_pImageTracking->Buffer );

						DWORD AlphaMask			=	(DWORD) ( m_nImageTrackingAlpha << 24 );

						for ( DWORD i = 0; i < BufferSize; ++i )
						{
							if ( pSrc[i] > AlphaMask )
								pSrc[i]			=	( pSrc[i] & 0x00FFFFFF ) | AlphaMask;
						}

					}
					*/


					/*
#ifdef _DEBUG
					CLSID ImageGUID;
					ImageRendering::ImageUtils::GetEncoderCLSID ( L"image/png", &ImageGUID );

					LONG ColorSpace = m_pImageTracking->ColorSpace;

					Gdiplus::Bitmap SetBitmap ( m_pImageTracking->Width, m_pImageTracking->Height, 
						m_pImageTracking->Width * 4, PixelFormat32bppARGB, m_pImageTracking->Buffer );

				//	Gdiplus::Bitmap SetBitmap ( nWidth, nHeight, 4 * nWidth, PixelFormat32bppARGB, pPixels );

					static long i = 0;
					i++;
					wchar_t buffer [256];
					wsprintfW ( buffer, L"C:\\SetImage_%i.png", i);
					SetBitmap.Save ( buffer , &ImageGUID );
#endif
					*/
				}
			}

			return S_OK;
		}

		if ( CComBSTR ( _T("SetImageTrackingAlpha") ) == ParamName )
		{
			if ( m_pImageTracking )
			{
				m_nImageTrackingAlpha	=	ParamValue.lVal;

				m_nImageTrackingAlpha	=	__min ( m_nImageTrackingAlpha, 255L );
				m_nImageTrackingAlpha	=	__max ( m_nImageTrackingAlpha, 0L );

				LONG BufferSize			=	m_pImageTracking->Width * m_pImageTracking->Height;
				DWORD* pSrc				=	(DWORD*) ( m_pImageTracking->Buffer );

				DWORD AlphaMask			=	(DWORD) ( m_nImageTrackingAlpha << 24 );

				for ( DWORD i = 0; i < BufferSize; ++i )
				{
					if ( pSrc[i] > AlphaMask )
						pSrc[i]			=	( pSrc[i] & 0x00FFFFFF ) | AlphaMask;
				}
			}

			return S_OK;
		}
		
		if ( CComBSTR ( _T("SetBounding") ) == ParamName )
			return SetBounding ( ParamValue.bstrVal );

		if ( CComBSTR ( _T("SetSticking") ) == ParamName )
			return SetSticking ( ParamValue.bstrVal );
		
		if ( CComBSTR ( _T("CustomizeScheme") ) == ParamName )
		{
			m_bEnableCustomizeScheme	=	( TRUE == ParamValue.lVal ) ? true : false;

			BGRA_Flip ( m_pPixels, m_nWidth, m_nHeight );
			
			return S_OK;
		}

		if (ParamStr == L"TrackingColors")
		{
			ParamVal.ChangeType(VT_I4, NULL); 

			put_trackColorScheme(ParamVal.lVal);
		}
		else if (ParamStr == L"MediaData")
		{
			ParamVal.ChangeType(VT_UNKNOWN, NULL);
			IUnknown* pImage = ParamVal.punkVal;

			SetMediaData(pImage);
		}
		else if ( ParamStr == L"TextBlockEditing" )
		{
			if ( NULL != m_pTracking )
			{
				if ( m_pTracking->IsTrackText () )
				{
					Tracking::CTrackObjectText* pTextTracking = (Tracking::CTrackObjectText*)m_pTracking;
					if ( NULL != pTextTracking )
					{
						pTextTracking->EnableBlockEditing ( ( ParamValue.boolVal == VARIANT_TRUE ) ? true : false );
						return S_OK;
					}
				}

				if ( m_pTracking->IsTrackTextRotatable () )
				{
					Tracking::CTrackObjectTextRotatable* pTextRotatableTracking = (Tracking::CTrackObjectTextRotatable*)m_pTracking;
					//Tracking::CTrackTextRotatable* pTextRotatableTracking = (Tracking::CTrackTextRotatable*) m_pTracking;
					if ( NULL != pTextRotatableTracking )
					{
						pTextRotatableTracking->EnableBlockEditing ( ( ParamValue.boolVal == VARIANT_TRUE ) ? true : false );
						return S_OK;
					}
				}
			}

			return S_OK;
		}
		else if ( ParamStr == L"noresize" )
		{
			SetNoResizeModeRFAT ( ParamValue.bstrVal );

			return S_OK;
		}
		else if ( ParamStr == L"sizesmode" )
		{
			SetSizesModeRFAT ( ParamValue.bstrVal );

			return S_OK;
		}
		else if ( ParamStr == L"TextBlockResize" )
		{
			if ( NULL != m_pTracking )
			{
				if ( m_pTracking->IsTrackTextRotatable () )
				{
					Tracking::CTrackObjectTextRotatable* pTextRotatableTracking = (Tracking::CTrackObjectTextRotatable*)m_pTracking;
					//Tracking::CTrackTextRotatable* pTextRotatableTracking = (Tracking::CTrackTextRotatable*)m_pTracking;

					if ( NULL != pTextRotatableTracking )
					{
						pTextRotatableTracking->EnableBlockResize ( ( ParamValue.boolVal == VARIANT_TRUE ) ? true : false );
						return S_OK;
					}
				}
			}

			return S_OK;
		}
		else if ( ParamStr == L"BlockPaint" )
		{
			if ( ParamValue.boolVal == VARIANT_TRUE )
				m_bEnableOnPaintBlock	=	true;
			else
				m_bEnableOnPaintBlock	=	false;
		
			return S_OK;
		}
		else if ( ParamStr	==	L"EnableDrawText" )
		{
			( ParamValue.boolVal == VARIANT_TRUE ) ? m_bEnableDrawText = true : m_bEnableDrawText = false;
			
			Tracking::CTrackObject*	pTracking = m_pTracking;
			if ( NULL == pTracking )
			{
				if ( m_oTrackManager.IsEnabled () )
				{
					pTracking = m_oTrackManager.GetTrack ( 0 );
				}
			}

			if ( NULL != pTracking )
			{
				if ( pTracking->IsTrackTextRotatable ()  )
				{

					Tracking::CTrackObjectTextRotatable* pTextRotatableTracking = (Tracking::CTrackObjectTextRotatable*)pTracking;
					//Tracking::CTrackTextRotatable* pTextRotatableTracking = (Tracking::CTrackTextRotatable*)pTracking;

					if ( NULL != pTextRotatableTracking )
					{
						pTextRotatableTracking->SetEnableDrawText ( m_bEnableDrawText );

						return S_OK;
					}
				}

				if ( pTracking->IsTrackText () )
				{
					Tracking::CTrackObjectText* pTextRotatableTracking = (Tracking::CTrackObjectText*)pTracking;
					if ( NULL != pTextRotatableTracking )
					{
						pTextRotatableTracking->SetEnableDrawText ( m_bEnableDrawText );

						return S_OK;
					}
				}
			}

			return S_OK;
		}

		/*

		if ( CComBSTR ( L"NotAutoFitText" ) == ParamStr )
		{
			Tracking::CTrackObject*	pTracking = m_pTracking;
			if ( NULL == pTracking )
			{
				if ( m_oTrackManager.IsEnabled () )
				{
					pTracking = m_oTrackManager.GetTrack ( 0 );
				}
			}

			Tracking::CTrackTextRotatable* pTrack = dynamic_cast < Tracking::CTrackTextRotatable* > ( pTracking );
			if ( NULL != pTrack )
			{
				pTrack->SetNotAutoFitText ( ( ParamValue.boolVal == VARIANT_TRUE ) ? true : false  );
			}

			return S_OK;
		}
		*/

		if ( CComBSTR ( L"ResizeShapeToFitText" ) == ParamStr )
		{
			( ParamValue.boolVal == VARIANT_TRUE ) ? m_bResizeShapeToFitText = true : m_bResizeShapeToFitText = false;

			Tracking::CTrackObject*	pTracking = m_pTracking;
			if ( NULL == pTracking )
			{
				if ( m_oTrackManager.IsEnabled () )
				{
					pTracking = m_oTrackManager.GetTrack ( 0 );
				}
			}

			Tracking::CTrackObjectTextRotatable* pTextRotatableTracking = (Tracking::CTrackObjectTextRotatable*)pTracking;
			//Tracking::CTrackTextRotatable* pTextRotatableTracking = (Tracking::CTrackTextRotatable*)pTracking;

			if ( NULL != pTextRotatableTracking )
			{
				pTextRotatableTracking->SetResizeShapeToFitText ( m_bResizeShapeToFitText );

				return S_OK;
			}

			//Tracking::CTrackTextRotatable* pTrack = dynamic_cast < Tracking::CTrackTextRotatable* > ( pTracking );
			//if ( NULL != pTrack )
			//{
			//	pTrack->SetResizeShapeToFitText ( ( ParamValue.boolVal == VARIANT_TRUE ) ? true : false  );
			//}

			return S_OK;
		}
		
		/*

		if ( CComBSTR ( L"ScrollingText" ) == ParamStr )
		{
			Tracking::CTrackObject*	pTracking = m_pTracking;
			if ( NULL == pTracking )
			{
				if ( m_oTrackManager.IsEnabled () )
				{
					pTracking = m_oTrackManager.GetTrack ( 0 );
				}
			}

			Tracking::CTrackTextRotatable* pTrack = dynamic_cast < Tracking::CTrackTextRotatable* > ( pTracking );
			if ( NULL != pTrack )
			{
				pTrack->SetScrollingText ( ( ParamValue.boolVal == VARIANT_TRUE ) ? true : false  );
			}

			return S_OK;
		}

		*/
	
		if ( CComBSTR ( _T("SetSectorRationRange") ) == ParamName )
			return SetSectorRationRange ( ParamValue.bstrVal );

#ifdef _USE_NEW_SCHEME_RENDER_

		if ( CComBSTR ( "SetRenderModes" ) == ParamName )
		{
			if ( ImageRendering::Serialize::XmlViewManagerSettings::SetRenderModes ( m_oRenderManager, ParamValue.bstrVal ) )
			{
				if ( IsWindow ( ) )
				{
					InvalidateRect ( NULL, TRUE );
				}
			}
			return S_OK;
		}

		if ( CComBSTR ( "SetGridOptions" ) == ParamName )
		{
			if ( ImageRendering::Serialize::XmlViewManagerSettings::SetGridOptions ( m_oRenderManager, ParamValue.bstrVal ) )
			{
				if ( IsWindow ( ) )
				{
					InvalidateRect ( NULL, TRUE );
				}
			}
			return S_OK;
		}

		if ( CComBSTR ( "SetRullers" ) == ParamName )
		{
			if ( ImageRendering::Serialize::XmlViewManagerSettings::SetRullers ( m_oRenderManager, ParamValue.bstrVal ) )
			{
				if ( IsWindow ( ) )
				{
					InvalidateRect ( NULL, TRUE );
				}
			}

			return S_OK;
		}

		if ( m_bEnableCustomizeScheme )
		{
			if ( ImageRendering::EnumerateViewProperties::SetProperty ( &m_oRenderManager, ParamStr, ParamValue ) )
				return S_OK;
		}
#endif

		return S_OK;
	}

	STDMETHOD ( IImageView5::GetAdditionalParam )	( BSTR ParamName,	VARIANT* ParamValue )
	{
		CComBSTR StrParam = ParamName;

		if ( CComBSTR ( L"CARETX" ) == StrParam )
		{
			ParamValue->vt		=	VT_R8;
			ParamValue->dblVal	=	0.0;


			Tracking::CTrackObject*	pTracking = m_pTracking;
			if ( NULL == pTracking )
			{
				if ( m_oTrackManager.IsEnabled () )
				{
					pTracking = m_oTrackManager.GetTrack ( 0 );
				}
			}

			if ( NULL != pTracking )
			{
				if ( pTracking->IsTrackTextRotatable () )
				{
					Tracking::CTrackObjectTextRotatable* pTrackText = dynamic_cast <Tracking::CTrackObjectTextRotatable*> ( m_pTracking );
					if ( NULL != pTrackText )
					{
						ParamValue->dblVal	=	pTrackText->GetCaretX ();
					}
				}
			}

			return S_OK;
		}

		if ( CComBSTR ( L"CARETY" ) == StrParam )
		{
			ParamValue->vt		=	VT_R8;
			ParamValue->dblVal	=	0.0;

			Tracking::CTrackObject*	pTracking = m_pTracking;
			if ( NULL == pTracking )
			{
				if ( m_oTrackManager.IsEnabled () )
				{
					pTracking = m_oTrackManager.GetTrack ( 0 );
				}
			}

			if ( NULL != pTracking )
			{
				if ( pTracking->IsTrackTextRotatable () )
				{
					Tracking::CTrackObjectTextRotatable* pTrackText = dynamic_cast <Tracking::CTrackObjectTextRotatable*> ( pTracking );
					if ( NULL != pTrackText )
					{
						ParamValue->dblVal	=	AspectXAdd ( pTrackText->GetCaretY () );
					}
				}
			}

			return S_OK;
		}

		if ( CComBSTR ( L"GetScreenImage" ) == StrParam )
		{
			return GetScreenImage ( ParamValue );
		}

		if ( CComBSTR ( L"TrackingColors" ) == StrParam  )
		{
			(*ParamValue).lVal = (LONG)m_nTrackingColorScheme;

			return S_OK;
		}

		if ( CComBSTR ( L"TrackChangeType" ) == StrParam )
		{
			POINT point;
			
			::GetCursorPos ( &point);
			::ScreenToClient ( m_hWnd, &point );

			int nPointStatus		=	Tracking::c_nTrackPointStatusNone;
			int nTrackChangeStatus	=	Tracking::c_nTrackChangeStatusNone;

			if ( m_pTracking )
			{
				m_pTracking->OnSetCursor(point.x, point.y, nPointStatus, nTrackChangeStatus);
			}

			(*ParamValue).lVal		=	(LONG)nTrackChangeStatus;

			return S_OK;
		}

		if ( CComBSTR ( L"TrackTextRotatable" ) == StrParam )
		{
			Tracking::CTrackObject*	pTracking = m_pTracking;

			if ( NULL != pTracking )
			{
				if ( pTracking->IsTrackTextRotatable () )
				{
					Tracking::CTrackObjectTextRotatable* pTrackText = dynamic_cast <Tracking::CTrackObjectTextRotatable*> ( pTracking );
					//Tracking::CTrackTextRotatable* pTrackText = dynamic_cast <Tracking::CTrackTextRotatable*> ( pTracking );
					if ( NULL != pTrackText )
					{
						double dAngle = Geometry::GetAngle ( pTracking->GetAt(9).dX, pTracking->GetAt(9).dY, pTracking->GetAt(8).dX, pTracking->GetAt(8).dY ) + M_PI_2;

						Structures::RECTD TextRC = pTrackText->GetUnrotatableRect ( dAngle );	
						TextRC.Normalize();

						dAngle = grad ( ((dAngle <= 0 )? dAngle + 2.0 * M_PI: dAngle));

						SAFEARRAYBOUND rgsab[1];
						rgsab[0].lLbound	= 0;
						rgsab[0].cElements	= 4; 
						SAFEARRAY *psaCmd = SafeArrayCreate(VT_R8, 1, rgsab);

						ParamValue->vt = VT_R8 | VT_ARRAY;
						ParamValue->parray = psaCmd;

						*(double*)(psaCmd->pvData)			= AspectXAdd ( TextRC.left ); 
						*((double*)psaCmd->pvData + 1)		= AspectYAdd ( TextRC.top ); 
						*((double*)psaCmd->pvData + 2)		= AspectXAdd ( TextRC.right ); 
						*((double*)psaCmd->pvData + 3)		= AspectYAdd ( TextRC.bottom );
						//*((double*)psaCmd->pvData + 4)	= dAngle;
						//*((double*)psaCmd->pvData + 5)	= pTrackText->GetStickAngle ();
					}
				}
			}

			return S_OK;
		}

		/*

		if ( CComBSTR ( L"GetTrackController" ) == StrParam )
		{
			CTrackController* pController	=	static_cast<CTrackController*> ( (void*)m_pTrackController );
			if ( NULL != pController )
			{
				Tracking::CTrackObject*	pTracking = m_pTracking;
				if ( NULL == pTracking )
				{
					if ( m_oTrackManager.IsEnabled () )
					{
						pTracking = m_oTrackManager.GetTrack ( 0 );
					}
				}

				pController->SetTracking ( pTracking );
				pController->SetSizeParams ( m_dAspect, m_nAspectWidth, m_nAspectHeight, m_nWidth, m_nHeight );

				ParamValue->vt		=	VT_UNKNOWN;
				m_pTrackController->QueryInterface ( &ParamValue->punkVal );
			
				return S_OK;
			}
			
			ParamValue->vt		=	VT_UNKNOWN;
			ParamValue->punkVal	=	NULL;

			return S_OK;
		}

		if ( CComBSTR ( L"GetTextTrackController" ) == StrParam )
		{
			CTextController* pController = static_cast<CTextController*>((void*)m_pTextController);
			if ( NULL != pController )
			{
				Tracking::CTrackObject*	pTracking = m_pTracking;
				if ( NULL == pTracking )
				{
					if ( m_oTrackManager.IsEnabled () )
					{
						pTracking = m_oTrackManager.GetTrack ( 0 );
					}
				}

				pController->SetTracking ( pTracking );

				ParamValue->vt = VT_UNKNOWN;
				m_pTextController->QueryInterface ( &ParamValue->punkVal );
			}

			return S_OK;
		}

		*/

#ifdef _USE_NEW_SCHEME_RENDER_
		
		if ( m_bEnableCustomizeScheme )
		{
			ImageRendering::EnumerateViewProperties::GetProperty ( &m_oRenderManager, StrParam, ParamValue );
		}
#endif
		return S_OK;
	}

private:

	HRESULT SetBounding				( BSTR XmlString )
	{
		CXmlNode oXmlNode;
		if ( oXmlNode.FromXmlString ( CString ( XmlString ) ) )
		{
			long SelectTrack	= _tstol ( oXmlNode.GetAttribute ( _T("track"), _T("0") ) );
			long EnableMinBound	= _tstol ( oXmlNode.GetAttribute ( _T("enableminbound") ) );
			long EnableMaxBound	= _tstol ( oXmlNode.GetAttribute ( _T("enablemaxbound") ) );

			double WidthMin		= _tstof ( oXmlNode.GetAttribute ( _T("widthmin") ) );
			double WidthMax		= _tstof ( oXmlNode.GetAttribute ( _T("widthmax") ) );
			double HeightMin	= _tstof ( oXmlNode.GetAttribute ( _T("heightmin") ) );
			double HeightMax	= _tstof ( oXmlNode.GetAttribute ( _T("heightmax") ) );

			if ( NULL != m_pTracking )
			{
				m_pTracking->SetBoundMin ( WidthMin, HeightMin );
				m_pTracking->SetBoundMax ( WidthMax, HeightMax );

				m_pTracking->EnableMinBound ( ( 0 == EnableMinBound ) ? false : true );
				m_pTracking->EnableMaxBound ( ( 0 == EnableMaxBound ) ? false : true );

				return S_OK;
			}

			Tracking::CTrackObject*	pTracking = NULL;
			if ( m_oTrackManager.IsEnabled () )
			{
				pTracking = m_oTrackManager.GetTrack ( SelectTrack );
				if ( NULL != pTracking )
				{
					pTracking->SetBoundMin ( WidthMin, HeightMin );
					pTracking->SetBoundMax ( WidthMax, HeightMax );

					pTracking->EnableMinBound ( ( 0 == EnableMinBound ) ? false : true );
					pTracking->EnableMaxBound ( ( 0 == EnableMaxBound ) ? false : true );
				}
			}
		}

		return S_OK;
	}

	HRESULT SetSticking				( BSTR XmlString )
	{
		CXmlNode oXmlNode;
		if ( oXmlNode.FromXmlString ( CString ( XmlString ) ) )
		{
			///
			/// <sticking track="0" virtualmode="1" realmode="0" cellsize="10" align_center="0"/>
			///

			long SelectTrack	=	_tstol ( oXmlNode.GetAttribute ( _T("track"),			_T("0") ) );
			long VirtualMode	=	_tstol ( oXmlNode.GetAttribute ( _T("virtualmode"),		_T("0") ) );
			long RealMode		=	_tstol ( oXmlNode.GetAttribute ( _T("realmode"),		_T("0") ) );
			long AlignCenter	=	_tstol ( oXmlNode.GetAttribute ( _T("align_center"),	_T("0") ) );
			
			long StickingLeft	=	_tstol ( oXmlNode.GetAttribute ( _T("side_left"),		_T("0") ) );
			long StickingTop	=	_tstol ( oXmlNode.GetAttribute ( _T("side_top"),		_T("0") ) );
			long StickingRight	=	_tstol ( oXmlNode.GetAttribute ( _T("side_right"),		_T("0") ) );
			long StickingBottom	=	_tstol ( oXmlNode.GetAttribute ( _T("side_bottom"),		_T("0") ) );

			long UnitSize		=	_tstof ( oXmlNode.GetAttribute ( _T("cellsize"),		_T("0") ) );

			Tracking::CTrackObject*	pTracking = m_pTracking;
			if ( NULL == pTracking )
			{
				if ( m_oTrackManager.IsEnabled () )
				{
					pTracking = m_oTrackManager.GetTrack ( SelectTrack );
				}
			}

			if ( NULL != pTracking )
			{
				m_oTrackSticking.SetUnitSize ( UnitSize );
				m_oTrackSticking.SetVirtualMode ( ( VirtualMode )	? true : false );
				m_oTrackSticking.SetAlign ( 0.0, 0.0 );
				m_oTrackSticking.SetRoundSticking ( 
					( StickingLeft )	? true : false,
					( StickingTop )		? true : false,
					( StickingRight )	? true : false,
					( StickingBottom )	? true : false );


				//pTracking->GetTrackSticking ().SetUnitSize ( UnitSize );
				//pTracking->GetTrackSticking ().SetVirtualMode ( ( VirtualMode )	? true : false );
				//pTracking->GetTrackSticking ().SetAlign ( 0.0, 0.0 );
				//pTracking->GetTrackSticking ().SetRoundSticking ( 
				//	( StickingLeft )	? true : false,
				//	( StickingTop )		? true : false,
				//	( StickingRight )	? true : false,
				//	( StickingBottom )	? true : false );

				if ( AlignCenter )
				{
					m_oTrackSticking.SetAlign ( /*UnitSize -*/ (double) ( (m_nWidth/2) % UnitSize ),/* UnitSize */ (double) ( (m_nHeight/2) % UnitSize ) );
					//pTracking->GetTrackSticking ().SetAlign ( /*UnitSize -*/ (double) ( (m_nWidth/2) % UnitSize ),/* UnitSize */ (double) ( (m_nHeight/2) % UnitSize ) );
				}

				pTracking->GetTrackSticking ()	=	m_oTrackSticking;
			}
		}

		return S_OK;
	}


	HRESULT SetImage				( IUnknown* pInterface )
	{
		if (NULL == pInterface)
			return S_OK;

		MediaCore::IAVSUncompressedVideoFrame* pFrame = NULL;
		pInterface->QueryInterface(MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pFrame);

		BYTE* pPixels = NULL;
		long nWidth = 0;
		long nHeight = 0;
		BOOL bIsOK = TRUE;

		BOOL bIsResize = TRUE;
		BOOL bIsFlip = FALSE;

		BOOL bIsReleaseEndOnly = FALSE;

		if (NULL != pFrame)
		{
			long lColorspace = pFrame->ColorSpace;
			if (lColorspace != CSP_BGRA && lColorspace != (CSP_BGRA | CSP_VFLIP))
			{
				RELEASEINTERFACE(pFrame);
				if (!ImageStudioUtils::MediaDataToBGRAMediaData(&pInterface, pFrame, FALSE))
				{
					RELEASEINTERFACE(pFrame);
					bIsOK = FALSE;
					return S_FALSE;
				}
				if (NULL != pFrame)
				{
					lColorspace = pFrame->ColorSpace;
					bIsReleaseEndOnly = TRUE;
				}
			}

			pFrame->get_Buffer(&pPixels);
		
			bIsFlip = !( lColorspace & CSP_VFLIP );

			if ( m_bEnableCustomizeScheme )
			{
				bIsFlip = !bIsFlip;
			}

			long nAspectX = 0;
			long nAspectY = 0;

			pFrame->get_Width(&nWidth);
			pFrame->get_Height(&nHeight);

			pFrame->get_AspectRatioX(&nAspectX);
			pFrame->get_AspectRatioY(&nAspectY);
			
			/*
#ifdef _DEBUG
			CLSID ImageGUID;
			ImageRendering::ImageUtils::GetEncoderCLSID ( L"image/png", &ImageGUID );
			Gdiplus::Bitmap SetBitmap ( nWidth, nHeight, 4 * nWidth, PixelFormat32bppARGB, pPixels );

			static long i = 0;
			i++;
			wchar_t buffer [256];
			wsprintfW ( buffer, L"C:\\SetImage_%i.png", i);
			SetBitmap.Save ( buffer , &ImageGUID );
#endif
			*/
			
			/*
#ifdef _DEBUG

			CString String; String.Format ( 
				_T("ImageWidth : %f, ImageHeight : %f, ImageAspectX : %f, ImageAspectY : %f"), 
				(float)nWidth, (float)nHeight, (float)nAspectX, (float)nAspectY );

			::MessageBox ( NULL, String, _T("Image info"), MB_ICONINFORMATION ); 
#endif
			*/


			if (nAspectX > 0 && nAspectY > 0)
				m_dAspect = (double)nAspectX / nAspectY;
			else
				m_dAspect = -1.0;

			if (m_dAspect == (((double)nWidth) / nHeight))
			{
				bIsResize = FALSE;
			}

			if (!bIsReleaseEndOnly)
			{
				RELEASEINTERFACE(pFrame);
			}

			// TODO :
			//if ((NULL == pPixels) || (nWidth < 2 || nHeight < 2))
			if ( NULL == pPixels )
			{
				bIsOK = FALSE;
			}
		}
		else 
		{
			bIsOK = FALSE;
		}

		if (!bIsOK)
		{
			RELEASEINTERFACE(pFrame);
			return S_OK;
		}

		Clear();

		// save image parameters
		m_nWidth = nWidth;
		m_nHeight = nHeight;

		// save non-aspected image
		m_nAspectWidth = m_nWidth;
		m_nAspectHeight = m_nHeight;

		// apply aspect correction
		if (m_dAspect >= 0.001 && bIsResize)
		{
			// apply aspect correction to the image
			double dAspectZoom = min(m_nWidth/(m_nHeight*m_dAspect), 1.0);

			// compute aspected image width and height
			int nAspectWidth = (int)(dAspectZoom*m_nHeight*m_dAspect);
			int nAspectHeight = (int)(dAspectZoom*m_nHeight);

			// allocate memory for new mask
			m_pPixels = new BYTE[4*m_nWidth*m_nHeight];

			// check for allocated memory
			if (!m_pPixels)
			{
				Clear();
				RELEASEINTERFACE(pFrame);
				return S_OK;
			}

			// variables
			int nSrcX, nSrcY, nSrcIndex = 0;
			int nDstX, nDstY, nDstIndex = 0;

			// resize image from [m_nWidth,m_nHeight] to [nAspectWidth,nAspectHeight]
			for (nDstY = 0; nDstY < nAspectHeight; ++nDstY)
			{
				nSrcY = (m_nHeight - 1)*nDstY/(nAspectHeight - 1);

				nSrcIndex = 4*nSrcY*m_nWidth;

				for (nDstX = 0; nDstX < nAspectWidth; ++nDstX, nDstIndex += 4)
				{
					nSrcX = (m_nWidth - 1)*nDstX/(nAspectWidth - 1);
					nSrcX = nSrcIndex + 4*nSrcX;

					m_pPixels[nDstIndex + 0] = pPixels[nSrcX + 0];
					m_pPixels[nDstIndex + 1] = pPixels[nSrcX + 1];
					m_pPixels[nDstIndex + 2] = pPixels[nSrcX + 2];
					m_pPixels[nDstIndex + 3] = pPixels[nSrcX + 3];
				}
			}

			// save image size
			m_nWidth	= nAspectWidth;
			m_nHeight	= nAspectHeight;

			if (bIsFlip)
			{
				BGRA_Flip(m_pPixels, m_nWidth, m_nHeight);
			}
		}
		else
		{
			// allocate memory for new mask
			m_pPixels = new BYTE[4*m_nWidth*m_nHeight];

			// check for allocated memory
			if (!m_pPixels)
			{
				Clear();
				RELEASEINTERFACE(pFrame);
				return S_OK;
			}

			if (bIsFlip)
			{
				BYTE* pLineDst = m_pPixels;
				BYTE* pLineSrc = pPixels;

				int nCountLines = m_nHeight;
				int nSizeLine = 4 * m_nWidth;
				for (int nIndex = 0; nIndex < nCountLines; ++nIndex)
				{
					pLineSrc = pPixels + (nCountLines - nIndex - 1) * nSizeLine;
					memcpy(pLineDst, pLineSrc, nSizeLine);
					pLineDst += nSizeLine;
				}
			}
			else
			{
				// retrieve data from safe array
				memcpy(m_pPixels, pPixels, 4*m_nWidth*m_nHeight);
			}				
		}

		// remove mask
		if (m_pMask)
		{
			delete[] m_pMask;
			m_pMask = 0;
		}

		RELEASEINTERFACE(pFrame);

#ifdef _USE_NEW_SCHEME_RENDER_

		m_oRenderManager.SetImage ( m_pPixels, m_nWidth, m_nHeight );
#endif

		// update window
		return S_OK;
	}


	HRESULT SetSectorRationRange	( BSTR XmlString )
	{
		CXmlNode oXmlNode;
		if ( oXmlNode.FromXmlString ( CString ( XmlString ) ) )
		{
			long Enable			= _tstol ( oXmlNode.GetAttribute ( _T("enable"),	_T("0") ) );
			double MinRange		= _tstof ( oXmlNode.GetAttribute ( _T("min"),		_T("0.0") ) );
			double MaxRange		= _tstof ( oXmlNode.GetAttribute ( _T("max"),		_T("0.95") ) );
			long SelectTrack	= _tstol ( oXmlNode.GetAttribute ( _T("track"),		_T("0") ) );

			Tracking::CTrackObject*	pTracking = m_pTracking;
			if ( NULL == pTracking )
			{
				if ( m_oTrackManager.IsEnabled () )
				{
					pTracking = m_oTrackManager.GetTrack ( SelectTrack );
				}
			}

			if ( NULL != pTracking )
			{
				if ( pTracking->IsTrackSector () )
				{
					CTrackObjectSector* pSector = static_cast <CTrackObjectSector*>  ( pTracking );
					if ( NULL != pSector )
					{
						pSector->SetRangeRation ( ( Enable ) ? true : false, MinRange, MaxRange );

					}
				}
			}
		}

		return S_OK;
	}

	HRESULT GetScreenImage			( VARIANT* pValue )
	{
		pValue->vt		=	VT_UNKNOWN;
		pValue->punkVal	=	NULL;

		MediaCore::IAVSUncompressedVideoFrame* pImage = NULL;
		if ( SUCCEEDED ( CoCreateInstance ( MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pImage ) ) )
		{
			pImage->put_ColorSpace		( CSP_BGRA | CSP_VFLIP );
			pImage->put_Height			( m_oDoubleBuffer.GetRect().GetHeight() );
			pImage->put_Width			( m_oDoubleBuffer.GetRect().GetWidth() );
			pImage->put_AspectRatioX	( 0 );
			pImage->put_AspectRatioY	( 0 );

			pImage->SetDefaultStrides	( );
			pImage->AllocateBuffer		( -1 );

			LONG PlaneSize = 0;			pImage->get_BufferSize ( &PlaneSize );
			BYTE* pPlaneData = NULL;	pImage->get_Plane ( 0, &pPlaneData );

#ifdef _USE_NEW_SCHEME_RENDER_

			if ( m_bEnableCustomizeScheme )
			{
				if ( m_oRenderManager.GetScreenImage ( pPlaneData ) )
				{
					pImage->put_ColorSpace	( CSP_BGRA );
					pImage->QueryInterface	( &pValue->punkVal );
					pImage->Release ();
				}

				return S_OK;
			}
#endif
			memcpy ( pPlaneData, m_oDoubleBuffer.GetBackBitmapPixels(), PlaneSize );

			pImage->QueryInterface ( &pValue->punkVal );
			pImage->Release ();

			return S_OK;
		}

		return S_OK;
	}

	
	HRESULT SetNoResizeModeRFAT		( BSTR XmlString )
	{
		m_bSetNoResizeRFAT = FALSE;

		if ( m_oTrackManager.IsEnabled () )
		{
			CXmlNode oXmlNode;
			if ( oXmlNode.FromXmlString ( CString ( XmlString ) ) )
			{
				long SelectTrack	= _tstol ( oXmlNode.GetAttribute ( _T("track"), _T("0") ) );
				m_bSetNoResizeRFAT	= (BOOL)_tstol ( oXmlNode.GetAttribute ( _T("noresize"), _T("0") ) );

				Tracking::CTrackObject*	pTracking = m_oTrackManager.GetTrack ( SelectTrack );
				if ( NULL != pTracking )
				{
					if ( pTracking->IsTrackRectangleRotatableFixedAspect () )
					{
						Tracking::CTrackObjectRectangleRotatableFixedAspect* pRFAT = (Tracking::CTrackObjectRectangleRotatableFixedAspect*)pTracking;

						if ( pRFAT )
						{
							pRFAT->SetNoResize ( m_bSetNoResizeRFAT );
						}
					}
				}				
			}
		}

		return S_OK;
	}

	HRESULT SetSizesModeRFAT		( BSTR XmlString )
	{
		m_bSizesResizeRFAT = FALSE;

		if ( m_oTrackManager.IsEnabled () )
		{
			CXmlNode oXmlNode;
			if ( oXmlNode.FromXmlString ( CString ( XmlString ) ) )
			{
				long SelectTrack	= _tstol ( oXmlNode.GetAttribute ( _T("track"), _T("0") ) );
				m_bSizesResizeRFAT	= (BOOL)_tstol ( oXmlNode.GetAttribute ( _T("sizesmode"), _T("0") ) );

				Tracking::CTrackObject*	pTracking = m_oTrackManager.GetTrack ( SelectTrack );
				if ( NULL != pTracking )
				{
					if ( pTracking->IsTrackRectangleRotatableFixedAspect () )
					{
						Tracking::CTrackObjectRectangleRotatableFixedAspect* pRFAT = (Tracking::CTrackObjectRectangleRotatableFixedAspect*)pTracking;

						if ( pRFAT )
						{
							pRFAT->SetSizesMode ( m_bSizesResizeRFAT );
						}
					}
				}				
			}
		}

		return S_OK;
	}


	void RenderBackBitmapPixels		( LONG Width, LONG Height, BYTE* pSource, HDC hDC )
	{
		if ( NULL == pSource || 0 >= Width || 0 >= Height || NULL == hDC )
			return;

		BITMAPINFO oBitmapInfo;

		oBitmapInfo.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
		oBitmapInfo.bmiHeader.biHeight			= - Height;						// DO NOT MODIFITY
		oBitmapInfo.bmiHeader.biWidth			= Width;
		oBitmapInfo.bmiHeader.biPlanes			= 1;
		oBitmapInfo.bmiHeader.biBitCount		= 32;
		oBitmapInfo.bmiHeader.biCompression		= BI_RGB;
		oBitmapInfo.bmiHeader.biSizeImage		= 0;
		oBitmapInfo.bmiHeader.biXPelsPerMeter	= 1;
		oBitmapInfo.bmiHeader.biYPelsPerMeter	= 1;
		oBitmapInfo.bmiHeader.biClrUsed			= 0;
		oBitmapInfo.bmiHeader.biClrImportant	= 0;

		SetDIBitsToDevice ( hDC, 0, 0, Width, Height, 0, 0, 0, Height, pSource, &oBitmapInfo, DIB_RGB_COLORS );

		/*
		CLSID ImageGUID;
		ImageRendering::ImageUtils::GetEncoderCLSID ( L"image/png", &ImageGUID );
		Gdiplus::Bitmap SetBitmap ( Width, Height, 4 * Width, PixelFormat32bppARGB, pSource );

		static long i = 0;
		i++;
		wchar_t buffer [256];
		wsprintfW ( buffer, L"C:\\SetImage_%i.png", i);
		SetBitmap.Save ( buffer , &ImageGUID );
		*/
	}

#ifdef _DEBUG

	typedef struct
	{
		BITMAPINFOHEADER bmiHeader;
		RGBQUAD bmiColors[256];
	}
	BITMAPINFOBIG;

	inline void Out2BmpFromHDC(HDC hDC, int iLineOffset, int iImageHeght, char *OutFileName)
	{ 
		BITMAPFILEHEADER FileHeader;
		BITMAPINFOBIG InfoHeader;
		HBITMAP bmpDevice = NULL;
		char *pImageBits = NULL;
		int iPaleteSize = 0, iSizeImage = 0;
		double dTemp;

		if (hDC==NULL) return;
		if (OutFileName==NULL) OutFileName = "Result.bmp";

		bmpDevice = (HBITMAP)GetCurrentObject(hDC,OBJ_BITMAP);
		if (bmpDevice==NULL) return;
		memset(&InfoHeader, 0, sizeof(BITMAPINFOBIG));
		InfoHeader.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		if (GetDIBits(hDC, bmpDevice, 0, 0, NULL, (LPBITMAPINFO)&InfoHeader, DIB_RGB_COLORS)==0) return;
		//—жатие не нужно
		InfoHeader.bmiHeader.biCompression = BI_RGB;
		//ѕерерасчет размера исходного изображени€ (если было со сжатием)
		dTemp = (double)(InfoHeader.bmiHeader.biBitCount*InfoHeader.bmiHeader.biWidth)/8.;
		iSizeImage = (int)dTemp;
		if ((dTemp-iSizeImage)>0) iSizeImage++;
		if ((iSizeImage%4)>0) iSizeImage += (4-(iSizeImage%4));
		InfoHeader.bmiHeader.biSizeImage = iSizeImage*InfoHeader.bmiHeader.biHeight;

		// орректна€ установка входных параметров
		if ((iImageHeght<=0)||(iImageHeght>InfoHeader.bmiHeader.biHeight))
		{
			iLineOffset = 0;
			iImageHeght = InfoHeader.bmiHeader.biHeight;
		}
		if ((iLineOffset+iImageHeght)>InfoHeader.bmiHeader.biHeight)
			iLineOffset = InfoHeader.bmiHeader.biHeight-iImageHeght;
		if (iLineOffset<0) iLineOffset = 0;

		//–асчет размера результирующего изображени€
		iSizeImage *= iImageHeght;

		//ќпределение размеров таблицы цветов
		iPaleteSize = InfoHeader.bmiHeader.biClrUsed*4;

		//Ќастройка полей FileHeader
		memset(&FileHeader, 0, sizeof(BITMAPFILEHEADER));
		*((char*)(&FileHeader.bfType)+0) = 'B';
		*((char*)(&FileHeader.bfType)+1) = 'M';
		FileHeader.bfSize    = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+iPaleteSize+iSizeImage;
		FileHeader.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+iPaleteSize;

		//¬ыделение пам€ти под данные
		pImageBits =  new  char [iSizeImage];
		if (pImageBits)
		{
			//”точнение начального смещени€
			iLineOffset = InfoHeader.bmiHeader.biHeight - iLineOffset - iImageHeght;
			//«аполнение данных
			if (GetDIBits(hDC, bmpDevice, iLineOffset, iImageHeght, pImageBits, (LPBITMAPINFO)&InfoHeader, DIB_RGB_COLORS)==iImageHeght)
			{
				//‘ормирование файла
				FILE *OutFile = fopen(OutFileName, "wb");
				if (OutFile)
				{
					//Ќастройка полей InfoHeader под результирующую картинку
					InfoHeader.bmiHeader.biHeight = iImageHeght;
					InfoHeader.bmiHeader.biSizeImage = iSizeImage;
					//«апись заголовка файла
					fwrite(&FileHeader, sizeof(BITMAPFILEHEADER), 1, OutFile);
					//«апись заголовка информации
					fwrite(&InfoHeader, sizeof(BITMAPINFOHEADER), 1, OutFile);
					//«апись данных палитры (если она есть)
					if (iPaleteSize>0) fwrite(&InfoHeader.bmiColors, iPaleteSize, 1, OutFile);
					//«апись данных изображени€
					fwrite(pImageBits, iSizeImage, 1, OutFile);
					//‘айл сформирован
					fclose(OutFile);
				}
			}

			//”даление данных
			delete [] pImageBits;
		}
	}

#endif

public:

	int										m_nLMButtonState;
	int										m_nRMButtonState;
	int										m_nMMButtonState;

	// image attributes
	int										m_nWidth;
	int										m_nHeight;
	BYTE*									m_pPixels;
	BYTE*									m_pMask;

	BYTE*									m_pAlphaPixels;

	// additional helpers
	BOOL									m_bAutoRefresh;

	// formatting
	BOOL									m_bShowBorder;
	int										m_nWindowStyle;

	int										m_nMaskStyle;
	int										m_nMaskColor;

	int										m_nBackStyle;
	int										m_nBackColor1;
	int										m_nBackColor2;

	int										m_nPointer;

	// preview attributes
	double									m_dCenterX;
	double									m_dCenterY;
	double									m_dZoom;
	double									m_dAspect;
	int										m_nAspectWidth;
	int										m_nAspectHeight;

	// handle to window for timer messages 
	HWND									m_hWndTimer;

	// tracking
	int										m_nTrackingColorScheme;

	Tracking::CTrackPainter					m_oTrackingPainter;
	Tracking::CTrackObject*					m_pTracking;

	// rendering
	Windows::CDoubleBuffer					m_oDoubleBuffer;

	// gdi+
	CGdiPlusInit							m_oGdiPlusInit;
	// MemoryUtils::CMemoryUtils			m_oMemoryUtils;

	TRACKMOUSEEVENT							tme;

	Core::CTrackManagerImp					m_oTrackManager;

	bool									m_bUseExternalRender;

	long									m_nInterpolationMode;
	//Core::CheckFeatures					m_oCheckFeatures;

	MediaCore::IAVSUncompressedVideoFrame*	m_pImageTracking;		// 
	LONG									m_nImageTrackingAlpha;	//	255 - изображение не мен€ем 0 - не рисуем

	Gdiplus::Bitmap*						m_pBitmap;
	Gdiplus::Graphics*						m_pGrahics;
	bool									m_bImageTrackingRebuild;

	// бльIирьAБE ъCрисовкБE
	bool									m_bEnableOnPaintBlock;

#ifdef _USE_NEW_SCHEME_RENDER_
	bool									m_bEnableCustomizeScheme;	// исБEБEзуем ыMвую схемБEъCрисовкБE
	ImageRendering::CViewManager			m_oRenderManager;
	
	CTrackSticking							m_oTrackSticking;
#endif

	//CComPtr < ITextController >			m_pTextController;
	//CComPtr < ITrackController >			m_pTrackController;

	bool									m_bEnableDrawText;			// рисуем или не рисуем текст
	bool									m_bResizeShapeToFitText;

	bool									m_bUseStudioTextRender;
	ImageStudioTextRender					m_oStudioTextRender;		//
	ImageStudioTextEffect					m_oStudioTextEffect;
	CSaveSurface							m_oSaveSurface;

	BOOL									m_bSetNoResizeRFAT;			// 
	BOOL									m_bSizesResizeRFAT;
};
