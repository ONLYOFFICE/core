#include "CapParagraph.h"

namespace HWP
{
CCapParagraph::CCapParagraph()
	: CHWPPargraph()
{}

CCapParagraph::CCapParagraph(CXMLReader& oReader, EHanType eType)
    : CHWPPargraph(oReader, eType)
{}

EParagraphType CCapParagraph::GetType() const
{
	return EParagraphType::Cap;
}
}
