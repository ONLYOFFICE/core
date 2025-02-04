#ifndef CELLPARAGRAPH_H
#define CELLPARAGRAPH_H

#include "HWPPargraph.h"

namespace HWP
{
class CCellParagraph : public CHWPPargraph
{
public:
	CCellParagraph();
	CCellParagraph(CXMLNode& oNode, int nVersion);

	EParagraphType GetType() const override;
};
}

#endif // CELLPARAGRAPH_H
