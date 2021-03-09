#include "base.h"
#include "../../../../../raster/BgraFrame.h"

CBgraFrame* BgraFrame_Create()
{
    return new CBgraFrame();
}
void BgraFrame_Destroy(CBgraFrame* p)
{
    delete p;
}
