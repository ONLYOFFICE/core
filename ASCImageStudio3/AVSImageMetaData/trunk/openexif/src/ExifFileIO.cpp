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


#include "ExifConf.h"
#include "ExifFileIO.h"

#if (defined _MSC_VER) && (defined WIN32)
#include <sys/stat.h>
#include <io.h>
#endif

#if defined(unix) || defined(__unix)
#include <sys/stat.h>
#elif defined(macintosh)
#include <stat.h>
#endif
namespace Exif{
// Read from the file
tsize_t ExifFileIO::read( tdata_t buf, tsize_t size)
{
    return fread(buf, size, 1, mFileHandle)*size ;
}

// Write to the file
tsize_t ExifFileIO::write( tdata_t buf, tsize_t size )
{
    return fwrite(buf, size, 1, mFileHandle)*size ;
}

// Seek to the given offset
exifoff_t ExifFileIO::seek( exifoff_t off, int whence)
{
    exifoff_t returnValue;
    if ( fseek(mFileHandle, off, whence) == 0 )
        returnValue = ftell(mFileHandle) ;
    else
        returnValue = -1 ;

    return returnValue ;
}

// Compute and return the size of the file
exifoff_t ExifFileIO::size()
{
    exifoff_t curPos;
    exifoff_t tempSize;
    curPos = this->seek(0, SEEK_CUR);
    tempSize = this->seek(0, SEEK_END);
    this->seek(curPos, SEEK_SET);
    return tempSize;
}

// Close the file
ExifStatus ExifFileIO::close()
{
    ExifStatus returnValue = EXIF_FILE_CLOSE_ERROR ;
    if ( mFileHandle )
    {
        if( fclose( mFileHandle ) == 0 )
            returnValue = EXIF_OK ;
    }
    return returnValue ;
}
}