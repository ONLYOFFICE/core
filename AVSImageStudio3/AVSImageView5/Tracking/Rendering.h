#pragma once


//using namespace System::Windows::Forms::ControlPaint;

#include <gdiplusex.h>
#include <paintstruct.h>
#include <geometry.h>
#include <xmlutils.h>

//  Font align by width
#define ALIGNED_LEFT        0
#define ALIGNED_CENTER      1
#define ALIGNED_RIGHT       2
#define ALIGNED_WIDTH		3

//  Font align by height
#define ALIGNED_TOP         0
#define ALIGNED_MIDDLE      1
#define ALIGNED_BOTTOM      2

//  FontStyle
#define FONTSTYLE_BOLD      0x00000001
#define FONTSTYLE_ITALIC    0x00000002
#define FONTSTYLE_UNDERLINE 0x00000004
#define FONTSTYLE_STRIKEOUT 0x00000008

#define GetFontAlignedW(x) ( (x & 0x00000030) >> 4 )
#define GetFontAlignedH(x) ( (x & 0x00000300) >> 8 )
#define GetFontScale(x)	   ( (x & 0x0000f000) >> 12 )

enum FontAlign
{
	byTop,		//	0
	byMiddle,	//	1
	byBottom	//	2
};

namespace Rendering
{
	namespace Utils
	{
		void MeasureString( HDC hDC, const WCHAR *string, int length, const Gdiplus::Font *font, const Gdiplus::RectF &layoutRect, Gdiplus::RectF *boundingBox )
		{
			Gdiplus::Graphics graphics( hDC );
			graphics.MeasureString( string, length, font, layoutRect, boundingBox );
		}
		double GetFontHeight( Gdiplus::Font *font )
		{
			Gdiplus::FontFamily internFontFamily;
			font->GetFamily( &internFontFamily );
			double fontSize		= font->GetSize();
			double emHeight		= internFontFamily.GetEmHeight( font->GetStyle() );
			double realHeight	= internFontFamily.GetCellAscent( font->GetStyle() );
			realHeight			+= internFontFamily.GetCellDescent( font->GetStyle() );
			return ( 0 == emHeight ) ? 0.0 : fontSize / emHeight * realHeight;
		}
	}
	namespace Gdi
	{
		void FillRectangle(HDC hDC, OLE_COLOR nColor, RECT rect)
		{
			COLORREF color; OleTranslateColor(nColor, NULL, &color);
			SetBkColor(hDC, color);
			ExtTextOutA(hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
		}
		void FillLine(HDC hDC, OLE_COLOR nColor, int x1, int y1, int x2, int y2)
		{
			RECT rect = {x1, y1, x2 + 1, y2 + 1};

			FillRectangle(hDC, nColor, rect);
		}
		void DrawLine(HDC hDC, int x1, int y1, int x2, int y2)
		{
			MoveToEx(hDC, x1, y1, 0);
			LineTo(hDC, x2, y2);
		}
		void DrawPolyline(HDC hDC, POINT* pPoints, int nCount, BOOL bClose)
		{
			Polyline(hDC, pPoints, nCount);

			if (bClose)
				DrawLine(hDC, pPoints[nCount - 1].x, pPoints[nCount - 1].y, pPoints[0].x, pPoints[0].y);
		}
		void DrawRectangle(HDC hDC, int left, int top, int right, int bottom)
		{
			POINT points[5];

			points[0].x = left;
			points[0].y = top;
			points[1].x = right;
			points[1].y = top;
			points[2].x = right;
			points[2].y = bottom;
			points[3].x = left;
			points[3].y = bottom;
			points[4].x = left;
			points[4].y = top;

			DrawPolyline(hDC, points, 5, FALSE);
		}
		void DrawEllipse(HDC hDC, int left, int top, int right, int bottom, int divisions = 128)
		{
			POINT* pPoints = new POINT[divisions + 1];

			double cx = 0.5*(left + right);
			double cy = 0.5*(top + bottom);
			double rx = 0.5*fabs((double)right - left);
			double ry = 0.5*fabs((double)bottom - top);

			for (int index = 0; index <= divisions; ++index)
			{
				pPoints[index].x = (int)(0.5 + cx + rx*cos(6.28*index/divisions));
				pPoints[index].y = (int)(0.5 + cy + ry*sin(6.28*index/divisions));
			}

			DrawPolyline(hDC, pPoints, divisions + 1, FALSE);

			delete[] pPoints;
		}
		void DrawArc(HDC hDC, int left, int top, int right, int bottom, int x1, int y1, int x2, int y2)
		{
			Arc(hDC, left, top, right, bottom, x1, y1, x2, y2);
		}
		void DrawPie(HDC hDC, int left, int top, int right, int bottom, int x1, int y1, int x2, int y2)
		{
			DrawArc(hDC, left, top, right, bottom, x1, y1, x2, y2);

			double cx = 0.5*(left + right);
			double cy = 0.5*(top + bottom);
			double rx = 0.5*fabs((double)right - left);
			double ry = 0.5*fabs((double)bottom - top);

			if (rx < 1 && ry < 1)
				return;

			double a1 = Geometry::GetAngle(cx, cy, x1, y1);
			double a2 = Geometry::GetAngle(cx, cy, x2, y2);

			double nx1, ny1; Geometry::GetEllipsePointCoord(nx1, ny1, rx, ry, a1);
			double nx2, ny2; Geometry::GetEllipsePointCoord(nx2, ny2, rx, ry, a2);

			DrawLine(hDC, (int)(0.5 + cx), (int)(0.5 + cy), (int)(0.5 + cx + nx1), (int)(0.5 + cy + ny1));
			DrawLine(hDC, (int)(0.5 + cx), (int)(0.5 + cy), (int)(0.5 + cx + nx2), (int)(0.5 + cy + ny2));
		}
		void DrawCircleSector(HDC hDC, double cx, double cy, double r1, double r2, double a1, double a2)
		{
			int x11 = (int)(0.5 + cx + r1*cos(a1));
			int y11 = (int)(0.5 + cy + r1*sin(a1));
			int x12 = (int)(0.5 + cx + r1*cos(a2));
			int y12 = (int)(0.5 + cy + r1*sin(a2));
			int x21 = (int)(0.5 + cx + r2*cos(a1));
			int y21 = (int)(0.5 + cy + r2*sin(a1));
			int x22 = (int)(0.5 + cx + r2*cos(a2));
			int y22 = (int)(0.5 + cy + r2*sin(a2));

			Arc(hDC, (int)(0.5 + cx - r1), (int)(0.5 + cy - r1), (int)(0.5 + cx + r1), (int)(0.5 + cy + r1), x11, y11, x12, y12);
			Arc(hDC, (int)(0.5 + cx - r2), (int)(0.5 + cy - r2), (int)(0.5 + cx + r2), (int)(0.5 + cy + r2), x21, y21, x22, y22);

			DrawLine(hDC, x11, y11, x21, y21);
			DrawLine(hDC, x12, y12, x22, y22);
		}
		void DrawRotatableSelector( HDC hDC, double cx, double cy, double radius )
		{
			Gdiplus::Graphics graphics( hDC );

			Gdiplus::Pen		borderPen ( Color( 255, 0, 0, 0 ), 2 );
			Gdiplus::SolidBrush	solidBrush( Color( 255, 0, 255, 100 ) );

			graphics.SetSmoothingMode( SmoothingModeHighQuality );
			graphics.DrawEllipse( &borderPen,  (Gdiplus::REAL)(cx - radius), (Gdiplus::REAL)(cy - radius), (Gdiplus::REAL)(2 * radius), (Gdiplus::REAL)(2 * radius) );
			graphics.FillEllipse( &solidBrush, (Gdiplus::REAL)(cx - radius), (Gdiplus::REAL)(cy - radius), (Gdiplus::REAL)(2 * radius), (Gdiplus::REAL)(2 * radius) );

		}
		void DrawCircleSector(HDC hDC, int left, int top, int right, int bottom, int x11, int y11, int x12, int y12, int x21, int y21, int x22, int y22)
		{
			double cx = 0.5*(left + right);
			double cy = 0.5*(top + bottom);

			double r1 = _hypot(x11 - cx, y11 - cy);
			double r2 = _hypot(x21 - cx, y21 - cy);

			Arc(hDC, (int)(0.5 + cx - r1), (int)(0.5 + cy - r1), (int)(0.5 + cx + r1), (int)(0.5 + cy + r1), x11, y11, x12, y12);
			Arc(hDC, (int)(0.5 + cx - r2), (int)(0.5 + cy - r2), (int)(0.5 + cx + r2), (int)(0.5 + cy + r2), x21, y21, x22, y22);

			DrawLine(hDC, x11, y11, x21, y21);
			DrawLine(hDC, x12, y12, x22, y22);
		}
		void DrawRectangle3D(HDC hDC, OLE_COLOR nColor1, OLE_COLOR nColor2, RECT rect)
		{
			// compute corner coordinates
			int x1 = rect.left, x2 = rect.right - 1;
			int y1 = rect.top, y2 = rect.bottom - 1;

			// show light lines
			FillLine(hDC, nColor1, x1, y1, x2, y1);
			FillLine(hDC, nColor1, x1, y1, x1, y2);

			// show dark lines
			FillLine(hDC, nColor2, x2, y1, x2, y2);
			FillLine(hDC, nColor2, x1, y2, x2, y2);
		}
		void DrawPolyline(HDC hDC, POINT* pPoints, int nCount, OLE_COLOR nColor1, OLE_COLOR nColor2, int nSize, int nStyle = PS_SOLID)
		{
			COLORREF color1; OleTranslateColor(nColor1, NULL, &color1);
			COLORREF color2; OleTranslateColor(nColor2, NULL, &color2);

			HPEN hPen = CreatePen(nStyle, nSize, color1);
			HBRUSH hBrush = CreateSolidBrush(color2);

			HGDIOBJ hOldPen = SelectObject(hDC, hPen);
			HGDIOBJ hOldBrush = SelectObject(hDC, hBrush);

			Polyline(hDC, pPoints, nCount);

			SelectObject(hDC, hOldPen);
			SelectObject(hDC, hOldBrush);

			DeleteObject(hPen);
			DeleteObject(hBrush);
		}
		void DrawRectangle(HDC hDC, RECT rect, OLE_COLOR nColor1, OLE_COLOR nColor2, int nSize, int nStyle = PS_SOLID)
		{
			POINT points[5];

			points[0].x = rect.left;
			points[0].y = rect.top;
			points[1].x = rect.right;
			points[1].y = rect.top;
			points[2].x = rect.right;
			points[2].y = rect.bottom;
			points[3].x = rect.left;
			points[3].y = rect.bottom;
			points[4].x = rect.left;
			points[4].y = rect.top;

			DrawPolyline(hDC, points, 5, nColor1, nColor2, nSize, nStyle);
		}

		void DrawTrajectoryPolyline(HDC hDC, POINT* pPoints, int nCount)
		{
			if( nCount < 2 || NULL == hDC ) return;
			Gdiplus::Graphics graphics( hDC );

			//int						endRange = 25;		//indentation from arrow
			Gdiplus::GraphicsPath	trajectoryPath;
			Gdiplus::Pen			trajectoryPen( Gdiplus::Color( 255, 0, 255, 0), 3 );
			Gdiplus::Pen			trajectoryBgPen( Gdiplus::Color( 255, 0, 0, 0), 3 );

			// set another (not standart) cap arrow
			Gdiplus::GraphicsPath    capArrowPath;
			capArrowPath.AddLine( Gdiplus::Point(  0, -1 ),	Gdiplus::Point( -2, -4 ) );
			capArrowPath.AddLine( Gdiplus::Point( -2, -4 ),	Gdiplus::Point(  2, -4 ) );
			capArrowPath.AddLine( Gdiplus::Point(  2, -4 ),	Gdiplus::Point(  0, -1 ) );
			Gdiplus::CustomLineCap   capArrow( &capArrowPath, NULL );

			trajectoryPen.SetStartCap( Gdiplus::LineCapRoundAnchor );
			//trajectoryPen.SetEndCap( Gdiplus::LineCapArrowAnchor );
			trajectoryPen.SetCustomEndCap( &capArrow );
			trajectoryPen.SetLineJoin( Gdiplus::LineJoinRound );
			trajectoryBgPen.SetCustomEndCap( &capArrow );
			trajectoryBgPen.SetLineJoin( Gdiplus::LineJoinRound );


			double oldStartX = pPoints[0].x;
			double oldStartY = pPoints[0].y;
			for( int i = 1; i < nCount; ++i )
			{
				trajectoryPath.AddLine ( (Gdiplus::REAL)oldStartX, (Gdiplus::REAL)oldStartY, (Gdiplus::REAL)pPoints[i].x, (Gdiplus::REAL)pPoints[i].y );
				oldStartX = pPoints[i].x;
				oldStartY = pPoints[i].y;
			}
			//	indentation
			//double lenght = Geometry::GetLength( pPoints[ nCount - 1 ].x, pPoints[ nCount - 1 ].y, pPoints[ nCount - 2 ].x, pPoints[ nCount - 2 ].y );
			//float additX = ( pPoints[ nCount - 1 ].x - pPoints[ nCount - 2 ].x ) * ( lenght + endRange ) / lenght + pPoints[ nCount - 2 ].x;
			//float additY = ( pPoints[ nCount - 1 ].y - pPoints[ nCount - 2 ].y ) * ( lenght + endRange ) / lenght + pPoints[ nCount - 2 ].y;
			//trajectoryPath.AddLine( oldStartX, oldStartY, additX, additY );

			graphics.SetSmoothingMode( Gdiplus::SmoothingModeHighQuality );
			graphics.TranslateTransform( 2.0f, 2.0f );
			graphics.DrawPath( &trajectoryBgPen, &trajectoryPath );
			graphics.ResetTransform();
			graphics.DrawPath( &trajectoryPen, &trajectoryPath );
		}
		void DrawTrajectoryPoint( Gdiplus::Graphics *graphics, Gdiplus::RectF &rect )
		{
			if( graphics )
			{
				Gdiplus::Pen		borderPen ( Color( 255, 0, 0, 0 ), 2 );
				Gdiplus::SolidBrush	solidBrush( Color( 255, 255, 106, 0 ) );

				graphics->SetSmoothingMode( Gdiplus::SmoothingModeHighQuality );
				graphics->DrawEllipse( &borderPen,  rect );
				graphics->FillEllipse( &solidBrush, rect );
			}
		}

		void DrawAngleScaleText( HDC hDC, CSimpleArray<CString> &lines, Gdiplus::Font& font, int FontColor, Gdiplus::StringFormat& stringFormat, double fontAngle, Structures::RECTD fontRect/*, Structures::POINTD scale*/ )
		{
			if( lines.GetSize() == 0 || NULL == hDC ) return;
			Graphics graphics( hDC );

			SolidBrush  solidBrush( Color( 255, GetRValue(FontColor), GetGValue(FontColor), GetBValue(FontColor) ) );

			double dWidth  = fontRect.GetWidth();
			double dHeight = fontRect.GetHeight();

			double dWidth_2	 = dWidth  / 2.0;
			double dHeight_2 = dHeight / 2.0;

			double centerX = fontRect.left + dWidth_2;
			double centerY = fontRect.top  + dHeight_2;

			RectF rectF, rectBB;
			RectF trackingRectF ( (Gdiplus::REAL)-dWidth_2, (Gdiplus::REAL)-dHeight_2, 
				(Gdiplus::REAL)fontRect.GetWidth(), (Gdiplus::REAL) fontRect.GetHeight() );

			Structures::POINTD scale( 1.0, 1.0 );

			double maxWidthStr  = 0.0;
			double maxHeightStr = 0.0;

			//stringFormat.SetFormatFlags( StringFormatFlags::StringFormatFlagsMeasureTrailingSpaces );

			for( int i = 0; i < lines.GetSize(); ++i )
			{
				BSTR line = lines[i].AllocSysString();
				graphics.MeasureString( line, -1, &font, rectF, &stringFormat, &rectBB ); 
				SysFreeString( line );
				maxWidthStr = max( maxWidthStr, rectBB.Width );
				maxHeightStr += rectBB.Height;				
			}

			if( maxWidthStr > 0 && maxHeightStr > 0 )
			{
				scale.x = fontRect.GetWidth() / maxWidthStr;
				scale.y = fontRect.GetHeight() / maxHeightStr;
			}

			GraphicsContainer  graphicsContainer;
			graphicsContainer = graphics.BeginContainer();
			graphics.TranslateTransform( (Gdiplus::REAL) centerX, (Gdiplus::REAL)centerY );
			graphics.RotateTransform ( (Gdiplus::REAL) grad ( fontAngle + M_PI_2 ) );
			graphics.ScaleTransform ( (Gdiplus::REAL)scale.x, (Gdiplus::REAL)scale.y );
			graphics.SetClip( trackingRectF );
			graphics.SetTextRenderingHint( TextRenderingHintAntiAlias );

			//because in ImagePaint3 scale text aligned by left only
			stringFormat.SetAlignment( StringAlignmentNear );

			for( int i = 0; i < lines.GetSize(); ++i )
			{
				BSTR line = lines[i].AllocSysString(); 
				graphics.MeasureString( line, -1, &font, rectF, &stringFormat, &rectBB ); 
				graphics.DrawString ( line, -1, &font, Gdiplus::PointF ( 
					(Gdiplus::REAL)-dWidth_2 / (Gdiplus::REAL)scale.x, 
					(Gdiplus::REAL)-dHeight_2 / (Gdiplus::REAL)scale.y + (Gdiplus::REAL)i * rectBB.Height ), &stringFormat, &solidBrush );
				SysFreeString( line );
			}
			graphics.EndContainer(graphicsContainer);
		}
		void DrawAngleText( HDC hDC, CSimpleArray<CString> &lines, Gdiplus::Font& font, int FontColor, Gdiplus::StringFormat& stringFormat, int fontAlignW, double fontAngle, Structures::RECTD fontRect )
		{
			if( 0 == lines.GetSize() || NULL == hDC ) return;
			Graphics graphics( hDC );

			SolidBrush  solidBrush( Color( 255, GetRValue(FontColor), GetGValue(FontColor), GetBValue(FontColor) ) );

			double dWidth  = fontRect.GetWidth();
			double dHeight = fontRect.GetHeight();

			double dWidth_2	 = dWidth  / 2.0;
			double dHeight_2 = dHeight / 2.0;

			double centerX = fontRect.left + dWidth_2;
			double centerY = fontRect.top  + dHeight_2;

			RectF rectF, rectBB;
			RectF trackingRectF( (Gdiplus::REAL)-dWidth_2, (Gdiplus::REAL)-dHeight_2, (Gdiplus::REAL)fontRect.GetWidth(), (Gdiplus::REAL)fontRect.GetHeight() );

			stringFormat.SetFormatFlags( StringFormatFlagsNoWrap );

			GraphicsContainer  graphicsContainer;
			graphicsContainer = graphics.BeginContainer();
			graphics.TranslateTransform ( (Gdiplus::REAL)centerX, (Gdiplus::REAL)centerY );
			graphics.RotateTransform( (Gdiplus::REAL) grad( fontAngle + M_PI_2 ) );
			graphics.SetClip( trackingRectF );
			graphics.SetTextRenderingHint( TextRenderingHintAntiAlias );

			double fontHeight = Rendering::Utils::GetFontHeight( &font );

			switch( fontAlignW )
			{
			case byTop: // FontAlign
				{
					for( int i = 0; i < lines.GetSize(); ++i )
					{
						// here was one more DrawString with displace on one pixel for shadow effect, but it's not seriously
						BSTR line = lines[i].AllocSysString();
						graphics.DrawString( line, -1, &font,
							Gdiplus::RectF( (Gdiplus::REAL)-dWidth_2, 
							(Gdiplus::REAL)-dHeight_2 + (Gdiplus::REAL)i * (Gdiplus::REAL)fontHeight, 
							(Gdiplus::REAL)dWidth, 
							(Gdiplus::REAL)dHeight - (Gdiplus::REAL)i * (Gdiplus::REAL)fontHeight ),
							&stringFormat, &solidBrush );

						SysFreeString( line );
					}
					break;
				}
			case byMiddle: // FontAlign
				{
					double displaceByY = lines.GetSize() * fontHeight / 2.0;
					for( int i = 0; i < lines.GetSize(); ++i )
					{
						BSTR line = lines[i].AllocSysString();
						graphics.DrawString( line, -1, &font, 
							Gdiplus::RectF( (Gdiplus::REAL)-dWidth_2,
							(Gdiplus::REAL)-displaceByY + (Gdiplus::REAL)i * (Gdiplus::REAL)fontHeight,
							(Gdiplus::REAL)dWidth,
							(Gdiplus::REAL)dHeight + (Gdiplus::REAL)i * (Gdiplus::REAL)fontHeight ), 
							&stringFormat, &solidBrush );

						SysFreeString( line );
					}
					break;		
				}
			case byBottom: // FontAlign
				{
					double displaceByY = lines.GetSize() * fontHeight;
					for( int i = 0; i < lines.GetSize(); ++i )
					{
						BSTR line = lines[i].AllocSysString();
						graphics.DrawString( line, -1, &font, 
							Gdiplus::RectF( (Gdiplus::REAL)-dWidth_2, 
							(Gdiplus::REAL)dHeight_2 -  (Gdiplus::REAL)displaceByY + (Gdiplus::REAL) i * (Gdiplus::REAL)fontHeight, 
							(Gdiplus::REAL)dWidth, (Gdiplus::REAL)dHeight + (Gdiplus::REAL) i * (Gdiplus::REAL)fontHeight ),
							&stringFormat, &solidBrush );
						SysFreeString( line );
					}
					break;		
				}
			default:
				{
					for( int i = 0; i < lines.GetSize(); ++i )
					{
						BSTR line = lines[i].AllocSysString();
						graphics.DrawString( line, -1, &font, Gdiplus::RectF( (Gdiplus::REAL)-dWidth_2, 
							(Gdiplus::REAL)-dHeight_2 + (Gdiplus::REAL)i * (Gdiplus::REAL)fontHeight, 
							(Gdiplus::REAL)dWidth, 
							(Gdiplus::REAL)dHeight - (Gdiplus::REAL)i * (Gdiplus::REAL)fontHeight ),
							&stringFormat, &solidBrush );
						SysFreeString( line );
					}
					break;	
				}
			}			
			graphics.EndContainer(graphicsContainer);
		}

		class CGdiPainter;

		void DrawScaleTextSelect( HDC hDC, CSimpleArray<CString> &lines, WCHAR* string, int selectStart, int selectEnd, Gdiplus::Font& font, Gdiplus::StringFormat &stringFormat, double fontAngle, Structures::RECTD trackingRect, Structures::POINTD scale )
		{
			if( selectStart == selectEnd || NULL == hDC ) return;

			struct Select
			{
				double  posStart;
				double  posEnd;
				int		strIndex;
			};

			if( selectStart > selectEnd )
			{
				int tmpSelect	= selectStart;	
				selectStart		= selectEnd;
				selectEnd		= tmpSelect;
			}

			Gdiplus::Graphics graphics( hDC );

			CSimpleArray<Select> selectLines;

			double centerX = trackingRect.left + ( trackingRect.right - trackingRect.left ) / 2.0;
			double centerY = trackingRect.top  + ( trackingRect.bottom - trackingRect.top ) / 2.0;

			double dWidth  = trackingRect.GetWidth();
			double dHeight = trackingRect.GetHeight();
			double dWidth_2  = dWidth  / 2.0;
			double dHeight_2 = dHeight / 2.0;

			Gdiplus::RectF	rectF, selectRectStart, selectRectEnd, selectRect;

			stringFormat.SetFormatFlags ( StringFormatFlagsMeasureTrailingSpaces );

			int		lineStart	= 0;
			int		idx			= 0;
			bool	stop		= false;
			Select	selectPos;
			while( selectEnd > 0 )
			{
				if( idx > lines.GetSize() - 1 || stop ) break;
				if( selectStart > lines[idx].GetLength() ) 
				{
					selectStart -= lines[idx].GetLength();
					selectEnd   -= lines[idx].GetLength();
					lineStart++;
					idx++;
					continue;
				}
				if( selectStart > 0 )
				{
					BSTR line = lines[idx].AllocSysString();
					graphics.MeasureString( line, selectStart, &font, rectF, &stringFormat, &selectRect );
					selectPos.posStart = selectRect.Width * scale.x;
					SysFreeString( line );
				}
				else
				{
					selectPos.posStart = font.GetSize() / 8.0 * scale.x;
				}

				if( selectEnd <= lines[idx].GetLength() )
				{
					BSTR line = lines[idx].AllocSysString();
					graphics.MeasureString( line, selectEnd, &font, rectF, &stringFormat, &selectRect );
					selectPos.posEnd = selectRect.Width * scale.x;
					stop = true;
					SysFreeString( line );
				}
				else
				{
					BSTR line = lines[idx].AllocSysString();
					graphics.MeasureString( line, -1, &font, rectF, &stringFormat, &selectRect );
					selectPos.posEnd = selectRect.Width * scale.x;
					selectEnd  -= lines[idx].GetLength();
					selectStart = 0;
					SysFreeString( line );
				}

				if( selectPos.posEnd > trackingRect.GetWidth() ) 
					selectPos.posEnd = trackingRect.GetWidth();

				selectPos.strIndex = idx;
				selectLines.Add( selectPos ); 
				idx++;
			}

			int countSpace = 1;
			for( int i = 0; i < lines.GetSize(); ++i )
			{
				BSTR line = lines[i].AllocSysString();
				WCHAR* currString = line;
				while( *currString++ )
				{
					if( *currString == '\n' ) countSpace++;
				}
				SysFreeString( line );
			}

			GraphicsContainer  graphicsContainer;
			graphicsContainer = graphics.BeginContainer();

			double fontHeight = Rendering::Utils::GetFontHeight( &font );
			fontHeight *= scale.y;

			LOGBRUSH brushSolid = {BS_SOLID, RGB(0,0,0), 0 };
			HPEN selectPen = ExtCreatePen ( PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_FLAT, (DWORD)fontHeight, &brushSolid, 0, NULL );

			graphics.TranslateTransform ( (Gdiplus::REAL)centerX, (Gdiplus::REAL)centerY );

			RectF trackingRectF( (Gdiplus::REAL)-dWidth_2, (Gdiplus::REAL)-dHeight_2, 
				(Gdiplus::REAL)trackingRect.GetWidth(), 
				(Gdiplus::REAL)trackingRect.GetHeight() );

			graphics.RotateTransform( (Gdiplus::REAL)grad( fontAngle + M_PI_2 ) );
			graphics.SetClip( trackingRectF );
			graphics.SetSmoothingMode ( SmoothingModeHighQuality );				

			for( int i = 0; i < selectLines.GetSize(); ++i )
			{
				graphics.MeasureString( string, 1, &font, rectF, &stringFormat, &selectRectStart ); 
				selectRectStart.Width  *= (Gdiplus::REAL)scale.x;
				selectRectStart.Height = (Gdiplus::REAL) ( fontHeight );

				double displaceByY = lines.GetSize() * selectRectStart.Height;

				Structures::POINTD startSelection( trackingRect.left + selectLines[i].posStart - font.GetSize() * scale.x / 8.0,  trackingRect.top + selectRectStart.Height / 2.0 + selectRectStart.Height * ( i + lineStart ) ) ;
				Structures::POINTD endSelection	 ( trackingRect.left + selectLines[i].posEnd - font.GetSize() * scale.x / 8.0,  trackingRect.top + selectRectStart.Height / 2.0 + selectRectStart.Height * ( i + lineStart ) );

				Geometry::RotatePoint( startSelection, Structures::POINTD( centerX, centerY ), fontAngle + M_PI_2 );
				Geometry::RotatePoint( endSelection, Structures::POINTD( centerX, centerY ), fontAngle + M_PI_2 );

				SelectObject( hDC, selectPen );
				int oldMode = SetROP2( hDC, R2_NOTXORPEN );
				MoveToEx( hDC, (int)startSelection.x, (int)startSelection.y , (LPPOINT) NULL );
				LineTo ( hDC, (int)endSelection.x, (int)endSelection.y );
				SetROP2( hDC, oldMode );
				DeleteObject( selectPen );
			}
			graphics.EndContainer(graphicsContainer);

			selectLines.RemoveAll();
		}
		void DrawTextSelect( HDC hDC, CSimpleArray<CString> &lines, WCHAR* string, int selectStart, int selectEnd, Gdiplus::Font& font, Gdiplus::StringFormat &stringFormat, int fontAlignW, double fontAngle, Structures::RECTD trackingRect )
		{
			if( selectStart == selectEnd || NULL == hDC ) return;

			struct Select
			{
				double  posStart;
				double  posEnd;
				int		strIndex;
			};

			if( selectStart > selectEnd )
			{
				int tmpSelect	= selectStart;	
				selectStart		= selectEnd;
				selectEnd		= tmpSelect;
			}

			Gdiplus::Graphics graphics( hDC );

			CSimpleArray<Select> selectLines;

			double centerX = trackingRect.left + ( trackingRect.right - trackingRect.left ) / 2.0;
			double centerY = trackingRect.top  + ( trackingRect.bottom - trackingRect.top ) / 2.0;

			double dWidth		= trackingRect.GetWidth();
			double dHeight		= trackingRect.GetHeight();
			double dWidth_2		= dWidth  / 2.0;
			double dHeight_2	= dHeight / 2.0;

			Gdiplus::RectF	rectF, selectRectStart, selectRectEnd, selectRect;

			if( dWidth < font.GetSize() / 2.0f )
				return;

			stringFormat.SetFormatFlags( StringFormatFlagsMeasureTrailingSpaces );

			double realHeight = Rendering::Utils::GetFontHeight( &font );

			int		lineStart	= 0;
			int		idx			= 0;
			bool	stop		= false;
			Select	selectPos;
			while( selectEnd > 0 )
			{
				if( idx > lines.GetSize() - 1 || stop ) break;
				if( selectStart > lines[idx].GetLength() ) 
				{
					selectStart -= lines[idx].GetLength();
					selectEnd   -= lines[idx].GetLength();
					lineStart++;
					idx++;
					continue;
				}
				if( selectStart > 0 )
				{
					BSTR line = lines[idx].AllocSysString();
					graphics.MeasureString( line, selectStart, &font, rectF, &stringFormat, &selectRect );
					selectPos.posStart = selectRect.Width;
					SysFreeString( line );
				}
				else
				{
					selectPos.posStart = font.GetSize() / 8.0;
				}

				if( selectEnd <= lines[idx].GetLength() )
				{
					BSTR line = lines[idx].AllocSysString();
					graphics.MeasureString( line, selectEnd, &font, rectF, &stringFormat, &selectRect );
					selectPos.posEnd = selectRect.Width;
					stop = true;
					SysFreeString( line );
				}
				else
				{
					BSTR line = lines[idx].AllocSysString();
					graphics.MeasureString( line, -1, &font, rectF, &stringFormat, &selectRect );
					selectPos.posEnd = selectRect.Width;
					selectEnd  -= lines[idx].GetLength();
					selectStart = 0;
					SysFreeString( line );
				}

				selectPos.strIndex = idx;
				selectLines.Add( selectPos ); 
				idx++;
			}

			int countSpace = 1;
			for( int i = 0; i < lines.GetSize(); ++i )
			{
				BSTR line = lines[i].AllocSysString();
				WCHAR* currString = line;
				while( *currString++ )
				{
					if( *currString == '\n' ) countSpace++;
				}
				SysFreeString( line );
			}

			GraphicsContainer  graphicsContainer;
			graphicsContainer = graphics.BeginContainer();
			graphics.TranslateTransform( (Gdiplus::REAL)centerX, (Gdiplus::REAL)centerY );
			RectF trackingRectF( (Gdiplus::REAL)-dWidth_2, (Gdiplus::REAL)-dHeight_2,
				(Gdiplus::REAL)trackingRect.GetWidth(), (Gdiplus::REAL)trackingRect.GetHeight() );

			LOGBRUSH brushSolid = {BS_SOLID, RGB(0,0,0), 0 };
			HPEN selectPen = ExtCreatePen( PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_FLAT, (DWORD)realHeight, &brushSolid, 0, NULL );

			graphics.RotateTransform ( (Gdiplus::REAL)grad( fontAngle + M_PI_2 ) );
			graphics.SetClip( trackingRectF );
			graphics.SetSmoothingMode( SmoothingModeHighQuality );

			for( int i = 0; i < selectLines.GetSize(); ++i )
			{
				if ( stringFormat.GetAlignment() == StringAlignmentCenter )
				{
					CString trimString = lines[ selectLines[i].strIndex ];
					trimString.TrimRight();
					BSTR str = trimString.AllocSysString();
					graphics.MeasureString( str, -1, &font, rectF, &stringFormat, &selectRect );
					selectLines[i].posStart += selectRect.X + dWidth_2;
					selectLines[i].posEnd	+= selectRect.X + dWidth_2;
					SysFreeString( str );
				}
				else if( stringFormat.GetAlignment() == StringAlignmentFar )
				{
					CString trimString = lines[ selectLines[i].strIndex ];
					trimString.TrimRight();
					BSTR str = trimString.AllocSysString();
					graphics.MeasureString( str, -1, &font, rectF, &stringFormat, &selectRect );
					selectLines[i].posStart += selectRect.X + dWidth;
					selectLines[i].posEnd	+= selectRect.X + dWidth;
					SysFreeString( str );
				}				

				double displaceByY = lines.GetSize() * realHeight - dHeight;

				Structures::POINTD startSelection;
				Structures::POINTD endSelection;

				switch( fontAlignW )
				{
				case byTop: // FontAlign
					{
						startSelection.x = trackingRect.left + selectLines[i].posStart - font.GetSize() / 8.0;
						startSelection.y = trackingRect.top  + realHeight / 2.0 + realHeight * (i+lineStart);
						endSelection.x	 = trackingRect.left + selectLines[i].posEnd - font.GetSize() / 8.0;
						endSelection.y	 = trackingRect.top  + realHeight / 2.0 + realHeight * (i+lineStart);
						//if( endSelection.y > trackingRect.bottom ) return;//old clamp
						break;
					}
				case byMiddle: // FontAlign
					{							
						startSelection.x = trackingRect.left + selectLines[i].posStart - font.GetSize() / 8.0;
						startSelection.y = trackingRect.top  + ( -displaceByY + realHeight ) / 2.0 + realHeight * (REAL)(i+lineStart);
						endSelection.x	 = trackingRect.left + selectLines[i].posEnd - font.GetSize() / 8.0;
						endSelection.y	 = trackingRect.top  + ( -displaceByY + realHeight ) / 2.0 + realHeight * (REAL)(i+lineStart);
						//if( endSelection.y < trackingRect.top )		continue;//old clamp
						//if( endSelection.y > trackingRect.bottom )	return;//old clamp
						break;
					}
				case byBottom: // FontAlign
					{
						startSelection.x = trackingRect.left + selectLines[i].posStart - font.GetSize() / 8.0;
						startSelection.y = trackingRect.top - displaceByY + realHeight / 2.0 + realHeight * (REAL)(i+lineStart);
						endSelection.x	 = trackingRect.left + selectLines[i].posEnd - font.GetSize() / 8.0;
						endSelection.y	 = trackingRect.top - displaceByY + realHeight / 2.0 + realHeight * (REAL)(i+lineStart);
						//if( endSelection.y < trackingRect.top )		continue;//old clamp
						break;
					}
				default:
					{
						startSelection.x = trackingRect.left + selectLines[i].posStart - font.GetSize() / 8.0;
						startSelection.y = trackingRect.top  + realHeight / 2.0 + realHeight * (i+lineStart);
						endSelection.x	 = trackingRect.left + selectLines[i].posEnd - font.GetSize() / 8.0;
						endSelection.y	 = trackingRect.top  + realHeight / 2.0 + realHeight * (i+lineStart);
						//if( endSelection.y > trackingRect.bottom ) return; //old clamp
						break;
					}
				}

				Structures::POINTD center = Structures::POINTD( centerX, centerY );
				double angle = fontAngle + M_PI_2;
				Geometry::RotatePoint( startSelection,	center, angle );
				Geometry::RotatePoint( endSelection,	center, angle );

				Structures::POINTD leftTop		( trackingRect.left,	trackingRect.top );
				Structures::POINTD rightTop		( trackingRect.right,	trackingRect.top );
				Structures::POINTD rightBottom	( trackingRect.right,	trackingRect.bottom );
				Structures::POINTD leftBottom	( trackingRect.left,	trackingRect.bottom );

				Geometry::RotatePoint( leftTop,		center, angle );
				Geometry::RotatePoint( rightTop,	center, angle );
				Geometry::RotatePoint( rightBottom, center, angle );
				Geometry::RotatePoint( leftBottom,	center, angle );

				POINT points[4];
				points[0].x = (long)leftTop.x;
				points[0].y = (long)leftTop.y;
				points[1].x = (long)rightTop.x;
				points[1].y = (long)rightTop.y;
				points[2].x = (long)rightBottom.x;
				points[2].y = (long)rightBottom.y;
				points[3].x = (long)leftBottom.x;
				points[3].y = (long)leftBottom.y;

				HRGN clipRegn = CreatePolygonRgn( points, 4, ALTERNATE );
				SelectClipRgn( hDC, clipRegn );

				SelectObject( hDC, selectPen );
				int oldMode = SetROP2( hDC, R2_NOTXORPEN );
				MoveToEx( hDC, (int)startSelection.x, (int)startSelection.y , (LPPOINT) NULL );
				LineTo( hDC, (int)endSelection.x, (int)endSelection.y );
				SetROP2( hDC, oldMode );
				DeleteObject( selectPen );
				DeleteObject( clipRegn );
				SelectClipRgn( hDC, NULL );
			}
			graphics.EndContainer(graphicsContainer);

			selectLines.RemoveAll();
		}
		void DrawScaleCaret( HDC hDC, CSimpleArray<CString> &lines, Gdiplus::StringFormat &stringFormat, int posCaret, Gdiplus::Font &font, double fontAngle, Structures::RECTD trackingRect, Structures::POINTD scale )
		{
			if( NULL == hDC ) return;
			Gdiplus::Graphics graphics( hDC );

			RectF	orig, strBB, chBB, caretRect, allStringRect;
			double	displaceByY		= 0.0;
			double	displaceByX		= 0.0;

			double	centerX			= trackingRect.left + ( trackingRect.right - trackingRect.left ) / 2.0;
			double	centerY			= trackingRect.top  + ( trackingRect.bottom - trackingRect.top ) / 2.0;

			double	dWidth			= trackingRect.GetWidth();
			double	dHeight			= trackingRect.GetHeight();
			double	dWidth_2		= dWidth  / 2.0;
			double	dHeight_2		= dHeight / 2.0;

			bool	bEmtyString		= false;
			bool	isLastString	= false;

			Gdiplus::RectF	rectF, fontRect;
			Gdiplus::Pen	fillCaretColor	 ( Gdiplus::Color( 255,   0,   0,   0 ), 2.0 );
			Gdiplus::Pen	borderCaretColor ( Gdiplus::Color( 255, 255, 255, 255 ), 4.0 );

			int		internCaretPos	= posCaret;
			int		index = 0;
			double	fontHeight		= Rendering::Utils::GetFontHeight( &font );
			double	fontHeightScale	= fontHeight * scale.y;

			if( 0 == lines.GetSize() ) bEmtyString = true;

			if( !bEmtyString )
			{
				for( index = 0; index < lines.GetSize(); index++ )
				{
					if( internCaretPos <= lines[index].GetLength() ) break;
					internCaretPos -= lines[index].GetLength();
				}

				if( ( internCaretPos == lines[index].GetLength() || internCaretPos == lines[index].GetLength() - 1 ) 
					&& (index + 1) < lines.GetSize() 
					&& lines[index][lines[index].GetLength() - 1] == '\n' ) 
				{
					internCaretPos = 0;
					index++;
				}
				else if( ( internCaretPos == lines[index].GetLength() || internCaretPos == lines[index].GetLength() - 1 ) 
					&& (index + 1) == lines.GetSize() 
					&& lines[index][lines[index].GetLength() - 1] == '\n' )
				{
					isLastString = true;	
					internCaretPos = 0;
				}

				stringFormat.SetFormatFlags ( StringFormatFlagsMeasureTrailingSpaces );

				CString trimString = lines[index];
				trimString.TrimRight();

				BSTR str = trimString.AllocSysString();
				graphics.MeasureString( str, -1, &font, orig, &stringFormat, &allStringRect );
				SysFreeString( str );

				allStringRect.Width *= (Gdiplus::REAL)scale.x;
				allStringRect.Height = (Gdiplus::REAL) ( fontHeightScale );

				if( internCaretPos == 0 )
				{
					if( 0 == allStringRect.Height )
					{
						BSTR line = lines[0].AllocSysString();
						graphics.MeasureString( line, -1, &font, orig, &stringFormat, &allStringRect );	
						SysFreeString( line );
						allStringRect.Width *= (Gdiplus::REAL)scale.x;
						allStringRect.Height = (Gdiplus::REAL) ( fontHeightScale );
					}

					caretRect = RectF ( 0.0f, 0.0f, (Gdiplus::REAL)font.GetSize() / 5.0f * (Gdiplus::REAL)scale.x, (Gdiplus::REAL)allStringRect.Height );
				}
				else
				{
					BSTR str = trimString.AllocSysString();
					graphics.MeasureString( str, internCaretPos, &font, orig, &stringFormat, &caretRect );
					SysFreeString( str );
					caretRect.Width *= (Gdiplus::REAL) scale.x;
					caretRect.Height = (Gdiplus::REAL) ( fontHeightScale );
				}
			}
			else
			{
				caretRect.X		 = 0;
				caretRect.Y		 = 0;
				caretRect.Width  = 5;
				caretRect.Height = (Gdiplus::REAL) ( trackingRect.GetHeight() );
			}

			GraphicsContainer  graphicsContainer;
			graphicsContainer = graphics.BeginContainer();
			graphics.TranslateTransform ( (Gdiplus::REAL)centerX, (Gdiplus::REAL)centerY );
			RectF trackingRectF( (Gdiplus::REAL)-dWidth_2, (Gdiplus::REAL)-dHeight_2,
				(Gdiplus::REAL)trackingRect.GetWidth(), (Gdiplus::REAL)trackingRect.GetHeight() );
			caretRect.Width += (Gdiplus::REAL) ( -dWidth_2 );

			displaceByY = lines.GetSize() * fontHeightScale;

			graphics.RotateTransform ( (Gdiplus::REAL)grad( fontAngle + M_PI_2 ) );
			graphics.SetClip ( trackingRectF );
			if( caretRect.Y + caretRect.Height <= dHeight )
			{
				caretRect.Y	= (Gdiplus::REAL) ( -dHeight_2 + ( isLastString ? index + 1 : index ) * fontHeightScale );

				graphics.DrawLine ( &borderCaretColor,
					(Gdiplus::REAL) caretRect.Width - (Gdiplus::REAL)font.GetSize() * (Gdiplus::REAL)scale.x / 8.0f,
					(Gdiplus::REAL)caretRect.Y - 1.0f, 
					(Gdiplus::REAL)caretRect.Width - (Gdiplus::REAL)font.GetSize() * (Gdiplus::REAL)scale.x / 8.0f,
					(Gdiplus::REAL)caretRect.Y + (Gdiplus::REAL)caretRect.Height + 1.0f );

				graphics.DrawLine ( &fillCaretColor, 
					(Gdiplus::REAL) caretRect.Width - (Gdiplus::REAL)font.GetSize() * (Gdiplus::REAL)scale.x / 8.0f,
					(Gdiplus::REAL)caretRect.Y,
					(Gdiplus::REAL)caretRect.Width - (Gdiplus::REAL)font.GetSize() * (Gdiplus::REAL)scale.x / 8.0f,
					(Gdiplus::REAL)caretRect.Y + (Gdiplus::REAL)caretRect.Height );
			}
			graphics.EndContainer( graphicsContainer );
		}
		void DrawCaret( HDC hDC, CSimpleArray<CString> &lines, Gdiplus::StringFormat &stringFormat, int fontAlignW, int posCaret, Gdiplus::Font &font, double fontAngle, Structures::RECTD trackingRect )
		{
			if( NULL == hDC ) return;
			Gdiplus::Graphics graphics( hDC );

			RectF	orig, strBB, chBB, caretRect, allStringRect;
			double	displaceByY		= 0.0;
			double	displaceByX		= 0.0;

			double	centerX			= trackingRect.left + ( trackingRect.right - trackingRect.left ) / 2.0;
			double	centerY			= trackingRect.top  + ( trackingRect.bottom - trackingRect.top ) / 2.0;

			double	dWidth			= trackingRect.GetWidth();
			double	dHeight			= trackingRect.GetHeight();
			double	dWidth_2		= dWidth  / 2.0;
			double	dHeight_2		= dHeight / 2.0;

			bool	bEmtyString		= false;
			bool	isLastString	= false;

			//if( dWidth < font.GetSize() + font.GetSize() / 8.0 ) return;

			if( 0 == lines.GetSize() ) bEmtyString = true;

			Gdiplus::RectF	rectF, fontRect;
			Gdiplus::Pen	fillCaretColor	 ( Gdiplus::Color( 255,   0,   0,   0 ), 2.0 );
			Gdiplus::Pen	borderCaretColor ( Gdiplus::Color( 255, 255, 255, 255 ), 4.0 );

			int		internCaretPos	= posCaret;
			int		index			= 0;
			double	fontHeight		= Rendering::Utils::GetFontHeight( &font );

			if( !bEmtyString )
			{
				for( index = 0; index < lines.GetSize(); index++ )
				{
					if( internCaretPos <= lines[index].GetLength() ) break;
					internCaretPos -= lines[index].GetLength();
				}

				if( ( internCaretPos == lines[index].GetLength() || internCaretPos == lines[index].GetLength() - 1 ) 
					&& (index + 1) < lines.GetSize() 
					&& lines[index][lines[index].GetLength() - 1] == '\n' ) 
				{
					internCaretPos = 0;
					index++;
				}
				else if( ( internCaretPos == lines[index].GetLength() || internCaretPos == lines[index].GetLength() - 1 ) 
					&& (index + 1) == lines.GetSize() 
					&& lines[index][lines[index].GetLength() - 1] == '\n' )
				{
					isLastString = true;	
					internCaretPos = 0;
				}

				stringFormat.SetFormatFlags( StringFormatFlagsMeasureTrailingSpaces );

				//char buff[32];
				//sprintf( buff, "line num %d caret pos %d\n", index, internCaretPos );
				//::OutputDebugStringA( buff );

				CString trimString = lines[index];
				trimString.TrimRight();

				BSTR str = trimString.AllocSysString();
				graphics.MeasureString( str, -1, &font, orig, &stringFormat, &allStringRect );
				SysFreeString( str );

				if( internCaretPos == 0 )
				{
					if( 0 == fontHeight )
					{
						BSTR line = lines[0].AllocSysString();
						graphics.MeasureString( line, -1, &font, orig, &stringFormat, &allStringRect );	
						SysFreeString( line );
					}

					caretRect = RectF ( 0.0f, 0.0f, font.GetSize() / 5.0f, (Gdiplus::REAL)fontHeight );
				}
				else
				{
					BSTR line = trimString.AllocSysString();
					graphics.MeasureString( str, internCaretPos, &font, orig, &stringFormat, &caretRect );
					caretRect.Height = (Gdiplus::REAL) ( fontHeight );
					SysFreeString( str );
				}
			}
			else
			{
				caretRect.X		 = 0;
				caretRect.Y		 = 0;
				caretRect.Width  = 5;
				caretRect.Height = (Gdiplus::REAL) ( fontHeight );
			}


			GraphicsContainer  graphicsContainer;
			graphicsContainer = graphics.BeginContainer();
			graphics.TranslateTransform ( (Gdiplus::REAL)centerX, (Gdiplus::REAL)centerY );
			RectF trackingRectF( (Gdiplus::REAL)-dWidth_2, (Gdiplus::REAL)-dHeight_2, 
				(Gdiplus::REAL)trackingRect.GetWidth(), (Gdiplus::REAL)trackingRect.GetHeight() );
			if( stringFormat.GetAlignment() == StringAlignmentNear )
				caretRect.Width += (Gdiplus::REAL) ( -dWidth_2 );
			else if ( stringFormat.GetAlignment() == StringAlignmentCenter )
			{
				caretRect.Offset ( ( caretRect.Width - allStringRect.Width ) / 2.0f, 0.0f );
				if( !bEmtyString ) caretRect.Width += ( lines[index][0] == '\r' ) ? 0 : caretRect.X;
				if( isLastString ) caretRect.Width = font.GetSize() / 5;
			}
			else if( stringFormat.GetAlignment() == StringAlignmentFar )
			{
				if( bEmtyString )
					caretRect.Width += (Gdiplus::REAL) ( dWidth_2 - 5.0 );
				else
				{
					caretRect.Offset( caretRect.Width - allStringRect.Width, 0.0 );
					caretRect.Width += ( lines[index][0] == '\r' ) ? 
						(Gdiplus::REAL) ( dWidth_2 - font.GetSize() / 5 ) :
					(Gdiplus::REAL) ( caretRect.X + dWidth_2 );
				}

				if( isLastString ) 
					caretRect.Width = (Gdiplus::REAL) ( dWidth_2 );
			}

			displaceByY = lines.GetSize() * fontHeight;

			graphics.RotateTransform ( (Gdiplus::REAL)grad( fontAngle + M_PI_2 ) );
			graphics.SetClip( trackingRectF );

			switch( fontAlignW )
			{
			case byTop:
				{
					caretRect.Y	= (Gdiplus::REAL) ( -dHeight_2 + ( isLastString ? index + 1 : index ) * fontHeight );
					break;
				}
			case byMiddle:
				{	
					caretRect.Y	= (Gdiplus::REAL) ( -displaceByY / 2.0 + ( isLastString ? index + 1 : index ) * fontHeight );
					if( bEmtyString ) caretRect.Y -= (Gdiplus::REAL) ( fontHeight / 2.0 );
					break;
				}
			case byBottom:
				{
					caretRect.Y	= (Gdiplus::REAL) ( dHeight_2 - displaceByY + ( isLastString ? index + 1 : index ) * fontHeight );
					if( bEmtyString ) caretRect.Y -= (Gdiplus::REAL) ( fontHeight );
					break;
				}
			default:
				{
					caretRect.Y	= (Gdiplus::REAL) ( -dHeight_2 + ( isLastString ? index + 1 : index ) * fontHeight );
				}
			}
			graphics.DrawLine( &borderCaretColor, caretRect.Width - font.GetSize() / 8, caretRect.Y - 1, caretRect.Width - font.GetSize() / 8, caretRect.Y + caretRect.Height + 1);
			graphics.DrawLine( &fillCaretColor,   caretRect.Width - font.GetSize() / 8, caretRect.Y, caretRect.Width - font.GetSize() / 8, caretRect.Y + caretRect.Height );

			graphics.EndContainer( graphicsContainer );
		}

		class CGdiObject
		{
		protected:

			HDC m_hDC;

			int m_nBkColor;
			int m_nOldBkColor;

			int m_nROP;
			int m_nOldROP;

			HPEN m_hPen;
			HGDIOBJ m_hOldPen;

			HBRUSH m_hBrush;
			HGDIOBJ m_hOldBrush;

		public:

			CGdiObject()
			{
				m_hDC = 0;
				m_nROP = -1;
				m_nOldROP = -1;
				m_nBkColor = -1;
				m_nOldBkColor = -1;
				m_hPen = 0;
				m_hOldPen = 0;
				m_hBrush = 0;
				m_hOldBrush = 0;
			}
			~CGdiObject()
			{
				m_hDC = 0;

				Destroy();
			}

			BOOL Valid()
			{
				if (m_hDC)
					return TRUE;

				return FALSE;
			}
			void Create(HDC hDC)
			{
				Destroy();

				if (!hDC)
					return;

				m_hDC = hDC;
			}
			virtual void Destroy()
			{
				if (m_hDC)
				{
					if (m_hPen)
						SelectObject(m_hDC, m_hOldPen);
					if (m_hBrush)
						SelectObject(m_hDC, m_hOldBrush);
					if (m_nROP >= 0)
						SetROP2(m_hDC, m_nOldROP);
					if (m_nBkColor >= 0)
						SetBkColor(m_hDC, m_nOldBkColor);
				}

				if (m_hPen)
					DeleteObject(m_hPen);
				if (m_hBrush)
					DeleteObject(m_hBrush);

				m_hDC = 0;
				m_nROP = -1;
				m_nOldROP = -1;
				m_nBkColor = -1;
				m_nOldBkColor = -1;
				m_hPen = 0;
				m_hOldPen = 0;
				m_hBrush = 0;
				m_hOldBrush = 0;
			}
		};
		class CGdiPen : public CGdiObject
		{
		public:

			// nStyle - PS_SOLID, PS_DOT, ... 
			// nSize -  size of the pen in pixels (valid only for PS_SOLID)
			// nColor1, nColor2 - color of the pen: valid COLORREF or -1 for invisible or -2 for xor painting
			void Create(HDC hDC, int nColor1, int nColor2, int nSize = 1, int nStyle = PS_SOLID)
			{
				Destroy();

				if (!hDC)
					return;

				m_hDC = hDC;

				HPEN hPen = 0;
				HBRUSH hBrush = 0;

				// select ROP mode
				if (TRUE)
				{
					if (nColor1 == -2 || nColor2 == -2)
						m_nOldROP = SetROP2(hDC, R2_NOTXORPEN);
					else
						m_nOldROP = SetROP2(hDC, R2_COPYPEN);
				}

				// create pen
				if (TRUE)
				{
					if (nColor1 == -2 || nColor1 >= 0)
					{
						m_hPen = CreatePen(nStyle, nSize, max(0, nColor1));
						hPen = m_hPen;
					}
					else // if (nCOlor == -1)
					{
						hPen = (HPEN)GetStockObject(NULL_PEN);
					}
				}

				// create brush
				if (TRUE)
				{
					if (nColor2 == -2)
						hBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
					else if (nColor2 == -1)
						hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
					else // if (nColor2 > 0)
					{
						m_hBrush = CreateSolidBrush(nColor2);
						m_nBkColor = nColor2;
						hBrush = m_hBrush;
					}
				}

				// select create objects
				if (TRUE)
				{
					if (hPen)
						m_hOldPen = SelectObject(hDC, hPen);
					if (hBrush)
						m_hOldBrush = SelectObject(hDC, hBrush);
					if (m_nBkColor >= 0)
						m_nOldBkColor = SetBkColor(hDC, m_nBkColor);
				}
			}
		};
		class CGdiPainter
		{
		protected:

			HDC m_hDC;

			CGdiPen m_oPen;

		public:

			CGdiPainter()
			{
				m_hDC = 0;
			}
			~CGdiPainter()
			{
				Destroy();
			}

			void Create(HDC hDC)
			{
				m_hDC = hDC;
			}
			void Destroy()
			{
				m_oPen.Destroy();
			}
			void SetPen(int nColor1, int nColor2, int nSize = 1, int nStyle = PS_SOLID)
			{
				m_oPen.Create(m_hDC, nColor1, nColor2, nSize, nStyle);
			}
		};
	}

	namespace Xml
	{
		class CXmlPainter
		{
		protected:

			GdiPlusEx::CGdiPlusImage* m_pImage;
			GdiPlusEx::CGdiPlusGraphics m_oGraphics;

		protected:

			BOOL BeginPaint()
			{
				EndPaint();

				if (!IsValid())
					return FALSE;

				return m_oGraphics.CreateFromImage(m_pImage);
			}
			void EndPaint()
			{
				m_oGraphics.Destroy();
			}

			void ApplyFormat(Painter::CFormat& oFormat)
			{
				Gdiplus::Graphics* pGraphics = (Gdiplus::Graphics*)m_oGraphics;
				if (!pGraphics)
					return;

				if (oFormat.AntiAliasPen)
					pGraphics->SetSmoothingMode(SmoothingModeAntiAlias);
				else
					pGraphics->SetSmoothingMode(SmoothingModeNone);

				if (oFormat.AntiAliasText)
					pGraphics->SetTextRenderingHint(TextRenderingHintAntiAlias);
				else
					pGraphics->SetTextRenderingHint(TextRenderingHintSystemDefault);
			}
			void PaintLine(Painter::CPen& oPen, Painter::CLine& oLine)
			{
				Gdiplus::Graphics* pGraphics = (Gdiplus::Graphics*)m_oGraphics;
				if (!pGraphics)
					return;

				Gdiplus::Pen* pPen = oPen.GetPen();
				if (!pPen)
					return;

				pGraphics->DrawLine(pPen, (float)oLine.X1, (float)oLine.Y1, (float)oLine.X2, (float)oLine.Y2);
			}
			void PaintRectangle(Painter::CPen& oPen, Painter::CBrush& oBrush, Painter::CRectangle& oRectangle)
			{
				Gdiplus::Graphics* pGraphics = (Gdiplus::Graphics*)m_oGraphics;
				if (!pGraphics)
					return;

				Gdiplus::Brush* pBrush = oBrush.GetBrush();
				if (pBrush)
					pGraphics->FillRectangle(pBrush, (float)oRectangle.Left, (float)oRectangle.Top, (float)oRectangle.Width, (float)oRectangle.Height);

				Gdiplus::Pen* pPen = oPen.GetPen();
				if (pPen)
					pGraphics->DrawRectangle(pPen, (float)oRectangle.Left, (float)oRectangle.Top, (float)oRectangle.Width, (float)oRectangle.Height);
			}
			void PaintEllipse(Painter::CPen& oPen, Painter::CBrush& oBrush, Painter::CEllipse& oEllipse)
			{
				Gdiplus::Graphics* pGraphics = (Gdiplus::Graphics*)m_oGraphics;
				if (!pGraphics)
					return;

				Gdiplus::Brush* pBrush = oBrush.GetBrush();
				if (pBrush)
					pGraphics->FillEllipse(pBrush, (float)oEllipse.Left, (float)oEllipse.Top, (float)oEllipse.Width, (float)oEllipse.Height);

				Gdiplus::Pen* pPen = oPen.GetPen();
				if (pPen)
					pGraphics->DrawEllipse(pPen, (float)oEllipse.Left, (float)oEllipse.Top, (float)oEllipse.Width, (float)oEllipse.Height);
			}
			void PaintText(Painter::CBrush& oBrush, Painter::CFont& oFont, Painter::CFormat& oFormat, Painter::CText& oText)
			{
				Gdiplus::Graphics* pGraphics = (Gdiplus::Graphics*)m_oGraphics;
				if (!pGraphics)
					return;

				Gdiplus::Brush* pBrush = oBrush.GetBrush();
				Gdiplus::Font* pFont = oFont.GetFont();
				if (!pBrush || !pFont)
					return;

				Gdiplus::StringFormat oStringAlignment;
				Gdiplus::StringFormat* pStringAlignment = oFormat.GetStringFormat();
				if (!pStringAlignment)
				{
					oStringAlignment.SetLineAlignment(Gdiplus::StringAlignmentCenter);
					oStringAlignment.SetAlignment(Gdiplus::StringAlignmentCenter);
					pStringAlignment = &oStringAlignment;
				}

				Gdiplus::RectF oRect((float)oText.Left, (float)oText.Top, (float)oText.Width, (float)oText.Height);

				BSTR bstrText = oText.Text.AllocSysString();
				pGraphics->DrawString(bstrText, oText.Text.GetLength(), pFont, oRect, pStringAlignment, pBrush);
				SysFreeString(bstrText);
			}

			void TransformCrop(Painter::CBrush* pBrushClear, Painter::CTransformCrop& oTransform)
			{
				Gdiplus::Graphics* pGraphics = (Gdiplus::Graphics*)m_oGraphics;
				if (!pGraphics)
					return;

				Gdiplus::Bitmap* pBitmapDst = (Gdiplus::Bitmap*)(*m_pImage);
				Gdiplus::Bitmap* pBitmapSrc = pBitmapDst->Clone(0, 0, pBitmapDst->GetWidth(), pBitmapDst->GetHeight(), PixelFormat32bppRGB);

				if (oTransform.ClearBackground)
				{
					Gdiplus::Brush* pBrush = pBrushClear->GetBrush();
					if (pBrush)
						pGraphics->FillRectangle(pBrush, 0, 0, pBitmapDst->GetWidth(), pBitmapDst->GetHeight());
				}
				pGraphics->DrawImage(pBitmapSrc, (float)oTransform.Left, (float)oTransform.Top, (float)oTransform.Width, (float)oTransform.Height);
			}

		public:

			CXmlPainter()
			{
				m_pImage = NULL;
			}
			~CXmlPainter()
			{
			}

			BOOL IsValid()
			{
				if (m_pImage && m_pImage->IsValid())
					return TRUE;

				return FALSE;
			}
			BOOL IsPainting()
			{
				return m_oGraphics.IsValid();
			}

			void SetImage(GdiPlusEx::CGdiPlusImage* pImage)
			{
				m_pImage = pImage;
			}
			GdiPlusEx::CGdiPlusImage* GetImage()
			{
				return m_pImage;
			}

			void PaintXml(CString strXml)
			{
				XmlUtils::CXmlReader oReader;

				if (!oReader.SetXmlString(strXml))
					return;

				int nLayerIndex = 0;

				if (!BeginPaint())
					return;

				// read all layers from the file and paint them
				while (true)
				{
					CString strLayerIndex; strLayerIndex.Format(_T("layer%d"), nLayerIndex);

					oReader.ReadRootNode();
					if (!oReader.ReadNode(strLayerIndex))
						break;

					CString strXml;
					Painter::CPen oPen;
					Painter::CBrush oBrush;
					Painter::CFont oFont;
					Painter::CFormat oFormat;

					// read painting settings
					if (true)
					{
						oReader.ReadRootNode();
						oReader.ReadNode(strLayerIndex);
						if (oReader.ReadNode(_T("format")))
						{
							strXml = oReader.ReadNodeXml();
							oFormat.FromXmlString(strXml);
							ApplyFormat(oFormat);
						}

						oReader.ReadRootNode();
						oReader.ReadNode(strLayerIndex);
						if (oReader.ReadNode(_T("pen")))
						{
							strXml = oReader.ReadNodeXml();
							oPen.FromXmlString(strXml);
						}

						oReader.ReadRootNode();
						oReader.ReadNode(strLayerIndex);
						if (oReader.ReadNode(_T("brush")))
						{
							strXml = oReader.ReadNodeXml();
							oBrush.FromXmlString(strXml);
						}

						oReader.ReadRootNode();
						oReader.ReadNode(strLayerIndex);
						if (oReader.ReadNode(_T("font")))
						{
							strXml = oReader.ReadNodeXml();
							oFont.FromXmlString(strXml);
						}
					}

					// read graphics primitives
					if (true)
					{
						oReader.ReadRootNode();
						oReader.ReadNode(strLayerIndex);
						if (oReader.ReadNode(_T("line")))
						{
							strXml = oReader.ReadNodeXml();

							Painter::CLine oLine;
							oLine.FromXmlString(strXml);
							PaintLine(oPen, oLine);
						}

						oReader.ReadRootNode();
						oReader.ReadNode(strLayerIndex);
						if (oReader.ReadNode(_T("rectangle")))
						{
							strXml = oReader.ReadNodeXml();

							Painter::CRectangle oRectangle;
							oRectangle.FromXmlString(strXml);
							PaintRectangle(oPen, oBrush, oRectangle);
						}

						oReader.ReadRootNode();
						oReader.ReadNode(strLayerIndex);
						if (oReader.ReadNode(_T("ellipse")))
						{
							strXml = oReader.ReadNodeXml();

							Painter::CEllipse oEllipse;
							oEllipse.FromXmlString(strXml);
							PaintEllipse(oPen, oBrush, oEllipse);
						}

						oReader.ReadRootNode();
						oReader.ReadNode(strLayerIndex);
						if (oReader.ReadNode(_T("text")))
						{
							strXml = oReader.ReadNodeXml();

							Painter::CText oText;
							oText.FromXmlString(strXml);
							PaintText(oBrush, oFont, oFormat, oText);
						}
					}

					// read and apply transforms
					if (true)
					{
						oReader.ReadRootNode();
						oReader.ReadNode(strLayerIndex);
						if (oReader.ReadNode(_T("transform-crop")))
						{
							strXml = oReader.ReadNodeXml();

							Painter::CTransformCrop oTransform;
							oTransform.FromXmlString(strXml);
							TransformCrop(&oBrush, oTransform);
						}
					}

					nLayerIndex++;
				}

				EndPaint();
			}
		};
	}

	namespace TextRendering
	{
		inline CStringW	NormNonBreakingSpaces ( CStringW Input, bool InsertSpace = 0 )
		{
			if ( false == InsertSpace )
				return Input;

			if ( Input.GetLength() > 3 )
			{
				if ( Input.GetAt ( Input.GetLength() - 1 ) == L' ' )
				{
					Input.Insert ( Input.GetLength() - 1, L'\xA0' );		// Неразрывный пробел

					return Input;
				}

				if ( Input.GetAt ( Input.GetLength() - 3 ) == L' ' )
				{
					Input.Insert ( Input.GetLength() - 3, L'\xA0' );		// Неразрывный пробел
				}
			}

			return Input;
		}

		inline void DrawAngleScaleTextW ( HDC hDC, CSimpleArray<CStringW> &lines, Gdiplus::Font& font, int FontColor, Gdiplus::StringFormat& stringFormat, double fontAngle, Structures::RECTD fontRect/*, Structures::POINTD scale*/ )
		{
			if( lines.GetSize() == 0 || NULL == hDC ) return;
			Graphics graphics( hDC );

			SolidBrush  solidBrush( Color( 255, GetRValue(FontColor), GetGValue(FontColor), GetBValue(FontColor) ) );

			double dWidth		=	fontRect.GetWidth();
			double dHeight		=	fontRect.GetHeight();

			double dWidth_2		=	dWidth  / 2.0;
			double dHeight_2	=	dHeight / 2.0;

			double centerX		=	fontRect.left + dWidth_2;
			double centerY		=	fontRect.top  + dHeight_2;

			RectF rectF, rectBB;
			RectF trackingRectF ( (Gdiplus::REAL)-dWidth_2, (Gdiplus::REAL)-dHeight_2, 
				(Gdiplus::REAL)fontRect.GetWidth(), (Gdiplus::REAL) fontRect.GetHeight() );

			Structures::POINTD scale ( 1.0, 1.0 );

			double maxWidthStr	=	0.0;
			double maxHeightStr	=	0.0;

			//stringFormat.SetFormatFlags( StringFormatFlags::StringFormatFlagsMeasureTrailingSpaces );

			for( int i = 0; i < lines.GetSize(); ++i )
			{
				//BSTR line = lines[i].AllocSysString();
				graphics.MeasureString ( NormNonBreakingSpaces ( lines[i] ), -1, &font, rectF, &stringFormat, &rectBB ); 
				//SysFreeString( line );
				maxWidthStr = max( maxWidthStr, rectBB.Width );
				maxHeightStr += rectBB.Height;				
			}

			if ( maxWidthStr > 0 && maxHeightStr > 0 )
			{
				scale.x = fontRect.GetWidth() / maxWidthStr;
				scale.y = fontRect.GetHeight() / maxHeightStr;
			}

			GraphicsContainer  graphicsContainer;
			graphicsContainer = graphics.BeginContainer();
			graphics.TranslateTransform( (Gdiplus::REAL) centerX, (Gdiplus::REAL)centerY );
			graphics.RotateTransform ( (Gdiplus::REAL) grad ( fontAngle + M_PI_2 ) );
			graphics.ScaleTransform ( (Gdiplus::REAL)scale.x, (Gdiplus::REAL)scale.y );
			graphics.SetClip( trackingRectF );
			graphics.SetTextRenderingHint( TextRenderingHintAntiAlias );

			//because in ImagePaint3 scale text aligned by left only
			stringFormat.SetAlignment( StringAlignmentNear );

			for( int i = 0; i < lines.GetSize(); ++i )
			{
				//BSTR line = lines[i].AllocSysString(); 
				graphics.MeasureString ( NormNonBreakingSpaces ( lines[i] ), -1, &font, rectF, &stringFormat, &rectBB ); 
				graphics.DrawString ( NormNonBreakingSpaces ( lines[i] ), -1, &font, Gdiplus::PointF ( 
					(Gdiplus::REAL)-dWidth_2 / (Gdiplus::REAL)scale.x, 
					(Gdiplus::REAL)-dHeight_2 / (Gdiplus::REAL)scale.y + (Gdiplus::REAL)i * rectBB.Height ), &stringFormat, &solidBrush );
				//SysFreeString( line );
			}
			graphics.EndContainer(graphicsContainer);
		}
		inline void DrawAngleTextW ( HDC hDC, CSimpleArray<CStringW> &lines, Gdiplus::Font& font, int FontColor, Gdiplus::StringFormat& stringFormat, int fontAlignW, double fontAngle, Structures::RECTD fontRect )
		{
			if ( 0 == lines.GetSize() || NULL == hDC )
				return;

			Graphics graphics( hDC );

			SolidBrush  solidBrush( Color( 255, GetRValue(FontColor), GetGValue(FontColor), GetBValue(FontColor) ) );

			double dWidth  = fontRect.GetWidth();
			double dHeight = fontRect.GetHeight();

			double dWidth_2	 = dWidth  / 2.0;
			double dHeight_2 = dHeight / 2.0;

			double centerX = fontRect.left + dWidth_2;
			double centerY = fontRect.top  + dHeight_2;

			RectF rectF, rectBB;
			RectF trackingRectF( (Gdiplus::REAL)-dWidth_2, (Gdiplus::REAL)-dHeight_2, (Gdiplus::REAL)fontRect.GetWidth(), (Gdiplus::REAL)fontRect.GetHeight() );

			stringFormat.SetFormatFlags( StringFormatFlagsNoWrap );

			GraphicsContainer  graphicsContainer;
			graphicsContainer = graphics.BeginContainer();
			graphics.TranslateTransform ( (Gdiplus::REAL)centerX, (Gdiplus::REAL)centerY );
			graphics.RotateTransform( (Gdiplus::REAL) grad( fontAngle + M_PI_2 ) );
			graphics.SetClip( trackingRectF );
			graphics.SetTextRenderingHint( TextRenderingHintAntiAlias );

			double fontHeight = Rendering::Utils::GetFontHeight( &font );
			
			switch( fontAlignW )
			{
			case byTop: // FontAlign
				{
					for( int i = 0; i < lines.GetSize(); ++i )
					{
						//BSTR line = lines[i].AllocSysString();
						graphics.DrawString ( NormNonBreakingSpaces ( lines[i] ), -1, &font,
							Gdiplus::RectF( (Gdiplus::REAL)-dWidth_2, 
							(Gdiplus::REAL)-dHeight_2 + (Gdiplus::REAL)i * (Gdiplus::REAL)fontHeight, 
							(Gdiplus::REAL)dWidth, 
							(Gdiplus::REAL)dHeight - (Gdiplus::REAL)i * (Gdiplus::REAL)fontHeight ),
							 &stringFormat, &solidBrush );

						//SysFreeString( line );
					}
					break;
				}
			case byMiddle: // FontAlign
				{
					double displaceByY = lines.GetSize() * fontHeight / 2.0;
					for( int i = 0; i < lines.GetSize(); ++i )
					{
						//BSTR line = lines[i].AllocSysString();
						graphics.DrawString( NormNonBreakingSpaces ( lines[i] ), -1, &font, 
							Gdiplus::RectF( (Gdiplus::REAL)-dWidth_2,
							(Gdiplus::REAL)-displaceByY + (Gdiplus::REAL)i * (Gdiplus::REAL)fontHeight,
							(Gdiplus::REAL)dWidth,
							(Gdiplus::REAL)dHeight + (Gdiplus::REAL)i * (Gdiplus::REAL)fontHeight ), 
							 &stringFormat, &solidBrush );

						//SysFreeString( line );
					}
					break;		
				}
			case byBottom: // FontAlign
				{
					double displaceByY = lines.GetSize() * fontHeight;
					for( int i = 0; i < lines.GetSize(); ++i )
					{
						//BSTR line = lines[i].AllocSysString();
						graphics.DrawString( NormNonBreakingSpaces ( lines[i] ), -1, &font, 
							Gdiplus::RectF( (Gdiplus::REAL)-dWidth_2, 
							(Gdiplus::REAL)dHeight_2 -  (Gdiplus::REAL)displaceByY + (Gdiplus::REAL) i * (Gdiplus::REAL)fontHeight, 
							(Gdiplus::REAL)dWidth, (Gdiplus::REAL)dHeight + (Gdiplus::REAL) i * (Gdiplus::REAL)fontHeight ),
							 &stringFormat, &solidBrush );
						//SysFreeString( line );
					}
					break;		
				}
			default:
				{
					for( int i = 0; i < lines.GetSize(); ++i )
					{
						//BSTR line = lines[i].AllocSysString();
						graphics.DrawString( NormNonBreakingSpaces ( lines[i] ), -1, &font, Gdiplus::RectF( (Gdiplus::REAL)-dWidth_2, 
							(Gdiplus::REAL)-dHeight_2 + (Gdiplus::REAL)i * (Gdiplus::REAL)fontHeight, 
							(Gdiplus::REAL)dWidth, 
							(Gdiplus::REAL)dHeight - (Gdiplus::REAL)i * (Gdiplus::REAL)fontHeight ),
							&stringFormat, &solidBrush );
						//SysFreeString( line );
					}
					break;	
				}
			}			
			graphics.EndContainer(graphicsContainer);
		}
		inline void DrawScaleTextSelectW ( HDC hDC, CSimpleArray<CStringW> &lines, WCHAR* string, int selectStart, int selectEnd, Gdiplus::Font& font, Gdiplus::StringFormat &stringFormat, double fontAngle, Structures::RECTD trackingRect, Structures::POINTD scale )
		{
			if( selectStart == selectEnd || NULL == hDC ) return;

			struct Select
			{
				double  posStart;
				double  posEnd;
				int		strIndex;
			};

			if( selectStart > selectEnd )
			{
				int tmpSelect	= selectStart;	
				selectStart		= selectEnd;
				selectEnd		= tmpSelect;
			}

			Gdiplus::Graphics graphics( hDC );

			CSimpleArray<Select> selectLines;

			double centerX = trackingRect.left + ( trackingRect.right - trackingRect.left ) / 2.0;
			double centerY = trackingRect.top  + ( trackingRect.bottom - trackingRect.top ) / 2.0;

			double dWidth  = trackingRect.GetWidth();
			double dHeight = trackingRect.GetHeight();
			double dWidth_2  = dWidth  / 2.0;
			double dHeight_2 = dHeight / 2.0;

			Gdiplus::RectF	rectF, selectRectStart, selectRectEnd, selectRect;

			stringFormat.SetFormatFlags ( StringFormatFlagsMeasureTrailingSpaces );

			int		lineStart	= 0;
			int		idx			= 0;
			bool	stop		= false;
			Select	selectPos;
			while( selectEnd > 0 )
			{
				if( idx > lines.GetSize() - 1 || stop ) break;
				if( selectStart > lines[idx].GetLength() ) 
				{
					selectStart -= lines[idx].GetLength();
					selectEnd   -= lines[idx].GetLength();
					lineStart++;
					idx++;
					continue;
				}
				if( selectStart > 0 )
				{
					//BSTR line = lines[idx].AllocSysString();
					graphics.MeasureString( lines[idx], selectStart, &font, rectF, &stringFormat, &selectRect );
					selectPos.posStart = selectRect.Width * scale.x;
					//SysFreeString( line );
				}
				else
				{
					selectPos.posStart = font.GetSize() / 8.0 * scale.x;
				}

				if( selectEnd <= lines[idx].GetLength() )
				{
					//BSTR line = lines[idx].AllocSysString();
					graphics.MeasureString( lines[idx], selectEnd, &font, rectF, &stringFormat, &selectRect );
					selectPos.posEnd = selectRect.Width * scale.x;
					stop = true;
					//SysFreeString( line );
				}
				else
				{
					//BSTR line = lines[idx].AllocSysString();
					graphics.MeasureString( lines[idx], -1, &font, rectF, &stringFormat, &selectRect );
					selectPos.posEnd = selectRect.Width * scale.x;
					selectEnd  -= lines[idx].GetLength();
					selectStart = 0;
					//SysFreeString( line );
				}

				if( selectPos.posEnd > trackingRect.GetWidth() ) 
					selectPos.posEnd = trackingRect.GetWidth();

				selectPos.strIndex = idx;
				selectLines.Add( selectPos ); 
				idx++;
			}

			int countSpace = 1;
			for( int i = 0; i < lines.GetSize(); ++i )
			{
				BSTR line = lines[i].AllocSysString();
				WCHAR* currString = line;
				while( *currString++ )
				{
					if( *currString == L'\n' ) countSpace++;
				}
				SysFreeString( line );
			}

			GraphicsContainer  graphicsContainer;
			graphicsContainer = graphics.BeginContainer();

			double fontHeight = Rendering::Utils::GetFontHeight( &font );
			fontHeight *= scale.y;

			LOGBRUSH brushSolid = {BS_SOLID, RGB(0,0,0), 0 };
			HPEN selectPen = ExtCreatePen ( PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_FLAT, (DWORD)fontHeight, &brushSolid, 0, NULL );

			graphics.TranslateTransform ( (Gdiplus::REAL)centerX, (Gdiplus::REAL)centerY );

			RectF trackingRectF( (Gdiplus::REAL)-dWidth_2, (Gdiplus::REAL)-dHeight_2, 
				(Gdiplus::REAL)trackingRect.GetWidth(), 
				(Gdiplus::REAL)trackingRect.GetHeight() );

			graphics.RotateTransform( (Gdiplus::REAL)grad( fontAngle + M_PI_2 ) );
			graphics.SetClip( trackingRectF );
			graphics.SetSmoothingMode ( SmoothingModeHighQuality );				

			for( int i = 0; i < selectLines.GetSize(); ++i )
			{
				graphics.MeasureString( string, 1, &font, rectF, &stringFormat, &selectRectStart ); 
				selectRectStart.Width  *= (Gdiplus::REAL)scale.x;
				selectRectStart.Height = (Gdiplus::REAL) ( fontHeight );

				double displaceByY = lines.GetSize() * selectRectStart.Height;

				Structures::POINTD startSelection( trackingRect.left + selectLines[i].posStart - font.GetSize() * scale.x / 8.0,  trackingRect.top + selectRectStart.Height / 2.0 + selectRectStart.Height * ( i + lineStart ) ) ;
				Structures::POINTD endSelection	 ( trackingRect.left + selectLines[i].posEnd - font.GetSize() * scale.x / 8.0,  trackingRect.top + selectRectStart.Height / 2.0 + selectRectStart.Height * ( i + lineStart ) );

				Geometry::RotatePoint( startSelection, Structures::POINTD( centerX, centerY ), fontAngle + M_PI_2 );
				Geometry::RotatePoint( endSelection, Structures::POINTD( centerX, centerY ), fontAngle + M_PI_2 );

				SelectObject( hDC, selectPen );
				int oldMode = SetROP2( hDC, R2_NOTXORPEN );
				MoveToEx( hDC, (int)startSelection.x, (int)startSelection.y , (LPPOINT) NULL );
				LineTo ( hDC, (int)endSelection.x, (int)endSelection.y );
				SetROP2( hDC, oldMode );
				DeleteObject( selectPen );
			}
			graphics.EndContainer(graphicsContainer);

			selectLines.RemoveAll();
		}
		inline void DrawScaleCaretW ( HDC hDC, CSimpleArray<CStringW> &lines, Gdiplus::StringFormat &stringFormat, int posCaret, Gdiplus::Font &font, double fontAngle, Structures::RECTD trackingRect, Structures::POINTD scale )
		{
			if( NULL == hDC ) return;
			Gdiplus::Graphics graphics( hDC );

			RectF	orig, strBB, chBB, caretRect, allStringRect;
			double	displaceByY		= 0.0;
			double	displaceByX		= 0.0;

			double	centerX			= trackingRect.left + ( trackingRect.right - trackingRect.left ) / 2.0;
			double	centerY			= trackingRect.top  + ( trackingRect.bottom - trackingRect.top ) / 2.0;

			double	dWidth			= trackingRect.GetWidth();
			double	dHeight			= trackingRect.GetHeight();
			double	dWidth_2		= dWidth  / 2.0;
			double	dHeight_2		= dHeight / 2.0;

			bool	bEmtyString		= false;
			bool	isLastString	= false;

			Gdiplus::RectF	rectF, fontRect;
			Gdiplus::Pen	fillCaretColor	 ( Gdiplus::Color( 255,   0,   0,   0 ), 2.0 );
			Gdiplus::Pen	borderCaretColor ( Gdiplus::Color( 255, 255, 255, 255 ), 4.0 );

			int		internCaretPos	= posCaret;
			int		index = 0;
			double	fontHeight		= Rendering::Utils::GetFontHeight( &font );
			double	fontHeightScale	= fontHeight * scale.y;

			if( 0 == lines.GetSize() ) bEmtyString = true;

			if( !bEmtyString )
			{
				for( index = 0; index < lines.GetSize(); index++ )
				{
					if( internCaretPos <= lines[index].GetLength() ) break;
					internCaretPos -= lines[index].GetLength();
				}

				if( ( internCaretPos == lines[index].GetLength() || internCaretPos == lines[index].GetLength() - 1 ) 
					&& (index + 1) < lines.GetSize() 
					&& lines[index][lines[index].GetLength() - 1] == '\n' ) 
				{
					internCaretPos = 0;
					index++;
				}
				else if( ( internCaretPos == lines[index].GetLength() || internCaretPos == lines[index].GetLength() - 1 ) 
					&& (index + 1) == lines.GetSize() 
					&& lines[index][lines[index].GetLength() - 1] == '\n' )
				{
					isLastString = true;	
					internCaretPos = 0;
				}

				stringFormat.SetFormatFlags ( StringFormatFlagsMeasureTrailingSpaces );

				CStringW trimString = lines[index];
				trimString.TrimRight();

				//BSTR str = trimString.AllocSysString();
				graphics.MeasureString( trimString, -1, &font, orig, &stringFormat, &allStringRect );
				//SysFreeString( str );

				allStringRect.Width *= (Gdiplus::REAL)scale.x;
				allStringRect.Height = (Gdiplus::REAL) ( fontHeightScale );

				if( internCaretPos == 0 )
				{
					if( 0 == allStringRect.Height )
					{
						//BSTR line = lines[0].AllocSysString();
						graphics.MeasureString( lines[0], -1, &font, orig, &stringFormat, &allStringRect );	
						//SysFreeString( line );
						allStringRect.Width *= (Gdiplus::REAL)scale.x;
						allStringRect.Height = (Gdiplus::REAL) ( fontHeightScale );
					}

					caretRect = RectF ( 0.0f, 0.0f, (Gdiplus::REAL)font.GetSize() / 5.0f * (Gdiplus::REAL)scale.x, (Gdiplus::REAL)allStringRect.Height );
				}
				else
				{
					//BSTR str = trimString.AllocSysString();
					graphics.MeasureString( trimString, internCaretPos, &font, orig, &stringFormat, &caretRect );
					//SysFreeString( str );
					caretRect.Width *= (Gdiplus::REAL) scale.x;
					caretRect.Height = (Gdiplus::REAL) ( fontHeightScale );
				}
			}
			else
			{
				caretRect.X		 = 0;
				caretRect.Y		 = 0;
				caretRect.Width  = 5;
				caretRect.Height = (Gdiplus::REAL) ( trackingRect.GetHeight() );
			}

			GraphicsContainer  graphicsContainer;
			graphicsContainer = graphics.BeginContainer();
			graphics.TranslateTransform ( (Gdiplus::REAL)centerX, (Gdiplus::REAL)centerY );
			RectF trackingRectF( (Gdiplus::REAL)-dWidth_2, (Gdiplus::REAL)-dHeight_2,
				(Gdiplus::REAL)trackingRect.GetWidth(), (Gdiplus::REAL)trackingRect.GetHeight() );
			caretRect.Width += (Gdiplus::REAL) ( -dWidth_2 );

			displaceByY = lines.GetSize() * fontHeightScale;

			graphics.RotateTransform ( (Gdiplus::REAL)grad( fontAngle + M_PI_2 ) );
			graphics.SetClip ( trackingRectF );
			if( caretRect.Y + caretRect.Height <= dHeight )
			{
				caretRect.Y	= (Gdiplus::REAL) ( -dHeight_2 + ( isLastString ? index + 1 : index ) * fontHeightScale );

				graphics.DrawLine ( &borderCaretColor,
					(Gdiplus::REAL) caretRect.Width - (Gdiplus::REAL)font.GetSize() * (Gdiplus::REAL)scale.x / 8.0f,
					(Gdiplus::REAL)caretRect.Y - 1.0f, 
					(Gdiplus::REAL)caretRect.Width - (Gdiplus::REAL)font.GetSize() * (Gdiplus::REAL)scale.x / 8.0f,
					(Gdiplus::REAL)caretRect.Y + (Gdiplus::REAL)caretRect.Height + 1.0f );

				graphics.DrawLine ( &fillCaretColor, 
					(Gdiplus::REAL) caretRect.Width - (Gdiplus::REAL)font.GetSize() * (Gdiplus::REAL)scale.x / 8.0f,
					(Gdiplus::REAL)caretRect.Y,
					(Gdiplus::REAL)caretRect.Width - (Gdiplus::REAL)font.GetSize() * (Gdiplus::REAL)scale.x / 8.0f,
					(Gdiplus::REAL)caretRect.Y + (Gdiplus::REAL)caretRect.Height );
			}
			graphics.EndContainer( graphicsContainer );
		}
		inline void DrawTextSelectW ( HDC hDC, CSimpleArray<CStringW> &lines, WCHAR* string, int selectStart, int selectEnd, Gdiplus::Font& font, Gdiplus::StringFormat &stringFormat, int fontAlignW, double fontAngle, Structures::RECTD trackingRect )
		{
			if( selectStart == selectEnd || NULL == hDC ) return;

			struct Select
			{
				double  posStart;
				double  posEnd;
				int		strIndex;
			};

			if( selectStart > selectEnd )
			{
				int tmpSelect	= selectStart;	
				selectStart		= selectEnd;
				selectEnd		= tmpSelect;
			}

			Gdiplus::Graphics graphics( hDC );

			CSimpleArray<Select> selectLines;

			double centerX = trackingRect.left + ( trackingRect.right - trackingRect.left ) / 2.0;
			double centerY = trackingRect.top  + ( trackingRect.bottom - trackingRect.top ) / 2.0;

			double dWidth		= trackingRect.GetWidth();
			double dHeight		= trackingRect.GetHeight();
			double dWidth_2		= dWidth  / 2.0;
			double dHeight_2	= dHeight / 2.0;

			Gdiplus::RectF	rectF, selectRectStart, selectRectEnd, selectRect;

			if( dWidth < font.GetSize() / 2.0f )
				return;

			stringFormat.SetFormatFlags( StringFormatFlagsMeasureTrailingSpaces );

			double realHeight = Rendering::Utils::GetFontHeight( &font );

			int		lineStart	= 0;
			int		idx			= 0;
			bool	stop		= false;
			Select	selectPos;
			while( selectEnd > 0 )
			{
				if( idx > lines.GetSize() - 1 || stop ) break;
				if( selectStart > lines[idx].GetLength() ) 
				{
					selectStart -= lines[idx].GetLength();
					selectEnd   -= lines[idx].GetLength();
					lineStart++;
					idx++;
					continue;
				}
				if( selectStart > 0 )
				{
					//BSTR line = lines[idx].AllocSysString();
					graphics.MeasureString( lines[idx], selectStart, &font, rectF, &stringFormat, &selectRect );
					selectPos.posStart = selectRect.Width;
					//SysFreeString( line );
				}
				else
				{
					selectPos.posStart = font.GetSize() / 8.0;
				}

				if( selectEnd <= lines[idx].GetLength() )
				{
					//BSTR line = lines[idx].AllocSysString();
					graphics.MeasureString( lines[idx], selectEnd, &font, rectF, &stringFormat, &selectRect );
					selectPos.posEnd = selectRect.Width;
					stop = true;
					//SysFreeString( line );
				}
				else
				{
					//BSTR line = lines[idx].AllocSysString();
					graphics.MeasureString( lines[idx], -1, &font, rectF, &stringFormat, &selectRect );
					selectPos.posEnd = selectRect.Width;
					selectEnd  -= lines[idx].GetLength();
					selectStart = 0;
					//SysFreeString( line );
				}

				selectPos.strIndex = idx;
				selectLines.Add( selectPos ); 
				idx++;
			}

			int countSpace = 1;
			for( int i = 0; i < lines.GetSize(); ++i )
			{
				BSTR line = lines[i].AllocSysString();
				WCHAR* currString = line;
				while( *currString++ )
				{
					if( *currString == '\n' ) countSpace++;
				}
				SysFreeString( line );
			}

			GraphicsContainer  graphicsContainer;
			graphicsContainer = graphics.BeginContainer();
			graphics.TranslateTransform( (Gdiplus::REAL)centerX, (Gdiplus::REAL)centerY );
			RectF trackingRectF( (Gdiplus::REAL)-dWidth_2, (Gdiplus::REAL)-dHeight_2,
				(Gdiplus::REAL)trackingRect.GetWidth(), (Gdiplus::REAL)trackingRect.GetHeight() );

			LOGBRUSH brushSolid = {BS_SOLID, RGB(0,0,0), 0 };
			HPEN selectPen = ExtCreatePen( PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_FLAT, (DWORD)realHeight, &brushSolid, 0, NULL );

			graphics.RotateTransform ( (Gdiplus::REAL)grad( fontAngle + M_PI_2 ) );
			graphics.SetClip( trackingRectF );
			graphics.SetSmoothingMode( SmoothingModeHighQuality );

			for( int i = 0; i < selectLines.GetSize(); ++i )
			{
				if ( stringFormat.GetAlignment() == StringAlignmentCenter )
				{
					CStringW trimString = lines [ selectLines[i].strIndex ];
					trimString.TrimRight();
					//BSTR str = trimString.AllocSysString();
					graphics.MeasureString( trimString, -1, &font, rectF, &stringFormat, &selectRect );
					selectLines[i].posStart += selectRect.X + dWidth_2;
					selectLines[i].posEnd	+= selectRect.X + dWidth_2;
					//SysFreeString( str );
				}
				else if( stringFormat.GetAlignment() == StringAlignmentFar )
				{
					CStringW trimString = lines [ selectLines[i].strIndex ];
					trimString.TrimRight();
					//BSTR str = trimString.AllocSysString();
					graphics.MeasureString( trimString, -1, &font, rectF, &stringFormat, &selectRect );
					selectLines[i].posStart += selectRect.X + dWidth;
					selectLines[i].posEnd	+= selectRect.X + dWidth;
					//SysFreeString( str );
				}				

				double displaceByY = lines.GetSize() * realHeight - dHeight;

				Structures::POINTD startSelection;
				Structures::POINTD endSelection;

				switch( fontAlignW )
				{
				case byTop: // FontAlign
					{
						startSelection.x = trackingRect.left + selectLines[i].posStart - font.GetSize() / 8.0;
						startSelection.y = trackingRect.top  + realHeight / 2.0 + realHeight * (i+lineStart);
						endSelection.x	 = trackingRect.left + selectLines[i].posEnd - font.GetSize() / 8.0;
						endSelection.y	 = trackingRect.top  + realHeight / 2.0 + realHeight * (i+lineStart);
						//if( endSelection.y > trackingRect.bottom ) return;//old clamp
						break;
					}
				case byMiddle: // FontAlign
					{							
						startSelection.x = trackingRect.left + selectLines[i].posStart - font.GetSize() / 8.0;
						startSelection.y = trackingRect.top  + ( -displaceByY + realHeight ) / 2.0 + realHeight * (REAL)(i+lineStart);
						endSelection.x	 = trackingRect.left + selectLines[i].posEnd - font.GetSize() / 8.0;
						endSelection.y	 = trackingRect.top  + ( -displaceByY + realHeight ) / 2.0 + realHeight * (REAL)(i+lineStart);
						//if( endSelection.y < trackingRect.top )		continue;//old clamp
						//if( endSelection.y > trackingRect.bottom )	return;//old clamp
						break;
					}
				case byBottom: // FontAlign
					{
						startSelection.x = trackingRect.left + selectLines[i].posStart - font.GetSize() / 8.0;
						startSelection.y = trackingRect.top - displaceByY + realHeight / 2.0 + realHeight * (REAL)(i+lineStart);
						endSelection.x	 = trackingRect.left + selectLines[i].posEnd - font.GetSize() / 8.0;
						endSelection.y	 = trackingRect.top - displaceByY + realHeight / 2.0 + realHeight * (REAL)(i+lineStart);
						//if( endSelection.y < trackingRect.top )		continue;//old clamp
						break;
					}
				default:
					{
						startSelection.x = trackingRect.left + selectLines[i].posStart - font.GetSize() / 8.0;
						startSelection.y = trackingRect.top  + realHeight / 2.0 + realHeight * (i+lineStart);
						endSelection.x	 = trackingRect.left + selectLines[i].posEnd - font.GetSize() / 8.0;
						endSelection.y	 = trackingRect.top  + realHeight / 2.0 + realHeight * (i+lineStart);
						//if( endSelection.y > trackingRect.bottom ) return; //old clamp
						break;
					}
				}

				Structures::POINTD center = Structures::POINTD( centerX, centerY );
				double angle = fontAngle + M_PI_2;
				Geometry::RotatePoint( startSelection,	center, angle );
				Geometry::RotatePoint( endSelection,	center, angle );

				Structures::POINTD leftTop		( trackingRect.left,	trackingRect.top );
				Structures::POINTD rightTop		( trackingRect.right,	trackingRect.top );
				Structures::POINTD rightBottom	( trackingRect.right,	trackingRect.bottom );
				Structures::POINTD leftBottom	( trackingRect.left,	trackingRect.bottom );

				Geometry::RotatePoint( leftTop,		center, angle );
				Geometry::RotatePoint( rightTop,	center, angle );
				Geometry::RotatePoint( rightBottom, center, angle );
				Geometry::RotatePoint( leftBottom,	center, angle );

				POINT points[4];
				points[0].x = (long)leftTop.x;
				points[0].y = (long)leftTop.y;
				points[1].x = (long)rightTop.x;
				points[1].y = (long)rightTop.y;
				points[2].x = (long)rightBottom.x;
				points[2].y = (long)rightBottom.y;
				points[3].x = (long)leftBottom.x;
				points[3].y = (long)leftBottom.y;

				HRGN clipRegn = CreatePolygonRgn( points, 4, ALTERNATE );
				SelectClipRgn( hDC, clipRegn );

				SelectObject( hDC, selectPen );
				int oldMode = SetROP2( hDC, R2_NOTXORPEN );
				MoveToEx( hDC, (int)startSelection.x, (int)startSelection.y , (LPPOINT) NULL );
				LineTo( hDC, (int)endSelection.x, (int)endSelection.y );
				SetROP2( hDC, oldMode );
				DeleteObject( selectPen );
				DeleteObject( clipRegn );
				SelectClipRgn( hDC, NULL );
			}
			graphics.EndContainer(graphicsContainer);

			selectLines.RemoveAll();
		}
		inline void DrawCaretW ( HDC hDC, CSimpleArray<CStringW> &lines, Gdiplus::StringFormat &stringFormat, int fontAlignW, int posCaret, Gdiplus::Font &font, double fontAngle, Structures::RECTD trackingRect )
		{
			if ( NULL == hDC ) 
				return;
			

			Gdiplus::Graphics graphics( hDC );

			RectF	orig, strBB, chBB, caretRect, allStringRect;
			double	displaceByY		= 0.0;
			double	displaceByX		= 0.0;

			double	centerX			= trackingRect.left + ( trackingRect.right - trackingRect.left ) / 2.0;
			double	centerY			= trackingRect.top  + ( trackingRect.bottom - trackingRect.top ) / 2.0;

			double	dWidth			= trackingRect.GetWidth();
			double	dHeight			= trackingRect.GetHeight();
			double	dWidth_2		= dWidth  / 2.0;
			double	dHeight_2		= dHeight / 2.0;

			bool	bEmtyString		= false;
			bool	isLastString	= false;

			//if( dWidth < font.GetSize() + font.GetSize() / 8.0 ) return;

			if( 0 == lines.GetSize() ) bEmtyString = true;

			Gdiplus::RectF	rectF, fontRect;
			Gdiplus::Pen	fillCaretColor	 ( Gdiplus::Color( 255,   0,   0,   0 ), 2.0 );
			Gdiplus::Pen	borderCaretColor ( Gdiplus::Color( 255, 255, 255, 255 ), 4.0 );

			int		internCaretPos	= posCaret;
			int		index			= 0;
			double	fontHeight		= Rendering::Utils::GetFontHeight( &font );

			if( !bEmtyString )
			{
				for( index = 0; index < lines.GetSize(); index++ )
				{
					if( internCaretPos <= lines[index].GetLength() ) break;
					internCaretPos -= lines[index].GetLength();
				}

				if( ( internCaretPos == lines[index].GetLength() || internCaretPos == lines[index].GetLength() - 1 ) 
					&& (index + 1) < lines.GetSize() 
					&& lines[index][lines[index].GetLength() - 1] == '\n' ) 
				{
					internCaretPos = 0;
					index++;
				}
				else if( ( internCaretPos == lines[index].GetLength() || internCaretPos == lines[index].GetLength() - 1 ) 
					&& (index + 1) == lines.GetSize() 
					&& lines[index][lines[index].GetLength() - 1] == '\n' )
				{
					isLastString = true;	
					internCaretPos = 0;
				}

				stringFormat.SetFormatFlags( StringFormatFlagsMeasureTrailingSpaces );

				//char buff[32];
				//sprintf( buff, "line num %d caret pos %d\n", index, internCaretPos );
				//::OutputDebugStringA( buff );
			
				CStringW trimString = lines[index];
				//trimString.TrimRight();
				trimString.Delete ( trimString.GetLength () - 2, 2 );
				//!!!!!!!!!!!
				//BSTR str = trimString.AllocSysString();
				graphics.MeasureString ( trimString, -1, &font, orig, &stringFormat, &allStringRect );
				//SysFreeString( str );

				if ( internCaretPos == 0 )
				{
					if ( 0 == fontHeight )
					{
						//BSTR line = lines[0].AllocSysString();
						graphics.MeasureString( NormNonBreakingSpaces ( lines[0] ), -1, &font, orig, &stringFormat, &allStringRect );	
						//SysFreeString( line );
					}

					caretRect = RectF ( 0.0f, 0.0f, font.GetSize() / 5.0f, (Gdiplus::REAL)fontHeight );
				}
				else
				{
					//BSTR line = trimString.AllocSysString();
					graphics.MeasureString ( trimString, internCaretPos, &font, orig, &stringFormat, &caretRect );
					caretRect.Height = (Gdiplus::REAL) ( fontHeight );
					//SysFreeString( str );
				}
			}
			else
			{
				caretRect.X		 = 0;
				caretRect.Y		 = 0;
				caretRect.Width  = 5;
				caretRect.Height = (Gdiplus::REAL) ( fontHeight );
			}

			GraphicsContainer  graphicsContainer;
			graphicsContainer = graphics.BeginContainer();
			graphics.TranslateTransform ( (Gdiplus::REAL)centerX, (Gdiplus::REAL)centerY );
			RectF trackingRectF( (Gdiplus::REAL)-dWidth_2, (Gdiplus::REAL)-dHeight_2, 
				(Gdiplus::REAL)trackingRect.GetWidth(), (Gdiplus::REAL)trackingRect.GetHeight() );
			
			if ( stringFormat.GetAlignment() == StringAlignmentNear )
			{
				caretRect.Width += (Gdiplus::REAL) ( -dWidth_2 );
			}
			else if ( stringFormat.GetAlignment() == StringAlignmentCenter )
			{
				caretRect.Offset ( ( caretRect.Width - allStringRect.Width ) / 2.0f, 0.0f );
				if( !bEmtyString ) caretRect.Width += ( lines[index][0] == '\r' ) ? 0 : caretRect.X;
				if( isLastString ) caretRect.Width = font.GetSize() / 5;
			}
			else if( stringFormat.GetAlignment() == StringAlignmentFar )
			{
				if( bEmtyString )
					caretRect.Width += (Gdiplus::REAL) ( dWidth_2 - 5.0 );
				else
				{
					caretRect.Offset( caretRect.Width - allStringRect.Width, 0.0 );
					caretRect.Width += ( lines[index][0] == '\r' ) ? 
						(Gdiplus::REAL) ( dWidth_2 - font.GetSize() / 5 ) :
					(Gdiplus::REAL) ( caretRect.X + dWidth_2 );
				}

				if( isLastString ) 
					caretRect.Width = (Gdiplus::REAL) ( dWidth_2 );
			}

			displaceByY = lines.GetSize() * fontHeight;

			graphics.RotateTransform ( (Gdiplus::REAL)grad( fontAngle + M_PI_2 ) );
			graphics.SetClip( trackingRectF );

			switch( fontAlignW )
			{
			case byTop:
				{
					caretRect.Y	= (Gdiplus::REAL) ( -dHeight_2 + ( isLastString ? index + 1 : index ) * fontHeight );
					break;
				}
			case byMiddle:
				{	
					caretRect.Y	= (Gdiplus::REAL) ( -displaceByY / 2.0 + ( isLastString ? index + 1 : index ) * fontHeight );
					if( bEmtyString ) caretRect.Y -= (Gdiplus::REAL) ( fontHeight / 2.0 );
					break;
				}
			case byBottom:
				{
					caretRect.Y	= (Gdiplus::REAL) ( dHeight_2 - displaceByY + ( isLastString ? index + 1 : index ) * fontHeight );
					if( bEmtyString ) caretRect.Y -= (Gdiplus::REAL) ( fontHeight );
					break;
				}
			default:
				{
					caretRect.Y	= (Gdiplus::REAL) ( -dHeight_2 + ( isLastString ? index + 1 : index ) * fontHeight );
				}
			}
		
			graphics.DrawLine ( &borderCaretColor, caretRect.Width - font.GetSize() / 8, caretRect.Y - 1, caretRect.Width - font.GetSize() / 8, caretRect.Y + caretRect.Height + 1);
			graphics.DrawLine ( &fillCaretColor,   caretRect.Width - font.GetSize() / 8, caretRect.Y, caretRect.Width - font.GetSize() / 8, caretRect.Y + caretRect.Height );

			graphics.EndContainer ( graphicsContainer );
		}
	}
}
