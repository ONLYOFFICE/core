#ifndef _SFONT_FILE_H
#define _SFONT_FILE_H

#include "STypes.h"

class StringExt;
class SFontEngine;
class SFont;
class SFontFileID;

//-------------------------------------------------------------------------------------------------------------------------------
// SFontFile
//-------------------------------------------------------------------------------------------------------------------------------

class SFontFile 
{
public:

	virtual ~SFontFile();

	// Создаем новый объект SFont, для данного FontFile.
	virtual SFont *MakeFont(double *pFontMatrix, double *pTextMatrix) = 0;

	// Font file ID.
	SFontFileID *GetFontFileID() 
	{ 
		return m_pFontFileID; 
	}

	// Счетчик ссылок. Удаляем объект если счетчик равен 0.
	void AddRef();
	void Release();

protected:

	SFontFile(SFontFileID *pFontFileID, wchar_t *wsFileName, BOOL bDeleteFile);

private:

	SFontFileID *m_pFontFileID;
	CStringW     m_wsFileName;
	BOOL         m_bDeleteFile;
	int          m_nRef;

	friend class SFontEngine;
};

#endif /* _SFONT_FILE_H */
