
/* Integer Multi-Dimensional Interpolation */
/*
 * Copyright 2000 - 2002 Graeme W. Gill
 * All rights reserved.
 *
 * This material is licenced under the GNU GENERAL PUBLIC LICENCE :-
 * see the Licence.txt file for licencing details.
 */

/* Run time table allocater and initialiser */

/*
 * The function here that knows how to create the
 * appropriate run time tables for our chosen kernel,
 * and the color mapping we want to perform.
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
#undef ASSERTS			/* Check asserts */

#ifdef ASSERTS
#include <numlib.h>
#endif

typedef unsigned char byte;


/* Left shift, handles >= 32 properly */
#define LSHIFT(aa, bb)  ((bb) <= 31 ? ((aa) << (bb)) : (((aa) << 31) << ((bb)-31)))

/* Specific entry size write routine */

void write_uchar(
byte *p,
#ifdef ALLOW64
unsigned longlong v
#else
unsigned long v
#endif
) {
	*((unsigned char *)p) = (unsigned char)v;
}

void write_ushort(
byte *p,
#ifdef ALLOW64
unsigned longlong v
#else
unsigned long v
#endif
) {
	*((unsigned short *)p) = (unsigned short)v;
}

void write_uint(
byte *p,
#ifdef ALLOW64
unsigned longlong v
#else
unsigned long v
#endif
) {
	*((unsigned int *)p) = (unsigned int)v;
}

void write_ulong(
byte *p,
#ifdef ALLOW64
unsigned longlong v
#else
unsigned long v
#endif
) {
	*((unsigned long *)p) = (unsigned long)v;
}

#ifdef ALLOW64
void write_ulonglong(
byte *p,
unsigned longlong v
) {
	*((unsigned longlong *)p) = (unsigned longlong)v;
}
#endif /* ALLOW64 */

void write_default(
byte *p,
#ifdef ALLOW64
unsigned longlong v
#else
unsigned long v
#endif
) {
	fprintf(stderr,"imdi_tabl: internal failure - unexpected write size!\n");
*((char *)NULL) = 0;	/* ~~999 */
	exit(-1);
}

/* Array of write routines */
#ifdef ALLOW64
void (*write_entry[16])(byte *p, unsigned longlong v);
#else
void (*write_entry[16])(byte *p, unsigned long v);
#endif

void
init_write_tab(void) {
	int i;

	for (i = 0; i < 16; i++)
		write_entry[i] = write_default;		/* Make sure any un-inited access bombs */

	write_entry[sizeof(unsigned char)]  = write_uchar;
	write_entry[sizeof(unsigned short)] = write_ushort;
	write_entry[sizeof(unsigned int)]   = write_uint;
	write_entry[sizeof(unsigned long)]  = write_ulong;
#ifdef ALLOW64
	write_entry[sizeof(unsigned longlong)]  = write_ulonglong;
#endif /* ALLOW64 */
}

imdi_imp *
imdi_tab(
	genspec *gs,	/* Pointer to gen spec */
	tabspec *ts,	/* Pointer to table spec */

	/* Callbacks to lookup the mdi table values */
	double (*input_curve) (void *cntx, int ch, double in_val),
	void   (*md_table)    (void *cntx, double *out_vals, double *in_vals),
	double (*output_curve)(void *cntx, int ch, double in_val),
	void *cntx		/* Context to callbacks */
) {
	static inited = 0;
	static bigend = 0;
	int e;
	imdi_imp *it;
	unsigned long etest = 0xff;
	int idinc[IXDI+1];		/* Increment for each dimension of interp table. */
	int ibdinc[IXDI+1];		/* idinc[] in bytes */
	int sdinc[IXDI+1];		/* Increment for each dimension of simplex table. */
	int sbdinc[IXDI+1];		/* sdinc[] in bytes */

#ifdef VERBOSE
	printf("imdi_tab called\n");
#endif

	if (inited == 0) {
		init_write_tab();
		if (*((unsigned char *)&etest) == 0xff)
			bigend = 0;		/* Little endian */
		else
			bigend = 1;		/* Big endian */
		inited = 1;
	}

	if ((it = (imdi_imp *)malloc(sizeof(imdi_imp))) == NULL) {
#ifdef VERBOSE
		printf("malloc imdi_imp size %d failed\n",sizeof(imdi_imp));
#endif
		return NULL;	/* Should we signal error ? How ? */
	}

	/* Compute interp and simplex table dimension increments & total sizes */
	idinc[0]  = 1;
	ibdinc[0] = ts->im_ts;
	for (e = 1; e <= gs->id; e++) {
		idinc[e]  = idinc[e-1]  * gs->itres;
		ibdinc[e] = ibdinc[e-1] * gs->itres;
	}

	if (!ts->sort) {
		sdinc[0]  = 1;
		sbdinc[0] = ts->sm_ts;
		for (e = 1; e <= gs->id; e++) {
			sdinc[e]  = sdinc[e-1]  * gs->stres;
			sbdinc[e] = sbdinc[e-1] * gs->stres;
		}
	}

	/* First we setup the input tables */
	for (e = 0; e < gs->id; e++) {
		byte *t, *p;	/* Pointer to input table, entry pointer */
		int ne;			/* Number of entries */
		int ex;			/* Entry index */
		int ix = 0;		/* Extract flag */

		/* Compute number of entries */
		if (ts->it_ix && !gs->in.packed) {	/* Input is the whole bpch[] size */
			ix = 1;					/* Need to do extraction in lookup */
			if (gs->in.pint) {
				ne = (1 << (gs->in.bpch[0]));		/* Same size used for all input tables */
			} else {
				ne = (1 << (gs->in.bpch[e]));		/* This input channels size */
			}
		} else {				/* Input is the value size */
			ne = (1 << (gs->in.bpv[e]));		/* This input values size */
		}

		/* Allocate the table */
		if ((t = (byte *)malloc(ts->it_ts * ne)) == NULL) {
#ifdef VERBOSE
			printf("malloc imdi input table size %d failed\n",ts->it_ts * ne);
#endif
			return NULL;	/* Should we signal error ? How ? */
		}

		/* For each possible input value, compute the entry value */
		for (ex = 0, p = t; ex < ne; ex++, p += ts->it_ts) {
			int ee;
			int iiv;		/* Integer input value */
			int ivr;		/* Input value range */
			int isb;		/* Input sign bit/signed to offset displacement */
			double riv;		/* Real input value, 0.0 - 1.0 */
			double rtv;		/* Real transformed value, 0.0 - 1.0 */
			double rmi;		/* Real interpolation table index */
			double rsi;		/* Real simplex index */
			int imi;		/* Interpiolation table index */
			int isi;		/* Integer simplex index */
			int iwe;		/* Integer weighting value */
			int vo;			/* Vertex offset value */

			if (ix) {		/* Extract value from index */
				ivr = ((1 << (gs->in.bpv[e])) -1);
				iiv = (ex >> gs->in.bov[e]) & ((1 << (gs->in.bpv[e])) -1);
			} else {
				ivr = (ne - 1);	/* (Should be bpv[e], but take no chances!) */
				iiv = ex;	/* Input value is simply index */
			}
			isb = ivr & ~(((unsigned int)ivr) >> 1);			/* Top bit */
			if (gs->in_signed & (1 << e))		/* Treat input as signed */
				iiv = (iiv & isb) ? iiv - isb : iiv + isb;	/* Convert to offset from signed */
			riv = (double) iiv / (double)ivr;	/* Compute floating point */
			rtv = input_curve(cntx, e, riv);	/* Lookup the input table transform */
			if (rtv < 0.0)						/* Guard against sillies */
				rtv = 0.0;
			else if (rtv > 1.0)
				rtv = 1.0;

			/* divide into interp base and cube sub index */
			rmi = rtv * (gs->itres - 1);
			imi = (int)floor(rmi);		/* Interp. entry coordinate */
			if (imi >= (gs->itres-1))	/* Keep cube base one row back from far edge */
				imi = gs->itres-2;
			rsi = rmi - (double)imi;	/* offset into entry cube */
			if (ts->sort) {
				iwe = (int)((rsi * (1 << gs->prec)) + 0.5);	/* Weighting scale */
				vo = idinc[e] * ts->vo_om;	/* Vertex offset */
			} else {
				isi = (int)((rsi * gs->stres) + 0.5);
				if (isi == gs->stres) {		/* Keep simplex index within table */
					isi = 0;
					imi++;					/* Move to next interp. lattice */
				}
				isi *= sdinc[e]; 		/* Convert the raw indexes into offset in this dim */
			}
			imi *= idinc[e]; 			/* Convert the raw indexes into offset in this dim */

#ifdef ASSERTS
			/* ~~~ needs fixing for sort ~~~~ */
			if ((imi & (LSHIFT(1,ts->it_ab)-1)) != imi)
				error("imdi_tab assert: (imi & ((1 << ts->it_ab)-1)) != imi, imi = 0x%x, it_ab = 0x%x\n",imi,ts->it_ab);
			if (imi >= idinc[gs->id])
				error("imdi_tab assert: imi >= idinc[gs->id]\n");
			if ((isi & (LSHIFT(1,ts->sx_ab)-1)) != isi) 
				error("imdi_tab assert: (isi & ((1 << ts->sx_ab)-1)) != isi, isi = 0x%x, sx_ab = 0x%x\n",isi,ts->sx_ab);
			if (!ts->sort && isi >= sdinc[gs->id])
				error("imdi_tab assert: isi >= sdinc[gs->id]\n");
#endif

			/* Now stuff them into the table entry */
			if (ts->sort) {
				if (ts->it_xs) {		/* Separate interp index and weight/offset*/
					if (ts->wo_xs) {	/* All 3 are separate */
						write_entry[ts->ix_es](p + ts->ix_eo, imi);
						write_entry[ts->we_es](p + ts->we_eo, iwe);
						write_entry[ts->vo_es](p + ts->vo_eo, vo);
					} else {
#ifdef ALLOW64
						unsigned longlong iwo;
#else
						unsigned long iwo;
#endif
	
						iwo = (iwe << ts->vo_ab) | vo; 	/* Combined weight+vertex offset */
						write_entry[ts->ix_es](p + ts->ix_eo, imi);
						write_entry[ts->wo_es](p + ts->wo_eo, iwo);
					}
				} else {			/* All 3 are combined  */
#ifdef ALLOW64
					unsigned longlong iit;
#else
					unsigned long iit;
#endif
					iit = (((imi << ts->we_ab) | iwe) << ts->vo_ab) | vo;
					write_entry[ts->it_ts](p, iit);
				}
			} else {
				if (ts->it_xs) {		/* Separate interp index and weight/offset*/
					write_entry[ts->ix_es](p + ts->ix_eo, imi);
					write_entry[ts->sx_es](p + ts->sx_eo, isi);
				} else {
#ifdef ALLOW64
					unsigned longlong iit;
#else
					unsigned long iit;
#endif
					iit = (imi << ts->sx_ab) | isi;	/* Combine interp and simplex indexes */
					write_entry[ts->it_ts](p, iit);
				}
			}
		}

		/* Put table into place */
		it->in_tables[e] = (void *)t;
	}
	it->nintabs = e;

	/* Setup the interpolation table */
	{
		byte *t, *p;	/* Pointer to interp table, pointer to total entry */
		PHILBERT(phc)	/* Pseudo Hilbert counter */
		double vscale;	/* Value scale for fixed point */
		int vsize;		/* Fixed point storage size */

		if (ts->im_cd)
			vsize = (gs->prec * 2)/8;	/* Fixed point entry & computation size */
		else
			vsize = gs->prec/8;			/* Fixed point entry size */
		vscale = (1 << gs->prec) -1.0;	/* Value scale for fixed point padding */
										/* -1.0 is to prevent carry after accumulation */
		/* Allocate the table */
		if ((t = (byte *)malloc(ibdinc[gs->id])) == NULL) {
#ifdef VERBOSE
			printf("malloc imdi interpolation table size %d failed\n",ibdinc[gs->id]);
#endif
			return NULL;	/* Should we signal error ? How ? */
		}
#ifdef VERBOSE
		printf("Allocated grid table = %d bytes\n",ibdinc[gs->id]);
#endif

		/* Get ready to access all the entries in the table */
		p = t;
		PH_INIT(phc, gs->id, gs->itres)

		/* Create all the interpolation table entry values */
		do {
			int ee, f;
			double riv[IXDI];	/* Real input values */
			double rev[IXDO];	/* Real entry values */
			unsigned long iev; 
			byte *pp;			/* Pointer to sub-entry */

			for (e = 0, p = t; e < gs->id; e++) {
				riv[e] = ((double)phc[e]) / (gs->itres - 1.0);
				p += phc[e] * ibdinc[e];		/* Compute pointer to entry value */
			}

			/* Lookup this verticies value */
			md_table(cntx, rev, riv);

			/* Create all the output values */

			/* I'm trying to avoid having to declare the actual entry sized */
			/* variables, since it is difficult dynamically. */

			/* For all the full entries */
			f = 0;
			pp = p;
			for (e = 0; e < ts->im_fn; e++, pp += ts->im_fs) {
				/* For all channels within full entry */
				for (ee = 0; ee < ts->im_fv; ee++, f++) {
					double revf = rev[f];
					if (revf < 0.0)						/* Guard against sillies */
						revf = 0.0;
					else if (revf > 1.0)
						revf = 1.0;
					iev = (unsigned long)(revf * vscale + 0.5);

					if (bigend) {
						write_entry[vsize](pp + (ts->im_fs - (ee+1) * vsize), iev);
					} else {
						write_entry[vsize](pp + ee * vsize, iev);
					}
				}
			}

			/* For all the 0 or 1 partial entry */
			for (e = 0; e < ts->im_pn; e++) {
				/* For all channels within partial entry */
				for (ee = 0; ee < ts->im_pv; ee++, f++) {
					double revf = rev[f];
					if (revf < 0.0)						/* Guard against sillies */
						revf = 0.0;
					else if (revf > 1.0)
						revf = 1.0;
					iev = (unsigned long)(revf * vscale + 0.5);

					if (bigend) {
						write_entry[vsize](pp + (ts->im_ps - (ee+1) * vsize), iev);
					} else {
						write_entry[vsize](pp + ee * vsize, iev);
					}
				}
			}
#ifdef ASSERTS
			if (f != gs->od)
				fprintf(stderr,"imdi_tab assert: f == gs->od\n");
#endif

			PH_INC(phc)
		} while (!PH_LOOPED(phc));

		/* Put table into place */
		it->im_table = (void *)t;
	}

	/* Setup the simplex table */
	if (ts->sort) {
		it->sw_table = (void *)NULL;

	} else {
		byte *t, *p;		/* Pointer to input table, pointer to total entry */
		int nsplx;			/* Total number of simplexes */
		XCOMBO(vcmb, gs->id+1, 1 << gs->id);/* Simplex dimension id out of cube dimention id */
		int comb[24][IXDI];	/* Parameter[id]->Absolute[id] coordinate index */
		int ps[IXDI+1];		/* Base simplex parameter space counter */
		int pse;			/* Base simplex parameter space counter index */
		int idioff;			/* Interpolation table diagonal offset value */

		if (gs->id > 4) {
			fprintf(stderr,"imdi_tabl: internal failure - trying to create simplex table with di > 4!\n");
			exit(-1);
		}

		/* Allocate the table */
		if ((t = (byte *)malloc(sbdinc[gs->id])) == NULL) {
#ifdef VERBOSE
			printf("malloc imdi simplex table size %d failed\n",sbdinc[gs->id]);
#endif
			return NULL;	/* Should we signal error ? How ? */
		}
#ifdef VERBOSE
		printf("Allocated simplex table = %d bytes\n",sbdinc[gs->id]);
#endif

		/* Compute the interp table offset to the diagonal vertex */
		for (idioff = 0, e = 0; e < gs->id; e++)
			idioff += idinc[e];		/* Sum one offset in each dimension */

		/* Figure out how many simplexes fit into this dimension cube, */
		/* and how to map from the base simplex to each actual simplex. */
		XCB_INIT(vcmb);
		for (nsplx = 0; ;) {
			int i;

			/* XCOMB generates verticies in order from max to min offest */
	
			/* Compute Absolute -> Parameter mapping */
			for (e = 0; e < gs->id; e++) {		/* For each absolute axis */
				for (i = 0; i < gs->id; i++) {	/* For each verticy, order large to small */
					if ((vcmb[i]   & (1<<e)) != 0 && 
					    (vcmb[i+1] & (1<<e)) == 0) {/* Transition from offset 1 to 0 */
						comb[nsplx][i] = e;
						break;
					}
				}
			}

/*	
printf("~~Verticies   = ");
for (i = 0; i <= gs->id; i++)
	printf("%d ",vcmb[i]);
printf("\n");

printf("~~Parm -> Abs = ");
for (e = 0; e < gs->id; e++)
	printf("%d ",comb[nsplx][e]);
printf("\n");
*/

			/* Increment the counter value */
			XCB_INC(vcmb);
			nsplx++;
			if (XCB_DONE(vcmb))
				break;
		}

		/* Now generate the contents of the base simplex, */
		/* and map it to all the symetrical simplexes */

		/* Init parameter space counter. */
		/* Note that ps[id-1] >= ps[id-2] >=  ... >= ps[1] >= ps[0] */
		for (pse = 0; pse < gs->id; pse++)
			ps[pse] = 0;
		ps[pse] = gs->stres-1;

		/* Itterate through the simplex parameter space */
		for (pse = 0; pse < gs->id;) {
			double qps[IXDI];	/* Quantized parameter values */
			int    we[IXDI+1];	/* Baricentric coords/vertex weighting */
			double wvscale = (1 << gs->prec);	/* Weighting value scale */
			int sx;				/* Simplex */

/*
printf("Param coord =");
for (e = gs->id-1; e >= 0; e--) {
	printf(" %d",ps[e]);
}
printf("\n");
*/
			for (e = 0; e < gs->id; e++) {
				/* (Should try wvscale + 0.49999999, or something ?) */
				double tt = (wvscale * (double)ps[e])/((double)gs->stres);
				qps[e] = (int)(tt + 0.5);
			}
	
			/* Convert quantized parameter values into weighting values */
			we[gs->id] = (1 << gs->prec) - qps[gs->id-1];
			for (e = gs->id-1; e > 0; e--)
				we[e] = qps[e] - qps[e-1];
			we[0] = qps[0];

#ifdef ASSERTS
			{
				int sow = 0;
				for (e = gs->id; e >= 0; e--)
					sow += we[e];
				
				if (sow != (1 << gs->prec))
					fprintf(stderr,"imdi_tab assert: sum weights == (1 << gs->prec)\n");
			}
#endif

/*
printf("Baricentric coord =");
for (e = gs->id; e >= 0; e--) {
	printf(" %d",we[e]);
}
printf("\n");
*/

			/* For each simplex, compute the interp. and */
			/* and entry offsets, and write the entry. */
			for (sx = 0; sx < nsplx; sx++ ) {
				int v;					/* Vertex index */
				byte *pp;				/* Pointer to sub-entry */
				unsigned long vofb;		/* Vertex offset, base */
				unsigned long vwe;		/* Vertex weight */

				for (e = 0, p = t; e < gs->id; e++) {
					int ee = comb[sx][e];		/* Absolute coord index */
					p += ps[e] * sbdinc[ee];	/* Pointer to entry */
				}

				/* For each vertex entry */
				for (v = 0, pp = p; v <= gs->id; v++) {
					unsigned long vof;
					if (v == 0) {
						vofb = idioff;		/* Start at diagonal offset */
					} else {
						vofb -= idinc[comb[sx][v-1]];/* Move to next vertex */
					}
					vwe = we[v];					/* Weight for this vertex */

					if (vwe == 0)
						vof = 0;					/* Use zero offset if weight is zero */
					else
						vof = vofb * ts->vo_om;		/* Strength reduce kernel scaling */

					/* Write vwe and vof to entry */
					if (ts->wo_xs) {	/* Separate entries */
						write_entry[ts->we_es](pp + ts->we_eo, vwe);
						write_entry[ts->vo_es](pp + ts->vo_eo, vof);
						pp += ts->wo_es;
					} else {			/* Combined entries */
#ifdef ALLOW64
						unsigned longlong iwo;
#else
						unsigned long iwo;
#endif
						iwo = (vwe << ts->vo_ab) | vof; 	/* Combined weight+vertex offset */
						write_entry[ts->wo_es](pp + ts->wo_eo, iwo);
						pp += ts->wo_es;
					}
				}

				/* Assert vofb == 0 */
#ifdef ASSERTS
				if (vofb != 0)
					fprintf(stderr,"imdi_tab assert: vofb == 0\n");
#endif
			}	/* Next simplex */

			/* Increment the parameter coords */
			for (pse = 0; pse < gs->id; pse++) {
				ps[pse]++;
				if (ps[pse] <= ps[pse+1])
					break;	/* No carry */
				ps[pse] = 0;
			}
		}

		/* Put table into place */
		it->sw_table = (void *)t;
	}
	
	/* Last, setup the output tables */
	for (e = 0; e < gs->od; e++) {
		byte *t, *p;	/* Pointer to output table, entry pointer */
		int ne;			/* Number of entries */
		int iiv;		/* Integer input value */
		double ivr = (double)((1 << gs->prec)-1); /* Input value range */
		double ovr = (double)((1 << ts->ot_bits[e])-1); /* Output value range */
		int	osb = (1 << (ts->ot_bits[e]-1)); /* Output offset to signed displacement */
		int ooff = ts->ot_off[e];	/* Output value bit offset */

		ne = (1 << gs->prec);		/* Output of clut is prec bits */

		/* Allocate the table */
		if ((t = (byte *)malloc(ts->ot_ts * ne)) == NULL) {
#ifdef VERBOSE
			printf("malloc imdi output table size %d failed\n",ts->ot_ts * ne);
#endif
			return NULL;	/* Should we signal error ? How ? */
		}

		/* For each possible output value, compute the entry value */
		for (iiv = 0, p = t; iiv < ne; iiv++, p += ts->ot_ts) {
			int ee;
			double riv;		/* Real input value, 0.0 - 1.0 */
			double rtv;		/* Real transformed value, 0.0 - 1.0 */
			unsigned long iov;	/* Integer output value */

			riv = (double) iiv / ivr;			/* Compute floating point */
			rtv = output_curve(cntx, e, riv);	/* Lookup the output table transform */
			if (rtv < 0.0)						/* Guard against sillies */
				rtv = 0.0;
			else if (rtv > 1.0)
				rtv = 1.0;
			iov = (unsigned long)(rtv * ovr + 0.5);	/* output value */
			if (gs->out_signed & (1 << e))		/* Treat output as signed */
				iov = (iov >= osb) ? iov - osb : iov + osb; /* Convert to signed from offset */
			iov <<= ooff;						/* Aligned for output */

			write_entry[ts->ot_ts](p, iov);		/* Write entry */
		}

		/* Put table into place */
		it->out_tables[e] = (void *)t;
	}
	it->nouttabs = e;

#ifdef VERBOSE
	printf("imdi_tabl returning OK\n");
#endif
	return it;
}

/* Free up the data allocated */
void
imdi_tab_free(
imdi_imp *it
) {
	int e;

	for (e = 0; e < it->nintabs; e++)
		free(it->in_tables[e]);

	free(it->sw_table);
	free(it->im_table);

	for (e = 0; e < it->nouttabs; e++)
		free(it->out_tables[e]);

	free(it);

}















