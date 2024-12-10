#ifndef CTRLTALBE_H
#define CTRLTALBE_H

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

class CCtrlTalbe : public CCtrlCommon
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
	CCtrlTalbe(const STRING& sCtrlID);
	CCtrlTalbe(const STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	~CCtrlTalbe();

	static int ParseCtrl(CCtrlTalbe& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
	static int ParseListHeaderAppend(CCtrlTalbe& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion);
};
}

#endif // CTRLTALBE_H
