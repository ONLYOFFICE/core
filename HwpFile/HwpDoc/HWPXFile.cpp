#include "HWPXFile.h"

#include <cwctype>

namespace HWP
{
CHWPXFile::CHWPXFile(const HWP_STRING& sFileName)
	: m_sFileName(sFileName), m_pZipFolder(nullptr), m_oDocInfo(this)
{}

CHWPXFile::~CHWPXFile()
{
	Close();
	CLEAR_ARRAY(CHWPSection, m_arSections);
}

VECTOR<const CHWPSection*> CHWPXFile::GetSections() const
{
	RETURN_VECTOR_CONST_PTR(CHWPSection, m_arSections);
}

bool CHWPXFile::Detect()
{
	BYTE *pBuffer = NULL;
	DWORD dwBufferSize = 0;

	if (!NSFile::CFileBinary::ReadAllBytes(m_sFileName, &pBuffer, dwBufferSize))
		return false;

	m_pZipFolder = new CZipFolderMemory(pBuffer, dwBufferSize);

	return GetFileHeader();
}

bool CHWPXFile::Open()
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

void CHWPXFile::Close()
{
	if (nullptr != m_pZipFolder)
	{
		delete m_pZipFolder;
		m_pZipFolder = nullptr;
	}
}

VECTOR<HWP_STRING> CHWPXFile::GetPathsToSections() const
{
	VECTOR<std::wstring> arPaths{m_pZipFolder->getFiles(L"Contents", false)};

	if (arPaths.empty())
		return VECTOR<HWP_STRING>{};

	struct TPathInfo
	{
		std::wstring m_wsPath;
		unsigned short m_ushIndex;

		bool operator<(const TPathInfo& oPathInfo) const
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

bool CHWPXFile::GetFileHeader()
{
	CXMLNode oVersionXml{GetDocument(L"version.xml")};
	CXMLReader oReader{GetDocument(L"version.xml")};
	return m_oFileHeader.Parse(oReader);
}

const CHWPDocInfo* CHWPXFile::GetDocInfo() const
{
	return &m_oDocInfo;
}

bool CHWPXFile::GetChildStream(const HWP_STRING& sFileName, CHWPStream& oBuffer)
{
	if (nullptr == m_pZipFolder || !m_pZipFolder->exists(sFileName))
		return false;

	CZipFolderMemory::CBuffer *pFileBuffer = nullptr;
	m_pZipFolder->read(sFileName, pFileBuffer);

	if (nullptr == pFileBuffer)
		return false;

	pFileBuffer->UnsetDestroy();

	oBuffer.SetStream((HWP_BYTE*)pFileBuffer->Buffer, pFileBuffer->Size);

	delete pFileBuffer;

	return true;
}

bool CHWPXFile::GetDocInfo(int nVersion)
{
	CXMLReader oContentReader{GetDocument(L"Contents/content.hpf")};
	if (m_oDocInfo.ReadContentHpf(oContentReader, nVersion))
	{
		CXMLReader oHeaderReader{GetDocument(L"Contents/header.xml")};
		return m_oDocInfo.Parse(oHeaderReader, nVersion);
	}

	return false;
}

bool CHWPXFile::ReadSection(const HWP_STRING& sName, int nVersion)
{
	CXMLReader oReader{GetDocument(sName)};

	CHWPSection* pSection = new CHWPSection();
	const bool bResult = pSection->Parse(oReader, nVersion);

	if (bResult)
		m_arSections.push_back(pSection);
	else
		delete pSection;

	return bResult;
}

CXMLReader CHWPXFile::GetDocument(const HWP_STRING& sEntryName)
{
	if (nullptr == m_pZipFolder)
		return CXMLReader();

	XmlUtils::CXmlLiteReader oTempReader{m_pZipFolder->getReaderFromFile(sEntryName)};

	std::cout << oTempReader.GetNameA() << std::endl;

	if (oTempReader.IsValid())
		std::cout << "Valid" << std::endl;
	else
		std::cout << "No valid" << std::endl;

	oTempReader.ReadNextNode();

	if (oTempReader.IsValid())
		std::cout << "Valid" << std::endl;
	else
		std::cout << "No valid" << std::endl;

	std::wcout << oTempReader.GetDepth() << L" _ " << oTempReader.GetName() << std::endl;

	CXMLReader oReader(oTempReader);

	std::wcout << oReader.GetInnerXml() << std::endl;


	return oReader;
}
}
