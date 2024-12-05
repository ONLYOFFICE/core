#include "CtrlShapeLine.h"

namespace HWP
{
CCtrlShapeLine::CCtrlShapeLine(const std::string& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrlGeneralShape(sCtrlID, nSize, oBuffer, nOff, nVersion)
{}

int CCtrlShapeLine::ParseElement(CCtrlShapeLine& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	BYTE* pOldCurrentPos = oBuffer.GetCurPtr();

	if ("loc$" == oObj.GetID())
		oBuffer.Skip(4);

	oBuffer.ReadInt(oObj.m_nStartX);
	oBuffer.ReadInt(oObj.m_nStartY);
	oBuffer.ReadInt(oObj.m_nEndX);
	oBuffer.ReadInt(oObj.m_nEndY);

	if (nSize == oBuffer.GetCurPtr() - pOldCurrentPos)
		return nSize;

	oBuffer.ReadShort(oObj.m_shAttr);
	oBuffer.Skip(2);

	return nSize;
}

int CCtrlShapeLine::ParseCtrl(CCtrlShapeLine& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	CCtrlGeneralShape::ParseCtrl(oObj, nSize, oBuffer, nOff, nVersion);
	return oObj.GetSize();
}

}
