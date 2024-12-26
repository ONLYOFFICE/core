#include "CtrlObjElement.h"

namespace HWP
{
CCtrlObjElement::CCtrlObjElement()
{}

CCtrlObjElement::CCtrlObjElement(const HWP_STRING& sCtrlID)
	: CCtrlCommon(sCtrlID)
{}

CCtrlObjElement::CCtrlObjElement(const CCtrlObjElement& oObjElement)
	: CCtrlCommon((CCtrlCommon)oObjElement)
{
	m_nXGrpOffset = oObjElement.m_nXGrpOffset;
	m_nYGrpOffset = oObjElement.m_nYGrpOffset;
	m_shNGrp = oObjElement.m_shNGrp;
	m_shVer = oObjElement.m_shVer;
	m_nIniWidth = oObjElement.m_nIniWidth;
	m_nIniHeight = oObjElement.m_nIniHeight;
	m_nCurWidth = oObjElement.m_nCurWidth;
	m_nCurHeight = oObjElement.m_nCurHeight;
	m_bHorzFlip = oObjElement.m_bHorzFlip;
	m_bVerFlip = oObjElement.m_bVerFlip;
	m_shRotat = oObjElement.m_shRotat;
	m_nXCenter = oObjElement.m_nXCenter;
	m_nYCenter = oObjElement.m_nYCenter;
	m_shMatCnt = oObjElement.m_shMatCnt;

	for (unsigned int unIndex = 0; unIndex < 6; ++unIndex)
		m_arMatrix[unIndex] = oObjElement.m_arMatrix[unIndex];

	m_arMatrixSeq.resize(oObjElement.m_arMatrixSeq.size());
	for (unsigned int unIndex = 0; unIndex < oObjElement.m_arMatrixSeq.size(); ++unIndex)
		m_arMatrixSeq[unIndex] = oObjElement.m_arMatrixSeq[unIndex];
}

CCtrlObjElement::CCtrlObjElement(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrlCommon(sCtrlID, nSize, oBuffer, nOff, nVersion)
{}

int CCtrlObjElement::GetSize()
{
	return m_nSize;
}

int CCtrlObjElement::GetCurWidth() const
{
	return m_nCurWidth;
}

int CCtrlObjElement::GetCurHeight() const
{
	return m_nCurHeight;
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

		for (int nIndex = 0; nIndex < nMatrixSize; ++nIndex)
			oBuffer.ReadDouble(oObj.m_arMatrixSeq[nIndex]);
	}

	return oBuffer.GetDistanceToLastPos(true);
}
}
