#include "HWPRecordFormObject.h"

namespace HWP
{
CHWPRecordFormObject::CHWPRecordFormObject(int nTagNum, int nLevel, int nSize)
	: CHWPRecord(nTagNum, nLevel, nSize)
{}

int CHWPRecordFormObject::ParseCtrl(CCtrlForm& oForm, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	oBuffer.SavePosition();

	oBuffer.Skip(4); // tbp+
	oBuffer.Skip(4); // tbp+
	oBuffer.Skip(4); // Длина строки?

	oBuffer.ReadString(m_sFormStr, EStringCharacter::UTF16);

	return oBuffer.GetDistanceToLastPos();
}
}
