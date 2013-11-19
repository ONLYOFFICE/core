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
/*$Id: gxwts.h 8022 2007-06-05 22:23:38Z giles $ */
#ifndef gxwts_INCLUDED
#  define gxwts_INCLUDED

typedef bits16 wts_screen_sample_t;

#ifndef wts_screen_t_DEFINED
#  define wts_screen_t_DEFINED
typedef struct wts_screen_s wts_screen_t;
#endif

/* We cache intermediate results for wts_get_samples_j. In general, if these
   are set so that a band fits, then the hit rate will be excellent. */ 
#define WTS_CACHE_SIZE_X 512
#define WTS_CACHE_SIZE_Y 512

typedef enum {
    WTS_SCREEN_RAT,
    WTS_SCREEN_J,
    WTS_SCREEN_H
} wts_screen_type;

struct wts_screen_s {
    wts_screen_type type;
    int cell_width;
    int cell_height;
    int cell_shift;
    wts_screen_sample_t *samples;
};

typedef struct {
    int tag;
    int x;
    int y;
    int nsamples;
} wts_j_cache_el;

typedef struct {
    wts_screen_t base;

    /* Probabilities of "jumps". A and B jumps can happen when moving
       one pixel to the right. C and D can happen when moving one pixel
       down. */
    int pa; /* change to double? */
    int pb;
    int pc;
    int pd;

    int XA;
    int YA;
    int XB;
    int YB;
    int XC;
    int YC;
    int XD;
    int YD;

#ifdef WTS_CACHE_SIZE_X
#define WTS_SCREEN_J_SIZE_NOCACHE 68
    wts_j_cache_el xcache[WTS_CACHE_SIZE_X];
    wts_j_cache_el ycache[WTS_CACHE_SIZE_Y];
#endif
} wts_screen_j_t;

typedef struct {
    wts_screen_t base;

    /* This is the exact value that x1 and (width-x1) approximates. */
    double px;
    /* Ditto y1 and (height-y1). */
    double py;

    int x1;
    int y1;
} wts_screen_h_t;

int
wts_get_samples(wts_screen_t *ws, int x, int y,
		int *pcellx, int *pcelly, int *p_nsamples);

#endif
