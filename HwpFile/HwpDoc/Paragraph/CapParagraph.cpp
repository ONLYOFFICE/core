#include "CapParagraph.h"

namespace HWP
{
CCapParagraph::CCapParagraph()
	: CHWPPargraph()
{}

CCapParagraph::CCapParagraph(CXMLNode& oNode, int nVersion)
	: CHWPPargraph(oNode, nVersion)
{}

EParagraphType CCapParagraph::GetType() const
{
	return EParagraphType::Cap;
}
}
