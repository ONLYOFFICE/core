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

CCtrlHeadFoot::CCtrlHeadFoot(const HWP_STRING& sCtrlID, CXMLReader& oReader, int nVersion, EHanType eType)
	: CCtrl(sCtrlID)
{
	m_bIsHeader = L"daeh" == sCtrlID;

	switch (eType)
	{
		case EHanType::HWPX: ReadFromHWPX(oReader, nVersion); break;
		case EHanType::HWPML:ReadFromHWPML(oReader); break;
	}

	m_bFullFilled = true;
}

void CCtrlHeadFoot::ReadFromHWPX(CXMLReader &oReader, int nVersion)
{
	m_eWhichPage = GetPageRange(oReader.GetAttributeInt("applyPageType"));

	WHILE_READ_NEXT_NODE_WITH_ONE_NAME(oReader, "hp:subList")
	{
		START_READ_ATTRIBUTES(oReader)
		{
			if ("hasNumRef" == sAttributeName)
				m_chRefLevelNum = (HWP_BYTE)oReader.GetInt();
			else if ("hasTextRef" == sAttributeName)
				m_chRefLevelText = (HWP_BYTE)oReader.GetInt();
			else if ("textHeight" == sAttributeName)
				m_nTextHeight = oReader.GetInt();
			else if ("textWidth" == sAttributeName)
				m_nTextWidth = oReader.GetInt();
		}
		END_READ_ATTRIBUTES(oReader)

		WHILE_READ_NEXT_NODE_WITH_DEPTH_ONE_NAME(oReader, Child, "hp:p")
			m_arParas.push_back(new CHWPPargraph(oReader, nVersion, EHanType::HWPX));
		END_WHILE
	}
	END_WHILE
}

void CCtrlHeadFoot::ReadFromHWPML(CXMLReader &oReader)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if ("ApplyPageType" == sAttributeName)
		{
			const std::string sValue{oReader.GetTextA()};

			if ("Both" == sValue)
				m_eWhichPage = EPageRange::BOTH;
			else if ("Even" == sValue)
				m_eWhichPage = EPageRange::EVEN;
			else if ("Odd" == sValue)
				m_eWhichPage = EPageRange::ODD;
		}
		else if ("TextHeight" == sAttributeName)
			m_nTextHeight = oReader.GetInt();
		else if ("TextWidth" == sAttributeName)
			m_nTextWidth = oReader.GetInt();
		//TODO:: Реализовать HasTextRef и HasTextRef
	}
	END_READ_ATTRIBUTES(oReader)

	WHILE_READ_NEXT_NODE_WITH_ONE_NAME(oReader, "PARALIST")
		WHILE_READ_NEXT_NODE_WITH_DEPTH_ONE_NAME(oReader, Child, "P")
			m_arParas.push_back(new CHWPPargraph(oReader, 0, EHanType::HWPML));
		END_WHILE
	END_WHILE
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
