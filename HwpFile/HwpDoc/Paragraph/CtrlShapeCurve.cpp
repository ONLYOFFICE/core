#include "CtrlShapeCurve.h"

namespace HWP
{
CCtrlShapeCurve::CCtrlShapeCurve(const std::string& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrlGeneralShape(sCtrlID, nSize, oBuffer, nOff, nVersion)
{}

int CCtrlShapeCurve::ParseElement(CCtrlShapeCurve& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	oBuffer.ReadInt(oObj.m_nPoints);

	if (0 < oObj.m_nPoints)
	{
		oObj.m_arPoints.reserve(oObj.m_nPoints);
		for (unsigned int unIndex = 0; unIndex < oObj.m_nPoints; ++unIndex)
		{
			oBuffer.ReadInt(oObj.m_arPoints[unIndex].m_nX);
			oBuffer.ReadInt(oObj.m_arPoints[unIndex].m_nY);
		}
	}

	if (1 < oObj.m_nPoints)
	{
		oObj.m_arSegmentType.reserve(oObj.m_nPoints - 1);
		for (unsigned int unIndex = 0; unIndex < oObj.m_nPoints - 1; ++unIndex)
			oBuffer.ReadByte(oObj.m_arSegmentType[unIndex]);
	}

	oBuffer.Skip(4);

	return nSize;
}

int CCtrlShapeCurve::ParseCtrl(CCtrlShapeCurve& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	return CCtrlGeneralShape::ParseCtrl(oObj, nSize, oBuffer, nOff, nVersion);
}

}
