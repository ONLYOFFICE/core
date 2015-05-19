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
 * Sam Fryer samuel.fryer@kodak.com 
 *
 * Contributor(s): 
 * 
 */ 
 
 /*
 * Contains code from jdmarker.c, with the following (C) notice:
 *
 * Copyright (C) 1991-1998, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 */

#include "ExifImageFile.h"
#include "ExifRawAppSeg.h"
#include "ExifImageFileIO.h"
namespace Exif{

// Read an Image in completely!
ExifStatus ExifImageFile::readImage()
{
    exifoff_t fileSize = mExifio->seek(0, SEEK_END);
    mExifio->seek(0, SEEK_SET);
    ExifStatus status = readSOI(mExifio) ;
    if (status == EXIF_OK)
    {
        uint8 input[4];
        unsigned int tagSize = 0;
        tsize_t readCount = 0;
        exifoff_t currPos = mExifio->seek(0,SEEK_CUR);
        while ((status == EXIF_OK) && (currPos < fileSize))
        {
            readCount = mExifio->read(input,4);
            currPos += readCount;
            
            if ((readCount == 4) && ((input[0] == 0xFF) &&
                    ((input[1] >= M_SOF0) && (input[1] <= M_COM))))
            {
                tagSize = input[2];
                tagSize = tagSize<<8;
                tagSize += input[3];

                switch (input[1])
                {
                    case M_SOF0  : /* Baseline */
                    case M_SOF1  : /* Extended sequential, Huffman */
                    case M_SOF2  : /* Progressive, Huffman */
                    case M_SOF9  : /* Extended sequential, arithmetic */
                    case M_SOF10 : /* Progressive, arithmetic */
                        status = readSOF(tagSize);
                        break;

//                     /* Currently unsupported SOFn types */
//                     case M_SOF3  : /* Lossless, Huffman */
//                     case M_SOF5  : /* Differential sequential, Huffman */
//                     case M_SOF6  : /* Differential progressive, Huffman */
//                     case M_SOF7  : /* Differential lossless, Huffman */
//                     case M_JPG   : /* Reserved for JPEG extensions */
//                     case M_SOF11 : /* Lossless, arithmetic */
//                     case M_SOF13 : /* Differential sequential, arithmetic */
//                     case M_SOF14 : /* Differential progressive, arithmetic */
//                     case M_SOF15 : /* Differential lossless, arithmetic */
//   
                    case M_DHT   : 
//                         printf("DHT Marker!\n");
                        status = readDHT(tagSize);
                        break;
                        
//   
//                     case M_DAC   : 
//   
//                     case M_RST0  : 
//                     case M_RST1  : 
//                     case M_RST2  : 
//                     case M_RST3  : 
//                     case M_RST4  : 
//                     case M_RST5  : 
//                     case M_RST6  : 
//                     case M_RST7  : 
//   
//                     case M_SOI   : 
//                     case M_EOI   : 

                    case M_SOS   : 
//                         printf("SOS Marker ... Stopped Parsing!\n");
                        currPos = fileSize;
                        break;
                        
                    case M_DQT   : 
//                         printf("DQT Marker!\n");
                        status = readDQT(tagSize);
                        break;
                        
//                     case M_DNL   : 
//                     case M_DRI   : 
//                     case M_DHP   : 
//                     case M_EXP   : 
//                         break;
  
                    case M_APP0  : 
                    case M_APP1  : 
                    case M_APP2  : 
                    case M_APP3  : 
                    case M_APP4  : 
                    case M_APP5  : 
                    case M_APP6  : 
                    case M_APP7  : 
                    case M_APP8  : 
                    case M_APP9  : 
                    case M_APP10 : 
                    case M_APP11 : 
                    case M_APP12 : 
                    case M_APP13 : 
                    case M_APP14 : 
                    case M_APP15 : 
//                         printf("App Segment!\n");
                        status = readAppSeg(0xFF00 + input[1],tagSize);
                        break;
  
//                     case M_JPG0  : 
//                     case M_JPG13 : 
                    case M_COM   : 
                        status = readCom(tagSize);
                        break;
                    
                    default:
//                         printf("Not Handled!\n");
                        break;
                }
                currPos = mExifio->seek(currPos + tagSize - 2 ,SEEK_SET);
            }
            // Handle the case where there isn't a valid tag.
            else if ((readCount == 4) && ((input[0] != 0xFF) || 
                    ((input[1] < M_SOF0) || (input[1] > M_COM))))
            {
                //search through the file byte by byte looking for a valid tag
                // Note that we do not search backwards, so some out-of-spec
                // files may still be unreadable using this approach
                int p = 0;
                bool tagNotFound = true;
                while ((tagNotFound) && (currPos < fileSize))
                {
                    p = 0;
                    while (((input[p] != 0xFF) || 
                        ((input[p+1] < M_SOF0) || (input[p+1] > M_COM))) &&
                        (p<3))
                        p++;
                    if (p>=3)
                    {
                        input[0] = input[3];
                        currPos += mExifio->read(&input[1],3);
                    }
                    else
                    {
                        tagNotFound = false;
                        currPos -= 4-p;
                        currPos = mExifio->seek(currPos,SEEK_SET);
                    }
                }
            }
        }
    }
    return status ;
}

ExifStatus ExifImageFile::readAppSeg(uint16 marker, tsize_t length)
{
    ExifStatus status = EXIF_OK;
    exifoff_t offsetToThisMarker = mExifio->seek(0,SEEK_CUR) - 4;
    
    // read the identifier, usually is 5 (App0-JFIF) or 6 bytes
    // but not restricted to it. They always end in at least one NULL
    // Note that we're limiting the identifier size to 127 chars.
    char ident[128] ;
    tsize_t readCount = mExifio->read(ident,128);
    if (  readCount > 0 )
    {

        int i = 0;
        while ( i < 128 && ident[i] != '\0' ) i++ ;

        if ( i >= 128 )
        {
            // We have an invalid identifier!!!
            i = 8;
            ident[0] = 'I';
            ident[1] = 'N';
            ident[2] = 'V';
            ident[3] = 'A';
            ident[4] = 'L';
            ident[5] = 'I';
            ident[6] = 'D';
            ident[7] = '\0';
        }

        ExifRawAppSeg* appSeg = new ExifRawAppSeg(marker, ident) ;
        mExifio->seek( offsetToThisMarker, SEEK_SET ) ;
        appSeg->init( mExifio, length, (int)0 ) ;

        mAppSegManager->insert( appSeg ) ;

    }
    else
    {
        status = EXIF_FILE_READ_ERROR ;
    }
    return status;
}

#define EXIF_N_QTABLE_ENTRIES 64

ExifStatus ExifImageFile::readDQT(tsize_t length)
{
    ExifStatus status = EXIF_OK;
    uint8 n = 0;
    tsize_t readCount = 0;
    
    if (mJpegTables == NULL)
        mJpegTables = new internalJPEGTableHolder();
    
    exifoff_t currPos = mExifio->seek(0,SEEK_CUR);
    exifoff_t endOfDQT = currPos + length - 2;
    
    while((currPos < endOfDQT) && (status == EXIF_OK))
    {
        currPos += mExifio->read(&n,1);
        unsigned int tableNum = n & 0x0F;
        if (mJpegTables->Q[tableNum] == NULL)
            mJpegTables->Q[tableNum] = new ExifJpegQuantTable();
        
        // If it's double precision qtables or not
        if ( n & 0xF0 )
        {
            uint16 qt[EXIF_N_QTABLE_ENTRIES];
            readCount = mExifio->read(qt,EXIF_N_QTABLE_ENTRIES*2);
            if (readCount == EXIF_N_QTABLE_ENTRIES*2)
            {
                for (int i = 0; i< EXIF_N_QTABLE_ENTRIES; i++)
                    mJpegTables->Q[tableNum]->quantizer[openexif_jpeg_natural_order[i]] = qt[i];
                currPos += readCount;
            }
            else
                status = EXIF_FILE_READ_ERROR;
        }
        else
        {
            uint8 qt[EXIF_N_QTABLE_ENTRIES];
            readCount = mExifio->read(qt,EXIF_N_QTABLE_ENTRIES);
            if (readCount == EXIF_N_QTABLE_ENTRIES)
            {
                for (int i = 0; i< EXIF_N_QTABLE_ENTRIES; i++)
                    mJpegTables->Q[tableNum]->quantizer[openexif_jpeg_natural_order[i]] = qt[i];
                currPos += readCount;
            }
            else
                status = EXIF_FILE_READ_ERROR;
        }
    }
    
    if (currPos > endOfDQT)
        status = EXIF_FILE_READ_ERROR;
    
    return status;
}

// SOF Format:
// # bytes  Meaning
//=======================================
//  1       precision (in bits)
//  2       Vertical Lines (height)
//  2       Horizontal Lines (Width)
//  1       Number of Components
//---------------------------------------
//  1       Component Number            |
//  1       H:V Sampling factor         | Repeat for each component
//  1       Quant designation           |
//---------------------------------------
//=======================================
 
ExifStatus ExifImageFile::readSOF(tsize_t length)
{
    ExifStatus status = EXIF_OK;
    uint8 buf[8];
    if (mExifio->read(buf,6) == 6)
    {
        mImgInfo.precision = buf[0];
        mImgInfo.height = buf[1];
        mImgInfo.height = (mImgInfo.height << 8) + buf[2];
        mImgInfo.width = buf[3];
        mImgInfo.width = (mImgInfo.width << 8) + buf[4];
        mImgInfo.numChannels = buf[5];
    }
    else
        status = EXIF_FILE_READ_ERROR;
    
    for (int i = 0 ; (i < mImgInfo.numChannels) && (status == EXIF_OK) ; i++)
    {
        if (mExifio->read(buf,3) == 3)
        {
            if (buf[0] < EXIF_MAX_COMPONENTS)
            {
                mImgInfo.components[buf[0]].hvSampling = buf[1];
                mImgInfo.components[buf[0]].quantizationDesignation = buf[2];
            }
            else
                status = EXIF_INVALID_FORMAT_ERROR;
       }
       else
           status = EXIF_FILE_READ_ERROR;
    }
    return status;
}

// DHT Format:
// # bytes  Meaning
//=======================================
//---------------------------------------
//  1       Index                       |
// 16       Bits                        | Repeat for each table
//  N       Table Values                |
//          (N = SUM(Bits))             |
//---------------------------------------
//=======================================
 
ExifStatus ExifImageFile::readDHT(tsize_t length)
{
    ExifStatus status = EXIF_OK;
    uint8 n = 0;
//    tsize_t readCount = 0;
    ExifJpegHUFFTable * ht = NULL;
    uint8 buf[257];
    int symsum = 0;
    
    if (mJpegTables == NULL)
        mJpegTables = new internalJPEGTableHolder();
    
    exifoff_t currPos = mExifio->seek(0,SEEK_CUR);
    exifoff_t endOfDHT = currPos + length - 2;
    
    while((currPos < endOfDHT) && (status == EXIF_OK))
    {
        currPos += mExifio->read(&n,1);
        
        if (n & 0x10)
        {
            n -= 0x10;
            if (mJpegTables->Huff_AC[n] == NULL)
                mJpegTables->Huff_AC[n] = new ExifJpegHUFFTable(EXIF_HUFF_AC);
            ht = mJpegTables->Huff_AC[n];
        }
        else
        {
            if (mJpegTables->Huff_DC[n] == NULL)
                mJpegTables->Huff_DC[n] = new ExifJpegHUFFTable(EXIF_HUFF_DC);
            ht = mJpegTables->Huff_DC[n];
        }
        
        if (mExifio->read(&buf,16) == 16)
        {
            currPos += 16;
            symsum = 0;
            for (int i = 0; i<16; i++)
            {
                ht->num_symbols[i+1] = buf[i];
                symsum += buf[i];
            }
            
            if ((symsum <= 256) && (currPos+symsum <= endOfDHT))
            {
                if (mExifio->read(&buf,symsum) == symsum)
                {
                    currPos += symsum;
                    for (int i = 0; i<symsum; i++)
                        ht->symbol_values[i] = buf[i];
                    status = ht->deriveTable();
                }
                else
                    status = EXIF_FILE_READ_ERROR;
            }
            else
                status = EXIF_INVALID_FORMAT_ERROR;
            
        }
        else
            status = EXIF_FILE_READ_ERROR;

    }
    
    if (currPos > endOfDHT)
        status = EXIF_FILE_READ_ERROR;
    
    return status;
}

ExifStatus 
ExifImageFile::readScanLines( ExifImageDesc &imgDesc, exif_uint32 numLines, 
                              exif_uint32& nextLine, exif_uint32& linesDecompressed)
{
	ExifStatus exifstatus = EXIF_OK;

#ifndef OPENEXIF_NO_IJG
    
    // First, check and see if we are open for reading...
    if ((mExifio->mode() == O_RDONLY) || (mExifio->mode() & O_RDWR))
    {
        if (mJpegDecompressor==NULL)
            this->generalInit();

        // mJpegDecompressor may still be null after call to generalInit()
        if (mJpegDecompressor!=NULL)
        {
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
            outLineStride = imgDesc.components[0].lineStride; 
            // outWidth * numComps;

            linesDecompressed = 0;
            exif_uint32 nextOutputLine;

            jpegStatus = mJpegDecompressor->readScanLines(pDest, numLines,
                outLineStride, &linesDecompressed, &nextOutputLine);

            if(jpegStatus != JPEG_OK) 
            {
                linesDecompressed = 0 ;
                exifstatus = EXIF_FILE_READ_ERROR;
            }
            else
            {
                nextLine = nextOutputLine;

                if(nextOutputLine >= outHeight)
                {
                    mJpegDecompressor->stopDecompress();
                    mDecompressorStarted = false ;
                }
            }

        }
        else
        {
            linesDecompressed = 0 ;
            exifstatus = EXIF_FILE_READ_ERROR ;
        }
    }
    else
    {
        linesDecompressed = 0 ;
        exifstatus = EXIF_FILE_READ_ERROR ;
    }
    
#endif
    return exifstatus;
}


ExifStatus ExifImageFile::readCom(tsize_t length)
{
    ExifStatus status = EXIF_OK;
    
    // length must be -2 from the reported length to take into the 2 bytes 
    // for the size reported right after the COM marker!
    uint16 realLength = length - 2;
    uint8 * comData = new uint8[realLength+1];
    tsize_t readCount = mExifio->read(comData,realLength);
    
    if (  readCount == realLength )
    {
        mComMarkerList.addData( comData, realLength ) ;
    }
    else
    {
        status = EXIF_FILE_READ_ERROR ;
    }
    delete[] comData;
    return status;
}
}