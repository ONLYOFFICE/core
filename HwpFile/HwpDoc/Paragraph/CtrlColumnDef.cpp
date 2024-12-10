#include "CtrlColumnDef.h"

namespace HWP
{
CCtrlColumnDef::CCtrlColumnDef(const STRING& sCtrlID)
	: CCtrl(sCtrlID)
{}

CCtrlColumnDef::CCtrlColumnDef(const STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrl(sCtrlID)
{
	BYTE* pOldCurrentPos = oBuffer.GetCurPtr();

	short shAttrLowBits;
	oBuffer.ReadShort(shAttrLowBits);
	oBuffer.ReadShort(m_shSameGap);

	m_shColCount = (short)(shAttrLowBits >> 2 & 0xFF);
	m_bSameSz = CHECK_FLAG(shAttrLowBits >> 12, 0x1);

	if (!m_bSameSz)
	{
		m_arColSzWidths.reserve(m_shColCount);
		m_arColSzGaps.reserve(m_shColCount - 1);

		for (int nIndex = 0; nIndex < m_shColCount; ++nIndex)
		{
			oBuffer.ReadShort(m_arColSzWidths[nIndex]);
			if (nIndex < m_shColCount - 1)
				oBuffer.ReadShort(m_arColSzGaps[nIndex]);
		}
	}

	short shAttrHighBits;
	oBuffer.ReadShort(shAttrHighBits);
	m_eColLineStyle = GetLineStyle2(oBuffer.ReadByte());
	oBuffer.ReadByte(m_chColLineWidth);
	oBuffer.ReadInt(m_nColLineColor);

	m_nAttr = shAttrHighBits << 16 | shAttrLowBits;

	m_nSize = oBuffer.GetCurPtr() - pOldCurrentPos;
	m_bFullFilled = true;
}

int CCtrlColumnDef::GetSize()
{
	return m_nSize;
}
}
