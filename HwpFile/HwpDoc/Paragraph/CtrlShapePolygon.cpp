#include "CtrlShapePolygon.h"

namespace HWP
{
CCtrlShapePolygon::CCtrlShapePolygon()
{}

CCtrlShapePolygon::CCtrlShapePolygon(const STRING& sCtrlID)
	: CCtrlGeneralShape(sCtrlID)
{}

CCtrlShapePolygon::CCtrlShapePolygon(const STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrlGeneralShape(sCtrlID, nSize, oBuffer, nOff, nVersion)
{}

int CCtrlShapePolygon::ParseElement(CCtrlShapePolygon& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	oBuffer.SavePosition();

	oBuffer.ReadInt(oObj.m_nPoints);

	if (0 < oObj.m_nPoints)
	{
		TPoint oPoint;
		for (unsigned int unIndex = 0; unIndex < oObj.m_nPoints; ++unIndex)
		{
			oBuffer.ReadInt(oPoint.m_nX);
			oBuffer.ReadInt(oPoint.m_nY);
			oObj.m_arPoints.push_back(oPoint);
		}
	}

	if (4 == (nSize - oBuffer.GetDistanceToLastPos()))
		oBuffer.Skip(4);

	oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos(true));

	return nSize;
}

int CCtrlShapePolygon::ParseCtrl(CCtrlShapePolygon& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	return CCtrlGeneralShape::ParseCtrl(oObj, nSize, oBuffer, nOff, nVersion);
}

int CCtrlShapePolygon::ParseListHeaderAppend(CCtrlShapePolygon& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	oBuffer.SavePosition();

	oBuffer.Skip(2);

	oBuffer.ReadShort(oObj.m_shLeftSpace);
	oBuffer.ReadShort(oObj.m_shRightSpace);
	oBuffer.ReadShort(oObj.m_shTopSpace);
	oBuffer.ReadShort(oObj.m_shBottomSpace);
	oBuffer.ReadInt(oObj.m_nMaxTxtWidth);

	oBuffer.Skip(13);

	if (nSize > oBuffer.GetDistanceToLastPos())
	{
		oBuffer.Skip(10);
		STRING sFieldName;
		oBuffer.ReadString(sFieldName, EStringCharacter::UTF16);

		oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos());
	}

	return oBuffer.GetDistanceToLastPos(true);
}


}
