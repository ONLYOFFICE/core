#include "HWPRecordBinData.h"

#include "../HWPFile.h"

#include <iomanip>
#include <regex>
#include <sstream>

namespace HWP
{
ECompressed GetCompressed(int nValue)
{
	SWITCH(ECompressed, nValue)
	{
		DEFAULT(ECompressed::FOLLOW_STORAGE);
		CASE(ECompressed::COMPRESS);
		CASE(ECompressed::NO_COMPRESS);
	}
}

EType GetType(int nValue)
{
	SWITCH(EType, nValue)
	{
		DEFAULT(EType::LINK);
		CASE(EType::EMBEDDING);
		CASE(EType::STORAGE);
	}
}


EState GetState(int nValue)
{
	SWITCH(EState, nValue)
	{
		DEFAULT(EState::NEVER_ACCESSED);
		CASE(EState::FOUND_FILE_BY_ACCESS);
		CASE(EState::ACCESS_FAILED);
		CASE(EState::LINK_ACCESS_IGNORED);
	}
}

CHWPRecordBinData::CHWPRecordBinData(CHWPDocInfo& oDocInfo, int nTagNum, int nLevel, int nSize, CHWPStream& oBuffer, int nOff, int nVersion)
	: CHWPRecord(nTagNum, nLevel, nSize)
{
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

CHWPRecordBinData::CHWPRecordBinData(CXMLReader& oReader, int nVersion)
	: CHWPRecord(EHWPTag::HWPTAG_BIN_DATA, 0, 0)
{
	std::string sType;
	HWP_STRING sSubPath;

	START_READ_ATTRIBUTES(oReader)
	{
		if ("id" == sAttributeName)
			m_sItemID = oReader.GetText();
		else if ("isEmbeded" == sAttributeName)
			sType = oReader.GetTextA();
		else if ("href" == sAttributeName)
			m_sAPath = oReader.GetText();
		else if ("sub-path" == sAttributeName)
			sSubPath = oReader.GetText();
		else if ("media-type" == sAttributeName)
		{
			m_sFormat = oReader.GetText();

			std::wregex oRegex(L"image/(.*)");
			m_sFormat = std::regex_replace(m_sFormat, oRegex, L"$1");
		}
	}
	END_READ_ATTRIBUTES(oReader)

	if ("0" == sType)
	{
		m_eType = EType::LINK;

		if (!sSubPath.empty())
			m_sAPath = sSubPath;
	}
	else if ("1" == sType)
		m_eType = EType::EMBEDDING;
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
