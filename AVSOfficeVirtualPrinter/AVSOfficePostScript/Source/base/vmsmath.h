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

/* $Id: vmsmath.h 8022 2007-06-05 22:23:38Z giles $ */
/* Substitute for math.h on VAX/VMS systems */

#ifndef vmsmath_INCLUDED
#  define vmsmath_INCLUDED

/* DEC VAX/VMS C comes with a math.h file but GNU VAX/VMS C does not. */
/* This file substitutes for math.h when using GNU C. */
#  ifndef __MATH
#    define __MATH
#    if CC$gfloat
#      define HUGE_VAL 8.988465674311578540726371186585e+307
#    else
#      define HUGE_VAL 1.70141183460469229e+38
#    endif
extern double acos(), asin(), atan(), atan2();
extern double sin(), tan(), cos();
extern double cosh(), sinh(), tanh();
extern double exp(), frexp(), ldexp(), log(), log10(), pow();
extern double modf(), fmod(), sqrt(), ceil(), floor();
extern double fabs(), cabs(), hypot();

#  endif

#endif /* vmsmath_INCLUDED */
