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
 * FirstName LastName <email address>
 * FirstName LastName <email address>
 */


#include "ExifImageDescUtils.h"
namespace Exif{
ExifStatus exifAllocImageDesc( ExifImageInfo& imgInfo,
                              ExifImageDesc &imageDesc)
{
    imageDesc.numberOfColumns = imgInfo.width ;
    imageDesc.numberOfRows = imgInfo.height ;
    imageDesc.numberOfComponents = imgInfo.numChannels ;

    uint8 *tmpReadBuf = 
        new uint8[imgInfo.width*imgInfo.height*imgInfo.numChannels];

    if(!tmpReadBuf)
        return EXIF_LOW_MEMORY_ERROR;

    for( int i=0; i<imgInfo.numChannels; i++)
    {
        imageDesc.components[i].myColorType.myDataType = UNSIGNED_BYTE;
        imageDesc.components[i].columnStride = imgInfo.numChannels;
        imageDesc.components[i].lineStride = imgInfo.numChannels*imgInfo.width;
        // Data is interleaved, e.g., RGBRGB
        imageDesc.components[i].theData = tmpReadBuf+i;
    }

    return EXIF_OK ;
}

ExifStatus exifDeleteImageDesc( ExifImageDesc &imageDesc )
{

    delete [] imageDesc.components[0].theData ;
    
    return EXIF_OK ;
}
}
