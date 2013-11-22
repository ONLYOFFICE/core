
/*
 * ExifIJGWriteFrame.h
 *
 * The contents of this file are subject to the IBM Common Public
 * License Version 0.5 (the "License"); you may not use this file 
 * except in compliance with the License. You may obtain a copy of 
 * the License at http://oss.software.ibm.com/developerworks/
 * opensource/license-cpl.html
 * 
 * The Initial Developer of the Original Code is Eastman 
 * Kodak Company. Portions created by Kodak are 
 * Copyright (C) 2004 Eastman Kodak Company. All 
 * Rights Reserved. 
 *
 * Creation Date: 12/04/2004
 *
 * Original Author: 
 * Sam Fryer samuel.fryer@kodak.com
 *
 * Contributor(s): 
 *
 *
 */

#ifdef OPENEXIF_DCF_COMPLIANT

#ifndef EXIFIJGWRITEFRAME_H
#define EXIFIJGWRITEFRAME_H
#ifndef OPENEXIF_NO_IJG
namespace Exif 
{
extern void (*openexif_orig_write_frame_header) (j_compress_ptr cinfo);
void openexif_write_frame_header (j_compress_ptr cinfo);

extern void (*openexif_orig_write_scan_header) (j_compress_ptr cinfo);
void openexif_write_scan_header (j_compress_ptr cinfo);
}
#endif

#endif

#endif
