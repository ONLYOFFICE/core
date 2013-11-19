#ifndef _FILE_H
#define _FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/stat.h>
#include <windows.h>
#include <time.h>

#include "StringExt.h"

//------------------------------------------------------------------------
static StringExt *GetHomeDir() 
{
	char *sTemp;
	StringExt *seResult;

	if ( ( sTemp = getenv("HOME" ) ) )
		seResult = new StringExt( sTemp );
	else
		seResult = new StringExt(".");
  return seResult;
}

static StringExt *GetCurrentDir()
{
	char sBuffer[MAX_PATH + 1];

	if ( GetCurrentDirectoryA( sizeof( sBuffer ), sBuffer ) )
		return new StringExt( sBuffer );
	return new StringExt();
}

static StringExt *AppendToPath(StringExt *sePath, char *sFileName)
{
	char sBuffer[256];
	char *sFilePath;

	StringExt *seTemp = new StringExt( sePath );
	seTemp->Append('/');
	seTemp->Append( sFileName );
	
	GetFullPathNameA( seTemp->GetBuffer(), sizeof(sBuffer), sBuffer, &sFilePath );
	delete seTemp;
	
	sePath->Clear();
	sePath->Append( sBuffer );
	return sePath;
}

static CStringW AppendToPath(CStringW wsPath, wchar_t *wsFileName)
{
	wchar_t wsBuffer[_MAX_PATH];
	wchar_t *wsFilePath = NULL;

	CStringW wsTemp = wsPath;
	wsTemp.Append( _T("/") );
	wsTemp.Append( wsFileName );

	GetFullPathNameW( wsTemp.GetBuffer(), _MAX_PATH, wsBuffer, &wsFilePath );

	wsTemp = _T("");
	wsTemp.Append( wsBuffer );
	return wsTemp;
}

static StringExt *GrabPath(char *sFileName) 
{
	char *sTemp;

	if ( ( sTemp = strrchr( sFileName, '/' ) ) )
		return new StringExt( sFileName, sTemp - sFileName);
	if ( ( sTemp = strrchr( sFileName, '\\') ) )
		return new StringExt( sFileName, sTemp - sFileName);
	if ( ( sTemp = strrchr( sFileName, ':' ) ) )
		return new StringExt( sFileName, sTemp + 1 - sFileName);
	return new StringExt();

}

static BOOL IsAbsolutePath(char *sPath) 
{
	return sPath[0] == '/' || sPath[0] == '\\' || sPath[1] == ':';
}

static StringExt *MakePathAbsolute(StringExt *sePath) 
{
	char sBuffer[_MAX_PATH];
	char *sFilePath;

	sBuffer[0] = '\0';
	if ( !GetFullPathNameA( sePath->GetBuffer(), _MAX_PATH, sBuffer, &sFilePath ) ) 
	{
		sePath->Clear();
		return sePath;
	}
	sePath->Clear();
	sePath->Append( sBuffer );
	return sePath;
}

static time_t GetModTime(char *sFileName) 
{
	return 0;
}
static BOOL OpenTempFile(CString *pwsName, FILE **ppFile, wchar_t *wsMode, wchar_t *wsExt, wchar_t *wsFolder, wchar_t *wsName = NULL) 
{
	if ( NULL == wsName )
	{
		wchar_t *wsTempDir;
		CStringW wsTemp, wsFileName;
		wchar_t wsBuffer[32];
		FILE *pTempFile;

		if ( ( wsTempDir = _wgetenv( _T("TEMP") ) ) && ( wsFolder == NULL ) ) 
		{
			wsTemp = CString( wsTempDir );
			wsTemp.Append( L"\\" );
		} 
		else if( wsFolder != NULL )
		{
			wsTemp = CString( wsFolder );
			wsTemp.Append( L"\\" );
		}
		else
		{ 
			wsTemp = L"";
		}
		wsTemp.Append( L"x" );
		int nTime = (int)time(NULL);
		for (int nIndex = 0; nIndex < 1000; ++nIndex ) 
		{
			_swprintf( wsBuffer, L"%d", nTime + nIndex );
			wsFileName = wsTemp;
			wsFileName.Append( wsBuffer );
			
			CString wsTempName = wsFileName + CString(_T(".*"));
			
			WIN32_FIND_DATA oFindData;
			oFindData.cFileName[0] = '\0';
			HANDLE hFindFile = FindFirstFile( wsTempName,  &oFindData );
			FindClose( hFindFile );

			if ( '\0' == oFindData.cFileName[0] ) 
			//if ( !( pTempFile = _wfopen( wsTempName.GetBuffer(), L"r" ) ) ) 
			{
				if ( wsExt ) 
				{
					wsFileName.Append( wsExt );
				}
				if ( !( pTempFile = _wfopen( wsFileName.GetBuffer(), wsMode ) ) ) 
				{
					return FALSE;
				}
				*pwsName = wsFileName;
				*ppFile = pTempFile;
				return TRUE;
			}

			//fclose( pTempFile );
		}
	}
	else
	{
		wchar_t *wsTempDir;
		CStringW wsFileName;
		FILE *pTempFile;

		if ( ( wsTempDir = _wgetenv( _T("TEMP") ) ) && ( wsFolder == NULL ) ) 
		{
			wsFileName = CString( wsTempDir );
			wsFileName.Append( L"\\" );
		} 
		else if( wsFolder != NULL )
		{
			wsFileName = CString( wsFolder );
			wsFileName.Append( L"\\" );
		}
		else
		{ 
			wsFileName = L"";
		}
		wsFileName.Append( wsName );

		if ( wsExt ) 
			wsFileName.Append( wsExt );

		if ( !( pTempFile = _wfopen( wsFileName.GetBuffer(), wsMode ) ) ) 
				return FALSE;

		*pwsName = wsFileName;
		*ppFile = pTempFile;
		return TRUE;

		fclose( pTempFile );
	}
	return FALSE;
}

static BOOL ExecuteCommand(char *sCommand) 
{
	return system( sCommand ) ? FALSE : TRUE;
}
static char *GetLine(char *sBuffer, int nSize, FILE *pFile) 
{
	int nChar, nCurIndex = 0;

	while ( nCurIndex < nSize - 1 ) 
	{
		if ( ( nChar = fgetc( pFile ) ) == EOF ) 
			break;

		sBuffer[nCurIndex++] = (char)nChar;
		if ( '\x0a' == nChar ) 
		{
			break;
		}
		if ( '\x0d' == nChar ) 
		{
			nChar = fgetc( pFile );
			if ( '\x0a' == nChar && nCurIndex < nSize - 1 ) 
			{
				sBuffer[nCurIndex++] = (char)nChar;
			} 
			else if ( EOF != nChar ) 
			{
				ungetc( nChar, pFile);
			}
			break;
		}
	}
	sBuffer[nCurIndex] = '\0';
	if ( 0 == nCurIndex ) 
		return NULL;
	return sBuffer;
}

//------------------------------------------------------------------------
// CDir and CDirEntry
//------------------------------------------------------------------------

class CDirEntry 
{
public:

  CDirEntry(char *sDirPath, char *sName, BOOL bCheckStat);
  ~CDirEntry();
  StringExt *GetName() 
  { 
	  return m_seName; 
  }
  BOOL IsDir() 
  { 
	  return m_bDir; 
  }

private:

  StringExt *m_seName; // Имя папки или файла
  BOOL       m_bDir;   // Папка или файл?
};

class CDir 
{

public:

  CDir(char *sName, BOOL bCheckStat = TRUE);
  ~CDir();
  CDirEntry *GetNextEntry();
  void Rewind();

private:

  StringExt       *m_sePath;      // путь к директории
  BOOL             m_bCheckStat;  // проверять ли является каждое вхождение папкой?
  WIN32_FIND_DATAA m_FileFindData;
  HANDLE           m_hHandle;
};

#endif /* _FILE_H */
