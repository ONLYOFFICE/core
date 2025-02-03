#include "CtrlField.h"
#include <iostream>
#include <ostream>

namespace HWP
{
CCtrlField::CCtrlField(const HWP_STRING& sCtrlID)
	: CCtrl(sCtrlID), m_eType(EFieldType::Unknown)
{
	UpdateType(sCtrlID);
}

CCtrlField::CCtrlField(const HWP_STRING& sCtrlID, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CCtrl(sCtrlID), m_eType(EFieldType::Unknown)
{
	UpdateType(sCtrlID);

	oBuffer.ReadInt(m_nProperty);
	oBuffer.ReadByte(m_chEtcProperty);
	oBuffer.ReadString(m_sCommand, EStringCharacter::UTF16);
	oBuffer.ReadInt(m_nInstanceID);
}

void CCtrlField::UpdateType(const HWP_STRING& sCtrlID)
{
	if (L"klh%" == sCtrlID)
		m_eType = EFieldType::Hyperlink;
	else if (L"klh " == sCtrlID)
		m_eType = EFieldType::HyperlinkClosing;
	else if (L"kmb%" == sCtrlID)
		m_eType = EFieldType::Bookmark;
	else if (L"kmb " == sCtrlID)
		m_eType = EFieldType::BookmarkClosing;
}

CCtrlField::CCtrlField(const HWP_STRING& sCtrlID, CXMLNode& oNode, int nVersion)
	: CCtrl(sCtrlID), m_eType(EFieldType::Unknown)
{
	if (L"hp:fieldBegin" == oNode.GetName())
	{
		HWP_STRING sType = oNode.GetAttribute(L"type");

		if (L"HYPERLINK" == sType)
			m_eType = EFieldType::Hyperlink;
		else if (L"BOOKMARK" == sType)
			m_eType = EFieldType::Bookmark;

		m_nInstanceID = oNode.GetAttributeInt(L"fieldid");

		HWP_STRING sName = oNode.GetAttribute(L"name");

		if (!sName.empty() && EFieldType::Bookmark == m_eType)
			AddStringParam(L"bookmarkname", sName);

		for (CXMLNode& oChild : oNode.GetChild(L"hp:parameters").GetChilds())
		{
			if (L"hp:stringParam" == oChild.GetName())
				AddStringParam(oChild.GetAttribute(L"name"), oChild.GetText());
			else if (L"hp:integerParam" == oChild.GetName())
				AddIntegerParam(oChild.GetAttribute(L"name"), std::stoi(oChild.GetText()));
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

void CCtrlField::AddStringParam(const HWP_STRING& wsName, const HWP_STRING& wsValue)
{
	m_mStringParams.insert(std::make_pair(wsName, wsValue));
}

void CCtrlField::AddIntegerParam(const HWP_STRING& wsName, int nValue)
{
	m_mIntegerParam.insert(std::make_pair(wsName, nValue));
}

HWP_STRING CCtrlField::GetStringParam(const HWP_STRING& wsName) const
{
	std::map<HWP_STRING, HWP_STRING>::const_iterator itFound = m_mStringParams.find(wsName);

	return ((m_mStringParams.cend() != itFound) ? itFound->second : HWP_STRING());
}

int CCtrlField::GetIntegerParam(const HWP_STRING& wsName) const
{
	std::map<HWP_STRING, int>::const_iterator itFound = m_mIntegerParam.find(wsName);

	return ((m_mIntegerParam.cend() != itFound) ? itFound->second : 0);
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
