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

unsigned char*  Zlib_GetFile (std::string buffer, const wchar_t* path)
{
    char* name = tmpnam(NULL);
    ofstream fout(name);
    fout << buffer;
    fout.close();

    /*
    FILE* pFile = fopen(name, "w");
    fprintf(pFile, "%s", buffer);
    fclose(pFile);
    */

    BYTE** fileInBytes = new BYTE*;
    ULONG nFileSize;

    bool res = ZLibZipUtils::LoadFileFromArchive(std::wstring((wchar_t*)name).c_str(), path, fileInBytes, nFileSize);
    remove(name);
    return res ? *fileInBytes : NULL;
    /*
    unzFile uf = NULL;
    bool isIn = false;
    BYTE* destBuf = NULL;
    BYTE** fileInBytes = new BYTE*;
    ULONG* destSize = new ULONG; *destSize = 0;
    ULONG nFileSize;

    if (buffer != NULL && path != NULL)
        isIn = ZLibZipUtils::UncompressBytes(destBuf, destSize, buffer, size) == Z_OK;

    //if (isIn && destBuf != NULL)
        isIn = ZLibZipUtils::get_file_in_archive(buffer, path, fileInBytes, nFileSize);
    remove(name);
    return isIn ? *fileInBytes : NULL;
    */
}
