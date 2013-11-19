// OfficeUtils.cpp : Implementation of COfficeUtils

#include "stdafx.h"
#include "OfficeUtils.h"

// COfficeUtils
STDMETHODIMP COfficeUtils::ExtractToDirectory(BSTR zipFile, BSTR unzipDir, BSTR password, SHORT extract_without_path)
{
  ProgressCallback progress;
  progress.OnProgress = OnProgressFunc;
  progress.caller = this;
	
  if( ZLibZipUtils::UnzipToDir( zipFile, unzipDir, &progress, password, ( extract_without_path > 0 ) ? (true) : (false) ) == 0 )
  {
    return S_OK;
  }
  else
  {
    return S_FALSE;
  }
}

/*========================================================================================================*/

STDMETHODIMP COfficeUtils::CompressFileOrDirectory(BSTR name, BSTR outputFile, SHORT level)
{
  WIN32_FIND_DATA ffd;
  HANDLE hFind = INVALID_HANDLE_VALUE;
  HRESULT result = S_FALSE;

  hFind = FindFirstFile( name, &ffd );
  
  if ( hFind == INVALID_HANDLE_VALUE )
  {
    return result;
  }

  if ( ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
  {
    ProgressCallback progress;
    progress.OnProgress = OnProgressFunc;
    progress.caller = this;
	  
	if ( ZLibZipUtils::ZipDir( name, outputFile, &progress, level ) == 0 )
    {
      result = S_OK;
    }
    else
    {
      result = S_FALSE;
    }
  }
  else
  {
	if ( ZLibZipUtils::ZipFile( name, outputFile, level ) == 0 )
    {
      result = S_OK;
    }
    else
    {
      result = S_FALSE;
    } 
  }

  FindClose( hFind );

  return result;
}

/*========================================================================================================*/

STDMETHODIMP COfficeUtils::Uncompress(BYTE* destBuf, ULONG* destSize, BYTE* sourceBuf, ULONG sourceSize)
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

/*========================================================================================================*/

STDMETHODIMP COfficeUtils::Compress(BYTE* destBuf, ULONG* destSize, BYTE* sourceBuf, ULONG sourceSize, SHORT level)
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

/*========================================================================================================*/

STDMETHODIMP COfficeUtils::IsArchive(BSTR filename)
{
  if( ZLibZipUtils::IsArchive(filename) )
  {
    return S_OK;
  }
  else
  {
    return S_FALSE;
  }
}

/*========================================================================================================*/

STDMETHODIMP COfficeUtils::IsFileExistInArchive(BSTR zipFile, BSTR filePath)
{
  if( ZLibZipUtils::IsFileExistInArchive( zipFile, filePath) )
  {
    return S_OK;
  }
  else
  {
    return S_FALSE;
  }
}

/*========================================================================================================*/

STDMETHODIMP COfficeUtils::LoadFileFromArchive(BSTR zipFile, BSTR filePath, BYTE** fileInBytes)
{
  if( ZLibZipUtils::LoadFileFromArchive( zipFile, filePath, fileInBytes))
  {
    return S_OK;
  }
  else
  {
    return S_FALSE;
  }
}

/*========================================================================================================*/

STDMETHODIMP COfficeUtils::ExtractFilesToMemory(BSTR zipFile, IExtractedFileEvent* data_receiver, VARIANT_BOOL* result)
{
	*result = ZLibZipUtils::ExtractFiles(_bstr_t(zipFile), ExtractedFileCallback (data_receiver)) ? VARIANT_TRUE : VARIANT_FALSE;
	return S_OK;
}

/*========================================================================================================*/

STDMETHODIMP COfficeUtils::CompressFilesFromMemory(BSTR zipFile, IRequestFileEvent* data_source, SHORT compression_level, VARIANT_BOOL* result)
{
	*result = ZLibZipUtils::CompressFiles(_bstr_t(zipFile), RequestFileCallback (data_source), compression_level) ? VARIANT_TRUE : VARIANT_FALSE;
	return S_OK;
}

/*========================================================================================================*/

void COfficeUtils::OnProgressFunc( LPVOID lpParam, long nID, long nPercent, short* Cancel )
{
  COfficeUtils* pOfficeUtils = reinterpret_cast<COfficeUtils*>( lpParam );

  if ( pOfficeUtils != NULL )
  {
    pOfficeUtils->OnProgress( nID, nPercent, Cancel );
  }
}
