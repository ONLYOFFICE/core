#ifndef _PAGEOPERATIONS_H
#define _PAGEOPERATIONS_H

#pragma once 

#include "Utils.h"
#include "Pages.h"

#include "math.h"

static const TPoint   c_oInitPosition = {0, 0};
static const DashMode c_oInitDashMode = {{0, 0, 0, 0, 0, 0, 0, 0}, 0, 0};

float AngToEllPrm(float fAngle, float fXRad, float fYRad);

unsigned long PageSetLineWidth         (Page pPage, float fLineWidth);
unsigned long PageSetLineCap           (Page pPage, LineCapStyle eLineCap);
unsigned long PageSetLineJoin          (Page pPage, LineJoinStyle eLineJoin);
unsigned long PageSetMiterLimit        (Page pPage, float fMiterLimit);
unsigned long PageSetDash              (Page pPage, const double *pdDashPtn, unsigned int nNumParam, float fPhase);
unsigned long PageSetFlat              (Page pPage, float fFlatness);
unsigned long PageSetExtGState         (Page pPage, ExtGState pExtGState);
unsigned long PageGSave                (Page pPage);
unsigned long PageGRestore             (Page pPage);
unsigned long PageConcat               (Page pPage, float fA, float fB, float fC, float fD, float fX, float fY);
unsigned long PageMoveTo               (Page pPage, float fX, float fY);
unsigned long PageLineTo               (Page pPage, float fX, float fY);
unsigned long PageCurveTo              (Page pPage, float fX1, float fY1, float fX2, float fY2, float fX3, float fY3);
unsigned long PageCurveTo2             (Page pPage, float fX2, float fY2, float fX3, float fY3);
unsigned long PageCurveTo3             (Page pPage, float fX1, float fY1, float fX3, float fY3);
unsigned long PageClosePath            (Page pPage);
unsigned long PageRectangle            (Page pPage, float fX, float fY, float fWidth, float fHeight);
unsigned long PageStroke               (Page pPage);
unsigned long PageClosePathStroke      (Page pPage);
unsigned long PageFill                 (Page pPage);
unsigned long PageEoFill               (Page pPage);
unsigned long PageFillStroke           (Page pPage);
unsigned long PageEoFillStroke         (Page pPage);
unsigned long PageClosePathFillStroke  (Page pPage);
unsigned long PageClosePathEoFillStroke(Page pPage);
unsigned long PageEndPath              (Page pPage);
unsigned long PageClip                 (Page pPage);
unsigned long PageEoclip               (Page pPage);
unsigned long PageBeginText            (Page pPage);
unsigned long PageEndText              (Page pPage);
unsigned long PageSetCharSpace         (Page pPage, float fValue);
unsigned long PageSetWordSpace         (Page pPage, float fValue);
unsigned long PageSetHorizontalScalling(Page pPage, float fValue);
unsigned long PageSetTextLeading       (Page pPage, float fValue);
unsigned long PageSetFontAndSize       (Page pPage, FontDict pFont, float fSize);
unsigned long PageSetTextRenderingMode (Page pPage, TextRenderingMode eMode);
unsigned long PageSetTextRise          (Page pPage, float fValue);
unsigned long PageMoveTextPos          (Page pPage, float fX, float fY);
unsigned long PageMoveTextPos2         (Page pPage, float fX, float fY);
unsigned long PageSetTextMatrix        (Page pPage, float fA, float fB, float fC, float fD, float fX, float fY);
unsigned long PageMoveToNextLine       (Page pPage);
unsigned long PageShowText             (Page pPage, const BYTE *sText, unsigned int nLen, unsigned int *pCIDs, unsigned int nLenCID);
unsigned long PageShowTextNextLine     (Page pPage, const BYTE *sText, unsigned int nLen, unsigned int *pCIDs, unsigned int nLenCID);
unsigned long PageShowTextNextLineEx   (Page pPage, float fWordSpace, float fCharSpace, const BYTE *sText, unsigned int nLen, unsigned int *pCIDs, unsigned int nLenCID);
unsigned long PageSetGrayStroke  (Page pPage, float fGray);
unsigned long PageSetGrayFill    (Page pPage, float fGray);
unsigned long PageSetRGBStroke   (Page pPage, float fR, float fG, float fB);
unsigned long PageSetRGBFill     (Page pPage, float fR, float fG, float fB);
unsigned long PageSetCMYKStroke  (Page pPage, float fC, float fM, float fY, float fK);
unsigned long PageSetCMYKFill    (Page pPage, float fC, float fM, float fY, float fK);
unsigned long PageExecuteXObject  (Page pPage, XObject pXobj);
unsigned long PageCircle      (Page pPage, float fX, float fY, float fRad);
unsigned long PageEllipse     (Page pPage, float fX, float fY, float fXRay, float fYRay);
unsigned long PageEllipseArc  (Page pPage, float fX, float fY, float fXRad, float fYRad, float fAngle1, float fAngle2, BOOL bClockDirection);
unsigned long PageEllipseArc2 (Page pPage, float fX, float fY, float fXRad, float fYRad, float fAngle1, float fAngle2, BOOL bClockDirection = FALSE);
unsigned long PageArc         (Page pPage, float fX, float fY, float fRay, float fAng1, float fAng2);
unsigned long PageDrawImage   (Page pPage, ImageDict pImage, float fX, float fY, float fWidth, float fHeight);
unsigned long PageTextOut     (Page pPage, float fXPos, float fYPos, const BYTE *sText, unsigned int nLen, unsigned int *pCIDs, unsigned int nLenCID);
unsigned long PageTextRect    (Page pPage, float fLeft, float fTop, float fRight, float fBottom, const char *sText, TextAlignment eAlign, unsigned int *pnLen, unsigned int *pCIDs, unsigned int nLenCID);
unsigned long PageSetSlideShow(Page pPage, TransitionStyle eType, float fDispTime, float fTransTime);
#endif /* _PAGEOPERATIONS_H */