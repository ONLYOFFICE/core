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


#ifndef _EXIF_FILE_IO_H_
#define _EXIF_FILE_IO_H_

#include "ExifErrorCodes.h"
#include "ExifIO.h"

/*!
  \author  George Sotak <george.sotak@kodak.com>
  \date   Sun Jan 20 16:26:06 2002
  
  \brief  Implementation of the File IO.

  This class is never used directly, all access is through
  the ExifIO class.  
  
*/
namespace Exif 
{

class ExifFileIO : public ExifHandleIO
{   
    public:
        //! Constructor
        /*!
          \param theParent   handle to the ExifIO parent instance
          \param fhandle     handle to the file itself
        */
        ExifFileIO( ExifIO& theParent, fhandle_t& fhandle )
            : ExifHandleIO(theParent), mFileHandle(fhandle) {}

        //! Destructor
        ~ExifFileIO( ) {}
        
        //! read size bytes from the file into the buf
        virtual tsize_t read( tdata_t buf, tsize_t size ) ;

        //! write size bytes from the buf into the file
        virtual tsize_t write( const tdata_t buf, const tsize_t size ) ;

        //! seek to the specified byte offset in the file
        /*!
          \param off    byte off to which to seek
          \param whence indication of where offset is relative to

          Values for whence:
             SEEK_SET - offset is relative to start of file
             SEEK_CUR - offset is relative to the current file pointer position
             SEEK_END - offset is relative to end of file
          \code 
          seek( 0, SEEK_SET) - seek to the beginning of the file
          seek( 0, SEEK_END) - seek to the end of the file
          \endcode
        */
        virtual exifoff_t seek( exifoff_t off, int whence ) ;

        //! returns the size of the file in bytes
        virtual exifoff_t size() ;

        //! close the file
        virtual ExifStatus close();

    protected:
        fhandle_t& mFileHandle ;
        
};
}
#endif // _EXIF_FILE_IO_H_
