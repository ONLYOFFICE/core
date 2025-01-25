#include "CtrlHeadFoot.h"

namespace HWP
{
EPageRange GetPageRange(int nValue)
{
	switch(static_cast<EPageRange>(nValue))
	{
		case EPageRange::BOTH: default: return EPageRange::BOTH;
		case EPageRange::EVEN: return EPageRange::EVEN;
		case EPageRange::ODD: return EPageRange::ODD;
	}
}

CCtrlHeadFoot::CCtrlHeadFoot(const HWP_STRING& sCtrlID)
	: CCtrl(sCtrlID)
{}

CCtrlHeadFoot::CCtrlHeadFoot(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion, bool bIsHeader)
	: CCtrl(sCtrlID), m_bIsHeader(bIsHeader)
{
	oBuffer.ReadInt(m_nAttr);
	m_eWhichPage = GetPageRange(m_nAttr & 0x03);
	oBuffer.ReadInt(m_nSerialInSec);
}

CCtrlHeadFoot::CCtrlHeadFoot(const HWP_STRING& sCtrlID, CXMLNode& oNode, int nVersion)
	: CCtrl(sCtrlID)
{
	m_bIsHeader = L"daeh" == sCtrlID;

	m_eWhichPage = GetPageRange(oNode.GetAttributeInt(L"applyPageType"));

	for (CXMLNode& oChild : oNode.GetChilds(L"hp:subList"))
	{
		m_chRefLevelNum = (HWP_BYTE)oChild.GetAttributeInt(L"hasNumRef");
		m_chRefLevelText = (HWP_BYTE)oChild.GetAttributeInt(L"hasTextRef");
		m_nTextHeight = oChild.GetAttributeInt(L"textHeight");
		m_nTextWidth = oChild.GetAttributeInt(L"textWidth");

		for (CXMLNode& oGrandChild : oChild.GetChilds(L"hp:p"))
			m_arParas.push_back(new CHWPPargraph(oGrandChild, nVersion));
	}

	m_bFullFilled = true;
}

ECtrlObjectType CCtrlHeadFoot::GetCtrlType() const
{
	return ECtrlObjectType::HeadFoot;
}

bool CCtrlHeadFoot::IsHeader() const
{
	return m_bIsHeader;
}

void CCtrlHeadFoot::AddParagraph(CHWPPargraph* pParagraph)
{
	m_arParas.push_back(pParagraph);
}

VECTOR<const CHWPPargraph*> CCtrlHeadFoot::GetParagraphs() const
{
	VECTOR<const CHWPPargraph*> arParagraphs(m_arParas.size());

	for (unsigned int unIndex = 0; unIndex < m_arParas.size(); ++unIndex)
		arParagraphs[unIndex] = dynamic_cast<const CHWPPargraph*>(m_arParas[unIndex]);

	return arParagraphs;
}

int CCtrlHeadFoot::ParseListHeaderAppend(CCtrlHeadFoot& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	oBuffer.SavePosition();

	oBuffer.Skip(2);
	oBuffer.ReadInt(oObj.m_nTextWidth);
	oBuffer.ReadInt(oObj.m_nTextHeight);
	oBuffer.ReadByte(oObj.m_chRefLevelText);
	oBuffer.ReadByte(oObj.m_chRefLevelNum);

	if (nSize - oBuffer.GetDistanceToLastPos())
		oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos());

	oObj.m_bFullFilled = true;

	return oBuffer.GetDistanceToLastPos(true);
}
}
