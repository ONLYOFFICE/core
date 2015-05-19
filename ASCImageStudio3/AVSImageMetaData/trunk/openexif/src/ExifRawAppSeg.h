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
 * Sam Fryer samuel.fryer@kodak.com 
 *
 * Contributor(s): 
 * Ricardo Rosario ricardo.rosario@kodak.com
 */ 


#ifndef _EXIF_RAW_APP_SEG_H
#define	_EXIF_RAW_APP_SEG_H


#include "ExifConf.h"
#include "ExifTypeDefs.h"
#include "ExifAppSegment.h"
namespace Exif 
{

/*!
  \author  Sam Fryer <samuel.fryer@kodak.com>
  \date   Wed Feb 19 16:42:26 EST 2003
  
  \brief  Interface to a raw Application Segment

  the Raw Application Segment holds the following information:
  <ul>
  <li> rawData - the raw data for the app segment from the file.
  </ul>
  
*/


class ExifRawAppSeg : public ExifAppSegment
{
    public:

        //! Constructor for direct instantiation
        ExifRawAppSeg( uint16 _marker, const char* _ident ) 
            : ExifAppSegment( _marker, _ident), mRawData(NULL)
            { }

        //! Copy Constructor
        ExifRawAppSeg( const ExifRawAppSeg& theSrc ) ;

        //! Destructor
        virtual ~ExifRawAppSeg()
        { if (mRawData) delete[] mRawData; }

        //! Factory method - return a copy of the ExifRawAppSeg.
        inline virtual ExifAppSegment* clone() const
        {
            return new ExifRawAppSeg(*this);
        }       

        // Yes, this is a raw app segment!
        virtual bool isRaw()
        { return true; }
        
        //! Init the instance after instantiation from the factory
        /*!
            The exifio's current position must be the beginning of the App
            Segment (AppSegment marker) when init is called.
        
          \param exifio    handle to the ExifIO, provides access to the file
          \param _length   length of the app segment in the file
          \param _exifHeaderOffset  offset due to the Exif header
        */
        virtual ExifStatus init( ExifIO* exifio, uint16 _length,
            exifoff_t _exifHeaderOffset ) ;

        //! Copy an raw app segment 
        /*!
          \param theSrc    source raw app segment
        */
        void copy( const ExifRawAppSeg* theSrc ) ;

        //! @name return the raw data
        //@{
        char * getRawData() const { return mRawData ; }
        //@}


        //! setRawData -- set the raw data
        /*!
          set the raw data
          \param theData   data used to initialize the raw data
          \param theLength   length of the data (in bytes)
	     */
        void setRawData( const char * theData, const unsigned int theLength ) ;

        //! Write 
        /*!
            Write the raw data.
            
	        \param exifio    handle to the ExifIO class
	    */
        virtual ExifStatus write( ExifIO* exifio ) ;
    
    protected:
        //! read the application segment from the file
        ExifStatus read( ExifIO* exifio ) ;
        
    private:
		
        //! The raw data from the file
        char * mRawData ;
		
};
}
#endif // _EXIF_RAW_APP_SEG_H

