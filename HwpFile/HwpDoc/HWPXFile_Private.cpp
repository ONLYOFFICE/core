#include "HWPXFile_Private.h"

namespace HWP
{
CHWPXFile_Private::CHWPXFile_Private(const HWP_STRING& sFileName)
	: m_sFileName(sFileName), m_pZipFolder(nullptr), m_oDocInfo(this)
{}

CHWPXFile_Private::~CHWPXFile_Private()
{
	Close();
	CLEAR_ARRAY(CHWPSection, m_arSections);
}

VECTOR<const CHWPSection*> CHWPXFile_Private::GetSections() const
{
	RETURN_VECTOR_CONST_PTR(CHWPSection, m_arSections);
}

bool CHWPXFile_Private::Detect()
{
	BYTE *pBuffer = NULL;
	DWORD dwBufferSize = 0;

	if (!NSFile::CFileBinary::ReadAllBytes(m_sFileName, &pBuffer, dwBufferSize))
		return false;

	m_pZipFolder = new CZipFolderMemory(pBuffer, dwBufferSize);

	return GetFileHeader();
}

bool CHWPXFile_Private::Open()
{
	if (!NSFile::CFileBinary::Exists(m_sFileName))
		return false;

	if (m_oFileHeader.VersionEmpty() && !Detect())
		return false;

	m_nVersion = std::stoi(m_oFileHeader.GetVersion());

	if (!GetDocInfo(m_nVersion))
		return false;

	std::vector<std::wstring> arPathToSections{GetPathsToSections()};

	for (const std::wstring& wsPath : arPathToSections)
		ReadSection(wsPath, m_nVersion);

	return true;
}

void CHWPXFile_Private::Close()
{
	if (nullptr != m_pZipFolder)
	{
		delete m_pZipFolder;
		m_pZipFolder = nullptr;
	}
}

VECTOR<HWP_STRING> CHWPXFile_Private::GetPathsToSections() const
{
	VECTOR<std::wstring> arPaths{m_pZipFolder->getFiles(L"Contents", false)};

	if (arPaths.empty())
		return VECTOR<HWP_STRING>{};

	struct TPathInfo
	{
		std::wstring m_wsPath;
		unsigned short m_ushIndex;

		bool operator<(const TPathInfo& oPathInfo)
		{
			return m_ushIndex < oPathInfo.m_ushIndex;
		}
	};

	VECTOR<TPathInfo> arPathsToSectors;
	size_t unFoundPos, unNumberStart, unNumberEnd;

	for (const std::wstring& wsPath : arPaths)
	{
		unFoundPos = wsPath.find(L"/Contents/section");
		if (std::wstring::npos ==unFoundPos)
			continue;

		unNumberStart = unFoundPos + 17;
		unNumberEnd   = unNumberStart;

		while (unNumberEnd < wsPath.length() && iswdigit(wsPath[unNumberEnd]))
			++unNumberEnd;

		if (unNumberEnd == unNumberStart ||
		    unNumberEnd >= wsPath.length() ||
		    L".xml" != wsPath.substr(unNumberEnd, 4))
			continue;

		arPathsToSectors.push_back({wsPath, static_cast<unsigned short>(std::stoi(wsPath.substr(unNumberStart, unNumberEnd - unNumberStart)))});
	}

	std::sort(arPathsToSectors.begin(), arPathsToSectors.end());

	VECTOR<HWP_STRING> arResult{arPathsToSectors.size()};

	for (unsigned int unIndex = 0; unIndex < arPathsToSectors.size(); ++unIndex)
		arResult[unIndex] = arPathsToSectors[unIndex].m_wsPath;

	return arResult;
}

bool CHWPXFile_Private::GetFileHeader()
{
	CXMLNode oVersionXml{GetDocument(L"version.xml")};
	return m_oFileHeader.Parse(oVersionXml);
}

const CHWPDocInfo* CHWPXFile_Private::GetDocInfo() const
{
	return &m_oDocInfo;
}

bool CHWPXFile_Private::GetDocInfo(int nVersion)
{
	CXMLNode oContent{GetDocument(L"Contents/content.hpf")};
	if (m_oDocInfo.ReadContentHpf(oContent, nVersion))
	{
		CXMLNode oHeader{GetDocument(L"Contents/header.xml")};
		return m_oDocInfo.Parse(oHeader, nVersion);
	}

	return false;
}

bool CHWPXFile_Private::ReadSection(const HWP_STRING& sName, int nVersion)
{
	CXMLNode oRootNode{GetDocument(sName)};

	CHWPSection* pSection = new CHWPSection();
	const bool bResult = pSection->Parse(oRootNode, nVersion);

	if (bResult)
		m_arSections.push_back(pSection);
	else
		delete pSection;

	return bResult;
}

CXMLNode CHWPXFile_Private::GetDocument(const HWP_STRING& sEntryName)
{
	if (nullptr == m_pZipFolder)
		return CXMLNode();

	return CXMLNode(m_pZipFolder->getNodeFromFile(sEntryName));
}
}
