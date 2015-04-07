#ifndef _GSTATE_H
#define _GSTATE_H

#pragma once 

#include "Font.h"
#include "Utils.h"
#include "Objects.h"

//------ graphic state stack object ------------------------------------------

typedef struct TGStateRec *GState;

typedef struct TGStateRec 
{
    TransMatrix        oTransMatrix;
    float              fLineWidth;
    LineCapStyle       eLineCap;
    LineJoinStyle      eLineJoin;
    float              fMiterLimit;
    DashMode           oDashMode;
    float              fFlatness;

    float              fCharSpace;
    float              fWordSpace;
    float              fHScalling;
    float              fTextLeading;
    TextRenderingMode  eRenderingMode;
    float              fTextRise;

    ColorSpace         eCSFill;
    ColorSpace         eCSStroke;
    RGBColor           oRGBFill;
    RGBColor           oRGBStroke;
    CMYKColor          oCMYKFill;
    CMYKColor          oCMYKStroke;
    float              fGrayFill;
    float              fGrayStroke;

    FontDict           pFont;
    float              fFontSize;
    WritingMode        eWritingMode;

    GState             pPrev;
    unsigned int       nDepth;
} GStateRec;

GState GStateNew  (MMgr oMMgr, GState pCurrent);
GState GStateFree (MMgr oMMgr, GState pGState);
//------ Extension of grapchic state ----------------------------------------

const static char  *BM_NAMES[] = 
{
	"Normal",
	"Multiply",
	"Screen",
	"Overlay",
	"Darken",
	"Lighten",
	"ColorDodge",
	"ColorBurn",
	"HardLight",
	"SoftLight",
	"Difference",
	"Exclusion"
};

BOOL          ExtGStateValidate            (ExtGState pExtGState);
unsigned long ExtGStateCheck               (ExtGState pExtGState);
ExtGState     ExtGStateNew                 (MMgr oMMgr, Xref pXref);
unsigned long ExtGStateSetAlphaStroke      (ExtGState pExtGState, float fValue);
unsigned long ExtGStateSetAlphaFill        (ExtGState pExtGState, float fValue);
unsigned long ExtGStateSetBlendMode        (ExtGState pExtGState, BlendMode eBlendMode);
unsigned long ExtGStateSetStrokeAdjustment (ExtGState pExtGState, BOOL bValue);
#endif /* _GSTATE_H */

