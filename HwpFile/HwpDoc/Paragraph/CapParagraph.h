#ifndef CAPPARAGRAPH_H
#define CAPPARAGRAPH_H

#include "HWPPargraph.h"

namespace HWP
{
class CCapParagraph : public CHWPPargraph
{
public:
	CCapParagraph();

	EParagraphType GetType() const override;
};
}

#endif // CAPPARAGRAPH_H
