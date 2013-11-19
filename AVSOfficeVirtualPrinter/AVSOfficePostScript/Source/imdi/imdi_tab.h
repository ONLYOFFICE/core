#ifndef IMDI_TAB_H
#define IMDI_TAB_H

/* Integer Multi-Dimensional Interpolation */
/*
 * Copyright 2000 - 2002 Graeme W. Gill
 * All rights reserved.
 *
 * This material is licenced under the GNU GENERAL PUBLIC LICENCE :-
 * see the Licence.txt file for licencing details.
 */

/* Implementation details needed for table initialisation for a particular kernel */

typedef struct {

	int sort;	/* NZ for explicit sort rather than simplex table lookup */
	int it_xs;	/* NZ if separate interp index and simplex index/Weighting+Offset values  */
	int wo_xs;	/* NZ if separate weighting and vertex offset entries are to be used */

	int it_ix;	/* Non-zero if input value extraction should be done in input table */
	int it_map[IXDI];	/* Mapping from input channels to table indexes needed */
	int it_ab;	/* Input table entry size in bits */
	int it_ts;	/* Input table :- total input table entry size in bytes */
				/* Bit packing order is (ms to ls) :
	          	    sort: ix, we, vo
	          	    sort: ix, wo
	          	   !sort: ix, sx
				 */

				/* Interpolation index is always in the input table */
	int ix_ab;	/* Interpolation index entry size in bits */
	int ix_es;	/* Interpolation index entry size in bytes */
	int ix_eo;	/* Interpolation index entry offset in bytes */

				/* Simplex Index is always in the input table */
	int sx_ab;	/* Simplex Index entry size in bits */
	int sx_es;	/* Simplex Index entry size in bytes */
	int sx_eo;	/* Simplex Index entry offset in bytes */

	int sm_ts;	/* Simplex table entry total size in bytes */
				/* Bit packing order is (ms to ls) : we, vo */

				/* Combined Weighting + Offset may be in input table or Simplex entry */
	int wo_ab;	/* Combined Weighting + Offset entry size in bits */
	int wo_es;	/* Combined Weighting + Offset entry size in bytes */
	int wo_eo;	/* Combined Weighting + Offset entry offset in bytes */

				/* Weighting may be in input table or Simplex entry */
	int we_ab;	/* Weighting entry size in bits */
	int we_es;	/* Weighting entry size in bytes */
	int we_eo;	/* Weighting entry offset in bytes */

				/* Vertex offset may be in input table or Simplex entry */
	int vo_ab;	/* Vertex Offset entry size in bits */
	int vo_es;	/* Vertex Offset entry size in bytes */
	int vo_eo;	/* Vertex Offset entry offset in bytes */
	int vo_om;	/* Vertex Offset scaling multiplier */

	int im_cd;	/* Non-zero if interpolation table entries are padded with fraction */
	int im_ts;	/* Interp. multidim :- total interp table entry size in bytes */
	int im_oc;	/* Interp. multidim :- offset scale to apply to index into interp entry */
	int im_fs;	/* Interp. multidim :- full table entry size in bytes */
	int im_fn;	/* Interp. multidim :- number of full entries */
	int im_fv;	/* Interp. multidim :- output values per full entry . */
	int im_ps;	/* Interp. multidim :- partial table entry size in bytes, used & unsused */
	int im_pn;	/* Interp. multidim :- number of partial entries - must be 0 or 1 */
	int im_pv;	/* Interp. multidim :- used output values per partial entry . */
	int im_map[IXDO];	/* Mapping from output table values to output positions needed */

	int ot_ts;	/* Output table :- total entry size in bytes of every table */
	int ot_off[IXDO];	/* Offset for each output value within the output word needed */
	int ot_bits[IXDO];	/* Number of bits for value within the output word needed */

	/* Associated interpolation function */
	void (*interp)(struct _imdi *s, void **inp, void **outp, unsigned int npix); /* At run time */
} tabspec;

/*
 * We declare the function that knows how to create the
 * appropriate run time tables for our chosen kernel,
 * and the color mapping we want to perform.
*/

imdi_imp *
imdi_tab(
	genspec *gs,	/* Pointer to gen spec */
	tabspec *ts,	/* Pointer to tabl spec */

	/* Callbacks to lookup the mdi table values */
	double (*input_curve) (void *cntx, int ch, double in_val),
	void   (*md_table)    (void *cntx, double *out_vals, double *in_vals),
	double (*output_curve)(void *cntx, int ch, double in_val),
	void *cntx		/* Context to callbacks */
);

void imdi_tab_free(imdi_imp *it);

#endif /* IMDI_TAB_H */
