#include "base.h"

void* Zlib_Malloc(unsigned int size)
{
    return ::malloc(size);
}
void  Zlib_Free(void* p)
{
    if (p) ::free(p);
}

CZLib* Zlib_Create()
{
    return new CZLib();
}
CZLib* Zlib_Load(unsigned char* buffer, unsigned long size)
{
    return new CZLib(buffer, size);
}
void  Zlib_Destroy(CZLib* p)
{
    if (p) delete p;
}

unsigned char* Zlib_GetPathsInArchive(CZLib* p)
{
    return p ? p->GetPathsInArchive() : NULL;
}
unsigned char* Zlib_GetFileFromArchive(CZLib* p, const char* path)
{
    return p ? p->GetFileFromArchive(path) : NULL;
}
unsigned char* Zlib_CompressFiles(CZLib* p,   unsigned char* tree)
{
    return p ? p->CompressFiles(tree) : NULL;
}
