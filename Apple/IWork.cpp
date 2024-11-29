#include "IWork.h"
#include "../DesktopEditor/common/File.h"

#include <libetonyek/libetonyek.h>
#include <libodfgen/OdtGenerator.hxx>
#include <libodfgen/OdsGenerator.hxx>
#include <libodfgen/OdpGenerator.hxx>
#include <libodfgen/test/StringDocumentHandler.hxx>

#include <memory>
#include <iostream>
#include <fstream>

class CIWorkFile_Private
{
public:
	std::wstring m_sTempDirectory;

public:
	CIWorkFile_Private()
	{
	}
	~CIWorkFile_Private()
	{
	}
};

CIWorkFile::CIWorkFile()
{
	m_internal = new CIWorkFile_Private();
}

CIWorkFile::~CIWorkFile()
{
	delete m_internal;
}

bool GetRVNGInputStream(const std::string& sFileA, std::shared_ptr<librevenge::RVNGInputStream>& oRVNGInputStream, libetonyek::EtonyekDocument::Type& oDocumentType)
{
	if (librevenge::RVNGDirectoryStream::isDirectory(sFileA.c_str()))
		oRVNGInputStream.reset(new librevenge::RVNGDirectoryStream(sFileA.c_str()));
	else
		oRVNGInputStream.reset(new librevenge::RVNGFileStream(sFileA.c_str()));

	oDocumentType = libetonyek::EtonyekDocument::TYPE_UNKNOWN;
	const libetonyek::EtonyekDocument::Confidence confidence = libetonyek::EtonyekDocument::isSupported(oRVNGInputStream.get(), &oDocumentType);

	return libetonyek::EtonyekDocument::CONFIDENCE_NONE != confidence;
}

IWorkFileType CIWorkFile::GetType(const std::wstring& sFile) const
{
	std::string sFileA = U_TO_UTF8(sFile);

	std::shared_ptr<librevenge::RVNGInputStream> input;
	libetonyek::EtonyekDocument::Type oDocumentType;

	if (!GetRVNGInputStream(sFileA, input, oDocumentType))
		return IWorkFileType::None;

	switch (oDocumentType)
	{
	case libetonyek::EtonyekDocument::TYPE_PAGES:
		return IWorkFileType::Pages;
	case libetonyek::EtonyekDocument::TYPE_NUMBERS:
		return IWorkFileType::Numbers;
	case libetonyek::EtonyekDocument::TYPE_KEYNOTE:
		return IWorkFileType::Keynote;
	default:
		break;
	}

	return IWorkFileType::None;
}

template<class Generator>
int Convert(const std::wstring& wsOutputFile, std::shared_ptr<librevenge::RVNGInputStream>& ptrInput, const std::wstring& wsPassword = L"", const std::wstring& wsTempDirectory = L"")
{
	StringDocumentHandler content;
	Generator generator;
	generator.addDocumentHandler(&content, ODF_FLAT_XML);

	bool bRes = libetonyek::EtonyekDocument::parse(ptrInput.get(), &generator);
	if (!bRes)
		return 1;

	const std::string sOutputFileA = U_TO_UTF8(wsOutputFile);
	std::ofstream output(sOutputFileA.c_str());
	output << content.cstr();

	if (output.bad())
		return -1;

	return 0;
}

int CIWorkFile::Convert2Odf(const std::wstring& sFile, const std::wstring& sOutputFile) const
{
	std::string sFileA = U_TO_UTF8(sFile);

	std::shared_ptr<librevenge::RVNGInputStream> input;
	libetonyek::EtonyekDocument::Type oDocumentType;

	if (!GetRVNGInputStream(sFileA, input, oDocumentType))
		return -1;

	switch (oDocumentType)
	{
	case libetonyek::EtonyekDocument::TYPE_PAGES:   return Convert<OdtGenerator>(sOutputFile, input);
	case libetonyek::EtonyekDocument::TYPE_NUMBERS: return Convert<OdsGenerator>(sOutputFile, input);
	case libetonyek::EtonyekDocument::TYPE_KEYNOTE: return Convert<OdpGenerator>(sOutputFile, input);
	default:
		break;
	}

	return -1;
}

void CIWorkFile::SetTmpDirectory(const std::wstring& sFolder)
{
	m_internal->m_sTempDirectory = sFolder;
}
