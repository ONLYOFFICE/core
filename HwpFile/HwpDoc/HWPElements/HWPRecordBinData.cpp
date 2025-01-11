#include "HWPRecordBinData.h"

#include "../HWPFile_Private.h"

#include <iomanip>
#include <regex>
#include <sstream>

namespace HWP
{
ECompressed GetCompressed(int nValue)
{
	SWITCH(ECompressed, nValue)
	{
		CASE(ECompressed::FOLLOW_STORAGE);
		CASE(ECompressed::COMPRESS);
		CASE(ECompressed::NO_COMPRESS);
		DEFAULT(ECompressed::UNKNOWN);
	}
}

EType GetType(int nValue)
{
	SWITCH(EType, nValue)
	{
		CASE(EType::LINK);
		CASE(EType::EMBEDDING);
		CASE(EType::STORAGE);
		DEFAULT(EType::UNKNOWN);
	}
}

EState GetState(int nValue)
{
	SWITCH(EState, nValue)
	{
		CASE(EState::NEVER_ACCESSED);
		CASE(EState::FOUND_FILE_BY_ACCESS);
		CASE(EState::ACCESS_FAILED);
		CASE(EState::LINK_ACCESS_IGNORED);
		DEFAULT(EState::UNKNOWN);
	}
}

CHWPRecordBinData::CHWPRecordBinData(CHWPDocInfo& oDocInfo, int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CHWPRecord(nTagNum, nLevel, nSize)
{
	if (EHanType::HWP == oDocInfo.GetHanType())
	{
		if (oDocInfo.GetParentHWP()->GetBinData().empty())
			oDocInfo.GetParentHWP()->SetBinData(oDocInfo.GetParentHWP()->GetOleFile()->GetChildEntries(L"BinData"));
	}

	oBuffer.SavePosition();

	short shTypeBits;
	oBuffer.ReadShort(shTypeBits);

	m_eType = ::HWP::GetType(shTypeBits & 0x0F);
	m_eCompressed = ::HWP::GetCompressed(shTypeBits & 0x30);
	m_eState = GetState(shTypeBits & 0x300);

	if (EType::LINK == m_eType)
	{
		oBuffer.ReadString(m_sAPath, EStringCharacter::UTF16);
		oBuffer.ReadString(m_sRPath, EStringCharacter::UTF16);
	}
	if (EType::EMBEDDING == m_eType || EType::STORAGE == m_eType)
	{
		oBuffer.ReadShort(m_shBinDataID);
		m_sItemID = TO_HWP_STRING(m_shBinDataID);
	}
	if (EType::EMBEDDING == m_eType || EType::STORAGE == m_eType)
	{
		oBuffer.ReadString(m_sFormat, EStringCharacter::UTF16);

		std::wostringstream oStringStream;
		oStringStream << L"BIN" << std::setw(4) << std::setfill(L'0') << std::hex << m_shBinDataID << L"." << m_sFormat;
		m_sAPath = oStringStream.str();
	}

	oBuffer.Skip(nSize - oBuffer.GetDistanceToLastPos(true));
}

CHWPRecordBinData::CHWPRecordBinData(XmlUtils::CXmlNode& oNode, int nVersion)
	: CHWPRecord(EHWPTag::HWPTAG_BIN_DATA, 0, 0)
{
	m_sItemID = oNode.GetAttribute(L"id");

	HWP_STRING sType = oNode.GetAttribute(L"isEmbeded");

	if (L"0" == sType)
	{
		m_eType = EType::LINK;

		m_sAPath = oNode.GetAttribute(L"sub-path");

		if (m_sAPath.empty())
			m_sAPath = oNode.GetAttribute(L"href");
	}
	else if (L"1" == sType)
	{
		m_eType = EType::EMBEDDING;
		m_sAPath = oNode.GetAttribute(L"href");
	}
	else
		m_sAPath = oNode.GetAttribute(L"href");

	m_sFormat = oNode.GetAttribute(L"media-type");

	std::wregex oRegex(L"image/(.*)");

	m_sFormat = std::regex_replace(m_sFormat, oRegex, L"$1");
}

HWP_STRING CHWPRecordBinData::GetPath() const
{
	return m_sAPath;
}

HWP_STRING CHWPRecordBinData::GetItemID() const
{
	return m_sItemID;
}

HWP_STRING CHWPRecordBinData::GetFormat() const
{
	return m_sFormat;
}

EType CHWPRecordBinData::GetType() const
{
	return m_eType;
}

ECompressed CHWPRecordBinData::GetCompressed() const
{
	return m_eCompressed;
}

short CHWPRecordBinData::GetBinDataID() const
{
	return m_shBinDataID;
}
}
