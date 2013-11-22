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

#ifndef _EXIF_STRIP_IMAGE_H_
#define _EXIF_STRIP_IMAGE_H_

#include "ExifConf.h"
#include "ExifTypeDefs.h"
#include "ExifErrorCodes.h"
namespace Exif 
{

class ExifIO ;
class ExifIFD ;

/*!
  \author  George Sotak <george.sotak@kodak.com>
  \date   Mon Jan 21 17:03:38 2002
  
  \brief  Provides the functionality to read/write uncompressed thumbnails

  This class is used internally by ExifIFD to write uncompressed thumbnails.
  Compressed thumbnails are done with ExifJpegImage.
*/

class ExifStripImage
{
    public:

        //! Constructor
        /*!
          \param tdir Pointer to the IFD object containing this image.
          \param tio  Pointer to the TiffIO object.
        */
        ExifStripImage(ExifStatus& status, ExifIFD* tdir, ExifIO* tio) ;

        //! Destructor
        ~ExifStripImage() ;

        //! Write the image
        ExifStatus writeImage( ExifImageDesc &imgDesc ) ;
        //! Read the image
        ExifStatus readImage( ExifImageDesc &imgDesc ) ;
        
        //! Write out a single scanline
        /*!
          \param buf    Data buffer.
          \param row    The row number of the scanline.
          \param sample Indicate which band of data.
          \return       1 if succeed; otherwise zero.
        */
        ExifStatus writeScanline(tdata_t buf, exif_uint32 row,
            tsample_t sample = 0);

        // Read in a specified scanline
        /*!
          \param buf    Data buffer.
          \param row    The row number of the scanline.
          \param sample Indicate which band of data.
          \return 1 if succeed; otherwise zero.
        */
        ExifStatus readScanline(tdata_t buf, exif_uint32 row, tsample_t sample = 0);

    protected:

        ExifStatus init();

        // Write the supplied data to the specified strip.
        //!param: strip - Strip number.
        //!param: data - Data buffer.
        //!param: cc - The count of data in byte.
        //!return: The number of bytes wrote.
        exif_int32 writeRawStrip(exif_uint32 strip, tdata_t data, exif_int32 cc);

        // Compute which strip a (row, sample) value is in.
        //!param: row - Row number.
        //!param: sample - Sample number.
        exif_uint32 computeStrip(exif_uint32 row, tsample_t sample, ExifStatus& errRtn);

        // Compute the number of bytes in a variable height, row-aligned strip.
        //!return: Size in bytes
        exif_int32 vStripSize(exif_uint32 nrows);

        // Compute the number of bytes in a (row-aligned) strip.
        //!return: Size in bytes
        exif_int32 stripSize();

       // Setup strip/tile offsets
        int setupOffsets();

        ExifStatus writeCheck(int tiles);

        // Append to strip/tile
        ExifStatus appendToStrip(exif_uint32 strip, tdata_t data, exif_int32 cc);

        // Grow strips/tiles
        int growStrips(int delta);

        //!i: methods for reading
        ExifStatus checkRead(int tiles);

        void reverseBits( tdata_t buf, tsize_t nBytes ) ;

    private:
        //!i: data members
        ExifIFD*        mExifdir;
        ExifIO*         mExifio;

        exif_uint32        mStripsperimage; 
        exif_uint32        mNStrips;        // size of the strip/tile offset
                                         // array, can grow

        exif_int32     mScanlinesize;
        exif_uint32    mCurstrip;   /* current strip for read/write */

};
}
#endif // _EXIF_STRIP_IMAGE_H_



