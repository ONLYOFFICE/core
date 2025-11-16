#ifndef CXIMAGEPNG_ADDON_H
#define CXIMAGEPNG_ADDON_H


#include "CxImage_Addon.h"
#include "../../Image.h"
#include "../../cximage/CxImage/ximapng.h"

class CxImagePNG_Addon: public CxImage_Addon
{
public:
    CxImagePNG_Addon(): CxImage_Addon() {}

    bool GetSpecializedGIFInfo(FILE* hFile);
    bool GetSpecializedGIFInfo(CxFile* hFile);
protected:
    static void user_read_data(png_structp png_ptr, png_bytep data, png_size_t length)
    {
        CxFile* hFile = (CxFile*)png_get_io_ptr(png_ptr);
        if (hFile == NULL || hFile->Read(data,1,length) != length) png_error(png_ptr, "Read Error");
    }

    static void user_error_fn(png_structp png_ptr,png_const_charp error_msg)
    {
        strncpy((char*)png_ptr->error_ptr,error_msg,255);
        longjmp(png_ptr->longjmp_buffer, 1);
    }
};


#endif // CXIMAGEPNG_ADDON_H
