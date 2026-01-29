#include "MDWriter.h"

namespace HTML
{
CMDWriter::CMDWriter(const TMDParametrs& oMDParametrs)
	: m_oMDParametrs(oMDParametrs)
{
	m_arStates.push(TState{});
	m_arStates.top().m_pCurrentDocument = &m_oDocument;
}

void CMDWriter::Begin(const std::wstring& wsDst, const THtmlParams* pParams)
{

}

void CMDWriter::End(const std::wstring& wsDst)
{
}

bool CMDWriter::WriteText(const std::wstring& wsText, const std::vector<NSCSS::CNode>& arSelectors)
{
	GetCurrentDocument()->WriteString(wsText);
	return true;
}

void CMDWriter::WriteEmptyParagraph(bool bVahish, bool bInP)
{

}

void CMDWriter::BeginBlock()
{

}

void CMDWriter::EndBlock(bool bAddBlock)
{
	GetCurrentDocument()->WriteString(m_oMDParametrs.m_wsLineBreak);
}

void CMDWriter::SetDataOutput(XmlString* pOutputData)
{
	SaveState();
	m_arStates.top().m_pCurrentDocument = pOutputData;
}

void CMDWriter::RevertDataOutput()
{

	RollBackState();
}

TMDParametrs CMDWriter::GetParametrs() const
{
	return m_oMDParametrs;;
}

XmlString* CMDWriter::GetCurrentDocument() const
{
	return m_arStates.top().m_pCurrentDocument;
}

void CMDWriter::SaveState()
{
	m_arStates.push(m_arStates.top());
}

void CMDWriter::RollBackState()
{
	if (m_arStates.size() > 1)
		m_arStates.pop();
}
}
