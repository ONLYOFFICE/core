/// \file   OfficeOdfFile.cpp

#include "stdafx.h"
#include "OfficeOdfFile.h"

#include <string>
#include <boost/uuid/uuid.hpp>
#include <cpdoccore/common/boost_filesystem_version.h>

#pragma warning(push)
#pragma warning(disable : 4244)
#include <boost/uuid/random_generator.hpp>
#pragma warning(pop)

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <lexical_cast.h>
#include <boost/filesystem.hpp>
#include <iostream>

#include "ConvertOO2OOX.h"

#include "..\..\Common\XmlUtils.h"

#include "..\..\Common\ASCATLError.h"

#pragma comment(lib, "cpformulasconvert-static.lib")
#pragma comment(lib, "cpxml-static.lib")
#pragma comment(lib, "cpodf-static.lib")
#pragma comment(lib, "cpcommon-static.lib")

#ifdef _DEBUG
	#pragma comment(lib, "../../ASCImageStudio3/ASCGraphics/OfficeSvmFile/Debug/OfficeSvmFile.lib")
#else
	#pragma comment(lib, "../../ASCImageStudio3/ASCGraphics/OfficeSvmFile/Release/OfficeSvmFile.lib")
#endif

// ВНИМАНИЕ:    значение 1 используется для тестирования, на выходе получаем заархивированный файл xlsx или docx
//              значение 0 используется для релиза, так как на выходе по спецификации нам требуется распакованный package
#ifndef STANDALONE_USE
	#define STANDALONE_USE 0// что получаем на выходе: файл (1) или папку (0)
#endif


COfficeOdfFile::COfficeOdfFile()
{
	bOnlyPresentation = false;
    office_utils_.CoCreateInstance(__uuidof(ASCOfficeUtils::COfficeUtils));    
}

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

bool COfficeOdfFile::loadOptionFromXML(CString parametr,BSTR sXMLOptions)
{
	bool result = false;
	XmlUtils::CXmlReader oXmlReader;
	if( TRUE == oXmlReader.OpenFromXmlString( sXMLOptions ) )
	{
		if( TRUE == oXmlReader.ReadRootNode( _T("Options") ) )
		{
			if( TRUE == oXmlReader.ReadNode( parametr ) )
			{
				result = true;
			}
		}		
	}

	return result;

}


HRESULT COfficeOdfFile::LoadFromFile(BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions)
{
    HRESULT hr;
    if (!initialized())
        return E_FAIL;

    if (!sDstPath)
    {
        _ASSERTE(!!sDstPath);
        return E_FAIL;
    }

#if defined(STANDALONE_USE) && (STANDALONE_USE == 1)
    boost::filesystem::wpath outputDir = boost::filesystem::wpath(bstr2wstring(sDstPath)).parent_path();
#else
    boost::filesystem::wpath outputDir = boost::filesystem::wpath(bstr2wstring(sDstPath));
#endif

    // временная директория, в которую распакуем исходный файл,
    // создаем её в директории куда запишем результат

    //boost::filesystem::wpath srcTempPath = MakeTempDirectoryName(outputDir.string<std::wstring>());
    boost::filesystem::wpath srcTempPath = MakeTempDirectoryName(BOOST_STRING_PATH(outputDir));
    

#if defined(STANDALONE_USE) && (STANDALONE_USE == 1)
    //boost::filesystem::wpath dstTempPath = MakeTempDirectoryName(outputDir.string<std::wstring>());
    boost::filesystem::wpath dstTempPath = MakeTempDirectoryName(BOOST_STRING_PATH(outputDir));
#else
    boost::filesystem::wpath dstTempPath = outputDir.string();
#endif

	bOnlyPresentation = loadOptionFromXML(L"onlyPresentation", sXMLOptions);

    try
    {
        boost::filesystem::create_directory(srcTempPath);
#if defined(STANDALONE_USE) && (STANDALONE_USE == 1)
        // создаем временную директорию для результирующих файлов
        boost::filesystem::create_directory(dstTempPath);
#endif
        //hr = LoadFromFileImpl(bstr2wstring(sSrcFileName), srcTempPath.string<std::wstring>(), dstTempPath.string<std::wstring>(), bstr2wstring(sDstPath));
        hr = LoadFromFileImpl(bstr2wstring(sSrcFileName), BOOST_STRING_PATH(srcTempPath), BOOST_STRING_PATH(dstTempPath), bstr2wstring(sDstPath));
        
    }
    catch(...)
    {
        hr = E_FAIL;
    }

    // стираем временную директорию с распакованным исходником
    try
    {
        boost::filesystem::remove_all(srcTempPath);
    }
    catch(...) 
    {
    }

#if defined(STANDALONE_USE) && (STANDALONE_USE == 1)
    // в случае если на выходе файл — стираем временную директорию (мы сами ее создали)
    try 
    {
        boost::filesystem::remove_all(dstTempPath);
    }
    catch(...)
    {
    }
#endif
     
    return hr;
}

HRESULT COfficeOdfFile::LoadFromFileImpl(const std::wstring & srcFileName,
                                         const std::wstring & srcTempPath,
                                         const std::wstring & dstTempPath,
                                         const std::wstring & dstPath)
{
    HRESULT hr = AVS_ERROR_UNEXPECTED;    
        
#ifdef BOOST_FILESYSTEM_LEGACY
    const std::wstring ext = boost::algorithm::to_lower_copy(boost::filesystem::wpath(srcFileName).extension());
#else
    const std::wstring ext = boost::algorithm::to_lower_copy(boost::filesystem::wpath(srcFileName).extension().string<std::wstring>());
#endif

    // распаковываем исходник во временную директорию
    if FAILED(hr = office_utils_->ExtractToDirectory(
        ATL::CComBSTR(srcFileName.c_str()),
        ATL::CComBSTR(srcTempPath.c_str()),
        NULL, 0))
        return hr;


	hr = ConvertOO2OOX(ext,srcTempPath, dstTempPath,bOnlyPresentation);

	if (hr != S_OK)  hr;
   
#if defined(STANDALONE_USE) && (STANDALONE_USE == 1)
    if FAILED(hr = office_utils_->CompressFileOrDirectory(ATL::CComBSTR(dstTempPath.c_str()), ATL::CComBSTR(dstPath.c_str()), (-1)))
        return hr;
#endif

    return S_OK;
}


HRESULT COfficeOdfFile::SaveToFile(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions)
{
    return E_NOTIMPL;
}

bool COfficeOdfFile::initialized()
{
    return (!!office_utils_);
}

// COfficeOdfFile3
