#include "CellParagraph.h"

namespace HWP
{
CCellParagraph::CCellParagraph()
	: CHWPPargraph()
{
}

CCellParagraph::CCellParagraph(CXMLReader& oReader, int nVersion)
    : CHWPPargraph(oReader, nVersion)
{}

EParagraphType CCellParagraph::GetType() const
{
	return EParagraphType::Cell;
}
}
