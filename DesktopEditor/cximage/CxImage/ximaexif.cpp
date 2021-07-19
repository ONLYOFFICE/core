﻿/*
 * File:	ximaexif.cpp
 * Purpose:	EXIF reader
 * 18/Aug/2002 Davide Pizzolato - www.xdp.it
 * CxImage version 7.0.2 07/Feb/2011
 * based on jhead-1.8 by Matthias Wandel <mwandel(at)rim(dot)net>
 */

#include "ximajpg.h"

#if CXIMAGEJPG_SUPPORT_EXIF

////////////////////////////////////////////////////////////////////////////////
CxImageJPG::CxExifInfo::CxExifInfo(EXIFINFO* info)
{
	if (info) {
		m_exifinfo = info;
		freeinfo = false;
	} else {
		m_exifinfo = new EXIFINFO;
		memset(m_exifinfo,0,sizeof(EXIFINFO));
		freeinfo = true;
	}

	m_szLastError[0]='\0';
	ExifImageWidth = MotorolaOrder = 0;
	SectionsRead=0;
	memset(&Sections, 0, MAX_SECTIONS * sizeof(Section_t));
}
////////////////////////////////////////////////////////////////////////////////
CxImageJPG::CxExifInfo::~CxExifInfo()
{
	for(int32_t i=0;i<MAX_SECTIONS;i++) if(Sections[i].Data) free(Sections[i].Data);
	if (freeinfo) delete m_exifinfo;
}
////////////////////////////////////////////////////////////////////////////////
bool CxImageJPG::CxExifInfo::DecodeExif(CxFile * hFile, int32_t nReadMode)
{
    int32_t a;
    int32_t HaveCom = FALSE;

    a = hFile->GetC();

    if (a != 0xff || hFile->GetC() != M_SOI){
        return FALSE;
    }

    for(;;){
        int32_t itemlen;
        int32_t marker = 0;
        int32_t ll,lh, got;
        uint8_t * Data;

        if (SectionsRead >= MAX_SECTIONS){
			strcpy(m_szLastError,"Too many sections in jpg file");
			return false;
        }

        for (a=0;a<7;a++){
            marker = hFile->GetC();
            if (marker != 0xff) break;

            if (a >= 6){
                printf("too many padding bytes\n");
                return false;
            }
        }

        if (marker == 0xff){
            // 0xff is legal padding, but if we get that many, something's wrong.
            strcpy(m_szLastError,"too many padding bytes!");
			return false;
        }

        Sections[SectionsRead].Type = marker;

        // Read the length of the section.
        lh = hFile->GetC();
        ll = hFile->GetC();

        itemlen = (lh << 8) | ll;

        if (itemlen < 2){
            strcpy(m_szLastError,"invalid marker");
			return false;
        }

        Sections[SectionsRead].Size = itemlen;

        Data = (uint8_t *)malloc(itemlen);
        if (Data == NULL){
            strcpy(m_szLastError,"Could not allocate memory");
			return false;
        }
        Sections[SectionsRead].Data = Data;

        // Store first two pre-read bytes.
        Data[0] = (uint8_t)lh;
        Data[1] = (uint8_t)ll;

        got = hFile->Read(Data+2, 1, itemlen-2); // Read the whole section.
        if (got != itemlen-2){
            strcpy(m_szLastError,"Premature end of file?");
			return false;
        }
        SectionsRead += 1;

        switch(marker){

            case M_SOS:   // stop before hitting compressed data 
                // If reading entire image is requested, read the rest of the data.
                if (nReadMode & EXIF_READ_IMAGE){
                    int32_t cp, ep, size;
                    // Determine how much file is left.
                    cp = hFile->Tell();
                    hFile->Seek(0, SEEK_END);
                    ep = hFile->Tell();
                    hFile->Seek(cp, SEEK_SET);

                    size = ep-cp;
                    Data = (uint8_t *)malloc(size);
                    if (Data == NULL){
                        strcpy(m_szLastError,"could not allocate data for entire image");
						return false;
                    }

                    got = hFile->Read(Data, 1, size);
                    if (got != size){
                        strcpy(m_szLastError,"could not read the rest of the image");
						return false;
                    }

                    Sections[SectionsRead].Data = Data;
                    Sections[SectionsRead].Size = size;
                    Sections[SectionsRead].Type = PSEUDO_IMAGE_MARKER;
                    SectionsRead ++;
                }
                return true;

            case M_EOI:   // in case it's a tables-only JPEG stream
                printf("No image in jpeg!\n");
                return FALSE;

            case M_COM: // Comment section
                if (HaveCom || ((nReadMode & EXIF_READ_EXIF) == 0)){
                    // Discard this section.
                    free(Sections[--SectionsRead].Data);
					Sections[SectionsRead].Data=0;
                }else{
                    process_COM(Data, itemlen);
                    HaveCom = TRUE;
                }
                break;

            case M_JFIF:
                // Regular jpegs always have this tag, exif images have the exif
                // marker instead, althogh ACDsee will write images with both markers.
                // this program will re-create this marker on absence of exif marker.
                // hence no need to keep the copy from the file.
                free(Sections[--SectionsRead].Data);
				Sections[SectionsRead].Data=0;
                break;

            case M_EXIF:
                // Seen files from some 'U-lead' software with Vivitar scanner
                // that uses marker 31 for non exif stuff.  Thus make sure 
                // it says 'Exif' in the section before treating it as exif.
                if ((nReadMode & EXIF_READ_EXIF) && itemlen >= 6 && memcmp(Data+2, "Exif", 4) == 0){
                    m_exifinfo->IsExif = process_EXIF((uint8_t *)Data+2, itemlen-2);
                }else{
                    // Discard this section.
                    free(Sections[--SectionsRead].Data);
					Sections[SectionsRead].Data=0;
                }
                break;

            case M_SOF0: 
            case M_SOF1: 
            case M_SOF2: 
            case M_SOF3: 
            case M_SOF5: 
            case M_SOF6: 
            case M_SOF7: 
            case M_SOF9: 
            case M_SOF10:
            case M_SOF11:
            case M_SOF13:
            case M_SOF14:
            case M_SOF15:
                if (itemlen < 8)
                    return false;
                process_SOFn(Data, marker);
                break;
            default:
                // Skip any other sections.
                //if (ShowTags) printf("Jpeg section marker 0x%02x size %d\n",marker, itemlen);
                break;
        }
    }
	return true;
}
////////////////////////////////////////////////////////////////////////////////
/*--------------------------------------------------------------------------
   Process a EXIF marker
   Describes all the drivel that most digital cameras include...
--------------------------------------------------------------------------*/
bool CxImageJPG::CxExifInfo::process_EXIF(uint8_t * CharBuf, uint32_t length)
{
    m_exifinfo->FlashUsed = 0; 
    /* If it's from a digicam, and it used flash, it says so. */
    m_exifinfo->Comments[0] = '\0';  /* Initial value - null string */

    if (length < 6)
        return false;

    ExifImageWidth = 0;

    {   /* Check the EXIF header component */
        static const uint8_t ExifHeader[] = "Exif\0\0";
        if (memcmp(CharBuf+0, ExifHeader,6)){
			strcpy(m_szLastError,"Incorrect Exif header");
			return false;
		}
    }

    if (length < 8)
        return false;

    if (memcmp(CharBuf+6,"II",2) == 0){
        MotorolaOrder = 0;
    }else{
        if (memcmp(CharBuf+6,"MM",2) == 0){
            MotorolaOrder = 1;
        }else{
            strcpy(m_szLastError,"Invalid Exif alignment marker.");
			return false;
        }
    }

    if (length < 14)
        return false;

    /* Check the next two values for correctness. */
    if (Get16u(CharBuf+8) != 0x2a){
        strcpy(m_szLastError,"Invalid Exif start (1)");
		return false;
    }

	int32_t FirstOffset = Get32u(CharBuf+10);
    /* <Richard Collins> 
	if (FirstOffset < 8 || FirstOffset > 16){
        // I used to ensure this was set to 8 (website I used indicated its 8)
        // but PENTAX Optio 230 has it set differently, and uses it as offset. (Sept 11 2002)
        strcpy(m_szLastError,"Suspicious offset of first IFD value");
		return false;
    }*/

    uint8_t * LastExifRefd = CharBuf;

#if 0
    /* First directory starts 16 bytes in.  Offsets start at 8 bytes in. */
    if (!ProcessExifDir(CharBuf+14, CharBuf+6, length-6, m_exifinfo, &LastExifRefd))
		return false;

	/* <Richard Collins> give a chance for a second directory */
	if (FirstOffset > 8) {
		if (!ProcessExifDir(CharBuf+14+FirstOffset-8, CharBuf+6, length-6, m_exifinfo, &LastExifRefd))
			return false;
	}
#else
    CSafeReader reader(CharBuf, length);
    /* First directory starts 16 bytes in.  Offsets start at 8 bytes in. */
    if (!ProcessExifDir2(reader.Offset(14), reader.Offset(6), length-6, m_exifinfo, &LastExifRefd))
        return false;

    /* <Richard Collins> give a chance for a second directory */
    if (FirstOffset > 8) {
        if (!ProcessExifDir2(reader.Offset(14+FirstOffset-8), reader.Offset(6), length-6, m_exifinfo, &LastExifRefd))
            return false;
    }
#endif

    /* This is how far the interesting (non thumbnail) part of the exif went. */
    // int32_t ExifSettingsLength = LastExifRefd - CharBuf;

    /* Compute the CCD width, in milimeters. */
    if (m_exifinfo->FocalplaneXRes != 0){
        m_exifinfo->CCDWidth = (float)(ExifImageWidth * m_exifinfo->FocalplaneUnits / m_exifinfo->FocalplaneXRes);
    }

	return true;
}
//--------------------------------------------------------------------------
// Get 16 bits motorola order (always) for jpeg header stuff.
//--------------------------------------------------------------------------
int32_t CxImageJPG::CxExifInfo::Get16m(void * Short)
{
    return (((uint8_t *)Short)[0] << 8) | ((uint8_t *)Short)[1];
}
////////////////////////////////////////////////////////////////////////////////
/*--------------------------------------------------------------------------
   Convert a 16 bit unsigned value from file's native byte order
--------------------------------------------------------------------------*/
int32_t CxImageJPG::CxExifInfo::Get16u(void * Short)
{
    if (MotorolaOrder){
        return (((uint8_t *)Short)[0] << 8) | ((uint8_t *)Short)[1];
    }else{
        return (((uint8_t *)Short)[1] << 8) | ((uint8_t *)Short)[0];
    }
}
////////////////////////////////////////////////////////////////////////////////
/*--------------------------------------------------------------------------
   Convert a 32 bit signed value from file's native byte order
--------------------------------------------------------------------------*/
int32_t CxImageJPG::CxExifInfo::Get32s(void * Long)
{
    if (MotorolaOrder){
        return  ((( char *)Long)[0] << 24) | (((uint8_t *)Long)[1] << 16)
              | (((uint8_t *)Long)[2] << 8 ) | (((uint8_t *)Long)[3] << 0 );
    }else{
        return  ((( char *)Long)[3] << 24) | (((uint8_t *)Long)[2] << 16)
              | (((uint8_t *)Long)[1] << 8 ) | (((uint8_t *)Long)[0] << 0 );
    }
}
////////////////////////////////////////////////////////////////////////////////
/*--------------------------------------------------------------------------
   Convert a 32 bit unsigned value from file's native byte order
--------------------------------------------------------------------------*/
uint32_t CxImageJPG::CxExifInfo::Get32u(void * Long)
{
    return (uint32_t)Get32s(Long) & 0xffffffff;
}
////////////////////////////////////////////////////////////////////////////////

/* Describes format descriptor */
static const int32_t BytesPerFormat[] = {0,1,1,2,4,8,1,1,2,4,8,4,8};
#define NUM_FORMATS 12

#define FMT_BYTE       1 
#define FMT_STRING     2
#define FMT_USHORT     3
#define FMT_ULONG      4
#define FMT_URATIONAL  5
#define FMT_SBYTE      6
#define FMT_UNDEFINED  7
#define FMT_SSHORT     8
#define FMT_SLONG      9
#define FMT_SRATIONAL 10
#define FMT_SINGLE    11
#define FMT_DOUBLE    12

/* Describes tag values */

#define TAG_EXIF_VERSION      0x9000
#define TAG_EXIF_OFFSET       0x8769
#define TAG_INTEROP_OFFSET    0xa005

#define TAG_MAKE              0x010F
#define TAG_MODEL             0x0110

#define TAG_ORIENTATION       0x0112
#define TAG_XRESOLUTION       0x011A
#define TAG_YRESOLUTION       0x011B
#define TAG_RESOLUTIONUNIT    0x0128

#define TAG_EXPOSURETIME      0x829A
#define TAG_FNUMBER           0x829D

#define TAG_SHUTTERSPEED      0x9201
#define TAG_APERTURE          0x9202
#define TAG_BRIGHTNESS        0x9203
#define TAG_MAXAPERTURE       0x9205
#define TAG_FOCALLENGTH       0x920A

#define TAG_DATETIME_ORIGINAL 0x9003
#define TAG_USERCOMMENT       0x9286

#define TAG_SUBJECT_DISTANCE  0x9206
#define TAG_FLASH             0x9209

#define TAG_FOCALPLANEXRES    0xa20E
#define TAG_FOCALPLANEYRES    0xa20F
#define TAG_FOCALPLANEUNITS   0xa210
#define TAG_EXIF_IMAGEWIDTH   0xA002
#define TAG_EXIF_IMAGELENGTH  0xA003

/* the following is added 05-jan-2001 vcs */
#define TAG_EXPOSURE_BIAS     0x9204
#define TAG_WHITEBALANCE      0x9208
#define TAG_METERING_MODE     0x9207
#define TAG_EXPOSURE_PROGRAM  0x8822
#define TAG_ISO_EQUIVALENT    0x8827
#define TAG_COMPRESSION_LEVEL 0x9102

#define TAG_THUMBNAIL_OFFSET  0x0201
#define TAG_THUMBNAIL_LENGTH  0x0202


/*--------------------------------------------------------------------------
   Process one of the nested EXIF directories.
--------------------------------------------------------------------------*/
bool CxImageJPG::CxExifInfo::ProcessExifDir(uint8_t * DirStart, uint8_t * OffsetBase, unsigned ExifLength,
                           EXIFINFO * const m_exifinfo, uint8_t ** const LastExifRefdP, int32_t NestingLevel)
{
    int32_t de;
    int32_t a;
    int32_t NumDirEntries;
    unsigned ThumbnailOffset = 0;
    unsigned ThumbnailSize = 0;

    if (NestingLevel > 4){
        strcpy(m_szLastError,"Maximum directory nesting exceeded (corrupt exif header)");
        return false;
    }

    NumDirEntries = Get16u(DirStart);

    if ((DirStart+2+NumDirEntries*12+2) > (OffsetBase+ExifLength)){
        strcpy(m_szLastError,"Illegally sized directory");
		return false;
    }

    for (de=0;de<NumDirEntries;de++){
        int32_t Tag, Format, Components;
        uint8_t * ValuePtr;
            /* This actually can point to a variety of things; it must be
               cast to other types when used.  But we use it as a byte-by-byte
               cursor, so we declare it as a pointer to a generic byte here.
            */
        int32_t ByteCount;
        uint8_t * DirEntry;
        DirEntry = DirStart+2+12*de;

        Tag = Get16u(DirEntry);
        Format = Get16u(DirEntry+2);
        Components = Get32u(DirEntry+4);

        if ((Format-1) >= NUM_FORMATS) {
            /* (-1) catches illegal zero case as unsigned underflows to positive large */
            strcpy(m_szLastError,"Illegal format code in EXIF dir");
			return false;
		}

        ByteCount = Components * BytesPerFormat[Format];

        if (ByteCount > 4){
            unsigned OffsetVal;
            OffsetVal = Get32u(DirEntry+8);
            /* If its bigger than 4 bytes, the dir entry contains an offset.*/
            if (OffsetVal+ByteCount > ExifLength){
                /* Bogus pointer offset and / or bytecount value */
                strcpy(m_szLastError,"Illegal pointer offset value in EXIF.");
				return false;
            }
            ValuePtr = OffsetBase+OffsetVal;
        }else{
            /* 4 bytes or less and value is in the dir entry itself */
            ValuePtr = DirEntry+8;
        }

        if (*LastExifRefdP < ValuePtr+ByteCount){
            /* Keep track of last byte in the exif header that was
               actually referenced.  That way, we know where the
               discardable thumbnail data begins.
            */
            *LastExifRefdP = ValuePtr+ByteCount;
        }

        /* Extract useful components of tag */
        switch(Tag){

            case TAG_MAKE:
                strncpy(m_exifinfo->CameraMake, (char*)ValuePtr, 31);
                break;

            case TAG_MODEL:
                strncpy(m_exifinfo->CameraModel, (char*)ValuePtr, 39);
                break;

			case TAG_EXIF_VERSION:
				strncpy(m_exifinfo->Version,(char*)ValuePtr, 4);
				break;

            case TAG_DATETIME_ORIGINAL:
                strncpy(m_exifinfo->DateTime, (char*)ValuePtr, 19);
                break;

            case TAG_USERCOMMENT:
                // Olympus has this padded with trailing spaces. Remove these first. 
                for (a=ByteCount;;){
                    a--;
                    if (((char*)ValuePtr)[a] == ' '){
                        ((char*)ValuePtr)[a] = '\0';
                    }else{
                        break;
                    }
                    if (a == 0) break;
                }

                /* Copy the comment */
                if (memcmp(ValuePtr, "ASCII",5) == 0){
                    for (a=5;a<10;a++){
                        char c;
                        c = ((char*)ValuePtr)[a];
                        if (c != '\0' && c != ' '){
                            strncpy(m_exifinfo->Comments, (char*)ValuePtr+a, 199);
                            break;
                        }
                    }
                    
                }else{
                    strncpy(m_exifinfo->Comments, (char*)ValuePtr, 199);
                }
                break;

            case TAG_FNUMBER:
                /* Simplest way of expressing aperture, so I trust it the most.
                   (overwrite previously computd value if there is one)
                   */
                m_exifinfo->ApertureFNumber = (float)ConvertAnyFormat(ValuePtr, Format);
                break;

            case TAG_APERTURE:
            case TAG_MAXAPERTURE:
                /* More relevant info always comes earlier, so only
                 use this field if we don't have appropriate aperture
                 information yet. 
                */
                if (m_exifinfo->ApertureFNumber == 0){
                    m_exifinfo->ApertureFNumber = (float)exp(ConvertAnyFormat(ValuePtr, Format)*log(2.0f)*0.5);
                }
                break;

			case TAG_BRIGHTNESS:
				m_exifinfo->Brightness = (float)ConvertAnyFormat(ValuePtr, Format);
				break;

            case TAG_FOCALLENGTH:
                /* Nice digital cameras actually save the focal length
                   as a function of how farthey are zoomed in. 
                */

                m_exifinfo->FocalLength = (float)ConvertAnyFormat(ValuePtr, Format);
                break;

            case TAG_SUBJECT_DISTANCE:
                /* Inidcates the distacne the autofocus camera is focused to.
                   Tends to be less accurate as distance increases.
                */
                m_exifinfo->Distance = (float)ConvertAnyFormat(ValuePtr, Format);
                break;

            case TAG_EXPOSURETIME:
                /* Simplest way of expressing exposure time, so I
                   trust it most.  (overwrite previously computd value
                   if there is one) 
                */
                m_exifinfo->ExposureTime = 
                    (float)ConvertAnyFormat(ValuePtr, Format);
                break;

            case TAG_SHUTTERSPEED:
                /* More complicated way of expressing exposure time,
                   so only use this value if we don't already have it
                   from somewhere else.  
                */
                if (m_exifinfo->ExposureTime == 0){
                    m_exifinfo->ExposureTime = (float)
                        (1/exp(ConvertAnyFormat(ValuePtr, Format)*log(2.0f)));
                }
                break;

            case TAG_FLASH:
                if ((int32_t)ConvertAnyFormat(ValuePtr, Format) & 7){
                    m_exifinfo->FlashUsed = 1;
                }else{
                    m_exifinfo->FlashUsed = 0;
                }
                break;

            case TAG_ORIENTATION:
                m_exifinfo->Orientation = (int32_t)ConvertAnyFormat(ValuePtr, Format);
                if (m_exifinfo->Orientation < 1 || m_exifinfo->Orientation > 8){
                    strcpy(m_szLastError,"Undefined rotation value");
                    m_exifinfo->Orientation = 0;
                }
                break;

            case TAG_EXIF_IMAGELENGTH:
            case TAG_EXIF_IMAGEWIDTH:
                /* Use largest of height and width to deal with images
                   that have been rotated to portrait format.  
                */
                a = (int32_t)ConvertAnyFormat(ValuePtr, Format);
                if (ExifImageWidth < a) ExifImageWidth = a;
                break;

            case TAG_FOCALPLANEXRES:
                m_exifinfo->FocalplaneXRes = (float)ConvertAnyFormat(ValuePtr, Format);
                break;

            case TAG_FOCALPLANEYRES:
                m_exifinfo->FocalplaneYRes = (float)ConvertAnyFormat(ValuePtr, Format);
                break;

			case TAG_RESOLUTIONUNIT:
                switch((int32_t)ConvertAnyFormat(ValuePtr, Format)){
                    case 1: m_exifinfo->ResolutionUnit = 1.0f; break; /* 1 inch */
                    case 2:	m_exifinfo->ResolutionUnit = 1.0f; break;
                    case 3: m_exifinfo->ResolutionUnit = 0.3937007874f;    break;  /* 1 centimeter*/
                    case 4: m_exifinfo->ResolutionUnit = 0.03937007874f;   break;  /* 1 millimeter*/
                    case 5: m_exifinfo->ResolutionUnit = 0.00003937007874f;  /* 1 micrometer*/
                }
                break;

            case TAG_FOCALPLANEUNITS:
                switch((int32_t)ConvertAnyFormat(ValuePtr, Format)){
                    case 1: m_exifinfo->FocalplaneUnits = 1.0f; break; /* 1 inch */
                    case 2:	m_exifinfo->FocalplaneUnits = 1.0f; break;
                    case 3: m_exifinfo->FocalplaneUnits = 0.3937007874f;    break;  /* 1 centimeter*/
                    case 4: m_exifinfo->FocalplaneUnits = 0.03937007874f;   break;  /* 1 millimeter*/
                    case 5: m_exifinfo->FocalplaneUnits = 0.00003937007874f;  /* 1 micrometer*/
                }
                break;

                // Remaining cases contributed by: Volker C. Schoech <schoech(at)gmx(dot)de>

            case TAG_EXPOSURE_BIAS:
                m_exifinfo->ExposureBias = (float) ConvertAnyFormat(ValuePtr, Format);
                break;

            case TAG_WHITEBALANCE:
                m_exifinfo->Whitebalance = (int32_t)ConvertAnyFormat(ValuePtr, Format);
                break;

            case TAG_METERING_MODE:
                m_exifinfo->MeteringMode = (int32_t)ConvertAnyFormat(ValuePtr, Format);
                break;

            case TAG_EXPOSURE_PROGRAM:
                m_exifinfo->ExposureProgram = (int32_t)ConvertAnyFormat(ValuePtr, Format);
                break;

            case TAG_ISO_EQUIVALENT:
                m_exifinfo->ISOequivalent = (int32_t)ConvertAnyFormat(ValuePtr, Format);
                if ( m_exifinfo->ISOequivalent < 50 ) m_exifinfo->ISOequivalent *= 200;
                break;

            case TAG_COMPRESSION_LEVEL:
                m_exifinfo->CompressionLevel = (int32_t)ConvertAnyFormat(ValuePtr, Format);
                break;

            case TAG_XRESOLUTION:
                m_exifinfo->Xresolution = (float)ConvertAnyFormat(ValuePtr, Format);
                break;
            case TAG_YRESOLUTION:
                m_exifinfo->Yresolution = (float)ConvertAnyFormat(ValuePtr, Format);
                break;

            case TAG_THUMBNAIL_OFFSET:
                ThumbnailOffset = (unsigned)ConvertAnyFormat(ValuePtr, Format);
                break;

            case TAG_THUMBNAIL_LENGTH:
                ThumbnailSize = (unsigned)ConvertAnyFormat(ValuePtr, Format);
                break;

        }

        if (Tag == TAG_EXIF_OFFSET || Tag == TAG_INTEROP_OFFSET){
            uint8_t * SubdirStart;
			unsigned Offset = Get32u(ValuePtr);
			if (Offset>8){
				SubdirStart = OffsetBase + Offset;
				if (SubdirStart < OffsetBase || 
					SubdirStart >= OffsetBase+ExifLength){
					strcpy(m_szLastError,"Illegal subdirectory link");
					return false;
				}
				ProcessExifDir(SubdirStart, OffsetBase, ExifLength, m_exifinfo, LastExifRefdP, NestingLevel+1);
			}
            continue;
        }
    }


    {
        /* In addition to linking to subdirectories via exif tags,
           there's also a potential link to another directory at the end
           of each directory.  This has got to be the result of a
           committee!  
        */
        uint8_t * SubdirStart;
        unsigned Offset;
        Offset = Get16u(DirStart+2+12*NumDirEntries);
        if (Offset){
            SubdirStart = OffsetBase + Offset;
            if (SubdirStart < OffsetBase 
                || SubdirStart >= OffsetBase+ExifLength){
                strcpy(m_szLastError,"Illegal subdirectory link");
				return false;
            }
            ProcessExifDir(SubdirStart, OffsetBase, ExifLength, m_exifinfo, LastExifRefdP, NestingLevel+1);
        }
    }


    if (ThumbnailSize && ThumbnailOffset){
        if (ThumbnailSize + ThumbnailOffset <= ExifLength){
            /* The thumbnail pointer appears to be valid.  Store it. */
            m_exifinfo->ThumbnailPointer = OffsetBase + ThumbnailOffset;
            m_exifinfo->ThumbnailSize = ThumbnailSize;
        }
    }

	return true;
}
bool CxImageJPG::CxExifInfo::ProcessExifDir2(CSafeReader DirStart, CSafeReader OffsetBase, unsigned ExifLength,
                           EXIFINFO * const m_exifinfo, uint8_t ** const LastExifRefdP, int32_t NestingLevel)
{
    int32_t de;
    int32_t a;
    int32_t NumDirEntries;
    unsigned ThumbnailOffset = 0;
    unsigned ThumbnailSize = 0;

    if (NestingLevel > 4){
        strcpy(m_szLastError,"Maximum directory nesting exceeded (corrupt exif header)");
        return false;
    }

    NumDirEntries = DirStart.Check(2) ? Get16u(DirStart.GetData(0)) : 0;

    if ((DirStart.GetData(2+NumDirEntries*12+2)) > (OffsetBase.GetData(ExifLength))){
        strcpy(m_szLastError,"Illegally sized directory");
        return false;
    }

    for (de=0;de<NumDirEntries;de++){
        int32_t Tag, Format, Components;
        CSafeReader ValuePtr;
            /* This actually can point to a variety of things; it must be
               cast to other types when used.  But we use it as a byte-by-byte
               cursor, so we declare it as a pointer to a generic byte here.
            */
        int32_t ByteCount;
        CSafeReader DirEntry = DirStart.Offset(2+12*de);

        Tag = DirEntry.Check(2) ? Get16u(DirEntry.GetData(0)) : 0;
        Format = DirEntry.Check(2, 2) ? Get16u(DirEntry.GetData(2)) : 0;
        Components = DirEntry.Check(4, 4) ? Get32u(DirEntry.GetData(4)) : 0;

        if ((Format-1) >= NUM_FORMATS) {
            /* (-1) catches illegal zero case as unsigned underflows to positive large */
            strcpy(m_szLastError,"Illegal format code in EXIF dir");
            return false;
        }

        ByteCount = Components * BytesPerFormat[Format];

        if (ByteCount > 4){
            unsigned OffsetVal;
            OffsetVal = DirEntry.Check(8, 4) ? Get32u(DirEntry.GetData(8)) : 0;
            /* If its bigger than 4 bytes, the dir entry contains an offset.*/
            if (OffsetVal+ByteCount > ExifLength){
                /* Bogus pointer offset and / or bytecount value */
                strcpy(m_szLastError,"Illegal pointer offset value in EXIF.");
                return false;
            }
            ValuePtr = OffsetBase.Offset(OffsetVal);
        }else{
            /* 4 bytes or less and value is in the dir entry itself */
            ValuePtr = DirEntry.Offset(8);
        }

        if (*LastExifRefdP < ValuePtr.GetData(ByteCount)){
            /* Keep track of last byte in the exif header that was
               actually referenced.  That way, we know where the
               discardable thumbnail data begins.
            */
            *LastExifRefdP = ValuePtr.GetData(ByteCount);
        }

        /* Extract useful components of tag */
        switch(Tag){

            case TAG_MAKE:
                if (ValuePtr.Check(31)) strncpy(m_exifinfo->CameraMake, (char*)ValuePtr.GetData(0), 31);
                break;

            case TAG_MODEL:
                if (ValuePtr.Check(39)) strncpy(m_exifinfo->CameraModel, (char*)ValuePtr.GetData(0), 39);
                break;

            case TAG_EXIF_VERSION:
                if (ValuePtr.Check(4)) strncpy(m_exifinfo->Version,(char*)ValuePtr.GetData(0), 4);
                break;

            case TAG_DATETIME_ORIGINAL:
                if (ValuePtr.Check(19)) strncpy(m_exifinfo->DateTime, (char*)ValuePtr.GetData(0), 19);
                break;

            case TAG_USERCOMMENT:
                // Olympus has this padded with trailing spaces. Remove these first.
                if (ValuePtr.Check(ByteCount))
                {
                    for (a=ByteCount;;){
                        a--;
                        if (*((char*)ValuePtr.GetData(a)) == ' '){
                            *((char*)ValuePtr.GetData(a)) = '\0';
                        }else{
                            break;
                        }
                        if (a == 0) break;
                    }
                }

                /* Copy the comment */
                if (ValuePtr.Check(5) && memcmp(ValuePtr.GetData(0), "ASCII",5) == 0){
                    for (a=5;a<10;a++){
                        char c;
                        c = *((char*)ValuePtr.GetData(a));
                        if (c != '\0' && c != ' '){
                            if (ValuePtr.Check(a, 199))
                                strncpy(m_exifinfo->Comments, (char*)ValuePtr.GetData(a), 199);
                            break;
                        }
                    }

                }else{
                    if (ValuePtr.Check(199))
                        strncpy(m_exifinfo->Comments, (char*)ValuePtr.GetData(0), 199);
                }
                break;

            case TAG_FNUMBER:
                /* Simplest way of expressing aperture, so I trust it the most.
                   (overwrite previously computd value if there is one)
                   */
                m_exifinfo->ApertureFNumber = (float)ConvertAnyFormat2(ValuePtr, Format);
                break;

            case TAG_APERTURE:
            case TAG_MAXAPERTURE:
                /* More relevant info always comes earlier, so only
                 use this field if we don't have appropriate aperture
                 information yet.
                */
                if (m_exifinfo->ApertureFNumber == 0){
                    m_exifinfo->ApertureFNumber = (float)exp(ConvertAnyFormat2(ValuePtr, Format)*log(2.0f)*0.5);
                }
                break;

            case TAG_BRIGHTNESS:
                m_exifinfo->Brightness = (float)ConvertAnyFormat2(ValuePtr, Format);
                break;

            case TAG_FOCALLENGTH:
                /* Nice digital cameras actually save the focal length
                   as a function of how farthey are zoomed in.
                */

                m_exifinfo->FocalLength = (float)ConvertAnyFormat2(ValuePtr, Format);
                break;

            case TAG_SUBJECT_DISTANCE:
                /* Inidcates the distacne the autofocus camera is focused to.
                   Tends to be less accurate as distance increases.
                */
                m_exifinfo->Distance = (float)ConvertAnyFormat2(ValuePtr, Format);
                break;

            case TAG_EXPOSURETIME:
                /* Simplest way of expressing exposure time, so I
                   trust it most.  (overwrite previously computd value
                   if there is one)
                */
                m_exifinfo->ExposureTime =
                    (float)ConvertAnyFormat2(ValuePtr, Format);
                break;

            case TAG_SHUTTERSPEED:
                /* More complicated way of expressing exposure time,
                   so only use this value if we don't already have it
                   from somewhere else.
                */
                if (m_exifinfo->ExposureTime == 0){
                    m_exifinfo->ExposureTime = (float)
                        (1/exp(ConvertAnyFormat2(ValuePtr, Format)*log(2.0f)));
                }
                break;

            case TAG_FLASH:
                if ((int32_t)ConvertAnyFormat2(ValuePtr, Format) & 7){
                    m_exifinfo->FlashUsed = 1;
                }else{
                    m_exifinfo->FlashUsed = 0;
                }
                break;

            case TAG_ORIENTATION:
                m_exifinfo->Orientation = (int32_t)ConvertAnyFormat2(ValuePtr, Format);
                if (m_exifinfo->Orientation < 1 || m_exifinfo->Orientation > 8){
                    strcpy(m_szLastError,"Undefined rotation value");
                    m_exifinfo->Orientation = 0;
                }
                break;

            case TAG_EXIF_IMAGELENGTH:
            case TAG_EXIF_IMAGEWIDTH:
                /* Use largest of height and width to deal with images
                   that have been rotated to portrait format.
                */
                a = (int32_t)ConvertAnyFormat2(ValuePtr, Format);
                if (ExifImageWidth < a) ExifImageWidth = a;
                break;

            case TAG_FOCALPLANEXRES:
                m_exifinfo->FocalplaneXRes = (float)ConvertAnyFormat2(ValuePtr, Format);
                break;

            case TAG_FOCALPLANEYRES:
                m_exifinfo->FocalplaneYRes = (float)ConvertAnyFormat2(ValuePtr, Format);
                break;

            case TAG_RESOLUTIONUNIT:
                switch((int32_t)ConvertAnyFormat2(ValuePtr, Format)){
                    case 1: m_exifinfo->ResolutionUnit = 1.0f; break; /* 1 inch */
                    case 2:	m_exifinfo->ResolutionUnit = 1.0f; break;
                    case 3: m_exifinfo->ResolutionUnit = 0.3937007874f;    break;  /* 1 centimeter*/
                    case 4: m_exifinfo->ResolutionUnit = 0.03937007874f;   break;  /* 1 millimeter*/
                    case 5: m_exifinfo->ResolutionUnit = 0.00003937007874f;  /* 1 micrometer*/
                }
                break;

            case TAG_FOCALPLANEUNITS:
                switch((int32_t)ConvertAnyFormat2(ValuePtr, Format)){
                    case 1: m_exifinfo->FocalplaneUnits = 1.0f; break; /* 1 inch */
                    case 2:	m_exifinfo->FocalplaneUnits = 1.0f; break;
                    case 3: m_exifinfo->FocalplaneUnits = 0.3937007874f;    break;  /* 1 centimeter*/
                    case 4: m_exifinfo->FocalplaneUnits = 0.03937007874f;   break;  /* 1 millimeter*/
                    case 5: m_exifinfo->FocalplaneUnits = 0.00003937007874f;  /* 1 micrometer*/
                }
                break;

                // Remaining cases contributed by: Volker C. Schoech <schoech(at)gmx(dot)de>

            case TAG_EXPOSURE_BIAS:
                m_exifinfo->ExposureBias = (float) ConvertAnyFormat2(ValuePtr, Format);
                break;

            case TAG_WHITEBALANCE:
                m_exifinfo->Whitebalance = (int32_t)ConvertAnyFormat2(ValuePtr, Format);
                break;

            case TAG_METERING_MODE:
                m_exifinfo->MeteringMode = (int32_t)ConvertAnyFormat2(ValuePtr, Format);
                break;

            case TAG_EXPOSURE_PROGRAM:
                m_exifinfo->ExposureProgram = (int32_t)ConvertAnyFormat2(ValuePtr, Format);
                break;

            case TAG_ISO_EQUIVALENT:
                m_exifinfo->ISOequivalent = (int32_t)ConvertAnyFormat2(ValuePtr, Format);
                if ( m_exifinfo->ISOequivalent < 50 ) m_exifinfo->ISOequivalent *= 200;
                break;

            case TAG_COMPRESSION_LEVEL:
                m_exifinfo->CompressionLevel = (int32_t)ConvertAnyFormat2(ValuePtr, Format);
                break;

            case TAG_XRESOLUTION:
                m_exifinfo->Xresolution = (float)ConvertAnyFormat2(ValuePtr, Format);
                break;
            case TAG_YRESOLUTION:
                m_exifinfo->Yresolution = (float)ConvertAnyFormat2(ValuePtr, Format);
                break;

            case TAG_THUMBNAIL_OFFSET:
                ThumbnailOffset = (unsigned)ConvertAnyFormat2(ValuePtr, Format);
                break;

            case TAG_THUMBNAIL_LENGTH:
                ThumbnailSize = (unsigned)ConvertAnyFormat2(ValuePtr, Format);
                break;

        }

        if (Tag == TAG_EXIF_OFFSET || Tag == TAG_INTEROP_OFFSET){
            unsigned Offset = ValuePtr.Check(4) ? Get32u(ValuePtr.GetData(0)) : 0;
            if (Offset>8){
                if (!OffsetBase.Check(Offset))
                {
                    strcpy(m_szLastError,"Illegal subdirectory link");
                    return false;
                }
                ProcessExifDir2(OffsetBase.Offset(Offset), OffsetBase, ExifLength, m_exifinfo, LastExifRefdP, NestingLevel+1);
            }
            continue;
        }
    }


    {
        /* In addition to linking to subdirectories via exif tags,
           there's also a potential link to another directory at the end
           of each directory.  This has got to be the result of a
           committee!
        */
        unsigned Offset;
        Offset = DirStart.Check(2+12*NumDirEntries, 2) ? Get16u(DirStart.GetData(2+12*NumDirEntries)) : 0;
        if (Offset){
            if (!OffsetBase.Check(Offset))
            {
                strcpy(m_szLastError,"Illegal subdirectory link");
                return false;
            }
            ProcessExifDir2(OffsetBase.Offset(Offset), OffsetBase, ExifLength, m_exifinfo, LastExifRefdP, NestingLevel+1);
        }
    }


    if (ThumbnailSize && ThumbnailOffset){
        if (ThumbnailSize + ThumbnailOffset <= ExifLength){
            /* The thumbnail pointer appears to be valid.  Store it. */
            m_exifinfo->ThumbnailPointer = OffsetBase.GetData(ThumbnailOffset);
            m_exifinfo->ThumbnailSize = ThumbnailSize;
        }
    }

    return true;
}
////////////////////////////////////////////////////////////////////////////////
/*--------------------------------------------------------------------------
   Evaluate number, be it int32_t, rational, or float from directory.
--------------------------------------------------------------------------*/
double CxImageJPG::CxExifInfo::ConvertAnyFormat(void * ValuePtr, int32_t Format)
{
    double Value;
    Value = 0;

    switch(Format){
        case FMT_SBYTE:     Value = *(signed char *)ValuePtr;  break;
        case FMT_BYTE:      Value = *(uint8_t *)ValuePtr;        break;

        case FMT_USHORT:    Value = Get16u(ValuePtr);          break;
        case FMT_ULONG:     Value = Get32u(ValuePtr);          break;

        case FMT_URATIONAL:
        case FMT_SRATIONAL: 
            {
                int32_t Num,Den;
                Num = Get32s(ValuePtr);
                Den = Get32s(4+(char *)ValuePtr);
                if (Den == 0){
                    Value = 0;
                }else{
                    Value = (double)Num/Den;
                }
                break;
            }

        case FMT_SSHORT:    Value = (int16_t)Get16u(ValuePtr);  break;
        case FMT_SLONG:     Value = Get32s(ValuePtr);                break;

        /* Not sure if this is correct (never seen float used in Exif format)
         */
        case FMT_SINGLE:    Value = (double)*(float *)ValuePtr;      break;
        case FMT_DOUBLE:    Value = *(double *)ValuePtr;             break;
    }
    return Value;
}
double CxImageJPG::CxExifInfo::ConvertAnyFormat2(CSafeReader& reader, int32_t Format)
{
    double Value;
    Value = 0;

    switch(Format){
        case FMT_SBYTE:
        case FMT_BYTE: if (!reader.Check(1)) {return Value;} break;
        case FMT_USHORT:
        case FMT_SSHORT: if (!reader.Check(2)) {return Value;} break;
        case FMT_ULONG:
        case FMT_SLONG: if (!reader.Check(4)) {return Value;} break;
        case FMT_URATIONAL:
        case FMT_SRATIONAL: if (!reader.Check(8)) {return Value;} break;
        case FMT_SINGLE: if (!reader.Check(sizeof(float))) {return Value;} break;
        case FMT_DOUBLE: if (!reader.Check(sizeof(double))) {return Value;} break;
    default:
        break;
    }

    void * ValuePtr = (void*)reader.GetData(0);
    switch(Format){
        case FMT_SBYTE:     Value = *(signed char *)ValuePtr;  break;
        case FMT_BYTE:      Value = *(uint8_t *)ValuePtr;        break;

        case FMT_USHORT:    Value = Get16u(ValuePtr);          break;
        case FMT_ULONG:     Value = Get32u(ValuePtr);          break;

        case FMT_URATIONAL:
        case FMT_SRATIONAL:
            {
                int32_t Num,Den;
                Num = Get32s(ValuePtr);
                Den = Get32s(4+(char *)ValuePtr);
                if (Den == 0){
                    Value = 0;
                }else{
                    Value = (double)Num/Den;
                }
                break;
            }

        case FMT_SSHORT:    Value = (int16_t)Get16u(ValuePtr);  break;
        case FMT_SLONG:     Value = Get32s(ValuePtr);                break;

        /* Not sure if this is correct (never seen float used in Exif format)
         */
        case FMT_SINGLE:    Value = (double)*(float *)ValuePtr;      break;
        case FMT_DOUBLE:    Value = *(double *)ValuePtr;             break;
    }
    return Value;
}
////////////////////////////////////////////////////////////////////////////////
void CxImageJPG::CxExifInfo::process_COM (const uint8_t * Data, int32_t length)
{
    int32_t ch;
    char Comment[MAX_COMMENT+1];
    int32_t nch;
    int32_t a;

    nch = 0;

    if (length > MAX_COMMENT) length = MAX_COMMENT; // Truncate if it won't fit in our structure.

    for (a=2;a<length;a++){
        ch = Data[a];

        if (ch == '\r' && (a < (length - 1) && Data[a+1] == '\n')) continue; // Remove cr followed by lf.

        if (isprint(ch) || ch == '\n' || ch == '\t'){
            Comment[nch++] = (char)ch;
        }else{
            Comment[nch++] = '?';
        }
    }

    Comment[nch] = '\0'; // Null terminate

    //if (ShowTags) printf("COM marker comment: %s\n",Comment);

    strcpy(m_exifinfo->Comments,Comment);
}
////////////////////////////////////////////////////////////////////////////////
void CxImageJPG::CxExifInfo::process_SOFn (const uint8_t * Data, int32_t marker)
{
    int32_t data_precision, num_components;

    data_precision = Data[2];
    m_exifinfo->Height = Get16m((void*)(Data+3));
    m_exifinfo->Width = Get16m((void*)(Data+5));
    num_components = Data[7];

    if (num_components == 3){
        m_exifinfo->IsColor = 1;
    }else{
        m_exifinfo->IsColor = 0;
    }

    m_exifinfo->Process = marker;

    //if (ShowTags) printf("JPEG image is %uw * %uh, %d color components, %d bits per sample\n",
    //               ImageInfo.Width, ImageInfo.Height, num_components, data_precision);
}
////////////////////////////////////////////////////////////////////////////////
/**
 * this will work only on a CxImageJPG object, if the image originally has valid EXIF data
 \verbatim
	CxImageJPG jpg;
	CxIOFile in,out;
	in.Open("D:\\exif_in.jpg","rb");
	out.Open("D:\\exif_out.jpg","w+b");
	jpg.Decode(&in);
	if (jpg.IsValid()){
		jpg.RotateLeft();
		jpg.Encode(&out);
	}
 \endverbatim
*/
bool CxImageJPG::CxExifInfo::EncodeExif(CxFile * hFile)
{
    int32_t a;

    if (FindSection(M_SOS)==NULL){
		strcpy(m_szLastError,"Can't write exif : didn't read all");
		return false;
    }

    // Initial static jpeg marker.
	hFile->PutC(0xff);
	hFile->PutC(0xd8);
    
    if (Sections[0].Type != M_EXIF && Sections[0].Type != M_JFIF){
        // The image must start with an exif or jfif marker.  If we threw those away, create one.
        static uint8_t JfifHead[18] = {
            0xff, M_JFIF,
            0x00, 0x10, 'J' , 'F' , 'I' , 'F' , 0x00, 0x01, 
            0x01, 0x01, 0x01, 0x2C, 0x01, 0x2C, 0x00, 0x00 
        };
        hFile->Write(JfifHead, 18, 1);
    }

    // Write all the misc sections
    for (a=0;a<SectionsRead-1;a++){
        hFile->PutC(0xff);
        hFile->PutC((uint8_t)(Sections[a].Type));
        hFile->Write(Sections[a].Data, Sections[a].Size, 1);
    }

    // Write the remaining image data.
    hFile->Write(Sections[a].Data, Sections[a].Size, 1);

	return true;
}
////////////////////////////////////////////////////////////////////////////////
void CxImageJPG::CxExifInfo::DiscardAllButExif()
{
    Section_t ExifKeeper;
    Section_t CommentKeeper;
    int32_t a;

    memset(&ExifKeeper, 0, sizeof(ExifKeeper));
    memset(&CommentKeeper, 0, sizeof(ExifKeeper));

    for (a=0;a<SectionsRead;a++){
        if (Sections[a].Type == M_EXIF && ExifKeeper.Type == 0){
            ExifKeeper = Sections[a];
        }else if (Sections[a].Type == M_COM && CommentKeeper.Type == 0){
            CommentKeeper = Sections[a];
        }else{
            free(Sections[a].Data);
			Sections[a].Data = 0;
        }
    }
    SectionsRead = 0;
    if (ExifKeeper.Type){
        Sections[SectionsRead++] = ExifKeeper;
    }
    if (CommentKeeper.Type){
        Sections[SectionsRead++] = CommentKeeper;
    }
}
////////////////////////////////////////////////////////////////////////////////
void* CxImageJPG::CxExifInfo::FindSection(int32_t SectionType)
{
    int32_t a;
    for (a=0;a<SectionsRead-1;a++){
        if (Sections[a].Type == SectionType){
            return &Sections[a];
        }
    }
    // Could not be found.
    return NULL;
}
////////////////////////////////////////////////////////////////////////////////
#endif 	// CXIMAGEJPG_SUPPORT_EXIF
