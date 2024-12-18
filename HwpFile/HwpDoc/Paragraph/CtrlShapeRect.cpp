#include "CtrlShapeRect.h"

namespace HWP
{
CCtrlShapeRect::CCtrlShapeRect()
{}

CCtrlShapeRect::CCtrlShapeRect(const STRING& sCtrlID)
	: CCtrlGeneralShape(sCtrlID)
{}

CCtrlShapeRect::CCtrlShapeRect(const STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrlGeneralShape(sCtrlID, nSize, oBuffer, nOff, nVersion)
{}

int CCtrlShapeRect::ParseElement(CCtrlShapeRect& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	oBuffer.ReadByte(oObj.m_chCurv);

	for (unsigned int unIndex = 0; unIndex < 4; ++unIndex)
	{
		oBuffer.ReadInt(oObj.m_arPoints[unIndex].m_nX);
		oBuffer.ReadInt(oObj.m_arPoints[unIndex].m_nY);
	}

	return nSize;
}

int CCtrlShapeRect::ParseCtrl(CCtrlShapeRect& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	return CCtrlGeneralShape::ParseCtrl(oObj, nSize, oBuffer, nOff, nVersion);
}

int CCtrlShapeRect::ParseListHeaderAppend(CCtrlShapeRect& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	BYTE* pOldCurentPos = oBuffer.GetCurPtr();

	oBuffer.Skip(2);

	oBuffer.ReadShort(oObj.m_shLeftSpace);
	oBuffer.ReadShort(oObj.m_shRightSpace);
	oBuffer.ReadShort(oObj.m_shTopSpace);
	oBuffer.ReadShort(oObj.m_shBottomSpace);
	oBuffer.ReadInt(oObj.m_nMaxTxtWidth);

	if (nSize - 12 <= 13)
		return nSize;

	oBuffer.Skip(13);

	if (nSize > (oBuffer.GetCurPtr() - pOldCurentPos))
	{
		oBuffer.Skip(10);
		STRING sFieldName;
		oBuffer.ReadString(sFieldName, EStringCharacter::UTF16);

		oBuffer.Skip(nSize - (oBuffer.GetCurPtr() - pOldCurentPos));
	}

	return oBuffer.GetCurPtr() - pOldCurentPos;
}

}
