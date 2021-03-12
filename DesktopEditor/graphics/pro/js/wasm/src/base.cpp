#include "base.h"
#include "../../cimage/CxImage/ximage.h"

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
    delete p;
}
bool CxImage_Decode(CxImage* p, unsigned char * buffer, unsigned int size, unsigned int imagetype)
{
    if (p) return p->Decode(buffer, size, imagetype);
    return false;
}
