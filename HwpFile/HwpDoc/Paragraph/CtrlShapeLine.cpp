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

CCtrlShapeLine::CCtrlShapeLine(const HWP_STRING& sCtrlID, CXMLReader& oReader, EHanType eType)
    : CCtrlGeneralShape(sCtrlID, oReader, eType)
{
	switch(eType)
	{
		case EHanType::HWPX:  ReadFromHWPX (oReader); return;
		case EHanType::HWPML: ReadFromHWPML(oReader); return;
		default: break;
	}
}

void CCtrlShapeLine::ReadFromHWPX(CXMLReader &oReader)
{
	m_shAttr = (short)oReader.GetAttributeBool("isReverseHV");

	WHILE_READ_NEXT_NODE_WITH_NAME(oReader)
	{
		if ("hc:startPt" == sNodeName)
		{
			START_READ_ATTRIBUTES(oReader)
			{
				if ("x" == sAttributeName)
					m_nStartX = oReader.GetInt();
				else if ("y" == sAttributeName)
					m_nStartY = oReader.GetInt();
			}
			END_READ_ATTRIBUTES(oReader)
		}
		else if ("hc:endPt" == sNodeName)
		{
			START_READ_ATTRIBUTES(oReader)
			{
				if ("x" == sAttributeName)
					m_nEndX = oReader.GetInt();
				else if ("y" == sAttributeName)
					m_nEndY = oReader.GetInt();
			}
			END_READ_ATTRIBUTES(oReader)
		}
		else
			CCtrlGeneralShape::ParseChildren(oReader, EHanType::HWPX);
	}
	END_WHILE
}

void CCtrlShapeLine::ReadFromHWPML(CXMLReader &oReader)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if ("StartX" == sAttributeName)
			m_nStartX = oReader.GetInt();
		else if ("StartY" == sAttributeName)
			m_nStartY = oReader.GetInt();
		else if ("EndX" == sAttributeName)
			m_nEndX = oReader.GetInt();
		else if ("EndY" == sAttributeName)
			m_nEndY = oReader.GetInt();
		else if ("IsReverseHV" == sAttributeName)
			m_shAttr = oReader.GetBool();
	}
	END_READ_ATTRIBUTES(oReader)

	WHILE_READ_NEXT_NODE(oReader)
		CCtrlGeneralShape::ParseChildren(oReader, EHanType::HWPML);
	END_WHILE
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
