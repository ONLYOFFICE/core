#include "base.h"
#include "ioapibuf.h"

#include <algorithm> // для std::min в get_file_in_archive

unzFile unzOpenHelp(BUFFER_IO* buffer)
{
    zlib_filefunc_def ffunc;
    fill_buffer_filefunc(&ffunc, buffer);
    return unzOpen2(NULL, &ffunc);
}

// begin from (ZipUtilsCP.cpp)
std::wstring codepage_issue_fixFromOEM( const char* sVal)
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
bool current_file_is_find(unzFile uf, const wchar_t *filename)
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
bool get_file(unzFile unzip_file_handle, BYTE* arr, uInt array_size)
{
    if(UNZ_OK == unzOpenCurrentFile(unzip_file_handle))
    {
        int data_read_size = unzReadCurrentFile(unzip_file_handle, arr, array_size);

        unzCloseCurrentFile(unzip_file_handle);
        return data_read_size == array_size ? true : false;
    }
    return false;
}
bool get_file_in_archive(unzFile unzip_file_handle, const wchar_t *filePathInZip, BYTE** fileInBytes, ULONG& nFileSize)
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
// end from (ZipUtilsCP.cpp)

void* Zlib_Malloc(unsigned int size)
{
    return ::malloc(size);
}
void Zlib_Free(void* p)
{
    if (p) ::free(p);
}

unsigned char** Zlib_GetPaths(unsigned char* buffer, int size)
{
    if (buffer == NULL)
        return NULL;
}

myFile*         Zlib_GetFile (char* buffer, int size, const wchar_t* path)
{
    BYTE** fileInBytes = new BYTE*;
    ULONG nFileSize;
    unzFile uf = NULL;
    bool isIn = false;
    BUFFER_IO* buf = new BUFFER_IO;
    if (buffer != NULL && path != NULL)
    {
        buf->buffer = buffer;
        buf->nSize = size;
        uf = unzOpenHelp(buf);
    }
    if (uf != NULL)
    {
        isIn = get_file_in_archive(uf, path, fileInBytes, nFileSize);
        unzClose(uf);
    }
    delete buf;
    myFile* res = new myFile;
    if (isIn)
    {
        res->data = *fileInBytes;
        res->size = nFileSize;
    }
    return res;
}
