#include "CtrlObjElement.h"

namespace HWP
{
CCtrlObjElement::CCtrlObjElement()
{
	InitMatrix();
}

CCtrlObjElement::CCtrlObjElement(const HWP_STRING& sCtrlID)
	: CCtrlCommon(sCtrlID)
{
	InitMatrix();
}

CCtrlObjElement::CCtrlObjElement(const CCtrlObjElement& oObjElement)
	: CCtrlCommon(oObjElement)
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

	m_arMatrix.resize(6);
	for (unsigned int unIndex = 0; unIndex < 6; ++unIndex)
		m_arMatrix[unIndex] = oObjElement.m_arMatrix[unIndex];

	m_arMatrixSeq.resize(oObjElement.m_arMatrixSeq.size());
	for (unsigned int unIndex = 0; unIndex < oObjElement.m_arMatrixSeq.size(); ++unIndex)
		m_arMatrixSeq[unIndex] = oObjElement.m_arMatrixSeq[unIndex];
}

CCtrlObjElement::CCtrlObjElement(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrlCommon(sCtrlID, nSize, oBuffer, nOff, nVersion)
{
	InitMatrix();
}

CCtrlObjElement::CCtrlObjElement(const HWP_STRING& sCtrlID, CXMLNode& oNode, int nVersion)
	: CCtrlCommon(sCtrlID, oNode, nVersion)
{
	InitMatrix();

	m_shNGrp = oNode.GetAttributeInt(L"groupLevel");

	m_arMatrix.resize(6);
	m_arMatrixSeq.resize((m_shNGrp + 1) * 6 * 2);

	short shScaMatCnt = 0, shRotMatCnt = 0;

	std::vector<CXMLNode> arChilds{oNode.GetChilds()};

	//TODO:: проверить чтение не в обратном порядке
	for (std::vector<CXMLNode>::reverse_iterator itNode = arChilds.rbegin(); itNode < arChilds.rend(); ++itNode)
	{
		if (L"hp:offset" == (*itNode).GetName())
		{
			m_nXGrpOffset = (*itNode).GetAttributeInt(L"x");
			m_nYGrpOffset = (*itNode).GetAttributeInt(L"y");
		}
		else if (L"hp:orgSz" == (*itNode).GetName())
		{
			m_nIniWidth  = (*itNode).GetAttributeInt(L"width");
			m_nIniHeight = (*itNode).GetAttributeInt(L"height");
		}
		else if (L"hp:curSz" == (*itNode).GetName())
		{
			m_nCurWidth  = (*itNode).GetAttributeInt(L"width");
			m_nCurHeight = (*itNode).GetAttributeInt(L"height");
		}
		else if (L"hp:flip" == (*itNode).GetName())
		{
			m_bHorzFlip = (*itNode).GetAttributeBool(L"horizontal");
			m_bVerFlip  = (*itNode).GetAttributeBool(L"vertical");
		}
		else if (L"hp:rotationInfo" == (*itNode).GetName())
		{
			m_shRotat = (*itNode).GetAttributeInt(L"angle");
			m_nXCenter = (*itNode).GetAttributeInt(L"centerX");
			m_nYCenter = (*itNode).GetAttributeInt(L"centerY");
		}
		else if (L"hp:renderingInfo" == (*itNode).GetName())
		{
			for (CXMLNode& oGrandChild : (*itNode).GetChilds())
			{
				if (L"hc:transMatrix" == oGrandChild.GetName())
					SetMatrix(oGrandChild, m_arMatrix, 0);
				else if (L"hc:scaMatrix" == oGrandChild.GetName())
				{
					SetMatrix(oGrandChild, m_arMatrixSeq, shScaMatCnt * 12);
					++shScaMatCnt;
				}
				else if (L"hc:rotMatrix" == oGrandChild.GetName())
				{
					SetMatrix(oGrandChild, m_arMatrixSeq, shRotMatCnt * 12 + 6);
					++shRotMatCnt;
				}
			}
		}
	}
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

void CCtrlObjElement::InitMatrix()
{
	m_arMatrix.resize(6);
	m_arMatrix[0] = 1.;
	m_arMatrix[1] = 0.;
	m_arMatrix[2] = 0.;
	m_arMatrix[3] = 1.;
	m_arMatrix[4] = 0.;
	m_arMatrix[5] = 0.;
}

void CCtrlObjElement::SetMatrix(CXMLNode& oNode, std::vector<double>& arMatrix, int nOffset)
{
	arMatrix[0 + nOffset] = oNode.GetAttributeDouble(L"e1");
	arMatrix[1 + nOffset] = oNode.GetAttributeDouble(L"e2");
	arMatrix[2 + nOffset] = oNode.GetAttributeDouble(L"e3");
	arMatrix[3 + nOffset] = oNode.GetAttributeDouble(L"e4");
	arMatrix[4 + nOffset] = oNode.GetAttributeDouble(L"e5");
	arMatrix[5 + nOffset] = oNode.GetAttributeDouble(L"e6");
}
}
