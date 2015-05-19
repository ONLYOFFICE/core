#pragma once
#include "salgdi.h"
#include <windows.h>


class WinGraphics : public SVMCore::SalGraphics
{
public:
	HDC 					mhDC;				// HDC
	HWND					mhWnd;				// Window-Handle, when Window-Graphics
	HFONT					mhFonts[ MAX_FALLBACK ];        // Font + Fallbacks
    const ImplWinFontData*  mpWinFontData[ MAX_FALLBACK ];  // pointer to the most recent font face
    ImplWinFontEntry*       mpWinFontEntry[ MAX_FALLBACK ]; // pointer to the most recent font instance
    float                   mfFontScale;        // allows metrics emulation of huge font sizes
	HPEN					mhPen;				// Pen
	HBRUSH					mhBrush;			// Brush
	HRGN					mhRegion;			// Region Handle
	HPEN					mhDefPen;			// DefaultPen
	HBRUSH					mhDefBrush; 		// DefaultBrush
	HFONT					mhDefFont;			// DefaultFont
	HPALETTE				mhDefPal;			// DefaultPalette
	COLORREF				mnPenColor; 		// PenColor
	COLORREF				mnBrushColor;		// BrushColor
	COLORREF				mnTextColor;		// TextColor
	RGNDATA*				mpClipRgnData;		// ClipRegion-Data
	RGNDATA*				mpStdClipRgnData;	// Cache Standard-ClipRegion-Data
	RECT*					mpNextClipRect; 	// Naechstes ClipRegion-Rect
	BOOL					mbFirstClipRect;	// Flag for first cliprect to insert
	LOGFONTA*				mpLogFont;			// LOG-Font which is currently selected (only W9x)
	ImplFontAttrCache*		mpFontAttrCache;	// Cache font attributes from files in so/share/fonts
	BYTE*					mpFontCharSets; 	// All Charsets for the current font
	BYTE					mnFontCharSetCount; // Number of Charsets of the current font; 0 - if not queried
	BOOL					mbFontKernInit; 	// FALSE: FontKerns must be queried
	KERNINGPAIR*			mpFontKernPairs;	// Kerning Pairs of the current Font
	ULONG					mnFontKernPairCount;// Number of Kerning Pairs of the current Font
	int 					mnPenWidth; 		// Linienbreite
	BOOL					mbStockPen; 		// is Pen a stockpen
	BOOL					mbStockBrush;		// is Brush a stcokbrush
	BOOL					mbPen;				// is Pen (FALSE == NULL_PEN)
	BOOL					mbBrush;			// is Brush (FALSE == NULL_BRUSH)
	BOOL					mbPrinter;			// is Printer
	BOOL					mbVirDev;			// is VirDev
	BOOL					mbWindow;			// is Window
	BOOL					mbScreen;			// is Screen compatible
	bool					mbXORMode;			// _every_ output with RasterOp XOR

    // remember RGB values for SetLineColor/SetFillColor
    SalColor                maLineColor;
    SalColor                maFillColor;

public: SalGraphics()
		{
		}
public: ~SalGraphics()
		{
		}
void	ImplSalInitGraphics( );
void	ImplSalDeInitGraphics( );

public: void drawPixel( long nX, long nY );
public: void drawPixel( long nX, long nY, Color oColor );

};
