
/* Integer Multi-Dimensional Interpolation */

/*
 * Copyright 2000 - 2002 Graeme W. Gill
 * All rights reserved.
 *
 * This material is licenced under the GNU GENERAL PUBLIC LICENCE :-
 * see the Licence.txt file for licencing details.
 */

/* 'C' code color transform kernel code generator. */

/*
   This module generates C code routines which implement
   an integer multi-channel transform. The input values
   are read, passed through per channel lookup tables,
   a multi-dimentional interpolation table, and then
   a per channel output lookup table, before being written.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>
#include <string.h>

#include "imdi_imp.h"
#include "imdi_gen.h"
#include "imdi_tab.h"

#undef VERBOSE
#undef FORCESORT		/* Use sort algorithm allways */

/*
 * TTBD:
 *		Need to implement g->dir
 *      Haven't used t->it_map[] or t->im_map[].
 *
 *
 */

/* ------------------------------------ */
/* Context */
typedef struct {
	FILE *of;			/* Output file */
	int indt;			/* Indent */

	/* Other info */
	genspec *g;			/* Generation specifications */
	tabspec *t;			/* Table setup data */
	mach_arch *a;		/* Machine architecture and tuning data */

	/* Code generation information */
	/* if() conditions are for entry usage */

	/* Pixel read information */
	int ipt[IXDI];		/* Input pointer types */
	int nip;			/* Actual number of input pointers, accounting for pint */
	int chv_bits;		/* Bits in chv temp variable ?? */

	/* Input table entry */
	int itet;			/* Input table entry type */
	int itvt;			/* Input table variable type */
	int itmnb;			/* Input table minimum bits (actual is it_ab) */

	/* Interpolation index */
	int ixet;			/* Interpolation index entry type */
	int ixvt;			/* Interpolation index variable type */
	int ixmnb;			/* Interpolation index minimum bits (actual is ix_ab) */
	int ixmxres;		/* Interpolation table maximum resolution */

	/* Simplex index: if(!sort && it_xs) */
	int sxet;			/* Simplex index entry type  */
	int sxvt;			/* Simplex index variable type */
	int sxmnb;			/* Simplex index bits minimum (actual is sx_ab) */
	int sxmxres;		/* Simplex table maximum resolution (0 if sort) */

	/* Combination Weighting + Vertex offset values: if(it_xs && !wo_xs) */
	int woet;			/* Weighting+offset entry type  */
	int wovt;			/* Weighting+offset variable type */
	int womnb;			/* Weighting+offset index bits minimum (actual is wo_ab) */

	/* Weighting value: if(it_xs && wo_xs) */
	int weet;			/* Weighting entry type  */
	int wevt;			/* Weighting variable type */
	int wemnb;			/* Weighting index bits minimum (actual is we_ab) */

	/* Vertex offset value: if(it_xs && wo_xs) */
	int voet;			/* Vertex offset entry type  */
	int vovt;			/* Vertex offset variable type */
	int vomnb;			/* Vertex offset index bits minimum (actual is vo_ab) */

	/* Interpolation table entry: */
	int imovb;			/* Interpolation output value bits per channel required */
	int imfvt;			/* Interpolation full entry & variable type */
	int impvt;			/* Interpolation partial entry variable type */

	/* Interpolation accumulators: */
	int iaovb;			/* Interpolation output value bits per channel required */
	int iafvt;			/* Interpolation full entry & variable type */
	int iapvt;			/* Interpolation partial entry variable type */
	int ian;			/* Total number of accumulators */

	/* Output table lookup */
	int otit;			/* Output table index type */
	int otvt;			/* Output table value type (size is ot_ts bytes) */

	/* Write information */
	int opt[IXDO];		/* Output pointer types */
	int nop;			/* Actual number of output pointers, accounting for pint */

} fileo;

void line(fileo *f, char *fmt, ...);	/* Output one line */
void sline(fileo *f, char *fmt, ...);	/* Output start of line line */
void mline(fileo *f, char *fmt, ...);	/* Output middle of line */
void eline(fileo *f, char *fmt, ...);	/* Output end of line */
void cr(fileo *f) { line(f,""); }		/* Output a blank line */
void inc(fileo *f) { f->indt++; }		/* Increment the indent level */
void dec(fileo *f) { f->indt--; }		/* Decrement the indent level */
/* ------------------------------------ */

int findord(fileo *f, int bits);		/* Find ordinal with bits or more */
int nord(fileo *f, int ov);				/* Round ordinal type up to natural size */
int findnord(fileo *f, int bits);		/* Find ordinal with bits, or natural larger */
int findint(fileo *f, int bits);		/* Find integer with bits or more */
int nint(fileo *f, int iv);				/* Round integer type up to natural size */
int findnint(fileo *f, int bits);		/* Find integer with bits, or natural larger */
static void doheader(fileo *f);

static int calc_bits(int dim, int res);
static int calc_res(int dim, int bits);
static int calc_obits(int dim, int res, int esize);
static int calc_ores(int dim, int bits, int esize);


/* return a hexadecimal mask string */
/* take care of the case when bits >= 32 */
char *hmask(int bits) {
	static char buf[20];

	if (bits < 32) {
		sprintf(buf, "0x%x",(1 << bits)-1);
	} else if (bits == 32) {
		return "0xffffffff";
	} else if (bits == 64) {
		return "0xffffffffffffffff";
	} else {	/* Bits > 32 */
		sprintf(buf, "0x%xffffffff",(1 << (bits-32))-1);
	}
	return buf;
}

/* Generate a source file to implement the specified */
/* interpolation kernel. Fill in return values and return 0 if OK. */
/* Return non-zero on error. */
int gen_c_kernel(
	genspec *g,				/* Specification of what to generate */
	mach_arch *a,
	FILE *fp,				/* File to write to */
	int index				/* Identification index, 1 = first */
) {
	unsigned char kk[] = { 0x43, 0x6F, 0x70, 0x79, 0x72, 0x69, 0x67, 0x68,
	                       0x74, 0x20, 0x32, 0x30, 0x30, 0x34, 0x20, 0x47,
	                       0x72, 0x61, 0x65, 0x6D, 0x65, 0x20, 0x57, 0x2E,
	                       0x20, 0x47, 0x69, 0x6C, 0x6C, 0x00 };
	fileo f[1];
	int e, i;
	tabspec tabsp, *t = &tabsp;
	int timp = 0;		/* Flag to use temporary imp pointer. */
						/* Seem to make x86 MSVC++ slower */
						/* Has no effect on x86 IBMCC */

	sprintf(g->kname, "imdi_k%d",index); /* Kernel routine base name */
	strcpy(g->kkeys, kk);				 /* Kernel keys for this session */
	
	/* Setup the file output context */
	f->of = fp;
	f->indt = 0;			/* Start with no indentation */
	f->g = g;
	f->t = t;
	f->a = a;

	if (g->prec == 8) {
		if (g->id <= 4)
			t->sort = 0;		/* Implicit sort using simplex table lookup */
		else
			t->sort = 1;		/* Explicit sort */

	} else  if (g->prec == 16) {
		t->sort = 1;			/* Explit sort, no simplex table */

	} else {
		fprintf(stderr,"Can't cope with requested precision of %d bits\n",g->prec);
		exit(-1);
	}
#ifdef FORCESORT
	t->sort = 1;
#endif

	/* Compute input read and input table lookup stuff */

	/* Compute number of input pointers */
	if (g->in.pint != 0)	/* Pixel interleaved */
		f->nip = 1;
	else
		f->nip = g->id;

	/* Figure out the input pointer types */
	for (e = 0; e < f->nip; e++) {
		if ((f->ipt[e] = findord(f, g->in.bpch[e])) < 0) {
			fprintf(stderr,"Input channel size can't be handled\n");
			exit(-1);
		}
	}

	/* Set a default input channel mapping */
	for (e = 0; e < g->id; e++)
		t->it_map[e] = e;

	/* Do the rest of the input table size calculations after figuring */
	/* out simplex and interpolation table sizes. */


	/* Figure out the interpolation multi-dimentional table structure */
	/* and output accumulation variable sizes. */

	if (g->prec == 8
	 || g->prec == 16 && a->ords[a->nords-1].bits >= (g->prec * 4)) {
		int tiby;		/* Total interpolation bytes needed */

		/* We assume that we can normally compute more than one */
		/* output value at a time, so we need to hold the interpolation */
		/* output data in the expanded fixed point format in both the */
		/* table and accumulator. */
		t->im_cd = 1;
		f->imovb = g->prec * 2;		/* 16 bits needed for 8 bit precision, */
		f->iaovb = g->prec * 2;		/* 32 bits needed for 16 bit precision */
		f->imfvt = a->nords-1;		/* Full variable entry type is biggest available */
		f->iafvt = a->nords-1;		/* Full variable accum. type is same */

		if (a->ords[f->imfvt].bits < f->imovb) {
			fprintf(stderr,"Interpolation table entry size can't be handled\n");
			exit(-1);
		}

		/* Compute details of table entry sizes, number */
		tiby = (f->imovb * g->od)/8;				/* Total table bytes needed */
		t->im_fs = a->ords[f->imfvt].bits/8;		/* Full entry bytes */
		t->im_fv = (t->im_fs * 8)/f->imovb;			/* output values per full entry . */
		t->im_fn = tiby/t->im_fs;					/* Number of full entries (may be 0) */
		t->im_ts = t->im_fn * t->im_fs;				/* Structure size so far */
		tiby -= t->im_fn * t->im_fs;				/* Remaining bytes */

		if (tiby <= 0) {
			t->im_pn = 0;		/* No partials */
			t->im_ps = 0;
			t->im_pv = 0;
			f->impvt = 0;
			f->iapvt = 0;

		} else {
			t->im_pn = 1;					/* Must be just 1 partial */
			t->im_pv = (tiby * 8)/f->imovb;	/* Partial holds remaining entries */ 

			if ((f->impvt = findnord(f, tiby * 8)) < 0) {
				fprintf(stderr,"Can't find partial interp table entry variable size\n");
				exit(-1);
			}
			f->iapvt = f->impvt;
			t->im_ps = a->ords[f->impvt].bits/8;/* Partial entry bytes */

			if (a->ords[f->imfvt].align)		/* If full entry's need to be aligned */
				t->im_ts += t->im_fs;			/* Round out struct size by full entry */
			else
				t->im_ts += t->im_ps;			/* Round out to natural size */
		}
	
	} else {
		/* One 16 bit output value per entry + 32 bit accumulator. */
		/* We can conserve table space by not holding the table data in expanded */
		/* fixed point format, but expanding it when it is read. */
		/* Without resorting to compicated code, this restricts us */
		/* to only computing one output value per accumulator. */
		t->im_cd = 0;
		f->imovb = g->prec;			/* Table holds 16 bit entries with no fractions */
		f->iaovb = g->prec * 2;		/* 32 bits needed for 16 bit precision in comp. */

		if ((f->imfvt = findord(f, f->imovb)) < 0) {
			fprintf(stderr,"Interpolation table entry size can't be handled\n");
			exit(-1);
		}
		if ((f->iafvt = findord(f, f->iaovb)) < 0) {
			fprintf(stderr,"Interpolation accumulator size can't be handled\n");
			exit(-1);
		}

		/* Compute details of table entry sizes, number */
		t->im_fs = a->ords[f->imfvt].bits/8;		/* Full entry bytes */
		t->im_fv = 1;								/* output values per full entry . */
		t->im_fn = g->od;							/* Number of full entries */
		t->im_ts = t->im_fn * t->im_fs;				/* Total structure size */

		t->im_pn = 0;		/* No partials */
		t->im_ps = 0;
		t->im_pv = 0;
		f->impvt = 0;
		f->iapvt = 0;
	}
	f->ian = t->im_fn + t->im_pn;			/* Total number of output accumulators */

	/* Figure out how much of the interpolation entry offset to put in the */
	/* vertex offset value, and how much to make explicit in accessing the */
	/* interpolation table enty. */
	if (a->oscale > 0) {		/* We have a scaled index mode */
		/* Use as much of the scaled index mode as possible */
		/* and then do the balance by scaling the simplex index entry. */
		for (t->im_oc = a->oscale; ; t->im_oc >>= 1) {
			t->vo_om = t->im_ts/t->im_oc;		/* Simplex index multiplier */
			if ((t->vo_om * t->im_oc) == t->im_ts)
				break;				/* Got appropriate offset scale */
		}
	} else if (a->smmul) {		/* Architecure supports fast small multiply */
		t->im_oc = t->im_ts;	/* Do scale by structure size explicitly */
		t->vo_om = 1;			/* Do none in the Simplex index */
	} else {					/* We have no fast tricks */
		t->im_oc = 1;			/* Do none explicitly */
		t->vo_om = t->im_ts;	/* Do all in Simplex index */
	}

	/* Compute the number of bits needed to hold an index into */
	/* the interpolation table (index is in terms of table entry size). */
	/* This value is used to figure out the room needed in the input */
	/* table to accumulate the interpolation cube base offset value. (IM_O macro) */
	f->ixmnb = calc_bits(g->id, g->itres);

	/* Set a default output channel mapping */
	for (e = 0; e < g->od; e++)
		t->im_map[e] = e;

#ifdef VERBOSE
	/* Summarise the interpolation table arrangements */
	printf("\n");
	printf("Interpolation table structure:\n");
	printf("  Minimum bits needed to index table %d\n", f->ixmnb);
	printf("  Entry total size %d bytes\n", t->im_ts);
	printf("  Simplex entry offset scale %d\n", t->vo_om);
	printf("  Explicit entry offset scale %d\n", t->im_oc);
	printf("  %d full entries, size %d bytes\n", t->im_fn, t->im_fs);
	printf("  %d partial entries, size %d bytes\n", t->im_pn, t->im_ps);
	printf("  to hold %d output values of %d bits\n", g->od, f->imovb);

#endif /* VERBOSE */

	/* Number of bits needed for the weighting value */
	f->wemnb = g->prec+1;	/* Need to hold a weighting factor of 0 - 256 for 8 bits */
							/* Need to hold a weighting factor of 0 - 65536 for 16 bits */

	/* Variable that would be used to hold it */
	if ((f->wevt = findnord(f, f->wemnb)) < 0) {
		fprintf(stderr,"Can't find entry size to hold weighting variable\n");
		exit(-1);
	}

	/* Number of bits needed for vertex offset value */
	f->vomnb = calc_obits(g->id, g->itres, t->vo_om);

	/* Variable that would be used to hold it */
	if ((f->vovt = findnord(f, f->vomnb)) < 0) {
		fprintf(stderr,"Can't find entry size to hold vertex offset variable\n");
		exit(-1);
	}

	if (t->sort) {
		/* If we are using an explicit sort, we need to figure how many */
		/* separate entries we need to use to hold the interpolation index, */
		/* weighting factor and vertex offset values in the input table. */

		/* First try all three in one entry */
		if ((f->itet = findord(f, f->ixmnb + f->wemnb + f->vomnb)) >= 0) {/* size to read */
			int rem;						/* Remainder bits */

			t->it_xs = 0;					/* Combined interp+weight+offset */
			t->wo_xs = 0;
			t->it_ab = a->ords[f->itet].bits;	/* Bits in combined input entry */
			rem = t->it_ab - f->ixmnb - f->wemnb - f->vomnb; /* Spair bits */
			t->we_ab = f->wemnb;				/* Get minimum weight bits */
			t->vo_ab = f->vomnb + rem/2;		/* vertex offset index bits actually available */
			t->ix_ab = t->it_ab - t->vo_ab - t->we_ab;	/* interp index bits actually available */
			t->wo_ab = t->we_ab + t->vo_ab;		/* Weight & offset total bits */
			t->it_ts = a->ords[f->itet].bits/8;	/* total size in bytes */
			f->itvt = nord(f, f->itet);			/* Variable type */

			if ((f->wovt = findnord(f, t->we_ab + t->vo_ab)) < 0) {
				fprintf(stderr,"Can't find variable size to hold weight/offset\n");
				exit(-1);
			}
			if ((f->wevt = findnord(f, t->we_ab)) < 0) {
				fprintf(stderr,"Can't find variable size to hold weighting factor\n");
				exit(-1);
			}
			if ((f->vovt = findnord(f, t->vo_ab)) < 0) {
				fprintf(stderr,"Can't find variable size to hold vertex offset index\n");
				exit(-1);
			}
			if ((f->ixvt = findnord(f, t->ix_ab)) < 0) {
				fprintf(stderr,"Interp index variable size can't be handled\n");
				exit(-1);
			}
		} else {	/* Interp index will be a separate entry */
			int wit, oft, bigt;		/* weighting type, offset type, biggest type */
			int combt;				/* Combined type */
			int sepbits, combits;	/* Total separate, combined bits */

			t->it_xs = 1;				/* Separate interp index and weighting+offset */
			if ((f->ixet = findord(f, f->ixmnb)) < 0) {
				fprintf(stderr,"Interp index entry size can't be handled\n");
				exit(-1);
			}
			f->ixvt = nord(f, f->ixet);		/* Variable type */
			t->ix_ab = a->ords[f->ixet].bits;
			t->ix_es = t->ix_ab/8;
			t->ix_eo = 0;
			t->it_ts = t->ix_es;			/* Input table size so far */

			/* Now figure weighting and vertex offset */

			/* See if we can fit them into separately readable entries, or whether */
			/* they should be combined to minimise overall table size. */

			if ((wit = findord(f, f->wemnb)) < 0) {
				fprintf(stderr,"Can't find entry size to hold weighting factor\n");
				exit(-1);
			}
			if ((oft = findord(f, f->vomnb)) < 0) {
				fprintf(stderr,"Can't find entry size to hold vertex offset index\n");
				exit(-1);
			}
			bigt = wit > oft ? wit : oft;			/* Bigest separate type */

			if ((combt = findord(f, f->wemnb + f->vomnb)) < 0) {/* Combined isn't possible */
				sepbits = 2 * a->ords[bigt].bits;		/* Total separate bits */
				combits = sepbits;						/* Force separate entries */
			} else {
				sepbits = 2 * a->ords[bigt].bits;		/* Total separate bits */
				combits = a->ords[combt].bits;			/* Total combined bits */
			}

			if (sepbits <= combits) {				/* We will use separate entries */
				t->wo_xs = 1;
				t->we_es = a->ords[bigt].bits/8;	/* size in bytes for weighting entry */
				t->we_ab = a->ords[bigt].bits;		/* bits available for weighting */
				t->we_eo = t->ix_es;				/* Entry offset in input table */
				t->vo_es = a->ords[bigt].bits/8;	/* size in bytes for vertex offset entry */
				t->vo_ab = a->ords[bigt].bits;		/* bits available for vertex offset */
				t->vo_eo = t->ix_es + t->we_es;		/* Entry offset in input table */
				t->wo_es = t->we_es + t->vo_es;		/* Total entry size for each vertex */
				t->it_ts += t->we_es + t->vo_es;	/* Total input entry size in bytes */

				f->weet = bigt;				/* Variable type for accessing weighting entry */
				f->voet = bigt;				/* Variable type for accessing vertex offset entry */
				f->wevt = nord(f, wit);		/* Variable type for holding weight value */
				f->vovt = nord(f, oft);		/* Variable type for holding offset value */

			} else {								/* We will combine the two entries */
				t->wo_xs = 0;
				t->wo_es = a->ords[combt].bits/8;	/* entry size in bytes for each entry */
				t->wo_ab = a->ords[combt].bits;		/* bits in weightig + offset */
				t->we_ab = f->wemnb;				/* bits available for weighting */
				t->vo_ab = t->wo_ab - t->we_ab;		/* Allow all spare bits to vertex offset */
				t->wo_eo = t->ix_es;				/* entry offset in input table */
				t->it_ts += t->wo_es;				/* Final input table size */

				f->woet = combt;			/* Variable type for accessing combined entry */
				f->wovt = nord(f, combt);	/* Variable type holding weight/offset read value */

				if ((f->wevt = findnord(f, t->we_ab)) < 0) {
					fprintf(stderr,"Can't find variable size to hold weighting factor\n");
					exit(-1);
				}
				if ((f->vovt = findnord(f, t->vo_ab)) < 0) {
					fprintf(stderr,"Can't find variable size to hold vertex offset index\n");
					exit(-1);
				}
			}
		}
#ifdef VERBOSE
		/* Summarise the input table arrangements */
		printf("\n");
		printf("Input table structure:\n");
		printf("  Input value re-ordering is [");
		for (e = 0; e < g->id; e++)
			printf("%s%d",e > 0 ? " " : "", t->it_map[e]);
		printf("]\n");
		printf("  Input table entry size = %d bytes\n",t->it_ts);
		if (t->it_ix) {
			printf("  Input table extracts value from read values\n");
			if (t->wo_xs) {
				printf("  Separate Interp., Weighting and Offset values\n");
				printf("  Interp. index is at offset %d, size %d bytes\n",t->ix_eo, t->ix_es);
				printf("  Weighting is at offset %d, size %d bytes\n",t->we_eo, t->we_es);
				printf("  Vertex offset is at offset %d, size %d bytes\n",t->vo_eo, t->vo_es);
			} else {
				printf("  Separate Interp. index and Weightint+Offset value\n");
				printf("  Interp. index is at offset %d, size %d bytes\n",t->ix_eo, t->ix_es);
				printf("  Weighting+Offset is at offset %d, size %d bytes\n",t->wo_eo, t->wo_es);
				printf("  Weighting     = %d bits\n",t->we_ab);
				printf("  Vertex offset = %d bits\n",t->vo_ab);
			}
		} else {
			printf("  Combined InterpIndex+Weighting+Voffset values\n");
			printf("  Values are stored in size %d bytes\n",t->it_ts);
			printf("  Interp. index = %d bits\n",t->ix_ab);
			printf("  Weighting     = %d bits\n",t->we_ab);
			printf("  Vertex offset = %d bits\n",t->vo_ab);
		}
#endif /* VERBOSE */

	} else {	/* Simplex table */
		/* If we are going to use a simplex table, figure out how we */
		/* will store the weighting value and vertex offset values in it, */
		/* as well as the size of index we'll need to address it. */
		int wit, oft, bigt;		/* weighting type, offset type, biggest type */
		int combt;				/* Combined type */
		int sepbits, combits;	/* Total separate, combined bits */

		/* See if we can fit them into separately readable entries, or whether */
		/* they should be combined to minimise overall table size. */

		if ((wit = findord(f, f->wemnb)) < 0) {
			fprintf(stderr,"Can't find entry size to hold weighting factor\n");
			exit(-1);
		}
		if ((oft = findord(f, f->vomnb)) < 0) {
			fprintf(stderr,"Can't find entry size to hold vertex offset index\n");
			exit(-1);
		}
		bigt = wit > oft ? wit : oft;			/* Bigest separate type */

		if ((combt = findord(f, f->wemnb + f->vomnb)) < 0) {/* Combined isn't possible */
			sepbits = 2 * a->ords[bigt].bits;		/* Total separate bits */
			combits = sepbits;						/* Force separate entries */
		} else {
			sepbits = 2 * a->ords[bigt].bits;		/* Total separate bits */
			combits = a->ords[combt].bits;			/* Total combined bits */
		}

		if (sepbits <= combits) {				/* We will use separate entries */
			t->wo_xs = 1;
			t->we_es = a->ords[bigt].bits/8;	/* size in bytes for weighting entry */
			t->we_ab = a->ords[bigt].bits;		/* bits available for weighting */
			t->we_eo = 0;						/* Entry offset in simplex table */
			t->vo_es = a->ords[bigt].bits/8;	/* size in bytes for vertex offset entry */
			t->vo_ab = a->ords[bigt].bits;		/* bits available for vertex offset */
			t->vo_eo = t->we_es;				/* Entry offset in simplex table */
			t->wo_es = t->we_es + t->vo_es;		/* Total entry size for each vertex */
			t->sm_ts = (g->id + 1) * (t->we_es + t->vo_es) ;	/* Total size in bytes */

			f->weet = bigt;				/* Variable type for accessing weighting entry */
			f->voet = bigt;				/* Variable type for accessing vertex offset entry */
			f->wevt = nord(f, wit);		/* Variable type for holding weight value */
			f->vovt = nord(f, oft);		/* Variable type for holding offset value */

		} else {								/* We will combine the two entries */
			t->wo_xs = 0;
			t->wo_es = a->ords[combt].bits/8;	/* entry size in bytes for each entry */
			t->wo_ab = a->ords[combt].bits;		/* bits in weightig + offset */
			t->we_ab = f->wemnb;				/* bits available for weighting */
			t->vo_ab = t->wo_ab - t->we_ab;		/* Allow all spare bits to vertex offset */
			t->wo_eo = 0;						/* entry offset in simplex table */
			t->sm_ts = (g->id + 1) * t->wo_es;	/* Total size in bytes */

			f->woet = combt;			/* Variable type for accessing combined entry */
			f->wovt = nord(f, combt);	/* Variable type holding weight/offset read value */

			if ((f->wevt = findnord(f, t->we_ab)) < 0) {
				fprintf(stderr,"Can't find variable size to hold weighting factor\n");
				exit(-1);
			}
			if ((f->vovt = findnord(f, t->vo_ab)) < 0) {
				fprintf(stderr,"Can't find variable size to hold vertex offset index\n");
				exit(-1);
			}
		}

		/* Compute the number of bits needed to hold an index into */
		/* the simplex table (index is in terms of table entry size). */
		/* This value is used to figure out the room needed in the input */
		/* table to accumulate the simplex cube base offset value. (SW_O macro) */
		f->sxmnb = calc_bits(g->id, g->stres);

#ifdef VERBOSE
		/* Summarise the simplex table arrangements */
		printf("\n");
		printf("Simplex table structure:\n");
		printf("  Minimum bits needed to index table %d\n", f->sxmnb);
		printf("  Total simplex entry size %d bytes to hold %d entries\n",t->sm_ts, g->id+1);
		if (t->wo_xs) {
			printf("  Separate entries for offset and weight\n");
			printf("  Weighting entry size %d bytes\n",t->we_es);
			printf("  Offset entry size %d bytes\n",t->vo_es);
		} else {
			printf("  Combined offset and weight entries in %d bytes\n",t->wo_es);
			printf("  Weighting entry size %d bits\n",t->we_ab);
			printf("  Offset entry size %d bits\n",t->vo_ab);
		}
		printf("  Vertex offset scale factor %d\n", t->vo_om);
#endif /* VERBOSE */

		/* We known how big the interpolation and simplex */
		/* tables indexes are going to be, so complete figuring out */
		/* how big the input table entries have to be. */
		if ((f->itet = findord(f, f->sxmnb + f->ixmnb)) >= 0) {/* size to read */
			int rem;						/* Remainder bits */

			t->it_xs = 0;					/* Combined simplex+interp index */

			t->it_ab = a->ords[f->itet].bits;	/* Bits in combined input entry */
			rem = t->it_ab - f->sxmnb - f->ixmnb;
			t->sx_ab = f->sxmnb + rem/2;		/* simplex index bits actually available */
			t->ix_ab = t->it_ab - t->sx_ab;		/* interp index bits actually available */
			t->it_ts = a->ords[f->itet].bits/8;	/* total size in bytes */
			f->itvt = nord(f, f->itet);			/* Variable type */

			if ((f->sxvt = findnord(f, t->sx_ab)) < 0) {
				fprintf(stderr,"Simplex index variable size can't be handled\n");
				exit(-1);
			}
			if ((f->ixvt = findnord(f, t->ix_ab)) < 0) {
				fprintf(stderr,"Interp index variable size can't be handled\n");
				exit(-1);
			}
		} else {						/* Separate entries */
			int bbits;					/* Largest number of bits needed */

			t->it_xs = 1;				/* Separate simplex+interp indexes */
			bbits = f->sxmnb > f->ixmnb ? f->sxmnb : f->ixmnb;

			/* Allocate same size for both so that total structure size is power of 2 */
			if ((f->sxet = f->ixet = findord(f, bbits)) < 0) {
				fprintf(stderr,"Interp/Simplex index entry size can't be handled\n");
				exit(-1);
			}

			t->sx_ab = a->ords[f->sxet].bits;		/* Actual bits available */
			t->sx_es = t->sx_ab/8;					/* Entry size in bytes */
			t->ix_ab = a->ords[f->ixet].bits;
			t->ix_es = t->sx_ab/8;
			t->it_ts = t->sx_es + t->ix_es;		/* total size in bytes */
			t->sx_eo = 0;						/* simplex index offset in bytes */
			t->ix_eo = t->sx_es;				/* interp. index offset in bytes */
			f->sxvt = nord(f, f->sxet);			/* Variable type */
			f->ixvt = nord(f, f->ixet);			/* Variable type */
		}

#ifdef VERBOSE
		/* Summarise the input table arrangements */
		printf("\n");
		printf("Input table structure:\n");
		if (t->it_ix) {
			printf("  Input table extracts value from read values\n");
		} else {
			printf("  Value extraction read values is explicit\n");
		}
		printf("  Input value re-ordering is [");
		for (e = 0; e < g->id; e++)
			printf("%s%d",e > 0 ? " " : "", t->it_map[e]);
		printf("]\n");
		printf("  Input table entry size = %d bytes\n",t->it_ts);
		if (t->it_xs) {
			printf("  Separate Interp. and Simplex index values\n");
			printf("  Interp. index is at offset %d, size %d bytes\n",t->ix_eo, t->ix_es);
			printf("  Simplex index is at offset %d, size %d bytes\n",t->sx_eo, t->sx_es);
		} else {
			printf("  Combined Interp. and Simplex index values\n");
			printf("  Values are size %d bytes\n",t->it_ts);
			printf("  Interp. index = %d bits\n",t->ix_ab);
			printf("  Simplex index = %d bits\n",t->sx_ab);
		}
#endif /* VERBOSE */
	}

	/* Figure out output table stuff */
	{
		/* A variable to hold the index into an output table */
		if ((f->otit = findord(f, g->prec)) < 0) {
			fprintf(stderr,"Can't find output table index size\n");
			exit(-1);
		}
		f->otit = nord(f,f->otit);				/* Make temp variable natural size */

		if (g->out.pint != 0)	/* Pixel interleaved */
			f->nop = 1;
		else
			f->nop = g->od;
	
		/* Figure out the output pointer types */
		f->otvt = 0;			/* Output table value type */
		for (e = 0; e < f->nop; e++) {
			if ((f->opt[e] = findord(f, g->out.bpch[e])) < 0) {
				fprintf(stderr,"Output channel size can't be handled\n");
				exit(-1);
			}
			if (f->opt[e] > f->otvt)
				f->otvt = f->opt[e];	/* Make value type big enough for any channel size */
		}
		t->ot_ts = a->ords[f->otvt].bits/8;	/* Output table entry size in bytes */

		/* Setup information on data placement in output table enries */
		for (e = 0; e < g->od; e++) {
			t->ot_off[e] = g->out.bov[e];		/* Transfer info from generation spec. */
			t->ot_bits[e] = g->out.bpv[e];
		}
	}

#ifdef VERBOSE
	/* Summarise the output table arrangements */
	printf("  Output value re-ordering is [");
	for (e = 0; e < g->od; e++)
		printf("%s%d",e > 0 ? " " : "", t->im_map[e]);
	printf("]\n");
	printf("\n");

	printf("Output table structure:\n");
	printf("  Entry size = %d bytes\n",t->ot_ts);
	printf("  Output value placement within each enry is:\n");
	for (e = 0; e < f->nop; e++) {
		printf("    %d: Offset %d bits, size %d bits\n", e, t->ot_off[e], t->ot_bits[e]);
	}
#endif /* VERBOSE */

	/* Compute the maximum interpolation table resolution we will be able to handle */
	{
		int res, ores;

		res = calc_res(g->id, t->ix_ab);
		ores = calc_ores(g->id, t->vo_ab, t->vo_om);
		f->ixmxres = res < ores ? res : ores;
	}

	/* Compute the maximum simplex table resolution we will be able to handle */
	if (t->sort) {
		f->sxmxres = 0;
	} else {
		f->sxmxres = calc_res(g->id, t->sx_ab);
	}

#ifdef VERBOSE
	printf("Emitting introductory code\n"); fflush(stdout);
#endif /* VERBOSE */

	/* Start of code generation */
	doheader(f);			/* Output the header comments */

	/* We need an include file */
	line(f,"#ifndef  IMDI_INCLUDED");
	line(f,"#include <memory.h>");
	line(f,"#include \"imdi_imp.h\"");
	line(f,"#define  IMDI_INCLUDED");
	line(f,"#endif  /* IMDI_INCLUDED */");
	cr(f);

	/* Declare our explicit pointer type */
	line(f,"#ifndef DEFINED_pointer");
	line(f,"#define DEFINED_pointer");
	line(f,"typedef unsigned char * pointer;");
	line(f,"#endif");
	cr(f);

	/* Declare our explicit structure access macros */

#ifdef VERBOSE
	printf("Declaring macros\n"); fflush(stdout);
#endif /* VERBOSE */

	/* Macros for accessing input table entries */
	if (t->sort) {
		if (t->it_xs) {
			line(f,"/* Input table interp. index */");
			line(f,"#define IT_IX(p, off) *((%s *)((p) + %d + (off) * %d))",
			     a->ords[f->ixet].name, t->ix_eo, t->it_ts);
			cr(f);
			if (t->wo_xs) {
				line(f,"/* Input table input weighting enty */");
				line(f,"#define IT_WE(p, off) *((%s *)((p) + %d + (off) * %d))",
				     a->ords[f->weet].name, t->we_eo, t->it_ts);
				cr(f);
				line(f,"/* Input table input offset value enty */");
				line(f,"#define IT_VO(p, off) *((%s *)((p) + %d + (off) * %d))",
				     a->ords[f->voet].name, t->vo_eo, t->it_ts);
				cr(f);
			} else {
				line(f,"/* Input table input weighting/offset value enty */");
				line(f,"#define IT_WO(p, off) *((%s *)((p) + %d + (off) * %d))",
				     a->ords[f->woet].name, t->wo_eo, t->it_ts);
				cr(f);
			}
		} else {
			line(f,"/* Input table interp index, weighting and vertex offset */");
			line(f,"#define IT_IT(p, off) *((%s *)((p) + %d + (off) * %d))",
			     a->ords[f->itet].name, 0, t->it_ts);
			cr(f);
		}

		/* Macro to conditionally exchange two varibles */
		/* Doing this in place using an xor seems to be fastest */
		/* on most architectures. */
		line(f,"/* Conditional exchange for sorting */");
		if (t->wo_xs) {
			line(f,"#define CEX(A, AA, B, BB) if (A < B) { \\");
			line(f,"            A ^= B; B ^= A; A ^= B; AA ^= BB; BB ^= AA; AA ^= BB; }");
		} else
			line(f,"#define CEX(A, B) if (A < B) { A ^= B; B ^= A; A ^= B; }");
		cr(f);

	} else {	/* Simplex table */
		if (t->it_xs) {
			line(f,"/* Input table interp. index */");
			line(f,"#define IT_IX(p, off) *((%s *)((p) + %d + (off) * %d))",
			     a->ords[f->ixet].name, t->ix_eo, t->it_ts);
			cr(f);
			line(f,"/* Input table simplex index enty */");
			line(f,"#define IT_SX(p, off) *((%s *)((p) + %d + (off) * %d))",
			     a->ords[f->sxet].name, t->sx_eo, t->it_ts);
			cr(f);
		} else {
			line(f,"/* Input table inter & simplex indexes */");
			line(f,"#define IT_IT(p, off) *((%s *)((p) + %d + (off) * %d))",
			     a->ords[f->itet].name, 0, t->it_ts);
			cr(f);
		}
	}

	if (!t->sort) {
		/* Macro for computing a simplex table entry */
		line(f,"/* Simplex weighting table access */");
		line(f,"#define SW_O(off) ((off) * %d)", t->sm_ts);
		cr(f);

		/* Macros for accessing the contents of the simplex table */
		if (t->wo_xs) { 			/* If separate */
			line(f,"/* Simplex table - get weighting value */");
			line(f,"#define SX_WE(p, v) *((%s *)((p) + (v) * %d + %d))",
			     a->ords[f->weet].name, t->wo_es, t->we_eo);
			cr(f);

			line(f,"/* Simplex table - get offset value */");
			line(f,"#define SX_VO(p, v) *((%s *)((p) + (v) * %d + %d))",
			     a->ords[f->voet].name, t->wo_es, t->vo_eo);
			cr(f);

		} else {	/* Combined */
			line(f,"/* Simplex table - get weighting/offset value */");
			line(f,"#define SX_WO(p, v) *((%s *)((p) + (v) * %d))",
			     a->ords[f->woet].name, t->wo_es);
			cr(f);
		}
	}

	/* Macro for computing an interpolation table entry */
	line(f,"/* Interpolation multi-dim. table access */");
	line(f,"#define IM_O(off) ((off) * %d)", t->im_ts);
	cr(f);

	/* Macro for accessing an entry in the interpolation table */
	line(f,"/* Interpolation table - get vertex values */");

	if (t->im_fn > 0) {
		/* Arguments to macro are cell base address, vertex offset, data offset */

		if (f->imfvt == f->iafvt) {	/* Table and accumulator are the same size */
			if (!timp || t->im_fn == 1) 
				line(f,"#define IM_FE(p, v, c) *((%s *)((p) + (v) * %d + (c) * %d))",
				     a->ords[f->imfvt].name, t->im_oc, t->im_fs);
			else {
				line(f,"#define IM_TP(p, v) ((p) + (v) * %d)", t->im_oc);
				line(f,"#define IM_FE(p, c) *((%s *)((p) + (c) * %d))",
				     a->ords[f->imfvt].name, t->im_fs);
			}
		} else {					/* Expand single table entry to accumulator size */
			if (!timp || t->im_fn == 1) 
				line(f,"#define IM_FE(p, v, c) ((%s)*((%s *)((p) + (v) * %d + (c) * %d)))",
				     a->ords[f->iafvt].name,
				     a->ords[f->imfvt].name, t->im_oc, t->im_fs);
			else {
				line(f,"#define IM_TP(p, v) ((p) + (v) * %d)", t->im_oc);
				line(f,"#define IM_FE(p, c) ((%s)*((%s *)((p) + (c) * %d)))",
				     a->ords[f->iafvt].name,
				     a->ords[f->imfvt].name, t->im_fs);
			}
		}
	}
	if (t->im_pn > 0) {
		/* Arguments to macro are cell base address, vertex offset */
		/* There is no data offset since there can be only be one partial entry */

		if (f->imfvt == f->iafvt)	/* Table and accumulator are the same size */
			line(f,"#define IM_PE(p, v) *((%s *)((p) + %d + (v) * %d))",
			     a->ords[f->impvt].name, t->im_fn * t->im_fs, t->im_oc);
		else						/* Expand single table entry to accumulator size */
			line(f,"#define IM_PE(p, v) ((%s)*((%s *)((p) + %d + (v) * %d)))",
			     a->ords[f->iafvt].name,
			     a->ords[f->impvt].name, t->im_fn * t->im_fs, t->im_oc);
	}
	cr(f);

	/* Macro for accessing an output table entry */
	line(f,"/* Output table indexes */");
	line(f,"#define OT_E(p, off) *((%s *)((p) + (off) * %d))",
	     a->ords[f->otvt].name, t->ot_ts);
	cr(f);

	/* =============================================== */

#ifdef VERBOSE
	printf("Starting interpolation function\n"); fflush(stdout);
#endif /* VERBOSE */

	/* Declare the function */
	line(f,"void");
	line(f, "imdi_k%d(",index);
	line(f, "imdi *s,			/* imdi context */");
	line(f, "void **outp,		/* pointer to output pointers */");
	line(f, "void **inp,		/* pointer to input pointers */");
	line(f, "unsigned int npix	/* Number of pixels to process */");
	line(f, ") {");
	inc(f);

	/* We need access to the imdi_imp */
	line(f, "imdi_imp *p = (imdi_imp *)(s->impl);");

	/* Declare the input pointers and init them */
	for (e = 0; e < f->nip; e++) {
		line(f, "%s *ip%d = (%s *)inp[%d];",
		     a->ords[f->ipt[e]].name, e, a->ords[f->ipt[e]].name, e);
	}

	/* Declare the output pointers and init them */
	for (e = 0; e < f->nop; e++) {
		line(f, "%s *op%d = (%s *)outp[%d];",
		     a->ords[f->opt[e]].name, e, a->ords[f->opt[e]].name, e);
	}

	/* Declare and intialise the end pointer */
	line(f, "%s *ep = ip0 + npix * %d ;",
	        a->ords[f->ipt[0]].name, g->in.chi[0]);

	/* Declare and initialise the input table pointers */
	for (e = 0; e < g->id; e++)
		line(f,"pointer it%d = (pointer)p->in_tables[%d];",e,e);

	/* Declare and initialise the output table pointers */
	for (e = 0; e < g->od; e++)
		line(f,"pointer ot%d = (pointer)p->out_tables[%d];",e,e);

	if (!t->sort) {
		/* Declare and initialise the Simplex weighting base pointer */
		line(f,"pointer sw_base = (pointer)p->sw_table;");
	}

	/* Declare and initialise the Interpolation multidim base pointer */
	line(f,"pointer im_base = (pointer)p->im_table;");

	/* Figure out whether input channel reads can be used directly as table offsets */
	t->it_ix = 1;				/* Default use input table lookup to extract value */

	if (g->in.packed != 0)
		t->it_ix = 0;				/* Extract will be done explicitly */

	for (e = 0; e < g->id; e++) {
		int ee = (g->in.pint != 0) ? 0 : e;		/* bpch index */

		if ((g->in.bov[e] + g->in.bpv[e]) <= 12)
			continue;							/* Table can do extract */

		if (g->in.bov[e] != 0 || g->in.bpv[e] != g->in.bpch[ee]) {
			t->it_ix = 0;						/* Extract will be done explicitly */
			break;
		}
	}
	
	/* ------------------------------- */
#ifdef VERBOSE
	printf("Starting pixel processing loop\n"); fflush(stdout);
#endif /* VERBOSE */

	/* Start the pixel processing loop */
	cr(f);
	sline(f, "for(;ip0 < ep;");
	for (e = 0; e < f->nip; e++)
		mline(f, " ip%d += %d,", e, g->in.chi[e]);
	for (e = 0; e < f->nop; e++)
		mline(f, " op%d += %d%s", e, g->out.chi[e], ((e+1) < f->nop) ? "," : "");
	eline(f, ") {");
	inc(f);

	/* Declare output value accumulator(s) */
	for (i = 0; i < t->im_fn; i++) {
		line(f,"%s ova%d;	/* Output value accumulator */",a->ords[f->iafvt].name,i);
	}
	for (; i < f->ian; i++) {
		line(f,"%s ova%d;	/* Output value partial accumulator */",a->ords[f->iapvt].name,i);
	}

	/* Context around interp/Simplex table lookup */
	line(f, "{");
	inc(f);

	if (!t->sort)
		line(f,"pointer swp;");		/* Declare Simplex weighting pointer */
	line(f,"pointer imp;");			/* Declare Interpolation multidim pointer */

	/* Declare the input weighting/vertex offset variables */
	if (t->sort) {
		for (e = 0; e < g->id; e++) {
			if (t->wo_xs) {
				line(f,"%s we%d;	/* Weighting value variable */",
				       a->ords[f->wevt].name, e);
				line(f,"%s vo%d;	/* Vertex offset variable */",
				       a->ords[f->vovt].name, e);
			} else {
				line(f,"%s wo%d;	/* Weighting value and vertex offset variable */",
				       a->ords[f->wovt].name, e);
			}
		}
	}

	/* Context around input table processing */
	line(f, "{");
	inc(f);

	/* Declare the table index variables/input weighting/vertex offset variables */
	if (t->sort) {
		if (!t->it_xs)
			line(f,"%s ti;		/* Input table entry variable */",a->ords[f->itvt].name);
		line(f,"%s ti_i;	/* Interpolation index variable */",a->ords[f->ixvt].name);
	} else {
		if (t->it_xs) {
			line(f,"%s ti_s;	/* Simplex index variable */",a->ords[f->sxvt].name);
			line(f,"%s ti_i;	/* Interpolation index variable */",a->ords[f->ixvt].name);
		} else {
			line(f,"%s ti;	/* Simplex+Interpolation index variable */",a->ords[f->itvt].name);
		}
	}

	if (g->in.packed != 0)	/* We need to unpack from a single read */
		line(f,"%s rdv;		/* Read value */",a->ords[f->ipt[0]].name);

	if (t->it_ix == 0) {
		int bv = 0;
		for (e = 0; e < f->nip; e++) {	/* Find largest input type */
			if (f->ipt[e] > bv)
				bv = f->ipt[e];
		}
		bv = nord(f, bv);
		line(f,"%s chv;	/* Channel value */",a->ords[bv].name);
		f->chv_bits = a->ords[bv].bits;
	}
	cr(f);

#ifdef VERBOSE
	printf("Read code\n"); fflush(stdout);
#endif /* VERBOSE */

	/* For all the input channels */
	for (e = 0; e < g->id; e++) {
		char rde[50];		/* Read expression */
		char toff[50];		/* Table offset expression */
		int ee = (g->in.pint != 0) ? 0 : e;		/* bpch index */

		if (g->in.pint != 0) 	/* Pixel interleaved */
			sprintf(rde,"ip0[%d]",e);	/* Offset from single pointer */
		else
			sprintf(rde,"*ip%d",e);		/* Pointer per channel */

		if (g->in.packed != 0) {
			if (e == 0)
				line(f,"rdv = %s;",rde);	/* Do single read */
			sprintf(rde,"rdv");				/* Use read value for extraction */
		}

		if (t->it_ix == 0) {
			if (g->in.bov[e] == 0 ) {				/* No offset */
				if (g->in.bpv[e] == g->in.bpch[ee])	/* No mask */
					line(f,"chv = %s;",rde);
				else								/* Just mask  */
					line(f,"chv = (%s & %s);",rde, hmask(g->in.bpv[e]));
			} else {								/* Offset */
				if ((g->in.bov[e] + g->in.bpv[e]) == g->in.bpch[ee])
					line(f,"chv = (%s >> %d);",rde, g->in.bov[e]);
				else {								/* Offset and mask */
					if (a->shfm || g->in.bpv[e] > 32) {
						/* Extract using just shifts */
						line(f,"chv = ((%s << %d) >> %d);", rde,
						        f->chv_bits - g->in.bpv[e] - g->in.bov[e],
						        f->chv_bits - g->in.bpv[e]);
					} else {
						/* Extract using shift and mask */
						line(f,"chv = ((%s >> %d) & %s);",
						        rde, g->in.bov[e], hmask(g->in.bpv[e]));
					}
				}
			}
			sprintf(toff,"chv");
		} else {									/* No extraction */
			sprintf(toff,"%s",rde);
		}

		if (t->sort) {
			if (t->it_xs) {
				line(f,"ti_i %s= IT_IX(it%d, %s);", e ? "+" : " ", e, toff);
				if (t->wo_xs) {
					line(f,"we%d   = IT_WE(it%d, %s);", e, e, toff);
					line(f,"vo%d   = IT_VO(it%d, %s);", e, e, toff);
				} else {
					line(f,"wo%d   = IT_WO(it%d, %s);", e, e, toff);
				}
			} else {	/* All three combined */
				line(f,"ti = IT_IT(it%d, %s);", e, toff);
				if (a->shfm || t->wo_ab > 32) {
					/* Extract using just shifts */
					line(f,"wo%d   = ((ti << %d) >> %d);	"
					     "/* Extract weighting/vertex offset value */",
					     e, a->ords[f->wovt].bits - t->wo_ab, a->ords[f->wovt].bits - t->wo_ab);
					line(f,"ti_i %s= (ti >> %d);	"
					     "/* Extract interpolation table value */",
					     e ? "+" : " ", t->wo_ab);
				} else {
					/* Extract using shift and mask */
					line(f,"wo%d   = (ti & %s);	"
					     "/* Extract weighting/vertex offset value */",
					     e, hmask(t->wo_ab));
					line(f,"ti_i %s= (ti >> %d);	"
					     "/* Extract interpolation table value */",
					     e ? "+" : " ", t->wo_ab);
				}
			}

		} else {	/* Simplex */
			if (t->it_xs) {
				/* ~~~~ should toff be forced to be a temp variable ?? */
				/* (ie. force use of rde (above) if t->it_xs is nonz) */ 
				line(f,"ti_i %s= IT_IX(it%d, %s);", e ? "+" : " ", e, toff);
				line(f,"ti_s %s= IT_SX(it%d, %s);", e ? "+" : " ", e, toff);
			} else {
				line(f,"ti %s= IT_IT(it%d, %s);", e ? "+" : " ", e, toff);
			}
		}
	}

#ifdef VERBOSE
	printf("Index extraction code\n"); fflush(stdout);
#endif /* VERBOSE */

	cr(f);

	if (t->sort) {
		/* Extract Simplex and Interpolation indexes from accumulator */
		line(f,"imp = im_base + IM_O(ti_i);		/* Compute interp. table entry pointer */");
	} else {
		if (t->it_xs) {		/* Extract Simplex and Interpolation indexes from accumulator */
			line(f,"swp = sw_base + SW_O(ti_s);		/* Compute simplex table entry pointer */");
			line(f,"imp = im_base + IM_O(ti_i);		/* Compute interp. table entry pointer */");
		} else {
			line(f,"imp = im_base + IM_O(ti >> %d);		"
			     "/* Extract interp. index and comp. entry */",
			     t->sx_ab);
			if (a->shfm || t->sx_ab > 32) {
				/* Extract using just shifts */
				line(f,"swp = sw_base + SW_O((ti << %d) >> %d);	"
				     "/* Extract simplex index & comp. entry */",
				     a->ords[f->itvt].bits - t->sx_ab, a->ords[f->itvt].bits - t->sx_ab);
			} else {
				/* Extract using shift and mask */
				line(f,"swp = sw_base + SW_O(ti & %s);	"
				     "/* Extract simplex index and comp. entry */",
				     hmask(t->sx_ab));
			}
		}
	}

	/* Do the explicit sort now */
	if (t->sort) {
		cr(f);
		/* Sort from largest to smallest using a selection sort */
		/* Use simple sequence for the moment. More elaborate sequence */
		/* may allow other optimisations. */
		line(f,"/* Sort weighting values and vertex offset values */");
		for (i = 0; i < (g->id-1); i++) {
			for (e = i+1; e < g->id; e++) {
				if (t->wo_xs)
					line(f,"CEX(we%d, vo%d, we%d, vo%d);",i,i,e,e);
				else
					line(f,"CEX(wo%d, wo%d);",i,e);
			}
		}
	}

	/* End of input table processing context */
	dec(f);
	line(f,"}");

	line(f,"{");	/* Context around vertex lookup and accumulation */
	inc(f);

	/* Declare vertex offset and weight variables */
	if (t->sort && t->wo_xs == 0) {
		line(f,"%s nvof;	/* Next vertex offset value */",a->ords[f->vovt].name);
	} else {
		if (!t->wo_xs)	/* If combined in table */
			line(f,"%s vowr;	/* Vertex offset/weight value */",a->ords[f->wovt].name);
	}
	line(f,"%s vof;	/* Vertex offset value */",a->ords[f->vovt].name);
	line(f,"%s vwe;	/* Vertex weighting */",a->ords[f->wevt].name);
	if (timp && t->im_fn > 1) 
		line(f,"pointer timp;		/* Temporary interpolation table pointer */");
	cr(f);

#ifdef VERBOSE
	printf("Vertex offset and weight code\n"); fflush(stdout);
#endif /* VERBOSE */

	/* For each vertex in the simplex */
	for (e = 0; e < (g->id +1); e++) {

		if (t->sort) {

			if (e == 0) {
				line(f,"vof = 0;				/* First vertex offset is 0 */");
			} else {
				if (t->wo_xs)
					line(f,"vof += vo%d;			/* Move to next vertex */",e-1);
				else
					line(f,"vof += nvof;			/* Move to next vertex */");
			}

			/* Extract the vertex offset and weight values from the sorted input values */
			if (e < g->id && !t->wo_xs) {
				if (a->shfm || t->vo_ab > 32) {
					/* Extract using just shifts */
					line(f,"nvof = ((wo%d << %d) >> %d);	"
					     "/* Extract offset value */",
					     e, a->ords[f->vovt].bits - t->vo_ab, a->ords[f->vovt].bits - t->vo_ab);
					line(f,"wo%d = (wo%d >> %d);	"
					     "	/* Extract weighting value */",
					     e, e, t->vo_ab);
				} else {
					/* Extract using shift and mask */
					line(f,"nvof = (wo%d & %s);	"
					     "/* Extract offset value */",
					     e, hmask(t->vo_ab));
					line(f,"wo%d = (wo%d >> %d);	"
					     "	/* Extract weighting value */",
					     e, e, t->vo_ab);
				}
			}
			/* Compute the weighting value */
			if (!t->wo_xs) {
				if (e == 0) {
					line(f,"vwe = %d - wo%d;		/* Baricentric weighting */", 1 << g->prec, e);
				} else if (e < g->id) {
					line(f,"vwe = wo%d - wo%d;		/* Baricentric weighting */", e-1, e);
				} else {
					line(f,"vwe = wo%d;				/* Baricentric weighting */", e-1);
				}
			} else {
				if (e == 0) {
					line(f,"vwe = %d - we%d;		/* Baricentric weighting */", 1 << g->prec, e);
				} else if (e < g->id) {
					line(f,"vwe = we%d - we%d;		/* Baricentric weighting */", e-1, e);
				} else {
					line(f,"vwe = we%d;				/* Baricentric weighting */", e-1);
				}
			}

		} else {	/* Not sort */
			/* Read the vertex offset and weight values from the simplex table */
			if (t->wo_xs) { 			/* If separate */
				line(f,"vof = SX_VO(swp, %d);	/* Read vertex offset value */", e);
				line(f,"vwe = SX_WE(swp, %d);	/* Read vertex weighting value */", e);
			} else { 			/* If combined in table */
				line(f,"vowr = SX_WO(swp, %d);	/* Read vertex offset+weighting values */", e);
				if (a->shfm || t->vo_ab > 32) {
					/* Extract using just shifts */
					line(f,"vof = ((vowr << %d) >> %d);	"
					     "/* Extract offset value */",
					     a->ords[f->vovt].bits - t->vo_ab, a->ords[f->vovt].bits - t->vo_ab);
					line(f,"vwe = (vowr >> %d);	"
					     "/* Extract weighting value */",
					     t->vo_ab);
				} else {
					/* Extract using shift and mask */
					line(f,"vof = (vowr & %s);	"
					     "/* Extract offset value */",
					     hmask(t->vo_ab));
					line(f,"vwe = (vowr >> %d);	"
					     "/* Extract weighting value */",
					     t->vo_ab);
				}
			}
		}

		/* Lookup the vertex value, weight it, and accumulate it into output value */
		if (timp && t->im_fn > 1) 
			line(f,"timp = IM_TP(imp, vof);	/* Vertex address */");
		for (i = 0; i < f->ian; i++) {		/* For each output accumulation chunk */
			if (i < t->im_fn) { 	/* Full entry */
				if (!timp || t->im_fn == 1) 
					line(f,"ova%d %s= IM_FE(imp, vof, %d) * vwe;	"
					     "/* Accumulate weighted output values */",
					     i, e ? "+" : " ", i);
				else
					line(f,"ova%d %s= IM_FE(timp, %d) * vwe;	"
					     "/* Accumulate weighted output values */",
					     i, e ? "+" : " ", i);
			} else				/* One partial entry */
				line(f,"ova%d %s= IM_PE(imp, vof) * vwe;	"
				     "/* Accumulate last weighted output values */",
				     i, e ? "+" : " ");
		}
	}

	dec(f);
	line(f, "}"); 	/* End of output value lookup context */

	dec(f);
	line(f, "}"); 	/* End of output value accumulation context */

	/* Start of output lookup and write */
	line(f,"{");
	inc(f);

#ifdef VERBOSE
	printf("Output table code\n"); fflush(stdout);
#endif /* VERBOSE */

	{
		char wre[50];		/* Write destination expression */

		if (g->out.packed != 0)	/* We need to pack results into a single write */
			line(f,"%s wrv;		/* Write value */",a->ords[f->ipt[0]].name);
	
		/* Declare temporary to hold index into output lookup table */
		line(f,"%s oti;	/* Vertex offset value */",a->ords[f->otit].name);
	
		/* For each accumulator value */
		/* (Assume they are in output order for the moment ?) */
		for (e = i = 0; i < f->ian; i++) {		/* For each output accumulation chunk */
			int vpa = i < t->im_fn ? t->im_fv : t->im_pv;		/* Chanel values per accumulator */
			int oat = i < t->im_fn ? f->iafvt : f->iapvt;		/* Output accumulator type */
			int ee;		/* Relative e to this accumulator */
	
			/* For each output value in this accumulator */
			for (ee = 0; ee < vpa && e < g->od; ee++, e++) {
				int off, size;		/* Bits to be extracted */
		
				/* Extract wanted 8 bits from the 8.8 bit result in accumulator */
				off = ee * f->iaovb + (f->iaovb - g->prec);	
				size = g->prec;
	
				if (e == 0 || g->out.packed == 0) {
					if (g->out.pint != 0) 	/* Pixel interleaved */
						sprintf(wre,"op0[%d]",e);	/* Offset from single pointer */
					else
						sprintf(wre,"*op%d",e);		/* Pointer per channel */
				}
	
				if (a->shfm || size > 32) {
					/* Extract using just shifts */
					line(f,"oti = ((ova%d << %d) >> %d);	"
					     "/* Extract integer part of result */",
					     i, a->ords[oat].bits - off - size, a->ords[oat].bits - size);
				} else {
					/* Extract using shift and mask */
					line(f,"oti = ((ova%d >> %d) & %s);	"
					     "/* Extract integer part of result */",
					     i, off, hmask(size));
				}
	
				/* Lookup in output table and write to destination */
				if (g->out.packed != 0) {
					line(f,"wrv %s= OT_E(ot%d, oti);", e ? "+" : "", e);
				} else {
					line(f,"%s = OT_E(ot%d, oti);	/* Write result */", wre, e);
				}
			}
		}
	
		if (g->out.packed != 0) {	/* Write out the accumulated value */
			line(f,"%s = wrv;	/* Write result */", wre);
		}
	}

	/* The end of the output lookup and write */
	dec(f);
	line(f, "}");

	/* The end of the pixel processing loop */
	dec(f);
	line(f, "}");

	/* The end of the function */
	dec(f);
	line(f, "}");

	/* Undefine all the macros */
	if (t->sort) {
		if (t->it_xs) {
			if (t->wo_xs) {
				line(f,"#undef IT_WE");
				line(f,"#undef IT_VO");
			} else
				line(f,"#undef IT_WO");
			line(f,"#undef IT_IX");
		} else {
			line(f,"#undef IT_IT");
		}
		line(f,"#undef CEX");
	} else {
		if (t->it_xs) {
			line(f,"#undef IT_IX");
			line(f,"#undef IT_SX");
		} else {
			line(f,"#undef IT_IT");
		}

		line(f,"#undef SW_O");
		if (t->wo_xs) {
			line(f,"#undef SX_WE");
			line(f,"#undef SX_VO");
		} else {
			line(f,"#undef SX_WO");
		}
	}
	line(f,"#undef IM_O");
	if (t->im_fn > 0) {
		if (timp && t->im_fn > 1) 
			line(f,"#undef IM_TP");
		line(f,"#undef IM_FE");
	}
	if (t->im_pn > 0) {
		line(f,"#undef IM_PE");
	}
	line(f,"#undef OT_E");

	/* =============================================== */
#ifdef VERBOSE
	printf("Done interpolation code\n"); fflush(stdout);
#endif /* VERBOSE */

	/* =============================================== */

	{
		int sog = sizeof(genspec);	/* Size of genspec structure */
		unsigned char *dp = (unsigned char *)g;

		int s_stres, s_itres;	/* Save values */

		s_stres = g->stres;
		s_itres = g->itres;
		g->stres = f->sxmxres;		/* Set maximum values */
		g->itres = f->ixmxres;
		
		/* Declare the generation structure data function */
		cr(f);
		line(f,"void");
		line(f, "imdi_k%d_gen(",index);
		line(f, "genspec *g			/* structure to be initialised */");
		line(f, ") {");
		inc(f);
	
		/* Declare the genspec initialisation data */
		line(f, "static unsigned char data[] = {");
		inc(f);
		for (i = 0; i < sog; i++) {
			if ((i & 7) == 0)
				sline(f,"");
			mline(f, "0x%02x%s ", dp[i], (i+1) < sog ? "," : "", dp[i]);
			if ((i & 7) == 7 || (i+1) == sog)
				eline(f,"");
		}
		dec(f);
		line(f, "};	/* Structure image */");

		cr(f);
		line(f, "memcpy(g, data, sizeof(data));	/* Initialise the structure */");
		/* The end of the function */
		dec(f);
		line(f, "}");

		g->stres = s_stres;		/* Restore entry values */
		g->itres = s_itres;
	}

	/* =============================================== */

	{
		int sot = sizeof(tabspec);	/* Size of tabspec structure */
		unsigned char *dp = (unsigned char *)t;

		/* Declare the generation structure data function */
		cr(f);
		line(f,"void");
		line(f, "imdi_k%d_tab(",index);
		line(f, "tabspec *t			/* structure to be initialised */");
		line(f, ") {");
		inc(f);
	
		/* Declare the genspec initialisation data */
		line(f, "static unsigned char data[] = {");
		inc(f);
		for (i = 0; i < sot; i++) {
			if ((i & 7) == 0)
				sline(f,"");
			mline(f, "0x%02x%s ", dp[i], (i+1) < sot ? "," : "", dp[i]);
			if ((i & 7) == 7 || (i+1) == sot)
				eline(f,"");
		}
		dec(f);
		line(f, "};	/* Structure image */");

		cr(f);
		line(f, "memcpy(t, data, sizeof(data));	/* Initialise the structure */");
		/* The end of the function */
		dec(f);
		line(f, "}");
	}

	/* =============================================== */

	cr(f); cr(f); cr(f); cr(f); cr(f); cr(f);

	return 0;
}


/* Return bits needed to store index into table of */
/* given resolution and dimensionality. */
static int
calc_bits(
int dim,
int res) {

	return ceil(log((double)res) * (double)dim/log(2.0) - 1e-14);
}

/* Return maximum resolution possible given dimensionality */
/* and number of index bits. */
static int
calc_res(
int dim,
int bits) {
	double fres;

	fres = log(2.0) * (double)bits/(double)dim;
	if (fres > 12 || (fres = exp(fres)) > 65536.0)
		fres = 65536.0;		/* Limit to a sane value */
	return (int)(fres + 1e-14);
}

/* Return bits needed to store a relative offset of 1, */
/* into a table of given resolution, dimensionality , and */
/* entry size. */
static int
calc_obits(
int dim,
int res,
int esize) {
	double off;		/* Maximum diagonal offset value */
	int bits;

	if (res == 0 || res == 1)
		return 0;
	if (dim == 1)
		off = esize;
	else {
		off = (double)esize * floor(exp(log((double)res) * dim - log(res-1.0)));
	}

	bits = ceil(log(off)/log(2.0) - 1e-14);
	return bits;
}

/* Return maximum resolution possible given dimensionality */
/* number of index bits, and entry size */
static int
calc_ores(
int dim,
int bits,
int esize) {
	int res;

	/* Find resolution. Stop at arbitrary 65536 */
	for (res = 1; res < 65537; res++) {
		int bn;
		bn = calc_obits(dim, res, esize);
		if (bn > bits) {
			return res-1;
		}
	}
	return res-1;
}



/* Output the introductory comments */
static void
doheader(
	fileo *f
) {
	genspec *g = f->g;
	tabspec *t = f->t;
	mach_arch *a  = f->a;
	int e;

	/* - - - - - - - - - - - - */
	/* Output file title block */
	line(f,"/* Integer Multi-Dimensional Interpolation */");
	line(f,"/* Interpolation Kernel Code */");
	line(f,"/* Generated by cgen */");
	line(f,"/* Copyright 2000 - 2002 Graeme W. Gill */");
	line(f,"/* This material is licenced under the GNU GENERAL PUBLIC LICENCE :- */\n");
	line(f,"/* see the Licence.txt file for licencing details.*/\n");
	cr(f);

	/* - - - - - - - - - - - - */
	/* Output the specification */
	line(f,"/*");
	line(f,"   Interpolation kernel specs:");
	cr(f);
	line(f,"   Input channels per pixel = %d",g->id);
	for (e = 0; e < g->id; e++) {
		line(f,"   Input channel %d bits = %d",e, g->in.bpch[e]);
		line(f,"   Input channel %d increment = %d",e, g->in.chi[e]);
	}
	if (g->in.pint != 0)
		line(f,"   Input is channel interleaved");
	else
		line(f,"   Input is plane interleaved");

	if (g->in.packed != 0)
		line(f,"   Input channels are packed into one word");
	else
		line(f,"   Input channels are separate words");

	if (t->it_ix)
		line(f,"   Input value extraction is done in input table lookup");
	cr(f);
	
	line(f,"   Output channels per pixel = %d",g->od);
	for (e = 0; e < g->od; e++) {
		line(f,"   Output channel %d bits = %d",e, g->out.bpch[e]);
		line(f,"   Output channel %d increment = %d",e, g->out.chi[e]);
	}
	if (g->out.pint != 0)
		line(f,"   Output is channel interleaved");
	else
		line(f,"   Output is plane interleaved");
	cr(f);
	if (g->out.packed != 0)
		line(f,"   Output channels are packed into one word");
	else
		line(f,"   Output channels are separate words");

	
	if (t->sort)
		line(f,"   Weight+voffset bits       = %d",t->sx_ab);
	else
		line(f,"   Simplex table index bits       = %d",t->sx_ab);
	line(f,"   Interpolation table index bits = %d",t->ix_ab);
	if (!t->sort)
		line(f,"   Simplex table max resolution = %d",f->sxmxres);
	line(f,"   Interpolation table max resolution = %d",f->ixmxres);
	line(f," */");
	cr(f);

	/* - - - - - - - - - - - - */
	line(f,"/*");
	line(f,"   Machine architecture specs:");
	cr(f);
	if (a->bigend != 0)
		line(f,"   Big Endian");
	else
		line(f,"   Little endian");

	if (a->uwa != 0)
		line(f,"   Using maximum sized memory accesses where possible");
	else
		line(f,"   Reading and writing pixel values separately");

	line(f,"   Pointer size = %d bits",a->pbits);
	cr(f);

	for (e = 0; e < a->nords; e++) {
		line(f,"   Ordinal size %2d bits is known as '%s'",
		        a->ords[e].bits,a->ords[e].name);
	}
	line(f,"   Natural ordinal is '%s'", a->ords[a->natord].name);
	cr(f);

	for (e = 0; e < a->nints; e++) {
		line(f,"   Integer size %2d bits is known as '%s'",
		        a->ints[e].bits,a->ints[e].name);
	}
	line(f,"   Natural integer is '%s'", a->ints[a->natint].name);
	cr(f);

	line(f," */");
	cr(f);
}


/* ---------------------------------------- */
/* Architecture support */
/* Find an ordinal with at least bits size */
/* Return -1 if failed */
int findord(
fileo *f,
int bits
) {
	mach_arch *a  = f->a;
	int i;

	for (i = 0; i < a->nords; i++) {
		if (a->ords[i].bits >= bits)
			return i;
	}
	return -1;
}

/* Round ordinal type up to natural size */
int nord(
	fileo *f,
	int ov
) {
	if (ov >= 0 && ov < f->a->natord)
		ov = f->a->natord;
	return ov;
}

/* Find an ordinal with at least bits size, */
/* or natural size, whichever is greater. */
/* Return -1 if failed */
int findnord(
	fileo *f,
	int bits
) {
	int ov;

	ov = findord(f, bits);
	ov = nord(f, ov);
	return ov;
}

/* Find an integer with at least bits size */
/* Return -1 if failed */
int findint(
	fileo *f,
	int bits
) {
	mach_arch *a  = f->a;
	int i;

	for (i = 0; i < a->nints; i++) {
		if (a->ints[i].bits >= bits)
			return i;
	}
	return -1;
}

/* Round integer type up to natural size */
int nint(
	fileo *f,
	int iv
) {
	if (iv >= 0 && iv < f->a->natint)
		iv = f->a->natint;
	return iv;
}

/* Find an interger with at least bits size, */
/* or natural size, whichever is greater. */
/* Return -1 if failed */
int findnint(
	fileo *f,
	int bits
) {
	int iv;

	iv = findint(f, bits);
	iv = nint(f, iv);
	return iv;
}


/* ------------------------------------ */
/* File output support */

/* Output a line to the file (including trailing \n) */
void
line(fileo *f, char *fmt, ...)
{
	int i;
	va_list args;

	/* Indent to the correct level */
	for (i = 0; i < f->indt; i++)
		fprintf(f->of,"	");
	
	va_start(args, fmt);
	vfprintf(f->of, fmt, args);
	va_end(args);
	fprintf(f->of, "\n");
}

/* Output the start of a line to the file) */
void
sline(fileo *f, char *fmt, ...)
{
	int i;
	va_list args;

	/* Indent to the correct level */
	for (i = 0; i < f->indt; i++)
		fprintf(f->of,"	");
	
	va_start(args, fmt);
	vfprintf(f->of, fmt, args);
	va_end(args);
}

/* Output the middle of a line to the file) */
void
mline(fileo *f, char *fmt, ...)
{
	int i;
	va_list args;

	va_start(args, fmt);
	vfprintf(f->of, fmt, args);
	va_end(args);
}

/* Output the end of a line to the file (including trailing \n) */
void
eline(fileo *f, char *fmt, ...)
{
	int i;
	va_list args;

	va_start(args, fmt);
	vfprintf(f->of, fmt, args);
	va_end(args);
	fprintf(f->of, "\n");
}
/* ------------------------------------ */




