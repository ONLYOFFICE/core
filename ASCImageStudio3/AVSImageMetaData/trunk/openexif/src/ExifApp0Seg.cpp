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

#include "ExifIO.h"
#include "ExifApp0Seg.h"

namespace Exif
{

ExifApp0Seg::~ExifApp0Seg()
{

}


ExifStatus ExifApp0Seg::init( ExifIO* exifio, uint16 _length, exifoff_t _exifHeaderOffset  )
{
    ExifAppSegment::init( exifio, _length, _exifHeaderOffset ) ;
    
    /*
     *   Default is to return data MSB2LSB
     */
    setMyExifioFlags( exifio ) ;
    
    ExifStatus status = read( exifio );
     
    restoreExifioFlags( exifio ) ;
    
    return  status;
}

// Saves current ExifIO flags and set them to mMyExifioFlags
void ExifApp0Seg::setMyExifioFlags( ExifIO* exifio )
{
    mSavedExifioFlags = exifio->flags() ;
    mSavedEndianState = exifio->bigEndian() ;

    // This is a major hack and should be re-examined.
    if ( !HOST_BIGENDIAN )
        exifio->flags() |= EXIF_SWAB;
}


void ExifApp0Seg::copy( const ExifApp0Seg* theSrc )
{
    *this = *theSrc ;

    return ;
}



ExifStatus ExifApp0Seg::write( ExifIO* exifio )
{
    exifio->swabShort( &mMarker ) ;
    exifio->write( &mMarker, sizeof(uint16) ) ;

    uint16 length = 16 ;
    exifio->swabShort( &length ) ;
    exifio->write( &length, sizeof(uint16) ) ;

    exifio->write( &mIdent[0], 5*sizeof(char) ) ;

    exifio->swabShort( &mVersion ) ;
    if( exifio->write( &mVersion, sizeof(uint16) ) != sizeof(uint16))
        return EXIF_FILE_WRITE_ERROR ;
    if( exifio->write( &mUnitsId, sizeof(uint8) ) != sizeof(uint8))
        return EXIF_FILE_WRITE_ERROR ;

    exifio->swabShort( &mXdensity ) ;
    if( exifio->write( &mXdensity, sizeof(uint16) ) != sizeof(uint16))
        return EXIF_FILE_WRITE_ERROR ;

    exifio->swabShort( &mYdensity ) ;
    if( exifio->write( &mYdensity, sizeof(uint16) ) != sizeof(uint16))
        return EXIF_FILE_WRITE_ERROR ;

    if( exifio->write( &mXthumbnail, sizeof(uint8) ) != sizeof(uint8))
        return EXIF_FILE_WRITE_ERROR ;

    if( exifio->write( &mYthumbnail, sizeof(uint8) ) != sizeof(uint8))
        return EXIF_FILE_WRITE_ERROR ;

    return EXIF_OK ;
}

ExifStatus ExifApp0Seg::read( ExifIO* exifio )
{
    if( exifio->read( &mVersion, sizeof(uint16) ) != sizeof(uint16))
        return EXIF_FILE_READ_ERROR ;
    exifio->swabShort( &mVersion ) ;

    if( exifio->read( &mUnitsId, sizeof(uint8) ) != sizeof(uint8))
        return EXIF_FILE_READ_ERROR ;

    if( exifio->read( &mXdensity, sizeof(uint16) ) != sizeof(uint16))
        return EXIF_FILE_READ_ERROR ;
    exifio->swabShort( &mXdensity ) ;

    if( exifio->read( &mYdensity, sizeof(uint16) ) != sizeof(uint16))
        return EXIF_FILE_READ_ERROR ;
    exifio->swabShort( &mYdensity ) ;
   
    if( exifio->read( &mXthumbnail, sizeof(uint8) ) != sizeof(uint8))
        return EXIF_FILE_READ_ERROR ;

    if( exifio->read( &mYthumbnail, sizeof(uint8) ) != sizeof(uint8))
        return EXIF_FILE_READ_ERROR ;

    return EXIF_OK ;
}
}