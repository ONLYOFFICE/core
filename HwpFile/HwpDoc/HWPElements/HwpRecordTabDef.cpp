#include "HwpRecordTabDef.h"

#include "../Common/NodeNames.h"

namespace HWP
{
TTab::TTab()
{}

TTab::TTab(CXMLReader& oReader)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if ("pos" == sAttributeName)
			m_nPos = oReader.GetInt();
		else if ("type" == sAttributeName)
			SetType(oReader.GetInt());
		else if ("leader" == sAttributeName)
			m_eLeader = GetLineStyle2(oReader.GetTextA(), EHanType::HWPX);
	}
	END_READ_ATTRIBUTES(oReader)
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
	: CHWPRecord(nTagNum, nLevel, nSize), m_pParent(nullptr), m_nAttr(0)
{}

CHwpRecordTabDef::CHwpRecordTabDef(CHWPDocInfo& oDocInfo, int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CHWPRecord(nTagNum, nLevel, nSize), m_pParent(&oDocInfo)
{
	oBuffer.SavePosition();

	oBuffer.ReadInt(m_nAttr);

	int nCount = 0;
	oBuffer.ReadInt(nCount);

	if (nSize - oBuffer.GetDistanceToLastPos() != nCount * 8)
		return; // TODO:: ошибка

	TTab *pTab = nullptr;
	for (unsigned int unIndex = 0; unIndex < nCount; ++unIndex)
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

CHwpRecordTabDef::CHwpRecordTabDef(CHWPDocInfo& oDocInfo, CXMLReader& oReader, int nVersion, EHanType eType)
	: CHWPRecord(EHWPTag::HWPTAG_TAB_DEF, 0, 0), m_pParent(&oDocInfo), m_nAttr(0)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if (GetAttributeName(EAttribute::AutoTabLeft, eType) == sAttributeName)
		{
			if (oReader.GetBool())
				m_nAttr |= 0x00000001;
			else
				m_nAttr &= 0xFFFFFFFE;
		}
		else if (GetAttributeName(EAttribute::AutoTabRight, eType) == sAttributeName)
		{
			if (oReader.GetBool())
				m_nAttr |= 0x00000002;
			else
				m_nAttr &= 0xFFFFFFFD;
		}
	}
	END_READ_ATTRIBUTES(oReader)

	// Дальнейшая структура встречается лишь в HWPX формате
	if (EHanType::HWPX != eType)
		return;
	
	WHILE_READ_NEXT_NODE_WITH_ONE_NAME(oReader, "hp:switch")
		WHILE_READ_NEXT_NODE_WITH_DEPTH_ONE_NAME(oReader, Child, "hp:default")
			WHILE_READ_NEXT_NODE_WITH_DEPTH_ONE_NAME(oReader, TabChild, "hh:tabItem")
				m_arTabs.push_back(new TTab(oReader));
			END_WHILE
		END_WHILE
	END_WHILE
}

int CHwpRecordTabDef::GetCount() const
{
	return m_arTabs.size();
}

const TTab* CHwpRecordTabDef::GetTab(unsigned int unIndex) const
{
	if (unIndex >= m_arTabs.size())
		return nullptr;

	return dynamic_cast<const TTab*>(m_arTabs.at(unIndex));
}
}
