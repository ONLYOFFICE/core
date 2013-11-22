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

#ifndef _EXIF_JPEG_EXTN_H_
#define _EXIF_JPEG_EXTN_H_

#ifndef OPENEXIF_NO_IJG

#include "OpenExifJpeg\OpenExifJpegLib.h"
#define SIZEOF(object)       ((size_t) sizeof(object))
namespace Exif 
{

typedef struct {
        int bytes_in_buffer ;
        size_t size_buffer ;
} openexif_jpeg_mem_client;

typedef openexif_jpeg_mem_client* openexif_jpeg_mem_client_ptr;

typedef struct {
  struct openexif_jpeg_destination_mgr pub; /* public fields */

  JOCTET *NIFbuffer;
  JOCTET *buffer;
} nif_destination_mgr;

typedef nif_destination_mgr *nif_dest_ptr;


/* and similarly for an in memory version for NIFTY. */
typedef struct {
  struct openexif_jpeg_source_mgr pub;   /* public fields */

  int    bufSize;
  JOCTET *NIFbuffer;
  JOCTET *buffer;
  boolean start_of_file;
  int   offset;
} nif_source_mgr;

typedef nif_source_mgr *nif_src_ptr;

/* and one for macintosh FSSpec */
#ifdef macintosh
typedef struct {
  struct jpeg_source_mgr pub;   /* public fields */
 
  short refNum;
  JOCTET * buffer;      /* start of buffer */
  boolean start_of_file;    /* have we gotten any data yet? */
} nif_ref_source_mgr;
typedef nif_ref_source_mgr *nif_ref_src_ptr;
#endif  /* macintosh */


EXTERN(void) openexif_jpeg_mem_dest JPP((j_compress_ptr cinfo, JOCTET *buffer));
EXTERN(void) openexif_jpeg_mem_src JPP((j_decompress_ptr cinfo, JOCTET *buffer,
                              int bufsize));
#ifdef macintosh
EXTERN(void) jpeg_ref_src JPP((j_decompress_ptr cinfo, short refNum));
#endif

#ifdef JPEG_INTERNET_ENABLED
#ifdef WIN32
#ifdef INTERNET_REQUIRED
EXTERN(void) jpeg_internet_dest JPP((j_compress_ptr cinfo, HINTERNET outfile));
EXTERN(void) jpeg_internet_src JPP((j_decompress_ptr cinfo, HINTERNET infile));
#endif // INTERNET_REQUIRED
#endif
#endif
}
#endif

#endif
