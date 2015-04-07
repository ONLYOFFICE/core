#include "stdafx.h"
#include "ASCDocFile.h"

#include "ASCOfficeCriticalSection.h"

#include "../DocFormatLib/DocFormatLib.h"
#include "../Common/Callback.h"

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

ASCOfficeCriticalSection  g_oCriticalSection;

STDMETHODIMP CASCOfficeDocFile::LoadFromFile(BSTR bsDocFile, BSTR bsDocxFilePath, BSTR bsXMLOptions) 
{
	g_oCriticalSection.Enter();

#ifdef _DEBUG
	//_CrtDumpMemoryLeaks();
	//_CrtSetBreakAlloc(11001);
#endif 	
	
	COfficeDocFile file;

	ProgressCallback ffCallBack;

	ffCallBack.OnProgress	= OnProgressFunc;
	ffCallBack.OnProgressEx	= OnProgressExFunc;
	ffCallBack.caller		= this;

	HRESULT hr	=	file.LoadFromFile(bsDocFile, bsDocxFilePath, bsXMLOptions,&ffCallBack);

#ifdef _DEBUG
	//_CrtDumpMemoryLeaks();
#endif

	g_oCriticalSection.Leave();
	return hr;
}

STDMETHODIMP CASCOfficeDocFile::SaveToFile (BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions)
{
	g_oCriticalSection.Enter();

#ifdef _DEBUG
	//_CrtDumpMemoryLeaks();
#endif 

	COfficeDocFile file;

	ProgressCallback ffCallBack;

	ffCallBack.OnProgress	=	OnProgressFunc;
	ffCallBack.OnProgressEx	=	OnProgressExFunc;
	ffCallBack.caller		=	this;

	HRESULT hr = file.SaveToFile(sSrcPath,  sDstFileName, sXMLOptions, &ffCallBack);

#ifdef _DEBUG
	//_CrtDumpMemoryLeaks();
#endif 

	g_oCriticalSection.Leave();

	return hr;
}

void CASCOfficeDocFile::OnProgressFunc (LPVOID lpParam, long nID, long nPercent)
{
	g_oCriticalSection.Enter();

	CASCOfficeDocFile* pDocFile = reinterpret_cast<CASCOfficeDocFile*>(lpParam);
	if (pDocFile != NULL)
	{
		pDocFile->OnProgress(nID, nPercent);
	}

	g_oCriticalSection.Leave();
}

void CASCOfficeDocFile::OnProgressExFunc (LPVOID lpParam, long nID, long nPercent, short* pStop)
{
	g_oCriticalSection.Enter();

	CASCOfficeDocFile* pDocFile = reinterpret_cast<CASCOfficeDocFile*>(lpParam);
	if (NULL != pDocFile)
	{
		pDocFile->OnProgressEx(nID, nPercent, pStop);
	}

	g_oCriticalSection.Leave();
}