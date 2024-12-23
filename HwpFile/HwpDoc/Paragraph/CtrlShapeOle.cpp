#include "CtrlShapeOle.h"

namespace HWP
{
CCtrlShapeOle::CCtrlShapeOle()
{}

CCtrlShapeOle::CCtrlShapeOle(const STRING& sCtrlID)
	: CCtrlGeneralShape(sCtrlID)
{}

CCtrlShapeOle::CCtrlShapeOle(const STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrlGeneralShape(sCtrlID, nSize, oBuffer, nOff, nVersion)
{}

int CCtrlShapeOle::ParseElement(CCtrlShapeOle& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	oBuffer.SavePosition();

	oBuffer.ReadInt(oObj.m_nAttr);
	oBuffer.ReadInt(oObj.m_nExtentX);
	oBuffer.ReadInt(oObj.m_nExtentY);
	oObj.m_sBinDataID = std::to_wstring(oBuffer.ReadShort());
	oBuffer.ReadColor(oObj.m_nBorderColor);
	oBuffer.ReadInt(oObj.m_nBorderThick);
	oBuffer.ReadInt(oObj.m_nBorderAttr);

	return oBuffer.GetDistanceToLastPos(true);
}

int CCtrlShapeOle::ParseCtrl(CCtrlShapeOle& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	return CCtrlObjElement::ParseCtrl(oObj, nSize, oBuffer, nOff, nVersion);
}

}
