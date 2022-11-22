/*
 * aconf2.h
 *
 * This gets included by aconf.h, and contains miscellaneous global
 * settings not directly controlled by autoconf.  This is a separate
 * file because otherwise the configure script will munge any
 * #define/#undef constructs.
 *
 * Copyright 2002-2003 Glyph & Cog, LLC
 */

#ifndef ACONF2_H
#define ACONF2_H

/*
 * This controls the use of the interface/implementation pragmas.
 */
#if defined(__GNUC__) && !defined(__clang__)
#define USE_GCC_PRAGMAS
#endif
/* There is a bug in the version of gcc which ships with MacOS X 10.2 */
#if defined(__APPLE__) && defined(__MACH__)
#  include <AvailabilityMacros.h>
#endif
#ifdef MAC_OS_X_VERSION_MAX_ALLOWED
#  if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_2
#    undef USE_GCC_PRAGMAS
#  endif
#endif

/*
 * Speed up Windows compilation.  This will only work for the command
 * line tools.
 */
/*
 *#ifdef _WIN32
 *#  define WIN32_LEAN_AND_MEAN
 *#endif
 */

/*
 * Use A4 paper size instead of Letter for PostScript output.
 */
//#define A4_PAPER

/*
 * Do not allow text selection.
 */
//#define NO_TEXT_SELECT

/*
 * Include support for OPI comments.
 */
//#define OPI_SUPPORT

/*
 * Enable multithreading support.
 */
//#define MULTITHREADED

/*
 * Enable C++ exceptions.
 */
//#define USE_EXCEPTIONS

/*
 * Use fixed point (instead of floating point) arithmetic.
 */
//#define USE_FIXEDPOINT

/*
 * Enable support for CMYK output.
 */
//#define SPLASH_CMYK

/*
 * Enable support for DeviceN output.
 */
//#define SPLASH_DEVICEN

/*
 * Enable support for highlighted regions.
 */
//#define HIGHLIGHTED_REGIONS

/*
 * Full path for the system-wide xpdfrc file.
 */
//@SYSTEM_XPDFRC_DEFINE@

/*
 * Directory to use for the ${DATADIR} variable in the xpdfrc config
 * file.
 */
//@XPDFRC_DATADIR_DEFINE@

/*
 * Various include files and functions.
 */
//#define HAVE_MKSTEMP
//#define HAVE_MKSTEMPS
//#define HAVE_POPEN
//#define HAVE_STD_SORT
//#define HAVE_FSEEKO
//#define HAVE_FSEEK64
//#define HAVE_FSEEKI64
#define _FILE_OFFSET_BITS 64
#define _LARGE_FILES 1
#define _LARGEFILE_SOURCE 1

/*
 * This is defined if using FreeType 2.
 */
//#define HAVE_FREETYPE_H

/*
 * This is defined if using D-Type 4.
 */
//#define HAVE_DTYPE4_H

/*
 * This is defined if using libpaper.
 */
//#define HAVE_PAPER_H

/*
 * This is defined if using libfontconfig.
 */
//#define HAVE_FONTCONFIG

/*
 * Defined if the Splash library is avaiable.
 */
//#define HAVE_SPLASH

/*
 * Defined if using lcms2.
 */
//#define HAVE_LCMS

/*
 * Defined for evaluation mode.
 */
//#define EVAL_MODE

/*
 * Defined when building the closed source XpdfReader binary.
 */
//#define BUILDING_XPDFREADER

#endif
