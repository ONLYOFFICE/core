#ifndef ARCH_H
#define ARCH_H

/* Integer Multi-Dimensional Interpolation */
/*
 * Copyright 2000 Graeme W. Gill
 *
 * This material is licenced under the GNU GENERAL PUBLIC LICENCE :-
 * see the Licence.txt file for licencing details.
 */

#define STR_DEF(def)  #def

#ifdef ALLOW64

/* Detect machine/compiler specifics here */
#if defined(NT) || defined(__WIN32__)
#define longlong __int64
#else	/* !NT, assume standard */
#define longlong long long
#endif	/* !NT */
#define str_longlong STR_DEF(longlong)

#endif /* ALLOW64 */



/* Machine/Language architectural specifications */
typedef struct {
	int bits;		/* Bits in this data type */
	char *name;		/* Name used to specify this type */
	int align;		/* Non-zero if this type should be accessed aligned */
} dtypes;

#define MXDTYPES 6

typedef struct {
	int bigend;		/* Non-zero if this is a bigendian architecture */
	int uwa;		/* Use wide memory access */

	int    pbits;	/* Number of bits in a pointer */

	int    nords;	/* Number of ord types */
	dtypes ords[MXDTYPES];		/* Ordinal types, in size order */
	int    natord;	/* Index of natural machine ordinal */

	int    nints;	/* Number of int types */
	dtypes ints[MXDTYPES];		/* Integer types, in size order */
	int    natint;	/* Index of natural machine integer */

	/* Optimisation settings */
	int    shfm;	/* Non-zero to use shifts for masking */
	int    oscale;	/* Maximum power of 2 scaled indexing mode, 0 for none. */
	int    smmul;	/* Has fast small multiply for index scaling */

} mach_arch;

#endif /* ARCH_H */
