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

IWorkFileType CIWorkFile::GetType(const std::wstring& sFile)
{
	std::string sFileA = U_TO_UTF8(sFile);
	std::shared_ptr<librevenge::RVNGInputStream> input;
	if (librevenge::RVNGDirectoryStream::isDirectory(sFileA.c_str()))
		input.reset(new librevenge::RVNGDirectoryStream(sFileA.c_str()));
	else
		input.reset(new librevenge::RVNGFileStream(sFileA.c_str()));

	libetonyek::EtonyekDocument::Type type = libetonyek::EtonyekDocument::TYPE_UNKNOWN;
	const libetonyek::EtonyekDocument::Confidence confidence = libetonyek::EtonyekDocument::isSupported(input.get(), &type);

	if (libetonyek::EtonyekDocument::CONFIDENCE_NONE == confidence)
		return IWorkFileType::None;

	switch (type)
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

int CIWorkFile::Convert2Odf(const std::wstring& sFile, const std::wstring& sOutputFile)
{
	std::string sFileA = U_TO_UTF8(sFile);
	std::shared_ptr<librevenge::RVNGInputStream> input;
	if (librevenge::RVNGDirectoryStream::isDirectory(sFileA.c_str()))
		input.reset(new librevenge::RVNGDirectoryStream(sFileA.c_str()));
	else
		input.reset(new librevenge::RVNGFileStream(sFileA.c_str()));

	libetonyek::EtonyekDocument::Type type = libetonyek::EtonyekDocument::TYPE_UNKNOWN;
	const libetonyek::EtonyekDocument::Confidence confidence = libetonyek::EtonyekDocument::isSupported(input.get(), &type);

	if (libetonyek::EtonyekDocument::CONFIDENCE_NONE == confidence)
		return -1;

	const std::string sOutputFileA = U_TO_UTF8(sOutputFile);

	switch (type)
	{
	case libetonyek::EtonyekDocument::TYPE_PAGES:
	{
		StringDocumentHandler content;
		OdtGenerator generator;
		generator.addDocumentHandler(&content, ODF_FLAT_XML);

		bool bRes = libetonyek::EtonyekDocument::parse(input.get(), &generator);
		if (!bRes)
			return 1;

		std::wofstream output(sOutputFileA.c_str());
		output << content.cstr();
		return 0;
	}
	case libetonyek::EtonyekDocument::TYPE_NUMBERS:
	{
		StringDocumentHandler content;
		OdsGenerator generator;
		generator.addDocumentHandler(&content, ODF_FLAT_XML);

		bool bRes = libetonyek::EtonyekDocument::parse(input.get(), &generator);
		if (!bRes)
			return 1;

		std::wofstream output(sOutputFileA.c_str());
		output << content.cstr();
		return 0;
	}
	case libetonyek::EtonyekDocument::TYPE_KEYNOTE:
	{
		StringDocumentHandler content;
		OdpGenerator generator;
		generator.addDocumentHandler(&content, ODF_FLAT_XML);

		bool bRes = libetonyek::EtonyekDocument::parse(input.get(), &generator);
		if (!bRes)
			return 1;

		std::wofstream output(sOutputFileA.c_str());
		output << content.cstr();
		return 0;
	}
	default:
		break;
	}

	return -1;
}

void CIWorkFile::SetTmpDirectory(const std::wstring& sFolder)
{
	m_internal->m_sTempDirectory = sFolder;
}
