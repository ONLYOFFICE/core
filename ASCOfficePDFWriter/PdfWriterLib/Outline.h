#ifndef _OUTLINE_H
#define _OUTLINE_H

#pragma once 

#include "Objects.h"
#include "Utils.h"
#include "Destination.h"

#define OUTLINE_CLOSED     0
#define OUTLINE_OPENED     1

//----- OutlineObject ---------------------------------------------------------

Outline OutlineRootNew  (MMgr oMMgr, Xref pXref);
Outline OutlineNew      (MMgr oMMgr, Outline pParent, const char *sTitle, EncoderRecPtr pEncoder, Xref pXref);
Outline OutlineGetFirst (Outline pOutline);
Outline OutlineGetLast  (Outline pOutline);
Outline OutlineGetPrev  (Outline pOutline);
Outline OutlineGetNext  (Outline pOutline);
Outline OutlineGetParent(Outline pOutline);
bool    OutlineGetOpened(Outline pOutline);
bool    OutlineValidate (Outline pOutline);


//----------------------------------------------------------------------------
unsigned long OutlineSetDestination(Outline pOutline, Destination pDst);
unsigned long OutlineSetOpened     (Outline pOutline, bool bOpened);
//----------------------------------------------------------------------------

#endif /* _OUTLINE_H */

