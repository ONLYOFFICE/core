#include "Ctrl.h"

namespace HWP
{
CCtrl::CCtrl()
	:  m_bFullFilled(false)
{}

CCtrl::CCtrl(const STRING& sCtrlID)
	: m_sCtrlID(sCtrlID), m_bFullFilled(false)
{}

void CCtrl::SetID(const STRING& sCtrlID)
{
	m_sCtrlID = sCtrlID;
}

STRING CCtrl::GetID() const
{
	return m_sCtrlID;
}

bool CCtrl::FullFilled() const
{
	return m_bFullFilled;
}

void CCtrl::SetFullFilled()
{
	m_bFullFilled = true;
}
}
