// OfficeXlsFile.cpp : Implementation of COfficeXlsFile

#include "stdafx.h"

#include <string>
#include <iostream>

#include <boost/uuid/uuid.hpp>
#pragma warning(push)
#pragma warning(disable : 4244)
#include <boost/uuid/random_generator.hpp>
#pragma warning(pop)

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>
#include "../Common/boost_filesystem_version.h"

#include "../source/XlsXlsxConverter/ConvertXls2Xlsx.h"

#include "../Common/XmlUtils.h"
#include "../Common/ASCATLError.h"


#include "OfficeXlsFile.h"

// ВНИМАНИЕ:    значение 1 используется для тестирования, на выходе получаем заархивированный файл xlsx или docx
//              значение 0 используется для релиза, так как на выходе по спецификации нам требуется распакованный package
#ifndef STANDALONE_USE
	#define STANDALONE_USE 0// что получаем на выходе: файл (1) или папку (0)
#endif

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
///------------------------------------------------------------------------------------

// COfficeXlsFile
COfficeXlsFile::COfficeXlsFile()
{
#if defined(STANDALONE_USE) && (STANDALONE_USE == 1)
    office_utils_.CoCreateInstance(__uuidof(ASCOfficeUtils::COfficeUtils));    
#endif
}

HRESULT COfficeXlsFile::SaveToFile(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions)
{
    return E_NOTIMPL;
}

HRESULT COfficeXlsFile::LoadFromFile(BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions)
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

   

#if defined(STANDALONE_USE) && (STANDALONE_USE == 1)
    boost::filesystem::wpath dstTempPath = MakeTempDirectoryName(BOOST_STRING_PATH(outputDir));
#else
    boost::filesystem::wpath dstTempPath = outputDir.string();
#endif

    try
    {
#if defined(STANDALONE_USE) && (STANDALONE_USE == 1)
        // создаем временную директорию для результирующих файлов
        boost::filesystem::create_directory(dstTempPath);
#endif
        hr = LoadFromFileImpl(bstr2wstring(sSrcFileName), BOOST_STRING_PATH(dstTempPath), bstr2wstring(sDstPath));
        
    }
    catch(...)
    {
        hr = E_FAIL;
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



HRESULT COfficeXlsFile::LoadFromFileImpl(const std::wstring & srcFileName,
                                         const std::wstring & dstTempPath,
                                         const std::wstring & dstPath)
{
    HRESULT hr = AVS_ERROR_UNEXPECTED;    
        
	ProgressCallback ffCallBack;

	ffCallBack.OnProgress	=	OnProgressFunc;
	ffCallBack.OnProgressEx	=	OnProgressExFunc;
	ffCallBack.caller		=	this;

	hr = ConvertXls2Xlsx(srcFileName, dstTempPath, &ffCallBack);

	if (hr != S_OK)  return hr;
   
#if defined(STANDALONE_USE) && (STANDALONE_USE == 1)
    if FAILED(hr = office_utils_->CompressFileOrDirectory(ATL::CComBSTR(dstTempPath.c_str()), ATL::CComBSTR(dstPath.c_str()), (-1)))
        return hr;
#endif

    return S_OK;
}

bool COfficeXlsFile::initialized()
{
#if defined(STANDALONE_USE) && (STANDALONE_USE == 1)
   return (!!office_utils_);
#endif
   return true;
}

void COfficeXlsFile::OnProgressFunc (LPVOID lpParam, long nID, long nPercent)
{
	//g_oCriticalSection.Enter();

	COfficeXlsFile* pXlsFile = reinterpret_cast<COfficeXlsFile*>(lpParam);
	if (pXlsFile != NULL)
	{
		pXlsFile->OnProgress(nID, nPercent);
	}

	//g_oCriticalSection.Leave();
}

void COfficeXlsFile::OnProgressExFunc (LPVOID lpParam, long nID, long nPercent, short* pStop)
{
	//g_oCriticalSection.Enter();

	COfficeXlsFile* pXlsFile = reinterpret_cast<COfficeXlsFile*>(lpParam);
	if (pXlsFile != NULL)
	{
		pXlsFile->OnProgressEx(nID, nPercent, pStop);
	}

	//g_oCriticalSection.Leave();
}
