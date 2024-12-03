#ifndef HWPRECORDFORMOBJECT_H
#define HWPRECORDFORMOBJECT_H

#include "../HWPStream.h"
#include "HWPRecord.h"
#include <string>

namespace HWP
{
class CHWPRecordFormObject : public CHWPRecord
{
	std::string m_sFormStr;
public:
	CHWPRecordFormObject(int nTagNum, int nLevel, int nSize);

	int ParseCtrl(CCtrlForm& oForm, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // HWPRECORDFORMOBJECT_H
