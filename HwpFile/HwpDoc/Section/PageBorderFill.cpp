#include "PageBorderFill.h"

namespace HWP
{
CPageBorderFill::CPageBorderFill()
{}

CPageBorderFill* CPageBorderFill::Parse(int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	CPageBorderFill* pBorderFill = new CPageBorderFill();

	if (nullptr == pBorderFill)
		return nullptr;

	oBuffer.SavePosition();

	int nAttr;
	oBuffer.ReadInt(nAttr);

	pBorderFill->m_bTextBorder = CHECK_FLAG(nAttr, 0x01);
	pBorderFill->m_bHeaderInside = CHECK_FLAG(nAttr, 0x02);
	pBorderFill->m_bHeaderInside = CHECK_FLAG(nAttr, 0x04);
	pBorderFill->m_chFillArea = (HWP_BYTE)((nAttr >> 3) & 0x03);

	oBuffer.ReadShort(pBorderFill->m_shOffsetLeft);
	oBuffer.ReadShort(pBorderFill->m_shOffsetRight);
	oBuffer.ReadShort(pBorderFill->m_shOffsetTop);
	oBuffer.ReadShort(pBorderFill->m_shOffsetBottom);
	oBuffer.ReadShort(pBorderFill->m_shBorderFill);

	oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos(true));

	return pBorderFill;
}
}
