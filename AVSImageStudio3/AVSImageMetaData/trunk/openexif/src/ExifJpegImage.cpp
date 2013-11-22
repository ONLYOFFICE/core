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

#if (defined _MSC_VER)
#pragma warning( disable : 4786 )
#endif

#include "ExifConf.h"
#include "ExifErrorCodes.h"
#include "ExifIO.h"
#include "ExifJpegImage.h"
#include "ExifJpegTables.h"
#include "ExifImageFileIO.h"

#include "..\..\Common\Utils.h"

#ifdef OPENEXIF_NO_IJG
#ifndef DCTSIZE2
#define DCTSIZE2 64
#endif
#endif

//const int jpeg_zigzag_order[DCTSIZE2] = {
//   0,  1,  5,  6, 14, 15, 27, 28,
//   2,  4,  7, 13, 16, 26, 29, 42,
//   3,  8, 12, 17, 25, 30, 41, 43,
//   9, 11, 18, 24, 31, 40, 44, 53,
//  10, 19, 23, 32, 39, 45, 52, 54,
//  20, 22, 33, 38, 46, 51, 55, 60,
//  21, 34, 37, 47, 50, 56, 59, 61,
//  35, 36, 48, 49, 57, 58, 62, 63
//};
//const int openexif_jpeg_natural_order[DCTSIZE2+16] = {
//  0,  1,  8, 16,  9,  2,  3, 10,
// 17, 24, 32, 25, 18, 11,  4,  5,
// 12, 19, 26, 33, 40, 48, 41, 34,
// 27, 20, 13,  6,  7, 14, 21, 28,
// 35, 42, 49, 56, 57, 50, 43, 36,
// 29, 22, 15, 23, 30, 37, 44, 51,
// 58, 59, 52, 45, 38, 31, 39, 46,
// 53, 60, 61, 54, 47, 55, 62, 63,
// 63, 63, 63, 63, 63, 63, 63, 63, /* extra entries for safety in decoder */
// 63, 63, 63, 63, 63, 63, 63, 63
//};
namespace Exif{



ExifJpegImage::ExifJpegImage()
    : ExifOpenFile(),
      mDctMethod(EXIF_DCT_FAST_INT),
      mJpegQuality(90),
      mSmoothingFactor(0),
      mOptimization(false),
      mProgressive(false),
      mJpegReadSampleFactor(1), mJpegColorMap(NULL),
      mUseColorMap(false), mApplyColorMap(false), 
      mCompressorStarted(false),
      mDecompressorStarted(false),
      mJpegCompressor(NULL), 
      mJpegDecompressor(NULL),
      mJpegTables(NULL), 
      mUseCustomTables(false)
{}

ExifJpegImage::ExifJpegImage(ExifStatus &status, ExifIO* exifIO) 
    : ExifOpenFile( exifIO ),
      mDctMethod(EXIF_DCT_FAST_INT),
      mJpegQuality(90),
      mSmoothingFactor(0),
      mOptimization(false),
      mProgressive(false),
      mJpegReadSampleFactor(1),
      mJpegColorMap(NULL),
      mUseColorMap(false), 
      mApplyColorMap(false), 
      mCompressorStarted(false),
      mDecompressorStarted(false),
      mJpegCompressor(NULL),
      mJpegDecompressor(NULL),
      mJpegTables(NULL),
      mUseCustomTables(false)
{
    status = generalInit( ) ;
}

ExifStatus ExifJpegImage::isJpegFile(char * fileName) 
{
	return isJpegFile((wchar_t *)(AVS::Utils::__StoW(fileName).c_str()));
}


ExifStatus ExifJpegImage::isJpegFile(wchar_t * fileName) 
{
    ExifStatus returnValue = EXIF_OK ;
    FILE *fp = NULL;
	
	if ((_wfopen_s(&fp, fileName, L"rb") != 0) || (NULL == fp))
		returnValue = EXIF_FILE_OPEN_ERROR ;

    if ( returnValue == EXIF_OK )
    {
        returnValue = ExifJpegImage::isJpegFile(fp);
        fclose(fp);
    }

    return returnValue ;
}

ExifStatus ExifJpegImage::isJpegFile(ExifIO * exifio)
{
    ExifStatus returnValue = EXIF_OK ;
    unsigned char tbuf[12] ;

    exifio->seek(0, SEEK_SET) ;
    if ( exifio->read(tbuf,12) != 12 )
        returnValue = EXIF_ERROR ;

    // look for SOI
    if (tbuf[0] != 0xff || tbuf[1] != 0xd8) 
        returnValue = EXIF_ERROR ;

    return returnValue;
}

ExifStatus ExifJpegImage::isJpegFile(FILE * fp)
{
    ExifStatus returnValue = EXIF_OK;
    unsigned char tbuf[12];

    fseek(fp, 0, SEEK_SET);
    if( fread_s(tbuf,12,12,1,fp)!=1 ) 
        returnValue = EXIF_ERROR ;

    // look for SOI
    if (tbuf[0] != 0xff || tbuf[1] != 0xd8) 
        returnValue = EXIF_ERROR;

    return returnValue ;
}


ExifJpegImage::~ExifJpegImage(void)
{
    if(mJpegTables) 
        delete mJpegTables;
    if(mJpegColorMap)
    {
        if ( mJpegColorMap->colorMap0 )
            delete [] mJpegColorMap->colorMap0 ;
        if ( mJpegColorMap->colorMap1 )
            delete [] mJpegColorMap->colorMap1 ;
        if ( mJpegColorMap->colorMap2 )
            delete [] mJpegColorMap->colorMap2 ;
        delete mJpegColorMap;
    }
    if(mJpegCompressor)
        delete mJpegCompressor;
    if(mJpegDecompressor)
        delete mJpegDecompressor;
}


ExifStatus ExifJpegImage::close( void )
{
    return ExifOpenFile::close() ;
}



ExifStatus ExifJpegImage::initAfterOpen( const char* cmode )
{
    return generalInit() ;
}

ExifStatus ExifJpegImage::generalInit( void )
{
#ifndef OPENEXIF_NO_IJG
    JpegStatus jpegStatus;

    if( (mExifio->mode() == O_RDONLY) || (mExifio->mode() & O_RDWR) ) 
    {
        // read JPEG header info to fill in imageInfo

        if ( mJpegDecompressor )
            delete mJpegDecompressor ;

        mJpegDecompressor = new ExifJpegDecompress();

        if(mExifio->isMapped()) 
        {
            jpegStatus = mJpegDecompressor->setBufferInput(
                mExifio->ioHandler().base(), mExifio->ioHandler().memSize());
        }
        else
        {
            jpegStatus = mJpegDecompressor->setFileInput(
                mExifio->getFilePtr() );
        }

        if( jpegStatus != JPEG_OK )
            return EXIF_ERROR ;

        // get Jpeg header info
        exif_uint32 width;
        exif_uint32 height; 
        uint16 numComps;
        jpegStatus = mJpegDecompressor->getImageInfo(&width, &height,
            &numComps);

        if(jpegStatus == JPEG_OK) 
        {
            mImgInfo.width = width;
            mImgInfo.height = height;
            mImgInfo.numChannels = numComps;
            mImgInfo.precision = mJpegDecompressor->outputPrecision();
        }
        else 
        {
            return EXIF_ERROR ;
        }

        // Determine scaled width and height dimensions for sample factors
        // 1, 2, 4 and 8
	
        mJpegDecompressor->getJpegScaleSize(JPEG_SCALE_NONE, 
            &mScaleDimensions[0].width, &mScaleDimensions[0].height);
        mJpegDecompressor->getJpegScaleSize(JPEG_SCALE_HALF,
            &mScaleDimensions[1].width, &mScaleDimensions[1].height);
        mJpegDecompressor->getJpegScaleSize(JPEG_SCALE_QUARTER,
            &mScaleDimensions[2].width, &mScaleDimensions[2].height);
        mJpegDecompressor->getJpegScaleSize(JPEG_SCALE_EIGHTH,
            &mScaleDimensions[3].width, &mScaleDimensions[3].height);
    }

    if( (mExifio->mode() & O_WRONLY) || (mExifio->mode() & O_RDWR) )
    {  // EXIF_CREATE

        if ( !mJpegCompressor )
            mJpegCompressor = new ExifJpegCompress();

        if(mExifio->isMapped()) 
            jpegStatus = mJpegCompressor->setBufferOutput(
                mExifio->ioHandler().base(), mExifio->ioHandler().memSize());
        else
        {
            jpegStatus = mJpegCompressor->setFileOutput(mExifio->getFilePtr());
        }

       if( jpegStatus != JPEG_OK )
            return EXIF_ERROR ;
    }    
#endif

    return EXIF_OK ;
}

void ExifJpegImage::setJpegDCTMethod(ExifJpegDCTMethod dctMeth)
{
    mDctMethod = dctMeth;
}

void ExifJpegImage::setJpegQuality(uint16 jpegQual)
{
    if ( jpegQual <= 100 )
        mJpegQuality = jpegQual;
}

void ExifJpegImage::setSmoothingFactor(uint16 smoothFact)
{
    if( smoothFact <= 100 )
        mSmoothingFactor = smoothFact;
}

ExifStatus ExifJpegImage::selectJpegTable(ExifJpegTableSelection jpegTable)
{
    if(mJpegTables) 
    {
        delete mJpegTables; 
        mJpegTables = NULL;
    }
    switch(jpegTable) 
    {
        case JPEG_CUSTOM_TABLE_1:
            mJpegTableOrder = JPEG_NATURAL;
            mUseCustomTables = true;
            mJpegTables = new internalJPEGTableHolder;

            mJpegTables->Q[0] = new ExifJpegQuantTable;
            *(mJpegTables->Q[0]) = q_1;

            mJpegTables->Q[1] = new ExifJpegQuantTable;
            *(mJpegTables->Q[1]) = q_1;

            mJpegTables->Q[2] = new ExifJpegQuantTable;
            *(mJpegTables->Q[2]) = q_1;

            mJpegTables->Huff_DC[0] = new ExifJpegHUFFTable(EXIF_HUFF_DC);
            mJpegTables->Huff_DC[0]->setCustomTables(
                                        h_DC_1_bits,
                                        h_DC_1_bits_size,
                                        h_DC_1_code,
                                        h_DC_1_code_size );

            mJpegTables->Huff_AC[0] = new ExifJpegHUFFTable(EXIF_HUFF_AC);
            mJpegTables->Huff_AC[0]->setCustomTables(
                                        h_AC_1_bits,
                                        h_AC_1_bits_size,
                                        h_AC_1_code,
                                        h_AC_1_code_size );

            break;
        default:
            mUseCustomTables = false;
            break;
    }

    return EXIF_OK ;
}


ExifStatus ExifJpegImage::setJpegTables(ExifJpegQuantTable*  Q0,
                             ExifJpegQuantTable*  Q1,
                             ExifJpegQuantTable*  Q2,
                             ExifJpegQuantTable*  Q3,
                             ExifJpegHUFFTable*   Huff_DC,
                             ExifJpegHUFFTable*   Huff_AC,
                             ExifJpegHUFFTable*   Huff_DC_Chroma, 
                             ExifJpegHUFFTable*   Huff_AC_Chroma,
                             ExifJpegTableOrder   tableOrder)
{
    // make sure we do not get mismatch of Table order
    if(mUseCustomTables && (mJpegTableOrder != tableOrder)) 
    {
        return EXIF_ERROR;
    }

    if(mJpegTables)
        delete mJpegTables ;
    
    mJpegTables = new internalJPEGTableHolder;

    mJpegTableOrder = tableOrder;
    mUseCustomTables = true;
	
    if (Q0 != NULL) 
    {
        mJpegTables->Q[0] = new ExifJpegQuantTable;
        *(mJpegTables->Q[0]) = *Q0;
    }
    if (Q1 != NULL) 
    {
        mJpegTables->Q[1] = new ExifJpegQuantTable;
        *(mJpegTables->Q[1]) = *Q1;
    }
    if (Q2 != NULL) 
    {
        mJpegTables->Q[2] = new ExifJpegQuantTable;
        *(mJpegTables->Q[2]) = *Q2;
    }
    if (Q3 != NULL) 
    {
        mJpegTables->Q[3] = new ExifJpegQuantTable;
        *(mJpegTables->Q[3]) = *Q3;
    }
    if (Huff_DC != NULL) 
    {
        mJpegTables->Huff_DC[0] = new ExifJpegHUFFTable;
        *(mJpegTables->Huff_DC[0]) = *Huff_DC;
    }
    if (Huff_AC != NULL) 
    {
        mJpegTables->Huff_AC[0] = new ExifJpegHUFFTable;
        *(mJpegTables->Huff_AC[0]) = *Huff_AC;
    }
    if (Huff_DC_Chroma != NULL) 
    {
        mJpegTables->Huff_DC[1] = new ExifJpegHUFFTable;
        *(mJpegTables->Huff_DC[1]) = *Huff_DC_Chroma;
    }
    if (Huff_AC_Chroma != NULL) 
    {
        mJpegTables->Huff_AC[1] = new ExifJpegHUFFTable;
        *(mJpegTables->Huff_AC[1]) = *Huff_AC_Chroma;
    }

    return EXIF_OK ;
}	

// Set the quantization table specified by qIndex
ExifStatus ExifJpegImage::setJpegQTable(  unsigned char qIndex, 
    ExifJpegQuantTable* qTable )
{
    ExifStatus returnValue = EXIF_ERROR;
    
#ifndef OPENEXIF_NO_IJG
    if (mJpegCompressor == NULL)
        mJpegCompressor = new ExifJpegCompress();
        
    if( mJpegCompressor->setJpegQuantTable(qIndex, qTable->quantizer,
        mJpegQuality) == JPEG_OK )
        returnValue = EXIF_OK;
#endif

    return returnValue;
}

// Get the quantization table specified by qIndex
ExifStatus ExifJpegImage::getJpegQTable(  unsigned char qIndex, 
    ExifJpegQuantTable* qTable )
{
    ExifStatus returnValue = EXIF_ERROR;
    
#ifndef OPENEXIF_NO_IJG
    if (mJpegDecompressor != NULL)
    {
        if ( mJpegDecompressor->getJpegQuantTable(qIndex,qTable->quantizer) ==
            JPEG_OK )
            returnValue = EXIF_OK;
    }
#endif

    return returnValue;
}

ExifStatus ExifJpegImage::setColorMap(ExifJpegColorMap *colorMap,
                                      bool applyToData)
{

    if(colorMap == NULL) 
    {
        return EXIF_ERROR;
    }
    if(colorMap->colorMap0 == NULL || colorMap->colorMap1 == NULL ||
       colorMap->colorMap2 == NULL)
    {
        return EXIF_ERROR;
    }
    if(mJpegColorMap) 
    {
        delete mJpegColorMap;
        mJpegColorMap = NULL;
    }

    mJpegColorMap = new ExifJpegColorMap;
    mJpegColorMap->colorMap0 = new uint16[colorMap->numColors];
    mJpegColorMap->colorMap1 = new uint16[colorMap->numColors];
    mJpegColorMap->colorMap2 = new uint16[colorMap->numColors];

    memcpy(mJpegColorMap->colorMap0, colorMap->colorMap0,
        colorMap->numColors * sizeof(uint16));
    memcpy(mJpegColorMap->colorMap1, colorMap->colorMap1,
        colorMap->numColors * sizeof(uint16));
    memcpy(mJpegColorMap->colorMap1, colorMap->colorMap1,
        colorMap->numColors * sizeof(uint16));
    mUseColorMap = true;
    mApplyColorMap = applyToData;

    return EXIF_OK ;
}	

// Sets sample factor. Computes new width and height with the specified
// sample factor.
ExifStatus ExifJpegImage::setJpegReadSampleFactor(uint16 sampFactor,
                                                  exif_uint32 *outWidth,
                                                  exif_uint32 *outHeight)
{
    ExifStatus returnValue = EXIF_OK ;

    if(mDecompressorStarted)
        returnValue = EXIF_ERROR ;
    else
    {
        mJpegReadSampleFactor = sampFactor;

        ExifImageInfo myInfo;

        getCurrentJpegInfo(myInfo);

        *outWidth = myInfo.width;
        *outHeight = myInfo.height;
    }

    return returnValue;
}

ExifStatus ExifJpegImage::getCurrentJpegInfo(ExifImageInfo &info)
{
    int index;

    switch(mJpegReadSampleFactor) 
    {
        case 2:
            index = 1;
            break;
        case 4:
            index = 2;
            break;
        case 8:	
            index = 3;
            break;
        case 1:
        default:
            index = 0;
            break;
    }
    info.width = mScaleDimensions[index].width; 
    info.height = mScaleDimensions[index].height; 
    info.numChannels = mImgInfo.numChannels;

    return EXIF_OK;
}

ExifStatus 
ExifJpegImage::readScanLines( ExifImageDesc &imgDesc, exif_uint32 numLines, 
                              exif_uint32& nextLine, exif_uint32& linesDecompressed)
{
#ifndef OPENEXIF_NO_IJG
    // First, check and see if we are open for reading...
    if ( (!((mExifio->mode() == O_RDONLY) || (mExifio->mode() & O_RDWR))) ||
        (mJpegDecompressor==NULL) )
    {
        linesDecompressed = 0 ;
        return EXIF_FILE_READ_ERROR ;
    }
    
    unsigned char * pDest;
    JpegStatus jpegStatus;


    if(!mDecompressorStarted) 
    {
        mJpegDecompressor->setScaling(mJpegReadSampleFactor);

        // initialize to default to get rid of compiler warning
        J_DCT_METHOD jpegDct = JDCT_IFAST ;

        switch(mDctMethod) 
        {
            case EXIF_DCT_SLOW_INT:
                jpegDct = JDCT_ISLOW;
                break;
            case EXIF_DCT_FAST_INT:
                jpegDct = JDCT_IFAST;
                break;
            case EXIF_DCT_FLOAT:
                jpegDct = JDCT_FLOAT;
                break;
        }

        mJpegDecompressor->setJpegDCTMethod(jpegDct);
        
        ExifColorSpace jpegColor = (imgDesc.numberOfComponents == 1)?
            JPEG_GRAYSCALE : EXIF_RGB;

        if(imgDesc.components[0].myColorType.myColor == RGB_B)
            jpegColor = JPEG_BGR;
        else
        {
            if (imgDesc.components[0].myColorType.myColor == YCRCB_Y)
                jpegColor = EXIF_YCbCr;
        }

        mJpegDecompressor->setOutputColorSpace(jpegColor);
        if ( mJpegDecompressor->startDecompress() != JPEG_OK )
        {
            linesDecompressed = 0;
            return EXIF_ERROR;
        }

        mDecompressorStarted = true;

    }

    unsigned int outWidth, outHeight;
    int outLineStride;
    pDest = imgDesc.components[0].theData;

    outWidth = mJpegDecompressor->outputWidth();
    outHeight = mJpegDecompressor->outputHeight();
    outLineStride = imgDesc.components[0].lineStride; // outWidth * numComps;

    linesDecompressed = 0;
    exif_uint32 nextOutputLine;

    jpegStatus = mJpegDecompressor->readScanLines(pDest, numLines,
        outLineStride, &linesDecompressed, &nextOutputLine);

    if(jpegStatus != JPEG_OK) 
    {
        linesDecompressed = 0 ;
        return EXIF_ERROR;
    }
	
    nextLine = nextOutputLine;
    
    if(nextOutputLine >= outHeight)
    {
        mJpegDecompressor->stopDecompress();
        mDecompressorStarted = false ;
    }
#endif
    return EXIF_OK;
}



ExifStatus 
ExifJpegImage::writeScanLines( ExifImageDesc &imgDesc, exif_uint32 numLines, 
                               exif_uint32 &nextLine, exif_uint32& linesCompressed)
{
#ifndef OPENEXIF_NO_IJG
    // First, check and see if we are open for writing...
    if (!((mExifio->mode() & O_WRONLY) || (mExifio->mode() & O_RDWR)))
    {
        linesCompressed = 0 ;
        return EXIF_FILE_WRITE_ERROR ;
    }
    
    if (mJpegCompressor == NULL)
        mJpegCompressor = new ExifJpegCompress();
        
    JpegStatus jpegStatus;

    // If not already started, then setup and start JPEG compressor
    if(mJpegCompressor->getState() != JPEG_STARTED)
    {
		
        ExifColorSpace jpegColor = (imgDesc.numberOfComponents==1) ?
            JPEG_GRAYSCALE:EXIF_RGB;
        
        if(imgDesc.components[0].myColorType.myColor == YCRCB_Y)
            jpegColor = EXIF_YCbCr;
        else 
            if(imgDesc.components[0].myColorType.myColor == RGB_B)
                jpegColor = JPEG_BGR;

        mJpegCompressor->setImageInfo(mImgInfo.width, mImgInfo.height,
            mImgInfo.numChannels, jpegColor);
        mJpegCompressor->setInputColorSpace(jpegColor);
        
        // initialize to default to get rid of compiler warning
        J_DCT_METHOD jpegDct = JDCT_IFAST;

        switch(mDctMethod) 
        {
            case EXIF_DCT_SLOW_INT:
                jpegDct = JDCT_ISLOW;
                break;
            case EXIF_DCT_FAST_INT:
                jpegDct = JDCT_IFAST;
                break;
            case EXIF_DCT_FLOAT:
                jpegDct = JDCT_FLOAT;
                break;
        }
        mJpegCompressor->setJpegDCTMethod(jpegDct);
        mJpegCompressor->setJpegQuality(mJpegQuality);
        mJpegCompressor->setSmoothingFactor(mSmoothingFactor);
        mJpegCompressor->setOptimization(mOptimization);
        mJpegCompressor->setProgressive(mProgressive);

        // see if Custom JPEG Tables have been set
        if(mUseCustomTables)
        {
            if(mJpegTables->Q[0])
                mJpegCompressor->setJpegQuantTable(0,
                    mJpegTables->Q[0]->quantizer,mJpegQuality);
            if(mJpegTables->Q[1])
                mJpegCompressor->setJpegQuantTable(1,
                    mJpegTables->Q[1]->quantizer,mJpegQuality);
            if(mJpegTables->Q[2])
                mJpegCompressor->setJpegQuantTable(2,
                    mJpegTables->Q[2]->quantizer,mJpegQuality);
            if(mJpegTables->Q[3])
                mJpegCompressor->setJpegQuantTable(3,
                    mJpegTables->Q[3]->quantizer,mJpegQuality);
        }

        if ( mJpegCompressor->startCompress() != JPEG_OK )
        {
            linesCompressed = 0;
            return EXIF_ERROR;
        }

        mCompressorStarted = true;
            
    }
    unsigned char *srcPtr;
    unsigned int srcLineStride;

    srcPtr = imgDesc.components[0].theData;
    srcLineStride = imgDesc.components[0].lineStride;
	
    jpegStatus = mJpegCompressor->writeScanLines(srcPtr, srcLineStride,
        numLines, &linesCompressed, &nextLine);

    if( jpegStatus != JPEG_OK )
    {
        linesCompressed = 0;
        return EXIF_ERROR;
    }

    if(nextLine == mImgInfo.height) 
    {
        mJpegCompressor->stopCompress();
        mCompressorStarted = false ;
    }
#endif
    return EXIF_OK;
}

ExifStatus ExifJpegImage::setImageInfo(ExifImageInfo &imgInfo)
{
    mImgInfo = imgInfo ;

    return EXIF_OK;
}

uint8 ExifJpegImage::getHSamplingFactor(uint8 componentID)
{
    uint8 returnValue = 0;
    
#ifndef OPENEXIF_NO_IJG
    if(mJpegDecompressor!=NULL)
        returnValue = mJpegDecompressor->getHSamplingFactor(componentID);
#endif

    return returnValue;
}

uint8 ExifJpegImage::getVSamplingFactor(uint8 componentID)
{
    uint8 returnValue = 0;
    
#ifndef OPENEXIF_NO_IJG
    if(mJpegDecompressor!=NULL)
        returnValue = mJpegDecompressor->getVSamplingFactor(componentID);
#endif

    return returnValue;
}

/*
void ExifJpegImage::setHSamplingFactor(uint8 componentID, uint8 value)
{
    if(mJpegCompressor!=NULL)
        mJpegCompressor->setHSamplingFactor(componentID, value);
}

void ExifJpegImage::setVSamplingFactor(uint8 componentID, uint8 value)
{
    if(mJpegCompressor!=NULL)
        mJpegCompressor->setVSamplingFactor(componentID, value);
}
*/

void ExifJpegImage::set422Sampling()
{
    if (mJpegCompressor == NULL)
        mJpegCompressor = new ExifJpegCompress();
        
    mJpegCompressor->set422Sampling();
}

ExifStatus ExifJpegImage::setICCProfile(uint8 *iccBuf, exif_uint32 iccSize)
{
    ExifStatus returnValue = EXIF_OK;
    if (mJpegCompressor == NULL)
        mJpegCompressor = new ExifJpegCompress();
        
    if(mJpegCompressor->setICCProfile(iccBuf,iccSize) != JPEG_OK)
        returnValue = EXIF_ERROR;
    
    return returnValue;
}
}