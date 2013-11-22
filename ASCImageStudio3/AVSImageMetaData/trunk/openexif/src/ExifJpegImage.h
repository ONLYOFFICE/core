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


#ifndef _EXIF_JPEG_IMAGE_H_
#define _EXIF_JPEG_IMAGE_H_

#if (defined _MSC_VER)
#pragma warning( disable : 4786 )
#endif

#include "ExifConf.h"
#include "ExifTypeDefs.h"
#include "ExifHuffmanTable.h"
#include "ExifIJGWrapper.h"
#include "ExifErrorCodes.h"
#include "ExifIO.h"
#include "ExifOpenFile.h"

/*!
  \author  George Sotak <george.sotak@kodak.com>
  \date   Sun Jan 20 09:16:48 2002
  \brief  Interface to a JPEG image file or just the JPEG image portion of Exif
  
  
  Example usage:

  \code
  #include "ExifImageFile.h"
  int main()
  {
    char inputFile[]= "KodakDC260.jpg" ;
    char outputFile[]= "copy.jpg" ;

    ExifJpegImage inImageFile ;
    ExifJpegImage outImageFile ;
    
    inImageFile.open( inputFile, "r" ) ;
    outImageFile.open( outputFile, "w" ) ;

    ExifImageInfo imgInfo ;
    inImageFile.getImageInfo( imgInfo ) ;
    outImageFile.setImageInfo( imgInfo ) ;
    
    ExifImageDesc imageBuf ;

    exifAllocImageDesc( imgInfo, imageBuf ) ;

    status = inImageFile.readScanLines( imageBuf, imageBuf.numberOfRows,
        nextLine, linesDone ) ;
    status = outImageFile.writeScanLines( imageBuf, imageBuf.numberOfRows,
        nextLine, linesDone ) ;

    exifDeleteImageDesc( imageBuf ) ;

    // close them up
    inImageFile.close() ;
    outImageFile.close() ;
  }
  \endcode
*/
namespace Exif 
{


class EXIF_DECL ExifJpegImage : public ExifOpenFile
{
    public:
        //! Default Constructor.
        ExifJpegImage() ;

        //! Destructor
        virtual ~ExifJpegImage();
        
        //! Close the image file, flush the image out to the file if necessary
        virtual ExifStatus close( void );

        /*! Is this a Jpeg file ?
        
            \param fileName - the name of the file to check
         */
        static ExifStatus isJpegFile(char * fileName) ;

        /*! Is this a Jpeg file ?
        
            \param fileName - the name of the file to check
         */
        static ExifStatus isJpegFile(wchar_t * fileName) ;

        /*! Is this a Jpeg file ?
        
            \param exifio - the handle to the ExifIO with the file
         */
        static ExifStatus isJpegFile(ExifIO * exifio) ;
        
        /*! Is this a Jpeg file ?
        
            \param fp - the handle to the open file
         */
        static ExifStatus isJpegFile(FILE * fp) ;

        /*! 
            @name Image Pixel Interface

            This collection of methods provides access to the image
            pixels of a JPEG image file. The uncompressed image is
            communicated between the application and toolkit through the
            ExifImageDesc structure. The ExifImageInfo structure is used
            to communicate the image's basic parameters (width, height,
            etc.)  between the application and toolkit without needing
            to read / write the image pixels.
        */
        //@{
        /*! Write the pixels to the file
        
          \param imgDesc the handle to image pixels, passed in from the app
          \param numLines the number of lines to write
          \param nextLine return the index of the next line to be written
          \param linesCompressed the number of lines compressed (written)

          The application fills in all the values and setups the image
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
        */
        virtual ExifStatus writeScanLines( ExifImageDesc &imgDesc, exif_uint32 numLines, 
                                   exif_uint32 &nextLine, exif_uint32& linesCompressed); 

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

       //! get the image's container information
        ExifStatus getImageInfo(ExifImageInfo &info) const
            { info = mImgInfo; return EXIF_OK ;}

        //! set the image's container information
        virtual ExifStatus setImageInfo(ExifImageInfo &info);


        /*! Set JPEG DCT Method used for compression/decompression.
        
            Default value of EXIF_DCT_FAST_INT can be overridden using this
            method.
         */
        virtual void setJpegDCTMethod(ExifJpegDCTMethod dctMeth);
        
        /*! Set JPEG Quality Factor used for compression.
        
            Default value of 90 can be overridden using this method. The value
            set must be less or equal to 100.
         */
        virtual void setJpegQuality(uint16 jpegQual);
        
        /*! Set JPEG Smoothing Factor used
        
            Default value is 0 (no smoothing). The value set should be an
            integer between 1...100 (0 for no smoothing).
        */
        virtual void setSmoothingFactor(uint16 smoothFact);
        
        /*! Set JPEG Optimization
        
            Default is false. Set to True for encoding optimization.
        */
        virtual void setOptimization(bool opt)
        {
            mOptimization = opt;
        }

        /*! Set JPEG Progression
        
            Default value false. Set to True for simple progression.
        */
        virtual void setProgressive(bool prog)
        {
            mProgressive = prog;
        }
        
        /*! Set Sample factor = 2, 4, or 8
        
            This corresponds to reading 1/2, 1/4, or 1/8 of image size.
            ExifImageDesc must be set properly during readScanLines to take
            into account scale factor.  Useful for displaying JPEG Thumnails
            from JPEG without real Thumbnail
         */
        virtual ExifStatus setJpegReadSampleFactor(uint16 sampFactor, exif_uint32 *outWidth,
            exif_uint32 *outHeight);
        
        //! Set all the Jpeg tables
        virtual ExifStatus setJpegTables(ExifJpegQuantTable* Q0,
                           ExifJpegQuantTable* Q1, ExifJpegQuantTable* Q2,
                           ExifJpegQuantTable* Q3, ExifJpegHUFFTable* Huff_DC,
                           ExifJpegHUFFTable* Huff_AC,
                           ExifJpegHUFFTable* Huff_DC_Chroma,
                           ExifJpegHUFFTable* Huff_AC_Chroma,
                           ExifJpegTableOrder tableOrder);
        
        //! Set a Q table
        virtual ExifStatus setJpegQTable( unsigned char qIndex,
            ExifJpegQuantTable* qTable );

        //! Get a Q tables
        virtual ExifStatus getJpegQTable( unsigned char qIndex, 
            ExifJpegQuantTable* qTable );
        
        //! Set the Jpeg subsample
        virtual void setJpegSubSample(ExifJpegSubSampleType subSample)
            {mSubSampleType = subSample;}
        
        //! Select a Jpeg table
        virtual ExifStatus selectJpegTable(ExifJpegTableSelection jpegTable);
        
        //! Set the color map
        virtual ExifStatus setColorMap( ExifJpegColorMap *colorMap,
            bool applyToData);
            
        /*! Get the horizontal sampling factor for the component ID specified
        
            Returns zero if the component with the specified ID isn't found.
         */
        virtual uint8 getHSamplingFactor(uint8 componentID);
        
        /*! Get the vertical sampling factor for the component ID specified
        
            Returns zero if the component with the specified ID isn't found.
         */
        virtual uint8 getVSamplingFactor(uint8 componentID);
       
        /*! Set the horizontal sampling factor for the component ID specified
 
         */
        //void setHSamplingFactor(uint8 componentID, uint8 value);

        /*! Set the vertical sampling factor for the component ID specified

         */
        //void setVSamplingFactor(uint8 componentID, uint8 value);

        void set422Sampling();

        //@}
        
        /*! Set an ICC profile
            
            NOTE: This function must be called before calling writeScanLines().
         */
        virtual ExifStatus setICCProfile(uint8 *iccBuf, exif_uint32 iccSize);
        
    protected:
        ExifImageInfo mImgInfo ;
        ExifJpegDCTMethod mDctMethod;	// default to FAST_INT
        uint16 mJpegQuality;			// default to 90
        uint16 mSmoothingFactor;        // default to 0
        bool mOptimization;             // default to false
        bool mProgressive;              // default to false
        uint16 mJpegReadSampleFactor;
        ExifJpegColorMap *mJpegColorMap;	
        bool	mUseColorMap;
        bool	mApplyColorMap;
        ExifJpegSubSampleType mSubSampleType;

        bool mCompressorStarted;
        bool mDecompressorStarted;

        ExifJpegCompress *mJpegCompressor;
        ExifJpegDecompress *mJpegDecompressor;

        // Holds the scale dimensions used to return width and height of
        // image data when user has set a sample factor to use when
        // reading
        ExifJpegScaleDimension mScaleDimensions[4];
        
        struct internalJPEGTableHolder
        {
                ExifJpegQuantTable* Q[4];
                ExifJpegHUFFTable*   Huff_DC[4];
                ExifJpegHUFFTable*   Huff_AC[4];
                ~internalJPEGTableHolder() 
                    {  
                        if (Q[0]) delete Q[0];
                        if (Q[1]) delete Q[1];
                        if (Q[2]) delete Q[2];
                        if (Q[3]) delete Q[3];
                        if (Huff_DC[0]) delete Huff_DC[0];
                        if (Huff_DC[1]) delete Huff_DC[1];
                        if (Huff_DC[2]) delete Huff_DC[2];
                        if (Huff_DC[3]) delete Huff_DC[3];
                        if (Huff_AC[0]) delete Huff_AC[0];
                        if (Huff_AC[1]) delete Huff_AC[1];
                        if (Huff_AC[2]) delete Huff_AC[2];
                        if (Huff_AC[3]) delete Huff_AC[3];
                    };
                internalJPEGTableHolder() 
                    { 
                      Q[0]=NULL; Q[1]=NULL; Q[2]=NULL; Q[3]=NULL;
                      Huff_DC[0]=NULL; Huff_DC[1]=NULL; Huff_DC[2]=NULL; 
                      Huff_DC[3]=NULL;
                      Huff_AC[0]=NULL; Huff_AC[1]=NULL; Huff_AC[2]=NULL; 
                      Huff_AC[3]=NULL;
                    };
        };
        
        internalJPEGTableHolder *mJpegTables;
        ExifJpegTableOrder mJpegTableOrder;
        bool mUseCustomTables;
        
        // ExifImageFile uses the constructor to instantiate the
        // support for reading/writing the image data. ExifImageFile
        // provides the support for reading/writing the App Segments.
        ExifJpegImage( ExifStatus &status, ExifIO* exifio) ;

    protected:
        virtual ExifStatus initAfterOpen( const char* cmode );
        ExifStatus generalInit( void ) ;

    private:
        ExifStatus getCurrentJpegInfo( ExifImageInfo &info );
};
}
#endif // _EXIF_JPEG_IMAGE_H_
