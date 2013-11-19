#ifndef IMDI_H
#define IMDI_H

/* Integer Multi-Dimensional Interpolation */
/*
 * Copyright 2000 - 2002 Graeme W. Gill
 * All rights reserved.
 *
 * This material is licenced under the GNU GENERAL PUBLIC LICENCE :-
 * see the Licence.txt file for licencing details.
 */

/*
 * This software provides support for high speed integer
 * multimensional interpolation.  
 */

/*
 * This file provides the common definitions for IMDI, and
 * the data structures for communcating between the client
 * imdi object.
*/

/* Pixel representation description */

/* This is a high level macro desciption of the pixel layout. */
/* It can be expanded by adding a new enumeration, and then */
/* implementing the code in imdi_gen to translate the enumeration */
/* into the exact pixlayout structure details. */

typedef enum {
	invalid_rep = 0,
	pixint8    = 1,		/* 8 Bits per value, pixel interleaved, no padding */
	planeint8  = 2,		/* 8 bits per value, plane interleaved */
	pixint16   = 3,		/* 16 Bits per value, pixel interleaved, no padding */
	planeint16 = 4		/* 16 bits per value, plane interleaved */
} imdi_pixrep;

/* IMDI Object */
struct _imdi {
	void *impl;			/* Pointer to implementation information */

	/* Do the interpolation */
	void (*interp)(struct _imdi *s, void **inp, void **outp, unsigned int npixels);
	void (*done)(struct _imdi *s);		/* Done with it */

}; typedef struct _imdi imdi;

/* Create a new imdi */
/* Return NULL if request is not supported */
imdi *new_imdi(
	int id,			/* Number of input dimensions */
	int od,			/* Number of output dimensions */
	imdi_pixrep in,	/* Input pixel representation */
	int in_signed,	/* Bit flag per channel, NZ if treat as signed */
	imdi_pixrep out,/* Output pixel representation */
	int out_signed,	/* Bit flag per channel, NZ if treat as signed */
	int res,		/* Desired table resolution */

	/* Callbacks to lookup the mdi table values */
	double (*input_curve) (void *cntx, int ch, double in_val),
	void   (*md_table)    (void *cntx, double *out_vals, double *in_vals),
	double (*output_curve)(void *cntx, int ch, double in_val),
	void *cntx		/* Context to callbacks */
);

#endif /* IMDI_H */






