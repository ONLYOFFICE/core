#include "CtrlEmpty.h"

namespace HWP
{
CCtrlEmpty::CCtrlEmpty()
	: m_nSize(0)
{}

CCtrlEmpty::CCtrlEmpty(const HWP_STRING& sCtrlID)
	: CCtrl(sCtrlID), m_nSize(0)
{}

int CCtrlEmpty::GetSize()
{
	return m_nSize;
}

void CCtrlEmpty::SetSize(int nSize)
{
	m_nSize = nSize;
}
}
