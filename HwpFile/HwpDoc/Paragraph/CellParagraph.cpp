#include "CellParagraph.h"

namespace HWP
{
CCellParagraph::CCellParagraph()
	: CHWPPargraph()
{
}

CCellParagraph::CCellParagraph(CXMLReader& oReader, int nVersion)
    : CHWPPargraph(oReader, nVersion, EHanType::HWPX)
{}

EParagraphType CCellParagraph::GetType() const
{
	return EParagraphType::Cell;
}
}
