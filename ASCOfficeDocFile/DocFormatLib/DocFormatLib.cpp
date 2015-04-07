#include "DocFormatLib.h"

#include "../DocDocxConverter/Converter.h"
//#include "../DocxDocConverter/FileConverter.h"

#include "../../Common/DocxFormat/Source/SystemUtility/FileSystem/Directory.h"
#include "../../ASCOfficeUtils/ASCOfficeUtilsLib/OfficeUtils.h"

HRESULT COfficeDocFile::LoadFromFile(CString bsDocFile, CString bsDocx, CString bsXMLOptions, ProgressCallback *ffCallBack ) 
{
#if defined(_DEBUG) && defined (_WIN32)
	//_CrtDumpMemoryLeaks();
	//_CrtSetBreakAlloc(11001);
#endif 	
	
	HRESULT hr = S_FALSE;

	DocFileFormat::Converter docToDocx;

	if (m_strTempDirectory.GetLength() < 1)
	{
		m_strTempDirectory = FileSystem::Directory::GetTempPath();
	}

	CString sDocxDirectory;
	
#if !defined(_WIN32) && !defined(_WIN64)
    sDocxDirectory = FileSystem::Directory::CreateDirectoryWithUniqueName(m_strTempDirectory);
#else
	sDocxDirectory = bsDocx;
	FileSystem::Directory::CreateDirectory(sDocxDirectory);
#endif

	hr= docToDocx.LoadAndConvert(bsDocFile,sDocxDirectory, ffCallBack);

#ifndef _WIN32
	if (hr == S_OK)
	{
		//zipping
		COfficeUtils oCOfficeUtils(NULL);
        hr = oCOfficeUtils.CompressFileOrDirectory (string2std_string(sDocxDirectory), string2std_string(bsDocx), -1);

		FileSystem::Directory::DeleteDirectory(sDocxDirectory);
	}
#endif

#if defined(_DEBUG) && (defined(_WIN32) || defined(_WIN64))
	//_CrtDumpMemoryLeaks();
#endif

	return hr;
}

HRESULT COfficeDocFile::SaveToFile (CString sDstFileName, CString sSrcFileName, CString sXMLOptions, ProgressCallback *ffCallBack )
{
	HRESULT hr = S_OK;

    //DOCXTODOC::CFileTransformer oTransformer;

#if defined(_DEBUG) && (defined(_WIN32) || defined(_WIN64))
	//_CrtDumpMemoryLeaks();
#endif 

    //hr = oTransformer.Convert(sSrcPath, sDstFileName, ffCallBack);

#if defined(_DEBUG) && (defined(_WIN32) || defined(_WIN64))
	//_CrtDumpMemoryLeaks();
#endif 

	return hr;
}
