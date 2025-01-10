#include "CapParagraph.h"

namespace HWP
{
CCapParagraph::CCapParagraph()
{}

EParagraphType CCapParagraph::GetType() const
{
	return EParagraphType::Cap;
}
}
