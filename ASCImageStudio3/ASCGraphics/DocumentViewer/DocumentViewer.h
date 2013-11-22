#pragma once
#include "..\stdafx.h"
#include "..\resource.h"       // main symbols
#include <atlctl.h>
#include "..\Interfaces\BaseThread.h"

#include "Viewer.h"
#include "PropertyManager.h"

#include "Timer.h"
using namespace NSDocumentViewer;

#include "..\agg\AVSWrapper\Graphics.h"
#include "..\Interfaces\AVSDocumentRenderer.h"
#include "OfficeSource.h"
#include "..\Objects\AVSWinFonts.h"

#include "Interfaces.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

const LONG g_lPropertyIndexViewMode			= 0;
const LONG g_lPropertyIndexZoom				= 1;
const LONG g_lPropertyIndexPositionHor		= 2;
const LONG g_lPropertyIndexPositionVer		= 3;
const LONG g_lPropertyIndexCurrentPage		= 4;
const LONG g_lPropertyIndexPagesInBlock		= 5;
const LONG g_lPropertyIndexControlSizeX		= 6;
const LONG g_lPropertyIndexControlSizeY		= 7;
const LONG g_lPropertyIndexAddPage			= 8;

const LONG g_lPropertiesCount				= 9;

// AVSDocumentViewer
[
	coclass,
	control,
	default(IAVSDocumentViewer, _IAVSDocumentViewerEvents),
	threading(apartment),
	event_source(com),
	vi_progid("AVSDocumentViewer.DocumentViewer"),
	progid("AVSDocumentViewer.DocumentViewer.1"),
	version(1.0),
	uuid("934FBEF6-A4FB-4c82-A8C7-7A188AAAAAC2"),
	support_error_info(IAVSDocumentViewer),
	registration_script("control.rgs")
]
class ATL_NO_VTABLE CAVSDocumentViewer :
	public IAVSDocumentViewer,
	public IAVSRenderer,
	public IPersistStreamInitImpl<CAVSDocumentViewer>,
	public IOleControlImpl<CAVSDocumentViewer>,
	public IOleObjectImpl<CAVSDocumentViewer>,
	public IOleInPlaceActiveObjectImpl<CAVSDocumentViewer>,
	public IViewObjectExImpl<CAVSDocumentViewer>,
	public IOleInPlaceObjectWindowlessImpl<CAVSDocumentViewer>,
	public IPersistStorageImpl<CAVSDocumentViewer>,
	public ISpecifyPropertyPagesImpl<CAVSDocumentViewer>,
	public IQuickActivateImpl<CAVSDocumentViewer>,
	public IProvideClassInfo2Impl<&__uuidof(CAVSDocumentViewer), NULL>,
	public CEventsSource,
	public CTimer<CAVSDocumentViewer, IAVSDocumentViewer, &__uuidof(IAVSDocumentViewer)>,
#ifndef _WIN32_WCE
	public IDataObjectImpl<CAVSDocumentViewer>,
#endif
#ifdef _WIN32_WCE // IObjectSafety is required on Windows CE for the control to be loaded correctly
	public IObjectSafetyImpl<CAVSDocumentViewer, INTERFACESAFE_FOR_UNTRUSTED_CALLER>,
#endif
	public CComControl<CAVSDocumentViewer>
{
public:

DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE | OLEMISC_CANTLINKINSIDE |	OLEMISC_INSIDEOUT |	OLEMISC_ACTIVATEWHENVISIBLE | OLEMISC_SETCLIENTSITEFIRST)

BEGIN_PROP_MAP(CAVSDocumentViewer)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
END_PROP_MAP()

BEGIN_MSG_MAP(CAVSDocumentViewer)
	//CHAIN_MSG_MAP(CComControl<CAVSDocumentViewer>)
	MESSAGE_RANGE_HANDLER(0, 0xFFFFFFFF, OnMessages)
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()

// IViewObjectEx
DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

public:
	__event __interface _IAVSDocumentViewerEvents;

private:
	
	ViewMode m_eViewMode;
	CDocument m_oDocument;
	
	double m_dZoom;
	
	double m_dDocumentWidthMM;
	double m_dDocumentHeightMM;

	double m_dDocumentWidthPix;
	double m_dDocumentHeightPix;

	double m_dPercentHor;
	double m_dPercentVer;

	double m_dDpiX;
	double m_dDpiY;

	LONG	m_lPageBetweenOffset;
	DWORD	m_dwBackgroundColor;

	CPainter				m_oPainter;
	NSDocumentViewer::CDIB*	m_pPixels;
	LONG					m_lWidth;
	LONG					m_lHeight;

	ULONGLONG	m_lDocumentWidth;
	ULONGLONG	m_lDocumentHeight;

	Aggplus::CGraphics   m_oGraphics;

	//CAtlArray<CDrawingPage> m_arPages;
	CDrawingDocument m_oCalculateDocument;
	CDrawingDocument m_oRenderingDocument;

	// временные переменные. надо подключать командрендерер
	double m_dLastPageWidth;
	double m_dLastPageHeight;

	// для мышки
	bool m_bIsMove;
	LONG m_lOldCursorX;
	LONG m_lOldCursorY;

	double m_dOldPositionHor;
	double m_dOldPositionVer;

	// для мышечной прокрутки - дубликат PositionVer, меняющийся при mousemove
	double m_dOldPositionVer2;

	double m_dKoefMousePositionHor;
	double m_dKoefMousePositionVer;

	// для навигации по страницам
	// должен проставляться при Calculate
	LONG m_lCurrentPage;

	// рамка. с ней смотрится гораздо красивей
	LONG m_lBorderLeft;
	LONG m_lBorderTop;
	LONG m_lBorderRight;
	LONG m_lBorderBottom;

	// текущий блок - 
	// первый и последний
	// и флаг учета этих блоков
	LONG m_lStartBlock;
	LONG m_lEndBlock;
	BOOL m_bNoBlockUsed;

	// флаг отрисовщика
	BOOL m_bIsRepaint;

	BOOL m_bIsDeleted;

	// отрисовщик
	BOOL m_bBreakFlag;

	IAVSMetafile* m_pCurrentPage;

	// менеджер настроек
	CPropertyManager m_oPropertyManager;

	// sorc
	COfficeSource m_oSource;

	// кэш
	CCacheDocument m_oCacheDocument;

	// нужен для фиксации позиции при подгрузке страниц
	ULONGLONG m_lOldDocumentHeight;

	// вертикальная позиция в пикселах
	ULONGLONG m_lPositionVer;		// первая видимая линия
	ULONGLONG m_lOldPositionVer;	// старая позиция

	// синхронизатор
	CRITICAL_SECTION m_oCS;

	// Thumbnails...
	IAVSThumbnailsViewer* m_pThumbnailsViewer;

	// этот конролл должен быть создан один раз!!!
	CThreadFonts m_oFonts;

	// флаг, посылки наверх позиций скролла
	BOOL m_bIsSendScrollPosition;

	// синхронизатор смоны сурса (чтобы не прыгало ничего)
	CRITICAL_SECTION m_oCS_ChangeSource;
	BOOL m_bIsSetSource;

	// задержка для скроллинга (вверх / низ)
	BOOL m_bIsRepaintPixels;
	LONG m_lCurrentVerCount;
	LONG m_lCurrentHorCount;

	LONG m_lMaxVerCount;
	LONG m_lMaxHorCount;

private:
	LRESULT OnMessages(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void OnCreate();
	void OnDestroy();
	void OnPaint(BOOL bVal = TRUE);
	void OnPaintDC(HDC hdc);
	void OnSize();
	void OnMove();
	void OnMouseWhell(LONG lOffset);
	void OnKeyDown(LONG lKey, DWORD dwData);
	   
	HRESULT OnLButtonDown(long xPos, long yPos);
	HRESULT OnLButtonUp();
	HRESULT OnMouseMove(long xPos, long yPos);

	BOOL UpdatePixels(LONG lSizeX, LONG lSizeY);

public:
	
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	CAVSDocumentViewer() : m_oCalculateDocument(), m_oRenderingDocument(), m_oPropertyManager(g_lPropertiesCount), m_oSource(NULL)
	{
		m_dwBackgroundColor = 0xFF676767;

		m_pPixels			= NULL;
		m_lWidth			= -1;
		m_lHeight			= -1;

		m_dDocumentWidthMM	= 0;
		m_dDocumentHeightMM	= 0;

		m_lDocumentWidth	= 0;
		m_lDocumentHeight	= 0;

		m_dLastPageWidth	= 0;
		m_dLastPageHeight	= 0;

		m_eViewMode			= FitWidth;
		m_dZoom				= 1.0;

		m_lPageBetweenOffset = 10;

		m_dPercentHor		= 0;
		m_dPercentVer		= 0;

		m_dDpiX				= 96;
		m_dDpiY				= 96;

		m_bIsMove = false;

		m_dKoefMousePositionHor = 0;
		m_dKoefMousePositionVer = 0;

		m_lCurrentPage		= 0;

		m_lBorderLeft		= 5;
		m_lBorderTop		= 5;
		m_lBorderRight		= 5;
		m_lBorderBottom		= 5;

		m_lStartBlock		= 0;
		m_lEndBlock			= 0;
		m_bNoBlockUsed		= FALSE;
		
		m_pCurrentPage = NULL;
		CoCreateInstance(__uuidof(CAVSMetafile), NULL, CLSCTX_INPROC, __uuidof(IAVSMetafile), (void**)&m_pCurrentPage);

		m_oCacheDocument.InitFonts();

		m_bIsDeleted = FALSE;

		m_bBreakFlag = FALSE;

		m_lPositionVer = 0;

		m_pThumbnailsViewer = NULL;

		m_bIsSendScrollPosition = TRUE;
		
		InitializeCriticalSection(&m_oCS);
		InitializeCriticalSection(&m_oCS_ChangeSource);

		m_bIsSetSource = FALSE;

		m_bIsRepaintPixels = FALSE;

		m_lCurrentVerCount	= 0;
		m_lCurrentHorCount	= 0;

		m_lMaxVerCount		= 2;
		m_lMaxHorCount		= 2;
	}

	void FinalRelease()
	{
		m_bIsDeleted = TRUE;
		
		APITimerOff();

		RELEASEOBJECT(m_pPixels);

		RELEASEINTERFACE(m_pCurrentPage);

		RELEASEINTERFACE(m_pThumbnailsViewer);

		DeleteCriticalSection(&m_oCS);
		DeleteCriticalSection(&m_oCS_ChangeSource);
	}

	~CAVSDocumentViewer()
	{
	}

protected:
	void Calculate(BOOL bIsUseRepaint = TRUE);
	void CalculateDocumentSize();
	void PaintPixels();
	void DrawPage(size_t nIndexPage);
	void AddPage();
	void CheckZoom(BOOL bPutZoom = TRUE);
	BOOL CheckZoom2();
	double GetCurrentZoom();
	virtual	void OnTimer();

	inline void BlitPixels();
	inline void BlitPixelsDC(HDC hdc);

public:

	// AVSDocumentViewer
	STDMETHOD(get_ViewMode)(LONG* lMode);
	STDMETHOD(put_ViewMode)(LONG lMode);

	STDMETHOD(get_Zoom)(double* dZoom);
	STDMETHOD(put_Zoom)(double dZoom);

	STDMETHOD(get_PositionHor)(double* lMode);
	STDMETHOD(put_PositionHor)(double lMode);

	STDMETHOD(get_PositionVer)(double* lMode);
	STDMETHOD(put_PositionVer)(double lMode);

	STDMETHOD(get_ControlWidth)(LONG* lWidth);
	STDMETHOD(get_ControlHeight)(LONG* lHeight);
	STDMETHOD(get_DocumentWidth)(double* dWidth);
	STDMETHOD(get_DocumentHeight)(double* dHeight);

	STDMETHOD(get_CountPages)(LONG* lCountPages);
	
	STDMETHOD(get_CurrentPage)(LONG* lCurrentPage);
	STDMETHOD(put_CurrentPage)(LONG lCurrentPage);

	STDMETHOD(get_PagesInBlock)(LONG* lCurrentPage);
	STDMETHOD(put_PagesInBlock)(LONG lCurrentPage);

	STDMETHOD(CloseFile)();
	STDMETHOD(Init)();

	STDMETHOD(_OnTimer)();

// ------------------------------------ аналог AVSDocumentRenderer'a ------------------------------------
	STDMETHOD(get_Type)(LONG* lType)
	{
		return S_OK;
	}
	//-------- Функции для работы со страницей --------------------------------------------------
	STDMETHOD(NewPage)()
	{
		return S_OK;
	}
	STDMETHOD(get_Height)(double* dHeight)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->get_Height(dHeight);
	}
	STDMETHOD(put_Height)(double dHeight)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->put_Height(dHeight);
	}
	STDMETHOD(get_Width)(double* dWidth)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->get_Width(dWidth);
	}
	STDMETHOD(put_Width)(double dWidth)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->put_Width(dWidth);
	}

	STDMETHOD(get_DpiX)(double* dDpiX)
	{
		if (NULL == dDpiX)
			return S_FALSE;

		*dDpiX = m_dDpiX;
		return S_OK;
	}
	STDMETHOD(get_DpiY)(double* dDpiY)
	{
		if (NULL == dDpiY)
			return S_FALSE;

		*dDpiY = m_dDpiY;
		return S_OK;
	}

// pen --------------------------------------------------------------------------------------
	STDMETHOD(SetPen)(BSTR bsXML)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->SetPen(bsXML);
	}
	STDMETHOD(get_PenColor)(LONG* lColor)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->get_PenColor(lColor);
	}
	STDMETHOD(put_PenColor)(LONG lColor)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->put_PenColor(lColor);
	}
	STDMETHOD(get_PenAlpha)(LONG* lAlpha)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->get_PenAlpha(lAlpha);
	}
	STDMETHOD(put_PenAlpha)(LONG lAlpha)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->put_PenAlpha(lAlpha);
	}
	STDMETHOD(get_PenSize)(double* dSize)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->get_PenSize(dSize);
	}
	STDMETHOD(put_PenSize)(double dSize)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->put_PenSize(dSize);
	}
	STDMETHOD(get_PenDashStyle)(BYTE* val)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->get_PenDashStyle(val);
	}
	STDMETHOD(put_PenDashStyle)(BYTE val)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->put_PenDashStyle(val);
	}
	STDMETHOD(get_PenLineStartCap)(BYTE* val)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->get_PenLineStartCap(val);
	}
	STDMETHOD(put_PenLineStartCap)(BYTE val)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->put_PenLineStartCap(val);
	}
	STDMETHOD(get_PenLineEndCap)(BYTE* val)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->get_PenLineEndCap(val);
	}
	STDMETHOD(put_PenLineEndCap)(BYTE val)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->put_PenLineEndCap(val);
	}
	STDMETHOD(get_PenLineJoin)(BYTE* val)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->get_PenLineJoin(val);
	}
	STDMETHOD(put_PenLineJoin)(BYTE val)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->put_PenLineJoin(val);
	}
	STDMETHOD(get_PenDashOffset)(double* val)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->get_PenDashOffset(val);
	}
	STDMETHOD(put_PenDashOffset)(double val)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->put_PenDashOffset(val);
	}
	STDMETHOD(get_PenAlign)(LONG* val)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->get_PenAlign(val);
	}
	STDMETHOD(put_PenAlign)(LONG val)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->put_PenAlign(val);
	}
	STDMETHOD(get_PenMiterLimit)(double* val)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->get_PenMiterLimit(val);
	}
	STDMETHOD(put_PenMiterLimit)(double val)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->put_PenMiterLimit(val);
	}
	STDMETHOD(PenDashPattern)(SAFEARRAY* pPattern)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->PenDashPattern(pPattern);
	}

// brush ------------------------------------------------------------------------------------
	STDMETHOD(SetBrush)(BSTR bsXML)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->SetBrush(bsXML);
	}
	STDMETHOD(get_BrushType)(LONG* lType)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->get_BrushType(lType);
	}
	STDMETHOD(put_BrushType)(LONG lType)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->put_BrushType(lType);
	}
	STDMETHOD(get_BrushColor1)(LONG* lColor)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->get_BrushColor1(lColor);
	}
	STDMETHOD(put_BrushColor1)(LONG lColor)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->put_BrushColor1(lColor);
	}
	STDMETHOD(get_BrushAlpha1)(LONG* lAlpha)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->get_BrushAlpha1(lAlpha);
	}
	STDMETHOD(put_BrushAlpha1)(LONG lAlpha)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->put_BrushAlpha1(lAlpha);
	}
	STDMETHOD(get_BrushColor2)(LONG* lColor)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->get_BrushColor2(lColor);
	}
	STDMETHOD(put_BrushColor2)(LONG lColor)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->put_BrushColor2(lColor);
	}
	STDMETHOD(get_BrushAlpha2)(LONG* lAlpha)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->get_BrushAlpha2(lAlpha);
	}
	STDMETHOD(put_BrushAlpha2)(LONG lAlpha)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->put_BrushAlpha2(lAlpha);
	}
	STDMETHOD(get_BrushTexturePath)(BSTR* bsPath)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->get_BrushTexturePath(bsPath);
	}
	STDMETHOD(put_BrushTexturePath)(BSTR bsPath)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->put_BrushTexturePath(bsPath);
	}
	STDMETHOD(get_BrushTextureMode)(LONG* lMode)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->get_BrushTextureMode(lMode);
	}
	STDMETHOD(put_BrushTextureMode)(LONG lMode)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->put_BrushTextureMode(lMode);
	}
	STDMETHOD(get_BrushTextureAlpha)(LONG* lTxAlpha)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->get_BrushTextureAlpha(lTxAlpha);
	}
	STDMETHOD(put_BrushTextureAlpha)(LONG lTxAlpha)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->put_BrushTextureAlpha(lTxAlpha);
	}
	STDMETHOD(get_BrushLinearAngle)(double* dAngle)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->get_BrushLinearAngle(dAngle);
	}
	STDMETHOD(put_BrushLinearAngle)(double dAngle)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->put_BrushLinearAngle(dAngle);
	}
	STDMETHOD(BrushRect)(BOOL val, double left, double top, double width, double height)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->BrushRect(val, left, top, width, height);
	}

// font -------------------------------------------------------------------------------------
	STDMETHOD(SetFont)(BSTR bsXML)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->SetFont(bsXML);
	}
	STDMETHOD(get_FontName)(BSTR* bsName)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->get_FontName(bsName);
	}
	STDMETHOD(put_FontName)(BSTR bsName)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->put_FontName(bsName);
	}
	STDMETHOD(get_FontPath)(BSTR* bsName)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->get_FontPath(bsName);
	}
	STDMETHOD(put_FontPath)(BSTR bsName)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->put_FontPath(bsName);
	}
	STDMETHOD(get_FontSize)(double* dSize)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->get_FontSize(dSize);
	}
	STDMETHOD(put_FontSize)(double dSize)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->put_FontSize(dSize);
	}
	STDMETHOD(get_FontStyle)(LONG* lStyle)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->get_FontStyle(lStyle);
	}
	STDMETHOD(put_FontStyle)(LONG lStyle)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->put_FontStyle(lStyle);
	}
	STDMETHOD(get_FontStringGID)(BOOL* bGID)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->get_FontStringGID(bGID);
	}
	STDMETHOD(put_FontStringGID)(BOOL bGID)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->put_FontStringGID(bGID);
	}
	STDMETHOD(get_FontCharSpace)(double* dSpace)
	{
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->get_FontCharSpace(dSpace);
	}
	STDMETHOD(put_FontCharSpace)(double dSpace)
	{
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->put_FontCharSpace(dSpace);
	}

// shadow -----------------------------------------------------------------------------------
	STDMETHOD(SetShadow)(BSTR bsXML)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->SetShadow(bsXML);
	}
	STDMETHOD(get_ShadowDistanceX)(double* val)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->get_ShadowDistanceX(val);
	}
	STDMETHOD(put_ShadowDistanceX)(double val)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->put_ShadowDistanceX(val);
	}
	STDMETHOD(get_ShadowDistanceY)(double* val)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->get_ShadowDistanceY(val);
	}
	STDMETHOD(put_ShadowDistanceY)(double val)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->put_ShadowDistanceY(val);
	}
	STDMETHOD(get_ShadowBlurSize)(double* val)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->get_ShadowBlurSize(val);
	}
	STDMETHOD(put_ShadowBlurSize)(double val)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->put_ShadowBlurSize(val);
	}
	STDMETHOD(get_ShadowColor)(LONG* val)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->get_ShadowColor(val);
	}
	STDMETHOD(put_ShadowColor)(LONG val)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->put_ShadowColor(val);
	}
	STDMETHOD(get_ShadowAlpha)(LONG* val)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->get_ShadowAlpha(val);
	}
	STDMETHOD(put_ShadowAlpha)(LONG val)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->put_ShadowAlpha(val);
	}
	STDMETHOD(get_ShadowVisible)(BOOL* val)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->get_ShadowVisible(val);
	}
	STDMETHOD(put_ShadowVisible)(BOOL val)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->put_ShadowVisible(val);
	}

// edge -------------------------------------------------------------------------------------
	STDMETHOD(SetEdgeText)(BSTR bsXML)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->SetEdgeText(bsXML);
	}
	STDMETHOD(get_EdgeVisible)(LONG* val)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->get_EdgeVisible(val);
	}
	STDMETHOD(put_EdgeVisible)(LONG val)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->put_EdgeVisible(val);
	}
	STDMETHOD(get_EdgeColor)(LONG* val)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->get_EdgeColor(val);
	}
	STDMETHOD(put_EdgeColor)(LONG val)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->put_EdgeColor(val);
	}
	STDMETHOD(get_EdgeAlpha)(LONG* val)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->get_EdgeAlpha(val);
	}
	STDMETHOD(put_EdgeAlpha)(LONG val)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->put_EdgeAlpha(val);
	}
	STDMETHOD(get_EdgeDist)(double* val)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->get_EdgeDist(val);
	}
	STDMETHOD(put_EdgeDist)(double val)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->put_EdgeDist(val);
	}

//-------- Функции для вывода текста --------------------------------------------------------
	STDMETHOD(CommandDrawText)(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->CommandDrawText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);
	}
	STDMETHOD(CommandDrawTextEx)(BSTR bsText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
	{
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->CommandDrawTextEx(bsText, bsGidText, bsSourceCodeText, fX, fY, fWidth, fHeight, fBaseLineOffset, lFlags);
	}

//-------- Маркеры для команд ---------------------------------------------------------------
	STDMETHOD(BeginCommand)(DWORD lType)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->BeginCommand(lType);
	}
	STDMETHOD(EndCommand)(DWORD lType)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->EndCommand(lType);
	}

//-------- Функции для работы с Graphics Path -----------------------------------------------
	STDMETHOD(PathCommandMoveTo)(double fX, double fY)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->PathCommandMoveTo(fX, fY);
	}
	STDMETHOD(PathCommandLineTo)(double fX, double fY)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->PathCommandLineTo(fX, fY);
	}
	STDMETHOD(PathCommandLinesTo)(SAFEARRAY* pPoints)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->PathCommandLinesTo(pPoints);
	}
	STDMETHOD(PathCommandCurveTo)(double fX1, double fY1, double fX2, double fY2, double fX3, double fY3)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->PathCommandCurveTo(fX1, fY1, fX2, fY2, fX3, fY3);
	}
	STDMETHOD(PathCommandCurvesTo)(SAFEARRAY* pPoints)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->PathCommandCurvesTo(pPoints);
	}
	STDMETHOD(PathCommandArcTo)(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->PathCommandArcTo(fX, fY, fWidth, fHeight, fStartAngle, fSweepAngle);
	}
	STDMETHOD(PathCommandClose)()
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->PathCommandClose();
	}
	STDMETHOD(PathCommandEnd)()
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->PathCommandEnd();
	}
	STDMETHOD(DrawPath)(long nType)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->DrawPath(nType);
	}
	STDMETHOD(PathCommandStart)()
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->PathCommandStart();
	}
	STDMETHOD(PathCommandGetCurrentPoint)(double* fX, double* fY)
	{
		return S_OK;
	}
	STDMETHOD(PathCommandText)(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->PathCommandText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);
	}
	STDMETHOD(PathCommandTextEx)(BSTR bsText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
	{
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->PathCommandTextEx(bsText, bsGidText, bsSourceCodeText, fX, fY, fWidth, fHeight, fBaseLineOffset, lFlags);
	}

//-------- Функции для вывода изображений ---------------------------------------------------
	STDMETHOD(DrawImage)(IUnknown* pInterface, double fX, double fY, double fWidth, double fHeight)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->DrawImage(pInterface, fX, fY, fWidth, fHeight);
	}
	STDMETHOD(DrawImageFromFile)(BSTR bstrVal, double fX, double fY, double fWidth, double fHeight)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->DrawImageFromFile(bstrVal, fX, fY, fWidth, fHeight);
	}

// transform --------------------------------------------------------------------------------
	STDMETHOD(GetCommandParams)(double* dAngle, double* dLeft, double* dTop, double* dWidth, double* dHeight, DWORD* lFlags)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->GetCommandParams(dAngle, dLeft, dTop, dWidth, dHeight, lFlags);
	}
	STDMETHOD(SetCommandParams)(double dAngle, double dLeft, double dTop, double dWidth, double dHeight, DWORD lFlags)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->SetCommandParams(dAngle, dLeft, dTop, dWidth, dHeight, lFlags);
	}

	STDMETHOD(SetTransform)(double dA, double dB, double dC, double dD, double dE, double dF)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->SetTransform(dA, dB, dC, dD, dE, dF);
	}
	STDMETHOD(GetTransform)(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->GetTransform(pdA, pdB, pdC, pdD, pdE, pdF);
	}
	STDMETHOD(ResetTransform)(void)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->ResetTransform();
	}

// -----------------------------------------------------------------------------------------
	STDMETHOD(get_ClipMode)(LONG* plMode)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->get_ClipMode(plMode);
	}
	STDMETHOD(put_ClipMode)(LONG lMode)
	{
		
		if (NULL == m_pCurrentPage)
			return S_FALSE;

		return m_pCurrentPage->put_ClipMode(lMode);
	}

// additiaonal params ----------------------------------------------------------------------
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
	
		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		return S_OK;
	}

	STDMETHOD(get_OfficeSource)(IUnknown** ppSource)
	{
		if (NULL == ppSource)
			return S_FALSE;

		*ppSource = m_oSource.GetSource();
		return S_OK;
	}
	STDMETHOD(put_OfficeSource)(IUnknown* pSource)
	{
		if (NULL == pSource)
			return S_FALSE;

		CloseFile();

		m_oSource.SetSource(pSource);
		m_oCacheDocument.Start();
		
		UpdateSource();			
		return S_OK;
	}

	STDMETHOD(get_ThumbnailsViewer)(IUnknown** ppThViewer)
	{
		if (NULL != ppThViewer)
		{
			*ppThViewer = (IUnknown*)m_pThumbnailsViewer;
			ADDREFINTERFACE(m_pThumbnailsViewer);
		}

		return S_OK;
	}
	STDMETHOD(put_ThumbnailsViewer)(IUnknown* pThViewer)
	{
		RELEASEINTERFACE(m_pThumbnailsViewer);
		if (NULL != pThViewer)
		{
			RELEASEINTERFACE(m_pThumbnailsViewer);
			pThViewer->QueryInterface(__uuidof(IAVSThumbnailsViewer), (void**)&m_pThumbnailsViewer);
		}
		
		return S_OK;
	}

	STDMETHOD(SetSource)(IUnknown* pSource, LONG lViewMode, double dZoom, double dPositionVer, double dPositionHor)
	{
		CTemporaryCS oCS(&m_oCS_ChangeSource);
		
		CloseFile();

		m_bIsSetSource = TRUE;

		if (-1 != lViewMode)
			put_ViewMode(lViewMode);

		if (-1.0 != dZoom)
			put_Zoom(dZoom);

		if (-1.0 != dPositionVer)
			put_PositionVer(dPositionVer);

		if (-1.0 != dPositionHor)
			put_PositionHor(dPositionHor);

		m_oSource.SetSource(pSource);
		m_oCacheDocument.Start();
		
		UpdateSource();

		return S_OK;
	}

protected:
	BOOL ActivateHyperlink(long x, long y);
	void UpdateSource()
	{
		CTemporaryCS oCS(&m_oCS);
		
		LONG lNewCount = 0;
		m_oSource.m_pSource->GetPageCount(&lNewCount);

		LONG lOldCount = (LONG)m_oDocument.GetPagesCount();
		
		LONG lMin = min(lNewCount, lOldCount);
		LONG lCur = 0;

		for (; lCur < lMin; ++lCur)
		{
			IUnknown* punkMeta = NULL;
			m_oSource.m_pSource->ReadMetafile(lCur, &punkMeta);

			if (m_oDocument.GetPage(lCur)->m_pPage != punkMeta)
			{
				RELEASEINTERFACE(punkMeta);
				break;
			}
			RELEASEINTERFACE(punkMeta);
		}

		// теперь нужно удалить все оставшиеся
		m_oDocument.Remove(lCur);

		LONG lNoChangeOldDocument = (lCur == lOldCount) ? 1 : 0;
		if (1 == lNoChangeOldDocument)
		{
			m_lOldDocumentHeight = m_oDocument.GetHeight();
		}

		for (; lCur < lNewCount; ++lCur)
		{
			IUnknown* punkMeta = NULL;
			m_oSource.m_pSource->ReadMetafile(lCur, &punkMeta);
			
			IAVSMetafile* pMeta = (IAVSMetafile*)punkMeta;
			punkMeta = NULL;

			if (NULL != pMeta)
			{
				CPage* pPage = new CPage();

				pMeta->get_Width(&pPage->m_dWidthMM);
				pMeta->get_Height(&pPage->m_dHeightMM);

				pPage->SetDpi(m_dDpiX, m_dDpiY);

				pPage->m_pPage = pMeta;
								
				pMeta = NULL;

				m_oDocument.AddPage(pPage);
			}
		}
		
		m_oPropertyManager.SetProperty(g_lPropertyIndexAddPage, (LONG)lNoChangeOldDocument);
		OnDocumentCountPagesChanged((LONG)m_oDocument.GetPagesCount());
	}
	virtual void OnCompletePage()
	{
		UpdateSource();
	}
	virtual void OnUpdateWatermark()
	{
		//CTemporaryCS oCS(&m_oCS);
		
		BSTR bsXml = NULL;
		m_oSource.m_pSource->get_Watermark(&bsXml);

		m_oCacheDocument.SetWatermark(bsXml);
		RELEASESYSSTRING(bsXml);

		m_oCacheDocument.SendRepaint();
	}
};

