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
 * Sam Fryer samuel.fryer@kodak.com
 */ 

#ifndef _EXIF_TIFF_APP_SEG_H
#define	_EXIF_TIFF_APP_SEG_H

#include <vector>
#include <string>

#include "ExifConf.h"
#include "ExifTypeDefs.h"
#include "ExifAppSegment.h"
#include "ExifTagEntry.h"
#include "ExifIFD.h"
#include "ExifIO.h"
namespace Exif 
{

/*!
  \author George Sotak <george.sotak@kodak.com>
  \date   Mon Jan 21 06:43:31 2002
  
  \brief  Provides the interface to Tiff-based application segments
  
  This class may be used for any Tiff-based application segment. The
  toolkit automatically uses it for Application Segment 1 - "Exif". To
  use it for other application segments, register it with the application
  segment factory with the appropriate application segment marker and
  identification string. See documentation on registering application
  segements for details.
  
*/

class EXIF_DECL ExifTiffAppSeg : public ExifAppSegment
{
    public:
	    typedef std::vector<ExifIFD*> ExifIFDVec;
        typedef ExifIFDVec::iterator ExifIFDVecIter;

	    //! Constructor.
        ExifTiffAppSeg( uint16 _marker, const char* _ident ) 
            : ExifAppSegment( _marker, _ident)
            { }

        //! Constructor
        ExifTiffAppSeg( ExifIO* exifio, uint16 _marker, uint16 _length,
            char* _ident, exifoff_t _exifHeaderOffset ) 
            : ExifAppSegment( _marker, _length, _ident, _exifHeaderOffset)
              
            { init( exifio ) ; }

        /*  Copy constructor
        
            This constructor creates a deep copy of the source.
         */
 //       ExifTiffAppSeg( const ExifTiffAppSeg& theSrc ) ;

        //! Destructor
        virtual ~ExifTiffAppSeg();
 
        //! Factory method - creates a copy of the ExifTiffAppSeg.
        inline virtual ExifAppSegment* clone() const
        {
            return new ExifTiffAppSeg(*this);
        }

        // declare that we are a TIFF App Segment!
        virtual bool isTiff()
        { return true; }
        
        //! Initialize the application segment after factory instantiation
        virtual ExifStatus init( ExifIO* exifio, uint16 _length,
            exifoff_t _exifHeaderOffset ) ;

	    /*! Get an existing main IFD corresponding to the provided index.
          
            \param index - The index of main IFD.
            \return A pointer to the ExifIFD object if the IFD found;
                    otherwise NULL.
        */
	    ExifIFD* getIFD( uint16 index) const ;
	
	    /*! Get an existing main or sub IFD.
        
            \param dirPath - Directory path of the IFD to retrieve. 
	        \return A pointer to the ExifIFD object if the IFD found; otherwise NULL.
        */
        ExifIFD* getIFD( const ExifIFDPath& ifdPath ) const ;

	    /*! Create a main IFD.
        
            \return A pointer to the ExifIFD object just created.

            The index of main IFD starts from 0 and increments by 1 with each 
            call to this function.
        */
	    ExifIFD* createIFD( void );

        /*! Create main/sub IFDs specified in the ExifIFDPath.
        
            \param dirPath IFDs to create, specified in ExifIFDPath format

            If the IFDs already exists then nothing happens. Note that each 
            of the IFD indexes in the ExifIFDPath should be less or equal to 
            the next available index for the corresponding IFD tag
        */
	    ExifStatus createIFD( const ExifIFDPath& dirPath);

	    //! Return the number of main directories.
	    inline int numMainIFDs() { return mMainIFDs.size(); }

        /*! Get all the metadata tags in this application segment
        
            \param pathsTags the metadata retrieved from this application segment
            \sa ExifImageFile::getAllTags()

            This is here to support ExifImageFile::getAllTags().
        */
        void getAllTags( ExifPathsTags &pathsTags ) ;
        
        /*! Support for read access to TIFF tag based metadata.
        
            \param tag       the tag number to retrieve.
            \param ifdPath   the IFD path to the tag
            \return pointer to an ExifTagEntry holding the info for the
                    requested tag if found, otherwise NULL
        */
        ExifTagEntry* getGenericTag( exiftag_t tag,
            const ExifIFDPath& ifdPath ) const;

        /*! Set all the metadata tags provided into this application segment
          
            \param pathsTags the metadata to be set into the specified
                           application segment
            \sa ExifImageFile::setAllTags()

            This is here to support ExifImageFile::setAllTags().
        */
        ExifStatus setAllTags( const ExifPathsTags &pathsTags ) ;
        
        /*! Support for write access to TIFF tag based metadata.
          
            \param entry     an instance of ExifTagEntry holding the metadata
                           info to be written
            \param idfPath   the IFD path to the tag
            \return errRtn   error return value
        */
        ExifStatus setGenericTag( const ExifTagEntry& entry,
            const ExifIFDPath& ifdPath  ) ;
        
        /*! Remove the specified tag from the specified IFD
        
            \param tag       the tag number to retrieve.
            \param ifdPath   the IFD path to the tag
         */
        void removeGenericTag( exiftag_t tag, const ExifIFDPath& ifdPath );
            
        /*! Write out the App Segment to the file
        
            \param exifio - handle to the ExifIO to where we will write
         */
        virtual ExifStatus write( ExifIO* exifio ) ;
                
    private:
        ExifTiffHeader mHeader ;
        ExifIFDVec     mMainIFDs;

        ExifStatus init( ExifIO* exifio ) ;
        ExifStatus readHeader( ExifIO* exifio ) ;
        ExifStatus writeHeader( ExifIO* exifio, exifoff_t& tiffHeaderOffset ) ;

        ExifStatus initEndian( ExifIO* exifio, bool bigendian ) ;

        void initHeader( ExifIO* exifio, bool bigendian ) ;
        void readIFDs( ExifIO* exifio );
        exifoff_t writeIFDs( ExifIO* exifio, exifoff_t& tiffHeaderOffset );
};
}

#endif // _EXIF_TIFF_APP_SEG_H

