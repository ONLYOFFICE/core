#include "base.h"

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

unsigned char*  Zlib_GetFile (void* uf, const wchar_t* path)
{
    bool isIn = false;
    BYTE* fileInBytes = NULL;
    unsigned long nFileSize;

    if (uf != NULL)
        isIn = ZLibZipUtils::get_file_in_archive( uf, path, &fileInBytes, nFileSize);
    return fileInBytes;
}
