/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
#include "OfficeUtils.h"
#include "ZipUtilsCP.h"
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Directory.h"

COfficeUtils::COfficeUtils(OnProgressCallback* fCallback)
{
	m_fCallback = fCallback;
}

HRESULT COfficeUtils::ExtractToDirectory(const std::wstring& zipFile, const std::wstring& unzipDir,  wchar_t* password, SHORT extract_without_path)
{
	if( ZLibZipUtils::UnzipToDir( zipFile.c_str(), unzipDir.c_str(), m_fCallback, password, ( extract_without_path > 0 ) ? (true) : (false) ) == 0 )
	{
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}


HRESULT COfficeUtils::CompressFileOrDirectory(const std::wstring& name, const std::wstring& outputFile, SHORT level)
{
	HRESULT result = S_FALSE;
	if(NSDirectory::Exists(name))
	{
		if ( ZLibZipUtils::ZipDir( name.c_str(), outputFile.c_str(), m_fCallback, level ) == 0 )
		{
			result = S_OK;
		}
		else
		{
			result = S_FALSE;
		}
	}
	else if(NSFile::CFileBinary::Exists(name))
	{
		if ( ZLibZipUtils::ZipFile( name.c_str(), outputFile.c_str(), level ) == 0 )
		{
			result = S_OK;
		}
		else
		{
			result = S_FALSE;
		} 
	}
	return result;
}

HRESULT COfficeUtils::Uncompress(BYTE* destBuf, ULONG* destSize, BYTE* sourceBuf, ULONG sourceSize)
{
  if ( ZLibZipUtils::UncompressBytes( destBuf, destSize, sourceBuf, sourceSize ) == Z_OK )
  {
    return S_OK;
  }
  else
  {
    return S_FALSE;  
  }
}

HRESULT COfficeUtils::Compress(BYTE* destBuf, ULONG* destSize, BYTE* sourceBuf, ULONG sourceSize, SHORT level)
{
  if ( ZLibZipUtils::CompressBytes( destBuf, destSize, sourceBuf, sourceSize, level ) == Z_OK )
  {
    return S_OK;
  }
  else
  {
    return S_FALSE;  
  }
}

HRESULT COfficeUtils::IsArchive(const std::wstring& filename)
{
  if( ZLibZipUtils::IsArchive(filename.c_str()) )
  {
    return S_OK;
  }
  else
  {
    return S_FALSE;
  }
}

HRESULT COfficeUtils::IsFileExistInArchive(const std::wstring& zipFile, const std::wstring& filePath)
{
  if( ZLibZipUtils::IsFileExistInArchive( zipFile.c_str(), filePath.c_str()) )
  {
    return S_OK;
  }
  else
  {
    return S_FALSE;
  }
}

HRESULT COfficeUtils::LoadFileFromArchive(const std::wstring& zipFile, const std::wstring& filePath, BYTE** fileInBytes, ULONG& nFileSize)
{
  if( ZLibZipUtils::LoadFileFromArchive( zipFile.c_str(), filePath.c_str(), fileInBytes, nFileSize))
  {
    return S_OK;
  }
  else
  {
    return S_FALSE;
  }
}

HRESULT COfficeUtils::ExtractFilesToMemory(const std::wstring& zipFile, const ExtractedFileCallback& data_receiver, void* pParam, bool* result)
{
    *result = ZLibZipUtils::ExtractFiles(zipFile.c_str(), data_receiver, pParam) ? true : false;
	return S_OK;
}

HRESULT COfficeUtils::CompressFilesFromMemory(const std::wstring& zipFile, const RequestFileCallback& data_source, void* pParam, SHORT compression_level, bool* result)
{
    *result = ZLibZipUtils::CompressFiles(zipFile.c_str(), data_source, pParam, compression_level) ? true : false;
	return S_OK;
}
