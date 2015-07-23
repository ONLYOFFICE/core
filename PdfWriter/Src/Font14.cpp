#include "Font14.h"

namespace PdfWriter
{
	static const char* c_sStandardFontNames[] =
	{
		"Helvetica",
		"Helvetica-Bold",
		"Helvetica-Oblique",
		"Helvetice-BoldOblique",
		"Courier",
		"Courier-Bold",
		"Courier-Oblique",
		"Courier-BoldOblique",
		"Times",
		"Times-Bold",
		"Times-Oblique",
		"Times-BoldOblique",
		"Symbol",
		"ZapfDingbats",
		NULL
	};
	CFont14::CFont14(CXref* pXref, CDocument* pDocument, EStandard14Fonts eType) : CFontDict(pXref, pDocument)
	{
		Add("Type", "Font");
		Add("Subtype", "Type1");
		Add("BaseFont", c_sStandardFontNames[(int)eType]);
	}
}