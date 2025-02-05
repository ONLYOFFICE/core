#include "CellParagraph.h"

namespace HWP
{
CCellParagraph::CCellParagraph()
	: CHWPPargraph()
{
}

CCellParagraph::CCellParagraph(CXMLNode& oNode, int nVersion)
	: CHWPPargraph(oNode, nVersion)
{}

EParagraphType CCellParagraph::GetType() const
{
	return EParagraphType::Cell;
}
}
