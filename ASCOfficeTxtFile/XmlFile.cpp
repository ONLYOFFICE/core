#include "stdafx.h"
#include "XmlFile.h"
//#include "Docx2Txt/Converter.h"
#include "Xml2Docx/Converter.h"
#include "XML.h"
#include "comutil.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <io.h>
#include "Resource.h"


CXmlFile::CXmlFile()
{
}


HRESULT CXmlFile::FinalConstruct()
{
	m_pOfficeUtils = NULL;
	return CoCreateInstance(__uuidof(AVSOfficeUtils::COfficeUtils),
													NULL, 
													CLSCTX_INPROC_SERVER, 
													__uuidof(AVSOfficeUtils::IOfficeUtils), 
													(void **)&(m_pOfficeUtils));
}


void CXmlFile::FinalRelease()
{
	if (m_pOfficeUtils != NULL )
	{
		m_pOfficeUtils->Release();
		m_pOfficeUtils = NULL; 
	}
}


bool CXmlFile::Progress(long ID, long Percent)
{
	SHORT res = 0;
	m_lPercent = Percent;
	OnProgressEx(ID, Percent, &res);
	return (res != 0);
}


STDMETHODIMP CXmlFile::LoadFromFile(BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions)
{
	const boost::filesystem::wpath xmlFile	= sSrcFileName;
	const boost::filesystem::wpath docxPath	= sDstPath;
	const boost::filesystem::wpath origin		= docxPath/L"Origin";

	createOriginDocx(origin);

	try
	{
		Progress(0, 0);
		Xml2Docx::Converter converter(origin);
		bool space = false;
		converter.read(xmlFile, space);
		Progress(0, 100000);
		converter.convert(*this);
		converter.write(docxPath);
		Progress(0, 1000000);
	}
	catch(...)
	{
		return AVS_ERROR_UNEXPECTED;
	}

	boost::filesystem::remove_all(origin);

	return S_OK;
}


STDMETHODIMP CXmlFile::SaveToFile(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions)
{
/*
	const boost::filesystem::wpath txtFile	= sDstFileName;
	const boost::filesystem::wpath docxPath	= sSrcPath;

	try
	{
		Progress(0, 0);
		Docx2Txt::Converter converter;
		converter.read(docxPath);
		Progress(0, 100000);
		converter.convert(*this);

		const std::wstring options = sXMLOptions;

		if (!options.empty())
		{
			XML::XDocument document(options);
			if (document.Root.element("TXTOptions").element("Encoding").exist())
			{
				const int encoding = document.Root.element("TXTOptions").element("Encoding").text();
				if (encoding == EncodingType::Utf8)
					converter.writeUtf8(txtFile);
				else if (encoding == EncodingType::Unicode)
					converter.writeUnicode(txtFile);
				else if (encoding == EncodingType::BigEndian)
					converter.writeBigEndian(txtFile);
				else if (encoding == EncodingType::Ansi)
					converter.writeAnsi(txtFile);
				else
					converter.write(txtFile);
			}
			else
			{
				converter.write(txtFile);
			}
		}
		else
		{
			converter.write(txtFile);
		}
		Progress(0, 1000000);
	}
	catch(...)
	{
		return AVS_ERROR_UNEXPECTED;
	}
*/
  return S_OK;
}


void CXmlFile::createOriginDocx(const boost::filesystem::wpath& path) const
{
	const boost::filesystem::wpath docxFile = path / L"origin.docx";
	boost::filesystem::create_directories(path);

	LoadFromResource(MAKEINTRESOURCE(IDR_DOCUMENT2), L"Document", _bstr_t(docxFile.string().c_str()));
	m_pOfficeUtils->ExtractToDirectory(_bstr_t(docxFile.string().c_str()), _bstr_t(path.string().c_str()), NULL, 0);
	boost::filesystem::remove(docxFile);
}


const unsigned long CXmlFile::LoadFromResource(LPCWSTR lpResName, LPCWSTR lpResType, LPCWSTR fileName) const
{
	HMODULE hMod = GetModuleHandle(L"AVSOfficeTxtFile.dll");
	if (hMod)
	{
		HRSRC hRes = FindResource(hMod, lpResName, lpResType);
		if (hRes)
		{
			HGLOBAL hGlob = LoadResource(hMod, hRes);
			if (hGlob)
			{
				BYTE *lpbArray = (BYTE*)LockResource(hGlob);
				if (lpbArray)
				{
					const DWORD dwFileSize = SizeofResource(hMod, hRes);
					if (dwFileSize != 0)
					{
						int hFile = 0;
						if (!_wsopen_s(&hFile, fileName, (O_BINARY | O_CREAT | _O_RDWR ), _SH_DENYNO, S_IWRITE))
						{
							_write(hFile, lpbArray, dwFileSize);
							_close(hFile);
						}
					}
				}
			}
		}
	}
	return GetLastError();
}