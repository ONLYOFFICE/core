#include "PageBorderFill.h"

#include "../Common/NodeNames.h"

namespace HWP
{
CPageBorderFill::CPageBorderFill()
{}

CPageBorderFill::CPageBorderFill(CXMLReader& oReader, EHanType eType)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if (GetAttributeName(EAttribute::BorderFillId, eType) == sAttributeName)
			m_shBorderFill = oReader.GetInt();
		else if (GetAttributeName(EAttribute::TextBorder, eType) == sAttributeName)
		{
			if (EHanType::HWPX == eType)
				m_bTextBorder = "PAPER" == oReader.GetTextA();
			else
				m_bTextBorder = oReader.GetBool();
		}
		else if (GetAttributeName(EAttribute::HeaderInside, eType) == sAttributeName)
			m_bHeaderInside = oReader.GetBool();
		else if (GetAttributeName(EAttribute::FooterInside, eType) == sAttributeName)
			m_bFooterInside = oReader.GetBool();
		else if (GetAttributeName(EAttribute::FillArea, eType) == sAttributeName)
		{
			const std::string sType{oReader.GetTextA()};

			if (GetValueName(EValue::Paper, eType) == sType)
				m_chFillArea = 0;
			else if (GetValueName(EValue::Page, eType) == sType)
				m_chFillArea = 1;
			else if (GetValueName(EValue::Border, eType) == sType)
				m_chFillArea = 2;
		}
	}
	END_READ_ATTRIBUTES(oReader)

	WHILE_READ_NEXT_NODE_WITH_ONE_NAME(oReader, GetNodeName(ENode::PageOffset, eType))
	{
		START_READ_ATTRIBUTES(oReader)
		{
			if (GetAttributeName(EAttribute::Left, eType) == sAttributeName)
				m_shOffsetLeft = oReader.GetInt();
			else if (GetAttributeName(EAttribute::Right, eType) == sAttributeName)
				m_shOffsetRight = oReader.GetInt();
			else if (GetAttributeName(EAttribute::Top, eType) == sAttributeName)
				m_shOffsetTop = oReader.GetInt();
			else if (GetAttributeName(EAttribute::Bottom, eType) == sAttributeName)
				m_shOffsetBottom = oReader.GetInt();
		}
		END_READ_ATTRIBUTES(oReader)
	}
	END_WHILE
}

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
