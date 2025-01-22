#include "PageBorderFill.h"

namespace HWP
{
CPageBorderFill::CPageBorderFill()
{}

CPageBorderFill::CPageBorderFill(CXMLNode& oNode, int nVersion)
{
	m_shBorderFill = oNode.GetAttributeInt(L"borderFillIDRef");
	m_bTextBorder = L"PAPER" == oNode.GetAttribute(L"textBorder");
	m_bHeaderInside = oNode.GetAttributeBool(L"headerInside");
	m_bFooterInside = oNode.GetAttributeBool(L"footerInside");

	HWP_STRING sType = oNode.GetAttribute(L"fillArea");

	if (L"PAPER" == sType)
		m_chFillArea = 0;
	else if (L"PAGE" == sType)
		m_chFillArea = 1;
	else if (L"BORDER" == sType)
		m_chFillArea = 2;

	for (CXMLNode& oChild : oNode.GetChilds(L"offset"))
	{
		m_shOffsetLeft   = oChild.GetAttributeInt(L"left");
		m_shOffsetRight  = oChild.GetAttributeInt(L"right");
		m_shOffsetTop    = oChild.GetAttributeInt(L"top");
		m_shOffsetBottom = oChild.GetAttributeInt(L"bottom");
	}
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
