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


#ifndef _EXIF_MEMORY_IO_H_
#define _EXIF_MEMORY_IO_H_

#include "ExifIO.h"

/*!
  \author  George Sotak <george.sotak@kodak.com>
  \date   Sun Jan 20 18:22:48 2002
  
  \brief  Provides file-type access to a memory buffer.
  
*/
namespace Exif 
{
class ExifMemoryIO : public ExifHandleIO
{
    public:
        //! Constructor
        ExifMemoryIO( ExifIO& theParent, void* imgBuf, tsize_t bufSize,
            tsize_t bytesWritten )
          : ExifHandleIO(theParent), mImageBuffer((unsigned char*)imgBuf), 
            mBufferSize(bufSize), mBytesWritten(bytesWritten),
            mCurBufOffset(0) {}

        //! Destructor
        ~ExifMemoryIO( ) {}

        //! see ExifHandleIO::read()
        virtual tsize_t read( tdata_t buf, tsize_t size ) ;

        //! see ExifHandleIO::write()
        virtual tsize_t write( const tdata_t buf, const tsize_t size ) ;

        //! see ExifHandleIO::seek()
        virtual exifoff_t seek( exifoff_t off, int whence ) ;

        //! see ExifHandleIO::size()
        virtual exifoff_t size() ;

        //! close it up
        virtual ExifStatus close();

        virtual inline uint8* base() const { return mImageBuffer; }
        virtual inline tsize_t memSize() const { return mBufferSize; }

    protected:
        uint8* mImageBuffer;
        tsize_t	mBufferSize;
        tsize_t	mBytesWritten;
        exifoff_t	mCurBufOffset;

        void memCopy( void* d, const tdata_t s, tsize_t c ) ;


};
}

#endif // _EXIF_MEMORY_IO_H_

