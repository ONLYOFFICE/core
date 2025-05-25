#include "CtrlEqEdit.h"

namespace HWP
{
CCtrlEqEdit::CCtrlEqEdit()
{}

CCtrlEqEdit::CCtrlEqEdit(const HWP_STRING& sCtrlID)
	: CCtrlGeneralShape(sCtrlID)
{}

CCtrlEqEdit::CCtrlEqEdit(const CCtrlGeneralShape& oShape)
	: CCtrlGeneralShape(oShape)
{}

CCtrlEqEdit::CCtrlEqEdit(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrlGeneralShape(sCtrlID, nSize, oBuffer, nOff, nVersion)
{}

CCtrlEqEdit::CCtrlEqEdit(const HWP_STRING& sCtrlID, CXMLNode& oNode, int nVersion)
	: CCtrlGeneralShape(sCtrlID)
{
	m_sVersion = oNode.GetAttribute(L"version");
	m_nBaseline = oNode.GetAttributeInt(L"baseLine");
	m_nColor = oNode.GetAttributeColor(L"textColor");
	m_nCharSize = oNode.GetAttributeInt(L"baseUnit");

	HWP_STRING sType = oNode.GetAttribute(L"lineMode");

	if (L"LINE" == sType)
		m_nAttr = 1;
	else if (L"CHAR" == sType)
		m_nAttr = 0;

	m_sFont = oNode.GetAttribute(L"font");
	m_sEqn = oNode.GetChild(L"hp:script").GetText();

	m_bFullFilled = true;
}

EShapeType CCtrlEqEdit::GetShapeType() const
{
	return EShapeType::EqEdit;
}

HWP_STRING CCtrlEqEdit::GetEqn() const
{
	return m_sEqn;
}

int CCtrlEqEdit::ParseElement(CCtrlEqEdit& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	oBuffer.SavePosition();

	oObj.m_bFullFilled = true;

	oBuffer.ReadInt(oObj.m_nAttr);
	oBuffer.ReadString(oObj.m_sEqn, EStringCharacter::UTF16);
	oBuffer.ReadInt(oObj.m_nCharSize);
	oBuffer.ReadColor(oObj.m_nColor);
	oBuffer.ReadInt(oObj.m_nBaseline);
	oBuffer.ReadString(oObj.m_sVersion, EStringCharacter::UTF16);

	if (oBuffer.GetDistanceToLastPos() + 2 <= nSize)
	{
		short shLen = oBuffer.ReadShort();

		if (oBuffer.GetDistanceToLastPos() + shLen <= nSize)
			oBuffer.ReadString(oObj.m_sFont, shLen, EStringCharacter::UTF16);
	}

	oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos(true));

	return nSize;
}

int CCtrlEqEdit::ParseCtrl(CCtrlEqEdit& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	return CCtrlCommon::ParseCtrl(oObj, nSize, oBuffer, nOff, nVersion);
}

int CCtrlEqEdit::ParseListHeaderAppend(CCtrlEqEdit& oObj, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
{
	if (24 != nSize)
	{
		oBuffer.Skip(nSize);
		return nSize;
	}

	oBuffer.Skip(2);
	oBuffer.ReadInt(oObj.m_nCaptionAttr);
	oBuffer.ReadInt(oObj.m_nCaptionWidth);
	oObj.m_nCaptionSpacing = oBuffer.ReadShort();
	oBuffer.ReadInt(oObj.m_nCaptionMaxW);
	oBuffer.Skip(8);

	return nSize;
}

}
