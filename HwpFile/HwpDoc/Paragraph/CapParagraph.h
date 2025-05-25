#ifndef CAPPARAGRAPH_H
#define CAPPARAGRAPH_H

#include "HWPPargraph.h"

namespace HWP
{
class CCapParagraph : public CHWPPargraph
{
public:
	CCapParagraph();
	CCapParagraph(CXMLNode& oNode, int nVersion);

	EParagraphType GetType() const override;
};
}

#endif // CAPPARAGRAPH_H
