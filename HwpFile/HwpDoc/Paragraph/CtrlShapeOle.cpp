#include "CtrlShapeOle.h"

namespace HWP
{
CCtrlShapeOle::CCtrlShapeOle(const std::string& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrlGeneralShape(sCtrlID, nSize, oBuffer, nOff, nVersion)
{}

int CCtrlShapeOle::ParseElement(CCtrlShapeOle& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	oBuffer.ReadInt(oObj.m_nAttr);
	oBuffer.ReadInt(oObj.m_nExtentX);
	oBuffer.ReadInt(oObj.m_nExtentY);
	oObj.m_sBinDataID = std::to_string(oBuffer.ReadShort());
	oBuffer.ReadColor(oObj.m_nBorderColor);
	oBuffer.ReadInt(oObj.m_nBorderThick);
	oBuffer.ReadInt(oObj.m_nBorderAttr);

	return nSize;
}

int CCtrlShapeOle::ParseCtrl(CCtrlShapeOle& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	return CCtrlObjElement::ParseCtrl(oObj, nSize, oBuffer, nOff, nVersion);
}

}
