#include "CellParagraph.h"

namespace HWP
{
CCellParagraph::CCellParagraph()
	: CHWPPargraph()
{}

CCellParagraph::CCellParagraph(CXMLReader& oReader, int nVersion, EHanType eType)
    : CHWPPargraph(oReader, nVersion, eType)
{}

EParagraphType CCellParagraph::GetType() const
{
	return EParagraphType::Cell;
}
}
