/* Integer Multi-Dimensional Interpolation */
/*
 * Copyright 2000 - 2002 Graeme W. Gill
 * All rights reserved.
 *
 * This material is licenced under the GNU GENERAL PUBLIC LICENCE :-
 * see the Licence.txt file for licencing details.
 */

/* Top level kerenel code generator */

/*
 * This module is invoked from the make system,
 * and generates all the versions and configurations of 
 * the IMDI kernel code. It includes all the generated
 * files in imdi_k.h, which also contains a table
 * so that the run time code knows what kernels
 * are available.
*/

/*
	Ideal grid resolutions for 8/16 bit precision calculations:

	There are a limited number of grid resolution and grid cells
	sub resolution values that work perfectly, and allow every possible
	input value to map to a precise interpolation value. For any
	system that deals with Lab data, it is also better if there is
	a grid point near a,b == 0, meaning that an odd grid size is
	preferable for 3 dimensional conversions. Reasonable memory
	usage is usuall somewhere between 100K and 1M entries for
	the grid table, limiting the maximum grid resolution.
	The following lists the some of the possible grid and sub grid
	resolutions in order of best to worst. (Fewer jumps is better).


	Grid	Sub8	Round8	Sub16	Round16	Jumps	
	4		85		85		21845	21845	0
	6		51		51		13107	13107	0
	16		17		17		4369	4369	0
	18		15		15		3855	3855	0
	52		5		5		1285	1285	0
	86		3		3		771		771		0
	256		1		1		257		257		0
	258						255		255		0
	772						85		85		0
	1286					51		51		0
	3856					17		17		0
	4370					15		15		0
	13108					5		5		0
	21846					3		3		0
	65536					1		1		0
	3		127.5	128						1
	5		63.75	64						1
	9		31.875	32						1
	17		15.937	16						1
	33		7.968	8						1
	65		3.984	4						1
	128		2.007	2						1
	129		1.992	2						1
	255		1.003	1						1
	12		23.188	23						2
	24		11.086	11						2
	254		1.007	1						2
	7		42.5	43						3
	8		36.428	36						3
	10		28.333	28						3
	13		21.25	21						3
	15		18.214	18						3
	19		14.166	14						3
	22		12.142	12						3
	29		9.107	9						3
	37		7.083	7						3
	43		6.071	6						3
	44		5.930	6						3
	64		4.047	4						3
	85		3.035	3						3
	87		2.965	3						3
	127		2.023	2						3
	130		1.976	2						3
	253		1.011	1						3

	[8  bit: sub = 255/(grid-1), jumps = abs((grid-1)*round(sub)-255)]
	[16 bit: sub = 65535/(grid-1), jumps = abs((grid-1)*round(sub)-65535)]

	The above takes into consideration the mapping of the sub-cell or
	simplex table resolution, but doesn't take into account the quantizing
	of the sub-cell weighting values into the range 0..256 or 0..65536.

	This will be best when round(sub) divides evenly into 256 or 65536,
	so grid resolutions of 3, 5, 9, 17, 33, 65, 128, 129, 255 may be the
	best choice for sort algorithm grid resolutions.

 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>
#include <string.h>
#include "copyright.h"
#include "config.h"

#include "imdi_imp.h"
#include "imdi_gen.h"
#include "imdi_tab.h"

#undef VERBOSE
#undef TEST1		/* Generate one test case */
#define FULL			/* Full set of combinations */

/* The following structure initialisations define what kernel routines should be built */
static
gendesc descs[] = {
#ifdef TEST1
	{
		{ 1, 0 },	/* Input dimension combinations */
		{ 256, 0 }, 		/* Interpolation table resolutions */
		{ 1, 0 },	 	/* Simplex table resolutions */
		{ 1, 0 },		/* Output dimension combinations */
		{pixint16, 0 },		/* Input pixel representation */
		{pixint16, 0},		/* Output pixel representation */
		{forward, 0}		/* Direction (not implemented) */
	}
#else
#ifndef FULL
	/* Normal */
								/* * means multiplies combination */
								/* + means lockstep with previous line */
	{
		{ 1,    3,  4, 8, 0 },		/*   Input dimension combinations */
		{ 256, 33, 18, 5, 0 }, 		/* + Interpolation table resolutions */
		{ 1,    8, 17, 1, 0 }, 		/* + Simplex table resolutions */
		{ 1, 3, 4, 8, 0 },			/* * Output dimension combinations */
		{pixint8, pixint8,  pixint16, 0 },
									/*   Input pixel representation (determines precision) */
		{pixint8, pixint16, pixint16, 0 },
									/* + Output pixel representation */
		{forward, 0}			/* * Direction (not implemented) */
	}
#else
	/* Full set of combinations */
								/* * means multiplies combination */
								/* + means lockstep with previous line */
	{
		{ 1,    3,  4,  5,  6,  7,  8, 0 },		/*   Input dimension combinations */
		{ 256, 33, 18, 16, 12,  8,  7, 0 }, 	/* + Min Interpolation table resolutions */
		{ 1,    8, 17,  1,  1,  1,  1, 0 }, 	/* + Min Simplex table resolutions */
		{ 1, 3, 4, 5, 6, 7, 8, 0 },				/* * Output dimension combinations */
		{pixint8, pixint8,  pixint16, 0 },
									/*   Input pixel representation (determines precision) */
		{pixint8, pixint16, pixint16, 0 },
									/* + Output pixel representation */
		{forward, 0}			/* * Direction (not implemented) */
	}
#endif
#endif /* TEST1 */
};

int set_genspec(genspec *gs, gendesc *gd, int comb, mach_arch *a);
void set_architecture(mach_arch *ar);

struct _knamestr {
	char name[100];
	char desc[100];
	struct _knamestr *next;
}; typedef struct _knamestr knamestr;

knamestr *
new_knamestr(char *name, char *desc) {
	knamestr *kn;
	
	if ((kn = (knamestr *)malloc(sizeof(knamestr))) == NULL) {
		fprintf(stderr,"new_knamestr malloc failed\n");
		exit(-1);
	}
	strcpy(kn->name, name);
	strcpy(kn->desc, desc);
	kn->next = NULL;
	return kn;
}

void usage(void) {
	fprintf(stderr,"Generate imdi kernel code Version %s\n",ARGYLL_VERSION_STR);
	fprintf(stderr,"usage: imdi_gen [-i]\n");
	fprintf(stderr," -i            Individial Files\n");
	exit(1);
}

int
main(int argc, char *argv[]) {
	int fa,nfa;				/* argument we're looking at */
	int indiv = 0;			/* Individual files */
	int rv;
	int dn, tnd;
	genspec gs;
	mach_arch ar;
	int ix = 1;
	knamestr *list = NULL, *lp;
	FILE *kcode;	/* Kernel routine code file */
	FILE *kheader;	/* Kernel routine header file */

	/* Process the arguments */
	for(fa = 1;fa < argc;fa++) {
		nfa = fa;					/* skip to nfa if next argument is used */
		if (argv[fa][0] == '-')	{	/* Look for any flags */
			char *na = NULL;		/* next argument after flag, null if none */

			if (argv[fa][2] != '\000')
				na = &argv[fa][2];		/* next is directly after flag */
			else {
				if ((fa+1) < argc) {
					if (argv[fa+1][0] != '-') {
						nfa = fa + 1;
						na = argv[nfa];		/* next is seperate non-flag argument */
					}
				}
			}

			if (argv[fa][1] == '?')
				usage();

			/* Individual files */
			else if (argv[fa][1] == 'i' || argv[fa][1] == 'I') {
				indiv = 1;
			}
			else 
				usage();
		} else
			break;
	}

	set_architecture(&ar);

	/* Open the file for kernel routine declaration header */
	if ((kheader = fopen("imdi_k.h", "w")) == NULL) {
		fprintf(stderr,"imdi_gen: unable to open file 'imdi_k.h'\n");
		exit(-1);
	}

	if (!indiv) {
		if ((kcode = fopen("imdi_k.c", "w")) == NULL) {
			fprintf(stderr,"imdi_gen: unable to open file 'imdi_k.c'\n");
			exit(-1);
		}
	}

	tnd = sizeof(descs)/sizeof(gendesc);	/* Total number of descriptions */
#ifdef VERBOSE
	printf("Number of descriptions = %d\n",tnd);
#endif /* VERBOSE */

	fprintf(kheader,"/* Declarations for all the generated kernel functions */\n");
	fprintf(kheader,"/* This file is generated by imdi_gen */\n");
	fprintf(kheader,"\n");

	/* For all the descriptions */
	for (dn = 0; dn < tnd; dn++) {
		int cb, ncb;

		for (cb = 0, ncb = 1; cb < ncb; cb++) {
			char ofname[100];

			/* Compute generate spec */
			ncb = set_genspec(&gs, &descs[dn], cb, &ar);

			if (indiv) {
				if ((kcode = fopen(ofname, "w")) == NULL) {
					fprintf(stderr,"imdi_gen: unable to open file '%s'\n",ofname);
					exit(-1);
				}
			}

			/* Generate it */
#ifdef TEST1
			rv = gen_c_kernel(&gs, &ar, kcode, 99); ix++;
#else
			rv = gen_c_kernel(&gs, &ar, kcode, ix++);
#endif
			if (rv != 0) {
				fprintf(stderr,"gen_c_kernel failed with %d\n",rv);
				exit(-1);
			}
			
			/* Add the name to the list */
			if (list == NULL)
				lp = list = new_knamestr(gs.kname, gs.kdesc);
			else {
				lp->next = new_knamestr(gs.kname, gs.kdesc);
				lp = lp->next;
			}
			if (indiv) {
				if (fclose(kcode) != 0) {
					fprintf(stderr,"imdi_gen: unable to close file '%s'\n",ofname);
					exit(-1);
				}
			}
		}
	}

	/* Include the kernel functions in the header file */
	if (indiv) {
		for(lp = list; lp != NULL; lp = lp->next) {
			fprintf(kheader,"#include \"%s_%s.c\"\n",lp->name,lp->desc);
		}
	} else {
		fprintf(kheader,"#include \"imdi_k.c\"	/* All the kernel code */\n");
	}
	fprintf(kheader,"\n");

	/* Output function table */
	
	fprintf(kheader,
		"struct {\n"
		"	void (*interp)(imdi *s, void **outp, void **inp, unsigned int npix);\n"
		"	void (*gen)(genspec *g);\n"
		"	void (*tab)(tabspec *t);\n"
		"} ktable[%d] = {\n",ix-1);

	for(lp = list; lp != NULL; lp = lp->next) {
		fprintf(kheader,"\t{ %s, %s_gen, %s_tab }%s\n", lp->name, lp->name, lp->name, 
		lp->next != NULL ? "," : "");
	}
	fprintf(kheader,"};\n");
	fprintf(kheader,"\n");
	fprintf(kheader,"int no_kfuncs = %d;\n",ix-1);
	fprintf(kheader,"\n");

	if (!indiv) {
		if (fclose(kcode) != 0) {
			fprintf(stderr,"imdi_gen: unable to close file 'imdi_k.c'\n");
			exit(-1);
		}
	}

	if (fclose(kheader) != 0) {
		fprintf(stderr,"imdi_gen: unable to close file 'imdi_k.h'\n");
		exit(-1);
	}

	/* Free the kname list */
	for(lp = list; lp != NULL;) {
		char *p = (char *)lp;
		lp = lp->next;
		free(p);
	}

	return 0;
}

void translate_pixrep(pixlayout *pl, char **desc, int *prec, imdi_pixrep rep, int dim, mach_arch *a);

/* Translate between a gendesc and genspec */

int 				/* Return number of combinations possible */
set_genspec(
genspec *gs,		/* Return specification */
gendesc *gd,		/* Input description */
int comb,			/* Combination index */
mach_arch *a		/* Machine architecture */
) {
	int nc = 1;			/* Number of combinations */
	int nidc, id;		/* Number of input dimension combinations, current index */
	int nodc, od;		/* Number of output dimension combinations, current index */
	int nirc, ir;		/* Number of input representation combinations, current index */
	int norc, or;		/* Number of output representation combinations, current index */
	int ndc, dir;		/* Number of direction combinations, current index */

	/* Figure out how many combinations there are */
	for (nidc = 0; gd->idcombs[nidc] != 0; nidc++)	/* Input dimension */
		;
	nc *= nidc;
	for (nodc = 0; gd->odcombs[nodc] != 0; nodc++)	/* Output dimension */
		;
	nc *= nodc;
	for (nirc = 0; gd->incombs[nirc] != 0; nirc++)	/* Input representation */
		;
	nc *= nirc;
	for (norc = 0; gd->outcombs[norc] != 0; norc++)	/* Output representation */
		;
	if (nirc != norc) {
		fprintf(stderr,"imdi_gen: Must be equal numberof input and output representations\n");
		exit(-1);
	}
		
	for (ndc = 0; gd->dircombs[ndc] != 0; ndc++)	/* Direction */
		;
	nc *= ndc;

	if (comb < nc) {	/* If we are within a legal combination */
		int iprec, oprec;
		char *idesc, *odesc;	/* Representation descriptions */
		char *ddesc;			/* Direction description */

		id = comb % nidc;
		comb /= nidc;
		od = comb % nodc;
		comb /= nodc;
		ir = comb % nirc;
		comb /= nirc;
		or = ir;			/* In and out combs track together */
		dir = comb % ndc;
		comb /= ndc;
#ifdef VERBOSE
		printf("Combination id = %d, od = %d, ir = %d, or = %d, dir = %d\n",
		       id,od,ir,or,dir);
#endif /* VERBOSE */

		gs->id = gd->idcombs[id];		/* Input dimensions */
		gs->itres = gd->itres[id];		/* Interpolation table resolution */
		gs->stres = gd->stres[id];		/* Simplex table resolution */
		gs->od = gd->odcombs[od];		/* Output dimensions */

		if (gs->id > IXDI) {
			fprintf(stderr,"imdi_gen: Input dimension %d exceeds limit %d\n",gs->id,IXDI);
			exit(-1);
		}
		if (gs->od > IXDO) {
			fprintf(stderr,"imdi_gen: Output dimension %d exceeds limit %d\n",gs->od,IXDO);
			exit(-1);
		}
		/* Input representation */
		gs->irep = gd->incombs[ir];		/* Keep a copy of this */
		translate_pixrep(&gs->in,  &idesc, &iprec, gd->incombs[ir], gs->id, a);
		gs->in_signed = 0x0;			/* Not used during generation, used at runtime setup */

		/* Output representation */
		gs->orep = gd->outcombs[or];		/* Keep a copy of this */
		translate_pixrep(&gs->out, &odesc, &oprec, gd->outcombs[or], gs->od, a);
		gs->out_signed = 0x0;				/* Not used during generation, used at runtime setup */

		/* Make native precision the smaller of input and output representation */
		gs->prec = iprec < oprec ? iprec : oprec;

		gs->dir = gd->dircombs[dir];	/* Direction */

		ddesc = gs->dir == backward ? "b" : "f";	/* Direction description */

#ifdef VERBOSE
		printf("translates to prec = %d, id = %d, od = %d, itres %d, stdres %d\n",
		       gs->prec,gs->id,gs->od,gs->itres,gs->stres);
#endif /* VERBOSE */

		/* Create a concise description string */
		sprintf(gs->kdesc,"%d_%d_%s_%s_%s", gs->id, gs->od, idesc, odesc, ddesc);
	}
	return nc;
}

void
translate_pixrep(
pixlayout *pl,		/* pixlayout to initialise */
char **desc,			/* Return description identifier */
int *prec,			/* Return basic precision specifier (may be NULL) */
imdi_pixrep rep,	/* Representation to be translated */
int dim,			/* Number of dimensions (values/pixel) */
mach_arch *a		/* Machine architecture */
) {
	switch (rep) {

		case pixint8: {		/* 8 Bits per value, pixel interleaved, no padding */
			int i;

			/* Could optimise this to packed for dim == 4 ~~~~ */

			pl->pint = 1;		/* pixel interleaved */
			pl->packed = 0;		/* Not packed */
	
			for (i = 0; i < dim; i++) {
				pl->bpch[i] = 8;	/* Bits per channel */
				pl->chi[i] = dim;	/* Channel increment */
				pl->bov[i] = 0;		/* Bit offset to value within channel */
				pl->bpv[i] = 8;		/* Bits per value within channel */
			}
			
			if (prec != NULL)
				*prec = 8;			/* Basic 8 bit precision */
			if (desc != NULL)
				*desc = "i8";		/* Interleaved 8 bit */
		} break;
	
		case planeint8: {		/* 8 bits per value, plane interleaved */
			int i;

			pl->pint = 0;		/* Not pixel interleaved */
			pl->packed = 0;		/* Not packed */
	
			for (i = 0; i < dim; i++) {
				pl->bpch[i] = 8;	/* Bits per channel */
				pl->chi[i] = 1;		/* Channel increment */
				pl->bov[i] = 0;		/* Bit offset to value within channel */
				pl->bpv[i] = 8;		/* Bits per value within channel */
			}
			
			if (prec != NULL)
				*prec = 8;			/* Basic 8 bit precision */
			if (desc != NULL)
				*desc = "p8";		/* Planar 8 bit */

		} break;

		case pixint16: {		/* 16 Bits per value, pixel interleaved, no padding */
			int i;

			/* Could optimise this to packed for dim == 4 ~~~~ */

			pl->pint = 1;		/* pixel interleaved */
			pl->packed = 0;		/* Not packed */
	
			for (i = 0; i < dim; i++) {
				pl->bpch[i] = 16;	/* Bits per channel */
				pl->chi[i] = dim;	/* Channel increment */
				pl->bov[i] = 0;		/* Bit offset to value within channel */
				pl->bpv[i] = 16;	/* Bits per value within channel */
			}
			
			if (prec != NULL)
				*prec = 16;			/* Basic 8 bit precision */
			if (desc != NULL)
				*desc = "i16";		/* Interleaved 16 bit */
		} break;
	
		case planeint16: {		/* 16 bits per value, plane interleaved */
			int i;

			pl->pint = 0;		/* Not pixel interleaved */
			pl->packed = 0;		/* Not packed */
	
			for (i = 0; i < dim; i++) {
				pl->bpch[i] = 16;	/* Bits per channel */
				pl->chi[i] = 1;		/* Channel increment */
				pl->bov[i] = 0;		/* Bit offset to value within channel */
				pl->bpv[i] = 16;	/* Bits per value within channel */
			}
			
			if (prec != NULL)
				*prec = 16;			/* Basic 8 bit precision */

			if (desc != NULL)
				*desc = "p16";		/* Planar 8 bit */
		} break;

		default: {
			fprintf(stderr,"Warning: Unknown pixel representation %d\n",rep);
		} break;
	}
}

/* Initialse the aritecture structure properly */
void
set_architecture(
mach_arch *ar
) {
	unsigned long etest = 0xff;

	if (*((unsigned char *)&etest) == 0xff) {
		ar->bigend = 0;		/* Little endian */
	} else {
		ar->bigend = 1;		/* Big endian endian */
	}

#ifdef __ppc__		/* Section tunable for PowerPC */

	ar->uwa    = 0;		/* Use wide memory access */
	ar->shfm   = 0;		/* Use shifts to mask values */
	ar->oscale = 8;		/* Has scaled indexing up to * 8 */
	ar->smmul  = 0;		/* Doesn't have fast small multiply for index scaling */
	ar->nords  = 3;		/* Number of ord types */
	ar->natord = 2;		/* Most natural type (assume unsigned int) */
	ar->nints  = 3;		/* Number of int types */
	ar->natint = 2;		/* Most natural type (assume int) */

	ar->pbits = sizeof(void *) * 8;		/* Number of bits in a pointer */

	ar->ords[0].bits = 8 * sizeof(unsigned char);
	ar->ords[0].name = "unsigned char";
	ar->ords[0].align = 1;

	ar->ords[1].bits = 8 * sizeof(unsigned short);
	ar->ords[1].name = "unsigned short";
	ar->ords[1].align = 1;

	ar->ords[2].bits = 8 * sizeof(unsigned int);
	ar->ords[2].name = "unsigned int";
	ar->ords[2].align = 1;

#ifdef ALLOW64
	ar->ords[3].bits = 8 * sizeof(unsigned longlong);
	ar->ords[3].name = "unsigned " str_longlong ;
	ar->ords[3].align = 0;
#endif /* ALLOW64 */

	ar->ints[0].bits = 8 * sizeof(signed char);
	ar->ints[0].name = "signed char";
	ar->ints[0].align = 1;

	ar->ints[1].bits = 8 * sizeof(short);
	ar->ints[1].name = "short";
	ar->ints[1].align = 1;

	ar->ints[2].bits = 8 * sizeof(int);
	ar->ints[2].name = "int";
	ar->ints[2].align = 1;

#ifdef ALLOW64
	ar->ints[3].bits = 8 * sizeof(longlong);
	ar->ints[3].name = str_longlong ;
	ar->ints[3].align = 0;
#endif /* ALLOW64 */


#else /* Currently assume x86 type */

	ar->uwa    = 0;		/* Use wide memory access */
	ar->shfm   = 0;		/* Use shifts to mask values */
	ar->oscale = 8;		/* Has scaled indexing up to * 8 */
	ar->smmul  = 0;		/* Doesn't have fast small multiply for index scaling */
/* ~~99 */
	ar->nords  = 3;		/* Number of ord types */
	ar->natord = 2;		/* Most natural type (assume unsigned int) */
	ar->nints  = 3;		/* Number of int types */
	ar->natint = 2;		/* Most natural type (assume int) */

	ar->pbits = sizeof(void *) * 8;		/* Number of bits in a pointer */

	ar->ords[0].bits = 8 * sizeof(unsigned char);
	ar->ords[0].name = "unsigned char";
	ar->ords[0].align = 1;

	ar->ords[1].bits = 8 * sizeof(unsigned short);
	ar->ords[1].name = "unsigned short";
	ar->ords[1].align = 1;

	ar->ords[2].bits = 8 * sizeof(unsigned int);
	ar->ords[2].name = "unsigned int";
	ar->ords[2].align = 1;

#ifdef ALLOW64
	ar->ords[3].bits = 8 * sizeof(unsigned longlong);
	ar->ords[3].name = "unsigned " str_longlong ;
	ar->ords[3].align = 0;
#endif /* ALLOW64 */

	ar->ints[0].bits = 8 * sizeof(signed char);
	ar->ints[0].name = "signed char";
	ar->ints[0].align = 1;

	ar->ints[1].bits = 8 * sizeof(short);
	ar->ints[1].name = "short";
	ar->ints[1].align = 1;

	ar->ints[2].bits = 8 * sizeof(int);
	ar->ints[2].name = "int";
	ar->ints[2].align = 1;

#ifdef ALLOW64
	ar->ints[3].bits = 8 * sizeof(longlong);
	ar->ints[3].name = str_longlong ;
	ar->ints[3].align = 0;
#endif /* ALLOW64 */

#endif	/* Processor Architecture */
}
