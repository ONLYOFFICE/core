/* Copyright (C) 2001-2006 Artifex Software, Inc.
   All Rights Reserved.
  
   This software is provided AS-IS with no warranty, either express or
   implied.

   This software is distributed under license and may not be copied, modified
   or distributed except as expressly authorized under the terms of that
   license.  Refer to licensing information at http://www.artifex.com/
   or contact Artifex Software, Inc.,  7 Mt. Lassen Drive - Suite A-134,
   San Rafael, CA  94903, U.S.A., +1(415)492-9861, for further information.
*/

/* $Id: dos_.h 8022 2007-06-05 22:23:38Z giles $ */
/* Generic MS-DOS interface */

#ifndef dos__INCLUDED
#  define dos__INCLUDED

/* This file is needed because the various DOS compilers */
/* provide slightly different procedures for interfacing to DOS and */
/* the I/O hardware, and because the Watcom compiler is 32-bit. */
#include <dos.h>
#if defined(__WATCOMC__) || defined(_MSC_VER)

/* ---------------- Microsoft C/C++, all models; */
/* ---------------- Watcom compiler, 32-bit flat model. */
/* ---------------- inp/outp prototypes are in conio.h, not dos.h. */

#  include <conio.h>
#  define inport(px) inpw(px)
#  define inportb(px) inp(px)
#  define outport(px,w) outpw(px,w)
#  define outportb(px,b) outp(px,b)
#  define enable() _enable()
#  define disable() _disable()
#  define PTR_OFF(ptr) ((ushort)(uint)(ptr))
/* Define the structure and procedures for file enumeration. */
#define ff_name name
#define dos_findfirst(n,b) _dos_findfirst(n, _A_NORMAL | _A_RDONLY, b)
#define dos_findnext(b) _dos_findnext(b)

/* Define things that differ between Watcom and Microsoft. */
#  ifdef __WATCOMC__
#    define MK_PTR(seg,off) (((seg) << 4) + (off))
#    define int86 int386
#    define int86x int386x
#    define rshort w
#    define ff_struct_t struct find_t
#  else
#    define MK_PTR(seg,off) (((ulong)(seg) << 16) + (off))
#    define cputs _cputs
#    define fdopen _fdopen
#    define O_BINARY _O_BINARY
#    define REGS _REGS
#    define rshort x
#    define ff_struct_t struct _find_t
#    define stdprn _stdprn
#  endif

#else /* not Watcom or Microsoft */

/* ---------------- Borland compiler, 16:16 pseudo-segmented model. */
/* ---------------- ffblk is in dir.h, not dos.h. */
#include <dir.h>
#  define MK_PTR(seg,off) MK_FP(seg,off)
#  define PTR_OFF(ptr) FP_OFF(ptr)
/* Define the regs union tag for short registers. */
#  define rshort x
/* Define the structure and procedures for file enumeration. */
#define ff_struct_t struct ffblk
#define dos_findfirst(n,b) findfirst(n, b, 0)
#define dos_findnext(b) findnext(b)

#endif

#endif /* dos__INCLUDED */
