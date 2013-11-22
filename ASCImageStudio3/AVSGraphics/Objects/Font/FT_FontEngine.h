#ifndef _FT_FONT_ENGINE_H
#define _FT_FONT_ENGINE_H

class CFontFile;
class CFont;

//-------------------------------------------------------------------------------------------------------------------------------
// CFreeTypeFontEngine
//-------------------------------------------------------------------------------------------------------------------------------

class CFreeTypeFontEngine 
{
public:

	static CFreeTypeFontEngine *Initialize(BOOL bAntiAliasing, BOOL bUseKerning);

	~CFreeTypeFontEngine();

	// Загрузка шрифтов.
	CFontFile *LoadFont(wchar_t *wsFileName, long lIndex);

private:

	CFreeTypeFontEngine(BOOL bAntiAliasing, FT_Library oLibrary, BOOL bUseKerning);

private:

	BOOL       m_bAntiAliasing;
	BOOL       m_bUseKerning;
	FT_Library m_pLibrary;
	BOOL       m_bUseCIDs;

	friend class CFontEngine;
	friend class CFreeTypeFontFile;
	friend class CFreeTypeFont;
};

#endif /* _FT_FONT_ENGINE_H */
