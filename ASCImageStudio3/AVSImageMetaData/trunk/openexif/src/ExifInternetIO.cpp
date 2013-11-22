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


#ifdef WIN32
#ifdef INTERNET_REQUIRED

#include "ExifInternetIO.h"

#include <WinInet.h>
#pragma comment (lib , "WinInet")
namespace Exif{
tsize_t
ExifInternetIO::read( tdata_t buf, tsize_t size)
{
	if (mHInternet) 
	{
		unsigned int szread;
		BOOL b = InternetReadFile(mHInternet, buf, size, &szread);
		return b? szread: 0;
	}

	return 0;
}



tsize_t
ExifInternetIO::write( tdata_t buf, tsize_t size )
{
	return 0;
}


exifoff_t
ExifInternetIO::seek( exifoff_t off, int whence)
{
	DWORD dwMoveMethod;
	switch(whence)
	{
	case 0:
		dwMoveMethod = FILE_BEGIN;
		break;
	case 1:
		dwMoveMethod = FILE_CURRENT;
		break;
	case 2:
		dwMoveMethod = FILE_END;
		break;
	default:
		dwMoveMethod = FILE_BEGIN;
		break;
	}
	return (exifoff_t)InternetSetFilePointer(mHInternet, off, 0, dwMoveMethod, 0);
}


exifoff_t ExifInternetIO::size()
{
	unsigned int sz;
	InternetQueryDataAvailable(mHInternet, &sz, 0, 0);
	return sz;
}


ExifStatus ExifInternetIO::close()
{
    InternetCloseHandle(mHInternet);
    
    return EXIF_OK ;
}
}
#endif // INTERNET_REQUIRED
#endif // WIN32
