#ifndef HWPRECORDCTRLHEADER_H
#define HWPRECORDCTRLHEADER_H

#include "HWPRecord.h"

#include "../Paragraph/Ctrl.h"
#include "../HWPStream.h"

namespace HWP
{
class CHWPRecordCtrlHeader : public CHWPRecord
{
public:
	CHWPRecordCtrlHeader(int nTagNum, int nLevel, int nSize);

	static CCtrl* Parse(int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // HWPRECORDCTRLHEADER_H
