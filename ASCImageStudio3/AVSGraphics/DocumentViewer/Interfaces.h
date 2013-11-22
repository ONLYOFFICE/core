#pragma once

// IAVSDocumentViewer
[object, uuid("239D54EB-8A00-4314-B8B5-C8BCA9DC84FC"), dual, pointer_default(unique)]
__interface IAVSDocumentViewer : public IDispatch
{
	[id(100), propget] HRESULT ViewMode([out, retval] LONG* lType);
	[id(100), propput] HRESULT ViewMode([in] LONG lType);

	[id(101), propget] HRESULT Zoom([out, retval] double* dZoom);
	[id(101), propput] HRESULT Zoom([in] double dZoom);

	[id(102), propget] HRESULT PositionHor([out, retval] double* dPosition);
	[id(102), propput] HRESULT PositionHor([in] double dPosition);

	[id(103), propget] HRESULT PositionVer([out, retval] double* dPosition);
	[id(103), propput] HRESULT PositionVer([in] double dPosition);

	[id(148), propget] HRESULT ControlWidth([out, retval] LONG* lWidth);
	[id(149), propget] HRESULT ControlHeight([out, retval] LONG* lHeight);
	[id(150), propget] HRESULT DocumentWidth([out, retval] double* dWidth);
	[id(151), propget] HRESULT DocumentHeight([out, retval] double* dHeight);

	[id(201), propget] HRESULT CountPages([out, retval] LONG* lCountPages);
	
	[id(202), propget] HRESULT CurrentPage([out, retval] LONG* lCurrentPage);
	[id(202), propput] HRESULT CurrentPage([in] LONG lCurrentPage);

	[id(203), propget] HRESULT PagesInBlock([out, retval] LONG* lCurrentPage);
	[id(203), propput] HRESULT PagesInBlock([in] LONG lCurrentPage);

	[id(300)]		   HRESULT CloseFile(); 

	[id(500)]		   HRESULT Init(); 

	[id(600), propget] HRESULT OfficeSource([out, retval] IUnknown** ppSource); 
	[id(600), propput] HRESULT OfficeSource([in] IUnknown* pSource); 

	[id(700), propget] HRESULT ThumbnailsViewer([out, retval] IUnknown** ppThViewer); 
	[id(700), propput] HRESULT ThumbnailsViewer([in] IUnknown* pThViewer); 

	[id(800)]		   HRESULT SetSource([in] IUnknown* punkSource, [in] LONG lViewMode, [in] double dZoom, [in] double dPositionVer, [in] double dPositionHor); 

	[id(10000), hidden]		HRESULT	_OnTimer(void);
};

// _IAVSDocumentViewerEvents
[uuid("9F707592-0C5A-4f59-A57A-D11F49FF5FDF"), dispinterface]
__interface _IAVSDocumentViewerEvents
{
	[id(10)] HRESULT OnDocumentSizeChanged(double dWidth, double dHeight, double dPositionHor, double dPositionVer);
	[id(11)] HRESULT OnDocumentPositionChanged(double dPositionHor, double dPositionVer);

	[id(12)] HRESULT OnDocumentPageChanged(LONG lPageNumber);
	[id(13)] HRESULT OnDocumentCountPagesChanged(LONG lPageCount);

	[id(101)] HRESULT OnMouseDown		( long Button, long Shift, double X, double Y );
	[id(102)] HRESULT OnMouseUp			( long Button, long Shift, double X, double Y );
	[id(103)] HRESULT OnMouseMove		( long Button, long Shift, double X, double Y );
	[id(104)] HRESULT OnMouseDblClick	( long Button, long Shift, double X, double Y );

	[id(151)] HRESULT OnKeyDown			( long Button, long Shift, double X, double Y );
	[id(152)] HRESULT OnKeyUp			( long Button, long Shift, double X, double Y );

	[id(200)] HRESULT OnMessage			( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL* bHandled); 
};

// IAVSViewerThumbnails
[object, uuid("4E2474FB-9C42-4691-B064-C01616C5B9FC"), dual, pointer_default(unique)]
__interface IAVSThumbnailsViewer : public IDispatch
{
	[id(50), propget] HRESULT FixWidth([out, retval] double* pdFixWidth);
	[id(50), propput] HRESULT FixWidth([in] double dFixWidth);

	[id(51), propget] HRESULT FixHeight([out, retval] double* pdFixHeight);
	[id(51), propput] HRESULT FixHeight([in] double dFixHeight);

	[id(101), propget] HRESULT Zoom([out, retval] double* dZoom);
	[id(101), propput] HRESULT Zoom([in] double dZoom);

	[id(102), propget] HRESULT PositionHor([out, retval] double* dPosition);
	[id(102), propput] HRESULT PositionHor([in] double dPosition);

	[id(103), propget] HRESULT PositionVer([out, retval] double* dPosition);
	[id(103), propput] HRESULT PositionVer([in] double dPosition);

	[id(148), propget] HRESULT ControlWidth([out, retval] LONG* lWidth);
	[id(149), propget] HRESULT ControlHeight([out, retval] LONG* lHeight);
	[id(150), propget] HRESULT DocumentWidth([out, retval] double* dWidth);
	[id(151), propget] HRESULT DocumentHeight([out, retval] double* dHeight);

	[id(201), propget] HRESULT CountPages([out, retval] LONG* lCountPages);
	
	[id(202), propget] HRESULT CurrentPage([out, retval] LONG* lCurrentPage);
	[id(202), propput] HRESULT CurrentPage([in] LONG lCurrentPage);

	[id(203), propget] HRESULT VisibleViewerBounds([out, retval] VARIANT_BOOL* vbValue);
	[id(203), propput] HRESULT VisibleViewerBounds([in] VARIANT_BOOL vbValue);

	[id(300)]		   HRESULT CloseFile(); 

	[id(500)]		   HRESULT Init(); 

	[id(600), propget] HRESULT OfficeSource([out, retval] IUnknown** ppSource); 
	[id(600), propput] HRESULT OfficeSource([in] IUnknown* pSource); 

	[id(700), propget] HRESULT DocumentViewer([out, retval] IUnknown** ppDocViewer); 
	[id(700), propput] HRESULT DocumentViewer([in] IUnknown* pDocViewer); 

	[id(710)]		   HRESULT SetCurrentPageInfo([in] LONG lCurrentPage, [in] double dLeft, [in] double dTop, [in] double dWidth, [in] double dHeight);
	[id(800)]		   HRESULT GetPagePointInside([in] double dX, [in] double dY, [out] LONG* lPage);

	[id(900)]		   HRESULT SetSource([in] IUnknown* punkSource, [in] double dZoom, [in] double dPositionVer, [in] double dPositionHor);

	[id(10000), hidden]		HRESULT	_OnTimer(void);
};

// _IAVSThumbnailsViewerEvents
[uuid("52FEA94B-26B2-4f2f-A5AF-F05461DB2D29"), dispinterface]
__interface _IAVSThumbnailsViewerEvents
{
	[id(10)] HRESULT OnDocumentSizeChanged(double dWidth, double dHeight, double dPositionHor, double dPositionVer);
	[id(11)] HRESULT OnDocumentPositionChanged(double dPositionHor, double dPositionVer);

	[id(12)] HRESULT OnDocumentPageChanged(LONG lPageNumber);
	[id(13)] HRESULT OnDocumentCountPagesChanged(LONG lPageCount);

	[id(101)] HRESULT OnMouseDown		( long Button, long Shift, double X, double Y );
	[id(102)] HRESULT OnMouseUp			( long Button, long Shift, double X, double Y );
	[id(103)] HRESULT OnMouseMove		( long Button, long Shift, double X, double Y );
	[id(104)] HRESULT OnMouseDblClick	( long Button, long Shift, double X, double Y );

	[id(151)] HRESULT OnKeyDown			( long Button, long Shift, double X, double Y );
	[id(152)] HRESULT OnKeyUp			( long Button, long Shift, double X, double Y );

	[id(200)] HRESULT OnMessage			( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL* bHandled); 
};