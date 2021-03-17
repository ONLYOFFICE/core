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
CBgraFrame* Raster_Load(unsigned char* buffer, unsigned int size)
{
    return NULL; // Open from buffer new CxImage(buffer, size);
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
bool Raster_Decode(CBgraFrame* p, unsigned char* buffer, unsigned int size)
{
    if (p) return false; // Open from buffer p->Decode(buffer, size, imagetype);
    return false;
}
unsigned char* Raster_Encode(CBgraFrame* p, unsigned int imagetype)
{
    unsigned char* buffer = NULL;
    int size;
    if (p) p->Encode(buffer, size, imagetype);
    return buffer;
}
unsigned char* Raster_GetRGBA(CBgraFrame* p)
{
    unsigned char* buffer = NULL;
    int size;
    if (p) p->Encode(buffer, size, 0 /* type RGBA array */);
    return buffer;
}
