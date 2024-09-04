/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

unzFile unzOpenHelp(const wchar_t* filename);

namespace ZLibZipUtils
{
	// internal class for C unzFile struct
	class CunzFileWrapped
	{
	public:
		CunzFileWrapped() = default;
		~CunzFileWrapped();

		// NOTE: calls Close() first
		void Open(const wchar_t* filename);

		// NOTE: calls Close() first
		void Open(BYTE* data, size_t len);

		int Close();

		// NOTE: unzfile will be released after Close() or ~CunzFileWrapped()!
		// return value will be NULL, if Open(...) fails
		unzFile Get();

	private:
		unzFile m_unzFile {NULL};
		BUFFER_IO* m_buffer {NULL};
	};

	CunzFileWrapped::~CunzFileWrapped()
	{
		Close();
	}

	void CunzFileWrapped::Open(const wchar_t* filename)
	{
		Close();
		if(filename != NULL)
		{
			int old = zlip_get_addition_flag();
			zlip_set_addition_flag(old | ZLIB_ADDON_FLAG_READ_ONLY);
			m_unzFile = unzOpenHelp(filename);
			zlip_set_addition_flag(old);
		}
	}

	void CunzFileWrapped::Open(BYTE* data, size_t len)
	{
		Close();
		m_buffer = new BUFFER_IO;
		if ((data != NULL) && (len != 0))
		{
			int old = zlip_get_addition_flag();
			zlip_set_addition_flag(old | ZLIB_ADDON_FLAG_READ_ONLY);

			m_buffer->buffer = data;
			m_buffer->nSize  = len;

			zlib_filefunc_def ffunc;
			fill_buffer_filefunc(&ffunc, m_buffer);
			m_unzFile = unzOpen2(NULL, &ffunc);

			zlip_set_addition_flag(old);
		}
	}
	int CunzFileWrapped::Close()
	{
		int err = 0;
		if(m_unzFile)
			err = unzClose(m_unzFile);
		m_unzFile = NULL;

		RELEASEOBJECT(m_buffer)
		return err;
	}

	unzFile CunzFileWrapped::Get()
	{
		return m_unzFile;
	}

	zipFile zipOpenHelp(const wchar_t* filename)
	{
#if defined(_WIN32) || defined (_WIN64)
		zlib_filefunc64_def ffunc;
		fill_win32_filefunc64W(&ffunc);
		zipFile zf = zipOpen2_64(filename, APPEND_STATUS_CREATE, NULL, &ffunc);
#else
#ifdef _IOS
		std::string filePath = NSFile::NSIOS::GetFileSystemRepresentation(filename);
		zipFile zf = filePath.empty() ? NULL : zipOpen(filePath.c_str(), APPEND_STATUS_CREATE);
#else
		BYTE* pUtf8 = NULL;
		LONG lLen = 0;
		NSFile::CUtf8Converter::GetUtf8StringFromUnicode(filename, wcslen(filename), pUtf8, lLen, false);
		zipFile zf = zipOpen( (char*)pUtf8, APPEND_STATUS_CREATE );
		delete [] pUtf8;
#endif
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
#ifdef _IOS
		std::string filePath = NSFile::NSIOS::GetFileSystemRepresentation(filename);
		unzFile uf = filePath.empty() ? NULL : unzOpen(filePath.c_str());
#else
		BYTE* pUtf8 = NULL;
		LONG lLen = 0;
		NSFile::CUtf8Converter::GetUtf8StringFromUnicode(filename, wcslen(filename), pUtf8, lLen, false);
		unzFile uf = unzOpen( (char*)pUtf8 );
		delete [] pUtf8;
#endif
#endif
		return uf;
	}

	static std::wstring ascii_to_unicode(const char *src)
	{
		// TODO: check codepage of system (for "bad" archive)
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
	static int do_extract_currentfile( unzFile uf, const wchar_t* unzip_dir, const int* popt_extract_without_path, int* popt_overwrite, const char* password, bool is_replace_slash = false );
	static int do_extract( unzFile uf, const wchar_t* unzip_dir, int opt_extract_without_path, int opt_overwrite, const char* password, const OnProgressCallback* progress );

	static bool is_file_in_archive(unzFile uf, const wchar_t *filename);
	static bool current_file_is_find(unzFile uf, const wchar_t *filename);
	static bool get_file_in_archive(unzFile uf, const wchar_t *filePathInZip, BYTE** fileInBytes, ULONG& nFileSize);

	static unsigned int get_files_count( const WCHAR* dirname );

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

	static void replace_all_w(std::wstring& subject, const std::wstring& search, const std::wstring& replace)
	{
		size_t pos = 0;
		while ((pos = subject.find(search, pos)) != std::wstring::npos)
		{
			subject.replace(pos, search.length(), replace);
			pos += replace.length();
		}
	}

	static int do_extract_currentfile( unzFile uf, const wchar_t* unzip_dir, const int* popt_extract_without_path, int* popt_overwrite, const char* password, bool is_replcace_slash )
	{
		char filename_inzipA[4096];
		int err = UNZ_OK;
		unz_file_info file_info;

		err = unzGetCurrentFileInfo(uf,&file_info,filename_inzipA,sizeof(filename_inzipA),NULL,0,NULL,0);
		if (err!=UNZ_OK)
			return err;

		std::wstring filenameW;
		if (file_info.flag & 2048 /*11 bit*/)
			filenameW = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)filename_inzipA, strlen(filename_inzipA));
		else
			filenameW = codepage_issue_fixFromOEM(filename_inzipA);

#ifdef _WIN32
		for(std::wstring::size_type i = 0, len = filenameW.length(); i < len; ++i)
			if(filenameW[i] == L'/')
				filenameW[i] = L'\\';
#else
		if (is_replcace_slash)
			for (std::wstring::size_type i = 0, len = filenameW.length(); i < len; ++i)
				if(filenameW[i] == L'\\')
					filenameW[i] = L'/';

#endif

		std::wstring filenameW_withoutpath = L"";
		std::wstring::size_type posSeparator = filenameW.find_last_of(FILE_SEPARATOR_STR);
		if (posSeparator == std::wstring::npos)
			filenameW_withoutpath = filenameW;
		else if (posSeparator != (filenameW.length() - 1))
			filenameW_withoutpath = filenameW.substr(posSeparator + 1);

		std::wstring output             = std::wstring(unzip_dir) + FILE_SEPARATOR_STR + filenameW;
		std::wstring output_withoutpath = std::wstring(unzip_dir) + FILE_SEPARATOR_STR + filenameW_withoutpath;

		if (filenameW_withoutpath.empty())
		{
			if ((*popt_extract_without_path)==0)
			{
				NSDirectory::CreateDirectory(output);
			}
		}
		else
		{
			const wchar_t* write_filename;
			int skip=0;

			if ((*popt_extract_without_path)==0)
				write_filename = output.c_str();
			else
				write_filename = output_withoutpath.c_str();

			err = unzOpenCurrentFilePassword(uf, password);

			//-------------------------------------------------------------------------------------------------

			if (unzip_dir)
			{
				std::wstring current_path(unzip_dir);
				current_path += L"/";

				std::wstring filename_inzip(filenameW);

#ifdef _WIN32
				replace_all_w(current_path, L"\\", L"/");
				replace_all_w(filename_inzip, L"\\", L"/");
#endif

				std::wstring norm_path = NSSystemPath::NormalizePath(current_path + filename_inzip);
				std::wstring norm_current_path = NSSystemPath::NormalizePath(current_path);

				if (std::wstring::npos == norm_path.find(norm_current_path))
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
						(filenameW_withoutpath!=filenameW))
				{
					std::wstring folder = NSDirectory::GetFolderPath(write_filename);
					NSDirectory::CreateDirectories(folder);

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
				// ?
				while (err>0);
				if (err==0)
				{
					oFile.CloseFile();

					struct tm time;
					memset(&time, 0, sizeof(struct tm));
					time.tm_sec = file_info.tmu_date.tm_sec;
					time.tm_min = file_info.tmu_date.tm_min;
					time.tm_hour = file_info.tmu_date.tm_hour;
					time.tm_mday = file_info.tmu_date.tm_mday;
					time.tm_mon = file_info.tmu_date.tm_mon + 1;
					time.tm_year = file_info.tmu_date.tm_year;

					NSFile::CFileBinary::SetTime(write_filename, &time);
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

	static int do_extract( unzFile uf, const wchar_t* unzip_dir, int opt_extract_without_path, int opt_overwrite, const char* password, const OnProgressCallback* progress )
	{
		uLong i, number_extract = 0;
		unz_global_info gi;
		int err;
		FILE* fout=NULL;

		// [Content-Types.xml] check
		// if found - it is a office file, so we can replace '\' to '/' for non-windows
		// fixes bad zips
		const char* content_types = "[Content_Types].xml";
		bool is_office = UNZ_OK == unzLocateFile(uf, content_types, true);
		unzGoToFirstFile(uf);

		err = unzGetGlobalInfo (uf,&gi);

		for (i = 0; i < gi.number_entry; i++)
		{
			if (do_extract_currentfile(uf, unzip_dir, &opt_extract_without_path,
									   &opt_overwrite,
									   password, is_office) == UNZ_OK)
			{
				number_extract++;
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

			if ((i + 1) < gi.number_entry)
			{
				err = unzGoToNextFile(uf);
				if (err!=UNZ_OK)
				{
					break;
				}
			}
		}

		if (number_extract < 1)
		{
			err = -1;
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

	int oneZipFile(zipFile & zf, std::wstring & file_name, std::wstring & zip_file_name, int method, int compressionLevel, bool bDateTime)
	{
		int err = -1;
		NSFile::CFileBinary oFile;

		zip_fileinfo zinfo;
		zinfo.dosDate = 0;
		zinfo.external_fa = 0;
		zinfo.internal_fa = 0;

		if (bDateTime)
		{
			struct tm edited;
			bool ok = NSFile::CFileBinary::GetTime(file_name, &edited);
			if (ok)
			{
				zinfo.tmz_date.tm_sec = edited.tm_sec;
				zinfo.tmz_date.tm_min = edited.tm_min;
				zinfo.tmz_date.tm_hour = edited.tm_hour;
				zinfo.tmz_date.tm_mday = edited.tm_mday;
				zinfo.tmz_date.tm_mon = edited.tm_mon - 1;
				zinfo.tmz_date.tm_year = edited.tm_year;
			}
		}
		zip_fileinfo* zi_new = bDateTime ? &zinfo : NULL;

		if (oFile.OpenFile(file_name))
		{
			DWORD dwSizeRead;
			BYTE* pData = new BYTE[oFile.GetFileSize()];
			if (oFile.ReadFile(pData, oFile.GetFileSize(), dwSizeRead))
			{
				std::string zipFileNameA = codepage_issue_fixToOEM(zip_file_name);

				err = zipOpenNewFileInZip( zf, zipFileNameA.c_str(), zi_new, NULL, 0, NULL, 0, NULL, method, compressionLevel );
				err = zipWriteInFileInZip( zf, pData, dwSizeRead );
				err = zipCloseFileInZip( zf );
			}
			RELEASEARRAYOBJECTS(pData);
		}
		return err;
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
						else if (sDirName == L"_rels")
						{
							StringDeque.push_front(aCurDirectories[i]);
							zipDeque.push_front(zipDir + sDirName);
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

                    if (    std::wstring::npos != cFileName.find(L"mimetype") ||
                            std::wstring::npos != cFileName.find(L"[Content_Types]") ||
                            cFileName == L".rels")
                    {
						file = NSSystemPath::Combine(szText, cFileName);
						zipFileName = zipDir + cFileName;

						oneZipFile(zf, file, zipFileName, 0, compressionLevel, bDateTime);

						aCurFiles.erase(aCurFiles.begin() + i, aCurFiles.begin() + i + 1);
						break;
					}
				}

				for (size_t i = 0; i < aCurFiles.size(); ++i)
				{
					std::wstring cFileName = NSSystemPath::GetFileName(aCurFiles[i]);
					file = NSSystemPath::Combine(szText, cFileName);
					zipFileName = zipDir + cFileName;

					oneZipFile(zf, file, zipFileName, method, compressionLevel, bDateTime);

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

		if (( inputFile != NULL) && (outputFile != NULL))
		{
			NSFile::CFileBinary oFile;

			zip_fileinfo zinfo;
			zinfo.dosDate = 0;
			zinfo.external_fa = 0;
			zinfo.internal_fa = 0;

			if (bDateTime)
			{
				struct tm edited;
				bool ok = NSFile::CFileBinary::GetTime(inputFile, &edited);
				if (ok)
				{
					zinfo.tmz_date.tm_sec = edited.tm_sec;
					zinfo.tmz_date.tm_min = edited.tm_min;
					zinfo.tmz_date.tm_hour = edited.tm_hour;
					zinfo.tmz_date.tm_mday = edited.tm_mday;
					zinfo.tmz_date.tm_mon = edited.tm_mon - 1;
					zinfo.tmz_date.tm_year = edited.tm_year;
				}
			}
			zip_fileinfo* zi_new = bDateTime ? &zinfo : NULL;

			if (oFile.OpenFile(inputFile))
			{
				DWORD dwSizeRead;
				BYTE* pData = new BYTE[oFile.GetFileSize()];
				if(oFile.ReadFile(pData, oFile.GetFileSize(), dwSizeRead))
				{
					zipFile zf = zipOpenHelp(outputFile);
					if (zf)
					{
						wstring zipFileName = NSFile::GetFileName(inputFile);
						std::string zipFileNameA = codepage_issue_fixToOEM(zipFileName);

						err = zipOpenNewFileInZip( zf, zipFileNameA.c_str(), zi_new, NULL, 0, NULL, 0, NULL, method, compressionLevel );
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
		CunzFileWrapped ufw;
		ufw.Open(zipFile);
		int err = UnzipToDir(ufw.Get(), unzipDir, progress, password, opt_extract_without_path, clearOutputDirectory);

		// call Close() instead ~...() because of error code
		if(err == UNZ_OK)
			err = ufw.Close();
		return err;
	}

	/*========================================================================================================*/

	int UnzipToDir(BYTE* data, size_t len, const WCHAR* unzipDir, const OnProgressCallback* progress, const WCHAR* password, bool opt_extract_without_path, bool clearOutputDirectory )
	{
		CunzFileWrapped ufw;
		ufw.Open(data, len);
		int err = UnzipToDir(ufw.Get(), unzipDir, progress, password, opt_extract_without_path, clearOutputDirectory);

		// call Close() instead ~...() because of error code
		if(err == UNZ_OK)
			err = ufw.Close();
		return err;
	}

	/*========================================================================================================*/

	int UnzipToDir(unzFile uf, const WCHAR* unzipDir, const OnProgressCallback* progress, const WCHAR* password, bool opt_extract_without_path, bool clearOutputDirectory )
	{
		int err = -1;

		if ( uf != NULL && unzipDir != NULL )
		{
			if (NSDirectory::Exists(unzipDir))
				err = 0;

			if ( clearOutputDirectory )
			{
				ClearDirectory( unzipDir );
			}

			if ( err == 0 )
			{
				if(NULL != password)
				{
					std::string passwordA = codepage_issue_fixToOEM(password);
					err = do_extract( uf, unzipDir, opt_extract_without_path, 1, passwordA.c_str(), progress );
				}
				else
					err = do_extract( uf, unzipDir, opt_extract_without_path, 1, NULL, progress );
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
		CunzFileWrapped ufw;
		ufw.Open(filename);
		return ufw.Get() != NULL;
	}

	/*========================================================================================================*/

	bool IsArchive(BYTE* data, size_t len)
	{
		CunzFileWrapped ufw;
		ufw.Open(data, len);
		return ufw.Get() != NULL;
	}

	/*========================================================================================================*/

	bool IsFileExistInArchive(const WCHAR* zipFile, const WCHAR* filePathInZip)
	{
		CunzFileWrapped ufw;
		ufw.Open(zipFile);
		unzFile uf = ufw.Get();

		return filePathInZip != NULL && uf != NULL && is_file_in_archive(uf, filePathInZip);
	}

	/*========================================================================================================*/

	bool IsFileExistInArchive(BYTE* data, size_t len, const WCHAR* filePathInZip)
	{
		CunzFileWrapped ufw;
		ufw.Open(data, len);
		unzFile uf = ufw.Get();

		return filePathInZip != NULL && uf != NULL && is_file_in_archive(uf, filePathInZip);
	}

	/*========================================================================================================*/

	bool LoadFileFromArchive(const WCHAR* zipFile, const WCHAR* filePathInZip, BYTE** fileInBytes, ULONG& nFileSize)
	{
		CunzFileWrapped ufw;
		ufw.Open(zipFile);
		unzFile uf = ufw.Get();

		return filePathInZip != NULL && uf != NULL && get_file_in_archive( uf, filePathInZip, fileInBytes, nFileSize);
	}

	/*========================================================================================================*/

	bool LoadFileFromArchive(BYTE* data, size_t len, const WCHAR* filePathInZip, BYTE** fileInBytes, ULONG& nFileSize)
	{
		CunzFileWrapped ufw;
		ufw.Open(data, len);
		unzFile uf = ufw.Get();

		return filePathInZip != NULL && uf != NULL && get_file_in_archive( uf, filePathInZip, fileInBytes, nFileSize);
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
