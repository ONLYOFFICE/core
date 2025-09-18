#include "CtrlShapeCurve.h"

#include "../Common/NodeNames.h"

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

CCtrlShapeCurve::CCtrlShapeCurve(const HWP_STRING& sCtrlID, CXMLReader& oReader, int nVersion, EHanType eType)
    : CCtrlGeneralShape(sCtrlID, oReader, nVersion, eType)
{
	// bool bReadedType
	TPoint oPoint1{0, 0}, oPoint2{0, 0};
	HWP_BYTE chSegmentType = 1;

	WHILE_READ_NEXT_NODE(oReader)
	{
		if (GetNodeName(ENode::Segment, eType) == oReader.GetName())
		{
			chSegmentType = 1;
			oPoint1 = {0, 0};
			oPoint2 = {0, 0};

			START_READ_ATTRIBUTES(oReader)
			{
				if (GetAttributeName(EAttribute::Type, eType) == sAttributeName)
				{
					const std::string sType{oReader.GetTextA()};

					if (GetValueName(EValue::Curve, eType) == sType)
						chSegmentType = 1;
					else if (GetValueName(EValue::Line, eType) == sType)
						chSegmentType = 0;
				}
				else if (GetAttributeName(EAttribute::X1, eType) == sAttributeName)
					oPoint1.m_nX = oReader.GetInt();
				else if (GetAttributeName(EAttribute::Y1, eType) == sAttributeName)
					oPoint1.m_nY = oReader.GetInt();
				else if (GetAttributeName(EAttribute::X2, eType) == sAttributeName)
					oPoint2.m_nX = oReader.GetInt();
				else if (GetAttributeName(EAttribute::Y2, eType) == sAttributeName)
					oPoint2.m_nY = oReader.GetInt();
			}
			END_READ_ATTRIBUTES(oReader)

			m_arSegmentType.push_back(chSegmentType);
			m_arPoints.push_back(oPoint1);
		}
		else
			CCtrlGeneralShape::ParseChildren(oReader, nVersion, eType);
	}
	END_WHILE

	m_arPoints.push_back(oPoint2);
}

EShapeType CCtrlShapeCurve::GetShapeType() const
{
	return EShapeType::Curve;
}

VECTOR<TPoint> CCtrlShapeCurve::GetPoints() const
{
	return m_arPoints;
}

VECTOR<HWP_BYTE> CCtrlShapeCurve::GetSegmentsType() const
{
	return m_arSegmentType;
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
