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

CCtrlShapeRect::CCtrlShapeRect(const HWP_STRING& sCtrlID, CXMLNode& oNode, int nVersion)
	: CCtrlGeneralShape(sCtrlID, oNode, nVersion)
{
	m_chCurv = (HWP_BYTE)oNode.GetAttributeInt(L"ratio");

	for (unsigned int unIndex = 0; unIndex < 4; ++unIndex)
	{
		if ((L"hc:pt" + std::to_wstring(unIndex)) == oNode.GetName())
		{
			m_arPoints[unIndex].m_nX = oNode.GetAttributeInt(L"x");
			m_arPoints[unIndex].m_nY = oNode.GetAttributeInt(L"y");
			break;
		}
	}
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
