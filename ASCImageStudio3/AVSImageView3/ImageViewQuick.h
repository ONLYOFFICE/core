#pragma once
	
#include "resource.h"
#include <atlctl.h>
#include <GdiPlusEx.h>
#include <Registration.h>
#include <ImageStudioUtils.h>
#include <XmlUtils.h>
#include <Rendering.h>
	
[object, uuid("E2D69456-A532-42D9-AA06-651D7C330530"), dual, pointer_default(unique)]
__interface IImageViewQuick : public IDispatch
{
	[id(101)] HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(102)] HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);

	[id(203)] HRESULT Clear();

	[id(301)] HRESULT SendSafeArray([in, satype("BYTE")] SAFEARRAY** Array);
	[id(303)] HRESULT SendImageFile([in] BSTR FilePath);
	[id(304)] HRESULT SendImageStream([in, out] BYTE* ByteArray, [in] long Size);

	[id(401), propget] HRESULT videoWidth([out, retval] long* Width);
	[id(402), propget] HRESULT videoHeight([out, retval] long* Height);
	
	[id(501), propput] HRESULT BackColor([in] long Color);
	[id(501), propget] HRESULT BackColor([out, retval] long* Color);
};
	
[dispinterface, uuid("03D73552-0EC4-46DC-A005-AFF2F64C10B1")]
__interface _IImageViewQuickEvents
{
	// Button: 0 - Left button, 1 - Middle button, 2 - Right button
	// Shift: 0 - None, 1 - Shift, 2 - Ctrl 
	// KeyCode: ASCII code of the key
	// X, Y: Coordinates of the cursor.
	[id(1)] HRESULT OnKeyDown(LONG KeyCode, LONG Shift);
	[id(2)] HRESULT OnKeyUp(LONG KeyCode, LONG Shift);
	[id(3)] HRESULT OnKeyPress(LONG KeyCode);
	[id(4)] HRESULT OnMouseDown(LONG Button, LONG X, LONG Y, LONG Shift);
	[id(5)] HRESULT OnMouseUp(LONG Button, LONG X, LONG Y, LONG Shift);
	[id(6)] HRESULT OnMouseMove(LONG X, LONG Y, LONG Shift);
	[id(7)] HRESULT OnDblClick(LONG Button, LONG X, LONG Y, LONG Shift);
};
	
[coclass, uuid("FB6FCB4C-2D2D-4D00-B861-2078B15250CD"), threading(apartment), vi_progid("AVSImageView.ImageViewQuick"), progid("AVSImageView.ImageViewQuick.1"), version(1.0), event_source(com), support_error_info(IImageViewQuick), registration_script("control.rgs")]
class ATL_NO_VTABLE ImageViewQuick : 
	public IImageViewQuick,
	public IPersistStreamInitImpl<ImageViewQuick>,
	public IOleControlImpl<ImageViewQuick>,
	public IOleObjectImpl<ImageViewQuick>,
	public IOleInPlaceActiveObjectImpl<ImageViewQuick>,
	public IViewObjectExImpl<ImageViewQuick>,
	public IOleInPlaceObjectWindowlessImpl<ImageViewQuick>,
	public IPersistStorageImpl<ImageViewQuick>,
	public ISpecifyPropertyPagesImpl<ImageViewQuick>,
	public IQuickActivateImpl<ImageViewQuick>,
	public IDataObjectImpl<ImageViewQuick>,
	public CComControl<ImageViewQuick>
{
public:

	__event __interface _IImageViewQuickEvents;

	DECLARE_VIEW_STATUS(0)
	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE | OLEMISC_CANTLINKINSIDE | OLEMISC_INSIDEOUT | OLEMISC_ACTIVATEWHENVISIBLE | OLEMISC_SETCLIENTSITEFIRST)

	BEGIN_PROP_MAP(ImageViewQuick)
		PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
		PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	END_PROP_MAP()

	BEGIN_MSG_MAP(ImageViewQuick)
		MESSAGE_RANGE_HANDLER(0, 0xFFFFFFFF, OnMessages)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

public:

	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		CComVariant ParamVal = ParamValue;
		CComBSTR ParamStr = ParamName;

		if (ParamStr == L"Transform")
		{
			ParamVal.ChangeType(VT_BSTR, NULL);

			m_strTransform = ParamVal.bstrVal;
		}
		else if (ParamStr == L"InterpolateSmooth")
		{
			ParamVal.ChangeType(VT_BOOL, NULL);

			m_bInterpolateSmooth = ParamVal.boolVal;
		}
		else if (ParamStr == L"ZoomShrinked")
		{
			ParamVal.ChangeType(VT_BOOL, NULL);

			m_bZoomShrinked = ParamVal.boolVal;
		}

		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		return S_OK;
	}
	
	STDMETHOD(Clear)()
	{
		m_oImage.Destroy();

		OnPaint();

		return S_OK;
	}
		
	STDMETHOD(SendSafeArray)(SAFEARRAY** Image)
	{
		m_oImage.CreateFromSafeArray(Image, FALSE);

		AxApplyTransform();

		OnPaint();

		return S_OK;
	}
	STDMETHOD(SendImageFile)(BSTR FilePath)
	{
		m_oImage.CreateFromFile(FilePath);

		AxApplyTransform();

		OnPaint();

		return S_OK;
	}
	STDMETHOD(SendImageStream)(BYTE* ByteArray, long Size)
	{
		m_oImage.CreateFromStream(ByteArray, Size);

		AxApplyTransform();

		OnPaint();

		return S_OK;
	}
	
	STDMETHOD(get_videoWidth)(long* Value)
	{
		*Value = m_oImage.GetWidth();
		
		return S_OK;
	}
	STDMETHOD(get_videoHeight)(long* Value)
	{
		*Value = m_oImage.GetHeight();
		
		return S_OK;
	}
	
	STDMETHOD(get_BackColor)(long* Value)
	{
		*Value = m_nBackColor;

		return S_OK;
	}
	STDMETHOD(put_BackColor)(long Value)
	{
		m_nBackColor = Value;

		Invalidate();

		return S_OK;
	}

	
public:
	
	ImageViewQuick()
	{
		m_nBackColor = 0xFF000000;

		m_bInterpolateSmooth = FALSE;

		m_bZoomShrinked = FALSE;

		m_bWindowOnly = TRUE;
	}
	~ImageViewQuick()
	{
	}
	
	LRESULT OnMessages(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;

		if (uMsg == WM_ERASEBKGND)
		{
			bHandled = TRUE;
		}
		else if (uMsg == WM_CREATE)
		{
			OnCreate();
		}
		else if (uMsg == WM_DESTROY)
		{
			OnDestroy();
		}
		else if (uMsg == WM_PAINT)
		{
			OnPaint();
		}
		else if (uMsg == WM_SIZE)
		{
			OnSize();
		}
		else if (uMsg == WM_MOVE)
		{
			OnMove();
		}
		else if (uMsg == WM_KEYUP)
		{
			long KeyCode = wParam;
			long Flags = HIWORD(lParam);
			long Shift = 0;
			bHandled = TRUE;

			if (KeyCode == 16) Shift = 1;
			if (KeyCode == 17) Shift = 2;

			OnKeyUp(KeyCode, Shift);
		}
		else if (uMsg == WM_KEYDOWN)
		{
			long KeyCode = wParam;
			long Flags = HIWORD(lParam);
			long Shift = 0;
			bHandled = TRUE;

			if (KeyCode == 16) Shift = 1;
			if (KeyCode == 17) Shift = 2;

			OnKeyDown(KeyCode, Shift);
			OnKeyPress(KeyCode);
		}
		else if (uMsg == WM_LBUTTONDOWN || uMsg == WM_MBUTTONDOWN || uMsg == WM_RBUTTONDOWN ||
				 uMsg == WM_LBUTTONUP || uMsg == WM_MBUTTONUP || uMsg == WM_RBUTTONUP ||
				 uMsg == WM_LBUTTONDBLCLK || uMsg == WM_MBUTTONDBLCLK || uMsg == WM_RBUTTONDBLCLK ||
				 uMsg == WM_MOUSEMOVE)
		{
			OnMouseEvens(uMsg, wParam, lParam);

			bHandled = TRUE;
		}

		return 0;
	}
	
	void OnCreate()
	{
		m_oGdiPlus.Init();
	}
	void OnDestroy()
	{
	}
	void OnPaint()
	{
		if (!m_hWnd || !IsWindow())
			return;

		HDC hDC = GetDC();
		if (!hDC)
			return;

		Graphics *pGraphics = new Graphics(hDC);

		if (pGraphics)
		{
			if (!m_oImage.IsValid())
			{
				Color oColor(GetRValue(m_nBackColor), GetGValue(m_nBackColor), GetBValue(m_nBackColor));

				pGraphics->Clear(oColor);
			}
			else
			{
				RECT oRect; GetClientRect(&oRect);

				AxPaintScreen(pGraphics, oRect);
			}

			delete pGraphics;
		}

		ReleaseDC(hDC);
	}
	void OnSize()
	{
		OnPaint();
	}
	void OnMove()
	{
	}
	void OnMouseEvens(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		long X = GET_X_LPARAM(lParam); 
		long Y = GET_Y_LPARAM(lParam);
		
		long Flags = wParam;
		long Shift = 0;

		if (Flags & MK_SHIFT) Shift = 1;
		if (Flags & MK_CONTROL) Shift = 2;

		switch (uMsg)
		{
			case WM_LBUTTONDOWN:	OnMouseDown(0, X, Y, Shift);	break;
			case WM_MBUTTONDOWN:	OnMouseDown(1, X, Y, Shift);	break;
			case WM_RBUTTONDOWN:	OnMouseDown(2, X, Y, Shift);	break;
			case WM_LBUTTONUP:		OnMouseUp(0, X, Y, Shift);		break;
			case WM_MBUTTONUP:		OnMouseUp(1, X, Y, Shift);		break;
			case WM_RBUTTONUP:		OnMouseUp(2, X, Y, Shift);		break;
			case WM_MOUSEMOVE:		OnMouseMove(X, Y, Shift);		break;
			case WM_LBUTTONDBLCLK:	OnDblClick(0, X, Y, Shift);		break;
			case WM_MBUTTONDBLCLK:	OnDblClick(1, X, Y, Shift);		break;
			case WM_RBUTTONDBLCLK:	OnDblClick(2, X, Y, Shift);		break;
		}
	}
	
	void AxPaintScreen(Graphics* pGraphics, RECT oRect)
	{
		Bitmap* pBitmap = (Bitmap*)m_oImage;

		if (!pGraphics || !pBitmap)
			return;

		Color oColor(GetRValue(m_nBackColor), GetGValue(m_nBackColor), GetBValue(m_nBackColor));

		// configure graphics
		pGraphics->SetPixelOffsetMode(PixelOffsetModeHalf);		
		if (m_bInterpolateSmooth)
			pGraphics->SetInterpolationMode(InterpolationModeBilinear);
		else
			pGraphics->SetInterpolationMode(InterpolationModeNearestNeighbor);
		
		// compute destination rectangle
		double dZoom = min((oRect.right - oRect.left)/(double)pBitmap->GetWidth(), (oRect.bottom - oRect.top)/(double)pBitmap->GetHeight());
		double dCenterX = 0.5*(oRect.right + oRect.left);
		double dCenterY = 0.5*(oRect.bottom + oRect.top);
		double dWidth = dZoom*pBitmap->GetWidth();
		double dHeight = dZoom*pBitmap->GetHeight();

		if (m_bZoomShrinked)
			dZoom = min(1, dZoom); // perform shrink to fit

		RectF oRectDest((REAL)(dCenterX - 0.5*dWidth), (REAL)(dCenterY - 0.5*dHeight), (REAL)dWidth, (REAL)dHeight);

		// draw background
		Gdiplus::SolidBrush oClearBrush(oColor);
		pGraphics->FillRectangle(&oClearBrush, (REAL)oRect.left, (REAL)oRect.top, oRectDest.GetLeft(), (REAL)(oRect.bottom - oRect.top));
		pGraphics->FillRectangle(&oClearBrush, oRectDest.GetRight(), (REAL)oRect.top, oRect.right - oRectDest.GetRight(), (REAL)(oRect.bottom - oRect.top));
		pGraphics->FillRectangle(&oClearBrush, oRectDest.GetLeft(), (REAL)oRect.top, oRectDest.Width, oRectDest.GetTop() - (REAL)oRect.top);
		pGraphics->FillRectangle(&oClearBrush, oRectDest.GetLeft(), oRectDest.GetBottom(), oRectDest.Width, (REAL)oRect.bottom - oRectDest.GetBottom());

		// draw image
		pGraphics->DrawImage((Bitmap*)m_oImage, oRectDest);
	}
	void AxApplyTransform()
	{
		if (!m_oImage.IsValid())
			return;

		if (m_strTransform.GetLength() < 1)
			return;

		Rendering::Xml::CXmlPainter oPainter;

		oPainter.SetImage(&m_oImage);

		oPainter.PaintXml(m_strTransform);
	}
		
public:
	
	COLORREF m_nBackColor;								// background color of the control

	CGdiPlusInit m_oGdiPlus;							// gdi+ for image painting

	GdiPlusEx::CGdiPlusImage m_oImage;					// the image to show

	CString	m_strTransform;								// transformation that should be done before the painting

	BOOL m_bInterpolateSmooth;							// type of the interpolation of the image
	BOOL m_bZoomShrinked;								// shrink image to fit (or zoom to fit)
};
