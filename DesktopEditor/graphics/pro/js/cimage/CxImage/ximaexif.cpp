#include "ximajpg.h"

#if CXIMAGEJPG_SUPPORT_EXIF

CxImageJPG::CxExifInfo::CxExifInfo(EXIFINFO* info)
{
    if (info)
    {
		m_exifinfo = info;
		freeinfo = false;
    }
    else
    {
		m_exifinfo = new EXIFINFO;
        memset(m_exifinfo, 0, sizeof(EXIFINFO));
		freeinfo = true;
	}

	m_szLastError[0]='\0';
	ExifImageWidth = MotorolaOrder = 0;
	SectionsRead=0;
	memset(&Sections, 0, MAX_SECTIONS * sizeof(Section_t));
}
CxImageJPG::CxExifInfo::~CxExifInfo()
{
	for(int32_t i=0;i<MAX_SECTIONS;i++) if(Sections[i].Data) free(Sections[i].Data);
	if (freeinfo) delete m_exifinfo;
}

#endif 	// CXIMAGEJPG_SUPPORT_EXIF
