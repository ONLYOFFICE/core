#include "CtrlNote.h"

namespace HWP
{
CCtrlNote::CCtrlNote(const std::string& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
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
}
