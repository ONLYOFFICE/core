#include "OfficeUtils.h"
#include "../ASCOfficeUtils/ZipUtilsCP.h"
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
