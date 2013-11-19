#ifndef _SGLYPH_LIB_FONT_ENGINE_H
#define _SGLYPH_LIB_FONT_ENGINE_H

#include <ft2build.h>
#include FT_FREETYPE_H

class SFontFile;
class SFontFileID;

//-------------------------------------------------------------------------------------------------------------------------------
// SGlyphLibFontEngine
//-------------------------------------------------------------------------------------------------------------------------------

class SGlyphLibFontEngine 
{
public:

	static SGlyphLibFontEngine *Init(BOOL bAntiAliasing);

	~SGlyphLibFontEngine();

	// Загрузка шрифтов.
	SFontFile *LoadType1Font      (SFontFileID *pFontFileID, wchar_t *wsFileName, BOOL bDeleteFile, char **ppEncoding);
	SFontFile *LoadType1CFont     (SFontFileID *pFontFileID, wchar_t *wsFileName, BOOL bDeleteFile, char **ppEncoding);
	SFontFile *LoadOpenTypeT1CFont(SFontFileID *pFontFileID, wchar_t *wsFileName, BOOL bDeleteFile, char **ppEncoding);
	SFontFile *LoadCIDFont        (SFontFileID *pFontFileID, wchar_t *wsFileName, BOOL bDeleteFile);
	SFontFile *LoadOpenTypeCFFFont(SFontFileID *pFontFileID, wchar_t *wsFileName, BOOL bDeleteFile);
	SFontFile *LoadTrueTypeFont   (SFontFileID *pFontFileID, wchar_t *wsFileName, BOOL bDeleteFile, unsigned short *pCodeToGID, int nCodeToGIDLen, wchar_t *wsTempFolder);

private:

	SGlyphLibFontEngine(BOOL bAntiAliasing, FT_Library oLibrary);

private:

	BOOL       m_bAntiAliasing;
	FT_Library m_oLibrary;
	BOOL       m_bUseCIDs;

	friend class SGlyphLibFontFile;
	friend class SGlyphLibFont;
};

#endif /* _SGLYPH_LIB_FONT_ENGINE_H */
