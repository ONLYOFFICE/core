#include "ParaText.h"

namespace HWP
{
CParaText::CParaText(const STRING& sCtrlID, const STRING& sText, int nStartIDx)
	: CCtrl(sCtrlID), m_sText(sText), m_nStartIDx(nStartIDx)
{}

int CParaText::GetSize()
{
	return m_sText.length();
}
}
