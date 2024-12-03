#ifndef HWPPARGRAPH_H
#define HWPPARGRAPH_H

#include "../HWPStream.h"

namespace HWP
{
class CHWPPargraph
{
	short m_shParaShapeID;
	short m_shParaStyleID;
	BYTE m_chBreakType;
public:
	CHWPPargraph();
};
}

#endif // HWPPARGRAPH_H
