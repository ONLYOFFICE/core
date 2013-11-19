#ifndef IMDI_IMP_H
#define IMDI_IMP_H

/* Integer Multi-Dimensional Interpolation */
/*
 * Copyright 2000 - 2002 Graeme W. Gill
 * All rights reserved.
 *
 * This material is licenced under the GNU GENERAL PUBLIC LICENCE :-
 * see the Licence.txt file for licencing details.
 */

/* Common implementation definitions */

#define IXDI 8		/* maximum input channels/dimensions allowed */
#define IXDO 8		/* maximum output channels/dimensions allowed */

#if IXDI > IXDO		/* Maximum of either DI or DO */
# define IXDIDO IXDI
#else
# define IXDIDO IXDO
#endif

#define ALLOW64		/* Allow declarations but not use of 64 bit types */
					/* Enabling must be done in architecture setup */

#undef USE64		/* Use 64 bit, even on architectures where it's */
					/* not a native size. ALLOW64 must be defined */

/* Private run time implementation definitions */
typedef struct {
	/* Table data */
	void *in_tables[IXDI];		/* Input dimension input lookup tables */
    void *sw_table;				/* Simplex weighting lookup table */
    void *im_table;				/* Interpolation Multi-dimensional lookup table */
	void *out_tables[IXDO];		/* Output dimension output lookup tables */
	int nintabs;				/* Number of input tables */
	int nouttabs;				/* Number of output tables */
} imdi_imp;


/* ------------------------------------------------------- */
/* Macros combination counter */
/* Declare the counter name nn, combinations out of total */
/* Maximum combinations is DI+2 */

#define COMBO(nn, comb, total) 				\
	int nn[IXDI+2];			/* counter value */				\
	int nn##_cmb = (comb);	/* number of combinations*/		\
	int nn##_tot = (total);	/* out of total possible */		\
	int nn##_e				/* dimension index */

/* Set total to new setting */
#define CB_SETT(nn, total)					 		\
	nn##_tot = (total)	/* total possible */

/* Set combinations to new setting */
#define CB_SETC(nn, comb)					 		\
	nn##_cmb = (comb)	/* number of combinations*/

/* Set the counter to its initial value */
#define CB_INIT(nn) 								\
{													\
	for (nn##_e = 0; nn##_e < nn##_cmb; nn##_e++)	\
		nn[nn##_e] = nn##_cmb-nn##_e-1;				\
	nn##_e = 0;										\
}

/* Increment the counter value */
#define CB_INC(nn)									\
{													\
	for (nn##_e = 0; nn##_e < nn##_cmb; nn##_e++) {	\
		nn[nn##_e]++;								\
		if (nn[nn##_e] < (nn##_tot-nn##_e)) {		\
			int nn##_ee;		/* No carry */		\
			for (nn##_ee = nn##_e-1; nn##_ee >= 0; nn##_ee--)	\
				nn[nn##_ee] = nn[nn##_ee+1] + 1;	\
			break;									\
		}											\
	}												\
}

/* After increment, expression is TRUE if counter is done */
#define CB_DONE(nn)									\
	(nn##_e >= nn##_cmb)
	

/* ------------------------------------------------------- */
/* Macros simplex combination counter. */
/* Based on COMBO, but skips invalid simplex combinations */

#define XCOMBO(nn, comb, total) 						\
		 COMBO(nn, comb, total)

/* Set total to new setting */
#define XCB_SETT(nn, total)					 			\
         CB_SETT(nn, total)

/* Set combinations to new setting */
#define XCB_SETC(nn, comb)					 			\
         CB_SETC(nn, comb)


/* Set the counter to its initial value */
#define XCB_INIT(nn) 									\
{														\
	int nn##_ii;										\
														\
	for (nn##_e = 0; nn##_e < nn##_cmb; nn##_e++)		\
		nn[nn##_e] = nn##_cmb-nn##_e-1;					\
	for (nn##_ii = 1; nn##_ii < nn##_cmb; nn##_ii++) {	\
		if ((nn[nn##_ii-1] ^ nn[nn##_ii]) & nn[nn##_ii])\
			break;	/* Went from 0 to 1 */				\
	}													\
	if (nn##_ii < nn##_cmb)	{ /* Fix invalid combination */	\
		XCB_INC(nn);									\
	}													\
	nn##_e = 0;											\
}

/* Increment the counter value */
#define XCB_INC(nn)										\
{														\
	int nn##_ii = 0;									\
														\
	while (nn##_ii < nn##_cmb) {						\
		for (nn##_e = 0; nn##_e < nn##_cmb; nn##_e++) {	\
			nn[nn##_e]++;								\
			if (nn[nn##_e] < (nn##_tot-nn##_e)) {		\
				int nn##_ee;		/* No carry */		\
				for (nn##_ee = nn##_e-1; nn##_ee >= 0; nn##_ee--)	\
					nn[nn##_ee] = nn[nn##_ee+1] + 1;	\
				break;									\
			}											\
		}												\
		if (nn##_e >= nn##_cmb)							\
			break;		/* Done */						\
														\
		/* Reject invalid combinations */				\
		for (nn##_ii = 1; nn##_ii < nn##_cmb; nn##_ii++) {		\
			if ((nn[nn##_ii-1] ^ nn[nn##_ii]) & nn[nn##_ii]) 	\
				break;	/* Went from 0 to 1 */			\
		}												\
	}													\
}

/* After increment, expression is TRUE if counter is done */
#define XCB_DONE(nn)									\
         CB_DONE(nn)
	
/* ------------------------------------------------------- */
/* Macro pseudo-hilbert counter */
/* This multi-dimensional count sequence is a distributed */
/* Gray code sequence, with direction reversal on every */
/* alternate power of 2 scale. */
/* It is intended to aid cache coherence in multi-dimensional */
/* regular sampling. It approximates the Hilbert curve sequence. */

#define PHILBERT(nn) 									\
	int      nn[IXDIDO];/* counter value */				\
	int      nn##di;	/* Dimensionality */			\
	unsigned nn##res;	/* Resolution per coordinate */	\
	unsigned nn##bits;	/* Bits per coordinate */		\
	unsigned nn##ix;	/* Current binary index */		\
	unsigned nn##tmask;	/* Total 2^n count mask */		\
	unsigned nn##count;	/* Usable count */

/* Init counter for dimenion di, resolution res */
#define PH_INIT(nn, pdi, pres) 									\
{																\
	int nn##e;													\
																\
	nn##di  = pdi;												\
	nn##res = (unsigned)pres;									\
																\
	/* Compute bits */											\
	for (nn##bits = 0; (1 << nn##bits) < nn##res; nn##bits++)	\
		;														\
																\
	/* Compute the total count mask */							\
	nn##tmask = ((((unsigned)1) << (nn##bits * nn##di))-1);		\
																\
	/* Compute usable count */									\
	nn##count = 1;												\
	for (nn##e = 0; nn##e < nn##di; nn##e++)					\
		nn##count *= nn##res;									\
																\
	nn##ix = 0;													\
	for (nn##e = 0; nn##e < nn##di; nn##e++)					\
		nn[nn##e] = 0;											\
}

/* Increment the counter value */
#define PH_INC(nn)												\
{																\
	int nn##e;													\
	do {														\
		int nn##b;												\
		int nn##gix;	/* Gray code index */					\
																\
		nn##ix = (nn##ix + 1) & nn##tmask;						\
																\
		/* Convert to gray code index */						\
		nn##gix = nn##ix ^ (nn##ix >> 1);						\
																\
		for (nn##e = 0; nn##e < nn##di; nn##e++) 				\
			nn[nn##e] = 0;										\
																\
		/* Distribute bits */									\
		for (nn##b = 0; nn##b < nn##bits; nn##b++) {			\
			if (nn##b & 1) {	/* In reverse order */			\
				for (nn##e = nn##di-1; nn##e >= 0; nn##e--)  {	\
					nn[nn##e] |= (nn##gix & 1) << nn##b;		\
					nn##gix >>= 1;								\
				}												\
			} else {	/* In normal order */					\
				for (nn##e = 0; nn##e < nn##di; nn##e++)  {		\
					nn[nn##e] |= (nn##gix & 1) << nn##b;		\
					nn##gix >>= 1;								\
				}												\
			}													\
		}														\
																\
		/* Convert from Gray to binary coordinates */			\
		for (nn##e = 0; nn##e < nn##di; nn##e++)  {				\
			unsigned nn##sh, nn##tv;							\
																\
			for(nn##sh = 1, nn##tv = nn[nn##e];; nn##sh <<= 1) {	\
				unsigned nn##ptv = nn##tv;						\
				nn##tv ^= (nn##tv >> nn##sh);					\
				if (nn##ptv <= 1 || nn##sh == 16)				\
					break;										\
			}													\
			/* Filter - increment again if outside cube range */	\
			if (nn##tv >= nn##res)								\
				break;											\
			nn[nn##e] = nn##tv;									\
		}														\
																\
	} while (nn##e < nn##di);									\
																\
}

/* After increment, expression is TRUE if counter has looped back to start. */
#define PH_LOOPED(nn)									\
	(nn##ix == 0)										\
	
/* ------------------------------------------------------- */

#endif /* IMDI_IMP_H */











