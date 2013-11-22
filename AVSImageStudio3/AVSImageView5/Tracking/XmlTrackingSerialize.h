#pragma once

#include <XmlUtils.h>
#include "Tracking.h"

#include "TrackExternal.h"
#include "TrackCubicBezierCurve.h"
#include "KenBurnsTrack.h"

using namespace XmlUtils;
using namespace Tracking;

namespace Core
{
	namespace Serialize
	{
		namespace Constants
		{
			const CString ConstTrack 								= _T("track");
			const CString ConstName 								= _T("name");

			const CString ConstTrackPoint 							= _T("point");
			const CString ConstTrackLine 							= _T("line");
			const CString ConstTrackEllipse 						= _T("ellipse");
			const CString ConstTrackRectangle	 					= _T("rectangle");
			const CString ConstTrackRectangleFixedSize 				= _T("rectanglefixedsize");
			const CString ConstTrackRectangleFixedAspect 			= _T("rectanglefixedaspect");
			const CString ConstTrackPolyline 						= _T("polyline");
			const CString ConstTrackPolygon 						= _T("polygon");
			const CString ConstTrackPie 							= _T("pie");
			const CString ConstTrackSector 							= _T("sector");
			const CString ConstTrackSimpleSector 					= _T("simplesector");
			const CString ConstTrackSimplePie 						= _T("simplepie");
			const CString ConstTrackEllipseInRectangle 				= _T("ellipseinrectangle");
			const CString ConstTrackCircle 							= _T("circle");
			const CString ConstTrackRectangleRotatable 				= _T("rectanglerotatable");
			const CString ConstTrackRectangleRotatableFixedAspect 	= _T("rectanglerotatablefixedaspect");
			const CString ConstTrackTextRotatable 					= _T("textrotatable");
			const CString ConstTrackText 							= _T("text");
			const CString ConstTrackTrajectoryPolyline 				= _T("trajectorypolyline");
			const CString ConstTrackCubicBezierCurve 				= _T("cubicbeziercurve");
			const CString ConstTrackEffectKenBurns					= _T("trackeffectkenburns");
		};
	}
}

namespace Core
{	
	namespace Serialize
	{
		class CTrackXmlWriter
		{
		public:
			CTrackXmlWriter ( double nAspect, int nAspectWidth, int nAspectHeight, int nWidth, int nHeight )
			{
				m_nAspect		= nAspect;
				m_nAspectWidth	= nAspectWidth;
				m_nAspectHeight	= nAspectHeight;
				m_nWidth		= nWidth;
				m_nHeight		= nHeight;
			}
			inline bool WriteXml ( Tracking::CTrackObject* pTrackObject, CString& XmlStr )
			{
				if ( pTrackObject->IsTrackNone() )
					return false;

				CXmlWriter oXmlWriter;

				oXmlWriter.WriteNodeBegin ( _TEXT("track"), TRUE );
				oXmlWriter.WriteAttribute ( _TEXT("name"), GetNodeTypeName (pTrackObject) );

				if ( pTrackObject->IsTrackPoint() )
				{
					oXmlWriter.WriteAttribute ( _TEXT("X"), FromInteger ( AspectXAdd ( pTrackObject->GetAt(0).dX ) ) );
					oXmlWriter.WriteAttribute ( _TEXT("Y"), FromInteger ( AspectYAdd ( pTrackObject->GetAt(0).dY ) ) );

					oXmlWriter.WriteNodeEnd ( GetNodeTypeName (pTrackObject), TRUE );

					XmlStr = oXmlWriter.GetXmlString ();

					return true;
				}

				if ( pTrackObject->IsTrackLine() )
				{
					oXmlWriter.WriteAttribute ( _TEXT("X1"), FromInteger ( AspectXAdd ( pTrackObject->GetAt(0).dX ) ) );
					oXmlWriter.WriteAttribute ( _TEXT("Y1"), FromInteger ( AspectYAdd ( pTrackObject->GetAt(0).dY ) ) );
					oXmlWriter.WriteAttribute ( _TEXT("X2"), FromInteger ( AspectXAdd ( pTrackObject->GetAt(1).dX ) ) );
					oXmlWriter.WriteAttribute ( _TEXT("Y2"), FromInteger ( AspectYAdd ( pTrackObject->GetAt(1).dY ) ) );

					oXmlWriter.WriteNodeEnd ( GetNodeTypeName (pTrackObject), TRUE );

					XmlStr = oXmlWriter.GetXmlString ();

					return true;
				}

				if ( pTrackObject->IsTrackEllipse() )
				{
					Structures::RECTD EllipseRC = pTrackObject->GetDoubleBoundingRect ( 0, 3 );
					EllipseRC.Normalize();

					oXmlWriter.WriteAttribute ( _TEXT("Left"),	FromInteger ( AspectXAdd ( EllipseRC.left) ) );
					oXmlWriter.WriteAttribute ( _TEXT("Top"),	FromInteger ( AspectYAdd ( EllipseRC.top ) ) );
					oXmlWriter.WriteAttribute ( _TEXT("Right"),	FromInteger ( AspectXAdd ( EllipseRC.right ) ) );
					oXmlWriter.WriteAttribute ( _TEXT("Bottom"),FromInteger ( AspectYAdd ( EllipseRC.bottom ) ) );

					oXmlWriter.WriteNodeEnd ( GetNodeTypeName (pTrackObject), TRUE );

					XmlStr = oXmlWriter.GetXmlString ();

					return true;
				}

				if ( pTrackObject->IsTrackRectangle() )
				{
					Structures::RECTD RectangleRC = pTrackObject->GetDoubleBoundingRect ( 0, 3 );
					RectangleRC.Normalize();

					oXmlWriter.WriteAttribute ( _TEXT("Left"),	FromInteger ( AspectXAdd ( RectangleRC.left) ) );
					oXmlWriter.WriteAttribute ( _TEXT("Top"),	FromInteger ( AspectYAdd ( RectangleRC.top ) ) );
					oXmlWriter.WriteAttribute ( _TEXT("Right"),	FromInteger ( AspectXAdd ( RectangleRC.right ) ) );
					oXmlWriter.WriteAttribute ( _TEXT("Bottom"),FromInteger ( AspectYAdd ( RectangleRC.bottom ) ) );

					oXmlWriter.WriteNodeEnd ( GetNodeTypeName (pTrackObject), TRUE );

					XmlStr = oXmlWriter.GetXmlString ();

					return true;
				}

				if ( pTrackObject->IsTrackRectangleFixedSize() )
				{
					Structures::RECTD RectangleRC = pTrackObject->GetDoubleBoundingRect ( 0, 3 );
					RectangleRC.Normalize();

					oXmlWriter.WriteAttribute ( _TEXT("Left"),		FromInteger ( AspectXAdd ( RectangleRC.left ) ) );
					oXmlWriter.WriteAttribute ( _TEXT("Top"),		FromInteger ( AspectYAdd ( RectangleRC.top ) ) );
					oXmlWriter.WriteAttribute ( _TEXT("Right"),		FromInteger ( AspectXAdd ( RectangleRC.right ) ) );
					oXmlWriter.WriteAttribute ( _TEXT("Bottom"),	FromInteger ( AspectYAdd ( RectangleRC.bottom ) ) );

					oXmlWriter.WriteNodeEnd ( GetNodeTypeName (pTrackObject), TRUE );

					XmlStr = oXmlWriter.GetXmlString ();

					return true;
				}

				if ( pTrackObject->IsTrackRectangleFixedAspect() )
				{
					Structures::RECTD RectangleRC = pTrackObject->GetDoubleBoundingRect ( 0, 3 );
					RectangleRC.Normalize();

					oXmlWriter.WriteAttribute ( _TEXT("Left"),	FromInteger ( AspectXAdd ( RectangleRC.left) ) );
					oXmlWriter.WriteAttribute ( _TEXT("Top"),	FromInteger ( AspectYAdd ( RectangleRC.top ) ) );
					oXmlWriter.WriteAttribute ( _TEXT("Right"),	FromInteger ( AspectXAdd ( RectangleRC.right ) ) );
					oXmlWriter.WriteAttribute ( _TEXT("Bottom"),FromInteger ( AspectYAdd ( RectangleRC.bottom ) ) );

					oXmlWriter.WriteNodeEnd ( GetNodeTypeName (pTrackObject), TRUE );

					XmlStr = oXmlWriter.GetXmlString ();

					return true;
				}

				if ( pTrackObject->IsTrackPolyline() )
				{
					oXmlWriter.WriteAttribute ( _TEXT("Points"), CreatePointsString ( pTrackObject ) );

					oXmlWriter.WriteNodeEnd ( GetNodeTypeName (pTrackObject), TRUE );

					XmlStr = oXmlWriter.GetXmlString ();

					return true;
				}

				if ( pTrackObject->IsTrackPolygon() )
				{
					oXmlWriter.WriteAttribute ( _TEXT("Points"), CreatePointsString ( pTrackObject ) );

					oXmlWriter.WriteNodeEnd ( GetNodeTypeName (pTrackObject), TRUE );

					XmlStr = oXmlWriter.GetXmlString ();

					return true;
				}

				if ( pTrackObject->IsTrackPie() )
				{
					Structures::RECTD PieRC = pTrackObject->GetDoubleBoundingRect ( 0, 3 );
					PieRC.Normalize();

					// Get center and angles
					double nCenterX		= PieRC.GetCenter().x;
					double nCenterY		= PieRC.GetCenter().y;

					double nAngle1		= grad ( Geometry::GetAngle ( nCenterX, nCenterY, pTrackObject->GetAt ( 5 ).dX, pTrackObject->GetAt(5).dY ) );
					double nAngle2		= grad ( Geometry::GetAngle ( nCenterX, nCenterY, pTrackObject->GetAt ( 4 ).dX, pTrackObject->GetAt(4).dY ) );

					nAngle1				= ( nAngle1 >= 0 ) ? nAngle1 : 360 + nAngle1;
					nAngle2				= ( nAngle2 >= 0 ) ? nAngle2 : 360 + nAngle2;

					double SweepAngle	= nAngle2 - nAngle1;
					SweepAngle = ( SweepAngle >= 0 ) ? SweepAngle : 360 + SweepAngle;

					oXmlWriter.WriteAttribute ( _TEXT("Left"),		FromInteger ( AspectXAdd ( PieRC.left) ) );
					oXmlWriter.WriteAttribute ( _TEXT("Top"),		FromInteger ( AspectYAdd ( PieRC.top ) ) );
					oXmlWriter.WriteAttribute ( _TEXT("Right"),		FromInteger ( AspectXAdd ( PieRC.right ) ) );
					oXmlWriter.WriteAttribute ( _TEXT("Bottom"),	FromInteger ( AspectYAdd ( PieRC.bottom ) ) );

					if ( CTrackObjectPie* pTrackPie = static_cast <CTrackObjectPie*> ( pTrackObject ) )
					{
						oXmlWriter.WriteAttribute ( _TEXT("StartAngle"),	FromInteger ( grad ( pTrackPie->GetStartAngle() ) ) );
						oXmlWriter.WriteAttribute ( _TEXT("SweepAngle"),	FromInteger ( grad ( pTrackPie->GetSweepAngle() ) ) );
					}
					else
					{

						oXmlWriter.WriteAttribute ( _TEXT("StartAngle"),	FromInteger ( nAngle1 ) );
						oXmlWriter.WriteAttribute ( _TEXT("SweepAngle"),	FromInteger ( SweepAngle ) );
					}

					oXmlWriter.WriteNodeEnd ( GetNodeTypeName (pTrackObject), TRUE );

					XmlStr = oXmlWriter.GetXmlString ();
					
				//	ATLTRACE ( "GET : %s\n", XmlStr );

					return true;
				}

				if ( pTrackObject->IsTrackPieSimple() )
				{
					// Get center, radiuses and angles
					double dCenterX	= pTrackObject->GetAt(2).dX;
					double dCenterY	= pTrackObject->GetAt(2).dY;

					double dAngle1	= grad ( Geometry::GetAngle ( dCenterX, dCenterY, pTrackObject->GetAt(1).dX, pTrackObject->GetAt(1).dY ) );
					double dAngle2	= grad ( Geometry::GetAngle ( dCenterX, dCenterY, pTrackObject->GetAt(0).dX, pTrackObject->GetAt(0).dY ) );

					dAngle1			= (dAngle1 >= 0)? dAngle1 : 360 + dAngle1;
					dAngle2			= (dAngle2 >= 0)? dAngle2 : 360 + dAngle2;

					double SweepAngle = dAngle2 - dAngle1;
					SweepAngle		= (SweepAngle >= 0)? SweepAngle : 360 + SweepAngle;

					// apply aspect correction
					dCenterX		= AspectXAdd ( dCenterX );
					dCenterY		= AspectYAdd ( dCenterY );

					// get radius
					double dRadius	= fabs ( dCenterX - AspectXAdd ( pTrackObject->GetAt(3).dX ) );

					oXmlWriter.WriteAttribute ( _TEXT("CenterX"),		FromInteger ( dCenterX ) );
					oXmlWriter.WriteAttribute ( _TEXT("CenterY"),		FromInteger ( dCenterY ) ); 
					oXmlWriter.WriteAttribute ( _TEXT("Radius"),		FromInteger ( dRadius ) );
					oXmlWriter.WriteAttribute ( _TEXT("StartAngle"),	FromInteger ( dAngle1 ) );
					oXmlWriter.WriteAttribute ( _TEXT("SweepAngle"),	FromInteger ( SweepAngle ) );

					oXmlWriter.WriteNodeEnd ( GetNodeTypeName (pTrackObject), TRUE );

					XmlStr = oXmlWriter.GetXmlString ();

					return true;
				}

				if ( pTrackObject->IsTrackSector() )
				{
					Structures::RECTD SectorRC	=	pTrackObject->GetDoubleBoundingRect ( 0, 3 );
					SectorRC.Normalize();

					// Get center, radiuses and angles
					double dCenterX		=	SectorRC.GetCenter().x;
					double dCenterY		=	SectorRC.GetCenter().y;

					double dAngle1		=	grad ( Geometry::GetAngle ( dCenterX, dCenterY, pTrackObject->GetAt(7).dX, pTrackObject->GetAt(7).dY ) );
					double dAngle2		=	grad ( Geometry::GetAngle ( dCenterX, dCenterY, pTrackObject->GetAt(6).dX, pTrackObject->GetAt(6).dY ) );

					dAngle1				=	( dAngle1 >= 0.0 ) ? dAngle1 : 360.0 + dAngle1;
					dAngle2				=	( dAngle2 >= 0.0 ) ? dAngle2 : 360.0 + dAngle2;

					double SweepAngle	=	dAngle2 - dAngle1;
					SweepAngle			=	( SweepAngle >= 0.0 ) ? SweepAngle : 360.0 + SweepAngle;

					double dRadius1		=	Geometry::GetLength ( dCenterX, dCenterY, pTrackObject->GetAt(6).dX, pTrackObject->GetAt(6).dY );
					double dRadius2		=	Geometry::GetLength ( dCenterX, dCenterY, pTrackObject->GetAt(4).dX, pTrackObject->GetAt(4).dY );
					double dRatio		=	( dRadius1 < 0.001 ) ? 1.0 : dRadius2 / dRadius1;

					oXmlWriter.WriteAttribute ( _TEXT("Left"),		FromInteger ( AspectXAdd ( SectorRC.left) ) );
					oXmlWriter.WriteAttribute ( _TEXT("Top"),		FromInteger ( AspectYAdd ( SectorRC.top ) ) );
					oXmlWriter.WriteAttribute ( _TEXT("Right"),		FromInteger ( AspectXAdd ( SectorRC.right ) ) );
					oXmlWriter.WriteAttribute ( _TEXT("Bottom"),	FromInteger ( AspectYAdd ( SectorRC.bottom ) ) );

					if ( CTrackObjectSector* pTrack = static_cast <CTrackObjectSector*> ( pTrackObject ) )
					{
						oXmlWriter.WriteAttribute ( _TEXT("StartAngle"),	FromInteger ( grad ( pTrack->GetStartAngle () ) ) );
						oXmlWriter.WriteAttribute ( _TEXT("SweepAngle"),	FromInteger ( grad ( pTrack->GetSweepAngle () ) ) );
						oXmlWriter.WriteAttribute ( _TEXT("Ratio"),			FromInteger ( pTrack->GetRatio () ) );
					}
					else
					{
						oXmlWriter.WriteAttribute ( _TEXT("StartAngle"),FromInteger ( dAngle1 ) );
						oXmlWriter.WriteAttribute ( _TEXT("SweepAngle"),FromInteger ( SweepAngle ) );
						oXmlWriter.WriteAttribute ( _TEXT("Ratio"),		FromInteger ( dRatio ) );
					}

					oXmlWriter.WriteNodeEnd ( GetNodeTypeName (pTrackObject), TRUE );

					XmlStr = oXmlWriter.GetXmlString ();

					return true;
				}

				if ( pTrackObject->IsTrackSectorSimple() )
				{
					// Get center
					double dCenterX = pTrackObject->GetAt(4).dX;
					double dCenterY = pTrackObject->GetAt(4).dY;

					// Get angles
					double dAngle1 = grad(Geometry::GetAngle(dCenterX, dCenterY, pTrackObject->GetAt(1).dX, pTrackObject->GetAt(1).dY));
					double dAngle2 = grad(Geometry::GetAngle(dCenterX, dCenterY, pTrackObject->GetAt(0).dX, pTrackObject->GetAt(0).dY));
					dAngle1 = (dAngle1 >= 0)? dAngle1 : 360 + dAngle1;
					dAngle2 = (dAngle2 >= 0)? dAngle2 : 360 + dAngle2;
					double SweepAngle = dAngle2 - dAngle1;
					SweepAngle = (SweepAngle >= 0)? SweepAngle : 360 + SweepAngle;

					// Set aspect for center
					dCenterX = AspectXAdd(dCenterX);
					dCenterY = AspectYAdd(dCenterY);

					// Get radiuses
					double dRadius1 = fabs(dCenterX - AspectXAdd(pTrackObject->GetAt(5).dX));
					double dRadius2 = fabs(dCenterX - AspectXAdd(pTrackObject->GetAt(9).dX));

					oXmlWriter.WriteAttribute ( _TEXT("CenterX"),	FromInteger ( dCenterX ) );
					oXmlWriter.WriteAttribute ( _TEXT("CenterY"),	FromInteger ( dCenterY ) );
					oXmlWriter.WriteAttribute ( _TEXT("Radius1"),	FromInteger ( dRadius1 ) );
					oXmlWriter.WriteAttribute ( _TEXT("Radius2"),	FromInteger ( dRadius2 ) );

					oXmlWriter.WriteAttribute ( _TEXT("StartAngle"),FromInteger ( dAngle1 ) );
					oXmlWriter.WriteAttribute ( _TEXT("SweepAngle"),FromInteger ( SweepAngle ) );

					oXmlWriter.WriteNodeEnd ( GetNodeTypeName (pTrackObject), TRUE );

					XmlStr = oXmlWriter.GetXmlString ();

					return true;
				}

				if ( pTrackObject->IsTrackCircle() )
				{
					Structures::RECTD CircleRC = pTrackObject->GetDoubleBoundingRect ( 0, 3 );
					CircleRC.Normalize();

					// Get center and angles
					double nCenterX	= CircleRC.GetCenter().x;
					double nCenterY	= CircleRC.GetCenter().y;

					// Apply aspect correction
					nCenterX		= AspectXAdd ( nCenterX );
					nCenterY		= AspectYAdd ( nCenterY );

					// Get radius
					double nRadius = fabs ( nCenterX - AspectXAdd ( pTrackObject->GetAt(0).dX ) );

					oXmlWriter.WriteAttribute ( _TEXT("CenterX"),	FromInteger ( nCenterX ) );
					oXmlWriter.WriteAttribute ( _TEXT("CenterY"),	FromInteger ( nCenterY ) );
					oXmlWriter.WriteAttribute ( _TEXT("Radius"),	FromInteger ( nRadius ) );

					oXmlWriter.WriteNodeEnd ( GetNodeTypeName (pTrackObject), TRUE );
					
					XmlStr = oXmlWriter.GetXmlString ();

					return true;
				}

				if ( pTrackObject->IsTrackEllipseInRectangle() )
				{
					Structures::RECTD EllipseRC = pTrackObject->GetDoubleBoundingRect ( 0, 3 );
					EllipseRC.Normalize();

					oXmlWriter.WriteAttribute ( _TEXT("Left"),	FromInteger ( AspectXAdd ( EllipseRC.left) ) );
					oXmlWriter.WriteAttribute ( _TEXT("Top"),	FromInteger ( AspectYAdd ( EllipseRC.top ) ) );
					oXmlWriter.WriteAttribute ( _TEXT("Right"),	FromInteger ( AspectXAdd ( EllipseRC.right ) ) );
					oXmlWriter.WriteAttribute ( _TEXT("Bottom"),FromInteger ( AspectYAdd ( EllipseRC.bottom ) ) );

					oXmlWriter.WriteNodeEnd ( GetNodeTypeName (pTrackObject), TRUE );

					XmlStr = oXmlWriter.GetXmlString ();

					return true;
				}

				if ( pTrackObject->IsTrackRectangleRotatable() )
				{
					double dAngle = Geometry::GetAngle( pTrackObject->GetAt(9).dX, pTrackObject->GetAt(9).dY,
						pTrackObject->GetAt(8).dX, pTrackObject->GetAt(8).dY ) + M_PI_2;

					Structures::RECTD RectangleRC = pTrackObject->GetUnrotatableRect ( dAngle );	

					dAngle = grad ( ( ( dAngle <= 0 )? dAngle + 2.0 * M_PI: dAngle ) );

					oXmlWriter.WriteAttribute ( _TEXT("Left"),		FromInteger ( AspectXAdd ( RectangleRC.left) ) );
					oXmlWriter.WriteAttribute ( _TEXT("Top"),		FromInteger ( AspectYAdd ( RectangleRC.top ) ) );
					oXmlWriter.WriteAttribute ( _TEXT("Right"),		FromInteger ( AspectXAdd ( RectangleRC.right ) ) );
					oXmlWriter.WriteAttribute ( _TEXT("Bottom"),	FromInteger ( AspectYAdd ( RectangleRC.bottom ) ) );

					oXmlWriter.WriteAttribute ( _TEXT("Angle"),		FromInteger ( dAngle ) );

					CTrackObjectRectangleRotatable* pTrack = (CTrackObjectRectangleRotatable*)pTrackObject;
					if ( NULL != pTrack )
						oXmlWriter.WriteAttribute ( _TEXT("StickAngle"),FromInteger ( pTrack->GetStickAngle () ) );

					oXmlWriter.WriteNodeEnd ( GetNodeTypeName (pTrackObject), TRUE );

					XmlStr = oXmlWriter.GetXmlString ();

					return true;
				}

				if ( pTrackObject->IsTrackRectangleRotatableFixedAspect() )
				{
					double dAngle = Geometry::GetAngle ( pTrackObject->GetAt(5).dX, pTrackObject->GetAt(5).dY, 
						pTrackObject->GetAt(4).dX, pTrackObject->GetAt(4).dY ) + M_PI_2;

					Structures::RECTD RectangleRC = pTrackObject->GetUnrotatableRect ( dAngle, 0, 2, 5 );	

					dAngle = grad ( ( ( dAngle <= 0 ) ? dAngle + 2.0 * M_PI : dAngle ) );

					oXmlWriter.WriteAttribute ( _TEXT("Left"),		FromInteger ( AspectXAdd ( RectangleRC.left) ) );
					oXmlWriter.WriteAttribute ( _TEXT("Top"),		FromInteger ( AspectYAdd ( RectangleRC.top ) ) );
					oXmlWriter.WriteAttribute ( _TEXT("Right"),		FromInteger ( AspectXAdd ( RectangleRC.right ) ) );
					oXmlWriter.WriteAttribute ( _TEXT("Bottom"),	FromInteger ( AspectYAdd ( RectangleRC.bottom ) ) );

					oXmlWriter.WriteAttribute ( _TEXT("Angle"),		FromInteger ( dAngle ) );

					CTrackObjectRectangleRotatableFixedAspect* pTrack = (CTrackObjectRectangleRotatableFixedAspect*)pTrackObject;

					if ( NULL != pTrack )
					{
						oXmlWriter.WriteAttribute ( _TEXT("StickAngle"),FromInteger ( pTrack->GetStickAngle () ) );
						oXmlWriter.WriteAttribute ( _TEXT("NoResize"),	FromInt ( pTrack->GetNoResize () ) );
						oXmlWriter.WriteAttribute ( _TEXT("SizesMode"),	FromInt ( pTrack->GetSizesMode () ) );
					}

					oXmlWriter.WriteNodeEnd ( GetNodeTypeName ( pTrackObject ), TRUE );

					XmlStr = oXmlWriter.GetXmlString ();

					return true;
				}

				if ( pTrackObject->IsTrackTextRotatable() )
				{
					Tracking::CTrackObjectTextRotatable* pTrackText = dynamic_cast <Tracking::CTrackObjectTextRotatable*> ( pTrackObject );
					if ( NULL != pTrackText )
					{
						double dAngle = Geometry::GetAngle ( 
							pTrackObject->GetAt(9).dX,
							pTrackObject->GetAt(9).dY,
							pTrackObject->GetAt(8).dX, pTrackObject->GetAt(8).dY ) + M_PI_2;

						Structures::RECTD TextRC = pTrackText->GetUnrotatableRect ( dAngle );	
						TextRC.Normalize();
						
						dAngle = grad( ((dAngle <= 0 )? dAngle + 2.0 * M_PI: dAngle));

						oXmlWriter.WriteAttribute ( _TEXT("Left"),		FromInteger ( AspectXAdd ( TextRC.left) ) );
						oXmlWriter.WriteAttribute ( _TEXT("Top"),		FromInteger ( AspectYAdd ( TextRC.top ) ) );
						oXmlWriter.WriteAttribute ( _TEXT("Right"),		FromInteger ( AspectXAdd ( TextRC.right ) ) );
						oXmlWriter.WriteAttribute ( _TEXT("Bottom"),	FromInteger ( AspectYAdd ( TextRC.bottom ) ) );
						oXmlWriter.WriteAttribute ( _TEXT("Angle"),		FromInteger ( dAngle ) );
						oXmlWriter.WriteAttribute ( _TEXT("StickAngle"),FromInteger ( pTrackText->GetStickAngle () ) );

						oXmlWriter.WriteAttribute ( _TEXT("Text"),		CString ( W2BSTR ( pTrackText->GetString () ) ) );
						oXmlWriter.WriteAttribute ( _TEXT("FontName"),	CString ( W2BSTR ( pTrackText->GetFontName () ) ) );
						oXmlWriter.WriteAttribute ( _TEXT("FontSize"),	FromInteger ( pTrackText->GetFontSize () ) );
						oXmlWriter.WriteAttribute ( _TEXT("FontType"),	FromInteger ( pTrackText->GetFontType () ) );
						oXmlWriter.WriteAttribute ( _TEXT("FontColor"),	FromInteger ( pTrackText->GetFontColor () ) );

						oXmlWriter.WriteNodeEnd ( GetNodeTypeName (pTrackObject), TRUE );

						XmlStr = oXmlWriter.GetXmlString ();

						return true;
					}

					return false;
				}

				if ( pTrackObject->IsTrackText() )
				{
					Tracking::CTrackObjectText* pTrackText = dynamic_cast <Tracking::CTrackObjectText*> ( pTrackObject );
					if ( NULL != pTrackText )
					{
						Structures::RECTD TextRC = pTrackObject->GetDoubleBoundingRect ( 0, 3 );
						TextRC.Normalize();

						oXmlWriter.WriteAttribute ( _TEXT("Left"),		FromInteger ( AspectXAdd ( TextRC.left) ) );
						oXmlWriter.WriteAttribute ( _TEXT("Top"),		FromInteger ( AspectYAdd ( TextRC.top ) ) );
						oXmlWriter.WriteAttribute ( _TEXT("Right"),		FromInteger ( AspectXAdd ( TextRC.right ) ) );
						oXmlWriter.WriteAttribute ( _TEXT("Bottom"),	FromInteger ( AspectYAdd ( TextRC.bottom ) ) );


						//	#pragma comment ("TODO : XML UNICODE TEXT")

						oXmlWriter.WriteAttribute ( _TEXT("Text"),		CString ( W2BSTR ( pTrackText->GetString () ) ) );
						oXmlWriter.WriteAttribute ( _TEXT("FontName"),	CString ( W2BSTR ( pTrackText->GetFontName () ) ) );
						oXmlWriter.WriteAttribute ( _TEXT("FontSize"),	FromInteger ( pTrackText->GetFontSize () ) );
						oXmlWriter.WriteAttribute ( _TEXT("FontType"),	FromInteger ( pTrackText->GetFontType () ) );
						oXmlWriter.WriteAttribute ( _TEXT("FontColor"),	FromInteger ( pTrackText->GetFontColor () ) );

						oXmlWriter.WriteNodeEnd ( GetNodeTypeName (pTrackObject), TRUE );

						XmlStr = oXmlWriter.GetXmlString ();

						return true;
					}

					return false;
				}

				if ( pTrackObject->IsTrackTrajectoryPolyline() )
				{
					oXmlWriter.WriteAttribute ( _TEXT("Points"), CreatePointsString ( pTrackObject ) );

					oXmlWriter.WriteNodeEnd ( GetNodeTypeName (pTrackObject), TRUE );

					XmlStr = oXmlWriter.GetXmlString ();

					return true;
				}

				if ( pTrackObject->IsTrackTypeCubicBezierCurve () )
				{
					oXmlWriter.WriteAttribute ( _TEXT("Points"), CreatePointsString ( pTrackObject ) );

					CTrackCubicBezierCurve* pTrackQ = static_cast<CTrackCubicBezierCurve*> (pTrackObject);
					if ( NULL != pTrackQ )
					{
						oXmlWriter.WriteAttribute ( _TEXT("Innerfactor"), pTrackQ->GetInnerFactor () );

						Tracking::CTrackObject* pInner	= pTrackQ->GetTrackInner ();
						double PositionTrackInner		= pTrackQ->GetTrackInnerInd ();
						if ( NULL != pInner )
						{
							CString TrackInXml;
							if ( WriteXml ( pInner, TrackInXml ) )
							{
								oXmlWriter.WriteString ( _T(">") );
								oXmlWriter.WriteString ( TrackInXml );
								oXmlWriter.WriteString ( _T("</track>") );

								XmlStr = oXmlWriter.GetXmlString ();

								return true;
							}
						}
					}

					oXmlWriter.WriteNodeEnd ( GetNodeTypeName (pTrackObject), TRUE );

					XmlStr = oXmlWriter.GetXmlString ();

					return true;
				}

				if ( pTrackObject->IsTracksTypeTrackEffectKenBurns() )
				{
					CTrackEffectKenBurns* pEKBTrack = static_cast<CTrackEffectKenBurns*> ( pTrackObject );
					if ( pEKBTrack )
					{
						Structures::RECTD RC01 = pEKBTrack->m_Track[0].GetDoubleBoundingRect ( 0, 3 );
						RC01.Normalize();
						
						Structures::RECTD RC02 = pEKBTrack->m_Track[1].GetDoubleBoundingRect ( 0, 3 );
						RC02.Normalize();

						oXmlWriter.WriteAttribute ( _TEXT("Left"),		FromInteger ( AspectXAdd ( RC01.left) ) );
						oXmlWriter.WriteAttribute ( _TEXT("Top"),		FromInteger ( AspectYAdd ( RC01.top ) ) );
						oXmlWriter.WriteAttribute ( _TEXT("Right"),		FromInteger ( AspectXAdd ( RC01.right ) ) );
						oXmlWriter.WriteAttribute ( _TEXT("Bottom"),	FromInteger ( AspectYAdd ( RC01.bottom ) ) );
						
						oXmlWriter.WriteAttribute ( _TEXT("Left2"),		FromInteger ( AspectXAdd ( RC02.left) ) );
						oXmlWriter.WriteAttribute ( _TEXT("Top2"),		FromInteger ( AspectYAdd ( RC02.top ) ) );
						oXmlWriter.WriteAttribute ( _TEXT("Right2"),	FromInteger ( AspectXAdd ( RC02.right ) ) );
						oXmlWriter.WriteAttribute ( _TEXT("Bottom2"),	FromInteger ( AspectYAdd ( RC02.bottom ) ) );

						oXmlWriter.WriteNodeEnd ( GetNodeTypeName ( pTrackObject ), TRUE );

						XmlStr = oXmlWriter.GetXmlString ();

						return true;
					}
				}


				return false;
			}


		private:
			inline CString CreatePointsString ( Tracking::CTrackObject* pTrackObject )
			{
				CString PointsString;
				int Count = pTrackObject->GetCount();

				for ( int nIndex = 0; nIndex < Count; ++nIndex )
				{
					CString Point; 
					Point.Format ( _TEXT("%f %f"), AspectXAdd ( pTrackObject->GetAt(nIndex).dX ), AspectYAdd ( pTrackObject->GetAt(nIndex).dY ) );

					PointsString += Point;

					if ( nIndex + 1 != Count )
						PointsString += _TEXT(" ");
				}

				return PointsString;
			}

			inline static CString FromInteger ( double Value )
			{
				CString String;

				String.Format ( _TEXT("%f"), Value );

				return String;
			}
			
			inline static CString FromInt ( int Value )
			{
				CString String;

				String.Format ( _TEXT("%d"), Value );

				return String;
			}

			inline CString GetNodeTypeName ( Tracking::CTrackObject* pTrackObject )
			{
				if ( pTrackObject->IsTrackNone() )
					return false;

				CString TrackType = _T("");

				if ( pTrackObject->IsTrackPoint() )
					TrackType = Constants::ConstTrackPoint;

				if ( pTrackObject->IsTrackLine() )
					TrackType = Constants::ConstTrackLine;

				if ( pTrackObject->IsTrackEllipse() )
					TrackType = Constants::ConstTrackEllipse;

				if ( pTrackObject->IsTrackRectangle() )
					TrackType = Constants::ConstTrackRectangle;

				if ( pTrackObject->IsTrackRectangleFixedSize() )
					TrackType = Constants::ConstTrackRectangleFixedSize;

				if ( pTrackObject->IsTrackRectangleFixedAspect() ) 
					TrackType = Constants::ConstTrackRectangleFixedAspect;

				if ( pTrackObject->IsTrackPolyline() ) 
					TrackType = Constants::ConstTrackPolyline;

				if ( pTrackObject->IsTrackPolygon() )
					TrackType = Constants::ConstTrackPolygon;

				if ( pTrackObject->IsTrackPie() ) 
					TrackType = Constants::ConstTrackPie;

				if ( pTrackObject->IsTrackPieSimple() )
					TrackType = Constants::ConstTrackSimplePie;

				if ( pTrackObject->IsTrackSector() ) 
					TrackType = Constants::ConstTrackSector;

				if ( pTrackObject->IsTrackSectorSimple() ) 
					TrackType = Constants::ConstTrackSimpleSector;

				if ( pTrackObject->IsTrackCircle () ) 
					TrackType = Constants::ConstTrackCircle;

				if ( pTrackObject->IsTrackEllipseInRectangle() ) 
					TrackType = Constants::ConstTrackEllipseInRectangle;

				if ( pTrackObject->IsTrackRectangleRotatable() ) 
					TrackType = Constants::ConstTrackRectangleRotatable;

				if ( pTrackObject->IsTrackRectangleRotatableFixedAspect() ) 
					TrackType = Constants::ConstTrackRectangleRotatableFixedAspect;

				if ( pTrackObject->IsTrackTextRotatable() ) 
					TrackType = Constants::ConstTrackTextRotatable;

				if ( pTrackObject->IsTrackText() ) 
					TrackType = Constants::ConstTrackText;

				if ( pTrackObject->IsTrackTrajectoryPolyline() )
					TrackType = Constants::ConstTrackTrajectoryPolyline;

				if ( pTrackObject->IsTrackTypeCubicBezierCurve ()  ) 
					TrackType = Constants::ConstTrackCubicBezierCurve;
				
				if ( pTrackObject->IsTracksTypeTrackEffectKenBurns ()  ) 
					TrackType = Constants::ConstTrackEffectKenBurns;

				return TrackType;
			}



			inline double AspectXAdd ( double x )
			{
				if ( m_nAspect < 0.001 )
					return x;

				return m_nAspectWidth*x/m_nWidth;
			}
			inline double AspectYAdd ( double y )
			{
				if ( m_nAspect < 0.001 )
					return y;

				return m_nAspectHeight*y/m_nHeight;
			}
			inline double AspectXRemove ( double x )
			{
				if ( m_nAspect < 0.001 )
					return x;

				return m_nWidth * x / m_nAspectWidth;
			}
			inline double AspectYRemove ( double y )
			{
				if ( m_nAspect < 0.001 )
					return y;

				return m_nHeight * y / m_nAspectHeight;
			}


		private:

			double	m_nAspect;
			int		m_nAspectWidth;
			int		m_nAspectHeight;
			int		m_nWidth;
			int		m_nHeight;
		};


		class CTrackXmlReader
		{
		public:
			CTrackXmlReader( double nAspect, int nAspectWidth, int nAspectHeight, int nWidth, int nHeight )
			{
				m_nAspect		= nAspect;
				m_nAspectWidth	= nAspectWidth;
				m_nAspectHeight	= nAspectHeight;
				m_nWidth		= nWidth;
				m_nHeight		= nHeight;
			}

			inline bool ReadXml ( Tracking::CTrackObject** ppTrackObject, const CString& XmlStr )
			{
				CXmlNode oXmlNode;

				if ( oXmlNode.FromXmlString ( XmlStr ) )
					return ReadXml ( ppTrackObject, oXmlNode );

				return false;
			}

			inline bool ReadXml ( Tracking::CTrackObject** ppTrackObject, CXmlNode& oXMLNode )
			{
				// считываеБEтрек БE именБEыMда
				CString TrackingName = oXMLNode.GetAttribute ( _T("name") );

				if ( Constants::ConstTrackPoint == TrackingName )
				{
					double X	= _tstof ( oXMLNode.GetAttribute ( _T("x") ) );
					double Y	= _tstof ( oXMLNode.GetAttribute ( _T("y") ) );

					if ( false == CreateTrackPoint ( ppTrackObject, X, Y ) )
						return false;

					return true;
				}

				if ( Constants::ConstTrackLine == TrackingName )
				{
					double X1	= _tstof ( oXMLNode.GetAttribute ( _T("x1") ) );
					double Y1	= _tstof ( oXMLNode.GetAttribute ( _T("y1") ) );
					double X2	= _tstof ( oXMLNode.GetAttribute ( _T("x2") ) );
					double Y2	= _tstof ( oXMLNode.GetAttribute ( _T("y2") ) );

					if ( false == CreateTrackLine ( ppTrackObject, X1, Y1, X2, Y2 ) )
						return false;

					return true;
				}

				if ( Constants::ConstTrackEllipse == TrackingName )
				{
					double Left		= _tstof ( oXMLNode.GetAttribute ( _T("left") ) );
					double Top		= _tstof ( oXMLNode.GetAttribute ( _T("top") ) );
					double Right	= _tstof ( oXMLNode.GetAttribute ( _T("right") ) );
					double Bottom	= _tstof ( oXMLNode.GetAttribute ( _T("bottom") ) );

					if ( false == CreateTrackEllipse ( ppTrackObject, Left, Top, Right, Bottom ) )
						return false;

					return true;
				}

				if ( Constants::ConstTrackRectangle == TrackingName )
				{
					double Left		= _tstof ( oXMLNode.GetAttribute ( _T("left") ) );
					double Top		= _tstof ( oXMLNode.GetAttribute ( _T("top") ) );
					double Right	= _tstof ( oXMLNode.GetAttribute ( _T("right") ) );
					double Bottom	= _tstof ( oXMLNode.GetAttribute ( _T("bottom") ) );

					if ( false == CreateTrackRectangle ( ppTrackObject, Left, Top, Right, Bottom ) )
						return false;

					return true;
				}

				if ( Constants::ConstTrackRectangleFixedSize == TrackingName )
				{
					double Left		=	_tstof ( oXMLNode.GetAttribute ( _T("left") ) );
					double Top		=	_tstof ( oXMLNode.GetAttribute ( _T("top") ) );
					double Right	=	_tstof ( oXMLNode.GetAttribute ( _T("right") ) );
					double Bottom	=	_tstof ( oXMLNode.GetAttribute ( _T("bottom") ) );

					if ( false == CreateTrackRectangleFixedSize ( ppTrackObject, Left, Top, Right, Bottom ) )
						return false;

					return true;
				}

				if ( Constants::ConstTrackRectangleFixedAspect == TrackingName )
				{
					double Left		= _tstof ( oXMLNode.GetAttribute ( _T("left") ) );
					double Top		= _tstof ( oXMLNode.GetAttribute ( _T("top") ) );
					double Right	= _tstof ( oXMLNode.GetAttribute ( _T("right") ) );
					double Bottom	= _tstof ( oXMLNode.GetAttribute ( _T("bottom") ) );

					if ( false == CreateTrackRectangleFixedAspect ( ppTrackObject, Left, Top, Right, Bottom ) )
						return false;

					return true;
				}

				if ( Constants::ConstTrackPolyline == TrackingName )
				{
					CSimpleArray<Structures::POINTD> arrPoints;
					if ( false == CreatePointsData ( arrPoints, oXMLNode.GetAttribute ( _T("points" ) ) ) )
						return false;

					if ( false == CreateTrackPolyline ( ppTrackObject, (Structures::POINTD*)arrPoints.GetData(), arrPoints.GetSize() ) )
						return false;

					return true;
				}

				if ( Constants::ConstTrackPolygon == TrackingName )
				{
					CSimpleArray<Structures::POINTD> arrPoints;
					if ( false == CreatePointsData ( arrPoints, oXMLNode.GetAttribute ( _T("points" ) ) ) )
						return false;

					if ( false == CreateTrackPolygon ( ppTrackObject, (Structures::POINTD*)arrPoints.GetData(), arrPoints.GetSize() ) )
						return false;

					return true;
				}

				if ( Constants::ConstTrackPie == TrackingName )
				{
					double Left			= _tstof ( oXMLNode.GetAttribute ( _T("left") ) );
					double Top			= _tstof ( oXMLNode.GetAttribute ( _T("top") ) );
					double Right		= _tstof ( oXMLNode.GetAttribute ( _T("right") ) );
					double Bottom		= _tstof ( oXMLNode.GetAttribute ( _T("bottom") ) );

					double StartAngle	= _tstof ( oXMLNode.GetAttribute ( _T("startangle") ) );
					double SweepAngle	= _tstof ( oXMLNode.GetAttribute ( _T("sweepangle") ) );

					if ( false == CreateTrackPie ( ppTrackObject, Left, Top, Right, Bottom, StartAngle, SweepAngle ) )
						return false;

					return true;
				}

				if ( Constants::ConstTrackSimplePie == TrackingName )
				{
					double CenterX		= _tstof ( oXMLNode.GetAttribute ( _T("centerx") ) );
					double CenterY		= _tstof ( oXMLNode.GetAttribute ( _T("centery") ) );
					double Radius		= _tstof ( oXMLNode.GetAttribute ( _T("radius") ) );
					double StartAngle	= _tstof ( oXMLNode.GetAttribute ( _T("startangle") ) );
					double SweepAngle	= _tstof ( oXMLNode.GetAttribute ( _T("sweepangle") ) );

					if ( false == CreateTrackSimplePie ( ppTrackObject, CenterX, CenterY, Radius, StartAngle, SweepAngle ) )
						return false;

					return true;
				}

				if ( Constants::ConstTrackSector == TrackingName )
				{
					double Left			= _tstof ( oXMLNode.GetAttribute ( _T("left") ) );
					double Top			= _tstof ( oXMLNode.GetAttribute ( _T("top") ) );
					double Right		= _tstof ( oXMLNode.GetAttribute ( _T("right") ) );
					double Bottom		= _tstof ( oXMLNode.GetAttribute ( _T("bottom") ) );
					double StartAngle	= _tstof ( oXMLNode.GetAttribute ( _T("startangle") ) );
					double SweepAngle	= _tstof ( oXMLNode.GetAttribute ( _T("sweepangle") ) );
					double Ratio		= _tstof ( oXMLNode.GetAttribute ( _T("ratio") ) );

					if ( false == CreateTrackSector ( ppTrackObject, Left, Top, Right, Bottom, StartAngle, SweepAngle, Ratio ) )
						return false;

					return true;
				}

				if ( Constants::ConstTrackSimpleSector == TrackingName )
				{
					double CenterX		= _tstof ( oXMLNode.GetAttribute ( _T("centerx") ) );
					double CenterY		= _tstof ( oXMLNode.GetAttribute ( _T("centery") ) );
					double Radius1		= _tstof ( oXMLNode.GetAttribute ( _T("radius1") ) );
					double Radius2		= _tstof ( oXMLNode.GetAttribute ( _T("radius2") ) );
					double StartAngle	= _tstof ( oXMLNode.GetAttribute ( _T("startangle") ) );
					double SweepAngle	= _tstof ( oXMLNode.GetAttribute ( _T("sweepangle") ) );

					if ( false == CreateTrackSimpleSector ( ppTrackObject, CenterX, CenterY, Radius1, Radius2, StartAngle, SweepAngle ) )
						return false;

					return true;
				}

				if ( Constants::ConstTrackEllipseInRectangle == TrackingName )
				{
					double Left		= _tstof ( oXMLNode.GetAttribute ( _T("left") ) );
					double Top		= _tstof ( oXMLNode.GetAttribute ( _T("top") ) );
					double Right	= _tstof ( oXMLNode.GetAttribute ( _T("right") ) );
					double Bottom	= _tstof ( oXMLNode.GetAttribute ( _T("bottom") ) );

					if ( false == CreateTrackEllipseInRectangle ( ppTrackObject, Left, Top, Right, Bottom ) )
						return false;

					return true;
				}

				if ( Constants::ConstTrackCircle == TrackingName )
				{
					double CenterX	= _tstof ( oXMLNode.GetAttribute ( _T("centerx") ) );
					double CenterY	= _tstof ( oXMLNode.GetAttribute ( _T("centery") ) );
					double Radius	= _tstof ( oXMLNode.GetAttribute ( _T("radius") ) );

					if ( false == CreateTrackCircle ( ppTrackObject, CenterX, CenterY, Radius ) )
						return false;

					return true;
				}

				if ( Constants::ConstTrackRectangleRotatable == TrackingName )
				{
					double Left			= _tstof ( oXMLNode.GetAttribute ( _T("left") ) );
					double Top			= _tstof ( oXMLNode.GetAttribute ( _T("top") ) );
					double Right		= _tstof ( oXMLNode.GetAttribute ( _T("right") ) );
					double Bottom		= _tstof ( oXMLNode.GetAttribute ( _T("bottom") ) );
					double Angle		= _tstof ( oXMLNode.GetAttribute ( _T("angle") ) );
					double StickAngle	= _tstof ( oXMLNode.GetAttribute ( _T("stickangle") ) );

					if ( false == CreateTrackRectangleRotatable ( ppTrackObject, Left, Top, Right, Bottom, Angle, StickAngle ) )
						return false;

					return true;
				}

				if ( Constants::ConstTrackRectangleRotatableFixedAspect == TrackingName )
				{
					double Left			= _tstof ( oXMLNode.GetAttribute ( _T("left") ) );
					double Top			= _tstof ( oXMLNode.GetAttribute ( _T("top") ) );
					double Right		= _tstof ( oXMLNode.GetAttribute ( _T("right") ) );
					double Bottom		= _tstof ( oXMLNode.GetAttribute ( _T("bottom") ) );
					double Angle		= _tstof ( oXMLNode.GetAttribute ( _T("angle") ) );
					double StickAngle	= _tstof ( oXMLNode.GetAttribute ( _T("stickangle") ) );
					int    NoResize		= _tstol ( oXMLNode.GetAttribute ( _T("noresize"), _T("0") ) );
					int    SizesMode	= _tstol ( oXMLNode.GetAttribute ( _T("sizesmode"), _T("0") ) ); 

					if ( false == CreateTrackRectangleRotatableFixedAspect ( ppTrackObject, Left, Top, Right, Bottom, Angle, StickAngle, NoResize, SizesMode ) )
						return false;

					return true;
				}

				if ( Constants::ConstTrackTextRotatable == TrackingName )
				{
					double Left			= _tstof ( oXMLNode.GetAttribute ( _T("left") ) );
					double Top			= _tstof ( oXMLNode.GetAttribute ( _T("top") ) );
					double Right		= _tstof ( oXMLNode.GetAttribute ( _T("right") ) );
					double Bottom		= _tstof ( oXMLNode.GetAttribute ( _T("bottom") ) );
					double Angle		= _tstof ( oXMLNode.GetAttribute ( _T("angle") ) );
					double StickAngle	= _tstof ( oXMLNode.GetAttribute ( _T("stickangle") ) );

					CString Text		= oXMLNode.GetAttribute ( _T("text") );
					
					int Length			= Text.GetLength();
					
					for ( int i = 0; i < Length; ++i )
					{
						if ( Text [i] == '\n' )
						{
							Text.Insert ( i,  '\r' );

							Length	=	Text.GetLength();
							++i;
						}
					}
					
					CString FontName	= oXMLNode.GetAttribute ( _T("fontname") );
					
					long FontSize		= _tstol ( oXMLNode.GetAttribute ( _T("fontsize") ) );
					int FontType		= _tstol ( oXMLNode.GetAttribute ( _T("fonttype") ) ); 
					int FontColor		= _tstol ( oXMLNode.GetAttribute ( _T("fontcolor") ) );

#ifdef UNICODE
					if ( false == CreateTrackTextRotatable ( ppTrackObject, Left, Top, Right, Bottom,
						Angle, StickAngle, W2BSTR ( Text ), W2BSTR ( FontName ), FontSize, FontType, FontColor ) )
						return false;
#else if
					if ( false == CreateTrackTextRotatable ( ppTrackObject, Left, Top, Right, Bottom,
						Angle, StickAngle, A2BSTR ( Text ), A2BSTR ( FontName ), FontSize, FontType, FontColor ) )
						return false;
#endif

					return true;
				}

				if ( Constants::ConstTrackText == TrackingName )
				{
					double Left			= _tstof ( oXMLNode.GetAttribute ( _T("left") ) );
					double Top			= _tstof ( oXMLNode.GetAttribute ( _T("top") ) );
					double Right		= _tstof ( oXMLNode.GetAttribute ( _T("right") ) );
					double Bottom		= _tstof ( oXMLNode.GetAttribute ( _T("bottom") ) );

					//CString XML			= oXMLNode.GetXml ();

					CString Text		= oXMLNode.GetAttribute ( _T("text") );
					
					int Length			= Text.GetLength();
					
					for ( int i = 0; i < Length; ++i )
					{
						if ( Text [i] == '\n' )
						{
							Text.Insert ( i,  '\r' );

							Length	=	Text.GetLength();
							++i;
						}
					}
					
					CString FontName	= oXMLNode.GetAttribute ( _T("fontname") );

					long FontSize		= _tstol ( oXMLNode.GetAttribute ( _T("fontsize") ) );
					int FontType		= _tstol ( oXMLNode.GetAttribute ( _T("fonttype") ) ); 
					int FontColor		= _tstol ( oXMLNode.GetAttribute ( _T("fontcolor") ) );

#ifdef UNICODE
					if ( false == CreateTrackText ( ppTrackObject, Left, Top, Right, Bottom,
						W2BSTR ( Text ), W2BSTR ( FontName ), FontSize, FontType, FontColor ) )
						return false;
#else if
					if ( false == CreateTrackText ( ppTrackObject, Left, Top, Right, Bottom,
						A2BSTR ( Text ), A2BSTR ( FontName ), FontSize, FontType, FontColor ) )
						return false;
#endif
					return true;
				}

				if ( Constants::ConstTrackTrajectoryPolyline == TrackingName )
				{
					CSimpleArray<Structures::POINTD> arrPoints;
					if ( false == CreatePointsData ( arrPoints, oXMLNode.GetAttribute ( _T("points" ) ) ) )
						return false;
					if ( false == CreateTrackTrajectoryPolyline ( ppTrackObject, (Structures::POINTD*)arrPoints.GetData(), arrPoints.GetSize() ) )
						return false;

					return true;
				}

				if ( Constants::ConstTrackCubicBezierCurve == TrackingName )
				{
					CSimpleArray<Structures::POINTD> arrPoints;
					if ( false == CreatePointsData ( arrPoints, oXMLNode.GetAttribute ( _T("points" ) ) ) )
						return false;

					Tracking::CTrackObject* pInner = NULL;

					// грузим внутреыLий трек дл€ даыLьBБE
					CXmlNode oXmlInner;
					if ( oXMLNode.GetNode ( _T("track"), oXmlInner ) )
						ReadXml ( &pInner, oXmlInner );

					// БEэффициеыQ дл€ БEисьDдиыDыLьBБEтрекинга
					double InnerFactor			=	_tstof ( oXMLNode.GetAttribute ( _T("innerfactor"), _T("0.0") ) );

					if ( false == CreateTrackCubicBezierCurve ( ppTrackObject, (Structures::POINTD*)arrPoints.GetData(), arrPoints.GetSize(), pInner, InnerFactor ) )
						return false;

					return true;
				}

				if ( Constants::ConstTrackEffectKenBurns == TrackingName )
				{
					double Left			= _tstof ( oXMLNode.GetAttribute ( _T("left") ) );
					double Top			= _tstof ( oXMLNode.GetAttribute ( _T("top") ) );
					double Right		= _tstof ( oXMLNode.GetAttribute ( _T("right") ) );
					double Bottom		= _tstof ( oXMLNode.GetAttribute ( _T("bottom") ) );
				
					double Left2		= _tstof ( oXMLNode.GetAttribute ( _T("left2") ) );
					double Top2			= _tstof ( oXMLNode.GetAttribute ( _T("top2") ) );
					double Right2		= _tstof ( oXMLNode.GetAttribute ( _T("right2") ) );
					double Bottom2		= _tstof ( oXMLNode.GetAttribute ( _T("bottom2") ) );

					if ( false == CreateTrackEffectKenBurns ( ppTrackObject, Left, Top, Right, Bottom, Left2, Top2, Right2, Bottom2 ) )
						return false;
				
					return true;
				}

				return false;
			}

		private:
			inline static CString FromInteger ( double Value )
			{
				CString String;

				String.Format ( _TEXT("%f"), Value );

				return String;
			}


			inline double AspectXAdd ( double x )
			{
				if ( m_nAspect < 0.001 )
					return x;

				return m_nAspectWidth*x/m_nWidth;
			}
			inline double AspectYAdd ( double y )
			{
				if ( m_nAspect < 0.001 )
					return y;

				return m_nAspectHeight*y/m_nHeight;
			}
			inline double AspectXRemove ( double x )
			{
				if ( m_nAspect < 0.001 )
					return x;

				return m_nWidth * x / m_nAspectWidth;
			}
			inline double AspectYRemove ( double y )
			{
				if ( m_nAspect < 0.001 )
					return y;

				return m_nHeight * y / m_nAspectHeight;
			}


			inline bool CreatePointsData ( CSimpleArray<Structures::POINTD>& arrPoints, const CString& Points )
			{
				if ( Points.GetLength () == 0 )
					return false;

				int ReadPos = 0;
				while ( ReadPos < Points.GetLength () )
				{
					if ( -1 == ReadPos )
						break;
					double X	= _tstof ( Points.Tokenize ( _T(" "), ReadPos ) );
					if ( -1 == ReadPos )
						break;
					double Y	= _tstof ( Points.Tokenize ( _T(" "), ReadPos ) );
					if ( -1 == ReadPos )
						break;
					arrPoints.Add ( Structures::POINTD ( X, Y ) );
				}

				return	arrPoints.GetSize () == 0 ? false: true;
			}

			inline bool CreateTrackPoint ( Tracking::CTrackObject** pTrack, double X, double Y )
			{
				double nX	= AspectXRemove ( X );
				double nY	= AspectYRemove ( Y );

				Tracking::CTrackObjectPoints* pTracking = new Tracking::CTrackObjectPoints ();
				if ( NULL != pTracking )
				{
					pTracking->Create ( nX, nY );

					(*pTrack) = pTracking;
					return true;
				}
				return false;
			}

			inline bool CreateTrackLine ( Tracking::CTrackObject** pTrack, double X1, double Y1, double X2, double Y2 )
			{
				double nX1	= AspectXRemove ( X1 );
				double nY1	= AspectYRemove ( Y1 );
				double nX2	= AspectXRemove ( X2 );
				double nY2	= AspectYRemove ( Y2 );

				Tracking::CTrackObjectLine* pTracking = new Tracking::CTrackObjectLine ();
				if ( NULL != pTracking )
				{
					pTracking->Create ( nX1, nY1, nX2, nY2 );

					(*pTrack) = pTracking;
					return true;
				}
				return false;
			}

			inline bool CreateTrackEllipse ( Tracking::CTrackObject** pTrack, double Left, double Top, double Right, double Bottom )
			{
				double nLeft	= AspectXRemove ( Left );
				double nTop		= AspectYRemove ( Top );
				double nRight	= AspectXRemove ( Right );
				double nBottom	= AspectYRemove ( Bottom );

				Tracking::CTrackObjectEllipse* pTracking = new Tracking::CTrackObjectEllipse ();
				if ( NULL != pTracking )
				{
					pTracking->Create ( nLeft, nTop, nRight, nBottom );

					(*pTrack) = pTracking;
					return true;
				}

				return false;
			}

			inline bool CreateTrackRectangle ( Tracking::CTrackObject** pTrack, double Left, double Top, double Right, double Bottom )
			{
				double nLeft	= AspectXRemove ( Left );
				double nTop		= AspectYRemove ( Top );
				double nRight	= AspectXRemove ( Right );
				double nBottom	= AspectYRemove ( Bottom );

				Tracking::CTrackObjectRectangle* pTracking = new Tracking::CTrackObjectRectangle ();
				if ( NULL != pTracking )
				{
					pTracking->Create ( nLeft, nTop, nRight, nBottom );

					(*pTrack) = pTracking;
					return true;
				}

				return false;
			}	


			inline bool CreateTrackRectangleFixedSize ( Tracking::CTrackObject** pTrack, double Left, double Top, double Right, double Bottom )
			{
				double dLeft	=	AspectXRemove ( Left	);
				double dTop		=	AspectYRemove ( Top		);
				double dRight	=	AspectXRemove (	Right	);
				double dBottom	=	AspectYRemove (	Bottom	);

				Tracking::CTrackObjectRectangleFixedSize* pTracking = new Tracking::CTrackObjectRectangleFixedSize();

				if ( NULL != pTracking )
				{
					pTracking->Create(dLeft, dTop, dRight, dBottom);

					(*pTrack) = pTracking;
					return true;
				}

				return false;
			}

			inline bool CreateTrackRectangleFixedAspect ( Tracking::CTrackObject** pTrack, double Left, double Top, double Right, double Bottom )
			{
				double dLeft	=	AspectXRemove ( Left	);
				double dTop		=	AspectYRemove ( Top		);
				double dRight	=	AspectXRemove (	Right	);
				double dBottom	=	AspectYRemove (	Bottom	);

				Tracking::CTrackObjectRectangleFixedAspect* pTracking = new Tracking::CTrackObjectRectangleFixedAspect();

				if ( NULL != pTracking )
				{
					pTracking->Create(dLeft, dTop, dRight, dBottom);

					(*pTrack) = pTracking;
					return true;
				}

				return false;
			}

			inline bool CreateTrackPolyline ( Tracking::CTrackObject** pTrack, Structures::POINTD* pPoints, int nPointsCount )
			{
				if ( NULL == pPoints || nPointsCount == 0 )
					return false;

				Tracking::CTrackObjectPolyline* pTracking = new Tracking::CTrackObjectPolyline();
				if ( NULL != pTracking )
				{
					pTracking->Create(pPoints, nPointsCount);
					(*pTrack) = pTracking;

					return true;
				}

				return false;
			}

			inline bool CreateTrackPolygon ( Tracking::CTrackObject** pTrack, Structures::POINTD* pPoints, int nPointsCount )
			{
				if ( NULL == pPoints || nPointsCount == 0 )
					return false;

				Tracking::CTrackObjectPolygon* pTracking = new Tracking::CTrackObjectPolygon();
				if ( NULL != pTracking )
				{
					pTracking->Create(pPoints, nPointsCount);
					(*pTrack) = pTracking;

					return true;
				}

				return false;
			}	

			inline bool CreateTrackPie ( Tracking::CTrackObject** pTrack, double Left, double Top, double Right, double Bottom, double StartAngle, double SweepAngle)
			{
				double dLeft	= AspectXRemove(Left);
				double dTop		= AspectYRemove(Top);
				double dRight	= AspectXRemove(Right);
				double dBottom	= AspectYRemove(Bottom);

				Tracking::CTrackObjectPie* pTracking = new Tracking::CTrackObjectPie();

				if ( NULL != pTracking )
				{
					pTracking->Create(dLeft, dTop, dRight, dBottom, StartAngle, SweepAngle);

					(*pTrack) = pTracking;
					return true;
				}

				return false;
			}

			inline bool CreateTrackSimplePie ( Tracking::CTrackObject** pTrack, double CenterX, double CenterY, double Radius, double StartAngle, double SweepAngle)
			{
				// create rect
				double Left = CenterX - Radius;
				double Right = CenterX + Radius;
				double Top = CenterY - Radius;
				double Bottom = CenterY + Radius;

				// set aspect
				double dLeft = AspectXRemove(Left);
				double dTop = AspectYRemove(Top);
				double dRight = AspectXRemove(Right);
				double dBottom = AspectYRemove(Bottom);

				Tracking::CTrackObjectPieSimple* pTracking = new Tracking::CTrackObjectPieSimple();
				if ( NULL != pTracking )
				{
					pTracking->Create(dLeft, dTop, dRight, dBottom, StartAngle, SweepAngle);

					(*pTrack) = pTracking;
					return true;
				}

				return false;
			}

			inline bool CreateTrackSector( Tracking::CTrackObject** pTrack, double Left, double Top, double Right, double Bottom, double StartAngle, double SweepAngle, double Ratio)
			{
				double dLeft = AspectXRemove(Left);
				double dTop = AspectYRemove(Top);
				double dRight = AspectXRemove(Right);
				double dBottom = AspectYRemove(Bottom);

				Tracking::CTrackObjectSector* pTracking = new Tracking::CTrackObjectSector();
				if ( NULL != pTracking )
				{
					pTracking->Create(dLeft, dTop, dRight, dBottom, StartAngle, SweepAngle, Ratio);

					(*pTrack) = pTracking;
					return true;
				}

				return false;
			}

			inline bool CreateTrackSimpleSector( Tracking::CTrackObject** pTrack, double CenterX, double CenterY, double Radius1, double Radius2, double StartAngle, double SweepAngle)
			{		
				// create rect
				double Left = CenterX - Radius1;
				double Right = CenterX + Radius1;
				double Top = CenterY - Radius1;
				double Bottom = CenterY + Radius1;

				// set aspect
				double dLeft = AspectXRemove(Left);
				double dTop = AspectYRemove(Top);
				double dRight = AspectXRemove(Right);
				double dBottom = AspectYRemove(Bottom);

				double Ratio = Radius2/Radius1;

				Tracking::CTrackObjectSectorSimple* pTracking = new Tracking::CTrackObjectSectorSimple();
				if ( NULL != pTracking )
				{
					pTracking->Create(dLeft, dTop, dRight, dBottom, StartAngle, SweepAngle, Ratio);

					(*pTrack) = pTracking;
					return true;
				}

				return false;
			}

			inline bool CreateTrackEllipseInRectangle( Tracking::CTrackObject** pTrack, double Left, double Top, double Right, double Bottom)
			{
				double dLeft = AspectXRemove(Left);
				double dTop = AspectYRemove(Top);
				double dRight = AspectXRemove(Right);
				double dBottom = AspectYRemove(Bottom);

				Tracking::CTrackObjectEllipseInRectangle* pTracking = new Tracking::CTrackObjectEllipseInRectangle();
				if ( NULL != pTracking )
				{
					pTracking->Create(dLeft, dTop, dRight, dBottom);

					(*pTrack) = pTracking;
					return true;
				}

				return false;
			}

			inline bool CreateTrackCircle( Tracking::CTrackObject** pTrack, double CenterX, double CenterY, double Radius)
			{
				// set aspect
				double dLeft = AspectXRemove(CenterX - Radius);
				double dTop = AspectYRemove(CenterY - Radius);
				double dRight = AspectXRemove(CenterX + Radius);
				double dBottom = AspectYRemove(CenterY + Radius);

				Tracking::CTrackObjectCircle* pTracking = new Tracking::CTrackObjectCircle();
				if ( NULL != pTracking )
				{
					pTracking->Create(dLeft, dTop, dRight, dBottom);

					(*pTrack) = pTracking;
					return true;
				}

				return false;
			}

			inline bool CreateTrackRectangleRotatable( Tracking::CTrackObject** pTrack, double Left, double Top, double Right, double Bottom, double Angle, double StickAngle)
			{
				double dLeft	= AspectXRemove(Left);
				double dTop		= AspectYRemove(Top);
				double dRight	= AspectXRemove(Right);
				double dBottom	= AspectYRemove(Bottom);

				Tracking::CTrackObjectRectangleRotatable* pTracking = new Tracking::CTrackObjectRectangleRotatable();
				if ( NULL != pTracking )
				{
					pTracking->Create(dLeft, dTop, dRight, dBottom, Angle, StickAngle );

					(*pTrack) = pTracking;
					return true;
				}

				return false;
			}

			inline bool CreateTrackRectangleRotatableFixedAspect( Tracking::CTrackObject** pTrack, double Left, double Top, double Right, double Bottom, double Angle, double StickAngle, int NoResize, int SizesMode )
			{
				double dLeft	= AspectXRemove(Left);
				double dTop		= AspectYRemove(Top);
				double dRight	= AspectXRemove(Right);
				double dBottom	= AspectYRemove(Bottom);

				Tracking::CTrackObjectRectangleRotatableFixedAspect* pTracking = new Tracking::CTrackObjectRectangleRotatableFixedAspect();
				if ( NULL != pTracking )
				{
					pTracking->Create( dLeft, dTop, dRight, dBottom, Angle, StickAngle );
					pTracking->SetNoResize	( (BOOL) NoResize );
					pTracking->SetSizesMode	( (BOOL) SizesMode );

					(*pTrack) = pTracking;
					return true;
				}

				return false;
			}

			inline bool CreateTrackTextRotatable( Tracking::CTrackObject** pTrack, double Left, double Top, double Right, double Bottom, double Angle, double StickAngle, BSTR inText, BSTR FontName, int FontSize, int FontType, int FontColor)
			{
				double dLeft	= AspectXRemove(Left);
				double dTop		= AspectYRemove(Top);
				double dRight	= AspectXRemove(Right);
				double dBottom	= AspectYRemove(Bottom);

				Tracking::CTrackObjectTextRotatable* pTracking = new Tracking::CTrackObjectTextRotatable();
				if ( NULL != pTracking )
				{
					pTracking->Create(dLeft, dTop, dRight, dBottom, Angle, StickAngle, inText, FontName, FontSize, FontType, FontColor);

					(*pTrack) = pTracking;
					return true;
				}

				return false;
			}

			inline bool CreateTrackText( Tracking::CTrackObject** pTrack, double Left, double Top, double Right, double Bottom, BSTR inText, BSTR FontName, int FontSize, int FontType, int FontColor)
			{
				double dLeft	= AspectXRemove(Left);
				double dTop		= AspectYRemove(Top);
				double dRight	= AspectXRemove(Right);
				double dBottom	= AspectYRemove(Bottom);

				Tracking::CTrackObjectText* pTracking = new Tracking::CTrackObjectText();
				if ( NULL != pTracking )
				{
					pTracking->Create(dLeft, dTop, dRight, dBottom, inText, FontName, FontSize, FontType, FontColor);

					(*pTrack) = pTracking;
					return true;
				}

				return false;
			}

			inline bool CreateTrackTrajectoryPolyline ( Tracking::CTrackObject** pTrack, Structures::POINTD* pPoints, int nPointsCount )
			{
				if ( NULL == pPoints || nPointsCount == 0 )
					return false;

				Tracking::CTrackObjectTrajectoryPolyline* pTracking = new Tracking::CTrackObjectTrajectoryPolyline();
				if ( NULL != pTracking )
				{
					pTracking->Create ( pPoints, nPointsCount );
					(*pTrack) = pTracking;

					return true;
				}

				return false;
			}

			inline bool CreateTrackCubicBezierCurve ( Tracking::CTrackObject** pTrack, Structures::POINTD* pPoints, int nPointsCount, Tracking::CTrackObject* pInner, double InnerFactor )
			{
				if ( NULL == pPoints || nPointsCount == 0 )
					return false;

				Tracking::CTrackCubicBezierCurve* pTracking = new Tracking::CTrackCubicBezierCurve();
				if ( NULL != pTracking )
				{
					pTracking->Create ( pPoints, nPointsCount, pInner, InnerFactor );
					(*pTrack) = pTracking;

					return true;
				}

				return false;
			}

			inline bool CreateTrackEffectKenBurns	( Tracking::CTrackObject** pTrack, double Left, double Top, double Right, double Bottom, double LeftEnd, double TopEnd, double RightEnd, double BottomEnd )
			{
				Tracking::CTrackEffectKenBurns* pTracking = new Tracking::CTrackEffectKenBurns();
				if ( NULL != pTracking )
				{
					pTracking->Create ( Left, Top, Right, Bottom, LeftEnd, TopEnd, RightEnd, BottomEnd );
					(*pTrack) = pTracking;

					return true;
				}

				return false;
			}

		private:																					
																								
			double	m_nAspect;																	
			int		m_nAspectWidth;
			int		m_nAspectHeight;
			int		m_nWidth;
			int		m_nHeight;
		};



		class CTracksXml
		{
		public:
			inline bool ReadXml ( CString Xml, CAtlArray<CTrackObject*>& pTracks, double nAspect, int nAspectWidth, int nAspectHeight, int nWidth, int nHeight )
			{
				Xml = Xml.MakeLower ();
				
				// ATLTRACE ( "SET : %s\n", Xml );

				CXmlNode oXmlNode;
				if ( oXmlNode.FromXmlString ( Xml ) )
				{
					CTrackObject* pCreateTrack	= NULL;
					CTrackXmlReader oReader ( nAspect, nAspectWidth, nAspectHeight, nWidth, nHeight );

					// Single Track
					if ( oReader.ReadXml ( &pCreateTrack, oXmlNode ) )
					{
						pCreateTrack->SetImageSizes ( nWidth, nHeight );

						pTracks.Add ( pCreateTrack );
						return true;
					}				

					CXmlNode oXmlSetNode;
					if ( oXmlNode.GetNode ( _T("set"), oXmlSetNode ) )
					{
						CXmlNodes oNodes;
						if ( oXmlSetNode.GetNodes ( _T("track"), oNodes ) )
						{
							for ( int Ind = 0; Ind < oNodes.GetCount(); ++Ind )
							{
								pCreateTrack = NULL;

								CXmlNode oXmlTrackNode;
								if ( oNodes.GetAt ( Ind, oXmlTrackNode ) )
								{
									if ( oReader.ReadXml ( &pCreateTrack, oXmlTrackNode ) )
									{
										CXmlNode oXmlSticking;
										if ( oXmlTrackNode.GetNode ( _T("sticking"), oXmlSticking ) )
										{
											CTrackSticking	oTrackSticking;

											///
											/// <sticking virtualmode="1" realmode="0" cellsize="10" align_center="0"/>
											///

											long VirtualMode	=	_tstol ( oXmlSticking.GetAttribute ( _T("virtualmode"),		_T("0") ) );
											long RealMode		=	_tstol ( oXmlSticking.GetAttribute ( _T("realmode"),		_T("0") ) );
											long AlignCenter	=	_tstol ( oXmlSticking.GetAttribute ( _T("align_center"),	_T("0") ) );

											long StickingLeft	=	_tstol ( oXmlSticking.GetAttribute ( _T("side_left"),		_T("0") ) );
											long StickingTop	=	_tstol ( oXmlSticking.GetAttribute ( _T("side_top"),		_T("0") ) );
											long StickingRight	=	_tstol ( oXmlSticking.GetAttribute ( _T("side_right"),		_T("0") ) );
											long StickingBottom	=	_tstol ( oXmlSticking.GetAttribute ( _T("side_bottom"),		_T("0") ) );

											long UnitSize		=	_tstol ( oXmlSticking.GetAttribute ( _T("cellsize"),		_T("0") ) );

											oTrackSticking.SetImageSizes	( nWidth, nHeight );
											oTrackSticking.SetUnitSize		( UnitSize );
											oTrackSticking.SetVirtualMode	( ( VirtualMode )	? true : false );
											oTrackSticking.SetAlign			( 0.0, 0.0 );
											oTrackSticking.SetRoundSticking ( ( StickingLeft )	? true : false, ( StickingTop ) ? true : false,  ( StickingRight ) ? true : false, ( StickingBottom ) ? true : false );

											if ( AlignCenter )
											{
												oTrackSticking.SetAlign ( (double) ( (nWidth/2) % UnitSize ), (double) ( (nHeight/2) % UnitSize ) );
											}

											pCreateTrack->GetTrackSticking ()	=	oTrackSticking;
										}
						
										pCreateTrack->SetImageSizes ( nWidth, nHeight );

										pTracks.Add ( pCreateTrack );
									}
								}
							}
						}
					}
				}

				if ( 0 == pTracks.GetCount () )
					return false;

				return true;
			}

			inline bool WriteXml ( BSTR* Xml, CAtlArray<CTrackObject*>& pTracks, double nAspect, int nAspectWidth, int nAspectHeight, int nWidth, int nHeight )
			{
				if ( 0 == pTracks.GetCount () )
					return false;

				CTrackXmlWriter oTrackWriter ( nAspect, nAspectWidth, nAspectHeight, nWidth, nHeight );

				// Single Track
				if ( 1 == pTracks.GetCount() )
				{
					CString TrackXml;
					if ( oTrackWriter.WriteXml ( pTracks[0], TrackXml ) )
					{
						*Xml = TrackXml.AllocSysString();
						return true;
					}

					return false;
				}

				CXmlWriter oWriter;
				oWriter.WriteString ( _T("<trackcompound><set>") );

				/*
				oWriter.WriteNodeBegin ( _T("trackcompound"), FALSE );
				oWriter.WriteNodeBegin ( _T("set"), FALSE );
				*/

				for ( size_t Ind = 0; Ind < pTracks.GetCount(); ++Ind )
				{
					CString TrackXml;
					if ( oTrackWriter.WriteXml ( pTracks[Ind], TrackXml ) )
						oWriter.WriteString ( TrackXml );
				}

				oWriter.WriteString ( _T("</set></trackcompound>") );

				/*
				oWriter.WriteNodeEnd ( _T("set"), FALSE );
				oWriter.WriteNodeEnd ( _T("trackcompound"), FALSE );
				*/

				*Xml = oWriter.GetXmlString ().AllocSysString();

				return true;
			}

		};

		class CTracksXmlSettings
		{
		public:

			inline bool ReadXml ( CString Xml, CTrackPainter& oPainter, CAtlArray<CTrackPainter*>& pPainters )
			{
				m_XmlSettings = _T("<?xml version=\"1.0\" encoding=\"utf-8\"?>") + Xml;
				m_XmlSettings = Xml.MakeLower ();
			
				CXmlNode oXmlNode;
				if ( oXmlNode.FromXmlString ( Xml ) )
				{
					// ьCин ъCрисовщик БE умьJчаыGю дл€ всех трекингоБE
					CXmlNode oXmlPainter;
					if ( oXmlNode.GetNode ( _T("painter"), oXmlPainter ) )
					{
						ReadPainterXml ( oXmlPainter, oPainter );

						return true;
					}

					// ъCдеБEыZБEъCрисовщикБEдл€ БEждьBБEтрекБE
					if ( oXmlNode.GetNode ( _T("painters"), oXmlPainter ) )
					{
						CXmlNodes oXmlPainters;
						if ( oXmlPainter.GetNodes ( _T("painter"), oXmlPainters ) )
						{
							for ( int Ind = 0; Ind < oXmlPainters.GetCount (); ++Ind )
							{
								CXmlNode oXmlPainter2;
								if ( oXmlPainters.GetAt ( Ind, oXmlPainter2 ) )
								{
									CTrackPainter* pPainter = new CTrackPainter ();
									if ( NULL != pPainter )
									{
										// считываеБEсоставыMБEъCрисовщик
										CXmlNode oXmlCompound; 
										if ( oXmlPainter2.GetNode ( _T("set"), oXmlCompound ) )
										{
											if ( ReadCompoundPainterXml ( oXmlCompound, *pPainter ) )
												pPainters.Add ( pPainter );
											else
												RELEASEOBJECT ( pPainter );

											continue;
										}

										if ( ReadPainterXml ( oXmlPainter2, *pPainter ) )
											pPainters.Add ( pPainter );
										else
											RELEASEOBJECT ( pPainter );
									}
								}
							}

							return ( pPainters.GetCount () == 0 ) ? false : true;
						}
					}
				}

				return false;
			}

			inline bool WriteXml ( BSTR* Xml, CTrackPainter& oPainter, CAtlArray<CTrackPainter*>& pPainters )
			{
				CXmlWriter oWriter;
				oWriter.WriteNodeBegin ( _T("settings"), FALSE );

				if ( 0 == pPainters.GetCount () )
				{
					oWriter.WriteString ( WritePainterXml (oPainter) );
				}
				else
				{
					for ( size_t Ind = 0; Ind < pPainters.GetCount (); ++Ind )
					{
						if ( 0 == pPainters[Ind]->CountInnerPainters () )
						{
							// БEъBтоБEъCрисовщик
							oWriter.WriteString ( WritePainterXml ( *pPainters[Ind] ) );
						}
						else
						{
							// составыMБEъCрисовщик - ъь БEрсБEБEивБE
							oWriter.WriteString ( WriteCompoundPainter ( *pPainters[Ind] ) );
						}
					}
				}

				oWriter.WriteNodeEnd ( _T("settings"), FALSE );

				*Xml = oWriter.GetXmlString ().AllocSysString();

				return true;
			}


		private:			

			inline CString WritePainterXml ( CTrackPainter& oPainter )
			{
				int nWidth = 0, nStyle = 0, nType = 0;
				DWORD nColor1 = 0, nColor2 = 0;

				CXmlWriter oWriter;
				oWriter.WriteNodeBegin ( _T("painter"), FALSE );

				oWriter.WriteNodeBegin ( _T("lines"), FALSE );
				oWriter.WriteNodeBegin ( _T("pen"), TRUE );

				oPainter.GetLinesProperty ( nWidth, nStyle, nColor1, nColor2, nType );

				oWriter.WriteAttribute ( _T("width"), FromInteger ( nWidth ) );
				oWriter.WriteAttribute ( _T("style"), FromInteger ( nStyle ) );
				oWriter.WriteAttribute ( _T("color1"), FromInteger ( nColor1 ) );
				oWriter.WriteAttribute ( _T("color2"), FromInteger ( nColor2 ) );
				oWriter.WriteAttribute ( _T("type"), FromInteger ( nType ) );

				oWriter.WriteNodeEnd ( _T("pen"), TRUE );
				oWriter.WriteNodeEnd ( _T("lines"), FALSE );

				oWriter.WriteNodeBegin ( _T("points"), FALSE );
				oWriter.WriteNodeBegin ( _T("pen"), TRUE );

				oPainter.GetPointsProperty ( nWidth, nStyle, nColor1, nColor2, nType );

				oWriter.WriteAttribute ( _T("width"), FromInteger ( nWidth ) );
				oWriter.WriteAttribute ( _T("style"), FromInteger ( nStyle ) );
				oWriter.WriteAttribute ( _T("color1"), FromInteger ( nColor1 ) );
				oWriter.WriteAttribute ( _T("color2"), FromInteger ( nColor2 ) );
				oWriter.WriteAttribute ( _T("type"), FromInteger ( nType ) );

				oWriter.WriteNodeEnd ( _T("pen"), TRUE );
				oWriter.WriteNodeEnd ( _T("points"), FALSE );

				oWriter.WriteNodeEnd ( _T("painter"), FALSE );

				return oWriter.GetXmlString ();
			}

			inline CString WriteCompoundPainter ( CTrackPainter& oPainter )
			{
				CXmlWriter oWriter;
				oWriter.WriteNodeBegin ( _T("painter"), FALSE );

				oWriter.WriteNodeBegin ( _T("set"), FALSE );

				for ( int Ind = 0; Ind < oPainter.CountInnerPainters (); ++Ind )
				{
					oWriter.WriteString ( WritePainterXml ( *oPainter.GetInnerPainter(Ind) ) );
				}

				oWriter.WriteNodeEnd ( _T("set"), FALSE );
				oWriter.WriteNodeEnd ( _T("painter"), FALSE );

				return oWriter.GetXmlString ();
			}
			inline bool ReadPainterXml ( CXmlNode& oXmlPainter, CTrackPainter& oPainter )
			{
				CString Xml = oXmlPainter.GetXml ();

				CXmlNode oXmlLines;
				if ( oXmlPainter.GetNode (_T("lines"), oXmlLines ) )
				{
					CXmlNode oXmlPen;
					if ( oXmlLines.GetNode (_T("pen"), oXmlPen ) )
					{
						int nWidth	= _tstol ( oXmlPen.GetAttribute ( _T("width"), _T("1") ) );
						int nStyle	= _tstol ( oXmlPen.GetAttribute ( _T("style"), _T("2") ) );		// PS_DOT
						DWORD nColor1	= (DWORD)_tstof ( oXmlPen.GetAttribute ( _T("color1"), _T("-2") ) );
						DWORD nColor2	= (DWORD)_tstof ( oXmlPen.GetAttribute ( _T("color2"), _T("-1") ) );
						int nType	= _tstol ( oXmlPen.GetAttribute ( _T("type"),	_T("0") ) );

						oPainter.SetLinesProperty ( nWidth, nStyle, nColor1, nColor2, nType );
					}
				}

				CXmlNode oXmlPoints;
				if ( oXmlPainter.GetNode (_T("points"), oXmlPoints ) )
				{
					CXmlNode oXmlPen;
					if ( oXmlPoints.GetNode (_T("pen"), oXmlPen ) )
					{
						int nWidth	= _tstol ( oXmlPen.GetAttribute ( _T("width"), _T("1") ) );
						int nStyle	= _tstol ( oXmlPen.GetAttribute ( _T("style"), _T("0") ) );	// PS_SOLID
						DWORD nColor1	= (DWORD)_tstof ( oXmlPen.GetAttribute ( _T("color1"), _T("-2") ) );
						DWORD nColor2	= (DWORD)_tstof ( oXmlPen.GetAttribute ( _T("color2"), _T("-2") ) );
						int nType	= _tstol ( oXmlPen.GetAttribute ( _T("type"),	_T("0") ) );

						oPainter.SetPointsProperty ( nWidth, nStyle, nColor1, nColor2, nType );
					}
				}

				return true;
			}

			inline bool ReadCompoundPainterXml ( CXmlNode& oXmlCompound, CTrackPainter& oPainter )
			{
				CXmlNodes oXmlPainters;
				if ( oXmlCompound.GetNodes ( _T("painter"), oXmlPainters ) )
				{
					for ( int Ind = 0; Ind < oXmlPainters.GetCount (); ++Ind )
					{
						CXmlNode oXmlPainter2;
						if ( oXmlPainters.GetAt ( Ind, oXmlPainter2 ) )
						{
							CTrackPainter* pPainter = new CTrackPainter ();
							if ( NULL != pPainter )
							{
								ReadPainterXml ( oXmlPainter2, *pPainter );
								oPainter.AddInnerPainter ( pPainter );
							}
						}
					}
				}

				return ( oPainter.CountInnerPainters () == 0 ) ? false : true;
			}

			inline static CString FromInteger ( int Value )
			{
				CString String;

				String.Format ( _TEXT("%d"), Value );

				return String;
			}

		private:

			CString m_XmlSettings;
		};
	}

	namespace Serialize
	{
		namespace Constants	// Commands
		{
			// БEБEыCБEдл€ трекинга
			const CString g_scSetCubicBezierInnerFactor		=	_T("setcubicbezierinnerfactor");
			const CString g_scGetCubicBezierBoundTrack 		=	_T("getcubicbezierboundtrack");
			const CString g_scGetCubicBezierValue			=	_T("getcubicbeziervalue");
			const CString g_scGetNewBezierNode 				=	_T("getnewbeziernode");
			const CString g_scSetCubicBezierMouseEditMode	=	_T("setcubicbeziermouseeditmode");
			const CString g_scGetCubicBezierMouseIntersect	=	_T("getcubicbezierintersect");

			// БEБEыCБEдл€ БEбическьH БEивьH ЅезБE ыD БEив€заыZБEБEтрекингу (внешыGБEвычисленБE БE заБEъBБE
			const CString g_scCubicBezierGetValue			=	_T("cubicbeziergetvalue");			//	БE 4-ем точкам БET БEБEчить значение ъь БEивьH
			const CString g_scCubicBezierGetDimensions		=	_T("cubicbeziergetdimensions");		//	БEБEчить габариты БEивьH (БEивых)
			const CString g_scCubicBezierGetSplit			=	_T("cubicbeziergetsplit");			//	БEБEчить ъьбоБEточеБEдл€ разбиеыGБEБEивьH ъь двБEБE значенБE T
		};

		class Command
		{
		public:
			inline static bool Execute ( BSTR CommandXml, VARIANT* pParamValue, CAtlArray<CTrackObject*>& arrTracks )
			{
				CXmlNode oXmlNode;

				if ( oXmlNode.FromXmlString ( CString ( CommandXml ) ) )
				{
#ifdef _DEBUG
					//OutputDebugString ( oXmlNode.GetXml () );
					//OutputDebugString ( _T("\n") );
#endif
					CString CommandName		=	oXmlNode.GetAttribute ( _T("name"), _T("") );
					long Track				=	_tstol ( oXmlNode.GetAttribute ( _T("track"), _T("0") ) );

					if ( (int)arrTracks.GetCount () > 0 )
					{
						//	изБEыDыGБEвнутреыLегБEзначение БE БEтороБE БEикреБE€етБE трек БEбезьБEБEивым
						//	"<command name=\"setinnerfactor\" value=\"1\" />"
						if ( CommandName == Serialize::Constants::g_scSetCubicBezierInnerFactor )
							return Command::SetCubicBezierInnerFactor ( oXmlNode, pParamValue, arrTracks[Track] );

						//	БEБEчить размерБEБEБEторыБEвписанБEБEивые
						//	"<command name=\"getboundtrack\" />",
						if ( CommandName == Serialize::Constants::g_scGetCubicBezierBoundTrack )
							return GetCubicBezierBoundTrack ( oXmlNode, pParamValue, arrTracks[Track] );

						//	изБEыDыGБEвнутреыLегБEзначение БE БEтороБE БEикреБE€етБE трек БEбезьБEБEивым
						//	"<command name=\"getbeziervalues\" curve=\"0\" innerfactor=\"0.5\"/>  
						if ( CommandName ==	Serialize::Constants::g_scGetCubicBezierValue )
							return GetCubicBezierValue ( oXmlNode, pParamValue, arrTracks[Track] );


						//	<command name="getnewbeziernode" curve="0" innerfactor="0.5"/> 
						//	<!-- curve - индеБE БEивьH, innerfactor - ЅезБE БEраБEтр дл€ этоБEБEивьH 0.0 <= T <= 1.0 -->
						if ( CommandName == Serialize::Constants::g_scGetNewBezierNode )
							return GetNewBezierNode ( oXmlNode, pParamValue, arrTracks[Track] );

						//	<command name="setcubicbeziermouseeditmode" value="0"/> 
						if ( CommandName == Serialize::Constants::g_scSetCubicBezierMouseEditMode )
							return SetCubicBezierMouseEditMode ( oXmlNode, pParamValue, arrTracks[Track] );

						// <command name="getcubicbezierintersect" x="0" y="0" />"
						if ( CommandName == Serialize::Constants::g_scGetCubicBezierMouseIntersect )
							return GetCubicBezierMouseIntersect ( oXmlNode, pParamValue, arrTracks[Track] );
					}

					// БEБEыCБEБEторыБEдоБEыZ возвращать БEстые строчкБE БEгдБEтрекБEыDту
					if ( CommandName == Serialize::Constants::g_scGetCubicBezierMouseIntersect || 
						CommandName == Serialize::Constants::g_scGetNewBezierNode ||
						CommandName == Serialize::Constants::g_scGetCubicBezierBoundTrack ||
						CommandName == Serialize::Constants::g_scGetCubicBezierValue ) 
					{
						pParamValue->vt			=	VT_BSTR;
						pParamValue->bstrVal	=	CString(_TEXT("")).AllocSysString();

						return false;
					}

					// БEБEыCБEдл€ БEбическьH БEивьH ЅезБE ыD БEив€заыZБEБEтрекингу (внешыGБEвычисленБE БE заБEъBБE
					if ( CommandName == Serialize::Constants::g_scCubicBezierGetValue )
						return CubicBezierGetValue ( oXmlNode, pParamValue );
					
					if ( CommandName == Serialize::Constants::g_scCubicBezierGetDimensions )
						return CubicBezierGetDimensions ( oXmlNode, pParamValue );
					
					if ( CommandName == Serialize::Constants::g_scCubicBezierGetSplit )
						return CubicBezierGetSplit ( oXmlNode, pParamValue );
				}

				return false;
			}


		private:

			// БEБEыCБEдл€ трекинга
			inline static bool SetCubicBezierInnerFactor ( CXmlNode& oXmlNode, VARIANT* pParamValue, CTrackObject* pTrack )
			{
				double Value = _tstof ( oXmlNode.GetAttribute ( _T("value"), _T("0.0") ) );

				if ( pTrack->IsTrackTypeCubicBezierCurve () )
				{
					CTrackCubicBezierCurve* pTrackQ	= static_cast<CTrackCubicBezierCurve*> ( pTrack );
					if ( NULL != pTrackQ )
					{
						pTrackQ->SetInnerFactor ( Value );
						return true;
					}
				}

				return false;
			}
			inline static bool GetCubicBezierBoundTrack ( CXmlNode& oXmlNode, VARIANT* pParamValue, CTrackObject* pTrack )
			{
				if ( pTrack->IsTrackTypeCubicBezierCurve () )
				{
					CTrackCubicBezierCurve* pTrackQ = static_cast<CTrackCubicBezierCurve*> (pTrack);
					if ( NULL != pTrackQ )
					{
						CTrackPoint PtBound [2];
						if ( pTrackQ->GetBoundExternalTrack ( &PtBound[0] ) )
						{
							CString Bound;
							Bound.Format ( _T("%f %f %f %f"), 
								PtBound[0].dX, PtBound[0].dY,PtBound[1].dX, PtBound[1].dY );

							pParamValue->vt			=	VT_BSTR;
							pParamValue->bstrVal	=	Bound.AllocSysString();

							return true;
						}
					}
				}
				
				pParamValue->vt			=	VT_BSTR;
				pParamValue->bstrVal	=	CString(_TEXT("")).AllocSysString();
				
				return false;
			}

			inline static bool GetCubicBezierValue ( CXmlNode& oXmlNode, VARIANT* pParamValue, CTrackObject* pTrack )
			{
				int	Curve			=	_tstoi ( oXmlNode.GetAttribute ( _T("curve"), _T("-1.0") ) );
				double InnerFactor	=	_tstof ( oXmlNode.GetAttribute ( _T("innerfactor"), _T("-1.0") ) );

				if ( -1.0f != InnerFactor && -1 != Curve )
				{
					if ( pTrack->IsTrackTypeCubicBezierCurve () )
					{
						CTrackCubicBezierCurve* pTrackQ =	static_cast<CTrackCubicBezierCurve*> (pTrack);
						if ( NULL != pTrackQ )
						{
							CTrackPoint BPoint;
							pTrackQ->GetBezierValue ( Curve, InnerFactor, BPoint );

							CString BezierPoint;
							BezierPoint.Format ( _T("%f %f"), BPoint.dX, BPoint.dY );

							pParamValue->vt			=	VT_BSTR;
							pParamValue->bstrVal	=	BezierPoint.AllocSysString();

							return true;
						}
					}
				}

				pParamValue->vt			=	VT_BSTR;
				pParamValue->bstrVal	=	CString(_TEXT("")).AllocSysString();

				return false;
			}
			inline static bool GetNewBezierNode ( CXmlNode& oXmlNode, VARIANT* pParamValue, CTrackObject* pTrack )
			{
				double InnerFactor		=	_tstof ( oXmlNode.GetAttribute ( _T("innerfactor"), _T("-1.0") ) );
				int SelectCurve			=	_tstoi ( oXmlNode.GetAttribute ( _T("curve"), _T("-1.0") ) );

				if ( -1.0f != InnerFactor && -1 != SelectCurve )
				{
					if ( pTrack->IsTrackTypeCubicBezierCurve () )
					{
						CTrackCubicBezierCurve* pTrackQ =	static_cast<CTrackCubicBezierCurve*> (pTrack);
						if ( NULL != pTrackQ )
						{
							CTrackPoint SplitCurve [5];
							if ( pTrackQ->GetSplit ( SelectCurve, InnerFactor, SplitCurve ) )
							{
								CString ReturnValues;

								ReturnValues.Format ( _T("%f %f %f %f %f %f %f %f %f %f"),
									SplitCurve[0].dX, SplitCurve[0].dY,
									SplitCurve[1].dX, SplitCurve[1].dY, 
									SplitCurve[2].dX, SplitCurve[2].dY,
									SplitCurve[3].dX, SplitCurve[3].dY, 
									SplitCurve[4].dX, SplitCurve[4].dY );

								pParamValue->vt			=	VT_BSTR;
								pParamValue->bstrVal	=	ReturnValues.AllocSysString();

								return true;
							}
						}
					}
				}

				pParamValue->vt			=	VT_BSTR;
				pParamValue->bstrVal	=	CString(_TEXT("")).AllocSysString();

				return false;
			}



			inline static bool SetCubicBezierMouseEditMode ( CXmlNode& oXmlNode, VARIANT* pParamValue, CTrackObject* pTrack )
			{
				if ( pTrack->IsTrackTypeCubicBezierCurve () )
				{
					CTrackCubicBezierCurve* pTrackQ	= static_cast<CTrackCubicBezierCurve*> ( pTrack );
					if ( NULL != pTrackQ )
					{
						int Value = _tstoi ( oXmlNode.GetAttribute ( _T("value"), _T("0") ) );

						if ( Value )
							pTrackQ->SetMouseEditMode ( true );
						else
							pTrackQ->SetMouseEditMode ( false );
					}
				}
				return true;
			}

			inline static bool GetCubicBezierMouseIntersect ( CXmlNode& oXmlNode, VARIANT* pParamValue, CTrackObject* pTrack )
			{
				long MouseX		=	_tstol ( oXmlNode.GetAttribute ( _T("x"), _T("-10000000.0") ) );
				long MouseY		=	_tstol ( oXmlNode.GetAttribute ( _T("y"), _T("-10000000.0") ) );

				//if ( MouseX != -10000000.0 && MouseY != -10000000.0 )
				if ( pTrack->IsTrackTypeCubicBezierCurve () )
				{
					CTrackCubicBezierCurve* pTrackQ =	static_cast<CTrackCubicBezierCurve*> (pTrack);
					if ( NULL != pTrackQ )
					{
						long Curve		=	-1;
						double Factor	=	0.0;

						if ( pTrackQ->GetIntersect ( MouseX, MouseY, Curve, Factor ) )
						{
							CString Intersect;		Intersect.Format ( _T("%d %f"), Curve, Factor );

							pParamValue->vt			=	VT_BSTR;
							pParamValue->bstrVal	=	Intersect.AllocSysString();

							return true;
						}
					}
				}

				pParamValue->vt			=	VT_BSTR;
				pParamValue->bstrVal	=	CString(_TEXT("")).AllocSysString();

				return false;
			}
			// БEБEыCБEдл€ БEбическьH БEивьH ЅезБE ыD БEив€заыZБEБEтрекингу (внешыGБEвычисленБE БE заБEъBБE
			inline static bool CubicBezierGetValue ( CXmlNode& oXmlNode, VARIANT* pParamValue )
			{
				double Factor			=	_tstof ( oXmlNode.GetAttribute ( _T("factor"), _T("-1.0") ) );
				CString BezierCurve		=	oXmlNode.GetAttribute ( _T("points"), _T("") );

				if ( 0 < BezierCurve.GetLength () && -1.0 != Factor )
				{
					Tracking::CurveCubicBezier Curve;
					if ( Curve.ReadFromString ( BezierCurve ) )
					{
						Tracking::Point2D BPoint = Curve.GetValue ( Factor );

						CString BezierPoint;
						BezierPoint.Format ( _T("%f %f"), BPoint.m_nX, BPoint.m_nY );

						pParamValue->vt			=	VT_BSTR;
						pParamValue->bstrVal	=	BezierPoint.AllocSysString();

						return true;
					}
				}

				pParamValue->vt			=	VT_BSTR;
				pParamValue->bstrVal	=	CString(_TEXT("")).AllocSysString();

				return false;
			}	
			inline static bool CubicBezierGetDimensions ( CXmlNode& oXmlNode, VARIANT* pParamValue )
			{
				CString BezierCurve		=	oXmlNode.GetAttribute ( _T("points"), _T("") );
				int	SplittingCount		=	_tstoi ( oXmlNode.GetAttribute ( _T("splitting"), _T("200") ) );

				if ( 0 < BezierCurve.GetLength () )
				{
					CSimpleArray<CurveCubicBezier> arrCurves;
					if ( Tracking::CurveCubicBezier::CreateCurvesFromString ( BezierCurve, arrCurves ) )
					{
						Point2D Min, Max;
						arrCurves[0].GetDimensions ( Max, Min );

						for ( int i = 1; i < arrCurves.GetSize(); ++i )
						{
							Point2D CurveMin, CurveMax;
							arrCurves[i].GetDimensions ( CurveMax, CurveMin, SplittingCount );
							
							if ( CurveMax.m_nX > Max.m_nX )
								 Max.m_nX = CurveMax.m_nX;
							
							if ( CurveMax.m_nY > Max.m_nY )
								 Max.m_nY = CurveMax.m_nY;
						
							if ( CurveMin.m_nX < Min.m_nX )
								 Min.m_nX = CurveMin.m_nX;
							
							if ( CurveMin.m_nY < Min.m_nY )
								 Min.m_nY = CurveMin.m_nY;
						}
						
						CString Dimensions;
						Dimensions.Format ( _T("%f %f %f %f"), Min.m_nX, Min.m_nY, Max.m_nX, Max.m_nY );

						pParamValue->vt			=	VT_BSTR;
						pParamValue->bstrVal	=	Dimensions.AllocSysString();

						return true;
					}
				}

				pParamValue->vt			=	VT_BSTR;
				pParamValue->bstrVal	=	CString(_TEXT("")).AllocSysString();
				
				return false;
			}	
			inline static bool CubicBezierGetSplit ( CXmlNode& oXmlNode, VARIANT* pParamValue )
			{
				double Factor			=	_tstof ( oXmlNode.GetAttribute ( _T("factor"), _T("-1.0") ) );
				CString BezierCurve		=	oXmlNode.GetAttribute ( _T("points"), _T("") );

				if ( 0 < BezierCurve.GetLength () && -1.0 != Factor )
				{
					Tracking::CurveCubicBezier Curve;
					if ( Curve.ReadFromString ( BezierCurve ) )
					{
						Tracking::Point2D SplitCurve [5];

						Curve.GetSplit ( Factor, &SplitCurve[0] );

						CString Values;
						Values.Format ( _T("%f %f %f %f %f %f %f %f %f %f"),
							SplitCurve[0].m_nX, SplitCurve[0].m_nY,	SplitCurve[1].m_nX, SplitCurve[1].m_nY, 
							SplitCurve[2].m_nX, SplitCurve[2].m_nY,	SplitCurve[3].m_nX, SplitCurve[3].m_nY, 
							SplitCurve[4].m_nX, SplitCurve[4].m_nY );

						pParamValue->vt			=	VT_BSTR;
						pParamValue->bstrVal	=	Values.AllocSysString();

						return true;
					}
				}

				pParamValue->vt			=	VT_BSTR;
				pParamValue->bstrVal	=	CString(_TEXT("")).AllocSysString();

				return false;
			}	
		};
	}
}
