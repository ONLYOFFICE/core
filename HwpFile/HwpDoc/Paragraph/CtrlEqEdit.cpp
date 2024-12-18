#include "CtrlEqEdit.h"

namespace HWP
{
CCtrlEqEdit::CCtrlEqEdit()
{}

CCtrlEqEdit::CCtrlEqEdit(const STRING& sCtrlID)
	: CCtrlGeneralShape(sCtrlID)
{}

CCtrlEqEdit::CCtrlEqEdit(const STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrlGeneralShape(sCtrlID, nSize, oBuffer, nOff, nVersion)
{}

int CCtrlEqEdit::ParseElement(CCtrlEqEdit& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	BYTE* pOldCurentPos = oBuffer.GetCurPtr();

	oObj.m_bFullFilled = true;

	oBuffer.ReadInt(oObj.m_nAttr);
	oBuffer.ReadString(oObj.m_sEqn, EStringCharacter::UTF16);
	oBuffer.ReadInt(oObj.m_nCharSize);
	oBuffer.ReadColor(oObj.m_nColor);
	oBuffer.ReadInt(oObj.m_nBaseline);
	oBuffer.ReadString(oObj.m_sVersion, EStringCharacter::UTF16);

	if (oBuffer.GetCurPtr() - pOldCurentPos +2 > nSize)
		return nSize;

	oBuffer.ReadString(oObj.m_sFont, EStringCharacter::UTF16);

	return nSize;
}

int CCtrlEqEdit::ParseCtrl(CCtrlEqEdit& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	return CCtrlCommon::ParseCtrl(oObj, nSize, oBuffer, nOff, nVersion);
}

int CCtrlEqEdit::ParseListHeaderAppend(CCtrlEqEdit& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	if (24 != nSize)
		return nSize;

	oBuffer.Skip(2);
	oBuffer.ReadInt(oObj.m_nCaptionAttr);
	oBuffer.ReadInt(oObj.m_nCaptionWidth);
	oObj.m_nCaptionSpacing = oBuffer.ReadShort();
	oBuffer.ReadInt(oObj.m_nCaptionMaxW);
	oBuffer.Skip(8);

	return nSize;
}

}
