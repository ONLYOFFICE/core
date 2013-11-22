/* 
 * The contents of this file are subject to the IBM Common Public
 * License Version 0.5 (the "License"); you may not use this file 
 * except in compliance with the License. You may obtain a copy of 
 * the License at http://oss.software.ibm.com/developerworks/
 * opensource/license-cpl.html
 * 
 * The Original Code is the Open Exif Toolkit,
 * released November 13, 2000.
 *
 * The Initial Developer of the Original Code is Eastman 
 * Kodak Company. Portions created by Kodak are 
 * Copyright (C) 2001 Eastman Kodak Company. All 
 * Rights Reserved. 
 *
 * Creation Date: 07/14/2001
 *
 * Original Author: 
 * George Sotak george.sotak@kodak.com 
 *
 * Contributor(s): 
 * Ricardo Rosario ricardo.rosario@kodak.com
 */ 


#if (defined _MSC_VER)
#pragma warning( disable : 4786 )
#endif

#include "ExifImageFile.h"
#include "ExifRawAppSeg.h"
#include "ExifApp0Seg.h"
#include "..\..\common\utils.h"
namespace Exif{
// Constuctor
ExifImageFile::ExifImageFile() 
  : ExifJpegImage(),
    mICCProf(NULL),
    mICCProfSize(0),
    mTmpImageFile(""),
    mImageModifiedOrCreated(false)
{
    mAppSegManager = new ExifAppSegManager() ;
    setupDefaults() ;
}

// Destructor
ExifImageFile::~ExifImageFile()
{
    close() ;
    deleteDefaults() ;
    if ( mAppSegManager )
        delete mAppSegManager ;
}

ExifStatus ExifImageFile::isExifFile(char * fileName) 
{
	return isExifFile((wchar_t*)(AVS::Utils::__StoW(fileName).c_str()));
}

// Opens the file specified as the argument and determines whether it is an
// Exif file.
ExifStatus ExifImageFile::isExifFile(wchar_t * fileName) 
{
    ExifStatus returnValue = EXIF_OK ;
    FILE *fp = NULL;
	
	if ((_wfopen_s(&fp, fileName, L"rb") != 0) || (NULL == fp))
        returnValue = EXIF_FILE_OPEN_ERROR ;
    else
    {
        returnValue = ExifImageFile::isExifFile(fp);
        fclose(fp);
    }

    return returnValue ;
}

// Determines whether the ExifIO is an Exif file
ExifStatus ExifImageFile::isExifFile(ExifIO * exifio)
{
    ExifStatus returnValue = EXIF_OK ;
    if ( exifio->isMapped() )
        returnValue = EXIF_ERROR ; // NOTE: Not currently implemented
    else
    {
        exifio->seek(0, SEEK_SET) ;
        returnValue = ExifImageFile::isExifFile( exifio->getFilePtr() );
    }
    return returnValue ;
}

// Determines whether the FILE* passed in points to an Exif file
ExifStatus ExifImageFile::isExifFile(FILE * fp)
{
    ExifStatus status = EXIF_ERROR;
    unsigned int readSize = 0;
    unsigned char tbuf[12];
    unsigned short app0Length;
    
    fseek(fp, 0, SEEK_SET);
    if( (readSize = fread(tbuf,12,1,fp)) == 1 ) 
    {

        // look for SOI
        if (tbuf[0] == 0xff && tbuf[1] == 0xd8) 
        {

            // check for APP0 Marker
            if(tbuf[2] == 0xff && tbuf[3] == 0xe0) 
            {    // APP0 found

                // look for "JFIF"
                if(tbuf[6] != 0x4A || tbuf[7] != 0x46 ||	
                   tbuf[8] != 0x49 || tbuf[9] != 0x46 ||
                   tbuf[10] != 0x00 || tbuf[11] != 0x00) 
                {
                    app0Length = (uint16)((int)tbuf[4] << 8 );
                    app0Length |= ((int)tbuf[5] );
                    fseek(fp, app0Length + 2, SEEK_SET);
                    readSize = fread(tbuf,12,1,fp); 
                }
                else
                    status = EXIF_OK ;
            }

            if ( ( status != EXIF_OK ) && ( readSize == 1 ) )
            {
                // look for the APP1 marker and the "Exif" Identifier
                if(tbuf[2] == 0xff && tbuf[3] == 0xe1 &&
                   tbuf[6] == 0x45 && tbuf[7] == 0x78 &&	
                   tbuf[8] == 0x69 && tbuf[9] == 0x66 &&
                   tbuf[10] == 0x00 && tbuf[11] == 0x00) 
                {
                    status = EXIF_OK ;
                }
            }
        }
    }
    return status ;
}

// Inserts the default entries into mDefaultEntries. Also registers the the
// standard IFD dir (IFD pointer) tags
void ExifImageFile::setupDefaults( void )
{
    if ( mDefaultEntries.size() != 0 )
        return ;
  
    // For App1
    ExifIFD::registerDirTag(330);          // Sub IFD (0x14A)
    ExifIFD::registerDirTag(EXIFTAG_EXIFIFDPOINTER);        // EXIF IFD (0x8769)
    ExifIFD::registerDirTag(EXIFTAG_GPSINFO);        // GPS IFD (0x8825)
    ExifIFD::registerDirTag(EXIFTAG_EXIFINTEROPIFDPOINTER);        // Interoperability IFD (0xA005)
    
    // For App3
    ExifIFD::registerDirTag(EXIFTAG_SPECIALEFFECTSIFD); // Special Effects IFD
    ExifIFD::registerDirTag(EXIFTAG_BORDERSIFD); // Borders IFD
    ExifIFD::registerDirTag(EXIFTAG_FACESINFO); // Face IFD
  
    // Bits per Sample
    std::vector<uint16> bitsPerSample(3, 8) ;
    ExifTagEntry *tagEntry = new ExifTagEntryT<std::vector<uint16> >
        ( EXIFTAG_BITSPERSAMPLE, EXIF_SHORT, 3, bitsPerSample );
    mDefaultEntries.insert( ExifIFD::EntryMap::value_type
        ( tagEntry->getTagNum(), tagEntry ) );
  
    // Orientation - CONDITIONALLY MANDATORY, REQUIRED IF SUPPORTED BY HARDWARE
    tagEntry = new ExifTagEntryT<uint16>
        ( EXIFTAG_ORIENTATION, EXIF_SHORT, 1, 1 );
    mDefaultEntries.insert( ExifIFD::EntryMap::value_type
        (tagEntry->getTagNum(), tagEntry ) );

    // Samples per Pixel - MANDATORY FOR UNCOMPRESSED -- NOT RECORDED for
    // COMPRESSED
    tagEntry = new ExifTagEntryT<uint16>
        ( EXIFTAG_SAMPLESPERPIXEL, EXIF_SHORT, 1, 3 );
    mDefaultEntries.insert( ExifIFD::EntryMap::value_type
        (tagEntry->getTagNum(), tagEntry ) );

    // Rows per Strip - MANDATORY FOR UNCOMPRESSED -- NOT RECORDED for
    // COMPRESSED
    tagEntry = new ExifTagEntryT<exif_uint32>
        ( EXIFTAG_ROWSPERSTRIP, EXIF_LONG, 1, LONG_MAX );
    mDefaultEntries.insert( ExifIFD::EntryMap::value_type
        (tagEntry->getTagNum(), tagEntry ) );

    // X Resolution - MANDATORY FOR UNCOMPRESSED and COMPRESSED
    tagEntry = new ExifTagEntryT<float>
        ( EXIFTAG_XRESOLUTION, EXIF_RATIONAL, 1, 72 );
    mDefaultEntries.insert( ExifIFD::EntryMap::value_type
        (tagEntry->getTagNum(), tagEntry ) );

    // Y Resolution - MANDATORY FOR UNCOMPRESSED and COMPRESSED
    tagEntry = new ExifTagEntryT<float>
        ( EXIFTAG_YRESOLUTION, EXIF_RATIONAL, 1, 72 );
    mDefaultEntries.insert( ExifIFD::EntryMap::value_type
        (tagEntry->getTagNum(), tagEntry ) );

    // Planar Configuration - MANDATORY FOR UNCOMPRESSED
    tagEntry = new ExifTagEntryT<uint16>
        ( EXIFTAG_PLANARCONFIG, EXIF_SHORT, 1, 1 );
    mDefaultEntries.insert( ExifIFD::EntryMap::value_type
        (tagEntry->getTagNum(), tagEntry ) );

    // Resolution Unit - MANDATORY FOR UNCOMPRESSED and COMPRESSED
    tagEntry = new ExifTagEntryT<uint16>
        ( EXIFTAG_RESOLUTIONUNIT, EXIF_SHORT, 1, 2 );
    mDefaultEntries.insert( ExifIFD::EntryMap::value_type
        (tagEntry->getTagNum(), tagEntry ) );

    // YCbCr Positioning - MANDATORY for UNCOMPRESSES YCC and COMPRESSED
    tagEntry = new ExifTagEntryT<uint16>
        ( EXIFTAG_YCBCRPOSITIONING, EXIF_SHORT, 1, 1 );
    mDefaultEntries.insert( ExifIFD::EntryMap::value_type
        (tagEntry->getTagNum(), tagEntry ) );

    // Exif Version - Exif IFD MANDATORY for UNCOMPRESSES and COMPRESSED
    std::vector<int8> byteVec ;
    byteVec.push_back( '0' ) ;
    byteVec.push_back( '2' ) ;
    byteVec.push_back( '1' ) ;
    byteVec.push_back( '0' ) ;
    tagEntry = new ExifTagEntryT<std::vector<int8> >
        ( EXIFTAG_EXIFVERSION, EXIF_UNDEFINED, byteVec.size(), byteVec );
    mDefaultEntries.insert( ExifIFD::EntryMap::value_type
        (tagEntry->getTagNum(), tagEntry ) );

    // FlashPix Version - Exif IFD MANDATORY for UNCOMPRESSES and COMPRESSED
    byteVec.erase( byteVec.begin(), byteVec.end() ) ;
    byteVec.push_back( '0' ) ;
    byteVec.push_back( '1' ) ;
    byteVec.push_back( '0' ) ;
    byteVec.push_back( '0' ) ;
    tagEntry = new ExifTagEntryT<std::vector<int8> >
        ( EXIFTAG_FLASHPIXVERSION, EXIF_UNDEFINED, byteVec.size(), byteVec );
    mDefaultEntries.insert( ExifIFD::EntryMap::value_type
        (tagEntry->getTagNum(), tagEntry ) );

    // Components Configuration - Exif IFD MANDATORY for UNCOMPRESSES and
    // COMPRESSED
    byteVec.erase( byteVec.begin(), byteVec.end() ) ;
    byteVec.push_back( 1 ) ;
    byteVec.push_back( 2 ) ;
    byteVec.push_back( 3 ) ;
    byteVec.push_back( 0 ) ;
    tagEntry = new ExifTagEntryT<std::vector<int8> >
        ( EXIFTAG_COMPONENTSCONFIGURATION, EXIF_UNDEFINED, byteVec.size(),
        byteVec );
    mDefaultEntries.insert( ExifIFD::EntryMap::value_type
        (tagEntry->getTagNum(), tagEntry ) );

    // Color Space - Exif IFD MANDATORY for UNCOMPRESSES and COMPRESSED
    tagEntry = new ExifTagEntryT<uint16>
        ( EXIFTAG_COLORSPACE, EXIF_SHORT, 1, 1 );
    mDefaultEntries.insert( ExifIFD::EntryMap::value_type
        (tagEntry->getTagNum(), tagEntry ) );

    // Pixel X Dimension - Exif IFD MANDATORY for COMPRESSED
    tagEntry = new ExifTagEntryT<exif_uint32>
        ( EXIFTAG_PIXELXDIMENSION, EXIF_LONG, 1, 0 );
    mDefaultEntries.insert( ExifIFD::EntryMap::value_type
        (tagEntry->getTagNum(), tagEntry ) );

    // Pixel Y Dimension - Exif IFD MANDATORY for COMPRESSED
    tagEntry = new ExifTagEntryT<exif_uint32>
        ( EXIFTAG_PIXELYDIMENSION, EXIF_LONG, 1, 0 );
    mDefaultEntries.insert( ExifIFD::EntryMap::value_type
        (tagEntry->getTagNum(), tagEntry ) );

    return ;
}

// Deletes the entries in mDefaultEntries
void ExifImageFile::deleteDefaults( void )
{
    ExifIFD::EntryIter crntIt = mDefaultEntries.begin() ;
    ExifIFD::EntryIter endIt = mDefaultEntries.end() ;
    while( crntIt != endIt )
    {
        delete (*crntIt).second ;
        crntIt++ ;
    }
    mDefaultEntries.clear() ;
}

ExifStatus ExifImageFile::close( void )
{
    ExifStatus status = EXIF_OK ;

    if (  mExifio && (mExifio->mode()&O_RDWR || mExifio->mode()&O_CREAT))
    {
        status = writeImage();
    }

    ExifOpenFile::close() ;


    // ***** Clean up *****

    // Delete all the app segs
    if ( mAppSegManager )
    {
        mAppSegManager->clear() ;
    }

    if ( mExifio )
    {
        delete mExifio ;
        mExifio = NULL ;
    }
        
    return status ;
}


ExifStatus ExifImageFile::initAfterOpen( const char* cmode )
{
    /*
     * Default is to return data MSB2LSB
     */
    mExifio->flags() |= FILLORDER_MSB2LSB ;

  /*
   * Process library-specific flags in the open mode std::string.
   * The following flags may be used to control intrinsic library
   * behaviour that may or may not be desirable (usually for
   * compatibility with some application that claims to support
   * EXIF but only supports some braindead idea of what the
   * vendor thinks EXIF is):
   *
   * 'L'        read/write information using LSB2MSB bit order
   * 'B'        read/write information using MSB2LSB bit order
   * 'H'        read/write information using host bit order
   *
   * The 'L', 'B', and 'H' flags are intended for applications
   * that can optimize operations on data by using a particular
   * bit order.  By default the library returns data in MSB2LSB
   * bit order for compatibiltiy with older versions of this
   * library.  Returning data in the bit order of the native cpu
   * makes the most sense but also requires applications to check
   * the value of the FillOrder tag; something they probably do
   * not do right now.
   *
   */
    const char* cp ;
    for(cp = cmode; *cp; cp++)
    {
        switch (*cp) 
        {
            case 'B':
                mExifio->flags() = (mExifio->flags() &~ EXIF_FILLORDER) |
                    FILLORDER_MSB2LSB;
                break;
            case 'L':
                mExifio->flags() = (mExifio->flags() &~ EXIF_FILLORDER) |
                    FILLORDER_LSB2MSB;
                break;
            case 'H':
                mExifio->flags() = (mExifio->flags() &~ EXIF_FILLORDER) |
                    HOST_FILLORDER;
                break;
        }
    }

    mAppSegManager->clear() ;

    // Check the endianness of the current architechture
    // HOST_BIGENDIAN is a configuration setting, true
    // if native cpu byte order is big endian, false if little
    bool bigendian = HOST_BIGENDIAN ;
    initEndian( bigendian );
    ExifStatus status ;
    
    // Setup for writing new 
    if( mExifio->mode() & O_CREAT )
    {
        status = initAppSegs( mExifio ) ;
        /* END OF Setting up a new file for writing */
    }
    else if( (mExifio->mode() & O_RDWR) && (ExifImageFile::isExifFile(mExifio)
        != EXIF_OK) && (ExifJpegImage::isJpegFile( mExifio ) == EXIF_OK) )
    {
        // If we have a plain Jpeg non Exif file (JFIF) opened in ReadWrite
        // mode, then we need to initialize the app segments for it as if we
        // were creating a new file. To do this we create a temp ExifIO* in
        // "w" mode and pass it to initAppSegs so that it behaves as if we
        // were creating a new exif file. Nothing is written to this temp
        // ExifIO.
        char buf[20];
        ExifIO * tmpExifIO = new ExifIO(status,buf,20, "w");
        if( status == EXIF_OK )
        {
            tmpExifIO->flags() = mExifio->flags();
            status = initAppSegs( tmpExifIO );
            tmpExifIO->close();
            delete tmpExifIO;
            if( status == EXIF_OK )
            {        
                status = ExifJpegImage::initAfterOpen( cmode ) ;
     
                /* The following code checks the real height and width and 
                   inserts it into the appropriate tags (in case they are 
                   incorrect). This forces a conversion of the Tiff App Seg 
                   from Raw form... Do we want to do this??? rlr */
                if ( status == EXIF_OK )
                {
                    // Set the value of EXIFTAG_PIXELXDIMENSION and 
                    // EXIFTAG_PIXELYDIMENSION to match the value in 
                    // the Jpeg Marker.
                    ExifImageInfo imgInfo ;
                    if ( getImageInfo( imgInfo ) == EXIF_OK )
                    {
                        ExifTagEntry * tagEntry = new ExifTagEntryT<exif_uint32>
                            ( EXIFTAG_PIXELXDIMENSION, EXIF_LONG, 1, 
                              imgInfo.width );
                        setGenericTag(*tagEntry,EXIF_APP1_EXIFIFD);
                        delete tagEntry ;
                        tagEntry = new ExifTagEntryT<exif_uint32>
                            ( EXIFTAG_PIXELYDIMENSION, EXIF_LONG, 1, 
                              imgInfo.height );
                        setGenericTag(*tagEntry,EXIF_APP1_EXIFIFD);
                        delete tagEntry ;
                    }
                }  
            }
        }          

    }
    else
    {
//         readSOI(mExifio) ;
// 
//         if( (status = readAppSegs(mExifio)) == EXIF_OK )
//             status = ExifJpegImage::initAfterOpen( cmode ) ;
        status = readImage();
    }

    return status;
}


ExifTagEntry* ExifImageFile::getDefaultTag(exiftag_t tagNum ) const
{
    ExifIFD::ConstEntryIter entryIter;

    if ( (entryIter = mDefaultEntries.find(tagNum)) == mDefaultEntries.end() )
        return NULL;

    return (*entryIter).second->clone();

}

// This method gets called when we are setting up a new ExifImageFile
// (we are creating a new file from scratch or converting a plain Jpeg file
// to include Exif data)
ExifStatus ExifImageFile::initAppSegs( ExifIO * exifio )
{
    ExifStatus returnValue = EXIF_OK ;
    
    ExifTiffAppSeg* appSeg1 = new ExifTiffAppSeg( 0xFFE1, "Exif" ) ;
    mAppSegManager->insert( appSeg1 ) ;
    
    
    appSeg1->init( exifio, 0, 0 ) ;

    ExifIFDPath ifdPath ;
    
    // Set path to the EXIFIFD path and create the path
    // This creates IFD0 and the EXIFIFD.
    ifdPath.push_back( ExifIFDPath::value_type(0, 0) ) ;
    ifdPath.push_back( ExifIFDPath::value_type(EXIFTAG_EXIFIFDPOINTER, 0) ) ;
    appSeg1->createIFD( ifdPath ) ;

    // Add default entries to the EXIFIFD
    ExifTagEntry* tagEntry ;
    ExifIFD* exifIFD = appSeg1->getIFD( ifdPath ) ;
    if( exifIFD == NULL )
        returnValue = EXIF_NO_EXIFIFD_ERROR ;
    else
    {
        // Assuming that the main image is always compressed
        const exif_uint32 numElements = 6 ;
        exiftag_t exifIFDTags[numElements] = { EXIFTAG_EXIFVERSION, 
            EXIFTAG_COMPONENTSCONFIGURATION, EXIFTAG_FLASHPIXVERSION,
            EXIFTAG_COLORSPACE, EXIFTAG_PIXELXDIMENSION,
            EXIFTAG_PIXELYDIMENSION };
        for( unsigned int i=0; i < numElements; i++ )
        {
            tagEntry = getDefaultTag( exifIFDTags[i] ) ;
            exifIFD->setGenericTag( tagEntry ) ;
        }
    }

    // Set path to IFD0 (by removing the last element)
    ifdPath.erase(ifdPath.end() - 1);

    // Add the default entries to IFD 0
    ExifIFD* ifd0 = appSeg1->getIFD( ifdPath ) ;
    if( ifd0 == NULL )
        returnValue = EXIF_NO_IFD0_ERROR ;
    else
    {
        // Assuming that the main image is always compressed
        const exif_uint32 numElements = 4 ;
        exiftag_t ifd0Tags[numElements] = { EXIFTAG_XRESOLUTION, EXIFTAG_YRESOLUTION,
            EXIFTAG_RESOLUTIONUNIT, EXIFTAG_YCBCRPOSITIONING };
        for( unsigned int i=0; i < numElements; i++ )
        {
            tagEntry = getDefaultTag( ifd0Tags[i] ) ;
            ifd0->setGenericTag( tagEntry ) ;
        }
    }
    return returnValue ;
}

// read the App Segments from the file
ExifStatus ExifImageFile::readAppSegs( ExifIO * exifio )
{
    // Scan the image file looking for App Segment
    // markers. Read them all in as Raw App Segments

    uint16 marker ;
    uint16 length ;
    std::string id ;
    exifoff_t offsetToThisMarker ;
    
    // Mark sure we are at the start of the file
    exifio->seek( 0, SEEK_SET ) ;
    readSOI(exifio) ;
    
    char tempIdent[16] ;
    while (true)
    {
        offsetToThisMarker = exifio->seek( 0, SEEK_CUR ) ;

        if ( exifio->read( &marker, sizeof(uint16) ) != sizeof(uint16) )
        {
            return EXIF_FILE_READ_ERROR;
        }
        
        exifio->swabShort( &marker ) ;

        // Once we hit the DQT marker, there connot be any more
        // app segments -- so we're done.
        if ( marker == EXIF_DQT_MARKER || marker == EXIF_SOS_MARKER )
            return EXIF_OK ;

        if ( exifio->read( &length, sizeof(uint16) ) != sizeof(uint16) )
        {
            return EXIF_FILE_READ_ERROR;
        }
        exifio->swabShort( &length ) ;

        off_t offsetToNextMarker = exifio->seek( 0, SEEK_CUR ) + length -
            sizeof(uint16) ;
        
        if ( marker >= EXIF_APP0_MARKER && marker <= EXIF_LAST_APP_MARKER )
        {
            // read the identifier, usually is 5 (App0-JFIF) or 6 bytes
            // but not restricted to it. They always end in at least one NULL
        
            // read in first 16 bytes (I just chose 16, because it seems to be
            // a reasonably enough large buffer to hold most identifiers,
            // if not all)
            if ( exifio->read( (tdata_t)&tempIdent[0], 16*sizeof(char) ) !=
                16*sizeof(char) )
            {
                return EXIF_FILE_READ_ERROR ;
            }

            char* ident ;
            int i = 0;
            while ( i < 16 && tempIdent[i] != '\0' ) i++ ;
        
            if ( tempIdent[i] == '\0' )
            {
                ident = new char[i+1] ;
                memcpy(ident, tempIdent, i+1) ;
            }
            else
            {
                // if we get here, then i=16... Keep on reading from the file
                // until we get a NULL
                char tempByte = 1 ;
                while ( tempByte != '\0' )
                {
                    if ( exifio->read( (tdata_t)&tempByte, 1*sizeof(char) ) !=
                        1*sizeof(char) )
                    {
                        return EXIF_FILE_READ_ERROR ;
                    }
                    i++ ;

                    // The identifier shouldn't get to be bigger than 128 chars
                    if ( i > 128 )
                        break;
                }
                
                if (i > 128)
                {
                    // We have an invalid identifier!!!
                    i = 8;
                    ident = new char[8];
                    ident[0] = 'I';
                    ident[1] = 'N';
                    ident[2] = 'V';
                    ident[3] = 'A';
                    ident[4] = 'L';
                    ident[5] = 'I';
                    ident[6] = 'D';
                    ident[7] = '\0';
                }
                else
                {
                    // allocate an identifier of the right size
                    ident = new char[i] ;
                
            
                    // go back to beginning of identifier and read it in
                    exifio->seek( offsetToThisMarker + 4, SEEK_SET ) ;
                    if ( exifio->read( (tdata_t)&ident[0], i*sizeof(char) ) !=
                        (tsize_t)(i*sizeof(char)) )
                    {
                        delete [] ident ;
                        return EXIF_FILE_READ_ERROR ;
                    }
                }
            }

            ExifRawAppSeg* appSeg = new ExifRawAppSeg(marker, ident) ;
            exifio->seek( offsetToThisMarker, SEEK_SET ) ;
            appSeg->init( exifio, length, (int)0 ) ;
        
            mAppSegManager->insert( appSeg ) ;
            
            delete [] ident ;        
        
        }
        
        // go to next marker
        exifio->seek( 0, SEEK_SET ) ;
        exifio->seek( offsetToNextMarker, SEEK_SET ) ;
    }
    return EXIF_OK ;
}


ExifStatus ExifImageFile::readSOI( ExifIO * exifio )
{
    uint8 soi[2] ;
    
    if( exifio->read( (tdata_t)&soi[0], 2*sizeof(uint8) ) != 2*sizeof(uint8) )
    {
        return EXIF_FILE_READ_ERROR ;
    }

    if( soi[0] != 0xFF || soi[1] != 0xD8 )
    {
        return EXIF_INVALID_FORMAT_ERROR ;
    }

  return EXIF_OK;
}

void ExifImageFile::initEndian( bool bigendian )
{
    // EXIF portion is always big endian
    mExifio->bigEndian( true ) ;
    
    // If the host processor is not big endian, then SWAB
    if( !bigendian )
        mExifio->flags() |= EXIF_SWAB;
    
    return;
}

// get all tags from a tiff based app segment
ExifStatus ExifImageFile::getAllTags( uint16 marker, const std::string& appSegId,
                                     ExifPathsTags &pathsTags )
{
    ExifStatus status = EXIF_OK ;
    
    ExifTiffAppSeg* tiffAppSeg = NULL ;
    ExifAppSegment* appSeg = mAppSegManager->getAppSeg( marker,
        appSegId.c_str() ) ;
    
    if ( appSeg == NULL )
        status = EXIF_INVALID_APP_ID ; // app seg not present in file
    else
    {
        tiffAppSeg = dynamic_cast<ExifTiffAppSeg*>(appSeg) ;
        if ( tiffAppSeg == NULL )
            status = EXIF_ERROR ; // could not cast to ExifTiffAppSeg*
    }
    
    if ( tiffAppSeg && status!=EXIF_ERROR )    
        tiffAppSeg->getAllTags( pathsTags ) ;
    
    return EXIF_OK ;
}

// set all tags to a tiff based app segment
ExifStatus ExifImageFile::setAllTags( uint16 marker, const std::string& appSegId,
                                     const ExifPathsTags &pathsTags )
{
    ExifStatus status = EXIF_OK ;

    ExifTiffAppSeg* tiffAppSeg ;
    ExifAppSegment* appSeg = mAppSegManager->getAppSeg( marker,
        appSegId.c_str() ) ;
    
    if ( appSeg != NULL )
    {
        tiffAppSeg = dynamic_cast<ExifTiffAppSeg*>(appSeg) ;
        if ( tiffAppSeg == NULL )
            status = EXIF_ERROR ;
    }
    else
    {
        tiffAppSeg = new ExifTiffAppSeg( marker, appSegId.c_str() ) ;
        if ( tiffAppSeg == NULL )
        {
            status = EXIF_LOW_MEMORY_ERROR ;
        }
        else
        {
            mAppSegManager->insert( tiffAppSeg ) ;
            tiffAppSeg->init( mExifio, 0, 0 ) ;
        }
    }

    if ( status == EXIF_OK )
        tiffAppSeg->setAllTags( pathsTags ) ;
    
    return status ;
}

ExifTagEntry* ExifImageFile::getGenericTag( exiftag_t tag, uint16 marker,
    const std::string& appSegId, const ExifIFDPath& ifdPath, ExifStatus& errRtn )
{
    ExifTagEntry* returnValue = NULL ;
    errRtn = EXIF_OK ;

    ExifTiffAppSeg* appSeg = dynamic_cast<ExifTiffAppSeg*>( 
        mAppSegManager->getAppSeg( marker, appSegId.c_str() ) ) ;

    if (appSeg == NULL)
        errRtn = EXIF_INVALID_APP_ID ;
    else
        returnValue = appSeg->getGenericTag( tag, ifdPath ) ;

    return returnValue ;
}

ExifTagEntry* ExifImageFile::getGenericTag( exiftag_t tag,
                                           ExifTagLocation tagLocation,
                                           ExifStatus& errRtn )
{
    ExifTagEntry* returnValue = NULL ;
    uint16 marker;
    std::string appSegId;
    ExifIFDPath ifdPath;
    errRtn = EXIF_OK;

    errRtn = convertTagLocation( tagLocation, marker, appSegId, ifdPath );
    if( errRtn == EXIF_OK )
        returnValue = getGenericTag( tag, marker, appSegId, ifdPath, errRtn );

    return returnValue ;
}

ExifStatus ExifImageFile::setGenericTag( const ExifTagEntry& tag,
    uint16 marker, const std::string& appSegId, const ExifIFDPath& ifdPath )
{
    ExifStatus returnValue = EXIF_OK ;

    ExifTiffAppSeg* appSeg = dynamic_cast<ExifTiffAppSeg*>(
        mAppSegManager->getAppSeg( marker, appSegId.c_str() ) ) ;
        
    if ( appSeg == NULL )
    {
        // we must be creating a new tiff-based app seg, otherwise
        // it would have already been allocated.
        appSeg = new ExifTiffAppSeg( marker, appSegId.c_str() ) ;
        if ( appSeg == NULL )
            returnValue = EXIF_ERROR ;
        else
        {
            mAppSegManager->insert( appSeg ) ;
            appSeg->init( mExifio, 0, 0 ) ;
            appSeg = dynamic_cast<ExifTiffAppSeg*>(
                mAppSegManager->getAppSeg( marker, appSegId.c_str() ) ) ;
            if (!appSeg)
                returnValue = EXIF_ERROR ;
        }
    }
    
    if ( returnValue == EXIF_OK )
        returnValue = appSeg->setGenericTag( tag, ifdPath ) ;

    return returnValue ;
}

ExifStatus ExifImageFile::setGenericTag( const ExifTagEntry& tag,
                                        ExifTagLocation tagLocation )
{
    ExifStatus returnValue = EXIF_OK ;
    uint16 marker;
    std::string appSegId;
    ExifIFDPath ifdPath;

    returnValue = convertTagLocation( tagLocation, marker, appSegId, ifdPath );
    if( returnValue == EXIF_OK )
        returnValue = setGenericTag( tag, marker, appSegId, ifdPath ) ;

    return returnValue ;
}

void ExifImageFile::removeGenericTag( exiftag_t tag, uint16 marker,
    const std::string& appSegId, const ExifIFDPath& ifdPath )
{
    ExifTiffAppSeg* appSeg = dynamic_cast<ExifTiffAppSeg*>( 
        mAppSegManager->getAppSeg( marker, appSegId.c_str() ) ) ;

    if (appSeg != NULL)
        appSeg->removeGenericTag( tag, ifdPath ) ;
}

void ExifImageFile::removeGenericTag( exiftag_t tag, 
    ExifTagLocation tagLocation )
{
    uint16 marker;
    std::string appSegId;
    ExifIFDPath ifdPath;
    ExifStatus status = EXIF_OK;

    status = convertTagLocation( tagLocation, marker, appSegId, ifdPath );
    if( status == EXIF_OK )
        removeGenericTag( tag, marker, appSegId, ifdPath );
}

ExifImageDesc* ExifImageFile::getThumbnail( void ) 
{
    ExifImageDesc* returnValue = NULL ;

    ExifTiffAppSeg* appSeg1 = dynamic_cast<ExifTiffAppSeg*> (
        mAppSegManager->getAppSeg( 0xFFE1, "Exif" ) ) ;

    if( appSeg1 != NULL )
    {
        // Path to IFD 1
        ExifIFDPath ifdPath ;
        ifdPath.push_back( ExifIFDPath::value_type(0, 1) ) ;
        ExifIFD* ifd1 = appSeg1->getIFD( ifdPath ) ;
        if( ifd1 != NULL )
            returnValue = ifd1->getImage() ;
    }
    
    return returnValue ;
}

ExifStatus ExifImageFile::setThumbnail( ExifImageDesc* thumbNail,
                                       bool compressIt ) 
{
    ExifStatus returnValue = EXIF_OK ;
    
    ExifTiffAppSeg* appSeg1 = dynamic_cast<ExifTiffAppSeg*> (
        mAppSegManager->getAppSeg( 0xFFE1, "Exif" ) ) ;
    
    if( appSeg1 == NULL )
        returnValue = EXIF_NO_APP1_ERROR ;
    else
    {
        // Path to IFD 1
        ExifIFDPath ifdPath ;
        ifdPath.push_back( ExifIFDPath::value_type(0, 1) ) ;
        ExifIFD* ifd1 = appSeg1->getIFD( ifdPath ) ;

        if( ifd1 == NULL )
            returnValue = EXIF_NO_IFD1_ERROR ;
        else
            ifd1->setImage( thumbNail, compressIt ) ;
    }

    return returnValue ;
}

// Set an ICC profile. It is not written out until writeScanLines is called,
// which actually just writes it to the temporary file.
ExifStatus ExifImageFile::setICCProfile(uint8 *iccBuf, exif_uint32 iccSize)
{
    mICCProf = iccBuf;
    mICCProfSize = iccSize;
    return EXIF_OK;
}

// For writeScanLines... we don't want to call the ExifJpegImage's
// writeScanLines at this time, since it will immediately write 
// the compressed stream directly to the file. We need to delay
// this write until the close of the image file so that the writing
// of the compressed stream is coordinated with the writing of the
// application segments. Two ways to do this. First, do the compression
// now, but have the ExifJpegImage write it to a temporary buffer, then
// we write this compressed data stream to the image file during the close.
// Second, we hold onto the reference to the ExifImageDesc passed in and
// do the compression and writing to the file during the close. Right now,
// I think the first is the best way to go...
ExifStatus 
ExifImageFile::writeScanLines( ExifImageDesc &imgDesc, exif_uint32 numLines, 
                               exif_uint32 &nextLine, exif_uint32& linesCompressed )
{ 

    // write to a temp file
    ExifStatus status = EXIF_ERROR;
    if( mTmpImageFile.size() == 0 )
    {
#ifdef _MSC_VER
 	    // Windows' _tempnam automatically uses the
	    // TMP environmental variable if it's present,
	    // otherwise it will use the "C:\\temp" directory.
	    mTmpImageFile = _tempnam("C:\\temp","oet");
        status = mOutImage.open( mTmpImageFile.c_str(), "w" ) ;
#elif (defined(__GNUC__) && (__GNUC__ > 2))
        // GCC Linkers always complain about the unsafe nature of
        // of tmpnam().  So here's a work-around. using mkstemp.
        // It's not great, but it works "safely".
        char tmpFileName[L_tmpnam];
        int len = strlen(P_tmpdir);
        strncpy(tmpFileName,P_tmpdir,len);
        strncpy(&tmpFileName[len],"/OpenExifXXXXXX",16);
        int tfd = mkstemp(tmpFileName);
        FILE * tf = fdopen(tfd,"w+");
        status = mOutImage.open(tf,tmpFileName,"w");
        mTmpImageFile = tmpFileName;
#else
        mTmpImageFile = tmpnam(NULL) ;
        status = mOutImage.open( mTmpImageFile.c_str(), "w" ) ;
#endif
    }

    if( status == EXIF_OK )
    {
        mImageModifiedOrCreated = true ;

        if( mICCProf != NULL )
        {
            mOutImage.setICCProfile(mICCProf,mICCProfSize);
            mICCProf = NULL;
        }


        status = mOutImage.writeScanLines( imgDesc, numLines,
            nextLine, linesCompressed ) ;
        mOutImage.close() ;
    }
    return status ;
}

// Convert an ExifTagLocation to a appSegMarker, appSegID, and ExifIFDPath
ExifStatus ExifImageFile::convertTagLocation( ExifTagLocation tagLocation,
    uint16& marker, std::string& appSegId, ExifIFDPath& ifdPath )
{
    switch (tagLocation)
    {
    case EXIF_APP1_IFD0:

        marker = 0xFFE1;
        appSegId = "Exif";
        ifdPath.push_back( ExifIFDPath::value_type(0, 0) );
        break;
    case EXIF_APP1_EXIFIFD:
        marker = 0xFFE1;
        appSegId = "Exif";
        ifdPath.push_back( ExifIFDPath::value_type(0, 0) );
        ifdPath.push_back( ExifIFDPath::value_type(EXIFTAG_EXIFIFDPOINTER,0) );
        break;
    case EXIF_APP1_EXIFINTEROP_IFD:
        marker = 0xFFE1;
        appSegId = "Exif";
        ifdPath.push_back( ExifIFDPath::value_type(0, 0) );
        ifdPath.push_back( ExifIFDPath::value_type(EXIFTAG_EXIFIFDPOINTER,0) );
        ifdPath.push_back( ExifIFDPath::value_type
            (EXIFTAG_EXIFINTEROPIFDPOINTER, 0) );
        break;
    case EXIF_APP1_IFD0_GPSINFO_IFD:
        marker = 0xFFE1;
        appSegId = "Exif";
        ifdPath.push_back( ExifIFDPath::value_type(0, 0) );
        ifdPath.push_back( ExifIFDPath::value_type(EXIFTAG_GPSINFO, 0) );
        break;
    case EXIF_APP1_IFD1:
        marker = 0xFFE1;
        appSegId = "Exif";
        ifdPath.push_back( ExifIFDPath::value_type(0, 1) );
        break;
    case EXIF_APP3_IFD0:
        marker = 0xFFE3;
        appSegId = "Meta";
        ifdPath.push_back( ExifIFDPath::value_type(0, 0) );
        break;
    default:
        return EXIF_INVALID_TAG_LOCATION;
    }

    return EXIF_OK;
}

// Get the offset to the DQT marker
// SMF 3/29/2005 -- This now returns the first of DQT/DHT/SOF 
//                  markers as it's only used to copy image data.
ExifStatus ExifImageFile::getOffsetToDQT( ExifIO *tmpExifIo,
    exifoff_t& offsetToDQT )
{
    tmpExifIo->bigEndian( true ) ;
    // If the host processor is not big endian, then SWAB
    bool bigendian = HOST_BIGENDIAN ;
    if( !bigendian )
        tmpExifIo->flags() |= EXIF_SWAB;

    uint16 marker ;
    uint16 length ;
    std::string id ;
    
    // Mark sure we are at the start of the file
    tmpExifIo->seek( 0, SEEK_SET ) ;
    
    uint16 soi ;
    tmpExifIo->read( &soi, sizeof(uint16) ) ;
    
    while (true)
    {
        if ( tmpExifIo->read( &marker, sizeof(uint16) ) != sizeof(uint16) )
        {
            return EXIF_FILE_READ_ERROR;
        }
        
        tmpExifIo->swabShort( &marker ) ;
        
        // Once we hit the DQT marker, we're done.
        if ( ( marker == EXIF_DQT_MARKER ) || 
             ( ( marker >= EXIF_SOF0_MARKER ) &&
               ( marker <= EXIF_SOF15_MARKER ) ) )
        {
            offsetToDQT = tmpExifIo->seek( 0, SEEK_CUR ) - sizeof(uint16);
            return EXIF_OK ;
        }

        if ( tmpExifIo->read( &length, sizeof(uint16) ) != sizeof(uint16) )
        {
            return EXIF_FILE_READ_ERROR;
        }
        tmpExifIo->swabShort( &length ) ;

        
        off_t offsetToNextMarker = tmpExifIo->seek( 0, SEEK_CUR )
            + length - sizeof(uint16) ;
        
        tmpExifIo->seek( 0, SEEK_SET ) ;
        tmpExifIo->seek( offsetToNextMarker, SEEK_SET ) ;
    }

    return EXIF_ERROR ;
}

// Insert an Application Segment into the AppSegManager
ExifStatus ExifImageFile::insertAppSeg( ExifAppSegment* appSeg )
{
    return mAppSegManager->insert(appSeg->clone()) ;
}

// Get the first application segment with the specified ID
ExifAppSegment* ExifImageFile::getAppSeg( uint16 marker,
    const char * ID, bool tryToConvert )
{
    return mAppSegManager->getAppSeg(marker,ID,tryToConvert) ;
}

// Get all the application segments with the specified ID
std::vector<ExifAppSegment*> ExifImageFile::getAllAppSegs( uint16 marker,
    const char * ID, bool tryToConvert )
{
    return mAppSegManager->getAllAppSegs(marker,ID,tryToConvert) ;
}

// Get all the application segments
std::vector<ExifAppSegment*> ExifImageFile::getAllAppSegs( bool tryToConvert )
{
    return mAppSegManager->getAllAppSegs(tryToConvert) ;
}

// Get the first raw application segment with the specified ID
ExifAppSegment* ExifImageFile::getRawAppSeg( uint16 marker, const char * ID, 
    bool tryToConvert )
{
    return mAppSegManager->getRawAppSeg(marker,ID,tryToConvert) ;
}

// Get all the raw application segments with the specified ID
std::vector<ExifAppSegment*> ExifImageFile::getAllRawAppSegs( uint16 marker,
    const char * ID, bool tryToConvert )
{
    return mAppSegManager->getAllRawAppSegs(marker,ID,tryToConvert) ;
}

// Get all the raw application segments
std::vector<ExifAppSegment*> ExifImageFile::getAllRawAppSegs( bool tryToConvert )
{
    return mAppSegManager->getAllRawAppSegs(tryToConvert) ;
}

// Delete an Application Segment with the specified ID
ExifStatus ExifImageFile::removeAppSeg( uint16 marker, const char * ID )
{
    return mAppSegManager->remove(marker,ID) ;
}

// Delete all the Application Segments with the specified ID
ExifStatus ExifImageFile::removeAllAppSegs( uint16 marker, const char * ID )
{
    return mAppSegManager->removeAll(marker,ID) ;
}

// Delete all the app segments
void ExifImageFile::removeAllAppSegs()
{
    mAppSegManager->clear() ;
}

// Get the quantization table specified by qIndex
ExifStatus ExifImageFile::getJpegQTable(  unsigned char qIndex, 
    ExifJpegQuantTable* qTable )
{
    ExifStatus returnValue = EXIF_ERROR;
    
    if ((mJpegTables != NULL) && (qIndex < EXIF_MAX_COMPONENTS))
        if (mJpegTables->Q[qIndex] != NULL)
        {
            for (int i = 0; i < 64; i++)
                qTable->quantizer[i] = mJpegTables->Q[qIndex]->quantizer[i];
            returnValue = EXIF_OK;
        }

    return returnValue;
}
}