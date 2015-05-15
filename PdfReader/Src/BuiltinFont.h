#ifndef _PDF_READER_BUILT_INFONT_H
#define _PDF_READER_BUILT_INFONT_H

namespace PdfReader
{
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

	static bool BuiltinFontGetWidth(BuiltinFont *pFont, char *sName, unsigned short *punWidth)
	{
		for (int nIndex = 0; nIndex < pFont->nSize; nIndex++)
		{
			BuiltinFontWidth oWitdh = pFont->pWidths[nIndex];
			if (!strcmp(oWitdh.sName, sName))
			{
				*punWidth = oWitdh.unWidth;
				return true;
			}
		}
		return false;
	}
}

#endif // _PDF_READER_BUILT_INFONT_H
