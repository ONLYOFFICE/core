#include "stdafx.h"
#include "ASCDocFile.h"

//#include "ASCOfficeCriticalSection.h"

#include "../DocFormatLib/DocFormatLib.h"
#include "../Common/Callback.h"

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif


STDMETHODIMP CASCOfficeDocFile::LoadFromFile(BSTR bsDocFile, BSTR bsDocxFilePath, BSTR bsXMLOptions) 
{

#ifdef _DEBUG
	//_CrtDumpMemoryLeaks();
	//_CrtSetBreakAlloc(11001);
#endif 	
	
	COfficeDocFile file;

	ProgressCallback ffCallBack;

	ffCallBack.OnProgress	= OnProgressFunc;
	ffCallBack.OnProgressEx	= OnProgressExFunc;
	ffCallBack.caller		= this;

	HRESULT hr	=	file.LoadFromFile(bsDocFile, bsDocxFilePath,&ffCallBack);

#ifdef _DEBUG
	//_CrtDumpMemoryLeaks();
#endif

	return hr;
}

STDMETHODIMP CASCOfficeDocFile::SaveToFile (BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions)
{

#ifdef _DEBUG
	//_CrtDumpMemoryLeaks();
#endif 

	COfficeDocFile file;

	ProgressCallback ffCallBack;

	ffCallBack.OnProgress	=	OnProgressFunc;
	ffCallBack.OnProgressEx	=	OnProgressExFunc;
	ffCallBack.caller		=	this;

	HRESULT hr = file.SaveToFile(sSrcPath,  sDstFileName, &ffCallBack);

#ifdef _DEBUG
	//_CrtDumpMemoryLeaks();
#endif 

	return hr;
}

void CASCOfficeDocFile::OnProgressFunc (LPVOID lpParam, long nID, long nPercent)
{
	CASCOfficeDocFile* pDocFile = reinterpret_cast<CASCOfficeDocFile*>(lpParam);
	if (pDocFile != NULL)
	{
		pDocFile->OnProgress(nID, nPercent);
	}

}

void CASCOfficeDocFile::OnProgressExFunc (LPVOID lpParam, long nID, long nPercent, short* pStop)
{

	CASCOfficeDocFile* pDocFile = reinterpret_cast<CASCOfficeDocFile*>(lpParam);
	if (NULL != pDocFile)
	{
		pDocFile->OnProgressEx(nID, nPercent, pStop);
	}

}