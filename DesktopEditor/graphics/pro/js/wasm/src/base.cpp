#include "base.h"

void* CxImage_Malloc(unsigned int size)
{
    return ::malloc(size);
}
void CxImage_Free(void* p)
{
    if (p) ::free(p);
}
CxImage* CxImage_Create()
{
    return new CxImage();
}
void CxImage_Destroy(CxImage* p)
{
    if (p) delete p;
}
unsigned int CxImage_GetHeight(CxImage* p)
{
    if (p) return p->GetHeight();
    return -1;
}
unsigned int CxImage_GetWidth (CxImage* p)
{
    if (p) return p->GetWidth();
    return -1;
}
bool CxImage_Decode(CxImage* p, unsigned char* buffer, unsigned int size, unsigned int imagetype)
{
    if (p) return p->Decode(buffer, size, imagetype);
    return false;
}
bool CxImage_Encode(CxImage* p, unsigned char* &buffer, int &size, unsigned int imagetype)
{
    if (p) return p->Encode(buffer, size, imagetype);
    return false;
}
bool CxImage_Encode2RGBA(CxImage* p, unsigned char* &buffer, int &size, bool bFlipY)
{
    if (p) return p->Encode2RGBA(buffer, size, bFlipY);
    return false;
}
