#include "CtrlEqEdit.h"

#include "../Common/NodeNames.h"

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

CCtrlEqEdit::CCtrlEqEdit(const HWP_STRING& sCtrlID, CXMLReader& oReader, int nVersion, EHanType eType)
	: CCtrlGeneralShape(sCtrlID, oReader, nVersion, eType)
{
	START_READ_ATTRIBUTES(oReader)
	{
		if (GetAttributeName(EAttribute::Version, eType) == sAttributeName)
			m_sVersion = oReader.GetText();
		else if (GetAttributeName(EAttribute::BaseLine, eType) == sAttributeName)
			m_nBaseline = oReader.GetInt();
		else if (GetAttributeName(EAttribute::TextColor, eType) == sAttributeName)
			m_nColor = oReader.GetColor();
		else if (GetAttributeName(EAttribute::BaseUnit, eType) == sAttributeName)
			m_nCharSize = oReader.GetInt();
		else if (GetAttributeName(EAttribute::LineMode, eType) == sAttributeName)
		{
			switch(eType)
			{
				case EHanType::HWPX:
				{
					const std::string sType{oReader.GetTextA()};

					if ("LINE" == sType)
						m_nAttr = 1;
					else if ("CHAR" == sType)
						m_nAttr = 0;

					break;
				}
				case EHanType::HWPML:
					m_nAttr = oReader.GetBool();
			}
		}
		else if (EHanType::HWPX == eType && "font" == sAttributeName)
			m_sFont = oReader.GetText();
	}
	END_READ_ATTRIBUTES(oReader)

	WHILE_READ_NEXT_NODE(oReader)
	{
		if (GetNodeName(ENode::Script, eType) == oReader.GetName())
			m_sEqn = oReader.GetText();
		else
			CCtrlGeneralShape::ParseChildren(oReader, nVersion, eType);
	}
	END_WHILE

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
