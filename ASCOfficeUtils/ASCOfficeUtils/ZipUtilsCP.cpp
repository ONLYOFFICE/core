#include "ZipUtilsCP.h"
#include <memory>
#include "CSLocker.h"
#include "../../DesktopEditor/common/Directory.h"
#include "../../DesktopEditor/common/Path.h"

#define WRITEBUFFERSIZE 8192
#define READBUFFERSIZE 8192

namespace ZLibZipUtils
{
  AVSOfficeCriticalSection  criticalSection;
  static zipFile zipOpenHelp(const wchar_t* filename)
  {
#if defined(_WIN32) || defined (_WIN64)
	  zipFile zf = zipOpen( filename, APPEND_STATUS_CREATE );
#else
	  BYTE* pUtf8 = NULL;
	  LONG lLen = 0;
	  NSFile::CUtf8Converter::GetUtf8StringFromUnicode(filename, wcslen(filename), pUtf8, lLen, false);
	  zipFile zf = zipOpen( (char*)pUtf8, APPEND_STATUS_CREATE );
	  delete [] pUtf8;
#endif
	  return zf;
  }
  static unzFile unzOpenHelp(const wchar_t* filename)
  {
#if defined(_WIN32) || defined (_WIN64)
	  unzFile uf = unzOpen (filename);
#else
	  BYTE* pUtf8 = NULL;
	  LONG lLen = 0;
	  NSFile::CUtf8Converter::GetUtf8StringFromUnicode(filename, wcslen(filename), pUtf8, lLen, false);
	  unzFile uf = unzOpen( (char*)pUtf8 );
	  delete [] pUtf8;
#endif
	  return uf;
  }
  static std::wstring ascii_to_unicode(const char *src)
  {
	  size_t nSize = mbstowcs(0, src, 0);
	  wchar_t* pBuffer = new wchar_t[nSize];
	  nSize = mbstowcs(pBuffer, src, nSize);
	  std::wstring sRes;
	  if (nSize != (size_t)-1)
		  sRes = std::wstring(pBuffer, nSize);
	  delete[] pBuffer;
	  return sRes;
  }
  static std::string unicode_to_ascii(const wchar_t *src)
  {
	  size_t nSize = wcstombs(0, src, 0);
	  char* pBuffer = new char[nSize];
	  nSize = wcstombs(pBuffer, src, nSize);
	  std::string sRes;
	  if (nSize != (size_t)-1)
		  sRes = std::string(pBuffer, nSize);
	  delete[] pBuffer;
	  return sRes;
  }
  /*This static functions are copies from ZLib miniunz.c with some changes.*/ 
  static std::wstring codepage_issue_fixFromOEM( const char* sVal)
  {
#if defined(_WIN32) || defined (_WIN64)
	  int nBufferSize = MultiByteToWideChar( CP_OEMCP, 0, sVal, -1, NULL, 0 );
	  wchar_t* pBuffer = new wchar_t[nBufferSize];
	  MultiByteToWideChar( CP_OEMCP, 0, sVal, -1, pBuffer, nBufferSize );
	  std::wstring sRes(pBuffer, nBufferSize);
	  RELEASEARRAYOBJECTS(pBuffer);
	  return sRes;
#else
	  return ascii_to_unicode(sVal); 
#endif
  }
  static std::string codepage_issue_fixToOEM(const std::wstring& sVal)
  {
#if defined(_WIN32) || defined (_WIN64)
	  const wchar_t* buffer = sVal.c_str();
	  int nBufferSize = WideCharToMultiByte( CP_OEMCP, 0, buffer, -1, NULL, 0, NULL, NULL );
	  char* pBuffer = new char[nBufferSize];
	  WideCharToMultiByte( CP_OEMCP, 0, buffer, -1, pBuffer, nBufferSize, NULL, NULL );
	  std::string sRes(pBuffer, nBufferSize);
	  RELEASEARRAYOBJECTS(pBuffer);
	  return sRes;
#else
	  return unicode_to_ascii(sVal.c_str()); 
#endif
  }
  static void change_file_date( const wchar_t *filename, uLong dosdate, tm_unz tmu_date );
  static int mymkdir( const wchar_t* dirname );
  static int makedir( const wchar_t *newdir );
  static int do_extract_currentfile( unzFile uf, const int* popt_extract_without_path, int* popt_overwrite, const char* password );
  static int do_extract( unzFile uf, int opt_extract_without_path, int opt_overwrite, const char* password, const OnProgressCallback* progress );
  
  static bool is_file_in_archive(unzFile uf, const wchar_t *filename);
  static bool current_file_is_find(unzFile uf, const wchar_t *filename);
  static bool get_file_in_archive(unzFile uf, const wchar_t *filePathInZip, BYTE** fileInBytes, ULONG& nFileSize);

  static unsigned int get_files_count( const WCHAR* dirname );

  /*========================================================================================================*/

  /* change_file_date : change the date/time of a file
     filename : the filename of the file where date/time must be modified
     dosdate : the new date at the MSDos format (4 bytes)
     tmu_date : the SAME new date at the tm_unz format */
  static void change_file_date( const wchar_t *filename, uLong dosdate, tm_unz tmu_date )
  {
#if defined(_WIN32) || defined (_WIN64)
    HANDLE hFile;
    FILETIME ftm,ftLocal,ftCreate,ftLastAcc,ftLastWrite;

    hFile = CreateFile(filename,GENERIC_READ | GENERIC_WRITE,
                        0,NULL,OPEN_EXISTING,0,NULL);
    GetFileTime(hFile,&ftCreate,&ftLastAcc,&ftLastWrite);
    DosDateTimeToFileTime((WORD)(dosdate>>16),(WORD)dosdate,&ftLocal);
    LocalFileTimeToFileTime(&ftLocal,&ftm);
    SetFileTime(hFile,&ftm,&ftLastAcc,&ftm);
    CloseHandle(hFile);
#endif
  }

  /*========================================================================================================*/

  /* mymkdir and change_file_date are not 100 % portable
     As I don't know well Unix, I wait feedback for the unix portion */

  static int mymkdir( const wchar_t* dirname )
  {
	return NSDirectory::CreateDirectory(dirname) ? 0 : -1;
  }

  /*========================================================================================================*/

  static int makedir( const wchar_t *newdir )
  {
    wchar_t *buffer ;
    wchar_t *p;
    int  len = (int)wcslen(newdir);

    if (len <= 0)
      return 0;

    buffer = new wchar_t[len+1];
    wcscpy(buffer, newdir);

    if (buffer[len-1] == '/') {
      buffer[len-1] = '\0';
    }
    if (mymkdir(buffer) == 0)
    {
      delete[] buffer;
      return 1;
    }

    p = buffer+1;
    while (1)
    {
      char hold;

      while(*p && *p != '\\' && *p != '/')
        p++;
      hold = *p;
      *p = 0;
	  if ((mymkdir(buffer) == -1) && !NSDirectory::Exists(buffer))
      {
        delete[] buffer;
        return 0;
      }
      if (hold == 0)
        break;
      *p++ = hold;
    }
    delete[] buffer;
    return 1;
  }

  /*========================================================================================================*/

  static int do_extract_currentfile( unzFile uf, const int* popt_extract_without_path, int* popt_overwrite, const char* password )
  {   	  
	char filename_inzipA[256];
	wchar_t filename_inzip[256];
    wchar_t* filename_withoutpath;
    wchar_t* p;
    int err=UNZ_OK;
	NSFile::CFileBinary oFile;
    FILE *fout=NULL;
    void* buf;
    uInt size_buf;

    unz_file_info file_info;
    uLong ratio=0;
    err = unzGetCurrentFileInfo(uf,&file_info,filename_inzipA,sizeof(filename_inzipA),NULL,0,NULL,0);

    std::wstring filenameW = codepage_issue_fixFromOEM(filename_inzipA);
	wcscpy(filename_inzip , filenameW.c_str());

    if (err!=UNZ_OK)
    {
      return err;
    }

    size_buf = WRITEBUFFERSIZE;
    buf = (void*)malloc(size_buf);
    if (buf==NULL)
    {
      return UNZ_INTERNALERROR;
    }

    p = filename_withoutpath = filename_inzip;
    while ((*p) != '\0')
    {
      if (((*p)=='/') || ((*p)=='\\'))
        filename_withoutpath = p+1;
      p++;
    }

    if ((*filename_withoutpath)=='\0')
    {
      if ((*popt_extract_without_path)==0)
      {
        mymkdir(filename_inzip);
      }
    }
    else
    {
      const wchar_t* write_filename;
      int skip=0;

      if ((*popt_extract_without_path)==0)
        write_filename = filename_inzip;
      else
        write_filename = filename_withoutpath;

      err = unzOpenCurrentFilePassword(uf,password);
      if (((*popt_overwrite)==0) && (err==UNZ_OK))
      {
        char rep=0;
		NSFile::CFileBinary oFileTemp;
		if (oFileTemp.OpenFile(write_filename))
        {
			oFileTemp.CloseFile();
        }

        if (rep == 'N')
          skip = 1;

        if (rep == 'A')
          *popt_overwrite=1;
      }

      if ((skip==0) && (err==UNZ_OK))
      {
		  if(oFile.CreateFileW(write_filename))
			 fout = oFile.GetFileNative();

        // some zipfile don't contain directory alone before file 
        if ((fout==NULL) && ((*popt_extract_without_path)==0) &&
		    (filename_withoutpath!=(wchar_t*)filename_inzip))
        {
          char c=*(filename_withoutpath-1);
          *(filename_withoutpath-1)='\0';
          makedir(write_filename);
          *(filename_withoutpath-1)=c;
		  if(oFile.CreateFileW(write_filename))
			  fout = oFile.GetFileNative();
        }
      }

      if (fout!=NULL)
      {
        do
        {
          err = unzReadCurrentFile(uf, buf, size_buf);
          if (err<0)
          {
            break;
          }
          if (err>0)
            if (fwrite(buf,err,1,fout)!=1)
            {			  
              err=UNZ_ERRNO;
              break;
            }
        }
        while (err>0);
		//close вызовется в oFile
        //if (fout)
        //  fclose(fout);

        if (err==0)
          change_file_date(write_filename,file_info.dosDate,
                           file_info.tmu_date);
      }

      if (err==UNZ_OK)
      {
        err = unzCloseCurrentFile (uf);
      }
      else
        unzCloseCurrentFile(uf); // don't lose the error 
    }

    free(buf);
    return err;
  }

  /*========================================================================================================*/

  static int do_extract( unzFile uf, int opt_extract_without_path, int opt_overwrite, const char* password, const OnProgressCallback* progress )
  {
    uLong i;
    unz_global_info gi;
    int err;
    FILE* fout=NULL;

    err = unzGetGlobalInfo (uf,&gi);

    for (i=0;i<gi.number_entry;i++)
    {		
	  if (do_extract_currentfile(uf,&opt_extract_without_path,
                                 &opt_overwrite,
                                 password) != UNZ_OK)
      break;

	  if ( progress != NULL )
	  {
	    short cancel = 0;
	    long progressValue = ( 1000000 / gi.number_entry * i );
		if(NULL != progress)
			(*progress)( UTILS_ONPROGRESSEVENT_ID, progressValue, &cancel );

	    if ( cancel != 0 )
	    {
	      return err;
	    }
	  }

      if ((i+1)<gi.number_entry)
      {
        err = unzGoToNextFile(uf);
        if (err!=UNZ_OK)
        {
          break;
        }
      }
    }

	if ( progress != NULL )
	{
	  short cancel = 0;
	  long progressValue = 1000000;
	  if(NULL != progress)
		(*progress)( UTILS_ONPROGRESSEVENT_ID, progressValue, &cancel );
	}

	return 0;
  }

  /*========================================================================================================*/

  static bool is_file_in_archive(unzFile uf, const wchar_t *filename)
  {
	  uLong i;
	  unz_global_info gi;
	  int err;

	  err = unzGetGlobalInfo (uf,&gi);

	  for (i = 0; i < gi.number_entry; i++)
	  {
		  if (current_file_is_find(uf, filename) == true)
			  return true;

		  if ((i + 1) < gi.number_entry)
		  {
			  err = unzGoToNextFile(uf);
			  if (err != UNZ_OK)
				  break;
		  }
	  }
	  return false;
  }
  
  /*========================================================================================================*/

  static bool current_file_is_find(unzFile uf, const wchar_t *filename)
  {
	  char filename_inzip[256]; 
	  int err = UNZ_OK;

	  unz_file_info file_info;

	  err = unzGetCurrentFileInfo(uf, &file_info, filename_inzip, sizeof(filename_inzip), NULL, 0, NULL, 0);

	  std::wstring filenameW = codepage_issue_fixFromOEM(filename_inzip);

	  if (wcscmp(filename, filenameW.c_str()))
		  return true;
	  return false;
  }

  /*========================================================================================================*/

	static const std::wstring get_filename_from_unzfile(unzFile unzip_file_handle)
	{
	  char filename_inzip[256]; 
	  int err = UNZ_OK;

	  if (UNZ_OK == unzGetCurrentFileInfo(unzip_file_handle, NULL, filename_inzip, sizeof(filename_inzip), NULL, 0, NULL, 0))
		return codepage_issue_fixFromOEM(filename_inzip);

	  return std::wstring(L"");
	}

  /*========================================================================================================*/

  static bool get_file(unzFile unzip_file_handle, BYTE* arr, uInt array_size)
  {	  
	  if(UNZ_OK == unzOpenCurrentFile(unzip_file_handle))
	  {
		  int data_read_size = unzReadCurrentFile(unzip_file_handle, arr, array_size);

		  unzCloseCurrentFile(unzip_file_handle);
		  return data_read_size == array_size ? true : false;
	  }
	  return false;
  }
  /*========================================================================================================*/

  static bool get_file_in_archive(unzFile unzip_file_handle, const wchar_t *filePathInZip, BYTE** fileInBytes, ULONG& nFileSize)
  {
	  if(NULL == fileInBytes)
		  return false;
	  (*fileInBytes) = NULL;
	  do 
	  {
		  if (current_file_is_find(unzip_file_handle, filePathInZip) == true)
		  {
			unz_file_info file_info;
			unzGetCurrentFileInfo(unzip_file_handle, &file_info, NULL, 0, NULL, 0, NULL, 0);
							
			if (nFileSize > 0)
				nFileSize = (std::min)(nFileSize, file_info.uncompressed_size);
			else
				nFileSize = file_info.uncompressed_size;
		  
			(*fileInBytes) = new BYTE[nFileSize];
			get_file(unzip_file_handle, (*fileInBytes), nFileSize);
			return true;
		  }
		  // else just skip the erroneous file
	  } while (UNZ_OK == unzGoToNextFile(unzip_file_handle));
      
      return false;
  }

  /*========================================================================================================*/
  int ZipDir( const WCHAR* dir, const WCHAR* outputFile, const OnProgressCallback* progress, int compressionLevel )
  {
    criticalSection.Enter();
	  
	int err = -1;

    if ( ( dir != NULL ) && ( outputFile != NULL ) )
    {
      deque<wstring> StringDeque;
      deque<wstring> zipDeque;
      StringDeque.push_back( wstring( dir ) );
  
      wstring zipDir;
      wstring file;
      wstring zipFileName;
      wstring szText;

	  zipFile zf = zipOpenHelp(outputFile);

      zip_fileinfo zi;

      zi.tmz_date.tm_sec = zi.tmz_date.tm_min = zi.tmz_date.tm_hour =
      zi.tmz_date.tm_mday = zi.tmz_date.tm_mon = zi.tmz_date.tm_year = 0;
      zi.dosDate = 0;
      zi.internal_fa = 0;
      zi.external_fa = 0;

#if defined(_WIN32) || defined (_WIN64)
      SYSTEMTIME currTime;

      GetLocalTime( &currTime );

      zi.tmz_date.tm_sec = currTime.wSecond;
      zi.tmz_date.tm_min = currTime.wMinute;
      zi.tmz_date.tm_hour = currTime.wHour;
      zi.tmz_date.tm_mday = currTime.wDay;
      zi.tmz_date.tm_mon = currTime.wMonth;
      zi.tmz_date.tm_year = currTime.wYear;
#endif

	  unsigned int filesCount = get_files_count( dir );
	  unsigned int currentFileIndex = 0;
	
	  while ( !StringDeque.empty() )
      {
		  szText = StringDeque.front();
			StringDeque.pop_front();
		  if(zipDeque.size() > 0)
		  {
			  zipDir = zipDeque.front() + wstring( L"/" );
			  zipDeque.pop_front();
		  }
		 
		  CArray<std::wstring> aCurFiles = NSDirectory::GetFiles(szText);
		  CArray<std::wstring> aCurDirectories = NSDirectory::GetDirectories(szText);
		  for(int i = 0; i < aCurDirectories.GetCount(); ++i)
		  {
			  std::wstring sCurDirectory = aCurDirectories[i];
			  std::wstring sDirName = NSSystemPath::GetFileName(sCurDirectory);
			  StringDeque.push_back( sCurDirectory );
			  zipDeque.push_back( zipDir + sDirName );
		  }
		
	    for(int i = 0; i < aCurFiles.GetCount(); ++i)
	    {
			std::wstring cFilePath = aCurFiles[i];
			std::wstring cFileName = NSSystemPath::GetFileName(cFilePath);
				file = NSSystemPath::Combine(szText, cFileName);
          zipFileName = zipDir + cFileName;
		  NSFile::CFileBinary oFile;
		  if(oFile.OpenFile(file))
		  {
			  DWORD dwSizeRead;
			  BYTE* pData = new BYTE[oFile.GetFileSize()];
			  if(oFile.ReadFile(pData, oFile.GetFileSize(), dwSizeRead))
			  {
				  std::string zipFileNameA = codepage_issue_fixToOEM(zipFileName);
				  err = zipOpenNewFileInZip( zf, zipFileNameA.c_str(), &zi, NULL, 0, NULL, 0, NULL, Z_DEFLATED, compressionLevel );
				  err = zipWriteInFileInZip( zf, pData, dwSizeRead );
				  err = zipCloseFileInZip( zf );
			  }
			  RELEASEARRAYOBJECTS(pData);
		  }

		  if ( progress != NULL )
	      {
	        short cancel = 0;
	        long progressValue = ( 1000000 / filesCount * currentFileIndex );
			if(NULL != progress)
				(*progress)( UTILS_ONPROGRESSEVENT_ID, progressValue, &cancel );

	        if ( cancel != 0 )
	        {
			  err = zipClose( zf, NULL );

			  return err;
	        }
	      }

		  currentFileIndex++;
	    }
      }

      err = zipClose( zf, NULL );

	  if ( progress != NULL )
	  {
	    short cancel = 0;
	    long progressValue = 1000000;
		if(NULL != progress)
			(*progress)( UTILS_ONPROGRESSEVENT_ID, progressValue, &cancel );

	  }
	}

    criticalSection.Leave();

    return err;
  }

  /*========================================================================================================*/

  int ZipFile( const WCHAR* inputFile, const WCHAR* outputFile, int compressionLevel )
  {
    criticalSection.Enter();
	  
	int err = -1;

    if ( ( inputFile != NULL ) && ( outputFile != NULL ) )
    {
		NSFile::CFileBinary oFile;
		if(oFile.OpenFile(inputFile))
		{
			DWORD dwSizeRead;
			BYTE* pData = new BYTE[oFile.GetFileSize()];
			if(oFile.ReadFile(pData, oFile.GetFileSize(), dwSizeRead))
			{
				zipFile zf = zipOpenHelp(outputFile);

				zip_fileinfo zi;

				zi.tmz_date.tm_sec = zi.tmz_date.tm_min = zi.tmz_date.tm_hour =
					zi.tmz_date.tm_mday = zi.tmz_date.tm_mon = zi.tmz_date.tm_year = 0;
				zi.dosDate = 0;
				zi.internal_fa = 0;
				zi.external_fa = 0;

#if defined(_WIN32) || defined (_WIN64)
				SYSTEMTIME currTime;

				GetLocalTime( &currTime );

				zi.tmz_date.tm_sec = currTime.wSecond;
				zi.tmz_date.tm_min = currTime.wMinute;
				zi.tmz_date.tm_hour = currTime.wHour;
				zi.tmz_date.tm_mday = currTime.wDay;
				zi.tmz_date.tm_mon = currTime.wMonth;
				zi.tmz_date.tm_year = currTime.wYear;
#endif

				wstring inputFileName( inputFile );

				wstring::size_type pos = 0;
				static const wstring::size_type npos = -1;

				pos = inputFileName.find_last_of( L'\\' );

				wstring zipFileName;

				if ( pos != npos )
				{
					zipFileName = wstring( ( inputFileName.begin() + pos + 1 ), inputFileName.end() );
				}
				else
				{
					zipFileName = wstring( inputFileName.begin(), inputFileName.end() );
				}
				std::string zipFileNameA = codepage_issue_fixToOEM(zipFileName);
				err = zipOpenNewFileInZip( zf, zipFileNameA.c_str(), &zi, NULL, 0, NULL, 0, NULL, Z_DEFLATED, compressionLevel );
				err = zipWriteInFileInZip( zf, pData, dwSizeRead );
				err = zipCloseFileInZip( zf );
				err = zipClose( zf, NULL );
			}
			RELEASEARRAYOBJECTS(pData);
		}
	}

	criticalSection.Leave();

    return false;
  }

  /*========================================================================================================*/

  bool ClearDirectory( const WCHAR* dir, bool delDir )
  {
    criticalSection.Enter();

	bool result = false;
	  
	if ( dir != NULL )
    {
      unsigned int size = (unsigned int)wcslen( dir );
	  WCHAR* _dir = new WCHAR[size + 2];
	  wcsncpy( _dir, dir, size );
      _dir[size++] = L'\0';
      _dir[size] = L'\0';

	  NSDirectory::DeleteDirectory(_dir);

      if ( !delDir )
      {
        NSDirectory::GetDirectories(dir);
      }

	  if ( _dir != NULL )
	  {
	    delete []_dir;
	    _dir = NULL;
	  }

	  result = true;
    }
    else
    {
      result = false;
    }

	criticalSection.Leave();

	return result;
  }

  /*========================================================================================================*/

  int UnzipToDir( const WCHAR* zipFile, const WCHAR* unzipDir, const OnProgressCallback* progress, const WCHAR* password, bool opt_extract_without_path, bool clearOutputDirectory )
  {
    criticalSection.Enter();
	
    unzFile uf = NULL;

    int err = -1;

    if ( ( zipFile != NULL ) && ( unzipDir != NULL ) )
    {
	  uf = unzOpenHelp (zipFile);
    }

    if ( uf != NULL )
    {
      if ( clearOutputDirectory )
	  {
	    ClearDirectory( unzipDir );
	  }
#if defined(_WIN32) || defined (_WIN64)
	  wchar_t* buffer = NULL;

	  buffer = _wgetcwd( NULL, 0 );
	  	  
	  err = _wchdir (unzipDir);
#else
	  char* buffer = NULL;

	  buffer = getcwd( NULL, 0 );
	  BYTE* pUtf8 = NULL;
	  LONG lLen = 0;
      NSFile::CUtf8Converter::GetUtf8StringFromUnicode(unzipDir, wcslen(unzipDir), pUtf8, lLen);
	  err = chdir ((char*)pUtf8);
	  RELEASEARRAYOBJECTS(pUtf8);
#endif
  
      if ( err == 0 )
	  {
	    if(NULL != password)
	    {
			std::string passwordA = codepage_issue_fixToOEM(password);
			err = do_extract( uf, opt_extract_without_path, 1, passwordA.c_str(), progress );
	    }
	    else
			err = do_extract( uf, opt_extract_without_path, 1, NULL, progress );
	  }

      if ( err == UNZ_OK )
	  {
	    err = unzClose( uf );
	  }

	  if ( buffer != NULL )
	  {
#if defined(_WIN32) || defined (_WIN64)
	    err = _wchdir( buffer );
#else
	    err = chdir( buffer );
#endif

	    free( buffer );
	    buffer = NULL;
	  }
    }

    criticalSection.Leave();

    return err;
  }
  
  /*========================================================================================================*/
  
  int UncompressBytes( BYTE* destBuf, ULONG* destSize, const BYTE* sourceBuf, ULONG sourceSize )
  {
    criticalSection.Enter();
	  
	int err = -1;

	err = uncompress( destBuf, destSize, sourceBuf, sourceSize );

	criticalSection.Leave();
	
	return err;
  }

  /*========================================================================================================*/

  int CompressBytes( BYTE* destBuf, ULONG* destSize, const BYTE* sourceBuf, ULONG sourceSize, SHORT level )
  {
    criticalSection.Enter();
	  
	int err = -1;

	err = compress2( destBuf, destSize, sourceBuf, sourceSize, level );

	criticalSection.Leave();

	return err;
  }

  /*========================================================================================================*/

	bool IsArchive(const WCHAR* filename)
	{
		criticalSection.Enter();

	  unzFile uf = NULL;
	  bool isZIP = false;

	  if (( filename != NULL ))
		  uf = unzOpenHelp( filename );

	  if ( uf != NULL )
	  {
		  isZIP = true;
		  unzClose( uf );
	  }

	  criticalSection.Leave();

	  return isZIP;
	}

	/*========================================================================================================*/

  bool IsFileExistInArchive(const WCHAR* zipFile, const WCHAR* filePathInZip)
  {
	  criticalSection.Enter();

	  unzFile uf = NULL;
	  bool isIn = false;

	  if ( ( zipFile != NULL ) && ( filePathInZip != NULL ) )
		  uf = unzOpenHelp( zipFile );
	  if ( uf != NULL )
	  {		  
		  isIn = is_file_in_archive( uf, filePathInZip );
		  unzClose( uf );
	  }	 

	  criticalSection.Leave();
	  return isIn;
  }

/*========================================================================================================*/

  bool LoadFileFromArchive(const WCHAR* zipFile, const WCHAR* filePathInZip, BYTE** fileInBytes, ULONG& nFileSize)
  {
	  criticalSection.Enter();

	  unzFile uf = NULL;
	  bool isIn = false;

	  if ( ( zipFile != NULL ) && ( filePathInZip != NULL ) )
		  uf = unzOpenHelp( zipFile );

	  if ( uf != NULL )
	  {		  
		  isIn = get_file_in_archive( uf, filePathInZip, fileInBytes, nFileSize);
		  unzClose( uf );
	  }
	  
	  criticalSection.Leave();
	  return isIn;
  }

/*========================================================================================================*/

	bool ExtractFiles(const wchar_t* zip_file_path, const ExtractedFileCallback& callback, void* pParam)
	{
		CSLocker locker(criticalSection);

		unzFile unzip_file_handle = unzOpenHelp(zip_file_path);
		if ( unzip_file_handle != NULL )
		{		  
			do 
			{
				unz_file_info file_info;
				unzGetCurrentFileInfo(unzip_file_handle, &file_info, NULL, 0, NULL, 0, NULL, 0);

				BYTE* pData = new BYTE[file_info.uncompressed_size];
				if(file_info.uncompressed_size == 0 || get_file(unzip_file_handle, pData, file_info.uncompressed_size))
				{
					callback(get_filename_from_unzfile(unzip_file_handle), pData, file_info.uncompressed_size, pParam);
				}
				RELEASEARRAYOBJECTS(pData);
				// else just skip the erroneous file
			} while (UNZ_OK == unzGoToNextFile(unzip_file_handle));
					
			unzClose( unzip_file_handle );
			return true;
		}
		return false;
	}

	/*========================================================================================================*/

	bool CompressFiles(const wchar_t* zip_file_path, const RequestFileCallback& callback, void* pParam, int compression_level)
	{
		CSLocker locker(criticalSection);

		zipFile zip_file_handle = zipOpenHelp(zip_file_path);

		if(NULL != zip_file_handle)
		{
			zip_fileinfo zi = {0};
#if defined(_WIN32) || defined (_WIN64)
			SYSTEMTIME currTime;
			GetLocalTime( &currTime );
			zi.tmz_date.tm_sec = currTime.wSecond;
			zi.tmz_date.tm_min = currTime.wMinute;
			zi.tmz_date.tm_hour = currTime.wHour;
			zi.tmz_date.tm_mday = currTime.wDay;
			zi.tmz_date.tm_mon = currTime.wMonth;
			zi.tmz_date.tm_year = currTime.wYear;
#endif

			BYTE* pData = NULL;
			long nSize;
			std::wstring in_zip_filename;
			while(callback(in_zip_filename, pData, nSize, pParam))
			{
				std::string in_zip_filenameA = codepage_issue_fixToOEM(in_zip_filename);
				if (ZIP_OK != zipOpenNewFileInZip( zip_file_handle, in_zip_filenameA.c_str(), &zi, NULL, 0, NULL, 0, NULL, Z_DEFLATED, compression_level ) ||
					ZIP_OK != zipWriteInFileInZip(zip_file_handle, pData, nSize) ||
					ZIP_OK != zipCloseFileInZip(zip_file_handle))
				{
					zipClose(zip_file_handle, NULL);
					return false;
				}
			}
			zipClose(zip_file_handle, NULL);

			return true;
		}
		return false;
	}

	/*========================================================================================================*/

	static unsigned int get_files_count( const WCHAR* dirname )
	{
	  unsigned int filescount = 0;

      if ( dirname != NULL )
      {
        deque<wstring> StringDeque;
        StringDeque.push_back( dirname );
  
        wstring file;
        wstring szText;

        while ( !StringDeque.empty() )
        {
		  szText = StringDeque.front();

		  StringDeque.pop_front();

		  CArray<std::wstring> aCurFiles = NSDirectory::GetFiles(szText);
		  filescount += aCurFiles.GetCount();
		  CArray<std::wstring> aCurDirectories = NSDirectory::GetDirectories(szText);
		  for(int i = 0; i < aCurDirectories.GetCount(); ++i)
		  {
			  std::wstring sCurDirectory = aCurDirectories[i];
			  StringDeque.push_back( sCurDirectory );
		  }
        }
	  }

      return filescount;
	}
}
