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

EArcType GetArcType(const std::string& sValue, EHanType eType)
{
	if (sValue.empty() || GetValueName(EValue::Normal, eType) == sValue)
		return EArcType::NORMAL;
	if (GetValueName(EValue::Pie, eType) == sValue)
		return EArcType::PIE;
	if (GetValueName(EValue::Chord, eType) == sValue)
		return EArcType::CHORD;

	return EArcType::NORMAL;
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

CCtrlShapeEllipse::CCtrlShapeEllipse(const HWP_STRING& sCtrlID, CXMLReader& oReader, int nVersion, EHanType eType)
    : CCtrlGeneralShape(sCtrlID, oReader, nVersion, eType)
{
	switch(eType)
	{
		case EHanType::HWPX:  ReadFromHWPX (oReader, nVersion); return;
		case EHanType::HWPML: ReadFromHWPML(oReader); return;
	}
}

void CCtrlShapeEllipse::ReadFromHWPX(CXMLReader &oReader, int nVersion)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if ("intervalDirty" == sAttributeName)
			m_bIntervalDirty = oReader.GetBool();
		else if ("hasArcPr" == sAttributeName)
			m_bHasArcProperty = oReader.GetBool();
		else if ("arcType" == sAttributeName)
			m_eArcType = GetArcType(oReader.GetTextA(), EHanType::HWPX);
	}
	END_READ_ATTRIBUTES(oReader)

	WHILE_READ_NEXT_NODE_WITH_NAME(oReader)
	{
		if ("hc:center" == sNodeName)
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
		else if ("hp:start1" == sNodeName)
		{
			START_READ_ATTRIBUTES(oReader)
			{
				if ("x" == sAttributeName)
					m_nStartX1 = oReader.GetInt();
				else if ("y" == sAttributeName)
					m_nStartY1 = oReader.GetInt();
			}
			END_READ_ATTRIBUTES(oReader)
		}
		else if ("hp:start2" == sNodeName)
		{
			START_READ_ATTRIBUTES(oReader)
			{
				if ("x" == sAttributeName)
					m_nStartX2 = oReader.GetInt();
				else if ("y" == sAttributeName)
					m_nStartY2 = oReader.GetInt();
			}
			END_READ_ATTRIBUTES(oReader)
		}
		else if ("hp:end1" == sNodeName)
		{
			START_READ_ATTRIBUTES(oReader)
			{
				if ("x" == sAttributeName)
					m_nEndX1 = oReader.GetInt();
				else if ("y" == sAttributeName)
					m_nEndY1 = oReader.GetInt();
			}
			END_READ_ATTRIBUTES(oReader)
		}
		else if ("hp:end2" == sNodeName)
		{
			START_READ_ATTRIBUTES(oReader)
			{
				if ("x" == sAttributeName)
					m_nEndX2 = oReader.GetInt();
				else if ("y" == sAttributeName)
					m_nEndY2 = oReader.GetInt();
			}
			END_READ_ATTRIBUTES(oReader)
		}
		else
			CCtrlGeneralShape::ParseChildren(oReader, nVersion, EHanType::HWPX);
	}
	END_WHILE
}

void CCtrlShapeEllipse::ReadFromHWPML(CXMLReader &oReader)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if ("IntervalDirty" == sAttributeName)
			m_bIntervalDirty = oReader.GetBool();
		else if ("HasArcProperty" == sAttributeName)
			m_bHasArcProperty = oReader.GetBool();
		else if ("ArcType" == sAttributeName)
			m_eArcType = GetArcType(oReader.GetTextA(), EHanType::HWPML);
		else if ("CenterX" == sAttributeName)
			m_nCenterX = oReader.GetInt();
		else if ("CenterY" == sAttributeName)
			m_nCenterY = oReader.GetInt();
		else if ("Axis1X" == sAttributeName)
			m_nAxixX1 = oReader.GetInt();
		else if ("Axis1Y" == sAttributeName)
			m_nAxixY1 = oReader.GetInt();
		else if ("Axis2X" == sAttributeName)
			m_nAxixX2 = oReader.GetInt();
		else if ("Axis2Y" == sAttributeName)
			m_nAxixY2 = oReader.GetInt();
		else if ("Start1X" == sAttributeName)
			m_nStartX1 = oReader.GetInt();
		else if ("Start1Y" == sAttributeName)
			m_nStartY1 = oReader.GetInt();
		else if ("End1X" == sAttributeName)
			m_nEndX1 = oReader.GetInt();
		else if ("End1Y" == sAttributeName)
			m_nEndY1 = oReader.GetInt();
		else if ("Start2X" == sAttributeName)
			m_nStartX2 = oReader.GetInt();
		else if ("Start2Y" == sAttributeName)
			m_nStartY2 = oReader.GetInt();
		else if ("End2X" == sAttributeName)
			m_nEndX2 = oReader.GetInt();
		else if ("End2Y" == sAttributeName)
			m_nEndY2 = oReader.GetInt();
	}
	END_READ_ATTRIBUTES(oReader)

	WHILE_READ_NEXT_NODE(oReader)
		CCtrlGeneralShape::ParseChildren(oReader, 0, EHanType::HWPML);
	END_WHILE
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
