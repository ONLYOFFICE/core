#pragma once

#include "UniversalString.h"

#include <string>
#include <deque>
#include <fstream>
#include <vector>
#include <list>

#include <direct.h>
#include "CallbackHelpers.h"

using namespace std;

#define ZLIB_WINAPI
#define CODEPAGE_ISSUE_FIX

//#pragma comment(lib, "zlibstat.lib")

#include "unzip.h"
#include "zip.h"

namespace ZLibZipUtils
{
  int ZipDir( const WCHAR* dir, const WCHAR* outputFile, const ProgressCallback* progress, int compressionLevel = -1 );
  int ZipFile( const WCHAR* inputFile, const WCHAR* outputFile, int compressionLevel = -1 );
  bool ClearDirectory( const WCHAR* dir, bool delDir = false );
  int UnzipToDir( const WCHAR* zipFile, const WCHAR* unzipDir, const ProgressCallback* progress, const WCHAR* password = NULL, bool opt_extract_without_path = false, bool clearOutputDirectory = false );
  int UncompressBytes( BYTE* destBuf, ULONG* destSize, const BYTE* sourceBuf, ULONG sourceSize );
  int CompressBytes( BYTE* destBuf, ULONG* destSize, const BYTE* sourceBuf, ULONG sourceSize, SHORT level );
  
  bool IsArchive(const WCHAR* filename);
  bool IsFileExistInArchive(const WCHAR* zipFile, const WCHAR* filePathInZip);
  bool LoadFileFromArchive(const WCHAR* zipFile, const WCHAR* filePathInZip, BYTE** fileInBytes);
  bool ExtractFiles(const _bstr_t zip_file_path, ExtractedFileCallback& callback);
  bool CompressFiles(_bstr_t zip_file_path, RequestFileCallback& callback, int compression_level);
}