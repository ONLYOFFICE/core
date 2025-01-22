#include "HWPRecordDocumentProperties.h"

namespace HWP
{
CHWPRecordDocumentProperties::CHWPRecordDocumentProperties(CHWPDocInfo& oDocInfo, int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CHWPRecord(nTagNum, nLevel, nSize), m_pParent(&oDocInfo)
{
	oBuffer.ReadShort(m_shSectionSize);
	oBuffer.ReadShort(m_shPageStartNo);
	oBuffer.ReadShort(m_shFootNoteStartNo);
	oBuffer.ReadShort(m_shEndNoteStartNo);
	oBuffer.ReadShort(m_shFigureStartNo);
	oBuffer.ReadShort(m_shTableStartNo);
	oBuffer.ReadInt(m_nListID);
	oBuffer.ReadInt(m_nParaID);
	oBuffer.ReadInt(m_nCharUnitLocInPara);
}

CHWPRecordDocumentProperties::CHWPRecordDocumentProperties(CHWPDocInfo& oDocInfo, CXMLNode& oNode, int nVersion)
	: CHWPRecord(EHWPTag::HWPTAG_DOCUMENT_PROPERTIES, 0, 0), m_pParent(&oDocInfo)
{
	m_shFigureStartNo   = oNode.GetAttributeInt(L"page");
	m_shFootNoteStartNo = oNode.GetAttributeInt(L"footnote");
	m_shEndNoteStartNo  = oNode.GetAttributeInt(L"endnote");
	m_shFigureStartNo   = oNode.GetAttributeInt(L"pic");
	m_shTableStartNo    = oNode.GetAttributeInt(L"tbl");
	m_shEqStartNo       = oNode.GetAttributeInt(L"equation");
}
}
