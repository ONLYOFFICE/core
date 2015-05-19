#pragma once
	
#include "resource.h"
#include <atlctl.h>
#include <GdiPlusEx.h>
#include <Registration.h>
#include <ImageStudioUtils.h>
#include <XmlUtils.h>
#include <Rendering.h>
	
[object, uuid("F6B39DEC-AEA0-422f-9F8C-54556410C95D"), dual, pointer_default(unique)]
__interface IImageViewLight3 : public IDispatch
{
	[id(101)] HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(102)] HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);

	[id(201)] HRESULT Start([in] long Width, [in] long Height, [in] double Aspect);
	[id(202)] HRESULT Stop();
	[id(203)] HRESULT Clear();
	[id(204)] HRESULT IsStarted([out, retval] VARIANT_BOOL* Started);

	[id(301)] HRESULT SendSafeArray([in, satype("BYTE")] SAFEARRAY** Array);
	[id(302)] HRESULT SendMediaData([in] IUnknown** Image);
	[id(303)] HRESULT SendImageFile([in] BSTR FilePath);
	[id(304)] HRESULT SendImageStream([in, out] BYTE* ByteArray, [in] long Size);

	[id(401), propget] HRESULT videoWidth([out, retval] long* Width);
	[id(402), propget] HRESULT videoHeight([out, retval] long* Height);
	[id(403), propget] HRESULT videoAspect([out, retval] double* Value);
	
	[id(501), propput] HRESULT BackColor([in] long Color);
	[id(501), propget] HRESULT BackColor([out, retval] long* Color);
};
	
[dispinterface, uuid("56B7F9EA-92F5-4e65-AF22-6B12F6B4252D")]
__interface _IImageViewLight3Events
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
	
[coclass, uuid("5FD42F43-6162-4b0e-9849-74E78853FDA8"), threading(apartment), vi_progid("AVSImageView.ImageViewLight3"), progid("AVSImageView.ImageViewLight3.1"), version(1.0), event_source(com), support_error_info(IImageViewLight3), registration_script("control.rgs")]
class ATL_NO_VTABLE ImageViewLight3 : 
	public IImageViewLight3,
	public IPersistStreamInitImpl<ImageViewLight3>,
	public IOleControlImpl<ImageViewLight3>,
	public IOleObjectImpl<ImageViewLight3>,
	public IOleInPlaceActiveObjectImpl<ImageViewLight3>,
	public IViewObjectExImpl<ImageViewLight3>,
	public IOleInPlaceObjectWindowlessImpl<ImageViewLight3>,
	public IPersistStorageImpl<ImageViewLight3>,
	public ISpecifyPropertyPagesImpl<ImageViewLight3>,
	public IQuickActivateImpl<ImageViewLight3>,
	public IDataObjectImpl<ImageViewLight3>,
	public CComControl<ImageViewLight3>
{
public:
	
	__event __interface _IImageViewLight3Events;
	
	DECLARE_VIEW_STATUS(0)
	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE | OLEMISC_CANTLINKINSIDE | OLEMISC_INSIDEOUT | OLEMISC_ACTIVATEWHENVISIBLE | OLEMISC_SETCLIENTSITEFIRST)
	
	BEGIN_PROP_MAP(ImageViewLight3)
		PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
		PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	END_PROP_MAP()
	
	BEGIN_MSG_MAP(ImageViewLight3)
		MESSAGE_RANGE_HANDLER(0, 0xFFFFFFFF, OnMessages)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()
	
public:
	
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		CComVariant ParamVal = ParamValue;
		CComBSTR ParamStr = ParamName;

		if (ParamStr == L"MediaData")
		{
			ParamVal.ChangeType(VT_UNKNOWN, NULL);
			IUnknown* pImage = ParamVal.punkVal;

			SendMediaData(&pImage);
		}
		else if (ParamStr == L"Transform")
		{
			ParamVal.ChangeType(VT_BSTR, NULL);

			m_strTransform = ParamVal.bstrVal;
		}

		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		return S_OK;
	}
	
	STDMETHOD(Start)(long Width, long Height, double Aspect)
	{
		m_nVideoOutWidth = Width;
		m_nVideoOutHeight = Height;
		m_dVideoOutAspect = Aspect;

		AxStopVideoOut();

		AxStartVideoOut();
	
		return S_OK;
	}
	STDMETHOD(Stop)()
	{
		AxStopVideoOut();
		
		return S_OK;
	}
	STDMETHOD(Clear)()
	{
		if (m_pVideoOutput)
			m_pVideoOutput->SendData(NULL);

		Invalidate(FALSE);

		return S_OK;
	}
	STDMETHOD(IsStarted)(VARIANT_BOOL* Started)
	{
		*Started = m_bVideoOutStarted ? VARIANT_TRUE : VARIANT_FALSE;
		
		return S_OK;
	}

		
	STDMETHOD(SendSafeArray)(SAFEARRAY** Image)
	{
		m_oImage.CreateFromSafeArray(Image, FALSE);

		AxUpdateImageFromImage();

		return S_OK;
	}
	STDMETHOD(SendMediaData)(IUnknown** Image)
	{
		AxUpdateImageFromInterface(Image);
		
		return S_OK;
	}
	STDMETHOD(SendImageFile)(BSTR FilePath)
	{
		m_oImage.CreateFromFile(FilePath);

		AxUpdateImageFromImage();

		return S_OK;
	}
	STDMETHOD(SendImageStream)(BYTE* ByteArray, long Size)
	{
		m_oImage.CreateFromStream(ByteArray, Size);

		AxUpdateImageFromImage();

		return S_OK;
	}
	
	STDMETHOD(get_videoWidth)(long* Value)
	{
		*Value = m_nVideoOutWidth;
		
		return S_OK;
	}
	STDMETHOD(get_videoHeight)(long* Value)
	{
		*Value = m_nVideoOutHeight;
		
		return S_OK;
	}
	STDMETHOD(get_videoAspect)(double* Value)
	{
		*Value = m_dVideoOutAspect;
		
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
	
	ImageViewLight3()
	{
		m_bVideoOutStarted = FALSE;
		m_nVideoOutWidth = -1;
		m_nVideoOutHeight = -1;
		m_dVideoOutAspect = -1;

		m_nBackColor = 0xFF0000FF;

		m_bWindowOnly = TRUE;
	}
	~ImageViewLight3()
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

		CoCreateInstance(VideoOutput::CLSID_CAVSVideoOutputCtrl3, NULL, CLSCTX_ALL, VideoOutput::IID_IAVSVideoOutputCtrl3, (void**)&m_pVideoOutput);

		if (m_pVideoOutput)
		{
			m_pVideoOutput->put_WindowHandle((long)m_hWnd);
			m_pVideoOutput->SendData(NULL);
		}
	}
	void OnDestroy()
	{
		AxStopVideoOut();

		if (m_pVideoOutput)
			m_pVideoOutput->Release();
	}
	void OnPaint()
	{
		if (!m_hWnd || !IsWindow())
			return;

		if (m_pVideoOutput && m_bVideoOutStarted)
			m_pVideoOutput->OnPaint();
		else
		{
			HDC hDC = GetDC();
			if (hDC)
			{
				HBRUSH hBrush = CreateSolidBrush(m_nBackColor);
				if (hBrush)
				{
					RECT oRect; GetClientRect(&oRect);
					
					FillRect(hDC, &oRect, hBrush);
					
					DeleteObject(hBrush);
				}

				ReleaseDC(hDC);
			}
		}
	}
	void OnSize()
	{
		if (m_pVideoOutput)
			m_pVideoOutput->OnSize();
	}
	void OnMove()
	{
		if (m_pVideoOutput)
			m_pVideoOutput->OnMove();
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
	
	void AxStopVideoOut()
	{
		// stop current video out (if it is working)
		if (m_bVideoOutStarted)
		{
			m_pVideoOutput->Stop();
			m_bVideoOutStarted = FALSE;
		}
	}
	void AxStartVideoOut()
	{
		if (!m_pVideoOutput)
			return;
		if (m_nVideoOutWidth < 0 || m_nVideoOutHeight < 0 || m_bVideoOutStarted)
			return;

		// configure the format
		if (true)
		{
			MediaFormat::IAVSVideoFormat* pVideoFormat = NULL;
			CoCreateInstance(MediaFormat::CLSID_CAVSVideoFormat, NULL, CLSCTX_ALL, MediaFormat::IID_IAVSVideoFormat, (void**)&pVideoFormat);
			if (NULL == pVideoFormat)
				return;

			pVideoFormat->put_ColorSpace(CSP_BGRA);
			pVideoFormat->put_Width(m_nVideoOutWidth);
			pVideoFormat->put_Height(m_nVideoOutHeight);

			if (m_dVideoOutAspect >= 0.001)
			{
				pVideoFormat->put_AspectRatioX(m_nVideoOutHeight*m_dVideoOutAspect);
				pVideoFormat->put_AspectRatioY(m_nVideoOutHeight);
			}
			else
			{
				pVideoFormat->put_AspectRatioX(m_nVideoOutWidth);
				pVideoFormat->put_AspectRatioY(m_nVideoOutHeight);
			}

			IUnknown* pUnk = NULL;
			pVideoFormat->QueryInterface(__uuidof(IUnknown), (void**)&pUnk);
			if (NULL != pUnk)
			{
				m_pVideoOutput->put_VideoSetting(pUnk);
				pUnk->Release();
			}
			pVideoFormat->Release();
		}

		// start the new video output
		if (true)
		{
			VARIANT_BOOL bStarted = VARIANT_FALSE;

			m_pVideoOutput->Start(&bStarted);

			m_bVideoOutStarted = (bStarted == VARIANT_TRUE);
		}
	}
	void AxApplyTransform()
	{
		if (m_strTransform.GetLength() < 1)
			return;

		Rendering::Xml::CXmlPainter oPainter;

		oPainter.SetImage(&m_oImage);

		oPainter.PaintXml(m_strTransform);
	}
	void AxUpdateImageFromInterface(IUnknown** pUnk)
	{
		if (!pUnk)
			return;
		if (!m_pVideoOutput)
			return;
		if (m_nVideoOutWidth < 0 || m_nVideoOutHeight < 0 || !m_bVideoOutStarted)
			return;

		try
		{
			m_pVideoOutput->SendData(*pUnk);
		}
		catch (...)
		{
		}
	}
	void AxUpdateImageFromImage()
	{
		if (!m_pVideoOutput)
			return;
		if (m_nVideoOutWidth < 0 || m_nVideoOutHeight < 0 || !m_bVideoOutStarted || !m_oImage.IsValid())
			return;

		AxApplyTransform();

		IUnknown* pUnk = NULL;
		if (ImageStudioUtils::GdiPlusImageToMediaData(&m_oImage, &pUnk, FALSE))
		{
			AxUpdateImageFromInterface(&pUnk);
			pUnk->Release();
		}
	}
		
public:
	
	COLORREF m_nBackColor;								// background color of the control

	CGdiPlusInit m_oGdiPlus;							// gdi+ for image painting

	GdiPlusEx::CGdiPlusImage m_oImage;					// the image to show

	BOOL m_bVideoOutStarted;							// state of the video out
	int m_nVideoOutWidth;								// width of the created video out
	int m_nVideoOutHeight;								// height of the created video out
	double m_dVideoOutAspect;							// aspect of the created video out

	CString	m_strTransform;								// transformation that should be done before the painting
	
	VideoOutput::IAVSVideoOutputCtrl3* m_pVideoOutput;	// seniour video out :-)
};
