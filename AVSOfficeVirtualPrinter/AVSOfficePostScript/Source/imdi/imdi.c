/* Integer Multi-Dimensional Interpolation */
/*
 * Copyright 2000 - 2002 Graeme W. Gill
 * All rights reserved.
 *
 * This material is licenced under the GNU GENERAL PUBLIC LICENCE :-
 * see the Licence.txt file for licencing details.
 */


/*
   This is the implementation of the run time code.
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>
#include <string.h>

#include "imdi_imp.h"
#include "imdi_gen.h"
#include "imdi_tab.h"
#include "imdi_k.h"			/* Declaration of all the kernel functions */

#undef VERBOSE

static void imdi_free(imdi *im);

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
) {
	int i;
	int bk = -1;	/* Best table */
	int bfig = 0x7fffffff;	/* Best tables figure of merit */
	int bstres;		/* Best tables target stres */
	genspec bgs;	/* Best gen spec */
	
	tabspec ts;		/* Table specifications */
	imdi *im;

#ifdef VERBOSE
	printf("new_imdi called with id %d, od %d, res %d\n", id, od, res);
	printf("about to checking %d kernels\n", no_kfuncs);
#endif

	/* The first thing to do is see if there is an available kernel function */
	for (i = 0; i < no_kfuncs; i++) {
		genspec gs;
		int stres;				/* Computed stres needed */
		int fig;
		ktable[i].gen(&gs);		/* Get the kernel functions genspec */

#ifdef VERBOSE
	printf("\n");
	printf("kernel %d has id %d, od %d, irep %d orep %d\n", i, gs.id, gs.od, gs.irep, gs.orep);
	printf("Input req is id %d, od %d, irep %d orep %d\n", id, od, in, out);
#endif
		/* First check mandatory things */
		if (id != gs.id			/* Input dimension */
		 || od != gs.od			/* Output dimension */
		 || in != gs.irep		/* Input  dimension */
		 || out != gs.orep) {	/* Output dimension */
			continue;
		}

#ifdef VERBOSE
		printf("  found match\n");
#endif
		ktable[i].tab(&ts);		/* Get the kernel functions tabspec */

		if (ts.sort) {
			stres = 0;
#ifdef VERBOSE
		printf("gres = %d\n",res);
#endif
			/* We want one that is equals or exceeds the desired */
			/* resolution, but doesn't exceed it too much, or the code */
			/* will be inefficient. */
			/* If there are no routines that can meet the desired precision, */
			/* then it is ok to use the one closest to the desired precision. */
			if (gs.itres >= res) {
				fig = 10 * (gs.itres - res);
			} {
				fig = 0;
				if (gs.itres < res) {
					fig += 10000 * (res - gs.itres);
				}
			}
		} else {
			/* compute the needed stres (Assumint not sort) */
			stres = ((1 << gs.prec)-1 + res-2)/(res-1);
/*			stres = ((1024)-1 + res-2)/(res-1); */

#ifdef VERBOSE
		printf("gres = %d, sres = %d\n",res,stres);
#endif
			/* We want one that is equals or exceeds the desired */
			/* resolution, but doesn't exceed it too much, or the code */
			/* will be inefficient. */
			/* If there are no routines that can meet the desired precision, */
			/* then it is ok to use the one closest to the desired precision. */
			if (gs.itres >= res && gs.stres >= stres) {
				fig = 10 * (gs.itres - res) + (gs.stres - stres);
			} {
				fig = 0;
				if (gs.itres < res) {
					fig += 10000 * (res - gs.itres);
				}
				if (gs.stres < stres) {
					fig += 1000 * (stres - gs.stres);
				}
			}
		}

#ifdef VERBOSE
		printf("  figure of merit %d\n",fig);
#endif
		/* Is this the best one so far ? */
		if (fig < bfig) {
			bfig = fig;
			bk = i;
			bstres = stres;
			bgs = gs;		/* Structure copy */
#ifdef VERBOSE
			printf("  best so far\n");
#endif
		}
	}

	if (bk < 0) {
#ifdef VERBOSE
		printf("new_imdi failed - dimensionality or representations couldn't be matched\n");
#endif
		return NULL;	/* Nothing matches */
	}

	if ((im = (imdi *)malloc(sizeof(imdi))) == NULL) {
#ifdef VERBOSE
		printf("new_imdi malloc imdi failed\n");
#endif
		/* Should we return an error somehow ? */
		return NULL;
	}

	/* We've decided kernel function bk is going to be the best, */
	/* so now setup the appropriate tables to use with it. */
	ktable[bk].tab(&ts);		/* Get the kernel functions tabspec */
	
	if (bgs.itres > res)
		bgs.itres = res;		/* Tell table create what the res is */
	if (bgs.stres > bstres)
		bgs.stres = bstres;

	/* Tel table setup how to treat integer input in per channel lookups */
	bgs.in_signed = in_signed;
	bgs.out_signed = out_signed;

#ifdef VERBOSE
	if (!ts.sort) {
		if ((bgs.stres * (bgs.itres-1)) < ((1 << bgs.prec)-1)) {
			printf("Warning: table chosen doesn't reach desired precision!\n");
			printf("Wanted %d, got %d\n", ((1 << bgs.prec)-1), (bgs.stres * (bgs.itres-1)));
		}
	}
#endif

	/* Allocate and initialise the appropriate tables */
	im->impl = (void *)imdi_tab(&bgs, &ts, input_curve, md_table, output_curve, cntx);

	if (im->impl == NULL) {
#ifdef VERBOSE
		printf("imdi_tab failed\n");
#endif
		imdi_free(im);
		return NULL;
	}

	im->interp   = ktable[bk].interp;	
	im->done     = imdi_free;

#ifdef VERBOSE
	printf("new_imdi returning 0x%x\n", im);
#endif
	return im;
}


/* Free up the structure */
static void imdi_free(
imdi *im
) {
	/* Free all the allocated tables */
	if (im->impl != NULL)
		imdi_tab_free((imdi_imp *)im->impl);

	/* Free this structure */
	free(im);
}


















