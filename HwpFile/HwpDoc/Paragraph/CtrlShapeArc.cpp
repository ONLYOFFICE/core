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

CCtrlShapeArc::CCtrlShapeArc(const HWP_STRING& sCtrlID, CXMLReader& oReader, int nVersion, EHanType eType)
    : CCtrlGeneralShape(sCtrlID, oReader, nVersion, eType)
{
	m_eType = GetArcType(oReader.GetAttributeInt("type"));

	WHILE_READ_NEXT_NODE_WITH_NAME(oReader)
	{
		if ("hp:center" == sNodeName)
		{
			START_READ_ATTRIBUTES(oReader)
			{
				if ("x" == sAttributeName)
					m_nCenterX = oReader.GetInt();
				else if ("y" == sAttributeName)
					m_nCenterY = oReader.GetInt();
			}
			END_READ_ATTRIBUTES(oReader)
		}
		else if ("hp:ax1" == sNodeName)
		{
			START_READ_ATTRIBUTES(oReader)
			{
				if ("x" == sAttributeName)
					m_nAxixX1 = oReader.GetInt();
				else if ("y" == sAttributeName)
					m_nAxixY1 = oReader.GetInt();
			}
			END_READ_ATTRIBUTES(oReader)
		}
		else if ("hp:ax2" == sNodeName)
		{
			START_READ_ATTRIBUTES(oReader)
			{
				if ("x" == sAttributeName)
					m_nAxixX2 = oReader.GetInt();
				else if ("y" == sAttributeName)
					m_nAxixY2 = oReader.GetInt();
			}
			END_READ_ATTRIBUTES(oReader)
		}
		else
			CCtrlGeneralShape::ParseChildren(oReader, nVersion, eType);
	}
	END_WHILE
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
