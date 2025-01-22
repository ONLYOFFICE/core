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

CCtrlSectionDef::CCtrlSectionDef(const HWP_STRING& sCtrlID, CXMLNode& oNode, int nVersion)
	: CCtrl(sCtrlID), m_pPage(nullptr)
{
	HWP_STRING sType = oNode.GetAttribute(L"textDirection");

	if (L"HORIZONTAL" == sType)
		m_chTextDirection = 0;
	else if (L"VERTICAL" == sType)
		m_chTextDirection = 1;

	m_shSpaceColumns = oNode.GetAttributeInt(L"spaceColumns");
	m_nTabStop = oNode.GetAttributeInt(L"tabStop");
	m_nOutlineNumberingID = oNode.GetAttributeInt(L"outlineShapeIDRef");

	for (CXMLNode& oChild : oNode.GetChilds())
	{
		if (L"hp:startNum" == oChild.GetName())
		{
			sType = oChild.GetAttribute(L"pageStartsOn");

			if (L"BOTH" == sType)
				m_chPageStartOn = 0;
			else if (L"EVEN" == sType)
				m_chPageStartOn = 1;
			else if (L"ODD" == sType)
				m_chPageStartOn = 2;

			m_shPageNum = oChild.GetAttributeInt(L"page");
			m_shFigure = oChild.GetAttributeInt(L"pic");
			m_shTable = oChild.GetAttributeInt(L"tbl");
			m_shEquation = oChild.GetAttributeInt(L"equation");
		}
		else if (L"hp:grid" == oChild.GetName())
		{
			m_shLineGrid = oChild.GetAttributeInt(L"lineGrid");
			m_shCharGrid = oChild.GetAttributeInt(L"charGrid");
		}
		else if (L"hp:visibility" == oChild.GetName())
		{
			m_bHideHeader = oChild.GetAttributeBool(L"hideFirstHeader");
			m_bHideFooter = oChild.GetAttributeBool(L"hideFirstFooter");
			m_bHideMasterPage = oChild.GetAttributeBool(L"hideFirstMasterPage");

			sType = oChild.GetAttribute(L"border");

			if (L"HIDE_FIRST" == sType)
			{
				m_bHideBorder = true;
				m_bShowFirstBorder = false;
			}
			else if (L"SHOW_FIRST" == sType)
			{
				m_bHideBorder = true;
				m_bShowFirstBorder = true;
			}
			else if (L"SHOW_ALL" == sType)
			{
				m_bHideBorder = false;
				m_bShowFirstBorder = false;
			}

			sType = oChild.GetAttribute(L"fill");

			if (L"HIDE_FIRST" == sType)
			{
				m_bHideFill = true;
				m_bShowFirstFill = false;
			}
			else if (L"SHOW_FIRST" == sType)
			{
				m_bHideFill = true;
				m_bShowFirstFill = true;
			}
			else if (L"SHOW_ALL" == sType)
			{
				m_bHideFill = false;
				m_bShowFirstFill = false;
			}

			m_bHidePageNumPos = oChild.GetAttributeBool(L"hideFirstPageNum");
			m_bHideEmptyLine = oChild.GetAttributeBool(L"hideFirstEmptyLine");
		}
		else if (L"hp:pagePr" == oChild.GetName())
			m_pPage = new CPage(oChild);
		else if (L"hp:footNotePr" == oChild.GetName() ||
		         L"hp:endNotePr"  == oChild.GetName())
			m_arNoteShapes.push_back(new CNoteShape(oChild, nVersion));
		else if (L"hp:pageBorderFill" == oChild.GetName())
			m_arBorderFills.push_back(new CPageBorderFill(oChild, nVersion));
		else if (L"hp:masterPage" == oChild.GetName())
		{
			//TODO:: добавить реализацию
		}
	}

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
