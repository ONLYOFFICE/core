#include "HWPMLFile.h"

#include "../../DesktopEditor/common/File.h"

namespace HWP
{

CHWPMLFile::CHWPMLFile(const HWP_STRING &wsFilePath)
    : m_wsFilePath(wsFilePath), m_oDocInfo(this)
{}

CHWPMLFile::~CHWPMLFile()
{
	CLEAR_ARRAY(CHWPSection, m_arSections);
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
	WHILE_READ_NEXT_NODE_WITH_NAME(oReader)
	{
		if ("SECTION" == sNodeName)
			ReadSection(oReader, 0);
	}
	END_WHILE
}

void CHWPMLFile::ReadTail(CXMLReader &oReader)
{
	WHILE_READ_NEXT_NODE_WITH_NAME(oReader)
	{
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

VECTOR<const CHWPSection*> CHWPMLFile::GetSections() const
{
	RETURN_VECTOR_CONST_PTR(CHWPSection, m_arSections);
}
}
