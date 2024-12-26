#include "CtrlNote.h"

namespace HWP
{
CCtrlNote::CCtrlNote()
{}

CCtrlNote::CCtrlNote(const HWP_STRING& sCtrlID)
	: CCtrl(sCtrlID)
{}

CCtrlNote::CCtrlNote(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrl(sCtrlID)
{
	oBuffer.Skip(8);
	m_bFullFilled = true;
	m_nSize = 8;
}

int CCtrlNote::GetSize()
{
	return m_nSize;
}

void CCtrlNote::AddParagraph(CHWPPargraph* pParagraph)
{
	m_arParas.push_back(pParagraph);
}
}
