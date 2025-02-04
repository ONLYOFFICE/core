#include "CtrlShapeArc.h"

namespace HWP
{
CCtrlShapeArc::CCtrlShapeArc()
{}

CCtrlShapeArc::CCtrlShapeArc(const HWP_STRING& sCtrlID)
	: CCtrlGeneralShape(sCtrlID)
{}

CCtrlShapeArc::CCtrlShapeArc(const CCtrlGeneralShape& oShape)
	: CCtrlGeneralShape(oShape)
{}

CCtrlShapeArc::CCtrlShapeArc(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrlGeneralShape(sCtrlID, nSize, oBuffer, nOff, nVersion)
{}

CCtrlShapeArc::CCtrlShapeArc(const HWP_STRING& sCtrlID, CXMLNode& oNode, int nVersion)
	: CCtrlGeneralShape(sCtrlID, oNode, nVersion)
{
	m_eType = GetArcType(oNode.GetAttributeInt(L"type"));

	for (CXMLNode& oChild : oNode.GetChilds())
	{
		if (L"hp:center" == oChild.GetName())
		{
			m_nCenterX = oChild.GetAttributeInt(L"x");
			m_nCenterY = oChild.GetAttributeInt(L"y");
		}
		else if (L"hp:ax1" == oChild.GetName())
		{
			m_nAxixX1 = oChild.GetAttributeInt(L"x");
			m_nAxixY1 = oChild.GetAttributeInt(L"y");
		}
		else if (L"hp:ax2" == oChild.GetName())
		{
			m_nAxixX2 = oChild.GetAttributeInt(L"x");
			m_nAxixY2 = oChild.GetAttributeInt(L"y");
		}
	}
}

EShapeType CCtrlShapeArc::GetShapeType() const
{
	return EShapeType::Arc;
}

TPoint CCtrlShapeArc::GetCenterPoint() const
{
	return {m_nCenterX, m_nCenterY};
}

TPoint CCtrlShapeArc::GetFirstPoint() const
{
	return {m_nAxixX1, m_nAxixY1};
}

TPoint CCtrlShapeArc::GetSecondPoint() const
{
	return {m_nAxixX2, m_nAxixY2};
}

int CCtrlShapeArc::ParseElement(CCtrlShapeArc& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	oBuffer.SavePosition();

	oObj.m_eType = GetArcType(oBuffer.ReadByte());
	oBuffer.ReadInt(oObj.m_nCenterX);
	oBuffer.ReadInt(oObj.m_nCenterY);
	oBuffer.ReadInt(oObj.m_nAxixX1);
	oBuffer.ReadInt(oObj.m_nAxixY1);
	oBuffer.ReadInt(oObj.m_nAxixX2);
	oBuffer.ReadInt(oObj.m_nAxixY2);

	oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos(true));

	return nSize;
}

int CCtrlShapeArc::ParseCtrl(CCtrlShapeArc& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	return CCtrlGeneralShape::ParseCtrl(oObj, nSize, oBuffer, nOff, nVersion);
}

}
