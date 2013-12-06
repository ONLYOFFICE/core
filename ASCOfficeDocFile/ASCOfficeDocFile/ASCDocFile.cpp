#include "stdafx.h"
#include "ASCDocFile.h"

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

ASCOfficeCriticalSection  g_oCriticalSection;

STDMETHODIMP COfficeDocFile::LoadFromFile (BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions)
{
	HRESULT hr					=	AVS_ERROR_UNEXPECTED;

	wstring fullFileNameStr (sSrcFileName);

	wstring::size_type lastCh	=	fullFileNameStr.find_last_of( _T( '\\' ) );
	wstring::size_type lastDot	=	fullFileNameStr.find_last_of( _T( '.' ) );

	wstring FileNameStr( ( fullFileNameStr.begin() + lastCh ), ( fullFileNameStr.begin() + lastDot ) );
	FileNameStr += wstring( _T( ".docx" ) );
	wstring outPutFile = wstring( sDstPath ) + FileNameStr;

#ifdef _DEBUG
	//_CrtDumpMemoryLeaks();
	//_CrtSetBreakAlloc(11001);
#endif 

	ProgressCallback			ffCallBack;

	ffCallBack.OnProgress		=	OnProgressFunc;
	ffCallBack.OnProgressEx		=	OnProgressExFunc;
	ffCallBack.caller			=	this;

	DocFileFormat::Converter docToDocx;

	hr							=	docToDocx.LoadAndConvert(sSrcFileName, outPutFile.c_str(), &ffCallBack);

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