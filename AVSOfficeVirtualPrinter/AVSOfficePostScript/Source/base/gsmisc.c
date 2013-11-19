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

/* $Id: gsmisc.c 8477 2008-01-07 18:43:02Z henrys $ */
/* Miscellaneous utilities for Ghostscript library */


/****************************************************************************
 * This global should be put into either gs_imager_state or gs_device,
 * but the "plumbing" retrofit is more than we need for things that were
 * previously compile time modifications. CPSI_mode will at least make these
 * dynamic (but NOT thread safe)!!!
 ****************************************************************************
 */
int CPSI_mode = 0; /* false */	/* default GS behavior is 'false' */

/*
 * In order to capture the original definition of sqrt, which might be
 * either a procedure or a macro and might not have an ANSI-compliant
 * prototype (!), we need to do the following:
 */
#include "std.h"
#if defined(VMS) && defined(__GNUC__)
/*  DEC VAX/VMS C comes with a math.h file, but GNU VAX/VMS C does not. */
#  include "vmsmath.h"
#else
#  include <math.h>
#endif
static inline double
orig_sqrt(double x)
{
    return sqrt(x);
}

/* Here is the real #include section. */
#include "ctype_.h"
#include "malloc_.h"
#include "math_.h"
#include "memory_.h"
#include "string_.h"
#include "gx.h"
#include "gpcheck.h"		/* for gs_return_check_interrupt */
#include "gserror.h"		/* for prototype */
#include "gserrors.h"
#include "gxfarith.h"
#include "gxfixed.h"
#include "stdint_.h"

/* ------ Redirected stdout and stderr  ------ */

#include <stdarg.h>
#define PRINTF_BUF_LENGTH 1024

int outprintf(const gs_memory_t *mem, const char *fmt, ...)
{
    int count;
    char buf[PRINTF_BUF_LENGTH];
    va_list args;

    va_start(args, fmt);

    count = vsprintf(buf, fmt, args);
    outwrite(mem, buf, count);
    if (count >= PRINTF_BUF_LENGTH) {
	count = sprintf(buf, 
	    "PANIC: printf exceeded %d bytes.  Stack has been corrupted.\n", 
	    PRINTF_BUF_LENGTH);
	outwrite(mem, buf, count);
    }
    va_end(args);
    return count;
}

int errprintf(const char *fmt, ...)
{
    int count;
    char buf[PRINTF_BUF_LENGTH];
    va_list args;

    va_start(args, fmt);

    count = vsprintf(buf, fmt, args);
    errwrite(buf, count);
    if (count >= PRINTF_BUF_LENGTH) {
	count = sprintf(buf, 
	    "PANIC: printf exceeded %d bytes.  Stack has been corrupted.\n", 
	    PRINTF_BUF_LENGTH);
	errwrite(buf, count);
    }
    va_end(args);
    return count;
}

/* ------ Debugging ------ */

/* Ghostscript writes debugging output to gs_debug_out. */
/* We define gs_debug and gs_debug_out even if DEBUG isn't defined, */
/* so that we can compile individual modules with DEBUG set. */
char gs_debug[128];
FILE *gs_debug_out;

/* Test whether a given debugging option is selected. */
/* Upper-case letters automatically include their lower-case counterpart. */
bool
gs_debug_c(int c)
{
    return
	(c >= 'a' && c <= 'z' ? gs_debug[c] | gs_debug[c ^ 32] : gs_debug[c]);
}

/* Define the formats for debugging printout. */
const char *const dprintf_file_and_line_format = "%10s(%4d): ";
const char *const dprintf_file_only_format = "%10s(unkn): ";

/*
 * Define the trace printout procedures.  We always include these, in case
 * other modules were compiled with DEBUG set.  Note that they must use
 * out/errprintf, not fprintf nor fput[cs], because of the way that 
 * stdout/stderr are implemented on DLL/shared library builds.
 */
void
dflush(void)
{
    errflush();
}
static const char *
dprintf_file_tail(const char *file)
{
    const char *tail = file + strlen(file);

    while (tail > file &&
	   (isalnum((unsigned char)tail[-1]) || tail[-1] == '.' || tail[-1] == '_')
	)
	--tail;
    return tail;
}
#if __LINE__			/* compiler provides it */
void
dprintf_file_and_line(const char *file, int line)
{
    if (gs_debug['/'])
	dpf(dprintf_file_and_line_format,
		dprintf_file_tail(file), line);
}
#else
void
dprintf_file_only(const char *file)
{
    if (gs_debug['/'])
	dpf(dprintf_file_only_format, dprintf_file_tail(file));
}
#endif
void
printf_program_ident(const gs_memory_t *mem, const char *program_name, long revision_number)
{
    if (program_name)
        outprintf(mem, (revision_number ? "%s " : "%s"), program_name);
    if (revision_number) {
	int fpart = revision_number % 100;

	outprintf(mem, "%d.%02d", (int)(revision_number / 100), fpart);
    }
}
void
eprintf_program_ident(const char *program_name,
		      long revision_number)
{
    if (program_name) {
	epf((revision_number ? "%s " : "%s"), program_name);
	if (revision_number) {
	    int fpart = revision_number % 100;

	    epf("%d.%02d", (int)(revision_number / 100), fpart);
	}
	epf(": ");
    }
}
#if __LINE__			/* compiler provides it */
void
lprintf_file_and_line(const char *file, int line)
{
    epf("%s(%d): ", file, line);
}
#else
void
lprintf_file_only(FILE * f, const char *file)
{
    epf("%s(?): ", file);
}
#endif

/* Log an error return.  We always include this, in case other */
/* modules were compiled with DEBUG set. */
#undef gs_log_error		/* in case DEBUG isn't set */
int
gs_log_error(int err, const char *file, int line)
{
    if (gs_log_errors) {
	if (file == NULL)
	    dprintf1("Returning error %d.\n", err);
	else
	    dprintf3("%s(%d): Returning error %d.\n",
		     (const char *)file, line, err);
    }
    return err;
}

/* Check for interrupts before a return. */
int
gs_return_check_interrupt(const gs_memory_t *mem, int code)
{
    if (code < 0)
	return code;
    {
	int icode = gp_check_interrupts(mem);

	return (icode == 0 ? code :
		gs_note_error((icode > 0 ? gs_error_interrupt : icode)));
    }
}


int gs_throw_imp(const char *func, const char *file, int line, int op, int code, const char *fmt, ...)
{
    char msg[1024];
    va_list ap;

    va_start(ap, fmt);
    vsprintf(msg, fmt, ap);
    msg[sizeof(msg) - 1] = 0;
    va_end(ap);

    if (!gs_debug_c('#')) {
	; /* NB: gs_log_errors  
	   * we could disable these printfs, and probably will when, 
	   * the code becomes more stable: 
	   * return code;
	   */
    }
       

    /* throw */
    if (op == 0)
	errprintf("+ %s:%d: %s(): %s\n", file, line, func, msg);

    /* rethrow */
    if (op == 1)
	errprintf("| %s:%d: %s(): %s\n", file, line, func, msg);

    /* catch */
    if (op == 2)
	errprintf("- %s:%d: %s(): %s\n", file, line, func, msg);

    /* warn */
    if (op == 3)
	errprintf("  %s:%d: %s(): %s\n", file, line, func, msg);

    return code;
}

const char *gs_errstr(int code)
{
    switch (code) {
    default:
    case gs_error_unknownerror: return "unknownerror";
    case gs_error_interrupt: return "interrupt";
    case gs_error_invalidaccess: return "invalidaccess";
    case gs_error_invalidfileaccess: return "invalidfileaccess";
    case gs_error_invalidfont: return "invalidfont";
    case gs_error_ioerror: return "ioerror";
    case gs_error_limitcheck: return "limitcheck";
    case gs_error_nocurrentpoint: return "nocurrentpoint";
    case gs_error_rangecheck: return "rangecheck";
    case gs_error_typecheck: return "typecheck";
    case gs_error_undefined: return "undefined";
    case gs_error_undefinedfilename: return "undefinedfilename";
    case gs_error_undefinedresult: return "undefinedresult";
    case gs_error_VMerror: return "vmerror";
    case gs_error_unregistered: return "unregistered";
    case gs_error_hit_detected: return "hit_detected";
    case gs_error_Fatal: return "Fatal";
    }
}


/* ------ Substitutes for missing C library functions ------ */

#ifdef MEMORY__NEED_MEMMOVE	/* see memory_.h */
/* Copy bytes like memcpy, guaranteed to handle overlap correctly. */
/* ANSI C defines the returned value as being the src argument, */
/* but with the const restriction removed! */
void *
gs_memmove(void *dest, const void *src, size_t len)
{
    if (!len)
	return (void *)src;
#define bdest ((byte *)dest)
#define bsrc ((const byte *)src)
    /* We use len-1 for comparisons because adding len */
    /* might produce an offset overflow on segmented systems. */
    if (PTR_LE(bdest, bsrc)) {
	register byte *end = bdest + (len - 1);

	if (PTR_LE(bsrc, end)) {
	    /* Source overlaps destination from above. */
	    register const byte *from = bsrc;
	    register byte *to = bdest;

	    for (;;) {
		*to = *from;
		if (to >= end)	/* faster than = */
		    return (void *)src;
		to++;
		from++;
	    }
	}
    } else {
	register const byte *from = bsrc + (len - 1);

	if (PTR_LE(bdest, from)) {
	    /* Source overlaps destination from below. */
	    register const byte *end = bsrc;
	    register byte *to = bdest + (len - 1);

	    for (;;) {
		*to = *from;
		if (from <= end)	/* faster than = */
		    return (void *)src;
		to--;
		from--;
	    }
	}
    }
#undef bdest
#undef bsrc
    /* No overlap, it's safe to use memcpy. */
    memcpy(dest, src, len);
    return (void *)src;
}
#endif

#ifdef MEMORY__NEED_MEMCPY	/* see memory_.h */
void *
gs_memcpy(void *dest, const void *src, size_t len)
{
    if (len > 0) {
#define bdest ((byte *)dest)
#define bsrc ((const byte *)src)
	/* We can optimize this much better later on. */
	register byte *end = bdest + (len - 1);
	register const byte *from = bsrc;
	register byte *to = bdest;

	for (;;) {
	    *to = *from;
	    if (to >= end)	/* faster than = */
		break;
	    to++;
	    from++;
	}
    }
#undef bdest
#undef bsrc
    return (void *)src;
}
#endif

#ifdef MEMORY__NEED_MEMCHR	/* see memory_.h */
/* ch should obviously be char rather than int, */
/* but the ANSI standard declaration uses int. */
void *
gs_memchr(const void *ptr, int ch, size_t len)
{
    if (len > 0) {
	register const char *p = ptr;
	register uint count = len;

	do {
	    if (*p == (char)ch)
		return (void *)p;
	    p++;
	} while (--count);
    }
    return 0;
}
#endif

#ifdef MEMORY__NEED_MEMSET	/* see memory_.h */
/* ch should obviously be char rather than int, */
/* but the ANSI standard declaration uses int. */
void *
gs_memset(void *dest, register int ch, size_t len)
{
    /*
     * This procedure is used a lot to fill large regions of images,
     * so we take some trouble to optimize it.
     */
    register char *p = dest;
    register size_t count = len;

    ch &= 255;
    if (len >= sizeof(long) * 3) {
	long wd = (ch << 24) | (ch << 16) | (ch << 8) | ch;

	while (ALIGNMENT_MOD(p, sizeof(long)))
	    *p++ = (char)ch, --count;
	for (; count >= sizeof(long) * 4;
	     p += sizeof(long) * 4, count -= sizeof(long) * 4
	     )
	    ((long *)p)[3] = ((long *)p)[2] = ((long *)p)[1] =
		((long *)p)[0] = wd;
	switch (count >> ARCH_LOG2_SIZEOF_LONG) {
	case 3:
	    *((long *)p) = wd; p += sizeof(long);
	case 2:
	    *((long *)p) = wd; p += sizeof(long);
	case 1:
	    *((long *)p) = wd; p += sizeof(long);
	    count &= sizeof(long) - 1;
	case 0:
	default:		/* can't happen */
	    DO_NOTHING;
	}
    }
    /* Do any leftover bytes. */
    for (; count > 0; --count)
	*p++ = (char)ch;
    return dest;
}
#endif

#ifdef malloc__need_realloc	/* see malloc_.h */
/* Some systems have non-working implementations of realloc. */
void *
gs_realloc(void *old_ptr, size_t old_size, size_t new_size)
{
    void *new_ptr;

    if (new_size) {
	new_ptr = malloc(new_size);
	if (new_ptr == NULL)
	    return NULL;
    } else
	new_ptr = NULL;
    /* We have to pass in the old size, since we have no way to */
    /* determine it otherwise. */
    if (old_ptr != NULL) {
	if (new_ptr != NULL)
	    memcpy(new_ptr, old_ptr, min(old_size, new_size));
	free(old_ptr);
    }
    return new_ptr;
}
#endif

/* ------ Debugging support ------ */

/* Dump a region of memory. */
void
debug_dump_bytes(const byte * from, const byte * to, const char *msg)
{
    const byte *p = from;

    if (from < to && msg)
	dprintf1("%s:\n", msg);
    while (p != to) {
	const byte *q = min(p + 16, to);

	dprintf1("0x%lx:", (ulong) p);
	while (p != q)
	    dprintf1(" %02x", *p++);
	dputc('\n');
    }
}

/* Dump a bitmap. */
void
debug_dump_bitmap(const byte * bits, uint raster, uint height, const char *msg)
{
    uint y;
    const byte *data = bits;

    for (y = 0; y < height; ++y, data += raster)
	debug_dump_bytes(data, data + raster, (y == 0 ? msg : NULL));
}

/* Print a string. */
void
debug_print_string(const byte * chrs, uint len)
{
    uint i;

    for (i = 0; i < len; i++)
	dputc(chrs[i]);
    dflush();
}

/* Print a string in hexdump format. */
void
debug_print_string_hex(const byte * chrs, uint len)
{
    uint i;

    for (i = 0; i < len; i++)
        dprintf1("%02x", chrs[i]);
    dflush();
}

/*
 * The following code prints a hex stack backtrace on Linux/Intel systems.
 * It is here to be patched into places where we need to print such a trace
 * because of gdb's inability to put breakpoints in dynamically created
 * threads.
 *
 * first_arg is the first argument of the procedure into which this code
 * is patched.
 */
#define BACKTRACE(first_arg)\
  BEGIN\
    ulong *fp_ = (ulong *)&first_arg - 2;\
    for (; fp_ && (fp_[1] & 0xff000000) == 0x08000000; fp_ = (ulong *)*fp_)\
	dprintf2("  fp=0x%lx ip=0x%lx\n", (ulong)fp_, fp_[1]);\
  END

/* ------ Arithmetic ------ */

/* Compute M modulo N.  Requires N > 0; guarantees 0 <= imod(M,N) < N, */
/* regardless of the whims of the % operator for negative operands. */
int
imod(int m, int n)
{
    if (n <= 0)
	return 0;		/* sanity check */
    if (m >= 0)
	return m % n;
    {
	int r = -m % n;

	return (r == 0 ? 0 : n - r);
    }
}

/* Compute the GCD of two integers. */
int
igcd(int x, int y)
{
    int c = x, d = y;

    if (c < 0)
	c = -c;
    if (d < 0)
	d = -d;
    while (c != 0 && d != 0)
	if (c > d)
	    c %= d;
	else
	    d %= c;
    return d + c;		/* at most one is non-zero */
}

/* Compute X such that A*X = B mod M.  See gxarith.h for details. */
int
idivmod(int a, int b, int m)
{
    /*
     * Use the approach indicated in Knuth vol. 2, section 4.5.2, Algorithm
     * X (p. 302) and exercise 15 (p. 315, solution p. 523).
     */
    int u1 = 0, u3 = m;
    int v1 = 1, v3 = a;
    /*
     * The following loop will terminate with a * u1 = gcd(a, m) mod m.
     * Then x = u1 * b / gcd(a, m) mod m.  Since we require that
     * gcd(a, m) | gcd(a, b), it follows that gcd(a, m) | b, so the
     * division is exact.
     */
    while (v3) {
	int q = u3 / v3, t;

	t = u1 - v1 * q, u1 = v1, v1 = t;
	t = u3 - v3 * q, u3 = v3, v3 = t;
    }
    return imod(u1 * b / igcd(a, m), m);
}

/* Compute floor(log2(N)).  Requires N > 0. */
int
ilog2(int n)
{
    int m = n, l = 0;

    while (m >= 16)
	m >>= 4, l += 4;
    return
	(m <= 1 ? l :
	 "\000\000\001\001\002\002\002\002\003\003\003\003\003\003\003\003"[m] + l);
}

/*
 * Compute A * B / C when 0 <= B < C and A * B exceeds (or might exceed)
 * the capacity of a long.
 * Note that this procedure takes the floor, rather than truncating
 * towards zero, if A < 0.  This ensures that 0 <= R < C.
 */

#define num_bits (sizeof(fixed) * 8)
#define half_bits (num_bits / 2)
#define half_mask ((1L << half_bits) - 1)

/*
 * If doubles aren't wide enough, we lose too much precision by using double
 * arithmetic: we have to use the slower, accurate fixed-point algorithm.
 * See the simpler implementation below for more information.
 */
#define MAX_OTHER_FACTOR_BITS\
  (ARCH_DOUBLE_MANTISSA_BITS - ARCH_SIZEOF_FIXED * 8)
#define ROUND_BITS\
  (ARCH_SIZEOF_FIXED * 8 * 2 - ARCH_DOUBLE_MANTISSA_BITS)

#if ROUND_BITS >= MAX_OTHER_FACTOR_BITS - 1

#ifdef DEBUG
struct {
    long mnanb, mnab, manb, mab, mnc, mdq, mde, mds, mqh, mql;
} fmq_stat;
#  define mincr(x) ++fmq_stat.x
#else
#  define mincr(x) DO_NOTHING
#endif
fixed
fixed_mult_quo(fixed signed_A, fixed B, fixed C)
{
    /* First compute A * B in double-fixed precision. */
    ulong A = (signed_A < 0 ? -signed_A : signed_A);
    long msw;
    ulong lsw;
    ulong p1;

    if (B <= half_mask) {
	if (A <= half_mask) {
	    ulong P = A * B;
	    fixed Q = P / (ulong)C;

	    mincr(mnanb);
	    /* If A < 0 and the division isn't exact, take the floor. */
	    return (signed_A >= 0 ? Q : Q * C == P ? -Q : ~Q /* -Q - 1 */);
	}
	/*
	 * We might still have C <= half_mask, which we can
	 * handle with a simpler algorithm.
	 */
	lsw = (A & half_mask) * B;
	p1 = (A >> half_bits) * B;
	if (C <= half_mask) {
	    fixed q0 = (p1 += lsw >> half_bits) / C;
	    ulong rem = ((p1 - C * q0) << half_bits) + (lsw & half_mask);
	    ulong q1 = rem / (ulong)C;
	    fixed Q = (q0 << half_bits) + q1;

	    mincr(mnc);
	    /* If A < 0 and the division isn't exact, take the floor. */
	    return (signed_A >= 0 ? Q : q1 * C == rem ? -Q : ~Q);
	}
	msw = p1 >> half_bits;
	mincr(manb);
    } else if (A <= half_mask) {
	p1 = A * (B >> half_bits);
	msw = p1 >> half_bits;
	lsw = A * (B & half_mask);
	mincr(mnab);
    } else {			/* We have to compute all 4 products.  :-( */
	ulong lo_A = A & half_mask;
	ulong hi_A = A >> half_bits;
	ulong lo_B = B & half_mask;
	ulong hi_B = B >> half_bits;
	ulong p1x = hi_A * lo_B;

	msw = hi_A * hi_B;
	lsw = lo_A * lo_B;
	p1 = lo_A * hi_B;
	if (p1 > max_ulong - p1x)
	    msw += 1L << half_bits;
	p1 += p1x;
	msw += p1 >> half_bits;
	mincr(mab);
    }
    /* Finish up by adding the low half of p1 to the high half of lsw. */
#if max_fixed < max_long
    p1 &= half_mask;
#endif
    p1 <<= half_bits;
    if (p1 > max_ulong - lsw)
	msw++;
    lsw += p1;
    /*
     * Now divide the double-length product by C.  Note that we know msw
     * < C (otherwise the quotient would overflow).  Start by shifting
     * (msw,lsw) and C left until C >= 1 << (num_bits - 1).
     */
    {
	ulong denom = C;
	int shift = 0;

#define bits_4th (num_bits / 4)
	if (denom < 1L << (num_bits - bits_4th)) {
	    mincr(mdq);
	    denom <<= bits_4th, shift += bits_4th;
	}
#undef bits_4th
#define bits_8th (num_bits / 8)
	if (denom < 1L << (num_bits - bits_8th)) {
	    mincr(mde);
	    denom <<= bits_8th, shift += bits_8th;
	}
#undef bits_8th
	while (!(denom & (-1L << (num_bits - 1)))) {
	    mincr(mds);
	    denom <<= 1, ++shift;
	}
	msw = (msw << shift) + (lsw >> (num_bits - shift));
	lsw <<= shift;
#if max_fixed < max_long
	lsw &= (1L << (sizeof(fixed) * 8)) - 1;
#endif
	/* Compute a trial upper-half quotient. */
	{
	    ulong hi_D = denom >> half_bits;
	    ulong lo_D = denom & half_mask;
	    ulong hi_Q = (ulong) msw / hi_D;

	    /* hi_Q might be too high by 1 or 2, but it isn't too low. */
	    ulong p0 = hi_Q * hi_D;
	    ulong p1 = hi_Q * lo_D;
	    ulong hi_P;

	    while ((hi_P = p0 + (p1 >> half_bits)) > msw ||
		   (hi_P == msw && ((p1 & half_mask) << half_bits) > lsw)
		) {		/* hi_Q was too high by 1. */
		--hi_Q;
		p0 -= hi_D;
		p1 -= lo_D;
		mincr(mqh);
	    }
	    p1 = (p1 & half_mask) << half_bits;
	    if (p1 > lsw)
		msw--;
	    lsw -= p1;
	    msw -= hi_P;
	    /* Now repeat to get the lower-half quotient. */
	    msw = (msw << half_bits) + (lsw >> half_bits);
#if max_fixed < max_long
	    lsw &= half_mask;
#endif
	    lsw <<= half_bits;
	    {
		ulong lo_Q = (ulong) msw / hi_D;
		long Q;

		p1 = lo_Q * lo_D;
		p0 = lo_Q * hi_D;
		while ((hi_P = p0 + (p1 >> half_bits)) > msw ||
		       (hi_P == msw && ((p1 & half_mask) << half_bits) > lsw)
		    ) {		/* lo_Q was too high by 1. */
		    --lo_Q;
		    p0 -= hi_D;
		    p1 -= lo_D;
		    mincr(mql);
		}
		Q = (hi_Q << half_bits) + lo_Q;
		return (signed_A >= 0 ? Q : p0 | p1 ? ~Q /* -Q - 1 */ : -Q);
	    }
	}
    }
}

#else				/* use doubles */

/*
 * Compute A * B / C as above using doubles.  If floating point is
 * reasonably fast, this is much faster than the fixed-point algorithm.
 */
fixed
fixed_mult_quo(fixed signed_A, fixed B, fixed C)
{
    /*
     * Check whether A * B will fit in the mantissa of a double.
     */
#define MAX_OTHER_FACTOR (1L << MAX_OTHER_FACTOR_BITS)
    if (B < MAX_OTHER_FACTOR || any_abs(signed_A) < MAX_OTHER_FACTOR) {
#undef MAX_OTHER_FACTOR
	/*
	 * The product fits, so a straightforward double computation
	 * will be exact.
	 */
	return (fixed)floor((double)signed_A * B / C);
    } else {
	/*
	 * The product won't fit.  However, the approximate product will
	 * only be off by at most +/- 1/2 * (1 << ROUND_BITS) because of
	 * rounding.  If we add 1 << ROUND_BITS to the value of the product
	 * (i.e., 1 in the least significant bit of the mantissa), the
	 * result is always greater than the correct product by between 1/2
	 * and 3/2 * (1 << ROUND_BITS).  We know this is less than C:
	 * because of the 'if' just above, we know that B >=
	 * MAX_OTHER_FACTOR; since B <= C, we know C >= MAX_OTHER_FACTOR;
	 * and because of the #if that chose between the two
	 * implementations, we know that C >= 2 * (1 << ROUND_BITS).  Hence,
	 * the quotient after dividing by C will be at most 1 too large.
	 */
	fixed q =
	    (fixed)floor(((double)signed_A * B + (1L << ROUND_BITS)) / C);

	/*
	 * Compute the remainder R.  If the quotient was correct,
	 * 0 <= R < C.  If the quotient was too high, -C <= R < 0.
	 */
	if (signed_A * B - q * C < 0)
	    --q;
	return q;
    }
}

#endif

#undef MAX_OTHER_FACTOR_BITS
#undef ROUND_BITS

#undef num_bits
#undef half_bits
#undef half_mask

/* Trace calls on sqrt when debugging. */
double
gs_sqrt(double x, const char *file, int line)
{
    if (gs_debug_c('~')) {
	dprintf3("[~]sqrt(%g) at %s:%d\n", x, (const char *)file, line);
	dflush();
    }
    return orig_sqrt(x);
}

static const int isincos[5] =
{0, 1, 0, -1, 0};

/* GCC with -ffast-math compiles ang/90. as ang*(1/90.), losing precission.
 * This doesn't happen when the numeral is replaced with a non-const variable.
 * So we define the variable to work around the GCC problem. 
 */
static double const_90_degrees = 90.;

double
gs_sin_degrees(double ang)
{
    double quot = ang / const_90_degrees;

    if (floor(quot) == quot) {
	/*
	 * We need 4.0, rather than 4, here because of non-ANSI compilers.
	 * The & 3 is because quot might be negative.
	 */
	return isincos[(int)fmod(quot, 4.0) & 3];
    }
    return sin(ang * (M_PI / 180));
}

double
gs_cos_degrees(double ang)
{
    double quot = ang / const_90_degrees;

    if (floor(quot) == quot) {
	/* See above re the following line. */
	return isincos[((int)fmod(quot, 4.0) & 3) + 1];
    }
    return cos(ang * (M_PI / 180));
}

void
gs_sincos_degrees(double ang, gs_sincos_t * psincos)
{
    double quot = ang / const_90_degrees;

    if (floor(quot) == quot) {
	/* See above re the following line. */
	int quads = (int)fmod(quot, 4.0) & 3;

	psincos->sin = isincos[quads];
	psincos->cos = isincos[quads + 1];
	psincos->orthogonal = true;
    } else {
	double arad = ang * (M_PI / 180);

	psincos->sin = sin(arad);
	psincos->cos = cos(arad);
	psincos->orthogonal = false;
    }
}

/*
 * Define an atan2 function that returns an angle in degrees and uses
 * the PostScript quadrant rules.  Note that it may return
 * gs_error_undefinedresult.
 */
int
gs_atan2_degrees(double y, double x, double *pangle)
{
    if (y == 0) {	/* on X-axis, special case */
	if (x == 0)
	    return_error(gs_error_undefinedresult);
	*pangle = (x < 0 ? 180 : 0);
    } else {
	double result = atan2(y, x) * radians_to_degrees;

	if (result < 0)
	    result += 360;
	*pangle = result;
    }
    return 0;
}

/*
 * Define a function for finding intersection of small bars.
 * Coordinates must be so small that their cubes fit into 60 bits.
 * This function doesn't check intersections at end of bars,
 * so  the caller must care of them on necessity.
 * Returns : *ry is the Y-coordinate of the intersection
 * truncated to 'fixed'; *ey is 1 iff the precise Y coordinate of
 * the intersection is greater than *ry (used by the shading algorithm).
 */
bool 
gx_intersect_small_bars(fixed q0x, fixed q0y, fixed q1x, fixed q1y, fixed q2x, fixed q2y, 
			fixed q3x, fixed q3y, fixed *ry, fixed *ey)
{
    fixed dx1 = q1x - q0x, dy1 = q1y - q0y;
    fixed dx2 = q2x - q0x, dy2 = q2y - q0y;
    fixed dx3 = q3x - q0x, dy3 = q3y - q0y;

    int64_t vp2a, vp2b, vp3a, vp3b;
    int s2, s3;

    if (dx1 == 0 && dy1 == 0)
	return false; /* Zero length bars are out of interest. */
    if (dx2 == 0 && dy2 == 0)
	return false; /* Contacting ends are out of interest. */
    if (dx3 == 0 && dy3 == 0)
	return false; /* Contacting ends are out of interest. */
    if (dx2 == dx1 && dy2 == dy1)
	return false; /* Contacting ends are out of interest. */
    if (dx3 == dx1 && dy3 == dy1)
	return false; /* Contacting ends are out of interest. */
    if (dx2 == dx3 && dy2 == dy3)
	return false; /* Zero length bars are out of interest. */
    vp2a = (int64_t)dx1 * dy2;
    vp2b = (int64_t)dy1 * dx2; 
    /* vp2 = vp2a - vp2b; It can overflow int64_t, but we only need the sign. */
    if (vp2a > vp2b)
	s2 = 1;
    else if (vp2a < vp2b)
	s2 = -1;
    else 
	s2 = 0;
    vp3a = (int64_t)dx1 * dy3;
    vp3b = (int64_t)dy1 * dx3; 
    /* vp3 = vp3a - vp3b; It can overflow int64_t, but we only need the sign. */
    if (vp3a > vp3b)
	s3 = 1;
    else if (vp3a < vp3b)
	s3 = -1;
    else 
	s3 = 0;
    if (s2 == 0) {
	if (s3 == 0)
	    return false; /* Collinear bars - out of interest. */
	if (0 <= dx2 && dx2 <= dx1 && 0 <= dy2 && dy2 <= dy1) {
	    /* The start of the bar 2 is in the bar 1. */
	    *ry = q2y;
	    *ey = 0;
	    return true;
	}
    } else if (s3 == 0) {
	if (0 <= dx3 && dx3 <= dx1 && 0 <= dy3 && dy3 <= dy1) {
	    /* The end of the bar 2 is in the bar 1. */
	    *ry = q3y;
	    *ey = 0;
	    return true;
	}
    } else if (s2 * s3 < 0) {
	/* The intersection definitely exists, so the determinant isn't zero.  */
	fixed d23x = dx3 - dx2, d23y = dy3 - dy2;
	int64_t det = (int64_t)dx1 * d23y - (int64_t)dy1 * d23x;
	int64_t mul = (int64_t)dx2 * d23y - (int64_t)dy2 * d23x;
	{
	    /* Assuming small bars : cubes of coordinates must fit into int64_t.
	       curve_samples must provide that.  */
	    int64_t num = dy1 * mul, iiy;
	    fixed iy;
	    fixed pry, pey;

	    {	/* Likely when called form wedge_trap_decompose or constant_color_quadrangle,
		   we always have det > 0 && num >= 0, but we check here for a safety reason. */
		if (det < 0)
		    num = -num, det = -det;
		iiy = (num >= 0 ? num / det : (num - det + 1) / det);
		iy = (fixed)iiy;
		if (iy != iiy) {
		    /* If it is inside the bars, it must fit into fixed. */
		    return false;
		}
	    }
	    if (dy1 > 0) {
		if (iy < 0 || iy >= dy1)
		    return false; /* Outside the bar 1. */
	    } else {
		if (iy > 0 || iy <= dy1)
		    return false; /* Outside the bar 1. */
	    }
	    if (dy2 < dy3) {
		if (iy <= dy2 || iy >= dy3)
		    return false; /* Outside the bar 2. */
	    } else {
		if (iy >= dy2 || iy <= dy3)
		    return false; /* Outside the bar 2. */
	    }
	    pry = q0y + (fixed)iy;
	    pey = (iy * det < num ? 1 : 0);
	    *ry = pry;
	    *ey = pey;
	}
	return true;
    }
    return false;
}
