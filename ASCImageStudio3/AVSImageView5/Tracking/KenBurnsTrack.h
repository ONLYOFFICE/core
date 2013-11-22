#pragma once

#include "Tracking.h"

namespace Tracking
{
	class CTrackEffectKenBurns : public CTrackObject
	{
	public:

		CTrackEffectKenBurns ( )
		{
			m_nType				= Constants::sc_nTrackTypeTrackEffectKenBurns;

			m_ID			=	0;
			
			m_bInitialized	=	FALSE;

			m_bLMButtonDown	=	FALSE;
			m_bOnMouseMove	=	FALSE;
		}
		
		//	первый прямоугольник, второй прямоугольник	-	аспект высчитывается по соотношению сторон
		inline void Create ( double dLeft, double dTop,	double dRight, double dBottom, double dLeftEnd,	double dTopEnd,	double dRightEnd, double dBottomEnd	)
		{
			m_ID	=	0;

			m_Track[0].Clear();

			m_Track[0].Add ( dLeft,		dTop	);
			m_Track[0].Add ( dRight,	dTop	);
			m_Track[0].Add ( dRight,	dBottom	);
			m_Track[0].Add ( dLeft,		dBottom	);
			m_Track[0].Add ( 0.5 * ( dLeft + dRight ), 0.5 * ( dTop + dBottom ) );

			double D1 = Geometry::GetLength ( m_Track[0].GetAt(0).dX, m_Track[0].GetAt(0).dY, m_Track[0].GetAt(1).dX, m_Track[0].GetAt(1).dY );
			double D2 = Geometry::GetLength ( m_Track[0].GetAt(2).dX, m_Track[0].GetAt(2).dY, m_Track[0].GetAt(1).dX, m_Track[0].GetAt(1).dY );

			if ( D1 < 2 || D2 < 2 )
				return;

			m_nAspect [ 0 ]		=	D1 / D2;

			m_Track[1].Clear();

			m_Track[1].Add ( dLeftEnd,	dTopEnd		);
			m_Track[1].Add ( dRightEnd,	dTopEnd		);
			m_Track[1].Add ( dRightEnd,	dBottomEnd	);
			m_Track[1].Add ( dLeftEnd,	dBottomEnd	);
			m_Track[1].Add ( 0.5 * ( dLeft + dRight ), 0.5 * ( dTop + dBottom ) );

			D1 = Geometry::GetLength ( m_Track[1].GetAt(0).dX, m_Track[1].GetAt(0).dY, m_Track[1].GetAt(1).dX, m_Track[1].GetAt(1).dY );
			D2 = Geometry::GetLength ( m_Track[1].GetAt(2).dX, m_Track[1].GetAt(2).dY, m_Track[1].GetAt(1).dX, m_Track[1].GetAt(1).dY );

			if ( D1 < 2 || D2 < 2 )
				return;

			m_nAspect [ 1 ]		=	D1 / D2;

			m_Track [ 0 ].SetCurrentPointIndex ( -1 );
			m_Track [ 1 ].SetCurrentPointIndex ( -1 );

			m_bInitialized	=	TRUE;
		}

		virtual BOOL OnSetCursor(int nX, int nY, int& nPointStatus, int& nChangeStatus)
		{
			if ( FALSE == m_bInitialized )
				return FALSE;

			nPointStatus			=	c_nTrackPointStatusNone;
			nChangeStatus			=	c_nTrackChangeStatusNone;

			int nTrackIndex = m_Track [ m_ID ].GetCurrentPointIndex();

			if (nTrackIndex < 0)
			{
				nTrackIndex			=	GetPointUnderCursor ( m_Track [ m_ID ], nX, nY );
			}

			if ( ( nTrackIndex == m_Track [ m_ID ].GetCount() - 1 ) || ( nTrackIndex >= 0 && GetAsyncKeyState(VK_CONTROL) < 0 ) )
			{
				nPointStatus		=	c_nTrackPointStatusSizeAll;
				nChangeStatus		=	c_nTrackChangeStatusMove;
			}
			else
			{
				nChangeStatus		=	c_nTrackChangeStatusSize;

				if ( nTrackIndex == 0 || nTrackIndex == 2 )
				{
					nPointStatus	=	c_nTrackPointStatusPoint;
				}
				else if ( nTrackIndex == 1 || nTrackIndex == 3 )
				{
					nPointStatus	=	c_nTrackPointStatusPoint;
				}
				else
				{
					nPointStatus	=	c_nTrackPointStatusNone;
					nChangeStatus	=	c_nTrackChangeStatusNone;
				}
			}

			return FALSE;
		}

		virtual void OnPaintLines(Tracking::CTrackPainter& oPainter )
		{
			if ( FALSE == m_bInitialized )
				return;

			Structures::RECTI YellowBound	=	m_Track [ 0 ].GetIntegerBoundingRect ( 0, 3 );
			Structures::RECTI RedBound		=	m_Track [ 1 ].GetIntegerBoundingRect ( 0, 3 );

			Gdiplus::Graphics oRender		( oPainter.GetDC() );

			oRender.SetCompositingQuality	( Gdiplus::CompositingQualityHighSpeed );
			oRender.SetSmoothingMode		( Gdiplus::SmoothingModeHighSpeed );

			float OffSetL			=	7.0f;
			float LineLength		=	15.0f;

			float fYellow_Size_2	=	2.0f;
			float fBlack_Size_4		=	4.0f;
			float fRed_Size_2		=	2.0f;
			float fGreeen_Size_2	=	2.0f;

			Gdiplus::Pen Yellow2	( Color::Yellow,	fYellow_Size_2 );
			Gdiplus::Pen Black4		( Color::Black,		fBlack_Size_4 );
			Gdiplus::Pen Red2		( Color::Red,		fRed_Size_2 );
			Gdiplus::Pen Green2		( Color::LawnGreen,	fGreeen_Size_2 );
			Gdiplus::Pen DarkRed4	( Color::DarkRed,	fBlack_Size_4 );

			if ( 1 == m_ID )
			{
				DrawColorTrack ( oRender, Black4, Green2, fBlack_Size_4, fGreeen_Size_2, (float)YellowBound.left, (float)YellowBound.top, (float)YellowBound.right, (float)YellowBound.bottom, FALSE, OffSetL, LineLength );
				oRender.SetClip ( Gdiplus::RectF ( (float)RedBound.left, (float)RedBound.top, abs ( (float)RedBound.right - (float)RedBound.left ), abs ( (float)RedBound.bottom - (float)RedBound.top ) ), CombineModeExclude );
			}
		
			if ( 0 == m_ID )
			{
				DrawColorTrack ( oRender, Black4, Red2, fBlack_Size_4, fRed_Size_2, (float)RedBound.left, (float)RedBound.top, (float)RedBound.right, (float)RedBound.bottom, FALSE, OffSetL, LineLength );
				oRender.SetClip ( Gdiplus::RectF ( (float)YellowBound.left, (float)YellowBound.top, abs ( (float)YellowBound.right - (float)YellowBound.left ), abs ( (float)YellowBound.bottom - (float)YellowBound.top ) ), CombineModeExclude );
			}

			Gdiplus::SolidBrush oBrush ( Color ( 100, 0, 0, 0 ) );
			oRender.FillRectangle ( &oBrush, Gdiplus::RectF ( 0.0f, 0.0f, (float)m_rectWindow.right, (float)m_rectWindow.bottom ) );

			oRender.ResetClip ();

			oRender.SetCompositingQuality	( Gdiplus::CompositingQualityHighSpeed );
			oRender.SetSmoothingMode		( Gdiplus::SmoothingModeHighSpeed );

			if ( 0 == m_ID )
			{
				DrawColorTrack ( oRender, Black4, Green2, fBlack_Size_4, fGreeen_Size_2, (float)YellowBound.left, (float)YellowBound.top, (float)YellowBound.right, (float)YellowBound.bottom, TRUE, OffSetL, LineLength );
			}

			if ( 1 == m_ID )
			{
				DrawColorTrack ( oRender, Black4, Red2, fBlack_Size_4, fRed_Size_2, (float)RedBound.left, (float)RedBound.top, (float)RedBound.right, (float)RedBound.bottom, TRUE );
			}
			
			// DRAW LINE CAP
			if ( TRUE )
			{
				double X1	=	( (double)RedBound.left			+ (double)RedBound.right )		* 0.5;
				double Y1	=	( (double)RedBound.bottom		+ (double)RedBound.top )		* 0.5;
				double X2	=	( (double)YellowBound.left		+ (double)YellowBound.right )	* 0.5;
				double Y2	=	( (double)YellowBound.bottom	+ (double)YellowBound.top )		* 0.5;

				double D	=	Geometry::GetLength ( X1, Y1, X2, Y2 );

				if ( D < 25.0 )
				{
					return;
				}

				oRender.SetCompositingQuality	( Gdiplus::CompositingQualityHighQuality );
				oRender.SetSmoothingMode		( Gdiplus::SmoothingModeHighQuality );

				Gdiplus::Pen PenColor ( Color::Yellow, 3.0f );

				Gdiplus::Pen PenCurve ( Color::Black, 4.0f );

				Gdiplus::GraphicsPath ArrowPath;

				float FactorCap		=	2;
				
				ArrowPath.AddLine ( Gdiplus::PointF (  0.0f * FactorCap, -1.0f * FactorCap + 3.0f * FactorCap ), Gdiplus::PointF ( -1.0f * FactorCap, -4.0f * FactorCap + 3.0f * FactorCap ) );
				ArrowPath.AddLine ( Gdiplus::PointF ( -1.0f * FactorCap, -4.0f * FactorCap + 3.0f * FactorCap ), Gdiplus::PointF (  1.0f * FactorCap, -4.0f * FactorCap + 3.0f * FactorCap ) );
				ArrowPath.AddLine ( Gdiplus::PointF (  1.0f * FactorCap, -4.0f * FactorCap + 3.0f * FactorCap ), Gdiplus::PointF (  0.0f * FactorCap, -1.0f * FactorCap + 3.0f * FactorCap ) );

				Gdiplus::CustomLineCap   CustomCapArrow ( &ArrowPath, NULL );

				PenColor.SetEndCap			( Gdiplus::LineCapArrowAnchor );
				PenColor.SetCustomEndCap	( &CustomCapArrow );
				PenColor.SetLineJoin		( Gdiplus::LineJoinRound );

				PenCurve.SetCustomEndCap	( &CustomCapArrow );
				PenCurve.SetLineJoin		( Gdiplus::LineJoinRound );

				oRender.SetSmoothingMode	( Gdiplus::SmoothingModeHighQuality );

				double NX	=	X1 - X2;
				double NY	=	Y1 - Y2;
				
				NX			=	NX / D;
				NY			=	NY / D;

				oRender.DrawLine ( &PenCurve, (float)X2, (float)Y2, (float)X1 - (float)NX * 22.0f,	(float)Y1 - (float)NY * 22.0f );
				oRender.DrawLine ( &PenColor, (float)X2, (float)Y2, (float)X1 - (float)NX * 22.0f,	(float)Y1 - (float)NY * 22.0f );
			}
		}


		virtual void OnPaintPoints(Tracking::CTrackPainter& oPainter)
		{
			if ( FALSE == m_bInitialized )
				return;
		}

		virtual BOOL OnPaint ( CTrackPainter& oPainter, Structures::RECTI rectWindow, Structures::RECTD rectData )
		{
			if ( FALSE == m_bInitialized )
				return FALSE;

			m_rectWindow	=	rectWindow;
			m_rectData		=	rectData;

			if ( 0 == m_Track [ 0 ].GetCount() || 0 == m_Track [ 1 ].GetCount() )
				return FALSE;

			// update tracking point coordinates
			m_Track [ 0 ].MapByDouble(rectData.left, rectData.top, rectData.right, rectData.bottom, rectWindow.left, rectWindow.top, rectWindow.right, rectWindow.bottom);
			m_Track [ 1 ].MapByDouble(rectData.left, rectData.top, rectData.right, rectData.bottom, rectWindow.left, rectWindow.top, rectWindow.right, rectWindow.bottom);

			// draw tracking lines
			oPainter.SetPenLines ();
			OnPaintLines(oPainter);

			// draw tracking points
			oPainter.SetPenPoints ();
			OnPaintPoints(oPainter);

			return TRUE;
		}
		virtual BOOL OnMessages ( UINT nMessage, WPARAM wParam, LPARAM lParam )
		{
			if (nMessage == WM_LBUTTONDOWN)
				CTrackEffectKenBurns::OnLButtonDown	( GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) );
			else if (nMessage == WM_LBUTTONUP)
				CTrackEffectKenBurns::OnLButtonUp	( GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) );
			else if (nMessage == WM_MOUSEMOVE)
				CTrackEffectKenBurns::OnMouseMove	( GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) );
			else if ( nMessage == WM_MOUSEWHEEL )
			{
				CTrackEffectKenBurns::OnMouseWheel ( GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) );
			}

			return FALSE;
		}

		virtual int GetPointUnderCursor(int nX, int nY)
		{
			return TRUE;
		}	

		inline void DrawColorTrack ( Gdiplus::Graphics& oRender, Gdiplus::Pen& oBoldPen, Gdiplus::Pen& oMainPen, float fBoldPenSize, float fMainPenWidth, float fLeft, float fTop, float fRight, float fBottom,	BOOL DrawCorners = TRUE, float OffSetL = 7.0f, float LineLength = 15.0f )
		{

			Gdiplus::Pen oPenYellow ( Color::YellowGreen, 3.0 );
			oRender.DrawRectangle ( &oBoldPen, Gdiplus::RectF ( fLeft - fBoldPenSize * 0.5f, fTop - fBoldPenSize * 0.5f, abs ( fRight - fLeft ) + fBoldPenSize, abs ( fBottom - fTop ) + fBoldPenSize ) );
			oRender.DrawRectangle ( &oMainPen, Gdiplus::RectF ( fLeft - fBoldPenSize * 0.5f, fTop - fBoldPenSize * 0.5f, abs ( fRight - fLeft ) + fBoldPenSize, abs ( fBottom - fTop ) + fBoldPenSize ) );

			if  ( DrawCorners )
			{
				float OffSetL		=	7;
				float LineLength	=	15;

				float Left			=	fLeft - OffSetL;
				float Right			=	fRight + OffSetL;
				float Top			=	fTop - OffSetL;
				float Bottom		=	fBottom + OffSetL;

				// Left Top
				oRender.DrawLine ( &oBoldPen, Left - fBoldPenSize * 0.5f,		Top,							Left + LineLength + fBoldPenSize * 0.25f,	Top );											//	horiz
				oRender.DrawLine ( &oBoldPen, Left,								Top - fBoldPenSize * 0.5f,		Left,										Top + LineLength + fBoldPenSize * 0.25f );		//	vert
				oRender.DrawLine ( &oMainPen, Left - fMainPenWidth * 0.5f,		Top,							Left + LineLength,							Top );											//	horiz
				oRender.DrawLine ( &oMainPen, Left,								Top - fMainPenWidth * 0.5f,		Left,										Top + LineLength );								//	vert

				// Right Top
				oRender.DrawLine ( &oBoldPen, Right + fBoldPenSize * 0.5f,		Top,							Right - LineLength - fBoldPenSize * 0.25f,	Top );
				oRender.DrawLine ( &oBoldPen, Right,							Top - fBoldPenSize * 0.5f,		Right,										Top + LineLength  + fBoldPenSize * 0.25f );
				oRender.DrawLine ( &oMainPen, Right - fMainPenWidth * 0.5f,		Top,							Right - LineLength,							Top );
				oRender.DrawLine ( &oMainPen, Right,							Top - fMainPenWidth * 0.5f,		Right,										Top + LineLength );

				// Left Bottom
				oRender.DrawLine ( &oBoldPen, Left - fBoldPenSize * 0.5f,		Bottom,							Left + LineLength + fBoldPenSize * 0.25f,	Bottom );
				oRender.DrawLine ( &oBoldPen, Left,								Bottom + fBoldPenSize * 0.5f,	Left,										Bottom - LineLength - fBoldPenSize * 0.25f );
				oRender.DrawLine ( &oMainPen, Left - fMainPenWidth * 0.5f,		Bottom,							Left + LineLength,							Bottom );
				oRender.DrawLine ( &oMainPen, Left,								Bottom + fMainPenWidth * 0.5f,	Left,										Bottom - LineLength  );

				// Right Bottom
				oRender.DrawLine ( &oBoldPen, Right + fBoldPenSize * 0.5f,		Bottom,							Right - LineLength - fBoldPenSize * 0.25f,	Bottom );
				oRender.DrawLine ( &oBoldPen, Right,							Bottom + fBoldPenSize * 0.5f,	Right,										Bottom - LineLength - fBoldPenSize * 0.25f );
				oRender.DrawLine ( &oMainPen, Right - fMainPenWidth * 0.5f,		Bottom,							Right - LineLength,							Bottom );
				oRender.DrawLine ( &oMainPen, Right,							Bottom + fMainPenWidth * 0.5f,	Right,										Bottom - LineLength );
			}

			if ( TRUE )
			{
				// Draw Cross On Center Rectangle
				float CX	=	( fLeft + fRight ) * 0.5f;
				float CY	=	( fBottom + fTop ) * 0.5f;

				oRender.DrawLine ( &oBoldPen, CX - 8.0f, CY, CX + 8.0f, CY );
				oRender.DrawLine ( &oBoldPen, CX , CY - 8.0f, CX , CY + 8.0f );

				oRender.DrawLine ( &oMainPen, CX - 7.0f, CY, CX + 7.0f, CY );
				oRender.DrawLine ( &oMainPen, CX , CY - 7.0f, CX , CY + 7.0f );
			}
		}

	public:

		bool IMouseButtonHandlers::OnMouseMove		( int MouseX, int MouseY )
		{
			if ( FALSE == m_bInitialized )
				return false;

			if ( FALSE == m_bLMButtonDown )
				return false;

			m_bOnMouseMove	=	TRUE;

			double dX		=	MapToDataX ( MouseX );
			double dY		=	MapToDataY ( MouseY );

			int nCurrentPoint = m_Track [ m_ID ].GetCurrentPointIndex();

			if ( ( nCurrentPoint == m_Track [ m_ID ].GetCount() - 1 ) || GetAsyncKeyState ( VK_CONTROL ) )
			{
				// центр трека
				double CX	=	( m_Track [ m_ID ].GetAt ( 2 ).dX + m_Track [ m_ID ].GetAt ( 0 ).dX ) * 0.5;
				double CY	=	( m_Track [ m_ID ].GetAt ( 2 ).dY + m_Track [ m_ID ].GetAt ( 0 ).dY ) * 0.5;

				// длина и ширина трека
				double NX	=	abs ( 0.5 * ( m_Track [ m_ID ].GetAt ( 2 ).dX - m_Track [ m_ID ].GetAt ( 0 ).dX ) );
				double NY	=	abs ( 0.5 * ( m_Track [ m_ID ].GetAt ( 2 ).dY - m_Track [ m_ID ].GetAt ( 0 ).dY ) );

				// смещение для перемещения позиции трека
				double OffSetX	=	dX - m_Track [ m_ID ].GetAt ( 0 ).dX - m_nMouseOffsetX;
				double OffSetY	=	dY - m_Track [ m_ID ].GetAt ( 0 ).dY - m_nMouseOffsetY;

				if ( OffSetX < 0.0 )
				{
					if ( CX - NX + OffSetX < 0.0 )
					{
						OffSetX	=	- ( CX - NX );
					}
				}

				if ( OffSetX > 0.0 )
				{
					if ( CX + NX + OffSetX > m_nImageWidth )
					{
						OffSetX	=	m_nImageWidth - ( CX + NX );
					}
				}

				if ( OffSetY < 0.0 )
				{
					if ( CY - NY + OffSetY < 0.0 )
					{
						OffSetY	=	- ( CY - NY );
					}
				}

				if ( OffSetY > 0.0 )
				{
					if ( CY + NY + OffSetY > m_nImageHeight )
					{
						OffSetY	=	m_nImageHeight - ( CY + NY );
					}
				}

				m_Track [ m_ID ].OffsetByDouble ( OffSetX, OffSetY );

				Messages::CDirectMessage::Send ( m_pMessageReceiver, Constants::sc_nMessageTrackPointProcessAll,  m_ID, 0 );
			}
			else
			{
				if ( -1 == nCurrentPoint )
					return TRUE;

#define			MAX_HYPOT_RECTANGLE		100.0

				double MaxY				=	sqrt ( ( MAX_HYPOT_RECTANGLE * MAX_HYPOT_RECTANGLE ) / ( m_nAspect [ m_ID ] * m_nAspect [ m_ID ] + 1 ) );
				double MaxX				=	MaxY * m_nAspect [ m_ID ];

				double NMouseX			=	dX;
				double NMouseY			=	dY;

#pragma region ("point0")

				if ( nCurrentPoint == 0 )
				{
					double SizeX		=	m_Track [ m_ID ].GetAt(0).dX - m_Track [ m_ID ].GetAt(2).dX;
					double SizeY		=	m_Track [ m_ID ].GetAt(0).dY - m_Track [ m_ID ].GetAt(2).dY;

					double MoveSizeX	=	NMouseX - m_Track [ m_ID ].GetAt(2).dX;
					double MoveSizeY	=	NMouseY - m_Track [ m_ID ].GetAt(2).dY;

					if ( abs ( MoveSizeX / MoveSizeY ) > abs ( SizeX / SizeY ) )
					{
						double OffSetY	=	-MoveSizeX / m_nAspect [ m_ID ];

						if ( OffSetY < 0.0f )
							return TRUE;

						if ( NMouseX < 0.0 )
						{
							NMouseX		=	0.0;
							OffSetY		=	m_Track [ m_ID ].GetAt(1).dX / m_nAspect [ m_ID ];
						}

						if ( OffSetY < MaxY )
						{
							NMouseX		=	m_Track [ m_ID ].GetAt(2).dX - MaxX;
							OffSetY		=	MaxY;
						}

						if ( m_Track [ m_ID ].GetAt(2).dY - OffSetY < 0.0 )
						{
							OffSetY		=	m_Track [ m_ID ].GetAt(2).dY;
							NMouseX		=	m_Track [ m_ID ].GetAt(2).dX - OffSetY * m_nAspect [ m_ID ];
						}

						m_Track [ m_ID ].GetAt(0).Create ( NMouseX,							m_Track [ m_ID ].GetAt(2).dY - OffSetY );
						m_Track [ m_ID ].GetAt(1).Create ( m_Track [ m_ID ].GetAt(2).dX,	m_Track [ m_ID ].GetAt(2).dY - OffSetY );
						m_Track [ m_ID ].GetAt(3).Create ( NMouseX,							m_Track [ m_ID ].GetAt(2).dY );
					}
					else
					{
						double OffSetX	=	-MoveSizeY * m_nAspect [ m_ID ];

						if ( OffSetX < 0.0 )
							return TRUE;

						if ( NMouseY < 0.0 )
						{
							NMouseY		=	0.0;
							OffSetX		=	m_Track [ m_ID ].GetAt(2).dY * m_nAspect [ m_ID ];
						}

						if ( OffSetX < MaxX )
						{
							NMouseY		=	m_Track [ m_ID ].GetAt(2).dY - MaxY;
							OffSetX		=	MaxX;
						}

						if ( m_Track [ m_ID ].GetAt(2).dX - OffSetX < 0.0 )
						{
							OffSetX		=	m_Track [ m_ID ].GetAt(2).dX;
							NMouseY		=	m_Track [ m_ID ].GetAt(2).dY - OffSetX / m_nAspect [ m_ID ];
						}

						m_Track [ m_ID ].GetAt(0).Create ( m_Track [ m_ID ].GetAt(2).dX - OffSetX,	NMouseY );
						m_Track [ m_ID ].GetAt(1).Create ( m_Track [ m_ID ].GetAt(2).dX,			NMouseY );
						m_Track [ m_ID ].GetAt(3).Create ( m_Track [ m_ID ].GetAt(2).dX - OffSetX,	m_Track [ m_ID ].GetAt(2).dY );
					}
				}
#pragma endregion

#pragma region ("point1")

				if ( nCurrentPoint == 1 )
				{
					double SizeX		=	m_Track [ m_ID ].GetAt(0).dX - m_Track [ m_ID ].GetAt(2).dX;
					double SizeY		=	m_Track [ m_ID ].GetAt(0).dY - m_Track [ m_ID ].GetAt(2).dY;

					double MoveSizeX	=	NMouseX - m_Track [ m_ID ].GetAt(3).dX;
					double MoveSizeY	=	NMouseY - m_Track [ m_ID ].GetAt(3).dY;

					if ( abs ( MoveSizeX / MoveSizeY ) > abs ( SizeX / SizeY ) )
					{
						double OffSetY	=	MoveSizeX / m_nAspect [ m_ID ];

						if ( OffSetY < 0.0 )
							return TRUE;

						if ( NMouseX > m_nImageWidth )
						{
							NMouseX		=	m_nImageWidth;
							OffSetY		=	( m_nImageWidth - m_Track [ m_ID ].GetAt(0).dX ) / m_nAspect [ m_ID ];
						}

						if ( OffSetY < MaxY )
						{
							NMouseX		=	m_Track [ m_ID ].GetAt(0).dX + MaxX;
							OffSetY		=	MaxY;
						}

						if ( m_Track [ m_ID ].GetAt(2).dY - OffSetY < 0.0 )
						{
							OffSetY		=	m_Track [ m_ID ].GetAt(2).dY;
							NMouseX		=	m_Track [ m_ID ].GetAt(0).dX + m_Track [ m_ID ].GetAt(2).dY * m_nAspect [ m_ID ];
						}

						m_Track [ m_ID ].GetAt(0).Create ( m_Track [ m_ID ].GetAt(3).dX,	m_Track [ m_ID ].GetAt(3).dY - OffSetY );
						m_Track [ m_ID ].GetAt(1).Create ( NMouseX,							m_Track [ m_ID ].GetAt(3).dY - OffSetY );
						m_Track [ m_ID ].GetAt(2).Create ( NMouseX,							m_Track [ m_ID ].GetAt(3).dY );
					}
					else
					{
						double OffSetX	=	-MoveSizeY * m_nAspect [ m_ID ];

						if ( OffSetX > m_nImageWidth )
							return TRUE;

						if ( NMouseY < 0.0 )
						{
							NMouseY		=	0.0;
							OffSetX		=	m_Track [ m_ID ].GetAt(2).dY * m_nAspect [ m_ID ];
						}

						if ( OffSetX < MaxX )
						{
							NMouseY		=	m_Track [ m_ID ].GetAt(2).dY - MaxY;
							OffSetX		=	MaxX;
						}

						if ( m_Track [ m_ID ].GetAt(0).dX + OffSetX > m_nImageWidth )
						{
							OffSetX		=	m_nImageWidth - m_Track [ m_ID ].GetAt(0).dX;
							NMouseY		=	m_Track [ m_ID ].GetAt(2).dY - OffSetX / m_nAspect [ m_ID ];
						}

						m_Track [ m_ID ].GetAt(0).Create ( m_Track [ m_ID ].GetAt(3).dX,			NMouseY );
						m_Track [ m_ID ].GetAt(1).Create ( m_Track [ m_ID ].GetAt(3).dX + OffSetX,	NMouseY );
						m_Track [ m_ID ].GetAt(2).Create ( m_Track [ m_ID ].GetAt(3).dX + OffSetX,	m_Track [ m_ID ].GetAt(3).dY );
					}
				}
#pragma endregion

#pragma region ("point2")

				if ( nCurrentPoint == 2 )
				{
					double SizeX		=	m_Track [ m_ID ].GetAt(2).dX - m_Track [ m_ID ].GetAt(0).dX;
					double SizeY		=	m_Track [ m_ID ].GetAt(2).dY - m_Track [ m_ID ].GetAt(0).dY;

					double MoveSizeX	=	NMouseX - m_Track [ m_ID ].GetAt(0).dX;
					double MoveSizeY	=	NMouseY - m_Track [ m_ID ].GetAt(0).dY;

					if ( abs ( MoveSizeX / MoveSizeY ) > abs ( SizeX / SizeY ) )
					{
						double OffSetY	=	MoveSizeX / m_nAspect [ m_ID ];

						if ( OffSetY > m_nImageHeight )
							return TRUE;

						if ( NMouseX > m_nImageWidth )
						{
							NMouseX		=	m_nImageWidth;
							OffSetY		=	( m_nImageWidth - m_Track [ m_ID ].GetAt(0).dX ) / m_nAspect [ m_ID ];
						}

						if ( OffSetY < MaxY )
						{
							NMouseX		=	m_Track [ m_ID ].GetAt(0).dX + MaxX;
							OffSetY		=	MaxY;
						}

						if ( m_Track [ m_ID ].GetAt(1).dY + OffSetY > m_nImageHeight )
						{
							OffSetY		=	m_nImageHeight - m_Track [ m_ID ].GetAt(1).dY;
							NMouseX		=	m_Track [ m_ID ].GetAt(0).dX + OffSetY * m_nAspect [ m_ID ];
						}

						m_Track [ m_ID ].GetAt(1).Create ( NMouseX,							m_Track [ m_ID ].GetAt(0).dY );
						m_Track [ m_ID ].GetAt(2).Create ( NMouseX,							m_Track [ m_ID ].GetAt(0).dY + OffSetY );
						m_Track [ m_ID ].GetAt(3).Create ( m_Track [ m_ID ].GetAt(0).dX,	m_Track [ m_ID ].GetAt(0).dY + OffSetY );
					}
					else
					{
						double OffSetX	=	MoveSizeY * m_nAspect [ m_ID ];

						if ( OffSetX > m_nImageWidth )
							return TRUE;

						if ( NMouseY > m_nImageHeight )
						{
							NMouseY		=	m_nImageHeight;
							OffSetX		=	( m_nImageHeight - m_Track [ m_ID ].GetAt(1).dY ) * m_nAspect [ m_ID ];
						}

						if ( OffSetX < MaxX )
						{
							NMouseY		=	m_Track [ m_ID ].GetAt(1).dY + MaxY;
							OffSetX		=	MaxX;
						}

						if ( m_Track [ m_ID ].GetAt(0).dX + OffSetX > m_nImageWidth )
						{
							OffSetX		=	m_nImageWidth - m_Track [ m_ID ].GetAt(0).dX;
							NMouseY		=	m_Track [ m_ID ].GetAt(1).dY + OffSetX / m_nAspect [ m_ID ];
						}

						m_Track [ m_ID ].GetAt(1).Create ( m_Track [ m_ID ].GetAt(0).dX + OffSetX,	m_Track [ m_ID ].GetAt(0).dY );
						m_Track [ m_ID ].GetAt(2).Create ( m_Track [ m_ID ].GetAt(0).dX + OffSetX,	NMouseY );
						m_Track [ m_ID ].GetAt(3).Create ( m_Track [ m_ID ].GetAt(0).dX,			NMouseY );
					}
				}
#pragma endregion

#pragma region ("point3")

				if ( nCurrentPoint == 3 )
				{
					double SizeX		=	m_Track [ m_ID ].GetAt(3).dX - m_Track [ m_ID ].GetAt(1).dX;
					double SizeY		=	m_Track [ m_ID ].GetAt(3).dY - m_Track [ m_ID ].GetAt(1).dY;

					double MoveSizeX	=	NMouseX - m_Track [ m_ID ].GetAt(1).dX;
					double MoveSizeY	=	NMouseY - m_Track [ m_ID ].GetAt(1).dY;

					if ( abs ( MoveSizeX / MoveSizeY ) > abs ( SizeX / SizeY ) )
					{
						double OffSetY	=	-MoveSizeX / m_nAspect [ m_ID ];

						if ( OffSetY > m_nImageHeight )
							return TRUE;

						if ( NMouseX < 0.0 )
						{
							NMouseX		=	0.0;
							OffSetY		=	m_Track [ m_ID ].GetAt(1).dX / m_nAspect [ m_ID ];
						}

						if ( OffSetY < MaxY )
						{
							NMouseX		=	m_Track [ m_ID ].GetAt(2).dX - MaxX;
							OffSetY		=	MaxY;
						}

						if ( m_Track [ m_ID ].GetAt(1).dY + OffSetY > m_nImageHeight )
						{
							OffSetY		=	( m_nImageHeight - m_Track [ m_ID ].GetAt(1).dY );
							NMouseX		=	m_Track [ m_ID ].GetAt(2).dX - OffSetY * m_nAspect [ m_ID ];
						}

						m_Track [ m_ID ].GetAt(0).Create ( NMouseX,							m_Track [ m_ID ].GetAt(1).dY );
						m_Track [ m_ID ].GetAt(2).Create ( m_Track [ m_ID ].GetAt(1).dX,	m_Track [ m_ID ].GetAt(1).dY + OffSetY );
						m_Track [ m_ID ].GetAt(3).Create ( NMouseX,							m_Track [ m_ID ].GetAt(1).dY + OffSetY );
					}
					else
					{
						double OffSetX	=	MoveSizeY * m_nAspect [ m_ID ];

						if ( OffSetX < 0.0 )
							return TRUE;

						if ( NMouseY > m_nImageHeight )
						{
							NMouseY		=	m_nImageHeight;
							OffSetX		=	( m_nImageHeight - m_Track [ m_ID ].GetAt(1).dY ) * m_nAspect [ m_ID ];
						}

						if ( OffSetX < MaxX )
						{
							NMouseY		=	m_Track [ m_ID ].GetAt(1).dY + MaxY;
							OffSetX		=	MaxX;
						}

						if ( m_Track [ m_ID ].GetAt(2).dX - OffSetX < 0.0 )
						{
							OffSetX		=	m_Track [ m_ID ].GetAt(1).dX;
							NMouseY		=	m_Track [ m_ID ].GetAt(1).dY + OffSetX / m_nAspect [ m_ID ];
						}

						m_Track [ m_ID ].GetAt(0).Create ( m_Track [ m_ID ].GetAt(1).dX - OffSetX,	m_Track [ m_ID ].GetAt(1).dY );
						m_Track [ m_ID ].GetAt(2).Create ( m_Track [ m_ID ].GetAt(1).dX,			NMouseY );
						m_Track [ m_ID ].GetAt(3).Create ( m_Track [ m_ID ].GetAt(1).dX - OffSetX,	NMouseY );
					}
				}

#pragma endregion		

				m_Track [ m_ID ].GetAt(4).Create (	0.5 * ( m_Track [ m_ID ].GetAt(0).dX + m_Track [ m_ID ].GetAt(2).dX ), 
					0.5 * ( m_Track [ m_ID ].GetAt(0).dY + m_Track [ m_ID ].GetAt(2).dY ) );

				Messages::CDirectMessage::Send ( m_pMessageReceiver, Constants::sc_nMessageTrackPointProcess,   m_ID, 0 );
			}

			return true;
		}

		bool IMouseButtonHandlers::OnLButtonDown	( int MouseX, int MouseY )
		{
			if ( FALSE == m_bInitialized )
				return false;

			m_bLMButtonDown		=	TRUE;

			int nTrackIndex		=	GetPointUnderCursor ( m_Track [ m_ID ], MouseX, MouseY );

			// смещение относительно нулевой точки
			m_nMouseOffsetX		=	MapToDataX ( MouseX ) -	m_Track [ m_ID ].GetAt ( 0 ).dX;
			m_nMouseOffsetY		=	MapToDataY ( MouseY ) -	m_Track [ m_ID ].GetAt ( 0 ).dY;

			// если не захватили точку, то проверяем попадаение в область трека
			if ( -1 == nTrackIndex )
			{
				Structures::RECTI rectBounds	= GetIntegerBoundingRectEx ( m_Track [ m_ID ] );
				Structures::POINTI pointTrack ( MouseX, MouseY );

				if ( rectBounds.IsPointInside ( pointTrack ) )
					return true;

				if ( 0 == m_ID )
				{
					nTrackIndex		=	GetPointUnderCursor ( m_Track [ 1 ], MouseX, MouseY );
					if ( -1 != nTrackIndex )
					{
						m_ID	=	1;

						return true;
					}
				}
				else
				{
					nTrackIndex		=	GetPointUnderCursor ( m_Track [ 0 ], MouseX, MouseY );
					if ( -1 != nTrackIndex )
					{
						m_ID	=	0;

						return true;
					}
				}

				return false;
			}

			if ( nTrackIndex ==  m_Track [ m_ID ].GetCount() - 1 )
			{
				m_Track [ m_ID ].GetAt ( nTrackIndex ).Create ( MapToDataX ( MouseX ), MapToDataY ( MouseY ) );
			}

			m_Track [ m_ID ].SetCurrentPointIndex ( nTrackIndex );

			Messages::CDirectMessage::Send ( m_pMessageReceiver, Constants::sc_nMessageTrackPointBegin,  m_ID, 0 );

			m_bOnMouseMove	=	FALSE;

			return true;
		}

		bool IMouseButtonHandlers::OnLButtonUp		( int MouseX, int MouseY )
		{
 			if ( FALSE == m_bInitialized )
				return false;
		
			m_bLMButtonDown		=	FALSE;

			// переключение на другой трек при щелчке без движения мыши
			if ( FALSE == m_bOnMouseMove )
			{
				int nTrackIndex		=	-1;

				if ( 0 == m_ID )
				{
					nTrackIndex		=	GetPointUnderCursor ( m_Track [ 1 ], MouseX, MouseY );
					if ( -1 != nTrackIndex )
					{
						m_ID	=	1;

						m_Track [ 0 ].SetCurrentPointIndex ( -1 );
						m_Track [ 1 ].SetCurrentPointIndex ( -1 );
			
						Messages::CDirectMessage::Send ( m_pMessageReceiver, Constants::sc_nMessageTrackPointEnd,  m_ID, 0 );

						return true;
					}
				}
				else
				{
					nTrackIndex		=	GetPointUnderCursor ( m_Track [ 0 ], MouseX, MouseY );
					if ( -1 != nTrackIndex )
					{
						m_ID	=	0;

						m_Track [ 0 ].SetCurrentPointIndex ( -1 );
						m_Track [ 1 ].SetCurrentPointIndex ( -1 );
						
						Messages::CDirectMessage::Send ( m_pMessageReceiver, Constants::sc_nMessageTrackPointEnd,  m_ID, 0 );

						return true;
					}
				}

				if ( -1 != m_Track [ 0 ].GetCurrentPointIndex () || -1 != m_Track [ 1 ].GetCurrentPointIndex () )
					Messages::CDirectMessage::Send ( m_pMessageReceiver, Constants::sc_nMessageTrackPointEnd,  m_ID, 0 );
				else
					Messages::CDirectMessage::Send ( m_pMessageReceiver, Constants::sc_nMessageTrackInvalidate, 0, 0 );
			
				m_Track [ 0 ].SetCurrentPointIndex ( -1 );
				m_Track [ 1 ].SetCurrentPointIndex ( -1 );
			
				m_bOnMouseMove	=	FALSE;

				return false;
			}

			if ( -1 != m_Track [ 0 ].GetCurrentPointIndex () || -1 != m_Track [ 1 ].GetCurrentPointIndex () )
				Messages::CDirectMessage::Send ( m_pMessageReceiver, Constants::sc_nMessageTrackPointEnd,  m_ID, 0 );
			else
				Messages::CDirectMessage::Send ( m_pMessageReceiver, Constants::sc_nMessageTrackInvalidate, 0, 0 );

			m_Track [ 0 ].SetCurrentPointIndex ( -1 );
			m_Track [ 1 ].SetCurrentPointIndex ( -1 );

			return true;
		}

		bool IMouseButtonHandlers::OnRButtonDown	( int MouseX, int MouseY )
		{
			if ( FALSE == m_bInitialized )
				return false;

			return false;
		}

		bool IMouseButtonHandlers::OnRButtonUp		( int MouseX, int MouseY )
		{
			if ( FALSE == m_bInitialized )
				return false;

			return false;
		}

		bool IMouseButtonHandlers::OnMButtonDown	( int MouseX, int MouseY )
		{
			if ( FALSE == m_bInitialized )
				return false;

			return false;
		}

		bool IMouseButtonHandlers::OnMButtonUp		( int MouseX, int MouseY )
		{
			if ( FALSE == m_bInitialized )
				return false;

			return false;
		}


		
		inline bool OnMouseWheel ( int MouseX, int MouseY )
		{
			if ( FALSE == m_bInitialized )
				return false;

			if ( 0 == m_ID )
			{
				m_ID	=	1;
			}
			else
			{
				m_ID	=	0;
			}
			
			Messages::CDirectMessage::Send ( m_pMessageReceiver, Constants::sc_nMessageTrackInvalidate, 0, 0 );

			return true;
		}

		
		inline int GetTrackID () 
		{
			return m_ID;
		}

		inline CTrackPoints& GetCurTrack ()
		{
			return m_Track [ m_ID ];
		}

	private:

		inline static int GetPointUnderCursor	( CTrackPoints& Points, int nX, int nY )
		{
			int nTrackIndex = FindByIntegerEx ( Points, nX, nY, 15, FALSE );

			if ( 3 < nTrackIndex )
				nTrackIndex = -1;

			if (nTrackIndex < 0 )
			{
				Structures::RECTI rectBounds = GetIntegerBoundingRectEx ( Points );
				Structures::POINTI pointTrack(nX, nY);

				if (rectBounds.IsPointInside(pointTrack))
					nTrackIndex = Points.GetCount() - 1;
			}

			return nTrackIndex;
		}
		inline static int FindByIntegerEx		( CTrackPoints& Points, int nX, int nY, int nEpsilon = 5, BOOL bReverse = TRUE)
		{
			if (bReverse)
			{
				for (int index = Points.GetCount() - 1; index >= 0; --index)
				{
					if (fabs(Points.GetAt(index).nX - (double)nX) < nEpsilon && fabs(Points.GetAt(index).nY - (double)nY) < nEpsilon)
						return index;
				}
			}
			else
			{
				for (int index = 0; index < Points.GetCount(); ++index)
				{
					if (fabs(Points.GetAt(index).nX - (double)nX) < nEpsilon && fabs(Points.GetAt(index).nY - (double)nY) < nEpsilon)
						return index;
				}
			}

			return -1;
		}

		inline static Structures::RECTI GetIntegerBoundingRectEx ( CTrackPoints& Points, int nIndexFirst = -1, int nIndexLast = -1 )
		{
			if (nIndexFirst < 0)
				nIndexFirst = 0;
			if (nIndexLast < 0)
				nIndexLast = Points.GetCount() - 1;
			if (nIndexLast >= Points.GetCount())
				nIndexLast = Points.GetCount() - 1;

			Structures::RECTI rectI;

			if ( Points.GetCount() < 0 || nIndexLast < nIndexFirst)
				return rectI;

			rectI.left = rectI.right = Points.GetAt(nIndexFirst).nX;
			rectI.top = rectI.bottom = Points.GetAt(nIndexFirst).nY;

			for ( int index = nIndexFirst; index <= nIndexLast; ++index )
			{
				rectI.left		=	min(rectI.left, Points.GetAt(index).nX);
				rectI.top		=	min(rectI.top, Points.GetAt(index).nY);
				rectI.right		=	max(rectI.right, Points.GetAt(index).nX);
				rectI.bottom	=	max(rectI.bottom, Points.GetAt(index).nY);
			}

			return rectI;
		}

	
	public:

		BOOL			m_bInitialized;

		int				m_ID;			
		CTrackPoints	m_Track		[ 2 ];
		double			m_nAspect	[ 2 ];

		BOOL			m_bOnMouseMove;

		BOOL			m_bLMButtonDown;

		double			m_nMouseOffsetX;
		double			m_nMouseOffsetY;
	};
}
