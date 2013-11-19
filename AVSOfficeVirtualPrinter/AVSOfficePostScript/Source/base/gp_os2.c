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

/* $Id: gp_os2.c 9052 2008-08-30 13:31:29Z ghostgum $ */
/* Common platform-specific routines for OS/2 and MS-DOS */
/* compiled with GCC/EMX */

#define INCL_DOS
#define INCL_SPL
#define INCL_SPLDOSPRINT
#define INCL_SPLERRORS
#define INCL_BASE
#define INCL_ERRORS
#define INCL_WIN
#include <os2.h>

#include "pipe_.h"
#include "stdio_.h"
#include "string_.h"
#include <fcntl.h>

#ifdef __IBMC__
#define popen fopen		/* doesn't support popen */
#define pclose fclose		/* doesn't support pclose */
#else
#include <dos.h>
#endif
/* Define the regs union tag for short registers. */
#  define rshort x
#define intdos(a,b) _int86(0x21, a, b)

#include "memory_.h"
#include "string_.h"
#include "gx.h"
#include "gsexit.h"
#include "gsmemory.h"
#include "gsstruct.h"
#include "gp.h"
#include "gpmisc.h"
#include "gsutil.h"
#include "stdlib.h"		/* need _osmode, exit */
#include "time_.h"
#include <time.h>		/* should this be in time_.h? */
#include "gp_os2.h"
#ifdef __EMX__
#include <sys/emxload.h>
#endif

#if defined(__DLL__) && defined( __EMX__)
/* This isn't provided in any of the libraries */
/* We set this to the process environment in gp_init */
char *fake_environ[3] =
{"", NULL, NULL};
char **environ = fake_environ;
char **_environ = fake_environ;
HWND hwndtext = (HWND) NULL;

#endif

#ifdef __DLL__
/* use longjmp instead of exit when using DLL */
#include <setjmp.h>
extern jmp_buf gsdll_env;

#endif

#ifdef __DLL__
#define isos2 TRUE
#else
#define isos2 (_osmode == OS2_MODE)
#endif
char pm_prntmp[256];		/* filename of printer spool temporary file */


/* ------ Miscellaneous ------ */

/* Get the string corresponding to an OS error number. */
/* All reasonable compilers support it. */
const char *
gp_strerror(int errnum)
{
    return strerror(errnum);
}

/* use Unix version for date and time */
/* ------ Date and time ------ */

/* Read the current time (in seconds since Jan. 1, 1970) */
/* and fraction (in nanoseconds since midnight). */
void
gp_get_realtime(long *pdt)
{
    struct timeval tp;
    struct timezone tzp;

    if (gettimeofday(&tp, &tzp) == -1) {
	lprintf("Ghostscript: gettimeofday failed!\n");
	tp.tv_sec = tp.tv_usec = 0;
    }
    /* tp.tv_sec is #secs since Jan 1, 1970 */
    pdt[0] = tp.tv_sec;
    pdt[1] = tp.tv_usec * 1000;

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
    gp_get_realtime(pdt);	/* Use an approximation for now.  */
}


/* ------ Console management ------ */

/* Answer whether a given file is the console (input or output). */
/* This is not a standard gp procedure, */
/* but the MS Windows configuration needs it, */
/* and other MS-DOS configurations might need it someday. */
/* Don't know if it is needed for OS/2. */
bool
gp_file_is_console(FILE * f)
{
#ifndef __DLL__
    if (!isos2) {
	union REGS regs;

	if (f == NULL)
	    return false;
	regs.h.ah = 0x44;	/* ioctl */
	regs.h.al = 0;		/* get device info */
	regs.rshort.bx = fileno(f);
	intdos(&regs, &regs);
	return ((regs.h.dl & 0x80) != 0 && (regs.h.dl & 3) != 0);
    }
#endif
    if (fileno(f) <= 2)
	return true;
    return false;
}

/* ------ Persistent data cache ------*/
  
/* insert a buffer under a (type, key) pair */
int gp_cache_insert(int type, byte *key, int keylen, void *buffer, int buflen)
{ 
    /* not yet implemented */
    return 0;
} 
 
/* look up a (type, key) in the cache */
int gp_cache_query(int type, byte* key, int keylen, void **buffer,
    gp_cache_alloc alloc, void *userdata)
{
    /* not yet implemented */
    return -1;
}


/*************************************************************/
/* from gp_iwatc.c and gp_itbc.c */

/* Intel processor, EMX/GCC specific routines for Ghostscript */
#include <signal.h>
#include "stat_.h"
#include "string_.h"

/* Library routines not declared in a standard header */
/* extern char *getenv(const char *); */

/* Forward declarations */
static void handle_FPE(int);

/* Do platform-dependent initialization. */
void
gp_init(void)
{
#if defined(__DLL__) && defined(__EMX__)
    PTIB pptib;
    PPIB pppib;
    int i;
    char *p;

    /* get environment of EXE */
    DosGetInfoBlocks(&pptib, &pppib);
    for (i = 0, p = pppib->pib_pchenv; *p; p += strlen(p) + 1)
	i++;
    _environ = environ = (char **)malloc((i + 2) * sizeof(char *));

    for (i = 0, p = pppib->pib_pchenv; *p; p += strlen(p) + 1) {
	environ[i] = p;
	i++;
    }
    environ[i] = p;
    i++;
    environ[i] = NULL;
#endif

    /* keep gsos2.exe in memory for number of minutes specified in */
    /* environment variable GS_LOAD */
#ifdef __EMX__
    _emxload_env("GS_LOAD");
#endif

    /* Set up the handler for numeric exceptions. */
    signal(SIGFPE, handle_FPE);
}


/* Trap numeric exceptions.  Someday we will do something */
/* more appropriate with these. */
static void
handle_FPE(int sig)
{
    eprintf("Numeric exception:\n");
    exit(1);
}

/* Do platform-dependent cleanup. */
void
gp_exit(int exit_status, int code)
{
#if defined(__DLL__) && defined(__EMX__)
    if (environ != fake_environ) {
	free(environ);
	environ = _environ = fake_environ;
    }
#endif
}

/* Exit the program. */
void
gp_do_exit(int exit_status)
{
    exit(exit_status);
}

/* ------ Printer accessing ------ */
static int is_os2_spool(const char *queue);

/* Put a printer file (which might be stdout) into binary or text mode. */
/* This is not a standard gp procedure, */
/* but all MS-DOS configurations need it. */
void
gp_set_file_binary(int prnfno, int binary)
{
#ifndef __IBMC__
    union REGS regs;

    regs.h.ah = 0x44;		/* ioctl */
    regs.h.al = 0;		/* get device info */
    regs.rshort.bx = prnfno;
    intdos(&regs, &regs);
    if (((regs.rshort.flags) & 1) != 0 || !(regs.h.dl & 0x80))
	return;			/* error, or not a device */
    if (binary)
	regs.h.dl |= 0x20;	/* binary (no ^Z intervention) */
    else
	regs.h.dl &= ~0x20;	/* text */
    regs.h.dh = 0;
    regs.h.ah = 0x44;		/* ioctl */
    regs.h.al = 1;		/* set device info */
    intdos(&regs, &regs);
#endif
}

/* Open a connection to a printer.  A null file name means use the */
/* standard printer connected to the machine, if any. */
/* Return NULL if the connection could not be opened. */
/* filename can be one of the following values
 *   ""                Spool in default queue
 *   "\\spool\queue"   Spool in "queue"
 *   "|command"        open an output pipe using popen()
 *   "filename"        open filename using fopen()
 *   "port"            open port using fopen()
 */
FILE *
gp_open_printer(char fname[gp_file_name_sizeof], int binary_mode)
{
    FILE *pfile;

    if ((strlen(fname) == 0) || is_os2_spool(fname)) {
	if (isos2) {
	    /* default or spool */
	    if (pm_spool(NULL, fname))	/* check if spool queue valid */
		return NULL;
	    pfile = gp_open_scratch_file(gp_scratch_file_name_prefix,
				     pm_prntmp, (binary_mode ? "wb" : "w"));
	} else
	    pfile = fopen("PRN", (binary_mode ? "wb" : "w"));
    } else if ((isos2) && (fname[0] == '|'))
	/* pipe */
	pfile = popen(fname + 1, (binary_mode ? "wb" : "w"));
    else
	/* normal file or port */
	pfile = fopen(fname, (binary_mode ? "wb" : "w"));

    if (pfile == (FILE *) NULL)
	return (FILE *) NULL;
    if (!isos2)
	gp_set_file_binary(fileno(pfile), binary_mode);
    return pfile;
}

/* Close the connection to the printer. */
void
gp_close_printer(FILE * pfile, const char *fname)
{
    if (isos2 && (fname[0] == '|'))
	pclose(pfile);
    else
	fclose(pfile);

    if ((strlen(fname) == 0) || is_os2_spool(fname)) {
	/* spool temporary file */
	pm_spool(pm_prntmp, fname);
	unlink(pm_prntmp);
    }
}

/* ------ File accessing -------- */

/* Set a file into binary or text mode. */
int
gp_setmode_binary(FILE * pfile, bool binary)
{
    gp_set_file_binary(fileno(pfile), binary);
    return 0;
}

/* ------ Printer Spooling ------ */
#ifndef NERR_BufTooSmall
#define NERR_BufTooSmall 2123	/* For SplEnumQueue */
#endif

/* If queue_name is NULL, list available queues */
/* If strlen(queue_name)==0, return default queue and driver name */
/* If queue_name supplied, return driver_name */
/* returns 0 if OK, non-zero for error */
int
pm_find_queue(char *queue_name, char *driver_name)
{
    SPLERR splerr;
    USHORT jobCount;
    ULONG cbBuf;
    ULONG cTotal;
    ULONG cReturned;
    ULONG cbNeeded;
    ULONG ulLevel;
    ULONG i;
    PSZ pszComputerName;
    PBYTE pBuf;
    PPRQINFO3 prq;

    ulLevel = 3L;
    pszComputerName = (PSZ) NULL;
    splerr = SplEnumQueue(pszComputerName, ulLevel, pBuf, 0L,	/* cbBuf */
			  &cReturned, &cTotal,
			  &cbNeeded, NULL);
    if (splerr == ERROR_MORE_DATA || splerr == NERR_BufTooSmall) {
	if (!DosAllocMem((PVOID) & pBuf, cbNeeded,
			 PAG_READ | PAG_WRITE | PAG_COMMIT)) {
	    cbBuf = cbNeeded;
	    splerr = SplEnumQueue(pszComputerName, ulLevel, pBuf, cbBuf,
				  &cReturned, &cTotal,
				  &cbNeeded, NULL);
	    if (splerr == NO_ERROR) {
		/* Set pointer to point to the beginning of the buffer.           */
		prq = (PPRQINFO3) pBuf;

		/* cReturned has the count of the number of PRQINFO3 structures.  */
		for (i = 0; i < cReturned; i++) {
		    if (queue_name) {
			/* find queue name and return driver name */
			if (strlen(queue_name) == 0) {	/* use default queue */
			    if (prq->fsType & PRQ3_TYPE_APPDEFAULT)
				strcpy(queue_name, prq->pszName);
			}
			if (strcmp(prq->pszName, queue_name) == 0) {
			    char *p;

			    for (p = prq->pszDriverName; *p && (*p != '.'); p++)
				/* do nothing */ ;
			    *p = '\0';	/* truncate at '.' */
			    if (driver_name != NULL)
				strcpy(driver_name, prq->pszDriverName);
			    DosFreeMem((PVOID) pBuf);
			    return 0;
			}
		    } else {
			/* list queue details */
			if (prq->fsType & PRQ3_TYPE_APPDEFAULT)
			    eprintf1("  \042%s\042  (DEFAULT)\n", prq->pszName);
			else
			    eprintf1("  \042%s\042\n", prq->pszName);
		    }
		    prq++;
		}		/*endfor cReturned */
	    }
	    DosFreeMem((PVOID) pBuf);
	}
    }
    /* end if Q level given */ 
    else {
	/* If we are here we had a bad error code. Print it and some other info. */
	eprintf4("SplEnumQueue Error=%ld, Total=%ld, Returned=%ld, Needed=%ld\n",
		splerr, cTotal, cReturned, cbNeeded);
    }
    if (splerr)
	return splerr;
    if (queue_name)
	return -1;
    return 0;
}


/* return TRUE if queue looks like a valid OS/2 queue name */
static int
is_os2_spool(const char *queue)
{
    char *prefix = "\\\\spool\\";	/* 8 characters long */
    int i;

    for (i = 0; i < 8; i++) {
	if (prefix[i] == '\\') {
	    if ((*queue != '\\') && (*queue != '/'))
		return FALSE;
	} else if (tolower(*queue) != prefix[i])
	    return FALSE;
	queue++;
    }
    return TRUE;
}

#define PRINT_BUF_SIZE 16384

/* Spool file to queue */
/* return 0 if successful, non-zero if error */
/* if filename is NULL, return 0 if spool queue is valid, non-zero if error */
int
pm_spool(char *filename, const char *queue)
{
    HSPL hspl;
    PDEVOPENSTRUC pdata;
    PSZ pszToken = "*";
    ULONG jobid;
    BOOL rc;
    char queue_name[256];
    char driver_name[256];
    char *buffer;
    FILE *f;
    int count;

    if (strlen(queue) != 0) {
	/* queue specified */
	strcpy(queue_name, queue + 8);	/* skip over \\spool\ */
    } else {
	/* get default queue */
	queue_name[0] = '\0';
    }
    if (pm_find_queue(queue_name, driver_name)) {
	/* error, list valid queue names */
	eprintf("Invalid queue name.  Use one of:\n");
	pm_find_queue(NULL, NULL);
	return 1;
    }
    if (!filename)
	return 0;		/* we were only asked to check the queue */


    if ((buffer = malloc(PRINT_BUF_SIZE)) == (char *)NULL) {
	eprintf("Out of memory in pm_spool\n");
	return 1;
    }
    if ((f = fopen(filename, "rb")) == (FILE *) NULL) {
	free(buffer);
	eprintf1("Can't open temporary file %s\n", filename);
	return 1;
    }
    /* Allocate memory for pdata */
    if (!DosAllocMem((PVOID) & pdata, sizeof(DEVOPENSTRUC),
		     (PAG_READ | PAG_WRITE | PAG_COMMIT))) {
	/* Initialize elements of pdata */
	pdata->pszLogAddress = queue_name;
	pdata->pszDriverName = driver_name;
	pdata->pdriv = NULL;
	pdata->pszDataType = "PM_Q_RAW";
	pdata->pszComment = "Ghostscript";
	pdata->pszQueueProcName = NULL;
	pdata->pszQueueProcParams = NULL;
	pdata->pszSpoolerParams = NULL;
	pdata->pszNetworkParams = NULL;

	hspl = SplQmOpen(pszToken, 4L, (PQMOPENDATA) pdata);
	if (hspl == SPL_ERROR) {
	    eprintf("SplQmOpen failed.\n");
	    DosFreeMem((PVOID) pdata);
	    free(buffer);
	    fclose(f);
	    return 1;		/* failed */
	}
	rc = SplQmStartDoc(hspl, "Ghostscript");
	if (!rc) {
	    eprintf("SplQmStartDoc failed.\n");
	    DosFreeMem((PVOID) pdata);
	    free(buffer);
	    fclose(f);
	    return 1;
	}
	/* loop, copying file to queue */
	while (rc && (count = fread(buffer, 1, PRINT_BUF_SIZE, f)) != 0) {
	    rc = SplQmWrite(hspl, count, buffer);
	    if (!rc)
		eprintf("SplQmWrite failed.\n");
	}
	free(buffer);
	fclose(f);

	if (!rc) {
	    eprintf("Aborting Spooling.\n");
	    SplQmAbort(hspl);
	} else {
	    SplQmEndDoc(hspl);
	    rc = SplQmClose(hspl);
	    if (!rc)
		eprintf("SplQmClose failed.\n");
	}
    } else
	rc = 0;			/* no memory */
    return !rc;
}



/* ------ Font enumeration ------ */
 
 /* This is used to query the native os for a list of font names and
  * corresponding paths. The general idea is to save the hassle of
  * building a custom fontmap file.
  */
 
void *gp_enumerate_fonts_init(gs_memory_t *mem)
{
    return NULL;
}
         
int gp_enumerate_fonts_next(void *enum_state, char **fontname, char **path)
{
    return 0;
}
                         
void gp_enumerate_fonts_free(void *enum_state)
{
}           

/* --------- 64 bit file access ----------- */
/* fixme: Not implemented yet.
 * Currently we stub it with 32 bits access. 
 */

FILE *gp_fopen_64(const char *filename, const char *mode)
{
    return fopen(filename, mode);
}

FILE *gp_open_scratch_file_64(const char *prefix,
			   char fname[gp_file_name_sizeof],
			   const char *mode)
{
    return gp_open_scratch_file(prefix, fname, mode);
}

FILE *gp_open_printer_64(char fname[gp_file_name_sizeof], int binary_mode)
{
    return gp_open_printer(fname, binary_mode);
}

int64_t gp_ftell_64(FILE *strm)
{
    return ftell(strm);
}

int gp_fseek_64(FILE *strm, int64_t offset, int origin)
{
    long offset1 = (long)offset;
    
    if (offset != offset1)
	return -1;
    return fseek(strm, offset1, origin);
}
