// OfficeOdfFileW.cpp : Implementation of COfficeOdfFileW

#include "stdafx.h"
#include "OfficeOdfFileW.h"

#include <string>

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/algorithm/string.hpp> 

#include "source\Oox2OdfConverter\Converter.h"

#import "../Redist/ASCOfficeOdfFile.dll" rename_namespace("ASCOfficeOdfFile"), raw_interfaces_only

#ifndef STANDALONE_USE
	#define STANDALONE_USE 0// что на входе: файл (1) или папка (0)
#endif 


// им€ директории - uuid
boost::filesystem::wpath MakeTempDirectoryName(const std::wstring& Dst)
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

STDMETHODIMP COfficeOdfFileW::LoadFromFile(BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions)
{
    ATL::CComPtr<IAVSOfficeFileTemplate> odfFile;
   
	HRESULT hr = odfFile.CoCreateInstance(__uuidof(ASCOfficeOdfFile::COfficeOdfFile));

	if (hr == S_OK && odfFile)
	{
		
		hr = odfFile->LoadFromFile(sSrcFileName, sDstPath, sXMLOptions);
		
	}
    return hr;
}

bool COfficeOdfFileW::initialized()
{
    return (!!office_utils_);
}


STDMETHODIMP COfficeOdfFileW::SaveToFile(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions)
{
    HRESULT hr;
    if (!initialized())
        return E_FAIL;

    if (!sSrcPath)
    {
        _ASSERTE(!!sSrcPath);
        return E_FAIL;
    }

#if defined(STANDALONE_USE) && (STANDALONE_USE == 1)
    boost::filesystem::wpath inputDir = boost::filesystem::wpath(CString(sSrcPath)).parent_path();
#else
    boost::filesystem::wpath inputDir = boost::filesystem::wpath(CString(sSrcPath));
#endif

    boost::filesystem::wpath outputDir = boost::filesystem::wpath(CString(sDstFileName)).parent_path();

    // создаем еЄ в директории куда запишем результат

    boost::filesystem::wpath dstTempPath = MakeTempDirectoryName(outputDir.string());
    

#if defined(STANDALONE_USE) && (STANDALONE_USE == 1)
    boost::filesystem::wpath srcTempPath = MakeTempDirectoryName(BOOST_STRING_PATH(outputDir));
#else
    boost::filesystem::wpath srcTempPath = inputDir.string();
#endif

    try
    {
        boost::filesystem::create_directory(dstTempPath);
#if defined(STANDALONE_USE) && (STANDALONE_USE == 1)
        
        boost::filesystem::create_directory(srcTempPath); // создаем временную директорию дл€ результирующих файлов
#endif
        hr = SaveToFileImpl(bstr2wstring(sSrcPath),srcTempPath.string(), dstTempPath.string(), bstr2wstring(sDstFileName));
        
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
    // в случае если на выходе файл Ч стираем временную директорию (мы сами ее создали)
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

HRESULT COfficeOdfFileW::SaveToFileImpl(const std::wstring & srcPath,
                                        const std::wstring & srcTempPath,
                                        const std::wstring & dstTempPath,
                                        const std::wstring & dstFileName)
{
    HRESULT hr = E_FAIL;
  

    // распаковываем исходник (если он файл) во временную директорию
#if defined(STANDALONE_USE) && (STANDALONE_USE == 1)
    if FAILED(hr = office_utils_->ExtractToDirectory(CComBSTR(srcPath.c_str()), CComBSTR(srcTempPath.c_str()), NULL, 0))
        return hr;
#endif

	try
	{
		std::wstring type = DetectTypeDocument(srcTempPath);

		ProgressCallback ffCallBack;

		ffCallBack.OnProgress	=	OnProgressFunc;
		ffCallBack.OnProgressEx	=	OnProgressExFunc;
		ffCallBack.caller		=	this;
		
		Oox2Odf::Converter converter(srcTempPath, type, &ffCallBack);
		

		converter.convert();
		converter.write(dstTempPath);
	} 
	catch(...)
	{
	}
   
    if FAILED(hr = office_utils_->CompressFileOrDirectory(CComBSTR(dstTempPath.c_str()), CComBSTR(dstFileName.c_str()), (-1)))
        return hr;

    return S_OK;
}
namespace fs = boost::filesystem;

std::wstring COfficeOdfFileW::DetectTypeDocument(const std::wstring & Path)
{
	fs::wpath full_path(/* fs::initial_path<fs::wpath>()*/ Path);

	//full_path = fs::system_complete( fs::wpath( Path ) );

	unsigned long file_count = 0;
	unsigned long dir_count = 0;
	unsigned long other_count = 0;
	unsigned long err_count = 0;

	if (!fs::exists( full_path ) )return L"";

	if (!fs::is_directory( full_path ) )return L"";

	fs::wdirectory_iterator end_iter;
	
	for ( fs::wdirectory_iterator dir_itr( full_path );	  dir_itr != end_iter;  ++dir_itr )
	{
		try
		{
			if ( fs::is_directory( dir_itr->status() ) )
			{
			  ++dir_count;
			  std::wstring tmp = dir_itr->path().filename();
			  boost::algorithm::to_lower(tmp);
			 
			  if (tmp == L"word") return  L"text";
			  if (tmp == L"xl") return  L"spreadsheet";
			}
			else if ( fs::is_regular_file( dir_itr->status() ) )
			{
			  ++file_count;
			}
			else
			{
			  ++other_count;
			}
		}
		catch ( const std::exception & ex )
		{
			++err_count;
		}
	}
	return L"";
}

void COfficeOdfFileW::OnProgressFunc (LPVOID lpParam, long nID, long nPercent)
{
	//g_oCriticalSection.Enter();

	COfficeOdfFileW* pOdfFile = reinterpret_cast<COfficeOdfFileW*>(lpParam);
	if (pOdfFile != NULL)
	{
		pOdfFile->OnProgress(nID, nPercent);
	}

	//g_oCriticalSection.Leave();
}

void COfficeOdfFileW::OnProgressExFunc (LPVOID lpParam, long nID, long nPercent, short* pStop)
{
	//g_oCriticalSection.Enter();

	COfficeOdfFileW* pOdfFile = reinterpret_cast<COfficeOdfFileW*>(lpParam);
	if (pOdfFile != NULL)
	{
		pOdfFile->OnProgressEx(nID, nPercent, pStop);
	}

	//g_oCriticalSection.Leave();
}