#include "TblCell.h"

namespace HWP
{
CTblCell::CTblCell(int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: m_nSize(nSize)
{
	oBuffer.SavePosition();

	oBuffer.Skip(2);

	oBuffer.ReadShort(m_shColAddr);
	oBuffer.ReadShort(m_shRowAddr);
	oBuffer.ReadShort(m_shColSpan);
	oBuffer.ReadShort(m_shRowSpan);
	oBuffer.ReadInt(m_nWidth);
	oBuffer.ReadInt(m_nHeight);

	for (unsigned int unIndex = 0; unIndex < 4; ++unIndex)
		m_arMargin[unIndex] = oBuffer.ReadShort();

	oBuffer.ReadShort(m_shBorderFill);

	oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos(true));
}

void CTblCell::SetVertAlign(EVertAlign eVertAlign)
{
	m_eVertAlign = eVertAlign;
}

void CTblCell::AddParagraph(CCellParagraph* pParagraph)
{
	m_arParas.push_back(pParagraph);
}

int CTblCell::GetSize()
{
	return m_nSize;
}

}
