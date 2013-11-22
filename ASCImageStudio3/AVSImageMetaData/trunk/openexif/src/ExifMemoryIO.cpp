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


#include "ExifConf.h"
#include "ExifMemoryIO.h"
namespace Exif{
// Seek to the specified offset
exifoff_t ExifMemoryIO::seek( exifoff_t off, int whence )
{
    switch(whence)
    {
        case SEEK_CUR:
            mCurBufOffset += off;
            break;
        case SEEK_END:
            mCurBufOffset = mBytesWritten + off;
            break;
	    case SEEK_SET:
        default:
            mCurBufOffset = off;
            break;
    }

    // Make sure mCurBufOffset is within the boundaries
    if ( mCurBufOffset < 0 )
        mCurBufOffset = 0 ;
    else if ( mCurBufOffset > mBytesWritten )
        mCurBufOffset = mBytesWritten ;

    return mCurBufOffset ;
}

// Read from the memory buffer
tsize_t ExifMemoryIO::read( tdata_t buf, tsize_t size )
{
    tsize_t returnValue = -1;
    if ( (mParent.mode() == O_RDONLY) || (mParent.mode() == O_RDWR) )
    {
        if (mCurBufOffset + size <= (exifoff_t)mBytesWritten)
        {
            memcpy( (char*)buf, mImageBuffer + mCurBufOffset, size ) ;
            mCurBufOffset += size ;
            returnValue = size ;
        }
    }

    return returnValue ;
}

// Write to the memory buffer
tsize_t ExifMemoryIO::write( tdata_t buf, tsize_t size )
{
    tsize_t returnValue = -1 ;
    if ( (mParent.mode() == O_RDWR) || (mParent.mode() == O_CREAT) )
    {
        if (mCurBufOffset + size <= (exifoff_t)mBufferSize)
        {
            memcpy( mImageBuffer + mCurBufOffset, (char*)buf, size ) ;
            mCurBufOffset += size ;
            if (mCurBufOffset > (exifoff_t)mBytesWritten)
                mBytesWritten = mCurBufOffset ;
            returnValue = size ;
        }
    }

    return returnValue ;
}

// Compute and return the size of the memory file/buffer
exifoff_t ExifMemoryIO::size()
{
    return mBytesWritten ;
}

// Close the memory buffer (does nothing)
ExifStatus ExifMemoryIO::close()
{
    return EXIF_OK ;
}

}