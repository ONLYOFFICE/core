#include "CtrlTalbe.h"

namespace HWP
{
CCtrlTalbe::CCtrlTalbe(const std::string& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrlCommon(sCtrlID, nSize, oBuffer, nOff, nVersion)
{}

CCtrlTalbe::~CCtrlTalbe()
{
	for (TCellZone* pCellzone : m_arCellzoneList)
	{
		if (nullptr != pCellzone)
			delete pCellzone;
	}

	for (CTblCell* pTblCell : m_arCells)
	{
		if (nullptr != pTblCell)
			delete pTblCell;
	}
}

int CCtrlTalbe::ParseCtrl(CCtrlTalbe& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	BYTE *pOldCurentPos = oBuffer.GetCurPtr();

	oBuffer.ReadInt(oObj.m_nAttr);
	oBuffer.ReadShort(oObj.m_shNRows);
	oBuffer.ReadShort(oObj.m_shNCols);
	oBuffer.ReadShort(oObj.m_shCellSpacing);
	oBuffer.ReadShort(oObj.m_shInLSpace);
	oBuffer.ReadShort(oObj.m_shInRSpace);
	oBuffer.ReadShort(oObj.m_shInTSpace);
	oBuffer.ReadShort(oObj.m_shInBSpace);

	oObj.m_arRowSize.reserve(oObj.m_shNRows);
	for (unsigned int unIndex = 0; unIndex < oObj.m_shNRows; ++unIndex)
		oBuffer.ReadShort(oObj.m_arRowSize[unIndex]);

	oBuffer.ReadShort(oObj.m_shBorderFillID);

	if (nVersion >= 5010 && ((oBuffer.GetCurPtr() - pOldCurentPos) < nSize))
	{
		oBuffer.ReadShort(oObj.m_shValidZoneSize);

		if (0 < oObj.m_shValidZoneSize && ((oBuffer.GetCurPtr() - pOldCurentPos) < nSize))
		{
			for (unsigned int unIndex = 0; unIndex < oObj.m_shValidZoneSize; ++unIndex)
			{
				TCellZone *pCellZone = new TCellZone();

				if (nullptr == pCellZone)
					continue;

				oBuffer.ReadShort(pCellZone->m_shStartRowAddr);
				oBuffer.ReadShort(pCellZone->m_shStartColAddr);
				oBuffer.ReadShort(pCellZone->m_shEndRowAddr);
				oBuffer.ReadShort(pCellZone->m_shEndColAddr);
				oBuffer.ReadShort(pCellZone->m_shBorderFillIDRef);

				oObj.m_arCellzoneList.push_back(pCellZone);
			}
		}
	}

	return nSize;
}

int CCtrlTalbe::ParseListHeaderAppend(CCtrlTalbe& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	if (24 != nSize)
		return nSize;

	oBuffer.Skip(2);
	oBuffer.ReadInt(oObj.m_nCaptionAttr);
	oBuffer.ReadInt(oObj.m_nCaptionWidth);
	oObj.m_nCaptionSpacing = oBuffer.ReadShort();
	oBuffer.ReadInt(oObj.m_nCaptionMaxW);
	oBuffer.Skip(8);

	return nSize;
}
}
