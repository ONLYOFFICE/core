#include "CtrlColumnDef.h"

namespace HWP
{
CCtrlColumnDef::CCtrlColumnDef(const HWP_STRING& sCtrlID)
	: CCtrl(sCtrlID)
{}

CCtrlColumnDef::CCtrlColumnDef(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrl(sCtrlID)
{
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

	m_bFullFilled = true;
}

CCtrlColumnDef::CCtrlColumnDef(const HWP_STRING& sCtrlID, CXMLNode& oNode, int nVersion)
	: CCtrl(sCtrlID)
{
	m_eColLineStyle = ELineStyle2::SOLID;
	m_shColCount = oNode.GetAttributeInt(L"colCount");
	m_bSameSz = oNode.GetAttributeBool(L"sameSz");

	if (!m_bSameSz)
	{
		m_arColSzWidths.resize(m_shColCount);
		m_arColSzGaps.resize(m_shColCount);
	}

	unsigned int unColSzIndex = 0;

	for (CXMLNode& oChild : oNode.GetChilds())
	{
		if (L"hp:colLine" == oChild.GetName())
		{
			m_eColLineStyle = GetLineStyle2(oChild.GetAttribute(L"type"));
			m_chColLineWidth = (HWP_BYTE)ConvertWidthToHWP(oChild.GetAttribute(L"width"));
			m_nColLineColor = oChild.GetAttributeColor(L"color");
		}
		else if (L"hp:colSz" == oChild.GetName())
		{
			m_arColSzWidths[unColSzIndex] = oChild.GetAttributeInt(L"width");
			m_arColSzGaps[unColSzIndex++] = oChild.GetAttributeInt(L"gap");
		}
	}

	m_bFullFilled = true;
}

ECtrlObjectType CCtrlColumnDef::GetCtrlType() const
{
	return ECtrlObjectType::ColumnDef;
}

short CCtrlColumnDef::GetColCount() const
{
	return m_shColCount;
}

ELineStyle2 CCtrlColumnDef::GetColLineStyle() const
{
	return m_eColLineStyle;
}
}
