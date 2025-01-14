#include "CtrlTable.h"

namespace HWP
{
CCtrlTable::CCtrlTable(const HWP_STRING& sCtrlID)
	: CCtrlCommon(sCtrlID)
{}

CCtrlTable::CCtrlTable(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrlCommon(sCtrlID, nSize, oBuffer, nOff, nVersion)
{}

CCtrlTable::~CCtrlTable()
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

ECtrlObjectType CCtrlTable::GetCtrlType() const
{
	return ECtrlObjectType::Table;
}

bool CCtrlTable::Empty() const
{
	return m_arCells.empty();
}

short CCtrlTable::GetRows() const
{
	return m_shNRows;
}

short CCtrlTable::GetCols() const
{
	return m_shNCols;
}

short CCtrlTable::GetColsInRow(short shRowIndex) const
{
	if (shRowIndex >= m_arRowSize.size())
		return 1;

	return m_arRowSize[shRowIndex];
}

short CCtrlTable::GetCountCells() const
{
	return m_arCells.size();
}

short CCtrlTable::GetBorderFillID() const
{
	return m_shBorderFillID;
}

void CCtrlTable::AddCell(CTblCell* pCell)
{
	m_arCells.push_back(pCell);
}

bool CCtrlTable::HaveCells()
{
	return !m_arCells.empty();
}

const CTblCell* CCtrlTable::GetCell(unsigned int unIndex) const
{
	if (unIndex >= m_arCells.size())
		return nullptr;

	return m_arCells[unIndex];
}

CTblCell* CCtrlTable::GetLastCell()
{
	return (!m_arCells.empty()) ? m_arCells.back() : nullptr;
}

int CCtrlTable::ParseCtrl(CCtrlTable& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	oBuffer.SavePosition();

	oBuffer.ReadInt(oObj.m_nAttr);
	oBuffer.ReadShort(oObj.m_shNRows);
	oBuffer.ReadShort(oObj.m_shNCols);
	oBuffer.ReadShort(oObj.m_shCellSpacing);
	oBuffer.ReadShort(oObj.m_shInLSpace);
	oBuffer.ReadShort(oObj.m_shInRSpace);
	oBuffer.ReadShort(oObj.m_shInTSpace);
	oBuffer.ReadShort(oObj.m_shInBSpace);

	oObj.m_arRowSize.resize(oObj.m_shNRows);
	for (unsigned int unIndex = 0; unIndex < oObj.m_shNRows; ++unIndex)
		oBuffer.ReadShort(oObj.m_arRowSize[unIndex]);

	oBuffer.ReadShort(oObj.m_shBorderFillID);

	if (nVersion >= 5010 && (oBuffer.GetDistanceToLastPos() < nSize))
	{
		oBuffer.ReadShort(oObj.m_shValidZoneSize);

		if (0 < oObj.m_shValidZoneSize && (oBuffer.GetDistanceToLastPos() < nSize))
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

	oObj.m_bFullFilled = true;

	oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos(true));
	return nSize;
}

int CCtrlTable::ParseListHeaderAppend(CCtrlTable& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	if (24 != nSize)
	{
		oBuffer.Skip(nSize);
		return nSize;
	}

	oBuffer.Skip(2);
	oBuffer.ReadInt(oObj.m_nCaptionAttr);
	oBuffer.ReadInt(oObj.m_nCaptionWidth);
	oObj.m_nCaptionSpacing = oBuffer.ReadShort();
	oBuffer.ReadInt(oObj.m_nCaptionMaxW);
	oBuffer.Skip(8);

	return nSize;
}
}
