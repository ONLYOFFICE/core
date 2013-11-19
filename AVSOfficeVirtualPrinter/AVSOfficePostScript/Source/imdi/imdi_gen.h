#ifndef IMDI_GEN_H
#define IMDI_GEN_H

/* Integer Multi-Dimensional Interpolation */
/*
 * Copyright 2000 - 2002 Graeme W. Gill
 * All rights reserved.
 *
 * This material is licenced under the GNU GENERAL PUBLIC LICENCE :-
 * see the Licence.txt file for licencing details.
 */

/* Kernel code generation definitions */

#include "imdi.h"
#include "arch.h"

typedef enum {
	invalid_dir = 0,
	forward = 1,		/* 8 Bits per value, pixel interleaved, no padding */
	backward = 2		/* 8 bits per value, plane interleaved */
} imdi_dir;

/* -------------------------------------------------- */
/* High level kernel generation description */

/* This sructure allows a series of related kernels to be generated, */
/* the only variation being the number of input and output dimensions. */
typedef struct {
	int idcombs[10];	/* Input dimension combinations (0 at end) */
	int itres[10];		/* Interpolation table resolutions for each input dimension */
	int stres[10];		/* Simplex table resolutions for each input dimension */
	int odcombs[10];	/* Output dimensions combinations (0 at end) */
	imdi_pixrep incombs[10];/* Input pixel representation combinations */
	imdi_pixrep outcombs[10];	/* Output pixel representation combinations */
	imdi_dir dircombs[10];	/* Direction combinations (not implemented) */
} gendesc;

/* -------------------------------------------------- */
/* Detailed level of generation specification */

/* Pixel layout: */
/* Each channel value is assumed to be read with a 		*/
/* native machine single read of size bpch, 			*/
/* and bov and bpv being bit indexes into that value.	*/
/*                                        				*/
/* If pint == 0, then each read will be of size bpch[], and will */
/* have its own pointer, will be incremented by chi[].	*/
/* If pint != 0, then the reads will be size bpch[0], from successive */
/* locations, chi[] apart. 								*/
/*                                        				*/
/* If packed == 0, then separate reads are needed for each input */
/* channel.												*/
/* If packed != 0, then the channel values will be extracted */
/* from a single read of size bpch[0]					*/
/*                                        				*/
/* Note that at all times the bit offset and size values */
/* will be obeyed for each input value. 				*/

typedef struct {
	int bpch[IXDIDO];	/* Bits per channel read (must be divisible by 8) */
	int chi[IXDIDO];	/* channel increment in multiples of bpch[] (0 == dimensionality) */
	int bov[IXDIDO];	/* Bit offset to value within channel */
	int	bpv[IXDIDO];	/* Bits per value within channel */
	int pint;			/* Flag - nonz if pixel interleaved (ie. reads from successice locations) */
	int packed;			/* Flag - nonz if all channels are packed into a single read */
} pixlayout;

/* Structure that specifies the configuration of a generated interpolation kernel. */
typedef struct {
	/* Input to code generator */
	int prec;		/* Basic precision:- either 8 or 16 bits, determined by irep */ 
	int id;			/* Number of input dimensions */
	int od;			/* Number of output dimensions */
	imdi_pixrep irep;/* Input pixel representation */
	imdi_pixrep orep;/* Output pixel representation */
	int in_signed;	/* Bit flag per channel, NZ if treat as signed (runtime setup) */
	int out_signed;	/* Bit flag per channel, NZ if treat as signed (runtime setup) */
	pixlayout in;	/* Input pixel layout */
	pixlayout out;	/* Output pixel layout */
	int dir;		/* forward or backward (not implemented */
	int itres;		/* Interpolation table resolution */
	int stres;		/* Simplex table resolution */

	/* Returned value */
	char kkeys[100];				/* Kernel keys */
	char kdesc[100];				/* At genspec time */
	char kname[100];				/* At generation time */
} genspec;

/* Supported code generators */

int gen_c_kernel(genspec *g, mach_arch *a, FILE *fp, int index);	/* The 'C' code generator */

/* asm, MMX, etc. generators declarations go here ! */

#endif /* IMDI_GEN_H */











