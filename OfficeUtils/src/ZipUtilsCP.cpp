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
#include "ZipUtilsCP.h"
#include <memory>
#include <algorithm>
#include "../../DesktopEditor/common/Directory.h"
#include "../../DesktopEditor/common/Path.h"

#if !defined(_WIN32) && !defined (_WIN64)
#include <unistd.h>
#include <sys/stat.h>
#endif

#define WRITEBUFFERSIZE 8192
#define READBUFFERSIZE 8192

namespace ZLibZipUtils
{
#ifndef _IOS
  zipFile zipOpenHelp(const wchar_t* filename)
  {   
#if defined(_WIN32) || defined (_WIN64)
	  zlib_filefunc64_def ffunc;
	  fill_win32_filefunc64W(&ffunc);
	  zipFile zf = zipOpen2_64(filename, APPEND_STATUS_CREATE, NULL, &ffunc);
#else
	  BYTE* pUtf8 = NULL;
	  LONG lLen = 0;
	  NSFile::CUtf8Converter::GetUtf8StringFromUnicode(filename, wcslen(filename), pUtf8, lLen, false);
	  zipFile zf = zipOpen( (char*)pUtf8, APPEND_STATUS_CREATE );
	  delete [] pUtf8;
#endif
	  return zf;
  }
  unzFile unzOpenHelp(const wchar_t* filename)
  {   
#if defined(_WIN32) || defined (_WIN64)
	  zlib_filefunc64_def ffunc;
	  fill_win32_filefunc64W(&ffunc);
	  unzFile uf = unzOpen2_64(filename, &ffunc);
#else
	  BYTE* pUtf8 = NULL;
	  LONG lLen = 0;
	  NSFile::CUtf8Converter::GetUtf8StringFromUnicode(filename, wcslen(filename), pUtf8, lLen, false);
	  unzFile uf = unzOpen( (char*)pUtf8 );
	  delete [] pUtf8;
#endif
	  return uf;
  }
#endif
  static std::wstring ascii_to_unicode(const char *src)
  {
      std::string sAnsi(src);
      return std::wstring(sAnsi.begin(), sAnsi.end());
  }
  static std::string unicode_to_ascii(const wchar_t *src)
  {
      std::wstring sUnicode(src);
      return std::string(sUnicode.begin(), sUnicode.end());
  }
  /*This static functions are copies from ZLib miniunz.c with some changes.*/ 
  static std::wstring codepage_issue_fixFromOEM( const char* sVal)
  {
#if defined(_WIN32) || defined (_WIN64)
	  int nBufferSize = MultiByteToWideChar( CP_OEMCP, 0, sVal, -1, NULL, 0 );
	  wchar_t* pBuffer = new wchar_t[nBufferSize];
	  MultiByteToWideChar( CP_OEMCP, 0, sVal, -1, pBuffer, nBufferSize );
	  //If this parameter is -1, the function processes the entire input string, including the terminating null character.
	  //Therefore, the resulting Unicode string has a terminating null character, and the length returned by the function includes this character.
	  std::wstring sRes(pBuffer, nBufferSize - 1);
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

    hFile = CreateFileW(filename,GENERIC_READ | GENERIC_WRITE,
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

  static void replace_all(std::string& subject, const std::string& search, const std::string& replace)
  {
      size_t pos = 0;
      while ((pos = subject.find(search, pos)) != std::string::npos)
      {
          subject.replace(pos, search.length(), replace);
          pos += replace.length();
      }
  }
  static int do_extract_currentfile( unzFile uf, const int* popt_extract_without_path, int* popt_overwrite, const char* password )
  {   	  
	char filename_inzipA[256];
	wchar_t filename_inzip[256];

    wchar_t* filename_withoutpath = NULL;
    wchar_t* p = NULL;
    int err = UNZ_OK;

    unz_file_info file_info;

    err = unzGetCurrentFileInfo(uf,&file_info,filename_inzipA,sizeof(filename_inzipA),NULL,0,NULL,0);

    std::wstring filenameW = codepage_issue_fixFromOEM(filename_inzipA);
	wcscpy(filename_inzip , filenameW.c_str());

    if (err!=UNZ_OK)
    {
      return err;
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

      err = unzOpenCurrentFilePassword(uf, password);
      if (((*popt_overwrite)==0) && (err==UNZ_OK))
      {
        char rep = 0;
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
//-------------------------------------------------------------------------------------------------

      char* current_directory = getcwd(NULL, 0);

      if (current_directory)
      {
            std::string current_path(current_directory);
            free(current_directory);

            current_path += FILE_SEPARATOR_STRA;

            replace_all(current_path, "/", FILE_SEPARATOR_STRA);
            replace_all(current_path, "\\", FILE_SEPARATOR_STRA);

            std::string filename_inzip(filename_inzipA);

            replace_all(filename_inzip, "/", FILE_SEPARATOR_STRA);
            replace_all(filename_inzip, "\\", FILE_SEPARATOR_STRA);

            std::string norm_path = NSSystemPath::NormalizePath(current_path + filename_inzip);
            std::string norm_current_path = NSSystemPath::NormalizePath(current_path);

            if (std::string::npos == norm_path.find(norm_current_path))
            {
                return UNZ_INTERNALERROR;
            }
     }
//-------------------------------------------------------------------------------------------------
      NSFile::CFileBinary oFile;
      FILE *fout = NULL;

      if ((skip==0) && (err==UNZ_OK))
      {
		  if(oFile.CreateFileW(write_filename))
			 fout = oFile.GetFileNative();

        // some zipfile don't contain directory alone before file 
        if ((fout == NULL) && ((*popt_extract_without_path)==0) &&
		    (filename_withoutpath!=(wchar_t*)filename_inzip))
        {

          char c=*(filename_withoutpath-1);
          *(filename_withoutpath-1)='\0';
          makedir(write_filename);
          *(filename_withoutpath-1)=c;

		  if(oFile.CreateFileW(write_filename))
          {
			  fout = oFile.GetFileNative();
		  }
        }
      }

      uInt size_buf = WRITEBUFFERSIZE;
      void* buf = (void*)malloc(size_buf);

      if (buf == NULL)
      {
        return UNZ_INTERNALERROR;
      }

      if (fout != NULL)
      {
          do
          {
            err = unzReadCurrentFile(uf, buf, size_buf);
            if (err<0)
            {
              break;
            }
            if (err>0)
              if (fwrite(buf, err, 1, fout) != 1)
              {
                err=UNZ_ERRNO;
                break;
              }
          }
          while (err>0);

 //close вызовется в oFile

        if (err==0)
        {
            change_file_date(write_filename, file_info.dosDate, file_info.tmu_date);
        }
      }

      if (err == UNZ_OK)
      {
        err = unzCloseCurrentFile (uf);
      }
      else
        unzCloseCurrentFile(uf); // don't lose the error

      free(buf);
    }

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
	  {
			err = -1;
			break;
	  }

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

	return err;
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

      //todooo есть ли необходимость свести все к нижнему ???
      if (wcscmp(filename, filenameW.c_str()) == 0)
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

int oneZipFile(zipFile & zf, zip_fileinfo* zi, std::wstring & file_name, std::wstring & zip_file_name, int method, int compressionLevel, bool bDateTime)
{
	int err = -1;

    NSFile::CFileBinary oFile;

    zip_fileinfo zinfo;
    zinfo.dosDate = zinfo.external_fa = zinfo.internal_fa = 0;
    zinfo.tmz_date.tm_sec = zinfo.tmz_date.tm_min = zinfo.tmz_date.tm_hour = 0;
    zinfo.tmz_date.tm_mday = 1;
    zinfo.tmz_date.tm_mon = 0;
    zinfo.tmz_date.tm_year = 1980;

    zip_fileinfo* zi_new = zi ? zi : &zinfo;
    if (bDateTime )
    {
        zi_new->dosDate = oFile.GetDateTime(file_name);
    }
	if(oFile.OpenFile(file_name))
    {
        DWORD dwSizeRead;
		BYTE* pData = new BYTE[oFile.GetFileSize()];
		if(oFile.ReadFile(pData, oFile.GetFileSize(), dwSizeRead))
		{
			std::string zipFileNameA = codepage_issue_fixToOEM(zip_file_name);

            err = zipOpenNewFileInZip( zf, zipFileNameA.c_str(), zi_new, NULL, 0, NULL, 0, NULL, method, compressionLevel );
			err = zipWriteInFileInZip( zf, pData, dwSizeRead );
			err = zipCloseFileInZip( zf );
		}
		RELEASEARRAYOBJECTS(pData);
	}
	return 0;
}
int ZipDir( const WCHAR* dir, const WCHAR* outputFile, const OnProgressCallback* progress, bool sorted, int method, int compressionLevel, bool bDateTime )
{ 
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
        if (!zf) return -1;

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

			std::vector<std::wstring> aCurFiles			= NSDirectory::GetFiles(szText);
			std::vector<std::wstring> aCurDirectories	= NSDirectory::GetDirectories(szText);
			
            for(size_t i = 0; i < aCurDirectories.size(); ++i)
			{
				std::wstring sDirName = NSSystemPath::GetFileName(aCurDirectories[i]);

                if (sorted)
                {
                    if (sDirName == L"ppt")
                    {
                         StringDeque.push_front(aCurDirectories[i] );
                         zipDeque.push_front( zipDir + sDirName );
                    }
                    else if(sDirName == L"xl")
                    {
                         StringDeque.push_front( aCurDirectories[i] );
                         zipDeque.push_front( zipDir + sDirName );
                     }
                    else if (sDirName == L"word")
                    {
                         StringDeque.push_front( aCurDirectories[i] );
                         zipDeque.push_front( zipDir + sDirName );
                     }
                    else
                    {
                        StringDeque.push_back( aCurDirectories[i] );
                        zipDeque.push_back( zipDir + sDirName );
                    }
                }
                else
                {
                    StringDeque.push_back( aCurDirectories[i] );
                    zipDeque.push_back( zipDir + sDirName );
                }
			}

			for (size_t i = 0; i < aCurFiles.size(); ++i)
			{
				std::wstring cFileName = NSSystemPath::GetFileName(aCurFiles[i]);
				
                if (std::wstring::npos != cFileName.find(L"mimetype") ||
                    std::wstring::npos != cFileName.find(L"[Content_Types]")) // возможно и полное соответствие
				{
					file = NSSystemPath::Combine(szText, cFileName);
					zipFileName = zipDir + cFileName;
					
                    oneZipFile(zf, NULL, file, zipFileName, 0, compressionLevel, bDateTime);

					aCurFiles.erase(aCurFiles.begin() + i, aCurFiles.begin() + i + 1);
					break;
				}
			}

			for (size_t i = 0; i < aCurFiles.size(); ++i)
			{
				std::wstring cFileName = NSSystemPath::GetFileName(aCurFiles[i]);
				file = NSSystemPath::Combine(szText, cFileName);
				zipFileName = zipDir + cFileName;

                oneZipFile(zf, NULL, file, zipFileName, method, compressionLevel, bDateTime);

				if ( progress != NULL )
				{
					short cancel = 0;
					long progressValue = ( 1000000 / filesCount * currentFileIndex );
					
					if(NULL != progress)
						(*progress)( UTILS_ONPROGRESSEVENT_ID, progressValue, &cancel );

					if ( cancel != 0 )
					{
						zipClose( zf, NULL );
					}
				}
				currentFileIndex++;
			}
		}
		zipClose( zf, NULL );

		if ( progress != NULL )
		{
			short cancel = 0;
			long progressValue = 1000000;
			if(NULL != progress)
				(*progress)( UTILS_ONPROGRESSEVENT_ID, progressValue, &cancel );
		}
	}
    return 0;
}

  /*========================================================================================================*/

  int ZipFile( const WCHAR* inputFile, const WCHAR* outputFile, int method, int compressionLevel, bool bDateTime )
  { 
	int err = -1;

    if ( ( inputFile != NULL ) && ( outputFile != NULL ) )
    {
        NSFile::CFileBinary oFile;

        zip_fileinfo zinfo;
        zinfo.external_fa = zinfo.internal_fa = 0;
        zinfo.dosDate = bDateTime ? oFile.GetDateTime(inputFile) : 0;

		if(oFile.OpenFile(inputFile))
		{
			DWORD dwSizeRead;
			BYTE* pData = new BYTE[oFile.GetFileSize()];
			if(oFile.ReadFile(pData, oFile.GetFileSize(), dwSizeRead))
			{
				zipFile zf = zipOpenHelp(outputFile);
                if (zf)
                {
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

                    err = zipOpenNewFileInZip( zf, zipFileNameA.c_str(), &zinfo, NULL, 0, NULL, 0, NULL, method, compressionLevel );
                    err = zipWriteInFileInZip( zf, pData, dwSizeRead );
                    err = zipCloseFileInZip( zf );
                    err = zipClose( zf, NULL );
                }
			}
			RELEASEARRAYOBJECTS(pData);
		}
	}

    return err;
  }

  /*========================================================================================================*/

  bool ClearDirectory( const WCHAR* dir, bool delDir )
  {
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

	return result;
  }

  /*========================================================================================================*/

  int UnzipToDir( const WCHAR* zipFile, const WCHAR* unzipDir, const OnProgressCallback* progress, const WCHAR* password, bool opt_extract_without_path, bool clearOutputDirectory )
  {
    unzFile uf = NULL;

    int err = -1;

    if ( ( zipFile != NULL ) && ( unzipDir != NULL ) )
    {
      int old = zlip_get_addition_flag();
      zlip_set_addition_flag(old | ZLIB_ADDON_FLAG_READ_ONLY);
	  uf = unzOpenHelp (zipFile);
      zlip_set_addition_flag(old);
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
	    int err1 = _wchdir( buffer );
#else
	    int err1 = chdir( buffer );
#endif

	    free( buffer );
	    buffer = NULL;
	  }
    }

    return err;
  }
  
  /*========================================================================================================*/
  
  int UncompressBytes( BYTE* destBuf, ULONG* destSize, const BYTE* sourceBuf, ULONG sourceSize )
  {
	int err = -1;

	err = uncompress( destBuf, destSize, sourceBuf, sourceSize );

	return err;
  }

  /*========================================================================================================*/

  int CompressBytes( BYTE* destBuf, ULONG* destSize, const BYTE* sourceBuf, ULONG sourceSize, SHORT level )
  {
	int err = -1;

	err = compress2( destBuf, destSize, sourceBuf, sourceSize, level );

	return err;
  }

  /*========================================================================================================*/

	bool IsArchive(const WCHAR* filename)
	{
	  unzFile uf = NULL;
	  bool isZIP = false;

	  if (( filename != NULL ))
		  uf = unzOpenHelp( filename );

	  if ( uf != NULL )
	  {
		  isZIP = true;
		  unzClose( uf );
	  }

	  return isZIP;
	}

	/*========================================================================================================*/

  bool IsFileExistInArchive(const WCHAR* zipFile, const WCHAR* filePathInZip)
  {
	  unzFile uf = NULL;
	  bool isIn = false;

	  if ( ( zipFile != NULL ) && ( filePathInZip != NULL ) )
		  uf = unzOpenHelp( zipFile );
	  if ( uf != NULL )
	  {		  
		  isIn = is_file_in_archive( uf, filePathInZip );
		  unzClose( uf );
	  }	 

	  return isIn;
  }

/*========================================================================================================*/

  bool LoadFileFromArchive(const WCHAR* zipFile, const WCHAR* filePathInZip, BYTE** fileInBytes, ULONG& nFileSize)
  {
	  unzFile uf = NULL;
	  bool isIn = false;

	  if ( ( zipFile != NULL ) && ( filePathInZip != NULL ) )
		  uf = unzOpenHelp( zipFile );

	  if ( uf != NULL )
	  {		  
		  isIn = get_file_in_archive( uf, filePathInZip, fileInBytes, nFileSize);
		  unzClose( uf );
	  }
	  
	  return isIn;
  }

/*========================================================================================================*/

	bool ExtractFiles(const wchar_t* zip_file_path, const ExtractedFileCallback& callback, void* pParam)
	{
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
		zipFile zip_file_handle = zipOpenHelp(zip_file_path);

		if(NULL != zip_file_handle)
		{
			BYTE* pData = NULL;
			long nSize;
			std::wstring in_zip_filename;
			while(callback(in_zip_filename, pData, nSize, pParam))
			{
				std::string in_zip_filenameA = codepage_issue_fixToOEM(in_zip_filename);
				if (ZIP_OK != zipOpenNewFileInZip( zip_file_handle, in_zip_filenameA.c_str(), NULL, NULL, 0, NULL, 0, NULL, Z_DEFLATED, compression_level ) ||
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

	bool GetFilesSize(const WCHAR*  zip_file_path, const std::wstring& searchPattern, ULONG64& nCommpressed, ULONG64& nUncommpressed)
	{
		nCommpressed = 0;
		nUncommpressed = 0;
		unzFile unzip_file_handle = unzOpenHelp(zip_file_path);
		if (unzip_file_handle != NULL)
		{
			//todo implement true pattern
			bool isAny = false;
			std::wstring searchExt;
			if (0 == searchPattern.length() || searchPattern == L"*")
			{
				isAny = true;
			}
			else if (searchPattern.length() > 1)
			{
				searchExt = searchPattern.substr(2);
			}
			do
			{
				char filename_inzip[256];
				unz_file_info file_info;
				unzGetCurrentFileInfo(unzip_file_handle, &file_info, filename_inzip, sizeof(filename_inzip), NULL, 0, NULL, 0);
				std::wstring filenameW = codepage_issue_fixFromOEM(filename_inzip);
				std::transform(filenameW.begin(), filenameW.end(), filenameW.begin(), ::tolower);
				if (isAny || NSFile::GetFileExtention(filenameW) == searchExt)
				{
					nCommpressed += file_info.compressed_size;
					nUncommpressed += file_info.uncompressed_size;
				}
				// else just skip the erroneous file
			} while (UNZ_OK == unzGoToNextFile(unzip_file_handle));

			unzClose (unzip_file_handle);
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

		  std::vector<std::wstring> aCurFiles = NSDirectory::GetFiles(szText);
		 
		  filescount += aCurFiles.size();
		  std::vector<std::wstring> aCurDirectories = NSDirectory::GetDirectories(szText);
		 
		  for(size_t i = 0; i < aCurDirectories.size(); ++i)
		  {
			  std::wstring sCurDirectory = aCurDirectories[i];
			  StringDeque.push_back( sCurDirectory );
		  }
        }
	  }

      return filescount;
	}
}
