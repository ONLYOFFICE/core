#include "base.h"

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
static const std::wstring get_filename_from_unzfile(unzFile unzip_file_handle)
{
    char filename_inzip[256];
    int err = UNZ_OK;

    if (UNZ_OK == unzGetCurrentFileInfo(unzip_file_handle, NULL, filename_inzip, sizeof(filename_inzip), NULL, 0, NULL, 0))
        return codepage_issue_fixFromOEM(filename_inzip);

    return std::wstring(L"");
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

Zlib* Zlib_Create()
{
    return new Zlib();
}
Zlib* Zlib_Load(unsigned char* buffer, unsigned long size)
{
    Zlib* oRes = new Zlib;
    oRes->file  = NULL;
    oRes->paths = NULL;
    oRes->fileIsIn = false;
    oRes->fileSize  = -1;
    oRes->pathsSize = -1;
    oRes->buffer = buffer;
    oRes->size   = size;
    return oRes;
}
void  Zlib_Destroy(Zlib* p)
{
    if (p)
    {
        if (p->file) delete[] p->file;
        if (p->paths) delete[] p->paths;
        delete p;
    }
}

int Zlib_GetNumberPaths(Zlib* p)
{
    if (p && p->buffer)
    {
        BUFFER_IO* buf = new BUFFER_IO;
        buf->buffer = p->buffer;
        buf->nSize  = p->size;
        unzFile uf = unzOpenHelp(buf);
        std::vector<std::wstring> res;
        do
        {
            unz_file_info file_info;
            unzGetCurrentFileInfo(uf, &file_info, NULL, 0, NULL, 0, NULL, 0);
            if (file_info.uncompressed_size != 0)
                res.push_back(get_filename_from_unzfile(uf));
        } while (UNZ_OK == unzGoToNextFile(uf));
        p->pathsSize = res.size();
        if (p->pathsSize > 0)
        {
            p->paths = new std::wstring[p->pathsSize];
            for (int i = 0; i < p->pathsSize; i++)
                p->paths[i] = res[i];
        }
        unzClose(uf);
        if (buf) delete buf;
        return p->pathsSize;
    }
    return -1;
}
std::wstring* Zlib_GetPaths(Zlib* p)
{
    if (p && p->paths) return p->paths;
    return NULL;
}
int Zlib_GetSizeFileByPath(Zlib* p, const wchar_t* path)
{
    if (p && p->buffer)
    {
        BUFFER_IO* buf = new BUFFER_IO;
        buf->buffer = p->buffer;
        buf->nSize  = p->size;
        unzFile uf = unzOpenHelp(buf);
        if (p->file) delete p->file;
        p->file = new BYTE*;
        p->fileSize = 0;
        p->fileIsIn = get_file_in_archive(uf, path, p->file, p->fileSize);
        unzClose(uf);
        if (p->fileIsIn) return p->fileSize;
    }
    return -1;
}
unsigned char* Zlib_GetLastFileByPath(Zlib* p)
{
    if (p && p->fileIsIn && p->file) return *p->file;
    return NULL;
}
