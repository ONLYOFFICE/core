#include "CtrlShapeLine.h"

namespace HWP
{
CCtrlShapeLine::CCtrlShapeLine()
{}

CCtrlShapeLine::CCtrlShapeLine(const HWP_STRING& sCtrlID)
	: CCtrlGeneralShape(sCtrlID)
{}

CCtrlShapeLine::CCtrlShapeLine(const CCtrlGeneralShape& oShape)
	: CCtrlGeneralShape(oShape)
{}

CCtrlShapeLine::CCtrlShapeLine(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrlGeneralShape(sCtrlID, nSize, oBuffer, nOff, nVersion)
{}

CCtrlShapeLine::CCtrlShapeLine(const HWP_STRING& sCtrlID, CXMLNode& oNode, int nVersion)
	: CCtrlGeneralShape(sCtrlID, oNode, nVersion)
{
	m_shAttr = (short)oNode.GetAttributeBool(L"isReverseHV");

	for (CXMLNode& oChild : oNode.GetChilds())
	{
		if (L"hc:startPt" == oChild.GetName())
		{
			m_nStartX = oChild.GetAttributeInt(L"x");
			m_nStartY = oChild.GetAttributeInt(L"y");
		}
		else if (L"hc:endPt" == oChild.GetName())
		{
			m_nEndX = oChild.GetAttributeInt(L"x");
			m_nEndY = oChild.GetAttributeInt(L"y");
		}
	}
}

EShapeType CCtrlShapeLine::GetShapeType() const
{
	return EShapeType::Line;
}

void CCtrlShapeLine::ParseElement(CCtrlShapeLine& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	oBuffer.SavePosition();

	if (L"loc$" == oObj.GetID())
		oBuffer.Skip(4);

	oBuffer.ReadInt(oObj.m_nStartX);
	oBuffer.ReadInt(oObj.m_nStartY);
	oBuffer.ReadInt(oObj.m_nEndX);
	oBuffer.ReadInt(oObj.m_nEndY);

	if (nSize == oBuffer.GetDistanceToLastPos())
	{
		oBuffer.RemoveLastSavedPos();
		return;
	}

	oBuffer.ReadShort(oObj.m_shAttr);
	oBuffer.Skip(2);

	oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos(true));
}

void CCtrlShapeLine::ParseCtrl(CCtrlShapeLine& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	CCtrlGeneralShape::ParseCtrl(oObj, nSize, oBuffer, nOff, nVersion);
}

}
