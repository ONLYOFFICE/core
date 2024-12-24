#include "CtrlObjElement.h"

namespace HWP
{
CCtrlObjElement::CCtrlObjElement()
{}

CCtrlObjElement::CCtrlObjElement(const STRING& sCtrlID)
	: CCtrlCommon(sCtrlID)
{}

CCtrlObjElement::CCtrlObjElement(const STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrlCommon(sCtrlID, nSize, oBuffer, nOff, nVersion)
{}

int CCtrlObjElement::GetSize()
{
	return m_nSize;
}

int CCtrlObjElement::ParseCtrl(CCtrlObjElement& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	oBuffer.SavePosition();

	CCtrlCommon::ParseCtrl(oObj, nSize, oBuffer, nOff, nVersion);

	oBuffer.ReadInt(oObj.m_nXGrpOffset);
	oBuffer.ReadInt(oObj.m_nYGrpOffset);
	oBuffer.ReadShort(oObj.m_shNGrp);
	oBuffer.ReadShort(oObj.m_shVer);
	oBuffer.ReadInt(oObj.m_nIniWidth);
	oBuffer.ReadInt(oObj.m_nIniHeight);
	oBuffer.ReadInt(oObj.m_nCurWidth);
	oBuffer.ReadInt(oObj.m_nCurHeight);
	oObj.m_bHorzFlip = CHECK_FLAG(oBuffer[0], 0x01);
	oObj.m_bVerFlip = CHECK_FLAG(oBuffer[0], 0x02);
	oBuffer.Skip(4);
	oBuffer.ReadShort(oObj.m_shRotat);
	oBuffer.ReadInt(oObj.m_nXCenter);
	oBuffer.ReadInt(oObj.m_nYCenter);
	oBuffer.ReadShort(oObj.m_shMatCnt);

	for (int nIndex = 0; nIndex < 6; ++nIndex)
		oBuffer.ReadDouble(oObj.m_arMatrix[nIndex]);

	int nMatrixSize = ((int)oObj.m_shMatCnt) * 6 * 2;
	if (nMatrixSize > 0)
	{
		oObj.m_arMatrixSeq.resize(nMatrixSize);

		for (int nIndex = 0; nIndex < nMatrixSize; ++nMatrixSize)
			oBuffer.ReadDouble(oObj.m_arMatrixSeq[nIndex]);
	}

	return oBuffer.GetDistanceToLastPos(true);
}
}
