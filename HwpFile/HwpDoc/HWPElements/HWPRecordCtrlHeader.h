#ifndef HWPRECORDCTRLHEADER_H
#define HWPRECORDCTRLHEADER_H

#include "HWPRecord.h"

#include "../Paragraph/CtrlAutoNumber.h"
#include "../Paragraph/CtrlClick.h"
#include "../Paragraph/CtrlColumnDef.h"
#include "../Paragraph/CtrlEqEdit.h"
#include "../Paragraph/CtrlForm.h"
#include "../Paragraph/CtrlGeneralShape.h"
#include "../Paragraph/CtrlHeadFoot.h"
#include "../Paragraph/CtrlNewNumber.h"
#include "../Paragraph/CtrlNote.h"
#include "../Paragraph/CtrlPageNumPos.h"
#include "../Paragraph/CtrlSectionDef.h"
#include "../Paragraph/CtrlTalbe.h"

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
