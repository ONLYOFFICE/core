#ifndef HWPRECORDSTYLE_H
#define HWPRECORDSTYLE_H

#include "HWPRecord.h"
#include "../HWPDocInfo.h"
#include "../Common/XMLNode.h"

namespace HWP
{
class CHWPRecordStyle : public CHWPRecord
{
	CHWPDocInfo* m_pParent;

	HWP_STRING m_sName;
	HWP_STRING m_sEngName;
	HWP_BYTE m_chType;
	HWP_BYTE m_chNextStyle;
	short m_shLangID;
	int m_nParaShape;
	int m_nCharShape;
	bool m_bLockForm;
public:
	CHWPRecordStyle(int nTagNum, int nLevel, int nSize);
	CHWPRecordStyle(CHWPDocInfo& oDocInfo, int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	CHWPRecordStyle(CHWPDocInfo& oDocInfo, CXMLNode& oNode, int nVersion);
};
}

#endif // HWPRECORDSTYLE_H
