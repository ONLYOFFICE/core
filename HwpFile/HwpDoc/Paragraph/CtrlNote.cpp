#include "CtrlNote.h"

#include "../Common/NodeNames.h"

namespace HWP
{
CCtrlNote::CCtrlNote()
{}

CCtrlNote::CCtrlNote(const HWP_STRING& sCtrlID)
	: CCtrl(sCtrlID)
{}

CCtrlNote::CCtrlNote(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrl(sCtrlID)
{
	oBuffer.Skip(8);
	m_bFullFilled = true;
}

CCtrlNote::CCtrlNote(const HWP_STRING& sCtrlID, CXMLReader& oReader, EHanType eType)
	: CCtrl(sCtrlID)
{
	WHILE_READ_NEXT_NODE_WITH_ONE_NAME(oReader, GetNodeName(ENode::ParaList, eType))
		WHILE_READ_NEXT_NODE_WITH_DEPTH_ONE_NAME(oReader, Child, GetNodeName(ENode::Paragraph, eType))
			m_arParas.push_back(new CHWPPargraph(oReader, eType));
		END_WHILE
	END_WHILE

	m_bFullFilled = true;
}

ECtrlObjectType CCtrlNote::GetCtrlType() const
{
	return ECtrlObjectType::Note;
}

std::vector<const CHWPPargraph*> CCtrlNote::GetParagraphs() const
{
	std::vector<const CHWPPargraph*> arParagraphs(m_arParas.size());

	for (unsigned int unIndex = 0; unIndex < m_arParas.size(); ++unIndex)
		arParagraphs[unIndex] = dynamic_cast<const CHWPPargraph*>(m_arParas[unIndex]);

	return arParagraphs;
}

void CCtrlNote::AddParagraph(CHWPPargraph* pParagraph)
{
	m_arParas.push_back(pParagraph);
}
}
