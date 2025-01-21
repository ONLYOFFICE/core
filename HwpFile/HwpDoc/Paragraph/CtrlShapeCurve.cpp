#include "CtrlShapeCurve.h"

namespace HWP
{
CCtrlShapeCurve::CCtrlShapeCurve()
{}

CCtrlShapeCurve::CCtrlShapeCurve(const HWP_STRING& sCtrlID)
	: CCtrlGeneralShape(sCtrlID)
{}

CCtrlShapeCurve::CCtrlShapeCurve(const CCtrlGeneralShape& oShape)
	: CCtrlGeneralShape(oShape)
{}

CCtrlShapeCurve::CCtrlShapeCurve(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrlGeneralShape(sCtrlID, nSize, oBuffer, nOff, nVersion)
{}

CCtrlShapeCurve::CCtrlShapeCurve(const HWP_STRING& sCtrlID, CXMLNode& oNode, int nVersion)
	: CCtrlGeneralShape(sCtrlID, oNode, nVersion)
{
	std::vector<CXMLNode> arChilds{oNode.GetChilds(L"hp:seg")};

	m_arPoints.resize(arChilds.size());
	m_arSegmentType.resize(arChilds.size());

	HWP_STRING sType;

	for (unsigned int unIndex = 0; unIndex < arChilds.size(); ++unIndex)
	{
		sType = arChilds[unIndex].GetAttribute(L"type");

		if (L"CURVE" == sType)
			m_arSegmentType[unIndex] = 1;
		else if (L"LINE" == sType)
			m_arSegmentType[unIndex] = 0;

		m_arPoints[unIndex].m_nX = arChilds[unIndex].GetAttributeInt(L"x1");
		m_arPoints[unIndex].m_nY = arChilds[unIndex].GetAttributeInt(L"y1");
	}
}

EShapeType CCtrlShapeCurve::GetShapeType() const
{
	return EShapeType::Curve;
}

int CCtrlShapeCurve::ParseElement(CCtrlShapeCurve& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	oBuffer.SavePosition();

	oBuffer.ReadInt(oObj.m_nPoints);

	if (0 < oObj.m_nPoints)
	{
		oObj.m_arPoints.resize(oObj.m_nPoints);
		for (unsigned int unIndex = 0; unIndex < oObj.m_nPoints; ++unIndex)
		{
			oBuffer.ReadInt(oObj.m_arPoints[unIndex].m_nX);
			oBuffer.ReadInt(oObj.m_arPoints[unIndex].m_nY);
		}
	}

	if (1 < oObj.m_nPoints)
	{
		oObj.m_arSegmentType.resize(oObj.m_nPoints - 1);
		for (unsigned int unIndex = 0; unIndex < oObj.m_nPoints - 1; ++unIndex)
			oBuffer.ReadByte(oObj.m_arSegmentType[unIndex]);
	}

	oBuffer.Skip(4);

	oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos(true));

	return nSize;
}

int CCtrlShapeCurve::ParseCtrl(CCtrlShapeCurve& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	return CCtrlGeneralShape::ParseCtrl(oObj, nSize, oBuffer, nOff, nVersion);
}

}
