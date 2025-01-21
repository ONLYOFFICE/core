#include "CtrlShapeEllipse.h"

namespace HWP
{
EArcType GetArcType(int nValue)
{
	SWITCH(EArcType, nValue)
	{
		DEFAULT(EArcType::NORMAL);
		CASE(EArcType::PIE);
		CASE(EArcType::CHORD);
	}
}

EArcType GetArcType(const HWP_STRING& sValue)
{
	IF_STRING_IN_ENUM(PIE, sValue, EArcType);
	ELSE_IF_STRING_IN_ENUM(CHORD, sValue, EArcType);
	ELSE_STRING_IN_ENUM(NORMAL, EArcType);
}

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

CCtrlShapeEllipse::CCtrlShapeEllipse(const HWP_STRING& sCtrlID, CXMLNode& oNode, int nVersion)
	: CCtrlGeneralShape(sCtrlID, oNode, nVersion)
{
	m_bIntervalDirty = oNode.GetAttributeBool(L"intervalDirty");
	m_bHasArcProperty = oNode.GetAttributeBool(L"hasArcPr");
	m_eArcType = GetArcType(oNode.GetAttribute(L"arcType"));

	for (CXMLNode& oChild : oNode.GetChilds())
	{
		if (L"hc:center" == oChild.GetName())
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
		else if (L"hc:start1" == oChild.GetName())
		{
			m_nStartX1 = oChild.GetAttributeInt(L"x");
			m_nStartY1 = oChild.GetAttributeInt(L"y");
		}
		else if (L"hc:start2" == oChild.GetName())
		{
			m_nStartX2 = oChild.GetAttributeInt(L"x");
			m_nStartY2 = oChild.GetAttributeInt(L"y");
		}
		else if (L"hc:end1" == oChild.GetName())
		{
			m_nEndX1 = oChild.GetAttributeInt(L"x");
			m_nEndY1 = oChild.GetAttributeInt(L"y");
		}
		else if (L"hc:end2" == oChild.GetName())
		{
			m_nEndX2 = oChild.GetAttributeInt(L"x");
			m_nEndY2 = oChild.GetAttributeInt(L"y");
		}
	}
}

EShapeType CCtrlShapeEllipse::GetShapeType() const
{
	return EShapeType::Ellipse;
}

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
	oBuffer.ReadInt(oObj.m_nStartX1);
	oBuffer.ReadInt(oObj.m_nStartY1);
	oBuffer.ReadInt(oObj.m_nEndX1);
	oBuffer.ReadInt(oObj.m_nEndY1);
	oBuffer.ReadInt(oObj.m_nStartX2);
	oBuffer.ReadInt(oObj.m_nStartY2);
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

	oBuffer.Skip(13);

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
