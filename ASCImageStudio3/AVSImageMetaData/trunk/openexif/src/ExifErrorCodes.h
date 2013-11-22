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


#ifndef _EXIF_ERROR_CODES_H_
#define _EXIF_ERROR_CODES_H_


namespace Exif 
{

// The type of an error returned by a EXIF routine
//typedef int ExifStatus;

/****************************************************************************
  TOOLKIT ERROR CODES
 ****************************************************************************/

typedef enum 
{
    EXIF_OK = 0,
    EXIF_ERROR,
    EXIF_INVALID_FORMAT_ERROR,
    EXIF_FILE_WRITE_ERROR,
    EXIF_FILE_READ_ERROR,
    EXIF_FILE_NOT_FOUND,
    EXIF_INVALID_APP_ID,
    EXIF_FILE_OPEN_ERROR,
    EXIF_FILE_CLOSE_ERROR,
    EXIF_FILE_OPEN_MODE_NOT_SUPPORTED,
    EXIF_CANNOT_RATIONALIZE_NUMBER,
    EXIF_RATIONAL_HAS_ZERO_DENOMINATOR,
    EXIF_TAG_IS_NOT_A_SUBIFD,
    EXIF_TAG_NOT_FOUND,
    EXIF_LOW_MEMORY_ERROR,
    EXIF_BAD_MAGIC_NUMBER,
    EXIF_BAD_IFD_PATH,
    EXIF_NO_APP1_ERROR,
    EXIF_NO_IFD0_ERROR,
    EXIF_NO_EXIFIFD_ERROR,
    EXIF_NO_IFD1_ERROR,
    EXIF_INVALID_TAG_LOCATION,
    EXIF_FILE_CLOSE_NO_IMAGE_ERROR,

    EXIF_JPEG_HUFFMAN_TABLE_ERROR,

    EXIF_AUDIO_NOT_AVAILABLE,
    EXIF_SUPPLIED_AUDIO_BUFFER_TOO_SMALL,

    /* Audio Extension error codes. */
    EXIF_INVALID_AUDIO_HANDLE = 2000,
    EXIF_INVALID_OWNING_STORAGE = 2001,
    EXIF_AUDIO_STREAM_HAS_BAD_CLSID = 2002,
    EXIF_ERROR_WRITING_TO_AUDIO_STREAM = 2003,
    EXIF_AUDIO_STORGE_COMMIT_ERROR = 2004,
    EXIF_AUDIO_STORGE_HAS_BAD_CLSID = 2005,
    EXIF_AUDIO_INFO_HAS_BAD_CLSID = 2006,
    EXIF_INVALID_AUDIO_INFO_PSET_FORMAT = 2007,
    EXIF_ERROR_WRITING_AUDIO_INFO = 2008,
    EXIF_NONEXISTENT_AUDIO_INFO = 2009,
    EXIF_NO_SECTION_IN_AUDIO_INFO = 2010,
    EXIF_INVALID_CREATED_EDITED_APP_CLSID = 2011,
    EXIF_INVALID_STREAM_OFFSET = 2012,
    EXIF_NONEXISTENT_STORAGE = 2013,
    EXIF_NONEXISTENT_AUDIO_EXTENSION = 2014,


	
    /* Exif Conversion Error Codes */

    EXIF_FPXR_RELATED_SOUND_IN_APP1 = 3000,   		//(WARNING)Embeded Audio and also Related sound tag exist
    EXIF_FPXR_NO_EMBEDED_AUDIO =  3001,    			//No Embeded Audio in APP2 segement
    EXIF_FPXR_ALREADY_EMBEDED_AUDIO_EXIST = 3002,   	//Already embeded audio is in APP2 segment
    EXIF_FPXR_INVALID_EXIFAUDIOHEADER = 3003,    		//invalid EXIFAudioHeader is pointed
    //(Judged by MagicNumber checking in OBJ)
    EXIF_FPXR_EMBEDED_AUDIO_EOF = 3004,   			//Tried to extract audio beyond the EndOfStream
    EXIF_FPXR_NO_MANDATORY_TAG_TO_CONVERT = 3005,    //lacking mandatory EXIF tag to get info. for conversion
    EXIF_FPXR_INVALID_MODE = 3006    //Invalid operation against openedmode


} ExifStatus;
}

#endif











