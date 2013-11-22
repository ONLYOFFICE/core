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
 * Chris Lin <ti.lin@kodak.com> 
 * Sam Fryer <samuel.fryer@kodak.com>
 */ 


#ifndef _EXIF_CONF_H_
#define	_EXIF_CONF_H_
/*
 * Library Configuration Definitions.
 *
 * This file defines the default configuration for the library.
 * If the target system does not have make or a way to specify
 * #defines on the command line, this file can be edited to
 * configure the library.
 */


/*
 * UNIX systems should run the configure script to generate
 * a TiffPort.h file that reflects the system capabilities.
 * Doing this obviates all the dreck done in TiffComp.h.
 */
#include "ExifComp.h"

// HERE TEMPORARILY TO ALLOW UNIX BUILDS WITHOUT USING
// autoconf stuff
#if defined(unix) || defined(__unix)
#include <fcntl.h>
#include <unistd.h>
#endif

#include <cstdio>
#ifndef macintosh
#include <sys/types.h>
#endif

#if defined( HAVE_FCNTL_H )
#include <fcntl.h>
#endif

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#endif

/* 
 * Later versions of GCC need this
 */
#ifdef __GNUC__
#if ( __GNUC__ > 3 )
#include <string.h>
#include <limits.h>
#endif
#endif

/*
 * General portability-related defines:
 *
 * HOST_FILLORDER	native cpu bit order: one of FILLORDER_MSB2LSB
 *			or FILLODER_LSB2MSB; this is typically set by the
 *			configure script
 * HOST_BIGENDIAN	native cpu byte order: 1 if big-endian (Motorola)
 *			or 0 if little-endian (Intel); this may be used
 *			in codecs to optimize code
 */
#if defined( HAVE_FILLORDER_LSB2MSB )
#define	HOST_FILLORDER	FILLORDER_LSB2MSB
#else
#define	HOST_FILLORDER	FILLORDER_MSB2LSB
#endif

#if defined( WORDS_BIGENDIAN )
#define	HOST_BIGENDIAN 1
#elif defined (macintosh)
#define HOST_BIGENDIAN 1
#else
#define	HOST_BIGENDIAN 0
#endif

#ifndef FEATURE_SUPPORT
/*
 * Feature support definitions:
 *
 *    COLORIMETRY_SUPPORT enable support for 6.0 colorimetry tags
 *    YCBCR_SUPPORT	enable support for 6.0 YCbCr tags
 *    CMYK_SUPPORT	enable support for 6.0 CMYK tags
 */
#define	COLORIMETRY_SUPPORT
#define	YCBCR_SUPPORT
#define	CMYK_SUPPORT
#endif /* FEATURE_SUPPORT */

#ifndef COMPRESSION_SUPPORT
/*
 * Compression support defines:
 *
 *    CCITT_SUPPORT	enable support for CCITT Group 3 & 4 algorithms
 *    PACKBITS_SUPPORT	enable support for Macintosh PackBits algorithm
 *    LZW_SUPPORT	enable support for LZW algorithm
 *    THUNDER_SUPPORT	enable support for ThunderScan 4-bit RLE algorithm
 *    NEXT_SUPPORT	enable support for NeXT 2-bit RLE algorithm
 *    OJPEG_SUPPORT	enable support for 6.0-style JPEG DCT algorithms
 *			(no builtin support, only a codec hook)
 *    JPEG_SUPPORT	enable support for post-6.0-style JPEG DCT algorithms
 *			(requires freely available IJG software, see tif_jpeg.c)
 *    ZIP_SUPPORT	enable support for Deflate algorithm
 *			(requires freely available zlib software, see tif_zip.c)
 *    PIXARLOG_SUPPORT	enable support for Pixar log-format algorithm
 */
#define	CCITT_SUPPORT
#define	PACKBITS_SUPPORT
#define	LZW_SUPPORT
#define	THUNDER_SUPPORT
#define	NEXT_SUPPORT
#endif /* COMPRESSION_SUPPORT */

/*
 * If JPEG compression is enabled then we must also include
 * support for the colorimetry and YCbCr-related tags.
 */
#ifdef JPEG_SUPPORT
#ifndef YCBCR_SUPPORT
#define	YCBCR_SUPPORT
#endif
#ifndef COLORIMETRY_SUPPORT
#define	COLORIMETRY_SUPPORT
#endif
#endif /* JPEG_SUPPORT */

/*
 * ``Orthogonal Features''
 *
 * STRIPCHOP_SUPPORT	automatically convert single-strip uncompressed images
 *			to mutiple strips of ~8Kb (for reducing memory use)
 * SUBIFD_SUPPORT	enable support for SubIFD tag (thumbnails and such)
 */
#ifndef STRIPCHOP_SUPPORT
#define	STRIPCHOP_SUPPORT	1	/* enable strip chopping */
#endif
#ifndef SUBIFD_SUPPORT
#define	SUBIFD_SUPPORT		1	/* enable SubIFD tag (330) support */
#endif


//: 
// OPENEXIF_USING_STD
//
//    For compilers which recognize namespaces, this macro translates to
//    the following:
//
//        namespace std { }; using namespace std;
//
//    Otherwise the macro is defined as the empty string.

#ifdef __mips
#define OPENEXIF_USING_STD
#endif

#ifdef __SUNPRO_CC
#define OPENEXIF_USING_STD
#endif

#ifdef __GNUC__
#ifdef EK_USE_NAMESPACE
#define OPENEXIF_USING_STD namespace std { }; using namespace std;
#else
#define OPENEXIF_USING_STD
#endif
#endif

#ifdef _WIN32_WCE
#define OPENEXIF_USING_STD
#endif

#ifdef _MSC_VER
#if (_MSC_VER < 1100)
#define OPENEXIF_USING_STD
#else
#define OPENEXIF_USING_STD namespace std { }; using namespace std;
#endif
#endif

#ifndef OPENEXIF_USING_STD
#define OPENEXIF_USING_STD namespace std { }; using namespace std;
#endif  // default

OPENEXIF_USING_STD

//:
//  EXIF_TYPENAME
//
//      Some compilers do not support the "typename" keyword correctly, and
//      so they don't require/accept it where they should.  Other compilers do.
//      For now, only GCC is complaining, but others can be added as needed.
//

#if (defined __GNUC__) || ((defined _MSC_VER) && (_MSC_VER > 1300))
#define EXIF_TYPENAME typename
#endif

// default
#ifndef EXIF_TYPENAME
#define EXIF_TYPENAME
#endif

#ifdef _MSC_VER
/* Windows platforms */
#if    (defined EXIF_BUILD_DLL)

#define EXIFEK_DECL __declspec(dllexport)
#define EXIFEK_EXPLICIT
#elif  (defined EXIF_DLL)
#define EXIFEK_DECL __declspec(dllimport)
#define EXIFEK_EXPLICIT
#define EXIFEK_TMPL_EXT extern
#endif

#pragma warning (disable:4251)
#pragma warning (disable:4661)
#pragma warning (disable:4786)

#endif

#ifndef EXIFEK_DECL
#define EXIFEK_DECL
#endif

#ifndef EXIFEK_TMPL_EXT
#define EXIFEK_TMPL_EXT
#endif



#endif /* _EXIF_CONF_H_ */
