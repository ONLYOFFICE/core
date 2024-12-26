#include "CtrlShapeEllipse.h"

namespace HWP
{
CCtrlShapeEllipse::CCtrlShapeEllipse()
{}

CCtrlShapeEllipse::CCtrlShapeEllipse(const HWP_STRING& sCtrlID)
	: CCtrlGeneralShape(sCtrlID)
{}

CCtrlShapeEllipse::CCtrlShapeEllipse(const CCtrlGeneralShape& oShape)
	: CCtrlGeneralShape(oShape)
{}

CCtrlShapeEllipse::CCtrlShapeEllipse(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrlGeneralShape(sCtrlID, nSize, oBuffer, nOff, nVersion)
{}

int CCtrlShapeEllipse::ParseElement(CCtrlShapeEllipse& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	oBuffer.SavePosition();

	int nAttr;
	oBuffer.ReadInt(nAttr);

	oObj.m_bIntervalDirty = CHECK_FLAG(nAttr, 0x01);
	oObj.m_bHasArcProperty = CHECK_FLAG(nAttr, 0x02);
	oObj.m_eArcType = GetArcType(nAttr << 2 & 0xFF);
	oBuffer.Skip(4);
	oBuffer.ReadInt(oObj.m_nCenterX);
	oBuffer.ReadInt(oObj.m_nCenterY);
	oBuffer.ReadInt(oObj.m_nAxixX1);
	oBuffer.ReadInt(oObj.m_nAxixY1);
	oBuffer.ReadInt(oObj.m_nAxixX2);
	oBuffer.ReadInt(oObj.m_nAxixY2);
	oBuffer.ReadInt(oObj.m_nSrartX1);
	oBuffer.ReadInt(oObj.m_nSrartY1);
	oBuffer.ReadInt(oObj.m_nEndX1);
	oBuffer.ReadInt(oObj.m_nEndY1);
	oBuffer.ReadInt(oObj.m_nSrartX2);
	oBuffer.ReadInt(oObj.m_nSrartY2);
	oBuffer.ReadInt(oObj.m_nEndX2);
	oBuffer.ReadInt(oObj.m_nEndY2);

	oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos(true));

	return nSize;
}

int CCtrlShapeEllipse::ParseCtrl(CCtrlShapeEllipse& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	oBuffer.SavePosition();
	CCtrlGeneralShape::ParseCtrl(oObj, nSize, oBuffer, nOff, nVersion);
	return oBuffer.GetDistanceToLastPos(true);
}

int CCtrlShapeEllipse::ParseListHeaderAppend(CCtrlShapeEllipse& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	oBuffer.SavePosition();

	oBuffer.Skip(2);

	oBuffer.ReadShort(oObj.m_shLeftSpace);
	oBuffer.ReadShort(oObj.m_shRightSpace);
	oBuffer.ReadShort(oObj.m_shTopSpace);
	oBuffer.ReadShort(oObj.m_shBottomSpace);

	oBuffer.ReadInt(oObj.m_nMaxTxtWidth);

	if (nSize > oBuffer.GetDistanceToLastPos())
	{
		oBuffer.Skip(10);
		HWP_STRING sFieldName;
		oBuffer.ReadString(sFieldName, EStringCharacter::UTF16);

		oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos());
	}

	return oBuffer.GetDistanceToLastPos(true);
}

}
