#ifndef _WMF_REGION_H
#define _WMF_REGION_H

#include "WmfTypes.h"

typedef bool (*pProcO)    (TWmfRegion*, TWmfRectF*, TWmfRectF*, TWmfRectF*, TWmfRectF*, float, float);
typedef bool (*pProcNonO) (TWmfRegion*, TWmfRectF*, TWmfRectF*, float, float);

static bool REGION_CopyRegion      (TWmfRegion *pDst, TWmfRegion *pSrc);
static bool REGION_UnionRegion     (TWmfRegion *pNewRegion, TWmfRegion *pRegionA, TWmfRegion *pRegionB);
static void REGION_IntersectRegion (TWmfRegion *pNewRegion, TWmfRegion *pRegionA, TWmfRegion *pRegionB);
static void REGION_SubtractRegion  (TWmfRegion *pNewRegion, TWmfRegion *pRegionA, TWmfRegion *pRegionB);

static bool REGION_UnionO    (TWmfRegion *pRegion, TWmfRectF *pRect1, TWmfRectF *pRect1End, TWmfRectF *pRect2, TWmfRectF *pRect2End, float fTop, float fBottom);
static bool REGION_UnionNonO (TWmfRegion *pRegion, TWmfRectF *pRect,  TWmfRectF *pRectEnd, float fTop, float fBottom);
static bool REGION_SubtractO (TWmfRegion *pRegion, TWmfRectF *pRect1, TWmfRectF *pRect1End, TWmfRectF *pRect2, TWmfRectF *pRect2End, float fTop, float fBottom);
static bool REGION_IntersectO(TWmfRegion *pRegion, TWmfRectF *pRect1, TWmfRectF *pRect1End, TWmfRectF *pRect2, TWmfRectF *pRect2End, float fTop, float fBottom);

static void REGION_RegionOp (TWmfRegion *pNewRegion, TWmfRegion *pRegionA, TWmfRegion *pRegionB, pProcO pOverlapFunc, pProcNonO pNonOverlap1Func, pProcNonO pNonOverlap2Func);

static unsigned int REGION_Coalesce (TWmfRegion *pRegion, unsigned int unPrevStart, unsigned int unCurStart);
static void REGION_SetExtents (TWmfRegion *pRegion);

static TWmfRectF *RegionMemChk(TWmfRegion *pRegion)
{	
	if ( pRegion->unNumRects < ( pRegion->unSize - 1 ) ) 
		return pRegion->pRects + pRegion->unNumRects;

	TWmfRectF *pMore = (TWmfRectF *)realloc( pRegion->pRects, (pRegion->unSize + 8) * sizeof (TWmfRectF));

	if ( NULL == pMore ) 
		return NULL;

	pRegion->pRects  = pMore;
	pRegion->unSize += 8;

	return pRegion->pRects + pRegion->unNumRects;
}



static bool       RectMerge   (TWmfRegion *pRegion, TWmfRectF *pRect, float fTop, float fBottom)
{
	TWmfRectF *pNextRect = 0;
	TWmfRectF *pPrior    = 0;

	if ( NULL == ( pNextRect = RegionMemChk( pRegion ) ) ) 
		return false;

	if ( pRegion->unNumRects == 0 )
	{	
		pRegion->unNumRects++;

		pNextRect->oTL.fX = pRect->oTL.fX;
		pNextRect->oTL.fY = fTop;
		pNextRect->oBR.fX = pRect->oBR.fX;
		pNextRect->oBR.fY = fBottom;
		
		pNextRect++;
	}
	else
	{	
		pPrior = pNextRect - 1;
		if ( ( pPrior->oTL.fY == fTop ) && ( pPrior->oBR.fY == fBottom ) && ( pPrior->oBR.fX >= pRect->oTL.fX ) )
		{	
			if ( pPrior->oBR.fX < pRect->oBR.fX ) 
				pPrior->oBR.fX = pRect->oBR.fX;
		}
		else
		{	
			pRegion->unNumRects++;

			pNextRect->oTL.fX = pRect->oTL.fX;
			pNextRect->oTL.fY = fTop;
			pNextRect->oBR.fX = pRect->oBR.fX;
			pNextRect->oBR.fY = fBottom;
		
			pNextRect++;
		}
	}

	return true;
}

// 1 если прямоугольники пересекаются
// 0 если не пересекаются
#define EXTENTCHECK(pRegion1,pRegion2)      \
	(  ((pRegion1)->oBR.fX > (pRegion2)->oTL.fX) \
	&& ((pRegion1)->oTL.fX < (pRegion2)->oBR.fX) \
	&& ((pRegion1)->oBR.fY > (pRegion2)->oTL.fY) \
	&& ((pRegion1)->oTL.fY < (pRegion2)->oBR.fY) )

static void WmfSetRectRegion(TWmfRegion *pRegion, TWmfRectF *pRect)
{	
	if ( ( NULL == pRect ) || ( pRect->oTL.fX == pRect->oBR.fX ) || ( pRect->oTL.fY == pRect->oBR.fY ) )
	{	
		pRegion->oExtents.oTL.fX = 0;
		pRegion->oExtents.oTL.fY = 0;
		pRegion->oExtents.oBR.fX = 0;
		pRegion->oExtents.oBR.fY = 0;

		pRegion->unNumRects = 0;
		pRegion->ushType = NULLREGION;
	}
	else
	{	
		pRegion->oExtents = (*pRect);
		(*(pRegion->pRects)) = (*pRect);

		pRegion->unNumRects = 1;
		pRegion->ushType = SIMPLEREGION;
	}
}

static void WmfCombineRegion(TWmfRegion *pDestObj, TWmfRegion *pSrcObjA, TWmfRegion *pSrcObjB, unsigned short ushMode)
{
	if ( ( NULL == pDestObj ) || ( NULL == pSrcObjA ) ) 
		return;

	if ( RGN_COPY == ushMode )
	{	
		REGION_CopyRegion( pDestObj, pSrcObjA );
		return;
	}

	if ( pSrcObjB )
	{	
		switch ( ushMode )
		{
		case RGN_OR:
			REGION_UnionRegion( pDestObj, pSrcObjA, pSrcObjB );
			break;

		case RGN_AND:
			REGION_IntersectRegion( pDestObj, pSrcObjA, pSrcObjB );
			break;
#if 0
		case RGN_XOR:
			REGION_XorRegion (API,destObj,src1Obj,src2Obj);
		break;
#endif
		case RGN_DIFF:
			REGION_SubtractRegion( pDestObj, pSrcObjA, pSrcObjB );
		break;
		}
	}
}

static bool REGION_UnionRegion(TWmfRegion *pNewRegion, TWmfRegion *pRegionA, TWmfRegion *pRegionB)
{
	
	// Сначала проверим все простейшие случаи

	// Области совпадают, или область А пустая
	if ( ( pRegionA == pRegionB ) || ( !( pRegionA->unNumRects ) ) )
	{	
		if ( pNewRegion != pRegionB ) 
			return REGION_CopyRegion( pNewRegion, pRegionB );
		return true;
	}

	// Область В пустая
	if ( 0 == pRegionB->unNumRects )
	{	
		if ( pNewRegion != pRegionA ) 
			return REGION_CopyRegion( pNewRegion, pRegionA );
		return true;
	}

	// Область B содержится в области A
	if ( ( pRegionA->unNumRects == 1 ) && ( pRegionA->oExtents.oTL.fX <= pRegionB->oExtents.oTL.fX ) && ( pRegionA->oExtents.oTL.fY <= pRegionB->oExtents.oTL.fY ) && ( pRegionA->oExtents.oBR.fX >= pRegionB->oExtents.oBR.fX ) && ( pRegionA->oExtents.oBR.fY >= pRegionB->oExtents.oBR.fY ) )
	{	
		if ( pNewRegion != pRegionA ) 
			return REGION_CopyRegion( pNewRegion, pRegionA );
		return true;
	}

	// Область A содержится в области B
	if ( ( pRegionB->unNumRects == 1 ) && ( pRegionB->oExtents.oTL.fX <= pRegionA->oExtents.oTL.fX ) && ( pRegionB->oExtents.oTL.fY <= pRegionA->oExtents.oTL.fY ) && ( pRegionB->oExtents.oBR.fX >= pRegionA->oExtents.oBR.fX ) && ( pRegionB->oExtents.oBR.fY >= pRegionA->oExtents.oBR.fY ) )
	{	
		if ( pNewRegion != pRegionB ) 
			return REGION_CopyRegion( pNewRegion, pRegionB );
		return true;
	}

	REGION_RegionOp( pNewRegion, pRegionA, pRegionB, REGION_UnionO, REGION_UnionNonO, REGION_UnionNonO);

    pNewRegion->oExtents.oTL.fX = (std::min)( pRegionA->oExtents.oTL.fX, pRegionB->oExtents.oTL.fX );
    pNewRegion->oExtents.oTL.fY = (std::min)( pRegionA->oExtents.oTL.fY, pRegionB->oExtents.oTL.fY );
    pNewRegion->oExtents.oBR.fX = (std::max)( pRegionA->oExtents.oBR.fX, pRegionB->oExtents.oBR.fX );
    pNewRegion->oExtents.oBR.fY = (std::max)( pRegionA->oExtents.oBR.fY, pRegionB->oExtents.oBR.fY );

	pNewRegion->ushType = ( pNewRegion->unNumRects ? COMPLEXREGION : NULLREGION );

	return true;
}

static bool REGION_CopyRegion (TWmfRegion *pDst, TWmfRegion *pSrc)
{	
	if ( pDst != pSrc )
	{	
		if ( pDst->unSize < pSrc->unNumRects )
		{	
			pDst->pRects = (TWmfRectF *)realloc( pDst->pRects, pSrc->unNumRects * sizeof (TWmfRectF) );
			if ( NULL == pDst->pRects )
				return false;

			pDst->unSize = pSrc->unNumRects;
		}
		pDst->unNumRects = pSrc->unNumRects;

		pDst->oExtents.oTL.fX = pSrc->oExtents.oTL.fX;
		pDst->oExtents.oTL.fY = pSrc->oExtents.oTL.fY;
		pDst->oExtents.oBR.fX = pSrc->oExtents.oBR.fX;
		pDst->oExtents.oBR.fY = pSrc->oExtents.oBR.fY;

		pDst->ushType = pSrc->ushType;

		memcpy ( (char*)pDst->pRects, (char*)pSrc->pRects, (int)(pSrc->unNumRects * sizeof(TWmfRectF)));
	}

	return true;
}

static void REGION_RegionOp   (TWmfRegion *pNewRegion, TWmfRegion *pRegionA, TWmfRegion *pRegionB, pProcO pOverlapFunc, pProcNonO pNonOverlap1Func, pProcNonO pNonOverlap2Func)
{	
	TWmfRectF* r1;                    /* Pointer into first region */
	TWmfRectF* r2;                    /* Pointer into 2d region */
	TWmfRectF* r1End;                 /* End of 1st region */
	TWmfRectF* r2End;                 /* End of 2d region */
	TWmfRectF* oldRects;              /* Old rects for newReg */
	TWmfRectF* r1BandEnd;             /* End of current band in r1 */
	TWmfRectF* r2BandEnd;             /* End of current band in r2 */
	TWmfRectF* prev_rects;

	float ytop;                       /* Top of intersection */
	float ybot;                       /* Bottom of intersection */
	float top;                        /* Top of non-overlapping band */
	float bot;                        /* Bottom of non-overlapping band */

	unsigned int prevBand;            /* Index of start of previous band in newReg */
	unsigned int curBand;             /* Index of start of current band in newReg */

/*Initialization:
 * 
 * set r1, r2, r1End and r2End appropriately, preserve the important
 * parts of the destination region until the end in case it's one of
 * the two source regions, then mark the "new" region empty, allocating
 * another array of rectangles for it to use.
 */
	r1 = pRegionA->pRects;
	r2 = pRegionB->pRects;

	r1End = r1 + pRegionA->unNumRects;
	r2End = r2 + pRegionB->unNumRects;

/* newReg may be one of the src regions so we can't empty it. We keep a 
 * note of its rects pointer (so that we can free them later), preserve its
 * extents and simply set numRects to zero. 
 */
	oldRects = pNewRegion->pRects;
	pNewRegion->unNumRects = 0;

/* Allocate a reasonable number of rectangles for the new region. The idea
 * is to allocate enough so the individual functions don't need to
 * reallocate and copy the array, which is time consuming, yet we don't
 * have to worry about using too much memory. I hope to be able to
 * nuke the Xrealloc() at the end of this function eventually.
 */
    pNewRegion->unSize = (std::max)( pRegionA->unNumRects, pRegionB->unNumRects ) * 2;

	if ( NULL == ( pNewRegion->pRects = (TWmfRectF *)malloc( sizeof(TWmfRectF) * pNewRegion->unSize ) ) )
	{
		pNewRegion->unSize = 0;
		return;
	}

/* Initialize ybot and ytop.
 *
 * In the upcoming loop, ybot and ytop serve different functions depending
 * on whether the band being handled is an overlapping or non-overlapping
 * band.
 *
 * In the case of a non-overlapping band (only one of the regions
 * has points in the band), ybot is the bottom of the most recent
 * intersection and thus clips the top of the rectangles in that band.
 * ytop is the top of the next intersection between the two regions and
 * serves to clip the bottom of the rectangles in the current band.
 *
 * For an overlapping band (where the two regions intersect), ytop clips
 * the top of the rectangles of both regions and ybot clips the bottoms.
 */
	if ( pRegionA->oExtents.oTL.fY < pRegionB->oExtents.oTL.fY )
	{	
		ybot = pRegionA->oExtents.oTL.fY;
	}
	else
	{	
		ybot = pRegionB->oExtents.oTL.fY;
	}
	 
/* prevBand serves to mark the start of the previous band so rectangles
 * can be coalesced into larger rectangles. qv. miCoalesce, above.
 * In the beginning, there is no previous band, so prevBand == curBand
 * (curBand is set later on, of course, but the first band will always
 * start at index 0). prevBand and curBand must be indices because of
 * the possible expansion, and resultant moving, of the new region's
 * array of rectangles.
 */
	prevBand = 0;
	 
	do
	{	
		curBand = pNewRegion->unNumRects;

/* This algorithm proceeds one source-band (as opposed to a
 * destination band, which is determined by where the two regions
 * intersect) at a time. r1BandEnd and r2BandEnd serve to mark the
 * rectangle after the last one in the current band for their
 * respective regions.
 */
		r1BandEnd = r1;
		while ( ( r1BandEnd != r1End ) && ( r1BandEnd->oTL.fY == r1->oTL.fY ) ) 
			r1BandEnd++;

		r2BandEnd = r2;
		while ( ( r2BandEnd != r2End ) && ( r2BandEnd->oTL.fY == r2->oTL.fY ) ) 
			r2BandEnd++;

/* First handle the band that doesn't intersect, if any.
 *
 * Note that attention is restricted to one band in the
 * non-intersecting region at once, so if a region has n
 * bands between the current position and the next place it overlaps
 * the other, this entire loop will be passed through n times.
 */
		if ( r1->oTL.fY < r2->oTL.fY )
		{	
            top = (std::max)( r1->oTL.fY, ybot );
            bot = (std::min)( r1->oBR.fY, r2->oTL.fY );

			if ( ( top != bot ) && ( pNonOverlap1Func ) )
			{	
				(*pNonOverlap1Func)( pNewRegion, r1, r1BandEnd, top, bot);
			}

			ytop = r2->oTL.fY;
		}
		else if ( r2->oTL.fY < r1->oTL.fY )
		{	
            top = (std::max)( r2->oTL.fY, ybot );
            bot = (std::min)( r2->oBR.fY, r1->oTL.fY );

			if ( ( top != bot ) && ( pNonOverlap2Func ) )
			{	
				(*pNonOverlap2Func)( pNewRegion, r2, r2BandEnd, top, bot);
			}

			ytop = r1->oTL.fY;
		}
		else
		{	
			ytop = r1->oTL.fY;
		}

/* If any rectangles got added to the region, try and coalesce them
 * with rectangles from the previous band. Note we could just do
 * this test in miCoalesce, but some machines incur a not
 * inconsiderable cost for function calls, so...
 */
		if ( pNewRegion->unNumRects != curBand )
		{	
			prevBand = REGION_Coalesce( pNewRegion, prevBand, curBand );
		}

/* Now see if we've hit an intersecting band. The two bands only
 * intersect if ybot > ytop
 */
        ybot = (std::min)( r1->oBR.fY, r2->oBR.fY );
		curBand = pNewRegion->unNumRects;
		if ( ( ybot > ytop ) && ( pOverlapFunc ) )
		{	
			(*pOverlapFunc)( pNewRegion, r1, r1BandEnd, r2, r2BandEnd, ytop, ybot );
		}

		if ( pNewRegion->unNumRects != curBand )
		{	
			prevBand = REGION_Coalesce( pNewRegion, prevBand, curBand );
		}

/* If we've finished with a band (bottom == ybot) we skip forward
 * in the region to the next band.
 */
		if ( r1->oBR.fY == ybot ) 
			r1 = r1BandEnd;
		if ( r2->oBR.fY == ybot ) 
			r2 = r2BandEnd;

	} while ((r1 != r1End) && (r2 != r2End));

/* Deal with whichever region still has rectangles left.
 */
	curBand = pNewRegion->unNumRects;
	if ( r1 != r1End )
	{	
		if ( pNonOverlap1Func )
		{	
			do
			{	
				r1BandEnd = r1;
				while ( ( r1BandEnd < r1End ) && ( r1BandEnd->oTL.fY == r1->oTL.fY ) ) 
					r1BandEnd++;

                (*pNonOverlap1Func)( pNewRegion, r1, r1BandEnd, (std::max)( r1->oTL.fY, ybot ), r1->oBR.fY );

				r1 = r1BandEnd;
			} while ( r1 != r1End );
		}
	}
	else if ( ( r2 != r2End ) && ( pNonOverlap2Func ) )
	{	
		do
		{	
			r2BandEnd = r2;
			while ( ( r2BandEnd < r2End ) && ( r2BandEnd->oTL.fY == r2->oTL.fY ) ) 
				r2BandEnd++;

            (*pNonOverlap2Func)( pNewRegion, r2, r2BandEnd, (std::max)( r2->oTL.fY, ybot ), r2->oBR.fY );

			r2 = r2BandEnd;
		} while ( r2 != r2End );
	}

	if ( pNewRegion->unNumRects != curBand )
	{	
		REGION_Coalesce( pNewRegion, prevBand, curBand );
	}

/* A bit of cleanup. To keep regions from growing without bound,
 * we shrink the array of rectangles to match the new number of
 * rectangles in the region. This never goes to 0, however...
 *
 * Only do this stuff if the number of rectangles allocated is more than
 * twice the number of rectangles in the region (a simple optimization...).
 */
	if ( pNewRegion->unNumRects  < ( pNewRegion->unSize >> 1 ) )
	{	
		if ( pNewRegion->unNumRects ) /* REGION_NOT_EMPTY */
		{	
			prev_rects         = pNewRegion->pRects;
			pNewRegion->unSize = pNewRegion->unNumRects;
			pNewRegion->pRects = (TWmfRectF *)realloc( pNewRegion->pRects, sizeof(TWmfRectF) * pNewRegion->unSize );
			if ( NULL == pNewRegion->pRects ) 
				pNewRegion->pRects = prev_rects;
		}
		else
		{	/* No point in doing the extra work involved in an Xrealloc if
			 * the region is empty
			 */
			pNewRegion->unSize = 1;
			free( pNewRegion->pRects );
			pNewRegion->pRects = (TWmfRectF *)malloc( sizeof(TWmfRectF) );
		}
	}

	free( oldRects );
}

static unsigned int REGION_Coalesce (TWmfRegion *pRegion, unsigned int unPrevStart, unsigned int unCurStart)
{	
	TWmfRectF *pPrevRect;      /* Current rect in previous band */
	TWmfRectF *pCurRect;       /* Current rect in current band */
	TWmfRectF *pRegEnd;        /* End of region */

	unsigned int curNumRects;  /* Number of rectangles in current band */
	unsigned int prevNumRects; /* Number of rectangles in previous band */

	float bandtop;             /* top coordinate for current band */

	pRegEnd      = pRegion->pRects + pRegion->unNumRects;
	pPrevRect    = pRegion->pRects + unPrevStart;
	prevNumRects = unCurStart - unPrevStart;

/* Figure out how many rectangles are in the current band. Have to do
 * this because multiple bands could have been added in REGION_RegionOp
 * at the end when one region has been exhausted.
 */
	pCurRect = pRegion->pRects + unCurStart;
	bandtop = pCurRect->oTL.fY;

	for (curNumRects = 0; (pCurRect != pRegEnd) && (pCurRect->oTL.fY == bandtop); curNumRects++)
	{	
		pCurRect++;
	}
	 
	if (pCurRect != pRegEnd)
	{	/* If more than one band was added, we have to find the start
		 * of the last band added so the next coalescing job can start
		 * at the right place... (given when multiple bands are added,
		 * this may be pointless -- see above).
		 */
		pRegEnd--;
		while ( pRegEnd[-1].oTL.fY == pRegEnd->oTL.fY )
			pRegEnd--;

		unCurStart = (unsigned int) ( pRegEnd - pRegion->pRects );
		pRegEnd = pRegion->pRects + pRegion->unNumRects;
	}
	
	if ((curNumRects == prevNumRects) && (curNumRects != 0))
	{	pCurRect -= curNumRects;

		/* The bands may only be coalesced if the bottom of the previous
		 * matches the top scanline of the current.
		 */
		if ( pPrevRect->oBR.fY == pCurRect->oTL.fY )
		{	/* Make sure the bands have rects in the same places. This
			 * assumes that rects have been added in such a way that they
			 * cover the most area possible. I.e. two rects in a band must
			 * have some horizontal space between them.
			 */
			do
			{	if ( (pPrevRect->oTL.fX != pCurRect->oTL.fX )
				|| (pPrevRect->oBR.fX != pCurRect->oBR.fX))
				{	/* The bands don't line up so they can't be coalesced.
					 */
					return (unCurStart);
				}
				pPrevRect++;
				pCurRect++;
				prevNumRects -= 1;
			} while (prevNumRects != 0);

			pRegion->unNumRects -= curNumRects;
			pCurRect -= curNumRects;
			pPrevRect -= curNumRects;

			/* The bands may be merged, so set the bottom of each rect
			 * in the previous band to that of the corresponding rect in
			 * the current band.
			 */
			do
			{	pPrevRect->oBR.fY = pCurRect->oBR.fY;
				pPrevRect++;
				pCurRect++;
				curNumRects -= 1;
			} while (curNumRects != 0);

			/* If only one band was added to the region, we have to backup
			 * curStart to the start of the previous band.
			 *
			 * If more than one band was added to the region, copy the
			 * other bands down. The assumption here is that the other bands
			 * came from the same region as the current one and no further
			 * coalescing can be done on them since it's all been done
			 * already... curStart is already in the right place.
			 */
			if (pCurRect == pRegEnd)
			{	unCurStart = unPrevStart;
			}
			else
			{	do
				{	*pPrevRect++ = *pCurRect++;
				} while (pCurRect != pRegEnd);
			}
		}
	}

	return (unCurStart);
}

static bool REGION_UnionO (TWmfRegion *pRegion, TWmfRectF *pRect1, TWmfRectF *pRect1End, TWmfRectF *pRect2, TWmfRectF *pRect2End, float fTop, float fBottom)
{	
	while ( ( pRect1 != pRect1End ) && ( pRect2 != pRect2End ) )
	{	
		if ( pRect1->oTL.fX < pRect2->oTL.fX )
		{	
			if ( !RectMerge( pRegion, pRect1, fTop, fBottom ) )
				return false;

			pRect1++;
		}
		else
		{	
			if ( !RectMerge( pRegion, pRect2, fTop, fBottom ) )
				return false;

			pRect2++;
		}
	}

	if ( pRect1 != pRect1End )
	{	
		do
		{	
			if ( !RectMerge( pRegion, pRect1, fTop, fBottom ) )
				return false;

			pRect1++;
		} while ( pRect1 != pRect1End );
	}
	else
	{	
		while ( pRect2 != pRect2End )
		{	
			if ( !RectMerge( pRegion, pRect2, fTop, fBottom ) )
				return false;
			pRect2++;
		}
	}

	return true;
}

static bool REGION_UnionNonO(TWmfRegion *pRegion, TWmfRectF *pRect, TWmfRectF *pRectEnd, float fTop, float fBottom)
{
	TWmfRectF* pNextRect = 0;

	while ( pRect != pRectEnd )
	{	
		if ( NULL == ( pNextRect = RegionMemChk( pRegion ) ) )
			return false;

		pRegion->unNumRects++;

		pNextRect->oTL.fX = pRect->oTL.fX;
		pNextRect->oTL.fY = fTop;
		pNextRect->oBR.fX = pRect->oBR.fX;
		pNextRect->oBR.fY = fBottom;

		pNextRect++;
		pRect++;
	}

	return true;
}

static bool REGION_SubtractNonO1 (TWmfRegion *pRegion, TWmfRectF *pRect, TWmfRectF *pRectEnd, float fTop, float fBottom)
{
	TWmfRectF *pNextRect = 0;

	while ( pRect != pRectEnd )
	{	
		if ( NULL == ( pNextRect = RegionMemChk( pRegion ) ) ) 
			return false;

		pRegion->unNumRects++;

		pNextRect->oTL.fX = pRect->oTL.fX;
		pNextRect->oTL.fY = fTop;
		pNextRect->oBR.fX = pRect->oBR.fX;
		pNextRect->oBR.fY = fBottom;

		pNextRect++;
		pRect++;
	}
	return true;
}

static void REGION_SubtractRegion(TWmfRegion *pNewRegion, TWmfRegion *pRegionA, TWmfRegion *pRegionB)
{
	if ( ( pRegionA->unNumRects == 0 ) || ( pRegionB->unNumRects == 0 ) || ( !EXTENTCHECK ( &pRegionA->oExtents, &pRegionB->oExtents ) ) )
	{
		REGION_CopyRegion ( pNewRegion, pRegionA );
		return;
	}

	REGION_RegionOp( pNewRegion, pRegionA, pRegionB, REGION_SubtractO, REGION_SubtractNonO1, NULL );

/* Can't alter newReg's extents before we call miRegionOp because
 * it might be one of the source regions and miRegionOp depends
 * on the extents of those regions being the unaltered. Besides, this
 * way there's no checking against rectangles that will be nuked
 * due to coalescing, so we have to examine fewer rectangles.
 */
	REGION_SetExtents( pNewRegion );
	pNewRegion->ushType = ( pNewRegion->unNumRects ? COMPLEXREGION : NULLREGION );
}

static bool REGION_SubtractO (TWmfRegion *pRegion, TWmfRectF *pRect1, TWmfRectF *pRect1End, TWmfRectF *pRect2, TWmfRectF *pRect2End, float fTop, float fBottom)
{	
	TWmfRectF* pNextRect = 0;

	float fLeft = pRect1->oTL.fX;

	if ( NULL == ( pNextRect = RegionMemChk( pRegion ) ) ) 
		return false;

	while ( ( pRect1 != pRect1End ) && ( pRect2 != pRect2End ) )
	{	
		if ( pRect2->oBR.fX <= fLeft )
		{	
			pRect2++;
		}
		else if ( pRect2->oTL.fX <= fLeft )
		{
			fLeft = pRect2->oBR.fX;
			if ( fLeft >= pRect1->oBR.fX )
			{
				pRect1++;
				if ( pRect1 != pRect1End ) 
					fLeft = pRect1->oTL.fX;
			}
			else
			{
				pRect2++;
			}
		}
		else if ( pRect2->oTL.fX < pRect1->oBR.fX )
		{
			if ( NULL == ( pNextRect = RegionMemChk( pRegion ) ) ) 
				return false;

			pRegion->unNumRects++;

			pNextRect->oTL.fX = fLeft;
			pNextRect->oTL.fY = fTop;
			pNextRect->oBR.fX = pRect2->oTL.fX;
			pNextRect->oBR.fY = fBottom;

			pNextRect++;

			fLeft = pRect2->oBR.fX;

			if ( fLeft >= pRect1->oBR.fX )
			{
				pRect1++;
				if ( pRect1 != pRect1End )
					fLeft = pRect1->oTL.fX;
			}
			else
			{
				pRect2++;
			}
		}
		else
		{
			if ( pRect1->oBR.fX > fLeft )
			{	
				if ( NULL == ( pNextRect = RegionMemChk( pRegion ) ) ) 
					return false;

				pRegion->unNumRects++;

				pNextRect->oTL.fX = fLeft;
				pNextRect->oTL.fY = fTop;
				pNextRect->oBR.fX = pRect1->oBR.fX;
				pNextRect->oBR.fY = fBottom;

				pNextRect++;
			}
			pRect1++;
			fLeft = pRect1->oTL.fX;
		}
	}

	while ( pRect1 != pRect1End )
	{	
		if ( NULL == ( pNextRect = RegionMemChk( pRegion ) ) ) 
			return false;

		pRegion->unNumRects++;

		pNextRect->oTL.fX = fLeft;
		pNextRect->oTL.fY = fTop;
		pNextRect->oBR.fX = pRect1->oBR.fX;
		pNextRect->oBR.fY = fBottom;

		pNextRect++;
		pRect1++;

		if ( pRect1 != pRect1End ) 
			fLeft = pRect1->oTL.fX;
	}

	return true;
}

static void REGION_SetExtents (TWmfRegion *pRegion)
{	
	if ( 0 == pRegion->unNumRects )
	{	
		pRegion->oExtents.oTL.fX = 0;
		pRegion->oExtents.oTL.fY = 0;
		pRegion->oExtents.oBR.fX = 0;
		pRegion->oExtents.oBR.fY = 0;

		return;
	}

	TWmfRectF *pExtents = &pRegion->oExtents;
	TWmfRectF *pRect    =  pRegion->pRects;
	TWmfRectF *pRectEnd = pRect + pRegion->unNumRects - 1;

	// Поскольку pRect - первый прямоугольник в данном pRegion, значит у него 
	// координата верха наименьшая, аналогично, у последнего pRectEnd координата
	// низа наибольшая.

	pExtents->oTL.fX = pRect->oTL.fX;
	pExtents->oTL.fY = pRect->oTL.fY;
	pExtents->oBR.fX = pRectEnd->oBR.fX;
	pExtents->oBR.fY = pRectEnd->oBR.fY;

	while ( pRect <= pRectEnd )
	{	
		if ( pRect->oTL.fX < pExtents->oTL.fX ) 
			pExtents->oTL.fX = pRect->oTL.fX;
		if ( pRect->oBR.fX > pExtents->oBR.fX ) 
			pExtents->oBR.fX = pRect->oBR.fX;

		pRect++;
	}
}

static void REGION_IntersectRegion (TWmfRegion *pNewRegion, TWmfRegion *pRegionA, TWmfRegion *pRegionB)
{
	if ( ( !( pRegionA->unNumRects ) ) || ( !( pRegionB->unNumRects ) )  || ( !EXTENTCHECK( &pRegionA->oExtents, &pRegionB->oExtents ) ) )
	{
		pNewRegion->unNumRects = 0;
	}
	else
	{	
		REGION_RegionOp( pNewRegion, pRegionA, pRegionB, REGION_IntersectO, 0, 0 );
	}

/* Can't alter newReg's extents before we call miRegionOp because
 * it might be one of the source regions and miRegionOp depends
 * on the extents of those regions being the same. Besides, this
 * way there's no checking against rectangles that will be nuked
 * due to coalescing, so we have to examine fewer rectangles.
 */
	REGION_SetExtents( pNewRegion );

	pNewRegion->ushType = ( pNewRegion->unNumRects ? COMPLEXREGION : NULLREGION );
}

static bool REGION_IntersectO (TWmfRegion *pRegion, TWmfRectF *pRect1, TWmfRectF *pRect1End, TWmfRectF *pRect2, TWmfRectF *pRect2End, float fTop, float fBottom)
{	
	TWmfRectF *pNextRect;

	while ( ( pRect1 != pRect1End ) && ( pRect2 != pRect2End ) )
	{	
        float fLeft  = (std::max)( pRect1->oTL.fX, pRect2->oTL.fX );
        float fRight = (std::min)( pRect1->oBR.fX, pRect2->oBR.fX );

/* If there's any overlap between the two rectangles, add that
 * overlap to the new region.
 * There's no need to check for subsumption because the only way
 * such a need could arise is if some region has two rectangles
 * right next to each other. Since that should never happen...
 */
		if ( fLeft < fRight )
		{	
			if ( NULL == ( pNextRect = RegionMemChk( pRegion ) ) ) 
				return false;

			pRegion->unNumRects++;

			pNextRect->oTL.fX = fLeft;
			pNextRect->oTL.fY = fTop;
			pNextRect->oBR.fX = fRight;
			pNextRect->oBR.fY = fBottom;

			pNextRect++;
		}

/* Need to advance the pointers. Shift the one that extends
 * to the right the least, since the other still has a chance to
 * overlap with that region's next rectangle, if you see what I mean.
 */
		if ( pRect1->oBR.fX < pRect2->oBR.fX )
		{	
			pRect1++;
		}
		else if ( pRect2->oBR.fX < pRect1->oBR.fX )
		{	
			pRect2++;
		}
		else
		{	
			pRect1++;
			pRect2++;
		}
	}

	return true;
}


static bool Clipping (TWmfRegion *pClip, TWmfRegion *pVis, TWmfRectF *pRect, unsigned short ushFlags)
{	
	TWmfRegion oRegion;
	oRegion.pRects = 0;

	if ( pClip->unNumRects == 0 )
	{	
		if ( pClip->pRects )
		{	
			oRegion.pRects = pClip->pRects;
			oRegion.unSize = pClip->unSize;

			pClip->pRects = 0;
			pClip->unSize = 0;
		}
	}

	if ( oRegion.pRects == 0 )
	{	
		oRegion.pRects = (TWmfRectF*)malloc( 8 * sizeof (TWmfRectF) );
		oRegion.unSize = 8;

		if ( !oRegion.pRects )
			return false;
	}

	WmfSetRectRegion( &oRegion, pRect );

	if ( ( pClip->unNumRects == 0 ) && ( ushFlags & CLIP_INTERSECT ) )
	{	
		(*pClip) = oRegion;
		return true;
	}
/* else if ( ushFlags & CLIP_EXCLUDE) ...
 */
	if ( pClip->unNumRects == 0 )
	{	
		if ( pClip->pRects == 0 )
		{	
			pClip->pRects = (TWmfRectF*)malloc( 8 * sizeof (TWmfRectF) );
			pClip->unSize = 8;

			if ( !pClip->pRects )
				return false;
		}

		WmfSetRectRegion( pClip, NULL );
		WmfCombineRegion( pClip, pVis, 0, RGN_COPY );
	}

	WmfCombineRegion( &oRegion, pClip, &oRegion, (unsigned short)( ( ushFlags & CLIP_EXCLUDE ) ? RGN_DIFF : RGN_AND ) );

	(*pClip) = oRegion; /* What about what *was* in clip ?? Check WmfCombineRgn */

	return true;
}


#endif /* _WMF_REGION_H */
