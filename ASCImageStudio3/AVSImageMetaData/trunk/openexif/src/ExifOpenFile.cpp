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

#include "ExifConf.h"
#include "ExifErrorCodes.h"
#include "ExifIO.h"
#include "ExifOpenFile.h"
#include "..\..\common\utils.h"
namespace Exif{
ExifOpenFile::~ExifOpenFile()
{
    close() ;
}

ExifStatus ExifOpenFile::close( void )
{
    if ( mExifio )
    {
        if( mIOwnExifIO )
        {
            delete mExifio ;
        }
        mExifio = NULL ;
    }
    
    return EXIF_OK;
}

ExifStatus ExifOpenFile::open( const char* filename, const char* cmode)
{
	return this->open((wchar_t *)(AVS::Utils::__StoW(filename).c_str()), cmode);
}

ExifStatus ExifOpenFile::open( const wchar_t* filename, const char* cmode)
{
    ExifStatus status = EXIF_FILE_OPEN_ERROR ;

    mIOwnExifIO = true ;
    mExifio = ExifIO::open( status, filename, cmode ) ;
  
    if( mExifio )
    {
        mFileName = filename ;
        status = initAfterOpen( cmode ) ;
    }

    return status ;
}


ExifStatus ExifOpenFile::open(fhandle_t fileHandle, const char* name,
                              const char* cmode)
{
	return this->open(fileHandle, (wchar_t*)(AVS::Utils::__StoW(name).c_str()), cmode);
}

ExifStatus ExifOpenFile::open(fhandle_t fileHandle, const wchar_t* name,
                              const char* cmode)
{
    ExifStatus status ;

    mExifio = new ExifIO( status, fileHandle, name, cmode ) ;
    if( status != EXIF_OK )
    {
        delete mExifio ;
        mExifio = NULL ;
    }
    else
    {
        mFileName = name ;
        mIOwnExifIO = true ;
        status = initAfterOpen( cmode ) ;
    }

    return status ;
}

// Open a file from memory
ExifStatus ExifOpenFile::open(void* imgBuf, tsize_t bufSize, const char* cmode,
    tsize_t bytesWritten)
{
    ExifStatus status = EXIF_OK;
    
    mExifio = new ExifIO(status, imgBuf, bufSize, cmode, bytesWritten) ;
    if( status != EXIF_OK )
    {
        delete mExifio ;
        mExifio = NULL ;
    }
    else
    {
        mIOwnExifIO = true ;
        status = initAfterOpen(cmode) ;
    }

    return status ;
}
}