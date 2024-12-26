#include "Ctrl.h"

namespace HWP
{
CCtrl::CCtrl()
	:  m_bFullFilled(false)
{}

CCtrl::CCtrl(const HWP_STRING& sCtrlID)
	: m_sCtrlID(sCtrlID), m_bFullFilled(false)
{}

CCtrl::~CCtrl()
{}

void CCtrl::SetID(const HWP_STRING& sCtrlID)
{
	m_sCtrlID = sCtrlID;
}

HWP_STRING CCtrl::GetID() const
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

bool CCtrl::Equals(CCtrl* pFirstCtrl, CCtrl* pSecondCtrl)
{
	if (nullptr == pFirstCtrl || nullptr == pSecondCtrl)
		return false;

	return pFirstCtrl->m_sCtrlID == pSecondCtrl->m_sCtrlID &&
	       pFirstCtrl->m_bFullFilled == pSecondCtrl->m_bFullFilled;
}
}
