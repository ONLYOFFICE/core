#pragma once
	
#include "resource.h"
#include <atlctl.h>
#include <registration.h>
#include <windowsex.h>
#include <messages.h>
#include <tracking.h>
#include "constants.h"
#include <imagestudioutils.h>

[object, uuid("B78D5241-08FB-43ef-A06B-F57D2DBB1F77"), dual, pointer_default(unique)]

__interface IImageView3 : public IDispatch
{
	[id(101)] HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(102)] HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);
	
	[id(201)] HRESULT SetImage([in, satype("BYTE")] SAFEARRAY** Image);
	[id(202)] HRESULT SetMediaData(IUnknown* pInterface);
	
	[id(211)] HRESULT SetMask([in, satype("BYTE")] SAFEARRAY** Image);
	
	[id(251)] HRESULT Refresh();
	[id(252)] HRESULT GetScreen([out, satype("BYTE")] SAFEARRAY** Image, [out, retval] VARIANT_BOOL* Success);
	
	[id(301), propget] HRESULT AutoRefresh([out, retval] VARIANT_BOOL* Allow);
	[id(301), propput] HRESULT AutoRefresh([in] VARIANT_BOOL Allow);

	[id(401)] HRESULT ZoomIn();
	[id(402)] HRESULT ZoomOut();
	[id(403)] HRESULT ZoomToFit();
	[id(404)] HRESULT ShrinkToFit();

	[id(501), propget] HRESULT viewCenterX([out, retval] double* CenterX);
	[id(501), propput] HRESULT viewCenterX([in] double CenterX);
	[id(502), propget] HRESULT viewCenterY([out, retval] double* CenterY);
	[id(502), propput] HRESULT viewCenterY([in] double CenterX);
	[id(503), propget] HRESULT viewZoom([out, retval] double* Zoom);
	[id(503), propput] HRESULT viewZoom([in] double Zoom);
	[id(504), propget] HRESULT viewAspect([out, retval] double* Aspect);
	[id(504), propput] HRESULT viewAspect([in] double Aspect);

	[id(511), propget] HRESULT formatShowBorder([out, retval] VARIANT_BOOL* Show);
	[id(511), propput] HRESULT formatShowBorder([in] VARIANT_BOOL Show);
	[id(512), propget] HRESULT formatWindowStyle([out, retval] long* Style);
	[id(512), propput] HRESULT formatWindowStyle([in] long Style);
	[id(513), propget] HRESULT formatMousePointer([out, retval] long* Pointer);
	[id(513), propput] HRESULT formatMousePointer([in] long Pointer);
	
	[id(521), propget] HRESULT formatMaskStyle([out, retval] long* Style);
	[id(521), propput] HRESULT formatMaskStyle([in] long Style);
	[id(522), propget] HRESULT formatMaskColor([out, retval] OLE_COLOR* Color);
	[id(522), propput] HRESULT formatMaskColor([in] OLE_COLOR Color);
	
	[id(531), propget] HRESULT formatBackStyle([out, retval] long* Style);
	[id(531), propput] HRESULT formatBackStyle([in] long Style);
	[id(532), propget] HRESULT formatBackColor1([out, retval] OLE_COLOR* Color);
	[id(532), propput] HRESULT formatBackColor1([in] OLE_COLOR Color);
	[id(533), propget] HRESULT formatBackColor2([out, retval] OLE_COLOR* Color);
	[id(533), propput] HRESULT formatBackColor2([in] OLE_COLOR Color);
	
	// tracking
	[id(600), propget] HRESULT trackType([out, retval] long* Type);
	[id(601), propget] HRESULT trackColorScheme([out, retval] long* ColorScheme);
	[id(601), propput] HRESULT trackColorScheme([in] long ColorScheme);
	
	[id(701)] HRESULT EndTrack();
	[id(702)] HRESULT BeginTrackPoint([in] double X, [in] double Y);
	[id(703)] HRESULT BeginTrackLine([in] double X1, [in] double Y1, [in] double X2, [in] double Y2);
	[id(704)] HRESULT BeginTrackEllipse([in] double Left, [in] double Top, [in] double Right, [in] double Bottom);
	[id(705)] HRESULT BeginTrackRectangle([in] double Left, [in] double Top, [in] double Right, [in] double Bottom);
	[id(706)] HRESULT BeginTrackRectangleFixedSize([in] double Left, [in] double Top, [in] double Width, [in] double Height);
	[id(707)] HRESULT BeginTrackRectangleFixedAspect([in] double Left, [in] double Top, [in] double Width, [in] double Height);
	[id(708)] HRESULT BeginTrackPolyline([in, satype("double")] SAFEARRAY** Points);
	[id(709)] HRESULT BeginTrackPolygon([in, satype("double")] SAFEARRAY** Points);
	[id(710)] HRESULT BeginTrackPie([in] double Left, [in] double Top, [in] double Right, [in] double Bottom, [in] double StartAngle, [in] double SweepAngle);
	[id(711)] HRESULT BeginTrackSector([in] double Left, [in] double Top, [in] double Right, [in] double Bottom, [in] double StartAngle, [in] double SweepAngle, [in] double Ratio);
	[id(712)] HRESULT BeginTrackSimpleSector([in] double CenterX, [in] double CenterY, [in] double Radius1, [in] double Radius2, [in] double StartAngle, [in] double SweepAngle);
	[id(713)] HRESULT BeginTrackSimplePie([in] double CenterX, [in] double CenterY, [in] double Radius1, [in] double StartAngle, [in] double SweepAngle);
	[id(714)] HRESULT BeginTrackEllipseInRectangle([in] double Left, [in] double Top, [in] double Right, [in] double Bottom);
	[id(715)] HRESULT BeginTrackCircle([in] double CenterX, [in] double CenterY, [in] double Radius);
	[id(716)] HRESULT BeginTrackRectangleRotatable([in] double Left, [in] double Top, [in] double Right, [in] double Bottom, [in] double Angle, [in] double StickAngle);
	[id(717)] HRESULT BeginTrackRectangleRotatableFixedAspect([in] double Left, [in] double Top, [in] double Right, [in] double Bottom, [in] double Angle, [in] double StickAngle);
	[id(718)] HRESULT BeginTrackTextRotatable([in] double Left, [in] double Top, [in] double Right, [in] double Bottom, [in] double Angle, [in] double StickAngle, [in] BSTR inText, [in] BSTR FontName, [in] int FontSize, [in] int FontType, [in] int FontColor);
	[id(719)] HRESULT BeginTrackText([in] double Left, [in] double Top, [in] double Right, [in] double Bottom, [in] BSTR inText, [in] BSTR FontName, [in] int FontSize, [in] int FontType, [in] int FontColor);
	[id(720)] HRESULT BeginTrackTrajectoryPolyline([in, satype("double")] SAFEARRAY** Points);

	[id(801)] HRESULT MoveTrack([in] double dX, [in] double dY);
	
	[id(901)] HRESULT TransformCoordToBitmap(double* X, double* Y);
	[id(902)] HRESULT TransformCoordToControl(double* X, double* Y);
};
	
[dispinterface, uuid("2CBB019D-2A27-406a-BA1A-ACFD768913F2")]
__interface _IImageView3Events
{
	[id(101)] HRESULT MouseDown(long Button, long Shift, double X, double Y);
	[id(102)] HRESULT MouseUp(long Button, long Shift, double X, double Y);
	[id(103)] HRESULT MouseMove(long Button, long Shift, double X, double Y);
	[id(104)] HRESULT MouseDblClick(long Button, long Shift, double X, double Y);

	[id(201)] HRESULT TrackPoint(long Type, double X, double Y);
	[id(202)] HRESULT TrackLine(long Type, double X1, double Y1, double X2, double Y2);
	[id(203)] HRESULT TrackEllipse(long Type, double Left, double Top, double Right, double Bottom);
	[id(204)] HRESULT TrackRectangle(long Type, double Left, double Top, double Right, double Bottom);
	[id(205)] HRESULT TrackRectangleFixedSize(long Type, double X, double Y);
	[id(206)] HRESULT TrackRectangleFixedAspect(long Type, double Left, double Top, double Right, double Bottom);
	[id(207)] HRESULT TrackPolyline(long Type, BYTE* Points);
	[id(208)] HRESULT TrackPolygon(long Type, BYTE* Points);
	[id(209)] HRESULT TrackPie(long Type, double Left, double Top, double Right, double Bottom, double StartAngle, double SweepAngle);
	[id(210)] HRESULT TrackSector(long Type, double Left, double Top, double Right, double Bottom, double StartAngle, double SweepAngle, double Ratio);
	[id(211)] HRESULT TrackSimpleSector(long Type, double CenterX, double CenterY, double Radius1, double Radius2, double StartAngle, double SweepAngle);
	[id(212)] HRESULT TrackSimplePie(long Type, double CenterX, double CenterY, double Radius1, double StartAngle, double SweepAngle);
	[id(213)] HRESULT TrackEllipseInRectangle(long Type, double Left, double Top, double Right, double Bottom);
	[id(214)] HRESULT TrackCircle(long Type, double CenterX, double CenterY, double Radius);
	[id(215)] HRESULT TrackRectangleRotatable(long Type, double Left, double Top, double Right, double Bottom, double Angle);
	[id(216)] HRESULT TrackTextRotatable(long Type, double Left, double Top, double Right, double Bottom, double Angle, BSTR Text);
	[id(217)] HRESULT TrackText(long Type, double Left, double Top, double Right, double Bottom, BSTR Text);
	[id(218)] HRESULT TrackRectangleRotatableFixedAspect(long Type, double Left, double Top, double Right, double Bottom, double Angle);
	[id(219)] HRESULT TrackTrajectoryPolyline(long Type, BYTE* Points);

	[id(501)] HRESULT ChangeBackBuffer( INT_PTR hDC );
};
	
[coclass, uuid("875E3D2E-FAFF-44f6-9FDA-6E01AEA4F8A6"), threading(apartment), vi_progid("AVSImageView.ImageView3"), progid("AVSImageView.ImageView3.2"), version(1.0), event_source(com), support_error_info(IImageView3), registration_script("control.rgs")]
class ATL_NO_VTABLE ImageView3 : 
	public IImageView3,
	public IPersistStreamInitImpl<ImageView3>,
	public IOleControlImpl<ImageView3>,
	public IOleObjectImpl<ImageView3>,
	public IOleInPlaceActiveObjectImpl<ImageView3>,
	public IViewObjectExImpl<ImageView3>,
	public IOleInPlaceObjectWindowlessImpl<ImageView3>,
	public IPersistStorageImpl<ImageView3>,
	public ISpecifyPropertyPagesImpl<ImageView3>,
	public IQuickActivateImpl<ImageView3>,
	public IDataObjectImpl<ImageView3>,
	public CComControl<ImageView3>,
	public Messages::CMessageReceiver
{
public:
	
	__event __interface _IImageView3Events;
		
	DECLARE_VIEW_STATUS(0)
	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE | OLEMISC_CANTLINKINSIDE | OLEMISC_INSIDEOUT | OLEMISC_ACTIVATEWHENVISIBLE | OLEMISC_SETCLIENTSITEFIRST)
		
	BEGIN_PROP_MAP(ImageView3)
		PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
		PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	END_PROP_MAP()
		
	BEGIN_MSG_MAP(ImageView3)
		MESSAGE_RANGE_HANDLER(0, 0xFFFFFFFF, OnMessages)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()
	
public:
	
	// image attributes
	int m_nWidth;
	int m_nHeight;
	BYTE* m_pPixels;
	BYTE* m_pMask;
	
	// additional helpers
	BOOL m_bAutoRefresh;

	// formatting
	BOOL m_bShowBorder;
	int	m_nWindowStyle;

	int m_nMaskStyle;
	int m_nMaskColor;

	int m_nBackStyle;
	int m_nBackColor1;
	int m_nBackColor2;

	int m_nPointer;

	// preview attributes
	double m_dCenterX;
	double m_dCenterY;
	double m_dZoom;
	double m_dAspect;
	int m_nAspectWidth;
	int m_nAspectHeight;

	// handle to window for timer messages 
	//HWND m_hWndTimer;

	// tracking
	int m_nTrackingColorScheme;
	Tracking::CTrackPainter m_oTrackingPainter;
	Tracking::CTrackObject* m_pTracking;

	// rendering
	Windows::CDoubleBuffer m_oDoubleBuffer;

	// gdi+
	CGdiPlusInit m_oGdiPlusInit;
	// MemoryUtils::CMemoryUtils m_oMemoryUtils;

	TRACKMOUSEEVENT tme;
	
public:
	
	LRESULT OnMessages(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// reset handling flag
		bHandled = FALSE;

		// setup mouse cursor if 
		if (uMsg == WM_SETCURSOR)
			bHandled = OnSetCursor();

		if (uMsg == WM_MOUSEMOVE)
		{
			// initialize track mouse event 
			tme.cbSize		= sizeof( TRACKMOUSEEVENT );
			tme.dwFlags		= TME_LEAVE;
			tme.hwndTrack	= m_hWnd;

			TrackMouseEvent( &tme );			
		}

		// handle mouse events - send necessary event to parent
		if (uMsg == WM_LBUTTONDOWN || uMsg == WM_RBUTTONDOWN || uMsg == WM_MBUTTONDOWN ||
			uMsg == WM_LBUTTONUP || uMsg == WM_RBUTTONUP || uMsg == WM_MBUTTONUP ||
			uMsg == WM_LBUTTONDBLCLK || uMsg == WM_RBUTTONDBLCLK || uMsg == WM_MBUTTONDBLCLK ||
			uMsg == WM_MOUSEMOVE)
			OnMouseEvent(uMsg);


		if (uMsg == WM_LBUTTONDBLCLK)
			SetFocus();
		
		// allow trackers to handle its own events
		if (m_pTracking)
			m_pTracking->OnMessages(uMsg, wParam, lParam);

		// handle painting messages
		if (uMsg == WM_ERASEBKGND)
			bHandled = TRUE;
		else if (uMsg == WM_PAINT)
		{
			OnPaint(c_SenderDraw);
			bHandled = FALSE; // не менять - иначе подвисает на постоянном painting'е
		}

		return 0;
	}
	
	BOOL OnPaint(int nSender)
	{
		// check for autorefresh flag
		if (!m_bAutoRefresh && nSender != c_SenderRefresh)
			return FALSE;

		if (!IsWindow())
			return FALSE;

		// retrieve window rectangle and check for valid size
		RECT rect; GetClientRect(&rect);
		if (rect.right - rect.left < 1 || rect.bottom - rect.top < 1)
			return FALSE;

		HDC	hPreviewDC = GetDC();
		m_oDoubleBuffer.Create(hPreviewDC, rect, TRUE);

		// extract last frame (skip mouse tracking case)
		BOOL bScreenValid = IsValid();
		if (nSender != c_SenderTrack)
		{
			int nWidth  = m_oDoubleBuffer.GetRect().GetWidth();
			int nHeight = m_oDoubleBuffer.GetRect().GetHeight();
			BYTE* pPixels = m_oDoubleBuffer.GetBackBitmapPixels();

			if( FALSE == IPP_ExtractRect(nWidth, nHeight, 4, pPixels) )
				bScreenValid = ExtractRect(nWidth, nHeight, 4, pPixels);
			else
				bScreenValid = TRUE;
		}

		// draw the screen
		if (bScreenValid)
		{
			m_oDoubleBuffer.RenderBackBitmapPixels();
			ShowTracking(m_oDoubleBuffer.GetBackDC(), rect);
		}
		else
			Rendering::Gdi::FillRectangle(m_oDoubleBuffer.GetBackDC(), m_nBackColor1, rect);

		ChangeBackBuffer( (INT_PTR)m_oDoubleBuffer.GetBackDC() );

		// show window borders
		ShowWindowBorders(m_oDoubleBuffer.GetBackDC(), rect);

		// flip back buffer to the screen
		m_oDoubleBuffer.Render();

		ReleaseDC(hPreviewDC);

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
			if (!m_pTracking)
				return FALSE;

			int nPointStatus = Tracking::c_nTrackPointStatusNone;
			int nTrackChangeStatus = Tracking::c_nTrackChangeStatusNone;

			m_pTracking->OnSetCursor(point.x, point.y, nPointStatus, nTrackChangeStatus);

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
		else if (nMessage == WM_LBUTTONUP || nMessage == WM_RBUTTONUP || nMessage == WM_MBUTTONUP)
		{
			ReleaseCapture();
			MouseUp(nButton, nShift, AspectXAdd(dX), AspectYAdd(dY));
		}
		else if (nMessage == WM_LBUTTONDBLCLK || nMessage == WM_RBUTTONDBLCLK || nMessage == WM_MBUTTONDBLCLK)
			MouseDblClick(nButton, nShift, AspectXAdd(dX), AspectYAdd(dY));
		else if (nMessage == WM_MOUSEMOVE)
			MouseMove(nButton, nShift, AspectXAdd(dX), AspectYAdd(dY));

		return TRUE;
	}
	
	virtual void ReceiveMessage(Messages::CMessage* pMessage)
	{
		if (!pMessage)
			return;

		if (pMessage->GetType() == Tracking::c_nTrackMessageTrackPointBegin)
			FireEvent(0);
		else if (pMessage->GetType() == Tracking::c_nTrackMessageTrackPointProcess)
			FireEvent(1);
		else if (pMessage->GetType() == Tracking::c_nTrackMessageTrackPointProcessAll)
			FireEvent(1);
		else if (pMessage->GetType() == Tracking::c_nTrackMessageTrackPointEnd)
			FireEvent(2);
		else if (pMessage->GetType() == Tracking::c_nTrackMessageTrackTextEnd)
			FireEvent(4);
		else if (pMessage->GetType() == Tracking::c_nTrackMessageTrackTextBegin)
			FireEvent(5);
		else if (pMessage->GetType() == Tracking::c_nTrackMessageTrackTextChange)
			FireEvent(6);

		OnPaint(c_SenderTrack);
	}
	
public:
	
	ImageView3()
	{
#ifdef _DEBUG
		int flag = _CrtSetDbgFlag ( _CRTDBG_REPORT_FLAG );	// Get current flag
		flag |= _CRTDBG_LEAK_CHECK_DF;						// Turn on leak-checking bit
		flag |= _CRTDBG_CHECK_ALWAYS_DF;					// Turn on CrtCheckMemory
		//	flag |= _CRTDBG_DELAY_FREE_MEM_DF;
		//_CrtSetBreakAlloc ( 4775 );
		_CrtSetDbgFlag(flag);								// Set flag to the new value
#endif
		// initialize gdi+
		m_oGdiPlusInit.Init();

		// setup invalid image parameters
		m_nWidth = 0;
		m_nHeight = 0;
		m_pPixels = 0;
		m_pMask = 0;

		// setup additional helpers
		m_bAutoRefresh = TRUE;

		m_bShowBorder = TRUE;
		m_nWindowStyle = c_WindowStyleClient;

		m_nMaskStyle = c_MaskStyleDotsDense;
		m_nMaskColor = RGB(255, 0, 0);

		m_nBackStyle = c_BackStyleSolid;
		m_nBackColor1 = ImageStudioUtils::GetColorLight(GetSysColor(COLOR_3DFACE), 16);
		m_nBackColor2 = ImageStudioUtils::GetColorDark(GetSysColor(COLOR_3DFACE), 16);

		// default preview image offsets and zoomings
		m_dCenterX = 0.0;
		m_dCenterY = 0.0;
		m_dZoom = 1.0;
		m_dAspect = -1.0;
		m_nAspectWidth = 0;
		m_nAspectHeight = 0;

		// disable mouse capturing flag - mouse unlocked by default
		m_nPointer = c_MousePointerDefault;

		// setup tracking object
		m_nTrackingColorScheme = c_TrackColorSchemeInverse;
		m_pTracking = 0;

		// disallow windowless activation
		m_bWindowOnly = TRUE;
	}

	//HWND CreateControlWindow(HWND hWndParent, RECT& rcPos)
	//{
	//	//T* pT = static_cast<T*>(this);
	//	//return pT->Create(hWndParent, rcPos);
	//	m_hWndTimer = CComControl::CreateControlWindow( hWndParent, rcPos );

	//	::SetTimer(	m_hWndTimer,        // handle to main window 
	//		IDT_TIMER1,         // timer identifier 
	//		1000,               // 1-second interval 
	//		(TIMERPROC) NULL ); // no timer callback 

	//	return hWndParent;
	//}

	~ImageView3()
	{
		Clear();

		if (m_pTracking)
		{
			delete m_pTracking;
			m_pTracking = 0;
		}
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
		if (m_pPixels)
		{
			delete[] m_pPixels;
			m_pPixels = 0;
		}
		if (m_pMask)
		{
			delete[] m_pMask;
			m_pMask = 0;
		}
/*
		if (m_pTracking)
		{
			delete m_pTracking;
			m_pTracking = 0;
		}
*/
		m_nWidth = 0;
		m_nHeight = 0;
	}
	
	BOOL IPP_ExtractRect( int nWidth, int nHeight, int nChannels, BYTE* pPixels )
	{
		return FALSE;

		// check for valid preview image
		if (!pPixels || !nWidth || !nHeight || nChannels != 4)
			return FALSE;

		// создаём буфер для промежуточных операций
		BYTE* pBuffer = NULL;
		BYTE* pImage  = m_pPixels;
		DWORD nImageSize = m_nWidth*m_nHeight*4;

		// отрисовываем маску
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
		
		// отрисовываем фон
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
		
		// копируем отображаемую картинку
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

		ippiResize_8u_C4R( pImage, SrcSize, m_nWidth*4, SrcRect, pDstPixels, nDstStride, DstSize, m_dZoom, m_dZoom, IPPI_INTER_LINEAR );

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
		if (!m_pTracking)
			return;

		// compute preview rectangle coordinates
		double dPreviewLeft, dPreviewTop, dPreviewRight, dPreviewBottom;
		ComputeRectangle(dPreviewLeft, dPreviewTop, dPreviewRight, dPreviewBottom);

		Structures::RECTI rectWindow(rect);
		Structures::RECTD rectData(dPreviewLeft, dPreviewTop, dPreviewRight, dPreviewBottom);

		m_oTrackingPainter.Create(hDC);
		m_pTracking->OnPaint(m_oTrackingPainter, rectWindow, rectData);
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

		// compute mouse button state
		nButton = GetKeyState(VK_LBUTTON) < 0 ? 1 : GetKeyState(VK_RBUTTON) < 0 ? 2 : GetKeyState(VK_MBUTTON) < 0 ? 4 : 0;
		nShift = (GetKeyState(VK_SHIFT) < 0 ? 1 : 0) + (GetKeyState(VK_CONTROL) < 0 ? 2 : 0) + (GetKeyState(VK_MENU) < 0 ? 4 : 0);
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
		if ((nSize % 2) || (nSize/2 >= 127))
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
		if (m_dAspect < 0.001)
			return x;

		return m_nAspectWidth*x/m_nWidth;
	}
	double AspectYAdd(double y)
	{
		if (m_dAspect < 0.001)
			return y;

		return m_nAspectHeight*y/m_nHeight;
	}
	double AspectXRemove(double x)
	{
		if (m_dAspect < 0.001)
			return x;

		return m_nWidth*x/m_nAspectWidth;
	}
	double AspectYRemove(double y)
	{
		if (m_dAspect < 0.001)
			return y;

		return m_nHeight*y/m_nAspectHeight;
	}

	void BGRA_Flip(BYTE* pBGRA, int nWidth, int nHeight)
	{
		/* @desc: 
		int BGRA_Flip(BYTE* pBGRA, int nWidth, int nHeight)
		{
			осуществляется вертикальное отражение картинки (pBGRA, nWidth, nHeight) 
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
	
public:
	
	// event wrappers with coordinates corrections
	void FireEvent(int Type)
	{
		if (!m_pTracking || m_pTracking->GetCount() < 1)
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
		else if( m_pTracking->IsTrackRectangleRotatable() )
		{
			double dAngle = Geometry::GetAngle( m_pTracking->GetAt(9).dX, m_pTracking->GetAt(9).dY, m_pTracking->GetAt(8).dX, m_pTracking->GetAt(8).dY) + M_PI_2;

			Structures::RECTD rectD = m_pTracking->GetUnrotatableRect( dAngle );	

			dAngle = grad( ((dAngle <= 0 )? dAngle + 2.0 * M_PI: dAngle));
		
			TrackRectangleRotatable( Type, AspectXAdd( rectD.left ), AspectYAdd( rectD.top ), AspectXAdd( rectD.right ), AspectYAdd( rectD.bottom ), dAngle);
		}
		else if( m_pTracking->IsTrackRectangleRotatableFixedAspect() )
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
			
			double dAngle1 = grad(Geometry::GetAngle(dCenterX, dCenterY, m_pTracking->GetAt(5).dX, m_pTracking->GetAt(5).dY));
			double dAngle2 = grad(Geometry::GetAngle(dCenterX, dCenterY, m_pTracking->GetAt(4).dX, m_pTracking->GetAt(4).dY));
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

			double dAngle1 = grad(Geometry::GetAngle(dCenterX, dCenterY, m_pTracking->GetAt(1).dX, m_pTracking->GetAt(1).dY));
			double dAngle2 = grad(Geometry::GetAngle(dCenterX, dCenterY, m_pTracking->GetAt(0).dX, m_pTracking->GetAt(0).dY));
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

			double dAngle1 = grad(Geometry::GetAngle(dCenterX, dCenterY, m_pTracking->GetAt(7).dX, m_pTracking->GetAt(7).dY));
			double dAngle2 = grad(Geometry::GetAngle(dCenterX, dCenterY, m_pTracking->GetAt(6).dX, m_pTracking->GetAt(6).dY));
			dAngle1 = (dAngle1 >= 0)? dAngle1 : 360 + dAngle1;
			dAngle2 = (dAngle2 >= 0)? dAngle2 : 360 + dAngle2;
			double SweepAngle = dAngle2 - dAngle1;
			SweepAngle = (SweepAngle >= 0)? SweepAngle : 360 + SweepAngle;

			double dRadius1 = Geometry::GetLength(dCenterX, dCenterY, m_pTracking->GetAt(6).dX, m_pTracking->GetAt(6).dY);
			double dRadius2 = Geometry::GetLength(dCenterX, dCenterY, m_pTracking->GetAt(4).dX, m_pTracking->GetAt(4).dY);
			double dRatio = (dRadius1 < 0.001)? 1. : dRadius2/dRadius1;
			
			TrackSector(Type, AspectXAdd(rectD.left), AspectYAdd(rectD.top), AspectXAdd(rectD.right), AspectYAdd(rectD.bottom), dAngle1, SweepAngle, dRatio);
		}
		else if (m_pTracking->IsTrackSectorSimple())
		{
			// get center
			double dCenterX = m_pTracking->GetAt(4).dX;
			double dCenterY = m_pTracking->GetAt(4).dY;

			// get angles
			double dAngle1 = grad(Geometry::GetAngle(dCenterX, dCenterY, m_pTracking->GetAt(1).dX, m_pTracking->GetAt(1).dY));
			double dAngle2 = grad(Geometry::GetAngle(dCenterX, dCenterY, m_pTracking->GetAt(0).dX, m_pTracking->GetAt(0).dY));
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
			
			Tracking::CTrackObjectTextRotatable *pTrackingTextRotatable = dynamic_cast <Tracking::CTrackObjectTextRotatable*> (m_pTracking);
			CString resString = "";
			if( NULL != pTrackingTextRotatable ) resString = pTrackingTextRotatable->GetString(); 
			
			TrackTextRotatable( Type, AspectXAdd( rectD.left ), AspectYAdd( rectD.top ), AspectXAdd( rectD.right ), AspectYAdd( rectD.bottom ), dAngle, resString.AllocSysString() );
		}
		else if (m_pTracking->IsTrackText())
		{
			Structures::RECTD rectD = m_pTracking->GetDoubleBoundingRect(0, 3);
			
			rectD.Normalize();

			Tracking::CTrackObjectText *pTrackingText = dynamic_cast <Tracking::CTrackObjectText*> (m_pTracking);
			CString resString = "";
			if( NULL != pTrackingText ) resString = pTrackingText->GetString(); 

			TrackText(Type, AspectXAdd(rectD.left), AspectYAdd(rectD.top), AspectXAdd(rectD.right), AspectYAdd(rectD.bottom), resString.AllocSysString());
		}
		else if (m_pTracking->IsTrackTrajectoryPolyline())
		{
			SAFEARRAY* pArray = 0; GetTrackArray(&pArray, TRUE);

			TrackTrajectoryPolyline(Type, (BYTE*)pArray);

			SafeArrayDestroy(pArray);
		}
	}
	
public:

	// common
	STDMETHOD(SetImage)(SAFEARRAY** Image)
	{

		if (!CRegistratorClient::IsRegistered())
			return S_OK;

		// destroy existing image
		Clear();
		
		// variables
		int nWidth = 0;
		int nHeight = 0;
		int nChannels = 0;
		BYTE* pPixels = 0;

		// extract image from the array
		pPixels = ImageStudioUtils::ExtractImage(Image, nWidth, nHeight, nChannels);

		// check that the image is valid
		if (!pPixels || nWidth < 2 || nHeight < 2 || nChannels != 4)
			return S_OK;

		// save image parameters
		m_nWidth = nWidth;
		m_nHeight = nHeight;

		// save non-aspected image
		m_nAspectWidth = m_nWidth;
		m_nAspectHeight = m_nHeight;

		// apply aspect correction
		if (m_dAspect >= 0.001)
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
				}
			}

			// save image size
			m_nWidth = nAspectWidth;
			m_nHeight = nAspectHeight;
		}
		else
		{
			// allocate memory for new mask
			m_pPixels = new BYTE[4*m_nWidth*m_nHeight];

			// check for allocated memory
			if (!m_pPixels)
			{
				Clear();
				return S_OK;
			}

			// retrieve data from safe array
			memcpy(m_pPixels, pPixels, 4*m_nWidth*m_nHeight);
		}

		// remove mask
		if (m_pMask)
		{
			delete[] m_pMask;
			m_pMask = 0;
		}

		// update window
		return Refresh();
	}
	STDMETHOD(SetMediaData)(IUnknown* pInterface)
	{
		// глупое решение - сконвертировать интерфейс в safearray и использовать старую функцию
		if (FALSE)
		{
			SAFEARRAY* pImageArray = NULL;
			if (ImageStudioUtils::MediaDataToSafeArray(&pInterface, pImageArray))
			{
				try
				{
					MediaCore::IAVSUncompressedVideoFrame* pFrame = (MediaCore::IAVSUncompressedVideoFrame*)pInterface;
					if (NULL != pFrame)
					{
						long nAspectX = 0;
						long nAspectY = 0;

						pFrame->get_AspectRatioX(&nAspectX);
						pFrame->get_AspectRatioY(&nAspectY);

						if (nAspectX > 0 && nAspectY > 0)
							m_dAspect = nAspectX/(double)nAspectY;
						else
							m_dAspect = -1.0;
					}
				}
				catch (...)
				{
				}

				SetImage(&pImageArray);
				ImageStudioUtils::DestroySafeArray(&pImageArray);
			}
		}
		else
		{
			// TODO: умное решение - взять указатель на память и его обработать
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

				bIsFlip = (lColorspace == CSP_BGRA);

				long nAspectX = 0;
				long nAspectY = 0;

				pFrame->get_Width(&nWidth);
				pFrame->get_Height(&nHeight);

				pFrame->get_AspectRatioX(&nAspectX);
				pFrame->get_AspectRatioY(&nAspectY);

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

				if ((NULL == pPixels) || (nWidth < 2 || nHeight < 2))
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
					}
				}

				// save image size
				m_nWidth = nAspectWidth;
				m_nHeight = nAspectHeight;

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
		
		OnPaint(c_SenderDraw);
		
		return S_OK;
	}
	STDMETHOD(ZoomOut)()
	{
		m_dZoom *= c_dImageFactorZoomOut;
		
		OnPaint(c_SenderDraw);
		
		return S_OK;
	}
	STDMETHOD(ZoomToFit)()
	{
		// check for valid image
		if (!IsValid())
			return S_OK;

		// convert size in inches to size in pixels
		RECT rect; GetClientRect(&rect);

		// correct zoom factor to shrink image in window
		m_dZoom = min((rect.right - rect.left)/(double)m_nWidth, (rect.bottom - rect.top)/(double)m_nHeight);

		// reinitialize center values
		m_dCenterX = 0.5*m_nWidth;
		m_dCenterY = 0.5*m_nHeight;

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
	
	// tracking
	STDMETHOD(get_trackType)(long* Type)
	{
		if (m_pTracking)
			*Type = m_pTracking->GetType();
		else
			*Type = Tracking::c_nTrackTypeNone;
		
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
	
	STDMETHOD(EndTrack)()
	{
		// check for valid image
		if (!IsValid())
			return S_OK;

		// check for no tracking
		if (!m_pTracking || m_pTracking->IsTrackNone())
			return S_OK;

		// fire final event
		FireEvent(3);

		// clear point list
		delete m_pTracking;
		m_pTracking = 0;

		// update preview
		OnPaint(c_SenderTrack);

		// all ok
		return S_OK;
	}

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

		BeginTrack(pTracking);

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

		BeginTrack(pTracking);

		return S_OK;
	}
	STDMETHOD(BeginTrackSector)(double Left, double Top, double Right, double Bottom, double StartAngle, double SweepAngle, double Ratio)
	{
		if (!IsValid())
			return S_OK;

		double dLeft = AspectXRemove(Left);
		double dTop = AspectYRemove(Top);
		double dRight = AspectXRemove(Right);
		double dBottom = AspectYRemove(Bottom);

		Tracking::CTrackObjectSector* pTracking = new Tracking::CTrackObjectSector();

		pTracking->Create(dLeft, dTop, dRight, dBottom, StartAngle, SweepAngle, Ratio);

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
	STDMETHOD(BeginTrackTextRotatable)(double Left, double Top, double Right, double Bottom, double Angle, double StickAngle, BSTR inText, BSTR FontName, int FontSize, int FontType, int FontColor)
	{
		if (!IsValid())
			return S_OK;

		double dLeft	= AspectXRemove(Left);
		double dTop		= AspectYRemove(Top);
		double dRight	= AspectXRemove(Right);
		double dBottom	= AspectYRemove(Bottom);

		Tracking::CTrackObjectTextRotatable* pTracking = new Tracking::CTrackObjectTextRotatable();

		pTracking->Create(dLeft, dTop, dRight, dBottom, Angle, StickAngle, inText, FontName, FontSize, FontType, FontColor);

		BeginTrack(pTracking);

		return S_OK;
	}

	STDMETHOD(BeginTrackText)(double Left, double Top, double Right, double Bottom, BSTR inText, BSTR FontName, int FontSize, int FontType, int FontColor)
	{
		if (!IsValid())
			return S_OK;

		double dLeft	= AspectXRemove(Left);
		double dTop		= AspectYRemove(Top);
		double dRight	= AspectXRemove(Right);
		double dBottom	= AspectYRemove(Bottom);

		Tracking::CTrackObjectText* pTracking = new Tracking::CTrackObjectText();

		pTracking->Create(dLeft, dTop, dRight, dBottom, inText, FontName, FontSize, FontType, FontColor);

		BeginTrack(pTracking);

		return S_OK;
	}
	
	STDMETHOD(MoveTrack)(double dX, double dY)
	{
		if (!IsValid())
			return S_OK;

		if (m_pTracking)
		{
			m_pTracking->MoveTrack(dX, dY);
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
	
	// additional parameters
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		CComVariant ParamVal = ParamValue;
		CComBSTR ParamStr = ParamName;

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
		else if (ParamStr == L"BeginTrackQuadraticBezier" )
		{
			Structures::POINTD* pPoints = 0;
			int nPointsCount = 0;

			if (!SafeArrayToPoints(&ParamValue.parray, TRUE, pPoints, nPointsCount))
				return S_OK;

			Tracking::CTrackObjectQuadraticBezier* pTracking = new Tracking::CTrackObjectQuadraticBezier();

			pTracking->Create(pPoints, nPointsCount);

			delete[] pPoints;

			BeginTrack(pTracking);
		}

		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		CComBSTR ParamStr = ParamName;

		if (ParamStr == L"TrackingColors")
			(*ParamValue).lVal = (LONG)m_nTrackingColorScheme;
		else if (ParamStr == L"TrackChangeType")
		{
			POINT point; GetCursorPos(&point); ::ScreenToClient(m_hWnd, &point);
			int nPointStatus		= Tracking::c_nTrackPointStatusNone;
			int nTrackChangeStatus	= Tracking::c_nTrackChangeStatusNone;
			if( m_pTracking )
				m_pTracking->OnSetCursor(point.x, point.y, nPointStatus, nTrackChangeStatus);
			(*ParamValue).lVal = (LONG)nTrackChangeStatus;
		}

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
};
