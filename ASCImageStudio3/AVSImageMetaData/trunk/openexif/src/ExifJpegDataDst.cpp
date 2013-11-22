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

#ifndef HAVE_SWIO_JPEG

#include "ExifConf.h"

#ifndef OPENEXIF_NO_IJG

#include "ExifJpegExtn.h"
#include "OpenExif_jerror.h"
#include "string.h"

#define OUTPUT_BUF_SIZE  4096	/* choose an efficiently fwrite'able size */

namespace Exif{
/*
 * Initialize destination --- called by jpeg_start_compress
 * before any data is actually written.
 */

/* and a variation of this for in memory writing for NIFty. */
METHODDEF(void)
init_mem_destination (j_compress_ptr cinfo)
{
  nif_dest_ptr dest = (nif_dest_ptr) cinfo->dest;
 
  /* Allocate the output buffer --- it will be released when done with image */
  dest->buffer = (JOCTET *)
      (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_IMAGE,
                  OUTPUT_BUF_SIZE * SIZEOF(JOCTET));
 
  dest->pub.next_output_byte = dest->buffer;
  dest->pub.free_in_buffer = OUTPUT_BUF_SIZE;
}


/*
 * Empty the output buffer --- called whenever buffer fills up.
 *
 * In typical applications, this should write the entire output buffer
 * (ignoring the current state of next_output_byte & free_in_buffer),
 * reset the pointer & count to the start of the buffer, and return TRUE
 * indicating that the buffer has been dumped.
 *
 * In applications that need to be able to suspend compression due to output
 * overrun, a FALSE return indicates that the buffer cannot be emptied now.
 * In this situation, the compressor will return to its caller (possibly with
 * an indication that it has not accepted all the supplied scanlines).  The
 * application should resume compression after it has made more room in the
 * output buffer.  Note that there are substantial restrictions on the use of
 * suspension --- see the documentation.
 *
 * When suspending, the compressor will back up to a convenient restart point
 * (typically the start of the current MCU). next_output_byte & free_in_buffer
 * indicate where the restart point will be if the current call returns FALSE.
 * Data beyond this point will be regenerated after resumption, so do not
 * write it out when emptying the buffer externally.
 */


/* a variation on this for NIFTY. */
METHODDEF(boolean)
empty_mem_output_buffer (j_compress_ptr cinfo)
{
  nif_dest_ptr dest = (nif_dest_ptr) cinfo->dest;
  openexif_jpeg_mem_client_ptr memInfo = (openexif_jpeg_mem_client_ptr)(cinfo->client_data) ;

  memInfo->bytes_in_buffer+=OUTPUT_BUF_SIZE;

  if(memInfo->bytes_in_buffer > (int)memInfo->size_buffer)
  {
      ERREXIT(cinfo, JERR_BUFFER_SIZE);
  }
  (void)memcpy(dest->NIFbuffer, dest->buffer, OUTPUT_BUF_SIZE);

    //*************
    // Thanks to Chuck Schneider for this bug fix:
    dest->NIFbuffer += OUTPUT_BUF_SIZE;
    //**************


  dest->pub.next_output_byte = dest->buffer;
  dest->pub.free_in_buffer = OUTPUT_BUF_SIZE;

  return TRUE;
}


/*
 * Terminate destination --- called by jpeg_finish_compress
 * after all data has been written.  Usually needs to flush buffer.
 *
 * NB: *not* called by jpeg_abort or jpeg_destroy; surrounding
 * application must deal with any cleanup that should happen even
 * for error exit.
 */

/* a variation of this for in memory work with NIFTY. */
METHODDEF(void)
term_mem_destination (j_compress_ptr cinfo)
{
  nif_dest_ptr dest = (nif_dest_ptr) cinfo->dest;
  openexif_jpeg_mem_client_ptr memInfo = (openexif_jpeg_mem_client_ptr)(cinfo->client_data) ;

  size_t datacount = OUTPUT_BUF_SIZE - dest->pub.free_in_buffer;

  memInfo->bytes_in_buffer+=datacount;
  if(memInfo->bytes_in_buffer > (int)memInfo->size_buffer)
  {
      ERREXIT(cinfo, JERR_BUFFER_SIZE);
  }

  (void)memcpy(dest->NIFbuffer, dest->buffer, datacount);

}

/*
 * Prepare for output to a stdio stream.
 * The caller must have already opened the stream, and is responsible
 * for closing it after finishing compression.
 */

GLOBAL(void)
openexif_jpeg_mem_dest(j_compress_ptr cinfo, JOCTET *UserBuffer)
{
  nif_dest_ptr dest;

  if (cinfo->dest == NULL) 
  {    /* first time for this JPEG object? */
    cinfo->dest = (struct openexif_jpeg_destination_mgr *)
      (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
                                  SIZEOF(nif_destination_mgr));
  }

  if( cinfo->client_data == NULL )
  {
      cinfo->client_data = (void *)
          (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
                                      SIZEOF(openexif_jpeg_mem_client));
  }
  

  dest = (nif_dest_ptr) cinfo->dest;
  dest->pub.init_destination = init_mem_destination;
  dest->pub.empty_output_buffer = empty_mem_output_buffer;
  dest->pub.term_destination = term_mem_destination;
  dest->NIFbuffer = UserBuffer;
  ((openexif_jpeg_mem_client_ptr)(cinfo->client_data))->bytes_in_buffer = 0;
  ((openexif_jpeg_mem_client_ptr)(cinfo->client_data))->size_buffer = 0;
}
}
#endif

#endif