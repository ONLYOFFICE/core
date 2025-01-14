#include "HwpRecordTabDef.h"

namespace HWP
{
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

}
