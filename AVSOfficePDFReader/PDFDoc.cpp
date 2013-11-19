#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <windows.h>

#include "StringExt.h"
#include "Constants.h"
#include "GlobalParams.h"
#include "Page.h"
#include "Catalog.h"
#include "Stream.h"
#include "XRef.h"
#include "Link.h"
#include "OutputDevice.h"
#include "ErrorConstants.h"
#include "Lexer.h"
#include "Parser.h"
#include "SecurityHandler.h"
#include "Outline.h"
#include "PDFDoc.h"

//-------------------------------------------------------------------------------------------------------------------------------

#define HeaderSearchSize 1024 // Максимальное количество байт для считывания с начала файла, в которых мы ищем запись '%PDF'

//-------------------------------------------------------------------------------------------------------------------------------
// PDFDoc
//-------------------------------------------------------------------------------------------------------------------------------

PDFDoc::PDFDoc(GlobalParams *pGlobalParams, CString wsFileName, StringExt *seOwnerPassword, StringExt *seUserPassword) 
{
	m_pGlobalParams = pGlobalParams;

	m_bValid     = FALSE;
	m_nErrorCode = ErrorNone;

	m_pFile    = NULL;

	m_hFile       = NULL;
	m_hMapFile    = NULL;
	m_pFileBuffer = NULL;

	m_pStream  = NULL;
	m_pXref    = NULL;
	m_pCatalog = NULL;
	m_pOutline = NULL;

	m_wsFileName = wsFileName;

	// Пытаемся открыть файл

	// открываем файл
	HANDLE hFile = CreateFile( wsFileName.GetBuffer(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
		return; // Невозможно открыть файл

	// мапим этот файл в память - так быстрее читаются данные из файла
	DWORD  nFileSize = GetFileSize(hFile, NULL);
	HANDLE hMapFile  = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, nFileSize, NULL);
	if (NULL == hMapFile)
	{
		CloseHandle( hFile );
		return; // Невозможно создать отображение файла
	}

	// создаем view of file 
	VOID* pBaseAddress	= MapViewOfFile( hMapFile, FILE_MAP_READ, 0, 0, 0 );
	if ( !pBaseAddress )
	{
		CloseHandle( hMapFile );
		CloseHandle( hFile );
		return;
	}

	m_pFileBuffer = (char*)pBaseAddress;//(char *)MemUtilsMalloc( sizeof(char) * nFileSize );
	m_hMapFile    = hMapFile;
	m_hFile       = hFile;


	// Создаем поток
	Object oTemp;
	oTemp.InitNull();
	m_pStream = new MemoryStream( m_pFileBuffer, 0, nFileSize, &oTemp );//FileStream( m_pFileBuffer, nFileSize, /*m_pFile,*/ 0, FALSE, 0, &oTemp );

	m_bValid = Setup( seOwnerPassword, seUserPassword );
}

PDFDoc::PDFDoc(GlobalParams *pGlobalParams, wchar_t *sFileName, int nFileNameLen, StringExt *seOwnerPassword, StringExt *seUserPassword) 
{
	m_pGlobalParams = pGlobalParams;

	wchar_t sTempFileName[_MAX_PATH + 1];
	int nIndex = 0;

	m_bValid = FALSE;
	m_nErrorCode = ErrorNone;

	m_hFile       = NULL;
	m_hMapFile    = NULL;
	m_pFileBuffer = NULL;

	m_pFile    = NULL;
	m_pStream  = NULL;
	m_pXref    = NULL;
	m_pCatalog = NULL;
	m_pOutline = NULL;

	m_wsFileName = CString( sFileName );

	for ( nIndex = 0; nIndex < nFileNameLen && nIndex < _MAX_PATH; ++nIndex ) 
	{
		sTempFileName[nIndex] = sFileName[nIndex];
	}
	sTempFileName[nIndex] = 0;

	// Пытаемся открыть файл
	OSVERSIONINFO oVersion;
	oVersion.dwOSVersionInfoSize = sizeof( oVersion );
	GetVersionEx( &oVersion );

	if ( oVersion.dwPlatformId == VER_PLATFORM_WIN32_NT ) 
	{
		m_pFile = _wfopen( sTempFileName, _T("rb") );
	} 
	else 
	{
		m_pFile = _wfopen( m_wsFileName.GetBuffer(), _T("rb") );
	}
	if ( !m_pFile ) 
	{
		// TO DO: Error "Couldn't open file"
		m_nErrorCode = ErrorOpenFile;
		return;
	}

	// Создаем поток
	Object oTemp;
	oTemp.InitNull();
	m_pStream = new FileStream( m_pFile, 0, FALSE, 0, &oTemp );

	m_bValid = Setup( seOwnerPassword, seUserPassword );
}

PDFDoc::PDFDoc(GlobalParams *pGlobalParams, BaseStream *pStream, StringExt *seOwnerPassword, StringExt *seUserPassword ) 
{
	m_pGlobalParams = pGlobalParams;

	m_bValid = FALSE;
	m_nErrorCode = ErrorNone;

	if ( pStream->GetFileName() ) 
	{
		m_wsFileName = CString( pStream->GetFileName()->GetBuffer() );
	} 
	else 
	{
		m_wsFileName = _T("");
	}

	m_hFile       = NULL;
	m_hMapFile    = NULL;
	m_pFileBuffer = NULL;

	m_pFile    = NULL;
	m_pXref    = NULL;
	m_pCatalog = NULL;
	m_pOutline = NULL;

	m_pStream = pStream;

	m_bValid = Setup( seOwnerPassword, seUserPassword );
}

BOOL PDFDoc::Setup(StringExt *seOwnerPassword, StringExt *seUserPassword) 
{
	m_pStream->Reset();

	// Проверяем заголовок PDF файла
	CheckHeader();

	// Считываем таблицу Xref
	m_pXref = new XRef( m_pStream );
	if ( !m_pXref->CheckValidate() ) 
	{
		// TO DO: Error "Couldn't read xref table"
		m_nErrorCode = m_pXref->GetErrorCode();
		return FALSE;
	}

	// Проверяем зашифрован ли документ
	if ( !CheckEncryption( seOwnerPassword, seUserPassword ) ) 
	{
		m_nErrorCode = ErrorEncrypted;
		return FALSE;
	}

	// Считываем объект Catalog
	m_pCatalog = new Catalog( m_pGlobalParams, m_pXref );
	if ( !m_pCatalog->CheckValidation() ) 
	{
		// TO DO: Error "Couldn't read page catalog"
		m_nErrorCode = ErrorBadCatalog;
		return FALSE;
	}

	// Считываем объект Outline
	m_pOutline = new Outline( m_pCatalog->GetOutline(), m_pXref );

	return TRUE;
}

PDFDoc::~PDFDoc() 
{
	if ( m_pOutline ) 
	{
		delete m_pOutline;
	}
	if ( m_pCatalog ) 
	{
		delete m_pCatalog;
	}
	if ( m_pXref ) 
	{
		delete m_pXref;
	}
	if ( m_pStream ) 
	{
		delete m_pStream;
	}

	if ( m_pFileBuffer )
	{
		UnmapViewOfFile( (void *)m_pFileBuffer );
	}
	if ( m_hMapFile )
	{
		CloseHandle( m_hMapFile);
	}
	if ( m_hFile )
	{
		CloseHandle( m_hFile );
	}

	if ( m_pFile )
	{
		fclose( m_pFile );
	}
}

void PDFDoc::CheckHeader() 
{
	char hdrBuf[HeaderSearchSize + 1];
	char *p;
	int nIndex = 0;

	m_dPDFVersion = 0;
	
	for ( nIndex = 0; nIndex < HeaderSearchSize; ++nIndex ) 
	{
		hdrBuf[nIndex] = m_pStream->GetChar();
	}
	hdrBuf[HeaderSearchSize] = '\0';

	for ( nIndex = 0; nIndex < HeaderSearchSize - 5; ++nIndex ) 
	{
		if ( !strncmp(&hdrBuf[nIndex], "%PDF-", 5) ) 
		{
			break;
		}
	}
	if ( nIndex >= HeaderSearchSize - 5 ) 
	{
		// TO DO: Error "May not be a PDF file"
		return;
	}

	m_pStream->SetStartPos( nIndex );

	if ( !( p = strtok(&hdrBuf[nIndex + 5], " \t\n\r") ) ) 
	{
		// TO DO: Error "May not be a PDF file"
		return;
	}
	m_dPDFVersion = atof( p );

	if ( !( hdrBuf[nIndex + 5] >= '0' && hdrBuf[nIndex + 5] <= '9' ) || m_dPDFVersion > SupportedPDFVersionNum + 0.0001 ) 
	{
		// TO DO: Error "Unsupported PDF version"
	}
}

BOOL PDFDoc::CheckEncryption(StringExt *ownerPassword, StringExt *userPassword) 
{
	BOOL bEncrypted = FALSE;
	BOOL bResult = TRUE;

	Object oEncypt;
	m_pXref->GetTrailerDict()->DictLookup("Encrypt", &oEncypt);

	if ( ( bEncrypted = oEncypt.IsDict() ) ) 
	{
		SecurityHandler *pSecurityHandler = NULL;
		if ( ( pSecurityHandler = SecurityHandler::Make(this, &oEncypt) ) ) 
		{
			if ( pSecurityHandler->CheckEncryption(ownerPassword, userPassword) ) 
			{
				// Авторизация пройдена
				m_pXref->SetEncryption( pSecurityHandler->GetPermissionFlags(), pSecurityHandler->GetOwnerPasswordValid(), pSecurityHandler->GetFileKey(), pSecurityHandler->GetFileKeyLength(), pSecurityHandler->GetEncodingVersion(), pSecurityHandler->GetEncodingAlgorithm() );
				bResult = TRUE;
			} 
			else 
			{
				// Авторизация не пройдена
				bResult = FALSE;
			}
			delete pSecurityHandler;
		} 
		else 
		{
			bResult = FALSE;
		}
	} 
	else 
	{
		// Данный PDF файл не зашифрован
		bResult = TRUE;
	}
	oEncypt.Free();
	return bResult;
}

void PDFDoc::DisplayPage(OutputDev *pOut, int nPageIndex, double dHorDPI, double dVerDPI, int nRotate, BOOL bUseMediaBox, BOOL bCrop, BOOL bPrinting, BOOL (*pAbortCheckCbk)(void *pData), void *pAbortCheckCbkData) 
{
	if ( m_pGlobalParams && m_pGlobalParams->GetPrintCommands() ) 
	{
		//printf("***** page %d *****\n", nPageIndex);
	}
	m_pCatalog->GetPage( nPageIndex )->Display(pOut, dHorDPI, dVerDPI, nRotate, bUseMediaBox, bCrop, bPrinting, m_pCatalog, pAbortCheckCbk, pAbortCheckCbkData);
}

void PDFDoc::DisplayPages(OutputDev *pOut, int nFirstPageIndex, int nLastPageIndex, double dHorDPI, double dVerDPI, int nRotate, BOOL bUseMediaBox, BOOL bCrop, BOOL bPrinting, BOOL (*pAbortCheckCbk)(void *pData), void *pAbortCheckCbkData) 
{
	for ( int nPageIndex = nFirstPageIndex; nPageIndex <= nLastPageIndex; ++nPageIndex ) 
	{
		DisplayPage( pOut, nPageIndex, dHorDPI, dVerDPI, nRotate, bUseMediaBox, bCrop, bPrinting, pAbortCheckCbk, pAbortCheckCbkData);
	}
}

void PDFDoc::DisplayPageSlice(OutputDev *pOut, int nPageIndex, double dHorDPI, double dVerDPI, int nRotate, BOOL bUseMediaBox, BOOL bCrop, BOOL bPrinting, int nSliceX, int nSliceY, int nSliceW, int nSliceH, BOOL (*pAbortCheckCbk)(void *pData), void *pAbortCheckCbkData) 
{
	m_pCatalog->GetPage(nPageIndex)->DisplaySlice( pOut, dHorDPI, dVerDPI, nRotate, bUseMediaBox, bCrop, nSliceX, nSliceY, nSliceW, nSliceH, bPrinting, m_pCatalog, pAbortCheckCbk, pAbortCheckCbkData);
}

Links *PDFDoc::GetLinks(int nPageIndex) 
{
	return m_pCatalog->GetPage(nPageIndex)->GetLinks(m_pCatalog);
}

void PDFDoc::ProcessLinks(OutputDev *pOut, int nPageIndex) 
{
	m_pCatalog->GetPage(nPageIndex)->ProcessLinks(pOut, m_pCatalog);
}

BOOL PDFDoc::IsLinearized() 
{
	BOOL bLinearized = FALSE;
	Object oTemp;
	oTemp.InitNull();
	Parser *pParser = new Parser( m_pXref, new Lexer( m_pXref,  m_pStream->MakeSubStream( m_pStream->GetStartPos(), FALSE, 0, &oTemp ) ), TRUE);
	if ( !pParser )
	{
		// TO DO: Error "Can't allocate memory"
		oTemp.Free();
		return FALSE;
	}
	oTemp.Free();

	Object oNum, oGen, oObj, oDict;
	pParser->GetObject(&oNum);
	pParser->GetObject(&oGen);
	pParser->GetObject(&oObj);
	pParser->GetObject(&oDict);

	if ( oNum.IsInt() && oGen.IsInt() && oObj.IsCommand("obj") && oDict.IsDict() ) 
	{
		Object oVersion;
		oDict.DictLookup("Linearized", &oVersion);
		if ( oVersion.IsNum() && oVersion.GetNum() > 0 ) 
		{
			bLinearized = TRUE;
		}
		oVersion.Free();
	}
	oDict.Free();
	oObj.Free();
	oGen.Free();
	oNum.Free();

	delete pParser;
	return bLinearized;
}

BOOL PDFDoc::SaveAs(CStringW wsName) 
{
	FILE *pFile = NULL;

	if ( !( pFile = _wfopen( wsName.GetBuffer(), _T("wb") ) ) ) 
	{
		// TO DO: Error "Couldn't open file"
		return FALSE;
	}
	m_pStream->Reset();

	int nChar = 0;
	while ( ( nChar = m_pStream->GetChar() ) != EOF ) 
	{
		fputc( nChar, pFile );
	}
	m_pStream->Close();

	fclose( pFile );

	return TRUE;
}
