/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
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

#include "UniversalConverterUtils.h"
#include "../../OOXML/SystemUtility/File.h"

int Utils::CopyDirOrFile(std::wstring sSource, std::wstring sDestination)
{
	//удаляем sDestination, чтобы там не было.
	if( 0 != RemoveDirOrFile( sDestination ) )
		return 1;

	//копируем
	CDirectory::CopyFile(sSource, sDestination);

	return 0;
}
// return "" если не удалось создать
int Utils::RemoveDirOrFile(std::wstring sPath)
{
	CDirectory::DeleteFile(sPath);
	return 0;
}
std::wstring Utils::CreateTempFile( )
{
	return CreateTempFile(NSDirectory::GetTempPath());
}
std::wstring Utils::CreateTempFile( std::wstring sDir )
{
	if( !sDir.empty() )
	{
		return NSDirectory::CreateTempFileWithUniqueName(sDir, L"img");
	}
	else
		return CreateTempFile();
}
// return "" если не удалось создать
std::wstring Utils::CreateTempDir( std::wstring sDir )//создаем файл в папке sDir
{
	if( !sDir.empty() )
	{
		return NSDirectory::CreateDirectoryWithUniqueName(sDir);
	}
	else
		return CreateTempDir();
}
std::wstring Utils::CreateTempDir()
{
	std::wstring tmpDirectory = NSDirectory::GetTempPath();
	return NSDirectory::CreateDirectoryWithUniqueName(tmpDirectory);
}
