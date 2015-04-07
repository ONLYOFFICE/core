#ifndef _DESTINATION_H
#define _DESTINATION_H

#pragma once 

#include "Objects.h"


//----- DestinationObject -----------------------------------------------------------------------

Destination   DestinationNew       (MMgr oMMgr, Page pTarget, Xref pXref);
BOOL          DestinationValidate  (Destination pDst);
//----- Внешние функции --------------------------------------------------------------------------
unsigned long DestinationSetXYZ    (Destination pDst, float fLeft, float fTop, float fZoom);
unsigned long DestinationSetFit    (Destination pDst);
unsigned long DestinationSetFitH   (Destination pDst, float fTop);
unsigned long DestinationSetFitV   (Destination pDst, float fLeft);
unsigned long DestinationSetFitR   (Destination pDst, float fLeft, float fBottom, float fRight, float fTop);
unsigned long DestinationSetFitB   (Destination pDst);
unsigned long DestinationSetFitBH  (Destination pDst, float fTop);
unsigned long DestinationSetFitBV  (Destination pDst, float fLeft);
#endif /* _DESTINATION_H */

