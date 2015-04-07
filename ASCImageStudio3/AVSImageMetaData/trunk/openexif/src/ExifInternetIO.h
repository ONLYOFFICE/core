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
 * FirstName LastName <email address>
 * FirstName LastName <email address>
 */ 


#ifndef _EXIF_INTERNET_IO_H_
#define _EXIF_INTERNET_IO_H_

#ifdef WIN32
#ifdef INTERNET_REQUIRED

#include "ExifIO.h"
namespace Exif 
{

class ExifInternetIO : public ExifHandleIO
{
    public:
	ExifInternetIO( ExifIO& theParent, HINTERNET hInet )
            : ExifHandleIO(theParent), mHInternet(hInet) {}
	~ExifInternetIO( ) {}
        
	virtual tsize_t read( tdata_t buf, tsize_t size ) ;
	virtual tsize_t write( const tdata_t buf, const tsize_t size ) ;
	virtual exifoff_t seek( exifoff_t off, int whence ) ;
	virtual exifoff_t size();
	virtual ExifStatus close();
        
    private:
	HINTERNET	mHInternet;
};
}
#endif // INTERNET_REQUIRED
#endif // WIN32

#endif // EXIF_INTERNET_IO_H
