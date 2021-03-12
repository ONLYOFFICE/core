#ifndef __ximajpg_h
#define __ximajpg_h

#include "ximage.h"

#if CXIMAGE_SUPPORT_JPG

#define CXIMAGEJPG_SUPPORT_EXIF CXIMAGE_SUPPORT_EXIF

extern "C"
{
#include "../../../../../cximage/jpeg/jpeglib.h"
#include "../../../../../cximage/jpeg/jerror.h"
}

class CxImageJPG: public CxImage
{
public:
    CxImageJPG();
    ~CxImageJPG();

    bool Decode(CxFile * hFile);

#if CXIMAGEJPG_SUPPORT_EXIF
    class CxExifInfo
    {
        typedef struct tag_Section_t
        {
            uint8_t* Data;
            int32_t  Type;
            unsigned Size;
        } Section_t;

    public:
        EXIFINFO* m_exifinfo;
        char m_szLastError[256];
        CxExifInfo(EXIFINFO* info = NULL);
        ~CxExifInfo();
    protected:
        int32_t ExifImageWidth;
        int32_t MotorolaOrder;
        Section_t Sections[MAX_SECTIONS];
        int32_t SectionsRead;
        bool freeinfo;
    };

    CxExifInfo* m_exif;
#endif //CXIMAGEJPG_SUPPORT_EXIF
};

#endif // CXIMAGE_SUPPORT_JPG

#endif // __ximajpg_h
