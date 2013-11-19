/*******************************************************************************/
/*                                                                             */
/* FILE:  defs.h                                                               */
/*                                                                             */
/*  Header file to satisfy defines for gomni.c                                .*/
/*                                                                             */
/*                                                                             */
/* Created: 03-21-2000                                                         */
/*                                                                             */
/*                                                                             */
/*                                                                             */
/*******************************************************************************/
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
 */

#define APIENTRY
#define LOCAL

#ifndef FALSE
#define FALSE   (0)
#endif

#ifndef TRUE
#define TRUE    (!FALSE)
#endif

typedef unsigned char BYTE;

typedef unsigned char *PBYTE;

typedef int INT;
typedef unsigned int UINT;

typedef INT *PINT;

typedef INT BOOL;
typedef BOOL *PBOOL;
typedef unsigned short WORD;
typedef unsigned long DWORD;

typedef char CHAR;
typedef unsigned char UCHAR;
typedef short SHORT;
typedef SHORT *PSHORT;
typedef unsigned short USHORT;
typedef USHORT *PUSHORT;
typedef long LONG;
typedef LONG *PLONG;
typedef unsigned long ULONG;
typedef ULONG *PULONG;
typedef unsigned long HFILE;
typedef HFILE *PHFILE;

typedef void VOID;
typedef VOID *PVOID;
typedef PVOID *PPVOID;

typedef struct _SIZEL            /* sizl */
{
   LONG cx;
   LONG cy;
} SIZEL;
typedef SIZEL *PSIZEL;


typedef struct _POINTL  {
    LONG  x;
    LONG  y;
} POINTL;
typedef POINTL *PPOINTL;

typedef struct _RECTL {                 /* rcl */
    LONG  xLeft;
    LONG  yBottom;
    LONG  xRight;
    LONG  yTop;
} RECTL;
typedef RECTL *PRECTL;

typedef struct _RGB {                   /* rgb */
    BYTE bBlue;
    BYTE bGreen;
    BYTE bRed;
} RGB;
/* typedef RGB *PRGB; */

typedef LONG APIRET;

/* Extract high and low order parts of 16 and 32 bit quantity */
#define LOBYTE(w)       LOUCHAR(w)
#define HIBYTE(w)       HIUCHAR(w)
#define LOUCHAR(w)      ((UCHAR)(w))
#define HIUCHAR(w)      ((UCHAR)(((USHORT)(w) >> 8) & 0xff))
#define LOUSHORT(l)     ((USHORT)((ULONG)l))
#define HIUSHORT(l)     ((USHORT)(((ULONG)(l) >> 16) & 0xffff))


#define CLR_FALSE                     (-5L)
#define CLR_TRUE                      (-4L)

#define CLR_ERROR                   (-255L)
#define CLR_DEFAULT                   (-3L)
#define CLR_WHITE                     (-2L)
#define CLR_BLACK                     (-1L)
#define CLR_BACKGROUND                  0L
#define CLR_BLUE                        1L
#define CLR_RED                         2L
#define CLR_PINK                        3L
#define CLR_GREEN                       4L
#define CLR_CYAN                        5L
#define CLR_YELLOW                      6L
#define CLR_NEUTRAL                     7L

#define CLR_DARKGRAY                    8L
#define CLR_DARKBLUE                    9L
#define CLR_DARKRED                    10L
#define CLR_DARKPINK                   11L
#define CLR_DARKGREEN                  12L
#define CLR_DARKCYAN                   13L
#define CLR_BROWN                      14L
#define CLR_PALEGRAY                   15L

/* rgb colors */
#define RGB_ERROR                   (-255L)
#define RGB_BLACK              0x00000000L
#define RGB_BLUE               0x000000FFL
#define RGB_GREEN              0x0000FF00L
#define RGB_CYAN               0x0000FFFFL
#define RGB_RED                0x00FF0000L
#define RGB_PINK               0x00FF00FFL
#define RGB_YELLOW             0x00FFFF00L
#define RGB_WHITE              0x00FFFFFFL



#define BMAP_MEMORY          0x00000001

/* bitmap parameterization used by GpiCreateBitmap and others */
typedef struct _BITMAPINFOHEADER         /* bmp */
{
   ULONG  cbFix;
   USHORT cx;
   USHORT cy;
   USHORT cPlanes;
   USHORT cBitCount;
} BITMAPINFOHEADER;
typedef BITMAPINFOHEADER *PBITMAPINFOHEADER;


typedef struct _BMAPINFO          /* bmapinfo */
{
    ULONG  ulLength;
    ULONG  ulType;
    ULONG  ulWidth;
    ULONG  ulHeight;
    ULONG  ulBpp;
    ULONG  ulBytesPerLine;
    PBYTE  pBits;
} BMAPINFO;
typedef BMAPINFO  *PBMAPINFO;

typedef struct _BITMAPINFOHEADER2        /* bmp2  */
{
   ULONG  cbFix;            /* Length of structure                    */
   ULONG  cx;               /* Bit-map width in pels                  */
   ULONG  cy;               /* Bit-map height in pels                 */
   USHORT cPlanes;          /* Number of bit planes                   */
   USHORT cBitCount;        /* Number of bits per pel within a plane  */
   ULONG  ulCompression;    /* Compression scheme used to store the bitmap */
   ULONG  cbImage;          /* Length of bit-map storage data in bytes*/
   ULONG  cxResolution;     /* x resolution of target device          */
   ULONG  cyResolution;     /* y resolution of target device          */
   ULONG  cclrUsed;         /* Number of color indices used           */
   ULONG  cclrImportant;    /* Number of important color indices      */
   USHORT usUnits;          /* Units of measure                       */
   USHORT usReserved;       /* Reserved                               */
   USHORT usRecording;      /* Recording algorithm                    */
   USHORT usRendering;      /* Halftoning algorithm                   */
   ULONG  cSize1;           /* Size value 1                           */
   ULONG  cSize2;           /* Size value 2                           */
   ULONG  ulColorEncoding;  /* Color encoding                         */
   ULONG  ulIdentifier;     /* Reserved for application use           */
} BITMAPINFOHEADER2;
typedef BITMAPINFOHEADER2 *PBITMAPINFOHEADER2;

typedef struct _RGB2         /* rgb2 */
{
   BYTE bBlue;              /* Blue component of the color definition */
   BYTE bGreen;             /* Green component of the color definition*/
   BYTE bRed;               /* Red component of the color definition  */
   BYTE fcOptions;          /* Reserved, must be zero                 */
} RGB2;
typedef RGB2 *PRGB2;

typedef struct _BITMAPINFO2      /* bmi2 */
{
   int  cbFix;
   int  cx;
   int  cy;
   int  cPlanes;
   int  cBitCount;
   int  ulCompresstion;
   int  cclrUsed;
   int  cclrImportant;
   RGB2 argbColor[1];
} BITMAPINFO2;
typedef BITMAPINFO2 *PBITMAPINFO2;


/******************************************************************************/
/*    PPOINTL     pptlSrc;            // src left, bottom, width and height.  */
/*    PPOINTL     pptlDst;            // dst left, bottom, width and height.  */
/*    ULONG       ulSrcBytesPerLine;  // src bytes per line                   */
/*    PBYTE       pbSrcBits;          // pointer to src image data            */
/*    ULONG       ulTrgBytesPerLine;  // dst bytes per line                   */
/*    PBYTE       pbTrgBits;          // pointer to dst image data            */
/*    ULONG       ulOptions;          // options                              */
/*    ULONG       ulcClrs;            // count of colors in rgb2 table        */
/*    PRGB2       pargb2;             // rgb2 table                           */
/******************************************************************************/

typedef struct _IMAGEINFOS          /* imginfo */
{
   PPOINTL     pptlSrc;
   PPOINTL     pptlDst;
   ULONG       ulSrcBpp;
   ULONG       ulSrcBytesPerLine;
   PBYTE       pbSrcBits;
   ULONG       ulcSrcClrs;
   ULONG       ulSrcClrType;
   PRGB2       pargb2Src;
   ULONG       ulTrgBpp;
   ULONG       ulTrgBytesPerLine;
   PBYTE       pbTrgBits;
   ULONG       ulcTrgClrs;
   ULONG       ulTrgClrType;
   PRGB2       pargb2Trg;
   ULONG       ulOptions;
   ULONG       ulPelSizeCorrection;
} IMAGEINFOS;
typedef IMAGEINFOS * PIMAGEINFO;    /* pimginfo */

#define  GDM_NO_DITHER               0x0000   /* No dithering in system                 */
#define  GDM_USERDEF_DITHER          0x0001   /* Users supplies own Dithering routines  */
#define  GDM_MATRIX_DITHER           0x0002   /* Use System ordered dithering           */
#define  GDM_ERRORDIF_DITHER         0x0004   /* Use System error diffusion dithering   */
#define  GDM_DITHER_BEGIN            0x0008   /* Use System Floyd-Steinberg dithering   */
#define  GDM_DITHER_END              0x0010   /* Use System error propigation dithering */
#define  GDM_COLOR_CONVERT           0x0020   /* use device's clr mapping functions     */


/*******************************************************************************************/
/*    ULONG       ulLength;      // length of structure                 - 88               */
/*    ULONG       ulType;        // type of dither info structure       - GDM_MATRIX_DITHER*/
/*    ULONG       fOptions;      // dither info options - DI_MONOINVERT - 00               */
/*    ULONG       ulIntensity;   // RGB Gama Correction Value           - 00               */
/*    BYTE        bRedWt;        // weight of primary color red         - 25               */
/*    BYTE        bGreenWt;      // weight of primary color green       - 60               */
/*    BYTE        bBlueWt;       // weight of primary color blue        - 15               */
/*    BYTE        bPad;          // 4 byte align                        - 00               */
/*    SIZEL       szMatrix;      // halftone pattern size               - 8                */
/*    BYTE        bHalftone[];   // array of halftone patterns          - see 32gdata.c    */
/*******************************************************************************************/

typedef struct _MATRIXDITHERINFO      /* mtrxdi */
{
   ULONG       ulLength;
   ULONG       ulType;
   ULONG       fOptions;
   ULONG       ulIntensity;
   BYTE        bRedWt;
   BYTE        bGreenWt;
   BYTE        bBlueWt;
   BYTE        bPad;
   SIZEL       szMatrix;
   BYTE        bHalftone[1];
} MATRIXDITHERINFO;
typedef MATRIXDITHERINFO *PMDI;  /* pmtrxdi */

#define STUCKI_DIF 0x01
#define JJN_DIF    0x02
#define RND_DIF    0x04
#define USER_DIF   0x08

typedef struct _DIFFUSIONDITHERINFO      /* difdi */
{
   ULONG       ulLength;
   ULONG       ulType;        /* Stucki, user defined, etc. */
   ULONG       fOptions;
   ULONG       ulIntensity;
   BYTE        bRedWt;
   BYTE        bGreenWt;
   BYTE        bBlueWt;
   BYTE        bPad;
   PBYTE       pBuffer;
   SIZEL       szFilter;
   BYTE        bFilterArry[1];

} DIFFUSIONDITHERINFO;
typedef DIFFUSIONDITHERINFO *PDDI;  /* pdifdi */

/* bitblt type */
typedef enum {
    BITBLT_BITMAP,
    BITBLT_AREA,
    BITBLT_TEXT
}  BITBLT_TYPE;
