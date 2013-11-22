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


#include <string>

#include "ExifConf.h"
#include "ExifErrorCodes.h"
#include "ExifTypeDefs.h"
namespace Exif 
{

class ExifIO ;

//! Base class providing file open / close facilities
/*!
  \author  George Sotak <george.sotak@kodak.com>
  \date   Sat Jan 19 18:51:24 2002

  For details on usage see ExifImageFile or ExifJpegImage
*/

class EXIF_DECL ExifOpenFile
{
    public:
 
        //! The destructor
        virtual ~ExifOpenFile();
        
        //! Open an image file.
        /*! 
          \param name - Name of the image file.
          \param cmode - Open mode.

          Possible Values for cmode (combinations also allowed):
          "r"    open for reading, file must exist else an error
          "r+"   open for read / write, file must exist else an error
          "w"    open for writing only, create file if does not exist,
                 truncate if it does.

          "l"    use little-endian byte order for creating a file.
          "b"    use big-endian byte order for creating a file
          "L"    read/write information using LSB2MSB bit order
          "B"    read/write information using MSB2LSB bit order
          "H"    read/write information using host bit order
        */
        ExifStatus open(const char* filename, const char* cmode);
		ExifStatus open(const wchar_t* filename, const char* cmode);
        
        //! Open an image file with the given file handle. 
        /*! 
          \param fileHandle File handle.
          \param name Name of the image file. It's not actually used to open
                      the file, just for the information purpose.
          \param cmode Open mode
          \sa ExifOpenFile::open(const char* name, const char* cmode) for
              details on cmode.
         */
        ExifStatus open(fhandle_t fileHandle, const char* name, const char* cmode);
		ExifStatus open(fhandle_t fileHandle, const wchar_t* name, const char* cmode);

        //* Open an in-memory image file.
        /*!
          \param imgBuf - buffer containin the image file
          \param bufSize - Size of image buffer.
          \param cmode - Open mode.
          \param bytesWritten - The actual size of the file within the buffer.
                                If no value is passed, it will be assumed to be
                                equal to bufSize for "r" and "r+" modes and
                                equal to bufSize for "w" mode.
          \sa ExifOpenFile::open(const char* name, const char* cmode) for
              details on cmode.
         */
        ExifStatus open(void* imgBuf, tsize_t bufSize, const char* cmode,
            tsize_t bytesWritten = -1);

#ifdef WIN32
#ifdef INTERNET_REQUIRED
        //! Open an image file with the supplied internet file handle.
        /*!
          \param hInternet - Internet file handle.
          \param name - File name, just for information purpose.
        */
        ExifStatus open(HINTERNET hInet, const char* name);
#endif // INTERNET_REQUIRED
#endif // WIN32
    
        //! Close the image file.
        virtual ExifStatus close( void ) ;

    protected:
        ExifIO*    mExifio;
        bool       mIOwnExifIO ;
        std::wstring     mFileName;

        ExifOpenFile( void ) : mExifio(NULL), mIOwnExifIO(false) {}
        ExifOpenFile( ExifIO* exifIO ) : mExifio(exifIO), mIOwnExifIO(false) {}

        virtual ExifStatus initAfterOpen( const char* cmode ) = 0 ;
};
}
