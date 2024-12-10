#ifndef HWPRECORDTABDEF_H
#define HWPRECORDTABDEF_H

#include "HWPRecord.h"
#include "../HWPDocInfo.h"
#include "HwpRecordTypes.h"

namespace HWP
{
struct TTab
{
	int m_nPos;
	enum class EType
	{
		LEFT,
		RIGHT,
		CENTER,
		DECIMAL
	} m_eType;
	ELineStyle2 m_eLeader;

	void SetType(int nValue);
};

class CHwpRecordTabDef : public CHWPRecord
{
	CHWPDocInfo *m_pParent;

	int m_nAttr;
	int m_nCount;
	LIST<TTab*> m_arTabs;
public:
	CHwpRecordTabDef(int nTagNum, int nLevel, int nSize);
	CHwpRecordTabDef(CHWPDocInfo& oDocInfo, int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // HWPRECORDTABDEF_H
