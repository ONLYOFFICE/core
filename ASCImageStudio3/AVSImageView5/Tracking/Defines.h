#pragma once

namespace Tracking
{
	const int c_nTrackPointStatusNone					= 0;
	const int c_nTrackPointStatusPoint					= 1;	// hand arrow
	const int c_nTrackPointStatusHorizontal				= 2;	// - size
	const int c_nTrackPointStatusVertical				= 3;	// | size
	const int c_nTrackPointStatusSize1					= 4;	// / size
	const int c_nTrackPointStatusSize2					= 5;	// \ size
	const int c_nTrackPointStatusSizeAll				= 6;	// + size
	const int c_nTrackPointStatusCanAdd					= 7;	// pick cross
	const int c_nTrackPointStatusRotation				= 8;	// rotation cross
	const int c_nTrackPointStatusIBeam					= 9;	// edit text
	const int c_nTrackPointStatusSizeForRotation		= 10;	// + size for rotation

	const int c_nTrackPointSizePaint 					= 3;
	const int c_nTrackPointSizeTouch 					= 5;

	const int c_nTrackChangeStatusNone					= 0;
	const int c_nTrackChangeStatusMove					= 1;
	const int c_nTrackChangeStatusSize					= 2;
	const int c_nTrackChangeStatusRotation				= 3;

	// карта констант 
	namespace Constants
	{
		// типы треков
		static const int sc_nTrackTypeNone							= 0;
		static const int sc_nTrackTypePoint							= 1;
		static const int sc_nTrackTypeLine							= 2;
		static const int sc_nTrackTypeEllipse						= 3;
		static const int sc_nTrackTypeRectangle						= 4;
		static const int sc_nTrackTypePolyline						= 5;
		static const int sc_nTrackTypePolygon						= 6;
		static const int sc_nTrackTypeRectangleFixedSize			= 7;
		static const int sc_nTrackTypePie							= 8;
		static const int sc_nTrackTypeSector						= 9;
		static const int sc_nTrackTypeSectorSimple					= 10;
		static const int sc_nTrackTypePieSimple						= 11;
		static const int sc_nTrackTypeRectangleFixedAspect			= 12;
		static const int sc_nTrackTypeEllipseInRectangle			= 13;
		static const int sc_nTrackTypeCircleInRectangle				= 14;
		static const int sc_nTrackTypeRectangleRotatable			= 15;
		static const int sc_nTrackTypeTextRotatable					= 16;
		static const int sc_nTrackTypeText							= 17;
		static const int sc_nTrackTypeRectangleRotatableFixedAspect = 18;
		static const int sc_nTrackTypeTrajectoryPolyline			= 19;
		static const int sc_nTrackTypeCubicBezierCurve				= 20;			// Безье второго порядка
		static const int sc_nTrackTypeTrackEffectKenBurns			= 21;			//

		static const int sc_nMessageTrackPointBegin					= 17001;
		static const int sc_nMessageTrackPointEnd					= 17002;
		static const int sc_nMessageTrackPointProcess				= 17003;
		static const int sc_nMessageTrackPointProcessAll			= 17004;
		static const int sc_nMessageTrackPointRemove				= 17005;
		static const int sc_nMessageTrackTextEnd					= 17006;
		static const int sc_nMessageTrackTextBegin					= 17007;
		static const int sc_nMessageTrackTextChange					= 17008;
		static const int sc_nMessageTrackInvalidate					= 17100;

		// сообщения
		static const int sc_nMessageTracksCreate					= 101010001;
		static const int sc_nMessageTracksDelete					= 101010002;

		// сообщения каретки
		static const int sc_nMessageTextCaretPositionChange			= 101010010;	// изменение положение каретки
	}
};

