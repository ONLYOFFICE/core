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
    Zlib* oRes   = new Zlib;
    oRes->buffer = new BUFFER_IO;
    oRes->buffer->buffer = buffer;
    oRes->buffer->nSize  = size;
    oRes->uf = unzOpenHelp(oRes->buffer);
    return oRes;
}
void  Zlib_Destroy(Zlib* p)
{
    if (p)
    {
        delete[] p->buffer;
        unzClose(p->uf);
        if (p->paths)
        {
            if (*p->paths) delete[] *p->paths;
            delete p->paths;
        }
        if (p->fileInBytes)
        {
            if (*p->fileInBytes) delete[] *p->fileInBytes;
            delete p->fileInBytes;
        }
        delete p;
    }
}

int Zlib_GetNumberPaths(Zlib* p)
{
    if (p && p->buffer && p->uf)
    {
        std::vector<std::wstring> res;
        do
        {
            unz_file_info file_info;
            unzGetCurrentFileInfo(p->uf, &file_info, NULL, 0, NULL, 0, NULL, 0);
            if (file_info.uncompressed_size != 0)
                res.push_back(get_filename_from_unzfile(p->uf));
        } while (UNZ_OK == unzGoToNextFile(p->uf));
        p->nNumPaths = res.size();
        if (p->nNumPaths > 0)
        {
            p->paths = new wchar_t*[p->nNumPaths];
            for (int i = 0; i < p->nNumPaths; i++)
            {
                p->paths[i] = new wchar_t[res[i].size()];
                for (size_t j = 0; j < res[i].size(); j++)
                    p->paths[i][j] = res[i][j];
            }
        }
        return p->nNumPaths;
    }
    return -1;
}
wchar_t** Zlib_GetPaths(Zlib* p)
{
    if (p && p->paths) return p->paths;
    return NULL;
}
int Zlib_GetSizeFileByPath(Zlib* p, const wchar_t* path)
{
    if (p && p->buffer && p->uf)
    {
        p->fileInBytes = new BYTE*;
        p->fileIsIn = get_file_in_archive(p->uf, path, p->fileInBytes, p->nFileSize);
        if (p->fileIsIn) return p->nFileSize;
    }
    return -1;
}
unsigned char* Zlib_GetLastFileByPath(Zlib* p)
{
    if (p && p->fileIsIn && p->fileInBytes) return *p->fileInBytes;
    return NULL;
}
