#ifndef _FT_FONT_FILE_H
#define _FT_FONT_FILE_H

#include "FontFile.h"

class CFont;
class CFreeTypeFontEngine;

//-------------------------------------------------------------------------------------------------------------------------------
// CFreeTypeFontFile
//-------------------------------------------------------------------------------------------------------------------------------

class CFreeTypeFontFile: public CFontFile 
{
public:

	static CFontFile *LoadFont(CFreeTypeFontEngine *pEngine, wchar_t *wsFileName, long lIndex);

	virtual ~CFreeTypeFontFile();

	virtual CFont *MakeFont();

private:

	CFreeTypeFontFile(CFreeTypeFontEngine *pEngine, wchar_t *wsFileName, long lIndex, FT_Face pFace, HANDLE hFile, HANDLE hMapFile, void *pBaseAddress);

private:

	CFreeTypeFontEngine *m_pEngine;
	FT_Face              m_pFace;

	void                *m_pBaseAddress;
	HANDLE               m_hFile;
	HANDLE               m_hMapFile;

	friend class         CFreeTypeFont;
};

#endif /* _FT_FONT_FILE_H */
