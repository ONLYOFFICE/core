#include "IWork.h"
#include "../DesktopEditor/common/File.h"
#include "../DesktopEditor/common/Directory.h"

#include <libetonyek/libetonyek.h>
#include <libodfgen/OdtGenerator.hxx>
#include <libodfgen/OdsGenerator.hxx>
#include <libodfgen/OdpGenerator.hxx>
#include <libodfgen/test/StringDocumentHandler.hxx>

#include <memory>
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

#if !defined(_WIN32) && !defined(_WIN64)
	#define DATA_TYPE_INPUTFILE std::string
#else
	#define DATA_TYPE_INPUTFILE std::wstring
#endif

bool GetRVNGInputStream(const DATA_TYPE_INPUTFILE& sFile, std::shared_ptr<librevenge::RVNGInputStream>& oRVNGInputStream, libetonyek::EtonyekDocument::Type& oDocumentType)
{
	oRVNGInputStream.reset(new librevenge::RVNGFileStream(sFile.c_str()));

	oDocumentType = libetonyek::EtonyekDocument::TYPE_UNKNOWN;
	const libetonyek::EtonyekDocument::Confidence confidence = libetonyek::EtonyekDocument::isSupported(oRVNGInputStream.get(), &oDocumentType);

	return libetonyek::EtonyekDocument::CONFIDENCE_NONE != confidence;
}

IWorkFileType CIWorkFile::GetType(const std::wstring& sFile) const
{
	//TODO:: так как на данный момент мы работает только напрямую с файлом, то работа с директорией нам пока не нужна
	if (NSDirectory::PathIsDirectory(sFile))
		return IWorkFileType::None;

	std::shared_ptr<librevenge::RVNGInputStream> input;
	libetonyek::EtonyekDocument::Type oDocumentType;

	#if !defined(_WIN32) && !defined(_WIN64)
	std::string sFileA = U_TO_UTF8(sFile);
	if (!GetRVNGInputStream(sFileA, input, oDocumentType))
		return IWorkFileType::None;
	#else
	if (!GetRVNGInputStream(sFile, input, oDocumentType))
		return IWorkFileType::None;
	#endif

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
	//TODO:: так как на данный момент мы работает только напрямую с файлом, то работа с директорией нам пока не нужна
	if (NSDirectory::PathIsDirectory(sFile))
		return -1;

	std::shared_ptr<librevenge::RVNGInputStream> input;
	libetonyek::EtonyekDocument::Type oDocumentType;

	#if !defined(_WIN32) && !defined(_WIN64)
		std::string sFileA = U_TO_UTF8(sFile);
		if (!GetRVNGInputStream(sFileA, input, oDocumentType))
			return -1;
	#else
		if (!GetRVNGInputStream(sFile, input, oDocumentType))
			return -1;
	#endif

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
