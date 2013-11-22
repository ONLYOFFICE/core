#pragma once

//---------------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------------

[object, uuid("0C20310F-722F-4ce5-8472-E39EB24262DC"), dual, pointer_default(unique)]
__interface IImageView5 : public IDispatch
{
	//---------------------------------------------------------------------------------------------

	[id(10000)]				HRESULT BeginTracks					( [in] BSTR XML );
	[id(20000)]				HRESULT EndTrack					( );

	[id(30000), propget]	HRESULT Serialize					( [out, retval] BSTR* XML );
	[id(30000), propput]	HRESULT Serialize					( [in] BSTR XML );
	[id(35000), propget]	HRESULT Settings					( [out, retval] BSTR* XML );
	[id(35000), propput]	HRESULT Settings					( [in] BSTR XML );

	[id(40000)]				HRESULT SetMediaData				( IUnknown* pInterface );

	[id(51000)]				HRESULT ZoomIn						( );
	[id(52000)]				HRESULT ZoomOut						( );
	[id(53000)]				HRESULT ZoomToFit					( );
	[id(54000)]				HRESULT ShrinkToFit					( );

	[id(54100), propget]	HRESULT InterpolationMode			( [out, retval] long* Mode );
	[id(54100), propput]	HRESULT InterpolationMode			( [in] long Mode );


	[id(55000)]				HRESULT Refresh						( );
	[id(56000), propget]	HRESULT AutoRefresh					( [out, retval] VARIANT_BOOL* Allow );
	[id(56000), propput]	HRESULT AutoRefresh					( [in] VARIANT_BOOL Allow );
	
	[id(57000)] HRESULT		MoveTrack							( [in] double dX, [in] double dY );
	
	[id(58000)] HRESULT		TransformCoordToBitmap				( double* X, double* Y );
	[id(59000)] HRESULT		TransformCoordToControl				( double* X, double* Y );
	
	[id(60000)] HRESULT		Command								( [in] BSTR CommandXml, [out, retval] VARIANT* ParamValue );
	
	[id(62000)] HRESULT		SetAdditionalParam 					( [in] BSTR ParamName, [in] VARIANT ParamValue );
	[id(63000)] HRESULT		GetAdditionalParam 					( [in] BSTR ParamName, [out, retval] VARIANT* ParamValue );


	//---------------------------------------------------------------------------------------------

	//[id(201)] HRESULT SetImage([in, satype("BYTE")] SAFEARRAY** Image);
	[id(211)] HRESULT SetMask									( [in, satype("BYTE")] SAFEARRAY** Image );
	
	[id(252)] HRESULT GetScreen									( [out, satype("BYTE")] SAFEARRAY** Image, [out, retval] VARIANT_BOOL* Success );
	
	[id(501), propget] HRESULT viewCenterX						( [out, retval] double* CenterX );
	[id(501), propput] HRESULT viewCenterX						( [in] double CenterX );
	[id(502), propget] HRESULT viewCenterY						( [out, retval] double* CenterY );
	[id(502), propput] HRESULT viewCenterY						( [in] double CenterX );
	[id(503), propget] HRESULT viewZoom							( [out, retval] double* Zoom );
	[id(503), propput] HRESULT viewZoom							( [in] double Zoom );
	[id(504), propget] HRESULT viewAspect						( [out, retval] double* Aspect );
	[id(504), propput] HRESULT viewAspect						( [in] double Aspect );

	[id(511), propget] HRESULT formatShowBorder					( [out, retval] VARIANT_BOOL* Show );
	[id(511), propput] HRESULT formatShowBorder					( [in] VARIANT_BOOL Show );
	[id(512), propget] HRESULT formatWindowStyle				( [out, retval] long* Style );
	[id(512), propput] HRESULT formatWindowStyle				( [in] long Style );
	[id(513), propget] HRESULT formatMousePointer				( [out, retval] long* Pointer );
	[id(513), propput] HRESULT formatMousePointer				( [in] long Pointer );
	
	[id(521), propget] HRESULT formatMaskStyle					( [out, retval] long* Style );
	[id(521), propput] HRESULT formatMaskStyle					( [in] long Style );
	[id(522), propget] HRESULT formatMaskColor					( [out, retval] OLE_COLOR* Color );
	[id(522), propput] HRESULT formatMaskColor					( [in] OLE_COLOR Color );
	
	[id(531), propget] HRESULT formatBackStyle					( [out, retval] long* Style );
	[id(531), propput] HRESULT formatBackStyle					( [in] long Style );
	[id(532), propget] HRESULT formatBackColor1					( [out, retval] OLE_COLOR* Color );
	[id(532), propput] HRESULT formatBackColor1					( [in] OLE_COLOR Color );
	[id(533), propget] HRESULT formatBackColor2					( [out, retval] OLE_COLOR* Color );
	[id(533), propput] HRESULT formatBackColor2					( [in] OLE_COLOR Color );
	
	// tracking
	[id(600), propget] HRESULT trackType						( [out, retval] long* Type );
	[id(601), propget] HRESULT trackColorScheme					( [out, retval] long* ColorScheme );
	[id(601), propput] HRESULT trackColorScheme					( [in] long ColorScheme );
		
	[id(702)] HRESULT BeginTrackPoint							( [in] double X, [in] double Y );
	[id(703)] HRESULT BeginTrackLine							( [in] double X1, [in] double Y1, [in] double X2, [in] double Y2 );
	[id(704)] HRESULT BeginTrackEllipse							( [in] double Left, [in] double Top, [in] double Right, [in] double Bottom );
	[id(705)] HRESULT BeginTrackRectangle						( [in] double Left, [in] double Top, [in] double Right, [in] double Bottom );
	[id(706)] HRESULT BeginTrackRectangleFixedSize				( [in] double Left, [in] double Top, [in] double Width, [in] double Height );
	[id(707)] HRESULT BeginTrackRectangleFixedAspect			( [in] double Left, [in] double Top, [in] double Width, [in] double Height );
	[id(708)] HRESULT BeginTrackPolyline						( [in, satype("double")] SAFEARRAY** Points );
	[id(709)] HRESULT BeginTrackPolygon							( [in, satype("double")] SAFEARRAY** Points );
	[id(710)] HRESULT BeginTrackPie								( [in] double Left, [in] double Top, [in] double Right, [in] double Bottom, [in] double StartAngle, [in] double SweepAngle );
	[id(711)] HRESULT BeginTrackSector							( [in] double Left, [in] double Top, [in] double Right, [in] double Bottom, [in] double StartAngle, [in] double SweepAngle, [in] double Ratio );
	[id(712)] HRESULT BeginTrackSimpleSector					( [in] double CenterX, [in] double CenterY, [in] double Radius1, [in] double Radius2, [in] double StartAngle, [in] double SweepAngle );
	[id(713)] HRESULT BeginTrackSimplePie						( [in] double CenterX, [in] double CenterY, [in] double Radius1, [in] double StartAngle, [in] double SweepAngle );
	[id(714)] HRESULT BeginTrackEllipseInRectangle				( [in] double Left, [in] double Top, [in] double Right, [in] double Bottom );
	[id(715)] HRESULT BeginTrackCircle							( [in] double CenterX, [in] double CenterY, [in] double Radius );
	[id(716)] HRESULT BeginTrackRectangleRotatable				( [in] double Left, [in] double Top, [in] double Right, [in] double Bottom, [in] double Angle, [in] double StickAngle );
	[id(717)] HRESULT BeginTrackRectangleRotatableFixedAspect	( [in] double Left, [in] double Top, [in] double Right, [in] double Bottom, [in] double Angle, [in] double StickAngle );
	[id(718)] HRESULT BeginTrackTextRotatable					( [in] double Left, [in] double Top, [in] double Right, [in] double Bottom, [in] double Angle, [in] double StickAngle, [in] BSTR inText, [in] BSTR FontName, [in] int FontSize, [in] int FontType, [in] int FontColor );
	[id(719)] HRESULT BeginTrackText							( [in] double Left, [in] double Top, [in] double Right, [in] double Bottom, [in] BSTR inText, [in] BSTR FontName, [in] int FontSize, [in] int FontType, [in] int FontColor );
	[id(720)] HRESULT BeginTrackTrajectoryPolyline				( [in, satype("double")] SAFEARRAY** Points );
};

[dispinterface, uuid("3F06B5BE-9444-4a73-AEDA-E60EACBF8474")]
__interface _ImageView5Events
{
	[id(101)] HRESULT MouseDown									( long Button, long Shift, double X, double Y );
	[id(102)] HRESULT MouseUp									( long Button, long Shift, double X, double Y );
	[id(103)] HRESULT MouseMove									( long Button, long Shift, double X, double Y );
	[id(104)] HRESULT MouseDblClick								( long Button, long Shift, double X, double Y );

	// изменился трэкинг ( эвент для xml схемы )
	[id(10000)] HRESULT TrackChange								( long TrackID, long Operation );
	
	// old style
	[id(201)] HRESULT TrackPoint								( long Type, double X, double Y );
	[id(202)] HRESULT TrackLine									( long Type, double X1, double Y1, double X2, double Y2 );
	[id(203)] HRESULT TrackEllipse								( long Type, double Left, double Top, double Right, double Bottom );
	[id(204)] HRESULT TrackRectangle							( long Type, double Left, double Top, double Right, double Bottom );
	[id(205)] HRESULT TrackRectangleFixedSize					( long Type, double X, double Y );
	[id(206)] HRESULT TrackRectangleFixedAspect					( long Type, double Left, double Top, double Right, double Bottom );
	[id(207)] HRESULT TrackPolyline								( long Type, BYTE* Points );
	[id(208)] HRESULT TrackPolygon								( long Type, BYTE* Points );
	[id(209)] HRESULT TrackPie									( long Type, double Left, double Top, double Right, double Bottom, double StartAngle, double SweepAngle );
	[id(210)] HRESULT TrackSector								( long Type, double Left, double Top, double Right, double Bottom, double StartAngle, double SweepAngle, double Ratio );
	[id(211)] HRESULT TrackSimpleSector							( long Type, double CenterX, double CenterY, double Radius1, double Radius2, double StartAngle, double SweepAngle );
	[id(212)] HRESULT TrackSimplePie							( long Type, double CenterX, double CenterY, double Radius1, double StartAngle, double SweepAngle );
	[id(213)] HRESULT TrackEllipseInRectangle					( long Type, double Left, double Top, double Right, double Bottom );
	[id(214)] HRESULT TrackCircle								( long Type, double CenterX, double CenterY, double Radius );
	[id(215)] HRESULT TrackRectangleRotatable					( long Type, double Left, double Top, double Right, double Bottom, double Angle );
	[id(216)] HRESULT TrackTextRotatable						( long Type, double Left, double Top, double Right, double Bottom, double Angle, BSTR Text );
	[id(217)] HRESULT TrackText									( long Type, double Left, double Top, double Right, double Bottom, BSTR Text );
	[id(218)] HRESULT TrackRectangleRotatableFixedAspect		( long Type, double Left, double Top, double Right, double Bottom, double Angle );
	[id(219)] HRESULT TrackTrajectoryPolyline					( long Type, BYTE* Points );

	[id(501)] HRESULT ChangeBackBuffer							( INT_PTR hDC );
};

