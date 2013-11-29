#include "stdafx.h"
#include "OdtFile.h"

#include "Docx2Odt/Converter.h"
#include "comutil.h"

#pragma warning(push)
#pragma warning(disable : 4267) 
#pragma warning(disable : 4244)

#include <fcntl.h>
#include <sys/stat.h>
#include <io.h>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/algorithm/string/case_conv.hpp>

#pragma warning(pop)

#include "Resource.h"

#ifdef _DEBUG
	#pragma comment(lib, "../ASCImageStudio3/ASCGraphics/OfficeSvmFile/Debug/OfficeSvmFile.lib")
#else
	#pragma comment(lib, "../ASCImageStudio3/ASCGraphics/OfficeSvmFile/Release/OfficeSvmFile.lib")
#endif


#import "libid:9AEF1F19-91CE-46C0-A380-06A7F56CE4AE" rename_namespace("AVSOfficeOdfFile"), raw_interfaces_only

// ВНИМАНИЕ:    значение 1 используется для тестирования, на выходе получаем заархивированный файл xlsx или docx
//              значение 0 используется для релиза, так как на выходе по спецификации нам требуется распакованный package
#ifndef STANDALONE_USE
	#define STANDALONE_USE 1// что получаем на выходе: файл (1) или папку (0)
#endif

namespace {

// имя директории - uuid
boost::filesystem::wpath MakeTempDirectoryName(const std::wstring & Dst)
{
    boost::uuids::random_generator gen;
    boost::uuids::uuid u = gen();
    boost::filesystem::wpath path = boost::filesystem::wpath(Dst) / boost::lexical_cast<std::wstring>(u);
    return path;
}

std::wstring bstr2wstring(BSTR str)
{
    return str ? std::wstring(&str[0], &str[::SysStringLen(str)]) : L"";
}

boost::filesystem::wpath MakeTempDirectoryName(BSTR Dst)
{
    return MakeTempDirectoryName(bstr2wstring(Dst));
}

}

COdtFile::COdtFile()
{
}


HRESULT COdtFile::FinalConstruct()
{
	m_pOfficeUtils = NULL;
    HRESULT hr = odfFile_.CoCreateInstance(__uuidof(AVSOfficeOdfFile::COfficeOdfFile));
	hr = CoCreateInstance(__uuidof(AVSOfficeUtils::COfficeUtils),
													NULL, 
													CLSCTX_INPROC_SERVER, 
													__uuidof(AVSOfficeUtils::IOfficeUtils), 
													(void **)&(m_pOfficeUtils));
    return hr;
}


void COdtFile::FinalRelease()
{
	if (m_pOfficeUtils != NULL )
	{
		m_pOfficeUtils->Release();
		m_pOfficeUtils = NULL;
	}
}


bool COdtFile::Progress(long ID, long Percent)
{
	SHORT res = 0;
	m_lPercent = Percent;
	OnProgressEx(ID, Percent, &res);
	return (res != 0);
}


STDMETHODIMP COdtFile::LoadFromFile(BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions)
{
    if (!odfFile_)
        return E_FAIL;

    SHORT val;
    OnProgressEx(0, 0, &val);
    HRESULT hr = odfFile_->LoadFromFile(sSrcFileName, sDstPath, sXMLOptions);
    OnProgressEx(0, 1000000, &val);
    return hr;
}
HRESULT COdtFile::convert(const std::wstring & srcPath, const std::wstring & dstPath)
{
	const boost::filesystem::wpath origin = boost::filesystem::wpath(srcPath) / L"Origin";//щаблон odt
	createOriginOdt(origin);

	try
	{
		Progress(0, 0);
		Docx2Odt::Converter converter(origin);
		converter.read(srcPath);
		Progress(0, 150000);
		converter.convert(*this);
		converter.write(dstPath);
		Progress(0, 1000000);
	}
	catch(...)
	{
		return AVS_ERROR_UNEXPECTED;
	}

	boost::filesystem::remove_all(origin);

	return S_OK;
}
HRESULT COdtFile::SaveToFileImpl(const std::wstring & srcPath,
                                         const std::wstring & srcTempPath,
                                         const std::wstring & dstTempPath,
                                         const std::wstring & dstFileName)
{
    HRESULT hr = E_FAIL;
  
//#ifdef BOOST_FILESYSTEM_LEGACY
//    const std::wstring ext = boost::algorithm::to_lower_copy(boost::filesystem::wpath(dstFileName).extension());
//#else
//    const std::wstring ext = boost::algorithm::to_lower_copy(boost::filesystem::wpath(dstFileName).extension().string<std::wstring>());
//#endif

    // распаковываем исходник (если он файл) во временную директорию
#if defined(STANDALONE_USE) && (STANDALONE_USE == 1)
    if FAILED(hr = m_pOfficeUtils->ExtractToDirectory(
        ATL::CComBSTR(srcPath.c_str()),
        ATL::CComBSTR(srcTempPath.c_str()),
        NULL, 0))
        return hr;
#endif
	hr = convert(srcTempPath, dstTempPath);     


    if FAILED(hr)
        return hr;
   
    if FAILED(hr = m_pOfficeUtils->CompressFileOrDirectory(ATL::CComBSTR(dstTempPath.c_str()), ATL::CComBSTR(dstFileName.c_str()), (-1)))
        return hr;

    return S_OK;
}


STDMETHODIMP COdtFile::SaveToFile(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions)
{
    HRESULT hr;

	if (!sSrcPath)
    {
        _ASSERTE(!!sSrcPath);
        return E_FAIL;
    }

#if defined(STANDALONE_USE) && (STANDALONE_USE == 1)
    boost::filesystem::wpath inputDir = boost::filesystem::wpath(bstr2wstring(sSrcPath)).parent_path();
#else
    boost::filesystem::wpath inputDir = boost::filesystem::wpath(bstr2wstring(sSrcPath));
#endif

    boost::filesystem::wpath outputDir = boost::filesystem::wpath(bstr2wstring(sDstFileName)).parent_path();

    // создаем её в директории куда запишем результат

    boost::filesystem::wpath dstTempPath = MakeTempDirectoryName(BOOST_STRING_PATH(outputDir));
    

#if defined(STANDALONE_USE) && (STANDALONE_USE == 1)
    boost::filesystem::wpath srcTempPath = MakeTempDirectoryName(BOOST_STRING_PATH(outputDir));
#else
    boost::filesystem::wpath srcTempPath = inputDir.string();
#endif

    try
    {
        boost::filesystem::create_directory(dstTempPath);
#if defined(STANDALONE_USE) && (STANDALONE_USE == 1)
        
        boost::filesystem::create_directory(srcTempPath); // создаем временную директорию для результирующих файлов
#endif
        hr = SaveToFileImpl(bstr2wstring(sSrcPath), BOOST_STRING_PATH(srcTempPath), BOOST_STRING_PATH(dstTempPath), bstr2wstring(sDstFileName));
        
    }
    catch(...)
    {
        hr = E_FAIL;
    }

    // стираем временную директорию с распакованным исходником
    try
    {
        boost::filesystem::remove_all(dstTempPath);
    }
    catch(...)
    {
    }

#if defined(STANDALONE_USE) && (STANDALONE_USE == 1)
    // в случае если на выходе файл — стираем временную директорию (мы сами ее создали)
    try 
    {
        boost::filesystem::remove_all(srcTempPath);
    }
    catch(...)
    {
    }
#endif
    
    return hr;
}


void COdtFile::createOriginDocx(const boost::filesystem::wpath& path) const
{
	const boost::filesystem::wpath docxFile = path / L"origin.docx";
	boost::filesystem::create_directories(path);

	LoadFromResource(MAKEINTRESOURCE(IDR_DOCUMENT1), L"Document", _bstr_t(docxFile.string().c_str()));
	m_pOfficeUtils->ExtractToDirectory(_bstr_t(docxFile.string().c_str()), _bstr_t(path.string().c_str()), NULL, 0);
	boost::filesystem::remove(docxFile);
}


void COdtFile::createOriginOdt(const boost::filesystem::wpath& path) const
{
	const boost::filesystem::wpath odtFile = path / L"origin.odt";
	boost::filesystem::create_directories(path);

	LoadFromResource(MAKEINTRESOURCE(IDR_DOCUMENT2), L"Document", _bstr_t(odtFile.string().c_str()));
	m_pOfficeUtils->ExtractToDirectory(_bstr_t(odtFile.string().c_str()), _bstr_t(path.string().c_str()), NULL, 0);

	boost::filesystem::remove(odtFile);
}


const unsigned long COdtFile::LoadFromResource(LPCWSTR lpResName, LPCWSTR lpResType, LPCWSTR fileName) const
{
	HMODULE hMod = GetModuleHandle(_T("AVSOfficeOdtFile.dll"));
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