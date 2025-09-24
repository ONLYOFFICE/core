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

CCtrlShapeConnectLine::CCtrlShapeConnectLine(const HWP_STRING& sCtrlID, CXMLReader& oReader, EHanType eType)
    : CCtrlGeneralShape(sCtrlID, oReader, eType)
{
	switch(eType)
	{
		case EHanType::HWPX:  ReadFromHWPX (oReader); return;
		case EHanType::HWPML: ReadFromHWPML(oReader); return;
		default: break;
	}
}

void CCtrlShapeConnectLine::ReadFromHWPX(CXMLReader &oReader)
{
	m_eType = GetConnectLineType(oReader.GetAttributeInt("type"));

	WHILE_READ_NEXT_NODE_WITH_NAME(oReader)
	{
		if ("hp:startPt" == sNodeName)
		{
			START_READ_ATTRIBUTES(oReader)
			{
				if ("x" == sAttributeName)
					m_oStartPt.m_nX = oReader.GetInt();
				else if ("y" == sAttributeName)
					m_oStartPt.m_nY = oReader.GetInt();
				else if ("subjectIDRef" == sAttributeName)
					m_oStartPt.m_shSubjectIDRef = oReader.GetInt();
				else if ("subjectIdx" == sAttributeName)
					m_oStartPt.m_shSubjectIdx = oReader.GetInt();
			}
			END_READ_ATTRIBUTES(oReader)
		}
		else if ("hp:endPt" == sNodeName)
		{
			START_READ_ATTRIBUTES(oReader)
			{
				if ("x" == sAttributeName)
					m_oEndPt.m_nX = oReader.GetInt();
				else if ("y" == sAttributeName)
					m_oEndPt.m_nY = oReader.GetInt();
				else if ("subjectIDRef" == sAttributeName)
					m_oEndPt.m_shSubjectIDRef = oReader.GetInt();
				else if ("subjectIdx" == sAttributeName)
					m_oEndPt.m_shSubjectIdx = oReader.GetInt();
			}
			END_READ_ATTRIBUTES(oReader)
		}
		else
			CCtrlGeneralShape::ParseChildren(oReader, EHanType::HWPX);
	}
	END_WHILE
}

void CCtrlShapeConnectLine::ReadFromHWPML(CXMLReader &oReader)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if ("Type" == sAttributeName)
			// TODO:: реализовать при встрече
			m_eType = EConnectLineType::STRAIGHT_NOARROW;
		else if ("StartX" == sAttributeName)
			m_oStartPt.m_nX = oReader.GetInt();
		else if ("StartY" == sAttributeName)
			m_oStartPt.m_nY = oReader.GetInt();
		else if ("EndX" == sAttributeName)
			m_oEndPt.m_nX = oReader.GetInt();
		else if ("EndY" == sAttributeName)
			m_oEndPt.m_nY = oReader.GetInt();
		else if ("StartSubjectID" == sAttributeName)
			m_oStartPt.m_shSubjectIDRef = oReader.GetInt();
		else if ("StartSubjectIndex" == sAttributeName)
			m_oStartPt.m_shSubjectIdx = oReader.GetInt();
		else if ("EndSubjectID" == sAttributeName)
			m_oEndPt.m_shSubjectIDRef = oReader.GetInt();
		else if ("EndSubjectIndex" == sAttributeName)
			m_oEndPt.m_shSubjectIdx = oReader.GetInt();
	}
	END_READ_ATTRIBUTES(oReader)

	WHILE_READ_NEXT_NODE(oReader)
		CCtrlGeneralShape::ParseChildren(oReader, EHanType::HWPML);
	END_WHILE
}

EShapeType CCtrlShapeConnectLine::GetShapeType() const
{
	return EShapeType::ConnectLine;
}
}
