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

#ifndef _EXIF_IO_H_
#define _EXIF_IO_H_

#include <cstdio>
#include <string>

#include "ExifErrorCodes.h"
#include "ExifTypeDefs.h"
namespace Exif 
{

typedef	FILE* fhandle_t;	/* client data handle */


class ExifIO;



/*!
  \author George Sotak <george.sotak@kodak.com>
  \date   Sun Jan 20 16:44:00 2002
  
  \brief  Base class for FileIO implementations
  
  OpenExif provides support for accessing Exif Image File data
  through either tradition file io (read / write from / to a file
  descriptior), and access to a real memory buffer holding the 
  image file data.
  
*/

class ExifHandleIO
{
    public:

        //! Constructor
        ExifHandleIO( ExifIO& theParent)
            : mParent( theParent ) {}

        //! Destructor
        virtual ~ExifHandleIO( ) {}

        //! read size bytes from the file into the buf
        virtual tsize_t read( tdata_t buf, tsize_t size ) = 0;

        //! write size bytes from the buf into the file
        virtual tsize_t write( const tdata_t buf, const tsize_t size ) = 0;

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
        virtual exifoff_t seek( exifoff_t off, int whence ) = 0;

        //! returns the size of the file in bytes
        virtual exifoff_t size() = 0;

        //! close the file
        virtual ExifStatus close() = 0;

        virtual uint8* base( void ) const
            { return NULL ; }
        virtual tsize_t memSize() const
            { return 0; }

    protected:
        ExifIO& mParent;

};

/*!
  \author George Sotak <george.sotak@kodak.com>
  \date   Sun Jan 20 16:44:00 2002
  
  \brief  Interface class for FileIO implementations
  
  
*/

class EXIF_DECL ExifIO
{
    public:

        //! Constructor to use when dealing with a file handle
        /*!
            \param status - Return of the error status
            \param theFileHandle - file handle
            \param filename - name of the file
            \param cmode - open mode
         */
        ExifIO(ExifStatus& status, fhandle_t theFileHandle,
            const std::wstring& filename, const char* cmode );



        //! Constructor to use when dealing with a memory buffer
        /*!
            \param status - Return of the error status
            \param imgBuf - buffer containing the image file
            \param bufSize - size of the buffer
            \param cmode - open mode
            \param bytesWritten - The actual size of the file within the buffer.
                                  If no value is passed, it will be assumed to
                                  be equal to bufSize for "r" and "r+" modes and
                                  equal to bufSize for "w" mode.
         */
        ExifIO(ExifStatus& status, void* imgBuf, tsize_t bufSize,
            const char* cmode, tsize_t bytesWritten = -1 );

#ifdef WIN32
#ifdef INTERNET_REQUIRED
        //! Access to the file over the network
        ExifIO( ExifStatus& status, HINTERNET hInet, const string& filename,
            const char* cmode) ;
#endif // INTERNET_REQUIRED
#endif // WIN32

        //! Destructor
        ~ExifIO( void ) { close(); }

        //! Retrieve a handle to the IO-Handler
        ExifHandleIO& ioHandler( void ) const
            { return *mHandleIO ; }
        
        //! Open a file and return a handle to the ExifIO instance
        /*!
          \param status  return of the error status
          \param filename  string holding the path to the file and its name
          \param mode    the file mode with which to open the file

          \sa ExifOpenFile::open() for details on mode
        */
        static ExifIO* open( ExifStatus& status, const char* filename,
            const char* mode );
        
        static ExifIO* open( ExifStatus& status, const wchar_t* filename,
            const char* mode );


        // A lot of classes use this method, so leave
        // it here even if memory mapping is disabled.
        // It returns the correct result in either case.
        //! \name Memory Mapped File IO Management
        //@{
        //! returns true if file is currently memory mapped, otherwise false
        bool isMapped() const
            { return (mFlags & EXIF_MAPPED)?true:false ; }

        //@}

        //! set / get accessor for ExifIO control flags
        /*!
          See ExifDefs.h for flag definitions
        */
        exif_uint32& flags( void ) 
            { return mFlags; }

        //! Retrieve the integer version of the file mode
        const exif_uint32& mode( void ) const
            { return mMode; }

        //! Retrieve the filename that is currently being accessed
        const std::wstring& name() const
            { return mFileName; }

        fhandle_t& getFileHandle( void )
            {return mFileHandle; }
        FILE* getFilePtr( void ) ;

        //! see ExifHandleIO::read()
        tsize_t read( tdata_t buf, tsize_t size )
            { return mHandleIO->read( buf, size ); }

        //! see ExifHandleIO::write()
        tsize_t write( const tdata_t buf, const tsize_t size )
            { return mHandleIO->write( buf, size ); }

        //! see ExifHandleIO::seek()
        exifoff_t seek( exifoff_t off, int whence )
            { return mHandleIO->seek( off, whence ); }

        //! close the file
        void close( void );

        //! return the size of the file in bytes
        exifoff_t size( void );
        
        //! \name get / set accessors for endianess
        //@{
        bool bigEndian( void ) const
            { return mBigEndian; }
        void bigEndian( bool state )
            { mBigEndian = state;  return; }
        //@}
        
        //! returns true if swabbing of bytes needs to be done, otherwise false
        bool doSwab( void ) const
            { return (mFlags & EXIF_SWAB)?true:false; }
        
        //! \name Helper methods for byte swabbing
        //@{
        void swabShort( uint16* wp );
        void swabLong( exif_uint32* lp );
        void swabArrayOfShort( uint16* wp, exif_uint32 n );
        void swabArrayOfLong( exif_uint32* lp, exif_uint32 n );
        void swabDouble( double *dp );
        void swabArrayOfDouble( double* dp, exif_uint32 n );
        //@}
        
    private:
        ExifHandleIO* mHandleIO;
    
        exif_uint32  mFlags;
        std::wstring  mFileName;
        exif_uint32  mMode;
        std::string  mCMode ;
        bool    mBigEndian;

        fhandle_t  mFileHandle;

        ExifIO( fhandle_t theFileHandle, const std::string& filename,
            const char* cmode, exif_uint32 imode );
        ExifIO( fhandle_t theFileHandle, const std::wstring& filename,
            const char* cmode, exif_uint32 imode );
        static ExifStatus convertMode( const char* cmode, exif_uint32 &imode );
};
}
#endif // _EXIF_IO_H_
