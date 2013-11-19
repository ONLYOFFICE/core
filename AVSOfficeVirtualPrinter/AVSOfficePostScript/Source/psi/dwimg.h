/* Copyright (C) 2001-2008 Artifex Software, Inc.
   All Rights Reserved.

   This software is provided AS-IS with no warranty, either express or
   implied.

   This software is distributed under license and may not be copied, modified
   or distributed except as expressly authorized under the terms of that
   license.  Refer to licensing information at http://www.artifex.com/
   or contact Artifex Software, Inc.,  7 Mt. Lassen Drive - Suite A-134,
   San Rafael, CA  94903, U.S.A., +1(415)492-9861, for further information.
*/
/* $Id: dwimg.h 9043 2008-08-28 22:48:19Z giles $ */

#ifndef dwimg_INCLUDED
#  define dwimg_INCLUDED


/* Windows Image Window structure */

typedef struct IMAGE_DEVICEN_S IMAGE_DEVICEN;
struct IMAGE_DEVICEN_S {
    int used;		/* non-zero if in use */
    int visible;	/* show on window */
    char name[64];
    int cyan;
    int magenta;
    int yellow;
    int black;
    int menu;		/* non-zero if menu item added to system menu */
};
#define IMAGE_DEVICEN_MAX 8

typedef struct IMAGE_S IMAGE;
struct IMAGE_S {
    void *handle;
    void *device;
    HWND hwnd;
    HBRUSH hBrush;	/* background */
    int raster;
    unsigned int format;
    unsigned char *image;
    BITMAPINFOHEADER bmih;
    HPALETTE palette;
    int bytewidth;
    int devicen_gray;	/* true if a single separation should be shown gray */
    IMAGE_DEVICEN devicen[IMAGE_DEVICEN_MAX];

    /* periodic redrawing */
    UINT update_timer;		/* identifier */
    int update_tick;		/* timer duration in milliseconds */
    int update_count;		/* Number of WM_TIMER messages received */
    int update_interval;	/* Number of WM_TIMER until refresh */
    int pending_update;		/* We have asked for periodic updates */
    int pending_sync;		/* We have asked for a SYNC */

    /* Window scrolling stuff */
    int cxClient, cyClient;
    int cxAdjust, cyAdjust;
    int nVscrollPos, nVscrollMax;
    int nHscrollPos, nHscrollMax;

    /* thread synchronisation */
    HANDLE hmutex;

    IMAGE *next;

    HWND hwndtext;	/* handle to text window */

    int x, y, cx, cy;	/* window position */
};

extern IMAGE *first_image;

/* Main thread only */
IMAGE *image_find(void *handle, void *device);
IMAGE *image_new(void *handle, void *device);
void image_delete(IMAGE *img);
int image_size(IMAGE *img, int new_width, int new_height, int new_raster,
   unsigned int new_format, void *pimage);
int image_separation(IMAGE *img, int comp_num, const char *name,
   unsigned short c, unsigned short m, unsigned short y, unsigned short k);

/* GUI thread only */
void image_open(IMAGE *img);
void image_close(IMAGE *img);
void image_sync(IMAGE *img);
void image_page(IMAGE *img);
void image_presize(IMAGE *img, int new_width, int new_height, int new_raster,
   unsigned int new_format);
void image_poll(IMAGE *img);
void image_updatesize(IMAGE *img);

/* To be called during initialization after the text window has been created */
void image_textwindow(HWND hwnd);


#endif /* dwimg_INCLUDED */
