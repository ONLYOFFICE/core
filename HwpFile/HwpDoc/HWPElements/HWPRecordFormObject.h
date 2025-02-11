#ifndef HWPRECORDFORMOBJECT_H
#define HWPRECORDFORMOBJECT_H

#include "../Paragraph/CtrlForm.h"
#include "HWPRecord.h"

namespace HWP
{
class CHWPRecordFormObject : public CHWPRecord
{
	static HWP_STRING m_sFormStr;
public:
	CHWPRecordFormObject(int nTagNum, int nLevel, int nSize);

	static int ParseCtrl(CCtrlForm& oForm, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // HWPRECORDFORMOBJECT_H
