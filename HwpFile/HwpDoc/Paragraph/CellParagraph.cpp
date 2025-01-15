#include "CellParagraph.h"

namespace HWP
{
CCellParagraph::CCellParagraph()
{
}

EParagraphType CCellParagraph::GetType() const
{
	return EParagraphType::Cell;
}
}
