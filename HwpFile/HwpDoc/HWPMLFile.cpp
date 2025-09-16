#include "HWPMLFile.h"

#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Base64.h"

namespace HWP
{

CHWPMLFile::CHWPMLFile(const HWP_STRING &wsFilePath)
    : m_wsFilePath(wsFilePath), m_oDocInfo(this)
{}

CHWPMLFile::~CHWPMLFile()
{
	CLEAR_ARRAY(CHWPSection, m_arSections);

	for (std::pair<unsigned int, BYTE*> oValue : m_mBinDates)
		if (nullptr != oValue.second)
			delete[] oValue.second;
}

bool CHWPMLFile::Open()
{
	if (m_wsFilePath.empty() || !NSFile::CFileBinary::Exists(m_wsFilePath) || !Detect())
		return false;

	CXMLReader oReader;

	oReader.GetReader()->FromFile(m_wsFilePath);
	oReader.ReadNextNode();

	WHILE_READ_NEXT_NODE_WITH_NAME(oReader)
	{
		if ("HEAD" == sNodeName)
			ReadHead(oReader);
		else if ("BODY" == sNodeName)
			ReadBody(oReader);
		else if ("TAIL" == sNodeName)
			ReadTail(oReader);
	}
	END_WHILE

	return true;
}

bool CHWPMLFile::Detect() const
{
	if (m_wsFilePath.empty())
		return false;

	CXMLReader oReader;

	if (nullptr == oReader.GetReader() || !oReader.GetReader()->FromFile(m_wsFilePath))
		return false;

	if (!oReader.ReadNextNode() && "HWPML" != oReader.GetName())
		return false;

	return true;
}

void CHWPMLFile::Close()
{}

const CHWPDocInfo* CHWPMLFile::GetDocInfo() const
{
	return &m_oDocInfo;
}

void CHWPMLFile::ReadHead(CXMLReader &oReader)
{
	m_oDocInfo.Parse(oReader);
}

void CHWPMLFile::ReadBody(CXMLReader &oReader)
{
	WHILE_READ_NEXT_NODE_WITH_ONE_NAME(oReader, "SECTION")
		ReadSection(oReader, 0);
	END_WHILE
}

void CHWPMLFile::ReadTail(CXMLReader &oReader)
{
	WHILE_READ_NEXT_NODE_WITH_ONE_NAME(oReader, "BINDATASTORAGE")
	{
		WHILE_READ_NEXT_NODE_WITH_DEPTH_AND_NAME(oReader, Item)
		{
			if ("BINDATA" == sNodeItemName)
				ReadBinData(oReader);
		}
		END_WHILE
	}
	END_WHILE
}

void CHWPMLFile::ReadSection(CXMLReader &oReader, int nVersion)
{
	CHWPSection* pSection = new CHWPSection();

	if (pSection->Parse(oReader, nVersion, EHanType::HWPML))
		m_arSections.push_back(pSection);
	else
		delete pSection;
}

void CHWPMLFile::ReadBinData(CXMLReader &oReader)
{
	size_t unSize = 0;
	std::string sEncoding;
	unsigned int unID = 0;
	bool bCompress = true;

	START_READ_ATTRIBUTES(oReader)
	{
		if ("Size" == sAttributeName)
			unSize = oReader.GetInt();
		else if ("Encoding" == sAttributeName)
			sEncoding = oReader.GetTextA();
		else if ("Id" == sAttributeName)
			unID = oReader.GetInt();
		else if ("Compress" == sAttributeName)
			bCompress = oReader.GetBool();
	}
	END_READ_ATTRIBUTES(oReader)

	const std::wstring wsImageData{oReader.GetText()};

	if (0 == unSize)
		unSize = wsImageData.length();

	int nImageSize = NSBase64::Base64DecodeGetRequiredLength(unSize);

	BYTE* pBuffer = new BYTE[nImageSize];

	if (nullptr == pBuffer)
		return;

	if (FALSE == NSBase64::Base64Decode(wsImageData.c_str(), unSize, pBuffer, &nImageSize))
	{
		delete[] pBuffer;
		return;
	}

	std::map<unsigned int, BYTE*>::iterator itFound = m_mBinDates.find(unID);

	if (m_mBinDates.end() != itFound)
	{
		if (nullptr != itFound->second)
			delete[] itFound->second;

		itFound->second = pBuffer;
		return;
	}

	m_mBinDates.insert(std::make_pair(unID, pBuffer));
}

VECTOR<const CHWPSection*> CHWPMLFile::GetSections() const
{
	RETURN_VECTOR_CONST_PTR(CHWPSection, m_arSections);
}

const BYTE *HWP::CHWPMLFile::GetBinData(unsigned int unID) const
{
	std::map<unsigned int, BYTE*>::const_iterator itFound = m_mBinDates.find(unID);

	if (m_mBinDates.end() == itFound)
		return nullptr;

	return itFound->second;
}
}
