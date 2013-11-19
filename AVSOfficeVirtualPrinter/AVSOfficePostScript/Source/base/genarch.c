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

/* $Id: genarch.c 8488 2008-01-17 13:37:19Z leonardo $ */
/*
 * Generate a header file (arch.h) with parameters
 * reflecting the machine architecture and compiler characteristics.
 */

#include "stdpre.h"
#include <ctype.h>
#include <stdio.h>
/*
 * In theory, not all systems provide <string.h> or declare memset 
 * there, but at this point we don't think we care about any that don't.
 */
#include <string.h>
#include <time.h>

/* We provide a _SIZEOF_ macro for GX_COLOR_INDEX_TYPE
   fallback to a generic int if no such type is defined.
   This default must be kept in sync with the one in gxcindex.h
   or ARCH_SIZEOF_GX_COLOR_INDEX will be incorrect for such builds. */
#ifndef GX_COLOR_INDEX_TYPE
#define GX_COLOR_INDEX_TYPE ulong
#endif

/* We should write the result on stdout, but the original Turbo C 'make' */
/* can't handle output redirection (sigh). */

static void
section(FILE * f, const char *str)
{
    fprintf(f, "\n\t /* ---------------- %s ---------------- */\n\n", str);
}

static void
define(FILE *f, const char *str)
{
    fprintf(f, "#define %s ", str);
}

static void
define_int(FILE *f, const char *str, int value)
{
    fprintf(f, "#define %s %d\n", str, value);
}

static void
print_ffs(FILE *f, int nbytes)
{
    int i;

    for (i = 0; i < nbytes; ++i)
	fprintf(f, "ff");
}

static int
ilog2(int n)
{
    int i = 0, m = n;

    while (m > 1)
	++i, m = (m + 1) >> 1;
    return i;
}

int
main(int argc, char *argv[])
{
    char *fname = argv[1];
    long one = 1;
    struct {
	char c;
	short s;
    } ss;
    struct {
	char c;
	int i;
    } si;
    struct {
	char c;
	long l;
    } sl;
    struct {
	char c;
	char *p;
    } sp;
    struct {
	char c;
	float f;
    } sf;
    struct {
	char c;
	double d;
    } sd;
    long lm1 = -1;
    long lr1 = lm1 >> 1, lr2 = lm1 >> 2;
    int im1 = -1;
    int ir1 = im1 >> 1, ir2 = im1 >> 2;
    union {
	long l;
	char *p;
    } pl0, pl1;
    int ars;
    union {
	float f;
	int i;
	long l;
    } f0, f1, fm1;
    int floats_are_IEEE;
    FILE *f = fopen(fname, "w");

    if (f == NULL) {
	fprintf(stderr, "genarch.c: can't open %s for writing\n", fname);
	return exit_FAILED;
    }
    fprintf(f, "/* Parameters derived from machine and compiler architecture. */\n");
    fprintf(f, "/* This file is generated mechanically by genarch.c. */\n");

    /* We have to test the size dynamically here, */
    /* because the preprocessor can't evaluate sizeof. */
    f0.f = 0.0, f1.f = 1.0, fm1.f = -1.0;
    floats_are_IEEE =
	(size_of(float) == size_of(int) ?
	 f0.i == 0 && f1.i == (int)0x3f800000 && fm1.i == (int)0xbf800000 :
	 f0.l == 0 && f1.l == 0x3f800000L && fm1.l == 0xbf800000L);

    section(f, "Scalar alignments");

#define OFFSET_IN(s, e) (int)((char *)&s.e - (char *)&s)
    define_int(f, "ARCH_ALIGN_SHORT_MOD", OFFSET_IN(ss, s));
    define_int(f, "ARCH_ALIGN_INT_MOD", OFFSET_IN(si, i));
    define_int(f, "ARCH_ALIGN_LONG_MOD", OFFSET_IN(sl, l));
    define_int(f, "ARCH_ALIGN_PTR_MOD", OFFSET_IN(sp, p));
    define_int(f, "ARCH_ALIGN_FLOAT_MOD", OFFSET_IN(sf, f));
    define_int(f, "ARCH_ALIGN_DOUBLE_MOD", OFFSET_IN(sd, d));
#undef OFFSET_IN

    /* Some architectures have special alignment requirements for   */
    /* jmp_buf, and we used to provide ALIGN_STRUCT_MOD for that.   */
    /* We've now dropped that in favor of aligning jmp_buf by hand. */
    /* See setjmp_.h for the implementation of this.                */

    section(f, "Scalar sizes");

    define_int(f, "ARCH_LOG2_SIZEOF_CHAR", ilog2(size_of(char)));
    define_int(f, "ARCH_LOG2_SIZEOF_SHORT", ilog2(size_of(short)));
    define_int(f, "ARCH_LOG2_SIZEOF_INT", ilog2(size_of(int)));
    define_int(f, "ARCH_LOG2_SIZEOF_LONG", ilog2(size_of(long)));
#if !defined(_MSC_VER) && ! (defined(__BORLANDC__) && defined(__WIN32__))
    /* MSVC does not provide 'long long' but we need this on some archs
       to define a 64 bit type. A corresponding #ifdef in stdint_.h handles
       that case for MSVC. Most other platforms do support long long if
       they have a 64 bit type at all */
    define_int(f, "ARCH_LOG2_SIZEOF_LONG_LONG", ilog2(size_of(long long)));
#endif
    define_int(f, "ARCH_SIZEOF_GX_COLOR_INDEX", sizeof(GX_COLOR_INDEX_TYPE));
    define_int(f, "ARCH_SIZEOF_PTR", size_of(char *));
    define_int(f, "ARCH_SIZEOF_FLOAT", size_of(float));
    define_int(f, "ARCH_SIZEOF_DOUBLE", size_of(double));
    if (floats_are_IEEE) {
	define_int(f, "ARCH_FLOAT_MANTISSA_BITS", 24);
	define_int(f, "ARCH_DOUBLE_MANTISSA_BITS", 53);
    } else {
	/*
	 * There isn't any general way to compute the number of mantissa
	 * bits accurately, especially if the machine uses hex rather
	 * than binary exponents.  Use conservative values, assuming
	 * the exponent is stored in a 16-bit word of its own.
	 */
	define_int(f, "ARCH_FLOAT_MANTISSA_BITS", sizeof(float) * 8 - 17);
	define_int(f, "ARCH_DOUBLE_MANTISSA_BITS", sizeof(double) * 8 - 17);
    }

    section(f, "Unsigned max values");

    /*
     * We can't use fprintf with a numeric value for PRINT_MAX, because
     * too many compilers produce warnings or do the wrong thing for
     * complementing or widening unsigned types.
     */
#define PRINT_MAX(str, typ, tstr, l)\
  BEGIN\
    define(f, str);\
    fprintf(f, "((%s)0x", tstr);\
    print_ffs(f, sizeof(typ));\
    fprintf(f, "%s + (%s)0)\n", l, tstr);\
  END
    PRINT_MAX("ARCH_MAX_UCHAR", unsigned char, "unsigned char", "");
    PRINT_MAX("ARCH_MAX_USHORT", unsigned short, "unsigned short", "");
    /*
     * For uint and ulong, a different approach is required to keep gcc
     * with -Wtraditional from spewing out pointless warnings.
     */
    define(f, "ARCH_MAX_UINT");
    fprintf(f, "((unsigned int)~0 + (unsigned int)0)\n");
    define(f, "ARCH_MAX_ULONG");
    fprintf(f, "((unsigned long)~0L + (unsigned long)0)\n");
#undef PRINT_MAX

    section(f, "Miscellaneous");

    define_int(f, "ARCH_IS_BIG_ENDIAN", 1 - *(char *)&one);
    pl0.l = 0;
    pl1.l = -1;
    define_int(f, "ARCH_PTRS_ARE_SIGNED", (pl1.p < pl0.p));
    define_int(f, "ARCH_FLOATS_ARE_IEEE", (floats_are_IEEE ? 1 : 0));

    /*
     * There are three cases for arithmetic right shift:
     * always correct, correct except for right-shifting a long by 1
     * (a bug in some versions of the Turbo C compiler), and
     * never correct.
     */
    ars = (lr2 != -1 || ir1 != -1 || ir2 != -1 ? 0 :
	   lr1 != -1 ? 1 :	/* Turbo C problem */
	   2);
    define_int(f, "ARCH_ARITH_RSHIFT", ars);
    /*
     * Determine whether dividing a negative integer by a positive one
     * takes the floor or truncates toward zero.
     */
    define_int(f, "ARCH_DIV_NEG_POS_TRUNCATES", im1 / 2 == 0);

/* ---------------- Done. ---------------- */

    fclose(f);
    return exit_OK;
}
