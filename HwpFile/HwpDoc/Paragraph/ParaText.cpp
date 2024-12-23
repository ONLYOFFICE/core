#include "ParaText.h"

namespace HWP
{
CParaText::CParaText(const STRING& sCtrlID, const STRING& sText, int nStartIDx)
	: CCtrl(sCtrlID), m_sText(sText), m_nStartIDx(nStartIDx)
{}

CParaText::CParaText(const STRING& sCtrlID, const STRING& sText, int nStartIDx, int nCharShapeID)
	: CCtrl(sCtrlID), m_sText(sText), m_nStartIDx(nStartIDx), m_nCharShapeID(nCharShapeID)
{}

int CParaText::GetSize()
{
	return m_sText.length();
}

int CParaText::GetStartIDx() const
{
	return m_nStartIDx;
}

STRING CParaText::GetText() const
{
	return m_sText;
}

void CParaText::SetCharShapeID(int nCharShapeID)
{
	m_nCharShapeID = nCharShapeID;
}

void CParaText::SetText(const STRING& sText)
{
	m_sText = sText;
}
}
