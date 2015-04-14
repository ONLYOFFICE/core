#ifndef _ANNOTATION_H
#define _ANNOTATION_H

#pragma once 

#include "Objects.h"

Annotation    LinkAnnotNew              (MMgr oMMgr, Xref pXref, TRect oRect, Destination pDst);
Annotation    TextAnnotNew              (MMgr oMMgr, Xref pXref, TRect oRect, const char *sText, EncoderRecPtr pEncoder);
Annotation    URILinkAnnotNew           (MMgr oMMgr, Xref pXref, TRect oRect, const char *sUri);
//------ Внешние функции --------------------------------------------------------------------------
unsigned long AnnotationSetBorderStyle  (Annotation  pAnnot, BSSubtype eSubtype, float fWidth, unsigned short nDashOn, unsigned short nDashOff, unsigned short nDashPhase);
unsigned long LinkAnnotSetBorderStyle   (Annotation  pAnnot, float fWidth, unsigned short nDashOn, unsigned short nDashOff );
unsigned long LinkAnnotSetHighlightMode (Annotation  pAnnot, AnnotHighlightMode eMode);
unsigned long TextAnnotSetIcon          (Annotation  pAnnot, AnnotIcon eIcon);
unsigned long TextAnnotSetOpened        (Annotation  pAnnot, bool bOpened);
#endif /* _ANNOTATION_H */
