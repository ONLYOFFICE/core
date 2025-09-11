#ifndef HWPRECORDSTYLE_H
#define HWPRECORDSTYLE_H

#include "HWPRecord.h"
#include "../HWPDocInfo.h"
#include "../Common/XMLReader.h"

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
	CHWPRecordStyle(CHWPDocInfo& oDocInfo, CXMLReader& oReader, int nVersion, EHanType eType);

	HWP_STRING GetName() const;
	HWP_STRING GetEngName() const;
	HWP_BYTE GetType() const;
	HWP_BYTE GetNextStyle() const;
	short GetLangId() const;
	int GetParaShapeId() const;
	int GetCharShapeId() const;
	bool LockForm() const;
};
}

#endif // HWPRECORDSTYLE_H
