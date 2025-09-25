#include "ParaText.h"

namespace HWP
{
CParaText::CParaText(const HWP_STRING& sCtrlID, const HWP_STRING& sText, int nStartIDx)
	: CCtrl(sCtrlID), m_sText(sText), m_nStartIDx(nStartIDx)
{}

CParaText::CParaText(const HWP_STRING& sCtrlID, const HWP_STRING& sText, int nStartIDx, int nCharShapeID)
	: CCtrl(sCtrlID), m_sText(sText), m_nStartIDx(nStartIDx), m_nCharShapeID(nCharShapeID)
{}

ECtrlObjectType CParaText::GetCtrlType() const
{
	return ECtrlObjectType::ParaText;
}

unsigned int CParaText::GetTextLength() const
{
	return m_sText.length();
}

int CParaText::GetStartIDx() const
{
	return m_nStartIDx;
}

int CParaText::GetCharShapeID() const
{
	return m_nCharShapeID;
}

HWP_STRING CParaText::GetText() const
{
	return m_sText;
}

void CParaText::SetCharShapeID(int nCharShapeID)
{
	m_nCharShapeID = nCharShapeID;
}

void CParaText::SetText(const HWP_STRING& sText)
{
	m_sText = sText;
}
}
