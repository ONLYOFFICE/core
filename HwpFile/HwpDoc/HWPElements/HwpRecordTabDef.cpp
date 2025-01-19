#include "HwpRecordTabDef.h"

namespace HWP
{
TTab::TTab()
{}

TTab::TTab(CXMLNode& oNode)
{
	m_nPos = oNode.GetAttributeInt(L"pos");
	SetType(oNode.GetAttributeInt(L"type"));
	m_eLeader = GetLineStyle2(oNode.GetAttribute(L"leader"));
}

void TTab::SetType(int nValue)
{
	SWITCH(TTab::EType, nValue)
	{
		case EType::LEFT: default: m_eType = EType::LEFT; break;
		case EType::RIGHT: m_eType = EType::RIGHT; break;
		case EType::CENTER: m_eType = EType::CENTER; break;
		case EType::DECIMAL: m_eType = EType::DECIMAL; break;
	}
}

CHwpRecordTabDef::CHwpRecordTabDef(int nTagNum, int nLevel, int nSize)
	: CHWPRecord(nTagNum, nLevel, nSize), m_pParent(nullptr)
{}

CHwpRecordTabDef::CHwpRecordTabDef(CHWPDocInfo& oDocInfo, int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CHWPRecord(nTagNum, nLevel, nSize), m_pParent(&oDocInfo)
{
	oBuffer.SavePosition();

	oBuffer.ReadInt(m_nAttr);
	oBuffer.ReadInt(m_nCount);

	if (nSize - oBuffer.GetDistanceToLastPos() != m_nCount * 8)
		return; // TODO:: ошибка

	TTab *pTab = nullptr;
	for (unsigned int unIndex = 0; unIndex < m_nCount; ++unIndex)
	{
		pTab = new TTab();

		if (nullptr == pTab)
		{
			oBuffer.Skip(8);
			continue;
		}

		oBuffer.ReadInt(pTab->m_nPos);
		pTab->SetType(oBuffer.ReadByte());
		pTab->m_eLeader = GetLineStyle2(oBuffer.ReadByte());
		oBuffer.Skip(2); // reserved 2 bytes for align

		m_arTabs.push_back(pTab);
	}

	oBuffer.RemoveLastSavedPos();
}

CHwpRecordTabDef::CHwpRecordTabDef(CHWPDocInfo& oDocInfo, CXMLNode& oNode, int nVersion)
	: CHWPRecord(EHWPTag::HWPTAG_TAB_DEF, 0, 0), m_pParent(&oDocInfo)
{
	if (oNode.GetAttributeBool(L"autoTabLeft"))
		m_nAttr |= 0x00000001;
	else
		m_nAttr &= 0xFFFFFFFE;

	if (oNode.GetAttributeBool(L"autoTabRight"))
		m_nAttr |= 0x00000002;
	else
		m_nAttr &= 0xFFFFFFFD;

	for (CXMLNode& oChild : oNode.GetChilds(L"hp:switch"))
		for (CXMLNode& oGrandChild : oChild.GetChilds(L"hp:default"))
		{
			CXMLNode oTabItem{oGrandChild.GetChild(L"hh:tabItem")};
			m_arTabs.push_back(new TTab(oTabItem));
		}
}
}
