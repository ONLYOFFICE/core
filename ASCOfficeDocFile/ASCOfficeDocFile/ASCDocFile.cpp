#include "stdafx.h"
#include "ASCDocFile.h"

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

ASCOfficeCriticalSection  g_oCriticalSection;

STDMETHODIMP COfficeDocFile::LoadFromFile(BSTR bsDocFile, BSTR bsDocxFilePath, BSTR bsXMLOptions) 
{
#ifdef _DEBUG
	//_CrtDumpMemoryLeaks();
	//_CrtSetBreakAlloc(11001);
#endif 	
	
	HRESULT hr = AVS_ERROR_UNEXPECTED;

	std::wstring strDocFile(bsDocFile);
	std::wstring strDocxPath(bsDocxFilePath);

	std::wstring::size_type lastCh	= strDocFile.find_last_of(_T('\\'));
	std::wstring::size_type lastDot	= strDocFile.find_last_of(_T('.'));

	std::wstring strFileName((strDocFile.begin() + lastCh), (strDocFile.begin() + lastDot));
	strFileName += std::wstring(_T(".docx"));
	std::wstring docxFile = strDocxPath + strFileName;

	ProgressCallback ffCallBack;

	ffCallBack.OnProgress	= OnProgressFunc;
	ffCallBack.OnProgressEx	= OnProgressExFunc;
	ffCallBack.caller		= this;

	DocFileFormat::Converter docToDocx;

	hr						=	docToDocx.LoadAndConvert(bsDocFile, docxFile.c_str(), &ffCallBack);

#ifdef _DEBUG
	//_CrtDumpMemoryLeaks();
#endif

	return hr;
}

STDMETHODIMP COfficeDocFile::SaveToFile (BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions)
{
	g_oCriticalSection.Enter();

	HRESULT hr = AVS_ERROR_UNEXPECTED;

	DOCXTODOC::CFileTransformer oTransformer;

#ifdef _DEBUG
	//_CrtDumpMemoryLeaks();
#endif 

	ProgressCallback ffCallBack;

	ffCallBack.OnProgress	=	OnProgressFunc;
	ffCallBack.OnProgressEx	=	OnProgressExFunc;
	ffCallBack.caller		=	this;

	hr = oTransformer.Convert(_bstr_t(sSrcPath), _bstr_t( sDstFileName ), &ffCallBack);

#ifdef _DEBUG
	//_CrtDumpMemoryLeaks();
#endif 

	g_oCriticalSection.Leave();

	return hr;
}

void COfficeDocFile::OnProgressFunc (LPVOID lpParam, long nID, long nPercent)
{
	g_oCriticalSection.Enter();

	COfficeDocFile* pDocFile = reinterpret_cast<COfficeDocFile*>(lpParam);
	if (pDocFile != NULL)
	{
		pDocFile->OnProgress(nID, nPercent);
	}

	g_oCriticalSection.Leave();
}

void COfficeDocFile::OnProgressExFunc (LPVOID lpParam, long nID, long nPercent, short* pStop)
{
	g_oCriticalSection.Enter();

	COfficeDocFile* pDocFile = reinterpret_cast<COfficeDocFile*>(lpParam);
	if (NULL != pDocFile)
	{
		pDocFile->OnProgressEx(nID, nPercent, pStop);
	}

	g_oCriticalSection.Leave();
}