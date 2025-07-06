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
		m_arColSzWidths.resize(m_shColCount);
		m_arColSzGaps.resize(m_shColCount - 1);

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

CCtrlColumnDef::CCtrlColumnDef(const HWP_STRING& sCtrlID, CXMLReader& oReader, int nVersion)
	: CCtrl(sCtrlID), m_eColLineStyle(ELineStyle2::SOLID)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if ("colCount" == sAttributeName)
			m_shColCount = oReader.GetInt();
		else if ("sameSz" == sAttributeName)
			m_bSameSz = oReader.GetBool();
	}
	END_READ_ATTRIBUTES(oReader)

	if (!m_bSameSz)
	{
		m_arColSzWidths.resize(m_shColCount);
		m_arColSzGaps.resize(m_shColCount);
	}

	unsigned int unColSzIndex = 0;
	std::string sNodeName;

	WHILE_READ_NEXT_NODE(oReader)
	{
		sNodeName = oReader.GetNameA();

		if ("hp:colLine" == sNodeName)
		{
			START_READ_ATTRIBUTES(oReader)
			{
				if ("type" == sAttributeName)
					m_eColLineStyle = GetLineStyle2(oReader.GetText2());
				else if ("width" == sAttributeName)
					m_chColLineWidth = (HWP_BYTE)ConvertWidthToHWP(oReader.GetText2A());
				else if ("color" == sAttributeName)
					m_nColLineColor = oReader.GetColor();
			}
			END_READ_ATTRIBUTES(oReader)
		}
		else if ("hp:colSz" == sNodeName)
		{
			START_READ_ATTRIBUTES(oReader)
			{
				if ("width" == sAttributeName)
					m_arColSzWidths[unColSzIndex] = oReader.GetInt();
				else if ("gap" == sAttributeName)
					m_arColSzGaps[unColSzIndex++] =  oReader.GetInt();
			}
			END_READ_ATTRIBUTES(oReader)
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
