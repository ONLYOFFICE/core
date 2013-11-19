/* Copyright (C) 2001-2006 Artifex Software, Inc.
   All Rights Reserved.
  
   This software is provided AS-IS with no warranty, either express or
   implied.

   This software is distributed under license and may not be copied, modified
   or distributed except as expressly authorized under the terms of that
   license.  Refer to licensing information at http://www.artifex.com/
   or contact Artifex Software, Inc.,  7 Mt. Lassen Drive - Suite A-134,
   San Rafael, CA  94903, U.S.A., +1(415)492-9861, for further information.
*/

/* $Id: zcolor.h  $ */
/* Definitions for setcolorspace */

#ifndef zcolor_INCLUDED
#  define zcolor_INCLUDED

/* 
 * The code to set color space and color values has been moved from PostScript
 * into C (mostly). The new C code broadly follows the old PostScript method, each
 * possible color space is defined by an instance of the PS_colour_space_s object below. These
 * are stored in an array (in zcolor.c) called colorProcs. When color spaces or 
 * color values are set, or values retrieved, we examine the array or name which 
 * represents the space, and extract a C string representation of the color space 
 * name. We compare the name with each of the names in the colorProcs array to 
 * retrieve the instance which handles that color space.
 *
 * When setting a new color space, we must follow the existing Ghostscript policy 
 * which requires us to set the spaces 'backwards'. That is, before we set a space
 * which has an alternate color space (eg Separation) we must first set the 
 * alternate space as the current. 
 *
 * Now, when setting a color space, we convert any tint transform procedure from 
 * a PostScript procedure into a PostScript Function, either a type 4 PostScript
 * calculator or a type 0 sampled function. This has performance benefits, especially
 * wehn dealing with images. Now, if we are converting into a type 4 function this is
 * easy, however a type 0 function requires us to sample the color space and in order
 * to do this, we must actually execute the tint transform procedure. This means we
 * must exit the C world and hand control back to the PostScript interpreter 
 * temporarily.
 *
 * We do this with a 'continuation function', we store our procdure on the PostScript
 * execution stack, and when we need to run a tint transform, we push the procedure
 * onto the execution stack after our function, and exit back to the interpreter.
 * When the interpreter has finished executing the transform, it executes the next 
 * entry on the execution stack, which is our C procedure, and returns control back
 * to our code. Of course, we need to know what stage of processing we were up to
 * and so we also store some variables on the execution stack. Continuation 
 * procedures are basically state machines.
 *
 * In fact, we need quite a few of these, for setting the color space, the current
 * color (we may need to execute tint transforms), converting the current color into
 * a device space (for currentcmykcolor and so on). These are all defined in zcolor.c.
 */
typedef struct PS_colour_space_s PS_colour_space_t;
struct PS_colour_space_s {
    char *name;					/* C string representing the name of the space */
    int (*setproc)(i_ctx_t * i_ctx_p, ref *r,	/* Routine to set the color space, if CIESubst */ 
	int *stage, int *cont, int CIESubst);	/* is true then we are already doing CIE substitution */
    int (*validateproc)(i_ctx_t * i_ctx_p,	/* Validates the color space operands */
	ref **r);	
    int (*alternateproc)(i_ctx_t * i_ctx_p,	/* Retrieve the alternate color space (if any) */ 
	ref *space, ref **r, int *CIESubst);	/* If CIESubst comes back true, then don't do further CIE substitution */
    int (*numcomponents)(i_ctx_t * i_ctx_p,	/* Returns the number of components in the space */
	ref *space, int *n);
    int (*range)(i_ctx_t * i_ctx_p, ref *space, /* Returns 'components' pairs of values which represent */
	float *ptr);				/* the valid ranges of values for this space */
    int (*domain)(i_ctx_t * i_ctx_p,		/* Returns 'components' pairs of values which represent */ 
	ref *space, float *ptr);		/* the domain over which values are valid */
    int (*basecolorproc)(i_ctx_t * i_ctx_p,	/* convert the current color values into device space */ 
	ref *space, int base, int *stage,	/* values. 'base' is the requested base space, 0=gray */
	int *cont, int *stack_depth);		/* 1 = HSB, 2 = RGB, 3 = CMYK. Separation and DeviceN */
						/* spaces will return default values if they are not using */
						/* the alternate space, and will convert the components */
						/* into values in the alternate otherwise, by executing */
						/* the tint transform procedure */
    int (*runtransformproc)(i_ctx_t *i_ctx_p,	/* executes the tint transform for this space */ 
	ref *space, int *usealternate, 
	int *stage, int *stack_depth);
    int (*validatecomponents)(i_ctx_t *i_ctx_p, /* Check the components supplied as an argument to setcolor */
	ref *space, float *values,		/* are valid */
	int num_comps);				
    int (*compareproc)(i_ctx_t *i_ctx_p,	/* Compare two color spaces of this type, to see if they */ 
	ref *space, ref *testspace);		/* are the same */
    int (*initialcolorproc)(i_ctx_t *i_ctx_p,	/* Set the appropriate initial color for this space */ 
	ref *space);
};    


/* Given a pointer to a color space (name or array), returns
 * the appropriate instance of PS_colour_space_s from the colorProcs array
 */
int get_space_object(i_ctx_t *i_ctx_p, ref *arr, PS_colour_space_t **obj);

#endif /* zcolor_INCLUDED */
