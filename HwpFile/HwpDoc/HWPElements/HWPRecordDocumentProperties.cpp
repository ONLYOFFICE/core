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
}
