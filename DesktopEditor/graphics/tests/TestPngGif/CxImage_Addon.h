#ifndef CXIMAGE_ADDON_H
#define CXIMAGE_ADDON_H

#include "../../cximage/CxImage/ximage.h"
#include "../../Image.h"

class CxImage_Addon
{
public:
    CxImage_Addon();
    virtual ~CxImage_Addon();

    BYTE* GetData();
    size_t GetDataSize() const;
    const char* GetError();
    bool GetSpecializedGIFInfo(FILE* hFile, uint32_t imagetype);
    bool GetSpecializedGIFInfo(CxFile* hFile, uint32_t imagetype);
    void Transfer(CxImage_Addon& from);

protected:
    BYTE* image_data;
    size_t image_data_size;
    char error[256];
};

#endif // CXIMAGE_ADDON_H
