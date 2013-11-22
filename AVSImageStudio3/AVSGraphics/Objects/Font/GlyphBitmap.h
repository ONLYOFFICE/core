#ifndef _GLYPH_BITMAP_H
#define _GLYPH_BITMAP_H

//-------------------------------------------------------------------------------------------------------------------------------
// TGlyphBitmap
//-------------------------------------------------------------------------------------------------------------------------------

struct TGlyphBitmap 
{
	int            nX;        // Сдвиг по X начальной точки для рисования символа
	int            nY;        // Сдвиг по Y начальной точки для рисования символа
	int            nWidth;    // Ширина символа
	int            nHeight;   // Высота символа
	BOOL           bAA;       // Anti-aliased: True означает, что Bitmap 8-битный(т.е. с альфой); False - Bitmap 1-битный
	unsigned char *pData;     // Bitmap data(картинка с символом)
	BOOL           bFreeData; // True, если память в pData нужно освободить

	TGlyphBitmap()
	{
		nX			= 0;
		nY			= 0;
		nWidth		= 0;
		nHeight		= 0;
		bAA			= FALSE;
		pData		= NULL;
		bFreeData	= NULL;
	}
};

#endif /* _GLYPH_BITMAP_H */