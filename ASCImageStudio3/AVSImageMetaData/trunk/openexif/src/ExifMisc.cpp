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
 * Creation Date: 8/28/2002
 *
 * Original Author: 
 * Ricardo Rosario ricardo.rosario@kodak.com
 *
 * Contributor(s): 
 *
 * 
 */ 

#include <iostream>
#include "ExifMisc.h"

#ifdef _MSC_VER
#include <direct.h>
#include <windows.h>
#pragma warning (disable:4800)

#elif defined(macintosh)

// This should really check whether we're linking against the Metrowerks
// Standard Libraries or actual POSIX libraries. This could make a difference
// for MacOS X, which is built on a Unix core.

#include <stdio.h>
#include <stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <Navigation.h>
#include <stdlib.h>
//#include <XMLString.hpp> 
//#include <util/PlatformUtils.hpp>
//#include <util/Platforms/MacOS/MacOSPlatformUtils.hpp>

// The Metrowerks standard libraries (MSL) don't implement the POSIX unlink
// function, and it would be very complicated to try and convert the POSIX
// style pathnames into MacOS file descriptors. As such, a copy file function
// is implemented natively to Pmt.
#define MACOSFILECOPYBUFFERSIZE	102400	// 100KB

#else  // #else assumes Unix for now.

#include <stdio.h>

#endif

namespace Exif{

#ifdef _MSC_VER
bool ExifCopyFile(const char * fromPath, const char * toPath,
                  bool overwriteIfExists)
{
    return CopyFileA(fromPath, toPath, ! overwriteIfExists);
}


#elif defined(macintosh)


bool ExifCopyFile(const char *fromPath, const char * toPath,
                  bool overwriteIfExists)
{
	char *buf;					// Intermediate buffer for file data
	int fromFd, toFd;			// Input and output file descriptors
	int exclusive;				// Exclusive flag for opening output file
	int numRead;				// Number of bytes read from the source file
								// in the current chunk

    // If we're not supposed to overwrite the existing file, and the file
    // already exists, then return false.
    if (! overwriteIfExists)
    {
        if ((toFd = open(toPath, O_RDONLY)) == -1)
        {
            return false;
        }
    }
	
	// Allocate memory for the intermediate buffer
	if ((buf = (char *)malloc(MACOSFILECOPYBUFFERSIZE)) == NULL) return false;
	
	// Open the source and destination files
	if ((fromFd = open(fromPath, O_RDONLY)) == -1)
	{	free(buf);
		return false;
	}

    // We already checked above, to make sure we will not incorrectly overwrite
    // the file, so here we can open it without the O_EXCL flag.
    if ((toFd = open(toPath, O_WRONLY | O_CREAT)) == -1)
    {
        free(buf);
        close(fromFd);
        return false;
    }
	
	// Read the input file, one buffer at a time, and write the data to the
    // output file
	while (1)
	{	// Try and read the next chunk
		if ((numRead = read(fromFd, buf, MACOSFILECOPYBUFFERSIZE)) < 1) break;

		// Write the data we just read, and break out on an error
		if ((numRead = write(toFd, buf, numRead)) == -1) break;
	}
	
	// Close the files and dispose of the buffer
	close(fromFd);
	close(toFd);
	free(buf);
	
	// If an error occurred (numRead = -1) return false, othewise, return true
	if (numRead == -1) return false;
	else return true;
}		
	

#else  // #else assumes Unix for now.


bool ExifCopyFile(const char * fromPath, const char * toPath,
                  bool overwriteIfExists)
{
  bool retVal = false;
  FILE * myFile1;
  FILE * myFile2;
  const int BUF_SIZE = 102400;

  // If we're not supposed to overwrite the existing file, and the file
  // already exists, then return false.
  if (! overwriteIfExists)
  {
    // Check for existance of file.
    myFile2 = fopen(toPath, "r");
    if (myFile2)
    {
      fclose(myFile2);
      return false;
    }
  }

  myFile1 = fopen(fromPath, "r");
  if (myFile1)
  {
    myFile2 = fopen(toPath, "w+");
    if (myFile2)
    {
      char * buf = new char[BUF_SIZE];
      if (buf)
      {
	size_t sizeRead;
	while ( (sizeRead = fread(buf,sizeof(char), BUF_SIZE - 1, myFile1)) )
	{
	  fwrite(buf, sizeof(char), sizeRead, myFile2);
	}

	delete [] buf;
      }

      fclose(myFile2);

      retVal = true;
    }   

    fclose(myFile1);
  }

  return retVal;
}


#endif
}