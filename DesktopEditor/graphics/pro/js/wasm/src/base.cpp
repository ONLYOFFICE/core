#include "base.h"

void* Raster_Malloc(unsigned int size)
{
    return ::malloc(size);
}
void Raster_Free(void* p)
{
    if (p) ::free(p);
}
CBgraFrame* Raster_Create()
{
    return new CBgraFrame();
}
CBgraFrame* Raster_Load(unsigned char* buffer, int size)
{
    CBgraFrame* oRes = new CBgraFrame();
    oRes->Decode(buffer, size);
    return oRes;
}
void Raster_Destroy(CBgraFrame* p)
{
    if (p) delete p;
}
int  Raster_GetHeight(CBgraFrame* p)
{
    if (p) return p->get_Height();
    return -1;
}
int  Raster_GetWidth (CBgraFrame* p)
{
    if (p) return p->get_Width();
    return -1;
}
bool Raster_Decode(CBgraFrame* p, unsigned char* buffer, int size)
{
    if (p) return p->Decode(buffer, size);
    return false;
}
unsigned char* Raster_GetRGBA(CBgraFrame* p)
{
    unsigned char* buffer = NULL;
    if (p) buffer = p->get_Data();
    return buffer;
}
