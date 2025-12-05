#ifndef HWPRECORDNUMBERING_H
#define HWPRECORDNUMBERING_H

#include "../HWPDocInfo.h"
#include "../HWPStream.h"
#include "HWPRecord.h"
#include "../Common/XMLReader.h"

namespace HWP
{

struct TParaHeadInfo
{
	HWP_BYTE m_chAlign;
	bool m_bUseInstWidth;
	bool m_bAutoIndent;
	HWP_BYTE m_chTextOffsetType;
	short m_shWidthAdjust;
	short m_shTextOffset;
	int m_nCharShape;
	int m_nStartNumber;
};

struct TNumbering : public TParaHeadInfo
{
	HWP_STRING m_sNumFormat;
};

class CHWPRecordNumbering : public CHWPRecord
{
	CHWPDocInfo *m_pParent;

	TNumbering m_arNumbering[7];
	short m_shStart;
	HWP_STRING m_arExtLevelFormat[3];
	int m_arExtLevelStart[3];
public:
	CHWPRecordNumbering(CHWPDocInfo& oDocInfo, int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	CHWPRecordNumbering(CHWPDocInfo& oDocInfo, CXMLReader& oReader, EHanType eType);

	short GetStart() const;
	HWP_STRING GetNumFormat(unsigned short ushIndex) const;
	HWP_BYTE GetAlign(unsigned short ushIndex) const;
	int GetStartNumber(unsigned short ushIndex) const;
	int GetCharShape(unsigned short ushIndex) const;
};
}

#endif // HWPRECORDNUMBERING_H
