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
 * Dan Graham dan.graham@kodak.com
 *
 * Contributor(s): 
 * Ricardo Rosario ricardo.rosario@kodak.com
 */ 

#if (defined _MSC_VER)
#pragma warning( disable : 4244 )
#endif

#define JPEG_INTERNALS

#include "ExifComp.h"
#include "ExifIJGWrapper.h"

#include "ExifIJGWriteFrame.h"

#include <string.h>
namespace Exif{
#ifndef OPENEXIF_NO_IJG
static    ExifColorSpace mapColorSpace(J_COLOR_SPACE jcs);
static  J_COLOR_SPACE mapIJGColorSpace(ExifColorSpace jcs);
#endif

//==============================================================================
//
//                            ExifJpegCompress
//
//==============================================================================

ExifJpegCompress::ExifJpegCompress() 
   :gJpegState(JPEG_INIT),
    gQualityFactor(90),
    gSmoothingFactor(0),
    gOptimizeEncoding(false),
    gProgressive(false),
    gDCTMethod(JDCT_ISLOW),
    gCompressBuf(NULL),
    gCompressBufSize(0),
    gCompressFile(NULL),
    gCompressFileName(NULL),
    gWriteApp0(false),
    tableScaleFactorSet(false),
    g422Sampling(false),
    gJpegTables(NULL),
    gUseCustomTables(false),
    gICCProfBuf(NULL),
    gDpiSet(false)
{
#ifndef OPENEXIF_NO_IJG
    gIJGCompInfo = new openexif_jpeg_compress_struct;
    memset(gIJGCompInfo,0,sizeof(openexif_jpeg_compress_struct));
#endif
}

ExifJpegCompress::~ExifJpegCompress(void)
{

#ifndef OPENEXIF_NO_IJG
    delete gIJGCompInfo;
#endif

    if(gCompressFileName) {
        delete gCompressFileName;
        gCompressFileName = NULL;
    }
    if(gJpegTables) {
        if(gJpegTables->Q0) {
            delete gJpegTables->Q0;
            gJpegTables->Q0 = NULL;
        }
        if(gJpegTables->Q1) {
            delete gJpegTables->Q1;
            gJpegTables->Q1 = NULL;
        }
        if(gJpegTables->Q2) {
            delete gJpegTables->Q2;
            gJpegTables->Q2 = NULL;
        }
        if(gJpegTables->Q3) {
            delete gJpegTables->Q3;
            gJpegTables->Q3 = NULL;
        }
        
        delete gJpegTables;
        gJpegTables = NULL;
    }
    if(gICCProfBuf) {
        delete gICCProfBuf;
        gICCProfBuf = NULL;
    }

}

JpegStatus 
ExifJpegCompress::setFileOutput(FILE *fp)
{
    gCompressFile = fp;
    return JPEG_OK;
}

JpegStatus 
ExifJpegCompress::setBufferOutput(unsigned char *buf, exif_uint32 bufSize)
{
    gCompressBuf = buf;
    gCompressBufSize = bufSize;
    return JPEG_OK;
}


JpegStatus 
ExifJpegCompress::setImageInfo(exif_uint32 width, exif_uint32 height,
                        unsigned short numComps, ExifColorSpace cs)
{
    gWidth = width;
    gHeight = height;
    gNumComps = numComps;
    gJpegColorSpace = gInputColorSpace = cs;
    return JPEG_OK;
}

JpegStatus ExifJpegCompress::setJpegQuantTable(
                unsigned char slot,
                unsigned short *qt,
                unsigned short qF)
{
    int i;
    if(qF > 0) {
        tableScaleFactorSet = true;
        tableScaleFactor = qF;
    }
    if(!gJpegTables) {
        gJpegTables = new JPEGTableHolder;
        gJpegTables->Q0 = NULL;
        gJpegTables->Q1 = NULL;
        gJpegTables->Q2 = NULL;
        gJpegTables->Q3 = NULL;
    }
    ExifJpegQTable *qPtr = NULL ;
    if(slot == 0) {
        gJpegTables->Q0 = new ExifJpegQTable;
        qPtr = gJpegTables->Q0;
    }
    else if (slot == 1) {
        gJpegTables->Q1 = new ExifJpegQTable;
        qPtr = gJpegTables->Q1;
    }
    else if (slot == 2) {
        gJpegTables->Q2 = new ExifJpegQTable;
        qPtr = gJpegTables->Q2;
    }
    else if (slot == 3) {
        gJpegTables->Q3 = new ExifJpegQTable;
        qPtr = gJpegTables->Q3;
    }
        if( qPtr )
            for(i = 0; i < DCTSIZE2; i++)
        qPtr->quantizer[i] = qt[i];

    gUseCustomTables = true;
    return JPEG_OK;
}
                
JpegStatus ExifJpegCompress::setJpegQuantTable(
                ExifJpegQTable*  Q0,
                  ExifJpegQTable*  Q1,
                  ExifJpegQTable*  Q2,
                  ExifJpegQTable*  Q3,
                  ExifJpegHUFFTable*   Huff_DC,
                  ExifJpegHUFFTable*   Huff_AC,
                  ExifJpegHUFFTable*   Huff_DC_Chroma,
                  ExifJpegHUFFTable*   Huff_AC_Chroma,
                ExifJpegTableOrder   tableOrder)
{

    if(!gJpegTables) {
        gJpegTables = new JPEGTableHolder;
        gJpegTables->Q0 = NULL;
        gJpegTables->Q1 = NULL;
        gJpegTables->Q2 = NULL;
        gJpegTables->Q3 = NULL;
    }

    jpegTableOrder = tableOrder;
    gUseCustomTables = true;
    
    if (Q0 != NULL) {
        if(gJpegTables->Q0) {
            delete gJpegTables->Q0;
            gJpegTables->Q0 = NULL;
        }
        gJpegTables->Q0 = new ExifJpegQTable;
        if (gJpegTables->Q0 == NULL) {
            return JPEG_ERROR;
        }
        *(gJpegTables->Q0) = *Q0;
    }
    
    if (Q1 != NULL) {
        if(gJpegTables->Q1) {
            delete gJpegTables->Q1;
            gJpegTables->Q1 = NULL;
        }
        gJpegTables->Q1 = new ExifJpegQTable;
        if (gJpegTables->Q1 == NULL) {
            return JPEG_ERROR;
        }
        *(gJpegTables->Q1) = *Q1;
    }
    
    if (Q2 != NULL) {
        if(gJpegTables->Q2) {
            delete gJpegTables->Q2;
            gJpegTables->Q2 = NULL;
        }
        gJpegTables->Q2 = new ExifJpegQTable;
        if (gJpegTables->Q2 == NULL) {
            return JPEG_ERROR;
        }
        *(gJpegTables->Q2) = *Q2;
    }
    
    if (Q3 != NULL) {
        if(gJpegTables->Q3) {
            delete gJpegTables->Q3;
            gJpegTables->Q3 = NULL;
        }
        gJpegTables->Q3 = new ExifJpegQTable;
        if (gJpegTables->Q3 == NULL) {
            return JPEG_ERROR;
        }
        *(gJpegTables->Q3) = *Q3;
    }
    
    return JPEG_OK;
}


#ifndef OPENEXIF_NO_IJG

// a handler for the error_exit method in the IJG error handling logic.
// this gets called when the library can not continue. So, we will
// longjmp our way back out (as opposed to the default IJG behavior of exit()).
void ExifJpegCompress::errorExitHandler(j_common_ptr cinfo)
{
    // at this point, we have a fatal error and must
    // longjmp our way back (ugh!!). we will get the
    // IJG message string and store it. Then the longjmp.
   
    /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
    my_error_mgr * myerr = (my_error_mgr *) cinfo->err;

    /* Create the message */
    (*cinfo->err->format_message) (cinfo, myerr->jpegMessageBuffer, JMSG_LENGTH_MAX);
    myerr->jpegStatus = JPEG_ERROR;

    // and jump off the bridge....

    /* Return control to the setjmp point */
    longjmp(myerr->setjmp_buffer, 1);
}

// This is a enumerated value from the IJG toolkit, and as such, it may change.
// However, the toolkit hasn't been worked on in several years, and I haven't
// seen any current efforts to update it, so we're going to use this value here.
#define JWRN_JFIF_MAJOR 119

void ExifJpegCompress::emitMessageHandler(j_common_ptr cinfo, int msgLevel)
{
    // at this point we have either a warning ( msgLeve == -1) or
    // just trace info. for the trace info, we just return and
    // keep going.
    if (msgLevel < 0 )
        // Bad JFIF version numbers don't concern us at the moment...
        if ( cinfo->err->msg_code != JWRN_JFIF_MAJOR )       
            // for now, other warnings are as good as fatals. jump!!!
            ExifJpegCompress::errorExitHandler(cinfo);

}
#endif

JpegStatus
ExifJpegCompress::startCompress()
{
    JpegStatus status = JPEG_OK;
    gLastLineWritten = 0;
    
#ifndef OPENEXIF_NO_IJG
    status = setupCompress();

    if(status != JPEG_OK)
        return status;

    if (setjmp(myErrorMgr.setjmp_buffer))
    {
        /* If we get here, the JPEG code has signaled an error.
        * We need to clean up the JPEG object and return.
        */
        openexif_jpeg_abort_compress(gIJGCompInfo);
        return myErrorMgr.jpegStatus;
    }  
    if(gDpiSet) {
        gIJGCompInfo->X_density = gXRes;
        gIJGCompInfo->Y_density = gYRes;
        gIJGCompInfo->density_unit = 1;
    }

    openexif_jpeg_start_compress(gIJGCompInfo, true);
    gJpegState = JPEG_STARTED;

#ifdef OPENEXIF_DCF_COMPLIANT
    // make our own write_frame_header so we only write 1 DQT marker!
    openexif_orig_write_frame_header = gIJGCompInfo->marker->write_frame_header;
    gIJGCompInfo->marker->write_frame_header = openexif_write_frame_header;

    // Again, make our own write_frame_header so we only write 1 DHT marker!
    openexif_orig_write_scan_header = gIJGCompInfo->marker->write_scan_header;
    gIJGCompInfo->marker->write_scan_header = openexif_write_scan_header;
#endif

    if(gICCProfBuf) {
        status = writeICCProfile();
    }
#endif    
    return status;
}


JpegStatus
ExifJpegCompress::setupCompress()
{
    JpegStatus status = JPEG_OK;

#ifndef OPENEXIF_NO_IJG

    if (setjmp(myErrorMgr.setjmp_buffer))
    {
        /* If we get here, the JPEG code has signaled an error.
         * We need to clean up the JPEG object and return.
         */
        openexif_jpeg_abort_compress(gIJGCompInfo);
        return myErrorMgr.jpegStatus;
    }  
    gIJGCompInfo->err = openexif_jpeg_std_error(&(myErrorMgr.pub));

    // setup the IJG error handler for this object.
    myErrorMgr.pub.error_exit = errorExitHandler;
    myErrorMgr.pub.emit_message = emitMessageHandler;

    openexif_jpeg_create_compress(gIJGCompInfo);
    if(gCompressFile)
        openexif_jpeg_stdio_dest(gIJGCompInfo, gCompressFile);
    else if(gCompressBuf) {
        openexif_jpeg_mem_dest(gIJGCompInfo, gCompressBuf);
        openexif_jpeg_mem_client_ptr memInfo = (openexif_jpeg_mem_client_ptr)(gIJGCompInfo->client_data) ;
        memInfo->bytes_in_buffer = 0;
        memInfo->size_buffer = gCompressBufSize;
    }

    gIJGCompInfo->image_width = gWidth;
    gIJGCompInfo->image_height = gHeight;
    gIJGCompInfo->input_components = gNumComps;
    gIJGCompInfo->in_color_space = mapIJGColorSpace(gInputColorSpace);

    openexif_jpeg_set_defaults(gIJGCompInfo);
    
    // set smoothing factor
    gIJGCompInfo->smoothing_factor = gSmoothingFactor;
    
    // set coding optimization
    gIJGCompInfo->optimize_coding = gOptimizeEncoding;
    
    if(gProgressive)
        openexif_jpeg_simple_progression(gIJGCompInfo);

    if(g422Sampling)
    {
        int numComponents = gIJGCompInfo->num_components;
        int i = 0;
        bool found = false;
        while( (i<numComponents) && (!found) )
        {
            if(gIJGCompInfo->comp_info[i].component_id == 1)
        {
                gIJGCompInfo->comp_info[i].v_samp_factor = 1;
                found = true;
            }
        i++;
        }
    }

    if(!gWriteApp0)
        gIJGCompInfo->write_JFIF_header = false;

    // set default compression parameter overrides
    // (quality, DCT method, Huffmann optimization, smoothing ... )

    // Jpeg quality
    openexif_jpeg_set_quality( gIJGCompInfo, gQualityFactor, true );

    // set DCT Method
    gIJGCompInfo->dct_method = gDCTMethod;
    
    if(gJpegTables)
    {
        int scaleFactor;
        unsigned short qF;
        if(tableScaleFactorSet)
            qF = tableScaleFactor;
        else
            qF = gQualityFactor;

        if (qF <= 0) qF = 1;
        if (qF > 100) qF = 100;
         if (qF < 50)
            scaleFactor = 5000 / qF;
        else
            scaleFactor = 200 - qF*2;

        exif_uint32 * newTable;
        newTable = new exif_uint32 [DCTSIZE2];
        int i;
        if(gJpegTables->Q3) 
        {
            for(i = 0; i < DCTSIZE2; i++) 
            {
                newTable[i] = gJpegTables->Q3->quantizer[i];
            }
            openexif_jpeg_add_quant_table(gIJGCompInfo, 0, (unsigned int *)newTable, scaleFactor, true);
        }
        if(gJpegTables->Q1)
        {
            for(i = 0; i < DCTSIZE2; i++)
            {
                newTable[i] = gJpegTables->Q1->quantizer[i];
            }
            openexif_jpeg_add_quant_table(gIJGCompInfo, 1, (unsigned int *)newTable, scaleFactor, true);
        }
        if(gJpegTables->Q2)
        {
            for(i = 0; i < DCTSIZE2; i++)
            {
                newTable[i] = gJpegTables->Q2->quantizer[i];
            }
            openexif_jpeg_add_quant_table(gIJGCompInfo, 2, (unsigned int *)newTable, scaleFactor, true);
            openexif_jpeg_component_info * compptr =  &gIJGCompInfo->comp_info[2];
            compptr->quant_tbl_no = 2;
        }
        delete [] newTable;
    }

#endif

    return status;

}

JpegStatus
ExifJpegCompress::stopCompress()
{
#ifndef OPENEXIF_NO_IJG
    if (setjmp(myErrorMgr.setjmp_buffer))
    {
        /* If we get here, the JPEG code has signaled an error.
        * We need to clean up the JPEG object and return.
        */
        openexif_jpeg_abort_compress(gIJGCompInfo);
        openexif_jpeg_destroy_compress(gIJGCompInfo);
        return myErrorMgr.jpegStatus;
    }  
    if(gJpegState != JPEG_STOPPED)
        openexif_jpeg_finish_compress(gIJGCompInfo);
	openexif_jpeg_destroy_compress(gIJGCompInfo);
#endif
    gJpegState = JPEG_STOPPED;
    return JPEG_OK;
}

JpegStatus 
ExifJpegCompress::writeScanLines(uint8 *buf, exif_uint32 bufLineStride, exif_uint32 numLines,
                             exif_uint32 *numLinesWritten, exif_uint32 *nextLine)
{
 #ifndef OPENEXIF_NO_IJG
   bool bTmpPtr = false;
    exif_uint32    dwRowBytes;
    unsigned short  linesCompressed;
    JSAMPARRAY    row_pointers;

    dwRowBytes = gIJGCompInfo->image_width;
    dwRowBytes *= gIJGCompInfo->input_components;
        
    row_pointers = (JSAMPARRAY) new(char[numLines * sizeof(JSAMPARRAY)]);
    exif_uint32 i,j;
    if(gInputColorSpace == JPEG_BGR)
    {
        JSAMPROW pDest;
        JSAMPLE *pTemp;
        bTmpPtr = true;
        for(i = 0; i < numLines; i++)
        {
            row_pointers[i] = pDest = (JSAMPROW) new(char[dwRowBytes]);
            pTemp = buf + (exif_uint32)i*bufLineStride;
            for(j=0; j<gIJGCompInfo->image_width; j++)
            {
                *(pDest+2) = *pTemp++;
                *(pDest+1) = *pTemp++;
                *(pDest) = *pTemp++;
                pDest += 3;
            }
        }
    }
    else
    {
        for (i=0; i<numLines; i++)
        {
            row_pointers[i] = buf + (exif_uint32)i*bufLineStride;    
        }
    }

    linesCompressed = openexif_jpeg_write_scanlines(gIJGCompInfo, row_pointers, numLines);
    
    if(bTmpPtr) {
        for(i=0; i< numLines; i++)
            delete [] row_pointers[i];
    }
    delete [] row_pointers;
    
    *numLinesWritten = linesCompressed;
    gLastLineWritten += linesCompressed;
    *nextLine = gLastLineWritten;
#endif
    return JPEG_OK;
}


exif_uint32 
ExifJpegCompress::getCompSize(void)
{
#ifndef OPENEXIF_NO_IJG
    if(gLastLineWritten >= gHeight) {
        // flush the compressed image buffer 
        if (setjmp(myErrorMgr.setjmp_buffer))
        {
            /* If we get here, the JPEG code has signaled an error.
            * We need to clean up the JPEG object and return.
            */
            openexif_jpeg_abort_compress(gIJGCompInfo);
            return 0;
        }  
        if(gJpegState != JPEG_STOPPED) {
            openexif_jpeg_finish_compress(gIJGCompInfo);
            gJpegState = JPEG_STOPPED;
        }
    }
    return ((openexif_jpeg_mem_client_ptr)(gIJGCompInfo->client_data))->bytes_in_buffer;
#else
	return 0;
#endif
}


JpegStatus ExifJpegCompress::setICCProfile(unsigned char *iccBuf, exif_uint32 iccSize)
{
    // Need to populate ICC Profile App buffer, 
    gICCProfBuf = new unsigned char [iccSize];
    gICCProfSize = iccSize;
    memcpy(gICCProfBuf, iccBuf, iccSize);
    
    return JPEG_OK;
}

#define MAX_ICC_SEG 0x00010000
void    WriteJPEGWord(unsigned char *p, int val)
{
    *p     = (unsigned char)( (val>> 8) & 255 );
    *(p+1) = (unsigned char)(  val      & 255 );
}

JpegStatus ExifJpegCompress::writeICCProfile(void)
{
    unsigned char *tmpAppBuf = NULL;
    unsigned char *writeBuf;
    exif_uint32 appBytesWritten = 0;
    char *iccId = "ICC_PROFILE";
    char iccIdBuf[12];
    memcpy(iccIdBuf,iccId,11);
    iccIdBuf[11] = 0x0;


    tmpAppBuf = new unsigned char[MAX_ICC_SEG];
    if(tmpAppBuf == NULL)
        return JPEG_ERROR;
    writeBuf = tmpAppBuf;
    exif_uint32 iccProfBytesRemaining = gICCProfSize;
    exif_uint32 iccMaxSegBytes = MAX_ICC_SEG - 24;
    unsigned char segNum, segTotal;
    exif_uint32 writeSize, iccWritten;
    segNum = 1;
    segTotal = gICCProfSize/iccMaxSegBytes;
    if(gICCProfSize - segTotal*iccMaxSegBytes > 0)
        segTotal ++;
    iccWritten = 0;
    while(iccProfBytesRemaining > 0) {
        if(iccProfBytesRemaining < iccMaxSegBytes)
            writeSize = iccProfBytesRemaining;
        else
            writeSize = iccMaxSegBytes;
        writeBuf = tmpAppBuf;

        WriteJPEGWord(writeBuf, 0xffe2); writeBuf+=2;
        appBytesWritten += 2;
 
        // App size = 16 + writeSize
        //      need to keep track of how big APP length is
        WriteJPEGWord(writeBuf, 16+writeSize);writeBuf += 2;
        appBytesWritten += 2;

        // write "APP" identifier
        memcpy(writeBuf, iccIdBuf, 12);
        writeBuf += 12;
        appBytesWritten += 12;

        // write 1 byte sequence number
        *writeBuf++ = segNum;
        // write 1 byte number of segments
        *writeBuf++ = segTotal;

        appBytesWritten += 2;

        memcpy(writeBuf,gICCProfBuf+iccWritten,writeSize);
        iccWritten += writeSize;
        appBytesWritten += writeSize;
        iccProfBytesRemaining -= writeSize;

#ifndef OPENEXIF_NO_IJG
        openexif_jpeg_write_marker(gIJGCompInfo,0xffe2,tmpAppBuf+4,appBytesWritten-4);
#endif
        segNum++;
        appBytesWritten = 0;
    }
    if(tmpAppBuf)
        delete tmpAppBuf;

    return JPEG_OK;
}


//==============================================================================
//
//                            ExifJpegDecompress
//
//==============================================================================
ExifJpegDecompress::ExifJpegDecompress() :
    gJpegState(JPEG_INIT),
    gDCTMethod(JDCT_ISLOW),
    gDecompressBuf(NULL),    
    gDecompressFile(NULL),
    gDecompressFileName(NULL),
    gJpegHeaderRead(false),
    gScaleFactor(JPEG_SCALE_NONE)
{
#ifndef OPENEXIF_NO_IJG
    gIJGDecompInfo = new openexif_jpeg_decompress_struct;
    memset(gIJGDecompInfo,0,sizeof(openexif_jpeg_decompress_struct));

    if (setjmp(myErrorMgr.setjmp_buffer))
    {
        /* If we get here, the JPEG code has signaled an error.
         * We need to clean up the JPEG object and return.
         */
        openexif_jpeg_abort_decompress(gIJGDecompInfo);
    //    return myErrorMgr.jpegStatus;
    }  
    gIJGDecompInfo->err = openexif_jpeg_std_error(&(myErrorMgr.pub));

    // setup the IJG error handler for this object.
    myErrorMgr.pub.error_exit = errorExitHandler;
    myErrorMgr.pub.emit_message = emitMessageHandler;

    openexif_jpeg_create_decompress(gIJGDecompInfo);
#endif
}

ExifJpegDecompress::~ExifJpegDecompress(void)
{
    if(gDecompressFileName)
        delete gDecompressFileName;
#ifndef OPENEXIF_NO_IJG
    openexif_jpeg_destroy_decompress(gIJGDecompInfo);
    delete gIJGDecompInfo;
#endif
}

JpegStatus 
ExifJpegDecompress::setFileInput(FILE *fp)
{
    gDecompressFile = fp;
    return JPEG_OK;
}

#ifdef WIN32
#ifdef INTERNET_REQUIRED
JpegStatus 
ExifJpegDecompress::setFileInput(HINTERNET internet)
{
    gInternetHandle = internet;
    return JPEG_OK;
}
#endif // INTERNET_REQUIRED
#endif // WIN32

JpegStatus 
ExifJpegDecompress::setBufferInput(unsigned char *buf, exif_uint32 bufSize)
{
    gDecompressBuf = buf;
    gDecompressBufSize = bufSize;
    return JPEG_OK;
}

JpegStatus 
ExifJpegDecompress::setScaling(ExifJpegScaleFactor scaleFactor) 
{
    gScaleFactor = scaleFactor; 
    getJpegScaleSize(gScaleFactor, &gScaledWidth, &gScaledHeight);

    return JPEG_OK;
}

JpegStatus 
ExifJpegDecompress::setScaling(unsigned short scaleFactor) 
{
    switch(scaleFactor) {
        case 1:
            gScaleFactor = JPEG_SCALE_NONE;
            break;
        case 2:
            gScaleFactor = JPEG_SCALE_HALF;
            break;
        case 4:
            gScaleFactor = JPEG_SCALE_QUARTER;
            break;
        case 8:
            gScaleFactor = JPEG_SCALE_EIGHTH;
            break;
    }
    getJpegScaleSize(gScaleFactor, &gScaledWidth, &gScaledHeight);

    return JPEG_OK;
}

JpegStatus 
ExifJpegDecompress::getJpegScaleSize(ExifJpegScaleFactor sF,
                exif_uint32 *width, exif_uint32* height)
{
    switch(sF) {
        case JPEG_SCALE_NONE:
            *width = (int)gWidth;
            *height = (int)gHeight;
            break;
        case JPEG_SCALE_HALF:
            *width = (int)((gWidth+1)/2);
            *height = (int)((gHeight+1)/2);
            break;
        case JPEG_SCALE_QUARTER:
            *width = (int)((gWidth+3)/4);
            *height = (int)((gHeight+3)/4);
            break;            
        case JPEG_SCALE_EIGHTH:
            *width = (int)((gWidth+7)/8);
            *height = (int)((gHeight+7)/8);
            break;            
        default:
            break;
    }
    return JPEG_OK;
}

#ifndef OPENEXIF_NO_IJG

// a handler for the error_exit method in the IJG error handling logic.
// this gets called when the library can not continue. So, we will
// longjmp our way back out (as opposed to the default IJG behavior of exit()).
void ExifJpegDecompress::errorExitHandler(j_common_ptr cinfo)
{
    // at this point, we have a fatal error and must
    // longjmp our way back (ugh!!). we will get the
    // IJG message string and store it. Then the longjmp.
   
    /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
    my_error_mgr * myerr = (my_error_mgr *) cinfo->err;

    /* Create the message */
    (*cinfo->err->format_message) (cinfo, myerr->jpegMessageBuffer, JMSG_LENGTH_MAX);
    myerr->jpegStatus = JPEG_ERROR;

    // and jump off the bridge....

    /* Return control to the setjmp point */
    longjmp(myerr->setjmp_buffer, 1);
}

void ExifJpegDecompress::emitMessageHandler(j_common_ptr cinfo, int msgLevel)
{
    // at this point we have either a warning ( msgLeve == -1) or
    // just trace info. for the trace info, we just return and
    // keep going.
    if (msgLevel < 0 )
        // Bad JFIF version numbers don't concern us at the moment...
        if ( cinfo->err->msg_code != JWRN_JFIF_MAJOR )       
            // for now, other warnings are as good as fatals. jump!!!
            ExifJpegDecompress::errorExitHandler(cinfo);
}

#endif

JpegStatus 
ExifJpegDecompress::setOutputColorSpace(ExifColorSpace outputCS)

{
    gOutputColorSpace = outputCS; 
    return JPEG_OK;
}

JpegStatus 
ExifJpegDecompress::startDecompress()
{
    JpegStatus status = JPEG_OK;
    gLastLineRead = 0;

#ifndef OPENEXIF_NO_IJG
    if(!gJpegHeaderRead) {
        status = getImageInfo(&gWidth, &gHeight, &gNumComps);
        if(status != JPEG_OK)
            return status;
    }

    if (setjmp(myErrorMgr.setjmp_buffer))
    {
        /* If we get here, the JPEG code has signaled an error.
        * We need to clean up the JPEG object and return.
        */
        openexif_jpeg_abort_decompress(gIJGDecompInfo);
        return JPEG_ERROR;
    }  

    int scaleDenom = 1;
    switch(gScaleFactor) {
        case JPEG_SCALE_HALF:
            scaleDenom = 2;
            break;
        case JPEG_SCALE_QUARTER:
            scaleDenom = 4;
            break;
        case JPEG_SCALE_EIGHTH:
            scaleDenom = 8;
            break;
        default:
            break;
    }
    gIJGDecompInfo->scale_denom = scaleDenom;
    openexif_jpeg_calc_output_dimensions(gIJGDecompInfo);

    gIJGDecompInfo->dct_method = gDCTMethod;

    J_COLOR_SPACE outColor;
    outColor = mapIJGColorSpace(gOutputColorSpace);
    gIJGDecompInfo->out_color_space = outColor;

    openexif_jpeg_start_decompress(gIJGDecompInfo);
#endif
    gJpegState = JPEG_STARTED;

    return JPEG_OK;
}

JpegStatus 
ExifJpegDecompress::stopDecompress()
{
#ifndef OPENEXIF_NO_IJG
    if (setjmp(myErrorMgr.setjmp_buffer))
    {
        /* If we get here, the JPEG code has signaled an error.
        * We need to clean up the JPEG object and return.
        */
        openexif_jpeg_abort_decompress(gIJGDecompInfo);
        return myErrorMgr.jpegStatus;
    }
    if(gJpegState != JPEG_STOPPED)
        openexif_jpeg_finish_decompress(gIJGDecompInfo);

    gJpegHeaderRead = false;
#endif
	gJpegState = JPEG_STOPPED;
    return JPEG_OK;
}

JpegStatus 
ExifJpegDecompress::resetDecompress()
{
#ifndef OPENEXIF_NO_IJG
    if (setjmp(myErrorMgr.setjmp_buffer)) {
        return myErrorMgr.jpegStatus;
    }  

    openexif_jpeg_abort_decompress(gIJGDecompInfo);
#endif
	gJpegHeaderRead = false;
    
    return JPEG_OK;
}

JpegStatus 
ExifJpegDecompress::getImageInfo(exif_uint32 *width, exif_uint32 *height,
                        unsigned short *numComps)
{
    JpegStatus status = JPEG_READHEADER_ERROR;

    if(!gJpegHeaderRead)
	{
#ifndef OPENEXIF_NO_IJG
		status = setupDecompress();

		if ((status == JPEG_OK)&&(setjmp(myErrorMgr.setjmp_buffer)))
		{
			/* If we get here, the JPEG code has signaled an error.
			* We need to clean up the JPEG object and return.
			*/
			openexif_jpeg_abort_decompress(gIJGDecompInfo);
			status = JPEG_READHEADER_ERROR;
		}  
	        
		if ((status == JPEG_OK)&&(openexif_jpeg_read_header(gIJGDecompInfo, true) != JPEG_SUSPENDED))
		{
			gWidth = gIJGDecompInfo->image_width;
			gHeight = gIJGDecompInfo->image_height;
			gNumComps = gIJGDecompInfo->num_components;
			gPrecision = gIJGDecompInfo->data_precision;
			gOutputColorSpace = mapColorSpace(gIJGDecompInfo->out_color_space);
		}
		else 
			status = JPEG_READHEADER_ERROR;    
	    
		*width = gScaledWidth = gWidth;
		*height = gScaledHeight = gHeight;
		*numComps = gNumComps;
#endif
		gJpegHeaderRead = true;
	}
    return status;
}

#ifndef OPENEXIF_NO_IJG
JpegStatus
ExifJpegDecompress::setupDecompress(void)
{
    JpegStatus status = JPEG_OK;

/*
        if (setjmp(myErrorMgr.setjmp_buffer))
        {
            // If we get here, the JPEG code has signaled an error.
            // We need to clean up the JPEG object and return.
            //
            openexif_jpeg_abort_decompress(gIJGDecompInfo);
            return myErrorMgr.jpegStatus;
        }  
        gIJGDecompInfo->err = openexif_jpeg_std_error(&(myErrorMgr.pub));

        // setup the IJG error handler for this object.
        myErrorMgr.pub.error_exit = errorExitHandler;
        myErrorMgr.pub.emit_message = emitMessageHandler;

        openexif_jpeg_create_decompress(gIJGDecompInfo);
*/

    if(gDecompressFile)
        openexif_jpeg_stdio_src(gIJGDecompInfo, gDecompressFile);
#ifdef WIN32
#ifdef INTERNET_REQUIRED
    else if(gInternetHandle)
        openexif_jpeg_internet_src(gIJGDecompInfo,gInternetHandle);
#endif // INTERNET_REQUIRED
#endif // WIN32
    else if(gDecompressBuf)
        openexif_jpeg_mem_src(gIJGDecompInfo, gDecompressBuf, gDecompressBufSize);
    else
        status = JPEG_SOURCE_NOT_SET;

    return status;
}
#endif

JpegStatus 
ExifJpegDecompress::readScanLines(uint8 *readBuf, exif_uint32 numLines,
                        exif_int32 lineStride,
                        exif_uint32 *numRead, exif_uint32 *nextLine)
{
#ifndef OPENEXIF_NO_IJG
    JSAMPROW    row_pointer[1];
    exif_uint32 dwRowBytes, sRead, i,j;
    unsigned short linesDecompressed ;
    unsigned char *pDest;
    unsigned char *pSrc;

    dwRowBytes = gIJGDecompInfo->output_width;
    dwRowBytes *= gIJGDecompInfo->out_color_components;
   
    linesDecompressed = 0;

    row_pointer[0] = (JSAMPROW) new(char[dwRowBytes]);

    if (setjmp(myErrorMgr.setjmp_buffer))
    {
        /* If we get here, the JPEG code has signaled an error.
        * We need to clean up the JPEG object and return.
        */
        openexif_jpeg_abort_decompress(gIJGDecompInfo);
        return JPEG_ERROR;
    }  

    for (i=0; i<numLines; i++) 
    {
        sRead = openexif_jpeg_read_scanlines(gIJGDecompInfo, row_pointer, 1);
        if (sRead != 1)
        {
            linesDecompressed = 0;
            break;
        }
        linesDecompressed += sRead;

        pSrc = row_pointer[0];
        pDest = readBuf + i*lineStride;
        if(gOutputColorSpace == JPEG_BGR) {
            for(j = 0; j<gIJGDecompInfo->output_width; j++, pSrc+=3) {
                *pDest++ = *(pSrc+2);
                *pDest++ = *(pSrc+1);
                *pDest++ = *(pSrc);
            }
        }
        else {
            for (j=0; j<dwRowBytes; j++)
                *pDest++ = *pSrc++;
        }
    }    

    *numRead = linesDecompressed;
    *nextLine = gIJGDecompInfo->output_scanline;

    delete [] (char *) row_pointer[0];
#endif
    return JPEG_OK;
}


JpegStatus 
ExifJpegDecompress::getJpegQuantTable(uint8 quantIndex, uint16 *table)
{
	JpegStatus status = JPEG_OK;
#ifndef OPENEXIF_NO_IJG
    int i;
    if (gIJGDecompInfo->quant_tbl_ptrs[quantIndex]) {
        for( i = 0; i < DCTSIZE2; i++)
            table[i] =  gIJGDecompInfo->quant_tbl_ptrs[quantIndex]->quantval[i];
    }
    else 
        status = JPEG_ERROR;
#else
	status = JPEG_ERROR;
#endif
	return status;
}


#ifndef OPENEXIF_NO_IJG
ExifColorSpace 
mapColorSpace(J_COLOR_SPACE jcs)
{
    ExifColorSpace cs;

    switch (jcs) {
        case JCS_RGB:
            cs = EXIF_RGB;
            break;
        case JCS_GRAYSCALE:
            cs = JPEG_GRAYSCALE;
            break;
        default:
            cs = EXIF_RGB;
            break;
    }
    return cs;
}

J_COLOR_SPACE 
mapIJGColorSpace(ExifColorSpace jcs)
{
    J_COLOR_SPACE cs;

    switch (jcs) {
        case EXIF_RGB:
        case JPEG_BGR:
            cs = JCS_RGB;
            break;
        case JPEG_GRAYSCALE:
            cs = JCS_GRAYSCALE;
            break;
        case EXIF_YCbCr:
            cs = JCS_YCbCr;
            break;
        default:
            cs = JCS_RGB;
            break;
    }
    return cs;
}
#endif

uint8 ExifJpegDecompress::getHSamplingFactor(uint8 componentID)
{
    uint8 returnValue = 0;
    
#ifndef OPENEXIF_NO_IJG
    if(gIJGDecompInfo!=NULL)
    {
        int numComponents = gIJGDecompInfo->num_components;
        int i = 0;
        bool found = false;
        while( (i<numComponents) && (!found) )
        {
            if( gIJGDecompInfo->comp_info[i].component_id == componentID )
            {
                returnValue = (uint8)gIJGDecompInfo->comp_info[i].h_samp_factor;
                found = true;
            }
            i++;
        }
    
    }
#endif

    return returnValue;
}

/*
void ExifJpegCompress::setHSamplingFactor(uint8 componentID, uint8 value)
{
    if(gIJGCompInfo!=NULL)
    {
        int numComponents = gIJGCompInfo->num_components;
    int i = 0;
    bool found = false;
    while ( (i<numComponents) && (!found) )
    {
            if (gIJGCompInfo->comp_info[i].component_id == componentID)
        {
            gIJGCompInfo->comp_info[i].h_samp_factor = value;
        }
        i++;
    }
    }

}
*/

uint8 ExifJpegDecompress::getVSamplingFactor(uint8 componentID)
{
    uint8 returnValue = 0;
    
#ifndef OPENEXIF_NO_IJG
    if(gIJGDecompInfo!=NULL)
    {
        int numComponents = gIJGDecompInfo->num_components;
        int i = 0;
        bool found = false;
        while( (i<numComponents) && (!found) )
        {
            if( gIJGDecompInfo->comp_info[i].component_id == componentID )
            {
                returnValue = (uint8)gIJGDecompInfo->comp_info[i].v_samp_factor;
                found = true;
            }
            i++;
        }
    
    }
#endif

    return returnValue;
}

/*
void ExifJpegCompress::setVSamplingFactor(uint8 componentID, uint8 value)
{
    if(gIJGCompInfo!=NULL)
    {
        int numComponents = gIJGCompInfo->num_components;
    int i = 0;
    bool found = false;
    while( (i<numComponents) && (!found) )
        {
            if ( gIJGCompInfo->comp_info[i].component_id == componentID )
            {
                gIJGCompInfo->comp_info[i].v_samp_factor = value;
            }
        i++;
        }
    }
}
*/
}