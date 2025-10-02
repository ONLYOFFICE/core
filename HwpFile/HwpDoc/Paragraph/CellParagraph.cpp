#include "CellParagraph.h"

namespace HWP
{
CCellParagraph::CCellParagraph()
	: CHWPPargraph()
{}

CCellParagraph::CCellParagraph(CXMLReader& oReader, EHanType eType)
    : CHWPPargraph(oReader, eType)
{}

EParagraphType CCellParagraph::GetType() const
{
	return EParagraphType::Cell;
}
}
