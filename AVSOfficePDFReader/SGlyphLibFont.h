#ifndef _SGLYPH_LIB_FONT_H
#define _SGLYPH_LIB_FONT_H

#include <ft2build.h>
#include FT_FREETYPE_H

#ifdef _VS_2005
//#ifdef _DEBUG
////#pragma comment (lib, "FreeType\\freetype2312_vs2005_Debug.lib")
//#else
////#pragma comment (lib, "FreeType\\freetype2312_vs2005.lib")
//#endif
#pragma comment (lib, "FreeType\\freetype2312_vs2005.lib")

#endif

#ifdef _VS_2008
#pragma comment (lib, "FreeType\\freetype231.lib")
#endif

#include "SFont.h"

class SGlyphLibFontFile;

//-------------------------------------------------------------------------------------------------------------------------------
// SGlyphLibFont
//-------------------------------------------------------------------------------------------------------------------------------

class SGlyphLibFont: public SFont 
{
public:

	SGlyphLibFont(SGlyphLibFontFile *pFontFile, double *pFontMatrix, double *pTextMatrix);

	virtual ~SGlyphLibFont();

	virtual BOOL GetGlyph(int nCode, int nFracX, int nFracY, SGlyphBitmap *pBitmap);

	virtual BOOL MakeGlyph(int nCode, int nFracX, int nFracY, SGlyphBitmap *pBitmap);

	virtual SPath *GetGlyphPath(int nCode);

private:

	FT_Size   m_oSize;
	FT_Matrix m_oFontMatrix;
	FT_Matrix m_oTextMatrix;
	double    m_dTextScale;
};

#endif /* _SGLYPH_LIB_FONT_H */
