#include "CtrlSectionDef.h"

namespace HWP
{
CCtrlSectionDef::CCtrlSectionDef(const STRING& sCtrlID)
	: CCtrl(sCtrlID)
{}

CCtrlSectionDef::CCtrlSectionDef(const STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrl(sCtrlID)
{
	oBuffer.SavePosition();

	int nAttr;
	oBuffer.ReadInt(nAttr);

	m_bHideHeader = CHECK_FLAG(nAttr, 0x01);
	m_bHideFooter = CHECK_FLAG(nAttr, 0x02);
	m_bHideMasterPage = CHECK_FLAG(nAttr, 0x04);
	m_bHideBorder = CHECK_FLAG(nAttr, 0x08);
	m_bHideFill = CHECK_FLAG(nAttr, 0x10);
	m_bHidePageNumPos = CHECK_FLAG(nAttr, 0x20);
	m_bShowFirstBorder = CHECK_FLAG(nAttr, 0x100);
	m_bShowFirstFill = CHECK_FLAG(nAttr, 0x200);
	m_chTextDirection = (BYTE)((nAttr >> 16) & 0x07);
	m_bHideEmptyLine = CHECK_FLAG(nAttr, 0x20000);
	m_chPageStartOn = (BYTE)((nAttr >> 20) & 0x03);

	oBuffer.ReadShort(m_shSpaceColumns);
	oBuffer.ReadShort(m_shLineGrid);
	oBuffer.ReadShort(m_shCharGrid);
	oBuffer.ReadInt(m_nTabStop);
	m_nOutlineNumberingID = oBuffer.ReadShort();
	oBuffer.ReadShort(m_shPageNum);
	oBuffer.ReadShort(m_shFigure);
	oBuffer.ReadShort(m_shTable);
	oBuffer.ReadShort(m_shEquation);

	if (nVersion >= 5015)
		oBuffer.ReadShort(m_shLang);

	m_bFullFilled = true;
	m_nSize = oBuffer.GetDistanceToLastPos();
}

void CCtrlSectionDef::AddHeadFoot(CCtrlHeadFoot* pHeadFoot)
{
	m_arHeaderFooter.push_back(pHeadFoot);
}

int CCtrlSectionDef::GetSize()
{
	return m_nSize;
}
}
