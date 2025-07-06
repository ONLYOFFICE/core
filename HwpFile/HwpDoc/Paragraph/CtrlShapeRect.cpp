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

CCtrlShapeRect::CCtrlShapeRect(const HWP_STRING& sCtrlID, CXMLReader& oReader, int nVersion)
    : CCtrlGeneralShape(sCtrlID, oReader, nVersion)
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
	}
	END_WHILE
}

EShapeType CCtrlShapeRect::GetShapeType() const
{
	return EShapeType::Rect;
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
