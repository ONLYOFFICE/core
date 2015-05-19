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
 * Portions of the Original Code are
 * Copyright (c) 1988-1996 Sam Leffler
 * Copyright (c) 1991-1996 Silicon Graphics, Inc.
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

//#include "ExifConf.h"
#include "ExifStripImage.h"
#include "ExifIO.h"
#include "ExifIFD.h"


#define	NOSTRIP	((exif_uint32) -1)			/* undefined state */

#define	WRITECHECKSTRIPS				\
	((mExifio->flags() & EXIF_BEENWRITING) || (writeCheck(0) == EXIF_OK))

namespace Exif{

ExifStripImage::ExifStripImage(ExifStatus& status, ExifIFD* tdir, ExifIO* tio)
    : mExifdir(tdir), mExifio(tio), mNStrips(0)
{
    mStripsperimage = mExifdir->numOfStrips();
    mScanlinesize = mExifdir->scanlineSize();
    mCurstrip = (exif_uint32)-1;
    
    status = init() ;
}

ExifStripImage::~ExifStripImage()
{
}


ExifStatus ExifStripImage::init()
{
    mStripsperimage = mExifdir->getGenericTag(EXIFTAG_ROWSPERSTRIP) ?
        mExifdir->numOfStrips() : 
        mExifdir->samplesPerPixel();

    mNStrips = mStripsperimage;
    if (mExifdir->planarConfig() == PLANARCONFIG_SEPARATE)
        mStripsperimage /= mExifdir->samplesPerPixel();
    
    uint16 compression ;
    compression = mExifdir->compression();
    if( compression != EXIF_COMPRESSION_NONE )
        return EXIF_ERROR ;
    
    return EXIF_OK ;	
}


ExifStatus ExifStripImage::writeImage( ExifImageDesc &imgDesc )
{ 
    ExifStatus status = EXIF_OK;

    exif_uint32 length = imgDesc.numberOfRows ;
    uint8* buf = imgDesc.components[0].theData ;
        
    for (unsigned int row=0; row<length; row++)
    {
        if( (status = writeScanline(buf, row) ) != EXIF_OK )
            return status ;
        buf += imgDesc.components[0].lineStride ;
    }

    return status ;
}


ExifStatus ExifStripImage::writeScanline(tdata_t buf, exif_uint32 row,
                                         tsample_t sample)
{
    int status, imagegrew = 0;
    exif_uint32 strip;

    if (!WRITECHECKSTRIPS)
        return (EXIF_ERROR);

    /*
     * Extend image length if needed
     * (but only for PlanarConfig=1).
     */
    if (row >= mExifdir->imageLength()) {	/* extend image */
        if (mExifdir->planarConfig() == PLANARCONFIG_SEPARATE) 
        {
            // Can not change "ImageLength" when using separate planes"
            return EXIF_ERROR;
        }
        mExifdir->imageLength() = row+1;
        imagegrew = 1;
    }
    /*
     * Calculate strip and check for crossings.
     */
    if (mExifdir->planarConfig() == PLANARCONFIG_SEPARATE) 
    {
        if (sample >= mExifdir->samplesPerPixel()) 
        {
            // Sample out of range
            return EXIF_ERROR;
        }
        strip = sample*mStripsperimage + row/mExifdir->rowsPerStrip();
    } 
    else
        strip = row / mExifdir->rowsPerStrip();

    if (strip != mCurstrip) 
    {
        /*
         * Changing strips -- flush any data present.
         */
//        if (!libFlushData2())
//            return (EXIF_ERROR);
        mCurstrip = strip;
        /*
         * Watch out for a growing image.  The value of
         * strips/image will initially be 1 (since it
         * can't be deduced until the imagelength is known).
         */
        if (strip >= mStripsperimage && imagegrew)
            mStripsperimage = EXIFhowmany(mExifdir->imageLength(),
            mExifdir->rowsPerStrip());

    }
    /*
     * Check strip array to make sure there's space.
     * We don't support dynamically growing files that
     * have data organized in separate bitplanes because
     * it's too painful.  In that case we require that
     * the imagelength be set properly before the first
     * write (so that the strips array will be fully
     * allocated above).
     */
    // todo: make sure that is not needed
    if (strip >= mNStrips && !growStrips(1))
        return (EXIF_ERROR);
    /*
     * Ensure the write is either sequential or at the
     * beginning of a strip (or that we can randomly
     * access the data -- i.e. no encoding).
     */

    status = writeRawStrip(strip, (tdata_t) buf, mScanlinesize ) ;
    
    return (status? EXIF_OK : EXIF_ERROR);
}


/*
 * Write the supplied data to the specified strip.
 * There must be space for the data; we don't check
 * if strips overlap!
 *
 * NB: Image length must be setup before writing.
 */
tsize_t ExifStripImage::writeRawStrip(exif_uint32 strip, tdata_t data, tsize_t cc)
{
    if (!WRITECHECKSTRIPS)
        return ((tsize_t) -1);
    /*
     * Check strip array to make sure there's space.
     * We don't support dynamically growing files that
     * have data organized in separate bitplanes because
     * it's too painful.  In that case we require that
     * the imagelength be set properly before the first
     * write (so that the strips array will be fully
     * allocated above).
     */
    if (strip >= mNStrips) 
    {
        if (mExifdir->planarConfig() == PLANARCONFIG_SEPARATE) 
        {
            // Can not grow image by strips when using separate planes
            return ((tsize_t) -1);
        }
        /*
         * Watch out for a growing image.  The value of
         * strips/image will initially be 1 (since it
         * can't be deduced until the imagelength is known).
         */
        if (strip >= mStripsperimage)
            mStripsperimage =
                EXIFhowmany(mExifdir->imageLength(),mExifdir->rowsPerStrip());
        if (!growStrips(1))
            return ((tsize_t) -1);
    }
    mCurstrip = strip;

    if (!mExifdir->isFillOrder( mExifio, FILLORDER_MSB2LSB) &&
        (mExifio->flags() & EXIF_NOBITREV) == 0)
    {
        reverseBits( (tdata_t)data, cc );
    }

    return ((appendToStrip(strip, (tdata_t) data, cc) == EXIF_OK) ?
	    cc : (tsize_t) -1);
}


ExifStatus ExifStripImage::appendToStrip(exif_uint32 strip, tdata_t data,
                                         tsize_t cc)
{
    std::vector<exif_uint32> offset = mExifdir->stripOffsets();
    std::vector<exif_uint32> bytecount = mExifdir->stripByteCounts();

    exif_uint32 stripPtr = offset[strip] + bytecount[strip] ;
    /*
     * No current offset, set the current strip.
     */
    if (offset[strip] != 0) {
        if (mExifio->seek(stripPtr, SEEK_SET )!= stripPtr ) 
        {
            // Seek error
            return EXIF_ERROR;
        }
    } 
    else
        offset[strip] = mExifio->seek((exifoff_t) 0, SEEK_END) ;


    if (mExifio->write(data, cc) != cc) 
    {
        // Write error
        return EXIF_ERROR;

    }

    bytecount[strip] += cc;

    ExifTagEntry* off = mExifdir->getGenericTag( EXIFTAG_STRIPOFFSETS );
    ExifTagEntry* cnt = mExifdir->getGenericTag( EXIFTAG_STRIPBYTECOUNTS );
    if( off->getCount() > 1 )
    {
        ExifTagEntryT< std::vector<exif_uint32> >* temp = 
            (ExifTagEntryT< std::vector<exif_uint32> >*)off;
        temp->setValue(offset);
    }
    else
    {
        ExifTagEntryT<exif_uint32>* temp = (ExifTagEntryT<exif_uint32>*)off;
        temp->setValue(offset.front()) ;
    }
    if( cnt->getCount() > 1 )
    {
        ExifTagEntryT< std::vector<exif_uint32> >* temp = 
            (ExifTagEntryT< std::vector<exif_uint32> >*)cnt;
        temp->setValue(bytecount);
    }
    else
    {
        ExifTagEntryT<exif_uint32>* temp = (ExifTagEntryT<exif_uint32>*)cnt;
        temp->setValue(bytecount.front()) ;
    }

    return EXIF_OK;
}


int ExifStripImage::growStrips(int delta)
{
    std::vector<exif_uint32> offset = mExifdir->stripOffsets();
    std::vector<exif_uint32> bytecount = mExifdir->stripByteCounts();
    ExifTagEntry* off = mExifdir->getGenericTag( EXIFTAG_STRIPOFFSETS );
    ExifTagEntry* cnt = mExifdir->getGenericTag( EXIFTAG_STRIPBYTECOUNTS );

    for (int i=0; i<delta; i++) 
    {
        offset.push_back(0);
        bytecount.push_back(0);	
    }
    mNStrips += delta;

    if( off->getCount() > 1 )
    {
        ExifTagEntryT< std::vector<exif_uint32> >* temp = 
            (ExifTagEntryT< std::vector<exif_uint32> >*)off;
        temp->setValue( offset );
        temp->setCount( mNStrips );
    }
    else if (off->getCount() == 1 && delta > 0)
    {
        ExifTagEntryT< std::vector<exif_uint32> >* temp = 
            new ExifTagEntryT< std::vector<exif_uint32> >
            (EXIFTAG_STRIPOFFSETS, EXIF_LONG, mNStrips, offset);
        mExifdir->setGenericTag( *temp );
        delete temp;
    }
    if( cnt->getCount() > 1 )
    {
        ExifTagEntryT< std::vector<exif_uint32> >* temp = 
            (ExifTagEntryT< std::vector<exif_uint32> >*)cnt;
        temp->setValue( bytecount );
        temp->setCount( mNStrips );
    }
    else if (off->getCount() == 1 && delta > 0)
    {
        ExifTagEntryT< std::vector<exif_uint32> >* temp = 
            new ExifTagEntryT< std::vector<exif_uint32> >
            (EXIFTAG_STRIPBYTECOUNTS, EXIF_LONG, mNStrips, offset);
        mExifdir->setGenericTag( *temp );
        delete temp;
    }

    return (1);
}


int ExifStripImage::setupOffsets()
{
	// setup stripoffsets and stripbytecounts
    if ( mNStrips > 1 )
    {
        ExifTagEntry* off = new ExifTagEntryT<std::vector<exif_uint32> >
            (EXIFTAG_STRIPOFFSETS, EXIF_LONG, mNStrips,
            std::vector<exif_uint32>(mNStrips, 0));
        mExifdir->setGenericTag(*off);
        delete off;
    
        ExifTagEntry* cnt =  new ExifTagEntryT<std::vector<exif_uint32> >
            (EXIFTAG_STRIPBYTECOUNTS, EXIF_LONG, mNStrips,
            std::vector<exif_uint32>(mNStrips, 0));
        mExifdir->setGenericTag(*cnt);
        delete cnt;
    }
    else
    {
        ExifTagEntry* off =  
            new ExifTagEntryT<exif_uint32>
            (EXIFTAG_STRIPOFFSETS, EXIF_LONG, mNStrips, 0);
        mExifdir->setGenericTag(*off);
        delete off;
    
        ExifTagEntry* cnt = 
            new ExifTagEntryT<exif_uint32>
            (EXIFTAG_STRIPBYTECOUNTS, EXIF_LONG, mNStrips, 0);
        mExifdir->setGenericTag(*cnt);
        delete cnt;
    }

    return (1);
}

ExifStatus ExifStripImage::writeCheck(int tiles)
{
    if (mExifio->mode() == O_RDONLY) 
    {
        // File not open for writing
        return EXIF_ERROR;
    }
    /*
     * On the first write verify all the required information
     * has been setup and initialize any data structures that
     * had to wait until directory information was set.
     * Note that a lot of our work is assumed to remain valid
     * because we disallow any of the important parameters
     * from changing after we start writing (i.e. once
     * EXIF_BEENWRITING is set, EXIFSetField will only allow
     * the image's length to be changed).
     */
    if (mExifdir->getGenericTag(EXIFTAG_IMAGEWIDTH) == NULL) 
    {
        // Must set "ImageWidth" before writing data
        return EXIF_ERROR ;
    }
    if (mExifdir->getGenericTag(EXIFTAG_PLANARCONFIG) == NULL) 
    {
        //Must set "PlanarConfiguration" before writing data
        return EXIF_ERROR;
    }
    
    ExifTagEntry* p = mExifdir->getGenericTag(EXIFTAG_STRIPOFFSETS);
     
    if (p != 0) 
    {
        mStripsperimage = mExifdir->getGenericTag(EXIFTAG_ROWSPERSTRIP) ?
            mExifdir->numOfStrips() : 
            mExifdir->samplesPerPixel();

        mNStrips = mStripsperimage;
        if (mExifdir->planarConfig() == PLANARCONFIG_SEPARATE)
            mStripsperimage /= mExifdir->samplesPerPixel();
        
        std::vector<exif_uint32> bytecounts = mExifdir->stripByteCounts();
        
        bytecounts.clear();
        for (exif_uint32 i=0; i<mNStrips; i++)
            bytecounts.push_back(0);

        if ( mNStrips > 1 )
        {
            ExifTagEntry* cnt = new ExifTagEntryT<std::vector<exif_uint32> >
                (EXIFTAG_STRIPBYTECOUNTS, EXIF_LONG, mNStrips, bytecounts );
            mExifdir->setGenericTag(*cnt);
            delete cnt;
        }
        else
        {
            ExifTagEntry* cnt = new ExifTagEntryT<exif_uint32>
                (EXIFTAG_STRIPBYTECOUNTS, EXIF_LONG, mNStrips,
                bytecounts.front() );
            mExifdir->setGenericTag(*cnt);
            delete cnt;
        }
    }
    else 
    {
        if (!setupOffsets()) 
        {
            // No space for strip/tile arrays
            return EXIF_ERROR;
        }
    }
    
    mExifio->flags() |= EXIF_BEENWRITING;
    return EXIF_OK;
}


ExifStatus ExifStripImage::checkRead(int tiles)
{
    if (mExifio->mode() == O_WRONLY) 
    {
        // File not open for reading
        return EXIF_ERROR;
    }

    return EXIF_OK;
}

ExifStatus ExifStripImage::readImage( ExifImageDesc &imgDesc )
{ 
   if (checkRead(0) != EXIF_OK)
        return EXIF_FILE_READ_ERROR ;

    ExifStatus status = EXIF_OK ;

    exif_uint32 length = imgDesc.numberOfRows ;
    uint8* buf = imgDesc.components[0].theData ;
        
    for (unsigned int row=0; row<length; row++)
    {
        if( (status = readScanline(buf, row) ) != EXIF_OK )
            return status ;
        buf += imgDesc.components[0].lineStride ;
    }

    return status ;
}

ExifStatus ExifStripImage::readScanline(tdata_t buf, exif_uint32 row,
                                        tsample_t sample)
{
     if (row >= mExifdir->imageLength()) 
    {	/* out of range */
        // Row out of range
        return EXIF_ERROR;
    }
    
    ExifStatus status = EXIF_OK;
    exif_uint32 strip = computeStrip( row, sample, status ) ;
    if ( status != EXIF_OK )
        return status;


//    if (strip != mCurstrip) 
//    { 	/* different strip, refill */
//        if (!fillStrip(strip))
//            return (0);
//    } 


    tsize_t bytecount;

    if (strip >= mNStrips) 
    {
        // Strip out of range
        return EXIF_ERROR;
    }

    std::vector<exif_uint32> stripbytecount = mExifdir->stripByteCounts();
    bytecount = stripbytecount[strip];
    if (bytecount <= 0) 
    {
        // Invalid strip byte count
        return EXIF_ERROR;
    }

    if (mScanlinesize != (tsize_t)-1 && mScanlinesize < bytecount)
        bytecount = mScanlinesize;


    std::vector<exif_uint32> stripoffset = mExifdir->stripOffsets();

    exifoff_t offset = stripoffset[strip] + mExifdir->getExifOffset() ;

    offset += (row - strip * mExifdir->rowsPerStrip()) * bytecount ;
    
    if (mExifio->seek(offset, SEEK_SET) != offset)
    {
        // Seek error at scanline
        return EXIF_ERROR;
    }
    if (mExifio->read(buf, bytecount) != bytecount)
    {
        // Read error at scanline
        return EXIF_ERROR;
    }

    if (!mExifdir->isFillOrder( mExifio, FILLORDER_MSB2LSB ) &&
        (mExifio->flags() & EXIF_NOBITREV) == 0)
        reverseBits( buf, bytecount );

    return EXIF_OK ;
}





exif_uint32 ExifStripImage::computeStrip(exif_uint32 row, tsample_t sample,
                                      ExifStatus& errRtn)
{
    errRtn = EXIF_OK;
    exif_uint32 strip = 0;

    strip = row / mExifdir->rowsPerStrip();
    if (mExifdir->planarConfig() == PLANARCONFIG_SEPARATE) 
    {
        if (sample >= mExifdir->samplesPerPixel()) 
        {
            // Sample out of range
            errRtn = EXIF_ERROR;
            return strip;
        }
        strip += sample*mStripsperimage;
    }
    return (strip);
}

tsize_t ExifStripImage::vStripSize(exif_uint32 nrows)
{
    return mExifdir->vStripSize( mExifio, nrows );
}

tsize_t ExifStripImage::stripSize()
{
    exif_uint32 rps = mExifdir->rowsPerStrip();
    if (rps > mExifdir->imageLength())
        rps = mExifdir->imageLength();
    return (vStripSize(rps));
}


/*
 * Bit reversal tables.  EXIFBitRevTable[<byte>] gives
 * the bit reversed value of <byte>.  Used in various
 * places in the library when the FillOrder requires
 * bit reversal of byte values (e.g. CCITT Fax 3
 * encoding/decoding).
 */
static const unsigned char EXIFBitRevTable[256] = {
    0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0,
    0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0,
    0x08, 0x88, 0x48, 0xc8, 0x28, 0xa8, 0x68, 0xe8,
    0x18, 0x98, 0x58, 0xd8, 0x38, 0xb8, 0x78, 0xf8,
    0x04, 0x84, 0x44, 0xc4, 0x24, 0xa4, 0x64, 0xe4,
    0x14, 0x94, 0x54, 0xd4, 0x34, 0xb4, 0x74, 0xf4,
    0x0c, 0x8c, 0x4c, 0xcc, 0x2c, 0xac, 0x6c, 0xec,
    0x1c, 0x9c, 0x5c, 0xdc, 0x3c, 0xbc, 0x7c, 0xfc,
    0x02, 0x82, 0x42, 0xc2, 0x22, 0xa2, 0x62, 0xe2,
    0x12, 0x92, 0x52, 0xd2, 0x32, 0xb2, 0x72, 0xf2,
    0x0a, 0x8a, 0x4a, 0xca, 0x2a, 0xaa, 0x6a, 0xea,
    0x1a, 0x9a, 0x5a, 0xda, 0x3a, 0xba, 0x7a, 0xfa,
    0x06, 0x86, 0x46, 0xc6, 0x26, 0xa6, 0x66, 0xe6,
    0x16, 0x96, 0x56, 0xd6, 0x36, 0xb6, 0x76, 0xf6,
    0x0e, 0x8e, 0x4e, 0xce, 0x2e, 0xae, 0x6e, 0xee,
    0x1e, 0x9e, 0x5e, 0xde, 0x3e, 0xbe, 0x7e, 0xfe,
    0x01, 0x81, 0x41, 0xc1, 0x21, 0xa1, 0x61, 0xe1,
    0x11, 0x91, 0x51, 0xd1, 0x31, 0xb1, 0x71, 0xf1,
    0x09, 0x89, 0x49, 0xc9, 0x29, 0xa9, 0x69, 0xe9,
    0x19, 0x99, 0x59, 0xd9, 0x39, 0xb9, 0x79, 0xf9,
    0x05, 0x85, 0x45, 0xc5, 0x25, 0xa5, 0x65, 0xe5,
    0x15, 0x95, 0x55, 0xd5, 0x35, 0xb5, 0x75, 0xf5,
    0x0d, 0x8d, 0x4d, 0xcd, 0x2d, 0xad, 0x6d, 0xed,
    0x1d, 0x9d, 0x5d, 0xdd, 0x3d, 0xbd, 0x7d, 0xfd,
    0x03, 0x83, 0x43, 0xc3, 0x23, 0xa3, 0x63, 0xe3,
    0x13, 0x93, 0x53, 0xd3, 0x33, 0xb3, 0x73, 0xf3,
    0x0b, 0x8b, 0x4b, 0xcb, 0x2b, 0xab, 0x6b, 0xeb,
    0x1b, 0x9b, 0x5b, 0xdb, 0x3b, 0xbb, 0x7b, 0xfb,
    0x07, 0x87, 0x47, 0xc7, 0x27, 0xa7, 0x67, 0xe7,
    0x17, 0x97, 0x57, 0xd7, 0x37, 0xb7, 0x77, 0xf7,
    0x0f, 0x8f, 0x4f, 0xcf, 0x2f, 0xaf, 0x6f, 0xef,
    0x1f, 0x9f, 0x5f, 0xdf, 0x3f, 0xbf, 0x7f, 0xff
};


void ExifStripImage::reverseBits( tdata_t buf, tsize_t nBytes )
{

    uint8 *cp = (uint8*)buf ;
    tsize_t n = nBytes ;
    
    for ( ; n > 8; n -= 8) 
    {
        cp[0] = EXIFBitRevTable[cp[0]];
        cp[1] = EXIFBitRevTable[cp[1]];
        cp[2] = EXIFBitRevTable[cp[2]];
        cp[3] = EXIFBitRevTable[cp[3]];
        cp[4] = EXIFBitRevTable[cp[4]];
        cp[5] = EXIFBitRevTable[cp[5]];
        cp[6] = EXIFBitRevTable[cp[6]];
        cp[7] = EXIFBitRevTable[cp[7]];
        cp += 8;
    }
    while (n-- > 0)
        *cp = EXIFBitRevTable[*cp], cp++;

    return ;
}

}




