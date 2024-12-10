#ifndef HWPRECORDSTYLE_H
#define HWPRECORDSTYLE_H

#include "HWPRecord.h"
#include "../HWPDocInfo.h"

namespace HWP
{
class CHWPRecordStyle : public CHWPRecord
{
	CHWPDocInfo* m_pParent;

	STRING m_sName;
	STRING m_sEngName;
	BYTE m_chType;
	BYTE m_chNextStyle;
	short m_shLangID;
	int m_nParaShape;
	int m_nCharShape;
	bool m_bLockForm;
public:
	CHWPRecordStyle(int nTagNum, int nLevel, int nSize);
	CHWPRecordStyle(CHWPDocInfo& oDocInfo, int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // HWPRECORDSTYLE_H
