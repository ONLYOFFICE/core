#include "CCtrlField.h"

namespace HWP
{
CCtrlField::CCtrlField(const HWP_STRING& sCtrlID)
	: CCtrl(sCtrlID), m_eType(EFieldType::Unknown)
{
	if (L"klh%" == sCtrlID)
		m_eType = EFieldType::Hyperlink;
	else if (L"klh " == sCtrlID)
		m_eType = EFieldType::HyperlinkClosing;
}

CCtrlField::CCtrlField(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrl(sCtrlID), m_eType(EFieldType::Unknown)
{
	if (L"klh%" == sCtrlID)
		m_eType = EFieldType::Hyperlink;
	else if (L"klh " == sCtrlID)
		m_eType = EFieldType::HyperlinkClosing;

	oBuffer.ReadInt(m_nProperty);
	oBuffer.ReadByte(m_chEtcProperty);
	oBuffer.ReadString(m_sCommand, EStringCharacter::UTF16);
	oBuffer.ReadInt(m_nInstanceID);
}

CCtrlField::CCtrlField(const HWP_STRING& sCtrlID, CXMLNode& oNode, int nVersion)
	: CCtrl(sCtrlID), m_eType(EFieldType::Unknown)
{
	if (L"hp:fieldBegin" == oNode.GetName())
	{
		HWP_STRING sType = oNode.GetAttribute(L"type");

		if (sType == L"HYPERLINK")
			m_eType = EFieldType::Hyperlink;

		m_nInstanceID = oNode.GetAttributeInt(L"fieldid");

		for (CXMLNode& oChild : oNode.GetChild(L"hp:parameters").GetChilds(L"hp:stringParam"))
		{
			if (L"Command" == oChild.GetAttribute(L"name"))
				m_sCommand = oChild.GetText();
		}

		for (CXMLNode& oChild : oNode.GetChild(L"hp:parameters").GetChilds(L"hp:integerParam"))
		{
			if (L"Prop" == oChild.GetAttribute(L"name"))
				m_chEtcProperty = stoi(oChild.GetText());
		}
	}
	else if (L"hp:fieldEnd" == oNode.GetName())
	{
		m_nInstanceID = oNode.GetAttributeInt(L"fieldid");
	}
}

ECtrlObjectType CCtrlField::GetCtrlType() const
{
	return ECtrlObjectType::Field;
}

int CCtrlField::GetInstanceID() const
{
	return m_nInstanceID;
}

EFieldType CCtrlField::GetType() const
{
	return m_eType;
}

HWP_STRING CCtrlField::GetCommand() const
{
	return m_sCommand;
}

}
