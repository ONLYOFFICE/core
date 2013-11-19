#ifndef _FONT_FILE_TYPE1_H
#define _FONT_FILE_TYPE1_H

#include "FontFileBase.h"

//------------------------------------------------------------------------
// CFontFileType1
//------------------------------------------------------------------------

class CFontFileType1: public CFontFileBase 
{
public:

	static CFontFileType1 *LoadFromBuffer(char *sBuffer, int nLen);
	static CFontFileType1 *LoadFromFile(wchar_t *wsFileName);

	virtual ~CFontFileType1();

	char *GetName();

	char **GetEncoding();

	void WriteEncoded(char **ppNewEncoding, FontFileOutputFunc pOutputFunc, void *pOutputStream);

private:

	CFontFileType1(char *sBuffer, int nLen, BOOL bFreeData);

	char *GetNextLine(char *sLine);
	void Parse();

private:

	char  *m_sName;
	char **m_arrEncoding;
	BOOL   m_bParsed;
};

#endif /* _FONT_FILE_TYPE1_H */
