#include "PageBorderFill.h"

namespace HWP
{
CPageBorderFill::CPageBorderFill()
{}

CPageBorderFill::CPageBorderFill(CXMLReader& oReader)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if ("borderFillIDRef" == sAttributeName)
			m_shBorderFill = oReader.GetInt();
		else if ("textBorder" == sAttributeName)
			m_bTextBorder = "PAPER" == oReader.GetTextA();
		else if ("headerInside" == sAttributeName)
			m_bHeaderInside = oReader.GetBool();
		else if ("footerInside" == sAttributeName)
			m_bFooterInside = oReader.GetBool();
		else if ("fillArea" == sAttributeName)
		{
			const std::string sType{oReader.GetTextA()};

			if ("PAPER" == sType)
				m_chFillArea = 0;
			else if ("PAGE" == sType)
				m_chFillArea = 1;
			else if ("BORDER" == sType)
				m_chFillArea = 2;
		}
	}
	END_READ_ATTRIBUTES(oReader)

	WHILE_READ_NEXT_NODE_WITH_ONE_NAME(oReader, "offset")
	{
		START_READ_ATTRIBUTES(oReader)
		{
			if ("left" == sAttributeName)
				m_shOffsetLeft = oReader.GetInt();
			else if ("right" == sAttributeName)
				m_shOffsetRight = oReader.GetInt();
			else if ("top" == sAttributeName)
				m_shOffsetTop = oReader.GetInt();
			else if ("bottom" == sAttributeName)
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
