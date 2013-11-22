#pragma once

#include <Messages.h>
#include "Tracking.h"
#include "TrackExternal.h"

using namespace Messages;

namespace Geometry
{
	static inline Tracking::CTrackPoint CubicBezierValue ( Tracking::CTrackPoint* pPoints, const double& nBezierFactor )
	{
		double mum1		= 0;
		double mum13	= 0;
		double mu3		= 0;

		mum1			= 1 - nBezierFactor;
		mum13			= mum1 * mum1 * mum1;
		mu3				= nBezierFactor * nBezierFactor * nBezierFactor;

		return Tracking::CTrackPoint (	mum13 * pPoints[0].dX + 3 * nBezierFactor * mum1 * mum1 * pPoints[1].dX +
			3 * nBezierFactor * nBezierFactor * mum1 * pPoints[2].dX + mu3 * pPoints[3].dX, 
			mum13 * pPoints[0].dY +	3 * nBezierFactor * mum1 * mum1 * pPoints[1].dY + 
			3 * nBezierFactor * nBezierFactor * mum1 * pPoints[2].dY + mu3 * pPoints[3].dY );
	}

	static inline Tracking::CTrackPoint CubicBezierValueN ( Tracking::CTrackPoint* pPoints, const double& nBezierFactor )
	{
		double mum1		= 0;
		double mum13	= 0;
		double mu3		= 0;

		mum1			= 1 - nBezierFactor;
		mum13			= mum1 * mum1 * mum1;
		mu3				= nBezierFactor * nBezierFactor * nBezierFactor;

		return Tracking::CTrackPoint (	mum13 * pPoints[0].nX + 3 * nBezierFactor * mum1 * mum1 * pPoints[1].nX +
			3 * nBezierFactor * nBezierFactor * mum1 * pPoints[2].nX + mu3 * pPoints[3].nX,
			mum13 * pPoints[0].nY + 3 * nBezierFactor * mum1 * mum1 * pPoints[1].nY +
			3 * nBezierFactor * nBezierFactor * mum1 * pPoints[2].nY + mu3 * pPoints[3].nY );
	}

	
	static inline bool IsPointOnCubicBezier ( int nX, int nY, Tracking::CTrackPoint* pPoints, double& nBezierFactor, int SplittingCount = 200 )
	{
		int Epsilon			=	3; 
		int nTotalIter		=	SplittingCount;
		double nIterStep	=	1.0 / (double)nTotalIter; // Шаг изменения параметра t

		for ( int nCurIter = 0; nCurIter < nTotalIter; ++nCurIter )
		{
			Tracking::CTrackPoint BeginSegment	= Geometry::CubicBezierValueN ( pPoints, nCurIter * nIterStep );
			Tracking::CTrackPoint EndSegment	= Geometry::CubicBezierValueN ( pPoints, ( nCurIter - 1 ) * nIterStep );

			// точка принадлежит сегменту данной кривой
			if ( Geometry::PointOnSegmentD ( BeginSegment.dX, BeginSegment.dY, EndSegment.dX, EndSegment.dY, nX, nY, Epsilon ) )
			{
				nBezierFactor = ( (double)nCurIter - 0.5 ) * nIterStep;

				return true;
			}
		}

		return false;
	}

	
	static inline void RotatePoint ( Tracking::CTrackPoint &point, Tracking::CTrackPoint center, const double& nAngle )
	{
		double newX	= point.dX - center.dX;
		double newY	= point.dY - center.dY;

		double sinA	= sin ( nAngle );
		double cosA	= cos ( nAngle );

		point.dX	= newX * cosA - newY * sinA + center.dX;
		point.dY	= newX * sinA + newY * cosA + center.dY;
	}

	static inline void RotatePoint ( Tracking::CTrackPoint &point, Tracking::CTrackPoint center, const double& Sin, const double& Cos )
	{
		double newX	= point.dX - center.dX;
		double newY	= point.dY - center.dY;

		point.dX	= newX * Cos - newY * Sin + center.dX;
		point.dY	= newX * Sin + newY * Cos + center.dY;
	}
}

namespace Tracking
{
	class CTrackCubicBezierCurve : public CTrackObject
	{
	public:

		CTrackCubicBezierCurve ()
		{
			m_nMouseCoordX				=	0;
			m_nMouseCoordY				=	0;
			m_nBezierCurveSelected		=	-1;

			m_nRightMouseButtonIsHold	=	false;
			m_nLeftMouseButtonIsHold	=	false;

			m_nCurveSplitValue			=	0.0;

			m_bUseTrackBorder			=	true;		// габаритный трек включен
			m_pTrackBorder				=	NULL;

			m_nInnerFactor				=	0.0;
			m_pTInner					=	NULL;
			m_nTInnerInd				=	1;
			m_bCaptureTrackInner		=	false;

			m_bMouseEditMode			=	false;
		}

		virtual ~CTrackCubicBezierCurve ()
		{
			RELEASEOBJECT ( m_pTrackBorder );
			RELEASEOBJECT ( m_pTInner );
		}


		virtual void Create ( Structures::POINTD* pPoints, int nCount, CTrackObject* pTrackInner = NULL, double InnerFactor = 0.0, bool UseTrackBorder = true )
		{
			m_bUseTrackBorder	= UseTrackBorder;
			m_nInnerFactor		= InnerFactor;

			// внешний прикрепленный трек
			m_pTInner			=	pTrackInner;
			if ( NULL != m_pTInner )
			{
				m_pTInner->SetChild ( true );
				m_pTInner->SetTrackID ( -1 );
			}


			SetType ( Constants::sc_nTrackTypeCubicBezierCurve );

			Clear();
			SetSize(nCount);

			for (int index = 0; index < nCount; ++index)
				GetAt(index).Create(pPoints[index].x, pPoints[index].y);

			SetCurrentPointIndex(-1);

			if ( m_bUseTrackBorder )
			{
				m_pTrackBorder = new CTrackExternal ();
				m_pTrackBorder->Create ( 0, 0, 0, 0 );
				m_pTrackBorder->SetChild ( true );

				for ( int i = 0; i < m_pTrackBorder->GetCount(); ++i )
					m_arrExtPoints.Add ( m_pTrackBorder->GetAt(i) );

				UpdateBounding ();
			}

			//if ( NULL != m_pTInner )
			//{
			//	// кривая к которой присоединен внешний трекинг
			//	int WorkCurve			=	static_cast<int> ( floor ( m_nInnerFactor ) );
			//	double BezierFactor		=	InnerFactor - static_cast<double> ( WorkCurve );

			//		CTrackPoint Points []=	{ GetAt ( WorkCurve * 3 ), GetAt ( WorkCurve * 3 + 1 ),
			//		GetAt ( WorkCurve * 3 + 2 ), GetAt ( WorkCurve * 3 + 3 ) };

			//	CTrackPoint Offset = Geometry::CubicBezierValue ( &Points[0], BezierFactor );
			//	m_pTInner->OffsetByDouble ( Offset.dX, Offset.dY );

			//	if ( m_pTInner->IsTrackRectangleRotatable() )
			//	{

			//	}
			//}

			UpdateTheInnerTrack ();
		}

		//-------------------------------------------------------------------------------------
		// IMouseButtonHandlers
		//-------------------------------------------------------------------------------------
		virtual bool IMouseButtonHandlers::OnMouseMove ( int MouseX, int MouseY )
		{
			if ( NULL != m_pTInner )
			{
				m_pTInner->OnMouseMove ( MouseX, MouseY );
				
				if ( m_bCaptureTrackInner )
				{
					UpdateOnTheInnerTrack ();
					UpdateBounding ();

					m_nMouseCoordX	= MouseX;
					m_nMouseCoordY	= MouseY;

					m_pTInner->SendMessage ( Constants::sc_nMessageTrackPointProcess, GetCurrentPointIndex() );

					return true;
				}
			}

			Structures::RECTD RectangleRC;

			if ( m_bUseTrackBorder ) 
				m_pTrackBorder->GetDoubleBoundingRect ( 0, 3 );

			if (IsCurrentPointValid())
			{
				double dX = MapToDataX ( MouseX );
				double dY = MapToDataY ( MouseY );

				if (GetAsyncKeyState(VK_CONTROL) < 0)
				{
					//if ( false == m_bUseTrackBorder )
					//{
					//OffsetByDouble(dX - GetCurrentPoint().dX, dY - GetCurrentPoint().dY);
					////OffsetByDouble ( DeltaX, DeltaY );
					//SendMessage(Constants::sc_nMessageTrackPointProcessAll, GetCurrentPointIndex());

					////double DeltaX = MapToDataX(nX) - MapToDataX(m_nMouseCoordX);
					////double DeltaY = MapToDataX(nY) - MapToDataX(m_nMouseCoordY);

					////OffsetByDouble ( DeltaX, DeltaY );
					//}
				}
				else
				{
					// перемещение концевых точек
					if ( 0 == m_nCurrentPoint % 3 )
					{
						MoveTrackPoint ( GetCurrentPointIndex (), dX, dY );

						if ( m_bUseTrackBorder ) 
							UpdateBounding ();

						UpdateTheInnerTrack ();
						SendMessage(Constants::sc_nMessageTrackPointProcess, GetCurrentPointIndex());
					}
				}
			}
			else 
			{
				if (GetAsyncKeyState(VK_CONTROL) < 0)
				{
					//double DeltaX = MapToDataX(nX) - MapToDataX(m_nMouseCoordX);
					//double DeltaY = MapToDataX(nY) - MapToDataX(m_nMouseCoordY);

					//int PrevCurrentPoint = m_nCurrentPoint;
					//m_nCurrentPoint = 0;

					////OffsetByDouble ( DeltaX, DeltaY );

					//double dX = MapToDataX(nX);
					//double dY = MapToDataY(nY);

					//OffsetByDouble(dX - GetCurrentPoint().dX, dY - GetCurrentPoint().dY);

					//if ( m_bUseTrackBorder ) 
					//	UpdateBounding ();

					//SendMessage(Constants::sc_nMessageTrackPointProcessAll, GetCurrentPointIndex());

					//m_nCurrentPoint = PrevCurrentPoint;
				}
				else
				{
					bool Condition = false;

					if ( m_bMouseEditMode )
						Condition	=	m_nRightMouseButtonIsHold && -1 != m_nPowerPoint;
					else
						Condition	=	m_nLeftMouseButtonIsHold && -1 != m_nPowerPoint;

					// перемещение управляющих точек (виртуально хватаем за кривую, а двигаем по коэф-ту упр.точки)
					if ( Condition )
					{
						int SavePowerPoint	= m_nPowerPoint;
						int SaveCurveSelect	= m_nBezierCurveSelected;

						double dX = MapToDataX ( MouseX );
						double dY = MapToDataY ( MouseY );

						double DeltaX = MapToDataX ( MouseX ) - MapToDataX(m_nMouseCoordX);
						double DeltaY = MapToDataX ( MouseY ) - MapToDataX(m_nMouseCoordY);

						int Ind = ( m_nBezierCurveSelected - 1 ) * 3;

						const double FactorMove = 2.8;

						GetAt(Ind+1).Create ( GetAt(Ind+1).dX + DeltaX * ( 1.0 - m_nCurveSplitValue ) * FactorMove,
							GetAt(Ind+1).dY + DeltaY * ( 1.0 - m_nCurveSplitValue ) * FactorMove  );

						GetAt(Ind+2).Create ( GetAt(Ind+2).dX + DeltaX * ( m_nCurveSplitValue ) * FactorMove,
							GetAt(Ind+2).dY + DeltaY * ( m_nCurveSplitValue ) * FactorMove );

						if ( false == IsCurveStraightLine ( Ind ) )
						{
							CTrackPoint Center	= CTrackPoint ( ( GetAt ( Ind ).dX + GetAt ( Ind + 3 ).dX ) / 2.0,
								( GetAt ( Ind ).dY + GetAt ( Ind + 3 ).dY ) / 2.0 );

							// если текущая точка находится на линии между двумя 0 и 3 точками то сдвигаем эти точки
							if ( Geometry::PointOnSegment ( (int)GetAt ( Ind ).dX, (int)GetAt ( Ind ).dY, (int)GetAt ( Ind + 3 ).dX, (int)GetAt ( Ind + 3 ).dY, (int)dX, (int)dY, 5 ) )
							{
								GetAt ( Ind + 1 ).Create ( Center.dX, Center.dY );
								GetAt ( Ind + 2 ).Create ( Center.dX, Center.dY );

								BOOL Add;
								GetPointUnderCursor ( MouseX, MouseY, Add );

								if ( m_nBezierCurveSelected != SaveCurveSelect )
								{
									m_nPowerPoint			= SavePowerPoint;
									m_nBezierCurveSelected	= SaveCurveSelect;
								}
							}
						}

						//ATLTRACE ( "CurveSplitValue : %f\n", m_nCurveSplitValue );
						//ATLTRACE ("PowerPoint : %d, BezierCurveSelected : %d \n", m_nPowerPoint, m_nBezierCurveSelected);
						//ATLTRACE ("SAVE PowerPoint : %d, BezierCurveSelected : %d \n", SavePowerPoint, m_nBezierCurveSelected);


						if ( m_bUseTrackBorder ) 
							UpdateBounding ();

						UpdateTheInnerTrack ();

						SendMessage(Constants::sc_nMessageTrackPointProcess, GetCurrentPointIndex());


						if ( m_bUseTrackBorder ) 
						{
							m_arrExtPoints.RemoveAll ();

							for ( int i = 0; i < m_pTrackBorder->GetCount(); ++i )
								m_arrExtPoints.Add ( m_pTrackBorder->GetAt(i) );
						}

						m_nMouseCoordX	= MouseX;
						m_nMouseCoordY	= MouseY;

						return true;
					}
				}
			}

			if ( m_bUseTrackBorder ) 
			{
				// произведен захват габаритного прямоугольника, переместим все точки для кривых соответственнно
				if ( m_pTrackBorder->OnMouseMove ( MouseX, MouseY ) )	// сообщения не приходят от этого метода
				{
					double DeltaX = MapToDataX ( MouseX ) - MapToDataX(m_nMouseCoordX);
					double DeltaY = MapToDataX ( MouseY ) - MapToDataX(m_nMouseCoordY);

					// вращаем гарабитный трекинг
					if ( m_pTrackBorder->HasOccurredRotate () )
					{
						if ( NULL != m_pTrackBorder )
							RotateTrack ( m_pTrackBorder->RotateAngle (), m_pTrackBorder->RotateCenter () );

						if ( NULL != m_pTInner )
						{
							if ( m_pTInner->IsTrackRectangleRotatableFixedAspect() )
								RotateTrack ( m_pTInner, m_pTrackBorder->RotateAngle (), m_pTInner->GetAt ( 4 ) );
						}

						UpdateTheInnerTrack ();


						SendMessage ( Constants::sc_nMessageTrackPointProcess, -1 );
					}
					else
					{
						if ( m_pTrackBorder->IsAllOffsetMove () )	// сообщения не приходят от этого метода
						{
							OffsetByDouble ( DeltaX, DeltaY );

							//SendMessage ( Constants::sc_nMessageTrackPointProcess, -1 );

							UpdateTheInnerTrack ();

							SendMessage ( Constants::sc_nMessageTrackPointProcessAll, GetCurrentPointIndex() );
						} 
						else
							// передвинули точку прямоугольника
							if ( m_pTrackBorder->IsPointMove () )	// сообщения не приходят от этого метода
							{
								UpdateExternalRectangle ( DeltaX, DeltaY, RectangleRC );

								UpdateTheInnerTrack ();

								SendMessage ( Constants::sc_nMessageTrackPointProcess, -1 );
							}
					}

				}

				m_arrExtPoints.RemoveAll ();

				for ( int i = 0; i < m_pTrackBorder->GetCount(); ++i )
					m_arrExtPoints.Add ( m_pTrackBorder->GetAt(i) );
			}

			m_nMouseCoordX	= MouseX;
			m_nMouseCoordY	= MouseY;

			return true;
		}

		virtual bool IMouseButtonHandlers::OnLButtonDown ( int MouseX, int MouseY ) 
		{
			m_nLeftMouseButtonIsHold	=	true;

			if ( NULL != m_pTInner )
			{
				//if ( m_pTInner->IsTrackRectangleRotatableFixedAspect() )
				//{
				//	// кривая к которой присоединен внешний трекинг
				//	int WorkCurve			=	static_cast<int> ( floor ( m_nInnerFactor ) );
				//	double BezierFactor		=	m_nInnerFactor - static_cast<double> ( WorkCurve );

				//	if ( 0.0 == BezierFactor )
				//		m_bCaptureTrackInner = m_pTInner->OnLButtonDown ( MouseX, MouseY );
				//}
				//else
				{
					m_bCaptureTrackInner = m_pTInner->OnLButtonDown ( MouseX, MouseY );
				}
			}

			m_nMouseCoordX	=	MouseX;
			m_nMouseCoordY	=	MouseY;

			BOOL bCanAdd	=	FALSE;
			int nTrackIndex =	GetPointUnderCursor ( MouseX, MouseY, bCanAdd );

			// если режим редактирования мышью включен, то можем добавлять точки
			if ( m_bMouseEditMode )
			{
				if ( nTrackIndex >= 0 && bCanAdd )
					SplitCurve ( m_nBezierCurveSelected );

				SetCurrentPointIndex(nTrackIndex);
			}
			else
			{
				if ( bCanAdd )
				{
					SetCurrentPointIndex(-1);
				}
				else
				{
					SetCurrentPointIndex(nTrackIndex);
				}
				bCanAdd		=	FALSE;
			}

			//SetCurrentPointIndex(nTrackIndex);

			if ( nTrackIndex >= 0 )
			{
				//if (!bCanAdd)
				if ( TRUE == bCanAdd )
					SendMessage ( Constants::sc_nMessageTrackPointBegin, nTrackIndex );
				else
					SendMessage ( Constants::sc_nMessageTrackPointProcess, nTrackIndex );

				//SetCurrentPointIndex(-1);

				m_bCaptureTrackInner = false;

				return true;
			}

			if ( m_bCaptureTrackInner )
			{
				m_pTInner->SendMessage ( Constants::sc_nMessageTrackPointBegin, GetCurrentPointIndex() );
				return true; 
			}
			
			if ( m_bUseTrackBorder ) 
				m_pTrackBorder->OnLButtonDown ( MouseX, MouseY );

			return true; 
		}

		virtual bool IMouseButtonHandlers::OnLButtonUp ( int MouseX, int MouseY ) 
		{
			m_nLeftMouseButtonIsHold	=	false;

			bool BInnerLMB		=	false;

			if ( NULL != m_pTInner )
			{
				int CurPoint = m_pTInner->GetCurrentPointIndex ();

				m_bCaptureTrackInner = false;

				if ( -1 != CurPoint )
					m_pTInner->SetChild ( false );

				BInnerLMB	=	m_pTInner->OnLButtonUp ( MouseX, MouseY );

				if ( -1 != CurPoint )
					m_pTInner->SetChild ( true );
			}

			if ( BInnerLMB )
			{
				m_nPowerPoint = -1;
				if (!IsCurrentPointValid())
				{
					SetCurrentPointIndex(-1);

					return false;
				}

				SetCurrentPointIndex(-1);
				return true;
			}

			if ( m_bUseTrackBorder )
			{
				// m_pTrackBorder->OnLButtonUp ( MouseX, MouseY );

				if ( m_pTrackBorder->OnLButtonUp ( MouseX, MouseY ) )
				{
					m_nPowerPoint = -1;
					if (!IsCurrentPointValid())
					{
						SetCurrentPointIndex(-1);

						return false;
					}

					SetCurrentPointIndex(-1);
					return true;
				}
			}

			m_nPowerPoint = -1;

			if (!IsCurrentPointValid())
			{
				SendMessage ( Constants::sc_nMessageTrackPointEnd, -1 );
				SetCurrentPointIndex(-1);

				return false;
			}

			SendMessage ( Constants::sc_nMessageTrackPointEnd, GetCurrentPointIndex() );

			SetCurrentPointIndex(-1);

			return false; 
		}

		virtual bool IMouseButtonHandlers::OnRButtonDown ( int MouseX, int MouseY )
		{
			m_nRightMouseButtonIsHold = true;

			if ( false == m_bMouseEditMode )
				return true;

			// если режим редактирования мышью включен, то можем удалять точки
			DeleteTrackPoint ( FindByInteger ( MouseX, MouseY, c_nTrackPointSizeTouch, TRUE ) );

			return false; 
		}

		virtual bool IMouseButtonHandlers::OnRButtonUp ( int MouseX, int MouseY )
		{
			if ( false == m_bMouseEditMode )
				return true;

			m_nPowerPoint				= -1;
			m_nRightMouseButtonIsHold	= false;

			SendMessage(Constants::sc_nMessageTrackPointEnd, GetCurrentPointIndex());

			SetCurrentPointIndex(-1);

			return false; 
		}

		virtual bool IMouseButtonHandlers::OnMButtonDown ( int MouseX, int MouseY )
		{
			return false;
		}

		virtual bool IMouseButtonHandlers::OnMButtonUp ( int MouseX, int MouseY )
		{
			return false;
		}

	protected:

		inline int			GetPointUnderCursor ( int nX, int nY, BOOL& bCanAdd )
		{
			bCanAdd = FALSE;
			m_nBezierCurveSelected = -1;
			m_nPowerPoint = -1;

			int nTrackIndex = CTrackCubicBezierCurve::FindByInteger ( nX, nY, c_nTrackPointSizeTouch, FALSE );

			if ( nTrackIndex < 0 && GetCount() >= 4 )
			{
				for ( int index = 0; index + 3 < GetCount(); index += 3 )
				{
					CTrackPoint CurvePoints [] = { GetAt ( index ), GetAt ( index + 1 ), GetAt ( index + 2 ), GetAt ( index + 3 ) };

					if ( Geometry::IsPointOnCubicBezier ( nX, nY, &CurvePoints[0], m_nCurveSplitValue ) )
					{
						bCanAdd					= TRUE;
						nTrackIndex				= index + 4;

						// индекс кривой ( кривая - последовательность трех точек )
						m_nBezierCurveSelected	= nTrackIndex / 3;

						double DistanceToP0		= Geometry::GetLength ( double(nX), double (nY), GetAt(index+1).dX, GetAt(index+1).dY );
						double DistanceToP1		= Geometry::GetLength ( double(nX), double (nY), GetAt(index+2).dX, GetAt(index+2).dY );

						if ( DistanceToP0 < DistanceToP1 )
						{
							m_nPowerPoint		= index + 1;
						}
						else
						{
							m_nPowerPoint		= index + 2;
						}

						break;
					}
				}
			}

			return nTrackIndex;
		}

		inline int			GetPointUnderCursorEx ( int nX, int nY )
		{
			m_nBezierCurveSelected	= -1;
			m_nPowerPoint			= -1;

			int nTrackIndex = CTrackCubicBezierCurve::FindByInteger ( nX, nY, c_nTrackPointSizeTouch, FALSE );

			int Count = GetCount();
			if ( nTrackIndex < 0 && Count >= 4 )
			{
				for ( int index = 0; index + 3 < Count; index += 3 )
				{
					CTrackPoint CurvePoints [] = { GetAt (index), GetAt (index + 1), GetAt (index + 2), GetAt (index + 3) };

					//if ( PointOnCurve ( nX, nY, &CurvePoints[0] ) )
					if ( Geometry::IsPointOnCubicBezier ( nX, nY, &CurvePoints[0], m_nCurveSplitValue ) )
					{
						nTrackIndex				= index + 4;

						// индекс кривой ( кривая - последовательность трех точек )
						m_nBezierCurveSelected	= nTrackIndex / 3;

						double DistanceToP0		= Geometry::GetLength ( double(nX), double (nY), GetAt(index+1).dX, GetAt(index+1).dY );
						double DistanceToP1		= Geometry::GetLength ( double(nX), double (nY), GetAt(index+2).dX, GetAt(index+2).dY );

						if ( DistanceToP0 < DistanceToP1 )
						{
							m_nPowerPoint		= index + 1;
						}
						else
						{
							m_nPowerPoint		= index + 2;
						}

						break;
					}
				}
			}

			return nTrackIndex;
		}

		inline int 			GetPointUnderCursor ( int MouseX, int MouseY )
		{
			if ( false == m_bUseTrackBorder )
				return GetPointUnderCursorEx ( MouseX, MouseY );

			// проверка на попадание во внутренний трек, поскольку он может выходить за габариты исходного трека
			if ( NULL != m_pTInner )
			{
				if ( -1 !=  m_pTInner->GetPointUnderCursor ( MouseX, MouseY ) )
					return true;
			}

			return m_pTrackBorder->GetPointUnderCursor ( MouseX, MouseY );
		}


		inline int			FindByInteger(int nX, int nY, int nEpsilon = 5, BOOL bReverse = TRUE)
		{
			if ( bReverse )
			{
				for ( int index = GetCount() - 1; index >= 0; index -= 3 )
				{
					if (fabs(m_arrPoints[index].nX - (double)nX) < nEpsilon && fabs(m_arrPoints[index].nY - (double)nY) < nEpsilon)
						return index;
				}
			}
			else
			{
				for ( int index = 0; index < GetCount(); index += 3 )
				{
					if (fabs(m_arrPoints[index].nX - (double)nX) < nEpsilon && fabs(m_arrPoints[index].nY - (double)nY) < nEpsilon)
						return index;
				}
			}

			return -1;
		}	


		inline void			RotateTrack ( double nAngle, CTrackPoint& Center )
		{
			for ( int Ind = 0; Ind < GetCount(); ++Ind )
				Geometry::RotatePoint ( GetAt(Ind).dX, GetAt(Ind).dY, Center.dX, Center.dY, nAngle );	
		}
		inline void			RotateTrack ( CTrackObject* pTracking, double nAngle, CTrackPoint& Center )
		{
			for ( int Ind = 0; Ind < pTracking->GetCount(); ++Ind )
				Geometry::RotatePoint ( pTracking->GetAt(Ind).dX, pTracking->GetAt(Ind).dY, Center.dX, Center.dY, nAngle );	
		}


		inline bool			GetBounding ( CTrackPoint& Min, CTrackPoint& Max )
		{
			Min = m_PtnMin;
			Max	= m_PtnMax;

			return true;
		}



	public:

		//-------------------------------------------------------------------------------------
		// обновление внутреннего трека по текущему
		//-------------------------------------------------------------------------------------
		inline void			UpdateTheInnerTrack ()
		{
			if ( NULL != m_pTInner )
			{
				int MoveTo = m_nTInnerInd * 3;
				CTrackPoint Center;

				if ( m_pTInner->IsTrackRectangleRotatableFixedAspect() )
				{
					// кривая к которой присоединен внешний трекинг
					int SplitCurve			=	(int)m_nInnerFactor;
					double CurvePoint		=	3 * m_nInnerFactor;

					MoveTo					=	(int) CurvePoint;

					if ( MoveTo >= GetCount () )
						return;

					if ( m_pTInner->IsTrackRectangleRotatableFixedAspect() ) 
						Center.Create ( ( m_pTInner->GetAt ( 0 ).dX + m_pTInner->GetAt ( 2 ).dX ) * 0.5, ( m_pTInner->GetAt ( 0 ).dY + m_pTInner->GetAt ( 2 ).dY ) * 0.5 );

					m_pTInner->OffsetByDouble ( GetAt (MoveTo).dX - Center.dX, GetAt (MoveTo).dY - Center.dY ); 

					m_InnerPoint.Create ( GetAt (MoveTo).dX, GetAt (MoveTo).dY );
				}

				//if ( m_pTInner->IsTrackPoint() ) { } 
				//if ( m_pTInner->IsTrackLine() ) { } 

				if ( m_pTInner->IsTrackEllipse() ) // +
					Center.Create ( ( m_pTInner->GetAt ( 1 ).dX + m_pTInner->GetAt ( 3 ).dX ) * 0.5, ( m_pTInner->GetAt ( 0 ).dY + m_pTInner->GetAt ( 2 ).dY ) * 0.5 );
				if ( m_pTInner->IsTrackRectangle() ) // +
					Center.Create ( ( m_pTInner->GetAt ( 0 ).dX + m_pTInner->GetAt ( 2 ).dX ) * 0.5,( m_pTInner->GetAt ( 0 ).dY + m_pTInner->GetAt ( 2 ).dY ) * 0.5 );
				//if ( m_pTInner->IsTrackRectangleFixedSize() )
				//	Center.Create ( ( m_pTInner->GetAt ( 0 ).dX + m_pTInner->GetAt ( 2 ).dX ) * 0.5,( m_pTInner->GetAt ( 0 ).dY + m_pTInner->GetAt ( 2 ).dY ) * 0.5 );
				//if ( m_pTInner->IsTrackRectangleFixedAspect() ) 
				//	Center.Create ( ( m_pTInner->GetAt ( 0 ).dX + m_pTInner->GetAt ( 2 ).dX ) * 0.5,( m_pTInner->GetAt ( 0 ).dY + m_pTInner->GetAt ( 2 ).dY ) * 0.5 );

				//if ( m_pTInner->IsTrackPolyline() ) { }  
				//if ( m_pTInner->IsTrackPolygon() ) { } 

				if ( m_pTInner->IsTrackPie() ) // +
					Center.Create ( ( m_pTInner->GetAt ( 0 ).dX + m_pTInner->GetAt ( 2 ).dX ) * 0.5,( m_pTInner->GetAt ( 0 ).dY + m_pTInner->GetAt ( 2 ).dY ) * 0.5 );
				if ( m_pTInner->IsTrackPieSimple() ) // +
					Center.Create ( m_pTInner->GetAt ( 2 ) );
				if ( m_pTInner->IsTrackSector() )  // +
					Center.Create ( ( m_pTInner->GetAt ( 0 ).dX + m_pTInner->GetAt ( 2 ).dX ) * 0.5,( m_pTInner->GetAt ( 0 ).dY + m_pTInner->GetAt ( 2 ).dY ) * 0.5 );
				if ( m_pTInner->IsTrackSectorSimple() ) // + 
					Center.Create ( m_pTInner->GetAt ( 4 ) );

				//if ( m_pTInner->IsTrackCircle () ) { }  
				//if ( m_pTInner->IsTrackEllipseInRectangle() ) { }  

				if ( m_pTInner->IsTrackRectangleRotatable() )  // +
					Center.Create ( ( m_pTInner->GetAt ( 0 ).dX + m_pTInner->GetAt ( 2 ).dX ) * 0.5, ( m_pTInner->GetAt ( 0 ).dY + m_pTInner->GetAt ( 2 ).dY ) * 0.5 );
				if ( m_pTInner->IsTrackRectangleRotatableFixedAspect() ) 
					Center.Create ( ( m_pTInner->GetAt ( 0 ).dX + m_pTInner->GetAt ( 2 ).dX ) * 0.5, ( m_pTInner->GetAt ( 0 ).dY + m_pTInner->GetAt ( 2 ).dY ) * 0.5 );

				//if ( m_pTInner->IsTrackTextRotatable() ) { }  
				//if ( m_pTInner->IsTrackText() ) { }  
				//if ( m_pTInner->IsTrackTrajectoryPolyline() ) { } 
				//if ( m_pTInner->IsTrackTypeCubicBezierCurve () ) { }  

				m_pTInner->OffsetByDouble ( GetAt (MoveTo).dX - Center.dX, GetAt (MoveTo).dY - Center.dY ); 
			}
		}


		//-------------------------------------------------------------------------------------
		// обновление текущего по внутреннему
		//-------------------------------------------------------------------------------------
		inline void			UpdateOnTheInnerTrack ()
		{
			if ( NULL != m_pTInner )
			{
				int PointInd	=	m_nTInnerInd * 3;

				CTrackPoint MovePoint;

				if ( m_pTInner->IsTrackRectangleRotatableFixedAspect() )
				{
					// кривая к которой присоединен внешний трекинг
					// кривая к которой присоединен внешний трекинг
					int WorkCurve			=	static_cast<int> ( floor ( m_nInnerFactor ) );
					double BezierFactor		=	m_nInnerFactor - static_cast<double> ( WorkCurve );
					double CurvePoint		=	3 * m_nInnerFactor;	//(double)( GetCount() / 3 ) * m_nInnerFactor;

					//int SplitCurve			=	(int) ( GetCount () / 3 * m_nInnerFactor );
					//double SplitForCurve	=	( (SplitCurve + 1 )* m_nInnerFactor - floor ( ( SplitCurve + 1 ) * m_nInnerFactor ) );

					if ( CurvePoint - floor ( CurvePoint ) == 0.0 )
					{
						m_InnerPoint			=	CTrackPoint ( 
							( m_pTInner->GetAt ( 0 ).dX + m_pTInner->GetAt ( 2 ).dX ) * 0.5,
							( m_pTInner->GetAt ( 0 ).dY + m_pTInner->GetAt ( 2 ).dY ) * 0.5 );

						MoveTrackPoint ( (int)( CurvePoint ), m_InnerPoint.dX, m_InnerPoint.dY );
						return;
					}
				}

				//if ( m_pTInner->IsTrackPoint() ) { } 
				//if ( m_pTInner->IsTrackLine() ) { } 

				if ( m_pTInner->IsTrackEllipse() ) // +
					MovePoint	= CTrackPoint ( ( m_pTInner->GetAt ( 1 ).dX + m_pTInner->GetAt ( 3 ).dX ) * 0.5, ( m_pTInner->GetAt ( 0 ).dY + m_pTInner->GetAt ( 2 ).dY ) * 0.5 );
				if ( m_pTInner->IsTrackRectangle() ) // +
					MovePoint	= CTrackPoint ( ( m_pTInner->GetAt ( 0 ).dX + m_pTInner->GetAt ( 2 ).dX ) * 0.5, ( m_pTInner->GetAt ( 0 ).dY + m_pTInner->GetAt ( 2 ).dY ) * 0.5 );
				if ( m_pTInner->IsTrackRectangleFixedSize() )
					MovePoint	= CTrackPoint ( ( m_pTInner->GetAt ( 0 ).dX + m_pTInner->GetAt ( 2 ).dX ) * 0.5, ( m_pTInner->GetAt ( 0 ).dY + m_pTInner->GetAt ( 2 ).dY ) * 0.5 );
				if ( m_pTInner->IsTrackRectangleFixedAspect() ) 
					MovePoint	= CTrackPoint ( ( m_pTInner->GetAt ( 0 ).dX + m_pTInner->GetAt ( 2 ).dX ) * 0.5, ( m_pTInner->GetAt ( 0 ).dY + m_pTInner->GetAt ( 2 ).dY ) * 0.5 );

				//if ( m_pTInner->IsTrackPolyline() ) { }  
				//if ( m_pTInner->IsTrackPolygon() ) { } 

				if ( m_pTInner->IsTrackPie() ) // +
					MovePoint	= CTrackPoint ( ( m_pTInner->GetAt ( 0 ).dX + m_pTInner->GetAt ( 2 ).dX ) * 0.5, ( m_pTInner->GetAt ( 0 ).dY + m_pTInner->GetAt ( 2 ).dY ) * 0.5 );
				if ( m_pTInner->IsTrackPieSimple() ) // +
					MovePoint	=  m_pTInner->GetAt ( 2 );

				if ( m_pTInner->IsTrackSector() )  // +
					MovePoint	= CTrackPoint ( ( m_pTInner->GetAt ( 0 ).dX + m_pTInner->GetAt ( 2 ).dX ) * 0.5, ( m_pTInner->GetAt ( 0 ).dY + m_pTInner->GetAt ( 2 ).dY ) * 0.5 );
				if ( m_pTInner->IsTrackSectorSimple() )  // +
					MovePoint	=  m_pTInner->GetAt ( 4 );

				//if ( m_pTInner->IsTrackCircle () ) { } 
				//if ( m_pTInner->IsTrackEllipseInRectangle() ) { } 

				if ( m_pTInner->IsTrackRectangleRotatable() )  // +
					MovePoint	= CTrackPoint ( ( m_pTInner->GetAt ( 0 ).dX + m_pTInner->GetAt ( 2 ).dX ) * 0.5, ( m_pTInner->GetAt ( 0 ).dY + m_pTInner->GetAt ( 2 ).dY ) * 0.5 );

				if ( m_pTInner->IsTrackRectangleRotatableFixedAspect() ) 
					MovePoint	= CTrackPoint ( ( m_pTInner->GetAt ( 0 ).dX + m_pTInner->GetAt ( 2 ).dX ) * 0.5, ( m_pTInner->GetAt ( 0 ).dY + m_pTInner->GetAt ( 2 ).dY ) * 0.5 );

				//if ( m_pTInner->IsTrackTextRotatable() ) 
				//	MovePoint	= CTrackPoint ( ( m_pTInner->GetAt ( 0 ).dX + m_pTInner->GetAt ( 2 ).dX ) * 0.5, ( m_pTInner->GetAt ( 0 ).dY + m_pTInner->GetAt ( 2 ).dY ) * 0.5 );
				//if ( m_pTInner->IsTrackText() ) 
				//	MovePoint	= CTrackPoint ( ( m_pTInner->GetAt ( 1 ).dX + m_pTInner->GetAt ( 3 ).dX ) * 0.5, ( m_pTInner->GetAt ( 0 ).dY + m_pTInner->GetAt ( 2 ).dY ) * 0.5 );

				//if ( m_pTInner->IsTrackTrajectoryPolyline() ) { } 
				//if ( m_pTInner->IsTrackTypeCubicBezierCurve () ) { }  

				MoveTrackPoint ( PointInd, MovePoint.dX, MovePoint.dY );
			}
		}



		virtual void		SetReceiver ( CMessageReceiver* pReceiver )
		{
			m_pMessageReceiver = pReceiver;

			if ( m_bUseTrackBorder ) 
				m_pTrackBorder->SetReceiver ( m_pMessageReceiver );

			if ( NULL != m_pTInner )
				m_pTInner->SetReceiver ( m_pMessageReceiver );
		}

		virtual BOOL		OnMessages ( UINT nMessage, WPARAM wParam, LPARAM lParam )
		{
			if ( WM_LBUTTONDOWN == nMessage )
				CTrackCubicBezierCurve::OnLButtonDown ( GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) );
			else if ( WM_LBUTTONUP == nMessage )
				CTrackCubicBezierCurve::OnLButtonUp ( GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) );
			else if ( WM_MOUSEMOVE == nMessage )
				CTrackCubicBezierCurve::OnMouseMove ( GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if ( WM_RBUTTONDOWN == nMessage )
				CTrackCubicBezierCurve::OnRButtonDown ( GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) );
			else if ( WM_RBUTTONUP == nMessage )
				CTrackCubicBezierCurve::OnRButtonUp ( GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) );

			return FALSE;
		}

		virtual BOOL		OnSetCursor ( int nX, int nY, int& nPointStatus, int& nChangeStatus )
		{
			nPointStatus	= c_nTrackPointStatusNone;
			nChangeStatus	= c_nTrackChangeStatusNone;

			BOOL bCanAdd	= FALSE;
			int nTrackIndex	= GetCurrentPointIndex();

			if ( nTrackIndex < 0 )
				nTrackIndex	= GetPointUnderCursor ( nX, nY, bCanAdd );

			if ( NULL != m_pTInner )
			{
				if ( -1 == nTrackIndex )
				{
					m_pTInner->OnSetCursor ( nX, nY, nPointStatus, nChangeStatus );

					if ( nPointStatus != c_nTrackPointStatusNone && nChangeStatus != c_nTrackChangeStatusNone )
						return FALSE;
				}
			}

			if ( m_bUseTrackBorder ) 
			{
				if ( -1 == nTrackIndex )
					m_pTrackBorder->OnSetCursor ( nX, nY, nPointStatus, nChangeStatus );
			}

			if (nTrackIndex >= 0 )
			{

				if (GetAsyncKeyState(VK_CONTROL) < 0)
				{
					nPointStatus	= c_nTrackPointStatusSizeAll;
					nChangeStatus	= c_nTrackChangeStatusMove;
				}
				else if (bCanAdd)
				{
					nPointStatus	= c_nTrackPointStatusPoint; // c_nTrackPointStatusCanAdd; // движение кривой
				}
				else
				{
					nPointStatus	= c_nTrackPointStatusPoint;
					nChangeStatus	= c_nTrackChangeStatusSize;
				}
			}

			return FALSE;
		}

		virtual BOOL		OnPaint ( CTrackPainter& oPainter, Structures::RECTI rectWindow, Structures::RECTD rectData )
		{
			m_rectWindow	=	rectWindow;
			m_rectData		=	rectData;

			// check for valid tracking status
			if ( GetCount() < 3 || IsTrackNone() )
				return FALSE;

			// update tracking point coordinates
			MapByDouble ( rectData.left, rectData.top, rectData.right, rectData.bottom, rectWindow.left, rectWindow.top, rectWindow.right, rectWindow.bottom );

			// 0 - painter - рисуем линии кривых
			// 1 - painter - рисуем точки кривых
			// 2 - painter - рисуем указательную точку ( inner point )
			// 3 - painter - рисуем внешний гарабитный трекинг
			// 4 - panter  - прикрепленный трекинг

			// paint external track			- 3 - painter - рисуем внешний гарабитный трекинг
			if ( true )
			{
				if ( m_bUseTrackBorder ) 
				{
					MapByDoubleEx ( m_pTrackBorder, rectWindow, rectData );
					//MapByDoubleEx ( m_pTrackBorder, rectData.left, rectData.top, rectData.right, rectData.bottom, rectWindow.left, rectWindow.top, rectWindow.right, rectWindow.bottom );

					CTrackPainter* pPainter	=	oPainter.GetInnerPainter ( 3 );
					if ( NULL == pPainter )
					{
						m_pTrackBorder->OnPaint ( oPainter, m_rectWindow, m_rectData );
					}
					else
					{
						pPainter->Create ( oPainter.GetDC () );
						m_pTrackBorder->OnPaint ( *pPainter, m_rectWindow, m_rectData );
						pPainter->Destroy ();
					}
				}
			}

			// paint bezier lines			- 0 - painter - рисуем линии кривых
			if ( true )
			{
				CTrackPainter* pPainter	=	oPainter.GetInnerPainter ( 0 );
				if ( NULL == pPainter )
				{
					oPainter.SetPenLines ();
					OnPaintLines(oPainter);
				}
				else
				{
					pPainter->Create ( oPainter.GetDC () );
					pPainter->DrawCubicBezierCurveGdiPlus ( this );
					pPainter->Destroy ();
				}
			}

			// paint inner tracking			- 4 - painter  - прикрепленный трекинг
			if ( true )
			{
				if ( NULL != m_pTInner )
				{
					//MapByDoubleEx ( m_pTInner, rectData.left, rectData.top, rectData.right, rectData.bottom, rectWindow.left, rectWindow.top, rectWindow.right, rectWindow.bottom);

					MapByDoubleEx ( m_pTInner, rectWindow, rectData );

					CTrackPainter* pPainter	=	oPainter.GetInnerPainter ( 4 );

					if ( NULL == pPainter )
					{
						m_pTInner->OnPaint ( oPainter, m_rectWindow, m_rectData );
					}
					else
					{
						pPainter->Create ( oPainter.GetDC () );
						m_pTInner->OnPaint ( *pPainter, m_rectWindow, m_rectData );
						pPainter->Destroy ();
					}
				}
			}

			// paint bezier curve points	- 1 - painter - рисуем точки кривых
			if ( true )
			{
				CTrackPainter* pPainter	=	oPainter.GetInnerPainter ( 1 );

				if ( NULL == pPainter )
				{
					oPainter.SetPenPoints ();
					oPainter.DrawPointsCubicBezierCurve ( this, GetCount(), c_nTrackPointSizePaint );
				}
				else
				{
					pPainter->Create ( oPainter.GetDC () );
					pPainter->DrawPointsCubicBezierCurveGdiPlus ( this, GetCount(), pPainter->GetPointsWidth () );
					pPainter->Destroy ();
				}
			}

			// paint inner point			- 2 - painter - рисуем указательную точку ( inner point )
			if ( true )
			{
				int WorkCurve			=	static_cast<int> ( floor ( m_nInnerFactor ) );
				if ( WorkCurve >= GetCount () / 3 )
					--WorkCurve;

				double SplitForCurve	=	m_nInnerFactor - static_cast<double> ( WorkCurve );

				if ( ( WorkCurve + 1 ) * 3  >= GetCount () )	// проверка
					return TRUE;

				//if ( (double)WorkCurve != m_nInnerFactor )
				{
					CTrackPoint Points []	=	{ GetAt ( WorkCurve * 3 ), GetAt ( WorkCurve * 3 + 1 ),
						GetAt ( WorkCurve * 3 + 2 ), GetAt ( WorkCurve * 3 + 3 ) };

					m_InnerPoint = Geometry::CubicBezierValue ( &Points[0], SplitForCurve );
				}

				MapByDoublePoint ( m_InnerPoint, rectData.left, rectData.top,
					rectData.right, rectData.bottom,
					rectWindow.left, rectWindow.top,
					rectWindow.right, rectWindow.bottom );

				CTrackPainter* pPainter	=	oPainter.GetInnerPainter ( 2 );

				if ( NULL == pPainter )
				{
					oPainter.SetPenPoints ();
					oPainter.DrawPointCubicBezierCurve ( m_InnerPoint, c_nTrackPointSizePaint );
				}
				else
				{
					// для точки используется GDI+ отрисовщик
					pPainter->Create ( oPainter.GetDC () );
					pPainter->DrawPointCubicBezierCurve ( m_InnerPoint, pPainter->GetPointsWidth () );
					pPainter->Destroy ();
				}
			}


			return TRUE;
		}

		virtual void		OnPaintLines ( Tracking::CTrackPainter& oPainter )
		{
			oPainter.DrawCubicBezierCurve ( this );
		}

		virtual void		OnPaintPoints ( Tracking::CTrackPainter& oPainter )
		{
			oPainter.DrawPointsCubicBezierCurve ( this, GetCount(), c_nTrackPointSizePaint );
		}

		inline double		GetTrackInnerInd () 
		{
			return m_nTInnerInd;
		}

		CTrackObject*		GetTrackInner () 
		{
			return m_pTInner;
		}

		inline double		GetInnerFactor () 
		{
			return m_nInnerFactor;
		}

		inline void			SetInnerFactor ( const double& Value )
		{
			m_nInnerFactor	=	Value;
		}

		inline bool			GetSplit ( int SplitCurve, double SplitFactor, CTrackPoint* pPoints )
		{
			CTrackPoint CurvePoints [] = {	GetAt ( SplitCurve * 3 ), GetAt ( SplitCurve * 3 + 1 ),
				GetAt ( SplitCurve * 3 + 2 ), GetAt ( SplitCurve * 3 + 3 ) };

			// получаем 5 точек. т.к. остальные формируются из смежных
			return DoSplitCurve ( &CurvePoints[0], SplitFactor, pPoints );
		}

		inline bool			GetBoundExternalTrack ( CTrackPoint* pPoints, int SplittingCount = 200  )
		{
			if ( GetCount() <= 3 )
				return false;

			// вычисляем миниум и максиум точки, которые определяют ограничения положения кривых вписаных в прямоугольник
			pPoints[0]			=	GetAt ( 0 );
			pPoints[1]			=	GetAt ( 0 );

			double FuncStep		=	1.0 / (double)SplittingCount;	//	изменение T
			int CurveCount		=	GetCount() / 3;

			for ( int Curve = 0; Curve < CurveCount; ++Curve )
			{
				CTrackPoint Points[] = { GetAt ( Curve * 3 ), GetAt ( Curve * 3 + 1 ), GetAt ( Curve * 3 + 2 ), GetAt ( Curve * 3 + 3 ) };

				for ( int Split = 1; Split <= SplittingCount; ++Split )
				{
					CTrackPoint BPoint = Geometry::CubicBezierValue ( &Points[0], Split * FuncStep );

					if ( BPoint.dX < pPoints[0].dX )
						pPoints[0].dX = BPoint.dX;
					if ( BPoint.dY < pPoints[0].dY )
						pPoints[0].dY = BPoint.dY;

					if ( BPoint.dX > pPoints[1].dX )
						pPoints[1].dX = BPoint.dX;
					if ( BPoint.dY > pPoints[1].dY )
						pPoints[1].dY = BPoint.dY;
				}
			}

			// увеличим область в которую вписываются кривые соответственно
			const int DisplaceRC = 5;

			pPoints[0].dX -= DisplaceRC;
			pPoints[0].dY -= DisplaceRC;
			pPoints[1].dX += DisplaceRC;
			pPoints[1].dY += DisplaceRC;

			return true;
		}

		inline bool			GetBezierValue ( int Curve, double Factor, CTrackPoint& Point )
		{
			if ( Curve * 3 + 3 > GetCount () )
				return false;

			CTrackPoint Points []	=	{ GetAt ( Curve * 3 ), GetAt ( Curve * 3 + 1 ),
				GetAt ( Curve * 3 + 2 ), GetAt ( Curve * 3 + 3 ) };

			Point					=	Geometry::CubicBezierValue ( &Points[0], Factor );

			return true;
		}


		inline bool			GetMouseEditMode ()
		{
			return m_bMouseEditMode;
		}

		inline void			SetMouseEditMode ( bool Value )
		{
			m_bMouseEditMode	=	Value;
		}

		inline bool			GetIntersect ( int MouseX, int MouseY, long& Curve, double& Factor )	// находит кривую по данным точкам, если она есть 
		{
			int nTrackIndex = CTrackCubicBezierCurve::FindByInteger ( MouseX, MouseY, c_nTrackPointSizeTouch, FALSE );
			
			// если захвачен последний узел составной кривой
			if ( -1 != nTrackIndex )
			{
				Curve		=	nTrackIndex / 3;
				Factor		=	0.0;
				
				if ( Curve > GetCount() / 3 - 1 )
				{
					Factor	=	1.0;
					--Curve;
				}

				return true;
			}

			for ( int index = 0; index + 3 < GetCount(); index += 3 )
			{
				CTrackPoint CurvePoints [] = { GetAt ( index ), GetAt ( index + 1 ), GetAt ( index + 2 ), GetAt ( index + 3 ) };

				if ( Geometry::IsPointOnCubicBezier ( MouseX, MouseY, &CurvePoints[0], Factor ) )
				{
					Curve	=	index / 3;

					return true;
				}
			}

			return false;
		}

	private:

		inline void			DeleteTrackPoint ( int nTrackIndex )
		{
			// удаляем точки, в том случае если есть хотя бы большой одной кривой
			if ( GetCount() <= 4 || nTrackIndex < 0 )
				return;

			int DeleteIndex = ( nTrackIndex / 3 ) * 3;	// индекс с которого нужно удалить все точки кривой

			// если удаляем последнею точку кривой в списке точек, то отработаем этот случа	отдельно
			if ( GetCount () - 1 == DeleteIndex )
			{
				m_arrPoints.RemoveAt ( DeleteIndex - 2, 3 );
			}
			else if ( 0 == DeleteIndex )
			{
				m_arrPoints.RemoveAt ( 0, 3 );
			}
			else
			{
				// внутри удаление с сохранением смежных направляющих.
				m_arrPoints.RemoveAt ( DeleteIndex - 1, 3 );
			}

			SetCurrentPointIndex(-1);

			SendMessage(Constants::sc_nMessageTrackPointProcess, nTrackIndex);

			if ( m_bUseTrackBorder ) 
				UpdateBounding ();

			UpdateTheInnerTrack ();
		}

		inline void			MoveTrackPoint ( int PointInd, double dX, double dY )
		{
			// перемещение концевых точек
			if ( 0 == PointInd % 3 )
			{
				// относительно смещение координат мыши
				double MouseMoveX	= GetAt(PointInd).dX - dX;
				double MouseMoveY	= GetAt(PointInd).dY - dY;

				// проверям смежные прямые на принадлежность к типу "прямая"
				if ( IsCurveStraightLine ( PointInd - 3 ) )
				{
					CTrackPoint oPoint	= CTrackPoint ( dX, dY );
					CTrackPoint Center	= CTrackPoint ( ( oPoint.dX + GetAt ( PointInd - 3 ).dX ) * 0.5,
						( oPoint.dY + GetAt ( PointInd - 3 ).dY ) * 0.5 );
					// если они образуют прямую, то смещаем точки по прямой
					GetAt ( PointInd - 1 ).Create ( Center.dX, Center.dY );
					GetAt ( PointInd - 2 ).Create ( Center.dX, Center.dY );
				}
				else
				{
					// передвигаем смежную точку с данной опорной
					int PrevInd			= PointInd - 1;
					if ( PrevInd >= 0 )
					{
						m_arrPoints[PrevInd].dX -= MouseMoveX;
						m_arrPoints[PrevInd].dY -= MouseMoveY;
					}
				}

				// проверям смежные прямые на принадлежность к типу "прямая"
				if ( IsCurveStraightLine ( PointInd ) )
				{
					// опорные точки, разрешается двигать как обычные трэки
					CTrackPoint oPoint	= CTrackPoint ( dX, dY );

					CTrackPoint Center	= CTrackPoint ( ( oPoint.dX + GetAt ( PointInd + 3 ).dX ) * 0.5,
						( oPoint.dY + GetAt ( PointInd + 3 ).dY ) * 0.5 );
					// если они образуют прямую, то смещаем точки по прямой
					GetAt ( PointInd + 1 ).Create ( Center.dX, Center.dY );
					GetAt ( PointInd + 2 ).Create ( Center.dX, Center.dY );
				}
				else
				{
					// передвигаем смежную точку с данной опорной
					int NextInd			= PointInd + 1;
					if ( NextInd < GetCount () )
					{
						m_arrPoints[NextInd].dX -= MouseMoveX;
						m_arrPoints[NextInd].dY -= MouseMoveY;
					}
				}

				// опорные точки, разрешается двигать как обычные трэки
				GetAt(PointInd).Create ( dX, dY );
			}
		}

		inline void			SplitCurve ( int nCurveSplit )
		{
			if ( -1 != nCurveSplit )
			{
				int nCurvePointInd = ( nCurveSplit - 1 ) * 3 + 1;

				CTrackPoint CurvePoints [] = {	GetAt ( nCurvePointInd - 1 ), GetAt ( nCurvePointInd ),
					GetAt ( nCurvePointInd + 1 ), GetAt ( nCurvePointInd + 2 ) };

				// получаем 5 точек. т.к. остальные формируются из смежных
				CTrackPoint ResultCurves [5];	
				if ( DoSplitCurve ( &CurvePoints[0], m_nCurveSplitValue, &ResultCurves[0] ) )
				{
					m_arrPoints.RemoveAt ( nCurvePointInd );
					m_arrPoints.RemoveAt ( nCurvePointInd );

					InsertAt ( nCurvePointInd, ResultCurves[4] );
					InsertAt ( nCurvePointInd, ResultCurves[3] );
					InsertAt ( nCurvePointInd, ResultCurves[2] );
					InsertAt ( nCurvePointInd, ResultCurves[1] );
					InsertAt ( nCurvePointInd, ResultCurves[0] );
				}
				UpdateTheInnerTrack ();
			}
		}

		inline bool			UpdateBounding ()
		{
			if ( GetCount() <= 3 )
				return false;

			// вычисляем миниум и максиум точки, которые определяют ограничения положения кривых вписаных в прямоугольник
			m_PtnMin			= GetAt ( 0 );
			m_PtnMax			= GetAt ( 0 );

			int nToltalIters	= 200;
			double FuncStep		= 1.0 / (double)nToltalIters; // изменение T

			int CurveCount = GetCount() / 3 ;
			for ( int Curve = 0; Curve < CurveCount; ++Curve )
			{
				for ( int Iter = 1; Iter <= nToltalIters; ++Iter )
				{
					CTrackPoint Points[]	=	{ GetAt ( Curve * 3 ), GetAt ( Curve * 3 + 1 ), GetAt ( Curve * 3 + 2 ), GetAt ( Curve * 3 + 3 ) };
					CTrackPoint ValueOnFunc	=	Geometry::CubicBezierValue ( &Points[0], Iter * FuncStep );

					if ( ValueOnFunc.dX < m_PtnMin.dX )
						m_PtnMin.dX = ValueOnFunc.dX;
					if ( ValueOnFunc.dY < m_PtnMin.dY )
						m_PtnMin.dY = ValueOnFunc.dY;

					if ( ValueOnFunc.dX > m_PtnMax.dX )
						m_PtnMax.dX = ValueOnFunc.dX;
					if ( ValueOnFunc.dY > m_PtnMax.dY )
						m_PtnMax.dY = ValueOnFunc.dY;
				}
			}

			// увеличим область в которую вписываются кривые соответственно
			const int DisplaceRC = 5;

			m_PtnMin.dX -= DisplaceRC;
			m_PtnMin.dY -= DisplaceRC;
			m_PtnMax.dX += DisplaceRC;
			m_PtnMax.dY += DisplaceRC;

			m_pTrackBorder->Create ( m_PtnMin.dX, m_PtnMin.dY, m_PtnMax.dX, m_PtnMax.dY );

			return true;
		}

		inline bool			UpdateExternalRectangle ( const double& DeltaX, const double& DeltaY, Structures::RECTD& RectangleRC )
		{
			// корректирует точки кривой в зависимости от изменений прямоугольника в который вписали

			//       8	
			//       |
			//   0---4---1
			//   |       |
			//   7   9   5
			//   |       |
			//   3---6---2

			CTrackPoint PrevCenter ( m_arrExtPoints[9] );
			CTrackPoint Center ( m_pTrackBorder->GetAt ( 9 ) );

			CTrackPoint UpPoint ( m_pTrackBorder->GetAt ( 8 ) );
			CTrackPoint FirstPoint ( m_pTrackBorder->GetAt ( 0 ) );
			CTrackPoint RightPoint ( m_pTrackBorder->GetAt ( 1 ) );
			CTrackPoint BottomPoint ( m_pTrackBorder->GetAt ( 3 ) );

			double AbsAngle		= Geometry::GetAngle ( Center.dX, Center.dY, UpPoint.dX, UpPoint.dY ) + M_PI_2;
			double WidthPrev	= Geometry::GetLength ( m_arrExtPoints[0].dX, m_arrExtPoints[0].dY, m_arrExtPoints[1].dX, m_arrExtPoints[1].dY );
			double HeightPrev	= Geometry::GetLength ( m_arrExtPoints[0].dX, m_arrExtPoints[0].dY, m_arrExtPoints[3].dX, m_arrExtPoints[3].dY );
			double WidthNew		= Geometry::GetLength ( FirstPoint.dX, FirstPoint.dY, RightPoint.dX, RightPoint.dY );
			double HeightNew	= Geometry::GetLength ( FirstPoint.dX, FirstPoint.dY, BottomPoint.dX, BottomPoint.dY );

			CTrackPoint Correction	= CTrackPoint ( WidthNew / WidthPrev, HeightNew / HeightPrev );

			double SinM			= sin ( -AbsAngle );
			double CosM			= cos ( -AbsAngle );

			double SinN			= sin ( AbsAngle );
			double CosN			= cos ( AbsAngle );

			for ( int Ind = 0; Ind < GetCount (); ++Ind )
			{
				CTrackPoint NewPoint ( GetAt ( Ind ).dX - PrevCenter.dX, GetAt ( Ind ).dY - PrevCenter.dY );

				Geometry::RotatePoint ( NewPoint, CTrackPoint(0,0), SinM, CosM );

				NewPoint.dX *= Correction.dX;
				NewPoint.dY *= Correction.dY;

				Geometry::RotatePoint ( NewPoint, CTrackPoint(0,0), SinN, CosN );

				GetAt ( Ind ).Create ( Center.dX + NewPoint.dX, Center.dY + NewPoint.dY );
			}

			return true;
		}



		inline bool			DoSplitCurve ( CTrackPoint* pPoints, const double& SplitValue, CTrackPoint* pCurves )
		{
			double SplitM	= ( 1 - SplitValue );

			pCurves[0].dX	= pPoints[0].dX * SplitM + pPoints[1].dX * SplitValue;
			pCurves[0].dY	= pPoints[0].dY * SplitM + pPoints[1].dY * SplitValue;

			double PtnAdjX	= pPoints[1].dX * SplitM + pPoints[2].dX * SplitValue;
			double PtnAdjY	= pPoints[1].dY * SplitM + pPoints[2].dY * SplitValue;

			pCurves[4].dX	= pPoints[2].dX * SplitM + pPoints[3].dX * SplitValue;
			pCurves[4].dY	= pPoints[2].dY * SplitM + pPoints[3].dY * SplitValue;

			pCurves[1].dX	= pCurves[0].dX * SplitM + PtnAdjX * SplitValue;
			pCurves[1].dY	= pCurves[0].dY * SplitM + PtnAdjY * SplitValue;

			pCurves[3].dX	= PtnAdjX * SplitM + pCurves[4].dX * SplitValue;
			pCurves[3].dY	= PtnAdjY * SplitM + pCurves[4].dY * SplitValue;

			pCurves[2].dX	= pCurves[1].dX * SplitM + pCurves[3].dX * SplitValue;
			pCurves[2].dY	= pCurves[1].dY * SplitM + pCurves[3].dY * SplitValue;

			return true;
		}
		inline bool			IsCurveStraightLine ( int Ind )
		{
			if ( Ind < 0 || (Ind + 3) >= GetCount () )
				return false;

			CTrackPoint Center	= CTrackPoint ( ( GetAt ( Ind ).dX + GetAt ( Ind + 3 ).dX ) / 2.0,
				( GetAt ( Ind ).dY + GetAt ( Ind + 3 ).dY ) / 2.0 );

			int Value1 = Geometry::PointOnSegment ( (int)GetAt ( Ind ).dX, (int)GetAt ( Ind ).dY,
				(int)GetAt ( Ind + 3 ).dX, (int)GetAt ( Ind + 3 ).dY, 
				(int)GetAt ( Ind + 1 ).dX, (int)GetAt ( Ind + 1 ).dY, 4 );
			int Value2 = Geometry::PointOnSegment ( (int)GetAt ( Ind ).dX, (int)GetAt ( Ind ).dY,
				(int)GetAt ( Ind + 3 ).dX, (int)GetAt ( Ind + 3 ).dY, 
				(int)GetAt ( Ind + 2 ).dX, (int)GetAt ( Ind + 2 ).dY, 4 );

			if ( Value1 && Value2 )
				return true;

			return false;
		}



	protected:

		static void			MapByDoubleEx ( Tracking::CTrackObject* pTrack, Structures::RECTI& rectWindow, Structures::RECTD& rectData )
		{
			double dKoefX = ((double)rectWindow.right - (double)rectWindow.left)/(rectData.right - rectData.left);
			double dKoefY = ((double)rectWindow.bottom - (double)rectWindow.top)/(rectData.bottom - rectData.top);

			for ( int index = 0; index < pTrack->GetCount(); ++index )
			{
				pTrack->GetAt(index).nX = (int)(0.5 + (double)rectWindow.left + dKoefX * ( pTrack->GetAt(index).dX - rectData.left ) );
				pTrack->GetAt(index).nY = (int)(0.5 + (double)rectWindow.top + dKoefY * ( pTrack->GetAt(index).dY - rectData.top ) );
			}
		}
		static void			MapByDoubleEx ( Tracking::CTrackObject* pTrack, double dL, double dT, double dR, double dB, int nL, int nT, int nR, int nB )
		{
			double dKoefX = (nR - nL)/(dR - dL);
			double dKoefY = (nB - nT)/(dB - dT);

			for ( int index = 0; index < pTrack->GetCount(); ++index )
			{
				pTrack->GetAt(index).nX = (int)(0.5 + nL + dKoefX * ( pTrack->GetAt(index).dX - dL ) );
				pTrack->GetAt(index).nY = (int)(0.5 + nT + dKoefY * ( pTrack->GetAt(index).dY - dT ) );
			}
		}

		static void			MapByDoublePoint ( CTrackPoint& Point, double dL, double dT, double dR, double dB, int nL, int nT, int nR, int nB )
		{
			double dKoefX = (nR - nL)/(dR - dL);
			double dKoefY = (nB - nT)/(dB - dT);

			Point.nX = (int)(0.5 + nL + dKoefX * ( Point.dX - dL ) );
			Point.nY = (int)(0.5 + nT + dKoefY * ( Point.dY - dT ) );
		}

		static void			MapByInteger ( Tracking::CTrackObject* pTrack, int nL, int nT, int nR, int nB, double dL, double dT, double dR, double dB)
		{
			double dKoefX = (dR - dL)/(nR - nL);
			double dKoefY = (dB - dT)/(nB - nT);

			for (int index = 0; index < pTrack->GetCount(); ++index)
			{
				pTrack->GetAt(index).dX = dL + dKoefX * ( pTrack->GetAt(index).nX - nL );
				pTrack->GetAt(index).dY = dT + dKoefY * ( pTrack->GetAt(index).nY - nT );
			}
		}


	private:

		// режим в котором можно добавлять удалять точки на кривой с помощью мыши
		bool					m_bMouseEditMode;

		// используем ограничительный прямоугольник
		bool					m_bUseTrackBorder;

		// внешний габаритный ограничительный прямоугольник
		CTrackExternal*			m_pTrackBorder;
		CAtlArray <CTrackPoint>	m_arrExtPoints;

		// коэффициент для присоединенного трекинга
		double					m_nInnerFactor;

		// используем внутренний прямоугольник центр которого расположен на кривой
		CTrackObject*			m_pTInner;
		CTrackPoint				m_InnerPoint;

		int						m_nTInnerInd;				// Позиция на кривых в [0;1]
		bool					m_bCaptureTrackInner;

		// правая кнопка удерживается
		bool					m_nRightMouseButtonIsHold;
		bool					m_nLeftMouseButtonIsHold;

		int						m_nMouseCoordX;
		int						m_nMouseCoordY;

		int						m_nBezierCurveSelected;		// Select Curve
		int						m_nPowerPoint;				// Select 

		double					m_nCurveSplitValue;			// коэффцииент расшепления кривой

		CTrackPoint				m_PtnMin;
		CTrackPoint				m_PtnMax;
	};

}

namespace Tracking
{
	struct Point2D
	{
		Point2D () : m_nX ( 0.0 ), m_nY ( 0.0 )
		{ 

		}

		Point2D ( double nX, double nY ) : m_nX ( nX ), m_nY ( nY )
		{ 

		}

		double m_nX;
		double m_nY;
	};

	struct CurveCubicBezier
	{
	public:
		inline bool ReadFromString ( const CString& Points )
		{
			int ReadPos = 0;

			// read 0 - point
			m_Points[0].m_nX = _tstof ( Points.Tokenize ( _T(" "), ReadPos ) );
			if ( -1 == ReadPos ) 
				return false;
			m_Points[0].m_nY = _tstof ( Points.Tokenize ( _T(" "), ReadPos ) );
			if ( -1 == ReadPos )
				return false;

			// read 1 - point
			m_Points[1].m_nX = _tstof ( Points.Tokenize ( _T(" "), ReadPos ) );
			if ( -1 == ReadPos )
				return false;
			m_Points[1].m_nY = _tstof ( Points.Tokenize ( _T(" "), ReadPos ) );
			if ( -1 == ReadPos )
				return false;

			// read 2 - point
			m_Points[2].m_nX = _tstof ( Points.Tokenize ( _T(" "), ReadPos ) );
			if ( -1 == ReadPos )
				return false;
			m_Points[2].m_nY = _tstof ( Points.Tokenize ( _T(" "), ReadPos ) );
			if ( -1 == ReadPos )
				return false;

			// read 3 point
			m_Points[3].m_nX = _tstof ( Points.Tokenize ( _T(" "), ReadPos ) );
			if ( -1 == ReadPos ) 
				return false;

			m_Points[3].m_nY = _tstof ( Points.Tokenize ( _T(" "), ReadPos ) );

			return true;
		}

		inline Point2D GetValue ( Point2D* pPoints, const double& nBezierFactor )
		{
			double mum1		= 0;
			double mum13	= 0;
			double mu3		= 0;

			mum1			= 1 - nBezierFactor;
			mum13			= mum1 * mum1 * mum1;
			mu3				= nBezierFactor * nBezierFactor * nBezierFactor;

			return Point2D (	mum13 * pPoints[0].m_nX + 3 * nBezierFactor * mum1 * mum1 * pPoints[1].m_nX +
				3 * nBezierFactor * nBezierFactor * mum1 * pPoints[2].m_nX + mu3 * pPoints[3].m_nX, 
				mum13 * pPoints[0].m_nY +	3 * nBezierFactor * mum1 * mum1 * pPoints[1].m_nY + 
				3 * nBezierFactor * nBezierFactor * mum1 * pPoints[2].m_nY + mu3 * pPoints[3].m_nY );
		}

		inline Point2D GetValue ( double nFactor )
		{
			double mum1		= 0;
			double mum13	= 0;
			double mu3		= 0;

			mum1			= 1 - nFactor;
			mum13			= mum1 * mum1 * mum1;
			mu3				= nFactor * nFactor * nFactor;

			return Point2D (	mum13 * m_Points[0].m_nX + 3 * nFactor * mum1 * mum1 * m_Points[1].m_nX +
				3 * nFactor * nFactor * mum1 * m_Points[2].m_nX + mu3 * m_Points[3].m_nX,

				mum13 * m_Points[0].m_nY +	3 * nFactor * mum1 * mum1 * m_Points[1].m_nY + 
				3 * nFactor * nFactor * mum1 * m_Points[2].m_nY + mu3 * m_Points[3].m_nY );
		}

		inline void GetSplit ( double nFactor, Point2D* pPoints )
		{
			double nNFactor	= ( 1.0 - nFactor );

			pPoints[0].m_nX		=	m_Points[0].m_nX * nNFactor + m_Points[1].m_nX * nFactor;
			pPoints[0].m_nY		=	m_Points[0].m_nY * nNFactor + m_Points[1].m_nY * nFactor;

			double PtnAdjX		=	m_Points[1].m_nX * nNFactor + m_Points[2].m_nX * nFactor;
			double PtnAdjY		=	m_Points[1].m_nY * nNFactor + m_Points[2].m_nY * nFactor;

			pPoints[4].m_nX		=	m_Points[2].m_nX * nNFactor + m_Points[3].m_nX * nFactor;
			pPoints[4].m_nY		=	m_Points[2].m_nY * nNFactor + m_Points[3].m_nY * nFactor;

			pPoints[1].m_nX		=	pPoints[0].m_nX * nNFactor + PtnAdjX * nFactor;
			pPoints[1].m_nY		=	pPoints[0].m_nY * nNFactor + PtnAdjY * nFactor;

			pPoints[3].m_nX		=	PtnAdjX * nNFactor + pPoints[4].m_nX * nFactor;
			pPoints[3].m_nY		=	PtnAdjY * nNFactor + pPoints[4].m_nY * nFactor;

			pPoints[2].m_nX		=	pPoints[1].m_nX * nNFactor + pPoints[3].m_nX * nFactor;
			pPoints[2].m_nY		=	pPoints[1].m_nY * nNFactor + pPoints[3].m_nY * nFactor;
		}

		inline void GetDimensions ( Point2D& ptMax, Point2D& ptMin, int SplittingCount = 200 )
		{
			int nTotalIters		=	SplittingCount;
			double nFactor		=	1.0 / (double)nTotalIters;		// изменение T

			ptMax				=	m_Points[0];
			ptMin				=	m_Points[0];

			for ( int Split = 1; Split <= SplittingCount; ++Split )
			{
				Point2D BPoint = GetValue ( Split * nFactor );

				if ( BPoint.m_nX < ptMin.m_nX )
					ptMin.m_nX	=	BPoint.m_nX;

				if ( BPoint.m_nY < ptMin.m_nY )
					ptMin.m_nY	=	BPoint.m_nY;

				if ( BPoint.m_nX > ptMax.m_nX )
					ptMax.m_nX	=	BPoint.m_nX;

				if ( BPoint.m_nY > ptMax.m_nY )
					ptMax.m_nY	=	BPoint.m_nY;
			}
		}


		inline static bool CreateCurvesFromString ( const CString& Points, CSimpleArray<CurveCubicBezier>& arrCurves )
		{
			CSimpleArray <Point2D> arrPoints;
			int ReadPos = 0;

			while ( ReadPos < Points.GetLength () )
			{
				if ( -1 == ReadPos )
					break;

				double DX = _tstof ( Points.Tokenize ( _T(" "), ReadPos ) );
				double DY = _tstof ( Points.Tokenize ( _T(" "), ReadPos ) );

				arrPoints.Add ( Point2D ( DX, DY ) );
			}

#ifdef _DEBUG
			if ( 1 != arrPoints.GetSize() % 3 )
			{
				::OutputDebugString ( _T("CreateCurvesFromString - Not enough points\n") );
				return false;
			}
#endif

			int BCount = arrPoints.GetSize() / 3;

			for ( int i = 0; i < BCount; ++i )
			{
				CurveCubicBezier oCurve;

				for ( int PtInd = 0; PtInd < 4; ++PtInd )
				{
					oCurve.m_Points[PtInd].m_nX		=	arrPoints [ i * 3 + PtInd ].m_nX;
					oCurve.m_Points[PtInd].m_nY		=	arrPoints [ i * 3 + PtInd ].m_nY;
				}

				arrCurves.Add ( oCurve );
			}

			return ( arrCurves.GetSize () != 0 ) ? true : false;
		}

	public:

		Point2D	m_Points[4];
	};
}
