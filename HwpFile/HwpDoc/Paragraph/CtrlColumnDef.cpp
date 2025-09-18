#include "CtrlColumnDef.h"

#include "../Common/NodeNames.h"

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

CCtrlColumnDef::CCtrlColumnDef(const HWP_STRING& sCtrlID, CXMLReader& oReader, int nVersion, EHanType eType)
	: CCtrl(sCtrlID), m_eColLineStyle(ELineStyle2::SOLID)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if (GetAttributeName(EAttribute::ColumnCount, eType) == sAttributeName)
			m_shColCount = oReader.GetInt();
		else if (GetAttributeName(EAttribute::SameSize, eType) == sAttributeName)
			m_bSameSz = oReader.GetBool();
		else if (GetAttributeName(EAttribute::SameGap, eType) == sAttributeName)
			m_shSameGap = oReader.GetInt();
	}
	END_READ_ATTRIBUTES(oReader)

	if (!m_bSameSz)
	{
		m_arColSzWidths.resize(m_shColCount);
		m_arColSzGaps.resize(m_shColCount);
	}

	unsigned int unColSzIndex = 0;

	WHILE_READ_NEXT_NODE_WITH_NAME(oReader)
	{
		if (GetNodeName(ENode::ColumnLine, eType) == sNodeName)
		{
			START_READ_ATTRIBUTES(oReader)
			{
				if (GetAttributeName(EAttribute::Type, eType) == sAttributeName)
					m_eColLineStyle = GetLineStyle2(oReader.GetText());
				else if (GetAttributeName(EAttribute::Width, eType) == sAttributeName)
					m_chColLineWidth = (HWP_BYTE)ConvertWidthToHWP(oReader.GetTextA());
				else if (GetAttributeName(EAttribute::Color, eType) == sAttributeName)
					m_nColLineColor = oReader.GetColor();
			}
			END_READ_ATTRIBUTES(oReader)
		}
		else if (unColSzIndex >= m_shColCount)
			break;
		else if (EHanType::HWPX == eType && "hp:colSz" == sNodeName)
			ReadColumn(oReader, eType, unColSzIndex);
		else if (EHanType::HWPML == eType && "COLUMNTABLE" == sNodeName)
		{
			WHILE_READ_NEXT_NODE_WITH_DEPTH_ONE_NAME(oReader, Child, "COLUMN")
				ReadColumn(oReader, eType, unColSzIndex);
			END_WHILE
		}
	}
	END_WHILE

	m_bFullFilled = true;
}

void CCtrlColumnDef::ReadColumn(CXMLReader &oReader, EHanType eType, unsigned int& unColSzIndex)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if (GetAttributeName(EAttribute::Width, eType) == sAttributeName)
			m_arColSzWidths[unColSzIndex] = oReader.GetInt();
		else if (GetAttributeName(EAttribute::Gap, eType) == sAttributeName)
			m_arColSzGaps[unColSzIndex] =  oReader.GetInt();
	}
	END_READ_ATTRIBUTES(oReader)

	++unColSzIndex;
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
