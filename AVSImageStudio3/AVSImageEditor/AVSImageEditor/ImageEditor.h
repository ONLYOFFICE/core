// ImageEditor.h : Declaration of the AVSImageEditor
#pragma once
#include "resource.h"       // main symbols
#include <atlctl.h>

#include "ViewManager.h"
#include "LayersManager.h"
#include "CommandManager.h"

#include "ViewSettings.h"

#include "Commands/CommandsHdr.h"

// IAVSImageEditor
[
	object,
	uuid(1C8179AF-5E09-4A83-9CF8-3006E7B35202),
	dual,
	helpstring("IAVSImageEditor Interface"),
	pointer_default(unique)
]
__interface IAVSImageEditor : public IDispatch
{
	[id(100)] HRESULT Open([in] BSTR filePath, [out, retval] VARIANT_BOOL* Success);
	[id(101)] HRESULT Save([in] BSTR filePath, [out, retval] VARIANT_BOOL* Success);

	[id(200)] HRESULT GetViewSettings([out, retval] IUnknown** pVal);


	[id(300)] HRESULT StartCommand([in] long comID, [out, retval] VARIANT_BOOL* Success);
	[id(301)] HRESULT GetCommand([out, retval] IUnknown** pVal);
	[id(302)] HRESULT StopCommand([in] VARIANT_BOOL goodStop, [out, retval] VARIANT_BOOL* Success);

	[id(310)] HRESULT GetBackgroundCommand([in] long comID, [out, retval] IUnknown** pVal);

	[id(400)] HRESULT GetUndoStack([out, retval] IUnknown** pVal);
	[id(401)] HRESULT GetRedoStack([out, retval] IUnknown** pVal);

	[id(500)] HRESULT GetThumbnail([out, retval] IUnknown** pInterface);
	[id(501)] HRESULT GetHistogramm([satype("BYTE"), out, retval] SAFEARRAY** pHistogramm);

	[id(600)] HRESULT GetMetaData([out, retval] BSTR* bsMetaDataXML);
	[id(601)] HRESULT SetMetaData([in] BSTR bsMetaDataXML);

	[id(1000)] HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(1001)] HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);

	[id(1002)] HRESULT LoadState([in] BSTR filePath, [out, retval] VARIANT_BOOL* Success);
	[id(1003)] HRESULT SaveState([in] BSTR filePath, [out, retval] VARIANT_BOOL* Success);

	[id(1004)] HRESULT FastSave([in] BSTR avspackPath, [in] BSTR resultPath, [out, retval] VARIANT_BOOL* Success);

	
};


// _IEditorEvents
[
	dispinterface,
	uuid("58A5F403-A159-4b35-BD53-A2A202A1D6F0"),
	helpstring("_IEditorEvents Interface")
]
__interface _IEditorEvents
{
	[id(100)] HRESULT OnDoubleClick();

	[id(200)] HRESULT OnStartProgress();
	[id(201)] HRESULT OnProgress([in] LONG percent);
	[id(202)] HRESULT OnFinishProgress();
	[id(203)] HRESULT OnChangePreview();
};

// AVSImageEditor
[
	coclass,
	default(IAVSImageEditor,_IEditorEvents),
	threading(apartment),
	event_source(com),
	vi_progid("AVSImageEditor.AVSImageEditor"),
	progid("AVSImageEditor.AVSImageEditor.1"),
	version(1.0),
	uuid("48BD5D71-0291-42D0-8BFD-DF9E5D367ED0"),
	helpstring("AVSImageEditor Class"),
	support_error_info(IAVSImageEditor),
	registration_script("control.rgs")
]
class ATL_NO_VTABLE AVSImageEditor : 
	public IAVSImageEditor,
	public IPersistStreamInitImpl<AVSImageEditor>,
	public IOleControlImpl<AVSImageEditor>,
	public IOleObjectImpl<AVSImageEditor>,
	public IOleInPlaceActiveObjectImpl<AVSImageEditor>,
	public IViewObjectExImpl<AVSImageEditor>,
	public IOleInPlaceObjectWindowlessImpl<AVSImageEditor>,
	public IPersistStorageImpl<AVSImageEditor>,
	public ISpecifyPropertyPagesImpl<AVSImageEditor>,
	public IQuickActivateImpl<AVSImageEditor>,
	public IDataObjectImpl<AVSImageEditor>,
	public IProvideClassInfo2Impl<&__uuidof(AVSImageEditor), NULL>,
	public CComControl<AVSImageEditor>,
	public IView,
	public ITimer,
	public IAppManager,
	public IUndoRedoCallback,
	public IProgresser
	//public IMessageReceiver
{
public:

	__event __interface _IEditorEvents;

	AVSImageEditor():m_current_command(NULL)
	{
		DEV_LOG("Create AVSImageEditor");
		
		if (!SUCCEEDED(m_image_studio.CreateInstance ( ImageStudio3::CLSID_ImageTransforms )))
		{
			::MessageBox ( NULL, _T("Cannot create ImageStudio COM object"), _T("Error"), MB_OK );
		}

		if ( FAILED ( m_pViewSettings.CoCreateInstance ( __uuidof(CViewSettings) ) ) )
		{
			::MessageBox ( NULL, _T("Cannot create IViewSettings COM object"), _T("Error"),MB_OK );
		}

		m_oLayersManager.SetUndoRedoCallbackPtr(this);
		m_oLayersManager.SetAppManager(this);
		
		m_background_command = new  CBckgTonesCommand(this);

		m_watermarks = NULL;

		m_bCommandPrint	= FALSE;
		m_nProgressPercent = 0;

		m_oViewManager.SetShowMouseMarkers ( TRUE );
	}

	~AVSImageEditor()
	{
		StopAllTimers();
		m_oLayersManager.ClearHistories();
		if (m_watermarks)
		{
			delete m_watermarks;
			m_watermarks = NULL;
		}
		if (m_background_command)
		{
			delete m_background_command;
			m_background_command = NULL;
		}
		if (m_current_command)
		{
			delete m_current_command;
			m_current_command = NULL;
		}
		m_oLayersManager.Clear();

		DEV_LOG("Delete AVSImageEditor");
	}

DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE | 
	OLEMISC_CANTLINKINSIDE | 
	OLEMISC_INSIDEOUT | 
	OLEMISC_ACTIVATEWHENVISIBLE | 
	OLEMISC_SETCLIENTSITEFIRST
)


BEGIN_PROP_MAP(AVSImageEditor)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()


BEGIN_MSG_MAP(AVSImageEditor)
	CHAIN_MSG_MAP(CComControl<AVSImageEditor>)
	MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackGround)
	MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDblClick)
	MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
	MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
	MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_KEYUP, OnKeyUp)
	MESSAGE_HANDLER(WM_TIMER, OnTimer)
	MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
	MESSAGE_HANDLER(WM_IEM_THREAD_FINISHED, OnFinishUpdateThread)
	MESSAGE_HANDLER(WM_IEM_STOP_COMMAND_YES, OnStopCommandGood)
	MESSAGE_HANDLER(WM_IEM_STOP_COMMAND_NO, OnStopCommandBad)
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// IAVSImageEditor
public:
	
	LRESULT OnEraseBackGround ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
	{
		return TRUE;
	}

	LRESULT OnLButtonDblClick ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
	{
		OnDoubleClick();
		return TRUE;
	}

	// IInvalidater interface
	virtual void InvalidateView(bool updateData = true, LPRECT invRect = NULL)
	{
		if (!::IsWindow(m_hWnd))
			return;

		DEV_LOG("InvalidateView - 1");
		long wdth, hght, widthSlow, heightSlow;
		double scaleCoef;
		if (GetLayersManager()->GetCurrentLayer() && updateData)
		{
			DEV_LOG("InvalidateView branch A-1");
			const BYTE* imData = GetLayersManager()->GetCurrentLayer()->GetActualImage(wdth,hght,scaleCoef);
			DEV_LOG("InvalidateView branch A-2");
			
			DEV_LOG("InvalidateView branch A-3");
			const BYTE* imDataTmp = GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::SLOW, widthSlow,heightSlow);
			m_oViewManager.SetImage(imData, wdth, hght, scaleCoef, widthSlow, heightSlow);
			DEV_LOG("InvalidateView branch A-4");
			if (invRect==NULL)
				Invalidate();	
			else
				InvalidateRect ( invRect, TRUE );

		}
		if (!updateData)
		{
			m_oViewManager.SetRefresh(ImageRendering::Constants::sc_nRefreshForCommand);
			InvalidateRect ( invRect, TRUE );
		}
		
		DEV_LOG("InvalidateView END");
	}

	virtual void           FitImageToWindow()
	{
		DEV_LOG("FIT start");
		m_oViewManager.ZoomToWindow();
		DEV_LOG("FIT END");
	}

	virtual Gdiplus::RectF GetImageRect()
	{
		return Gdiplus::RectF(m_oViewManager.GetTransformLeft()-1, m_oViewManager.GetTransformTop()-1,
			m_oViewManager.GetTransformRight()-m_oViewManager.GetTransformLeft(),
			m_oViewManager.GetTransformBottom()-m_oViewManager.GetTransformTop());
	}

	virtual Gdiplus::Color GetBackgroundColor() 
	{
		return Gdiplus::Color(m_oViewManager.GetBackClearColor());
	}

	virtual Gdiplus::RectF GetWindowRect()
	{
		if (!::IsWindow(m_hWnd))
			return Gdiplus::RectF(0,0,0,0);
		RECT rct;
		::GetClientRect(m_hWnd, &rct);
		return Gdiplus::RectF(rct.left, rct.top, rct.right-rct.left, rct.bottom-rct.top);
	}

	virtual void           EnableDraw(bool enbl)
	{
		if (enbl)
			m_oViewManager.SetInterpolationMode(0);
		else
			m_oViewManager.SetInterpolationMode(-1);
	}
	
	HRESULT OnDraw(ATL_DRAWINFO& di)
	{
		RECT& rc = *(RECT*)di.prcBounds;
		//DEV_LOG("OnDraw start");		
		//DEV_LOG("OnDraw - 1");		

		m_oViewManager.SetHWND ( m_hWnd );

		if ( m_oViewManager.BeginPaint ( di.hdcDraw, rc ) )
		{
			if ( m_bCommandPrint )
			{
				CPrintCommand* pCommand = static_cast<CPrintCommand*> ( m_current_command );
				if ( pCommand )
				{
					LONG WindowW			=	abs ( rc.right - rc.left );
					LONG WindowH			=	abs ( rc.bottom - rc.top );

					if ( WindowW > 2 && WindowH > 2 )
					{					
						LONG Width			=	( GetDeviceCaps ( di.hdcDraw, LOGPIXELSX ) / 2.540005 ) * pCommand->GetPageW () * 0.1;
						LONG Height			=	( GetDeviceCaps ( di.hdcDraw, LOGPIXELSY ) / 2.540005 ) * pCommand->GetPageH () * 0.1;
						
						if ( Width > 2 && Height > 2 )
						{
							double Zoom		=	__min ( ( (double) WindowW - pCommand->GetIndent() ) / (double)Width, 
														( (double) WindowH - pCommand->GetIndent() ) / (double)Height );

							double OffsetX	=	(double) ( WindowW ) * 0.5 - (double) ( Width  ) * 0.5 * Zoom;
							double OffsetY	=	(double) ( WindowH ) * 0.5 - (double) ( Height ) * 0.5 * Zoom;

							m_oViewManager.SetUserRullers	( TRUE, WindowW, WindowH, Width, Height, Zoom, OffsetX, OffsetY );
							m_oViewManager.SetRefresh		( ImageRendering::Constants::sc_nRefreshForRebuild );
						}
					}
				}
			}

			//DEV_LOG("OnDraw - 2");		
			Gdiplus::Graphics* pPainter = m_oViewManager.GetPainter ();
			if ( pPainter && Gdiplus::Ok == pPainter->GetLastStatus())
			{
				//DEV_LOG("OnDraw - 3");		
				CRect rct;
				rct.left = m_oViewManager.GetStartDX();
				rct.right = m_oViewManager.GetEndDX();
				rct.top = m_oViewManager.GetStartDY();
				rct.bottom = m_oViewManager.GetEndDY();
				
				if (m_background_command && ((CBckgTonesCommand*)m_background_command)->isInteractive())
				{
					m_background_command->DrawOnGraphics(pPainter, rct);
				}
				else
				if (m_current_command && m_current_command->IsInvalidGraphics())
				{
					m_current_command->DrawOnGraphics(pPainter, rct);
				}
				else
				if (m_watermarks)
				{
					m_oViewManager.SetRefresh		( ImageRendering::Constants::sc_nRefreshForCommand );
					m_watermarks->Draw(pPainter);
				}
				//DEV_LOG("OnDraw - 5");		
			}

			//DEV_LOG("OnDraw - 6");		
			m_oViewManager.EndPaint ();
		}
		//DEV_LOG("OnDraw End");		
		return S_OK;
	}

	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		POINT point;

		point.x = GET_X_LPARAM(lParam);
		point.y = GET_Y_LPARAM(lParam);


 		if (m_background_command && ((CBckgTonesCommand*)m_background_command)->isInteractive())
 			m_background_command->OnMouseDown( point.x, point.y, (LONG)wParam );


		if (m_current_command)
			m_current_command->OnMouseDown( point.x, point.y, (LONG)wParam );
		else
			if (m_watermarks)
				m_watermarks->OnMouseDown( point.x, point.y, (LONG)wParam );

		return 0;
	}
	LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		POINT point;
	
		point.x = GET_X_LPARAM(lParam);
		point.y = GET_Y_LPARAM(lParam);

		/*
			if (m_background_command)
						m_background_command->OnMouseUp( point.x, point.y, (LONG)wParam );*/
			
		if (m_current_command)
			m_current_command->OnMouseUp( point.x, point.y, (LONG)wParam );
		else
			if (m_watermarks)
				m_watermarks->OnMouseUp( point.x, point.y, (LONG)wParam );
		
		return 0;
	}
	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		POINT point;

		point.x = GET_X_LPARAM(lParam);
		point.y = GET_Y_LPARAM(lParam);
		
		if (m_background_command && ((CBckgTonesCommand*)m_background_command)->isInteractive())
		{
			m_background_command->OnMouseMove( point.x, point.y, (int)wParam );
		}
		else	
			if (m_current_command)
			{
				m_current_command->OnMouseMove( point.x, point.y, (int)wParam );
				if (wParam & MK_MBUTTON)
				{
					m_oViewManager.SetRefresh(ImageRendering::Constants::sc_nRefreshForRebuild);
				}
				if ( m_bCommandPrint )
				{
					InvalidateView(false);
				}
			}
			else
			if (m_watermarks)
			{
				if (m_watermarks->OnMouseMove( point.x, point.y, (LONG)wParam ))
				{
					InvalidateView(false);
				}

			}
		
		return 0;
	}
	LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if (m_current_command)
			m_current_command->OnKeyDown( wParam, lParam );
		return 0;
	}
	LRESULT OnKeyUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if (m_current_command)
			m_current_command->OnKeyUp( wParam, lParam  );
		return 0;
	}

	LRESULT OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if (m_current_command)
			m_current_command->OnMouseLeave(   );
		return 0;
	}

	LRESULT OnTimer ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
	{
		POSITION pos = m_timers.GetStartPosition();

		while (NULL != pos)
		{
			CAtlMap<ITimerHandler*, UINT_PTR>::CPair* pPair = m_timers.GetNext(pos);

			if (NULL != pPair && pPair->m_value==wParam)
			   pPair->m_key->OnTimer();
		}

		return 0;
	}


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease() 
	{
	}



	STDMETHOD(IAVSImageEditor::Open)(BSTR filePath, VARIANT_BOOL* Success)
	{
		*Success = VARIANT_FALSE;
		CString filePathStr(filePath);
		m_oViewManager.SetHWND(m_hWnd);
		m_oLayersManager.Clear();

		DEV_LOG( "Load file %S ...", filePath );

		if (!m_watermarks)
			m_watermarks = new ImageEditor::CWatermarks(this);
		
		m_oLayersManager.SetWatermarks( m_watermarks );

		int errCode = 0;
		if (!filePathStr.IsEmpty() && m_oLayersManager.AddLayer(filePathStr,errCode))
		{
			long  imageW;
			long  imageH;
			double scaleC;
			const BYTE* imageData = m_oLayersManager.GetResultImage(imageW, imageH, scaleC);
			m_oViewManager.SetImage(imageData, imageW, imageH);
			m_oViewManager.SetCenterX ( imageW / 2 );
			m_oViewManager.SetCenterY ( imageH / 2 );
			Invalidate();
			*Success  = VARIANT_TRUE;
			
			DEV_LOG( "... loading SUCCESS!" );

			m_oLayersManager.GetCurrentLayer()->AddSubscriber((CBckgTonesCommand*)m_background_command);

			((CBckgTonesCommand*)m_background_command)->Reset();

			m_watermarks->SetImageSizes(imageW, imageH);

			return S_OK;
		}

		m_oViewManager.SetImage(NULL, 0, 0);
		m_oViewManager.SetRefresh ( ImageRendering::Constants::sc_nRefreshForRebuild );
		Invalidate();
		*Success  = VARIANT_TRUE;
		if (errCode==2)
			*Success  = VARIANT_FALSE;

		DEV_LOG( "... loading FAIL!" );

		m_watermarks->SetImageSizes(0, 0);
		return S_OK;
	}

	STDMETHOD(IAVSImageEditor::Save)(BSTR filePath, VARIANT_BOOL* Success)
	{
		*Success = m_oLayersManager.Save( CString(filePath) , m_watermarks)?VARIANT_TRUE:VARIANT_FALSE;
		if (*Success==VARIANT_TRUE)
		{
			//CString  logStr = CString("Save file ( ")+CString(filePath)+CString(" ) success!");
			//DEV_LOG(logStr.GetBuffer());
			//logStr.ReleaseBuffer();
		}
		else
		{
			//DEV_LOG("Error:  Save file failed!!! ");
		}
		return S_OK;
	}


	STDMETHOD(IAVSImageEditor::GetViewSettings)( IUnknown** pVal )
	{
		*pVal = NULL;

		CViewSettings*  pVSettings = static_cast<CViewSettings*>((void*)m_pViewSettings);
		if ( NULL != pVSettings )
		{
			pVSettings->SetViewPtr ( &m_oViewManager );
			m_pViewSettings->QueryInterface ( *&pVal );
		}

		DEV_LOG("GetViewSettings: result - success");

		return S_OK;
	}


	STDMETHOD(StartCommand)(long comID, VARIANT_BOOL* Success)
	{
		if (m_current_command)
		{
			delete m_current_command;
			m_current_command = NULL;
		}

		if (m_oLayersManager.GetCurrentLayer()==NULL)
		{
			*Success = VARIANT_FALSE;
			return S_OK;
		}

		*Success = VARIANT_TRUE;

		/*if (comID==0)
		{
			m_current_command = new CTestCommand(this);
			*Success = VARIANT_TRUE;
		}
		else*/
		//CString  logStr;
		//logStr.Format(_T("StartCommand with %i ID"), comID);
		//DEV_LOG(logStr.GetBuffer());
		//logStr.ReleaseBuffer();

		switch (comID)
		{
			case 0:
			case 100:
				m_current_command = new CStampCommand(this);	break;
			case 101:
				m_current_command = new CEraserCommand(this);	break;
			case 102:
				m_current_command = new CCropCommand(this);		break;
			case 103:
				m_current_command = new CResizeCommand(this);	break;
			case 104:
				m_current_command = new CBrushCommand(this);	break;
			case 105:
				m_current_command = new CRedEyeCommand(this);	break;
			case 109:
				m_current_command = new CXMLCommand(this);	    break;
			case 110:
				m_current_command = new CPrintCommand(this);	break;
			default:
				*Success = VARIANT_FALSE;
		}

		DEV_LOG("SUCCESS create command");

		if ( 110 == comID )	// print command enable
		{
			m_bCommandPrint	=	TRUE;
			
			m_oViewManager.SetInterpolationMode ( ImageRendering::PixelsUtils::sc_nInterpolationModeNone );
		}
		else
		{
			m_bCommandPrint	=	FALSE;
			
			m_oViewManager.SetInterpolationMode ( ImageRendering::PixelsUtils::sc_nInterpolationModeNearestNeighbor );

			m_oViewManager.SetUserRullers ( FALSE );
		}

		InvalidateView();
		
		if (m_current_command && m_current_command->IsInvalidGraphics())
		{
			DEV_LOG("Create Command invalidate start");
			InvalidateView();
			DEV_LOG("Create Command invalidate end");
		}
	
		return S_OK;
	}
	STDMETHOD(GetCommand)( IUnknown** pVal)
	{
		if (m_current_command)
		{
			m_current_command->GetCommandManagerIDisp((ICommand**)pVal);
			DEV_LOG("GetCommand: result - success");
		}
		else
		{
			*pVal = NULL;
			DEV_LOG("GetCommand: result - NULL");
		}
		return S_OK;
	}

	STDMETHOD(StopCommand)(VARIANT_BOOL goodStop, VARIANT_BOOL* Success)
	{
		DEV_LOG("StopCommand Start");
		
		*Success = VARIANT_TRUE;
		if (m_current_command)
		{
			if (goodStop==VARIANT_TRUE && m_current_command->IsApplyChangesAfterFinish())
			{
				DEV_LOG("StopCommand with TRUE branch start");
				//if (m_current_command->IsInvalidLayer())
				
				if (m_current_command->DrawOnLayer())
				{
					DEV_LOG("StopCommand with TRUE branch start - After DrawOnLayer");
					long wdth, hght, widthSlow, heightSlow;
					double scC;
					
					ILayer* pLayer = GetLayersManager()->GetCurrentLayer();
					
					const BYTE* imDataCur = pLayer->GetActualImage(wdth,hght, scC);
					const BYTE* imDataTmp = pLayer->GetImage(ILayer::SLOW, widthSlow,heightSlow);

					m_oViewManager.SetImage(imDataCur, wdth, hght, scC, widthSlow, heightSlow);
				}
				else
				{
					m_pViewSettings->Refresh();
					
					delete m_current_command;
					m_current_command = NULL;
					
					Invalidate();
					
					*Success = VARIANT_FALSE;
					return S_OK;
				}
				m_oViewManager.SetRefresh ( ImageRendering::Constants::sc_nRefreshForRebuild );	
				DEV_LOG("StopCommand with TRUE branch end");
			}
			else
			{
				DEV_LOG("StopCommand with FALSE branch start");
				m_oViewManager.SetRefresh ( ImageRendering::Constants::sc_nRefreshForRebuild );
				DEV_LOG("StopCommand with FALSE branch end");
			}
			
			m_pViewSettings->Refresh();
			
			delete m_current_command;
			m_current_command = NULL;
			
			Invalidate();
		}
		else
		{
			DEV_LOG("StopCommand: no command");
		}
		DEV_LOG("StopCommand End");


		return S_OK;
	}


	STDMETHOD(GetBackgroundCommand)( long comID,  IUnknown** pVal)
	{
		if (m_background_command)
		{
			m_background_command->GetCommandManagerIDisp((ICommand**)pVal);
			DEV_LOG("GetBackgroundCommand: result - success");
		}
		else
		{
			*pVal = NULL;
			DEV_LOG("GetBackgroundCommand: result - NULL");
		}
		return S_OK;
	}


	STDMETHOD(GetUndoStack)( IUnknown** pVal)
	{
		m_oLayersManager.GetUndoStackIDisp((IHistoryStack**)pVal);
		DEV_LOG("GetUndoStack");
		return S_OK;
	}

	STDMETHOD(GetRedoStack)( IUnknown** pVal)
	{
		m_oLayersManager.GetRedoStackIDisp((IHistoryStack**)pVal);
		DEV_LOG("GetRedoStack");
		return S_OK;
	}


	STDMETHOD(GetThumbnail)( IUnknown** pInterface)
	{
		long  imageW;
		long  imageH;
		const BYTE*  bitmap = GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::THUMBNAIL, imageW, imageH);

		if (!bitmap)
			return S_FALSE;

		*pInterface = NULL;

		MediaCore::IAVSUncompressedVideoFrame* pMediaData = NULL;

		CoCreateInstance(MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)(&pMediaData));
		if (NULL == pMediaData)
		{
			return FALSE;
		}

		// specify settings
		pMediaData->put_Width(imageW);
		pMediaData->put_Height(imageH);
		pMediaData->put_AspectRatioX(imageW);
		pMediaData->put_AspectRatioY(imageH);
		pMediaData->put_Interlaced(VARIANT_FALSE);
		pMediaData->put_Stride(0, 4*imageW);
		pMediaData->AllocateBuffer(4*imageW*imageH);

		// allocate necesasry buffer
		BYTE* pBufferPtr = 0;
		long nCreatedBufferSize = 0;
		pMediaData->get_Buffer(&pBufferPtr);
		pMediaData->get_BufferSize(&nCreatedBufferSize);
		pMediaData->put_Plane(0, pBufferPtr);

		// check for valid created buffer
		if (!pBufferPtr || nCreatedBufferSize != 4*imageW*imageH)
		{
			RELEASEINTERFACE(pMediaData);
			return FALSE;
		}

		memcpy(pBufferPtr, bitmap, nCreatedBufferSize);

		//if (m_watermarks)
		//{
		//	m_watermarks->DrawOnData(pBufferPtr,imageW,imageH);
		//}


		//  FLIP
		{
				int nLineSize = 4*imageW;
				BYTE* pBGRALineCopy = new BYTE[nLineSize];

				BYTE* pBGRALineTop = pBufferPtr;
				BYTE* pBGRALineBottom = pBufferPtr + nLineSize*(imageH - 1);

				for (int nY = 0; nY < imageH/2; ++nY)
				{
						memcpy(pBGRALineCopy, pBGRALineTop, nLineSize);
						memcpy(pBGRALineTop, pBGRALineBottom, nLineSize);
						memcpy(pBGRALineBottom, pBGRALineCopy, nLineSize);

						pBGRALineTop += nLineSize;
						pBGRALineBottom -= nLineSize;
				}

				delete[] pBGRALineCopy;
		}

		// save interface
		*pInterface = pMediaData;

		DEV_LOG("GetThumbnail");

		return S_OK;
	}

	STDMETHOD(GetHistogramm)( SAFEARRAY** pHistogramm)
	{
		if (!GetLayersManager()->GetCurrentLayer() || !GetLayersManager()->GetCurrentLayer()->GetHistogramm())
			return S_FALSE;
		
		SAFEARRAYBOUND bounds[1]; 
		bounds[0].cElements = 256+256+256+256; 
		bounds[0].lLbound = 0;
		
		*pHistogramm = SafeArrayCreate(VT_I4,1,bounds); 
		long * psadata = NULL; 
		
		SafeArrayAccessData(*pHistogramm,reinterpret_cast<void **>(&psadata)); 
		memcpy(psadata, GetLayersManager()->GetCurrentLayer()->GetHistogramm(), (256+256+256+256)*sizeof(long));   
		SafeArrayUnaccessData(*pHistogramm);   
		
		return S_OK;
	}


	STDMETHOD(SetMetaData)(BSTR bsMetaDataXML)
	{
		//CString sMetaDataXML(bsMetaDataXML);
		//m_oMetaData.SetXML(sMetaDataXML);
		return S_OK;
	}
	STDMETHOD(GetMetaData)(BSTR * bsMetaDataXML)
	{
		*bsMetaDataXML = m_oLayersManager.GetMetaData().AllocSysString();
		return S_OK;
	}


	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		CString sParamNameStr ( ParamName);

		if(sParamNameStr == _T("RCheck"))
		{
			int rcheck = ParamValue.intVal;
			DEV_LOG("SetAdditionalParam - RCheck");	
			m_oLayersManager.SetRegCheck(rcheck==1);
		}

		if(sParamNameStr == _T("FullScreen"))
		{
			int fs = ParamValue.intVal;

			if (m_watermarks)
				m_watermarks->SetEditRegime(fs==0);
		}

		if ( sParamNameStr == _T("refresh"))
		{
			DrawSurface ();

			return S_OK;
		}

		if (sParamNameStr.Find(_T("WatermarkParam"))>-1 && m_watermarks)
		{
			m_watermarks->SetParam(sParamNameStr, ParamValue.dblVal);
			
			//VARIANT_BOOL vbResult;
			//StopCommand( VARIANT_TRUE, &vbResult );
			
			InvalidateView();
		}
		else
			if (sParamNameStr.Find(_T("Watermark"))>-1 && m_watermarks)
			{
				m_watermarks->SetXML(sParamNameStr, &ParamValue.bstrVal);

				//VARIANT_BOOL vbResult;
				//StopCommand( VARIANT_TRUE, &vbResult );

				InvalidateView();
			}
		
		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		CString sParamNameStr ( ParamName);

		if (!ParamValue)
			return S_FALSE;
		VariantInit(ParamValue);
		if(sParamNameStr == _T("ImageWithoutEffects"))
		{
			long  imageW;
			long  imageH;
			const BYTE*  bitmap = GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::SLOW, imageW, imageH);

			if (!bitmap)
				return S_FALSE;

			MediaCore::IAVSUncompressedVideoFrame* pMediaData = NULL;

			CoCreateInstance(MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)(&pMediaData));
			if (NULL == pMediaData)
			{
				return FALSE;
			}

			// specify settings
			pMediaData->put_Width(imageW);
			pMediaData->put_Height(imageH);
			pMediaData->put_AspectRatioX(imageW);
			pMediaData->put_AspectRatioY(imageH);
			pMediaData->put_Interlaced(VARIANT_FALSE);
			pMediaData->put_Stride(0, 4*imageW);
			pMediaData->AllocateBuffer(4*imageW*imageH);

			// allocate necesasry buffer
			BYTE* pBufferPtr = 0;
			long nCreatedBufferSize = 0;
			pMediaData->get_Buffer(&pBufferPtr);
			pMediaData->get_BufferSize(&nCreatedBufferSize);
			pMediaData->put_Plane(0, pBufferPtr);

			// check for valid created buffer
			if (!pBufferPtr || nCreatedBufferSize != 4*imageW*imageH)
			{
				RELEASEINTERFACE(pMediaData);
				return FALSE;
			}

			memcpy(pBufferPtr, bitmap, nCreatedBufferSize);

			//  FLIP
			{
				int nLineSize = 4*imageW;
				BYTE* pBGRALineCopy = new BYTE[nLineSize];

				BYTE* pBGRALineTop = pBufferPtr;
				BYTE* pBGRALineBottom = pBufferPtr + nLineSize*(imageH - 1);

				for (int nY = 0; nY < imageH/2; ++nY)
				{
					memcpy(pBGRALineCopy, pBGRALineTop, nLineSize);
					memcpy(pBGRALineTop, pBGRALineBottom, nLineSize);
					memcpy(pBGRALineBottom, pBGRALineCopy, nLineSize);

					pBGRALineTop += nLineSize;
					pBGRALineBottom -= nLineSize;
				}

				delete[] pBGRALineCopy;
			}

			ParamValue->vt = VT_UNKNOWN;
			ParamValue->punkVal =  pMediaData;

		}
		else if(sParamNameStr == _T("DimensionX") || sParamNameStr == _T("DimensionY"))
		{
			long  imageW;
			long  imageH;
			if (GetLayersManager()->GetCurrentLayer())
			{
				const BYTE*  bitmap = GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::SLOW, imageW, imageH);
				if (bitmap)
				{
					ParamValue->vt = VT_I4;
					if (sParamNameStr == _T("DimensionX"))
						ParamValue->lVal = imageW;
					else 
						if (sParamNameStr == _T("DimensionY"))
							ParamValue->lVal = imageH;
					return S_OK;
				}
			}
			ParamValue->vt = VT_I4;
			ParamValue->lVal = 0;
		}
		else if(sParamNameStr == _T("DimensionXInch") || sParamNameStr == _T("DimensionYInch"))
		{
			long  imageW;
			long  imageH;
			if (GetLayersManager()->GetCurrentLayer())
			{
				const BYTE*  bitmap = GetLayersManager()->GetCurrentLayer()->GetImage(ILayer::SLOW, imageW, imageH);
				if (bitmap)
				{
					ParamValue->vt = VT_R8;
					if (sParamNameStr == _T("DimensionXInch"))
						ParamValue->dblVal = ((double)imageW)/72.0;
					else 
						if (sParamNameStr == _T("DimensionYInch"))
							ParamValue->dblVal = ((double)imageH)/72.0;
					return S_OK;
				}
			}
			ParamValue->vt = VT_R8;
			ParamValue->dblVal = 0;
		}
		else
		{
			if (GetLayersManager()->GetCurrentLayer())
			{
				const CAtlMap<CString,double>::CPair *pDblPair = pDblPair = GetLayersManager()->GetCurrentLayer()->GetDoublesInfos()->Lookup(sParamNameStr);
				if (pDblPair)
				{
					ParamValue->vt = VT_R8;
					ParamValue->dblVal = pDblPair->m_value;
					return S_OK;
				}
			}
		}
		//resolution, bit depth, color mode
		return S_OK;
	}


	STDMETHOD(IAVSImageEditor::LoadState)(BSTR filePath, VARIANT_BOOL* Success)
	{
		*Success = VARIANT_FALSE;
		CString filePathStr(filePath);
		
		if (m_background_command)
			if 	(((CBckgTonesCommand*)m_background_command)->LoadState(filePathStr))
				*Success = VARIANT_TRUE;

		CString avsPackFile = filePathStr;
		avsPackFile.Replace(_T(".xml"),_T(".avspack"));

		FILE*  fl = _tfopen(avsPackFile, _T("rb"));

		if (!fl)
		{
			*Success = VARIANT_FALSE;
			return S_FALSE;
		}

		long wdth = 0;
		long hght = 0;

		fread(&wdth, sizeof(wdth), 1, fl);
		fread(&hght, sizeof(hght), 1, fl);

		if (wdth==0 || hght==0)
		{
			*Success = VARIANT_FALSE;
			return S_FALSE;
		}

		BYTE* tmp_imData = new BYTE[wdth*hght*4];

		fread(tmp_imData, sizeof(BYTE), wdth*hght*4, fl);

		fclose(fl);

		m_oLayersManager.GetCurrentLayer()->StartHistoryBlock();
		m_oLayersManager.GetCurrentLayer()->ChangeAllLayer(tmp_imData, wdth, hght);
		m_oLayersManager.GetCurrentLayer()->StopHistoryBlock(_T("LoadState"));

		m_oLayersManager.GetCurrentLayer()->ChangeImage(ILayer::SLOW,NULL);
				
		return S_OK;
			
	}

	STDMETHOD(IAVSImageEditor::SaveState)(BSTR filePath, VARIANT_BOOL* Success)
	{
		*Success = VARIANT_FALSE;
		CString filePathStr(filePath);

		if (m_background_command)
			if 	(((CBckgTonesCommand*)m_background_command)->SaveState(filePathStr))
				*Success = VARIANT_TRUE;

		CString avsPackFile = filePathStr;
		avsPackFile.Replace(_T(".xml"),_T(".avspack"));

		FILE*  fl = _tfopen(avsPackFile, _T("wb"));

		if (!fl)
		{
			*Success = VARIANT_FALSE;
			return S_FALSE;
		}

		long wdth1, hght1;
		BYTE* tmp_imData = m_oLayersManager.GetCurrentLayer()->GetImage(ILayer::SLOW, wdth1,hght1);

		if (m_watermarks)
			m_watermarks->DrawOnData(tmp_imData, wdth1, hght1);

		fwrite(&wdth1, sizeof(wdth1), 1, fl);
		fwrite(&hght1, sizeof(hght1), 1, fl);
		fwrite(tmp_imData, sizeof(BYTE), wdth1*hght1*4, fl);

		fclose(fl);

		return S_OK;
	}

	STDMETHOD(IAVSImageEditor::FastSave)(BSTR avspackPath, BSTR resultPath,  VARIANT_BOOL* Success)
	{
		*Success = VARIANT_FALSE;

		FILE*  fl = _tfopen(CString(avspackPath), _T("rb"));

		long wdth1, hght1;
		BYTE* tmp_imData = NULL;

		fread(&wdth1, sizeof(wdth1), 1, fl);
		fread(&hght1, sizeof(hght1), 1, fl);
		tmp_imData = (BYTE*)malloc(sizeof(BYTE)*wdth1*hght1*4);
		fread(tmp_imData, sizeof(BYTE), wdth1*hght1*4, fl);

		fclose(fl);

		bool rslt = m_oLayersManager.SaveData(CString(resultPath), tmp_imData, wdth1, hght1,true,NULL/*m_watermarks*/,CString(_T("")));

		free(tmp_imData);

		m_oLayersManager.ClearHistories();

		if (rslt)
		{
			*Success = VARIANT_TRUE;
			return S_OK;
		}

		return S_FALSE;
	}




	/************************************************************************/
	/* APPLICATION MANAGER                                                  */
	/************************************************************************/
	virtual IView*             GetView()  {return this;}
	virtual ICoordinatSystem*  GetCoordinateTransformer()  {return (ICoordinatSystem*)(&m_oViewManager);}

	virtual ILayersManager*     GetLayersManager()    {return (ILayersManager*)&m_oLayersManager;}

	virtual ITimer*            GetTimer()  {return this;};
	virtual IProgresser*       GetProgresser() {return this;};
	virtual ImageStudio3::IImageTransformsPtr   GetImageStudio()  {return m_image_studio;}
	/************************************************************************/
	/* ITIMER interface                                                     */
	/************************************************************************/
	virtual void StartTimer(ITimerHandler* timerHdlr, int timeStepInMillisec)
	{
		CAtlMap<ITimerHandler*, UINT_PTR>::CPair* pPair = m_timers.Lookup(timerHdlr);

		if (pPair!=NULL)
			StopTimer(pPair);

		UINT_PTR timerID = SetTimer(reinterpret_cast<UINT_PTR>(timerHdlr), timeStepInMillisec);
        m_timers.SetAt(timerHdlr, timerID);
	}
	virtual void StopTimer(ITimerHandler* timerHdlr)
	{
		CAtlMap<ITimerHandler*, UINT_PTR>::CPair* pPair = m_timers.Lookup(timerHdlr);
		StopTimer(pPair);
	}
	void StopTimer(CAtlMap<ITimerHandler*, UINT_PTR>::CPair* mapEl)
	{
		if (!::IsWindow(m_hWnd))
			return;
		if (mapEl)
		{
			KillTimer(mapEl->m_value);
			m_timers.RemoveKey(mapEl->m_key);
		}
	}
	void StopAllTimers()
	{
		if (!::IsWindow(m_hWnd))
			return;
		POSITION pos = m_timers.GetStartPosition();

		while (NULL != pos)
		{
			CAtlMap<ITimerHandler*, UINT_PTR>::CPair* pPair = m_timers.GetNext(pos);
			KillTimer(pPair->m_value);
		}
		m_timers.RemoveAll();
	}
	/************************************************************************/
	/* PROGRESSER                                                           */
	/************************************************************************/
	virtual void StartProgress()
	{
		DEV_LOG("Start Progress Event");

		m_nProgressPercent = -1;

		OnStartProgress();
	}
	virtual void Progress(LONG percent)
	{
		if (percent != m_nProgressPercent)
		{
			m_nProgressPercent = percent;

			DEV_LOG("Progress - %i %%", percent);

			OnProgress(percent);
		}
	}
	virtual void FinishProgress()
	{
		DEV_LOG("Finish Progress Event");

		OnFinishProgress();
	}


	virtual void  OnUndo(int stepsCount, bool asReset)
	{
		if (m_current_command)
			m_current_command->Undo(stepsCount);
	}
	virtual void  OnRedo(int stepsCount)
	{
		if (m_current_command)
			m_current_command->Redo(stepsCount);
	}



	virtual void SendMsg(int msgID, void* messageData)
	{
		switch (msgID)
		{
		case WM_IEM_UPDATE_THUMBNAIL:
			{
				OnChangePreview();
			}
			break;
		case WM_IEM_THREAD_FINISHED:
			{
				::PostMessage(m_hWnd, WM_IEM_THREAD_FINISHED, 0, 0);
			}
			break;
		case WM_IEM_STOP_COMMAND_YES:
			{
				::PostMessage(m_hWnd, WM_IEM_STOP_COMMAND_YES, 0, 0);
			}
			break;
		case WM_IEM_STOP_COMMAND_NO:
			{
				::PostMessage(m_hWnd, WM_IEM_STOP_COMMAND_NO, 0, 0);
			}
			break;
		default:
			break;
		}
	}

	LRESULT OnFinishUpdateThread ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
	{
		if (m_background_command)
			((CBckgTonesCommand*)m_background_command)->AfterThreadProc();

		return 0;
	}

	LRESULT OnStopCommandGood ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
	{
		if (m_current_command)
			m_current_command->RestartEvent(true);
		return TRUE;
	}
	LRESULT OnStopCommandBad ( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
	{
		if (m_current_command)
			m_current_command->RestartEvent(false);
		return TRUE;
	}

	virtual void   ApplyBackgroundCommands(BYTE* pPixels, int nWidth, int nHeight, bool scaleParams = true)
	{
		if (m_background_command)
			((CBckgTonesCommand*)m_background_command)->ApplyToImage(pPixels, nWidth, nHeight, scaleParams);
	}

	virtual void   ResizeImage(int nWidth, int nHeight)
	{
		if (m_watermarks)
			m_watermarks->SetImageSizes(nWidth, nHeight);
	}

	virtual bool   IsBackgroundCommandOnFullImage()
	{
		if (m_background_command)
			return ((CBckgTonesCommand*)m_background_command)->IsBackgroundCommandOnFullImage();
		
		return false;
	}
	virtual void   EnableInterfaceControls(bool enbl)
	{
		if (m_background_command)
		{
			if (enbl)
				m_background_command->SwitchViewToSlow();
			else
				m_background_command->SwitchViewToFast();
		}
	}

	
	BOOL DrawSurface ()
	{
		RECT rect; ::GetClientRect ( m_hWnd, &rect );
		if ( rect.right - rect.left < 1 || rect.bottom - rect.top < 1 )
			return FALSE;

		m_oViewManager.SetHWND ( m_hWnd );

		HDC	hPreviewDC =  GetDC ();
		if ( m_oViewManager.BeginPaint ( hPreviewDC, rect ) )
		{
			//DEV_LOG("OnDraw - 2");		
			Gdiplus::Graphics* pPainter = m_oViewManager.GetPainter ();
			if ( pPainter && Gdiplus::Ok == pPainter->GetLastStatus())
			{
				//DEV_LOG("OnDraw - 3");		
				CRect rct;
				rct.left = m_oViewManager.GetStartDX();
				rct.right = m_oViewManager.GetEndDX();
				rct.top = m_oViewManager.GetStartDY();
				rct.bottom = m_oViewManager.GetEndDY();

				if (m_background_command && ((CBckgTonesCommand*)m_background_command)->isInteractive())
				{
					m_background_command->DrawOnGraphics(pPainter, rct);
				}
				else		
					if (m_current_command && m_current_command->IsInvalidGraphics())
						m_current_command->DrawOnGraphics(pPainter, rct);
					else
						if (m_watermarks)
						{
							m_oViewManager.SetRefresh		( ImageRendering::Constants::sc_nRefreshForCommand );
							m_watermarks->Draw(pPainter);
						}
				//DEV_LOG("OnDraw - 5");		
			}

			//DEV_LOG("OnDraw - 6");		
			m_oViewManager.EndPaint ();
		}
		//DEV_LOG("OnDraw End");		
		
		ReleaseDC(hPreviewDC);

		return TRUE;
	}

private:
	ImageEditor::Layers::CLayersManager m_oLayersManager;

	ImageRendering::CViewManager		m_oViewManager;
	CComPtr < IViewSettings >			m_pViewSettings;

	CCommand*                           m_current_command;
	CCommand*                           m_background_command;

	ImageStudio3::IImageTransformsPtr   m_image_studio; // not used

	CAtlMap<ITimerHandler*, UINT_PTR>   m_timers;

	ImageEditor::CWatermarks*			m_watermarks;
	BOOL								m_bCommandPrint;
	
	LONG m_nProgressPercent;
};

