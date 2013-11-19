/*
 *   IBM Omni driver
 *   Copyright (c) International Business Machines Corp., 2000
 *
 *   This library is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Lesser General Public License as published
 *   by the Free Software Foundation; either version 2.1 of the License, or
 *   (at your option) any later version.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 *   the GNU Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public License
 *   along with this library; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 *   Portions of this file are used with permission granted by Aladdin
 *   Enterprises
 *
 *
 *
 *
 * Copyright (C) 1998, 1999, 2000 Aladdin Enterprises.
 *
 * The program in this file is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser
 * General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */
/************************************************************************/
/*              FILE: gomni.c                                           */
/*                                                                      */
/*                                                                      */
/*    Change History                                                    */
/*    =============                                                     */
/*                                                                      */
/*    @@08162000       Changed device PageCount value so that we could  */
/*                     print multiple pages.  The PageCount value was   */
/*                     not being updated internal to Ghostscript        */
/*                                                                      */
/*    @@10172000       changed to flip coordinates from zero based top  */
/*                     to actual top of page                            */
/*                                                                      */
/*    @@10192000       adjusted dot size correction values for          */
/*                     monochrome mode                                  */
/*                                                                      */
/*    @@10252000       added code to make sure that we have a full band */
/*                     of data.  Rendering high res is slow.            */
/*                                                                      */
/*    @@10312000       correct line decrement and get_bits of a line    */
/*                                                                      */
/*    04062001         rewrote module to allow for sync and async       */
/*                     function since GS does not handle async at the   */
/*                     moment                                           */
/*                                                                      */
/*    @@04062001       removed mono dither routines and support from    */
/*                     this module into the core omni code              */
/*                                                                      */
/*    @@04162001       Added support for newFrame                       */
/*                                                                      */
/*    @@04252001       Added support for GS mono output                 */
/*                     -smonodither=GSMONO on the command line          */
/*                                                                      */
/*    @@05092001       Added the ability to have a client server        */
/*                     interaction instead of writing the data          */
/*                     directly to a file                               */
/*                                                                      */
/*    @@05052001A      Update mulitplier for .0001 mm vs. .001 mm       */
/*                                                                      */
/*    1/28/2002        changed code to utilize a core omni structure    */
/*                     to allow for management of devices linked        */
/*                     to servers and allow device code re-entrancy     */
/*                                                                      */
/*    @@01302002       Changed code to utilize -sserver from the        */
/*                     command line instead of having to provide a      */
/*                     file name along with the server                  */
/*                                                                      */
/************************************************************************/

#include "std.h"
#ifdef OMNI_USE_GLIB
/* Include these first.  Ghostscript redefines printf */
#include <glib.h>
#include <gmodule.h>
#else
#include <dlfcn.h>
#define GModule void
#define GModuleFlags int
typedef void* gpointer;
#define g_module_open dlopen
#define g_module_close dlclose
int dlsym_wrapper(void *lib, char const *name, void **symbol)
{
	*symbol=dlsym(lib, name);
	return *symbol != 0;
}
#define g_module_symbol dlsym_wrapper
#define g_module_error dlerror
#endif
#ifdef HAVE_STDINT_H
#  include <stdint.h>
#endif /* HAVE_STDINT_H */

#include "gdevprna.h"
#include "gdevpccm.h"
#include "gdevprn.h"
#include "gsparam.h"
#include "gdevbmp.h"
#include "gserrors.h"
#include "stdio_.h"
#include "gserrors.h"
#include "gpsync.h"

#include "defs.h"

#define MAX_LENGTH 65

#define Signature "OMNI"

const static int fDebugOutput = 0;

#define DEVICES_DEFINED

typedef struct _HWMAR {
  float fLeftClip;
  float fBottomClip;
  float fRightClip;
  float fTopClip;
  float fxWidth;
  float fyHeight;
} HWMARGINS;

typedef struct _HWRES {
  float xRes;
  float yRes;
  float fScanDots;  /* number of dots in scan line */
} HWRESOLUTION;

typedef struct _PRTMODE {
  int iBitCount;
  int iPlanes;
} PRINTMODE;

long DiffusionDither (PDDI pdi, PIMAGEINFO pimg_infos, PBYTE *pBuffer, ULONG ulFlgs);

/*
 *   Prototypes for routines that we will be calling into the core Omni code
 *   to resolve
 */
typedef bool   (*PFNGIINIT)            (void         *pDev);
typedef bool   (*PFNGITERM)            (void         *pDev);
typedef bool   (*PFNGETPRINTMODEINFO)  (void         *pDev,
                                        PRINTMODE    *pPrtMode);
typedef bool   (*PFNGETRESOLUTIONINFO) (void         *pDev,
                                        HWRESOLUTION *hwRes);
typedef bool   (*PFNGETMARGININFO)     (void         *pDev,
                                        HWMARGINS    *hwMargins);
typedef void   (*PFNBEGINJOB)          (void         *pOmni,
                                        FILE         *pfpOut);
typedef void   (*PFNNEWFRAME)          (void         *pDev); /*@@04162001*/
typedef void   (*PFNENDJOB)            (void         *pDev);
typedef void   (*PFNRASTERIZE)         (void         *pcoreOmni,
                                        PBYTE         pbBits,
                                        PBITMAPINFO2  pbmi,
                                        PSIZEL        psizelPage,
                                        PRECTL        prectlPageLocation,
                                        BITBLT_TYPE   eType);
typedef void * (*PFNCREATEDEVICE)      (void         *pOmni,
                                        void        **vhDevice,
                                        int           iUsePDC);
typedef void * (*PFNDELETEDEVICE)      (void         *pOmni);
typedef long   (*PFNMONODITHER)        (PDDI          pdi,           /*@@04062001*/
                                        PIMAGEINFO    pimg_infos,
                                        PBYTE        *pBuffer,
                                        ULONG         ulFlgs);

ULONG         FindBandSize             (ULONG ulYHeight,
                                        ULONG ulXWidth,
                                        ULONG ulBitsPerPel,
                                        ULONG ulNumPlanes,
                                        ULONG ulModulus,
                                        ULONG ulMemoryLimit);

typedef int (*PFNGETOMNIJOBPROPERTIES)  (char  *pszDriverName,
                                         char **pJobProperties);
typedef int (*PFNFREEOMNIJOBPROPERTIES) (char **pJobProperties);

#define uint32 unsigned long
#define int32 long

#define strcmpi strcasecmp

/* ------ The device descriptors ------ */

typedef struct omni_dev_struct {
    char  cSignature[4];
    int   cbSize;                     /* length of the structure */
    char  cVersion[10];
    char  cOmniVersion[10];
    bool  bPDCDevice;                 /* boolean to tell if this is a PDC device or not */
    void *pDevice;                    /* pointer to a device */
    FILE *pfpOut;                     /* pointer to the output handle from beginjob */
    FILE *pfpErr;
    char *pszJobOptions;              /* pointer to the job options */
    char  cDebugFile[MAX_LENGTH];     /* debug file name */
    char  cDeviceName[MAX_LENGTH];
    char  cServer[64];
    void *pvReserved;
} core_omni, core_omni_device;

/* All omni specific data */
typedef struct _DevStruct {
   char                  cAsyncOpt[MAX_LENGTH];      /* text value for dither */
   char                  cMonoDither[MAX_LENGTH];
   char                  cServerFile[256];           /*@@05092001 */
   core_omni_device     *pcoreOmni;
   void                 *pCallPtr;

   GModule              *hmodOmni;                   /* handle to libomni.so */
   GModule              *hmodDevice;                 /* handle to libDeviceXXX.so */
   PFNGETPRINTMODEINFO   pfnGetPrintModeInfo;
   PFNGETRESOLUTIONINFO  pfnGetResolutionInfo;
   PFNGETMARGININFO      pfnGetMarginInfo;
   PFNBEGINJOB           pfnBeginJob;
   PFNNEWFRAME           pfnNewFrame;                /*@@04162001*/
   PFNENDJOB             pfnEndJob;
   PFNRASTERIZE          pfnRasterize;
   PFNCREATEDEVICE       pfnCreateDevice;
   PFNDELETEDEVICE       pfnDeleteDevice;
   PFNMONODITHER         pfnMonoDither;              /*@@04162001*/
   PFNGIINIT             pfnGIInit;
   PFNGITERM             pfnGITerm;
   unsigned long         ulPelSizeCorrection;

   int                   iVertDots;
   int                   width;
   int                   height;
   int                   iSync;
   int                   iPageNumber;                /*@@04162001 */
   int                   iGSMono;                    /*@@04252001 */
   int                   iUseServer;                 /*@@05092001 */
   int                   iUseDialog;
   FILE                 *ProxyFile;                  /*@@05092001 */

   byte                 *outbuf;                     /* Output-Buffer */

   int                   iPage;
   int                   iSetup;
} Omni_Dev,
 *PDEVSTRUCT; /** Type & Pointer of device-specifics */

/* Define data type for this device based on prn_device */
typedef struct gx_device_omni_s {
    gx_device_common;
    gx_prn_device_common;
    int         buffered_page_exists;
    long        file_offset_to_data;
    int         copies_printed;
    PDEVSTRUCT  pDev;                 /** Omni specific extensions */
} gx_device_omni;
typedef gx_device_omni omni_device;


/* Define initializer for device */
#define omni_device(procs, dname, w10, h10, xdpi, ydpi, lm, bm, rm, tm, color_bits, print_page)\
{ prn_device_std_margins_body(gx_device_omni, procs, dname,\
    w10, h10, xdpi, ydpi, lm, tm, lm, bm, rm, tm, color_bits, print_page),\
  0, 0L\
}

static int  PrintPage            (gx_device_printer            *pgx_prt_dev,
                                   FILE                         *prn_stream,
                                   int                           num_copies);
static int  omni_print_page      (gx_device_printer            *pdev,
                                   FILE                         *prn_stream);
static int  SetupDevice          (gx_device                    *pgxdev,
                                   gs_param_list                *plist);
static int  OpenDevice           (gx_device                    *pdev);
static int  CloseDevice          (gx_device                    *pdev);
static int  GetDeviceParams      (gx_device                    *pgxdev,
                                   gs_param_list                *plist);
static int  BufferPage           (gx_device_printer            *pgx_prt_dev,
                                   FILE                         *file,
                                   int                           num_copies);
static void RenderThread         (void                         *params);
static int  StartRenderThread    (gdev_prn_start_render_params *params);
static int  OpenRenderDevice     (gx_device_printer            *ppdev);
static void GetSpaceParams       (const gx_device_printer      *pgx_prt_dev,
                                   gdev_prn_space_params        *space_params);

/* Print-page, parameters and miscellaneous procedures */
static      dev_proc_print_page_copies       (PrintPage);
static      prn_dev_proc_buffer_page         (BufferPage);
static      prn_dev_proc_start_render_thread (StartRenderThread);
static      dev_proc_open_device             (OpenDevice);
static      dev_proc_print_page              (omni_print_page);
static      dev_proc_close_device            (CloseDevice);
static      dev_proc_get_params              (GetDeviceParams);
static      dev_proc_put_params              (SetupDevice);
static      dev_proc_output_page             (PrintPageMultiple);
static      prn_dev_proc_get_space_params    (GetSpaceParams);

/* 24-bit color. only want 16M colors  */

static gx_device_procs omni16m_procs = {
                  OpenDevice,                       /*  open_device */
                  NULL,                             /*  get_initial_matrix */
                  NULL,                             /*  sync_output */
                  gdev_prn_output_page,             /*  output_page */
                  CloseDevice,                      /*  close_device */
                  NULL, /* bmp_map_16m_rgb_color,*/ /*  map_rgb_color      @@04252001 */
                  NULL, /* bmp_map_16m_color_rgb,*/ /*  map_color_rgb      @@04252001 */
                  NULL,                             /** fill_rectangle */
                  NULL,                             /** tile_rectangle */
                  NULL,                             /** copy_mono */
                  NULL,                             /** copy_color */
                  NULL,                             /** draw_line */
                  gx_default_get_bits,              /* Reads scan lines */
                  GetDeviceParams,                  /** Export parameters, special */
                  SetupDevice                       /** Import parameters, special */
};


gx_device_omni far_data gs_omni_device =
  omni_device (omni16m_procs,
               "omni",
               DEFAULT_WIDTH_10THS,  /* 85 x 110 - page physical area non clipped */
               DEFAULT_HEIGHT_10THS,
               X_DPI, Y_DPI,         /* std resolution - 72 dpi          */
               0,0,0,0,              /* margins - need defaults  */
               1,                    /* Bpp value */
               omni_print_page);     /* sync routine for output of raster data */


/* ------------------------------------------------------*/
/* ------------------------------------------------------*/

/* Generic routine to send the page to the printer. */
static int
PrintPageMultiple (gx_device *pDev, int iCopies, int flush)
{
   /*
    * HACK: open the printer page with the positionable attribute since
    * we need to seek back & forth to support partial rendering.
    */
   if (  iCopies > 0
      || !flush
      )
   {
      int iRet = gdev_prn_open_printer_positionable (pDev, 1, 1);

      if (iRet < 0)
         return iRet;
   }

   return gdev_prn_output_page(pDev, iCopies, flush);
}

/* ------------ Writer Instance procedures ---------- */

/* Writer's open procedure */
static int
OpenDevice (gx_device *pdev  /* Driver instance to open */)
{
   gx_device_omni * const pwdev              = (gx_device_omni *)pdev;
   PDEVSTRUCT             pDev               = pwdev->pDev;
   int                    max_width;
   int                    max_raster;
   int                    min_band_height;
   int                    max_src_image_row;

   if (fDebugOutput) dprintf2 ("OpenDevice: pDev = %p, pDev->hmodOmni = %p\n", pDev, pDev->hmodOmni);

   if (!pDev)
   {
      eprintf ("\n<<<<<<<<<<<<<<<<<<<<<< ERROR >>>>>>>>>>>>>>>>>>>>>>>\n\n");
      eprintf ("Error: No pDev in OpenDevice.\n");
      if (fDebugOutput) lprintf ("error was here.\n");

      return gs_error_Fatal;
   }

   if (!pDev->hmodOmni)
   {
      eprintf ("\n<<<<<<<<<<<<<<<<<<<<<< ERROR >>>>>>>>>>>>>>>>>>>>>>>\n\n");
      eprintf ("Error: Could not load libomni.so\n");
      if (fDebugOutput) lprintf ("error was here.\n");

      return gs_error_Fatal;
   }

   if (!pDev->pcoreOmni->cDeviceName[0])
   {
      eprintf ("\n<<<<<<<<<<<<<<<<<<<<<< ERROR >>>>>>>>>>>>>>>>>>>>>>>\n\n");
      eprintf ("Error: -sDeviceName=XXX was not defined\n");
      if (fDebugOutput) lprintf ("error was here.\n");

      return gs_error_Fatal;
   }

   pDev->iUseServer  = 0;
   pDev->iPageNumber = 0;
   pDev->iUseDialog  = 0;

   /*
    * Set up device's printer proc vector to point to this driver, since
    * there are no convenient macros for setting them up in static template.
    */
   if (pDev->iSync)
   {
      init_async_render_procs (pwdev,
                               StartRenderThread,
                               BufferPage,
                               PrintPage);
   }
   else
   {
      /* don't really think this needs to be done but we'll do it anyway */
      set_dev_proc (pdev, sync_output, gx_default_sync_output);
   }

   set_dev_proc (pdev, put_params, SetupDevice);
   set_dev_proc (pdev, get_params, GetDeviceParams);

   if (!pDev->iGSMono)
   {
      if (fDebugOutput) dprintf("Remapping color pointers\n");

      set_dev_proc(pdev, map_rgb_color, bmp_map_16m_rgb_color);
      set_dev_proc(pdev, map_color_rgb, bmp_map_16m_color_rgb);
   }

   if (pDev->iSync)
   {
      set_dev_proc(pdev, output_page, PrintPageMultiple);
      pwdev->printer_procs.get_space_params   = GetSpaceParams;
      pwdev->printer_procs.open_render_device = OpenRenderDevice;
   }

   /*
    * Determine MAXIMUM parameters this device will have to support over
    * lifetime.  See comments for GetSpaceParams().
    */
   max_width = DEFAULT_WIDTH_10THS * 60 * 16;   /* figure max wid = default @ 600dpi */
                                      /* multiplied by 2 for 1200 dpi */
   min_band_height = max(1, (DEFAULT_HEIGHT_10THS * 60) / 100);
   max_raster = (max_width * pwdev->color_info.depth) / 8;        /* doesn't need to be super accurate */
   max_src_image_row = max_width * 3 * 2;

#ifdef OMNI_ASYNC
   if (pDev->iSync)
   {
      return gdev_prn_async_write_open ((gx_device_printer *)pdev,
                                        max_raster,
                                        min_band_height,
                                        max_src_image_row);

   }
   else
#endif
   {
      return gdev_prn_open (pdev);
   }
}

static int
CloseDevice (gx_device * pdev)
{
   gx_device_omni * const pwdev = (gx_device_omni *)pdev;
   PDEVSTRUCT             pDev  = pwdev->pDev;

   if (fDebugOutput) dprintf2 ("CloseDevice: pDev = %p, pDev->pcoreOmni->pDevice = %p\n", pDev, pDev->pcoreOmni->pDevice);

   if (!pDev)
   {
      eprintf ("\n<<<<<<<<<<<<<<<<<<<<<< ERROR >>>>>>>>>>>>>>>>>>>>>>>\n\n");
      eprintf ("Error: No device handle in CloseDevice\n");
      if (fDebugOutput) lprintf ("error was here.\n");

      return_error (gs_error_Fatal);  /* handle no pDev */
   }

   if (  pDev->pcoreOmni
      && pDev->pcoreOmni->pDevice
      && pDev->pfnEndJob
      )
   {
       pDev->pfnEndJob ((void*)pDev->pCallPtr);
   }

   if (  pDev->pcoreOmni
      && pDev->pcoreOmni->pszJobOptions
      )
   {
      gs_free (gs_lib_ctx_get_non_gc_memory_t(), pDev->pcoreOmni->pszJobOptions, strlen (pDev->pcoreOmni->pszJobOptions) + 1, 1, "Option String");
   }

   if (  pDev->pcoreOmni
      && pDev->pcoreOmni->pDevice
      && pDev->pfnDeleteDevice
      )
   {
      pDev->pfnDeleteDevice (pDev->pcoreOmni->pDevice);
      pDev->pcoreOmni->pDevice = 0;
   }

   if (pDev->pfnGITerm)
   {
      pDev->pfnGITerm (pDev->pcoreOmni);
   }

   if (pDev->pcoreOmni)
   {
      gs_free (gs_lib_ctx_get_non_gc_memory_t(), pDev->pcoreOmni, sizeof (core_omni_device), 1, "omni/device");
      pDev->pcoreOmni = 0;
   }

   if (pDev->hmodDevice)
   {
      g_module_close (pDev->hmodDevice);
      pDev->hmodDevice = 0;
   }

   if (pDev->hmodOmni)
   {
      g_module_close (pDev->hmodOmni);
      pDev->hmodOmni = 0;
   }

   return gdev_prn_close (pdev);
}

/* -------------------------------------------------------------------------*/
/*                                                                          */
/*  Function: GetDeviceParams                                               */
/*                                                                          */
/*   This function calls back into Ghostscript to set the string values     */
/*   so that next time Ghostscript calls SetupDevice, the correct           */
/*   values can be pulled back out again                                    */
/*                                                                          */
/* -------------------------------------------------------------------------*/

static int
GetDeviceParams (gx_device *pgxdev, gs_param_list *plist)
{
   omni_device *const      odev                = (omni_device *) pgxdev;
   PDEVSTRUCT              pDev                = odev->pDev;
   int                     iReturnCode         = 0;
   gs_param_string         strdata;

   const char             *pszDeviceName       = "";
   const char             *pszDebug            = "";
   const char             *pszOther            = "";
   const char             *pszAsync            = "";
   const char             *pszMonoDither       = "";
   const char             *pszServer           = "";
   char                    pszTemp[3];
   unsigned long           ulPelSizeCorrection = 100;
   bool                    bPersist            = false;

   if (fDebugOutput) dprintf1 ("GetDeviceParams: pDev = %p\n", pDev);

   iReturnCode = gdev_prn_get_params (pgxdev, plist);

   if (pDev)
   {
       bPersist            = true;
       pszDeviceName       = pDev->pcoreOmni->cDeviceName;
       pszDebug            = pDev->pcoreOmni->cDebugFile;
       pszOther            = pDev->pcoreOmni->pszJobOptions;
       pszAsync            = pDev->cAsyncOpt;
       ulPelSizeCorrection = pDev->ulPelSizeCorrection;
       pszMonoDither       = pDev->cMonoDither;
       pszServer           = pDev->pcoreOmni->cServer;
   }

   if (fDebugOutput) dprintf1 ("GetDeviceParams: pszDeviceName       = %s\n", pszDeviceName);
   if (fDebugOutput) dprintf1 ("GetDeviceParams: pszDebug            = %s\n", pszDebug);
   if (fDebugOutput) dprintf1 ("GetDeviceParams: pszOther            = %s\n", pszOther);
   if (fDebugOutput) dprintf1 ("GetDeviceParams: pszAsync            = %s\n", pszAsync);
   if (fDebugOutput) dprintf1 ("GetDeviceParams: ulPelSizeCorrection = %ld\n", ulPelSizeCorrection);
   if (fDebugOutput) dprintf1 ("GetDeviceParams: pszMonoDither       = %s\n", pszMonoDither);
   if (fDebugOutput) dprintf1 ("GetDeviceParams: pszServer           = %s\n", pszServer);

   strdata.data = pszDeviceName;
   strdata.size = strlen(pszDeviceName);
   strdata.persistent = bPersist;
   iReturnCode = param_write_string(plist,"DeviceName",&strdata);

   strdata.data = pszDebug;
   strdata.size = strlen(pszDebug);
   strdata.persistent = bPersist;
   iReturnCode = param_write_string(plist,"dbgout",&strdata);

   if (  pDev
      && pDev->pcoreOmni->pszJobOptions
      )
   {
      strdata.size = strlen (pszOther);
      strdata.data = pszOther;
   }
   else
   {
      pszTemp[0] = '\0';
      strdata.size = 0;
      strdata.data = pszTemp;
   }

   strdata.persistent = bPersist;
   iReturnCode = param_write_string(plist,"properties",&strdata);

   strdata.data = pszAsync;
   strdata.size = strlen(pszAsync);
   strdata.persistent = bPersist;
   iReturnCode = param_write_string(plist,"async",&strdata);

   strdata.data = pszMonoDither;
   strdata.size = strlen(pszMonoDither);
   strdata.persistent = bPersist;
   iReturnCode = param_write_string(plist,"monodither",&strdata);

   strdata.data = pszServer;
   strdata.size = strlen(pszServer);
   strdata.persistent = bPersist;
   iReturnCode = param_write_string(plist,"server",&strdata);

   strdata.data = pszServer;
   strdata.size = strlen(pszServer);
   strdata.persistent = bPersist;
   iReturnCode = param_write_string(plist,"jobdialog",&strdata);

   iReturnCode = param_write_long(plist,"PelSizeCorrection",&ulPelSizeCorrection);

   if (fDebugOutput) dprintf1 ("GetDeviceParams returning %d\n", iReturnCode);

   return iReturnCode;
}

/*****************************************************************************/
/*                                                                           */
/*  FUNCTION: SetupDevice                                                    */
/*                                                                           */
/*  We need to set up our device and job specific information here           */
/*                                                                           */
/*****************************************************************************/

/* Put device parameters. */
/* IMPORTANT: async drivers must NOT CLOSE the device while doing put_params.*/

static int
SetupDevice (gx_device *pgxdev, gs_param_list *plist)
{
   int                     iReturnCode,
                           i;
   BOOL                    bRet;
   HWMARGINS               hwMargins;
   HWRESOLUTION            hwRes;
   gx_device_omni * const  prgxdev            = (gx_device_omni *)pgxdev;
   PDEVSTRUCT              pDev               = prgxdev->pDev;
   char                   *pszDeviceLib       = 0;
   static char             cOmnilib[]         = "libomni.so";
   static char             cDialogName[]      = "libomnijobdialog.so";
   static const char      *apszLibraryPaths[] = {
#if __WORDSIZE == 64
      "/usr/lib64/ghostscript/",
#else
      "/usr/lib/ghostscript/",
#endif
      "/usr/lib/Omni/",
      "/opt/Omni/bin/",
      "/opt/Omni/lib/",
      "./",
      ""                /* give a chance for $LD_LIBRARY_PATH to work*/
   };
   PRINTMODE               prtMode;           /* definition of printed output Bpp ..*/
   gs_param_string         fname;

   if (fDebugOutput) dprintf1 ("SetupDevice: pDev = %p\n", pDev);

   /* Do we have a handle? */
   if (pDev == NULL)
   {
      PDEVSTRUCT p;

      p = (PDEVSTRUCT)gs_malloc (gs_lib_ctx_get_non_gc_memory_t(), 1, sizeof (Omni_Dev), "omni/instance");
      if (!p)
      {
          dprintf ("<<<<<<<<<<<<<<<<<<<<<< ERROR >>>>>>>>>>>>>>>>>>>>>>>\n\n");
          dprintf ("Error: Failed allocation in SetupDevice\n");
          if (fDebugOutput) lprintf ("error was here.\n");

          return_error (gs_error_VMerror);
      }

      pDev = prgxdev->pDev = p;

      memset (pDev, 0, sizeof (Omni_Dev));

      pDev->pcoreOmni = (core_omni_device *)gs_malloc (gs_lib_ctx_get_non_gc_memory_t(), 1, sizeof (core_omni_device), "omni/device");

      if (!pDev->pcoreOmni)
      {
          eprintf ("<<<<<<<<<<<<<<<<<<<<<< ERROR >>>>>>>>>>>>>>>>>>>>>>>\n\n");
          eprintf ("Error: Failed allocation in SetupDevice\n");
          if (fDebugOutput) lprintf ("error was here.\n");

          return_error (gs_error_VMerror);
      }

      memset (pDev->pcoreOmni, 0, sizeof (core_omni_device));

      memcpy ((void *)pDev->pcoreOmni->cSignature, (const void *)Signature, 4);

      pDev->pcoreOmni->cbSize = sizeof (core_omni_device);
   }

   if (fDebugOutput) dprintf1 ("SetupDevice: pDev->hmodOmni = %p\n", pDev->hmodOmni);

   /* Is the handle initialized? */
   if (!pDev->hmodOmni)
   {
      #define dimof(a) (sizeof (a)/sizeof (a[0]))
      typedef struct _EntryPointList {
         char     *pszEntryPoint;
         gpointer *pfnEntryPoint;
      } ENTRYPOINTLIST, *PENTRYPOINTLIST;
      ENTRYPOINTLIST aeplEntryPoints[] = {
         { "GetPrintModeInfo",         (gpointer *)&pDev->pfnGetPrintModeInfo  },
         { "GetResolutionInfo",        (gpointer *)&pDev->pfnGetResolutionInfo },
         { "GetMarginInfo",            (gpointer *)&pDev->pfnGetMarginInfo     },
         { "BeginJob",                 (gpointer *)&pDev->pfnBeginJob          },
         { "NewFrame",                 (gpointer *)&pDev->pfnNewFrame          },
         { "EndJob",                   (gpointer *)&pDev->pfnEndJob            },
         { "Rasterize",                (gpointer *)&pDev->pfnRasterize         },
         { "CreateDevice",             (gpointer *)&pDev->pfnCreateDevice      },
         { "DeleteDevice",             (gpointer *)&pDev->pfnDeleteDevice      },
         { "DiffusionDither",          (gpointer *)&pDev->pfnMonoDither        },
         { "GhostscriptInferfaceInit", (gpointer *)&pDev->pfnGIInit            },
         { "GhostscriptInferfaceTerm", (gpointer *)&pDev->pfnGITerm            }
      };
      bool           fFailure = FALSE;
      int            i;

      pDev->ulPelSizeCorrection = 100;

      for (i = 0; i < sizeof (apszLibraryPaths)/sizeof (apszLibraryPaths[0]) && !pDev->hmodOmni; i++)
      {
          pszDeviceLib = (char *)gs_malloc (gs_lib_ctx_get_non_gc_memory_t(), 1,
                                            strlen (cOmnilib)
                                            + strlen (apszLibraryPaths[i])
                                            + 1,
                                            "Devicestring");
          if (pszDeviceLib)
          {
              sprintf (pszDeviceLib, "%s%s", apszLibraryPaths[i], cOmnilib);

              pDev->hmodOmni = g_module_open (pszDeviceLib, (GModuleFlags)0);

              if (fDebugOutput) dprintf2 ("SetupDevice: Trying to load %s = %p\n", pszDeviceLib, pDev->hmodOmni);
          }
          gs_free (gs_lib_ctx_get_non_gc_memory_t(), pszDeviceLib, strlen (pszDeviceLib) + 1, 1, "Devicestring");
      }

      if (!pDev->hmodOmni)
      {
         GModule *pModule = 0;

         /* Failure! */
         eprintf ("<<<<<<<<<<<<<<<<<<<<<< ERROR >>>>>>>>>>>>>>>>>>>>>>>\n\n");
         eprintf ("Error: Could not load libomni.so\n");
         if (fDebugOutput) lprintf ("error was here.\n");

         for (i = 0; i < sizeof (apszLibraryPaths)/sizeof (apszLibraryPaths[0]) && !pDev->hmodOmni; i++)
         {
             pszDeviceLib = (char *)gs_malloc (gs_lib_ctx_get_non_gc_memory_t(), 1,
                                               strlen (cOmnilib)
                                               + strlen (apszLibraryPaths[i])
                                               + 1,
                                               "Devicestring");
             if (pszDeviceLib)
             {
                 sprintf (pszDeviceLib, "%s%s", apszLibraryPaths[i], cOmnilib);

                 pModule = g_module_open (pszDeviceLib, (GModuleFlags)0);

                 if (!pModule)
                 {
                    eprintf2 ("Error: Loading \"%s\" returns \"%s\"\n", pszDeviceLib, g_module_error ());
                 }
                 else
                 {
                    g_module_close (pModule);
                 }
             }
             gs_free (gs_lib_ctx_get_non_gc_memory_t(), pszDeviceLib, strlen (pszDeviceLib) + 1, 1, "Devicestring");
         }

         return 1;
      }

      for (i = 0; i < (int)dimof (aeplEntryPoints); i++)
      {
         g_module_symbol (pDev->hmodOmni,
                          aeplEntryPoints[i].pszEntryPoint,
                          aeplEntryPoints[i].pfnEntryPoint);

         if (!*aeplEntryPoints[i].pfnEntryPoint)
         {
            if (!fFailure)
            {
               eprintf ("\n<<<<<<<<<<<<<<<<<<<<<< ERROR >>>>>>>>>>>>>>>>>>>>>>>\n\n");
            }

            eprintf1 ("Error: Could not load required entry point \"%s\" from libomni.so\n", aeplEntryPoints[i].pszEntryPoint);

            fFailure = TRUE;
         }

         if (fDebugOutput)
            dprintf2 ("SetupDevice: g_module_symbol (%s) = %p\n",
                      aeplEntryPoints[i].pszEntryPoint,
                     *aeplEntryPoints[i].pfnEntryPoint);
      }

      if (  !pDev->pfnCreateDevice
         || !pDev->pfnBeginJob
         || !pDev->pfnNewFrame
         || !pDev->pfnEndJob
         || !pDev->pfnRasterize
         || !pDev->pfnGetPrintModeInfo
         || !pDev->pfnGetResolutionInfo
         || !pDev->pfnGetMarginInfo
         || !pDev->pfnMonoDither
         )
      {
         /* Failure! */
         g_module_close (pDev->hmodOmni);
         pDev->hmodOmni = 0;

         return 1;
      }
   }

   /* device name in structure*/
   if (!pDev->pcoreOmni->cDeviceName[0])
   {
      memset (&fname, 0, sizeof (fname));
      iReturnCode = param_read_string (plist, "DeviceName", &fname);
      if (fDebugOutput) dprintf2 ("param_read_string of DeviceName = %d, fname.size = %d\n", iReturnCode, fname.size);

      if (  (iReturnCode < 1)
         && (fname.size > 0)
         )
      {
         memcpy (pDev->pcoreOmni->cDeviceName, fname.data, fname.size);
         pDev->pcoreOmni->cDeviceName[fname.size] = '\0';
      }
   }

   /* set debugging file name */
   if (!pDev->pcoreOmni->cDebugFile[0])
   {
      memset (&fname, 0, sizeof (fname));
      iReturnCode = param_read_string (plist, "dbgout", &fname);
      if (fDebugOutput) dprintf2 ("param_read_string of dbgout = %d, fname.size = %d\n", iReturnCode, fname.size);

      if (  (iReturnCode < 1)
         && (fname.size > 0)
         )
      {
         memcpy (pDev->pcoreOmni->cDebugFile, fname.data, fname.size);
         pDev->pcoreOmni->cDebugFile[fname.size] = '\0';
      }
   }

   /* set Async option */
   if (!pDev->cAsyncOpt[0])
   {
      memset (&fname, 0, sizeof (fname));
      iReturnCode = param_read_string (plist, "async", &fname);
      if (fDebugOutput) dprintf2 ("param_read_string of async = %d, fname.size = %d\n", iReturnCode, fname.size);
      if (  (iReturnCode < 1)
         && (fname.size > 0)
         )
      {
         memcpy (pDev->cAsyncOpt, fname.data, fname.size);
         pDev->cAsyncOpt[fname.size] = '\0';
         if (  !strcmp (pDev->cAsyncOpt, "TRUE")
            || !strcmp (pDev->cAsyncOpt, "true")
            )
         {
            pDev->iSync = 1;
         }
         else
         {
            pDev->iSync = 0;
         }
      }
   }

   iReturnCode = param_read_long (plist, "PelSizeCorrection", &pDev->ulPelSizeCorrection);
   if (fDebugOutput) dprintf2 ("param_read_long of PelSizeCorrection = %d, ulPelSizeCorrection = %ld\n", iReturnCode, pDev->ulPelSizeCorrection);

   /*
    *  Set user options for the job
    */
   if (!pDev->pcoreOmni->pszJobOptions)
   {
      memset (&fname, 0, sizeof (fname));
      iReturnCode = param_read_string (plist, "properties", &fname);
      if (fDebugOutput) dprintf2 ("param_read_string of properties = %d, fname.size = %d\n", iReturnCode, fname.size);

      if (  (iReturnCode < 1)
         && (fname.size > 0)
         )
      {
         if (!pDev->pcoreOmni->pszJobOptions)
            pDev->pcoreOmni->pszJobOptions = (char *)gs_malloc (gs_lib_ctx_get_non_gc_memory_t(), 1, fname.size + 1, "Option String");
         memcpy (pDev->pcoreOmni->pszJobOptions, fname.data, fname.size);
         pDev->pcoreOmni->pszJobOptions[fname.size] = '\0';
      }
   }

   /* Set user options for monodither by GS */
   if (!pDev->cMonoDither[0])
   {
      memset (&fname, 0, sizeof (fname));
      iReturnCode = param_read_string (plist, "monodither", &fname);
      if (fDebugOutput) dprintf2 ("param_read_string of monodither = %d, fname.size = %d\n", iReturnCode, fname.size);

      if (  (iReturnCode < 1)
         && (fname.size > 0)
         )
      {
         memcpy (pDev->cMonoDither, fname.data, fname.size);
         pDev->cMonoDither[fname.size] = '\0';

         if (!strcmp (pDev->cMonoDither, "GSMONO"))
         {
            if (fDebugOutput) dprintf ("turning on GSMONO dither\n");

            pDev->iGSMono = 1;
         }
         else
         {
            pDev->iGSMono = 0;
         }
      }
   }

   /* Set user options for client / server function */
   if (!pDev->pcoreOmni->cServer[0])
   {
      memset (&fname, 0, sizeof (fname));
      iReturnCode = param_read_string (plist, "server", &fname);
      if (fDebugOutput) dprintf2 ("param_read_string of server = %d, fname.size = %d\n", iReturnCode, fname.size);

      if (  (iReturnCode < 1)
         && (fname.size > 0)
         )
      {
         memcpy (pDev->pcoreOmni->cServer, fname.data, fname.size);

         pDev->pcoreOmni->cServer[fname.size] = '\0';

         dprintf1 ("Utilizing Server - %s \n", pDev->pcoreOmni->cServer);;

         pDev->iUseServer = 1;
      }
   }

   /*
    *  Set user options for dialog function
    */
   if (!pDev->iUseDialog)
   {
      memset (&fname, 0, sizeof (fname));
      iReturnCode = param_read_string (plist, "jobdialog", &fname);
      if (fDebugOutput) dprintf2 ("param_read_string of jobdialog = %d, fname.size = %d\n", iReturnCode, fname.size);

      if (!iReturnCode)
      {
         if (fDebugOutput) dprintf ("Utilizing dialog\n");

         pDev->iUseDialog = 1;
      }
   }

   if (  pDev->pcoreOmni->cDeviceName[0]
      && pDev->iSetup != 1
      )
   {
      /* create a device based on the user supplied inputs */
      if (fDebugOutput) dprintf  ("setting following options on CreateDevice\n");
      if (fDebugOutput) dprintf1 ("DeviceName = %s\n", pDev->pcoreOmni->cDeviceName );
      if (fDebugOutput) dprintf1 ("pszJobOptions = %s\n", pDev->pcoreOmni->pszJobOptions);
      if (fDebugOutput) dprintf1 ("debug parameter = %s \n", pDev->pcoreOmni->cDebugFile );

      if (pDev->iUseDialog)
      {
         int                       iLength                  = 0;
         char                     *pszSelectedJobProperties;
         PFNGETOMNIJOBPROPERTIES   pfnJobDialog;
         PFNFREEOMNIJOBPROPERTIES  pfnFreeDialogMemory;
         GModule                  *pModule                  = 0;

         for (i = 0; i < sizeof (apszLibraryPaths)/sizeof (apszLibraryPaths[0]) && !pModule; i++)
         {
            pszDeviceLib = (char *)gs_malloc (gs_lib_ctx_get_non_gc_memory_t(), 1,
                                              strlen (cDialogName)
                                              + strlen (apszLibraryPaths[i])
                                              + 1,
                                              "Devicestring");
            pszDeviceLib[0] = '\0';

            if (pszDeviceLib)
            {
                sprintf (pszDeviceLib, "%s%s", apszLibraryPaths[i], cDialogName);

                if (fDebugOutput) dprintf1 ("attempting to load - %s\n", pszDeviceLib);

                pModule = g_module_open (pszDeviceLib, (GModuleFlags)0);
            }
            gs_free (gs_lib_ctx_get_non_gc_memory_t(), pszDeviceLib, strlen (pszDeviceLib) + 1, 1, "Devicestring");
         }

         if (!pModule)
         {
            /* Print the error messages! */
            eprintf ("\n<<<<<<<<<<<<<<<<<<<<<< ERROR >>>>>>>>>>>>>>>>>>>>>>>\n\n");
            eprintf1 ("Error: Could not load %s\n", cDialogName);

            for (i = 0; i < sizeof (apszLibraryPaths)/sizeof (apszLibraryPaths[0]) && !pModule; i++)
            {
               pszDeviceLib = (char *)gs_malloc (gs_lib_ctx_get_non_gc_memory_t(), 1,
                                                 strlen (cDialogName)
                                                 + strlen (apszLibraryPaths[i])
                                                 + 1,
                                                 "Devicestring");
               pszDeviceLib[0] = '\0';

               if (pszDeviceLib)
               {
                   sprintf (pszDeviceLib, "%s%s", apszLibraryPaths[i], cDialogName);

                   pModule = g_module_open (pszDeviceLib, (GModuleFlags)0);

                   if (!pModule)
                   {
                      eprintf2 ("Error: Loading \"%s\" returns \"%s\"\n", pszDeviceLib, g_module_error ());
                   }
                   else
                   {
                      g_module_close (pModule);
                   }
               }
               gs_free (gs_lib_ctx_get_non_gc_memory_t(), pszDeviceLib, strlen (pszDeviceLib) + 1, 1, "Devicestring");
            }
         }
         else
         {
            g_module_symbol (pModule, "GetOmniJobProperties", (gpointer *)&pfnJobDialog);

            if (pfnJobDialog)
            {
               pszSelectedJobProperties = pDev->pcoreOmni->pszJobOptions;

               pfnJobDialog (pDev->pcoreOmni->cDeviceName, &pszSelectedJobProperties);

               if (pDev->pcoreOmni->pszJobOptions)
               {
                  gs_free (gs_lib_ctx_get_non_gc_memory_t(), pDev->pcoreOmni->pszJobOptions, strlen (pDev->pcoreOmni->pszJobOptions) + 1, 1, "Option String");
                  pDev->pcoreOmni->pszJobOptions = 0;
               }

               iLength = strlen (pszSelectedJobProperties);

               pDev->pcoreOmni->pszJobOptions = (char *)gs_malloc (gs_lib_ctx_get_non_gc_memory_t(), 1, iLength + 1, "Option String");

               strcpy (pDev->pcoreOmni->pszJobOptions, pszSelectedJobProperties);

               if (fDebugOutput) dprintf1 ("options = %s\n", pDev->pcoreOmni->pszJobOptions);

               g_module_symbol (pModule, "FreeOmniJobProperties", (gpointer *)&pfnFreeDialogMemory);

               if (pfnFreeDialogMemory)
               {
                  pfnFreeDialogMemory (&pszSelectedJobProperties);

                  if (fDebugOutput) dprintf ("Freeing Dialog Memory\n");
               }
            }
            else
            {
               eprintf1 ("Error: Unable to find <<GetOmniJobProperties>> in <<%s>>.\n", cDialogName);
            }

            g_module_close (pModule);
         }
      }

      if (pDev->iUseServer)
      {
         pDev->pcoreOmni->bPDCDevice = true;
      }

      /* get the omni version in case we need it */
      if (pDev->pfnGIInit)
      {
         pDev->pfnGIInit ((void*)pDev->pcoreOmni);

         if (fDebugOutput) dprintf1 ("core_omni_device size = %d\n",pDev->pcoreOmni->cbSize);
         if (fDebugOutput) dprintf1 ("GI version = %s\n",pDev->pcoreOmni->cVersion);

         pDev->pCallPtr = (void *)pDev->pcoreOmni;
      }
      else
      {
         pDev->pCallPtr = (void *)pDev->pcoreOmni->pDevice;
      }

      pDev->pcoreOmni->pDevice = pDev->pfnCreateDevice (pDev->pcoreOmni,
                                                        (void **)&pDev->hmodDevice,
                                                        pDev->iUseServer);

      if (!pDev->pcoreOmni->pDevice)
      {
         eprintf ("<<<<<<<<<<<<<<<<<<<<<< ERROR >>>>>>>>>>>>>>>>>>>>>>>\n\n");
         eprintf ("Error: Unable create a omni device\n");
         if (fDebugOutput) lprintf ("error was here.\n");

         return_error (gs_error_Fatal);  /* fail if no device was created */
      }

      if (fDebugOutput) dprintf1 ("Device = %p\n", pDev->pcoreOmni->pDevice);
      if (fDebugOutput) dprintf1 ("Omni version = %s\n", pDev->pcoreOmni->cOmniVersion);
      if (fDebugOutput) dprintf1 ("Device = \"%s\"\n", pDev->pcoreOmni->cDeviceName);

      bRet = pDev->pfnGetPrintModeInfo ((void*)pDev->pCallPtr, &prtMode);

      if (fDebugOutput) dprintf1 ("GetPrintModeInfo returns %d\n", bRet);
      if (fDebugOutput) dprintf1 ("GetPrintModeInfo.iBitCount = %d\n", prtMode.iBitCount);
      if (fDebugOutput) dprintf1 ("GetPrintModeInfo.iPlanes = %d\n", prtMode.iPlanes);

      if (bRet)
      {
         /* turn off Ghostscript mono if we are not in mono mode */
         if (  (prtMode.iBitCount > 1)
            && pDev->iGSMono
            )
         {
            if (fDebugOutput) dprintf1 ("Turning off GSMONO - prtMode.iBitCount = %d\n", prtMode.iBitCount);
            pDev->iGSMono = 0;
         }
      }
                         /* this routine is called multiple times */
      pDev->iSetup = 1;  /* don't go setting up new devices */

      /* get the hardware resolution information */

      bRet = pDev->pfnGetResolutionInfo ((void*)pDev->pCallPtr, &hwRes);

      if (!bRet)
      {
          eprintf ("<<<<<<<<<<<<<<<<<<<<<< ERROR >>>>>>>>>>>>>>>>>>>>>>>\n\n");
          eprintf ("Error: Failed GetResolutionInfo in SetupDevice\n");
          if (fDebugOutput) lprintf ("error was here.\n");

          return_error (gs_error_Fatal);
      }

      /* get the hardware page margins information */

      bRet = pDev->pfnGetMarginInfo ((void*)pDev->pCallPtr, &hwMargins);

      if (!bRet)
      {
          eprintf ("<<<<<<<<<<<<<<<<<<<<<< ERROR >>>>>>>>>>>>>>>>>>>>>>>\n\n");
          eprintf ("Error: Failed GetMarginInfo in SetupDevice\n");
          if (fDebugOutput) lprintf ("error was here.\n");

          return_error (gs_error_Fatal);
      }

      if (fDebugOutput) dprintf1 ("GetMarginInfo returns %d\n", bRet);
      if (fDebugOutput) dprintf1 ("GetMarginInfo.fLeftClip = %f\n", hwMargins.fLeftClip);
      if (fDebugOutput) dprintf1 ("GetMarginInfo.fBottomClip = %f\n", hwMargins.fBottomClip);
      if (fDebugOutput) dprintf1 ("GetMarginInfo.fRightClip = %f\n", hwMargins.fRightClip);
      if (fDebugOutput) dprintf1 ("GetMarginInfo.fTopClip = %f\n", hwMargins.fTopClip);
      if (fDebugOutput) dprintf1 ("GetMarginInfo.fxWidth = %f\n", hwMargins.fxWidth);
      if (fDebugOutput) dprintf1 ("GetMarginInfo.fyHeight = %f\n", hwMargins.fyHeight);

      /*
       *  We have to let GS know about our page and resolution parameters
       */

      /* setup the width and height of the page */
      prgxdev->width  = (int)hwMargins.fxWidth;
      prgxdev->height = (int)hwMargins.fyHeight;
      prgxdev->MediaSize[1] = (hwMargins.fyHeight/(float)hwRes.yRes)*72.0;
      prgxdev->MediaSize[0] = (hwMargins.fxWidth/(float)hwRes.xRes)*72.0;

      /* does not seem like pels works correctly for margins so lets set the margins */
      /* using points instead */

      /* left margin */
      prgxdev->HWMargins[0] = (float)hwMargins.fLeftClip/25400.0 *72.0;

      /* bottom margin */
      prgxdev->HWMargins[1] = (float)hwMargins.fBottomClip/25400.0 *72.0;

      /* right margin */
      prgxdev->HWMargins[2] = (float)hwMargins.fRightClip/25400.0 *72.0;

      /* top margin */
      prgxdev->HWMargins[3] = (float)hwMargins.fTopClip/25400.0 *72.0;

      /* set x resolution */
      prgxdev->HWResolution[0] = (float)hwRes.xRes;

      /* set y resolution */
      prgxdev->HWResolution[1] = (float)hwRes.yRes;

      prgxdev->pDev->iVertDots = (float)hwRes.fScanDots;  /* fill in the scan line */

      if (fDebugOutput) dprintf1 ("left margin pts   = %f \n", prgxdev->HWMargins[0]);
      if (fDebugOutput) dprintf1 ("bottom margin pts = %f \n", prgxdev->HWMargins[1]);
      if (fDebugOutput) dprintf1 ("right margin pts  = %f \n", prgxdev->HWMargins[2]);
      if (fDebugOutput) dprintf1 ("top margin pts    = %f \n", prgxdev->HWMargins[3]);

      if (!pDev->iGSMono)
      {
         /* 1,1,1,0,2,0       - mono   */
         /* 3, 24,255,255,5,5 - color  */
         if (fDebugOutput) dprintf1 ("depth = %d \n", prgxdev->color_info.depth              );
         if (fDebugOutput) dprintf1 ("num_comp = %d \n", prgxdev->color_info.num_components  );
         if (fDebugOutput) dprintf1 ("max_gray = %d \n", prgxdev->color_info.max_gray        );
         if (fDebugOutput) dprintf1 ("max_color = %d \n", prgxdev->color_info.max_color      );
         if (fDebugOutput) dprintf1 ("dither_grays = %d \n", prgxdev->color_info.dither_grays   );
         if (fDebugOutput) dprintf1 ("dither_colors = %d \n", prgxdev->color_info.dither_colors );

         prgxdev->color_info.depth          = 24;
         prgxdev->color_info.num_components = 3;
         prgxdev->color_info.max_gray       = 255;
         prgxdev->color_info.max_color      = 255;
         prgxdev->color_info.dither_grays   = 5;
         prgxdev->color_info.dither_colors  = 5;
      }
   }

   iReturnCode = gdev_prn_put_params ((gx_device *)prgxdev, plist);

   if (fDebugOutput) dprintf1 ("SetupDevice returning %d\n", iReturnCode);

   return iReturnCode;  /* likely not be a failure */
}

static int
omni_print_page (gx_device_printer *pdev, FILE *prn_stream)
{
   return PrintPage (pdev, prn_stream, 1);
}

/* ------------------------------------------------------*/
/* ------------------------------------------------------*/

static int
PrintPage (gx_device_printer *pgx_prt_dev, FILE *prn_stream, int num_copies)
{
   gx_device_omni * const pasyncDev        = (gx_device_omni *)pgx_prt_dev;
   gx_device * const      pgx_dev          = (gx_device *)pgx_prt_dev;
   uint                   uiLineSize       = gdev_prn_raster (pgx_prt_dev);  /* Raster line size  in bytes*/

   /* BMP scan lines need to be padded to 32 bits. */

   /*
    * The following variables are uses for outputting the bitmap information
    */
   uint                   uiBytesPerLine   = uiLineSize + (-uiLineSize & 3);  /*actual raster line size used for row */

   byte                  *pBitmapMem       = NULL;   /* bitmap memory for the bitmap band */
   bool                   bRet;
   RECTL                  rectPageLocation;
   SIZEL                  sizelPage;
   ULONG                  ulBandLength;              /* number of scan-lines to match the memory threshold for a band */
   int                    iBytesToAlloc;

   int                    iYBand;
   int                    code             = 0;
   int                    y,
                          iNumColors;
   byte                  *pGSData          = 0;
   byte                  *raster_data;

   /* MONO */
   byte                  *pMonoData        = NULL;
   DIFFUSIONDITHERINFO    DiffInfo;
   byte                  *pBuffer          = NULL;
   IMAGEINFOS             ImageInfo;
   POINTL                 ptlBounds;
   POINTL                 ptlTrgBounds;

   PRINTMODE              prtMode;                   /* definition of printed output Bpp .. */

   /* Get things setup to call into the driver code */
   BITMAPINFO2           *pbmi;

   PDEVSTRUCT             pDev             = pasyncDev->pDev;

   ptlBounds.x    = 0;
   ptlBounds.y    = 0;
   ptlTrgBounds.x = 0;
   ptlTrgBounds.y = 0;

   if (  !pDev
      || !pDev->pcoreOmni->pDevice
      )
   {
      eprintf ("<<<<<<<<<<<<<<<<<<<<<< ERROR >>>>>>>>>>>>>>>>>>>>>>>\n\n");
      eprintf ("Error: -sDeviceName=XXX is not specified\n");
      if (fDebugOutput) lprintf ("error was here.\n");

      return_error (gs_error_Fatal);
   }

   pDev->iPage++;
   pDev->iPageNumber++;  /*keep track of the page @@04162001 */

   bRet = pDev->pfnGetPrintModeInfo ((void*)pasyncDev->pDev->pCallPtr, &prtMode);

   iNumColors = 1 << prtMode.iBitCount;

   /* Allocate a full bitmapinfo2 and bitmap header message */
   iBytesToAlloc = sizeof (BITMAPINFO2);
   if (256 >= iNumColors)
   {
      iBytesToAlloc += (iNumColors - 1) * sizeof (RGB2);
   }

   pbmi = (PBITMAPINFO2)gs_malloc (gs_lib_ctx_get_non_gc_memory_t(), 1, iBytesToAlloc, "Bmpi Memory");
   if (!pbmi)
   {
      eprintf("<<<<<<<<<<<<<<<<<<<<<< ERROR >>>>>>>>>>>>>>>>>>>>>>>\n\n");
      eprintf("Error: Failed allocation of pbmi in PrintPage\n");
      if (fDebugOutput) lprintf ("error was here.\n");

      return_error (gs_error_VMerror);
   }

   pbmi->cbFix = sizeof(BITMAPINFO2); /* Length of fixed portion of structure?? */

   if (256 >= iNumColors)  /* must be 16 or 24 bit */
   {
      if (iNumColors == 2)
      {
         memset (&DiffInfo, 0xFF, sizeof (DiffInfo));
         memset (&ImageInfo, 0xFF, sizeof (ImageInfo));

         DiffInfo.ulLength    = sizeof (DiffInfo);
         DiffInfo.ulType      = GDM_MATRIX_DITHER;
         DiffInfo.fOptions    =  0;
         DiffInfo.ulIntensity = 80;
         DiffInfo.bRedWt      = 30; /*25 */
         DiffInfo.bGreenWt    = 50; /*60 */
         DiffInfo.bBlueWt     = 20; /*15 */
         DiffInfo.bPad        =  0;

         if (!pDev->iGSMono)
         {
            pbmi->argbColor[0].bBlue  = 0xFF;  /* background        */
            pbmi->argbColor[0].bRed   = 0xFF;
            pbmi->argbColor[0].bGreen = 0xFF;
            pbmi->argbColor[1].bBlue  = 0x00;  /* foreground        */
            pbmi->argbColor[1].bRed   = 0x00;
            pbmi->argbColor[1].bGreen = 0x00;
         }
         else
         {
            pbmi->argbColor[1].bBlue  = 0xFF;  /* background        */
            pbmi->argbColor[1].bRed   = 0xFF;
            pbmi->argbColor[1].bGreen = 0xFF;
            pbmi->argbColor[0].bBlue  = 0x00;  /* foreground        */
            pbmi->argbColor[0].bRed   = 0x00;
            pbmi->argbColor[0].bGreen = 0x00;
         }

         ImageInfo.pptlSrc             = &ptlBounds;       /* rectangle extents */
         ImageInfo.pptlDst             = &ptlTrgBounds;    /* rectangle extents */
         ImageInfo.ulSrcBpp            = 24;
         ImageInfo.ulSrcBytesPerLine   = uiBytesPerLine;   /* src bytes per line */
         ImageInfo.ulcSrcClrs          = 0;
         ImageInfo.ulSrcClrType        = 0;
         ImageInfo.pargb2Src           = 0;                /* if srcBPP != 24 */
         ImageInfo.ulTrgBpp            = 1;
         ImageInfo.ulTrgBytesPerLine   = pasyncDev->width + (-pasyncDev->width & 31);
         ImageInfo.ulTrgBytesPerLine  /= 8;
         ImageInfo.ulcTrgClrs          = 2;
         ImageInfo.ulTrgClrType        = 0;
         ImageInfo.pargb2Trg           = NULL;
         ImageInfo.ulOptions           = 0;            /* options */
         ImageInfo.ulPelSizeCorrection = pDev->ulPelSizeCorrection;

         ptlBounds.x = pasyncDev->width;             /* Bounding rectangle for input bitmap */
         ptlTrgBounds.x = pasyncDev->width;          /* Bounding rectangle for output bitmap */
      }
      else
      {
         /*
          * add for 256 color support -- @TBD ...
          */
      }
   }
   /* set the bitmap width to match the page */

   pbmi->cx = pgx_prt_dev->width;

   if (fDebugOutput) dprintf  ("***************************************************************\n");
   if (fDebugOutput) dprintf1 ("ulTrgBytesPerLine = %ld\n", ImageInfo.ulTrgBytesPerLine);
   if (fDebugOutput) dprintf1 ("ulBytesPerLine = %d\n", uiBytesPerLine);
   if (fDebugOutput) dprintf1 ("ulLineSize = %d\n", uiLineSize);
   if (fDebugOutput) dprintf1 ("width = %d\n", pgx_prt_dev->width);
   if (fDebugOutput) dprintf1 ("height = %d\n", pgx_prt_dev->height);
   if (fDebugOutput) dprintf ("***************************************************************\n");

   pbmi->cPlanes   = prtMode.iPlanes;   /* Number of bit planes                   */
   pbmi->cBitCount = prtMode.iBitCount; /*  number of bits per pel                */

   /*
    * BMP format is single page, so discard all but 1st printable page
    * This logic isn't quite right, since we can't truncate file if
    * num_pages == 0.
    */
   if (pasyncDev->copies_printed > 0)
     return 0;

   if (pDev->iSync)
   {
      /* If there's data in buffer, need to process w/overlays */
      if (pasyncDev->buffered_page_exists)
      {
         code = BufferPage(pgx_prt_dev, prn_stream, num_copies);
         goto done;
      }
   }

   pGSData = (byte *)gs_malloc (gs_lib_ctx_get_non_gc_memory_t(), uiBytesPerLine, 1, "bmp file buffer");

   if (pGSData == 0)
      /* can't allocate row buffer */
      return_error (gs_error_VMerror);

   /* seek to the file's end */
   if (pDev->iPage == 1)
   {
      fseek (prn_stream, 0L, SEEK_SET);
   }

   /* need to calculate the correct bandsize needed for the device */
   ulBandLength = FindBandSize (pasyncDev->height,
                                pasyncDev->width,
                                24,                         /* always use 24 bits for source bitmap */
                                pbmi->cPlanes,
                                pasyncDev->pDev->iVertDots,
                                8000*1024);                 /*eight meg buffer */

   pBitmapMem = (byte *) gs_malloc (gs_lib_ctx_get_non_gc_memory_t(), 1, uiBytesPerLine * ulBandLength, "Bitmap Memory");

   if(!pBitmapMem)
   {
      eprintf ("<<<<<<<<<<<<<<<<<<<<<< ERROR >>>>>>>>>>>>>>>>>>>>>>>\n\n");
      eprintf ("Error: Failed allocation of pBitmapMem in PrintPage\n");
      if (fDebugOutput) lprintf ("error was here.\n");

      return_error (gs_error_VMerror);
   }

   /* is the band bigger than the page ? */

   sizelPage.cx = pasyncDev->width;
   sizelPage.cy = pasyncDev->height;

   ulBandLength = ulBandLength >=sizelPage.cy ? sizelPage.cy : ulBandLength;

   if (pBitmapMem)
   {
      memset (pBitmapMem, 0xFF, uiBytesPerLine * ulBandLength ); /* memset the memory to white? maybe zero */
   }
   else
   {
      eprintf("<<<<<<<<<<<<<<<<<<<<<< ERROR >>>>>>>>>>>>>>>>>>>>>>>\n\n");
      eprintf("Error: Bitmap memory alloc Failed\n");
      if (fDebugOutput) lprintf ("error was here.\n");

      return_error (gs_error_VMerror);
   }

   /* Do initial setup for Rasterize for dithering work on the page. */

   rectPageLocation.xLeft  = 0;
   rectPageLocation.xRight = pasyncDev->width;


   if (pDev->iPageNumber == 1)
   {
      pDev->pcoreOmni->pfpOut = prn_stream;

      pDev->pfnBeginJob ((void*)pasyncDev->pDev->pCallPtr, prn_stream);
   }
   else
   {
      pDev->pfnNewFrame ((void*)pasyncDev->pDev->pCallPtr);
   }

/* rectPageLocation.yTop = rectPageLocation.yBottom  = 0;  @@10172000 */
   rectPageLocation.yTop = pasyncDev->height-1;       /* @@10172000 */

   /*
    *    Banding Implementation based on bandsize - need to put bits in place so that we can
    *    tell if this is a top to down or bottom to top printer.
    */
   {
      /*BEGIN BLOCK for Banding code */

/*    int iCurTop = 0, iCurBot = 0;  @@10172000 */

      /*
       *  Since this band needs to be flipped, we need to get a pointer to the top of memory
       *  so that we can copy from the top down into the band
       *  We also need to seek to the end of the file because this type of printer needs the
       *  last lines in the bitmap sent first
       */

      byte * pBitmapTop = pBitmapMem + ((ulBandLength-1) * uiBytesPerLine);

      iYBand = 0;

      /* Call DiffusionDither to setup pBuffer - quick in and out  @@04062001 */
      if (!pDev->iGSMono)
         pDev->pfnMonoDither (&DiffInfo,
                              &ImageInfo,
                              &pBuffer,
                              (unsigned long)GDM_DITHER_BEGIN);

      /* Go through the bitmap that we have been given */
      for (y = pasyncDev->height - 1; y >= 0; y--)
      {
         if (  (iYBand < ulBandLength)
            && ((code = dev_proc(pgx_dev, get_bits)(pgx_dev, pasyncDev->height - 1 - y, pGSData, &raster_data)) >= 0)
            && (y != 0)
            )
         {
            memcpy (pBitmapTop - (iYBand  * uiBytesPerLine), raster_data, uiBytesPerLine);

            iYBand++;  /* keep track of lines in the band */
         }
         else
         {
            if (  (code < 0)
               && (y > 0)
               )             /* add back in the line we failed to get if an error was returned from get_bits */
            {
               if (iYBand != ulBandLength)
               {
                  y++;
                  continue;
               }
            }

            /* we always decrement y when we come through here even if this is just a complete band */
            if (iYBand == ulBandLength)
               y++;

            /* we currently support 24bit or mono so if this is the mono case, allocate memory to be */
            /* used for the gray-scaled bitmap buffer */
            if (!pDev->iGSMono)
            {
               if (prtMode.iBitCount < 16)
               {
                  pMonoData = (byte *)gs_malloc (gs_lib_ctx_get_non_gc_memory_t(), 1, iYBand * ImageInfo.ulTrgBytesPerLine, "Mono Memory");

                  if (pMonoData)
                  {
                     memset (pMonoData, 0xFF, iYBand * ImageInfo.ulTrgBytesPerLine );
                  }
                  else
                  {
                     eprintf ("<<<<<<<<<<<<<<<<<<<<<< ERROR >>>>>>>>>>>>>>>>>>>>>>>\n\n");
                     eprintf ("Error: Failed allocation of pMonoData in PrintPage\n");
                     if (fDebugOutput) lprintf ("error was here.\n");

                     return_error (gs_error_VMerror);
                  }

                  ImageInfo.pptlSrc->y = ImageInfo.pptlDst->y = iYBand;               /* exclusive?? */
                  ImageInfo.pbSrcBits  = pBitmapTop - ((iYBand-1) * uiBytesPerLine);  /* pointer to src image data */
                  ImageInfo.pbTrgBits  = pMonoData;                                   /* pointer to dst image data */

                  /* Call DiffusionDither to generate a gray-scaled image from our 24bit bitmap */

                  pDev->pfnMonoDither (&DiffInfo, &ImageInfo, &pBuffer, (unsigned long) 0);
               }
            }

            pbmi->cy = iYBand;

            /* set the page location to account for the new band size (cy) */

            rectPageLocation.yBottom  = rectPageLocation.yTop - pbmi->cy ; /* @@10172000 */

            /* If this in a mono device, we need to send the mono data to the device and */
            /* not the color */

            if (  pMonoData
               && !pDev->iGSMono
               )
            {
               pDev->pfnRasterize ((void*)pasyncDev->pDev->pCallPtr,
                                   pMonoData,
                                   pbmi,
                                   &sizelPage,
                                   &rectPageLocation,
                                   BITBLT_BITMAP);

               /* We're done with the mono band */
               /* now free up the mono buffer so we can get clean data buffer if more lines are to be */
               /* gray-scaled */
               gs_free (gs_lib_ctx_get_non_gc_memory_t(), (char *)pMonoData, iYBand * ImageInfo.ulTrgBytesPerLine, 1, "Mono Memory");

               pMonoData = NULL;
            }
            else
            {  /* non-mono device -- Send the rgb bitmap to the dither and rasterize routines */
               if (fDebugOutput) dprintf3 ("rendering band lines %d - %d w/%d \n", (int)rectPageLocation.yTop, (int)rectPageLocation.yBottom, iYBand);

               if (fDebugOutput) dprintf  ("***************************************************************\n");
               if (fDebugOutput) dprintf  ("***************************************************************\n");
               if (fDebugOutput) dprintf1 ("Number of lines in band = %d\n", iYBand);
               if (fDebugOutput) dprintf1 ("rectPageLocation - Top = %ld\n", rectPageLocation.yTop);
               if (fDebugOutput) dprintf1 ("rectPageLocation - Bot = %ld\n", rectPageLocation.yBottom);
               if (fDebugOutput) dprintf  ("***************************************************************\n");
               if (fDebugOutput) dprintf  ("***************************************************************\n");

               pDev->pfnRasterize ((void*)pasyncDev->pDev->pCallPtr,
                                   pBitmapTop - ((iYBand-1) * uiBytesPerLine),
                                   pbmi,
                                   &sizelPage,
                                   &rectPageLocation,
                                   BITBLT_BITMAP);
                                                       /* memset the memory to white */
               memset (pBitmapMem, 0xFF, uiBytesPerLine * ulBandLength);
            }

            /*
	     * implemented based on the number of lines sent to the
             * printer and not based on the bandsize
	     */

            rectPageLocation.yTop -= iYBand;

            iYBand = 0;
         }
      }

      if (!pDev->iGSMono)
         pDev->pfnMonoDither (&DiffInfo,
                              &ImageInfo,
                              &pBuffer,
                              (unsigned long)GDM_DITHER_END);

      pBuffer = NULL;
   } /*END BLOCK*/

done:

   gs_free (gs_lib_ctx_get_non_gc_memory_t(), (char *) pBitmapMem, uiBytesPerLine * ulBandLength, 1, "Bitmap Memory");
   dprintf ("Page Completed\n");

   gs_free (gs_lib_ctx_get_non_gc_memory_t(), (char *)pGSData, uiBytesPerLine, 1, "bmp file buffer");
   gs_free (gs_lib_ctx_get_non_gc_memory_t(), (char *)pbmi, uiBytesPerLine, 1, "Bpmi Memory");

   /******************************************************************/
   /* Note:                                         @@08162000       */
   /*  GS has at least two threads for doing output on async devices.*/
   /*  There is a writer and a render thread running at one time and */
   /*  each have their own PageCount values.  The PageCount on the   */
   /*  writer thread gets updated but the render thread (us) does    */
   /*  not receive the update.  It looks like there needs to be a    */
   /*  way to update the thread's device structure that is doing the */
   /*  output with the new value when PageCount is changed.          */
   /*  If the thread doing the rasterizing queries the PageCount     */
   /*  value (calling put_params from inside of gxclrast.c) it will  */
   /*  stop rendering the page -- BAD....                            */
   /*  There needs to be a way to synchronize the two threads so that*/
   /*  when the writer's thread actually updates the PageCount value */
   /*  in both threads device structures get updated so we don't     */
   /*  stop rasterizing the page.                                    */
   /*                                                                */
   /*    |  |  |  |                                                  */
   /*   V  V  V  V                                                   */
   /******************************************************************/
   pgx_prt_dev->PageCount = pDev->iPage - 1;

   if (pasyncDev->copies_printed > 0)
   {
      pasyncDev->copies_printed = num_copies;
   }

   pasyncDev->buffered_page_exists = 0;

   return code;
}

/* -------------- Renderer instance procedures ----------*/

/* Thread to do rendering, started by StartRenderThread */
static void
RenderThread (void *params)
{
#ifdef OMNI_ASYNC
   gdev_prn_async_render_thread ((gdev_prn_start_render_params *)params);
#endif
}

/* ------------------------------------------------------*/
/* ------------------------------------------------------*/

static int /* rets 0 ok, -ve error if couldn't start thread */
StartRenderThread (gdev_prn_start_render_params *params)
{
   return gp_create_thread(RenderThread, params);
}

/* ------------------------------------------------------*/
/* ------------------------------------------------------*/

static int
OpenRenderDevice (gx_device_printer *ppdev)
{
   gx_device_omni * const pasyncDev = (gx_device_omni *)ppdev;

   /* Do anything that needs to be done at open time here... */
   pasyncDev->copies_printed = 0;

   /* Cascade down to the default handler */
#ifdef OMNI_ASYNC
   return gdev_prn_async_render_open(ppdev);
#else
   return gs_note_error(gs_error_undefined);
#endif
}

/* ------------------------------------------------------*/
/* ------------------------------------------------------*/

/* Buffer a (partial) rasterized page & optionally print result multiple times. */
static int
BufferPage (gx_device_printer *pgx_prt_dev, FILE *file, int num_copies)
{
#ifndef OMNI_ASYNC

   /* locate_overlay_buffer is gone, so for now async printing is disabled */
   return gs_note_error (gs_error_undefined);

#else
   gx_device_omni * const pasyncDev = (gx_device_omni *)pgx_prt_dev;
   gx_device * const      pgx_dev   = (gx_device *)pgx_prt_dev;
   int                    code      = 0;

   /* BMP format is single page, so discard all but 1st page */
   if (pasyncDev->copies_printed > 0)
      return 0;

   /* If there's no data in buffer, no need to do any overlays */
   if (!pasyncDev->buffered_page_exists)
   {
      code = PrintPage(pgx_prt_dev, file, num_copies);

      goto done;
   }

   /*
    * Overlay file's bits on top of existing file There are two choices for
    * doing this: get_overlay_bits vs.  the combination of
    * locate_overlay_buffer and get_bits. If you already have a buffer in a
    * format compatible with GS's format, use get_overlay_bits. If you'd
    * rather use the buffer already in the device, use
    * locate_overlay_buffer, copy the bits into the returned buffer, then
    * get_bits.
    *
    * Either way, try to do entire bands at a shot for much greater
    * efficiency.
    */

   /* Seek to beginning of data portion of file */
   if (fseek (file, pasyncDev->file_offset_to_data, SEEK_SET))
   {
      code = gs_note_error (gs_error_ioerror);

      goto done;
   }

   {
      byte  *raster_data;
      int    raster           = gx_device_raster (pgx_dev, 1);
      ulong  bmp_raster       = raster + (-raster & 3); /* BMP scan lines are padded to 32 bits. */
      int    max_band_height  = (*pgx_prt_dev->printer_procs.locate_overlay_buffer) (pgx_prt_dev, 0, &raster_data);
      int    band;
      int    file_raster_good = min (raster, bmp_raster);
      long   file_raster_slop = bmp_raster - file_raster_good;

      /*
       * iterate thru bands from top to bottom.
       * Do this an entire band at a time for efficiency.
       */
      for (band = (pgx_prt_dev->height - 1) / max_band_height; band >= 0; --band)
      {
         int band_base_line = max_band_height * band;
         int band_height    = (*pgx_prt_dev->printer_procs.locate_overlay_buffer) (pgx_prt_dev, band_base_line, &raster_data);
         int line;

         /* Fill in overlay buffer with a band from the BMP file. */
         /* Need to do this backward since BMP is top to bottom */
         for (line = band_height - 1; line >= 0; --line)
         {
            if (  fread (raster_data + line * bmp_raster,
                         file_raster_good,
                         1,
                         file) < 1
               || fseek (file, file_raster_slop, SEEK_CUR)
               )
            {
               code = gs_note_error (gs_error_ioerror);

               goto done;
            }
         }

         /* Rewind & write out buffer with contents of get_bits */
         if (fseek (file,
                    -(file_raster_good + file_raster_slop) * band_height,
                    SEEK_CUR))
         {
            code = gs_note_error(gs_error_ioerror);

            goto done;
         }

         for (line = band_height - 1; line >= 0; --line)
         {
            if ((code = dev_proc (pgx_dev, get_bits) (pgx_dev, line + band_base_line, 0, &raster_data)) < 0 )
               goto done;

            if (  fwrite (raster_data, file_raster_good, 1, file) < 1
               || fseek(file, file_raster_slop, SEEK_CUR)
               )
            {
               code = gs_note_error(gs_error_ioerror);

               goto done;
            }
         }
      }
   }

done:
   if (  code >= 0
      && pasyncDev->copies_printed > 0
      )
      pasyncDev->copies_printed = num_copies;

   pasyncDev->buffered_page_exists = (code >= 0);

   return code;

#endif
}

/* ------------------------------------------------------*/
/*------------ Procedures common to writer & renderer -------- */
/* ------------------------------------------------------*/

/* Compute space parameters */
static void
GetSpaceParams (const gx_device_printer *pgx_prt_dev,
                gdev_prn_space_params   *space_params)
{
   /* Plug params into device before opening it
    *
    * You ask "How did you come up with these #'s?" You asked, so...
    *
    * To answer clearly, let me begin by recapitulating how command list
    * (clist) device memory allocation works in the non-overlapped case:
    * When the device is opened, a buffer is allocated. How big? For
    * starters, it must be >= PRN_MIN_BUFFER_SPACE, and as we'll see, must
    * be sufficient to satisfy the rest of the band params. If you don't
    * specify a size for it in space_params.band.BandBufferSpace, the open
    * routine will use a heuristic where it tries to use PRN_BUFFER_SPACE,
    * then works its way down by factors of 2 if that much memory isn't
    * available.
    *
    * The device proceeds to divide the buffer into several parts: one of
    * them is used for the same thing during writing & rasterizing; the
    * other parts are redivided and used differently writing and
    * rasterizing. The limiting factor dictating memory requirements is the
    * rasterizer's render buffer.  This buffer needs to be able to contain
    * a bitmap that covers an entire band. Memory consumption is whatever
    * is needed to hold N rows of data aligned on word boundaries, +
    * sizeof(pointer) for each of N rows. Whatever is left over in the
    * rasterized is allocated to a tile cache. You want to make sure that
    * cache is at least 50KB.
    *
    * For example, take a 600 dpi b/w device at 8.5 x 11 inches.  For the
    * whole device, that's 6600 rows @ 638 bytes = ~4.2 MB total.  If the
    * device is divided into 100 bands, each band's rasterizer buffer is
    * 62K. Add on a 50K tile cache, and you get a 112KB (+ add a little
    * slop) total device buffer size.
    *
    * Now that we've covered the rasterizer, let's switch back to the
    * writer. The writer must have a tile cache *exactly* the same size as
    * the reader. This means that the space to divide up for the writer is
    * equal is size to the rasterizer's band buffer.  This space is divided
    * into 2 sections: per-band bookeeping info and a command buffer. The
    * bookeeping info currently uses ~72 bytes for each band. The rest is
    * the command buffer.
    *
    * To continue the same 112KB example, we have 62KB to slice up.
    * We need 72 bytes * 100 bands = 7.2KB, leaving a 55K command buffer.
    *
    * A larger command buffer has some performance (see gxclmem.c comments)
    * advantages in the general case, but is critical in one special case:
    * high-level images. Whenever possible, images are transmitted across
    * the band buffer in their original resolution and bits/pixel. The
    * alternative fallback behavior can be very slow.  Here, the relevant
    * restriction is that at least one entire source image row must fit
    * into the command buffer. This means that, in our example, an RGB
    * source image would have to be <= 18K pixels wide. If the image is
    * sampled at the same resolution as the hardware (600 dpi), that means
    * the row would be limited to a very reasonable 30 inches. However, if
    * the source image is sampled at 2400 dpi, that limit is only 7.5
    * inches. The situation gets worse as bands get smaller, but the
    * implementor must decide on the tradeoff point.
    *
    * The moral of the story is that you should never make a band
    * so small that its buffer limits the command buffer excessively.
    * Again, Max image row bytes = band buffer size - # bands * 72.
    *
    * In the overlapped case, everything is exactly as above, except that
    * two identical devices, each with an identical buffer, are allocated:
    * one for the writer, and one for the rasterizer. Because it's critical
    * to allocate identical buffers, I *strongly* recommend setting these
    * params in the writer's open routine:
    * space_params.band.BandBufferSpace, .BandWidth and .BandHeight.  If
    * you don't force these values to a known value, the memory allocation
    * heuristic may not come to the same result for both copies of the
    * device, since the first allocation will diminish the amount of free
    * memory.
    *
    * There is room for an important optimization here: allocate the
    * writer's space with enough memory for a generous command buffer, but
    * allocate the reader with only enough memory for a band rasterization
    * buffer and the tile cache.  To do this, observe that the space_params
    * struct has two sizes: BufferSpace vs. BandBufferSpace.  To start,
    * BandBufferSpace is always <= BufferSpace. On the reader side,
    * BandBufferSpace is divided between the tile cache and the rendering
    * buffer -- that's all the memory that's needed to rasterize. On the
    * writer's side, BandBufferSpace is divided the same way: the tile
    * cache (which must be identical to the reader's) is carved out, and
    * the space that would have been used for a rasterizing buffer is used
    * as a command buffer. However, you can further increase the cmd buf
    * further by setting BufferSize (not BandBufferSize) to a higher number
    * than BandBufferSize. In that case, the command buffer is increased by
    * the difference (BufferSize - BandBufferSize). There is logic in the
    * memory allocation for printers that will automatically use BufferSize
    * for writers (or non-async printers), and BandBufferSize for readers.
    *
    * Note: per the comments in gxclmem.c, the banding logic will perform
    * better with 1MB or better for the command list.
    */

   /* This will give us a very "ungenerous" buffer. */
   /* Here, my arbitrary rule for min image row is: twice the dest width */
   /* in full RGB. */
   ulong       render_space = 0;
   ulong       writer_space;
   const int tile_cache_space = 50 * 1024;
   const int min_image_rows   = 2;
   int       min_row_space    = min_image_rows * (3 * (pgx_prt_dev->width + sizeof (int) - 1));
   int       min_band_count   = max (1, pgx_prt_dev->height / 100); /* make bands >= 1% of total */

   space_params->band.BandWidth  = pgx_prt_dev->width;
   space_params->band.BandHeight = (pgx_prt_dev->height + min_band_count - 1) / min_band_count;

   gdev_mem_data_size ((const gx_device_memory *)pgx_prt_dev, space_params->band.BandWidth,
			space_params->band.BandHeight, &render_space);

   /* need to include minimal writer requirements to satisfy rasterizer init */
   writer_space = 5000 /* add 5K slop for good measure */
                + (72 + 8) * ((pgx_prt_dev->height / space_params->band.BandHeight) + 1);

   space_params->band.BandBufferSpace = max (render_space, writer_space) + tile_cache_space;
   space_params->BufferSpace = max (render_space, writer_space + min_row_space) + tile_cache_space;
}

/*-----------------------------------------------------------------------------*/
/*                                                                             */
/* Function: FindBandSize                                                      */
/*  This function figures out the appropriate band size based on the amount    */
/*  of memory that can be occupied by the band and bitmap information.         */
/*  bitmap, and the specifics of the hardware                                  */
/*                                                                             */
/*-----------------------------------------------------------------------------*/
ULONG
FindBandSize (ULONG ulYHeight,
              ULONG ulXWidth,
              ULONG ulBitsPerPel,
              ULONG ulNumPlanes,
              ULONG ulModulus,
              ULONG ulMemoryLimit)
{
   ULONG ulSizeScanLine;
   ULONG ulMemoryNeeded;
   ULONG ulNumLinesFit;

   if (0 == ulModulus)
   {
      ulModulus = 1;
   }

   /* figure out how much memory is needed fore each line */
   ulSizeScanLine = ((ulBitsPerPel*ulXWidth+31)/32)*ulNumPlanes*4;

   /* Figure out how much memory is needed for the page */
   ulMemoryNeeded = ulYHeight * ulSizeScanLine;

   /* How many lines can fit in the size given? */
   ulNumLinesFit = ulMemoryLimit / ulSizeScanLine;

   if (0 == ulNumLinesFit)
      /* Minimum of 1 scan line */
      ulNumLinesFit = 1;

   if (ulNumLinesFit <= ulModulus)
      /* Not enough lines... Promote it to a modulus. */
      ulNumLinesFit = ulModulus;
   else
      /* Bump down the number of lines so that it is a modulus. */
      ulNumLinesFit -= ulNumLinesFit % ulModulus;

   if ((ulYHeight % ulNumLinesFit) * 100 / ulYHeight <= 15)
   {
      USHORT  usBumpUp;

      usBumpUp  = ulYHeight % ulNumLinesFit;
      usBumpUp += ulModulus - 1;
      usBumpUp /= ulModulus;
      usBumpUp *= ulModulus;
      ulNumLinesFit += usBumpUp;
   }

   return ulNumLinesFit ;  /* return the number of lines we want for the band */
}
