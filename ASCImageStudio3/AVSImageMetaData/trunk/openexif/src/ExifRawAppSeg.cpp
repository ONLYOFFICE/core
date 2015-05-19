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
 * Creation Date: 02/19/2003
 *
 * Original Author: 
 * Sam Fryer <samuel.fryer@kodak.com>
 *
 * Contributor(s): 
 * Ricardo Rosario <ricardo.rosario@kodak.com>
 */ 


#if (defined _MSC_VER)
#pragma warning( disable : 4786 )
#endif

#include "ExifIO.h"
#include "ExifRawAppSeg.h"

// This is registered as a default so that it will automatically
// be chosen for any app segment that we don't have another registered 
// class for.
// EK_PROVIDE(ExifRawAppSeg, EXIF_APP_SEG_TYPE_INIT("default", "RAW", 0x0000, ExifRawAppSeg))

namespace Exif{
// Copy Constructor
ExifRawAppSeg::ExifRawAppSeg( const ExifRawAppSeg& theSrc )
  : ExifAppSegment(theSrc), mRawData(NULL)
{
    setRawData(theSrc.mRawData, theSrc.mLength) ;
}

// Routine to initialize the class and read the data in from a file
ExifStatus ExifRawAppSeg::init( ExifIO* exifio, uint16 _length,
    exifoff_t _exifHeaderOffset  )
{
    ExifAppSegment::init( exifio, _length, _exifHeaderOffset ) ;
    
    // the exifio's current position should be the AppSeg Marker. Seek
    // to right after that...
    exifio->seek(2, SEEK_CUR) ;
    return read( exifio ) ;
}

// routine to copy the class
void ExifRawAppSeg::copy( const ExifRawAppSeg* theSrc )
{
    if (mRawData) delete[] mRawData;
    *this = *theSrc ;
    mRawData = new char[mLength];
    memcpy(mRawData,theSrc->mRawData, mLength);

    return ;
}

// routine to set the data to new data
void ExifRawAppSeg::setRawData( const char* theData,
    const unsigned int theLength)
{
    if (mRawData) delete[] mRawData;
    mRawData = new char[theLength];
    if (theData != NULL)
    {
        memcpy(mRawData,theData,theLength);
        mLength = theLength;
    }
    else 
        mLength = 0;
    
    return ;
}

// routine to write the data out via the exifio write method.
ExifStatus ExifRawAppSeg::write( ExifIO* exifio )
{
    exifio->swabShort( &mMarker ) ;
    exifio->write( &mMarker, sizeof(uint16) ) ;

    exifio->write( mRawData, mLength );

    return EXIF_OK ;
}

// routine to read the data in via the exifio read method
ExifStatus ExifRawAppSeg::read( ExifIO* exifio )
{
    ExifStatus status = EXIF_OK;
    mRawData = new char[mLength];
    if ( mRawData )
    {
        if( exifio->read( mRawData, mLength ) != mLength)
            status = EXIF_FILE_READ_ERROR ;
    }
    else
        status = EXIF_ERROR ;

    return status ;
}

}