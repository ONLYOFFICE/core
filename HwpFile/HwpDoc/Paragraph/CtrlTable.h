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
	VECTOR<short> m_arRowSize;
	short m_shBorderFillID;
	short m_shValidZoneSize;
	VECTOR<TCellZone*> m_arCellzoneList;
	VECTOR<CTblCell*> m_arCells;
public:
	CCtrlTable(const HWP_STRING& sCtrlID);
	CCtrlTable(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	CCtrlTable(const HWP_STRING& sCtrlID, CXMLReader& oReader, int nVersion, EHanType eType);
	~CCtrlTable();

	ECtrlObjectType GetCtrlType() const override;

	bool Empty() const;
	short GetRows() const;
	short GetCols() const;
	short GetColsInRow(short shRowIndex) const;
	short GetCountCells() const;
	short GetBorderFillID() const;

	short GetInLSpace() const;
	short GetInRSpace() const;
	short GetInTSpace() const;
	short GetInBSpace() const;

	void AddCell(CTblCell* pCell);

	bool HaveCells();

	const CTblCell* GetCell(unsigned int unIndex) const;
	CTblCell* GetLastCell();

	static int ParseCtrl(CCtrlTable& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int ParseListHeaderAppend(CCtrlTable& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // CTRLTABLE_H
