#ifndef HWPRECORDDOCUMENTPROPERTIES_H
#define HWPRECORDDOCUMENTPROPERTIES_H

#include "../HWPDocInfo.h"
#include "../HWPStream.h"
#include "HWPRecord.h"
#include "../Common/XMLReader.h"

namespace HWP
{
class CHWPRecordDocumentProperties : public CHWPRecord
{
	CHWPDocInfo *m_pParent;

	short m_shSectionSize;
	short m_shPageStartNo;
	short m_shFootNoteStartNo;
	short m_shEndNoteStartNo;
	short m_shFigureStartNo;
	short m_shTableStartNo;
	short m_shEqStartNo;
	int m_nListID;
	int m_nParaID;
	int m_nCharUnitLocInPara;
public:
	CHWPRecordDocumentProperties(CHWPDocInfo& oDocInfo, int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	CHWPRecordDocumentProperties(CHWPDocInfo& oDocInfo, CXMLReader& oReader);
};
}

#endif // HWPRECORDDOCUMENTPROPERTIES_H
