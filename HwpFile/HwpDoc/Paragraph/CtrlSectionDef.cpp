#include "CtrlSectionDef.h"

namespace HWP
{
CCtrlSectionDef::CCtrlSectionDef(const HWP_STRING& sCtrlID)
	: CCtrl(sCtrlID), m_pPage(nullptr)
{}

CCtrlSectionDef::CCtrlSectionDef(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrl(sCtrlID), m_pPage(nullptr)
{
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
	m_chTextDirection = (HWP_BYTE)((nAttr >> 16) & 0x07);
	m_bHideEmptyLine = CHECK_FLAG(nAttr, 0x20000);
	m_chPageStartOn = (HWP_BYTE)((nAttr >> 20) & 0x03);

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
}

CCtrlSectionDef::~CCtrlSectionDef()
{
	if (nullptr != m_pPage)
		delete m_pPage;
}

ECtrlObjectType CCtrlSectionDef::GetCtrlType() const
{
	return ECtrlObjectType::SectionDef;
}

void CCtrlSectionDef::SetPage(CPage* pPage)
{
	m_pPage = pPage;
}

void CCtrlSectionDef::AddHeadFoot(CCtrlHeadFoot* pHeadFoot)
{
	m_arHeaderFooter.push_back(pHeadFoot);
}

void CCtrlSectionDef::AddParagraph(CHWPPargraph* pParagraph)
{
	m_arParas.push_back(pParagraph);
}

void CCtrlSectionDef::AddNoteShape(CNoteShape* pNoteShape)
{
	m_arNoteShapes.push_back(pNoteShape);
}

void CCtrlSectionDef::AddPageBorderFill(CPageBorderFill* pPageBorderFill)
{
	m_arBorderFills.push_back(pPageBorderFill);
}

const CPage* CCtrlSectionDef::GetPage() const
{
	return m_pPage;
}

std::vector<const CCtrlHeadFoot*> CCtrlSectionDef::GetHeaderFooters() const
{
	VECTOR<const CCtrlHeadFoot*> arHeaderFooters(m_arHeaderFooter.size());

	for (unsigned int unIndex = 0; unIndex < m_arHeaderFooter.size(); ++unIndex)
		arHeaderFooters[unIndex] = dynamic_cast<const CCtrlHeadFoot*>(m_arHeaderFooter[unIndex]);

	return arHeaderFooters;
}
}
