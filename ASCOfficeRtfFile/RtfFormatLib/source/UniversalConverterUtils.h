#pragma once
#define BUFSIZE 2048
#include <time.h>

#include "../../../Common/DocxFormat/Source/SystemUtility/FileSystem/Directory.h"
#include "../../../Common/DocxFormat/Source/SystemUtility/File.h"

class Utils
{
public:	
	static int CopyDirOrFile(CString sSource, CString sDestination)
	{
		//удаляем sDestination, чтобы там не было.
		if( 0 != RemoveDirOrFile( sDestination ) )
			return 1;

		//копируем
		CDirectory::CopyFile(sSource, sDestination, NULL, NULL);

		return 0;
	}
// return "" если не удалось создать
 
	static int RemoveDirOrFile(CString sPath)
	{
		CDirectory::DeleteFile(sPath);
		return 0;
	}
	static CString CreateTempFile( )
	{
		return CreateTempFile(FileSystem::Directory::GetTempPath());
	}
	static CString CreateTempFile( CString sDir )
	{
		if( _T("") != sDir )
		{
			return FileSystem::Directory::CreateTempFileWithUniqueName(sDir,_T("img"));
		}
		else
			return CreateTempFile();
	}
// return "" если не удалось создать
	static CString CreateTempDir( CString sDir )//создаем файл в папке sDir
	{
		if( _T("") != sDir )
		{
			return FileSystem::Directory::CreateDirectoryWithUniqueName(sDir);
		}
		else
			return CreateTempDir();
	}
	static CString CreateTempDir()
	{
		return FileSystem::Directory::CreateDirectoryWithUniqueName(FileSystem::Directory::GetTempPath());
	}
	static  CString PrepareToXML( CString sInput)
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
};
