#ifndef _SGLYPH_LIB_FONT_FILE_H
#define _SGLYPH_LIB_FONT_FILE_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include "SFontFile.h"

class SFontFileID;
class SGlyphLibFontEngine;

//-------------------------------------------------------------------------------------------------------------------------------
// SGlyphLibFontFile
//-------------------------------------------------------------------------------------------------------------------------------

class SGlyphLibFontFile: public SFontFile 
{
public:

	static SFontFile *LoadType1Font   (SGlyphLibFontEngine *pEngine, SFontFileID *pFontFileID, wchar_t *wsFileName, BOOL bDeleteFile, char **ppEncoding);
	static SFontFile *LoadCIDFont     (SGlyphLibFontEngine *pEngine, SFontFileID *pFontFileID, wchar_t *wsFileName, BOOL bDeleteFile, unsigned short *pCodeToCID, int nCodeToGIDLen);
	static SFontFile *LoadTrueTypeFont(SGlyphLibFontEngine *pEngine, SFontFileID *pFontFileID, wchar_t *wsFileName, BOOL bDeleteFile, unsigned short *pCodeToCID, int nCodeToGIDLen);

	virtual ~SGlyphLibFontFile();

	virtual SFont *MakeFont(double *pFontMatrix, double *pTextMatrix);

private:

	SGlyphLibFontFile(SGlyphLibFontEngine *pEngine, SFontFileID *pFontFileID, wchar_t *wsFileName, BOOL bDeleteFile, FT_Face oFace, unsigned short *pCodeToGID, int nCodeToGIDLen, BOOL bTrueType);

private:

	SGlyphLibFontEngine *m_pEngine;
	FT_Face              m_oFace;
	unsigned short      *m_pCodeToGID;
	int                  m_nCodeToGIDLen;
	BOOL                 m_bTrueType;

	friend class         SGlyphLibFont;
};

#endif /* _SGLYPH_LIB_FONT_FILE_H */
