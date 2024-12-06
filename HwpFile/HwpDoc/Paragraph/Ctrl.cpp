#include "Ctrl.h"

namespace HWP
{
CCtrl::CCtrl()
{}

CCtrl::CCtrl(const std::string& sCtrlID)
	: m_sCtrlID(sCtrlID)
{}

void CCtrl::SetID(const std::string& sCtrlID)
{
	m_sCtrlID = sCtrlID;
}

std::string CCtrl::GetID() const
{
	return m_sCtrlID;
}
}
