#include "Font.h"

namespace PdfWriter
{
	CFontDict::CFontDict(CXref* pXref, CDocument* pDocument)
	{
		m_pXref = pXref;
		pXref->Add(this);
		m_pDocument = pDocument;
	}
	CFontDict::~CFontDict()
	{
	}	
}
