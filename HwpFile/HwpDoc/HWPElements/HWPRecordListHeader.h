#ifndef HWPRECORDLISTHEADER_H
#define HWPRECORDLISTHEADER_H

#include "HWPRecord.h"
#include "../HWPStream.h"
#include "../Paragraph/CtrlCommon.h"

namespace HWP
{
class CHWPRecordListHeader : public CHWPRecord
{
public:
	CHWPRecordListHeader(int nTagNum, int nLevel, int nSize);

	static int GetCount(int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static EVertAlign GetVertAlign(int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // HWPRECORDLISTHEADER_H
