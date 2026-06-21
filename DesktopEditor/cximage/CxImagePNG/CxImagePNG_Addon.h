#ifndef CXIMAGEPNG_ADDON_H
#define CXIMAGEPNG_ADDON_H


#include "CxImage_Addon.h"
#include "../../graphics/Image.h"
#include "../CxImage/ximapng.h"

class CxImagePNG_Addon: public CxImage_Addon, public CxImagePNG
{
public:
    CxImagePNG_Addon(): CxImage_Addon() {}

    bool GetSpecializedGIFInfo(FILE* hFile);
    bool GetSpecializedGIFInfo(CxFile* hFile);
};


#endif // CXIMAGEPNG_ADDON_H
