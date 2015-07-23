#ifndef _PDF_WRITER_SRC_FONT14_H
#define _PDF_WRITER_SRC_FONT14_H

#include "Font.h"

namespace PdfWriter
{
	class CXref;
	class CDocument;
	class CFont14 : public CFontDict
	{
	public:
		CFont14(CXref* pXref, CDocument* pDocument, EStandard14Fonts eType);
		EFontType GetFontType()
		{
			return fontType1;
		}

	};
}

#endif // _PDF_WRITER_SRC_FONT14_H