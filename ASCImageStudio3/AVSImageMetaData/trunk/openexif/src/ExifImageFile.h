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


#ifndef _EXIF_IMAGE_FILE_H_
#define _EXIF_IMAGE_FILE_H_

#if (defined _MSC_VER)
#pragma warning( disable : 4786 )
#endif


#include "ExifConf.h"
#include "ExifTypeDefs.h"
#include "ExifErrorCodes.h"
#include "ExifTags.h"
#include "ExifTagEntry.h"
#include "ExifAppSegment.h"
#include "ExifApp0Seg.h"
#include "ExifTiffAppSeg.h"
#include "ExifJpegImage.h"

#include "ExifAppSegManager.h"
#include "ExifComMarker.h"

#include <vector>
#include <string>

/*!
  \author  George Sotak <george.sotak@kodak.com>
  \date   Sat Jan 19 19:25:42 2002
  \brief  The main interface class to an Exif image file.
  
  
  Example usage:

  \code
  #include "ExifImageFile.h"
  int main()
  {
    char inputFile[]= "KodakDC260.jpg" ;
    char outputFile[]= "copyMd.jpg" ;

    ExifImageFile inImageFile ;
    ExifImageFile outImageFile ;
    
    inImageFile.open( inputFile, "r" ) ;
    outImageFile.open( outputFile, "w" ) ;

    // do things with the image files
    ...

    // close them up
    inImageFile.close() ;
    outImageFile.close() ;
  }
  \endcode

*/
namespace Exif 
{

class EXIF_DECL ExifImageFile : public ExifJpegImage
{
    public:

        //! Default Constructor.
        ExifImageFile() ;

        //! Class Destructor
        virtual ~ExifImageFile() ;

        /*! Probes files to determine if it is an Exif File
        
          \param fileName The filename of the image to be probed
          \return EXIF_OK if an Exif file, EXIF_ERROR otherwise
         */
        static ExifStatus isExifFile(char * fileName) ;
		static ExifStatus isExifFile(wchar_t * fileName) ;
        
        /*! Probes files to determine if it is an Exif File
        
          \param exifio - the exifio containing the image file
          \return EXIF_OK if an Exif file, EXIF_ERROR otherwise
         */
        static ExifStatus isExifFile(ExifIO * exifio) ;
        
        /*! Probes files to determine if it is an Exif File
        
          \param fp -  the file handle
          \return EXIF_OK if an Exif file, EXIF_ERROR otherwise
         */
        static ExifStatus isExifFile(FILE * fp) ;
    
        //! Close the image file, flush the image out to the file if necessary 
        virtual ExifStatus close( void );

        /*! @name Image Pixel Interface
          This collection of methods provides access to the image pixels
          and thumbnail of the Exif image file. The uncompressed image
          is communicated between the application and toolkit through
          the ExifImageDesc structure. The ExifImageInfo structure is
          used to communicate the image's basic parameters (width, height, etc.)
          between the application and toolkit without needing to read / write
          the image pixels.
        */
        //@{

        //! set the image's container information
        virtual ExifStatus setImageInfo(ExifImageInfo &info)
        {
            return mOutImage.setImageInfo(info);
        }
        
        /*! Set JPEG DCT Method used for compression/decompression.
        
            Default value of EXIF_DCT_FAST_INT can be overridden using this
            method.
         */
        virtual void setJpegDCTMethod(ExifJpegDCTMethod dctMeth)
        {   
            mOutImage.setJpegDCTMethod(dctMeth);
        }
        
        /*! Set JPEG Quality Factor used for compression.
        
            Default value of 90 can be overridden using this method. The value
            set must be less or equal to 100.
         */
        virtual void setJpegQuality(uint16 jpegQual)
        {
            mOutImage.setJpegQuality(jpegQual);
        }
        
        /*! Set JPEG Smoothing Factor used
        
            Default value is 0 (no smoothing). The value set should be an
            integer between 1...100 (0 for no smoothing).
        */
        virtual void setSmoothingFactor(uint16 smoothFact)
        {
            mOutImage.setSmoothingFactor(smoothFact);
        }
        
        /*! Set JPEG Optimization
        
            Default is false. Set to True for encoding optimization.
        */
        virtual void setOptimization(bool opt)
        { 
            mOutImage.setOptimization(opt);
        }

        /*! Set JPEG Progression
        
            Default value false. Set to True for simple progression.
        */
        virtual void setProgressive(bool prog)
        { 
            mOutImage.setProgressive(prog);
        }
        
        /*! Set Sample factor = 2, 4, or 8
        
            This corresponds to reading 1/2, 1/4, or 1/8 of image size.
            ExifImageDesc must be set properly during readScanLines to take
            into account scale factor.  Useful for displaying JPEG Thumnails
            from JPEG without real Thumbnail
         */
        virtual ExifStatus setJpegReadSampleFactor(uint16 sampFactor,
            exif_uint32 *outWidth, exif_uint32 *outHeight)
        { 
            return mOutImage.setJpegReadSampleFactor(sampFactor,outWidth,
                                                                    outHeight);
        }
        
        //! Set all the Jpeg tables
        virtual ExifStatus setJpegTables(ExifJpegQuantTable* Q0,
                           ExifJpegQuantTable* Q1, ExifJpegQuantTable* Q2,
                           ExifJpegQuantTable* Q3, ExifJpegHUFFTable* Huff_DC,
                           ExifJpegHUFFTable* Huff_AC,
                           ExifJpegHUFFTable* Huff_DC_Chroma,
                           ExifJpegHUFFTable* Huff_AC_Chroma,
                           ExifJpegTableOrder tableOrder)
        {
            return mOutImage.setJpegTables(Q0,Q1,Q2,Q3,Huff_DC,Huff_AC,
                                    Huff_DC_Chroma,Huff_AC_Chroma,tableOrder);
        }
        
        //! Set a Q table
        virtual ExifStatus setJpegQTable( unsigned char qIndex,
            ExifJpegQuantTable* qTable, uint16 jpegQuality = 90 )
        {
            mOutImage.setJpegQuality(jpegQuality);
            return mOutImage.setJpegQTable(qIndex,qTable);
        }
        
        //! Set chrominance sampling to 4:2:2
        virtual void set422ChromaSampling()
        {
            mOutImage.set422Sampling();
        }

        /*! Write the pixels to the file
          
          \param imgDesc the handle to image pixels, passed in from the
                         application
          \param numLines the number of lines to write
          \param nextLine return the index of the next line to be written
          \param linesCompressed the number of lines compressed (written)

          The application fills in all the values and sets up the image
          pixel buffer pointers of the imgDesc argument. The
          application must also populate a variable of type
          ExifImageInfo with the proper container values of the image
          and pass it into the toolkit via the setImageInfo() method
          prior to calling this method. The image can either be
          written in chunks or as a whole (as far as the file goes,
          the result is the same). If numLines is less than the height
          of the image, then more than one call will be required to
          write the entire image. The return value of linesCompressed
          can be used to verify that the correct number of lines have
          been written. The return value of nextLine is the expected
          scanlne for the start of the next write (it is strictly an
          output value).

          NOTE: the image isn't actually written to the file until the
          the close() is performed. This method actually writes the
          image to a temporary file.
        */
        virtual ExifStatus writeScanLines( ExifImageDesc &imgDesc,
                                exif_uint32 numLines, exif_uint32 &nextLine,
                                exif_uint32& linesCompressed );

        /*! Read the pixels from the file
        
          \param imgDesc the handle to image pixels, passed in from the app
          \param numLines the number of lines to read
          \param nextLine return the index of the next line to be read
          \param linesDecompressed the number of lines decompressed (written)

          The application fills in all the values and setups the image
          pixel buffer pointers of the imgDesc argument. The
          information need to do this is obtained from the
          getImageInfo() method and the structure ExifImageInfo. The
          exifAllocImageDesc() function is provided to do this
          procedure for the application. As with the write, the read
          can be performed either in chunks or as a whole. If numLines
          is less than the height of the image, then more than one
          call will be required to read the entire image. The return
          value of linesCompressed can be used to verify that the
          correct number of lines have been read. The return value
          of nextLine is the expected scanlne for the start of the
          next read (it is strictly an output value).
        */
        virtual ExifStatus readScanLines ( ExifImageDesc &imgDesc, exif_uint32 numLines, 
                                  exif_uint32 &nextLine, exif_uint32& linesDecompressed);

        /*! Get the Thumbnail
        
           This will retrieve the thumbnail from the image file, if it is
           present. If the thumbnail is not already present, then a NULL
           pointer is returned.
        */
        ExifImageDesc* getThumbnail( void )  ;

        /*! Set the Thumbnail for the image
        
          \param thumbNail a pointer to an ExifImageDesc holding the thumbnail
          \param compressIt boolean to indicate whether or not to compress the
                 thumbnail
          
          The thumbnail is placed in the metadata cache and is flushed to the
          file with the rest of the metadata when the file is closed.
         */
        ExifStatus setThumbnail( ExifImageDesc* thumbNail,
            bool compressIt=true ) ;
        //@}
        
        /*! Set an ICC profile
        
            NOTE: This is a work in progress... Currently this only works
            properly when you are recompressing or creating and compressing a
            new image. It must be called before calling the writeScanLines()
            function. The iccBuf must be around until writeScanLines() is called
            (DO NOT DELETE IT!!!).
         */
        virtual ExifStatus setICCProfile(uint8 *iccBuf, exif_uint32 iccSize);


// APP2-FPX AUDIO SUPPORT NOT IMPLEMENTED
        // Imbedded Audio Stream Support. Actual work is provided
        // by the ExifApp2Seg class
        // Check if audio stream in available in APP2
//        bool isAudioStreamAvailable( void ) ;
//        exif_uint32 getAudioStreamLength( void ) ;
        // return value is the amount read
//        exif_uint32 readAudioStream( uint8* buffer, exif_uint32 amountToRead ) ;
        // return value is the amount written
//        exif_uint32 writeAudioStream( uint8* buffer, exif_uint32 amountToRead ) ;
        // END- Imbeded Audio Stream Support.

        /*! @name Tiff Tag-based Metadata Interface
        
          This collection of methods provides access to the metadata that
          is stored in Tiff Tag-based format (e.g., Application Segment 1
          with the id of "Exif").
        */
        //@{
        //! Gets all metadata from a Tiff tag based application segment
        /*! 
          \param marker    app segment marker (0xFFE1, 0xFFE2, etc.)
          \param appSegId  app segment identification string (e.g., "Exif")
          \param pathsTags the metadata retrieved from the specified
                           application segment
          \sa setAllTags()

          The primary purpose for this method is to be used in conjunction
          with setAllTags() for fast copy all tag-based metadata from source
          to destination.

          Example:
          \code
          ExifPathsTags pathsTags ;
          inImageFile.getAllTags( 0xFFE1, "Exif", pathsTags ) ;
          outImageFile.setAllTags( 0xFFE1, "Exif", pathsTags ) ;
          \endcode
         */
        ExifStatus getAllTags( uint16 marker, const std::string& appSegId,
            ExifPathsTags &pathsTags ) ;
        
        /*!Support for read access to TIFF tag based metadata.
        
          \param tag       the tag number to retrieve.
          \param marker    application segment marker (0xFFE1, 0xFFE2, etc.)
          \param appSegId  application segment identification string
                          (e.g., "Exif")
          \param idfPath   the IFD path to the tag
          \param errRtn    error return value
          \return pointer to an ExifTagEntry holding the info for the requested
                          tag if found, otherwise NULL
        */
        ExifTagEntry* getGenericTag( exiftag_t tag, uint16 marker,
            const std::string& appSegId, const ExifIFDPath& ifdPath,
            ExifStatus& errRtn );

        /*!Support for read access to TIFF tag based metadata.
        
          \param tag       the tag number to retrieve.
          \param tagLocation    location of the tag    
          \param errRtn    error return value
          \return pointer to an ExifTagEntry holding the info for the requested
                          tag if found, otherwise NULL
          \todo Look into overloading this so that for Exif standard metadata,
               there is no need to deal with marker, appSegId, and ifdPath -
               getGenericTag( tag, errRtn ) (ditto for getGenericTag()
        */
        ExifTagEntry* getGenericTag( exiftag_t tag, ExifTagLocation tagLocation,
            ExifStatus& errRtn );

        /*! Sets all metadata provided into a Tiff tag based app segment
        
          \param marker    app segment marker (0xFFE1, 0xFFE2, etc.)
          \param appSegId  app segment identification string (e.g., "Exif")
          \param pathsTags the metadata to be set into specified app segment
          \sa getAllTags()

          The primary purpose for this method is to be used in conjunction with
          getAllTags() for fast copy all tag-based metadata from source to
          destination.
         */
        ExifStatus setAllTags( uint16 marker, const std::string& appSegId,
            const ExifPathsTags &pathsTags ) ;

        /*!Support for write access to TIFF tag based metadata.
        
          \param entry     an instance of ExifTagEntry holding the metadata info
                           to be written
          \param marker    app segment marker (0xFFE1, 0xFFE2, etc.)
          \param appSegId  app segment identification string (e.g., "Exif")
          \param idfPath   the IFD path to the tag
          \return errRtn   error return value
        */
        ExifStatus setGenericTag( const ExifTagEntry& entry, uint16 marker, 
                                  const std::string& appSegId,
                                  const ExifIFDPath& ifdPath );

        /*! Support for write access to TIFF tag based metadata.
        
          \param entry     an instance of ExifTagEntry holding the metadata
                           info to be written
          \param tagLocation    location of the tag  
          \return errRtn   error return value
        */
        ExifStatus setGenericTag ( const ExifTagEntry& entry,
            ExifTagLocation tagLocation );

        /*! Remove the specified tag
        
            If the tag is a sub IFD tag, all the content of 
            the sub IFD will be removed as well.
            
            \param tag       the tag number to retrieve.
            \param marker    application segment marker (0xFFE1, 0xFFE2, etc.)
            \param appSegId  application segment identification string
                            (e.g., "Exif")
            \param idfPath   the IFD path to the tag
         */
        void removeGenericTag(  exiftag_t tag, uint16 marker,
            const std::string& appSegId, const ExifIFDPath& ifdPath );


        /*! Remove the tag specified from the specified location
        
            \param tag          the tag number to remove
            \param tagLocation  location of the tag
         */
        void removeGenericTag( exiftag_t tag, ExifTagLocation tagLocation );
            
        //@}

        /*! Insert an Application Segment
        
            This actually inserts a clone of the Appsegment passed as argument.
            
            \param appSeg - the ExifAppSegment to insert
            \param isRaw - is this a raw App Segment ? (default: true)
            \param isTiff - is this App Segment Tiff tag based? (default: false)
            \return ExifStatus
         */
        ExifStatus insertAppSeg( ExifAppSegment* appSeg );

        /*! Get the first application segment with the specified ID.
          
            This method returns the actual app seg, not a copy.
            Do not delete it!
            
            \param marker - the App Segment marker
            \param ID - the App Segment identifier
            \param tryToConvert - try to convert the raw app seg (default: true)
            \return the Application Segment
         */
        ExifAppSegment* getAppSeg( uint16 marker, const char * ID,
            bool tryToConvert = true ) ;
        
        /*! Get all the application segments with the specified ID.
        
            This method returns the actual app segs, not copies.
            Do not delete them!
            
            \param marker - the App Segment marker
            \param ID - the App Segment identifier
            \param tryToConvert - try to convert the raw app seg (default: true)
            \return vector of Application Segments
         */
        std::vector<ExifAppSegment*> getAllAppSegs( uint16 marker, const char * ID, 
            bool tryToConvert = true ) ;

        /*! Get all the application segments.
        
            This method returns the actual app segs, not copies.
            Do not delete them!
            
            \param tryToConvert - try to convert the raw app seg (default: true)
            \return vector of Application Segments
         */                        
        std::vector<ExifAppSegment*> getAllAppSegs( bool tryToConvert = true );

        /*! Get the first raw application segment with the specified ID.
        
            This method returns NULL if their is no such raw app seg.
            Otherwise, it returns the actual app seg, not a copy.
            Do not delete it!
            
            \param marker - the App Segment marker
            \param ID - the App Segment identifier
            \param tryToConvert - try to convert the raw app seg (default: true)
            \return the Application Segment 
        */                      
        ExifAppSegment* getRawAppSeg( uint16 marker, const char * ID, 
                                     bool tryToConvert = true );
                                     
         /*! Get all the raw application segments with the specified ID.
        
            This method returns the actual app segs, not copies.
            Do not delete them!
            
            \param ID - the App Segment identifier
            \param tryToConvert - try to convert the raw app seg (default: true)
            \return the Application Segment
         */                        
        std::vector<ExifAppSegment*> getAllRawAppSegs( uint16 marker, const char * ID, 
                                     bool tryToConvert = true );

        /*! Get all the raw application segments.
        
            This method returns the actual app segs, not copies.
            Do not delete them!
            
            \param tryToConvert - try to convert the raw app seg (default: true)
            \return the Application Segment
         */
        std::vector<ExifAppSegment*> getAllRawAppSegs( bool tryToConvert = true );
        
        /*!  Delete an Application Segment with the specified ID.
        
            If there is more than one App Segment with the ID, only the first
            one will get deleted. Returns EXIF_ERROR if no app seg with the ID
            is found, otherwise returns EXIF_OK.
            
            \param marker - the App Segment marker
            \param ID - the App Segment identifier
            \return ExifStatus
         */
        ExifStatus removeAppSeg( uint16 marker, const char * ID ) ;
        
        /*! Delete all the Application Segments with the specified ID.
        
            \param marker - the App Segment marker
            \param ID - the App Segment identifier
            \return ExifStatus
         */
        ExifStatus removeAllAppSegs( uint16 marker, const char * ID ) ;

        //! Delete all the app segments
        void removeAllAppSegs() ;
        
        /*! Get the horizontal sampling factor for the component ID specified
        
            Returns zero if the component with the specified ID isn't found.
         */
        virtual uint8 getHSamplingFactor(uint8 componentID)
        {
            uint8 ret = 0;
            if (componentID < EXIF_MAX_COMPONENTS)
                ret =  mImgInfo.components[componentID].getHSampling();
            return ret;
        }
        
        /*! Get the vertical sampling factor for the component ID specified
        
            Returns zero if the component with the specified ID isn't found.
         */
        virtual uint8 getVSamplingFactor(uint8 componentID)
        {
            uint8 ret = 0;
            if (componentID < EXIF_MAX_COMPONENTS)
                ret =  mImgInfo.components[componentID].getVSampling();
            return ret;
        }
       
        //! Get a Q tables
        virtual ExifStatus getJpegQTable( unsigned char qIndex, 
            ExifJpegQuantTable* qTable );
        
        const ExifComMarkerList * getComData()
        {
            return &mComMarkerList;
        }
        
        void setComData(uint8 * comData, tsize_t dataSize)
        {
            mComMarkerList.addData(comData,dataSize);
        }
        
    protected:
        virtual ExifStatus initAfterOpen( const char* cmode );

    private:
        //! The application segment manager
        ExifAppSegManager* mAppSegManager ;
        
        uint8* mICCProf;
        exif_uint32 mICCProfSize;
        
        // This ExifJpegImage will be used for compressing to a
        // temporary file before this ExifImageFile is written and
        // closed.
        ExifJpegImage mOutImage;

        ExifIFD::EntryMap  mDefaultEntries;

        std::string mTmpImageFile ;
        bool mImageModifiedOrCreated ;
    
        ExifComMarkerList mComMarkerList;
        
        ExifStatus readSOI( ExifIO* exifio ) ;

        ExifStatus readAppSegs( ExifIO* exifio ) ;
        
        // Routine to read data from COM markers
        ExifStatus readCom( tsize_t length ) ;
         
        // Routine to read in one app seg from mExifio
        ExifStatus readAppSeg(uint16 marker, tsize_t length);
        
        // Rountine to read in an image
        ExifStatus readImage();
        
         // Routine to read in Q-Tables
        ExifStatus readDQT(tsize_t length);
       
         // Routine to read in Huffman Tables
        ExifStatus readDHT(tsize_t length);
       
         // Routine to read in SOF data
        ExifStatus readSOF(tsize_t length);
       
        
        // Rountine to write in an image
        ExifStatus writeImage();
        
        ExifStatus writeAppSegs( void ) ;
        ExifStatus writeSOI( void ) ;
        ExifStatus writeCom( void ) ;

        void initEndian( bool bigendian );

        ExifTagEntry* getDefaultTag(exiftag_t tagNum ) const ;

        ExifStatus initAppSegs( ExifIO * exifio ) ;
        
        // Inserts the default entries into mDefaultEntries
        void setupDefaults( void ) ;

        // Deletes the entries in mDefaultEntries
        void deleteDefaults( void ) ;

        // Convert an ExifTagLocation to a appSegMarker, appSegID,
        // and ExifIFDPath
        ExifStatus convertTagLocation( ExifTagLocation tagLocation,
            uint16& marker, std::string& appSegId, ExifIFDPath& ifdPath );
        
        // Get the offset to the DQT marker
        // SMF 3/29/2005 -- This now returns the first of DQT/DHT/SOF 
        //                  markers as it's only used to copy image data.
        ExifStatus getOffsetToDQT( ExifIO* tmpExifIo, exifoff_t& offsetToDQT ) ;
};
}
#endif // _EXIF_IMAGE_FILE_H_
