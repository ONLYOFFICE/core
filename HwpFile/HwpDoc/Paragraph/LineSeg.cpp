#include "LineSeg.h"

namespace HWP
{
CLineSeg::CLineSeg(int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	oBuffer.SavePosition();

	oBuffer.ReadInt(m_nStartPos);
	oBuffer.ReadInt(m_nLineVerticalPos);
	oBuffer.ReadInt(m_nLineHeight);
	oBuffer.ReadInt(m_nTextHeight);
	oBuffer.ReadInt(m_nLineDistanceToBase);
	oBuffer.ReadInt(m_nLineSpacing);
	oBuffer.ReadInt(m_nColumnStartPos);
	oBuffer.ReadInt(m_nSegmentWidth);
	oBuffer.ReadInt(m_nLineTag);
	m_bIsHeadingApplied = CHECK_FLAG(m_nLineTag >> 21, 0x01);

	oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos(true));
}
}
