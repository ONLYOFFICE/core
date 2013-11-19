#ifndef _SFONT_ENGINE_H
#define _SFONT_ENGINE_H

class SGlyphLibFontEngine;
class SFontFile;
class SFontFileID;
class SFont;

//-------------------------------------------------------------------------------------------------------------------------------

#define sfontCacheSize 16

//-------------------------------------------------------------------------------------------------------------------------------
// SFontEngine
//-------------------------------------------------------------------------------------------------------------------------------

class SFontEngine 
{
public:

	SFontEngine(BOOL bEnableGlyphLibrary, BOOL bAntiAliasing );

	~SFontEngine();

	// Получаем FontFile из кэша. Функция возвращает NULL, если в кэше нет соответствующего FontFile.
	SFontFile *GetFontFile(SFontFileID *pFontFileID);

	// Загружаем Font, при этом создается новый объект SFontFile.
	SFontFile *LoadType1Font      (SFontFileID *pFontFileID, wchar_t *wsFileName, BOOL bDeleteFile, char **ppEncoding);
	SFontFile *LoadType1CFont     (SFontFileID *pFontFileID, wchar_t *wsFileName, BOOL bDeleteFile, char **ppEncoding);
	SFontFile *LoadOpenTypeT1CFont(SFontFileID *pFontFileID, wchar_t *wsFileName, BOOL bDeleteFile, char **ppEncoding);
	SFontFile *LoadCIDFont        (SFontFileID *pFontFileID, wchar_t *wsFileName, BOOL bDeleteFile);
	SFontFile *LoadOpenTypeCFFFont(SFontFileID *pFontFileID, wchar_t *wsFileName, BOOL bDeleteFile);
	SFontFile *LoadTrueTypeFont   (SFontFileID *pFontFileID, wchar_t *wsFileName, BOOL bDeleteFile, unsigned short *pCodeToGID, int nCodeToGIDLen, wchar_t *wsTempFolder);

	// Функция возвращает Font по данному FontFile: сначала ищем фонт в кэше, и если Font не найден, тогда создаем новый объект
	// SFont и добавляем его в кэш. При этом FontMatrix = TextMat * CTM: [ FontMatrix[0] FontMatrix[1] ]
	//                                                                   [ FontMatrix[2] FontMatrix[3] ].
	SFont *GetFont(SFontFile *pFontFile, double *pTextMatrix, double *pCTM);

private:

	SFont               *m_pFontCache[sfontCacheSize];
	SGlyphLibFontEngine *m_pGlyphLibEngine;
};

#endif /* _SFONT_ENGINE_H */
