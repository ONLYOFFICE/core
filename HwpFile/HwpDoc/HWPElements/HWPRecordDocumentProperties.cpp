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

CHWPRecordDocumentProperties::CHWPRecordDocumentProperties(CHWPDocInfo& oDocInfo, CXMLReader& oReader)
	: CHWPRecord(EHWPTag::HWPTAG_DOCUMENT_PROPERTIES, 0, 0), m_pParent(&oDocInfo)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if ("page" == sAttributeName)
			m_shPageStartNo = oReader.GetInt();
		else if ("footnote" == sAttributeName)
			m_shFootNoteStartNo = oReader.GetInt();
		else if ("endnote" == sAttributeName)
			m_shEndNoteStartNo = oReader.GetInt();
		else if ("pic" == sAttributeName)
			m_shFigureStartNo = oReader.GetInt();
		else if ("tbl" == sAttributeName)
			m_shTableStartNo = oReader.GetInt();
		else if ("equation" == sAttributeName)
			m_shEqStartNo = oReader.GetInt();
	}
	END_READ_ATTRIBUTES(oReader)
}
}
