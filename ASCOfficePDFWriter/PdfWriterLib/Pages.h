#ifndef _PAGES_H
#define _PAGES_H

#pragma once 

#include "GraphicState.h"
#include "Utils.h"
#include "Annotation.h"
#include "Destination.h"


//-------- Дополнительные определения и вспомогательные функции -------------

typedef struct TPageSizeValue 
{
    float   fX;
    float   fY;
} PageSizeValue;

static PageSizeValue c_asPredefinedPageSizes[] = 
{
    {612, 792},           /* PAGE_SIZE_LETTER */
    {612, 1008},          /* PAGE_SIZE_LEGAL */
    {841.89, 1199.551},   /* PAGE_SIZE_A3 */
    {595.276, 841.89},    /* PAGE_SIZE_A4 */
    {419.528, 595.276},   /* PAGE_SIZE_A5 */
    {708.661, 1000.63},   /* PAGE_SIZE_B4 */
    {498.898, 708.661},   /* PAGE_SIZE_B5 */
    {522, 756},           /* PAGE_SIZE_EXECUTIVE */
    {288, 432},           /* PAGE_SIZE_US4x6 */
    {288, 576},           /* PAGE_SIZE_US4x8 */
    {360, 504},           /* PAGE_SIZE_US5x7 */
    {297, 684}            /* PAGE_SIZE_COMM10 */
};


static const RGBColor  c_oDefaultRBGColor  = {0, 0, 0};

static const CMYKColor c_oDefaultCMYKColor = {0, 0, 0, 0};

static const char *c_asInheritableEntries[5] = 
{
    "Resources",
    "MediaBox",
    "CropBox",
    "Rotate",
    NULL
};

typedef struct TPageAttrRec  *PageAttr;

typedef struct TPageAttrRec 
{
    Pages          pParent;
    Dict           pFonts;
    Dict           pXObjects;
    Dict           pExtGStates;
    GState         pGState;
    TPoint         oStrPos;
    TPoint         oCurPos;
    TPoint         oTextPos;
    TransMatrix    oTextMatrix;
    unsigned short nGMode;
    Dict           pContents;
    StreamRecPtr         pStream;
    Xref           pXref;
    unsigned int   nCompressionMode;
	PDFVer        *peVer; 
} PageAttrRec;

#include "PageOperations.h"


//----- PagesObject -----------------------------------------------------------
unsigned long PagesAddKids     (Pages pParent, Dict pKid);
Pages         PagesNew         (MMgr  oMMgr, Pages pParent, Xref pXref);
bool          PagesValidate    (Pages pPages);
unsigned long PageInsertBefore (Page  pPage, Page pTarget);
//----- PageObject ------------------------------------------------------------
bool          PageValidate            (Page pPage);
Page          PageNew                 (MMgr oMMgr, Xref pXref);
void*         PageGetInheritableItem  (Page pPage, const char *sKey, unsigned short nObjClass);
const char*   PageGetXObjectName      (Page pPage, XObject pXobj);
const char*   PageGetLocalFontName    (Page pPage, FontDict pFont);
const char*   PageGetExtGStateName    (Page pPage, ExtGState pState);
Box           PageGetMediaBox         (Page pPage);
unsigned long PageSetBoxValue         (Page pPage, const char *sName, unsigned int nIndex, float fValue);
void          PageSetFilter           (Page pPage, unsigned int nFilter);
unsigned long PageCheckState          (Page pPage, unsigned int nMode);
//------ Внешние функции ------------------------------------------------------
float             PageTextWidth            (Page pPage, const BYTE *sText, unsigned int nLen, unsigned int *pCIDs, unsigned int nLenCID);
unsigned int      PageMeasureText          (Page pPage, const char *sText, unsigned int *pCIDs, unsigned int nLenCID, float fWidth, bool bWordWrap, float *fRealWidth);
float             PageGetWidth             (Page pPage);
float             PageGetHeight            (Page pPage);
FontDict          PageGetCurrentFont       (Page pPage);
float             PageGetCurrentFontSize   (Page pPage);
TransMatrix       PageGetTransMatrix       (Page pPage);
float             PageGetLineWidth         (Page pPage);
LineCapStyle      PageGetLineCap           (Page pPage);
LineJoinStyle     PageGetLineJoin          (Page pPage);
float             PageGetMiterLimit        (Page pPage);
DashMode          PageGetDash              (Page pPage);
float             PageGetFlat              (Page pPage);
float             PageGetWordSpace         (Page pPage);
float             PageGetCharSpace         (Page pPage);
float             PageGetHorizontalScalling(Page pPage);
float             PageGetTextLeading       (Page pPage);
TextRenderingMode PageGetTextRenderingMode (Page pPage);
float             PageGetTextRise          (Page pPage);
RGBColor          PageGetRGBFill           (Page pPage);
RGBColor          PageGetRGBStroke         (Page pPage);
CMYKColor         PageGetCMYKFill          (Page pPage);
CMYKColor         PageGetCMYKStroke        (Page pPage);
float             PageGetGrayFill          (Page pPage);
float             PageGetGrayStroke        (Page pPage);
ColorSpace        PageGetStrokingColorSpace(Page pPage);
ColorSpace        PageGetFillingColorSpace (Page pPage);
TransMatrix       PageGetTextMatrix        (Page pPage);
unsigned int      PageGetGStateDepth       (Page pPage);
unsigned short    PageGetGMode             (Page pPage);
TPoint            PageGetCurrentPos        (Page pPage);
unsigned long     PageGetCurrentPos2       (Page pPage, TPoint *poPos);
TPoint            PageGetCurrentTextPos    (Page pPage);
unsigned long     PageGetCurrentTextPos2   (Page pPage, TPoint *poPos);
unsigned long     PageSetRotate            (Page pPage, unsigned short nAngle);
unsigned long     PageSetWidth             (Page pPage, float fValue);
unsigned long     PageSetHeight            (Page pPage, float fValue);
unsigned long     PageSetSize              (Page pPage, PageSizes eSize, PageDirection eDirection);
Destination       PageCreateDestination    (Page pPage);
Annotation        PageCreateTextAnnot      (Page pPage, TRect oRect, const char *sText, EncoderRecPtr pEncoder);
Annotation        PageCreateLinkAnnot      (Page pPage, TRect oRect, Destination pDest);
Annotation        PageCreateUriLinkAnnot   (Page pPage, TRect oRect, const char *sUri);
#endif /* _PAGES_H */

