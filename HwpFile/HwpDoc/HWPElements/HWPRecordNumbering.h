#ifndef HWPRECORDNUMBERING_H
#define HWPRECORDNUMBERING_H

#include "../HWPDocInfo.h"
#include "../HWPStream.h"
#include "HWPRecord.h"

namespace HWP
{

struct TParaHeadInfo
{
	BYTE m_chAlign;
	bool m_bUseInstWidth;
	bool m_bAutoIndent;
	BYTE m_chTextOffsetType;
	short m_shWidthAdjust;
	short m_shTextOffset;
	int m_nCharShape;
	int m_nStartNumber;
};

struct TNumbering : public TParaHeadInfo
{
	STRING m_sNumFormat;
};

class CHWPRecordNumbering : public CHWPRecord
{
	CHWPDocInfo *m_pParent;

	TNumbering m_arNumbering[10];
	short m_shStart;
	STRING m_arExtLevelFormat[3];
	int m_arExtLevelStart[3];
public:
	CHWPRecordNumbering(CHWPDocInfo& oDocInfo, int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // HWPRECORDNUMBERING_H
