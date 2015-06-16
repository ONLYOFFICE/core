#ifndef _PDF_WRITER_SRC_FONT_H
#define _PDF_WRITER_SRC_FONT_H

#include "Objects.h"

namespace PdfWriter
{
	class CDocument;
	class CFontDict : public CDictObject
	{
	public:
		CFontDict(CXref* pXref, CDocument* pDocument);
		virtual ~CFontDict();

		EDictType GetDictType() const
		{
			return dict_type_FONT;
		}
		virtual EFontType GetFontType()
		{
			return fontUnknownType;
		}

	protected:

		CXref*     m_pXref;
		CDocument* m_pDocument;
	};
}

#endif // _PDF_WRITER_SRC_FONT_H
