#include "CapParagraph.h"

namespace HWP
{
CCapParagraph::CCapParagraph()
	: CHWPPargraph()
{}

CCapParagraph::CCapParagraph(CXMLReader& oReader, int nVersion)
    : CHWPPargraph(oReader, nVersion, EHanType::HWPX)
{}

EParagraphType CCapParagraph::GetType() const
{
	return EParagraphType::Cap;
}
}
