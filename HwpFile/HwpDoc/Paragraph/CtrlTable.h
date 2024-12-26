#ifndef CTRLTABLE_H
#define CTRLTABLE_H

#include "TblCell.h"

namespace HWP
{
struct TCellZone
{
	short m_shStartRowAddr;
	short m_shStartColAddr;;
	short m_shEndRowAddr;
	short m_shEndColAddr;
	short m_shBorderFillIDRef;
};

class CCtrlTable : public CCtrlCommon
{
	int m_nAttr;
	short m_shNRows;
	short m_shNCols;
	short m_shCellSpacing;
	short m_shInLSpace;
	short m_shInRSpace;
	short m_shInTSpace;
	short m_shInBSpace;
	std::vector<short> m_arRowSize;
	short m_shBorderFillID;
	short m_shValidZoneSize;
	std::list<TCellZone*> m_arCellzoneList;
	std::list<CTblCell*> m_arCells;
public:
	CCtrlTable(const HWP_STRING& sCtrlID);
	CCtrlTable(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	~CCtrlTable();

	void AddCell(CTblCell* pCell);

	bool HaveCells();

	CTblCell* GetLastCell();

	static int ParseCtrl(CCtrlTable& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int ParseListHeaderAppend(CCtrlTable& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // CTRLTABLE_H
