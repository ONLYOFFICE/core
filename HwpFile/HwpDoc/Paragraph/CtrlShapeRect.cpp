#include "CtrlShapeRect.h"

namespace HWP
{
CCtrlShapeRect::CCtrlShapeRect()
{}

CCtrlShapeRect::CCtrlShapeRect(const HWP_STRING& sCtrlID)
	: CCtrlGeneralShape(sCtrlID)
{}

CCtrlShapeRect::CCtrlShapeRect(const CCtrlGeneralShape& oShape)
	: CCtrlGeneralShape(oShape)
{}

CCtrlShapeRect::CCtrlShapeRect(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrlGeneralShape(sCtrlID, nSize, oBuffer, nOff, nVersion)
{}

CCtrlShapeRect::CCtrlShapeRect(const HWP_STRING& sCtrlID, CXMLReader& oReader, EHanType eType)
    : CCtrlGeneralShape(sCtrlID, oReader, eType)
{
	switch(eType)
	{
		case EHanType::HWPX:  ReadFromHWPX (oReader); return;
		case EHanType::HWPML: ReadFromHWPML(oReader); return;
		default: break;
	}
}

void CCtrlShapeRect::ReadFromHWPX(CXMLReader &oReader)
{
	m_chCurv = (HWP_BYTE)oReader.GetAttributeInt("ratio");

	#define READ_POINT(point_index)\
	{\
		START_READ_ATTRIBUTES(oReader)\
		{\
			if ("x" == sAttributeName)\
				m_arPoints[point_index].m_nX = oReader.GetInt();\
			else if ("y" == sAttributeName)\
				m_arPoints[point_index].m_nY = oReader.GetInt();\
		}\
		END_READ_ATTRIBUTES(oReader)\
	}

	WHILE_READ_NEXT_NODE_WITH_NAME(oReader)
	{
		if ("hc:pt0" == sNodeName)
			READ_POINT(0)
		else if ("hc:pt1" == sNodeName)
			READ_POINT(1)
		else if ("hc:pt2" == sNodeName)
			READ_POINT(2)
		else if ("hc:pt3" == sNodeName)
			READ_POINT(3)
		else
			CCtrlGeneralShape::ParseChildren(oReader, EHanType::HWPX);
	}
	END_WHILE
}

void CCtrlShapeRect::ReadFromHWPML(CXMLReader &oReader)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if ("Ratio" == sAttributeName)
			m_chCurv = (HWP_BYTE)oReader.GetInt();
		else if ("X0" == sAttributeName)
			m_arPoints[0].m_nX = oReader.GetInt();
		else if ("Y0" == sAttributeName)
			m_arPoints[0].m_nY = oReader.GetInt();
		else if ("X1" == sAttributeName)
			m_arPoints[1].m_nX = oReader.GetInt();
		else if ("Y1" == sAttributeName)
			m_arPoints[1].m_nY = oReader.GetInt();
		else if ("X2" == sAttributeName)
			m_arPoints[2].m_nX = oReader.GetInt();
		else if ("Y2" == sAttributeName)
			m_arPoints[2].m_nY = oReader.GetInt();
		else if ("X3" == sAttributeName)
			m_arPoints[3].m_nX = oReader.GetInt();
		else if ("Y3" == sAttributeName)
			m_arPoints[3].m_nY = oReader.GetInt();
	}
	END_READ_ATTRIBUTES(oReader)

	WHILE_READ_NEXT_NODE(oReader)
		CCtrlGeneralShape::ParseChildren(oReader, EHanType::HWPML);
	END_WHILE
}

EShapeType CCtrlShapeRect::GetShapeType() const
{
	return EShapeType::Rect;
}

void CCtrlShapeRect::GetPoints(TPoint (&arPoints)[4]) const
{
	memcpy(arPoints, m_arPoints, sizeof(TPoint) * 4);
}

int CCtrlShapeRect::ParseElement(CCtrlShapeRect& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	oBuffer.SavePosition();

	oBuffer.ReadByte(oObj.m_chCurv);

	for (unsigned int unIndex = 0; unIndex < 4; ++unIndex)
	{
		oBuffer.ReadInt(oObj.m_arPoints[unIndex].m_nX);
		oBuffer.ReadInt(oObj.m_arPoints[unIndex].m_nY);
	}

	oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos(true));
	return nSize;
}

int CCtrlShapeRect::ParseCtrl(CCtrlShapeRect& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	return CCtrlGeneralShape::ParseCtrl(oObj, nSize, oBuffer, nOff, nVersion);
}

int CCtrlShapeRect::ParseListHeaderAppend(CCtrlShapeRect& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	oBuffer.SavePosition();

	oBuffer.Skip(2);

	oBuffer.ReadShort(oObj.m_shLeftSpace);
	oBuffer.ReadShort(oObj.m_shRightSpace);
	oBuffer.ReadShort(oObj.m_shTopSpace);
	oBuffer.ReadShort(oObj.m_shBottomSpace);
	oBuffer.ReadInt(oObj.m_nMaxTxtWidth);

	if (nSize - 12 <= 13)
	{
		oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos(true));
		return nSize;
	}

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
