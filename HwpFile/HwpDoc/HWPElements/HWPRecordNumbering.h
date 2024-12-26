#ifndef HWPRECORDNUMBERING_H
#define HWPRECORDNUMBERING_H

#include "../HWPDocInfo.h"
#include "../HWPStream.h"
#include "HWPRecord.h"

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

	TNumbering m_arNumbering[10];
	short m_shStart;
	HWP_STRING m_arExtLevelFormat[3];
	int m_arExtLevelStart[3];
public:
	CHWPRecordNumbering(CHWPDocInfo& oDocInfo, int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // HWPRECORDNUMBERING_H
