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
 * FirstName LastName <email address>
 * FirstName LastName <email address>
 */ 

#ifndef _EXIF_APP0_SEG_H
#define	_EXIF_APP0_SEG_H


#include <vector>
#include <string>


#include "ExifConf.h"
#include "ExifTypeDefs.h"
#include "ExifAppSegment.h"

namespace Exif
{
class ExifIO ;
class ExifApp0Seg ;

/*!
  \author  George Sotak <george.sotak@kodak.com>
  \date   Sun Jan 20 15:57:47 2002
  
  \brief  Interface to Application Segment 0

  Application Segment 0 holds the following information:
  - Version - the most significant byte is used for major revisions,
              the least significant is used for minor revisions.
  - Units - Units X and Y densities. Default: 0x00
  - Xdensity - Horizontal pixel density. Default: 0x0001
  - Ydensity - Vertical pixel density. Default: 0x0001
  - Xthumbnail - thumbnail horizontal pixel count. Default: 0x00
  - Ythumbnail - thumbnail horizontal pixel count. Default: 0x00
*/

class ExifApp0Seg : public ExifAppSegment
{
    public:

        //! Constructor for direct instantiation
        ExifApp0Seg( uint16 _marker, const char* _ident ) 
            : ExifAppSegment( _marker, _ident), mVersion(0), mUnitsId(0), 
              mXdensity(1), mYdensity(1), mXthumbnail(0), mYthumbnail(0)
            { }

        //! Destructor
        virtual ~ExifApp0Seg();
        
        //! Factory method - return a copy of the ExifApp0Seg.
        inline virtual ExifAppSegment* clone() const
        {
            return new ExifApp0Seg(*this);
        }       

        /*! Init the instance after instantiation from the factory
        
            \param exifio  handle to the ExifIO class, provides access to the file
            \param _length  length of the app segment in the file
            \param _exifHeaderOffset  offset due to the Exif header
        */
        virtual ExifStatus init( ExifIO* exifio, uint16 _length,
            exifoff_t _exifHeaderOffset ) ;

        /*! Copy an app segment 0
        
            \param theSrc    source app segment 0
        */
        void copy( const ExifApp0Seg* theSrc ) ;

        //! \name get / set accessors for the version field
        //@{
        const uint16& version() const { return mVersion ; }
        uint16& version() { return mVersion ; }
        //@}

        //! @name get / set accessors for the density units field
        //@{
        const uint8& unitsId() const { return mUnitsId ; }
        uint8& unitsId() { return mUnitsId ; }
        //@}

        //! @name get / set accessors for the x,y density fields
        //@{
        const uint16& xDensity() const { return mXdensity ; }
        uint16& xDensity() { return mXdensity ; }

        const uint16& yDensity() const { return mYdensity ; }
        uint16& yDensity() { return mYdensity ; }
        //@}

        //! @name get / set accessors for the x,y thumbnail fields
        //@{
        const uint8& xThumbnail() const { return mXthumbnail ; }
        uint8& xThumbnail() { return mXthumbnail ; }

        const uint8& yThumbnail() const { return mYthumbnail ; }
        uint8& yThumbnail() { return mYthumbnail ; }
        //@}

        virtual ExifStatus write( ExifIO* exifio ) ;
    
    protected:
        //! read the application segment from the file
        ExifStatus read( ExifIO* exifio ) ;
        
        // Saves current ExifIO flags and set them to mMyExifioFlags
        virtual void setMyExifioFlags( ExifIO* exifio ) ;

    private:
        //! Version - the most significant byte is used for major revisions,
        //! the least significant is used for minor revisions.
        uint16 mVersion;
        
        //! Units - Units X and Y densities. Default: 0x00
        uint8 mUnitsId;
        
        //! Xdensity - Horizontal pixel density. Default: 0x0001
        uint16 mXdensity;
        
        //! Ydensity - Vertical pixel density. Default: 0x0001
        uint16 mYdensity;
        
        //! Xthumbnail - thumbnail horizontal pixel count. Default: 0x00
        uint8 mXthumbnail;
        
        //! Ythumbnail - thumbnail horizontal pixel count. Default: 0x00
        uint8 mYthumbnail;
        
};
}
#endif // _EXIF_APP0_SEG_H

