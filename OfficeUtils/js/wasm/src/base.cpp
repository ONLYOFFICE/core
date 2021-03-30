#include "base.h"

void* Zlib_Malloc(unsigned int size)
{
    return ::malloc(size);
}
void Zlib_Free(void* p)
{
    if (p) ::free(p);
}

const    char** Zlib_GetPaths(unsigned char* buffer, int size)
{

}

unsigned char*  Zlib_GetFile (unzFile* file, const wchar_t* path)
{
    if (file == NULL)
        return NULL;
    bool isIn = false;
    BYTE* fileInBytes = NULL;
    unsigned long nFileSize;
    isIn = ZLibZipUtils::get_file_in_archive( file, path, &fileInBytes, nFileSize);
    return fileInBytes;
}
