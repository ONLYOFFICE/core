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

/* $Id: gp_unix.c 9077 2008-09-09 20:31:37Z giles $ */
/* Unix-specific routines for Ghostscript */

#include "pipe_.h"
#include "string_.h"
#include "time_.h"
#include "gx.h"
#include "gsexit.h"
#include "gp.h"

#ifdef HAVE_FONTCONFIG
#  include <fontconfig/fontconfig.h>
#endif

/*
 * This is the only place in Ghostscript that calls 'exit'.  Including
 * <stdlib.h> is overkill, but that's where it's declared on ANSI systems.
 * We don't have any way of detecting whether we have a standard library
 * (some GNU compilers perversely define __STDC__ but don't provide
 * an ANSI-compliant library), so we check __PROTOTYPES__ and
 * hope for the best.  We pick up getenv at the same time.
 */
#ifdef __PROTOTYPES__
#  include <stdlib.h>		/* for exit and getenv */
#else
extern void exit(int);
extern char *getenv(const char *);
#endif

#ifdef GS_DEVS_SHARED
#ifndef GS_DEVS_SHARED_DIR
#  define GS_DEVS_SHARED_DIR "/usr/lib/ghostscript/8.16"
#endif
/*
 * use shared library for drivers, always load them when starting, this
 * avoid too many modifications, and since it is supported only under linux
 * and applied as a patch (preferable).
 */
#include <sys/types.h>
#include <dirent.h>
#include <dlfcn.h>
#include <string.h>

void
gp_init(void)
{
  DIR*           dir = NULL;
  struct dirent* dirent;
  char           buff[1024];
  char*          pbuff;
  void*          handle;
  void           (*gs_shared_init)(void);

  strncpy(buff, GS_DEVS_SHARED_DIR, sizeof(buff) - 2);
  pbuff = buff + strlen(buff);
  *pbuff++ = '/'; *pbuff = '\0';

  dir = opendir(GS_DEVS_SHARED_DIR);
  if (dir == 0) return;

  while ((dirent = readdir(dir)) != 0) {
    strncpy(pbuff, dirent->d_name, sizeof(buff) - (pbuff - buff) - 1);
    if ((handle = dlopen(buff, RTLD_NOW)) != 0) {
      if ((gs_shared_init = dlsym(handle, "gs_shared_init")) != 0) {
	(*gs_shared_init)();
      } else {
      }
    }
  }

  closedir(dir);
}
#else
/* Do platform-dependent initialization. */
void
gp_init(void)
{
}
#endif

/* Do platform-dependent cleanup. */
void
gp_exit(int exit_status, int code)
{
}

/* Exit the program. */
void
gp_do_exit(int exit_status)
{
    exit(exit_status);
}

/* ------ Miscellaneous ------ */

/* Get the string corresponding to an OS error number. */
/* Unix systems support this so inconsistently that we don't attempt */
/* to figure out whether it's available. */
const char *
gp_strerror(int errnum)
{
    return NULL;
}

/* read in a MacOS 'resource' from an extended attribute. */
/* we don't try to implemented this since it requires support */
/* for Apple's HFS(+) filesystem */
int
gp_read_macresource(byte *buf, const char *filename, 
                    const uint type, const ushort id)
{
    return 0;
}

/* ------ Date and time ------ */

/* Read the current time (in seconds since Jan. 1, 1970) */
/* and fraction (in nanoseconds). */
void
gp_get_realtime(long *pdt)
{
    struct timeval tp;

#if gettimeofday_no_timezone	/* older versions of SVR4 */
    {
	if (gettimeofday(&tp) == -1) {
	    lprintf("Ghostscript: gettimeofday failed!\n");
	    tp.tv_sec = tp.tv_usec = 0;
	}
    }
#else /* All other systems */
    {
	struct timezone tzp;

	if (gettimeofday(&tp, &tzp) == -1) {
	    lprintf("Ghostscript: gettimeofday failed!\n");
	    tp.tv_sec = tp.tv_usec = 0;
	}
    }
#endif

    /* tp.tv_sec is #secs since Jan 1, 1970 */
    pdt[0] = tp.tv_sec;

    /* Some Unix systems (e.g., Interactive 3.2 r3.0) return garbage */
    /* in tp.tv_usec.  Try to filter out the worst of it here. */
    pdt[1] = tp.tv_usec >= 0 && tp.tv_usec < 1000000 ? tp.tv_usec * 1000 : 0;

#ifdef DEBUG_CLOCK
    printf("tp.tv_sec = %d  tp.tv_usec = %d  pdt[0] = %ld  pdt[1] = %ld\n",
	   tp.tv_sec, tp.tv_usec, pdt[0], pdt[1]);
#endif
}

/* Read the current user CPU time (in seconds) */
/* and fraction (in nanoseconds).  */
void
gp_get_usertime(long *pdt)
{
#if use_times_for_usertime
    struct tms tms;
    long ticks;
    const long ticks_per_sec = CLK_TCK;

    times(&tms);
    ticks = tms.tms_utime + tms.tms_stime + tms.tms_cutime + tms.tms_cstime;
    pdt[0] = ticks / ticks_per_sec;
    pdt[1] = (ticks % ticks_per_sec) * (1000000000 / ticks_per_sec);
#else
    gp_get_realtime(pdt);	/* Use an approximation on other hosts.  */
#endif
}

/* ------ Screen management ------ */

/* Get the environment variable that specifies the display to use. */
const char *
gp_getenv_display(void)
{
    return getenv("DISPLAY");
}

/* ------ Printer accessing ------ */

/* Open a connection to a printer.  See gp.h for details. */
FILE *
gp_open_printer(char fname[gp_file_name_sizeof], int binary_mode)
{
    const char *fmode = (binary_mode ? "wb" : "w");

    return (strlen(fname) == 0 ? 0 : fopen(fname, fmode));
}
FILE *
gp_open_printer_64(char fname[gp_file_name_sizeof], int binary_mode)
{
    const char *fmode = (binary_mode ? "wb" : "w");

    return (strlen(fname) == 0 ? 0 : gp_fopen_64(fname, fmode));
}

/* Close the connection to the printer. */
void
gp_close_printer(FILE * pfile, const char *fname)
{
    if (fname[0] == '|')
	pclose(pfile);
    else
	fclose(pfile);
}

/* ------ Font enumeration ------ */
 
 /* This is used to query the native os for a list of font names and
  * corresponding paths. The general idea is to save the hassle of
  * building a custom fontmap file.
  */
 

/* Mangle the FontConfig family and style information into a
 * PostScript font name */
#ifdef HAVE_FONTCONFIG
static void makePSFontName(char* family, int weight, int slant, char *buf, int bufsize)
{
    int bytesCopied, length, i;
    const char *slantname, *weightname;

    switch (slant) {
	case FC_SLANT_ROMAN:   slantname=""; break;;
	case FC_SLANT_OBLIQUE: slantname="Oblique"; break;;
	case FC_SLANT_ITALIC:  slantname="Italic"; break;;
	default:               slantname="Unknown"; break;;
    }

    switch (weight) {
	case FC_WEIGHT_MEDIUM:   weightname=""; break;;
	case FC_WEIGHT_LIGHT:    weightname="Light"; break;;
	case FC_WEIGHT_DEMIBOLD: weightname="Demi"; break;;
	case FC_WEIGHT_BOLD:     weightname="Bold"; break;;
	case FC_WEIGHT_BLACK:    weightname="Black"; break;;
	default:                 weightname="Unknown"; break;;
    }

    length = strlen(family);
    if (length >= bufsize)
	length = bufsize;
    /* Copy the family name, stripping spaces */
    bytesCopied=0;
    for (i = 0; i < length; i++)
	if (family[i] != ' ')
	    buf[bytesCopied++] = family[i];

    if ( ((slant != FC_SLANT_ROMAN) || (weight != FC_WEIGHT_MEDIUM)) \
	    && bytesCopied < bufsize )
    {
	buf[bytesCopied] = '-';
	bytesCopied++;
	if (weight != FC_WEIGHT_MEDIUM)
	{
	    length = strlen(family);
	    if ((length + bytesCopied) >= bufsize)
		length = bufsize - bytesCopied - 1;
	    strncpy(buf+bytesCopied, weightname, length);
	    bytesCopied += length;
	}
	if (slant != FC_SLANT_ROMAN)
	{
	    length = strlen(family);
	    if ((length + bytesCopied) >= bufsize)
		length = bufsize - bytesCopied - 1;
	    strncpy(buf+bytesCopied, slantname, length);
	    bytesCopied += length;
	}
    }
    buf[bytesCopied] = '\0';
}
#endif

/* State struct for font iteration
 * - passed as an opaque 'void*' through the rest of gs */
#ifdef HAVE_FONTCONFIG
typedef struct {
    int index;              /* current index of iteration over font_list */
    FcConfig* fc;           /* FontConfig library handle */
    FcFontSet* font_list;   /* FontConfig font list */
    char name[255];         /* name of last font */
} unix_fontenum_t;
#endif

void *gp_enumerate_fonts_init(gs_memory_t *mem)
{
#ifdef HAVE_FONTCONFIG
    unix_fontenum_t *state;
    FcPattern *pat;
    FcObjectSet *os;

    state = (unix_fontenum_t *)malloc(sizeof(unix_fontenum_t));
    if (state == NULL)
	return NULL;    /* Failed to allocate state */

    state->index     = 0;
    state->fc        = NULL;
    state->font_list = NULL;

    /* Load the fontconfig library */
    state->fc = FcInitLoadConfigAndFonts();
    if (state->fc == NULL) {
	free(state);
	state = NULL;
	dlprintf("destroyed state - fontconfig init failed");
	return NULL;  /* Failed to open fontconfig library */
    }

    /* load the font set that we'll iterate over */
    pat = FcPatternBuild(NULL,
	    FC_OUTLINE, FcTypeBool, 1,
	    FC_SCALABLE, FcTypeBool, 1,
	    NULL);
    os = FcObjectSetBuild(FC_FILE, FC_OUTLINE,
	    FC_FAMILY, FC_WEIGHT, FC_SLANT,
	    NULL);
    state->font_list = FcFontList(0, pat, os);
    FcPatternDestroy(pat);
    FcObjectSetDestroy(os);
    if (state->font_list == NULL) {
	free(state);
	state = NULL;
	return NULL;  /* Failed to generate font list */
    }
    return (void *)state;
#else
    return NULL;
#endif
}

int gp_enumerate_fonts_next(void *enum_state, char **fontname, char **path)
{
#ifdef HAVE_FONTCONFIG
    unix_fontenum_t* state = (unix_fontenum_t *)enum_state;
    FcChar8 *file_fc = NULL;
    FcChar8 *family_fc = NULL;
    int outline_fc, slant_fc, weight_fc;
    FcPattern *font;
    FcResult result;

    if (state == NULL) {
	return 0;   /* gp_enumerate_fonts_init failed for some reason */
    }

    if (state->index == state->font_list->nfont) {
	return 0; /* we've run out of fonts */
    }

    /* Bits of the following were borrowed from Red Hat's
     * fontconfig patch for Ghostscript 7 */
    font = state->font_list->fonts[state->index];

    result = FcPatternGetString (font, FC_FAMILY, 0, &family_fc);
    if (result != FcResultMatch || family_fc == NULL) {
	dlprintf ("DEBUG: FC_FAMILY mismatch\n");
	return 0;
    }

    result = FcPatternGetString (font, FC_FILE, 0, &file_fc);
    if (result != FcResultMatch || file_fc == NULL) {
	dlprintf ("DEBUG: FC_FILE mismatch\n");
	return 0;
    }

    result = FcPatternGetBool (font, FC_OUTLINE, 0, &outline_fc);
    if (result != FcResultMatch) {
	dlprintf1 ("DEBUG: FC_OUTLINE failed to match on %s\n", (char*)family_fc);
	return 0;
    }

    result = FcPatternGetInteger (font, FC_SLANT, 0, &slant_fc);
    if (result != FcResultMatch) {
	dlprintf ("DEBUG: FC_SLANT didn't match\n");
	return 0;
    }

    result = FcPatternGetInteger (font, FC_WEIGHT, 0, &weight_fc);
    if (result != FcResultMatch) {
	dlprintf ("DEBUG: FC_WEIGHT didn't match\n");
	return 0;
    }

    /* Gross hack to work around Fontconfig's inability to tell
     * us the font's PostScript name - generate it ourselves.
     * We must free the memory allocated here next time around. */
    makePSFontName((char *)family_fc, weight_fc, slant_fc,
	(char *)&state->name, sizeof(state->name));
    *fontname = (char *)&state->name;

    /* return the font path straight out of fontconfig */
    *path = (char*)file_fc;

    state->index ++;
    return 1;
#else
    return 0;
#endif
}

void gp_enumerate_fonts_free(void *enum_state)
{
#ifdef HAVE_FONTCONFIG
    unix_fontenum_t* state = (unix_fontenum_t *)enum_state;
    if (state != NULL) {
	if (state->font_list != NULL)
	    FcFontSetDestroy(state->font_list);
	free(state);
    }
#endif
}

