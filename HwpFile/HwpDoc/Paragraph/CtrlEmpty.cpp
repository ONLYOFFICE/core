#include "CtrlEmpty.h"

namespace HWP
{
CCtrlEmpty::CCtrlEmpty()
{}

CCtrlEmpty::CCtrlEmpty(const HWP_STRING& sCtrlID)
	: CCtrl(sCtrlID)
{}

ECtrlObjectType CCtrlEmpty::GetCtrlType() const
{
	return ECtrlObjectType::Empty;
}
}
