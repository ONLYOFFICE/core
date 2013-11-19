
/* 
 * Color Correct a TIFF file, using an ICC Device link profile.
 *
 * Author:  Graeme W. Gill
 * Date:    00/3/8
 * Version: 1.30
 *
 * Copyright 2000 - 2004 Graeme W. Gill
 * All rights reserved.
 *
 * This material is licenced under the GNU GENERAL PUBLIC LICENCE :-
 * see the Licence.txt file for licencing details.
 */

/*
 * Thanks to Neil Okamoto for the 16 bit TIFF mods.
 */

/* TTBD:
 */

/*
	This program is a framework that exercises the
	IMDI code, as well as a demonstration of simple
    profile linking.  It can also do the conversion using the
    floating point code in ICCLIB as a reference.

 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <fcntl.h>
#include <string.h>
#include <math.h>
#include "copyright.h"
#include "config.h"
#include "tiffio.h"
#include "icc.h"
#include "imdi.h"

#undef TREAT_CMY_AS_RGB

void error(char *fmt, ...), warning(char *fmt, ...);

void usage(void) {
	fprintf(stderr,"Color Correct a TIFF file using an ICC device link profile, V%s\n",ARGYLL_VERSION_STR);
	fprintf(stderr,"Author: Graeme W. Gill, licensed under the GPL\n");
	fprintf(stderr,"usage: cctiff [-options] devlinkprofile.icm infile.tif outfile.tif\n");
	fprintf(stderr,"usage: cctiff [-options] -l inprofile.icm outprofile.icm infile.tif outfile.tif\n");
	fprintf(stderr," -v            Verbose\n");
	fprintf(stderr," -c            Combine linearisation curves into one transform\n");
	fprintf(stderr," -p            Use slow precise correction\n");
	fprintf(stderr," -k            Check fast result against precise, and report\n");
	fprintf(stderr," -l            Link input and output profiles\n"); 
	fprintf(stderr,"  -i in_intent  p = perceptual, r = relative colorimetric,\n");
	fprintf(stderr,"                s = saturation, a = absolute colorimetric\n");
	fprintf(stderr,"  -o out_intent p = perceptual, r = relative colorimetric,\n");
	fprintf(stderr,"                s = saturation, a = absolute colorimetric\n");
	exit(1);
}

/* Convert an ICC colorspace to the corresponding possible TIFF Photometric tags. */
/* Return the number of matching tags, and 0 if there is no corresponding tag. */
int
ColorSpaceSignature2TiffPhotometric(
uint16 tags[10],					/* Pointer to return array, up to 10 */
icColorSpaceSignature cspace	/* Input ICC colorspace */
) {
	switch(cspace) {
		case icSigGrayData:
			tags[0] = PHOTOMETRIC_MINISBLACK;
			return 1;
		case icSigRgbData:
#ifdef TREAT_CMY_AS_RGB
		case icSigCmyData:
#endif
			tags[0] = PHOTOMETRIC_RGB;
			return 1;
#ifndef TREAT_CMY_AS_RGB
		case icSigCmyData:
#endif
		case icSigCmykData:
			tags[0] = PHOTOMETRIC_SEPARATED;
			return 1;
		case icSigYCbCrData:
			tags[0] = PHOTOMETRIC_YCBCR;
			return 1;
		case icSigLabData:
			tags[0] = PHOTOMETRIC_CIELAB;
#ifdef PHOTOMETRIC_ICCLAB
			tags[1] = PHOTOMETRIC_ICCLAB;
			tags[2] = PHOTOMETRIC_ITULAB;
#endif
			return 3;

		case icSigXYZData:
		case icSigLuvData:
		case icSigYxyData:
		case icSigHsvData:
		case icSigHlsData:
			return 0;

		case icSig2colorData:
			tags[0] = PHOTOMETRIC_SEPARATED;
			tags[1] = 2;			/* Cheat */
			return 1;

		case icSig3colorData:
			tags[0] = PHOTOMETRIC_SEPARATED;
			tags[1] = 3;			/* Cheat */
			return 1;

		case icSig4colorData:
			tags[0] = PHOTOMETRIC_SEPARATED;
			tags[1] = 4;			/* Cheat */
			return 1;

		case icSig5colorData:
		case icSigMch5Data:
			tags[0] = PHOTOMETRIC_SEPARATED;
			tags[1] = 5;			/* Cheat */
			return 1;

		case icSig6colorData:
		case icSigMch6Data:
			tags[0] = PHOTOMETRIC_SEPARATED;
			tags[1] = 6;			/* Cheat */
			return 1;

		case icSig7colorData:
		case icSigMch7Data:
			tags[0] = PHOTOMETRIC_SEPARATED;
			tags[1] = 7;			/* Cheat */
			return 1;

		case icSig8colorData:
		case icSigMch8Data:
			tags[0] = PHOTOMETRIC_SEPARATED;
			tags[1] = 8;			/* Cheat */
			return 1;

		case icSig9colorData:
			tags[0] = PHOTOMETRIC_SEPARATED;
			tags[1] = 9;			/* Cheat */
			return 1;

		case icSig10colorData:
			tags[0] = PHOTOMETRIC_SEPARATED;
			tags[1] = 10;			/* Cheat */
			return 1;

		case icSig11colorData:
			tags[0] = PHOTOMETRIC_SEPARATED;
			tags[1] = 11;			/* Cheat */
			return 1;

		case icSig12colorData:
			tags[0] = PHOTOMETRIC_SEPARATED;
			tags[1] = 12;			/* Cheat */
			return 1;

		case icSig13colorData:
			tags[0] = PHOTOMETRIC_SEPARATED;
			tags[1] = 13;			/* Cheat */
			return 1;

		case icSig14colorData:
			tags[0] = PHOTOMETRIC_SEPARATED;
			tags[1] = 14;			/* Cheat */
			return 1;

		case icSig15colorData:
			tags[0] = PHOTOMETRIC_SEPARATED;
			tags[1] = 15;			/* Cheat */
			return 1;

		default:
			return 0;
	}
	return 0;
}


/* Compute the length of a double nul terminated string, including */
/* the nuls. */
static int zzstrlen(char *s) {
	int i;
	for (i = 0;; i++) {
		if (s[i] == '\000' && s[i+1] == '\000')
			return i+2;
	}
	return 0;
}

/* Convert an ICC colorspace to the corresponding TIFF Inkset tag */
/* return 0xffff if not possible or applicable. */

int
ColorSpaceSignature2TiffInkset(
icColorSpaceSignature cspace,
int *len,						/* Return length of ASCII inknames */
char **inknames					/* Return ASCII inknames if non NULL */
) {
	switch(cspace) {
		case icSigCmyData:
			return 0xffff;	/* ~~9999 */
			if (inknames != NULL) {
				*inknames = "cyan\000magenta\000yellow\000\000";
				*len = zzstrlen(*inknames);
			}
			return 0;				/* Not CMYK */
		case icSigCmykData:
			if (inknames != NULL) {
				*inknames = NULL;	/* No inknames */
				*len = 0;
			}
			return INKSET_CMYK;

		case icSigGrayData:
		case icSigRgbData:
		case icSigYCbCrData:
		case icSigLabData:
		case icSigXYZData:
		case icSigLuvData:
		case icSigYxyData:
		case icSigHsvData:
		case icSigHlsData:
		case icSig2colorData:
		case icSig3colorData:
		case icSig4colorData:
		case icSig5colorData:
		case icSigMch5Data:
			return 0xffff;

		case icSig6colorData:
		case icSigMch6Data:
				/* This is a cheat and a hack. Should really make sure that icclink */
				/* transfers the right information from the destination */
				/* profile, and then copies it to the device profile, */
				/* allowing cctiff to read it. */
			if (inknames != NULL) {
				*inknames = "cyan\000magenta\000yellow\000black\000orange\000green\000\000";
				*len = zzstrlen(*inknames);
			}
			return 0;				/* Not CMYK */

		case icSig7colorData:
		case icSigMch7Data:
			return 0xffff;

		case icSig8colorData:
		case icSigMch8Data:
				/* This is a cheat and a hack. Should really make sure that icclink */
				/* transfers the right information from the destination */
				/* profile, and then copies it to the device profile, */
				/* allowing cctiff to read it. */
			if (inknames != NULL) {
				*inknames = "cyan\000magenta\000yellow\000black\000orange\000green\000lightcyan\000lightmagenta\000\000";
				*len = zzstrlen(*inknames);
			}
			return 0;				/* Not CMYK */
		case icSig9colorData:
		case icSig10colorData:
		case icSig11colorData:
		case icSig12colorData:
		case icSig13colorData:
		case icSig14colorData:
		case icSig15colorData:
		default:
			return 0xffff;
	}
	return 0xffff;
}

char *
Photometric2str(
int pmtc
) {
	static char buf[80];
	switch (pmtc) {
		case PHOTOMETRIC_MINISWHITE:
			return "Subtractive Gray";
		case PHOTOMETRIC_MINISBLACK:
			return "Additive Gray";
		case PHOTOMETRIC_RGB:
			return "RGB";
		case PHOTOMETRIC_PALETTE:
			return "Indexed";
		case PHOTOMETRIC_MASK:
			return "Transparency Mask";
		case PHOTOMETRIC_SEPARATED:
			return "Separated";
		case PHOTOMETRIC_YCBCR:
			return "YCbCr";
		case PHOTOMETRIC_CIELAB:
			return "CIELab";
#ifdef PHOTOMETRIC_ICCLAB
		case PHOTOMETRIC_ICCLAB:
			return "ICCLab";
		case PHOTOMETRIC_ITULAB:
			return "ITULab";
#endif
		case PHOTOMETRIC_LOGL:
			return "CIELog2L";
		case PHOTOMETRIC_LOGLUV:
			return "CIELog2Luv";
	}
	sprintf(buf,"Unknonw Tag %d",pmtc);
	return buf;
}

/* Callbacks used to initialise imdi */

/* Information needed from a profile */
struct _profinfo {
	char name[100];
	icmFile *fp;
	icc *c;
	icmHeader *h;
	icRenderingIntent intent;
	icmLuBase *luo;				/* Base Lookup type object */
	icmLuAlgType alg;			/* Type of lookup algorithm */
	int chan;					/* Device channels */
}; typedef struct _profinfo profinfo;

/* Context for imdi setup callbacks */
typedef struct {
	/* Overall parameters */
	int verb;			/* Non-zero if verbose */
	icColorSpaceSignature ins, outs;	/* Input/Output spaces */
	int id, od;			/* Input/Output dimensions */
	int icombine;		/* Non-zero if input curves are to be combined */
	int ocombine;		/* Non-zero if output curves are to be combined */
	int link;			/* Non-zero if input and output profiles are to be linked */

	profinfo dev;		/* Device link profile */
	profinfo in;		/* Device to PCS profile */
	profinfo out;		/* PCS to Device profile */
} sucntx;

/* Input curve function */
double input_curve(
	void *cntx,
	int ch,
	double in_val
) {
	sucntx *rx = (sucntx *)cntx;
	int i;
	double vals[MAX_CHAN];

	if (rx->icombine)
		return in_val;

	if (rx->link) {

		for (i = 0; i < rx->id; i++)
			vals[i] = 0.0;
		vals[ch] = in_val;

		switch(rx->in.alg) {
		    case icmMonoFwdType: {
				icmLuMono *lu = (icmLuMono *)rx->in.luo;	/* Safe to coerce */
				lu->fwd_curve(lu, vals, vals);
				break;
			}
		    case icmMatrixFwdType: {
				icmLuMatrix *lu = (icmLuMatrix *)rx->in.luo;	/* Safe to coerce */
				lu->fwd_curve(lu, vals, vals);
				break;
			}
		    case icmLutType: {
				icmLuLut *lu = (icmLuLut *)rx->in.luo;	/* Safe to coerce */
				/* Since not PCS, in_abs and matrix cannot be valid, */
				/* so input curve on own is ok to use. */
				lu->input(lu, vals, vals);
				break;
			}
			default:
				error("Unexpected algorithm type in input curve");
		}
	} else {
		icmLuLut *lu = (icmLuLut *)rx->dev.luo;		/* Safe to coerce */
	
		for (i = 0; i < rx->id; i++)
			vals[i] = 0.0;
		vals[ch] = in_val;
	
		/* Since input not PCS, in_abs and matrix cannot be valid, */
		/* so input curve on own is ok to use. */
		lu->input(lu, vals, vals);
	
	}
	return vals[ch];
}

/* Multi-dim table function */
void md_table(
void *cntx,
double *out_vals,
double *in_vals
) {
	sucntx *rx = (sucntx *)cntx;
	double pcsv[3];
	int i;
	
	if (rx->link) {
		double vals[MAX_CHAN];

		switch(rx->in.alg) {
		    case icmMonoFwdType: {
				icmLuMono *lu = (icmLuMono *)rx->in.luo;	/* Safe to coerce */
				if (rx->icombine) {
					lu->fwd_curve(lu, vals, in_vals);
					lu->fwd_map(lu, pcsv, vals);
				} else {
					lu->fwd_map(lu, pcsv, in_vals);
				}
				lu->fwd_abs(lu, pcsv, pcsv);
				break;
			}
		    case icmMatrixFwdType: {
				icmLuMatrix *lu = (icmLuMatrix *)rx->in.luo;	/* Safe to coerce */
				if (rx->icombine) {
					lu->fwd_curve(lu, vals, in_vals);
					lu->fwd_matrix(lu, pcsv, vals);
				} else {
					lu->fwd_matrix(lu, pcsv, in_vals);
				}
				lu->fwd_abs(lu, pcsv, pcsv);
				break;
			}
		    case icmLutType: {
				icmLuLut *lu = (icmLuLut *)rx->in.luo;	/* Safe to coerce */
				if (rx->icombine) {
					/* Since not PCS, in_abs and matrix cannot be valid, */
					/* so input curve on own is ok to use. */
					lu->input(lu, vals, in_vals);
					lu->clut(lu, pcsv, vals);
				} else {
					lu->clut(lu, pcsv, in_vals);
				}
				lu->output(lu, pcsv, pcsv);
				lu->out_abs(lu, pcsv, pcsv);
				break;
			}
			default:
				error("Unexpected algorithm type in clut lookup");
		}

		switch(rx->out.alg) {
		    case icmMonoBwdType: {
				icmLuMono *lu = (icmLuMono *)rx->out.luo;	/* Safe to coerce */
				lu->bwd_abs(lu, pcsv, pcsv);
				lu->bwd_map(lu, out_vals, pcsv);
				if (rx->ocombine) {
					lu->bwd_curve(lu, out_vals, out_vals);
				}
				break;
			}
		    case icmMatrixBwdType: {
				icmLuMatrix *lu = (icmLuMatrix *)rx->out.luo;	/* Safe to coerce */
				lu->bwd_abs(lu, pcsv, pcsv);
				lu->bwd_matrix(lu, out_vals, pcsv);
				if (rx->ocombine) {
					lu->bwd_curve(lu, out_vals, out_vals);
				}
				break;
			}
		    case icmLutType: {
				icmLuLut *lu = (icmLuLut *)rx->out.luo;	/* Safe to coerce */
				lu->in_abs(lu, pcsv, pcsv);
				lu->matrix(lu, pcsv, pcsv);
				lu->input(lu, pcsv, pcsv);
				lu->clut(lu, out_vals, pcsv);
				if (rx->ocombine) {
					lu->output(lu, out_vals, out_vals);
					/* Since not PCS, out_abs is never used */
				}
				break;
			}

			default:
				error("Unexpected algorithm type in clut lookup");
		}
	} else {
		icmLuLut *lu = (icmLuLut *)rx->dev.luo;		/* Safe to coerce */

		if (rx->icombine && rx->ocombine) {
			lu->lookup((icmLuBase *)lu, out_vals, in_vals);		/* Do everything here */
		} else {
			lu->clut(lu, out_vals, in_vals);
		}
	}
}

/* Output curve function */
double output_curve(
void *cntx,
int ch,
double in_val
) {
	sucntx *rx = (sucntx *)cntx;
	int i;
	double vals[MAX_CHAN];
	
	if (rx->ocombine)
		return in_val;

	if (rx->link) {
		for (i = 0; i < rx->od; i++)
			vals[i] = 0.0;
		vals[ch] = in_val;
	
		switch(rx->out.alg) {
		    case icmMonoBwdType: {
				icmLuMono *lu = (icmLuMono *)rx->out.luo;	/* Safe to coerce */
				lu->bwd_curve(lu, vals, vals);
				break;
			}
		    case icmMatrixBwdType: {
				icmLuMatrix *lu = (icmLuMatrix *)rx->out.luo;	/* Safe to coerce */
				lu->bwd_curve(lu, vals, vals);
				break;
			}
		    case icmLutType: {
				icmLuLut *lu = (icmLuLut *)rx->out.luo;	/* Safe to coerce */
				lu->output(lu, vals, vals);
				/* Since not PCS, out_abs is never used */
				break;
			}
			default:
				error("Unexpected algorithm type in devop_devo()");
		}

	} else {
		icmLuLut *lu = (icmLuLut *)rx->dev.luo;		/* Safe to coerce */
	
		for (i = 0; i < rx->od; i++)
			vals[i] = 0.0;
		vals[ch] = in_val;
	
		/* Since output not PCS, out_abs cannot be valid, */
		lu->output(lu, vals, vals);
	
	}
	return vals[ch];
}


int
main(int argc, char *argv[]) {
	int fa,nfa;				/* argument we're looking at */
	char in_name[100];		/* Raster file name */
	char out_name[100];		/* Raster file name */
	int slow = 0;
	int check = 0;
	int i, rv = 0;

	TIFF *rh = NULL, *wh = NULL;
	int x, y, width, height;					/* Size of image */
	uint16 samplesperpixel, bitspersample;
	int no_pmtc;								/* Number of input photometrics */
	uint16 photometric, pmtc[10];				/* Photometrics of input file, and input profile */
	uint16 pconfig;								/* Planar configuration */
	uint16 resunits;
	float resx, resy;
	tdata_t *inbuf, *outbuf, *checkbuf;

	/* IMDI */
	imdi *s = NULL;
	sucntx su;		/* Setup context */
	unsigned char *inp[MAX_CHAN];
	unsigned char *outp[MAX_CHAN];
	int clutres = 33;

	/* Error check */
	int mxerr = 0;
	double avgerr = 0.0;
	double avgcount = 0.0;

	if (argc < 2)
		usage();

	su.verb = 0;
	su.icombine = 0;
	su.ocombine = 0;
	su.link = 0;
	su.in.intent = icmDefaultIntent;
	su.out.intent = icmDefaultIntent;

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

			/* Slow, Precise */
			else if (argv[fa][1] == 'p' || argv[fa][1] == 'P') {
				slow = 1;
			}

			/* Combine per channel curves */
			else if (argv[fa][1] == 'c' || argv[fa][1] == 'C') {
				su.icombine = 1;
				su.ocombine = 1;
			}

			/* Check curves */
			else if (argv[fa][1] == 'k' || argv[fa][1] == 'K') {
				check = 1;
			}

			/* Link profiles */
			else if (argv[fa][1] == 'l' || argv[fa][1] == 'L') {
				su.link = 1;
			}

			/* Input profile Intent */
			else if (argv[fa][1] == 'i' || argv[fa][1] == 'I') {
				fa = nfa;
				if (na == NULL) usage();
    			switch (na[0]) {
					case 'p':
					case 'P':
						su.in.intent = icPerceptual;
						break;
					case 'r':
					case 'R':
						su.in.intent = icRelativeColorimetric;
						break;
					case 's':
					case 'S':
						su.in.intent = icSaturation;
						break;
					case 'a':
					case 'A':
						su.in.intent = icAbsoluteColorimetric;
						break;
					default:
						usage();
				}
			}

			/* Output profile Intent */
			else if (argv[fa][1] == 'o' || argv[fa][1] == 'O') {
				fa = nfa;
				if (na == NULL) usage();
    			switch (na[0]) {
					case 'p':
					case 'P':
						su.out.intent = icPerceptual;
						break;
					case 'r':
					case 'R':
						su.out.intent = icRelativeColorimetric;
						break;
					case 's':
					case 'S':
						su.out.intent = icSaturation;
						break;
					case 'a':
					case 'A':
						su.out.intent = icAbsoluteColorimetric;
						break;
					default:
						usage();
				}
			}

			/* Verbosity */
			else if (argv[fa][1] == 'v' || argv[fa][1] == 'V') {
				su.verb = 1;
			}

			else 
				usage();
		} else
			break;
	}

	if (su.link) {
		if (fa >= argc || argv[fa][0] == '-') usage();
		strcpy(su.in.name,argv[fa++]);

		if (fa >= argc || argv[fa][0] == '-') usage();
		strcpy(su.out.name,argv[fa++]);
	} else {
		if (fa >= argc || argv[fa][0] == '-') usage();
		strcpy(su.dev.name,argv[fa++]);
	}

	if (fa >= argc || argv[fa][0] == '-') usage();
	strcpy(in_name,argv[fa++]);

	if (fa >= argc || argv[fa][0] == '-') usage();
	strcpy(out_name,argv[fa++]);

	/* - - - - - - - - - - - - - - - - */

	if (su.link) {
		icColorSpaceSignature natpcs;

		/* Open up the input device profile for reading */
		if ((su.in.fp = new_icmFileStd_name(su.in.name,"r")) == NULL)
			error ("Can't open file '%s'",su.in.name);
		
		if ((su.in.c = new_icc()) == NULL)
			error ("Creation of Input profile ICC object failed");
	
		/* Read header etc. */
		if ((rv = su.in.c->read(su.in.c,su.in.fp,0)) != 0)
			error ("%d, %s on file '%s'",rv,su.in.c->err,su.in.name);
		su.in.h = su.in.c->header;
	
		/* Check that it is a suitable device input icc */
		if (su.in.h->deviceClass != icSigInputClass
		 && su.in.h->deviceClass != icSigDisplayClass
		 && su.in.h->deviceClass != icSigOutputClass
		 && su.in.h->deviceClass != icSigColorSpaceClass)	/* For sRGB etc. */
			error("Input profile isn't a device profile");
	
		/* Get a conversion object */
		if ((su.in.luo = su.in.c->get_luobj(su.in.c, icmFwd, su.in.intent,
		                                             icSigLabData, icmLuOrdNorm)) == NULL)
			error ("%d, %s for profile '%s'",su.in.c->errc, su.in.c->err, su.in.name);
	
		/* Get details of conversion (Arguments may be NULL if info not needed) */
		su.in.luo->spaces(su.in.luo, &su.ins, &su.id, NULL, NULL, &su.in.alg, NULL, NULL, NULL);

		/* Get native PCS space */
		su.in.luo->lutspaces(su.in.luo, NULL, NULL, NULL, NULL, &natpcs);
	
		if (natpcs == icSigXYZData) {
			su.icombine = 1;			/* XYZ is to non-linear to be a benefit */
		}

		/* Open up the output device profile for reading */
		if ((su.out.fp = new_icmFileStd_name(su.out.name,"r")) == NULL)
			error ("Can't open file '%s'",su.out.name);
		
		if ((su.out.c = new_icc()) == NULL)
			error ("Creation of Output profile ICC object failed");
	
		/* Read header etc. */
		if ((rv = su.out.c->read(su.out.c,su.out.fp,0)) != 0)
			error ("%d, %s on file '%s'",rv,su.out.c->err,su.out.name);
		su.out.h = su.out.c->header;
	
		/* Check that it is a suitable device output icc */
		if (su.out.h->deviceClass != icSigInputClass
		 && su.out.h->deviceClass != icSigDisplayClass
		 && su.out.h->deviceClass != icSigOutputClass
		 && su.out.h->deviceClass != icSigColorSpaceClass)	/* For sRGB etc. */
			error("Output profile isn't a device profile");
	
		/* Get a conversion object */
		if ((su.out.luo = su.out.c->get_luobj(su.out.c, icmBwd, su.out.intent,
		                                             icSigLabData, icmLuOrdNorm)) == NULL)
			error ("%d, %s for profile '%s'",su.out.c->errc, su.out.c->err, su.out.name);
	
		/* Get details of conversion (Arguments may be NULL if info not needed) */
		su.out.luo->spaces(su.out.luo, NULL, NULL, &su.outs, &su.od, &su.out.alg, NULL, NULL, NULL);

		/* Get native PCS space */
		su.out.luo->lutspaces(su.out.luo, NULL, NULL, NULL, NULL, &natpcs);
	
		if (natpcs == icSigXYZData) {
			su.ocombine = 1;			/* XYZ is to non-linear to be a benefit */
		}

		/* See discussion in imdi/imdi_gen.c for ideal numbers */
		/* Use "high quality" resolution numbers */
		switch (su.id) {
			case 0:
				error ("Illegal number of input chanels");
			case 1:
	  		  	clutres = 256;
				break;
			case 2:
	  		  	clutres = 256;
				break;
			case 3:
	  		  	clutres = 33;
				break;
			case 4:
  		  		clutres = 18;
				break;
			case 5:
  		  		clutres = 16;
				break;
			case 6:
  		  		clutres = 9;
				break;
			case 7:
  		  		clutres = 7;
				break;
			case 8:
  		  		clutres = 6;
				break;
			deault: /* > 8 chan */
				clutres = 3;
				break;
		}	

	} else {
		icmLut *lut;						/* ICC LUT table */
		icmLuLut *luluo;					/* LUT lookup object */

		/* Open up the device link profile for reading */
		if ((su.dev.fp = new_icmFileStd_name(su.dev.name,"r")) == NULL)
			error ("Can't open file '%s'",su.dev.name);
	
		if ((su.dev.c = new_icc()) == NULL)
			error ("Creation of ICC object failed");
	
		if ((rv = su.dev.c->read(su.dev.c, su.dev.fp, 0)) != 0)
			error ("%d, %s",rv,su.dev.c->err);
		su.dev.h = su.dev.c->header;

		if (su.verb) {
			icmFile *op;
			if ((op = new_icmFileStd_fp(stdout)) == NULL)
				error ("Can't open stdout");
			su.dev.h->dump(su.dev.h, op, 1);
			op->del(op);
		}

		/* Check that the profile is appropriate */
		if (su.dev.h->deviceClass != icSigLinkClass)
			error("Profile isn't a device link profile");

		/* Get a conversion object */
		if ((su.dev.luo = su.dev.c->get_luobj(su.dev.c, icmFwd, icmDefaultIntent,
		                                             icmSigDefaultData, icmLuOrdNorm)) == NULL)
			error ("%d, %s",su.dev.c->errc, su.dev.c->err);
	
		/* Get details of conversion (Arguments may be NULL if info not needed) */
		su.dev.luo->spaces(su.dev.luo, &su.ins, &su.id, &su.outs, &su.od, &su.dev.alg, NULL, NULL, NULL);

		if (su.dev.alg != icmLutType)
			error ("DeviceLink profile doesn't have Lut !");

		luluo = (icmLuLut *)su.dev.luo;		/* Safe to coerce */
		luluo->get_info(luluo, &lut, NULL, NULL, NULL);	/* Get some details */
		clutres = lut->clutPoints;			/* Desired table resolution */
	}

	/* - - - - - - - - - - - - - - - */
	/* Open up input tiff file ready for reading */
	/* Got arguments, so setup to process the file */
	if ((rh = TIFFOpen(in_name, "r")) == NULL)
		error("error opening read file '%s'",in_name);

	TIFFGetField(rh, TIFFTAG_IMAGEWIDTH,  &width);
	TIFFGetField(rh, TIFFTAG_IMAGELENGTH, &height);

	TIFFGetField(rh, TIFFTAG_BITSPERSAMPLE, &bitspersample);
	if (bitspersample != 8 && bitspersample != 16) {
		error("TIFF Input file must be 8 or 16 bit/channel");
	}

	TIFFGetField(rh, TIFFTAG_PHOTOMETRIC, &photometric);
	if  ((no_pmtc = ColorSpaceSignature2TiffPhotometric(pmtc, su.ins)) == 0)
		error("ICC  input colorspace '%s' can't be handled by a TIFF file!",
		      icm2str(icmColorSpaceSignature, su.ins));
	for (i = 0; i < no_pmtc; i++) {
		if (pmtc[i] == photometric)
			break;				/* Matches */
	}
	if (i >= no_pmtc) {
		switch (no_pmtc) {
			case 1:
				error("ICC input colorspace '%s' doesn't match TIFF photometric '%s'!",
			      icm2str(icmColorSpaceSignature, su.ins), Photometric2str(pmtc[0]));
			case 2:
				error("ICC input colorspace '%s' doesn't match TIFF photometric '%s' or '%s'!",
			      icm2str(icmColorSpaceSignature, su.ins), Photometric2str(pmtc[0]),
			      Photometric2str(pmtc[1]));
			default:
				error("ICC input colorspace '%s' doesn't match TIFF photometric '%s', '%s' or '%s'!",
			      icm2str(icmColorSpaceSignature, su.ins), Photometric2str(pmtc[0]),
			      Photometric2str(pmtc[1]), Photometric2str(pmtc[2]));
		}
	}

	TIFFGetField(rh, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel);
	if (su.id != samplesperpixel)
		error ("TIFF Input file has %d input channels mismatched to colorspace '%s'",
		       samplesperpixel, icm2str(icmColorSpaceSignature, su.ins));

	TIFFGetField(rh, TIFFTAG_PLANARCONFIG, &pconfig);
	if (pconfig != PLANARCONFIG_CONTIG)
		error ("TIFF Input file must be planar");

	TIFFGetField(rh, TIFFTAG_RESOLUTIONUNIT, &resunits);
	TIFFGetField(rh, TIFFTAG_XRESOLUTION, &resx);
	TIFFGetField(rh, TIFFTAG_YRESOLUTION, &resy);

	/* - - - - - - - - - - - - - - - */
	if ((wh = TIFFOpen(out_name, "w")) == NULL)
		error("Can\'t create TIFF file '%s'!",out_name);
	
	TIFFSetField(wh, TIFFTAG_IMAGEWIDTH,  width);
	TIFFSetField(wh, TIFFTAG_IMAGELENGTH, height);
	TIFFSetField(wh, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
	TIFFSetField(wh, TIFFTAG_SAMPLESPERPIXEL, su.od);
	TIFFSetField(wh, TIFFTAG_BITSPERSAMPLE, bitspersample);
	TIFFSetField(wh, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
	if  ((no_pmtc = ColorSpaceSignature2TiffPhotometric(pmtc, su.outs)) == 0)
		error("TIFF file can't handle output colorspace '%s'!",
		      icm2str(icmColorSpaceSignature, su.outs));
	TIFFSetField(wh, TIFFTAG_PHOTOMETRIC, pmtc[0]);	/* Use first returned */
	if (pmtc[0] == PHOTOMETRIC_SEPARATED) {
		int iset;
		int inlen;
		char *inames;
		iset = ColorSpaceSignature2TiffInkset(su.outs, &inlen, &inames);
		if (iset != 0xffff && inlen > 0 && inames != NULL) {
			TIFFSetField(wh, TIFFTAG_INKSET, iset);
			if (inames != NULL) {
				TIFFSetField(wh, TIFFTAG_INKNAMES, inlen, inames);
			}
		}
	}
	TIFFSetField(wh, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
	if (resunits) {
		TIFFSetField(wh, TIFFTAG_RESOLUTIONUNIT, resunits);
		TIFFSetField(wh, TIFFTAG_XRESOLUTION, resx);
		TIFFSetField(wh, TIFFTAG_YRESOLUTION, resy);
	}
	TIFFSetField(wh, TIFFTAG_IMAGEDESCRIPTION, "Color corrected by Argyll");

	/* - - - - - - - - - - - - - - - */
	/* Setup the imdi */

	if (!slow) {
		s = new_imdi(
			su.id,			/* Number of input dimensions */
			su.od,			/* Number of output dimensions */
							/* Input pixel representation */
			bitspersample == 8 ? pixint8 : pixint16,
							/* Output pixel representation */
			0x0,			/* Treat every channel as unsigned */
			bitspersample == 8 ? pixint8 : pixint16,
			0x0,			/* Treat every channel as unsigned */
			clutres,		/* Desired table resolution */
			input_curve,	/* Callback functions */
			md_table,
			output_curve,
			(void *)&su		/* Context to callbacks */
		);
	
		if (s == NULL)
			error("new_imdi failed");
	}

	/* - - - - - - - - - - - - - - - */
	/* Process colors to translate */
	/* (Should fix this to process a group of lines at a time ?) */

	inbuf  = _TIFFmalloc(TIFFScanlineSize(rh));
	outbuf = _TIFFmalloc(TIFFScanlineSize(wh));
	if (check)
		checkbuf = _TIFFmalloc(TIFFScanlineSize(wh));

	inp[0] = (unsigned char *)inbuf;
	outp[0] = (unsigned char *)outbuf;

	if (!slow) {		/* Fast */
		for (y = 0; y < height; y++) {

			/* Read in the next line */
			if (TIFFReadScanline(rh, inbuf, y, 0) < 0)
				error ("Failed to read TIFF line %d",y);

			/* Do fast conversion */
			s->interp(s, (void **)outp, (void **)inp, width);
			
			if (check) {
				/* Do floating point conversion */
				for (x = 0; x < width; x++) {
					int i;
					double in[MAX_CHAN], out[MAX_CHAN];
					
					if (bitspersample == 8)
						for (i = 0; i < su.id; i++)
							in[i] = ((unsigned char *)inbuf)[x * su.id + i]/255.0;
					else
						for (i = 0; i < su.id; i++)
							in[i] = ((unsigned short *)inbuf)[x * su.id + i]/65535.0;
					
					if (su.link) {
						if ((rv = su.in.luo->lookup(su.in.luo, out, in)) > 1)
							error ("%d, %s",su.dev.c->errc,su.dev.c->err);
						if ((rv = su.out.luo->lookup(su.out.luo, out, out)) > 1)
							error ("%d, %s",su.dev.c->errc,su.dev.c->err);
					} else {
						if ((rv = su.dev.luo->lookup(su.dev.luo, out, in)) > 1)
							error ("%d, %s",su.dev.c->errc,su.dev.c->err);
					}
	
					if (bitspersample == 8)
						for (i = 0; i < su.od; i++)
							((unsigned char *)checkbuf)[x * su.od + i] = (int)(out[i] * 255.0 + 0.5);
					else
						for (i = 0; i < su.od; i++)
							((unsigned short *)checkbuf)[x * su.od + i] = (int)(out[i] * 65535.0 + 0.5);
				}
				/* Compute the errors */
				for (x = 0; x < (width * su.od); x++) {
					int err;
					if (bitspersample == 8)
						err = ((unsigned char *)outbuf)[x] - ((unsigned char *)checkbuf)[x];
					else
						err = ((unsigned short *)outbuf)[x] - ((unsigned short *)checkbuf)[x];
					if (err < 0)
						err = -err;
					if (err > mxerr)
						mxerr = err;
					avgerr += (double)err;
					avgcount++;
				}
			}
				
			if (TIFFWriteScanline(wh, outbuf, y, 0) < 0)
				error ("Failed to write TIFF line %d",y);

		}

	} else {	/* Slow but precise */
		if (bitspersample == 8) {
			for (y = 0; y < height; y++) {

				/* Read in the next line */
				if (TIFFReadScanline(rh, inbuf, y, 0) < 0)
					error ("Failed to read TIFF line %d",y);

				/* Do floating point conversion */
				for (x = 0; x < width; x++) {
					int i;
					double in[MAX_CHAN], out[MAX_CHAN];
					
					for (i = 0; i < su.id; i++) {
						in[i] = ((unsigned char *)inbuf)[x * su.id + i]/255.0;
					}
					
					if (su.link) {
						if ((rv = su.in.luo->lookup(su.in.luo, out, in)) > 1)
							error ("%d, %s",su.dev.c->errc,su.dev.c->err);
						if ((rv = su.out.luo->lookup(su.out.luo, out, out)) > 1)
							error ("%d, %s",su.dev.c->errc,su.dev.c->err);
					} else {
						if ((rv = su.dev.luo->lookup(su.dev.luo, out, in)) > 1)
							error ("%d, %s",su.dev.c->errc,su.dev.c->err);
					}

					for (i = 0; i < su.od; i++) {
						double outi = out[i];
						if (outi < 0.0)			/* Protect against sillies */
							outi = 0.0;
						else if (outi > 1.0)
							outi = 1.0;
						((unsigned char *)outbuf)[x * su.od + i] = (int)(outi * 255.0 + 0.5);
					}
				}
				if (TIFFWriteScanline(wh, outbuf, y, 0) < 0)
					error ("Failed to write TIFF line %d",y);
			}
		} else if (bitspersample == 16) {
			for (y = 0; y < height; y++) {

				/* Read in the next line */
				if (TIFFReadScanline(rh, inbuf, y, 0) < 0)
					error ("Failed to read TIFF line %d",y);

				/* Do floating point conversion */
				for (x = 0; x < width; x++) {
					int i;
					double in[MAX_CHAN], out[MAX_CHAN];
					
					for (i = 0; i < su.id; i++) {
						in[i] = ((unsigned short *)inbuf)[x * su.id + i]/65535.0;
					}
					
					if (su.link) {
						if ((rv = su.in.luo->lookup(su.in.luo, out, in)) > 1)
							error ("%d, %s",su.dev.c->errc,su.dev.c->err);
						if ((rv = su.out.luo->lookup(su.out.luo, out, out)) > 1)
							error ("%d, %s",su.dev.c->errc,su.dev.c->err);
					} else {
						if ((rv = su.dev.luo->lookup(su.dev.luo, out, in)) > 1)
							error ("%d, %s",su.dev.c->errc,su.dev.c->err);
					}

					for (i = 0; i < su.od; i++) {
						double outi = out[i];
						if (outi < 0.0)			/* Protect against sillies */
							outi = 0.0;
						else if (outi > 1.0)
							outi = 1.0;
						((unsigned short *)outbuf)[x * su.od + i] = (int)(outi * 65535.0 + 0.5);
					}
				}
				if (TIFFWriteScanline(wh, outbuf, y, 0) < 0)
					error ("Failed to write TIFF line %d",y);
			}
		}
	}

	if (check) {
		printf("Worst error = %d bits, average error = %f bits\n", mxerr, avgerr/avgcount);
		if (bitspersample == 8)
			printf("Worst error = %f%%, average error = %f%%\n",
			       mxerr/2.55, avgerr/(2.55 * avgcount));
		else
			printf("Worst error = %f%%, average error = %f%%\n",
			       mxerr/655.35, avgerr/(655.35 * avgcount));
	}

	/* Done with lookup object */
	if (s != NULL)
		s->done(s);

	if (su.link) {
		su.in.luo->del(su.in.luo);
		su.in.c->del(su.in.c);
		su.in.fp->del(su.in.fp);
		su.out.luo->del(su.out.luo);
		su.out.c->del(su.out.c);
		su.out.fp->del(su.out.fp);
	} else {
		su.dev.luo->del(su.dev.luo);
		su.dev.c->del(su.dev.c);
		su.dev.fp->del(su.dev.fp);
	}

	_TIFFfree(inbuf);
	_TIFFfree(outbuf);
	if (check)
		_TIFFfree(checkbuf);

	TIFFClose(rh);		/* Close Input file */
	TIFFClose(wh);		/* Close Output file */

	return 0;
}


/* Basic printf type error() and warning() routines */

void
error(char *fmt, ...)
{
	va_list args;

	fprintf(stderr,"cctiff: Error - ");
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
	fprintf(stderr, "\n");
	exit (-1);
}

void
warning(char *fmt, ...)
{
	va_list args;

	fprintf(stderr,"cctiff: Warning - ");
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
	fprintf(stderr, "\n");
}
