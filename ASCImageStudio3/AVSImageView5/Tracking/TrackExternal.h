#pragma once

#include "Tracking.h"
#include <Messages.h>

namespace Tracking
{
	//-------------------------------------------------------------------------------------
	// Внешний габаритный трэкинг
	//-------------------------------------------------------------------------------------
	class CTrackExternal : public CTrackObject
	{
		//       8	
		//       |
		//   0---4---1
		//   |       |
		//   7   9   5
		//   |       |
		//   3---6---2
	public:

		inline void			Create ( double dLeft, double dTop, double dRight, double dBottom, double dAngle = 0.0, double dStickAngle = 0.0 )
		{
			m_bIsOffesetMove		= false;
			m_bIsPointMove			= false;
			m_bBlockeMode			= true;
			m_bRotateHasOccurred	= false;
			m_nRotateAngle			= 0.0;

			// if create by mouse
			if( abs( dRight - dLeft ) < 1 || abs( dBottom - dTop ) < 1 )	m_IsFirstResize = true;
			else															m_IsFirstResize = false;

			m_dStickAngle = dStickAngle;
			
			SetType( Constants::sc_nTrackTypeRectangleRotatable );

			Clear();
			Add( dLeft, dTop );
			Add( dRight, dTop );
			Add( dRight, dBottom );
			Add( dLeft, dBottom );

			Add( 0.5 * ( dLeft + dRight ), dTop );
			Add( dRight, 0.5 * ( dTop + dBottom ) );
			Add( 0.5 * ( dLeft + dRight ), dBottom );
			Add( dLeft, 0.5 * ( dTop + dBottom ) );

			Add( 0.5 * ( dLeft + dRight ), dTop - 30 );					//rotate selector
			Add( 0.5 * ( dLeft + dRight ), 0.5 * ( dTop + dBottom ) );	//center

			Add( 0.5 * ( dLeft + dRight ), 0.5 * ( dTop + dBottom ) );

			SetCurrentPointIndex(-1);

			RotateToAlpha( rad( dAngle ) );
		}



		// IMouseButtonHandlers
		bool IMouseButtonHandlers::OnMouseMove ( int MouseX, int MouseY )
		{
			if (!IsCurrentPointValid())
				return false;

			double dX = MapToDataX(MouseX);
			double dY = MapToDataY(MouseY);

			//int nCurrentPoint = GetCurrentPointIndex();
			int nCurrentPoint = m_IsFirstResize ? 2 : GetCurrentPointIndex();

			if (nCurrentPoint == 9 || nCurrentPoint == GetCount() - 1 || GetAsyncKeyState(VK_CONTROL))
			{
				OffsetByDouble(dX - GetCurrentPoint().dX, dY - GetCurrentPoint().dY);

				//SendMessage(Constants::sc_nMessageTrackPointProcessAll, GetCurrentPointIndex());

				m_bIsOffesetMove		= true;
				m_bIsPointMove			= false;
				m_bRotateHasOccurred	= false;
			}
			else
			{
				m_bIsOffesetMove		= false;
				m_bIsPointMove			= true;
				m_bRotateHasOccurred	= false;

				// correct rectangle points
				double dx, dy;
				double eX1, eX2, eY1, eY2;

				if (nCurrentPoint <= 8)
				{	
					eX1 = 1.0; eY1 = 0.0;
					eX2 = 0.0; eY2 = 1.0;

					double mAngle = Geometry::GetAngle( GetAt(9).dX, GetAt(9).dY, GetAt(8).dX, GetAt(8).dY ) + M_PI / 2.0;
					Geometry::RotatePoint( eX1, eY1, 0.0, 0.0, mAngle );
					Geometry::RotatePoint( eX2, eY2, 0.0, 0.0, mAngle );
				}

				if (nCurrentPoint == 0)
				{
					dX -= GetAt(0).dX;
					dY -= GetAt(0).dY;

					dx = dX*eX1 + dY*eY1;
					dy = dX*eX2 + dY*eY2;

					GetAt(0).dX += dX;
					GetAt(0).dY += dY;
					GetAt(1).dX += eX2*dy;
					GetAt(1).dY += eY2*dy;
					GetAt(3).dX += eX1*dx;
					GetAt(3).dY += eY1*dx;
				}
				else if (nCurrentPoint == 1)
				{
					dX -= GetAt(1).dX;
					dY -= GetAt(1).dY;

					dx = dX*eX1 + dY*eY1;
					dy = dX*eX2 + dY*eY2;

					GetAt(1).dX += dX;
					GetAt(1).dY += dY;
					GetAt(0).dX += eX2*dy;
					GetAt(0).dY += eY2*dy;
					GetAt(2).dX += eX1*dx;
					GetAt(2).dY += eY1*dx;

				}
				else if (nCurrentPoint == 2)
				{
					dX -= GetAt(2).dX;
					dY -= GetAt(2).dY;

					dx = dX*eX1 + dY*eY1;
					dy = dX*eX2 + dY*eY2;

					GetAt(2).dX += dX;
					GetAt(2).dY += dY;
					GetAt(1).dX += eX1*dx;
					GetAt(1).dY += eY1*dx;
					GetAt(3).dX += eX2*dy;
					GetAt(3).dY += eY2*dy;
				}
				else if (nCurrentPoint == 3)
				{
					dX -= GetAt(3).dX;
					dY -= GetAt(3).dY;

					dx = dX*eX1 + dY*eY1;
					dy = dX*eX2 + dY*eY2;

					GetAt(3).dX += dX;
					GetAt(3).dY += dY;
					GetAt(0).dX += eX1*dx;
					GetAt(0).dY += eY1*dx;
					GetAt(2).dX += eX2*dy;
					GetAt(2).dY += eY2*dy;

				}
				else if (nCurrentPoint == 4)
				{
					dX -= GetAt(4).dX;
					dY -= GetAt(4).dY;

					dx = dX*eX1 + dY*eY1;
					dy = dX*eX2 + dY*eY2;

					GetAt(4).dX += dX;
					GetAt(4).dY += dY;
					GetAt(0).dX += eX2*dy;
					GetAt(0).dY += eY2*dy; 
					GetAt(1).dX += eX2*dy;
					GetAt(1).dY += eY2*dy;

				}
				else if (nCurrentPoint == 5)
				{
					dX -= GetAt(5).dX;
					dY -= GetAt(5).dY;

					dx = dX*eX1 + dY*eY1;
					dy = dX*eX2 + dY*eY2;

					GetAt(5).dX += dX;
					GetAt(5).dY += dY;
					GetAt(1).dX += eX1*dx;
					GetAt(1).dY += eY1*dx;
					GetAt(2).dX += eX1*dx;
					GetAt(2).dY += eY1*dx;					
				}
				else if (nCurrentPoint == 6)
				{
					dX -= GetAt(6).dX;
					dY -= GetAt(6).dY;

					dx = dX*eX1 + dY*eY1;
					dy = dX*eX2 + dY*eY2;

					GetAt(6).dX += dX;
					GetAt(6).dY += dY;
					GetAt(2).dX += eX2*dy;
					GetAt(2).dY += eY2*dy; 
					GetAt(3).dX += eX2*dy;
					GetAt(3).dY += eY2*dy;
				}
				else if (nCurrentPoint == 7)
				{
					dX -= GetAt(7).dX;
					dY -= GetAt(7).dY;

					dx = dX*eX1 + dY*eY1;
					dy = dX*eX2 + dY*eY2;

					GetAt(7).dX += dX;
					GetAt(7).dY += dY;
					GetAt(0).dX += eX1*dx;
					GetAt(0).dY += eY1*dx;
					GetAt(3).dX += eX1*dx;
					GetAt(3).dY += eY1*dx;	
				}				
				else if (nCurrentPoint == 8)
				{
					m_bRotateHasOccurred	= true;

					double baseAngle = Geometry::GetAngle( GetAt(9).dX, GetAt(9).dY, dX, dY );
					StickAngle( baseAngle, m_dStickAngle );
					double mAngle	 = baseAngle - Geometry::GetAngle( GetAt(9).dX, GetAt(9).dY, GetAt(8).dX, GetAt(8).dY );
					RotateToAlpha ( mAngle );

					m_nRotateAngle			= mAngle;
				}

				// recompute centers
				if (nCurrentPoint >= 0 && nCurrentPoint < 8)
				{

					GetAt(4).Create( 0.5*( GetAt(0).dX + GetAt(1).dX ), 0.5*( GetAt(1).dY + GetAt(0).dY ) );
					GetAt(5).Create( 0.5*( GetAt(1).dX + GetAt(2).dX ), 0.5*( GetAt(1).dY + GetAt(2).dY ) );
					GetAt(6).Create( 0.5*( GetAt(3).dX + GetAt(2).dX ), 0.5*( GetAt(3).dY + GetAt(2).dY ) );
					GetAt(7).Create( 0.5*( GetAt(3).dX + GetAt(0).dX ), 0.5*( GetAt(3).dY + GetAt(0).dY ) );

					double vX = GetAt(4).dX - GetAt(6).dX;
					double vY = GetAt(4).dY - GetAt(6).dY;

					double length = Geometry::GetLength( GetAt(4).dX, GetAt(4).dY,GetAt(6).dX, GetAt(6).dY );
					if( length < 1 )
					{
						vX = -eX2; 
						vY = -eY2;
					}
					else
					{
						vX /= length;
						vY /= length;
					}

					GetAt(8).dX = GetAt(4).dX + vX * 30;
					GetAt(8).dY = GetAt(4).dY + vY * 30;

					GetAt(9).Create(0.5*(GetAt(0).dX + GetAt(2).dX), 0.5 * ( GetAt(2).dY + GetAt(0).dY ) );
				}
				//SendMessage(Constants::sc_nMessageTrackPointProcess, GetCurrentPointIndex());
			}
			return true;
		}
		bool IMouseButtonHandlers::OnLButtonDown ( int MouseX, int MouseY )
		{
			int nTrackIndex = GetPointUnderCursor( MouseX, MouseY );

			if( nTrackIndex == GetCount() - 1 )
				GetAt(nTrackIndex).Create( MapToDataX( MouseX ), MapToDataY( MouseY ) );

			SetCurrentPointIndex( nTrackIndex );

			SendMessage( Constants::sc_nMessageTrackPointBegin, nTrackIndex );

			return true;
		}

		bool IMouseButtonHandlers::OnLButtonUp ( int MouseX, int MouseY )
		{
			m_IsFirstResize = false;

			if (!IsCurrentPointValid())
				return false;

			SendMessage( Constants::sc_nMessageTrackPointEnd, GetCurrentPointIndex() );

			SetCurrentPointIndex(-1);

			return true;
		}

		bool IMouseButtonHandlers::OnRButtonDown ( int MouseX, int MouseY )
		{
			return false; 
		}

		bool IMouseButtonHandlers::OnRButtonUp ( int MouseX, int MouseY )
		{
			return false; 
		}

		bool IMouseButtonHandlers::OnMButtonDown ( int MouseX, int MouseY )
		{
			return false;
		}

		bool IMouseButtonHandlers::OnMButtonUp ( int MouseX, int MouseY )
		{
			return false;
		}




		inline int			GetPointUnderCursor ( int nX, int nY )
		{
			int nTrackIndex = FindByInteger( nX, nY, c_nTrackPointSizeTouch, FALSE );

			if( nTrackIndex < 0 )
			{
				Structures::RECTI rectD;

				double dX = MapToDataX( nX );
				double dY = MapToDataY( nY );

				double dAngle = Geometry::GetAngle( GetAt(9).dX, GetAt(9).dY, GetAt(8).dX, GetAt(8).dY ) + M_PI_2;

				double tX		= 0.0;
				double tY		= 0.0;

				double cosA		= cos ( -dAngle );
				double sinA		= sin ( -dAngle );

				tX				= dX - GetAt(9).dX;
				tY				= dY - GetAt(9).dY;				
				dX				= GetAt(9).dX + tX * cosA - tY * sinA;
				dY				= GetAt(9).dY + tX * sinA + tY * cosA;

				Structures::POINTI pointTrack ( (int)dX, (int)dY );

				rectD.left		= (int)GetAt(0).dX;
				rectD.top		= (int)GetAt(0).dY;
				rectD.right		= (int)GetAt(2).dX;
				rectD.bottom	= (int)GetAt(2).dY;

				tX				= GetAt(0).dX - GetAt(9).dX;
				tY				= GetAt(0).dY - GetAt(9).dY;				
				rectD.left		= (int) ( GetAt(9).dX + tX * cosA - tY * sinA );
				rectD.top		= (int) ( GetAt(9).dY + tX * sinA + tY * cosA );	

				tX				= GetAt(2).dX - GetAt(9).dX;
				tY				= GetAt(2).dY - GetAt(9).dY;
				rectD.right		= (int) ( GetAt(9).dX + tX * cosA - tY * sinA );
				rectD.bottom	= (int) ( GetAt(9).dY + tX * sinA + tY * cosA );

				if( rectD.right < rectD.left ) 
				{ 
					double tmpRight		= rectD.right; 
					rectD.right			= rectD.left;
					rectD.left			= (int)tmpRight;
				}
				if( rectD.top > rectD.bottom ) 
				{ 
					double tmpBottom	= rectD.bottom; 
					rectD.bottom		= rectD.top;
					rectD.top			= (int)tmpBottom;
				}

				if( rectD.IsPointInside( pointTrack ) )
					nTrackIndex = GetCount() - 1;
			}

			return nTrackIndex;
		}

		inline void			RotateToAlpha ( double alpha )
		{
			for( int index = 0; index < 9; ++index )
				Geometry::RotatePoint( GetAt(index).dX, GetAt(index).dY, GetAt(9).dX, GetAt(9).dY, alpha );	
		}
		inline void			StickAngle ( double &dAngle, double angleStick )
		{
			int		countStick	 = 0;
			double  tmpBaseAngle = dAngle;
			double  rangeStick	 = rad(2);
			if( 0 == angleStick || rangeStick > angleStick ) angleStick = M_PI_2;
			if( dAngle < 0 ) angleStick = -angleStick;
			while( (dAngle < 0) ? (tmpBaseAngle < angleStick + rangeStick) : (tmpBaseAngle > angleStick - rangeStick) )
			{ 
				tmpBaseAngle -= angleStick;
				countStick++;
			}
			if( abs( tmpBaseAngle ) < rangeStick ) 
			{
				tmpBaseAngle = countStick * angleStick;
				dAngle = tmpBaseAngle;
			}
		}		


		virtual BOOL		OnSetCursor(int nX, int nY, int& nPointStatus, int& nChangeStatus)
		{
			nPointStatus = c_nTrackPointStatusNone;
			nChangeStatus = c_nTrackChangeStatusNone;

			int nTrackIndex = GetCurrentPointIndex();

			if (nTrackIndex < 0)
				nTrackIndex = GetPointUnderCursor(nX, nY);

			if ( nTrackIndex == 9 || (nTrackIndex == GetCount() - 1) || (nTrackIndex >= 0 && GetAsyncKeyState(VK_CONTROL) < 0))
			{
				nPointStatus = c_nTrackPointStatusSizeAll;
				nChangeStatus = c_nTrackChangeStatusMove;
			}
			else if ( nTrackIndex >= 0 && nTrackIndex < 8 )
			{
				nPointStatus = c_nTrackPointStatusPoint;
				nChangeStatus = c_nTrackChangeStatusSize;
			}
			else if (nTrackIndex == 8)
			{
				nPointStatus = c_nTrackPointStatusRotation;
				nChangeStatus = c_nTrackChangeStatusRotation;
			}
			else
			{
				nPointStatus = c_nTrackPointStatusNone;
				nChangeStatus = c_nTrackChangeStatusNone;
			}

			return FALSE;
		}
		virtual void		OnPaintLines(Tracking::CTrackPainter& oPainter)
		{
			oPainter.DrawRectangleRotatable(this);
		}
		virtual void		OnPaintPoints(Tracking::CTrackPainter& oPainter)
		{
			oPainter.DrawPoints(this, GetCount() - 3, c_nTrackPointSizePaint);
		}

		
		inline bool			IsAllOffsetMove () const
		{
			return 	m_bIsOffesetMove;
		}

		inline bool			IsPointMove () const
		{
			return m_bIsPointMove;
		}

		inline bool			HasOccurredRotate () const 
		{
			return m_bRotateHasOccurred;
		}

		inline double		RotateAngle () const
		{
			return m_nRotateAngle;
		}

		inline CTrackPoint	RotateCenter ()
		{
			return GetAt(9);
		}

	private:

		bool	m_IsFirstResize;
		double	m_dStickAngle;

		bool	m_bIsOffesetMove;
		bool	m_bIsPointMove;
		bool	m_bBlockeMode;

		// вращение
		bool	m_bRotateHasOccurred;
		//
		double	m_nRotateAngle;
	};
}