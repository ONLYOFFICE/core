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

CCtrlSectionDef::CCtrlSectionDef(const HWP_STRING& sCtrlID, CXMLReader& oReader)
	: CCtrl(sCtrlID), m_pPage(nullptr)
{
	std::string sType;

	START_READ_ATTRIBUTES(oReader)
	{
		if ("textDirection" == sAttributeName)
		{
			sType = oReader.GetTextA();

			if ("HORIZONTAL" == sType)
				m_chTextDirection = 0;
			else if ("VERTICAL" == sType)
				m_chTextDirection = 1;
		}
		else if ("spaceColumns" == sAttributeName)
			m_shSpaceColumns = oReader.GetInt();
		else if ("tabStop" == sAttributeName)
			m_nTabStop = oReader.GetInt();
		else if ("outlineShapeIDRef" == sAttributeName)
			m_nOutlineNumberingID = oReader.GetInt();
	}
	END_READ_ATTRIBUTES(oReader)

	WHILE_READ_NEXT_NODE_WITH_NAME(oReader)
	{
		if ("hp:startNum" == sNodeName)
		{
			START_READ_ATTRIBUTES(oReader)
			{
				if ("pageStartsOn" == sAttributeName)
				{
					sType = oReader.GetTextA();

					if ("BOTH" == sType)
						m_chPageStartOn = 0;
					else if ("EVEN" == sType)
						m_chPageStartOn = 1;
					else if ("ODD" == sType)
						m_chPageStartOn = 2;
				}
				else if ("page" == sAttributeName)
					m_shPageNum = oReader.GetInt();
				else if ("pic" == sAttributeName)
					m_shFigure = oReader.GetInt();
				else if ("tbl" == sAttributeName)
					m_shTable = oReader.GetInt();
				else if ("equation" == sAttributeName)
					m_shEquation = oReader.GetInt();
			}
			END_READ_ATTRIBUTES(oReader)
		}
		else if ("hp:grid" == sNodeName)
		{
			START_READ_ATTRIBUTES(oReader)
			{
				if ("lineGrid" == sAttributeName)
					m_shLineGrid = oReader.GetInt();
				else if ("charGrid" == sAttributeName)
					m_shCharGrid = oReader.GetInt();
			}
			END_READ_ATTRIBUTES(oReader)
		}
		else if ("hp:visibility" == sNodeName)
		{
			START_READ_ATTRIBUTES(oReader)
			{
				if ("hideFirstHeader" == sAttributeName)
					m_bHideHeader = oReader.GetBool();
				else if ("hideFirstFooter" == sAttributeName)
					m_bHideFooter = oReader.GetBool();
				else if ("hideFirstMasterPage" == sAttributeName)
					m_bHideMasterPage = oReader.GetBool();
				else if ("border" == sAttributeName)
				{
					sType = oReader.GetTextA();

					if ("HIDE_FIRST" == sType)
					{
						m_bHideBorder = true;
						m_bShowFirstBorder = false;
					}
					else if ("SHOW_FIRST" == sType)
					{
						m_bHideBorder = true;
						m_bShowFirstBorder = true;
					}
					else if ("SHOW_ALL" == sType)
					{
						m_bHideBorder = false;
						m_bShowFirstBorder = false;
					}
				}
				else if ("fill" == sAttributeName)
				{
					sType = oReader.GetTextA();

					if ("HIDE_FIRST" == sType)
					{
						m_bHideFill = true;
						m_bShowFirstFill = false;
					}
					else if ("SHOW_FIRST" == sType)
					{
						m_bHideFill = true;
						m_bShowFirstFill = true;
					}
					else if ("SHOW_ALL" == sType)
					{
						m_bHideFill = false;
						m_bShowFirstFill = false;
					}
				}
				else if ("hp:visibility" == sAttributeName)
					m_bHidePageNumPos = oReader.GetBool();
				else if ("hideFirstEmptyLine" == sAttributeName)
					m_bHideEmptyLine = oReader.GetBool();
			}
			END_READ_ATTRIBUTES(oReader)
		}
		else if ("hp:pagePr" == sNodeName)
			m_pPage = new CPage(oReader);
		else if ("hp:footNotePr" == sNodeName ||
		         "hp:endNotePr"  == sNodeName)
			m_arNoteShapes.push_back(new CNoteShape(oReader));
		else if ("hp:pageBorderFill" == sNodeName)
			m_arBorderFills.push_back(new CPageBorderFill(oReader));
		else if ("hp:masterPage" == sNodeName)
		{
			//TODO:: добавить реализацию
		}
	}
	END_WHILE

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

VECTOR<const CCtrlHeadFoot*> CCtrlSectionDef::GetHeaderFooters() const
{
	VECTOR<const CCtrlHeadFoot*> arHeaderFooters(m_arHeaderFooter.size());

	for (unsigned int unIndex = 0; unIndex < m_arHeaderFooter.size(); ++unIndex)
		arHeaderFooters[unIndex] = dynamic_cast<const CCtrlHeadFoot*>(m_arHeaderFooter[unIndex]);

	return arHeaderFooters;
}
}
