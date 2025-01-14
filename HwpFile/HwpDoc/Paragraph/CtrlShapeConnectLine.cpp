#include "CtrlShapeConnectLine.h"

namespace HWP
{
EConnectLineType GetConnectLineType(int nValue)
{
	switch(static_cast<EConnectLineType>(nValue))
	{
		case EConnectLineType::STRAIGHT_NOARROW:
		case EConnectLineType::STRAIGHT_ONEWAY:
		case EConnectLineType::STRAIGHT_BOTH:
		case EConnectLineType::STROKE_NOARROW:
		case EConnectLineType::STROKE_ONEWAY:
		case EConnectLineType::STROKE_BOTH:
		case EConnectLineType::ARC_NOARROW:
		case EConnectLineType::ARC_ONEWAY:
		case EConnectLineType::ARC_BOTH:
			return static_cast<EConnectLineType>(nValue);
		default:
			return EConnectLineType::null;
	}
}

CCtrlShapeConnectLine::CCtrlShapeConnectLine(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrlGeneralShape(sCtrlID, nSize, oBuffer, nOff, nVersion)
{}

CCtrlShapeConnectLine::CCtrlShapeConnectLine(const HWP_STRING& sCtrlID, CXMLNode& oNode, int nVersion)
	: CCtrlGeneralShape(sCtrlID, oNode, nVersion)
{
	m_eType = GetConnectLineType(oNode.GetAttributeInt(L"type"));

	for (CXMLNode& oChild : oNode.GetChilds())
	{
		if (L"hp:startPt" == oChild.GetName())
		{
			m_oStartPt.m_nX = oChild.GetAttributeInt(L"x");
			m_oStartPt.m_nY = oChild.GetAttributeInt(L"y");
			m_oStartPt.m_shSubjectIDRef = oChild.GetAttributeInt(L"subjectIDRef");
			m_oStartPt.m_shSubjectIdx = oChild.GetAttributeInt(L"subjectIdx");
		}
		else if (L"hp:endPt" == oChild.GetName())
		{
			m_oEndPt.m_nX = oChild.GetAttributeInt(L"x");
			m_oEndPt.m_nY = oChild.GetAttributeInt(L"y");
			m_oEndPt.m_shSubjectIDRef = oChild.GetAttributeInt(L"subjectIDRef");
			m_oEndPt.m_shSubjectIdx = oChild.GetAttributeInt(L"subjectIdx");
		}
	}
}

EShapeType CCtrlShapeConnectLine::GetShapeType() const
{
	return EShapeType::ConnectLine;
}
}
