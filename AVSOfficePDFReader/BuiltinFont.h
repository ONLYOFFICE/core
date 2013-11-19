#ifndef _BUILT_INFONT_H
#define _BUILT_INFONT_H

struct BuiltinFont;
class BuiltinFontWidths;

//------------------------------------------------------------------------
struct BuiltinFontWidth 
{
	char             *sName;
	unsigned short    unWidth;
	BuiltinFontWidth *pNext;
};

struct BuiltinFont 
{
	char              *sName;
	char             **ppDefaultBaseEncoding;
	short              nAscent;
	short              nDescent;
	short              arrBBox[4];
	BuiltinFontWidth  *pWidths;
	int                nSize;
};

//------------------------------------------------------------------------

static BOOL BuiltinFontGetWidth(BuiltinFont *pFont, char *sName, unsigned short *punWidth) 
{
	for ( int nIndex = 0; nIndex < pFont->nSize; nIndex++  ) 
	{
		BuiltinFontWidth oWitdh = pFont->pWidths[nIndex];
		if ( !strcmp( oWitdh.sName, sName ) ) 
		{
			*punWidth = oWitdh.unWidth;
			return TRUE;
		}
	}
	return FALSE;
}


#endif /* _BUILT_INFONT_H */
