#include "CtrlForm.h"

namespace HWP
{
CCtrlForm::CCtrlForm(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrl(sCtrlID)
{}

ECtrlObjectType CCtrlForm::GetCtrlType() const
{
	return ECtrlObjectType::Form;
}
}
