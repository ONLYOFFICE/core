#include "ximajpg.h"

#if CXIMAGE_SUPPORT_JPG

#include "../../../../../cximage/jpeg/jmorecfg.h"

CxImageJPG::CxImageJPG() : CxImage(CXIMAGE_FORMAT_JPG)
{
#if CXIMAGEJPG_SUPPORT_EXIF
    m_exif = NULL;
    memset(&info.ExifInfo, 0, sizeof(EXIFINFO));
#endif
}
CxImageJPG::~CxImageJPG()
{
#if CXIMAGEJPG_SUPPORT_EXIF
    if (m_exif) delete m_exif;
#endif
}
#if CXIMAGE_SUPPORT_DECODE
bool CxImageJPG::Decode(CxFile* hFile)
{
    return false;
}
#endif //CXIMAGE_SUPPORT_DECODE

#endif // CXIMAGE_SUPPORT_JPG

