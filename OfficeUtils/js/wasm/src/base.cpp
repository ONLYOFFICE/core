#include "base.h"
#include "ioapibuf.h"

unzFile unzOpenHelp(BUFFER_IO* buffer)
{
    zlib_filefunc_def ffunc;
    fill_buffer_filefunc(&ffunc, buffer);
    return unzOpen2(NULL, &ffunc);
}

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
        isIn = ZLibZipUtils::get_file_in_archive(uf, path, fileInBytes, nFileSize);
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
