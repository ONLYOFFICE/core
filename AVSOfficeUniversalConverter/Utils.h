#pragma once
#define BUFSIZE 2048
#include <time.h>

class CCritSect
{
public: CRITICAL_SECTION m_csThread;
	CCritSect()
	{
		InitializeCriticalSection(&m_csThread);
	}
	~CCritSect()
	{
		DeleteCriticalSection(&m_csThread);
	}
};

__declspec(selectany) CCritSect g_oCS;

class Utils
{
public: static INT32 CopyDirOrFile(CString sSource, CString sDestination)
		{
			//если последний символ '\' или '/' удаляем его
			int nSourceLen = sSource.GetLength();
			int nDestLen = sDestination.GetLength();
			if( sSource[ nSourceLen - 1 ] == '\\' || sSource[ nSourceLen - 1 ] == '/' )
				sSource.Delete( nSourceLen - 1, 1 );
			if( sDestination[ nDestLen - 1 ] == '\\' || sDestination[ nDestLen - 1 ] == '/' )
				sDestination.Delete( nDestLen - 1, 1 );

			//удаляем sDestination, чтобы там не было.
			if( 0 != RemoveDirOrFile( sDestination ) )
				return 1;

			//копируем
			DWORD dwFileAttrib = ::GetFileAttributes( sSource );
			if( dwFileAttrib != INVALID_FILE_ATTRIBUTES )
			{
				DWORD dwResult = 0;
				if( 0 != (FILE_ATTRIBUTE_DIRECTORY & dwFileAttrib) )
				{
					DWORD dwRes = CreateDirectory( sDestination ,NULL) ;
					DWORD dwExistDirAttrib = ::GetFileAttributes( sDestination );
					if( dwExistDirAttrib == INVALID_FILE_ATTRIBUTES)
						return dwRes;

					HANDLE Handle;
					WIN32_FIND_DATA FindData;
					DWORDLONG Result = 0;

					Handle = FindFirstFile( ( sSource + _T("\\*.*") ), &FindData );
					if ( Handle == INVALID_HANDLE_VALUE )
						return 0;
					do
					{
						BOOL bRes = TRUE;
						if( ( CString( FindData.cFileName ) != _T(".") ) && ( CString( FindData.cFileName ) != _T("..") ) )
							if( FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
								dwResult = CopyDirOrFile( sSource + _T("\\") + FindData.cFileName, sDestination + _T("\\") + FindData.cFileName );
							else
								bRes = CopyFile( sSource + _T("\\") + FindData.cFileName, sDestination + _T("\\") + FindData.cFileName, FALSE );
						if( FALSE == bRes || 1 == dwResult )
						{
							dwResult = 1;
							break;
						}
					}
					while( FindNextFile( Handle, &FindData ) != 0 );
					FindClose( Handle );
				}
				else
				{
					BOOL bRes = CopyFile( sSource, sDestination, FALSE );
					if( FALSE == bRes )
						dwResult = 1;
				}

				return dwResult;
			}// 0 - все закончилось хорошо
			return 0;
		}
// return "" если не удалось создать
public: static INT32 RemoveDirOrFile(CString sPath)
		{
			DWORD dwFileAttrib = ::GetFileAttributes( sPath );
			if(  dwFileAttrib != INVALID_FILE_ATTRIBUTES )
			{
				DWORD dwResult = 0;
				if( 0 != (FILE_ATTRIBUTE_DIRECTORY & dwFileAttrib) )
				{
					HANDLE Handle;
					WIN32_FIND_DATA FindData;
					DWORDLONG Result = 0;

					Handle = FindFirstFile( ( sPath + _T("\\*.*") ), &FindData );
					if ( Handle == INVALID_HANDLE_VALUE )
						return 0;
					do
					{
						BOOL bRes = TRUE;
						if( ( CString( FindData.cFileName ) != _T(".") ) && ( CString( FindData.cFileName ) != _T("..") ) )
							if( FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
								Result += RemoveDirOrFile( sPath + _T("\\") + FindData.cFileName ); //
							else
								bRes = DeleteFile( sPath + _T("\\") + FindData.cFileName );
						if( FALSE == bRes )
							dwResult += 1;
					}
					while( FindNextFile( Handle, &FindData ) != 0 );
					FindClose( Handle );
					BOOL bRes = RemoveDirectory( sPath );
					if( FALSE == bRes )
						dwResult += 1;
				}
				else
				{
					if( FALSE == DeleteFile( sPath ) )
						dwResult = 1;
				}

				return dwResult;
			}// 0 - все закончилось хорошо
			return 0;
		}
public: static CString CreateTempFile( )
		{
			DWORD dwBufSize = BUFSIZE;
			TCHAR lpPathBuffer[BUFSIZE];
			//получаем имя темповой директории
			DWORD dwRetVal = ::GetTempPath(dwBufSize,lpPathBuffer);
			if( dwRetVal < dwBufSize && 0 != dwRetVal )
				return CreateTempFile( CString(lpPathBuffer) );
			else
				return _T("");
		}
public: static CString CreateTempFile( CString sDir )
		{
			if( _T("") != sDir )
			{
				int nLen = sDir.GetLength();
				if( sDir.GetLength() > 0 && ( sDir[sDir.GetLength() - 1 ] == '\\' || sDir[sDir.GetLength()- 1 ] == '/' ) )
				{
					sDir = sDir.Left( sDir.GetLength() - 1 );
				}
				TCHAR szTempName[BUFSIZE];  
				//получаем уникальное имя для темповой папки
				UINT uRetVal = ::GetTempFileName(sDir, NULL, 0, szTempName);
				//HANDLE hFile = CreateFile( szTempName, GENERIC_ALL, FILE_SHARE_READ , NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
				if (0!=uRetVal)
				{
					//RELEASEHANDLE( hFile );
					return CString( szTempName );
				}
				else
					return _T("");
			}
			else
				return CreateTempFile();
		}
// return "" если не удалось создать
public: static CString CreateTempDir( CString sDir )//создаем файл в папке sDir
		{
			if( _T("") != sDir )
			{
				int nLen = sDir.GetLength();
				if( sDir.GetLength() > 0 && ( sDir[sDir.GetLength() - 1 ] == '\\' || sDir[sDir.GetLength()- 1 ] == '/' ) )
				{
					sDir = sDir.Left( sDir.GetLength() - 1 );
				}
				HANDLE oMutex = CreateMutex(NULL, FALSE, _T("AVSOfficeUniversalConverter_Utils_h"));
				DWORD dwWaitResult = WaitForSingleObject( oMutex, INFINITE);
				DWORD dwRes = 0;
				CString sNewDirName;
				try
				{
					if(WAIT_OBJECT_0 == dwWaitResult)
					{
						srand( (unsigned)time(NULL) );
						do
						{
							int nRandom = rand() % 100000000;
							CString sRandName;
							sRandName.AppendFormat( _T("AvsTmpDll%d"), nRandom );
							sNewDirName = sDir + _T("\\") + sRandName;
						}
						while( INVALID_FILE_ATTRIBUTES != ::GetFileAttributes( sNewDirName ) );
						dwRes = CreateDirectory( sNewDirName ,NULL);
					}
				}
				catch(...)
				{
				}
				ReleaseMutex(oMutex);
				if( 0 != dwRes)
					return sNewDirName;
				return _T("");
			}
			else
				return CreateTempDir();
		}
public: static CString CreateTempDir()
		 {
			DWORD dwBufSize = BUFSIZE;
			TCHAR lpPathBuffer[BUFSIZE];
			//получаем имя темповой директории
			DWORD dwRetVal = ::GetTempPath(dwBufSize, lpPathBuffer);
			if( dwRetVal < dwBufSize && 0 != dwRetVal )
				return CreateTempDir( CString(lpPathBuffer) );
			else
				return _T("");
		 }
public:	 static  CString PrepareToXML( CString sInput)
		 {
			 CString sResult = sInput;
			 //&amp; («&»), &lt; («<»), &gt; («>»), &apos; («'»), и &quot; («"») 
			 sResult.Replace(_T("&"), _T("&amp;"));
 			 sResult.Replace(_T("<"), _T("&lt;"));
  			 sResult.Replace(_T(">"), _T("&gt;"));
  			 sResult.Replace(_T("\""), _T("&quot;"));
   			 sResult.Replace(_T("'"), _T("&apos;"));
			 return sResult;
		 }
public: static CStringA UnicodeToCodepage( CStringW sUni, UINT nCodepage = CP_UTF8 )
		{
			CStringA sUtf;
			int nBufSize = WideCharToMultiByte(nCodepage, 0, sUni, -1, NULL, NULL, NULL, NULL);
			LPSTR pBuff = new CHAR[ nBufSize + 1 ];
			WideCharToMultiByte(nCodepage, 0, sUni, -1, pBuff, nBufSize, NULL, NULL);
			pBuff[ nBufSize ] = 0;
			sUtf.Append( pBuff );
			delete pBuff;
			return sUtf;
		}
public: static CStringW CodepageToUnicode( CStringA sUtf, UINT nCodepage = CP_UTF8 )
		{
			CStringW sUni;
			int nBufSize = MultiByteToWideChar(nCodepage, 0, sUtf, -1, NULL, NULL);
			LPWSTR pBuff = new WCHAR[ nBufSize + 1 ];
			MultiByteToWideChar(nCodepage, 0, sUtf, -1, pBuff, nBufSize);
			pBuff[ nBufSize ] = 0;
			sUni.Append( pBuff );
			delete pBuff;
			return sUni;
		}
};
