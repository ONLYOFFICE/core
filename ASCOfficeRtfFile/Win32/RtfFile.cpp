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
// RtfFile.cpp : Implementation of CRtfFile

#include "stdafx.h"

#include "RtfFile.h"
#include "../../Common/OfficeFileErrorDescription.h"

#include "../RtfFormatLib/source/ConvertationManager.h"
#include "../RtfFormatLib/source/Basic.h"

#include "../../Common/DocxFormat/Source/SystemUtility/File.h"

// CRtfFile
STDMETHODIMP CRtfFile::LoadFromFile(BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions)
{
	CString sFilename( sSrcFileName );
	//проверяем входной файл на rtf
	long nError;
	if( FALSE == IsRtfFile( sFilename, nError ) )
	{
		if( NOERROR == nError )
			return AVS_ERROR_FILEFORMAT;
		else
			return nError;
	}

	//проверяем существует ли папка
	DWORD dwDirectoryAttribute = ::GetFileAttributes( sDstPath );
	if( 0 == ( dwDirectoryAttribute & FILE_ATTRIBUTE_DIRECTORY ) )
		return AVS_ERROR_FILEACCESS;
	
	//конвертация
	RtfConvertationManager oConvertationManager;
	
	return oConvertationManager.ConvertRtfToOOX( std::wstring( sSrcFileName ), std::wstring( sDstPath ) );
}


STDMETHODIMP CRtfFile::SaveToFile(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions)
{
	//проверяем доступен ли для записи файл
	HANDLE hFile = CreateFile (sDstFileName, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	if ( INVALID_HANDLE_VALUE == hFile )
		return AVS_ERROR_FILEACCESS;
	CloseHandle( hFile );

	//проверяем существует ли папка
	DWORD dwDirectoryAttribute = ::GetFileAttributes( sSrcPath );
	if( 0 == ( dwDirectoryAttribute & FILE_ATTRIBUTE_DIRECTORY ) )
		return AVS_ERROR_FILEACCESS;
	
	//конвертация
	RtfConvertationManager oConvertationManager;

	oConvertationManager.ConvertOOXToRtf( std::wstring( sDstFileName ), std::wstring( sSrcPath)  );
	return S_OK;
}

bool CRtfFile::IsRtfFile(CString sFilename, long& nError )
{
	nError = NOERROR;
    BYTE    pBuffer[ 5 ];
    DWORD    dwBytesRead;

	CFile file;

	if (file.OpenFile(sFilename) != S_OK) return false;

	file.ReadFile(pBuffer, 5);
	file.CloseFile();

	if( 5 == dwBytesRead && '{' == pBuffer[0] && '\\' == pBuffer[1] && 'r' == pBuffer[2] && 't' == pBuffer[3] && 'f' == pBuffer[4] )
		return true;

	return false;
}
