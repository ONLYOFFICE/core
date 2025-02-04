#ifndef HWPRECORDPARARANGETAG_H
#define HWPRECORDPARARANGETAG_H

#include "HWPRecord.h"
#include "../Paragraph/HWPPargraph.h"

namespace HWP
{
class CHWPRecordParaRangeTag : public CHWPRecord
{
public:
	CHWPRecordParaRangeTag(int nTagNum, int nLevel, int nSize);

	static int Parse(CHWPPargraph& oPara, int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // HWPRECORDPARARANGETAG_H
