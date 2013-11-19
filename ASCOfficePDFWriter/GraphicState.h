#ifndef _GSTATE_H
#define _GSTATE_H

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

GState GStateNew  (MMgr oMMgr, GState pCurrent)
{
    GState  pGState;

	if ( pCurrent && pCurrent->nDepth >= LIMIT_MAX_GSTATE ) 
	{
        SetError( oMMgr->oError, AVS_OFFICEPDFWRITER_ERROR_EXCEED_GSTATE_LIMIT, 0);
        return NULL;
    }

    pGState = (GState)GetMem( oMMgr, sizeof(GStateRec) );

	if ( !pGState )
        return NULL;

    if ( pCurrent ) 
	{
		pGState->oTransMatrix = pCurrent->oTransMatrix;
		pGState->fLineWidth   = pCurrent->fLineWidth;
		pGState->eLineCap     = pCurrent->eLineCap;
		pGState->eLineJoin    = pCurrent->eLineJoin;
		pGState->fMiterLimit  = pCurrent->fMiterLimit;
		pGState->oDashMode    = pCurrent->oDashMode;
		pGState->fFlatness    = pCurrent->fFlatness;

		pGState->fCharSpace     = pCurrent->fCharSpace;
		pGState->fWordSpace     = pCurrent->fWordSpace;
		pGState->fHScalling     = pCurrent->fHScalling;
		pGState->fTextLeading   = pCurrent->fTextLeading;
		pGState->eRenderingMode = pCurrent->eRenderingMode;
		pGState->fTextRise      = pCurrent->fTextRise;

		pGState->eCSStroke   = pCurrent->eCSStroke;
		pGState->eCSFill     = pCurrent->eCSFill;
		pGState->oRGBFill    = pCurrent->oRGBFill;
		pGState->oRGBStroke  = pCurrent->oRGBStroke;
		pGState->oCMYKFill   = pCurrent->oCMYKFill;
		pGState->oCMYKStroke = pCurrent->oCMYKStroke;
		pGState->fGrayFill   = pCurrent->fGrayFill;
		pGState->fGrayStroke = pCurrent->fGrayStroke;

        pGState->pFont     = pCurrent->pFont;
        pGState->fFontSize = pCurrent->fFontSize;

        pGState->pPrev  = pCurrent;
        pGState->nDepth = pCurrent->nDepth + 1;
    } 
	else 
	{
        TransMatrix DEF_MATRIX     = {1, 0, 0, 1, 0, 0};
        RGBColor    DEF_RGB_COLOR  = {0, 0, 0};
        CMYKColor   DEF_CMYK_COLOR = {0, 0, 0, 0};
        DashMode    DEF_DASH_MODE  = {{0, 0, 0, 0, 0, 0, 0, 0}, 0, 0};
		ColorSpace  DEF_CS = CSDeviceGray;
		WritingMode DEF_WRITING_MODE = WModeHorizontal;

        pGState->oTransMatrix = DEF_MATRIX;
        pGState->fLineWidth   = DEF_LINEWIDTH;
        pGState->eLineCap     = DEF_LINECAP;
        pGState->eLineJoin    = DEF_LINEJOIN;
        pGState->fMiterLimit  = DEF_MITERLIMIT;
        pGState->oDashMode    = DEF_DASH_MODE;
        pGState->fFlatness    = DEF_FLATNESS;

        pGState->fCharSpace     = DEF_CHARSPACE;
        pGState->fWordSpace     = DEF_WORDSPACE;
        pGState->fHScalling     = DEF_HSCALING;
        pGState->fTextLeading   = DEF_LEADING;
        pGState->eRenderingMode = DEF_RENDERING_MODE;
        pGState->fTextRise      = DEF_RISE;

        pGState->eCSStroke   = DEF_CS;
        pGState->eCSFill     = DEF_CS;
        pGState->oRGBFill    = DEF_RGB_COLOR;
        pGState->oRGBStroke  = DEF_RGB_COLOR;
        pGState->oCMYKFill   = DEF_CMYK_COLOR;
        pGState->oCMYKStroke = DEF_CMYK_COLOR;
        pGState->fGrayFill   = 0;
        pGState->fGrayStroke = 0;

        pGState->pFont        = NULL;
        pGState->fFontSize    = 0;
        pGState->eWritingMode = DEF_WRITING_MODE;

        pGState->pPrev  = NULL;
        pGState->nDepth = 1;
    }

    return pGState;
}


GState GStateFree (MMgr oMMgr, GState pGState)
{
    GState pCurrent = NULL;

    if ( pGState ) 
	{
		pCurrent = pGState->pPrev;
        FreeMem( oMMgr, pGState);
    }

    return pCurrent;
}



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

BOOL          ExtGStateValidate            (ExtGState pExtGState)
{
    if ( !pExtGState || (pExtGState->pHeader.nObjClass != (OSUBCLASS_EXT_GSTATE | OCLASS_DICT) && pExtGState->pHeader.nObjClass != (OSUBCLASS_EXT_GSTATE_R | OCLASS_DICT) ) )
        return FALSE;

    return TRUE;
}

unsigned long ExtGStateCheck               (ExtGState pExtGState)
{
    if ( !ExtGStateValidate( pExtGState ) )
        return AVS_OFFICEPDFWRITER_ERROR_INVALID_OBJECT;
    
    if ( pExtGState->pHeader.nObjClass == (OSUBCLASS_EXT_GSTATE_R | OCLASS_DICT) )
        return RaiseError( pExtGState->oError, AVS_OFFICEPDFWRITER_ERROR_EXT_GSTATE_READ_ONLY, 0);

    return OK;
}

ExtGState     ExtGStateNew                 (MMgr oMMgr, Xref pXref)
{
    ExtGState pObj = (ExtGState)DictNew( oMMgr );

    if ( !pObj )
        return NULL;

    if ( OK != XrefAdd( pXref, pObj ) )
        return NULL;

    if ( OK != DictAddName( pObj, "Type", "ExtGState" ) )
        return NULL;

	pObj->pHeader.nObjClass |= OSUBCLASS_EXT_GSTATE;

    return pObj;
}

unsigned long ExtGStateSetAlphaStroke      (ExtGState pExtGState, float fValue)
{
    unsigned long nRet = ExtGStateCheck( pExtGState );
    
    if ( OK != nRet )
        return nRet;
    
    if ( fValue < 0 || fValue > 1.0f )
        return RaiseError( pExtGState->oError, AVS_OFFICEPDFWRITER_ERROR_EXT_GSTATE_OUT_OF_RANGE, 0);

    return DictAddReal( pExtGState, "CA", fValue );
}

unsigned long ExtGStateSetAlphaFill        (ExtGState pExtGState, float fValue)
{
    unsigned long nRet = ExtGStateCheck( pExtGState );
    
    if ( OK != nRet )
        return nRet;

    if ( fValue < 0 || fValue > 1.0f )
		return RaiseError( pExtGState->oError, AVS_OFFICEPDFWRITER_ERROR_EXT_GSTATE_OUT_OF_RANGE, 0 );

    return DictAddReal( pExtGState, "ca", fValue );
}

unsigned long ExtGStateSetBlendMode        (ExtGState pExtGState, BlendMode eBlendMode)
{
    unsigned long nRet = ExtGStateCheck( pExtGState );
    
    if ( OK != nRet )
        return nRet;

    if ( (int)eBlendMode < 0 || (int)eBlendMode > (int)BMEOF )
		return RaiseError( pExtGState->oError, AVS_OFFICEPDFWRITER_ERROR_EXT_GSTATE_OUT_OF_RANGE, 0);

    return DictAddName( pExtGState, "BM", BM_NAMES[ (int)eBlendMode ] );
}

unsigned long ExtGStateSetStrokeAdjustment (ExtGState pExtGState, BOOL bValue)
{
    unsigned long nRet = ExtGStateCheck( pExtGState );
    
    if ( OK != nRet )
        return nRet;

    return DictAddBoolean( pExtGState, "SA", bValue);
}


#endif /* _GSTATE_H */

