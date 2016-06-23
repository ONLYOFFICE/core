/*
 * (c) Copyright Ascensio System SIA 2010-2016
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
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
        CString tmpDirectory = FileSystem::Directory::GetTempPath();
        return FileSystem::Directory::CreateDirectoryWithUniqueName(tmpDirectory);
	}
	static  CString PrepareToXML( const CString & sInput)
	{
		 CString sResult = sInput;
		 //&amp; («&И), &lt; («<И), &gt; («>И), &apos; («'И), и &quot; («"И)
		 sResult.Replace(_T("\x06"), _T(""));	//ЗБ·О±Ч·ҐАіЮн.rtf
		 sResult.Replace(_T("&"), _T("&amp;"));
		 sResult.Replace(_T("<"), _T("&lt;"));
		 sResult.Replace(_T(">"), _T("&gt;"));
		 sResult.Replace(_T("\""), _T("&quot;"));
		 sResult.Replace(_T("'"), _T("&apos;"));
		 return sResult;
	 }
};
