#include "AvsSalGraphics.h"
//#include <gdiplus.h>
//
//#pragma comment(lib, "gdiplus.lib")
//
//using namespace Gdiplus;
//
//void AvsGraphics::drawPixel( long nX, long nY )
//	{
//	if ( mbXORMode )
//	{
//		HBRUSH	hBrush = CreateSolidBrush( mnPenColor );
//		HBRUSH	hOldBrush = SelectBrush( mhDC, hBrush );
//		PatBlt( mhDC, (int)nX, (int)nY, (int)1, (int)1, PATINVERT );
//		SelectBrush( mhDC, hOldBrush );
//		DeleteBrush( hBrush );
//	}
//	else
//		::SetPixel( mhDC, (int)nX, (int)nY, mnPenColor );
//	}
//void AvsGraphics::drawPixel( long nX, long nY, Color oColor )
//{
//	COLORREF nCol = PALETTERGB( oColor.GetRed() ,oColor.GetGreen(), oColor.GetBlue() );
//
//	if ( !mbPrinter &&
//		 GetSalData()->mhDitherPal &&
//		 ImplIsSysColorEntry( nSalColor ) )
//		nCol = PALRGB_TO_RGB( nCol );
//
//	if ( mbXORMode )
//	{
//		HBRUSH	hBrush = CreateSolidBrush( nCol );
//		HBRUSH	hOldBrush = SelectBrush( mhDC, hBrush );
//		PatBlt( mhDC, (int)nX, (int)nY, (int)1, (int)1, PATINVERT );
//		SelectBrush( mhDC, hOldBrush );
//		DeleteBrush( hBrush );
//	}
//	else
//		::SetPixel( mhDC, (int)nX, (int)nY, nCol );
//}