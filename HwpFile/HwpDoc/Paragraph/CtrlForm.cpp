#include "CtrlForm.h"

namespace HWP
{
CCtrlForm::CCtrlForm(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrl(sCtrlID), m_nSize(nSize)
{}

int CCtrlForm::GetSize()
{
	return m_nSize;
}
}
