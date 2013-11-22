#include "AvsOutputDevice.h"
#include "tools/bigint.hxx"
#include "vcl/salbtype.hxx"
#include "vcl/alpha.hxx"
#include "implncvt.hxx"
#include "vcl/wall2.hxx"
#include "vcl/bmpacc.hxx"
#include "vcl/impbmp.hxx"
#include "vcl/salgtype.hxx"
#include "vcl/salgdi.hxx"

#include <math.h>
#include "outmap.cxx"



using namespace SVMCore;

namespace SVMCore{

#define TEXT_DRAW_ELLIPSIS  (TEXT_DRAW_ENDELLIPSIS | TEXT_DRAW_PATHELLIPSIS | TEXT_DRAW_NEWSELLIPSIS)

// =======================================================================

#define UNDERLINE_LAST      UNDERLINE_BOLDWAVE
#define STRIKEOUT_LAST      STRIKEOUT_X


#define OUTDEV_POLYPOLY_STACKBUF		32

// =======================================================================

static void ImplRotatePos( long nOriginX, long nOriginY, long& rX, long& rY,
                           int nOrientation )
{
    if ( (nOrientation >= 0) && !(nOrientation % 900) )
    {
        if ( (nOrientation >= 3600) )
            nOrientation %= 3600;

        if ( nOrientation )
        {
            rX -= nOriginX;
            rY -= nOriginY;

            if ( nOrientation == 900 )
            {
                long nTemp = rX;
                rX = rY;
                rY = -nTemp;
            }
            else if ( nOrientation == 1800 )
            {
                rX = -rX;
                rY = -rY;
            }
            else /* ( nOrientation == 2700 ) */
            {
                long nTemp = rX;
                rX = -rY;
                rY = nTemp;
            }

            rX += nOriginX;
            rY += nOriginY;
        }
    }
    else
    {
        double nRealOrientation = nOrientation*F_PI1800;
        double nCos = cos( nRealOrientation );
        double nSin = sin( nRealOrientation );

        // Translation...
        long nX = rX-nOriginX;
        long nY = rY-nOriginY;

        // Rotation...
        rX = +((long)(nCos*nX + nSin*nY)) + nOriginX;
        rY = -((long)(nSin*nX - nCos*nY)) + nOriginY;
    }
}

// =======================================================================

struct ImplObjStack
{
    ImplObjStack*   mpPrev;
    MapMode*        mpMapMode;
    Region*         mpClipRegion;
    Color*          mpLineColor;
    Color*          mpFillColor;
    Font*           mpFont;
    Color*          mpTextColor;
    Color*          mpTextFillColor;
    Color*          mpTextLineColor;
    Color*          mpOverlineColor;
    Point*          mpRefPoint;
    TextAlign       meTextAlign;
    RasterOp        meRasterOp;
    ULONG           mnTextLayoutMode;
    LanguageType    meTextLanguage;
    USHORT          mnFlags;
	ImplObjStack()
	{
		mpPrev = NULL;
		mpMapMode = NULL;
		mpClipRegion = NULL;
		mpLineColor = NULL;
		mpFillColor = NULL;
		mpFont = NULL;
		mpTextColor = NULL;
		mpTextFillColor = NULL;
		mpTextLineColor = NULL;
		mpOverlineColor = NULL;
		mpRefPoint = NULL;
	}
};

// -----------------------------------------------------------------------

static void ImplDeleteObjStack( ImplObjStack* pObjStack )
{
	if ( pObjStack->mnFlags & PUSH_LINECOLOR )
	{
		if ( pObjStack->mpLineColor )
			delete pObjStack->mpLineColor;
	}
	if ( pObjStack->mnFlags & PUSH_FILLCOLOR )
	{
		if ( pObjStack->mpFillColor )
			delete pObjStack->mpFillColor;
	}
	if ( pObjStack->mnFlags & PUSH_FONT )
		delete pObjStack->mpFont;
	if ( pObjStack->mnFlags & PUSH_TEXTCOLOR )
		delete pObjStack->mpTextColor;
	if ( pObjStack->mnFlags & PUSH_TEXTFILLCOLOR )
	{
		if ( pObjStack->mpTextFillColor )
			delete pObjStack->mpTextFillColor;
	}
	if ( pObjStack->mnFlags & PUSH_TEXTLINECOLOR )
	{
		if ( pObjStack->mpTextLineColor )
			delete pObjStack->mpTextLineColor;
	}
	if ( pObjStack->mnFlags & PUSH_OVERLINECOLOR )
	{
		if ( pObjStack->mpOverlineColor )
			delete pObjStack->mpOverlineColor;
	}
	if ( pObjStack->mnFlags & PUSH_MAPMODE )
	{
		if ( pObjStack->mpMapMode )
			delete pObjStack->mpMapMode;
	}
	if ( pObjStack->mnFlags & PUSH_CLIPREGION )
	{
		if ( pObjStack->mpClipRegion )
			delete pObjStack->mpClipRegion;
	}
	if ( pObjStack->mnFlags & PUSH_REFPOINT )
	{
		if ( pObjStack->mpRefPoint )
			delete pObjStack->mpRefPoint;
	}

	delete pObjStack;
}

inline UINT8 ImplGetGradientColorValue( long nValue )
{
	if ( nValue < 0 )
		return 0;
	else if ( nValue > 0xFF )
		return 0xFF;
	else
		return (UINT8)nValue;
}

// ----------------
// - Cmp-Function -
// ----------------

int ImplHatchCmpFnc( const void* p1, const void* p2 )
{
	const long nX1 = ( (Point*) p1 )->X();
	const long nX2 = ( (Point*) p2 )->X();
	const long nY1 = ( (Point*) p1 )->Y();
	const long nY2 = ( (Point*) p2 )->Y();

	return ( nX1 > nX2 ? 1 : nX1 == nX2 ? nY1 > nY2 ? 1: nY1 == nY2 ? 0 : -1 : -1 );
}

// =======================================================================
ULONG ImplAdjustTwoRect( SalTwoRect& rTwoRect, const Size& rSizePix )
{
	ULONG nMirrFlags = 0;

	if ( rTwoRect.mnDestWidth < 0 )
	{
		rTwoRect.mnSrcX = rSizePix.Width() - rTwoRect.mnSrcX - rTwoRect.mnSrcWidth;
		rTwoRect.mnDestWidth = -rTwoRect.mnDestWidth;
		rTwoRect.mnDestX -= rTwoRect.mnDestWidth-1;
		nMirrFlags |= BMP_MIRROR_HORZ;
	}

	if ( rTwoRect.mnDestHeight < 0 )
	{
		rTwoRect.mnSrcY = rSizePix.Height() - rTwoRect.mnSrcY - rTwoRect.mnSrcHeight;
		rTwoRect.mnDestHeight = -rTwoRect.mnDestHeight;
		rTwoRect.mnDestY -= rTwoRect.mnDestHeight-1;
		nMirrFlags |= BMP_MIRROR_VERT;
	}

	if( ( rTwoRect.mnSrcX < 0 ) || ( rTwoRect.mnSrcX >= rSizePix.Width() ) ||
		( rTwoRect.mnSrcY < 0 ) || ( rTwoRect.mnSrcY >= rSizePix.Height() ) ||	
		( ( rTwoRect.mnSrcX + rTwoRect.mnSrcWidth ) > rSizePix.Width() ) ||
		( ( rTwoRect.mnSrcY + rTwoRect.mnSrcHeight ) > rSizePix.Height() ) )
	{
		const Rectangle	aSourceRect( Point( rTwoRect.mnSrcX, rTwoRect.mnSrcY ), 
									 Size( rTwoRect.mnSrcWidth, rTwoRect.mnSrcHeight ) );
		Rectangle		aCropRect( aSourceRect );

		aCropRect.Intersection( Rectangle( Point(), rSizePix ) );

		if( aCropRect.IsEmpty() )
			rTwoRect.mnSrcWidth = rTwoRect.mnSrcHeight = rTwoRect.mnDestWidth = rTwoRect.mnDestHeight = 0;
		else
		{
			const double	fFactorX = ( rTwoRect.mnSrcWidth > 1 ) ? (double) ( rTwoRect.mnDestWidth - 1 ) / ( rTwoRect.mnSrcWidth - 1 ) : 0.0;
			const double	fFactorY = ( rTwoRect.mnSrcHeight > 1 ) ? (double) ( rTwoRect.mnDestHeight - 1 ) / ( rTwoRect.mnSrcHeight - 1 ) : 0.0;

			const long nDstX1 = rTwoRect.mnDestX + FRound( fFactorX * ( aCropRect.Left() - rTwoRect.mnSrcX ) );
			const long nDstY1 = rTwoRect.mnDestY + FRound( fFactorY * ( aCropRect.Top() - rTwoRect.mnSrcY ) );
			const long nDstX2 = rTwoRect.mnDestX + FRound( fFactorX * ( aCropRect.Right() - rTwoRect.mnSrcX ) );
			const long nDstY2 = rTwoRect.mnDestY + FRound( fFactorY * ( aCropRect.Bottom() - rTwoRect.mnSrcY ) );

			rTwoRect.mnSrcX = aCropRect.Left();
			rTwoRect.mnSrcY = aCropRect.Top();
			rTwoRect.mnSrcWidth = aCropRect.GetWidth();
			rTwoRect.mnSrcHeight = aCropRect.GetHeight();
			rTwoRect.mnDestX = nDstX1;
			rTwoRect.mnDestY = nDstY1;
			rTwoRect.mnDestWidth = nDstX2 - nDstX1 + 1;
			rTwoRect.mnDestHeight = nDstY2 - nDstY1 + 1;
		}
	}

	return nMirrFlags;
}


// -----------------------------------------------------------------------

#define OUTDEV_INIT()						\
{											\
	if ( !mpGraphics )						\
		return; 							\
											\
	if ( mbInitClipRegion ) 				\
		ImplInitClipRegion();				\
											\
	if ( mbOutputClipped )					\
		return; 							\
}
// -----------
// - Defines -
// -----------

#define HATCH_MAXPOINTS				1024
#define GRADIENT_DEFAULT_STEPCOUNT	0


// -------------
// - externals -
// -------------

extern ULONG nVCLRLut[ 6 ];
extern ULONG nVCLGLut[ 6 ];
extern ULONG nVCLBLut[ 6 ];
extern ULONG nVCLDitherLut[ 256 ];
extern ULONG nVCLLut[ 256 ];

}//SVMCore

//int OutputDevice::ImplGetGraphics()
//{
//	if ( mpGraphics )
//		return TRUE;
//
//	mbInitLineColor 	= TRUE;
//	mbInitFillColor 	= TRUE;
//	mbInitFont			= TRUE;
//	mbInitTextColor 	= TRUE;
//	mbInitClipRegion	= TRUE;
//
//	if ( meOutDevType == OUTDEV_VIRDEV )
//		;//mpGraphics = new WinSalGraphics();
//
//	if ( mpGraphics )
//	{
//		mpGraphics->SetXORMode( (ROP_INVERT == meRasterOp) || (ROP_XOR == meRasterOp), ROP_INVERT == meRasterOp );
//        //mpGraphics->setAntiAliasB2DDraw(mnAntialiasing & ANTIALIASING_ENABLE_B2DDRAW);
//        return TRUE;
//	}
//	return FALSE;
//}

void VirtualDevice::ImplInitVirDev( long nDX, long nDY, USHORT nBitCount )
{
	if ( nDX < 1 )
		nDX = 1;

	if ( nDY < 1 )
		nDY = 1;

	mnBitCount		= ( nBitCount ? nBitCount : GetBitCount() );
	mnOutWidth		= nDX;
	mnOutHeight 	= nDY;
	//mbScreenComp	= TRUE;
    //mnAlphaDepth	= -1;

 	meOutDevType	= OUTDEV_VIRDEV;
	//mbDevOutput 	= TRUE;
	mpFontList		= new ImplDevFontList();
	mpFontCache 	= new ImplFontCache( false );
	mpGraphics = GetWinGraphics();
	//mnDPIX			= pOutDev->mnDPIX;
	//mnDPIY			= pOutDev->mnDPIY;
	//maFont			= pOutDev->maFont;

	// Virtuelle Devices haben defaultmaessig einen weissen Hintergrund
	SetBackground( Wallpaper( Color( COL_WHITE ) ) );
}

OutputDevice::OutputDevice() :
	maRegion( REGION_NULL ),
	maFillColor( COL_WHITE ),
	maTextLineColor( COL_TRANSPARENT )//,
	//maSettings( Application::GetSettings() )
{
	mpGraphics			= NULL;
	//mpUnoGraphicsList	= NULL;
	//mpPrevGraphics		= NULL;
	//mpNextGraphics		= NULL;
	//mpMetaFile			= NULL;
	mpFontEntry 		= NULL;
	mpFontCache 		= NULL;
	mpFontList			= NULL;
	//mpGetDevFontList	= NULL;
	//mpGetDevSizeList	= NULL;
	mpObjStack			= NULL;
	//mpOutDevData		= NULL;
    //mpPDFWriter			= NULL;
    //mpAlphaVDev			= NULL;
	//mpExtOutDevData		= NULL;
	mnOutOffX			= 0;
	mnOutOffY			= 0;
	mnOutWidth			= 0;
	mnOutHeight 		= 0;
	mnDPIX				= 96;//todo
	mnDPIY				= 96;//todo
	mnTextOffX			= 0;
	mnTextOffY			= 0;
	mnOutOffOrigX		= 0;
	mnOutOffLogicX		= 0;
	mnOutOffOrigY		= 0;
	mnOutOffLogicY		= 0;
	mnEmphasisAscent	= 0;
	mnEmphasisDescent	= 0;
	//mnDrawMode			= 0;
	mnTextLayoutMode        = TEXT_LAYOUT_DEFAULT;
#ifdef AVS
	if( Application::GetSettings().GetLayoutRTL() ) //#i84553# tip BiDi preference to RTL
		mnTextLayoutMode = TEXT_LAYOUT_BIDI_RTL | TEXT_LAYOUT_TEXTORIGIN_LEFT;
#endif
	meOutDevType		= OUTDEV_VIRDEV;
    //meOutDevViewType    = OUTDEV_VIEWTYPE_DONTKNOW;
	mbMap				= FALSE;
	//mbMapIsDefault		= TRUE;
	mbClipRegion		= FALSE;
	mbBackground		= FALSE;
	//mbOutput			= TRUE;
	//mbDevOutput 		= FALSE;
	mbOutputClipped 	= FALSE;
	maTextColor 		= Color( COL_BLACK );
	maOverlineColor 	= Color( COL_TRANSPARENT );
	//meTextAlign 		= maFont.GetAlign();
	meRasterOp			= ROP_OVERPAINT;
	mnAntialiasing		= 0;
    meTextLanguage      = 0;  // TODO: get default from configuration?
	mbLineColor 		= TRUE;
	mbFillColor 		= TRUE;
	mbInitLineColor 	= TRUE;
	mbInitFillColor 	= TRUE;
	mbInitFont			= TRUE;
	mbInitTextColor 	= TRUE;
	mbInitClipRegion	= TRUE;
	mbClipRegionSet 	= FALSE;
	mbKerning			= FALSE;
	mbNewFont			= TRUE;
	mbTextLines 		= FALSE;
	mbTextSpecial		= FALSE;
	mbRefPoint			= FALSE;
    mbEnableRTL         = FALSE;    // mirroring must be explicitly allowed (typically for windows only)

	// struct ImplMapRes
    maMapRes.mnMapOfsX 			= 0;
    maMapRes.mnMapOfsY 			= 0;
    maMapRes.mnMapScNumX 		= 1;
    maMapRes.mnMapScNumY 		= 1;
    maMapRes.mnMapScDenomX 		= 1;
    maMapRes.mnMapScDenomY		= 1;
	// struct ImplThresholdRes
    maThresRes.mnThresLogToPixX	= 0;
    maThresRes.mnThresLogToPixY	= 0;
    maThresRes.mnThresPixToLogX	= 0;
    maThresRes.mnThresPixToLogY	= 0;

	ImplInitVirDev( 1, 1, 0 );
}

// -----------------------------------------------------------------------

OutputDevice::~OutputDevice()
{
#ifdef AVS
	if ( GetUnoGraphicsList() )
	{
		UnoWrapperBase* pWrapper = Application::GetUnoWrapper( FALSE );
		if ( pWrapper )
			pWrapper->ReleaseAllGraphics( this );
		delete mpUnoGraphicsList;
		mpUnoGraphicsList = NULL;
	}
	if ( mpOutDevData )
		ImplDeInitOutDevData();
#endif

	ImplObjStack* pData = mpObjStack;
	if ( pData )
	{
		while ( pData )
		{
			ImplObjStack* pTemp = pData;
			pData = pData->mpPrev;
			ImplDeleteObjStack( pTemp );
		}
	}
#ifdef AVS
    // release the active font instance
    if( mpFontEntry )
        mpFontCache->Release( mpFontEntry );

    // remove cached results of GetDevFontList/GetDevSizeList
    // TODO: use smart pointers for them
    if( mpGetDevFontList )
        delete mpGetDevFontList;
    if( mpGetDevSizeList )
        delete mpGetDevSizeList;

    // release ImplFontCache specific to this OutputDevice
    // TODO: refcount ImplFontCache
    if( mpFontCache
    && (mpFontCache != ImplGetSVData()->maGDIData.mpScreenFontCache)
    && (ImplGetSVData()->maGDIData.mpScreenFontCache != NULL) )
    {
        delete mpFontCache;
        mpFontCache = NULL;
    }

    // release ImplFontList specific to this OutputDevice
    // TODO: refcount ImplFontList
    if( mpFontList
    && (mpFontList != ImplGetSVData()->maGDIData.mpScreenFontList)
    && (ImplGetSVData()->maGDIData.mpScreenFontList != NULL) )
    {
        mpFontList->Clear();
        delete mpFontList;
        mpFontList = NULL;
    }
    delete mpAlphaVDev;
#endif
#ifndef AVS_INSERT
	delete mpFontList;
	delete mpFontCache;
	delete mpGraphics;
#endif
}



void OutputDevice::ImplInitClipRegion()
{
	if ( mbClipRegion )
	{
		if ( maRegion.IsEmpty() )
			mbOutputClipped = TRUE;
		else
		{
			mbOutputClipped = FALSE;
			ImplSelectClipRegion(
                                  // #102532# Respect output offset also for clip region
                                  ImplPixelToDevicePixel( maRegion ) );
		}

		mbClipRegionSet = TRUE;
	}
	else
	{
		if ( mbClipRegionSet )
		{
			mpGraphics->ResetClipRegion();
			mbClipRegionSet = FALSE;
		}

		mbOutputClipped = FALSE;
	}
	mbInitClipRegion = FALSE;
}
bool OutputDevice::ImplSelectClipRegion( const Region& rRegion, SalGraphics* pGraphics )
{
    // TODO(Q3): Change from static to plain method - everybody's
    // calling it with pOutDev=this!
    // => done, but only with minimal changes for now => TODO
	OutputDevice* const pOutDev = this;
	if( !pGraphics )
	{
		if( !mpGraphics )
				return false;
		pGraphics = mpGraphics;
	}

	long				nX;
	long				nY;
	long				nWidth;
	long				nHeight;
	ULONG				nRectCount;
	ImplRegionInfo		aInfo;
	BOOL				bRegionRect;
	BOOL				bClipRegion = TRUE;
    const BOOL          bClipDeviceBounds = TRUE;//( !pOutDev->GetPDFWriter() && pOutDev->GetOutDevType() != OUTDEV_PRINTER );

	nRectCount = rRegion.GetRectCount();
	pGraphics->BeginSetClipRegion( nRectCount );
	bRegionRect = rRegion.ImplGetFirstRect( aInfo, nX, nY, nWidth, nHeight );
    if( bClipDeviceBounds )
    {
        // #b6520266# Perform actual rect clip against outdev
        // dimensions, to generate empty clips whenever one of the
        // values is completely off the device.
        const long nOffX( pOutDev->mnOutOffX );
        const long nOffY( pOutDev->mnOutOffY );
        const long nDeviceWidth( pOutDev->GetOutputWidthPixel() );
        const long nDeviceHeight( pOutDev->GetOutputHeightPixel() );
        Rectangle aDeviceBounds( nOffX, nOffY,
                                 nOffX+nDeviceWidth-1,
                                 nOffY+nDeviceHeight-1 );
        while ( bRegionRect )
        {
            // #i59315# Limit coordinates passed to sal layer to actual
            // outdev dimensions - everything else bears the risk of
            // overflowing internal coordinates (e.g. the 16 bit wire
            // format of X11).
            Rectangle aTmpRect(nX,nY,nX+nWidth-1,nY+nHeight-1);
            aTmpRect.Intersection(aDeviceBounds);

            if( !aTmpRect.IsEmpty() )
            {
                if ( !pGraphics->UnionClipRegion( aTmpRect.Left(),
                                                  aTmpRect.Top(),
                                                  aTmpRect.GetWidth(),
                                                  aTmpRect.GetHeight(),
                                                  pOutDev ) )
                {
                    bClipRegion = FALSE;
                }
            }
            else
            {
                //#i79850# Fake off-screen clip
                if ( !pGraphics->UnionClipRegion( nDeviceWidth+1,
                                                  nDeviceHeight+1,
                                                  1, 1,
                                                  pOutDev ) )
                {
                    bClipRegion = FALSE;
                }
            }
            //DBG_ASSERT( bClipRegion, "OutputDevice::ImplSelectClipRegion() - can't create region" );
            bRegionRect = rRegion.ImplGetNextRect( aInfo, nX, nY, nWidth, nHeight );
        }
    }
    else
    {
        // #i65720# Actually, _don't_ clip anything on printer or PDF
        // export, since output might be visible outside the specified
        // device boundaries.
        while ( bRegionRect )
        {
            if ( !pGraphics->UnionClipRegion( nX, nY, nWidth, nHeight, pOutDev ) )
                bClipRegion = FALSE;
            //DBG_ASSERT( bClipRegion, "OutputDevice::ImplSelectClipRegion() - can't cerate region" );
            bRegionRect = rRegion.ImplGetNextRect( aInfo, nX, nY, nWidth, nHeight );
        }
    }
	pGraphics->EndSetClipRegion();
	return bClipRegion;
}

void OutputDevice::ImplInitLineColor()
{
	//DBG_TESTSOLARMUTEX();

	if( mbLineColor )
	{
		if( ROP_0 == meRasterOp )
			mpGraphics->SetROPLineColor( SAL_ROP_0 );
		else if( ROP_1 == meRasterOp )
			mpGraphics->SetROPLineColor( SAL_ROP_1 );
		else if( ROP_INVERT == meRasterOp )
			mpGraphics->SetROPLineColor( SAL_ROP_INVERT );
		else
			mpGraphics->SetLineColor( ImplColorToSal( maLineColor ) );
	}
	else
		mpGraphics->SetLineColor();

	mbInitLineColor = FALSE;
}
void OutputDevice::ImplInitFillColor()
{
	if( mbFillColor )
	{
		if( ROP_0 == meRasterOp )
			mpGraphics->SetROPFillColor( SAL_ROP_0 );
		else if( ROP_1 == meRasterOp )
			mpGraphics->SetROPFillColor( SAL_ROP_1 );
		else if( ROP_INVERT == meRasterOp )
			mpGraphics->SetROPFillColor( SAL_ROP_INVERT );
		else
			mpGraphics->SetFillColor( ImplColorToSal( maFillColor ) );
	}
	else
		mpGraphics->SetFillColor();

	mbInitFillColor = FALSE;
}


Polygon OutputDevice::ImplSubdivideBezier( const Polygon& rPoly )
{
    Polygon aPoly;

    // #100127# Use adaptive subdivide instead of fixed 25 segments
    rPoly.AdaptiveSubdivide( aPoly );

    return aPoly;
}
// =======================================================================

PolyPolygon OutputDevice::ImplSubdivideBezier( const PolyPolygon& rPolyPoly )
{
    USHORT i, nPolys = rPolyPoly.Count();
    PolyPolygon aPolyPoly( nPolys );
    for( i=0; i<nPolys; ++i )
        aPolyPoly.Insert( ImplSubdivideBezier( rPolyPoly.GetObject(i) ) );

    return aPolyPoly;
}

void OutputDevice::DrawPixel( const Point& rPt, const Color& rColor )
{
	Color aColor( rColor );

	Point aPt = ImplLogicToDevicePixel( rPt );

	// we need a graphics
	if ( !mpGraphics )
		return;

	if ( mbInitClipRegion )
		ImplInitClipRegion();
	if ( mbOutputClipped )
		return;

	mpGraphics->DrawPixel( aPt.X(), aPt.Y(), ImplColorToSal( rColor ), this );
}
void OutputDevice::DrawPixel( const Point& rPt )
{
	Point aPt = ImplLogicToDevicePixel( rPt );

	// we need a graphics
	if ( !mpGraphics )
		return;

	if ( mbInitClipRegion )
		ImplInitClipRegion();
	if ( mbOutputClipped )
		return;

	if ( mbInitLineColor )
		ImplInitLineColor();

	mpGraphics->DrawPixel( aPt.X(), aPt.Y(), this );
}
void OutputDevice::DrawLine( const Point& rStartPt, const Point& rEndPt )
{
	if ( !mbLineColor )
		return;

	if ( !mpGraphics )
			return;

	if ( mbInitClipRegion )
		ImplInitClipRegion();
	if ( mbOutputClipped )
		return;

	if ( mbInitLineColor )
		ImplInitLineColor();

	Point aStartPt = ImplLogicToDevicePixel( rStartPt );
	Point aEndPt = ImplLogicToDevicePixel( rEndPt );

	mpGraphics->DrawLine( aStartPt.X(), aStartPt.Y(), aEndPt.X(), aEndPt.Y(), this );
}
void OutputDevice::DrawLine( const Point& rStartPt, const Point& rEndPt, const LineInfo& rLineInfo )
{
	if ( rLineInfo.IsDefault() )
	{
		DrawLine( rStartPt, rEndPt );
		return;
	}

	if ( !mbLineColor || ( LINE_NONE == rLineInfo.GetStyle() ) )
		return;

	if( !mpGraphics )
		return;

	if ( mbInitClipRegion )
		ImplInitClipRegion();

	if ( mbOutputClipped )
		return;

	const LineInfo aInfo( ImplLogicToDevicePixel( rLineInfo ) );

	if( ( aInfo.GetWidth() > 1L ) || ( LINE_DASH == aInfo.GetStyle() ) )
	{
		Polygon 			aPoly( 2 ); aPoly[ 0 ] = rStartPt; aPoly[ 1 ] = rEndPt;
		ImplLineConverter	aLineCvt( ImplLogicToDevicePixel( aPoly ), aInfo, ( mbRefPoint ) ? &maRefPoint : NULL );

		if ( aInfo.GetWidth() > 1 )
		{
			const Color 	aOldLineColor( maLineColor );
			const Color 	aOldFillColor( maFillColor );

			SetLineColor();
			ImplInitLineColor();
			SetFillColor( aOldLineColor );
			ImplInitFillColor();

			for( const Polygon* pPoly = aLineCvt.ImplGetFirst(); pPoly; pPoly = aLineCvt.ImplGetNext() )
				mpGraphics->DrawPolygon( pPoly->GetSize(), (const SalPoint*) pPoly->GetConstPointAry(), this );

			SetFillColor( aOldFillColor );
			SetLineColor( aOldLineColor );
		}
		else
		{
			if ( mbInitLineColor )
				ImplInitLineColor();

			for ( const Polygon* pPoly = aLineCvt.ImplGetFirst(); pPoly; pPoly = aLineCvt.ImplGetNext() )
				mpGraphics->DrawLine( (*pPoly)[ 0 ].X(), (*pPoly)[ 0 ].Y(), (*pPoly)[ 1 ].X(), (*pPoly)[ 1 ].Y(), this );
		}
	}
	else
	{
		const Point aStartPt( ImplLogicToDevicePixel( rStartPt ) );
		const Point aEndPt( ImplLogicToDevicePixel( rEndPt ) );

		if ( mbInitLineColor )
			ImplInitLineColor();

		mpGraphics->DrawLine( aStartPt.X(), aStartPt.Y(), aEndPt.X(), aEndPt.Y(), this );
	}
}
void OutputDevice::DrawRect( const Rectangle& rRect )
{
	if ( (!mbLineColor && !mbFillColor) )
		return;

	Rectangle aRect( ImplLogicToDevicePixel( rRect ) );

	if ( aRect.IsEmpty() )
		return;
	aRect.Justify();

	if ( !mpGraphics )
			return;

	if ( mbInitClipRegion )
		ImplInitClipRegion();
	if ( mbOutputClipped )
		return;

	if ( mbInitLineColor )
		ImplInitLineColor();
	if ( mbInitFillColor )
		ImplInitFillColor();

	mpGraphics->DrawRect( aRect.Left(), aRect.Top(), aRect.GetWidth(), aRect.GetHeight(), this );
}
void OutputDevice::DrawRect( const Rectangle& rRect, ULONG nHorzRound, ULONG nVertRound )
{
	if ( !mbLineColor && !mbFillColor )
		return;

	const Rectangle aRect( ImplLogicToDevicePixel( rRect ) );

	if ( aRect.IsEmpty() )
		return;

	nHorzRound = ImplLogicWidthToDevicePixel( nHorzRound );
	nVertRound = ImplLogicHeightToDevicePixel( nVertRound );

	// we need a graphics
	if ( !mpGraphics )
			return;

	if ( mbInitClipRegion )
		ImplInitClipRegion();
	if ( mbOutputClipped )
		return;

	if ( mbInitLineColor )
		ImplInitLineColor();
	if ( mbInitFillColor )
		ImplInitFillColor();

	if ( !nHorzRound && !nVertRound )
		mpGraphics->DrawRect( aRect.Left(), aRect.Top(), aRect.GetWidth(), aRect.GetHeight(), this );
	else
	{
		const Polygon aRoundRectPoly( aRect, nHorzRound, nVertRound );

		if ( aRoundRectPoly.GetSize() >= 2 )
		{
			const SalPoint* pPtAry = (const SalPoint*)aRoundRectPoly.GetConstPointAry();
			
			if ( !mbFillColor )
				mpGraphics->DrawPolyLine( aRoundRectPoly.GetSize(), pPtAry, this );
			else
				mpGraphics->DrawPolygon( aRoundRectPoly.GetSize(), pPtAry, this );
		}
	}
}
void OutputDevice::DrawEllipse( const Rectangle& rRect )
{
	if	( !mbLineColor && !mbFillColor )
		return;

	Rectangle aRect( ImplLogicToDevicePixel( rRect ) );
	if ( aRect.IsEmpty() )
		return;

	// we need a graphics
	if ( !mpGraphics )
		return;

	if ( mbInitClipRegion )
		ImplInitClipRegion();
	if ( mbOutputClipped )
		return;

	if ( mbInitLineColor )
		ImplInitLineColor();

	Polygon aRectPoly( aRect.Center(), aRect.GetWidth() >> 1, aRect.GetHeight() >> 1 );
	if ( aRectPoly.GetSize() >= 2 )
	{
		const SalPoint* pPtAry = (const SalPoint*)aRectPoly.GetConstPointAry();
		if ( !mbFillColor )
			mpGraphics->DrawPolyLine( aRectPoly.GetSize(), pPtAry, this );
		else
		{
			if ( mbInitFillColor )
				ImplInitFillColor();
			mpGraphics->DrawPolygon( aRectPoly.GetSize(), pPtAry, this );
		}
	}
}
void OutputDevice::DrawArc( const Rectangle& rRect, const Point& rStartPt, const Point& rEndPt )
{
	if ( !mbLineColor  )
		return;

	Rectangle aRect( ImplLogicToDevicePixel( rRect ) );
	if ( aRect.IsEmpty() )
		return;

	// we need a graphics
	if ( !mpGraphics )
		return;

	if ( mbInitClipRegion )
		ImplInitClipRegion();
	if ( mbOutputClipped )
		return;

	if ( mbInitLineColor )
		ImplInitLineColor();

	const Point 	aStart( ImplLogicToDevicePixel( rStartPt ) );
	const Point 	aEnd( ImplLogicToDevicePixel( rEndPt ) );
	Polygon 		aArcPoly( aRect, aStart, aEnd, POLY_ARC );

	if ( aArcPoly.GetSize() >= 2 )
	{
		const SalPoint* pPtAry = (const SalPoint*)aArcPoly.GetConstPointAry();
		mpGraphics->DrawPolyLine( aArcPoly.GetSize(), pPtAry, this );
	}
}
void OutputDevice::DrawPie( const Rectangle& rRect, const Point& rStartPt, const Point& rEndPt )
{
	if ( !mbLineColor && !mbFillColor )
		return;

	Rectangle aRect( ImplLogicToDevicePixel( rRect ) );
	if ( aRect.IsEmpty() )
		return;

	// we need a graphics
	if ( !mpGraphics )
		return;

	if ( mbInitClipRegion )
		ImplInitClipRegion();
	if ( mbOutputClipped )
		return;

	if ( mbInitLineColor )
		ImplInitLineColor();

	const Point 	aStart( ImplLogicToDevicePixel( rStartPt ) );
	const Point 	aEnd( ImplLogicToDevicePixel( rEndPt ) );
	Polygon 		aPiePoly( aRect, aStart, aEnd, POLY_PIE );

	if ( aPiePoly.GetSize() >= 2 )
	{
		const SalPoint* pPtAry = (const SalPoint*)aPiePoly.GetConstPointAry();
		if ( !mbFillColor )
			mpGraphics->DrawPolyLine( aPiePoly.GetSize(), pPtAry, this );
		else
		{
			if ( mbInitFillColor )
				ImplInitFillColor();
			mpGraphics->DrawPolygon( aPiePoly.GetSize(), pPtAry, this );
		}
	}
}
void OutputDevice::DrawChord( const Rectangle& rRect, const Point& rStartPt, const Point& rEndPt )
{
	if ( !mbLineColor && !mbFillColor )
		return;

	Rectangle aRect( ImplLogicToDevicePixel( rRect ) );
	if ( aRect.IsEmpty() )
		return;

	// we need a graphics
	if ( !mpGraphics )
		return;

	if ( mbInitClipRegion )
		ImplInitClipRegion();
	if ( mbOutputClipped )
		return;

	if ( mbInitLineColor )
		ImplInitLineColor();

	const Point 	aStart( ImplLogicToDevicePixel( rStartPt ) );
	const Point 	aEnd( ImplLogicToDevicePixel( rEndPt ) );
	Polygon 		aChordPoly( aRect, aStart, aEnd, POLY_CHORD );

	if ( aChordPoly.GetSize() >= 2 )
	{
		const SalPoint* pPtAry = (const SalPoint*)aChordPoly.GetConstPointAry();
		if ( !mbFillColor )
			mpGraphics->DrawPolyLine( aChordPoly.GetSize(), pPtAry, this );
		else
		{
			if ( mbInitFillColor )
				ImplInitFillColor();
			mpGraphics->DrawPolygon( aChordPoly.GetSize(), pPtAry, this );
		}
	}
}

void OutputDevice::DrawPolyLine( const Polygon& rPoly )
{
	USHORT nPoints = rPoly.GetSize();
	if ( !mbLineColor || (nPoints < 2) )
		return;

	// we need a graphics
	if ( !mpGraphics )
			return;

	if ( mbInitClipRegion )
		ImplInitClipRegion();
	if ( mbOutputClipped )
		return;

	if ( mbInitLineColor )
		ImplInitLineColor();

	Polygon aPoly = ImplLogicToDevicePixel( rPoly );
    const SalPoint* pPtAry = (const SalPoint*)aPoly.GetConstPointAry();

    // #100127# Forward beziers to sal, if any
    if( aPoly.HasFlags() )
    {
        const BYTE* pFlgAry = aPoly.GetConstFlagAry();
        if( !mpGraphics->DrawPolyLineBezier( nPoints, pPtAry, pFlgAry, this ) )
        {
            aPoly = ImplSubdivideBezier(aPoly);
            pPtAry = (const SalPoint*)aPoly.GetConstPointAry();
            mpGraphics->DrawPolyLine( aPoly.GetSize(), pPtAry, this );
        }
    }
    else
    {
        mpGraphics->DrawPolyLine( nPoints, pPtAry, this );
    }
}
void OutputDevice::DrawPolyLine( const Polygon& rPoly, const LineInfo& rLineInfo )
{
	if ( rLineInfo.IsDefault() )
	{
		DrawPolyLine( rPoly );
		return;
	}
#ifdef AVS
    // #i101491#
    // Try direct Fallback to B2D-Version of DrawPolyLine
    if((mnAntialiasing & ANTIALIASING_ENABLE_B2DDRAW) 
        && LINE_SOLID == rLineInfo.GetStyle())
    {
		DrawPolyLine(rPoly.getB2DPolygon(), (double)rLineInfo.GetWidth(), basegfx::B2DLINEJOIN_ROUND);
		return;
    }
#endif
    ImpDrawPolyLineWithLineInfo(rPoly, rLineInfo);
}
void OutputDevice::ImpDrawPolyLineWithLineInfo(const Polygon& rPoly, const LineInfo& rLineInfo)
{
	USHORT nPoints = rPoly.GetSize();

	if ( !mbLineColor || ( nPoints < 2 ) || ( LINE_NONE == rLineInfo.GetStyle() ) )
		return;

    Polygon aPoly = ImplLogicToDevicePixel( rPoly );

    // #100127# LineInfo is not curve-safe, subdivide always
    if( aPoly.HasFlags() )
    {
        aPoly = ImplSubdivideBezier( aPoly );
        nPoints = aPoly.GetSize();
    }

	// we need a graphics
	if ( !mpGraphics )
		return;

	if ( mbInitClipRegion )
		ImplInitClipRegion();

	if ( mbOutputClipped )
		return;

	const LineInfo aInfo( ImplLogicToDevicePixel( rLineInfo ) );

	if( aInfo.GetWidth() > 1L )
	{
		const Color 		aOldLineColor( maLineColor );
		const Color 		aOldFillColor( maFillColor );
		ImplLineConverter	aLineCvt( aPoly, aInfo, ( mbRefPoint ) ? &maRefPoint : NULL );

		SetLineColor();
		ImplInitLineColor();
		SetFillColor( aOldLineColor );
		ImplInitFillColor();
        bool bDone(false);

        if(!bDone)
        {
            for( const Polygon* pPoly = aLineCvt.ImplGetFirst(); pPoly; pPoly = aLineCvt.ImplGetNext() )
            {
	    		mpGraphics->DrawPolygon( pPoly->GetSize(), (const SalPoint*) pPoly->GetConstPointAry(), this );
            }
        }

        SetLineColor( aOldLineColor );
		SetFillColor( aOldFillColor );
	}
	else
	{
		if ( mbInitLineColor )
			ImplInitLineColor();
		if ( LINE_DASH == aInfo.GetStyle() )
		{
			ImplLineConverter	aLineCvt( aPoly, aInfo, ( mbRefPoint ) ? &maRefPoint : NULL );
			for( const Polygon* pPoly = aLineCvt.ImplGetFirst(); pPoly; pPoly = aLineCvt.ImplGetNext() )
				mpGraphics->DrawPolyLine( pPoly->GetSize(), (const SalPoint*)pPoly->GetConstPointAry(), this );
		}
		else
		mpGraphics->DrawPolyLine( nPoints, (const SalPoint*) aPoly.GetConstPointAry(), this );
	}
}

void OutputDevice::DrawPolygon( const Polygon& rPoly )
{
	USHORT nPoints = rPoly.GetSize();
	if ( (!mbLineColor && !mbFillColor) || (nPoints < 2) )
		return;

	// we need a graphics
	if ( !mpGraphics )
			return;
	if ( mbInitClipRegion )
		ImplInitClipRegion();
	if ( mbOutputClipped )
		return;

	if ( mbInitLineColor )
		ImplInitLineColor();
	if ( mbInitFillColor )
		ImplInitFillColor();

	Polygon aPoly = ImplLogicToDevicePixel( rPoly );
	const SalPoint* pPtAry = (const SalPoint*)aPoly.GetConstPointAry();

    // #100127# Forward beziers to sal, if any
    if( aPoly.HasFlags() )
    {
        const BYTE* pFlgAry = aPoly.GetConstFlagAry();
        if( !mpGraphics->DrawPolygonBezier( nPoints, pPtAry, pFlgAry, this ) )
        {
            aPoly = ImplSubdivideBezier(aPoly);
            pPtAry = (const SalPoint*)aPoly.GetConstPointAry();
            mpGraphics->DrawPolygon( aPoly.GetSize(), pPtAry, this );
        }
    }
    else
    {
        mpGraphics->DrawPolygon( nPoints, pPtAry, this );
    }
}
// =======================================================================

void OutputDevice::ImplDrawPolygon( const Polygon& rPoly, const PolyPolygon* pClipPolyPoly )
{
	if( pClipPolyPoly )
		ImplDrawPolyPolygon( rPoly, pClipPolyPoly );
	else
	{
		USHORT nPoints = rPoly.GetSize();

		if ( nPoints < 2 )
			return;

		const SalPoint* pPtAry = (const SalPoint*)rPoly.GetConstPointAry();
		mpGraphics->DrawPolygon( nPoints, pPtAry, this );
	}
}
void OutputDevice::ImplDrawPolyPolygon( const PolyPolygon& rPolyPoly, const PolyPolygon* pClipPolyPoly )
{
	PolyPolygon* pPolyPoly;

	if( pClipPolyPoly )
	{
		pPolyPoly = new PolyPolygon;
		rPolyPoly.GetIntersection( *pClipPolyPoly, *pPolyPoly );
	}
	else
		pPolyPoly = (PolyPolygon*) &rPolyPoly;

	if( pPolyPoly->Count() == 1 )
	{
		const Polygon	rPoly = pPolyPoly->GetObject( 0 );
		USHORT			nSize = rPoly.GetSize();
		
		if( nSize >= 2 )
		{
			const SalPoint* pPtAry = (const SalPoint*)rPoly.GetConstPointAry();
			mpGraphics->DrawPolygon( nSize, pPtAry, this );
		}
	}
	else if( pPolyPoly->Count() )
	{
		USHORT				nCount = pPolyPoly->Count();
		sal_uInt32*			pPointAry = new sal_uInt32[nCount];
		PCONSTSALPOINT* 	pPointAryAry = new PCONSTSALPOINT[nCount];
		USHORT				i = 0;
		do
		{
			const Polygon&	rPoly = pPolyPoly->GetObject( i );
			USHORT			nSize = rPoly.GetSize();
			if ( nSize )
			{
				pPointAry[i]	= nSize;
				pPointAryAry[i] = (PCONSTSALPOINT)rPoly.GetConstPointAry();
				i++;
			}
			else
				nCount--;
		}
		while( i < nCount );

		if( nCount == 1 )
			mpGraphics->DrawPolygon( *pPointAry, *pPointAryAry, this );
		else
			mpGraphics->DrawPolyPolygon( nCount, pPointAry, pPointAryAry, this );
		
		delete[] pPointAry;
		delete[] pPointAryAry;
	}

	if( pClipPolyPoly )
		delete pPolyPoly;
}

void OutputDevice::DrawPolyPolygon( const PolyPolygon& rPolyPoly )
{
	USHORT nPoly = rPolyPoly.Count();

	if ( (!mbLineColor && !mbFillColor) || !nPoly )
		return;

	// we need a graphics
	if ( !mpGraphics )
			return;

	if ( mbInitClipRegion )
		ImplInitClipRegion();
	if ( mbOutputClipped )
		return;

	if ( mbInitLineColor )
		ImplInitLineColor();
	if ( mbInitFillColor )
		ImplInitFillColor();

	if ( nPoly == 1 )
	{
        // #100127# Map to DrawPolygon
        Polygon aPoly = rPolyPoly.GetObject( 0 );
		if( aPoly.GetSize() >= 2 )
			DrawPolygon( aPoly );
	}
	else
	{
        // #100127# moved real PolyPolygon draw to separate method,
        // have to call recursively, avoiding duplicate
        // ImplLogicToDevicePixel calls
        ImplDrawPolyPolygon( nPoly, ImplLogicToDevicePixel( rPolyPoly ) );
	}
}
// #100127# Extracted from OutputDevice::DrawPolyPolygon()
void OutputDevice::ImplDrawPolyPolygon( USHORT nPoly, const PolyPolygon& rPolyPoly )
{
    // AW: This crashes on empty PolyPolygons, avoid that
    if(!nPoly)
        return;

    sal_uInt32			aStackAry1[OUTDEV_POLYPOLY_STACKBUF];
    PCONSTSALPOINT		aStackAry2[OUTDEV_POLYPOLY_STACKBUF];
    BYTE*				aStackAry3[OUTDEV_POLYPOLY_STACKBUF];
    sal_uInt32*			pPointAry;
    PCONSTSALPOINT* 	pPointAryAry;
    const BYTE**	 	pFlagAryAry;
    USHORT				i = 0, j = 0, last = 0;
    BOOL				bHaveBezier = sal_False;
    if ( nPoly > OUTDEV_POLYPOLY_STACKBUF )
    {
        pPointAry		= new sal_uInt32[nPoly];
        pPointAryAry	= new PCONSTSALPOINT[nPoly];
        pFlagAryAry		= new const BYTE*[nPoly];
    }
    else
    {
        pPointAry		= aStackAry1;
        pPointAryAry	= aStackAry2;
        pFlagAryAry		= (const BYTE**)aStackAry3;
    }
    do
    {
        const Polygon&	rPoly = rPolyPoly.GetObject( i );
        USHORT			nSize = rPoly.GetSize();
        if ( nSize )
        {
            pPointAry[j]	= nSize;
            pPointAryAry[j] = (PCONSTSALPOINT)rPoly.GetConstPointAry();
            pFlagAryAry[j]  = rPoly.GetConstFlagAry();
            last 			= i;

            if( pFlagAryAry[j] )
                bHaveBezier = sal_True;

            ++j;
        }

        ++i;
    }
    while ( i < nPoly );

    if ( j == 1 )
    {
        // #100127# Forward beziers to sal, if any
        if( bHaveBezier )
        {
            if( !mpGraphics->DrawPolygonBezier( *pPointAry, *pPointAryAry, *pFlagAryAry, this ) )
            {
                Polygon aPoly = ImplSubdivideBezier( rPolyPoly.GetObject( last ) );
                mpGraphics->DrawPolygon( aPoly.GetSize(), (const SalPoint*)aPoly.GetConstPointAry(), this );
            }
        }
        else
        {
            mpGraphics->DrawPolygon( *pPointAry, *pPointAryAry, this );
        }
    }
    else
    {
        // #100127# Forward beziers to sal, if any
        if( bHaveBezier )
        {
            if( !mpGraphics->DrawPolyPolygonBezier( j, pPointAry, pPointAryAry, pFlagAryAry, this ) )
            {
                PolyPolygon aPolyPoly = ImplSubdivideBezier( rPolyPoly );
                ImplDrawPolyPolygon( aPolyPoly.Count(), aPolyPoly );
            }
        }
        else
        {
            mpGraphics->DrawPolyPolygon( j, pPointAry, pPointAryAry, this );
        }
    }

    if ( pPointAry != aStackAry1 )
    {
        delete[] pPointAry;
        delete[] pPointAryAry;
        delete[] pFlagAryAry;
    }
}

// -----------------------------------------------------------------------

void OutputDevice::ImplDrawText( const Rectangle& rRect,
                                 const String& rOrigStr, USHORT nStyle,
                                 MetricVector* pVector, String* pDisplayText )
{
    Color aOldTextColor;
    Color aOldTextFillColor;
    BOOL  bRestoreFillColor = false;
#ifdef AVS
    if ( (nStyle & TEXT_DRAW_DISABLE) && ! pVector )
    {
        BOOL  bHighContrastBlack = FALSE;
        BOOL  bHighContrastWhite = FALSE;
		Color aCol;
		if( IsBackground() )
			aCol = GetBackground().GetColor();
		else
			// best guess is the face color here
			// but it may be totally wrong. the background color
			// was typically already reset
			aCol = GetSettings().GetStyleSettings().GetFaceColor();

		bHighContrastBlack = aCol.IsDark();
		bHighContrastWhite = aCol.IsBright() && GetSettings().GetStyleSettings().GetHighContrastMode();

        aOldTextColor = GetTextColor();
        if ( IsTextFillColor() )
        {
            bRestoreFillColor = TRUE;
            aOldTextFillColor = GetTextFillColor();
        }
        else
            bRestoreFillColor = FALSE;
        if( bHighContrastBlack )
            SetTextColor( COL_GREEN );
        else if( bHighContrastWhite )
            SetTextColor( COL_LIGHTGREEN );
        else
        {
            // draw disabled text always without shadow
            // as it fits better with native look
            /*
            SetTextColor( GetSettings().GetStyleSettings().GetLightColor() );
            Rectangle aRect = rRect;
            aRect.Move( 1, 1 );
            DrawText( aRect, rOrigStr, nStyle & ~TEXT_DRAW_DISABLE );
            */
            SetTextColor( GetSettings().GetStyleSettings().GetShadowColor() );
        }
    }
#endif
    long        nWidth          = rRect.GetWidth();
    long        nHeight         = rRect.GetHeight();

    if ( ((nWidth <= 0) || (nHeight <= 0)) && (nStyle & TEXT_DRAW_CLIP) )
        return;

    Point       aPos            = rRect.TopLeft();

    long        nTextHeight     = GetTextHeight();
    TextAlign   eAlign          = GetTextAlign();
    xub_StrLen  nMnemonicPos    = STRING_NOTFOUND;

    String aStr = rOrigStr;
    if ( nStyle & TEXT_DRAW_MNEMONIC )
        aStr = GetNonMnemonicString( aStr, nMnemonicPos );
#ifdef AVS
    // Mehrzeiligen Text behandeln wir anders
    if ( nStyle & TEXT_DRAW_MULTILINE )
    {

        XubString               aLastLine;
        ImplMultiTextLineInfo   aMultiLineInfo;
        ImplTextLineInfo*       pLineInfo;
        long                    nMaxTextWidth;
        xub_StrLen              i;
        xub_StrLen              nLines;
        xub_StrLen              nFormatLines;

        if ( nTextHeight )
        {
            nMaxTextWidth = ImplGetTextLines( aMultiLineInfo, nWidth, aStr, nStyle );
            nLines = (xub_StrLen)(nHeight/nTextHeight);
            nFormatLines = aMultiLineInfo.Count();
            if ( !nLines )
                nLines = 1;
            if ( nFormatLines > nLines )
            {
                if ( nStyle & TEXT_DRAW_ENDELLIPSIS )
                {
                    // Letzte Zeile zusammenbauen und kuerzen
                    nFormatLines = nLines-1;

                    pLineInfo = aMultiLineInfo.GetLine( nFormatLines );
                    aLastLine = aStr.Copy( pLineInfo->GetIndex() );
                    aLastLine.ConvertLineEnd( LINEEND_LF );
                    // Alle LineFeed's durch Spaces ersetzen
                    xub_StrLen nLastLineLen = aLastLine.Len();
                    for ( i = 0; i < nLastLineLen; i++ )
                    {
                        if ( aLastLine.GetChar( i ) == _LF )
                            aLastLine.SetChar( i, ' ' );
                    }
                    aLastLine = GetEllipsisString( aLastLine, nWidth, nStyle );
                    nStyle &= ~(TEXT_DRAW_VCENTER | TEXT_DRAW_BOTTOM);
                    nStyle |= TEXT_DRAW_TOP;
                }
            }
            else
            {
                if ( nMaxTextWidth <= nWidth )
                    nStyle &= ~TEXT_DRAW_CLIP;
            }

            // Muss in der Hoehe geclippt werden?
            if ( nFormatLines*nTextHeight > nHeight )
                nStyle |= TEXT_DRAW_CLIP;

            // Clipping setzen
            if ( nStyle & TEXT_DRAW_CLIP )
            {
                Push( PUSH_CLIPREGION );
                IntersectClipRegion( rRect );
            }

            // Vertikales Alignment
            if ( nStyle & TEXT_DRAW_BOTTOM )
                aPos.Y() += nHeight-(nFormatLines*nTextHeight);
            else if ( nStyle & TEXT_DRAW_VCENTER )
                aPos.Y() += (nHeight-(nFormatLines*nTextHeight))/2;

            // Font Alignment
            if ( eAlign == ALIGN_BOTTOM )
                aPos.Y() += nTextHeight;
            else if ( eAlign == ALIGN_BASELINE )
                aPos.Y() += GetFontMetric().GetAscent();

            // Alle Zeilen ausgeben, bis auf die letzte
            for ( i = 0; i < nFormatLines; i++ )
            {
                pLineInfo = aMultiLineInfo.GetLine( i );
                if ( nStyle & TEXT_DRAW_RIGHT )
                    aPos.X() += nWidth-pLineInfo->GetWidth();
                else if ( nStyle & TEXT_DRAW_CENTER )
                    aPos.X() += (nWidth-pLineInfo->GetWidth())/2;
                xub_StrLen nIndex   = pLineInfo->GetIndex();
                xub_StrLen nLineLen = pLineInfo->GetLen();
                DrawText( aPos, aStr, nIndex, nLineLen, pVector, pDisplayText );
                if ( !(GetSettings().GetStyleSettings().GetOptions() & STYLE_OPTION_NOMNEMONICS) && !pVector )
                {
                    if ( (nMnemonicPos >= nIndex) && (nMnemonicPos < nIndex+nLineLen) )
                    {
                        long        nMnemonicX;
                        long        nMnemonicY;
                        long        nMnemonicWidth;

                        sal_Int32* pCaretXArray = (sal_Int32*) alloca( 2 * sizeof(sal_Int32) * nLineLen );
                        /*BOOL bRet =*/ GetCaretPositions( aStr, pCaretXArray,
                                                nIndex, nLineLen);
                        long lc_x1 = pCaretXArray[2*(nMnemonicPos - nIndex)];
                        long lc_x2 = pCaretXArray[2*(nMnemonicPos - nIndex)+1];
                        nMnemonicWidth = ::abs((int)(lc_x1 - lc_x2));

                        Point       aTempPos = LogicToPixel( aPos );
                        nMnemonicX = mnOutOffX + aTempPos.X() + ImplLogicWidthToDevicePixel( Min( lc_x1, lc_x2 ) );
                        nMnemonicY = mnOutOffY + aTempPos.Y() + ImplLogicWidthToDevicePixel( GetFontMetric().GetAscent() );
                        ImplDrawMnemonicLine( nMnemonicX, nMnemonicY, nMnemonicWidth );
                    }
                }
                aPos.Y() += nTextHeight;
                aPos.X() = rRect.Left();
            }


            // Gibt es noch eine letzte Zeile, dann diese linksbuendig ausgeben,
            // da die Zeile gekuerzt wurde
            if ( aLastLine.Len() )
                DrawText( aPos, aLastLine, 0, STRING_LEN, pVector, pDisplayText );

            // Clipping zuruecksetzen
            if ( nStyle & TEXT_DRAW_CLIP )
                Pop();
        }
    }
    else
#endif
    {
        long nTextWidth = GetTextWidth( aStr );
#ifdef AVS
        // Evt. Text kuerzen
        if ( nTextWidth > nWidth )
        {
            if ( nStyle & TEXT_DRAW_ELLIPSIS )
            {
                aStr = GetEllipsisString( aStr, nWidth, nStyle );
                nStyle &= ~(TEXT_DRAW_CENTER | TEXT_DRAW_RIGHT);
                nStyle |= TEXT_DRAW_LEFT;
                nTextWidth = GetTextWidth( aStr );
            }
        }
        else
        {
            if ( nTextHeight <= nHeight )
                nStyle &= ~TEXT_DRAW_CLIP;
        }

        // horizontal text alignment
        if ( nStyle & TEXT_DRAW_RIGHT )
            aPos.X() += nWidth-nTextWidth;
        else if ( nStyle & TEXT_DRAW_CENTER )
            aPos.X() += (nWidth-nTextWidth)/2;

        // vertical font alignment
        if ( eAlign == ALIGN_BOTTOM )
            aPos.Y() += nTextHeight;
        else if ( eAlign == ALIGN_BASELINE )
            aPos.Y() += GetFontMetric().GetAscent();

        if ( nStyle & TEXT_DRAW_BOTTOM )
            aPos.Y() += nHeight-nTextHeight;
        else if ( nStyle & TEXT_DRAW_VCENTER )
            aPos.Y() += (nHeight-nTextHeight)/2;

        long        nMnemonicX = 0;
        long        nMnemonicY = 0;
        long        nMnemonicWidth = 0;
        if ( nMnemonicPos != STRING_NOTFOUND )
        {
            sal_Int32* pCaretXArray = (sal_Int32*) alloca( 2 * sizeof(sal_Int32) * aStr.Len() );
            /*BOOL bRet =*/ GetCaretPositions( aStr, pCaretXArray, 0, aStr.Len() );
            long lc_x1 = pCaretXArray[2*(nMnemonicPos)];
            long lc_x2 = pCaretXArray[2*(nMnemonicPos)+1];
            nMnemonicWidth = ::abs((int)(lc_x1 - lc_x2));

            Point aTempPos = LogicToPixel( aPos );
            nMnemonicX = mnOutOffX + aTempPos.X() + ImplLogicWidthToDevicePixel( Min(lc_x1, lc_x2) );
            nMnemonicY = mnOutOffY + aTempPos.Y() + ImplLogicWidthToDevicePixel( GetFontMetric().GetAscent() );
        }
#endif
        if ( nStyle & TEXT_DRAW_CLIP )
        {
            Push( PUSH_CLIPREGION );
            IntersectClipRegion( rRect );
            DrawText( aPos, aStr, 0, STRING_LEN, pVector, pDisplayText );
#ifdef AVS
            if ( !(GetSettings().GetStyleSettings().GetOptions() & STYLE_OPTION_NOMNEMONICS) && !pVector )
            {
                if ( nMnemonicPos != STRING_NOTFOUND )
                    ImplDrawMnemonicLine( nMnemonicX, nMnemonicY, nMnemonicWidth );
            }
#endif
            Pop();
        }
        else
        {
            DrawText( aPos, aStr, 0, STRING_LEN, pVector, pDisplayText );
#ifdef AVS
            if ( !(GetSettings().GetStyleSettings().GetOptions() & STYLE_OPTION_NOMNEMONICS) && !pVector )
            {
                if ( nMnemonicPos != STRING_NOTFOUND )
                    ImplDrawMnemonicLine( nMnemonicX, nMnemonicY, nMnemonicWidth );
            }
#endif
        }
    }

    if ( nStyle & TEXT_DRAW_DISABLE && !pVector )
    {
        SetTextColor( aOldTextColor );
        if ( bRestoreFillColor )
            SetTextFillColor( aOldTextFillColor );
    }
}
// -----------------------------------------------------------------------

void OutputDevice::ImplDrawTextRect( long nBaseX, long nBaseY,
                                     long nX, long nY, long nWidth, long nHeight )
{
    short nOrientation = mpFontEntry->mnOrientation;
    if ( nOrientation )
    {
        // Rotate rect without rounding problems for 90 degree rotations
        if ( !(nOrientation % 900) )
        {
            nX -= nBaseX;
            nY -= nBaseY;

            if ( nOrientation == 900 )
            {
                long nTemp = nX;
                nX = nY;
                nY = -nTemp;
                nTemp = nWidth;
                nWidth = nHeight;
                nHeight = nTemp;
                nY -= nHeight;
            }
            else if ( nOrientation == 1800 )
            {
                nX = -nX;
                nY = -nY;
                nX -= nWidth;
                nY -= nHeight;
            }
            else /* ( nOrientation == 2700 ) */
            {
                long nTemp = nX;
                nX = -nY;
                nY = nTemp;
                nTemp = nWidth;
                nWidth = nHeight;
                nHeight = nTemp;
                nX -= nWidth;
            }

            nX += nBaseX;
            nY += nBaseY;
        }
        else
        {
            // inflate because polygons are drawn smaller
            Rectangle aRect( Point( nX, nY ), Size( nWidth+1, nHeight+1 ) );
            Polygon   aPoly( aRect );
            aPoly.Rotate( Point( nBaseX, nBaseY ), mpFontEntry->mnOrientation );
            ImplDrawPolygon( aPoly );
            return;
        }
    }

    mpGraphics->DrawRect( nX, nY, nWidth, nHeight, this );
}
// -----------------------------------------------------------------------

long OutputDevice::GetTextWidth( const String& rStr,
                                 xub_StrLen nIndex, xub_StrLen nLen ) const
{
    //DBG_TRACE( "OutputDevice::GetTextWidth()" );
    //DBG_CHKTHIS( OutputDevice, ImplDbgCheckOutputDevice );

    long nWidth = GetTextArray( rStr, NULL, nIndex, nLen );
    return nWidth;
}

// -----------------------------------------------------------------------

long OutputDevice::GetTextArray( const String& rStr, sal_Int32* pDXAry,
                                 xub_StrLen nIndex, xub_StrLen nLen )  const
{
    //DBG_TRACE( "OutputDevice::GetTextArray()" );
    //DBG_CHKTHIS( OutputDevice, ImplDbgCheckOutputDevice );

    if( nIndex >= rStr.Len() )
        return 0;
    if( (ULONG)nIndex+nLen >= rStr.Len() )
        nLen = rStr.Len() - nIndex;

    // do layout
    SalLayout* pSalLayout = ImplLayout( rStr, nIndex, nLen );
    if( !pSalLayout )
        return 0;

    long nWidth = pSalLayout->FillDXArray( pDXAry );
    int nWidthFactor = pSalLayout->GetUnitsPerPixel();
    pSalLayout->Release();

    // convert virtual char widths to virtual absolute positions
    if( pDXAry )
        for( int i = 1; i < nLen; ++i )
            pDXAry[ i ] += pDXAry[ i-1 ];

    // convert from font units to logical units
    if( mbMap )
    {
        if( pDXAry )
            for( int i = 0; i < nLen; ++i )
                pDXAry[i] = ImplDevicePixelToLogicWidth( pDXAry[i] );
        nWidth = ImplDevicePixelToLogicWidth( nWidth );
    }

    if( nWidthFactor > 1 )
    {
        if( pDXAry )
            for( int i = 0; i < nLen; ++i )
                pDXAry[i] /= nWidthFactor;
        nWidth /= nWidthFactor;
    }

    return nWidth;
}
long OutputDevice::GetTextHeight() const
{
    //DBG_TRACE( "OutputDevice::GetTextHeight()" );
    //DBG_CHKTHIS( OutputDevice, ImplDbgCheckOutputDevice );

    if( mbNewFont )
        if( !ImplNewFont() )
            return 0;

    if( mbInitFont )
		ImplInitFont();
#ifdef AVS
        if( !ImplNewFont() )
            return 0;
#endif

    long nHeight = mpFontEntry->mnLineHeight + mnEmphasisAscent + mnEmphasisDescent;

    if ( mbMap )
        nHeight = ImplDevicePixelToLogicHeight( nHeight );

    return nHeight;
}

// -----------------------------------------------------------------------

String OutputDevice::GetNonMnemonicString( const String& rStr, xub_StrLen& rMnemonicPos )
{
    String   aStr    = rStr;
    xub_StrLen  nLen    = aStr.Len();
    xub_StrLen  i       = 0;

    rMnemonicPos = STRING_NOTFOUND;
    while ( i < nLen )
    {
        if ( aStr.GetChar( i ) == '~' )
        {
            if ( aStr.GetChar( i+1 ) != '~' )
            {
                if ( rMnemonicPos == STRING_NOTFOUND )
                    rMnemonicPos = i;
                aStr.Erase( i, 1 );
                nLen--;
            }
            else
            {
                aStr.Erase( i, 1 );
                nLen--;
                i++;
            }
        }
        else
            i++;
    }

    return aStr;
}
void OutputDevice::DrawText( const Point& rStartPt, const XubString& rStr,
                                  xub_StrLen nIndex , xub_StrLen nLen ,
                                  MetricVector* pVector, String* pDisplayText )
{
    if( pVector )
    {
        Region aClip( GetClipRegion() );
        if( meOutDevType == OUTDEV_WINDOW )
            aClip.Intersect( Rectangle( Point(), GetOutputSize() ) );
        if( ! aClip.IsNull() )
        {
            MetricVector aTmp;
            GetGlyphBoundRects( rStartPt, rStr, nIndex, nLen, nIndex, aTmp );

            bool bInserted = false;
            for( MetricVector::const_iterator it = aTmp.begin(); it != aTmp.end(); ++it, nIndex++ )
            {
                bool bAppend = false;

                if( aClip.IsOver( *it ) )
                    bAppend = true;
                else if( rStr.GetChar( nIndex ) == ' ' && bInserted )
                {
                    MetricVector::const_iterator next = it;
                    ++next;
                    if( next != aTmp.end() && aClip.IsOver( *next ) )
                        bAppend = true;
                }

                if( bAppend )
                {
                    pVector->push_back( *it );
                    if( pDisplayText )
                        pDisplayText->Append( rStr.GetChar( nIndex ) );
                    bInserted = true;
                }
            }
        }
        else
        {
            GetGlyphBoundRects( rStartPt, rStr, nIndex, nLen, nIndex, *pVector );
            if( pDisplayText )
                pDisplayText->Append( rStr.Copy( nIndex, nLen ) );
        }
    }

    if ( pVector )
        return;

    SalLayout* pSalLayout = ImplLayout( rStr, nIndex, nLen, rStartPt, 0, NULL, true );
    if( pSalLayout )
    {
        ImplDrawText( *pSalLayout );
        pSalLayout->Release();
	}
}
// -----------------------------------------------------------------------

void OutputDevice::ImplDrawText( SalLayout& rSalLayout )
{
    if( mbInitClipRegion )
        ImplInitClipRegion();
    if( mbOutputClipped )
        return;
    if( mbInitTextColor )
        ImplInitTextColor();

    rSalLayout.DrawBase() += Point( mnTextOffX, mnTextOffY );

    if( IsTextFillColor() )
        ImplDrawTextBackground( rSalLayout );

    if( mbTextSpecial )
        ImplDrawSpecialText( rSalLayout );
    else
        ImplDrawTextDirect( rSalLayout, mbTextLines );
}
// -----------------------------------------------------------------------

void OutputDevice::ImplDrawTextBackground( const SalLayout& rSalLayout )
{
    const long nWidth = rSalLayout.GetTextWidth() / rSalLayout.GetUnitsPerPixel();
    const Point aBase = rSalLayout.DrawBase();
    const long nX = aBase.X();
    const long nY = aBase.Y();

    if ( mbLineColor || mbInitLineColor )
    {
        mpGraphics->SetLineColor();
        mbInitLineColor = TRUE;
    }
    mpGraphics->SetFillColor( ImplColorToSal( GetTextFillColor() ) );
    mbInitFillColor = TRUE;

    ImplDrawTextRect( nX, nY, nX, nY-mpFontEntry->maMetric.mnAscent-mnEmphasisAscent,
                      nWidth,
                      mpFontEntry->mnLineHeight+mnEmphasisAscent+mnEmphasisDescent );
}
// -----------------------------------------------------------------------

void OutputDevice::ImplDrawTextDirect( SalLayout& rSalLayout, BOOL bTextLines )
{

    if( mpFontEntry->mnOwnOrientation )
        if( ImplDrawRotateText( rSalLayout ) )
            return;

    long nOldX = rSalLayout.DrawBase().X();
#ifdef AVS
    if( ! (mpPDFWriter && mpPDFWriter->isBuiltinFont(mpFontEntry->maFontSelData.mpFontData) ) )
    {

        if( ImplHasMirroredGraphics() )
        {
            long w = meOutDevType == OUTDEV_VIRDEV ? mnOutWidth : mpGraphics->GetGraphicsWidth();
            long x = rSalLayout.DrawBase().X();
       		rSalLayout.DrawBase().X() = w - 1 - x;
            if( !IsRTLEnabled() )
            {
                OutputDevice *pOutDevRef = (OutputDevice *)this;
                // mirror this window back
                long devX = w-pOutDevRef->mnOutWidth-pOutDevRef->mnOutOffX;   // re-mirrored mnOutOffX
                rSalLayout.DrawBase().X() = devX + ( pOutDevRef->mnOutWidth - 1 - (rSalLayout.DrawBase().X() - devX) ) ;
            }
        }
        else if( IsRTLEnabled() )
        {
            //long w = meOutDevType == OUTDEV_VIRDEV ? mnOutWidth : mpGraphics->GetGraphicsWidth();
            //long x = rSalLayout.DrawBase().X();
            OutputDevice *pOutDevRef = (OutputDevice *)this;
            // mirror this window back
            long devX = pOutDevRef->mnOutOffX;   // re-mirrored mnOutOffX
            rSalLayout.DrawBase().X() = pOutDevRef->mnOutWidth - 1 - (rSalLayout.DrawBase().X() - devX) + devX;
        }
#endif
        rSalLayout.DrawText( *mpGraphics );
#ifdef AVS
    }
#endif
    rSalLayout.DrawBase().X() = nOldX;

    if( bTextLines )
        ImplDrawTextLines( rSalLayout,
            maFont.GetStrikeout(), maFont.GetUnderline(), maFont.GetOverline(),
            maFont.IsWordLineMode(), ImplIsUnderlineAbove( maFont ) );
#ifdef QWE
    // emphasis marks
    if( maFont.GetEmphasisMark() & EMPHASISMARK_STYLE )
        ImplDrawEmphasisMarks( rSalLayout );
#endif

}
// -----------------------------------------------------------------------

void OutputDevice::ImplDrawEmphasisMarks( SalLayout& rSalLayout )
{
#ifdef QWE
    Color               aOldColor       = GetTextColor();
    Color               aOldLineColor   = GetLineColor();
    Color               aOldFillColor   = GetFillColor();
    BOOL                bOldMap         = mbMap;
    //GDIMetaFile*        pOldMetaFile    = mpMetaFile;
    //mpMetaFile = NULL;
    EnableMapMode( FALSE );

    FontEmphasisMark    nEmphasisMark = ImplGetEmphasisMarkStyle( maFont );
    PolyPolygon         aPolyPoly;
    Rectangle           aRect1;
    Rectangle           aRect2;
    long                nEmphasisYOff;
    long                nEmphasisWidth;
    long                nEmphasisHeight;
    BOOL                bPolyLine;

    if ( nEmphasisMark & EMPHASISMARK_POS_BELOW )
        nEmphasisHeight = mnEmphasisDescent;
    else
        nEmphasisHeight = mnEmphasisAscent;

    ImplGetEmphasisMark( aPolyPoly, bPolyLine,
                         aRect1, aRect2,
                         nEmphasisYOff, nEmphasisWidth,
                         nEmphasisMark,
                         nEmphasisHeight, mpFontEntry->mnOrientation );

    if ( bPolyLine )
    {
        SetLineColor( GetTextColor() );
        SetFillColor();
    }
    else
    {
        SetLineColor();
        SetFillColor( GetTextColor() );
    }

    Point aOffset = Point(0,0);

    if ( nEmphasisMark & EMPHASISMARK_POS_BELOW )
        aOffset.Y() += mpFontEntry->maMetric.mnDescent + nEmphasisYOff;
    else
        aOffset.Y() -= mpFontEntry->maMetric.mnAscent + nEmphasisYOff;

    long nEmphasisWidth2  = nEmphasisWidth / 2;
    long nEmphasisHeight2 = nEmphasisHeight / 2;
    aOffset += Point( nEmphasisWidth2, nEmphasisHeight2 );

    Point aOutPoint;
    Rectangle aRectangle;
    for( int nStart = 0;;)
    {
        sal_GlyphId nGlyphIndex;
        if( !rSalLayout.GetNextGlyphs( 1, &nGlyphIndex, aOutPoint, nStart ) )
            break;

        if( !mpGraphics->GetGlyphBoundRect( nGlyphIndex, aRectangle ) )
            continue;

        if( !rSalLayout.IsSpacingGlyph( nGlyphIndex ) )
        {
            Point aAdjPoint = aOffset;
            aAdjPoint.X() += aRectangle.Left() + (aRectangle.GetWidth() - nEmphasisWidth) / 2;
            if ( mpFontEntry->mnOrientation )
                ImplRotatePos( 0, 0, aAdjPoint.X(), aAdjPoint.Y(), mpFontEntry->mnOrientation );
            aOutPoint += aAdjPoint;
            aOutPoint -= Point( nEmphasisWidth2, nEmphasisHeight2 );
            ImplDrawEmphasisMark( rSalLayout.DrawBase().X(),
                                  aOutPoint.X(), aOutPoint.Y(),
                                  aPolyPoly, bPolyLine, aRect1, aRect2 );
        }
    }

    SetLineColor( aOldLineColor );
    SetFillColor( aOldFillColor );
    EnableMapMode( bOldMap );
    //mpMetaFile = pOldMetaFile;
#endif
}
// -----------------------------------------------------------------------

void OutputDevice::ImplDrawTextLines( SalLayout& rSalLayout,
    FontStrikeout eStrikeout, FontUnderline eUnderline, FontUnderline eOverline, BOOL bWordLine, BOOL bUnderlineAbove )
{
    if( bWordLine )
    {
        Point aPos, aStartPt;
        sal_Int32 nWidth = 0, nAdvance=0;
        for( int nStart = 0;;)
        {
            sal_GlyphId nGlyphIndex;
            if( !rSalLayout.GetNextGlyphs( 1, &nGlyphIndex, aPos, nStart, &nAdvance ) )
                break;

            if( !rSalLayout.IsSpacingGlyph( nGlyphIndex ) )
            {
                if( !nWidth )
                {
                    aStartPt = aPos;//rSalLayout.DrawBase() - (aPos - rSalLayout.DrawOffset());
                }

                nWidth += nAdvance;
            }
            else if( nWidth > 0 )
            {
                ImplDrawTextLine( rSalLayout.DrawBase().X(), aStartPt.X(), aStartPt.Y(), nWidth,
                    eStrikeout, eUnderline, eOverline, bUnderlineAbove );
                nWidth = 0;
            }
        }

        if( nWidth > 0 )
        {
            ImplDrawTextLine( rSalLayout.DrawBase().X(), aStartPt.X(), aStartPt.Y(), nWidth,
                eStrikeout, eUnderline, eOverline, bUnderlineAbove );
        }
    }
    else
    {
        Point aStartPt = rSalLayout.GetDrawPosition();
        int nWidth = rSalLayout.GetTextWidth() / rSalLayout.GetUnitsPerPixel();
        ImplDrawTextLine( rSalLayout.DrawBase().X(), aStartPt.X(), aStartPt.Y(), nWidth,
            eStrikeout, eUnderline, eOverline, bUnderlineAbove );
    }
}
// -----------------------------------------------------------------------

void OutputDevice::ImplDrawTextLine( long nBaseX,
                                     long nX, long nY, long nWidth,
                                     FontStrikeout eStrikeout,
                                     FontUnderline eUnderline,
                                     FontUnderline eOverline,
                                     BOOL bUnderlineAbove )
{
    if ( !nWidth )
        return;

    Color           aStrikeoutColor = GetTextColor();
    Color           aUnderlineColor = GetTextLineColor();
    Color           aOverlineColor  = GetOverlineColor();
    BOOL            bStrikeoutDone = FALSE;
    BOOL            bUnderlineDone = FALSE;
    BOOL            bOverlineDone  = FALSE;

    // TODO: fix rotated text
    if ( IsRTLEnabled() )
        // --- RTL --- mirror at basex
        nX = nBaseX - nWidth - (nX - nBaseX - 1);

    if ( !IsTextLineColor() )
        aUnderlineColor = GetTextColor();

    if ( !IsOverlineColor() )
        aOverlineColor = GetTextColor();

    if ( (eUnderline == UNDERLINE_SMALLWAVE) ||
         (eUnderline == UNDERLINE_WAVE) ||
         (eUnderline == UNDERLINE_DOUBLEWAVE) ||
         (eUnderline == UNDERLINE_BOLDWAVE) )
    {
        ImplDrawWaveTextLine( nBaseX, nY, nX, nY, nWidth, eUnderline, aUnderlineColor, bUnderlineAbove );
        bUnderlineDone = TRUE;
    }
    if ( (eOverline == UNDERLINE_SMALLWAVE) ||
         (eOverline == UNDERLINE_WAVE) ||
         (eOverline == UNDERLINE_DOUBLEWAVE) ||
         (eOverline == UNDERLINE_BOLDWAVE) )
    {
        ImplDrawWaveTextLine( nBaseX, nY, nX, nY, nWidth, eOverline, aOverlineColor, TRUE );
        bOverlineDone = TRUE;
    }

    if ( (eStrikeout == STRIKEOUT_SLASH) ||
         (eStrikeout == STRIKEOUT_X) )
    {
        ImplDrawStrikeoutChar( nBaseX, nY, nX, nY, nWidth, eStrikeout, aStrikeoutColor );
        bStrikeoutDone = TRUE;
    }

    if ( !bUnderlineDone )
        ImplDrawStraightTextLine( nBaseX, nY, nX, nY, nWidth, eUnderline, aUnderlineColor, bUnderlineAbove );

    if ( !bOverlineDone )
        ImplDrawStraightTextLine( nBaseX, nY, nX, nY, nWidth, eOverline, aOverlineColor, TRUE );

    if ( !bStrikeoutDone )
        ImplDrawStrikeoutLine( nBaseX, nY, nX, nY, nWidth, eStrikeout, aStrikeoutColor );
}
// -----------------------------------------------------------------------

void OutputDevice::ImplDrawStraightTextLine( long nBaseX, long nBaseY,
                                             long nX, long nY, long nWidth,
                                             FontUnderline eTextLine,
                                             Color aColor,
                                             BOOL bIsAbove )
{
    ImplFontEntry*  pFontEntry = mpFontEntry;
    long            nLineHeight = 0;
    long            nLinePos  = 0;
    long            nLinePos2 = 0;

    if ( eTextLine > UNDERLINE_LAST )
        eTextLine = UNDERLINE_SINGLE;

    switch ( eTextLine )
    {
        case UNDERLINE_SINGLE:
        case UNDERLINE_DOTTED:
        case UNDERLINE_DASH:
        case UNDERLINE_LONGDASH:
        case UNDERLINE_DASHDOT:
        case UNDERLINE_DASHDOTDOT:
            if ( bIsAbove )
            {
                nLineHeight = pFontEntry->maMetric.mnAboveUnderlineSize;
                nLinePos    = nY + pFontEntry->maMetric.mnAboveUnderlineOffset;
            }
            else
            {
                nLineHeight = pFontEntry->maMetric.mnUnderlineSize;
                nLinePos    = nY + pFontEntry->maMetric.mnUnderlineOffset;
            }
            break;
        case UNDERLINE_BOLD:
        case UNDERLINE_BOLDDOTTED:
        case UNDERLINE_BOLDDASH:
        case UNDERLINE_BOLDLONGDASH:
        case UNDERLINE_BOLDDASHDOT:
        case UNDERLINE_BOLDDASHDOTDOT:
            if ( bIsAbove )
            {
                nLineHeight = pFontEntry->maMetric.mnAboveBUnderlineSize;
                nLinePos    = nY + pFontEntry->maMetric.mnAboveBUnderlineOffset;
            }
            else
            {
                nLineHeight = pFontEntry->maMetric.mnBUnderlineSize;
                nLinePos    = nY + pFontEntry->maMetric.mnBUnderlineOffset;
            }
            break;
        case UNDERLINE_DOUBLE:
            if ( bIsAbove )
            {
                nLineHeight = pFontEntry->maMetric.mnAboveDUnderlineSize;
                nLinePos    = nY + pFontEntry->maMetric.mnAboveDUnderlineOffset1;
                nLinePos2   = nY + pFontEntry->maMetric.mnAboveDUnderlineOffset2;
            }
            else
            {
                nLineHeight = pFontEntry->maMetric.mnDUnderlineSize;
                nLinePos    = nY + pFontEntry->maMetric.mnDUnderlineOffset1;
                nLinePos2   = nY + pFontEntry->maMetric.mnDUnderlineOffset2;
            }
            break;
        default:
            break;
    }

    if ( nLineHeight )
    {
        if ( mbLineColor || mbInitLineColor )
        {
            mpGraphics->SetLineColor();
            mbInitLineColor = TRUE;
        }
        mpGraphics->SetFillColor( ImplColorToSal( aColor ) );
        mbInitFillColor = TRUE;

        long nLeft = nX;

        switch ( eTextLine )
        {
            case UNDERLINE_SINGLE:
            case UNDERLINE_BOLD:
                ImplDrawTextRect( nBaseX, nBaseY, nLeft, nLinePos, nWidth, nLineHeight );
                break;
            case UNDERLINE_DOUBLE:
                ImplDrawTextRect( nBaseX, nBaseY, nLeft, nLinePos,  nWidth, nLineHeight );
                ImplDrawTextRect( nBaseX, nBaseY, nLeft, nLinePos2, nWidth, nLineHeight );
                break;
            case UNDERLINE_DOTTED:
            case UNDERLINE_BOLDDOTTED:
                {
                    long nDotWidth = nLineHeight*mnDPIY;
                    nDotWidth += mnDPIY/2;
                    nDotWidth /= mnDPIY;
                    long nTempWidth = nDotWidth;
                    long nEnd = nLeft+nWidth;
                    while ( nLeft < nEnd )
                    {
                        if ( nLeft+nTempWidth > nEnd )
                            nTempWidth = nEnd-nLeft;
                        ImplDrawTextRect( nBaseX, nBaseY, nLeft, nLinePos, nTempWidth, nLineHeight );
                        nLeft += nDotWidth*2;
                    }
                }
                break;
            case UNDERLINE_DASH:
            case UNDERLINE_LONGDASH:
            case UNDERLINE_BOLDDASH:
            case UNDERLINE_BOLDLONGDASH:
                {
                    long nDotWidth = nLineHeight*mnDPIY;
                    nDotWidth += mnDPIY/2;
                    nDotWidth /= mnDPIY;
                    long nMinDashWidth;
                    long nMinSpaceWidth;
                    long nSpaceWidth;
                    long nDashWidth;
                    if ( (eTextLine == UNDERLINE_LONGDASH) ||
                         (eTextLine == UNDERLINE_BOLDLONGDASH) )
                    {
                        nMinDashWidth = nDotWidth*6;
                        nMinSpaceWidth = nDotWidth*2;
                        nDashWidth = 200;
                        nSpaceWidth = 100;
                    }
                    else
                    {
                        nMinDashWidth = nDotWidth*4;
                        nMinSpaceWidth = (nDotWidth*150)/100;
                        nDashWidth = 100;
                        nSpaceWidth = 50;
                    }
                    nDashWidth = ((nDashWidth*mnDPIX)+1270)/2540;
                    nSpaceWidth = ((nSpaceWidth*mnDPIX)+1270)/2540;
                    // DashWidth wird gegebenenfalls verbreitert, wenn
                    // die dicke der Linie im Verhaeltnis zur Laenge
                    // zu dick wird
                    if ( nDashWidth < nMinDashWidth )
                        nDashWidth = nMinDashWidth;
                    if ( nSpaceWidth < nMinSpaceWidth )
                        nSpaceWidth = nMinSpaceWidth;
                    long nTempWidth = nDashWidth;
                    long nEnd = nLeft+nWidth;
                    while ( nLeft < nEnd )
                    {
                        if ( nLeft+nTempWidth > nEnd )
                            nTempWidth = nEnd-nLeft;
                        ImplDrawTextRect( nBaseX, nBaseY, nLeft, nLinePos, nTempWidth, nLineHeight );
                        nLeft += nDashWidth+nSpaceWidth;
                    }
                }
                break;
            case UNDERLINE_DASHDOT:
            case UNDERLINE_BOLDDASHDOT:
                {
                    long nDotWidth = nLineHeight*mnDPIY;
                    nDotWidth += mnDPIY/2;
                    nDotWidth /= mnDPIY;
                    long nDashWidth = ((100*mnDPIX)+1270)/2540;
                    long nMinDashWidth = nDotWidth*4;
                    // DashWidth wird gegebenenfalls verbreitert, wenn
                    // die dicke der Linie im Verhaeltnis zur Laenge
                    // zu dick wird
                    if ( nDashWidth < nMinDashWidth )
                        nDashWidth = nMinDashWidth;
                    long nTempDotWidth = nDotWidth;
                    long nTempDashWidth = nDashWidth;
                    long nEnd = nLeft+nWidth;
                    while ( nLeft < nEnd )
                    {
                        if ( nLeft+nTempDotWidth > nEnd )
                            nTempDotWidth = nEnd-nLeft;
                        ImplDrawTextRect( nBaseX, nBaseY, nLeft, nLinePos, nTempDotWidth, nLineHeight );
                        nLeft += nDotWidth*2;
                        if ( nLeft > nEnd )
                            break;
                        if ( nLeft+nTempDashWidth > nEnd )
                            nTempDashWidth = nEnd-nLeft;
                        ImplDrawTextRect( nBaseX, nBaseY, nLeft, nLinePos, nTempDashWidth, nLineHeight );
                        nLeft += nDashWidth+nDotWidth;
                    }
                }
                break;
            case UNDERLINE_DASHDOTDOT:
            case UNDERLINE_BOLDDASHDOTDOT:
                {
                    long nDotWidth = nLineHeight*mnDPIY;
                    nDotWidth += mnDPIY/2;
                    nDotWidth /= mnDPIY;
                    long nDashWidth = ((100*mnDPIX)+1270)/2540;
                    long nMinDashWidth = nDotWidth*4;
                    // DashWidth wird gegebenenfalls verbreitert, wenn
                    // die dicke der Linie im Verhaeltnis zur Laenge
                    // zu dick wird
                    if ( nDashWidth < nMinDashWidth )
                        nDashWidth = nMinDashWidth;
                    long nTempDotWidth = nDotWidth;
                    long nTempDashWidth = nDashWidth;
                    long nEnd = nLeft+nWidth;
                    while ( nLeft < nEnd )
                    {
                        if ( nLeft+nTempDotWidth > nEnd )
                            nTempDotWidth = nEnd-nLeft;
                        ImplDrawTextRect( nBaseX, nBaseY, nLeft, nLinePos, nTempDotWidth, nLineHeight );
                        nLeft += nDotWidth*2;
                        if ( nLeft > nEnd )
                            break;
                        if ( nLeft+nTempDotWidth > nEnd )
                            nTempDotWidth = nEnd-nLeft;
                        ImplDrawTextRect( nBaseX, nBaseY, nLeft, nLinePos, nTempDotWidth, nLineHeight );
                        nLeft += nDotWidth*2;
                        if ( nLeft > nEnd )
                            break;
                        if ( nLeft+nTempDashWidth > nEnd )
                            nTempDashWidth = nEnd-nLeft;
                        ImplDrawTextRect( nBaseX, nBaseY, nLeft, nLinePos, nTempDashWidth, nLineHeight );
                        nLeft += nDashWidth+nDotWidth;
                    }
                }
                break;
            default:
                break;
        }
    }
}

// -----------------------------------------------------------------------

void OutputDevice::ImplDrawStrikeoutChar( long nBaseX, long nBaseY,
                                          long nX, long nY, long nWidth,
                                          FontStrikeout eStrikeout,
                                          Color aColor )
{
    BOOL bOldMap = IsMapModeEnabled();
    EnableMapMode( FALSE );

    Color aOldColor = GetTextColor();
    SetTextColor( aColor );
    ImplInitTextColor();

    xub_Unicode pChars[5];
    if ( eStrikeout == STRIKEOUT_SLASH )
        pChars[0] = '/';
    else // ( eStrikeout == STRIKEOUT_X )
        pChars[0] = 'X';
    pChars[3]=pChars[2]=pChars[1]=pChars[0];

    // calculate approximation of strikeout atom size
    long nStrikeoutWidth = nWidth;
    String aStrikeoutTest( pChars, 4 );
    SalLayout* pLayout = ImplLayout( aStrikeoutTest, 0, 4 );
    if ( pLayout )
    {
        nStrikeoutWidth = (pLayout->GetTextWidth() + 2) / 4;
        pLayout->Release();
        if ( nStrikeoutWidth <= 0 ) // sanity check
            nStrikeoutWidth = 1;
    }

    // calculate acceptable strikeout length
    // allow the strikeout to be one pixel larger than the text it strikes out
    long nMaxWidth = nStrikeoutWidth/2;
    if ( nMaxWidth < 2 )
        nMaxWidth = 2;
    nMaxWidth += nWidth + 1;

    // build strikeout string
    long nFullStrikeoutWidth = 0;
    String aStrikeoutText( pChars, 0 );
    while ( (nFullStrikeoutWidth+=nStrikeoutWidth) < nMaxWidth+1 )
        aStrikeoutText += pChars[0];

    // if the text width is smaller than the strikeout text, then do not
    // strike out at all. This case requires user interaction, e.g. adding
    // a space to the text
    if ( (aStrikeoutText.Len() > 0) )
#ifdef AVS
    && !(mpPDFWriter && mpPDFWriter->isBuiltinFont(mpFontEntry->maFontSelData.mpFontData) ) )
#endif
    {
        if ( mpFontEntry->mnOrientation )
            ImplRotatePos( nBaseX, nBaseY, nX, nY, mpFontEntry->mnOrientation );

        // strikeout text has to be left aligned
        ULONG nOrigTLM = mnTextLayoutMode;
        mnTextLayoutMode = TEXT_LAYOUT_BIDI_STRONG | TEXT_LAYOUT_COMPLEX_DISABLED;
        SalLayout* pSalLayout = ImplLayout( aStrikeoutText, 0, STRING_LEN );
        mnTextLayoutMode = nOrigTLM;

        if ( pSalLayout )
        {
            pSalLayout->DrawBase() = Point( nX+mnTextOffX, nY+mnTextOffY );
            pSalLayout->DrawText( *mpGraphics );
            pSalLayout->Release();
        }
    }

    SetTextColor( aOldColor );
    ImplInitTextColor();
    EnableMapMode( bOldMap );
}

// -----------------------------------------------------------------------

void OutputDevice::ImplDrawStrikeoutLine( long nBaseX, long nBaseY,
                                          long nX, long nY, long nWidth,
                                          FontStrikeout eStrikeout,
                                          Color aColor )
{
    ImplFontEntry*  pFontEntry = mpFontEntry;
    long            nLineHeight = 0;
    long            nLinePos  = 0;
    long            nLinePos2 = 0;

    if ( eStrikeout > STRIKEOUT_LAST )
        eStrikeout = STRIKEOUT_SINGLE;

    switch ( eStrikeout )
    {
        case STRIKEOUT_SINGLE:
            nLineHeight = pFontEntry->maMetric.mnStrikeoutSize;
            nLinePos    = nY + pFontEntry->maMetric.mnStrikeoutOffset;
            break;
        case STRIKEOUT_BOLD:
            nLineHeight = pFontEntry->maMetric.mnBStrikeoutSize;
            nLinePos    = nY + pFontEntry->maMetric.mnBStrikeoutOffset;
            break;
        case STRIKEOUT_DOUBLE:
            nLineHeight = pFontEntry->maMetric.mnDStrikeoutSize;
            nLinePos    = nY + pFontEntry->maMetric.mnDStrikeoutOffset1;
            nLinePos2   = nY + pFontEntry->maMetric.mnDStrikeoutOffset2;
            break;
        default:
            break;
    }

    if ( nLineHeight )
    {
        if ( mbLineColor || mbInitLineColor )
        {
            mpGraphics->SetLineColor();
            mbInitLineColor = TRUE;
        }
        mpGraphics->SetFillColor( ImplColorToSal( aColor ) );
        mbInitFillColor = TRUE;

        long nLeft = nX;

        switch ( eStrikeout )
        {
            case STRIKEOUT_SINGLE:
            case STRIKEOUT_BOLD:
                ImplDrawTextRect( nBaseX, nBaseY, nLeft, nLinePos, nWidth, nLineHeight );
                break;
            case STRIKEOUT_DOUBLE:
                ImplDrawTextRect( nBaseX, nBaseY, nLeft, nLinePos, nWidth, nLineHeight );
                ImplDrawTextRect( nBaseX, nBaseY, nLeft, nLinePos2, nWidth, nLineHeight );
                break;
            default:
                break;
        }
    }
}
// -----------------------------------------------------------------------

void OutputDevice::ImplDrawWaveTextLine( long nBaseX, long nBaseY,
                                         long nX, long nY, long nWidth,
                                         FontUnderline eTextLine,
                                         Color aColor,
                                         BOOL bIsAbove )
{
    ImplFontEntry*  pFontEntry = mpFontEntry;
    long            nLineHeight;
    long            nLinePos;

    if ( bIsAbove )
    {
        nLineHeight = pFontEntry->maMetric.mnAboveWUnderlineSize;
        nLinePos = pFontEntry->maMetric.mnAboveWUnderlineOffset;
    }
    else
    {
        nLineHeight = pFontEntry->maMetric.mnWUnderlineSize;
        nLinePos = pFontEntry->maMetric.mnWUnderlineOffset;
    }
    if ( (eTextLine == UNDERLINE_SMALLWAVE) && (nLineHeight > 3) )
        nLineHeight = 3;
    long nLineWidth = (mnDPIX/300);
    if ( !nLineWidth )
        nLineWidth = 1;
    if ( eTextLine == UNDERLINE_BOLDWAVE )
        nLineWidth *= 2;
    nLinePos += nY - (nLineHeight / 2);
    long nLineWidthHeight = ((nLineWidth*mnDPIX)+(mnDPIY/2))/mnDPIY;
    if ( eTextLine == UNDERLINE_DOUBLEWAVE )
    {
        long nOrgLineHeight = nLineHeight;
        nLineHeight /= 3;
        if ( nLineHeight < 2 )
        {
            if ( nOrgLineHeight > 1 )
                nLineHeight = 2;
            else
                nLineHeight = 1;
        }
        long nLineDY = nOrgLineHeight-(nLineHeight*2);
        if ( nLineDY < nLineWidthHeight )
            nLineDY = nLineWidthHeight;
        long nLineDY2 = nLineDY/2;
        if ( !nLineDY2 )
            nLineDY2 = 1;

        nLinePos -= nLineWidthHeight-nLineDY2;
        ImplDrawWaveLine( nBaseX, nBaseY, nX, nLinePos, nWidth, nLineHeight,
                          nLineWidth, mpFontEntry->mnOrientation, aColor );
        nLinePos += nLineWidthHeight+nLineDY;
        ImplDrawWaveLine( nBaseX, nBaseY, nX, nLinePos, nWidth, nLineHeight,
                          nLineWidth, mpFontEntry->mnOrientation, aColor );
    }
    else
    {
        nLinePos -= nLineWidthHeight/2;
        ImplDrawWaveLine( nBaseX, nBaseY, nX, nLinePos, nWidth, nLineHeight,
                          nLineWidth, mpFontEntry->mnOrientation, aColor );
    }
}
// -----------------------------------------------------------------------

static void ImplDrawWavePixel( long nOriginX, long nOriginY,
                               long nCurX, long nCurY,
                               short nOrientation,
                               SalGraphics* pGraphics,
                               OutputDevice* pOutDev,
                               BOOL bDrawPixAsRect,

                               long nPixWidth, long nPixHeight )
{
    if ( nOrientation )
        ImplRotatePos( nOriginX, nOriginY, nCurX, nCurY, nOrientation );

    if ( bDrawPixAsRect )
    {

        pGraphics->DrawRect( nCurX, nCurY, nPixWidth, nPixHeight, pOutDev );
    }
    else
    {
        pGraphics->DrawPixel( nCurX, nCurY, pOutDev );
    }
}
// -----------------------------------------------------------------------

void OutputDevice::ImplDrawWaveLine( long nBaseX, long nBaseY,
                                     long nStartX, long nStartY,
                                     long nWidth, long nHeight,
                                     long nLineWidth, short nOrientation,
                                     const Color& rColor )
{
    if ( !nHeight )
        return;

    // Bei Hoehe von 1 Pixel reicht es, eine Linie auszugeben
    if ( (nLineWidth == 1) && (nHeight == 1) )
    {
        mpGraphics->SetLineColor( ImplColorToSal( rColor ) );
        mbInitLineColor = TRUE;

        long nEndX = nStartX+nWidth;
        long nEndY = nStartY;
        if ( nOrientation )
        {
            ImplRotatePos( nBaseX, nBaseY, nStartX, nStartY, nOrientation );
            ImplRotatePos( nBaseX, nBaseY, nEndX, nEndY, nOrientation );
        }
        mpGraphics->DrawLine( nStartX, nStartY, nEndX, nEndY, this );

    }
    else
    {
        long    nCurX = nStartX;
        long    nCurY = nStartY;
        long    nDiffX = 2;
        long    nDiffY = nHeight-1;
        long    nCount = nWidth;
        long    nOffY = -1;
        long    nFreq;
        long    i;
        long    nPixWidth;
        long    nPixHeight;
        BOOL    bDrawPixAsRect;
        // Auf Druckern die Pixel per DrawRect() ausgeben
        if ( (GetOutDevType() == OUTDEV_PRINTER) || (nLineWidth > 1) )
        {
            if ( mbLineColor || mbInitLineColor )
            {
                mpGraphics->SetLineColor();
                mbInitLineColor = TRUE;
            }
            mpGraphics->SetFillColor( ImplColorToSal( rColor ) );
            mbInitFillColor = TRUE;
            bDrawPixAsRect  = TRUE;
            nPixWidth       = nLineWidth;
            nPixHeight      = ((nLineWidth*mnDPIX)+(mnDPIY/2))/mnDPIY;
        }
        else
        {
            mpGraphics->SetLineColor( ImplColorToSal( rColor ) );
            mbInitLineColor = TRUE;
            nPixWidth       = 1;
            nPixHeight      = 1;
            bDrawPixAsRect  = FALSE;
        }

        if ( !nDiffY )
        {
            while ( nWidth )
            {
                ImplDrawWavePixel( nBaseX, nBaseY, nCurX, nCurY, nOrientation,
                                   mpGraphics, this,
                                   bDrawPixAsRect, nPixWidth, nPixHeight );
                nCurX++;
                nWidth--;
            }
        }
        else
        {
            nCurY += nDiffY;
            nFreq = nCount / (nDiffX+nDiffY);
            while ( nFreq-- )
            {
                for( i = nDiffY; i; --i )
                {
                    ImplDrawWavePixel( nBaseX, nBaseY, nCurX, nCurY, nOrientation,
                                       mpGraphics, this,
                                       bDrawPixAsRect, nPixWidth, nPixHeight );
                    nCurX++;
                    nCurY += nOffY;
                }
                for( i = nDiffX; i; --i )
                {
                    ImplDrawWavePixel( nBaseX, nBaseY, nCurX, nCurY, nOrientation,
                                       mpGraphics, this,
                                       bDrawPixAsRect, nPixWidth, nPixHeight );
                    nCurX++;
                }
                nOffY = -nOffY;
            }
            nFreq = nCount % (nDiffX+nDiffY);
            if ( nFreq )
            {
                for( i = nDiffY; i && nFreq; --i, --nFreq )
                {
                    ImplDrawWavePixel( nBaseX, nBaseY, nCurX, nCurY, nOrientation,
                                       mpGraphics, this,
                                       bDrawPixAsRect, nPixWidth, nPixHeight );
                    nCurX++;
                    nCurY += nOffY;

                }
                for( i = nDiffX; i && nFreq; --i, --nFreq )
                {
                    ImplDrawWavePixel( nBaseX, nBaseY, nCurX, nCurY, nOrientation,
                                       mpGraphics, this,
                                       bDrawPixAsRect, nPixWidth, nPixHeight );
                    nCurX++;
                }
            }
        }

    }
}

// -----------------------------------------------------------------------

BOOL OutputDevice::ImplIsUnderlineAbove( const Font& rFont )
{
    if ( !rFont.IsVertical() )
        return FALSE;

    if( (LANGUAGE_JAPANESE == rFont.GetLanguage())
    ||  (LANGUAGE_JAPANESE == rFont.GetCJKContextLanguage()) )
        // the underline is right for Japanese only
        return TRUE;

    return FALSE;
}
// -----------------------------------------------------------------------

bool OutputDevice::ImplDrawRotateText( SalLayout& rSalLayout )
{
#ifdef QWE
    int nX = rSalLayout.DrawBase().X();
    int nY = rSalLayout.DrawBase().Y();

    Rectangle aBoundRect;
    rSalLayout.DrawBase() = Point( 0, 0 );
    rSalLayout.DrawOffset() = Point( 0, 0 );
    if( !rSalLayout.GetBoundRect( *mpGraphics, aBoundRect ) )
    {
        // guess vertical text extents if GetBoundRect failed
        int nRight = rSalLayout.GetTextWidth();
        int nTop = mpFontEntry->maMetric.mnAscent + mnEmphasisAscent;
        long nHeight = mpFontEntry->mnLineHeight + mnEmphasisAscent + mnEmphasisDescent;
        aBoundRect = Rectangle( 0, -nTop, nRight, nHeight - nTop );
    }
#ifdef AVS
    // cache virtual device for rotation
    if ( !mpOutDevData )
        ImplInitOutDevData();
    if ( !mpOutDevData->mpRotateDev )
        mpOutDevData->mpRotateDev = new VirtualDevice( *this, 1 );
    VirtualDevice* pVDev = mpOutDevData->mpRotateDev;
#endif
    VirtualDevice* pVDev = new VirtualDevice( *this, 1 );
    // size it accordingly
    if( !pVDev->SetOutputSizePixel( aBoundRect.GetSize() ) )
        return false;

    Font aFont( GetFont() );
    aFont.SetOrientation( 0 );
    aFont.SetSize( Size( mpFontEntry->maFontSelData.mnWidth, mpFontEntry->maFontSelData.mnHeight ) );
    pVDev->SetFont( aFont );
    pVDev->SetTextColor( Color( COL_BLACK ) );
    pVDev->SetTextFillColor();
    pVDev->ImplNewFont();
    pVDev->ImplInitFont();
    pVDev->ImplInitTextColor();

    // draw text into upper left corner
    rSalLayout.DrawBase() -= aBoundRect.TopLeft();
    rSalLayout.DrawText( *((OutputDevice*)pVDev)->mpGraphics );

    Bitmap aBmp = pVDev->GetBitmap( Point(), aBoundRect.GetSize() );
    if ( !aBmp || !aBmp.Rotate( mpFontEntry->mnOwnOrientation, COL_WHITE ) )
        return false;

    // calculate rotation offset
    Polygon aPoly( aBoundRect );
    aPoly.Rotate( Point(), mpFontEntry->mnOwnOrientation );
    Point aPoint = aPoly.GetBoundRect().TopLeft();
    aPoint += Point( nX, nY );
#ifdef AVS
    // mask output with text colored bitmap
    GDIMetaFile* pOldMetaFile = mpMetaFile;
#endif
    long nOldOffX = mnOutOffX;
    long nOldOffY = mnOutOffY;
    BOOL bOldMap = mbMap;

    mnOutOffX   = 0L;
    mnOutOffY   = 0L;
#ifdef AVS
    mpMetaFile  = NULL;
#endif
    EnableMapMode( FALSE );

    DrawMask( aPoint, aBmp, GetTextColor() );

    EnableMapMode( bOldMap );
    mnOutOffX   = nOldOffX;
    mnOutOffY   = nOldOffY;
#ifdef AVS
    mpMetaFile  = pOldMetaFile;
#endif
#endif
    return true;
}

// -----------------------------------------------------------------------

void OutputDevice::ImplDrawSpecialText( SalLayout& rSalLayout )
{
#ifdef QWE
    Color       aOldColor           = GetTextColor();
    Color       aOldTextLineColor   = GetTextLineColor();
    Color       aOldOverlineColor   = GetOverlineColor();
    FontRelief  eRelief             = maFont.GetRelief();

    Point aOrigPos = rSalLayout.DrawBase();
    if ( eRelief != RELIEF_NONE )
    {
        Color   aReliefColor( COL_LIGHTGRAY );
        Color   aTextColor( aOldColor );

        Color   aTextLineColor( aOldTextLineColor );
        Color   aOverlineColor( aOldOverlineColor );

        // we don't have a automatic color, so black is always drawn on white
        if ( aTextColor.GetColor() == COL_BLACK )
            aTextColor = Color( COL_WHITE );
        if ( aTextLineColor.GetColor() == COL_BLACK )
            aTextLineColor = Color( COL_WHITE );
        if ( aOverlineColor.GetColor() == COL_BLACK )
            aOverlineColor = Color( COL_WHITE );

        // relief-color is black for white text, in all other cases
        // we set this to LightGray
        if ( aTextColor.GetColor() == COL_WHITE )
            aReliefColor = Color( COL_BLACK );
        SetTextLineColor( aReliefColor );
        SetOverlineColor( aReliefColor );
        SetTextColor( aReliefColor );
        ImplInitTextColor();

        // calculate offset - for high resolution printers the offset
        // should be greater so that the effect is visible
        long nOff = 1;
        nOff += mnDPIX/300;

        if ( eRelief == RELIEF_ENGRAVED )
            nOff = -nOff;
        rSalLayout.DrawOffset() += Point( nOff, nOff);
        ImplDrawTextDirect( rSalLayout, mbTextLines );
        rSalLayout.DrawOffset() -= Point( nOff, nOff);

        SetTextLineColor( aTextLineColor );
        SetOverlineColor( aOverlineColor );
        SetTextColor( aTextColor );
        ImplInitTextColor();
        ImplDrawTextDirect( rSalLayout, mbTextLines );

        SetTextLineColor( aOldTextLineColor );
        SetOverlineColor( aOldOverlineColor );

        if ( aTextColor != aOldColor )
        {
            SetTextColor( aOldColor );
            ImplInitTextColor();
        }
    }
    else
    {
        if ( maFont.IsShadow() )
        {
            long nOff = 1 + ((mpFontEntry->mnLineHeight-24)/24);
            if ( maFont.IsOutline() )
                nOff++;
            SetTextLineColor();
            SetOverlineColor();
            if ( (GetTextColor().GetColor() == COL_BLACK)
            ||   (GetTextColor().GetLuminance() < 8) )
                SetTextColor( Color( COL_LIGHTGRAY ) );
            else
                SetTextColor( Color( COL_BLACK ) );
            ImplInitTextColor();
            rSalLayout.DrawBase() += Point( nOff, nOff );
            ImplDrawTextDirect( rSalLayout, mbTextLines );
            rSalLayout.DrawBase() -= Point( nOff, nOff );
            SetTextColor( aOldColor );
            SetTextLineColor( aOldTextLineColor );
            SetOverlineColor( aOldOverlineColor );
            ImplInitTextColor();

            if ( !maFont.IsOutline() )
                ImplDrawTextDirect( rSalLayout, mbTextLines );
        }

        if ( maFont.IsOutline() )
        {
            rSalLayout.DrawBase() = aOrigPos + Point(-1,-1);
            ImplDrawTextDirect( rSalLayout, mbTextLines );
            rSalLayout.DrawBase() = aOrigPos + Point(+1,+1);
            ImplDrawTextDirect( rSalLayout, mbTextLines );
            rSalLayout.DrawBase() = aOrigPos + Point(-1,+0);
            ImplDrawTextDirect( rSalLayout, mbTextLines );
            rSalLayout.DrawBase() = aOrigPos + Point(-1,+1);
            ImplDrawTextDirect( rSalLayout, mbTextLines );
            rSalLayout.DrawBase() = aOrigPos + Point(+0,+1);
            ImplDrawTextDirect( rSalLayout, mbTextLines );
            rSalLayout.DrawBase() = aOrigPos + Point(+0,-1);
            ImplDrawTextDirect( rSalLayout, mbTextLines );
            rSalLayout.DrawBase() = aOrigPos + Point(+1,-1);
            ImplDrawTextDirect( rSalLayout, mbTextLines );
            rSalLayout.DrawBase() = aOrigPos + Point(+1,+0);
            ImplDrawTextDirect( rSalLayout, mbTextLines );
            rSalLayout.DrawBase() = aOrigPos;

            SetTextColor( Color( COL_WHITE ) );
            SetTextLineColor( Color( COL_WHITE ) );
            SetOverlineColor( Color( COL_WHITE ) );
            ImplInitTextColor();
            ImplDrawTextDirect( rSalLayout, mbTextLines );
            SetTextColor( aOldColor );
            SetTextLineColor( aOldTextLineColor );
            SetOverlineColor( aOldOverlineColor );
            ImplInitTextColor();
        }
    }
#endif
}
// -----------------------------------------------------------------------

void OutputDevice::ImplInitTextColor()
{
    if ( mbInitTextColor )
    {
        mpGraphics->SetTextColor( ImplColorToSal( GetTextColor() ) );
        mbInitTextColor = FALSE;
    }
}

SalLayout* OutputDevice::ImplLayout( const String& rOrigStr,
                                     xub_StrLen nMinIndex,
                                     xub_StrLen nLen,
                                     const Point& rLogicalPos,
                                     long nLogicalWidth,
                                     const sal_Int32* pDXArray,
                                     bool bFilter )  const
{

    // we need a graphics
    if( !mpGraphics )
            return NULL;

    // initialize font if needed
    if( mbNewFont )
        if( !ImplNewFont() )
            return NULL;
    if( mbInitFont )
        ImplInitFont();

    // check string index and length
    String aStr = rOrigStr;
    if( (ULONG)nMinIndex + nLen >= aStr.Len() )
        if( nMinIndex < aStr.Len() )
            nLen = aStr.Len() - nMinIndex;
        else
            return NULL;

    // filter out special markers
    if( bFilter )
    {
        xub_StrLen nCutStart, nCutStop, nOrgLen = nLen;
        bool bFiltered = mpGraphics->filterText( rOrigStr, aStr, nMinIndex, nLen, nCutStart, nCutStop );
        if( !nLen )
            return NULL;

        if( bFiltered && nCutStop != nCutStart && pDXArray )
        {
            if( !nLen )
                pDXArray = NULL;
            else
            {
                sal_Int32* pAry = (sal_Int32*)alloca(sizeof(sal_Int32)*nLen);
                if( nCutStart > nMinIndex )
                    memcpy( pAry, pDXArray, sizeof(sal_Int32)*(nCutStart-nMinIndex) );
                // note: nCutStart will never be smaller than nMinIndex
                memcpy( pAry+nCutStart-nMinIndex,
                        pDXArray + nOrgLen - (nCutStop-nMinIndex),
                        sizeof(sal_Int32)*(nLen - (nCutStart-nMinIndex)) );
                pDXArray = pAry;
            }
        }
    }

    // convert from logical units to physical units
    // recode string if needed
    if( mpFontEntry->mpConversion )
        ImplRecodeString( mpFontEntry->mpConversion, aStr, 0, aStr.Len() );

    long nPixelWidth = nLogicalWidth;
    if( nLogicalWidth && mbMap )
        nPixelWidth = ImplLogicWidthToDevicePixel( nLogicalWidth );
    if( pDXArray && mbMap )
    {
        // convert from logical units to font units using a temporary array
        sal_Int32* pTempDXAry = (sal_Int32*)alloca( nLen * sizeof(sal_Int32) );
        // using base position for better rounding a.k.a. "dancing characters"
        int nPixelXOfs = ImplLogicWidthToDevicePixel( rLogicalPos.X() );
        for( int i = 0; i < nLen; ++i )
            pTempDXAry[i] = ImplLogicWidthToDevicePixel( rLogicalPos.X() + pDXArray[i] ) - nPixelXOfs;

        pDXArray = pTempDXAry;
    }

    ImplLayoutArgs aLayoutArgs = ImplPrepareLayoutArgs( aStr, nMinIndex, nLen, nPixelWidth, pDXArray );

    // get matching layout object for base font
    SalLayout* pSalLayout = NULL;
#ifdef AVS
    if( mpPDFWriter )
        pSalLayout = mpPDFWriter->GetTextLayout( aLayoutArgs, &mpFontEntry->maFontSelData );
#endif
    if( !pSalLayout )
        pSalLayout = mpGraphics->GetTextLayout( aLayoutArgs, 0 );

    // layout text
    if( pSalLayout && !pSalLayout->LayoutText( aLayoutArgs ) )
    {
        pSalLayout->Release();
        pSalLayout = NULL;
    }

    if( !pSalLayout )
        return NULL;
#ifdef QWE
    // do glyph fallback if needed
    // #105768# avoid fallback for very small font sizes
    if( aLayoutArgs.NeedFallback() )
        if( mpFontEntry && (mpFontEntry->maFontSelData.mnHeight >= 3) )
            pSalLayout = ImplGlyphFallbackLayout( pSalLayout, aLayoutArgs );
#endif
    // position, justify, etc. the layout
    pSalLayout->AdjustLayout( aLayoutArgs );
    pSalLayout->DrawBase() = ImplLogicToDevicePixel( rLogicalPos );
    // adjust to right alignment if necessary
    if( aLayoutArgs.mnFlags & SAL_LAYOUT_RIGHT_ALIGN )
    {
        long nRTLOffset;
        if( pDXArray )
            nRTLOffset = pDXArray[ nLen - 1 ];
        else if( nPixelWidth )
            nRTLOffset = nPixelWidth;
        else
            nRTLOffset = pSalLayout->GetTextWidth() / pSalLayout->GetUnitsPerPixel();
        pSalLayout->DrawOffset().X() = 1 - nRTLOffset;
    }

    return pSalLayout;
}
#ifdef QWE
// -----------------------------------------------------------------------

SalLayout* OutputDevice::ImplGlyphFallbackLayout( SalLayout* pSalLayout, ImplLayoutArgs& rLayoutArgs ) const
{
    // prepare multi level glyph fallback
    MultiSalLayout* pMultiSalLayout = NULL;
    ImplLayoutRuns aLayoutRuns = rLayoutArgs.maRuns;
    rLayoutArgs.PrepareFallback();
    rLayoutArgs.mnFlags |= SAL_LAYOUT_FOR_FALLBACK;

#if defined(HDU_DEBUG)
    {
        int nCharPos = -1;
        bool bRTL = false;
        fprintf(stderr,"OD:ImplLayout Glyph Fallback for");
        for( int i=0; i<8 && rLayoutArgs.GetNextPos( &nCharPos, &bRTL); ++i )
            fprintf(stderr," U+%04X", rLayoutArgs.mpStr[ nCharPos ] );
        fprintf(stderr,"\n");
        rLayoutArgs.ResetPos();
    }
#endif
    // get list of unicodes that need glyph fallback
    int nCharPos = -1;
    bool bRTL = false;
    rtl::OUStringBuffer aMissingCodeBuf;
    while( rLayoutArgs.GetNextPos( &nCharPos, &bRTL) )
        aMissingCodeBuf.append( rLayoutArgs.mpStr[ nCharPos ] );
    rLayoutArgs.ResetPos();
    rtl::OUString aMissingCodes = aMissingCodeBuf.makeStringAndClear();

    ImplFontSelectData aFontSelData = mpFontEntry->maFontSelData;
    // when device specific font substitution may have been performed for
    // the originally selected font then make sure that a fallback to that
    // font is performed first
    int nDevSpecificFallback = 0;
    if( mpOutDevData && !mpOutDevData->maDevFontSubst.Empty() )
        nDevSpecificFallback = 1;

    // try if fallback fonts support the missing unicodes
    for( int nFallbackLevel = 1; nFallbackLevel < MAX_FALLBACK; ++nFallbackLevel )
    {
        // find a font family suited for glyph fallback
#ifndef FONTFALLBACK_HOOKS_DISABLED 
        // GetGlyphFallbackFont() needs a valid aFontSelData.mpFontEntry
        // if the system-specific glyph fallback is active
        aFontSelData.mpFontEntry = mpFontEntry; // reset the fontentry to base-level
#endif
        ImplFontEntry* pFallbackFont = mpFontCache->GetGlyphFallbackFont( mpFontList,
            aFontSelData, nFallbackLevel-nDevSpecificFallback, aMissingCodes );
        if( !pFallbackFont )
            break;

        aFontSelData.mpFontEntry = pFallbackFont;
        aFontSelData.mpFontData = pFallbackFont->maFontSelData.mpFontData;
        if( mpFontEntry && nFallbackLevel < MAX_FALLBACK-1)
        {
            // ignore fallback font if it is the same as the original font
            if( mpFontEntry->maFontSelData.mpFontData == aFontSelData.mpFontData )
            {
                mpFontCache->Release( pFallbackFont );
                continue;
            }
        }

#if defined(HDU_DEBUG)
        {
            ByteString aOrigFontName( maFont.GetName(), RTL_TEXTENCODING_UTF8);
            ByteString aFallbackName( aFontSelData.mpFontData->GetFamilyName(),
                RTL_TEXTENCODING_UTF8);
            fprintf(stderr,"\tGlyphFallback[lvl=%d] \"%s\" -> \"%s\" (q=%d)\n",
                nFallbackLevel, aOrigFontName.GetBuffer(), aFallbackName.GetBuffer(),
                aFontSelData.mpFontData->GetQuality());
        }
#endif

        pFallbackFont->mnSetFontFlags = mpGraphics->SetFont( &aFontSelData, nFallbackLevel );

        // create and add glyph fallback layout to multilayout
        rLayoutArgs.ResetPos();
        SalLayout* pFallback = mpGraphics->GetTextLayout( rLayoutArgs, nFallbackLevel );
        if( pFallback )
        {
            if( pFallback->LayoutText( rLayoutArgs ) )
            {
                if( !pMultiSalLayout )
                    pMultiSalLayout = new MultiSalLayout( *pSalLayout );
                pMultiSalLayout->AddFallback( *pFallback,
                    rLayoutArgs.maRuns, aFontSelData.mpFontData );
                if (nFallbackLevel == MAX_FALLBACK-1)
                    pMultiSalLayout->SetInComplete();
            }
            else
            {
                // there is no need for a font that couldn't resolve anything
                pFallback->Release();
            }
        }

        mpFontCache->Release( pFallbackFont );

        // break when this fallback was sufficient
        if( !rLayoutArgs.PrepareFallback() )
            break;
    }

    if( pMultiSalLayout && pMultiSalLayout->LayoutText( rLayoutArgs ) )
        pSalLayout = pMultiSalLayout;

    // restore orig font settings
    pSalLayout->InitFont();
    rLayoutArgs.maRuns = aLayoutRuns;

    return pSalLayout;
}
#endif
// -----------------------------------------------------------------------

ImplLayoutArgs OutputDevice::ImplPrepareLayoutArgs( String& rStr,
                                       xub_StrLen nMinIndex, xub_StrLen nLen,
                                       long nPixelWidth, const sal_Int32* pDXArray ) const
{
    // get string length for calculating extents
    xub_StrLen nEndIndex = rStr.Len();
    if( (ULONG)nMinIndex + nLen < nEndIndex )
        nEndIndex = nMinIndex + nLen;

    // don't bother if there is nothing to do
    if( nEndIndex < nMinIndex )
        nEndIndex = nMinIndex;

    int nLayoutFlags = 0;
    if( mnTextLayoutMode & TEXT_LAYOUT_BIDI_RTL )
        nLayoutFlags |= SAL_LAYOUT_BIDI_RTL;
    if( mnTextLayoutMode & TEXT_LAYOUT_BIDI_STRONG )
        nLayoutFlags |= SAL_LAYOUT_BIDI_STRONG;
    else if( 0 == (mnTextLayoutMode & TEXT_LAYOUT_BIDI_RTL) )
    {
        // disable Bidi if no RTL hint and no RTL codes used
        const xub_Unicode* pStr = rStr.GetBuffer() + nMinIndex;
        const xub_Unicode* pEnd = rStr.GetBuffer() + nEndIndex;
        for( ; pStr < pEnd; ++pStr )
            if( ((*pStr >= 0x0580) && (*pStr < 0x0800))   // middle eastern scripts
            ||  ((*pStr >= 0xFB18) && (*pStr < 0xFE00))   // hebrew + arabic A presentation forms
            ||  ((*pStr >= 0xFE70) && (*pStr < 0xFEFF)) ) // arabic presentation forms B
                break;
        if( pStr >= pEnd )
            nLayoutFlags |= SAL_LAYOUT_BIDI_STRONG;
    }

    if( mbKerning )
        nLayoutFlags |= SAL_LAYOUT_KERNING_PAIRS;
    if( maFont.GetKerning() & KERNING_ASIAN )
        nLayoutFlags |= SAL_LAYOUT_KERNING_ASIAN;
    if( maFont.IsVertical() )
        nLayoutFlags |= SAL_LAYOUT_VERTICAL;

    if( mnTextLayoutMode & TEXT_LAYOUT_ENABLE_LIGATURES )
        nLayoutFlags |= SAL_LAYOUT_ENABLE_LIGATURES;
    else if( mnTextLayoutMode & TEXT_LAYOUT_COMPLEX_DISABLED )
        nLayoutFlags |= SAL_LAYOUT_COMPLEX_DISABLED;
    else
    {
        // disable CTL for non-CTL text
        const sal_Unicode* pStr = rStr.GetBuffer() + nMinIndex;
        const sal_Unicode* pEnd = rStr.GetBuffer() + nEndIndex;
        for( ; pStr < pEnd; ++pStr )
            if( ((*pStr >= 0x0300) && (*pStr < 0x0370))   // diacritical marks
            ||  ((*pStr >= 0x0590) && (*pStr < 0x10A0))   // many CTL scripts
            ||  ((*pStr >= 0x1100) && (*pStr < 0x1200))   // hangul jamo
            ||  ((*pStr >= 0x1700) && (*pStr < 0x1900))   // many CTL scripts
            ||  ((*pStr >= 0xFB1D) && (*pStr < 0xFE00))   // middle east presentation
            ||  ((*pStr >= 0xFE70) && (*pStr < 0xFEFF)) ) // arabic presentation B
                break;
        if( pStr >= pEnd )
            nLayoutFlags |= SAL_LAYOUT_COMPLEX_DISABLED;
    }

    if( meTextLanguage ) //TODO: (mnTextLayoutMode & TEXT_LAYOUT_SUBSTITUTE_DIGITS)
    {
        // disable character localization when no digits used
        const sal_Unicode* pBase = rStr.GetBuffer();
        const sal_Unicode* pStr = pBase + nMinIndex;
        const sal_Unicode* pEnd = pBase + nEndIndex;
        for( ; pStr < pEnd; ++pStr )
        {
            // TODO: are there non-digit localizations?
            if( (*pStr >= '0') && (*pStr <= '9') )
            {
                // translate characters to local preference
                sal_UCS4 cChar = GetLocalizedChar( *pStr, meTextLanguage );
                if( cChar != *pStr )
                    // TODO: are the localized digit surrogates?
                    rStr.SetChar( static_cast<USHORT>(pStr - pBase),
                                 static_cast<sal_Unicode>(cChar) );
            }
        }
    }

    // right align for RTL text, DRAWPOS_REVERSED, RTL window style
    bool bRightAlign = ((mnTextLayoutMode & TEXT_LAYOUT_BIDI_RTL) != 0);
    if( mnTextLayoutMode & TEXT_LAYOUT_TEXTORIGIN_LEFT )
        bRightAlign = false;
    else if ( mnTextLayoutMode & TEXT_LAYOUT_TEXTORIGIN_RIGHT )
        bRightAlign = true;
    // SSA: hack for western office, ie text get right aligned
    //      for debugging purposes of mirrored UI
    //static const char* pEnv = getenv( "SAL_RTL_MIRRORTEXT" );
    bool bRTLWindow = IsRTLEnabled();
    bRightAlign ^= bRTLWindow;
    if( bRightAlign )
        nLayoutFlags |= SAL_LAYOUT_RIGHT_ALIGN;

    // set layout options
    ImplLayoutArgs aLayoutArgs( rStr.GetBuffer(), rStr.Len(), nMinIndex, nEndIndex, nLayoutFlags );

    int nOrientation = mpFontEntry ? mpFontEntry->mnOrientation : 0;
    aLayoutArgs.SetOrientation( nOrientation );

    aLayoutArgs.SetLayoutWidth( nPixelWidth );
    aLayoutArgs.SetDXArray( pDXArray );

    return aLayoutArgs;
}


// =======================================================================

void OutputDevice::ImplInitFont() const
{
    //DBG_TESTSOLARMUTEX();

    if ( mbInitFont )
    {
        if ( meOutDevType != OUTDEV_PRINTER )
        {
            // decide if antialiasing is appropriate
            bool bNonAntialiased = (GetAntialiasing() & ANTIALIASING_DISABLE_TEXT) != 0;
#ifdef AVS
            const StyleSettings& rStyleSettings = GetSettings().GetStyleSettings();
            bNonAntialiased |= ((rStyleSettings.GetDisplayOptions() & DISPLAY_OPTION_AA_DISABLE) != 0);
            bNonAntialiased |= (int(rStyleSettings.GetAntialiasingMinPixelHeight()) > mpFontEntry->maFontSelData.mnHeight);
#endif
            mpFontEntry->maFontSelData.mbNonAntialiased = bNonAntialiased;
        }
#ifdef AVS
        if( !mpPDFWriter || !mpPDFWriter->isBuiltinFont( mpFontEntry->maFontSelData.mpFontData ) )
        {
#endif
            // select font in the device layers
            mpFontEntry->mnSetFontFlags = mpGraphics->SetFont( &(mpFontEntry->maFontSelData), 0 );
#ifdef AVS
        }
#endif
        mbInitFont = false;
    }
}

// -----------------------------------------------------------------------

bool OutputDevice::ImplNewFont() const
{
    //DBG_TESTSOLARMUTEX();
#ifdef AVS
    // get correct font list on the PDF writer if necessary
    if( mpPDFWriter )
    {
        const ImplSVData* pSVData = ImplGetSVData();
        if( mpFontList == pSVData->maGDIData.mpScreenFontList
        ||  mpFontCache == pSVData->maGDIData.mpScreenFontCache )
            const_cast<OutputDevice&>(*this).ImplUpdateFontData( true );
    }
#endif
    if ( !mbNewFont )
        return true;

    // we need a graphics
    if ( !mpGraphics )
        return false;
    SalGraphics* pGraphics = mpGraphics;
    ImplInitFontList();

    // convert to pixel height
    // TODO: replace integer based aSize completely with subpixel accurate type
    float fExactHeight = ImplFloatLogicHeightToDevicePixel( static_cast<float>(maFont.GetHeight()) );
    Size aSize = ImplLogicToDevicePixel( maFont.GetSize() );
    if ( !aSize.Height() )
    {
        // use default pixel height only when logical height is zero
        if ( maFont.GetSize().Height() )
            aSize.Height() = 1;
        else
            aSize.Height() = (12*mnDPIY)/72;
        fExactHeight =  static_cast<float>(aSize.Height());
    }

    // select the default width only when logical width is zero
    if( (0 == aSize.Width()) && (0 != maFont.GetSize().Width()) )
        aSize.Width() = 1;

    // get font entry
    ImplDirectFontSubstitution* pDevSpecificSubst = NULL;
#ifdef AVS
    if( mpOutDevData )
        pDevSpecificSubst = &mpOutDevData->maDevFontSubst;
#endif
    ImplFontEntry* pOldEntry = mpFontEntry;
    mpFontEntry = mpFontCache->GetFontEntry( mpFontList, maFont, aSize, fExactHeight, pDevSpecificSubst );

    if( pOldEntry )
        mpFontCache->Release( pOldEntry );

    ImplFontEntry* pFontEntry = mpFontEntry;
    // mark when lower layers need to get involved
    mbNewFont = FALSE;
    if( pFontEntry != pOldEntry )
        mbInitFont = TRUE;

    // select font when it has not been initialized yet
    if ( !pFontEntry->mbInit )
    {
        ImplInitFont();

        // get metric data from device layers
        if ( pGraphics )
        {
            pFontEntry->mbInit = true;

            pFontEntry->maMetric.mnOrientation  = static_cast<short>(pFontEntry->maFontSelData.mnOrientation);
#ifdef AVS
            if( mpPDFWriter && mpPDFWriter->isBuiltinFont( pFontEntry->maFontSelData.mpFontData ) )
                mpPDFWriter->getFontMetric( &pFontEntry->maFontSelData, &(pFontEntry->maMetric) );
            else
#endif
                pGraphics->GetFontMetric( &(pFontEntry->maMetric) );

            pFontEntry->maMetric.ImplInitTextLineSize( this );
            pFontEntry->maMetric.ImplInitAboveTextLineSize();

            pFontEntry->mnLineHeight = pFontEntry->maMetric.mnAscent + pFontEntry->maMetric.mnDescent;

            if( pFontEntry->maFontSelData.mnOrientation
            && !pFontEntry->maMetric.mnOrientation
            && (meOutDevType != OUTDEV_PRINTER) )
            {
                pFontEntry->mnOwnOrientation = static_cast<short>(pFontEntry->maFontSelData.mnOrientation);
                pFontEntry->mnOrientation = pFontEntry->mnOwnOrientation;
            }
            else
                pFontEntry->mnOrientation = pFontEntry->maMetric.mnOrientation;
        }
    }

    // enable kerning array if requested
    if ( maFont.GetKerning() & KERNING_FONTSPECIFIC )
    {
        // TODO: test if physical font supports kerning and disable if not
        if( pFontEntry->maMetric.mbKernableFont )
            mbKerning = true;
    }
    else
        mbKerning = false;
    if ( maFont.GetKerning() & KERNING_ASIAN )
        mbKerning = true;

    // calculate EmphasisArea
    mnEmphasisAscent = 0;
    mnEmphasisDescent = 0;
    if ( maFont.GetEmphasisMark() & EMPHASISMARK_STYLE )
    {
        FontEmphasisMark    nEmphasisMark = ImplGetEmphasisMarkStyle( maFont );
        long                nEmphasisHeight = (pFontEntry->mnLineHeight*250)/1000;
        if ( nEmphasisHeight < 1 )
            nEmphasisHeight = 1;
        if ( nEmphasisMark & EMPHASISMARK_POS_BELOW )
            mnEmphasisDescent = nEmphasisHeight;
        else
            mnEmphasisAscent = nEmphasisHeight;
    }

    // calculate text offset depending on TextAlignment
    TextAlign eAlign = maFont.GetAlign();
    if ( eAlign == ALIGN_BASELINE )
    {
        mnTextOffX = 0;
        mnTextOffY = 0;
    }
    else if ( eAlign == ALIGN_TOP )
    {
        mnTextOffX = 0;
        mnTextOffY = +pFontEntry->maMetric.mnAscent + mnEmphasisAscent;
        if ( pFontEntry->mnOrientation )
            ImplRotatePos( 0, 0, mnTextOffX, mnTextOffY, pFontEntry->mnOrientation );
    }
    else // eAlign == ALIGN_BOTTOM
    {
        mnTextOffX = 0;
        mnTextOffY = -pFontEntry->maMetric.mnDescent + mnEmphasisDescent;
        if ( pFontEntry->mnOrientation )
            ImplRotatePos( 0, 0, mnTextOffX, mnTextOffY, pFontEntry->mnOrientation );
    }

    mbTextLines     = ((maFont.GetUnderline() != UNDERLINE_NONE) && (maFont.GetUnderline() != UNDERLINE_DONTKNOW)) ||
                      ((maFont.GetOverline()  != UNDERLINE_NONE) && (maFont.GetOverline()  != UNDERLINE_DONTKNOW)) ||
                      ((maFont.GetStrikeout() != STRIKEOUT_NONE) && (maFont.GetStrikeout() != STRIKEOUT_DONTKNOW));
    mbTextSpecial   = maFont.IsShadow() || maFont.IsOutline() ||
                      (maFont.GetRelief() != RELIEF_NONE);

    // #95414# fix for OLE objects which use scale factors very creatively
    if( mbMap && !aSize.Width() )
    {
        int nOrigWidth = pFontEntry->maMetric.mnWidth;
        float fStretch = (float)maMapRes.mnMapScNumX * maMapRes.mnMapScDenomY;
        fStretch /= (float)maMapRes.mnMapScNumY * maMapRes.mnMapScDenomX;
        int nNewWidth = (int)(nOrigWidth * fStretch + 0.5);
        if( (nNewWidth != nOrigWidth) && (nNewWidth != 0) )
        {
            Size aOrigSize = maFont.GetSize();
            const_cast<Font&>(maFont).SetSize( Size( nNewWidth, aSize.Height() ) );
            mbMap = FALSE;
            mbNewFont = TRUE;
            ImplNewFont();  // recurse once using stretched width
            mbMap = TRUE;
            const_cast<Font&>(maFont).SetSize( aOrigSize );
        }
    }
    return true;
}
// =======================================================================

FontEmphasisMark OutputDevice::ImplGetEmphasisMarkStyle( const Font& rFont ) const
{
    FontEmphasisMark nEmphasisMark = rFont.GetEmphasisMark();

    // If no Position is set, then calculate the default position, which
    // depends on the language
    if ( !(nEmphasisMark & (EMPHASISMARK_POS_ABOVE | EMPHASISMARK_POS_BELOW)) )
    {
        LanguageType eLang = rFont.GetLanguage();
        // In Chinese Simplified the EmphasisMarks are below/left
        if ( (eLang == LANGUAGE_CHINESE_SIMPLIFIED) ||
             (eLang == LANGUAGE_CHINESE_SINGAPORE) )
            nEmphasisMark |= EMPHASISMARK_POS_BELOW;
        else
        {
            eLang = rFont.GetCJKContextLanguage();
            // In Chinese Simplified the EmphasisMarks are below/left
            if ( (eLang == LANGUAGE_CHINESE_SIMPLIFIED) ||
                 (eLang == LANGUAGE_CHINESE_SINGAPORE) )
                nEmphasisMark |= EMPHASISMARK_POS_BELOW;
            else
                nEmphasisMark |= EMPHASISMARK_POS_ABOVE;
        }
    }

    return nEmphasisMark;
}
// =======================================================================

void OutputDevice::ImplInitFontList() const
{
    if( ! mpFontList->Count() )
    {
        if( mpGraphics )
        {
			//RTL_LOGFILE_CONTEXT( aLog, "OutputDevice::ImplInitFontList()" );
            mpGraphics->GetDevFontList( mpFontList );
        }
    }
}
void OutputDevice::DrawTextArray( const Point& rStartPt, const XubString& rStr,
                                       const sal_Int32* pDXAry,
                                       xub_StrLen nIndex,
                                       xub_StrLen nLen )
{
    SalLayout* pSalLayout = ImplLayout( rStr, nIndex, nLen, rStartPt, 0, pDXAry, true );
    if( pSalLayout )
    {
        ImplDrawText( *pSalLayout );
        pSalLayout->Release();
    }
}
void OutputDevice::DrawStretchText( const Point& rStartPt, ULONG nWidth,
                                         const XubString& rStr,
                                         xub_StrLen nIndex , xub_StrLen nLen )
{
    SalLayout* pSalLayout = ImplLayout( rStr, nIndex, nLen, rStartPt, nWidth, NULL, true );
    if( pSalLayout )
    {
        ImplDrawText( *pSalLayout );
        pSalLayout->Release();
    }
}
void OutputDevice::DrawText( const Rectangle& rRect,
                                  const XubString& rOrigStr, USHORT nStyle,
                                  MetricVector* pVector , String* pDisplayText )
{
    if ( ! pVector || !rOrigStr.Len() || rRect.IsEmpty() )
        return;

    // we need a graphics
    if( !mpGraphics )
        return;
    if( mbInitClipRegion )
        ImplInitClipRegion();
    if( mbOutputClipped )
        return;

    // #i47157# Factored out to ImplDrawTextRect(), to be used also
    // from AddTextRectActions()
    ImplDrawText( rRect, rOrigStr, nStyle, pVector, pDisplayText );
}
void OutputDevice::DrawTextLine( const Point& rPos, long nWidth,
                                      FontStrikeout eStrikeout,
                                      FontUnderline eUnderline,
                                      FontUnderline eOverline,
                                      BOOL bUnderlineAbove )
{
    if ( ((eUnderline == UNDERLINE_NONE) || (eUnderline == UNDERLINE_DONTKNOW)) &&
         ((eOverline  == UNDERLINE_NONE) || (eOverline  == UNDERLINE_DONTKNOW)) &&
         ((eStrikeout == STRIKEOUT_NONE) || (eStrikeout == STRIKEOUT_DONTKNOW)) )
        return;

    // we need a graphics
    if( !mpGraphics )
        return;
    if( mbInitClipRegion )
        ImplInitClipRegion();
    if( mbOutputClipped )
        return;

    // initialize font if needed to get text offsets
    // TODO: only needed for mnTextOff!=(0,0)
    if( mbNewFont )
        if( !ImplNewFont() )
            return;
    if( mbInitFont )
        ImplInitFont();

    Point aPos = ImplLogicToDevicePixel( rPos );
    nWidth = ImplLogicWidthToDevicePixel( nWidth );
    aPos += Point( mnTextOffX, mnTextOffY );
    ImplDrawTextLine( aPos.X(), aPos.X(), aPos.Y(), nWidth, eStrikeout, eUnderline, eOverline, bUnderlineAbove );
}

void OutputDevice::DrawBitmap( const Point& rDestPt,
                                    const Bitmap& rBitmap )
{
	const Size aSizePix( rBitmap.GetSizePixel() );
	ImplDrawBitmap( rDestPt, PixelToLogic( aSizePix ), Point(), aSizePix, rBitmap, META_BMP_ACTION );
}
void OutputDevice::DrawBitmap( const Point& rDestPt, const Size& rDestSize,
                                    const Bitmap& rBitmap )
{
	ImplDrawBitmap( rDestPt, rDestSize, Point(), rBitmap.GetSizePixel(), rBitmap, META_BMPSCALE_ACTION );
}
void OutputDevice::DrawBitmap( const Point& rDestPt, const Size& rDestSize,
                                    const Point& rSrcPtPixel, const Size& rSrcSizePixel,
                                    const Bitmap& rBitmap )
{
	ImplDrawBitmap( rDestPt, rDestSize, rSrcPtPixel, rSrcSizePixel, rBitmap, META_BMPSCALEPART_ACTION );
}

void OutputDevice::ImplDrawBitmap( const Point& rDestPt, const Size& rDestSize,
								   const Point& rSrcPtPixel, const Size& rSrcSizePixel,
								   const Bitmap& rBitmap, const ULONG nAction )
{
	Bitmap aBmp( rBitmap );

	if ( ROP_INVERT == meRasterOp )
	{
		DrawRect( Rectangle( rDestPt, rDestSize ) );
		return;
	}
	OUTDEV_INIT();

	if( !aBmp.IsEmpty() )
	{
		SalTwoRect aPosAry;

		aPosAry.mnSrcX = rSrcPtPixel.X();
		aPosAry.mnSrcY = rSrcPtPixel.Y();
		aPosAry.mnSrcWidth = rSrcSizePixel.Width();
		aPosAry.mnSrcHeight = rSrcSizePixel.Height();
		aPosAry.mnDestX = ImplLogicXToDevicePixel( rDestPt.X() );
		aPosAry.mnDestY = ImplLogicYToDevicePixel( rDestPt.Y() );
		aPosAry.mnDestWidth = ImplLogicWidthToDevicePixel( rDestSize.Width() );
		aPosAry.mnDestHeight = ImplLogicHeightToDevicePixel( rDestSize.Height() );

		const ULONG nMirrFlags = ImplAdjustTwoRect( aPosAry, aBmp.GetSizePixel() );

		if ( aPosAry.mnSrcWidth && aPosAry.mnSrcHeight && aPosAry.mnDestWidth && aPosAry.mnDestHeight )
		{
			if ( nMirrFlags )
				aBmp.Mirror( nMirrFlags );
            
            /* #i75264# (corrected with #i81576#)
            * sometimes a bitmap is scaled to a ridiculous size and drawn
            * to a quite normal VDev, so only a very small part of
            * the scaled bitmap will be visible. However actually scaling
            * the bitmap will use so much memory that we end with a crash.
            * Workaround: since only a small part of the scaled bitmap will
            * be actually drawn anyway (because of clipping on the device
            * boundary), limit the destination and source rectangles so
            * that the destination rectangle will overlap the device but only
            * be reasonably (say factor 2) larger than the device itself.
            */
            if( aPosAry.mnDestWidth > 2048 || aPosAry.mnDestHeight > 2048 )
            {
                // #i81576# do the following trick only if there is overlap at all
                // else the formulae don't work
                // theoretically in this case we wouldn't need to draw the bitmap at all
                // however there are some esoteric case where that is needed
                if( aPosAry.mnDestX + aPosAry.mnDestWidth >= 0
                    && aPosAry.mnDestX < mnOutWidth
                    && aPosAry.mnDestY + aPosAry.mnDestHeight >= 0
                    && aPosAry.mnDestY < mnOutHeight )
                {
                    // reduce scaling to something reasonable taking into account the output size
                    if( aPosAry.mnDestWidth > 3*mnOutWidth && aPosAry.mnSrcWidth )
                    {
                        const double nScaleX = aPosAry.mnDestWidth/double(aPosAry.mnSrcWidth);

                        if( aPosAry.mnDestX + aPosAry.mnDestWidth > mnOutWidth )
                        {
                            aPosAry.mnDestWidth = Max(long(0),mnOutWidth-aPosAry.mnDestX);
                        }
                        if( aPosAry.mnDestX < 0 )
                        {
                            aPosAry.mnDestWidth += aPosAry.mnDestX;
                            aPosAry.mnSrcX -= static_cast<long>(aPosAry.mnDestX / nScaleX);
                            aPosAry.mnDestX = 0;
                        }

                        aPosAry.mnSrcWidth = static_cast<long>(aPosAry.mnDestWidth / nScaleX);
                    }

                    if( aPosAry.mnDestHeight > 3*mnOutHeight && aPosAry.mnSrcHeight != 0 )
                    {
                        const double nScaleY = aPosAry.mnDestHeight/double(aPosAry.mnSrcHeight);

                        if( aPosAry.mnDestY + aPosAry.mnDestHeight > mnOutHeight )
                        {
                            aPosAry.mnDestHeight = Max(long(0),mnOutHeight-aPosAry.mnDestY);
                        }
                        if( aPosAry.mnDestY < 0 )
                        {
                            aPosAry.mnDestHeight += aPosAry.mnDestY;
                            aPosAry.mnSrcY -= static_cast<long>(aPosAry.mnDestY / nScaleY);
                            aPosAry.mnDestY = 0;
                        }

                        aPosAry.mnSrcHeight = static_cast<long>(aPosAry.mnDestHeight / nScaleY);
                    }
                }
            }

            if ( aPosAry.mnSrcWidth && aPosAry.mnSrcHeight && aPosAry.mnDestWidth && aPosAry.mnDestHeight )
                mpGraphics->DrawBitmap( &aPosAry, *aBmp.ImplGetImpBitmap()->ImplGetSalBitmap(), this );
		}
	}
}
void OutputDevice::DrawBitmapEx( const Point& rDestPt,
                                      const BitmapEx& rBitmapEx )
{
	if( TRANSPARENT_NONE == rBitmapEx.GetTransparentType() )
		DrawBitmap( rDestPt, rBitmapEx.GetBitmap() );
	else
	{
		const Size aSizePix( rBitmapEx.GetSizePixel() );
		ImplDrawBitmapEx( rDestPt, PixelToLogic( aSizePix ), Point(), aSizePix, rBitmapEx, META_BMPEX_ACTION );
	}
}
void OutputDevice::DrawBitmapEx( const Point& rDestPt, const Size& rDestSize,
                                      const BitmapEx& rBitmapEx )
{
	if ( TRANSPARENT_NONE == rBitmapEx.GetTransparentType() )
		DrawBitmap( rDestPt, rDestSize, rBitmapEx.GetBitmap() );
	else
		ImplDrawBitmapEx( rDestPt, rDestSize, Point(), rBitmapEx.GetSizePixel(), rBitmapEx, META_BMPEXSCALE_ACTION );

}
void OutputDevice::DrawBitmapEx( const Point& rDestPt, const Size& rDestSize,
                                      const Point& rSrcPtPixel, const Size& rSrcSizePixel,
                                      const BitmapEx& rBitmapEx )
{
	if( TRANSPARENT_NONE == rBitmapEx.GetTransparentType() )
		DrawBitmap( rDestPt, rDestSize, rSrcPtPixel, rSrcSizePixel, rBitmapEx.GetBitmap() );
	else
		ImplDrawBitmapEx( rDestPt, rDestSize, rSrcPtPixel, rSrcSizePixel, rBitmapEx, META_BMPEXSCALEPART_ACTION );

}
void OutputDevice::ImplDrawBitmapEx( const Point& rDestPt, const Size& rDestSize,
										const Point& rSrcPtPixel, const Size& rSrcSizePixel,
										const BitmapEx& rBitmapEx, const ULONG nAction )
{
	BitmapEx aBmpEx( rBitmapEx );

	if ( ROP_INVERT == meRasterOp )
	{
		DrawRect( Rectangle( rDestPt, rDestSize ) );
		return;
	}

	OUTDEV_INIT();

	if( aBmpEx.IsAlpha() )
	{
		ImplDrawAlpha( aBmpEx.GetBitmap(), aBmpEx.GetAlpha(), rDestPt, rDestSize, rSrcPtPixel, rSrcSizePixel );
		return;
	}

	if( !( !aBmpEx ) )
	{
		SalTwoRect aPosAry;

		aPosAry.mnSrcX = rSrcPtPixel.X();
		aPosAry.mnSrcY = rSrcPtPixel.Y();
		aPosAry.mnSrcWidth = rSrcSizePixel.Width();
		aPosAry.mnSrcHeight = rSrcSizePixel.Height();
		aPosAry.mnDestX = ImplLogicXToDevicePixel( rDestPt.X() );
		aPosAry.mnDestY = ImplLogicYToDevicePixel( rDestPt.Y() );
		aPosAry.mnDestWidth = ImplLogicWidthToDevicePixel( rDestSize.Width() );
		aPosAry.mnDestHeight = ImplLogicHeightToDevicePixel( rDestSize.Height() );

		const ULONG nMirrFlags = ImplAdjustTwoRect( aPosAry, aBmpEx.GetSizePixel() );

		if( aPosAry.mnSrcWidth && aPosAry.mnSrcHeight && aPosAry.mnDestWidth && aPosAry.mnDestHeight )
		{

			if( nMirrFlags )
				aBmpEx.Mirror( nMirrFlags );

			const ImpBitmap* pImpBmp = aBmpEx.ImplGetBitmapImpBitmap();
			const ImpBitmap* pMaskBmp = aBmpEx.ImplGetMaskImpBitmap();

			if ( pMaskBmp )
            {
                // #4919452# reduce operation area to bounds of
                // cliprect. since masked transparency involves
                // creation of a large vdev and copying the screen
                // content into that (slooow read from framebuffer),
                // that should considerably increase performance for
                // large bitmaps and small clippings.

                // Note that this optimisation is a workaround for a
                // Writer peculiarity, namely, to decompose background
                // graphics into myriads of disjunct, tiny
                // rectangles. That otherwise kills us here, since for
                // transparent output, SAL always prepares the whole
                // bitmap, if aPosAry contains the whole bitmap (and
                // it's _not_ to blame for that).

                // Note the call to ImplPixelToDevicePixel(), since
                // aPosAry already contains the mnOutOff-offsets, they
                // also have to be applied to the region
                Rectangle aClipRegionBounds( ImplPixelToDevicePixel(maRegion).GetBoundRect() );

                // TODO: Also respect scaling (that's a bit tricky,
                // since the source points have to move fractional
                // amounts (which is not possible, thus has to be
                // emulated by increases copy area)
                // const double nScaleX( aPosAry.mnDestWidth / aPosAry.mnSrcWidth );
                // const double nScaleY( aPosAry.mnDestHeight / aPosAry.mnSrcHeight );

                // for now, only identity scales allowed
                if( !aClipRegionBounds.IsEmpty() &&
                    aPosAry.mnDestWidth == aPosAry.mnSrcWidth &&
                    aPosAry.mnDestHeight == aPosAry.mnSrcHeight )
                {
                    // now intersect dest rect with clip region
                    aClipRegionBounds.Intersection( Rectangle( aPosAry.mnDestX, 
                                                               aPosAry.mnDestY,
                                                               aPosAry.mnDestX + aPosAry.mnDestWidth - 1,
                                                               aPosAry.mnDestY + aPosAry.mnDestHeight - 1 ) );

                    // Note: I could theoretically optimize away the
                    // DrawBitmap below, if the region is empty
                    // here. Unfortunately, cannot rule out that
                    // somebody relies on the side effects.
                    if( !aClipRegionBounds.IsEmpty() )
                    {
                        aPosAry.mnSrcX += aClipRegionBounds.Left() - aPosAry.mnDestX;
                        aPosAry.mnSrcY += aClipRegionBounds.Top() - aPosAry.mnDestY;
                        aPosAry.mnSrcWidth = aClipRegionBounds.GetWidth();
                        aPosAry.mnSrcHeight = aClipRegionBounds.GetHeight();
                        
                        aPosAry.mnDestX = aClipRegionBounds.Left();
                        aPosAry.mnDestY = aClipRegionBounds.Top();
                        aPosAry.mnDestWidth = aClipRegionBounds.GetWidth();
                        aPosAry.mnDestHeight = aClipRegionBounds.GetHeight();
                    }
                }

				mpGraphics->DrawBitmap( &aPosAry, *pImpBmp->ImplGetSalBitmap(), 
                                        *pMaskBmp->ImplGetSalBitmap(), 
                                        this );
            }
			else
            {
				mpGraphics->DrawBitmap( &aPosAry, *pImpBmp->ImplGetSalBitmap(), this );
            }
		}
	}
}
void OutputDevice::ImplDrawAlpha( const Bitmap& rBmp, const AlphaMask& rAlpha,
								  const Point& rDestPt, const Size& rDestSize,
								  const Point& rSrcPtPixel, const Size& rSrcSizePixel )
{
	const Point aNullPt;
	Point		aOutPt( LogicToPixel( rDestPt ) );
	Size        aOutSz( LogicToPixel( rDestSize ) );
	Rectangle	aDstRect( aNullPt, GetOutputSizePixel() );
	const BOOL	bHMirr = aOutSz.Width() < 0, bVMirr = aOutSz.Height() < 0;
#ifdef AVS
	if( OUTDEV_WINDOW == meOutDevType )
	{
		const Region aPaintRgn( ( (Window*) this )->GetPaintRegion() );

		if( !aPaintRgn.IsNull() )
			aDstRect.Intersection( LogicToPixel( aPaintRgn.GetBoundRect() ) );
	}
#endif
	if( bHMirr )
	{
		aOutSz.Width() = -aOutSz.Width();
		aOutPt.X() -= ( aOutSz.Width() - 1L );
	}

	if( bVMirr )
	{
		aOutSz.Height() = -aOutSz.Height();
		aOutPt.Y() -= ( aOutSz.Height() - 1L );
	}

	if( !aDstRect.Intersection( Rectangle( aOutPt, aOutSz ) ).IsEmpty() )
	{
        bool bNativeAlpha = false;
        static const char* pDisableNative = getenv( "SAL_DISABLE_NATIVE_ALPHA");
        // #i83087# Naturally, system alpha blending cannot work with
        // separate alpha VDev
        if( !pDisableNative && !bHMirr && !bVMirr ) 
        {
            Point aRelPt = aOutPt + Point( mnOutOffX, mnOutOffY );
            SalTwoRect aTR = {
                rSrcPtPixel.X(), rSrcPtPixel.Y(),
                rSrcSizePixel.Width(), rSrcSizePixel.Height(),
                aRelPt.X(), aRelPt.Y(),
                aOutSz.Width(), aOutSz.Height()
            };
            SalBitmap* pSalSrcBmp = rBmp.ImplGetImpBitmap()->ImplGetSalBitmap();
            SalBitmap* pSalAlphaBmp = rAlpha.ImplGetImpBitmap()->ImplGetSalBitmap();
            bNativeAlpha = mpGraphics->DrawAlphaBitmap( aTR, *pSalSrcBmp, *pSalAlphaBmp, this );
        }

        //VirtualDevice* pOldVDev = mpAlphaVDev;

		Rectangle aBmpRect( aNullPt, rBmp.GetSizePixel() );
		if( !bNativeAlpha
                &&  !aBmpRect.Intersection( Rectangle( rSrcPtPixel, rSrcSizePixel ) ).IsEmpty() ) 
		{
			//GDIMetaFile*	pOldMetaFile = mpMetaFile; mpMetaFile = NULL;
			const BOOL		bOldMap = mbMap; mbMap = FALSE;
			Bitmap			aBmp( GetBitmap( aDstRect.TopLeft(), aDstRect.GetSize() ) );

            // #109044# The generated bitmap need not necessarily be
            // of aDstRect dimensions, it's internally clipped to
            // window bounds. Thus, we correct the dest size here,
            // since we later use it (in nDstWidth/Height) for pixel
            // access)
            // #i38887# reading from screen may sometimes fail
            if( aBmp.ImplGetImpBitmap() )
                aDstRect.SetSize( aBmp.GetSizePixel() );

			BitmapColor 	aDstCol;
			const long		nSrcWidth = aBmpRect.GetWidth(), nSrcHeight = aBmpRect.GetHeight();
			const long		nDstWidth = aDstRect.GetWidth(), nDstHeight = aDstRect.GetHeight();
			const long		nOutWidth = aOutSz.Width(), nOutHeight = aOutSz.Height();
			const long		nOffX = aDstRect.Left() - aOutPt.X(), nOffY = aDstRect.Top() - aOutPt.Y();
			long			nX, nOutX, nY, nOutY;
			long			nMirrOffX = 0;
			long			nMirrOffY = 0;
			long*			pMapX = new long[ nDstWidth ];
			long*			pMapY = new long[ nDstHeight ];

			// create horizontal mapping table
			if( bHMirr )
				nMirrOffX = ( aBmpRect.Left() << 1 ) + nSrcWidth - 1;

			for( nX = 0L, nOutX = nOffX; nX < nDstWidth; nX++, nOutX++ )
			{
				pMapX[ nX ] = aBmpRect.Left() + nOutX * nSrcWidth / nOutWidth;

				if( bHMirr )
					pMapX[ nX ] = nMirrOffX - pMapX[ nX ];
			}
            
			// create vertical mapping table
			if( bVMirr )
				nMirrOffY = ( aBmpRect.Top() << 1 ) + nSrcHeight - 1;

			for( nY = 0L, nOutY = nOffY; nY < nDstHeight; nY++, nOutY++ )
			{
				pMapY[ nY ] = aBmpRect.Top() + nOutY * nSrcHeight / nOutHeight;

				if( bVMirr )
					pMapY[ nY ] = nMirrOffY - pMapY[ nY ];
			}

            BitmapReadAccess*	pP = ( (Bitmap&) rBmp ).AcquireReadAccess();
            BitmapReadAccess*	pA = ( (AlphaMask&) rAlpha ).AcquireReadAccess();

            //DBG_ASSERT( pA->GetScanlineFormat() == BMP_FORMAT_8BIT_PAL ||
                        //pA->GetScanlineFormat() == BMP_FORMAT_8BIT_TC_MASK, 
                        //"OutputDevice::ImplDrawAlpha(): non-8bit alpha no longer supported!" );

            // #i38887# reading from screen may sometimes fail
            if( aBmp.ImplGetImpBitmap() )
            {
                Bitmap aTmp;
#ifdef AVS
                if( mpAlphaVDev )
                {
#endif

                    aTmp = ImplBlendWithAlpha(
                        aBmp,pP,pA,
                        aDstRect,
                        nOffY,nDstHeight,
                        nOffX,nDstWidth,
                        pMapX,pMapY );

#ifdef AVS
                }
                else
                {
                    aTmp = ImplBlend(
                        aBmp,pP,pA,
                        nOffY,nDstHeight,
                        nOffX,nDstWidth,
                        aBmpRect,aOutSz,
                        bHMirr,bVMirr,
                        pMapX,pMapY );
                }

                // #110958# Disable alpha VDev, we're doing the necessary
                // stuff explicitely furher below
                if( mpAlphaVDev )
                    mpAlphaVDev = NULL;
#endif
                DrawBitmap( aDstRect.TopLeft(), 
                            aTmp );
#ifdef AVS
                // #110958# Enable alpha VDev again
                mpAlphaVDev = pOldVDev;
#endif
            }
            
            ( (Bitmap&) rBmp ).ReleaseAccess( pP );
            ( (AlphaMask&) rAlpha ).ReleaseAccess( pA );

			delete[] pMapX;
			delete[] pMapY;
			mbMap = bOldMap;
			//mpMetaFile = pOldMetaFile;
		}
	}
}
// ------------------------------------------------------------------------

Bitmap OutputDevice::ImplBlendWithAlpha( Bitmap              aBmp,
                                         BitmapReadAccess*	 pP,
                                         BitmapReadAccess*	 pA,
                                         const Rectangle&    aDstRect,
                                         const sal_Int32     nOffY, 
                                         const sal_Int32     nDstHeight, 
                                         const sal_Int32     nOffX, 
                                         const sal_Int32     nDstWidth,
                                         const long*         pMapX,
                                         const long*         pMapY )
{
    BitmapColor aDstCol,aSrcCol;
    BYTE        nSrcAlpha, nDstAlpha;
    Bitmap      res;
    int         nX, nOutX, nY, nOutY;
#ifdef QWE
    OSL_ENSURE(mpAlphaVDev,
               "ImplBlendWithAlpha(): call me only with valid alpha VDev!" );

    BOOL bOldMapMode( mpAlphaVDev->IsMapModeEnabled() );
    mpAlphaVDev->EnableMapMode(FALSE);

    Bitmap aAlphaBitmap( mpAlphaVDev->GetBitmap( aDstRect.TopLeft(), aDstRect.GetSize() ) );
    BitmapWriteAccess*	pAlphaW = aAlphaBitmap.AcquireWriteAccess();
#endif
    if( GetBitCount() <= 8 )
    {
        Bitmap				aDither( aBmp.GetSizePixel(), 8 );
        BitmapColor 		aIndex( 0 );
        BitmapReadAccess*	pB = aBmp.AcquireReadAccess();
        BitmapWriteAccess*	pW = aDither.AcquireWriteAccess();

        if( pB && pP && pA && pW /*&& pAlphaW*/ )
        {
            for( nY = 0, nOutY = nOffY; nY < nDstHeight; nY++, nOutY++ )
            {
                const long nMapY = pMapY[ nY ];
                const long nModY = ( nOutY & 0x0FL ) << 4L;

                for( nX = 0, nOutX = nOffX; nX < nDstWidth; nX++, nOutX++ )
                {
                    const long	nMapX = pMapX[ nX ];
                    const ULONG nD = nVCLDitherLut[ nModY | ( nOutX & 0x0FL ) ];

                    aSrcCol = pP->GetColor( nMapY, nMapX );
                    aDstCol = pB->GetColor( nY, nX );
                    nSrcAlpha  = 255 - pA->GetPixel( nMapY, nMapX ).GetBlueOrIndex();
                    nDstAlpha  = 0;//255 - pAlphaW->GetPixel( nY, nX ).GetBlueOrIndex();

                    if( nSrcAlpha + nDstAlpha == 0 )
                    {
                        // #i70653# zero alpha -> zero color values
                        aIndex.SetIndex( (BYTE) ( nVCLRLut[ ( nVCLLut[ 0 ] + nD ) >> 16UL ] +
                                                  nVCLGLut[ ( nVCLLut[ 0 ] + nD ) >> 16UL ] +
                                                  nVCLBLut[ ( nVCLLut[ 0 ] + nD ) >> 16UL ] ) );
                    }
                    else
                    {
                        aDstCol.SetRed( (BYTE)(((int)(aSrcCol.GetRed())*nSrcAlpha + (int)(aDstCol.GetRed())*nDstAlpha) /
                                               (nSrcAlpha+nDstAlpha)) );
                        aDstCol.SetGreen( (BYTE)(((int)(aSrcCol.GetGreen())*nSrcAlpha + (int)(aDstCol.GetGreen())*nDstAlpha) /
                                                 (nSrcAlpha+nDstAlpha)) );
                        aDstCol.SetBlue( (BYTE)(((int)(aSrcCol.GetBlue())*nSrcAlpha + (int)(aDstCol.GetBlue())*nDstAlpha) /
                                                (nSrcAlpha+nDstAlpha)) );
                                     
                        aIndex.SetIndex( (BYTE) ( nVCLRLut[ ( nVCLLut[ aDstCol.GetRed() ] + nD ) >> 16UL ] +
                                                  nVCLGLut[ ( nVCLLut[ aDstCol.GetGreen() ] + nD ) >> 16UL ] +
                                                  nVCLBLut[ ( nVCLLut[ aDstCol.GetBlue() ] + nD ) >> 16UL ] ) );
                    }
                    pW->SetPixel( nY, nX, aIndex );

                    // Have to perform the compositing 'algebra' in
                    // the inverse alpha space (with 255 meaning
                    // opaque), otherwise, transitivity is not
                    // achieved.
                    nSrcAlpha = 255-COLOR_CHANNEL_MERGE( 255, (BYTE)nDstAlpha, nSrcAlpha );

                    aIndex.SetIndex( (BYTE) ( nVCLRLut[ ( nVCLLut[ nSrcAlpha ] + nD ) >> 16UL ] +
                                              nVCLGLut[ ( nVCLLut[ nSrcAlpha ] + nD ) >> 16UL ] +
                                              nVCLBLut[ ( nVCLLut[ nSrcAlpha ] + nD ) >> 16UL ] ) );
#ifdef AVS
                    pAlphaW->SetPixel( nY, nX, aIndex );
#endif
                }
            }
        }

        aBmp.ReleaseAccess( pB );
        aDither.ReleaseAccess( pW );
        res = aDither;
    }
    else
    {
        BitmapWriteAccess*	pB = aBmp.AcquireWriteAccess();
        if( pP && pA && pB )
        {
            for( nY = 0; nY < nDstHeight; nY++ )
            {
                const long	nMapY = pMapY[ nY ];

                for( nX = 0; nX < nDstWidth; nX++ )
                {
                    const long nMapX = pMapX[ nX ];

                    aSrcCol = pP->GetColor( nMapY, nMapX );
                    aDstCol = pB->GetColor( nY, nX );
                    nSrcAlpha  = 255 - pA->GetPixel( nMapY, nMapX ).GetBlueOrIndex();
                    nDstAlpha  = 255;//255 - pAlphaW->GetPixel( nY, nX ).GetBlueOrIndex();

                    if( nSrcAlpha + nDstAlpha == 0 )
                    {
                        // #i70653# zero alpha -> zero color values
                        aDstCol.SetRed(0);
                        aDstCol.SetGreen(0);
                        aDstCol.SetBlue(0);
                    }
                    else
                    {
                        aDstCol.SetRed( (BYTE)(((int)(aSrcCol.GetRed())*nSrcAlpha + (int)(aDstCol.GetRed())*nDstAlpha) /
                                               (nSrcAlpha+nDstAlpha)) );
                        aDstCol.SetGreen( (BYTE)(((int)(aSrcCol.GetGreen())*nSrcAlpha + (int)(aDstCol.GetGreen())*nDstAlpha) /
                                                 (nSrcAlpha+nDstAlpha)) );
                        aDstCol.SetBlue( (BYTE)(((int)(aSrcCol.GetBlue())*nSrcAlpha + (int)(aDstCol.GetBlue())*nDstAlpha) /
                                                (nSrcAlpha+nDstAlpha)) );
                    }

                    pB->SetPixel( nY, nX, aDstCol );

                    // Have to perform the compositing 'algebra' in
                    // the inverse alpha space (with 255 meaning
                    // opaque), otherwise, transitivity is not
                    // achieved.
                    nSrcAlpha = 255-COLOR_CHANNEL_MERGE( 255, (BYTE)nDstAlpha, nSrcAlpha );
#ifdef AVS
                    pAlphaW->SetPixel( nY, nX, Color(nSrcAlpha, nSrcAlpha, nSrcAlpha) );
#endif
                }
            }
        }

        aBmp.ReleaseAccess( pB );
        res = aBmp;
    }
#ifdef AVS
    aAlphaBitmap.ReleaseAccess( pAlphaW );
    mpAlphaVDev->DrawBitmap( aDstRect.TopLeft(), aAlphaBitmap );
    mpAlphaVDev->EnableMapMode( bOldMapMode );
#endif
    return res;
}

// ------------------------------------------------------------------------

Bitmap OutputDevice::ImplBlend( Bitmap              aBmp,
                                BitmapReadAccess*	pP,
                                BitmapReadAccess*	pA,
                                const sal_Int32     nOffY, 
                                const sal_Int32     nDstHeight, 
                                const sal_Int32     nOffX, 
                                const sal_Int32     nDstWidth,
                                const Rectangle&    aBmpRect, 
                                const Size&         aOutSz,
                                const bool          bHMirr,
                                const bool          bVMirr,
                                const long*         pMapX,
                                const long*         pMapY )
{
    BitmapColor aDstCol;
    Bitmap      res;
    int         nX, nOutX, nY, nOutY;

    if( GetBitCount() <= 8 )
    {
        Bitmap				aDither( aBmp.GetSizePixel(), 8 );
        BitmapColor 		aIndex( 0 );
        BitmapReadAccess*	pB = aBmp.AcquireReadAccess();
        BitmapWriteAccess*	pW = aDither.AcquireWriteAccess();

        if( pB && pP && pA && pW )
        {
            for( nY = 0, nOutY = nOffY; nY < nDstHeight; nY++, nOutY++ )
            {
                const long nMapY = pMapY[ nY ];
                const long nModY = ( nOutY & 0x0FL ) << 4L;

                for( nX = 0, nOutX = nOffX; nX < nDstWidth; nX++, nOutX++ )
                {
                    const long	nMapX = pMapX[ nX ];
                    const ULONG nD = nVCLDitherLut[ nModY | ( nOutX & 0x0FL ) ];

                    aDstCol = pB->GetColor( nY, nX );
                    aDstCol.Merge( pP->GetColor( nMapY, nMapX ), (BYTE) pA->GetPixel( nMapY, nMapX ) );
                    aIndex.SetIndex( (BYTE) ( nVCLRLut[ ( nVCLLut[ aDstCol.GetRed() ] + nD ) >> 16UL ] +
                                              nVCLGLut[ ( nVCLLut[ aDstCol.GetGreen() ] + nD ) >> 16UL ] +
                                              nVCLBLut[ ( nVCLLut[ aDstCol.GetBlue() ] + nD ) >> 16UL ] ) );
                    pW->SetPixel( nY, nX, aIndex );
                }
            }
        }

        aBmp.ReleaseAccess( pB );
        aDither.ReleaseAccess( pW );
        res = aDither;
    }
    else
    {
        BitmapWriteAccess*	pB = aBmp.AcquireWriteAccess();

        bool bFastBlend = false;
#ifdef AVS //fast делать не обязательно
        if( pP && pA && pB )
        {
            SalTwoRect aTR;
            aTR.mnSrcX      = aBmpRect.Left();
            aTR.mnSrcY      = aBmpRect.Top();
            aTR.mnSrcWidth  = aBmpRect.GetWidth();
            aTR.mnSrcHeight = aBmpRect.GetHeight();
            aTR.mnDestX     = nOffX;
            aTR.mnDestY     = nOffY;
            aTR.mnDestWidth = aOutSz.Width();
            aTR.mnDestHeight= aOutSz.Height();

            if( !bHMirr || !bVMirr )
                bFastBlend = ImplFastBitmapBlending( *pB,*pP,*pA, aTR );
        }
#endif
        if( pP && pA && pB && !bFastBlend )
        {
            switch( pP->GetScanlineFormat() )
            {
                case( BMP_FORMAT_8BIT_PAL ):
                    {
                        for( nY = 0; nY < nDstHeight; nY++ )
                        {
                            const long	nMapY = pMapY[ nY ];
                            Scanline	pPScan = pP->GetScanline( nMapY );
                            Scanline	pAScan = pA->GetScanline( nMapY );

                            for( nX = 0; nX < nDstWidth; nX++ )
                            {
                                const long nMapX = pMapX[ nX ];
                                aDstCol = pB->GetPixel( nY, nX );
                                pB->SetPixel( nY, nX, aDstCol.Merge( pP->GetPaletteColor( pPScan[ nMapX ] ),
                                                                     pAScan[ nMapX ] ) );
                            }
                        }
                    }
                    break;

                case( BMP_FORMAT_24BIT_TC_BGR ):
                    {
                        for( nY = 0; nY < nDstHeight; nY++ )
                        {
                            const long	nMapY = pMapY[ nY ];
                            Scanline	pPScan = pP->GetScanline( nMapY );
                            Scanline	pAScan = pA->GetScanline( nMapY );

                            for( nX = 0; nX < nDstWidth; nX++ )
                            {
                                const long	nMapX = pMapX[ nX ];
                                Scanline	pTmp = pPScan + nMapX * 3;

                                aDstCol = pB->GetPixel( nY, nX );
                                pB->SetPixel( nY, nX, aDstCol.Merge( pTmp[ 2 ], pTmp[ 1 ], pTmp[ 0 ],
                                                                     pAScan[ nMapX ] ) );
                            }
                        }
                    }
                    break;

                case( BMP_FORMAT_24BIT_TC_RGB ):
                    {
                        for( nY = 0; nY < nDstHeight; nY++ )
                        {
                            const long	nMapY = pMapY[ nY ];
                            Scanline	pPScan = pP->GetScanline( nMapY );
                            Scanline	pAScan = pA->GetScanline( nMapY );

                            for( nX = 0; nX < nDstWidth; nX++ )
                            {
                                const long	nMapX = pMapX[ nX ];
                                Scanline    pTmp = pPScan + nMapX * 3;

                                aDstCol = pB->GetPixel( nY, nX );
                                pB->SetPixel( nY, nX, aDstCol.Merge( pTmp[ 0 ], pTmp[ 1 ], pTmp[ 2 ],
                                                                     pAScan[ nMapX ] ) );
                            }
                        }
                    }
                    break;

                default:
                {
                    for( nY = 0; nY < nDstHeight; nY++ )
                    {
                        const long	nMapY = pMapY[ nY ];
                        Scanline	pAScan = pA->GetScanline( nMapY );

                        for( nX = 0; nX < nDstWidth; nX++ )
                        {
                            const long nMapX = pMapX[ nX ];
                            aDstCol = pB->GetPixel( nY, nX );
                            pB->SetPixel( nY, nX, aDstCol.Merge( pP->GetColor( nMapY, nMapX ),
                                                                 pAScan[ nMapX ] ) );
                        }
                    }
                }
                break;
            }
        }

        aBmp.ReleaseAccess( pB );
        res = aBmp;
    }

    return res;
}
void OutputDevice::DrawMask( const Point& rDestPt,
                                  const Bitmap& rBitmap, const Color& rMaskColor )
{
	const Size aSizePix( rBitmap.GetSizePixel() );
	ImplDrawMask( rDestPt, PixelToLogic( aSizePix ), Point(), aSizePix, rBitmap, rMaskColor, META_MASK_ACTION );
}
void OutputDevice::DrawMask( const Point& rDestPt, const Size& rDestSize,
                                  const Bitmap& rBitmap, const Color& rMaskColor )
{
	ImplDrawMask( rDestPt, rDestSize, Point(), rBitmap.GetSizePixel(), rBitmap, rMaskColor, META_MASKSCALE_ACTION );
}
void OutputDevice::DrawMask( const Point& rDestPt, const Size& rDestSize,
                                  const Point& rSrcPtPixel, const Size& rSrcSizePixel,
                                  const Bitmap& rBitmap, const Color& rMaskColor )
{
	ImplDrawMask( rDestPt, rDestSize, rSrcPtPixel, rSrcSizePixel, rBitmap, rMaskColor, META_MASKSCALEPART_ACTION );
}
// ------------------------------------------------------------------

void OutputDevice::ImplDrawMask( const Point& rDestPt, const Size& rDestSize,
								 const Point& rSrcPtPixel, const Size& rSrcSizePixel,
								 const Bitmap& rBitmap, const Color& rMaskColor,
								 const ULONG nAction )
{
	if( ROP_INVERT == meRasterOp )
	{
		DrawRect( Rectangle( rDestPt, rDestSize ) );
		return;
	}

	OUTDEV_INIT();

	const ImpBitmap* pImpBmp = rBitmap.ImplGetImpBitmap();
	if ( pImpBmp )
	{
		SalTwoRect aPosAry;

		aPosAry.mnSrcX = rSrcPtPixel.X();
		aPosAry.mnSrcY = rSrcPtPixel.Y();
		aPosAry.mnSrcWidth = rSrcSizePixel.Width();
		aPosAry.mnSrcHeight = rSrcSizePixel.Height();
		aPosAry.mnDestX = ImplLogicXToDevicePixel( rDestPt.X() );
		aPosAry.mnDestY = ImplLogicYToDevicePixel( rDestPt.Y() );
		aPosAry.mnDestWidth = ImplLogicWidthToDevicePixel( rDestSize.Width() );
		aPosAry.mnDestHeight = ImplLogicHeightToDevicePixel( rDestSize.Height() );

		// spiegeln via Koordinaten wollen wir nicht
		const ULONG nMirrFlags = ImplAdjustTwoRect( aPosAry, pImpBmp->ImplGetSize() );

		// check if output is necessary
		if( aPosAry.mnSrcWidth && aPosAry.mnSrcHeight && aPosAry.mnDestWidth && aPosAry.mnDestHeight )
		{

			if( nMirrFlags )
			{
				Bitmap aTmp( rBitmap );
				aTmp.Mirror( nMirrFlags );
				mpGraphics->DrawMask( &aPosAry, *aTmp.ImplGetImpBitmap()->ImplGetSalBitmap(),
									  ImplColorToSal( rMaskColor ) , this);
			}
			else
				mpGraphics->DrawMask( &aPosAry, *pImpBmp->ImplGetSalBitmap(),
									  ImplColorToSal( rMaskColor ), this );

		}
	}
}
void OutputDevice::DrawGradient( const Rectangle& rRect, const Gradient& rGradient )
{
	Gradient aGradient( rGradient );
    
	// Rechteck in Pixel umrechnen
	Rectangle aRect( ImplLogicToDevicePixel( rRect ) );
	aRect.Justify();

	// Wenn Rechteck leer ist, brauchen wir nichts machen
	if ( !aRect.IsEmpty() )
	{
		// Clip Region sichern
		Push( PUSH_CLIPREGION );
		IntersectClipRegion( rRect );

        // because we draw with no border line, we have to expand gradient
        // rect to avoid missing lines on the right and bottom edge
        aRect.Left()--;
        aRect.Top()--;
        aRect.Right()++;
        aRect.Bottom()++;

		// we need a graphics
		if ( !mpGraphics )
				return;

		if ( mbInitClipRegion )
			ImplInitClipRegion();
	
		if ( !mbOutputClipped )
		{
			// Gradienten werden ohne Umrandung gezeichnet
			if ( mbLineColor || mbInitLineColor )
			{
				mpGraphics->SetLineColor();
				mbInitLineColor = TRUE;
			}
			
			mbInitFillColor = TRUE;

			// calculate step count if neccessary
			if ( !aGradient.GetSteps() )
				aGradient.SetSteps( GRADIENT_DEFAULT_STEPCOUNT );

            if( aGradient.GetStyle() == GRADIENT_LINEAR || aGradient.GetStyle() == GRADIENT_AXIAL )
    			ImplDrawLinearGradient( aRect, aGradient, FALSE, NULL );
            else
				ImplDrawComplexGradient( aRect, aGradient, FALSE, NULL );
		}

		Pop();
	}
}
void OutputDevice::DrawGradient( const PolyPolygon& rPolyPoly, const Gradient& rGradient )
{
#ifdef AVS
	if( mbInitClipRegion )
		ImplInitClipRegion();
	
	if( mbOutputClipped )
		return;

	if( !mpGraphics )
			return;

	if( rPolyPoly.Count() && rPolyPoly[ 0 ].GetSize() )
	{
		const Rectangle	aRect( rPolyPoly.GetBoundRect() );

		Gradient aGradient( rGradient );

		{
			const PolyPolygon	aPolyPoly( LogicToPixel( rPolyPoly ) );
			const Rectangle		aBoundRect( aPolyPoly.GetBoundRect() );
			Point aPoint;
			Rectangle			aDstRect( aPoint, GetOutputSizePixel() );

			aDstRect.Intersection( aBoundRect );

			if( !aDstRect.IsEmpty() )
			{
				VirtualDevice*	pVDev;
				const Size		aDstSize( aDstRect.GetSize() );

                if( HasAlpha() )
                {
                    // #110958# Pay attention to alpha VDevs here, otherwise, 
                    // background will be wrong: Temp VDev has to have alpha, too.
                    pVDev = new VirtualDevice( *this, 0, GetAlphaBitCount() > 1 ? 0 : 1 );
                }
                else
                {
                    // nothing special here. Plain VDev
                    pVDev = new VirtualDevice();
                }

				if( pVDev->SetOutputSizePixel( aDstSize) )
				{
					MapMode			aVDevMap;
					const BOOL		bOldMap = mbMap;

					EnableMapMode( FALSE );

					pVDev->DrawOutDev( Point(), aDstSize, aDstRect.TopLeft(), aDstSize, *this );
					pVDev->SetRasterOp( ROP_XOR );
					aVDevMap.SetOrigin( Point( -aDstRect.Left(), -aDstRect.Top() ) );
					pVDev->SetMapMode( aVDevMap );
					pVDev->DrawGradient( aBoundRect, aGradient );
					pVDev->SetFillColor( COL_BLACK );
					pVDev->SetRasterOp( ROP_0 );
					pVDev->DrawPolyPolygon( aPolyPoly );
					pVDev->SetRasterOp( ROP_XOR );
					pVDev->DrawGradient( aBoundRect, aGradient );
					aVDevMap.SetOrigin( Point() );
					pVDev->SetMapMode( aVDevMap );
					DrawOutDev( aDstRect.TopLeft(), aDstSize, Point(), aDstSize, *pVDev );

					EnableMapMode( bOldMap );
				}

                delete pVDev;
			}
		}
	}
#endif
}
void OutputDevice::ImplDrawLinearGradient( const Rectangle& rRect,
										   const Gradient& rGradient,
										   BOOL bMtf, const PolyPolygon* pClipPolyPoly )
{
	// rotiertes BoundRect ausrechnen
	Rectangle aRect = rRect;
	aRect.Left()--;
	aRect.Top()--;
	aRect.Right()++;
	aRect.Bottom()++;
	USHORT	nAngle = rGradient.GetAngle() % 3600;
	double	fAngle	= nAngle * F_PI1800;
	double	fWidth	= aRect.GetWidth();
	double	fHeight = aRect.GetHeight();
	double	fDX 	= fWidth  * fabs( cos( fAngle ) ) +
					  fHeight * fabs( sin( fAngle ) );
	double	fDY 	= fHeight * fabs( cos( fAngle ) ) +
					  fWidth  * fabs( sin( fAngle ) );
			fDX 	= (fDX - fWidth)  * 0.5 + 0.5;
			fDY 	= (fDY - fHeight) * 0.5 + 0.5;
	aRect.Left()   -= (long)fDX;
	aRect.Right()  += (long)fDX;
	aRect.Top()    -= (long)fDY;
	aRect.Bottom() += (long)fDY;

	// Rand berechnen und Rechteck neu setzen
	Point		aCenter = rRect.Center();
	Rectangle	aFullRect = aRect;
	long		nBorder = (long)rGradient.GetBorder() * aRect.GetHeight() / 100;
	BOOL		bLinear;

	// Rand berechnen und Rechteck neu setzen fuer linearen Farbverlauf
	if ( rGradient.GetStyle() == GRADIENT_LINEAR )
	{
		bLinear = TRUE;
		aRect.Top() += nBorder;
	}
	// Rand berechnen und Rechteck neu setzen fuer axiale Farbverlauf
	else
	{
		bLinear = FALSE;
		nBorder >>= 1;

		aRect.Top()    += nBorder;
		aRect.Bottom() -= nBorder;
	}

	// Top darf nicht groesser als Bottom sein
	aRect.Top() = Min( aRect.Top(), (long)(aRect.Bottom() - 1) );

	long nMinRect = aRect.GetHeight();

	// Intensitaeten von Start- und Endfarbe ggf. aendern und
	// Farbschrittweiten berechnen
	long			nFactor;
	Color			aStartCol	= rGradient.GetStartColor();
	Color			aEndCol 	= rGradient.GetEndColor();
	long			nStartRed	= aStartCol.GetRed();
	long			nStartGreen = aStartCol.GetGreen();
	long			nStartBlue	= aStartCol.GetBlue();
	long			nEndRed 	= aEndCol.GetRed();
	long			nEndGreen	= aEndCol.GetGreen();
	long			nEndBlue	= aEndCol.GetBlue();
					nFactor 	= rGradient.GetStartIntensity();
					nStartRed	= (nStartRed   * nFactor) / 100;
					nStartGreen = (nStartGreen * nFactor) / 100;
					nStartBlue	= (nStartBlue  * nFactor) / 100;
					nFactor 	= rGradient.GetEndIntensity();
					nEndRed 	= (nEndRed	 * nFactor) / 100;
					nEndGreen	= (nEndGreen * nFactor) / 100;
					nEndBlue	= (nEndBlue  * nFactor) / 100;
	long			nRedSteps	= nEndRed	- nStartRed;
	long			nGreenSteps = nEndGreen - nStartGreen;
	long			nBlueSteps	= nEndBlue	- nStartBlue;
	long            nStepCount = rGradient.GetSteps();

	// Bei nicht linearen Farbverlaeufen haben wir nur die halben Steps
	// pro Farbe
	if ( !bLinear )
	{
		nRedSteps	<<= 1;
		nGreenSteps <<= 1;
		nBlueSteps	<<= 1;
	}

	// Anzahl der Schritte berechnen, falls nichts uebergeben wurde
	if ( !nStepCount )
	{
		long nInc;

		if ( !bMtf )
        {
			nInc = (nMinRect < 50) ? 2 : 4;
        }
		else
        {
            // #105998# Use display-equivalent step size calculation
			nInc = (nMinRect < 800) ? 10 : 20;
        }

		if ( !nInc )
			nInc = 1;

		nStepCount = nMinRect / nInc;
	}
	// minimal drei Schritte und maximal die Anzahl der Farbunterschiede
	long nSteps = Max( nStepCount, 2L );
	long nCalcSteps  = Abs( nRedSteps );
	long nTempSteps = Abs( nGreenSteps );
	if ( nTempSteps > nCalcSteps )
		nCalcSteps = nTempSteps;
	nTempSteps = Abs( nBlueSteps );
	if ( nTempSteps > nCalcSteps )
		nCalcSteps = nTempSteps;
	if ( nCalcSteps < nSteps )
		nSteps = nCalcSteps;
	if ( !nSteps )
		nSteps = 1;

	// Falls axialer Farbverlauf, muss die Schrittanzahl ungerade sein
	if ( !bLinear && !(nSteps & 1) )
		nSteps++;

	// Berechnung ueber Double-Addition wegen Genauigkeit
	double fScanLine = aRect.Top();
	double fScanInc  = (double)aRect.GetHeight() / (double)nSteps;

	// Startfarbe berechnen und setzen
	UINT8	nRed;
	UINT8	nGreen;
	UINT8	nBlue;
	long	nSteps2;
	long	nStepsHalf = 0;
	if ( bLinear )
	{
		// Um 1 erhoeht, um die Border innerhalb der Schleife
		// zeichnen zu koennen
		nSteps2 	= nSteps + 1;
		nRed		= (UINT8)nStartRed;
		nGreen		= (UINT8)nStartGreen;
		nBlue		= (UINT8)nStartBlue;
	}
	else
	{
		// Um 2 erhoeht, um die Border innerhalb der Schleife
		// zeichnen zu koennen
		nSteps2 	= nSteps + 2;
		nRed		= (UINT8)nEndRed;
		nGreen		= (UINT8)nEndGreen;
		nBlue		= (UINT8)nEndBlue;
		nStepsHalf	= nSteps >> 1;
	}

	if ( bMtf )
		;//mpMetaFile->AddAction( new MetaFillColorAction( Color( nRed, nGreen, nBlue ), TRUE ) );
	else
		mpGraphics->SetFillColor( MAKE_SALCOLOR( nRed, nGreen, nBlue ) );

	// Startpolygon erzeugen (== Borderpolygon)
	Polygon 	aPoly( 4 );
	Polygon 	aTempPoly( 2 );
	aPoly[0] = aFullRect.TopLeft();
	aPoly[1] = aFullRect.TopRight();
	aPoly[2] = aRect.TopRight();
	aPoly[3] = aRect.TopLeft();
	aPoly.Rotate( aCenter, nAngle );

	// Schleife, um rotierten Verlauf zu fuellen
	for ( long i = 0; i < nSteps2; i++ )
	{
		// berechnetesPolygon ausgeben
		if ( bMtf )
			;//mpMetaFile->AddAction( new MetaPolygonAction( aPoly ) );
		else
			ImplDrawPolygon( aPoly, pClipPolyPoly );

		// neues Polygon berechnen
		aRect.Top() = (long)(fScanLine += fScanInc);

		// unteren Rand komplett fuellen
		if ( i == nSteps )
		{
			aTempPoly[0] = aFullRect.BottomLeft();
			aTempPoly[1] = aFullRect.BottomRight();
		}
		else
		{
			aTempPoly[0] = aRect.TopLeft();
			aTempPoly[1] = aRect.TopRight();
		}
		aTempPoly.Rotate( aCenter, nAngle );

		aPoly[0] = aPoly[3];
		aPoly[1] = aPoly[2];
		aPoly[2] = aTempPoly[1];
		aPoly[3] = aTempPoly[0];

		// Farbintensitaeten aendern...
		// fuer lineare FV
		if ( bLinear )
		{
			nRed	= ImplGetGradientColorValue( nStartRed+((nRedSteps*i)/nSteps2) );
			nGreen	= ImplGetGradientColorValue( nStartGreen+((nGreenSteps*i)/nSteps2) );
			nBlue	= ImplGetGradientColorValue( nStartBlue+((nBlueSteps*i)/nSteps2) );
		}
		// fuer radiale FV
		else
		{
			// fuer axiale FV muss die letzte Farbe der ersten
			// Farbe entsprechen
            // #107350# Setting end color one step earlier, as the
            // last time we get here, we drop out of the loop later
            // on.
			if ( i >= nSteps )
			{
				nRed	= (UINT8)nEndRed;
				nGreen	= (UINT8)nEndGreen;
				nBlue	= (UINT8)nEndBlue;
			}
			else
			{
				if ( i <= nStepsHalf )
				{
					nRed	= ImplGetGradientColorValue( nEndRed-((nRedSteps*i)/nSteps2) );
					nGreen	= ImplGetGradientColorValue( nEndGreen-((nGreenSteps*i)/nSteps2) );
					nBlue	= ImplGetGradientColorValue( nEndBlue-((nBlueSteps*i)/nSteps2) );
				}
				// genau die Mitte und hoeher
				else
				{
					long i2 = i - nStepsHalf;
					nRed	= ImplGetGradientColorValue( nStartRed+((nRedSteps*i2)/nSteps2) );
					nGreen	= ImplGetGradientColorValue( nStartGreen+((nGreenSteps*i2)/nSteps2) );
					nBlue	= ImplGetGradientColorValue( nStartBlue+((nBlueSteps*i2)/nSteps2) );
				}
			}
		}

		if ( bMtf )
			;//mpMetaFile->AddAction( new MetaFillColorAction( Color( nRed, nGreen, nBlue ), TRUE ) );
		else
			mpGraphics->SetFillColor( MAKE_SALCOLOR( nRed, nGreen, nBlue ) );
	}
}

// -----------------------------------------------------------------------

void OutputDevice::ImplDrawComplexGradient( const Rectangle& rRect,
										    const Gradient& rGradient,
										    BOOL bMtf, const PolyPolygon* pClipPolyPoly )
{
	// Feststellen ob Ausgabe ueber Polygon oder PolyPolygon
	// Bei Rasteroperationen ungleich Overpaint immer PolyPolygone,
	// da es zu falschen Ergebnissen kommt, wenn man mehrfach uebereinander
	// ausgibt
	// Bei Druckern auch immer PolyPolygone, da nicht alle Drucker
	// das Uebereinanderdrucken von Polygonen koennen
	// Virtuelle Device werden auch ausgeklammert, da einige Treiber
	// ansonsten zu langsam sind
	PolyPolygon*    pPolyPoly;
	Rectangle	    aRect( rRect );
	Color			aStartCol( rGradient.GetStartColor() );
	Color			aEndCol( rGradient.GetEndColor() );
    long			nStartRed = ( (long) aStartCol.GetRed() * rGradient.GetStartIntensity() ) / 100;
    long			nStartGreen = ( (long) aStartCol.GetGreen() * rGradient.GetStartIntensity() ) / 100;
	long			nStartBlue = ( (long) aStartCol.GetBlue() * rGradient.GetStartIntensity() ) / 100;
    long			nEndRed = ( (long) aEndCol.GetRed() * rGradient.GetEndIntensity() ) / 100;
    long			nEndGreen = ( (long) aEndCol.GetGreen() * rGradient.GetEndIntensity() ) / 100;
	long			nEndBlue = ( (long) aEndCol.GetBlue() * rGradient.GetEndIntensity() ) / 100;
	long			nRedSteps = nEndRed	- nStartRed;
	long			nGreenSteps = nEndGreen - nStartGreen;
	long			nBlueSteps = nEndBlue	- nStartBlue;
	long            nStepCount = rGradient.GetSteps();
	USHORT	        nAngle = rGradient.GetAngle() % 3600;
	
    if( (meRasterOp != ROP_OVERPAINT) || bMtf )
		pPolyPoly = new PolyPolygon( 2 );
	else
		pPolyPoly = NULL;

    if( rGradient.GetStyle() == GRADIENT_SQUARE || rGradient.GetStyle() == GRADIENT_RECT )
    {
        const double    fAngle	= nAngle * F_PI1800;
	    const double    fWidth	= aRect.GetWidth();
	    const double    fHeight = aRect.GetHeight();
	    double          fDX = fWidth  * fabs( cos( fAngle ) ) + fHeight * fabs( sin( fAngle ) ); 
	    double          fDY = fHeight * fabs( cos( fAngle ) ) + fWidth  * fabs( sin( fAngle ) ); 

        fDX = ( fDX - fWidth ) * 0.5 + 0.5;
        fDY = ( fDY - fHeight ) * 0.5 + 0.5;
    
	    aRect.Left() -= (long) fDX;
	    aRect.Right() += (long) fDX;
	    aRect.Top() -= (long) fDY;
	    aRect.Bottom() += (long) fDY;
    }
	
    Size aSize( aRect.GetSize() );
	
    if( rGradient.GetStyle() == GRADIENT_RADIAL )
	{
	    // Radien-Berechnung fuer Kreis
		aSize.Width() = (long)(0.5 + sqrt((double)aSize.Width()*(double)aSize.Width() + (double)aSize.Height()*(double)aSize.Height()));
		aSize.Height() = aSize.Width();
	}
	else if( rGradient.GetStyle() == GRADIENT_ELLIPTICAL )
	{
	    // Radien-Berechnung fuer Ellipse
		aSize.Width() = (long)( 0.5 + (double) aSize.Width()  * 1.4142 );
		aSize.Height() = (long)( 0.5 + (double) aSize.Height() * 1.4142 );
	}
    else if( rGradient.GetStyle() == GRADIENT_SQUARE )
    {
		if ( aSize.Width() > aSize.Height() )
			aSize.Height() = aSize.Width();
		else
			aSize.Width() = aSize.Height();
    }

	// neue Mittelpunkte berechnen
	long	nZWidth = aRect.GetWidth()	* (long) rGradient.GetOfsX() / 100;
	long	nZHeight = aRect.GetHeight() * (long) rGradient.GetOfsY() / 100;
	long	nBorderX = (long) rGradient.GetBorder() * aSize.Width()  / 100;
	long	nBorderY = (long) rGradient.GetBorder() * aSize.Height() / 100;
	Point	aCenter( aRect.Left() + nZWidth, aRect.Top() + nZHeight );

	// Rand beruecksichtigen
	aSize.Width() -= nBorderX;
	aSize.Height() -= nBorderY;

	// Ausgaberechteck neu setzen
	aRect.Left() = aCenter.X() - ( aSize.Width() >> 1 );
	aRect.Top() = aCenter.Y() - ( aSize.Height() >> 1 );
    
    aRect.SetSize( aSize );
	long nMinRect = Min( aRect.GetWidth(), aRect.GetHeight() );

	// Anzahl der Schritte berechnen, falls nichts uebergeben wurde
    if( !nStepCount )
	{
		long nInc;

		if ( !bMtf )
        {
			nInc = ( nMinRect < 50 ) ? 2 : 4;
        }
		else
        {
            // #105998# Use display-equivalent step size calculation
			nInc = (nMinRect < 800) ? 10 : 20;
        }

		if( !nInc )
			nInc = 1;

		nStepCount = nMinRect / nInc;
	}
	
    // minimal drei Schritte und maximal die Anzahl der Farbunterschiede
	long nSteps = Max( nStepCount, 2L );
	long nCalcSteps  = Abs( nRedSteps );
	long nTempSteps = Abs( nGreenSteps );
	if ( nTempSteps > nCalcSteps )
		nCalcSteps = nTempSteps;
	nTempSteps = Abs( nBlueSteps );
	if ( nTempSteps > nCalcSteps )
		nCalcSteps = nTempSteps;
	if ( nCalcSteps < nSteps )
		nSteps = nCalcSteps;
	if ( !nSteps )
		nSteps = 1;

	// Ausgabebegrenzungen und Schrittweite fuer jede Richtung festlegen
    Polygon aPoly;
	double  fScanLeft = aRect.Left();
	double  fScanTop = aRect.Top();
	double  fScanRight = aRect.Right();
	double  fScanBottom = aRect.Bottom();
	double  fScanInc = (double) nMinRect / (double) nSteps * 0.5;
    UINT8   nRed = (UINT8) nStartRed, nGreen = (UINT8) nStartGreen, nBlue = (UINT8) nStartBlue;
    bool	bPaintLastPolygon( false ); // #107349# Paint last polygon only if loop has generated any output

	if( bMtf )
		;//mpMetaFile->AddAction( new MetaFillColorAction( Color( nRed, nGreen, nBlue ), TRUE ) );
	else
		mpGraphics->SetFillColor( MAKE_SALCOLOR( nRed, nGreen, nBlue ) );

	if( pPolyPoly )
	{   
    	pPolyPoly->Insert( aPoly = rRect );
		pPolyPoly->Insert( aPoly );
	}
	else
    {
	    // extend rect, to avoid missing bounding line
        Rectangle aExtRect( rRect );

        aExtRect.Left() -= 1;
	    aExtRect.Top() -= 1;
	    aExtRect.Right() += 1;
	    aExtRect.Bottom() += 1;

		ImplDrawPolygon( aPoly = aExtRect, pClipPolyPoly );
    }

	// Schleife, um nacheinander die Polygone/PolyPolygone auszugeben
	for( long i = 1; i < nSteps; i++ )
	{
		// neues Polygon berechnen
		aRect.Left() = (long)( fScanLeft += fScanInc );
		aRect.Top() = (long)( fScanTop += fScanInc );
		aRect.Right() = (long)( fScanRight -= fScanInc );
		aRect.Bottom() = (long)( fScanBottom -= fScanInc );

		if( ( aRect.GetWidth() < 2 ) || ( aRect.GetHeight() < 2 ) )
			break;

        if( rGradient.GetStyle() == GRADIENT_RADIAL || rGradient.GetStyle() == GRADIENT_ELLIPTICAL )
		    aPoly = Polygon( aRect.Center(), aRect.GetWidth() >> 1, aRect.GetHeight() >> 1 );
        else
    		aPoly = Polygon( aRect );

		aPoly.Rotate( aCenter, nAngle );

		// Farbe entsprechend anpassen
        const long nStepIndex = ( ( pPolyPoly != NULL ) ? i : ( i + 1 ) );
        nRed = ImplGetGradientColorValue( nStartRed + ( ( nRedSteps * nStepIndex ) / nSteps ) );
		nGreen = ImplGetGradientColorValue( nStartGreen + ( ( nGreenSteps * nStepIndex ) / nSteps ) );
		nBlue = ImplGetGradientColorValue( nStartBlue + ( ( nBlueSteps * nStepIndex ) / nSteps ) );

		// entweder langsame PolyPolygon-Ausgaben oder schnelles Polygon-Painting
		if( pPolyPoly )
		{
            bPaintLastPolygon = true; // #107349# Paint last polygon only if loop has generated any output

			pPolyPoly->Replace( pPolyPoly->GetObject( 1 ), 0 );
			pPolyPoly->Replace( aPoly, 1 );

			if( bMtf )
				;//mpMetaFile->AddAction( new MetaPolyPolygonAction( *pPolyPoly ) );
			else
				ImplDrawPolyPolygon( *pPolyPoly, pClipPolyPoly );

            // #107349# Set fill color _after_ geometry painting:
            // pPolyPoly's geometry is the band from last iteration's
            // aPoly to current iteration's aPoly. The window outdev
            // path (see else below), on the other hand, paints the
            // full aPoly. Thus, here, we're painting the band before
            // the one painted in the window outdev path below. To get
            // matching colors, have to delay color setting here.
            if( bMtf )
                ;//mpMetaFile->AddAction( new MetaFillColorAction( Color( nRed, nGreen, nBlue ), TRUE ) );
            else
                mpGraphics->SetFillColor( MAKE_SALCOLOR( nRed, nGreen, nBlue ) );
		}
		else
        {
            // #107349# Set fill color _before_ geometry painting
            if( bMtf )
                ;//mpMetaFile->AddAction( new MetaFillColorAction( Color( nRed, nGreen, nBlue ), TRUE ) );
            else
                mpGraphics->SetFillColor( MAKE_SALCOLOR( nRed, nGreen, nBlue ) );

			ImplDrawPolygon( aPoly, pClipPolyPoly );
        }
	}

	// Falls PolyPolygon-Ausgabe, muessen wir noch ein letztes inneres Polygon zeichnen
	if( pPolyPoly )
	{
		const Polygon& rPoly = pPolyPoly->GetObject( 1 );
		
        if( !rPoly.GetBoundRect().IsEmpty() )
		{
            // #107349# Paint last polygon with end color only if loop
            // has generated output. Otherwise, the current
            // (i.e. start) color is taken, to generate _any_ output.
            if( bPaintLastPolygon )
            {
                nRed = ImplGetGradientColorValue( nEndRed );
                nGreen = ImplGetGradientColorValue( nEndGreen );
                nBlue = ImplGetGradientColorValue( nEndBlue );
            }

    		if( bMtf )
            {
	    		;//mpMetaFile->AddAction( new MetaFillColorAction( Color( nRed, nGreen, nBlue ), TRUE ) );
				;//mpMetaFile->AddAction( new MetaPolygonAction( rPoly ) );
            }
		    else
            {
			    mpGraphics->SetFillColor( MAKE_SALCOLOR( nRed, nGreen, nBlue ) );
   				ImplDrawPolygon( rPoly, pClipPolyPoly );
            }
		}

		delete pPolyPoly;
	}
}
void OutputDevice::DrawHatch( const PolyPolygon& rPolyPoly, const ::Hatch& rHatch )
{
	Hatch aHatch( rHatch );

	if( !mpGraphics )
		return;

	if( mbInitClipRegion )
		ImplInitClipRegion();

	if( mbOutputClipped )
		return;

	if( rPolyPoly.Count() )
	{ 
		PolyPolygon		aPolyPoly( LogicToPixel( rPolyPoly ) );
		BOOL			bOldMap = mbMap;

		aPolyPoly.Optimize( POLY_OPTIMIZE_NO_SAME );
        aHatch.SetDistance( ImplLogicWidthToDevicePixel( aHatch.GetDistance() ) );

		EnableMapMode( FALSE );
		Push( PUSH_LINECOLOR );
		SetLineColor( aHatch.GetColor() );
		ImplInitLineColor();
		ImplDrawHatch( aPolyPoly, aHatch, FALSE );
		Pop();
		EnableMapMode( bOldMap );
	}
}
void OutputDevice::ImplDrawHatch( const PolyPolygon& rPolyPoly, const Hatch& rHatch, BOOL bMtf )
{
	Rectangle	aRect( rPolyPoly.GetBoundRect() );
	const long	nLogPixelWidth = ImplDevicePixelToLogicWidth( 1 );
	const long	nWidth = ImplDevicePixelToLogicWidth( Max( ImplLogicWidthToDevicePixel( rHatch.GetDistance() ), 3L ) );
	Point*		pPtBuffer = new Point[ HATCH_MAXPOINTS ];
	Point		aPt1, aPt2, aEndPt1;
	Size		aInc;

	// Single hatch
	aRect.Left() -= nLogPixelWidth; aRect.Top() -= nLogPixelWidth; aRect.Right() += nLogPixelWidth; aRect.Bottom() += nLogPixelWidth;
	ImplCalcHatchValues( aRect, nWidth, rHatch.GetAngle(), aPt1, aPt2, aInc, aEndPt1 );
	do
	{
		ImplDrawHatchLine( Line( aPt1, aPt2 ), rPolyPoly, pPtBuffer, bMtf );
		aPt1.X() += aInc.Width(); aPt1.Y() += aInc.Height();
		aPt2.X() += aInc.Width(); aPt2.Y() += aInc.Height();
	}
	while( ( aPt1.X() <= aEndPt1.X() ) && ( aPt1.Y() <= aEndPt1.Y() ) );

	if( ( rHatch.GetStyle() == HATCH_DOUBLE ) || ( rHatch.GetStyle() == HATCH_TRIPLE ) )
	{
		// Double hatch
		ImplCalcHatchValues( aRect, nWidth, rHatch.GetAngle() + 900, aPt1, aPt2, aInc, aEndPt1 );
		do
		{
			ImplDrawHatchLine( Line( aPt1, aPt2 ), rPolyPoly, pPtBuffer, bMtf );
			aPt1.X() += aInc.Width(); aPt1.Y() += aInc.Height();
			aPt2.X() += aInc.Width(); aPt2.Y() += aInc.Height();
		}
		while( ( aPt1.X() <= aEndPt1.X() ) && ( aPt1.Y() <= aEndPt1.Y() ) );

		if( rHatch.GetStyle() == HATCH_TRIPLE )
		{
			// Triple hatch
			ImplCalcHatchValues( aRect, nWidth, rHatch.GetAngle() + 450, aPt1, aPt2, aInc, aEndPt1 );
			do
			{
				ImplDrawHatchLine( Line( aPt1, aPt2 ), rPolyPoly, pPtBuffer, bMtf );
				aPt1.X() += aInc.Width(); aPt1.Y() += aInc.Height();
				aPt2.X() += aInc.Width(); aPt2.Y() += aInc.Height();
			}
			while( ( aPt1.X() <= aEndPt1.X() ) && ( aPt1.Y() <= aEndPt1.Y() ) );
		}
	}

	delete[] pPtBuffer;
}

// -----------------------------------------------------------------------
void OutputDevice::ImplCalcHatchValues( const Rectangle& rRect, long nDist, USHORT nAngle10,
										Point& rPt1, Point& rPt2, Size& rInc, Point& rEndPt1 )
{
	Point	aRef;
	long	nAngle = nAngle10 % 1800;
	long	nOffset = 0;

	if( nAngle > 900 )
		nAngle -= 1800;

	aRef = ( !IsRefPoint() ? rRect.TopLeft() : GetRefPoint() );

	if( 0 == nAngle )
	{
		rInc = Size( 0, nDist );
		rPt1 = rRect.TopLeft();
		rPt2 = rRect.TopRight();
		rEndPt1 = rRect.BottomLeft();

		if( aRef.Y() <= rRect.Top() )
			nOffset = ( ( rRect.Top() - aRef.Y() ) % nDist );
		else
			nOffset = ( nDist - ( ( aRef.Y() - rRect.Top() ) % nDist ) );

		rPt1.Y() -= nOffset;
		rPt2.Y() -= nOffset;
	}
	else if( 900 == nAngle )
	{
		rInc = Size( nDist, 0 );
		rPt1 = rRect.TopLeft();
		rPt2 = rRect.BottomLeft();
		rEndPt1 = rRect.TopRight();

		if( aRef.X() <= rRect.Left() )
			nOffset = ( rRect.Left() - aRef.X() ) % nDist;
		else
			nOffset = nDist - ( ( aRef.X() - rRect.Left() ) % nDist );

		rPt1.X() -= nOffset;
		rPt2.X() -= nOffset;
	}
	else if( nAngle >= -450 && nAngle <= 450 )
	{
		const double	fAngle = F_PI1800 * labs( nAngle );
		const double	fTan = tan( fAngle );
		const long		nYOff = FRound( ( rRect.Right() - rRect.Left() ) * fTan );
		long			nPY;

		rInc = Size( 0, nDist = FRound( nDist / cos( fAngle ) ) );

		if( nAngle > 0 )
		{
			rPt1 = rRect.TopLeft();
			rPt2 = Point( rRect.Right(), rRect.Top() - nYOff );
			rEndPt1 = Point( rRect.Left(), rRect.Bottom() + nYOff );
			nPY = FRound( aRef.Y() - ( ( rPt1.X() - aRef.X() ) * fTan ) );
		}
		else
		{
			rPt1 = rRect.TopRight();
			rPt2 = Point( rRect.Left(), rRect.Top() - nYOff );
			rEndPt1 = Point( rRect.Right(), rRect.Bottom() + nYOff );
			nPY = FRound( aRef.Y() + ( ( rPt1.X() - aRef.X() ) * fTan ) );
		}

		if( nPY <= rPt1.Y() )
			nOffset = ( rPt1.Y() - nPY ) % nDist;
		else
			nOffset = nDist - ( ( nPY - rPt1.Y() ) % nDist );

		rPt1.Y() -= nOffset;
		rPt2.Y() -= nOffset;
	}
	else
	{
		const double fAngle = F_PI1800 * labs( nAngle );
		const double fTan = tan( fAngle );
		const long	 nXOff = FRound( ( rRect.Bottom() - rRect.Top() ) / fTan );
		long		 nPX;

		rInc = Size( nDist = FRound( nDist / sin( fAngle ) ), 0 );

		if( nAngle > 0 )
		{
			rPt1 = rRect.TopLeft();
			rPt2 = Point( rRect.Left() - nXOff, rRect.Bottom() );
			rEndPt1 = Point( rRect.Right() + nXOff, rRect.Top() );
			nPX = FRound( aRef.X() - ( ( rPt1.Y() - aRef.Y() ) / fTan ) );
		}
		else
		{
			rPt1 = rRect.BottomLeft();
			rPt2 = Point( rRect.Left() - nXOff, rRect.Top() );
			rEndPt1 = Point( rRect.Right() + nXOff, rRect.Bottom() );
			nPX = FRound( aRef.X() + ( ( rPt1.Y() - aRef.Y() ) / fTan ) );
		}

		if( nPX <= rPt1.X() )
			nOffset = ( rPt1.X() - nPX ) % nDist;
		else
			nOffset = nDist - ( ( nPX - rPt1.X() ) % nDist );

		rPt1.X() -= nOffset;
		rPt2.X() -= nOffset;
	}
}
// ------------------------------------------------------------------------
void OutputDevice::ImplDrawHatchLine( const Line& rLine, const PolyPolygon& rPolyPoly,
									  Point* pPtBuffer, BOOL bMtf )
{
	double	fX, fY;
	long	nAdd, nPCounter = 0;

	for( long nPoly = 0, nPolyCount = rPolyPoly.Count(); nPoly < nPolyCount; nPoly++ )
	{
		const Polygon& rPoly = rPolyPoly[ (USHORT) nPoly ];

		if( rPoly.GetSize() > 1 )
		{
			Line	aCurSegment( rPoly[ 0 ], Point() );

			for( long i = 1, nCount = rPoly.GetSize(); i <= nCount; i++ )
			{
				aCurSegment.SetEnd( rPoly[ (USHORT)( i % nCount ) ] );
				nAdd = 0;

				if( rLine.Intersection( aCurSegment, fX, fY ) )
				{
					if( ( fabs( fX - aCurSegment.GetStart().X() ) <= 0.0000001 ) && 
						( fabs( fY - aCurSegment.GetStart().Y() ) <= 0.0000001 ) )
					{
						const Line		aPrevSegment( rPoly[ (USHORT)( ( i > 1 ) ? ( i - 2 ) : ( nCount - 1 ) ) ], aCurSegment.GetStart() );
						const double	fPrevDistance = rLine.GetDistance( aPrevSegment.GetStart() );
						const double	fCurDistance = rLine.GetDistance( aCurSegment.GetEnd() );

						if( ( fPrevDistance <= 0.0 && fCurDistance > 0.0 ) || 
							( fPrevDistance > 0.0 && fCurDistance < 0.0 ) )
						{
							nAdd = 1;
						}
					}
					else if( ( fabs( fX - aCurSegment.GetEnd().X() ) <= 0.0000001 ) && 
							 ( fabs( fY - aCurSegment.GetEnd().Y() ) <= 0.0000001 ) )
					{
						const Line aNextSegment( aCurSegment.GetEnd(), rPoly[ (USHORT)( ( i + 1 ) % nCount ) ] );

						if( ( fabs( rLine.GetDistance( aNextSegment.GetEnd() ) ) <= 0.0000001 ) && 
							( rLine.GetDistance( aCurSegment.GetStart() ) > 0.0 ) )
						{
							nAdd = 1;
						}
					}
					else
						nAdd = 1;

					if( nAdd )
						pPtBuffer[ nPCounter++ ] = Point( FRound( fX ), FRound( fY ) );
				}

				aCurSegment.SetStart( aCurSegment.GetEnd() );
			}
		}
	}

	if( nPCounter > 1 )
	{
		qsort( pPtBuffer, nPCounter, sizeof( Point ), ImplHatchCmpFnc );

		if( nPCounter & 1 )
			nPCounter--;

		if( bMtf )
		{
#ifdef AVS
			for( long i = 0; i < nPCounter; i += 2 )
				mpMetaFile->AddAction( new MetaLineAction( pPtBuffer[ i ], pPtBuffer[ i + 1 ] ) );
#endif
		}
		else
		{
			for( long i = 0; i < nPCounter; i += 2 )
			{
#ifdef AVS
                if( mpPDFWriter )
                {
                    mpPDFWriter->drawLine( pPtBuffer[ i ], pPtBuffer[ i+1 ] );
                }
                else
                {
#endif
                    const Point aPt1( ImplLogicToDevicePixel( pPtBuffer[ i ] ) );
                    const Point aPt2( ImplLogicToDevicePixel( pPtBuffer[ i + 1 ] ) );
                    mpGraphics->DrawLine( aPt1.X(), aPt1.Y(), aPt2.X(), aPt2.Y(), this );
#ifdef AVS
                }
#endif
			}
		}
	}
}

void OutputDevice::DrawWallpaper( const Rectangle& rRect, const Wallpaper& rWallpaper )
{
	if ( rWallpaper.GetStyle() != WALLPAPER_NULL )
	{
		Rectangle aRect = LogicToPixel( rRect );
		aRect.Justify();

		if ( !aRect.IsEmpty() )
		{
			ImplDrawWallpaper( aRect.Left(), aRect.Top(), aRect.GetWidth(), aRect.GetHeight(),
							   rWallpaper );
		}
	}
}

void OutputDevice::ImplDrawWallpaper( long nX, long nY,
									  long nWidth, long nHeight,
									  const Wallpaper& rWallpaper )
{
	if( rWallpaper.IsBitmap() )
		ImplDrawBitmapWallpaper( nX, nY, nWidth, nHeight, rWallpaper );
	else if( rWallpaper.IsGradient() )
		ImplDrawGradientWallpaper( nX, nY, nWidth, nHeight, rWallpaper );
	else
		ImplDrawColorWallpaper(  nX, nY, nWidth, nHeight, rWallpaper );
}
void OutputDevice::ImplDrawBitmapWallpaper( long nX, long nY,
											long nWidth, long nHeight,
											const Wallpaper& rWallpaper )
{
	BitmapEx				aBmpEx;
	const BitmapEx* 		pCached = rWallpaper.ImplGetImpWallpaper()->ImplGetCachedBitmap();
	Point					aPos;
	Size					aSize;
	const WallpaperStyle	eStyle = rWallpaper.GetStyle();
	const BOOL				bOldMap = mbMap;
	BOOL					bDrawn = FALSE;
	BOOL					bDrawGradientBackground = FALSE;
	BOOL					bDrawColorBackground = FALSE;

	if( pCached )
		aBmpEx = *pCached;
	else
		aBmpEx = rWallpaper.GetBitmap();

	const long nBmpWidth = aBmpEx.GetSizePixel().Width();
	const long nBmpHeight = aBmpEx.GetSizePixel().Height();
	const BOOL bTransparent = aBmpEx.IsTransparent();

	// draw background
	if( bTransparent )
	{
		if( rWallpaper.IsGradient() )
			bDrawGradientBackground = TRUE;
		else
		{
			if( !pCached && !rWallpaper.GetColor().GetTransparency() )
			{
				VirtualDevice aVDev( *this );
				aVDev.SetBackground( rWallpaper.GetColor() );
				aVDev.SetOutputSizePixel( Size( nBmpWidth, nBmpHeight ) );
				aVDev.DrawBitmapEx( Point(), aBmpEx );
				aBmpEx = aVDev.GetBitmap( Point(), aVDev.GetOutputSizePixel() );
			}

			bDrawColorBackground = TRUE;
		}
	}
	else if( eStyle != WALLPAPER_TILE && eStyle != WALLPAPER_SCALE )
	{
		if( rWallpaper.IsGradient() )
			bDrawGradientBackground = TRUE;
		else
			bDrawColorBackground = TRUE;
	}

	// background of bitmap?
	if( bDrawGradientBackground )
		ImplDrawGradientWallpaper( nX, nY, nWidth, nHeight, rWallpaper );
	else if( bDrawColorBackground && bTransparent )
	{
		ImplDrawColorWallpaper( nX, nY, nWidth, nHeight, rWallpaper );
		bDrawColorBackground = FALSE;
	}

	// calc pos and size
	if( rWallpaper.IsRect() )
	{
		const Rectangle aBound( LogicToPixel( rWallpaper.GetRect() ) );
		aPos = aBound.TopLeft();
		aSize = aBound.GetSize();
	}
	else
	{
		aPos = Point( nX, nY );
		aSize = Size( nWidth, nHeight );
	}

	EnableMapMode( FALSE );
	Push( PUSH_CLIPREGION );
	IntersectClipRegion( Rectangle( Point( nX, nY ), Size( nWidth, nHeight ) ) );

	switch( eStyle )
	{
		case( WALLPAPER_SCALE ):
		{
			if( !pCached || ( pCached->GetSizePixel() != aSize ) )
			{
				if( pCached )
					rWallpaper.ImplGetImpWallpaper()->ImplReleaseCachedBitmap();

				aBmpEx = rWallpaper.GetBitmap();
				aBmpEx.Scale( aSize );
				aBmpEx = BitmapEx( aBmpEx.GetBitmap().CreateDisplayBitmap( this ), aBmpEx.GetMask() );
			}
		}
		break;

		case( WALLPAPER_TOPLEFT ):
		break;

		case( WALLPAPER_TOP ):
			aPos.X() += ( aSize.Width() - nBmpWidth ) >> 1;
		break;

		case( WALLPAPER_TOPRIGHT ):
			aPos.X() += ( aSize.Width() - nBmpWidth );
		break;

		case( WALLPAPER_LEFT ):
			aPos.Y() += ( aSize.Height() - nBmpHeight ) >> 1;
		break;

		case( WALLPAPER_CENTER ):
		{
			aPos.X() += ( aSize.Width() - nBmpWidth ) >> 1;
			aPos.Y() += ( aSize.Height() - nBmpHeight ) >> 1;
		}
		break;

		case( WALLPAPER_RIGHT ):
		{
			aPos.X() += ( aSize.Width() - nBmpWidth );
			aPos.Y() += ( aSize.Height() - nBmpHeight ) >> 1;
		}
		break;

		case( WALLPAPER_BOTTOMLEFT ):
			aPos.Y() += ( aSize.Height() - nBmpHeight );
		break;

		case( WALLPAPER_BOTTOM ):
		{
			aPos.X() += ( aSize.Width() - nBmpWidth ) >> 1;
			aPos.Y() += ( aSize.Height() - nBmpHeight );
		}
		break;

		case( WALLPAPER_BOTTOMRIGHT ):
		{
			aPos.X() += ( aSize.Width() - nBmpWidth );
			aPos.Y() += ( aSize.Height() - nBmpHeight );
		}
		break;

		default:
		{
			const long	nRight = nX + nWidth - 1L;
			const long	nBottom = nY + nHeight - 1L;
			long		nFirstX;
			long		nFirstY;

			if( eStyle == WALLPAPER_TILE )
			{
				nFirstX = aPos.X();
				nFirstY = aPos.Y();
			}
			else
			{
				nFirstX = aPos.X() + ( ( aSize.Width() - nBmpWidth ) >> 1 );
				nFirstY = aPos.Y() + ( ( aSize.Height() - nBmpHeight ) >> 1 );
			}

			const long	nOffX = ( nFirstX - nX ) % nBmpWidth;
			const long	nOffY = ( nFirstY - nY ) % nBmpHeight;
			long		nStartX = nX + nOffX;
			long		nStartY = nY + nOffY;

			if( nOffX > 0L )
				nStartX -= nBmpWidth;

			if( nOffY > 0L )
				nStartY -= nBmpHeight;

			for( long nBmpY = nStartY; nBmpY <= nBottom; nBmpY += nBmpHeight )
				for( long nBmpX = nStartX; nBmpX <= nRight; nBmpX += nBmpWidth )
					DrawBitmapEx( Point( nBmpX, nBmpY ), aBmpEx );

			bDrawn = TRUE;
		}
		break;
	}

	if( !bDrawn )
	{
		// optimized for non-transparent bitmaps
		if( bDrawColorBackground )
		{
			const Size		aBmpSize( aBmpEx.GetSizePixel() );
			const Point 	aTmpPoint;
			const Rectangle aOutRect( aTmpPoint, GetOutputSizePixel() );
			const Rectangle aColRect( Point( nX, nY ), Size( nWidth, nHeight ) );
			Rectangle		aWorkRect;

			aWorkRect = Rectangle( 0, 0, aOutRect.Right(), aPos.Y() - 1L );
			aWorkRect.Justify();
			aWorkRect.Intersection( aColRect );
			if( !aWorkRect.IsEmpty() )
			{
				ImplDrawColorWallpaper( aWorkRect.Left(), aWorkRect.Top(),
										aWorkRect.GetWidth(), aWorkRect.GetHeight(),
										rWallpaper );
			}

			aWorkRect = Rectangle( 0, aPos.Y(), aPos.X() - 1L, aPos.Y() + aBmpSize.Height() - 1L );
			aWorkRect.Justify();
			aWorkRect.Intersection( aColRect );
			if( !aWorkRect.IsEmpty() )
			{
				ImplDrawColorWallpaper( aWorkRect.Left(), aWorkRect.Top(),
										aWorkRect.GetWidth(), aWorkRect.GetHeight(),
										rWallpaper );
			}

			aWorkRect = Rectangle( aPos.X() + aBmpSize.Width(), aPos.Y(), aOutRect.Right(), aPos.Y() + aBmpSize.Height() - 1L );
			aWorkRect.Justify();
			aWorkRect.Intersection( aColRect );
			if( !aWorkRect.IsEmpty() )
			{
				ImplDrawColorWallpaper( aWorkRect.Left(), aWorkRect.Top(),
										aWorkRect.GetWidth(), aWorkRect.GetHeight(),
										rWallpaper );
			}

			aWorkRect = Rectangle( 0, aPos.Y() + aBmpSize.Height(), aOutRect.Right(), aOutRect.Bottom() );
			aWorkRect.Justify();
			aWorkRect.Intersection( aColRect );
			if( !aWorkRect.IsEmpty() )
			{
				ImplDrawColorWallpaper( aWorkRect.Left(), aWorkRect.Top(),
										aWorkRect.GetWidth(), aWorkRect.GetHeight(),
										rWallpaper );
			}
		}

		DrawBitmapEx( aPos, aBmpEx );
	}

	rWallpaper.ImplGetImpWallpaper()->ImplSetCachedBitmap( aBmpEx );

	Pop();
	EnableMapMode( bOldMap );
}

void OutputDevice::ImplDrawGradientWallpaper( long nX, long nY,
											  long nWidth, long nHeight,
											  const Wallpaper& rWallpaper )
{
	Rectangle		aBound;
	const BOOL		bOldMap = mbMap;
    BOOL            bNeedGradient = TRUE;

	if ( rWallpaper.IsRect() )
		aBound = LogicToPixel( rWallpaper.GetRect() );
	else
		aBound = Rectangle( Point( nX, nY ), Size( nWidth, nHeight ) );

	EnableMapMode( FALSE );
	Push( PUSH_CLIPREGION );
	IntersectClipRegion( Rectangle( Point( nX, nY ), Size( nWidth, nHeight ) ) );

    if( bNeedGradient )
	    DrawGradient( aBound, rWallpaper.GetGradient() );

	Pop();
	EnableMapMode( bOldMap );
}
void OutputDevice::ImplDrawColorWallpaper( long nX, long nY,
										   long nWidth, long nHeight,
										   const Wallpaper& rWallpaper )
{
	// Wallpaper ohne Umrandung zeichnen
	Color aOldLineColor = GetLineColor();
	Color aOldFillColor = GetFillColor();
	SetLineColor();
	SetFillColor( rWallpaper.GetColor() );
    BOOL bMap = mbMap;
    EnableMapMode( FALSE );
    DrawRect( Rectangle( Point( nX, nY ), Size( nWidth, nHeight ) ) );
	SetLineColor( aOldLineColor );
	SetFillColor( aOldFillColor );
    EnableMapMode( bMap );
}
void OutputDevice::DrawTransparent( const PolyPolygon& rPolyPoly, USHORT nTransparencePercent )
{
    bool bDrawn = false;
	// short circuit for drawing an opaque polygon
	if( (nTransparencePercent < 1) )
	{
		DrawPolyPolygon( rPolyPoly );
		return;
	}

	// short circut for drawing an invisible polygon
	if( !mbFillColor || (nTransparencePercent >= 100) )
	{
		// short circuit if the polygon border is invisible too
		if( !mbLineColor )
			return;

		// DrawTransparent() assumes that the border is NOT to be drawn transparently???
		Push( PUSH_FILLCOLOR );
		SetFillColor();
		DrawPolyPolygon( rPolyPoly );
		Pop();
		return;
	}


	// get the device graphics as drawing target
	if( !mpGraphics )
			return;

    // debug helper:
    static const char* pDisableNative = getenv( "SAL_DISABLE_NATIVE_ALPHA");

	if( 1 )
	{

		{
 			PolyPolygon 	aPolyPoly( LogicToPixel( rPolyPoly ) );
			Rectangle		aPolyRect( aPolyPoly.GetBoundRect() );
			Point			aPoint;
			Rectangle		aDstRect( aPoint, GetOutputSizePixel() );

			aDstRect.Intersection( aPolyRect );

			if( !aDstRect.IsEmpty() )
			{
                // #i66849# Added fast path for exactly rectangular
                // polygons
                // #i83087# Naturally, system alpha blending cannot
                // work with separate alpha VDev
                if( !pDisableNative && aPolyPoly.IsRect() )
                {
                    // setup Graphics only here (other cases delegate
                    // to basic OutDev methods)
                    if( 1 )
                    {
                        if ( mbInitClipRegion )
                            ImplInitClipRegion();        
                        if ( mbInitLineColor )
                            ImplInitLineColor();
                        if ( mbInitFillColor )
                            ImplInitFillColor();
    
                        Rectangle aLogicPolyRect( rPolyPoly.GetBoundRect() );
                        Rectangle aPixelRect( ImplLogicToDevicePixel( aLogicPolyRect ) );

                        if( !mbOutputClipped )
                        {
                            bDrawn = mpGraphics->DrawAlphaRect( 
                               aPixelRect.Left(), aPixelRect.Top(), 
                               // #i98405# use methods with small g, else one pixel too much will be painted.
                               // This is because the source is a polygon which when painted would not paint
                               // the rightmost and lowest pixel line(s), so use one pixel less for the 
                               // rectangle, too.
                               aPixelRect.getWidth(), aPixelRect.getHeight(),
                               static_cast<sal_uInt8>(nTransparencePercent), 
                               this );
                        }
                        else
                            bDrawn = true;
                    }
                }

                if( !bDrawn )
                {
                    //VirtualDevice	aVDev( *this, 1 );
                    VirtualDevice	aVDev( *this );
                    const Size		aDstSz( aDstRect.GetSize() );
                    const BYTE		cTrans = (BYTE) MinMax( FRound( nTransparencePercent * 2.55 ), 0, 255 );

                    if( aDstRect.Left() || aDstRect.Top() )
                        aPolyPoly.Move( -aDstRect.Left(), -aDstRect.Top() );

                    if( aVDev.SetOutputSizePixel( aDstSz ) )
                    {
                        const BOOL bOldMap = mbMap;

                        EnableMapMode( FALSE );

                        aVDev.SetLineColor( COL_BLACK );
                        aVDev.SetFillColor( COL_BLACK );
                        aVDev.DrawPolyPolygon( aPolyPoly );

                        Bitmap				aPaint( GetBitmap( aDstRect.TopLeft(), aDstSz ) );
                        Bitmap				aPolyMask( aVDev.GetBitmap( Point(), aDstSz ) );

                        // #107766# check for non-empty bitmaps before accessing them
                        if( !!aPaint && !!aPolyMask )
                        {
                            BitmapWriteAccess*	pW = aPaint.AcquireWriteAccess();
                            BitmapReadAccess*	pR = aPolyMask.AcquireReadAccess();

                            if( pW && pR )
                            {
                                BitmapColor 		aPixCol;
                                const BitmapColor	aFillCol( GetFillColor() );
                                const BitmapColor	aWhite( pR->GetBestMatchingColor( Color( COL_WHITE ) ) );
                                const BitmapColor	aBlack( pR->GetBestMatchingColor( Color( COL_BLACK ) ) );
                                const long			nWidth = pW->Width(), nHeight = pW->Height();
                                const long			nR = aFillCol.GetRed(), nG = aFillCol.GetGreen(), nB = aFillCol.GetBlue();
                                long				nX, nY;

                                if( aPaint.GetBitCount() <= 8 )
                                {
                                    const BitmapPalette&	rPal = pW->GetPalette();
                                    const USHORT			nCount = rPal.GetEntryCount();
                                    BitmapColor*			pMap = (BitmapColor*) new BYTE[ nCount * sizeof( BitmapColor ) ];

                                    for( USHORT i = 0; i < nCount; i++ )
                                    {
                                        BitmapColor aCol( rPal[ i ] );
                                        pMap[ i ] = BitmapColor( (BYTE) rPal.GetBestIndex( aCol.Merge( aFillCol, cTrans ) ) );
                                    }

                                    if( pR->GetScanlineFormat() == BMP_FORMAT_1BIT_MSB_PAL &&
                                        pW->GetScanlineFormat() == BMP_FORMAT_8BIT_PAL )
                                    {
                                        const BYTE cBlack = aBlack.GetIndex();

                                        for( nY = 0; nY < nHeight; nY++ )
                                        {
                                            Scanline	pWScan = pW->GetScanline( nY );
                                            Scanline	pRScan = pR->GetScanline( nY );
                                            BYTE		cBit = 128;

                                            for( nX = 0; nX < nWidth; nX++, cBit >>= 1, pWScan++ )
                                            {
                                                if( !cBit )
                                                    cBit = 128, pRScan++;

                                                if( ( *pRScan & cBit ) == cBlack )
                                                    *pWScan = (BYTE) pMap[ *pWScan ].GetIndex();
                                            }
                                        }
                                    }
                                    else
                                    {
                                        for( nY = 0; nY < nHeight; nY++ )
                                            for( nX = 0; nX < nWidth; nX++ )
                                                if( pR->GetPixel( nY, nX ) == aBlack )
                                                    pW->SetPixel( nY, nX, pMap[ pW->GetPixel( nY, nX ).GetIndex() ] );
                                    }

                                    delete[] (BYTE*) pMap;
                                }
                                else
                                {
                                    if( pR->GetScanlineFormat() == BMP_FORMAT_1BIT_MSB_PAL &&
                                        pW->GetScanlineFormat() == BMP_FORMAT_24BIT_TC_BGR )
                                    {
                                        const BYTE cBlack = aBlack.GetIndex();

                                        for( nY = 0; nY < nHeight; nY++ )
                                        {
                                            Scanline	pWScan = pW->GetScanline( nY );
                                            Scanline	pRScan = pR->GetScanline( nY );
                                            BYTE		cBit = 128;

                                            for( nX = 0; nX < nWidth; nX++, cBit >>= 1, pWScan += 3 )
                                            {
                                                if( !cBit )
                                                    cBit = 128, pRScan++;

                                                if( ( *pRScan & cBit ) == cBlack )
                                                {
                                                    pWScan[ 0 ] = COLOR_CHANNEL_MERGE( pWScan[ 0 ], nB, cTrans );
                                                    pWScan[ 1 ] = COLOR_CHANNEL_MERGE( pWScan[ 1 ], nG, cTrans );
                                                    pWScan[ 2 ] = COLOR_CHANNEL_MERGE( pWScan[ 2 ], nR, cTrans );
                                                }
                                            }
                                        }
                                    }
                                    else
                                    {
                                        for( nY = 0; nY < nHeight; nY++ )
                                        {
                                            for( nX = 0; nX < nWidth; nX++ )
                                            {
                                                if( pR->GetPixel( nY, nX ) == aBlack )
                                                {
                                                    aPixCol = pW->GetColor( nY, nX );
                                                    pW->SetPixel( nY, nX, aPixCol.Merge( aFillCol, cTrans ) );
                                                }
                                            }
                                        }
                                    }
                                }
                            }

                            aPolyMask.ReleaseAccess( pR );
                            aPaint.ReleaseAccess( pW );

                            DrawBitmap( aDstRect.TopLeft(), aPaint );

                            EnableMapMode( bOldMap );

                            if( mbLineColor )
                            {
                                Push( PUSH_FILLCOLOR );
                                SetFillColor();
                                DrawPolyPolygon( rPolyPoly );
                                Pop();
                            }
                        }
                    }
                    else
                        DrawPolyPolygon( rPolyPoly );
                }
            }
        }
	}
}
void OutputDevice::DrawTransparent( const GDIMetaFile& rMtf, const Point& rPos, const Size& rSize, const Gradient& rTransparenceGradient )
{
	const Color aBlack( COL_BLACK );

	if( ( rTransparenceGradient.GetStartColor() == aBlack && rTransparenceGradient.GetEndColor() == aBlack ) )
	{
		( (GDIMetaFile&) rMtf ).WindStart();
		( (GDIMetaFile&) rMtf ).Play( this, rPos, rSize );
		( (GDIMetaFile&) rMtf ).WindStart();
	}
	else
	{
		Rectangle		aOutRect( LogicToPixel( rPos ), LogicToPixel( rSize ) );
		Point			aPoint;
		Rectangle		aDstRect( aPoint, GetOutputSizePixel() );

		aDstRect.Intersection( aOutRect );

		if( !aDstRect.IsEmpty() )
		{
			VirtualDevice* pVDev = new VirtualDevice;

			((OutputDevice*)pVDev)->mnDPIX = mnDPIX;
			((OutputDevice*)pVDev)->mnDPIY = mnDPIY;

			if( pVDev->SetOutputSizePixel( aDstRect.GetSize() ) )
			{
				Bitmap		aPaint, aMask;
				AlphaMask	aAlpha;
				MapMode 	aMap( GetMapMode() );
				Point		aOutPos( PixelToLogic( aDstRect.TopLeft() ) );
				const BOOL	bOldMap = mbMap;

				aMap.SetOrigin( Point( -aOutPos.X(), -aOutPos.Y() ) );
				pVDev->SetMapMode( aMap );
				const BOOL	bVDevOldMap = pVDev->IsMapModeEnabled();

				// create paint bitmap
				( (GDIMetaFile&) rMtf ).WindStart();
				( (GDIMetaFile&) rMtf ).Play( pVDev, rPos, rSize );
				( (GDIMetaFile&) rMtf ).WindStart();
				pVDev->EnableMapMode( FALSE );
				aPaint = pVDev->GetBitmap( Point(), pVDev->GetOutputSizePixel() );
				pVDev->EnableMapMode( bVDevOldMap ); // #i35331#: MUST NOT use EnableMapMode( TRUE ) here!

				// create mask bitmap
				pVDev->SetLineColor( COL_BLACK );
				pVDev->SetFillColor( COL_BLACK );
				pVDev->DrawRect( Rectangle( pVDev->PixelToLogic( Point() ), pVDev->GetOutputSize() ) );
#ifdef QWE
				pVDev->SetDrawMode( DRAWMODE_WHITELINE | DRAWMODE_WHITEFILL | DRAWMODE_WHITETEXT |
									DRAWMODE_WHITEBITMAP | DRAWMODE_WHITEGRADIENT );
#endif
				( (GDIMetaFile&) rMtf ).WindStart();
				( (GDIMetaFile&) rMtf ).Play( pVDev, rPos, rSize );
				( (GDIMetaFile&) rMtf ).WindStart();
				pVDev->EnableMapMode( FALSE );
				aMask = pVDev->GetBitmap( Point(), pVDev->GetOutputSizePixel() );
				pVDev->EnableMapMode( bVDevOldMap ); // #i35331#: MUST NOT use EnableMapMode( TRUE ) here!
#ifdef QWE
				// create alpha mask from gradient
				pVDev->SetDrawMode( DRAWMODE_GRAYGRADIENT );
#endif
				pVDev->DrawGradient( Rectangle( rPos, rSize ), rTransparenceGradient );
#ifdef QWE
				pVDev->SetDrawMode( DRAWMODE_DEFAULT );
#endif
				pVDev->EnableMapMode( FALSE );
				pVDev->DrawMask( Point(), pVDev->GetOutputSizePixel(), aMask, Color( COL_WHITE ) );

				aAlpha = pVDev->GetBitmap( Point(), pVDev->GetOutputSizePixel() );

				delete pVDev;

				EnableMapMode( FALSE );
				DrawBitmapEx( aDstRect.TopLeft(), BitmapEx( aPaint, aAlpha ) );
				EnableMapMode( bOldMap );
			}
			else
				delete pVDev;
		}
	}
}

void OutputDevice::DrawEPS( const Point& rPoint, const Size& rSize,
							const GfxLink& rGfxLink, GDIMetaFile* pSubst )
{
	if( mbOutputClipped )
		return;

	Rectangle	aRect( ImplLogicToDevicePixel( Rectangle( rPoint, rSize ) ) );
	if( !aRect.IsEmpty() )
	{
        // else draw the substitution graphics
		if( pSubst )
		{
			//Graphic( *pSubst ).Draw( this, rPoint, rSize );
			Gradient rTransparenceGradient;
			rTransparenceGradient.SetStartColor( Color( COL_BLACK ) );
			rTransparenceGradient.SetEndColor( Color( COL_BLACK ) );
			DrawTransparent( *pSubst, rPoint, rSize, rTransparenceGradient );
		}

	}
}
// -----------------------------------------------------------------------

bool OutputDevice::ImplIsAntiparallel() const
{
    bool bRet = false;
    if( mpGraphics )
    {
        if( ( (mpGraphics->GetLayout() & SAL_LAYOUT_BIDI_RTL) && ! IsRTLEnabled() ) ||
            ( ! (mpGraphics->GetLayout() & SAL_LAYOUT_BIDI_RTL) && IsRTLEnabled() ) )
        {
            bRet = true;
        }
    }
    return bRet;
}
void OutputDevice::ImplSetClipRegion( const Region* pRegion )
{
	if ( !pRegion )
	{
		if ( mbClipRegion )
		{
			maRegion			= Region( REGION_NULL );
			mbClipRegion		= FALSE;
			mbInitClipRegion	= TRUE;
		}
	}
	else
	{
		maRegion			= *pRegion;
		mbClipRegion		= TRUE;
		mbInitClipRegion	= TRUE;
	}
}
void OutputDevice::SetClipRegion()
{
	ImplSetClipRegion( NULL );
}
void OutputDevice::SetClipRegion( const Region& rRegion )
{
	if ( rRegion.GetType() == REGION_NULL )
		ImplSetClipRegion( NULL );
	else
	{
		Region aRegion = LogicToPixel( rRegion );
		ImplSetClipRegion( &aRegion );
	}
}
void OutputDevice::MoveClipRegion( long nHorzMove, long nVertMove )
{
	if ( mbClipRegion )
	{
		maRegion.Move( ImplLogicWidthToDevicePixel( nHorzMove ),
					   ImplLogicHeightToDevicePixel( nVertMove ) );
		mbInitClipRegion = TRUE;
	}
}
void OutputDevice::IntersectClipRegion( const Rectangle& rRect )
{
	Rectangle aRect = LogicToPixel( rRect );
	maRegion.Intersect( aRect );
	mbClipRegion		= TRUE;
	mbInitClipRegion	= TRUE;
}
void OutputDevice::IntersectClipRegion( const Region& rRegion )
{
	RegionType eType = rRegion.GetType();
	if ( eType != REGION_NULL )
	{
		Region aRegion = LogicToPixel( rRegion );
		maRegion.Intersect( aRegion );
		mbClipRegion		= TRUE;
		mbInitClipRegion	= TRUE;
	}
}
// -----------------------------------------------------------------------
Region OutputDevice::GetClipRegion() const
{
	return PixelToLogic( maRegion );
}
void OutputDevice::SetLineColor()
{
	if ( mbLineColor )
	{
		mbInitLineColor = TRUE;
		mbLineColor = FALSE;
		maLineColor = Color( COL_TRANSPARENT );
	}
}
void OutputDevice::SetLineColor( const Color& rColor )
{
	if( rColor.GetTransparency() )
	{
		if ( mbLineColor )
		{
			mbInitLineColor = TRUE;
			mbLineColor = FALSE;
			maLineColor = Color( COL_TRANSPARENT );
		}
	}
	else
	{
		if( maLineColor != rColor )
		{
			mbInitLineColor = TRUE;
			mbLineColor = TRUE;
			maLineColor = rColor;
		}
	}
}
void OutputDevice::SetFillColor()
{
	if ( mbFillColor )
	{
		mbInitFillColor = TRUE;
		mbFillColor = FALSE;
		maFillColor = Color( COL_TRANSPARENT );
	}
}
void OutputDevice::SetFillColor( const Color& rColor )
{
	if ( rColor.GetTransparency() )
	{
		if ( mbFillColor )
		{
			mbInitFillColor = TRUE;
			mbFillColor = FALSE;
			maFillColor = Color( COL_TRANSPARENT );
		}
	}
	else
	{
		if ( maFillColor != rColor )
		{
			mbInitFillColor = TRUE;
			mbFillColor = TRUE;
			maFillColor = rColor;
		}
	}
}
void OutputDevice::SetTextColor( const Color& rColor )
{
    if ( maTextColor != rColor )
    {
        maTextColor = rColor;
        mbInitTextColor = TRUE;
    }
}
void OutputDevice::SetTextFillColor()
{
    if ( maFont.GetColor() != Color( COL_TRANSPARENT ) )
        maFont.SetFillColor( Color( COL_TRANSPARENT ) );
    if ( !maFont.IsTransparent() )
        maFont.SetTransparent( TRUE );

}
void OutputDevice::SetTextFillColor( const Color& rColor )
{
    BOOL bTransFill = rColor.GetTransparency() ? TRUE : FALSE;
    if ( maFont.GetFillColor() != rColor )
        maFont.SetFillColor( rColor );
    if ( maFont.IsTransparent() != bTransFill )
        maFont.SetTransparent( bTransFill );
}
Color OutputDevice::GetTextFillColor() const
{
    if ( maFont.IsTransparent() )
        return Color( COL_TRANSPARENT );
    else
        return maFont.GetFillColor();
}
void OutputDevice::SetTextLineColor()
{
    maTextLineColor = Color( COL_TRANSPARENT );
}
void OutputDevice::SetTextLineColor( const Color& rColor )
{
    maTextLineColor = rColor;
}
void OutputDevice::SetOverlineColor()
{
}
void OutputDevice::SetOverlineColor( const Color& rColor )
{
}
void OutputDevice::SetTextAlign( TextAlign eAlign )
{
    if ( maFont.GetAlign() != eAlign )
    {
        maFont.SetAlign( eAlign );
        mbNewFont = TRUE;
    }
}
// -----------------------------------------------------------------------

void OutputDevice::SetBackground()
{
	maBackground = Wallpaper();
	mbBackground = FALSE;
}

// -----------------------------------------------------------------------

void OutputDevice::SetBackground( const Wallpaper& rBackground )
{
	maBackground = rBackground;

	if( rBackground.GetStyle() == WALLPAPER_NULL )
		mbBackground = FALSE;
	else
		mbBackground = TRUE;
}
USHORT OutputDevice::GetBitCount() const
{
#ifdef AVS
	if ( meOutDevType == OUTDEV_VIRDEV )
#endif
		return ((OutputDevice*)this)->mnBitCount;
#ifdef AVS
	// we need a graphics
	if ( !mpGraphics )
		return 0;

	return (USHORT)mpGraphics->GetBitCount();
#endif
}
void OutputDevice::SetDigitLanguage( LanguageType eLang )
{
    meTextLanguage = eLang;
}
void OutputDevice::SetLayoutMode( ULONG nTextLayoutMode )
{
    mnTextLayoutMode = nTextLayoutMode;
}
void OutputDevice::SetRefPoint()
{
	mbRefPoint = FALSE;
	maRefPoint.X() = maRefPoint.Y() = 0L;
}

// -----------------------------------------------------------------------

void OutputDevice::SetRefPoint( const Point& rRefPoint )
{
	mbRefPoint = TRUE;
	maRefPoint = rRefPoint;
}
void OutputDevice::Push( USHORT nFlags )
{
	ImplObjStack* pData = new ImplObjStack;
	pData->mpPrev = mpObjStack;
	mpObjStack = pData;

	pData->mnFlags = nFlags;

	if ( nFlags & PUSH_LINECOLOR )
	{
		if ( mbLineColor )
			pData->mpLineColor = new Color( maLineColor );
		else
			pData->mpLineColor = NULL;
	}
	if ( nFlags & PUSH_FILLCOLOR )
	{
		if ( mbFillColor )
			pData->mpFillColor = new Color( maFillColor );
		else
			pData->mpFillColor = NULL;
	}
	if ( nFlags & PUSH_FONT )
		pData->mpFont = new Font( maFont );
	if ( nFlags & PUSH_TEXTCOLOR )
		pData->mpTextColor = new Color( GetTextColor() );
	if ( nFlags & PUSH_TEXTFILLCOLOR )
	{
		if ( IsTextFillColor() )
			pData->mpTextFillColor = new Color( GetTextFillColor() );
		else
			pData->mpTextFillColor = NULL;
	}
	if ( nFlags & PUSH_TEXTLINECOLOR )
	{
		if ( IsTextLineColor() )
			pData->mpTextLineColor = new Color( GetTextLineColor() );
		else
			pData->mpTextLineColor = NULL;
	}
	if ( nFlags & PUSH_OVERLINECOLOR )
	{
		if ( IsOverlineColor() )
			pData->mpOverlineColor = new Color( GetOverlineColor() );
		else
			pData->mpOverlineColor = NULL;
	}
	if ( nFlags & PUSH_TEXTALIGN )
		pData->meTextAlign = GetTextAlign();
	if( nFlags & PUSH_TEXTLAYOUTMODE )
		pData->mnTextLayoutMode = GetLayoutMode();
	if( nFlags & PUSH_TEXTLANGUAGE )
		pData->meTextLanguage = GetDigitLanguage();
	if ( nFlags & PUSH_RASTEROP )
		pData->meRasterOp = GetRasterOp();
	if ( nFlags & PUSH_MAPMODE )
	{
		if ( mbMap )
			pData->mpMapMode = new MapMode( maMapMode );
		else
			pData->mpMapMode = NULL;
	}
	if ( nFlags & PUSH_CLIPREGION )
	{
		if ( mbClipRegion )
			pData->mpClipRegion = new Region( maRegion );
		else
			pData->mpClipRegion = NULL;
	}
	if ( nFlags & PUSH_REFPOINT )
	{
		if ( mbRefPoint )
			pData->mpRefPoint = new Point( maRefPoint );
		else
			pData->mpRefPoint = NULL;
	}
}

// -----------------------------------------------------------------------

void OutputDevice::Pop()
{
	ImplObjStack*	pData = mpObjStack;

	if ( !pData )
	{
		//DBG_ERRORFILE( "OutputDevice::Pop() without OutputDevice::Push()" );
		return;
	}

	mpObjStack = pData->mpPrev;

	if ( pData->mnFlags & PUSH_LINECOLOR )
	{
		if ( pData->mpLineColor )
			SetLineColor( *pData->mpLineColor );
		else
			SetLineColor();
	}
	if ( pData->mnFlags & PUSH_FILLCOLOR )
	{
		if ( pData->mpFillColor )
			SetFillColor( *pData->mpFillColor );
		else
			SetFillColor();
	}
	if ( pData->mnFlags & PUSH_FONT )
		SetFont( *pData->mpFont );
	if ( pData->mnFlags & PUSH_TEXTCOLOR )
		SetTextColor( *pData->mpTextColor );
	if ( pData->mnFlags & PUSH_TEXTFILLCOLOR )
	{
		if ( pData->mpTextFillColor )
			SetTextFillColor( *pData->mpTextFillColor );
		else
			SetTextFillColor();
	}
	if ( pData->mnFlags & PUSH_TEXTLINECOLOR )
	{
		if ( pData->mpTextLineColor )
			SetTextLineColor( *pData->mpTextLineColor );
		else
			SetTextLineColor();
	}
	if ( pData->mnFlags & PUSH_OVERLINECOLOR )
	{
		if ( pData->mpOverlineColor )
			SetOverlineColor( *pData->mpOverlineColor );
		else
			SetOverlineColor();
	}
	if ( pData->mnFlags & PUSH_TEXTALIGN )
		SetTextAlign( pData->meTextAlign );
	if( pData->mnFlags & PUSH_TEXTLAYOUTMODE )
		SetLayoutMode( pData->mnTextLayoutMode );
	if( pData->mnFlags & PUSH_TEXTLANGUAGE )
		SetDigitLanguage( pData->meTextLanguage );
	if ( pData->mnFlags & PUSH_RASTEROP )
		SetRasterOp( pData->meRasterOp );
	if ( pData->mnFlags & PUSH_MAPMODE )
	{
		if ( pData->mpMapMode )
			SetMapMode( *pData->mpMapMode );
		else
			SetMapMode();
	}
	if ( pData->mnFlags & PUSH_CLIPREGION )
		ImplSetClipRegion( pData->mpClipRegion );
	if ( pData->mnFlags & PUSH_REFPOINT )
	{
		if ( pData->mpRefPoint )
			SetRefPoint( *pData->mpRefPoint );
		else
			SetRefPoint();
	}

	ImplDeleteObjStack( pData );
}
void OutputDevice::SetFont( const Font& rNewFont )
{
    Font aFont( rNewFont );

    if ( !maFont.IsSameInstance( aFont ) )
    {
        // Optimization MT/HDU: COL_TRANSPARENT means SetFont should ignore the font color,
        // because SetTextColor() is used for this.
        // #i28759# maTextColor might have been changed behind our back, commit then, too.
        if( aFont.GetColor() != COL_TRANSPARENT
        && (aFont.GetColor() != maFont.GetColor() || aFont.GetColor() != maTextColor ) )
        {
            maTextColor = aFont.GetColor();
            mbInitTextColor = TRUE;
        }
        maFont      = aFont;
        mbNewFont   = TRUE;
    }
}

#ifdef QWE
// -----------------------------------------------------------------------

void OutputDevice::SetDrawMode( ULONG nDrawMode )
{
	mnDrawMode = nDrawMode;
}
#endif
void OutputDevice::SetRasterOp( RasterOp eRasterOp )
{
	if ( meRasterOp != eRasterOp )
	{
		meRasterOp = eRasterOp;
		mbInitLineColor = mbInitFillColor = TRUE;

		if( mpGraphics )
			mpGraphics->SetXORMode( (ROP_INVERT == meRasterOp) || (ROP_XOR == meRasterOp), ROP_INVERT == meRasterOp );
	}
}
BOOL OutputDevice::SetOutputSizePixel( const Size& rNewSize, BOOL bErase )
{
    return ImplSetOutputSizePixel(rNewSize, bErase);
}
BOOL OutputDevice::ImplSetOutputSizePixel( const Size& rNewSize, BOOL bErase )
{
#ifdef AVS
	DBG_TRACE3( "VirtualDevice::ImplSetOutputSizePixel( %ld, %ld, %d )", rNewSize.Width(), rNewSize.Height(), (int)bErase );

	if ( !mpVirDev )
		return FALSE;
	else 
#endif
		if ( rNewSize == GetOutputSizePixel() )
	{
		if ( bErase )
			Erase();
		return TRUE;
	}

	BOOL bRet;
	long nNewWidth = rNewSize.Width(), nNewHeight = rNewSize.Height();

	if ( nNewWidth < 1 )
		nNewWidth = 1;

	if ( nNewHeight < 1 )
		nNewHeight = 1;
#ifdef AVS
	if ( bErase )
	{
		bRet = mpVirDev->SetSize( nNewWidth, nNewHeight );

		if ( bRet )
		{
#endif
			mnOutWidth	= rNewSize.Width();
			mnOutHeight = rNewSize.Height();
			mpGraphics->SetOutputSizePixel( rNewSize );
			Erase();
#ifdef AVS
		}
	}
	else
	{
		SalVirtualDevice*	pNewVirDev;
		ImplSVData* 		pSVData = ImplGetSVData();

		// we need a graphics
		if ( !mpGraphics )
			return FALSE;

		pNewVirDev = pSVData->mpDefInst->CreateVirtualDevice( mpGraphics, nNewWidth, nNewHeight, mnBitCount );
		if ( pNewVirDev )
		{
			SalGraphics* pGraphics = pNewVirDev->GetGraphics();
			if ( pGraphics )
			{
				SalTwoRect aPosAry;
				long nWidth;
				long nHeight;
				if ( mnOutWidth < nNewWidth )
					nWidth = mnOutWidth;
				else
					nWidth = nNewWidth;
				if ( mnOutHeight < nNewHeight )
					nHeight = mnOutHeight;
				else
					nHeight = nNewHeight;
				aPosAry.mnSrcX		 = 0;
				aPosAry.mnSrcY		 = 0;
				aPosAry.mnSrcWidth	 = nWidth;
				aPosAry.mnSrcHeight  = nHeight;
				aPosAry.mnDestX 	 = 0;
				aPosAry.mnDestY 	 = 0;
				aPosAry.mnDestWidth  = nWidth;
				aPosAry.mnDestHeight = nHeight;

				pGraphics->CopyBits( &aPosAry, mpGraphics, this, this );
				pNewVirDev->ReleaseGraphics( pGraphics );
				ImplReleaseGraphics();
				pSVData->mpDefInst->DestroyVirtualDevice( mpVirDev );
				mpVirDev = pNewVirDev;
				mnOutWidth	= rNewSize.Width();
				mnOutHeight = rNewSize.Height();
				bRet = TRUE;
			}
			else
			{
				bRet = FALSE;
				pSVData->mpDefInst->DestroyVirtualDevice( pNewVirDev );
			}
		}
		else
			bRet = FALSE;
	}
	return bRet;
#endif
	return TRUE;
}
Bitmap OutputDevice::GetBitmap( const Point& rSrcPt, const Size& rSize ) const
{
	Bitmap	aBmp;
	long	nX = ImplLogicXToDevicePixel( rSrcPt.X() );
	long	nY = ImplLogicYToDevicePixel( rSrcPt.Y() );
	long	nWidth = ImplLogicWidthToDevicePixel( rSize.Width() );
	long	nHeight = ImplLogicHeightToDevicePixel( rSize.Height() );

	if ( mpGraphics )
	{
		if ( nWidth && nHeight )
		{
			Rectangle	aRect( Point( nX, nY ), Size( nWidth, nHeight ) );
			BOOL		bClipped = FALSE;

			// X-Koordinate ausserhalb des Bereichs?
			if ( nX < mnOutOffX )
			{
				nWidth -= ( mnOutOffX - nX );
				nX = mnOutOffX;
				bClipped = TRUE;
			}

			// Y-Koordinate ausserhalb des Bereichs?
			if ( nY < mnOutOffY )
			{
				nHeight -= ( mnOutOffY - nY );
				nY = mnOutOffY;
				bClipped = TRUE;
			}

			// Breite ausserhalb des Bereichs?
			if ( (nWidth + nX) > (mnOutWidth + mnOutOffX) )
			{
				nWidth	= mnOutOffX + mnOutWidth - nX;
				bClipped = TRUE;
			}

			// Hoehe ausserhalb des Bereichs?
			if ( (nHeight + nY) > (mnOutHeight + mnOutOffY) )
			{
				nHeight = mnOutOffY + mnOutHeight - nY;
				bClipped = TRUE;
			}

			if ( bClipped )
			{
				// Falls auf den sichtbaren Bereich geclipped wurde,
				// muessen wir eine Bitmap in der rchtigen Groesse
				// erzeugen, in die die geclippte Bitmap an die angepasste
				// Position kopiert wird
				OutputDevice aVDev( *this );

				if ( aVDev.SetOutputSizePixel( aRect.GetSize() ) )
				{
					if ( ((OutputDevice*)&aVDev)->mpGraphics )
					{
						SalTwoRect aPosAry;

						aPosAry.mnSrcX = nX;
						aPosAry.mnSrcY = nY;
						aPosAry.mnSrcWidth = nWidth;
						aPosAry.mnSrcHeight = nHeight;
						aPosAry.mnDestX = ( aRect.Left() < mnOutOffX ) ? ( mnOutOffX - aRect.Left() ) : 0L;
						aPosAry.mnDestY = ( aRect.Top() < mnOutOffY ) ? ( mnOutOffY - aRect.Top() ) : 0L;
						aPosAry.mnDestWidth = nWidth;
						aPosAry.mnDestHeight = nHeight;

						if ( (nWidth > 0) && (nHeight > 0) )
							(((OutputDevice*)&aVDev)->mpGraphics)->CopyBits( &aPosAry, mpGraphics, this, this );

						aBmp = aVDev.GetBitmap( Point(), aVDev.GetOutputSizePixel() );
					 }
					 else
						bClipped = FALSE;
				}
				else
					bClipped = FALSE;
			}

			if ( !bClipped )
			{
				SalBitmap* pSalBmp = mpGraphics->GetBitmap( nX, nY, nWidth, nHeight, this );

				if( pSalBmp )
				{
					ImpBitmap* pImpBmp = new ImpBitmap;
					pImpBmp->ImplSetSalBitmap( pSalBmp );
					aBmp.ImplSetImpBitmap( pImpBmp );
				}
			}
		}
	}

	return aBmp;
}
void OutputDevice::Erase()
{ 
	if ( mbBackground )
	{
		RasterOp eRasterOp = GetRasterOp();
		if ( eRasterOp != ROP_OVERPAINT )
			SetRasterOp( ROP_OVERPAINT );
		ImplDrawWallpaper( 0, 0, mnOutWidth, mnOutHeight, maBackground );
		if ( eRasterOp != ROP_OVERPAINT )
			SetRasterOp( eRasterOp );
	}
}


BOOL OutputDevice::GetGlyphBoundRects( const Point& rOrigin, const String& rStr,
    int nIndex, int nLen, int nBase, MetricVector& rVector )
{

    rVector.clear();

    if( nLen == STRING_LEN )
        nLen = rStr.Len() - nIndex;

    Rectangle aRect;
    for( int i = 0; i < nLen; i++ )
    {
        if( !GetTextBoundRect( aRect, rStr, static_cast<xub_StrLen>(nBase), static_cast<xub_StrLen>(nIndex+i), 1 ) )
            break;
        aRect.Move( rOrigin.X(), rOrigin.Y() );
        rVector.push_back( aRect );
    }

    return (nLen == (int)rVector.size());
}
BOOL OutputDevice::GetTextBoundRect( Rectangle& rRect,
    const String& rStr, xub_StrLen nBase, xub_StrLen nIndex,
    xub_StrLen nLen ) const
{
    //DBG_TRACE( "OutputDevice::GetTextBoundRect()" );
    //DBG_CHKTHIS( OutputDevice, ImplDbgCheckOutputDevice );

    BOOL bRet = FALSE;
    rRect.SetEmpty();

    SalLayout* pSalLayout = NULL;
    // calculate offset when nBase!=nIndex
    long nXOffset = 0;
    if( nBase != nIndex )
    {
        xub_StrLen nStart = Min( nBase, nIndex );
        xub_StrLen nOfsLen = Max( nBase, nIndex ) - nStart;
        pSalLayout = ImplLayout( rStr, nStart, nOfsLen );
        if( pSalLayout )
        {
            nXOffset = pSalLayout->GetTextWidth();
            nXOffset /= pSalLayout->GetUnitsPerPixel();
            pSalLayout->Release();
            // TODO: fix offset calculation for Bidi case
            if( nBase < nIndex)
                nXOffset = -nXOffset;
        }
    }

    pSalLayout = ImplLayout( rStr, nIndex, nLen );
    Rectangle aPixelRect;
    if( pSalLayout )
    {
        bRet = pSalLayout->GetBoundRect( *mpGraphics, aPixelRect );

        if( bRet )
        {
            int nWidthFactor = pSalLayout->GetUnitsPerPixel();

            if( nWidthFactor > 1 )
            {
                double fFactor = 1.0 / nWidthFactor;
                aPixelRect.Left()
                    = static_cast< long >(aPixelRect.Left() * fFactor);
                aPixelRect.Right()
                    = static_cast< long >(aPixelRect.Right() * fFactor);
                aPixelRect.Top()
                    = static_cast< long >(aPixelRect.Top() * fFactor);
                aPixelRect.Bottom()
                    = static_cast< long >(aPixelRect.Bottom() * fFactor);
            }

            Point aRotatedOfs( mnTextOffX, mnTextOffY );
            aRotatedOfs -= pSalLayout->GetDrawPosition( Point( nXOffset, 0 ) );
            aPixelRect += aRotatedOfs;
            rRect = PixelToLogic( aPixelRect );
            if( mbMap )
                rRect += Point( maMapRes.mnMapOfsX, maMapRes.mnMapOfsY );
        }

        pSalLayout->Release();
    }
	return TRUE;
#ifdef AVS

    if( bRet || (OUTDEV_PRINTER == meOutDevType) || !mpFontEntry )
        return bRet;

    // fall back to bitmap method to get the bounding rectangle,
    // so we need a monochrome virtual device with matching font
    VirtualDevice aVDev( 1 );
    Font aFont( GetFont() );
    aFont.SetShadow( FALSE );
    aFont.SetOutline( FALSE );
    aFont.SetRelief( RELIEF_NONE );
    aFont.SetOrientation( 0 );
    aFont.SetSize( Size( mpFontEntry->maFontSelData.mnWidth, mpFontEntry->maFontSelData.mnHeight ) );
    aVDev.SetFont( aFont );
    aVDev.SetTextAlign( ALIGN_TOP );

    // layout the text on the virtual device
    pSalLayout = aVDev.ImplLayout( rStr, nIndex, nLen );
    if( !pSalLayout )
        return false;

    // make the bitmap big enough
    // TODO: use factors when it would get too big
    long nWidth = pSalLayout->GetTextWidth();
    long nHeight = mpFontEntry->mnLineHeight + mnEmphasisAscent + mnEmphasisDescent;
    Point aOffset( nWidth/2, 8 );
    Size aOutSize( nWidth + 2*aOffset.X(), nHeight + 2*aOffset.Y() );
    if( !nWidth || !aVDev.SetOutputSizePixel( aOutSize ) )
        return false;

    // draw text in black
    pSalLayout->DrawBase() = aOffset;
    aVDev.SetTextColor( Color( COL_BLACK ) );
    aVDev.SetTextFillColor();
    aVDev.ImplInitTextColor();
    aVDev.ImplDrawText( *pSalLayout );
    pSalLayout->Release();

    // find extents using the bitmap
    Bitmap aBmp = aVDev.GetBitmap( Point(), aOutSize );
    BitmapReadAccess* pAcc = aBmp.AcquireReadAccess();
    if( !pAcc )
        return FALSE;
    const BitmapColor aBlack( pAcc->GetBestMatchingColor( Color( COL_BLACK ) ) );
    const long nW = pAcc->Width();
    const long nH = pAcc->Height();
    long nLeft = 0;
    long nRight = 0;

    // find top left point
    long nTop = 0;
    for(; nTop < nH; ++nTop )
    {
        for( nLeft = 0; nLeft < nW; ++nLeft )
            if( pAcc->GetPixel( nTop, nLeft ) == aBlack )
                break;
        if( nLeft < nW )
            break;
    }

    // find bottom right point
    long nBottom = nH;
    while( --nBottom >= nTop )
    {
        for( nRight = nW; --nRight >= 0; )
            if( pAcc->GetPixel( nBottom, nRight ) == aBlack )
                break;
        if( nRight >= 0 )
            break;
    }
    if( nRight < nLeft )
    {
        long nX = nRight;
        nRight = nLeft;
        nLeft  = nX;
    }

    for( long nY = nTop; nY <= nBottom; ++nY )
    {
        // find leftmost point
        long nX;
        for( nX = 0; nX < nLeft; ++nX )
            if( pAcc->GetPixel( nY, nX ) == aBlack )
                break;
        nLeft = nX;

        // find rightmost point
        for( nX = nW; --nX > nRight; )
            if( pAcc->GetPixel( nY, nX ) == aBlack )
                break;
        nRight = nX;
    }

    aBmp.ReleaseAccess( pAcc );

    if( nTop <= nBottom )
    {
        Size aSize( nRight - nLeft + 1, nBottom - nTop + 1 );
        Point aTopLeft( nLeft, nTop );
        aTopLeft -= aOffset;
        // adjust to text alignment
        aTopLeft.Y()+= mnTextOffY - (mpFontEntry->maMetric.mnAscent + mnEmphasisAscent);
        // convert to logical coordinates
        aSize = PixelToLogic( aSize );
        aTopLeft.X() = ImplDevicePixelToLogicWidth( aTopLeft.X() );
        aTopLeft.Y() = ImplDevicePixelToLogicHeight( aTopLeft.Y() );
        rRect = Rectangle( aTopLeft, aSize );
        return TRUE;
    }

    return FALSE;
#endif
}

// =======================================================================

ImplFontEntry::ImplFontEntry( const ImplFontSelectData& rFontSelData )
:   maFontSelData( rFontSelData ),
    maMetric( rFontSelData ),
    mpConversion( NULL ),
    mnRefCount( 1 ),
    mnSetFontFlags( 0 ),
    mnOwnOrientation( 0 ),
    mnOrientation( 0 ),
    mbInit( false )//,
    //mpUnicodeFallbackList( NULL )
{
    maFontSelData.mpFontEntry = this;
}

// -----------------------------------------------------------------------

ImplFontEntry::~ImplFontEntry()
{
    //delete mpUnicodeFallbackList;
}
// -----------------------------------------------------------------------

static String GetNextFontToken( const String& rTokenStr, xub_StrLen& rIndex )
{
    // check for valid start index
    int nStringLen = rTokenStr.Len();
    if( rIndex >= nStringLen )
    {
        rIndex = STRING_NOTFOUND;
        return String();
    }

    // find the next token delimiter and return the token substring
    const sal_Unicode* pStr = rTokenStr.GetBuffer() + rIndex;
    const sal_Unicode* pEnd = rTokenStr.GetBuffer() + nStringLen;
    for(; pStr < pEnd; ++pStr )
        if( (*pStr == ';') || (*pStr == ',') )
            break;

    xub_StrLen nTokenStart = rIndex;
    xub_StrLen nTokenLen;
    if( pStr < pEnd )
    {
        rIndex = static_cast<xub_StrLen>(pStr - rTokenStr.GetBuffer());
        nTokenLen = rIndex - nTokenStart;
        ++rIndex; // skip over token separator
    }
    else
    {
        // no token delimiter found => handle last token
        rIndex = STRING_NOTFOUND;
        nTokenLen = STRING_LEN;

        // optimize if the token string consists of just one token
        if( !nTokenStart )
            return rTokenStr;
    }

    return String( rTokenStr, nTokenStart, nTokenLen );
}

ImplFontMetricData::ImplFontMetricData( const ImplFontSelectData& rFontSelData )
:   ImplFontAttributes( rFontSelData )
{

	// initialize the members provided by the font request
    mnWidth        = rFontSelData.mnWidth;
    mnOrientation  = static_cast<short>(rFontSelData.mnOrientation);

	// intialize the used font name 
    if( rFontSelData.mpFontData )
    {
        maName     = rFontSelData.mpFontData->maName;
        maStyleName= rFontSelData.mpFontData->maStyleName;
        mbDevice   = rFontSelData.mpFontData->mbDevice;
        mbKernableFont = true;
    }
    else
    {
        xub_StrLen nTokenPos = 0;
        maName     = GetNextFontToken( rFontSelData.maName, nTokenPos );
        maStyleName= rFontSelData.maStyleName;
        mbDevice   = false;
        mbKernableFont = false;
    }

	// reset metrics that are usually measured for the font instance 
    mnAscent       = 0;
    mnDescent      = 0;
    mnIntLeading   = 0;
    mnExtLeading   = 0;
    mnSlant        = 0;
    mnMinKashida   = 0;

    // reset metrics that are usually derived from the measurements
    mnUnderlineSize            = 0;
    mnUnderlineOffset          = 0;
    mnBUnderlineSize           = 0;
    mnBUnderlineOffset         = 0;
    mnDUnderlineSize           = 0;
    mnDUnderlineOffset1        = 0;
    mnDUnderlineOffset2        = 0;
    mnWUnderlineSize           = 0;
    mnWUnderlineOffset         = 0;
    mnAboveUnderlineSize       = 0;
    mnAboveUnderlineOffset     = 0;
    mnAboveBUnderlineSize      = 0;
    mnAboveBUnderlineOffset    = 0;
    mnAboveDUnderlineSize      = 0;
    mnAboveDUnderlineOffset1   = 0;
    mnAboveDUnderlineOffset2   = 0;
    mnAboveWUnderlineSize      = 0;
    mnAboveWUnderlineOffset    = 0;
    mnStrikeoutSize            = 0;
    mnStrikeoutOffset          = 0;
    mnBStrikeoutSize           = 0;
    mnBStrikeoutOffset         = 0;
    mnDStrikeoutSize           = 0;
    mnDStrikeoutOffset1        = 0;
    mnDStrikeoutOffset2        = 0;

}
// -----------------------------------------------------------------------

void ImplFontMetricData::ImplInitTextLineSize( const OutputDevice* pDev )
{
    long nDescent = mnDescent;
    if ( nDescent <= 0 )
    {
        nDescent = mnAscent / 10;
        if ( !nDescent )
            nDescent = 1;
    }

    // #i55341# for some fonts it is not a good idea to calculate
    // their text line metrics from the real font descent
    // => work around this problem just for these fonts
    if( 3*nDescent > mnAscent )
        nDescent = mnAscent / 3;

    long nLineHeight = ((nDescent*25)+50) / 100;
    if ( !nLineHeight )
        nLineHeight = 1;
    long nLineHeight2 = nLineHeight / 2;
    if ( !nLineHeight2 )
        nLineHeight2 = 1;

    long nBLineHeight = ((nDescent*50)+50) / 100;
    if ( nBLineHeight == nLineHeight )
        nBLineHeight++;
    long nBLineHeight2 = nBLineHeight/2;
    if ( !nBLineHeight2 )
        nBLineHeight2 = 1;

    long n2LineHeight = ((nDescent*16)+50) / 100;
    if ( !n2LineHeight )
        n2LineHeight = 1;
    long n2LineDY = n2LineHeight;
     /* #117909#
      * add some pixels to minimum double line distance on higher resolution devices
      */
    long nMin2LineDY = 1 + pDev->ImplGetDPIY()/150;
    if ( n2LineDY < nMin2LineDY )
        n2LineDY = nMin2LineDY;
    long n2LineDY2 = n2LineDY/2;
    if ( !n2LineDY2 )
        n2LineDY2 = 1;

    long nUnderlineOffset = mnDescent/2 + 1;
    long nStrikeoutOffset = -((mnAscent - mnIntLeading) / 3);

    mnUnderlineSize        = nLineHeight;
    mnUnderlineOffset      = nUnderlineOffset - nLineHeight2;

    mnBUnderlineSize       = nBLineHeight;
    mnBUnderlineOffset     = nUnderlineOffset - nBLineHeight2;

    mnDUnderlineSize       = n2LineHeight;
    mnDUnderlineOffset1    = nUnderlineOffset - n2LineDY2 - n2LineHeight;
    mnDUnderlineOffset2    = mnDUnderlineOffset1 + n2LineDY + n2LineHeight;

    long nWCalcSize = mnDescent;
    if ( nWCalcSize < 6 )
    {
        if ( (nWCalcSize == 1) || (nWCalcSize == 2) )
            mnWUnderlineSize = nWCalcSize;
        else
            mnWUnderlineSize = 3;
    }
    else
        mnWUnderlineSize = ((nWCalcSize*50)+50) / 100;

    // #109280# the following line assures that wavelnes are never placed below the descent, however
    // for most fonts the waveline then is drawn into the text, so we better keep the old solution
    // pFontEntry->maMetric.mnWUnderlineOffset     = pFontEntry->maMetric.mnDescent + 1 - pFontEntry->maMetric.mnWUnderlineSize;
    mnWUnderlineOffset     = nUnderlineOffset;

    mnStrikeoutSize        = nLineHeight;
    mnStrikeoutOffset      = nStrikeoutOffset - nLineHeight2;

    mnBStrikeoutSize       = nBLineHeight;
    mnBStrikeoutOffset     = nStrikeoutOffset - nBLineHeight2;

    mnDStrikeoutSize       = n2LineHeight;
    mnDStrikeoutOffset1    = nStrikeoutOffset - n2LineDY2 - n2LineHeight;
    mnDStrikeoutOffset2    = mnDStrikeoutOffset1 + n2LineDY + n2LineHeight;
}

// -----------------------------------------------------------------------

void ImplFontMetricData::ImplInitAboveTextLineSize()
{
    long nIntLeading = mnIntLeading;
    // TODO: assess usage of nLeading below (changed in extleading CWS)
    // if no leading is available, we assume 15% of the ascent
    if ( nIntLeading <= 0 )
    {
        nIntLeading = mnAscent*15/100;
        if ( !nIntLeading )
            nIntLeading = 1;
    }

    long nLineHeight = ((nIntLeading*25)+50) / 100;
    if ( !nLineHeight )
        nLineHeight = 1;

    long nBLineHeight = ((nIntLeading*50)+50) / 100;
    if ( nBLineHeight == nLineHeight )
        nBLineHeight++;

    long n2LineHeight = ((nIntLeading*16)+50) / 100;
    if ( !n2LineHeight )
        n2LineHeight = 1;

    long nCeiling = -mnAscent;

    mnAboveUnderlineSize       = nLineHeight;
    mnAboveUnderlineOffset     = nCeiling + (nIntLeading - nLineHeight + 1) / 2;

    mnAboveBUnderlineSize      = nBLineHeight;
    mnAboveBUnderlineOffset    = nCeiling + (nIntLeading - nBLineHeight + 1) / 2;

    mnAboveDUnderlineSize      = n2LineHeight;
    mnAboveDUnderlineOffset1   = nCeiling + (nIntLeading - 3*n2LineHeight + 1) / 2;
    mnAboveDUnderlineOffset2   = nCeiling + (nIntLeading +   n2LineHeight + 1) / 2;

    long nWCalcSize = nIntLeading;
    if ( nWCalcSize < 6 )
    {
        if ( (nWCalcSize == 1) || (nWCalcSize == 2) )
            mnAboveWUnderlineSize = nWCalcSize;
        else
            mnAboveWUnderlineSize = 3;
    }
    else
        mnAboveWUnderlineSize = ((nWCalcSize*50)+50) / 100;

    mnAboveWUnderlineOffset = nCeiling + (nIntLeading + 1) / 2;
}
// =======================================================================

ImplDevFontList::ImplDevFontList()
:   mbMatchData( false )
,   mbMapNames( false )
,   mpPreMatchHook( NULL )
,   mpFallbackHook( NULL )
,   mpFallbackList( NULL )
,   mnFallbackCount( -1 )
{}


// -----------------------------------------------------------------------

ImplDevFontList::~ImplDevFontList()
{
    Clear();
}
// -----------------------------------------------------------------------

void ImplDevFontList::Clear()
{
    // remove fallback lists
    delete[] mpFallbackList;
    mpFallbackList = NULL;
    mnFallbackCount = -1;

    // clear all entries in the device font list
    DevFontList::iterator it = maDevFontList.begin();
    for(; it != maDevFontList.end(); ++it )
    {
        ImplDevFontListData* pEntry = (*it).second;
        delete pEntry;
    }

    maDevFontList.clear();

    // match data must be recalculated too
    mbMatchData = false;
}

// -----------------------------------------------------------------------

void ImplDevFontList::Add( ImplFontData* pNewData )
{
    int nAliasQuality = pNewData->mnQuality - 100;
    String aMapNames = pNewData->maMapNames;
    pNewData->maMapNames = String();

    bool bKeepNewData = false;
    for( xub_StrLen nMapNameIndex = 0; nMapNameIndex != STRING_NOTFOUND; )
    {
        String aSearchName = pNewData->maName;
#ifdef AVS
        ImplGetEnglishSearchFontName( aSearchName );
#endif
        DevFontList::const_iterator it = maDevFontList.find( aSearchName );
        ImplDevFontListData* pFoundData = NULL;
        if( it != maDevFontList.end() )
            pFoundData = (*it).second;

        if( !pFoundData )
        {
            pFoundData = new ImplDevFontListData( aSearchName );
            maDevFontList[ aSearchName ] = pFoundData;
        }

        bKeepNewData = pFoundData->AddFontFace( pNewData );

        // add font alias if available
        // a font alias should never win against an original font with similar quality
        if( aMapNames.Len() >= nMapNameIndex )
            break;
        if( bKeepNewData ) // try to recycle obsoleted object
            pNewData = pNewData->CreateAlias();
        bKeepNewData = false;
        pNewData->mnQuality = nAliasQuality;
        pNewData->maName = GetNextFontToken( aMapNames, nMapNameIndex );
    }

    if( !bKeepNewData )
        delete pNewData;
}
namespace SVMCore{
// -----------------------------------------------------------------------

void ImplGetEnglishSearchFontName( String& rName )
{
    bool        bNeedTranslation = false;
    xub_StrLen  nLen = rName.Len();

    // Remove trailing whitespaces
    xub_StrLen i = nLen;
    while ( i && (rName.GetChar( i-1 ) < 32) )
        i--;
    if ( i != nLen )
        rName.Erase( i );

    // Remove Script at the end
    // Scriptname must be the last part of the fontname and
    // looks like "fontname (scriptname)". So there can only be a
    // script name at the and of the fontname, when the last char is ')'
    if ( (nLen >= 3) && rName.GetChar( nLen-1 ) == ')' )
    {
        int nOpen = 1;
        xub_StrLen nTempLen = nLen-2;
        while ( nTempLen )
        {
            if ( rName.GetChar( nTempLen ) == '(' )
            {
                nOpen--;
                if ( !nOpen )
                {
                    // Remove Space at the end
                    if ( nTempLen && (rName.GetChar( nTempLen-1 ) == ' ') )
                        nTempLen--;
                    rName.Erase( nTempLen );
                    nLen = nTempLen;
                    break;
                }
            }
            if ( rName.GetChar( nTempLen ) == ')' )
                nOpen++;
            nTempLen--;
        }
    }

    // remove all whitespaces and converts to lower case ASCII
    // TODO: better transliteration to ASCII e.g. all digits
    i = 0;
    while ( i < nLen )
    {
        sal_Unicode c = rName.GetChar( i );
        if ( c > 127 )
        {
            // Translate to Lowercase-ASCII
            // FullWidth-ASCII to half ASCII
            if ( (c >= 0xFF00) && (c <= 0xFF5E) )
            {
                c -= 0xFF00-0x0020;
                // Upper to Lower
                if ( (c >= 'A') && (c <= 'Z') )
                    c += 'a' - 'A';
                rName.SetChar( i, c );
            }
            else
            {
                // Only Fontnames with None-Ascii-Characters must be translated
                bNeedTranslation = true;
            }
        }
        // not lowercase Ascii
        else if ( !((c >= 'a') && (c <= 'z')) )
        {
            // To Lowercase-Ascii
            if ( (c >= 'A') && (c <= 'Z') )
            {
                c += 'a' - 'A';
                rName.SetChar( i, c );
            }
            else if( ((c < '0') || (c > '9')) && (c != ';') ) // not 0-9 or semicolon
            {
                // Remove white spaces and special characters
                rName.Erase( i, 1 );
                nLen--;
                continue;
            }
        }

        i++;
    }
#ifdef AVS
    // translate normalized localized name to its normalized English ASCII name
    if( bNeedTranslation )
    {
        typedef std::hash_map<const String, const char*,FontNameHash> FontNameDictionary;
        static FontNameDictionary aDictionary( sizeof(aImplLocalizedNamesList) / sizeof(*aImplLocalizedNamesList) );

        // the font name dictionary needs to be intialized once
        if( aDictionary.empty() )
        {
            // TODO: check if all dictionary entries are already normalized?
            const ImplLocalizedFontName* pList = aImplLocalizedNamesList;
            for(; pList->mpEnglishName; ++pList )
                aDictionary[ pList->mpLocalizedNames ] = pList->mpEnglishName;
        }

        FontNameDictionary::const_iterator it = aDictionary.find( rName );
        if( it != aDictionary.end() )
            rName.AssignAscii( it->second );
    }
#endif
}
}//SVMCore

// -----------------------------------------------------------------------

ImplFontCache::ImplFontCache( bool bPrinter )
:   mpFirstEntry( NULL ),
    mnRef0Count( 0 ),
    mbPrinter( bPrinter )
{}

// -----------------------------------------------------------------------

ImplFontCache::~ImplFontCache()
{
#ifdef AVS
    FontInstanceList::iterator it = maFontInstanceList.begin();
    for(; it != maFontInstanceList.end(); ++it )
    {
        ImplFontEntry* pEntry = (*it).second;
        delete pEntry;
    }
#endif
}
// -----------------------------------------------------------------------

void ImplFontCache::Release( ImplFontEntry* pEntry )
{
    static const int FONTCACHE_MAX = 50;

   // DBG_ASSERT( (pEntry->mnRefCount > 0), "ImplFontCache::Release() - font refcount underflow" );
    if( --pEntry->mnRefCount > 0 )
        return;

    if( ++mnRef0Count < FONTCACHE_MAX )
        return;
#ifdef AVS
    // remove unused entries from font instance cache
    FontInstanceList::iterator it_next = maFontInstanceList.begin();
    while( it_next != maFontInstanceList.end() )
    {
        FontInstanceList::iterator it = it_next++;
        ImplFontEntry* pFontEntry = (*it).second;
        if( pFontEntry->mnRefCount > 0 )
            continue;

        maFontInstanceList.erase( it );
        delete pFontEntry;
        --mnRef0Count;
        DBG_ASSERT( (mnRef0Count>=0), "ImplFontCache::Release() - refcount0 underflow" );

        if( mpFirstEntry == pFontEntry )
            mpFirstEntry = NULL;
    }
#endif
    //DBG_ASSERT( (mnRef0Count==0), "ImplFontCache::Release() - refcount0 mismatch" );
}
// -----------------------------------------------------------------------

ImplFontEntry* ImplFontCache::GetFontEntry( ImplDevFontList* pFontList,
    const Font& rFont, const Size& rSize, float fExactHeight, ImplDirectFontSubstitution* pDevSpecific )
{
    String aSearchName = rFont.GetName();
#ifdef QWE
    // TODO: also add device specific name caching
    if( !pDevSpecific )
    {
        // check if the requested font name is already known
        // if it is already known get its normalized search name
        FontNameList::const_iterator it_name = maFontNameList.find( aSearchName );
        if( it_name != maFontNameList.end() )
            if( !(*it_name).second.EqualsAscii( "hg", 0, 2) )
                aSearchName = (*it_name).second;
    }
#endif
    // initialize internal font request object
    ImplFontSelectData aFontSelData( rFont, aSearchName, rSize, fExactHeight );
    return GetFontEntry( pFontList, aFontSelData, pDevSpecific );
}

// -----------------------------------------------------------------------

ImplFontEntry* ImplFontCache::GetFontEntry( ImplDevFontList* pFontList,
    ImplFontSelectData& aFontSelData, ImplDirectFontSubstitution* pDevSpecific )
{
    // check if a directly matching logical font instance is already cached,
    // the most recently used font usually has a hit rate of >50%
    ImplFontEntry *pEntry = NULL;
    ImplDevFontListData* pFontFamily = NULL;
#ifdef  AVS
    IFSD_Equal aIFSD_Equal;
    if( mpFirstEntry && aIFSD_Equal( aFontSelData, mpFirstEntry->maFontSelData ) )
        pEntry = mpFirstEntry;
    else
    {
        FontInstanceList::iterator it = maFontInstanceList.find( aFontSelData );
        if( it != maFontInstanceList.end() )
            pEntry = (*it).second;
    }

    if( !pEntry ) // no direct cache hit
    {
#endif
		int nCount = pFontList->Count();
        // find the best matching logical font family and update font selector accordingly
        pFontFamily = pFontList->ImplFindByFont( aFontSelData, mbPrinter, pDevSpecific );
        //DBG_ASSERT( (pFontFamily != NULL), "ImplFontCache::Get() No logical font found!" );
        if( pFontFamily )
            aFontSelData.maSearchName = pFontFamily->GetSearchName();
#ifdef AVS
        // check if an indirectly matching logical font instance is already cached
        FontInstanceList::iterator it = maFontInstanceList.find( aFontSelData );
        if( it != maFontInstanceList.end() )
        {
            // we have an indirect cache hit
            pEntry = (*it).second;
            // cache the requested and the selected font names
            // => next time there is a good chance for a direct cache hit
            // don't allow the cache to grow too big
            // TODO: implement some fancy LRU caching?
            if( maFontNameList.size() >= 4000 )
                maFontNameList.clear();
            // TODO: also add device specific name caching
            if( !pDevSpecific )
                if( aFontSelData.maName != aFontSelData.maSearchName )
                    maFontNameList[ aFontSelData.maName ] = aFontSelData.maSearchName;
        }
    }
    if( pEntry ) // cache hit => use existing font instance
    {
        // increase the font instance's reference count
        if( !pEntry->mnRefCount++ )
            --mnRef0Count;
    }

    else // no cache hit => create a new font instance
    {
#endif
        // find the best matching physical font face
        ImplFontData* pFontData = pFontFamily->FindBestFontFace( aFontSelData );
        aFontSelData.mpFontData = pFontData;

        // create a new logical font instance from this physical font face
        pEntry = pFontData->CreateFontInstance( aFontSelData );
#ifdef QWE
        // if we found a different symbol font we need a symbol conversion table
        if( pFontData->IsSymbolFont() )
            if( aFontSelData.maTargetName != aFontSelData.maSearchName )
                pEntry->mpConversion = ImplGetRecodeData( aFontSelData.maTargetName, aFontSelData.maSearchName );
        // add the new entry to the cache
        maFontInstanceList[ aFontSelData ] = pEntry;
    }
#endif
    mpFirstEntry = pEntry;
    return pEntry;
}
struct FontMatchStatus
{
public:
    int                 mnFaceMatch;
    int                 mnHeightMatch;
    int                 mnWidthMatch;
    const xub_Unicode*  mpTargetStyleName;
};
// -----------------------------------------------------------------------

ImplDevFontListData* ImplDevFontList::ImplFindByFont( ImplFontSelectData& rFSD,
    bool bPrinter, ImplDirectFontSubstitution* pDevSpecific ) const
{
    // give up if no fonts are available
    if( !Count() )
        return NULL;
    // test if a font in the token list is available
    // substitute the font if this was requested
    USHORT nSubstFlags = FONT_SUBSTITUTE_ALWAYS;
    if ( bPrinter )
        nSubstFlags |= FONT_SUBSTITUTE_SCREENONLY;

    bool bMultiToken = false;
    xub_StrLen nTokenPos = 0;
    String& aSearchName = rFSD.maSearchName; // TODO: get rid of reference
    for(;;)
    {
        rFSD.maTargetName = GetNextFontToken( rFSD.maName, nTokenPos );
        aSearchName = rFSD.maTargetName;
#ifdef AVS
        ImplGetEnglishSearchFontName( aSearchName );
        ImplFontSubstitute( aSearchName, nSubstFlags, pDevSpecific );
        // #114999# special emboldening for Ricoh fonts
        // TODO: smarter check for special cases by using PreMatch infrastructure?
        if( (rFSD.meWeight > WEIGHT_MEDIUM)
        &&  aSearchName.EqualsAscii( "hg", 0, 2) )
        {
            String aBoldName;
            if( aSearchName.EqualsAscii( "hggothicb", 0, 9) )
                aBoldName = String(RTL_CONSTASCII_USTRINGPARAM("hggothice"));
            else if( aSearchName.EqualsAscii( "hgpgothicb", 0, 10) )
                aBoldName = String(RTL_CONSTASCII_USTRINGPARAM("hgpgothice"));
            else if( aSearchName.EqualsAscii( "hgminchol", 0, 9) )
                aBoldName = String(RTL_CONSTASCII_USTRINGPARAM("hgminchob"));
            else if( aSearchName.EqualsAscii( "hgpminchol", 0, 10) )
                aBoldName = String(RTL_CONSTASCII_USTRINGPARAM("hgpminchob"));
            else if( aSearchName.EqualsAscii( "hgminchob" ) )
                aBoldName = String(RTL_CONSTASCII_USTRINGPARAM("hgminchoe"));
            else if( aSearchName.EqualsAscii( "hgpminchob" ) )
                aBoldName = String(RTL_CONSTASCII_USTRINGPARAM("hgpminchoe"));

            if( aBoldName.Len() && ImplFindBySearchName( aBoldName ) )
            {
                // the other font is available => use it
                aSearchName = aBoldName;
                // prevent synthetic emboldening of bold version
                rFSD.meWeight = WEIGHT_DONTKNOW;
            }
        }
#endif
        // check if the current font name token or its substitute is valid
        ImplDevFontListData* pFoundData = ImplFindBySearchName( aSearchName );
        if( pFoundData )
            return pFoundData;
#ifdef AVS
        // some systems provide special customization
        // e.g. they suggest "serif" as UI-font, but this name cannot be used directly
        //      because the system wants to map it to another font first, e.g. "Helvetica"
        if( mpPreMatchHook )
        {
            if( mpPreMatchHook->FindFontSubstitute( rFSD ) )
            {
                ImplGetEnglishSearchFontName( aSearchName );
                pFoundData = ImplFindBySearchName( aSearchName );
                if( pFoundData )
                    return pFoundData;
            }
        }
#endif
        // break after last font name token was checked unsuccessfully
        if( nTokenPos == STRING_NOTFOUND)
            break;
        bMultiToken = true;
    }
#ifdef AVS
    // if the first font was not available find the next available font in
    // the semicolon separated list of font names. A font is also considered
    // available when there is a matching entry in the Tools->Options->Fonts
    // dialog witho neither ALWAYS nor SCREENONLY flags set and the substitution
    // font is available
    for( nTokenPos = 0; nTokenPos != STRING_NOTFOUND; )
    {
        if( bMultiToken )
        {
            rFSD.maTargetName = GetNextFontToken( rFSD.maName, nTokenPos );
            aSearchName = rFSD.maTargetName;
            ImplGetEnglishSearchFontName( aSearchName );
        }
        else
            nTokenPos = STRING_NOTFOUND;
        if( mpPreMatchHook )
            if( mpPreMatchHook->FindFontSubstitute( rFSD ) )
                ImplGetEnglishSearchFontName( aSearchName );
        ImplFontSubstitute( aSearchName, nSubstFlags, pDevSpecific );
        ImplDevFontListData* pFoundData = ImplFindBySearchName( aSearchName );
        if( pFoundData )
            return pFoundData;
    }

    // if no font with a directly matching name is available use the
    // first font name token and get its attributes to find a replacement
    if ( bMultiToken )
    {
        nTokenPos = 0;
        rFSD.maTargetName = GetNextFontToken( rFSD.maName, nTokenPos );
        aSearchName = rFSD.maTargetName;
        ImplGetEnglishSearchFontName( aSearchName );
    }

    String      aSearchShortName;
    String      aSearchFamilyName;
    FontWeight  eSearchWeight   = rFSD.meWeight;
    FontWidth   eSearchWidth    = rFSD.meWidthType;
    ULONG       nSearchType     = 0;
    FontSubstConfiguration::getMapName( aSearchName, aSearchShortName, aSearchFamilyName,
                                        eSearchWeight, eSearchWidth, nSearchType );

    // note: the search name was already translated to english (if possible)

    // use the font's shortened name if needed
    if ( aSearchShortName != aSearchName )
    {
       ImplDevFontListData* pFoundData = ImplFindBySearchName( aSearchShortName );
       if( pFoundData )
       {
#ifdef UNX
            /* #96738# don't use mincho as an replacement for "MS Mincho" on X11: Mincho is
            a korean bitmap font that is not suitable here. Use the font replacement table,
            that automatically leads to the desired "HG Mincho Light J". Same story for
            MS Gothic, there are thai and korean "Gothic" fonts, so we even prefer Andale */
            static String aMS_Mincho( RTL_CONSTASCII_USTRINGPARAM("msmincho") );
            static String aMS_Gothic( RTL_CONSTASCII_USTRINGPARAM("msgothic") );
            if ((aSearchName != aMS_Mincho) && (aSearchName != aMS_Gothic))
                // TODO: add heuristic to only throw out the fake ms* fonts
#endif
            {
                return pFoundData;
            }
        }
    }

    // use font fallback
    const FontNameAttr* pFontAttr = NULL;
    if( aSearchName.Len() )
    {
        // get fallback info using FontSubstConfiguration and
        // the target name, it's shortened name and family name in that order
        const FontSubstConfiguration& rFontSubst = *FontSubstConfiguration::get();
        pFontAttr = rFontSubst.getSubstInfo( aSearchName );
        if ( !pFontAttr && (aSearchShortName != aSearchName) )
            pFontAttr = rFontSubst.getSubstInfo( aSearchShortName );
        if ( !pFontAttr && (aSearchFamilyName != aSearchShortName) )
            pFontAttr = rFontSubst.getSubstInfo( aSearchFamilyName );

        // try the font substitutions suggested by the fallback info
        if( pFontAttr )
        {
            ImplDevFontListData* pFoundData = ImplFindBySubstFontAttr( *pFontAttr );
            if( pFoundData )
                return pFoundData;
        }
    }

    // if a target symbol font is not available use a default symbol font
    if( rFSD.IsSymbolFont() )
    {
        com::sun::star::lang::Locale aDefaultLocale( OUString( RTL_CONSTASCII_USTRINGPARAM("en") ), OUString(), OUString() );
        aSearchName = DefaultFontConfiguration::get()->getDefaultFont( aDefaultLocale, DEFAULTFONT_SYMBOL );
        ImplDevFontListData* pFoundData = ImplFindByTokenNames( aSearchName );
        if( pFoundData )
            return pFoundData;
    }

    // now try the other font name tokens
    while( nTokenPos != STRING_NOTFOUND )
    {
        rFSD.maTargetName = GetNextFontToken( rFSD.maName, nTokenPos );
        if( !rFSD.maTargetName.Len() )
            continue;

        aSearchName = rFSD.maTargetName;
        ImplGetEnglishSearchFontName( aSearchName );

        String      aTempShortName;
        String      aTempFamilyName;
        ULONG       nTempType   = 0;
        FontWeight  eTempWeight = rFSD.meWeight;
        FontWidth   eTempWidth  = WIDTH_DONTKNOW;
        FontSubstConfiguration::getMapName( aSearchName, aTempShortName, aTempFamilyName,
                                            eTempWeight, eTempWidth, nTempType );

        // use a shortend token name if available
        if( aTempShortName != aSearchName )
        {
            ImplDevFontListData* pFoundData = ImplFindBySearchName( aTempShortName );
            if( pFoundData )
                return pFoundData;
        }

        // use a font name from font fallback list to determine font attributes

        // get fallback info using FontSubstConfiguration and
        // the target name, it's shortened name and family name in that order
        const FontSubstConfiguration& rFontSubst = *FontSubstConfiguration::get();
        const FontNameAttr* pTempFontAttr = rFontSubst.getSubstInfo( aSearchName );
        if ( !pTempFontAttr && (aTempShortName != aSearchName) )
            pTempFontAttr = rFontSubst.getSubstInfo( aTempShortName );
        if ( !pTempFontAttr && (aTempFamilyName != aTempShortName) )
            pTempFontAttr = rFontSubst.getSubstInfo( aTempFamilyName );

        // try the font substitutions suggested by the fallback info
        if( pTempFontAttr )
        {
            ImplDevFontListData* pFoundData = ImplFindBySubstFontAttr( *pTempFontAttr );
            if( pFoundData )
                return pFoundData;
            if( !pFontAttr )
                pFontAttr = pTempFontAttr;
        }
    }

    // if still needed use the alias names of the installed fonts
    if( mbMapNames )
    {
        ImplDevFontListData* pFoundData = ImplFindByAliasName( rFSD.maTargetName, aSearchShortName );
        if( pFoundData )
            return pFoundData;
    }

    // if still needed use the font request's attributes to find a good match
    switch( rFSD.meLanguage )
    {
        case LANGUAGE_CHINESE:
        case LANGUAGE_CHINESE_SIMPLIFIED:
        case LANGUAGE_CHINESE_SINGAPORE:
            nSearchType |= IMPL_FONT_ATTR_CJK | IMPL_FONT_ATTR_CJK_SC;
            break;
        case LANGUAGE_CHINESE_TRADITIONAL:
        case LANGUAGE_CHINESE_HONGKONG:
        case LANGUAGE_CHINESE_MACAU:
            nSearchType |= IMPL_FONT_ATTR_CJK | IMPL_FONT_ATTR_CJK_TC;
            break;
        case LANGUAGE_KOREAN:
        case LANGUAGE_KOREAN_JOHAB:
            nSearchType |= IMPL_FONT_ATTR_CJK | IMPL_FONT_ATTR_CJK_KR;
            break;
        case LANGUAGE_JAPANESE:
            nSearchType |= IMPL_FONT_ATTR_CJK | IMPL_FONT_ATTR_CJK_JP;
            break;
        default:
            nSearchType |= ImplIsCJKFont( rFSD.maName );
            if( rFSD.IsSymbolFont() )
                nSearchType |= IMPL_FONT_ATTR_SYMBOL;
            break;
    }

    ImplCalcType( nSearchType, eSearchWeight, eSearchWidth, rFSD.meFamily, pFontAttr );
    ImplDevFontListData* pFoundData = ImplFindByAttributes( nSearchType,
        eSearchWeight, eSearchWidth, rFSD.meFamily, rFSD.meItalic, aSearchFamilyName );

    if( pFoundData )
    {
        // overwrite font selection attributes using info from the typeface flags
        if( (eSearchWeight >= WEIGHT_BOLD)
        &&  (eSearchWeight > rFSD.meWeight)
        &&  (pFoundData->mnTypeFaces & IMPL_DEVFONT_BOLD) )
            rFSD.meWeight = eSearchWeight;
        else if( (eSearchWeight < WEIGHT_NORMAL)
        &&  (eSearchWeight < rFSD.meWeight)
        &&  (eSearchWeight != WEIGHT_DONTKNOW)
        &&  (pFoundData->mnTypeFaces & IMPL_DEVFONT_LIGHT) )
            rFSD.meWeight = eSearchWeight;

        if( (nSearchType & IMPL_FONT_ATTR_ITALIC)
        &&  ((rFSD.meItalic == ITALIC_DONTKNOW) || (rFSD.meItalic == ITALIC_NONE))
        &&  (pFoundData->mnTypeFaces & IMPL_DEVFONT_ITALIC) )
            rFSD.meItalic = ITALIC_NORMAL;
    }
    else
    {
        // if still needed fall back to default fonts
        pFoundData = FindDefaultFont();
    }
    return pFoundData;
#endif
    return FindDefaultFont();
}
// -----------------------------------------------------------------------

ImplDevFontListData* ImplDevFontList::FindDefaultFont() const
{
#ifdef AVS
    // try to find one of the default fonts of the
    // UNICODE, SANSSERIF, SERIF or FIXED default font lists
    const DefaultFontConfiguration& rDefaults = *DefaultFontConfiguration::get();
    com::sun::star::lang::Locale aLocale( OUString( RTL_CONSTASCII_USTRINGPARAM("en") ), OUString(), OUString() );
    String aFontname = rDefaults.getDefaultFont( aLocale, DEFAULTFONT_SANS_UNICODE );
    ImplDevFontListData* pFoundData = ImplFindByTokenNames( aFontname );
    if( pFoundData )
        return pFoundData;

    aFontname = rDefaults.getDefaultFont( aLocale, DEFAULTFONT_SANS );
    pFoundData = ImplFindByTokenNames( aFontname );
    if( pFoundData )
        return pFoundData;

    aFontname = rDefaults.getDefaultFont( aLocale, DEFAULTFONT_SERIF );
    pFoundData = ImplFindByTokenNames( aFontname );
    if( pFoundData )
        return pFoundData;

    aFontname = rDefaults.getDefaultFont( aLocale, DEFAULTFONT_FIXED );
    pFoundData = ImplFindByTokenNames( aFontname );
    if( pFoundData )
        return pFoundData;

    // now try to find a reasonable non-symbol font

    InitMatchData();

    DevFontList::const_iterator it = maDevFontList.begin();
    for(; it !=  maDevFontList.end(); ++it )
    {
        ImplDevFontListData* pData = (*it).second;
        if( pData->mnMatchType & IMPL_FONT_ATTR_SYMBOL )
            continue;
        pFoundData = pData;
        if( pData->mnMatchType & (IMPL_FONT_ATTR_DEFAULT|IMPL_FONT_ATTR_STANDARD) )
            break;
    }
    if( pFoundData )
        return pFoundData;
#endif
	ImplDevFontListData* pFoundData = NULL;
	DevFontList::const_iterator it = maDevFontList.find( String(L"Arial") );
    if( it !=  maDevFontList.end() )
        pFoundData = (*it).second;

	if( NULL == pFoundData )
	{
		DevFontList::const_iterator it = maDevFontList.begin();
		// finding any font is better than finding no font at all
		it = maDevFontList.begin();
		if( it !=  maDevFontList.end() )
			pFoundData = (*it).second;
	}

    return pFoundData;
}
// find the font from the normalized font family name
ImplDevFontListData* ImplDevFontList::ImplFindBySearchName( const String& rSearchName ) const
{
#ifdef AVS
#ifdef DEBUG
    String aTempName = rSearchName;
    ImplGetEnglishSearchFontName( aTempName );
    DBG_ASSERT( aTempName == rSearchName, "ImplDevFontList::ImplFindBySearchName() called with non-normalized name" );
#endif
#endif

    DevFontList::const_iterator it = maDevFontList.find( rSearchName );
    if( it == maDevFontList.end() )
        return NULL;

    ImplDevFontListData* pFoundData = (*it).second;
    return pFoundData;

	return NULL;
}
// =======================================================================

ImplFontData::ImplFontData( const ImplDevFontAttributes& rDFA, int nMagic )
:   ImplDevFontAttributes( rDFA ),
    mnWidth(0),
    mnHeight(0),
    mnMagic( nMagic ),
    mpNext( NULL )
{
    // StarSymbol is a unicode font, but it still deserves the symbol flag
    if( !mbSymbolFlag )
        if( 0 == GetFamilyName().CompareIgnoreCaseToAscii( "starsymbol", 10)
        ||  0 == GetFamilyName().CompareIgnoreCaseToAscii( "opensymbol", 10) )
            mbSymbolFlag = true;
}
bool ImplFontData::IsBetterMatch( const ImplFontSelectData& rFSD, FontMatchStatus& rStatus ) const
{
    int nMatch = 0;

    const String& rFontName = rFSD.maTargetName;
    if( (rFontName == maName) || rFontName.EqualsIgnoreCaseAscii( maName ) )
        nMatch += 240000;

    if( rStatus.mpTargetStyleName
    &&  maStyleName.EqualsIgnoreCaseAscii( rStatus.mpTargetStyleName ) )
        nMatch += 120000;

    if( (rFSD.mePitch != PITCH_DONTKNOW) && (rFSD.mePitch == mePitch) )
        nMatch += 20000;

    // prefer NORMAL font width
    // TODO: change when the upper layers can tell their width preference
    if( meWidthType == WIDTH_NORMAL )
        nMatch += 400;
    else if( (meWidthType == WIDTH_SEMI_EXPANDED) || (meWidthType == WIDTH_SEMI_CONDENSED) )
        nMatch += 300;

    if( rFSD.meWeight != WEIGHT_DONTKNOW )
    {
        // if not bold prefer light fonts to bold fonts
        int nReqWeight = (int)rFSD.meWeight;
        if ( rFSD.meWeight > WEIGHT_MEDIUM )
            nReqWeight += 100;

        int nGivenWeight = (int)meWeight;
        if( meWeight > WEIGHT_MEDIUM )
            nGivenWeight += 100;

        int nWeightDiff = nReqWeight - nGivenWeight;

        if ( nWeightDiff == 0 )
            nMatch += 1000;
        else if ( nWeightDiff == +1 || nWeightDiff == -1 )
            nMatch += 700;
        else if ( nWeightDiff < +50 && nWeightDiff > -50)
            nMatch += 200;
    }
    else // requested weight == WEIGHT_DONTKNOW
    {
        // prefer NORMAL font weight
        // TODO: change when the upper layers can tell their weight preference
        if( meWeight == WEIGHT_NORMAL )
            nMatch += 450;
        else if( meWeight == WEIGHT_MEDIUM )
            nMatch += 350;
        else if( (meWeight == WEIGHT_SEMILIGHT) || (meWeight == WEIGHT_SEMIBOLD) )
            nMatch += 200;
        else if( meWeight == WEIGHT_LIGHT )
            nMatch += 150;
    }

    if ( rFSD.meItalic == ITALIC_NONE )
    {
        if( meItalic == ITALIC_NONE )
            nMatch += 900;
    }
    else
    {
        if( rFSD.meItalic == meItalic )
            nMatch += 900;
        else if( meItalic != ITALIC_NONE )
            nMatch += 600;
    }

    if( mbDevice )
        nMatch += 1;

    int nHeightMatch = 0;
    int nWidthMatch = 0;

    if( IsScalable() )
    {
        if( rFSD.mnOrientation != 0 )
            nMatch += 80;
        else if( rFSD.mnWidth != 0 )
            nMatch += 25;
        else
            nMatch += 5;
    }
    else
    {
        if( rFSD.mnHeight == mnHeight )
        {
            nMatch += 20;
            if( rFSD.mnWidth == mnWidth )
                nMatch += 10;
        }
        else
        {
            // for non-scalable fonts the size difference is very important
            // prefer the smaller font face because of clipping/overlapping issues
            int nHeightDiff = (rFSD.mnHeight - mnHeight) * 1000;
            nHeightMatch = (nHeightDiff >= 0) ? -nHeightDiff : 100+nHeightDiff;
            if( rFSD.mnHeight )
                nHeightMatch /= rFSD.mnHeight;

            if( (rFSD.mnWidth != 0) && (mnWidth != 0) && (rFSD.mnWidth != mnWidth) )
            {
                int nWidthDiff = (rFSD.mnWidth - mnWidth) * 100;
                nWidthMatch = (nWidthDiff >= 0) ? -nWidthDiff : +nWidthDiff;
            }
        }
    }

    if( rStatus.mnFaceMatch > nMatch )
        return false;
    else if( rStatus.mnFaceMatch < nMatch )
    {
        rStatus.mnFaceMatch      = nMatch;
        rStatus.mnHeightMatch    = nHeightMatch;
        rStatus.mnWidthMatch     = nWidthMatch;
        return true;
    }

    // when two fonts are still competing prefer the
    // one with the best matching height
    if( rStatus.mnHeightMatch > nHeightMatch )
        return false;
    else if( rStatus.mnHeightMatch < nHeightMatch )
    {
        rStatus.mnHeightMatch    = nHeightMatch;
        rStatus.mnWidthMatch     = nWidthMatch;
        return true;
    }

    if( rStatus.mnWidthMatch > nWidthMatch )
        return false;

    rStatus.mnWidthMatch = nWidthMatch;
    return true;
}
// -----------------------------------------------------------------------

StringCompare ImplFontData::CompareIgnoreSize( const ImplFontData& rOther ) const
{
    // compare their width, weight, italic and style name
    if( meWidthType < rOther.meWidthType )
        return COMPARE_LESS;
    else if( meWidthType > rOther.meWidthType )
        return COMPARE_GREATER;

    if( meWeight < rOther.meWeight )
        return COMPARE_LESS;
    else if( meWeight > rOther.meWeight )
        return COMPARE_GREATER;

    if( meItalic < rOther.meItalic )
        return COMPARE_LESS;
    else if( meItalic > rOther.meItalic )
        return COMPARE_GREATER;

    StringCompare eCompare = maName.CompareTo( rOther.maName );
    return eCompare;
}

// -----------------------------------------------------------------------

StringCompare ImplFontData::CompareWithSize( const ImplFontData& rOther ) const
{
    StringCompare eCompare = CompareIgnoreSize( rOther );
    if( eCompare != COMPARE_EQUAL )
        return eCompare;

    if( mnHeight < rOther.mnHeight )
        return COMPARE_LESS;
    else if( mnHeight > rOther.mnHeight )
        return COMPARE_GREATER;

    if( mnWidth < rOther.mnWidth )
        return COMPARE_LESS;
    else if( mnWidth > rOther.mnWidth )
        return COMPARE_GREATER;

    return COMPARE_EQUAL;
}
// =======================================================================

ImplDevFontListData::ImplDevFontListData( const String& rSearchName )
:   mpFirst( NULL ),
    maSearchName( rSearchName ),
    mnTypeFaces( 0 ),
    mnMatchType( 0 ),
    meMatchWeight( WEIGHT_DONTKNOW ),
    meMatchWidth( WIDTH_DONTKNOW ),
    meFamily( FAMILY_DONTKNOW ),
    mePitch( PITCH_DONTKNOW ),
    mnMinQuality( -1 )
{}

// -----------------------------------------------------------------------

ImplDevFontListData::~ImplDevFontListData()
{
    // release all physical font faces
    while( mpFirst )
    {
        ImplFontData* pFace = mpFirst;
        mpFirst = pFace->GetNextFace();
        delete pFace;
    }
}
// -----------------------------------------------------------------------

bool ImplDevFontListData::AddFontFace( ImplFontData* pNewData )
{
    pNewData->mpNext = NULL;

    if( !mpFirst )
    {
        maName         = pNewData->maName;
        maMapNames     = pNewData->maMapNames;
        meFamily       = pNewData->meFamily;
        mePitch        = pNewData->mePitch;
        mnMinQuality   = pNewData->mnQuality;
    }
    else
    {
        if( meFamily == FAMILY_DONTKNOW )
            meFamily = pNewData->meFamily;
        if( mePitch == PITCH_DONTKNOW )
            mePitch = pNewData->mePitch;
        if( mnMinQuality > pNewData->mnQuality )
            mnMinQuality = pNewData->mnQuality;
    }

    // set attributes for attribute based font matching
    if( pNewData->IsScalable() )
        mnTypeFaces |= IMPL_DEVFONT_SCALABLE;

    if( pNewData->IsSymbolFont() )
        mnTypeFaces |= IMPL_DEVFONT_SYMBOL;
    else
        mnTypeFaces |= IMPL_DEVFONT_NONESYMBOL;

    if( pNewData->meWeight != WEIGHT_DONTKNOW )
    {
        if( pNewData->meWeight >= WEIGHT_SEMIBOLD )
            mnTypeFaces |= IMPL_DEVFONT_BOLD;
        else if( pNewData->meWeight <= WEIGHT_SEMILIGHT )
            mnTypeFaces |= IMPL_DEVFONT_LIGHT;
        else
            mnTypeFaces |= IMPL_DEVFONT_NORMAL;
    }

    if( pNewData->meItalic == ITALIC_NONE )
        mnTypeFaces |= IMPL_DEVFONT_NONEITALIC;
    else if( (pNewData->meItalic == ITALIC_NORMAL)
         ||  (pNewData->meItalic == ITALIC_OBLIQUE) )
        mnTypeFaces |= IMPL_DEVFONT_ITALIC;

    if( (meMatchWeight == WEIGHT_DONTKNOW)
    ||  (meMatchWidth  == WIDTH_DONTKNOW)
    ||  (mnMatchType   == 0) )
    {
        // TODO: is it cheaper to calc matching attributes now or on demand?
        // calc matching attributes if other entries are already initialized

        // MT: Perform05: Do lazy, quite expensive, not needed in start-up!
        // const FontSubstConfiguration& rFontSubst = *FontSubstConfiguration::get();
        // InitMatchData( rFontSubst, maSearchName );
		// mbMatchData=true; // Somewhere else???
    }

    // reassign name (sharing saves memory)
    if( pNewData->maName == maName )
        pNewData->maName = maName;

    // insert new physical font face into linked list
    // TODO: get rid of linear search?
    ImplFontData* pData;
    ImplFontData** ppHere = &mpFirst;

    for(; (pData=*ppHere) != NULL; ppHere=&pData->mpNext )
    {
        StringCompare eComp = pNewData->CompareWithSize( *pData );
        if( eComp == COMPARE_GREATER )
            continue;
        if( eComp == COMPARE_LESS )
            break;

        // ignore duplicate if its quality is worse
        if( pNewData->mnQuality < pData->mnQuality )
            return false;

        // keep the device font if its quality is good enough
        if( (pNewData->mnQuality == pData->mnQuality)
        &&  (pData->mbDevice || !pNewData->mbDevice) )
            return false;

        // replace existing font face with a better one
        pNewData->mpNext = pData->mpNext;
        *ppHere = pNewData;
        delete pData;
        return true;
    }

    // insert into or append to list of physical font faces
    pNewData->mpNext = pData;
    *ppHere = pNewData;
    return true;
}
// -----------------------------------------------------------------------

ImplFontData* ImplDevFontListData::FindBestFontFace( const ImplFontSelectData& rFSD ) const
{
    if( !mpFirst )
        return NULL;
    if( !mpFirst->GetNextFace() )
        return mpFirst;

    // FontName+StyleName should map to FamilyName+StyleName
    const String& rSearchName = rFSD.maTargetName;
    const xub_Unicode* pTargetStyleName = NULL;
    if( (rSearchName.Len() > maSearchName.Len())
    &&   rSearchName.Equals( maSearchName, 0, maSearchName.Len() ) )
        pTargetStyleName = rSearchName.GetBuffer() + maSearchName.Len() + 1;

    // linear search, TODO: improve?
    ImplFontData* pFontFace = mpFirst;
    ImplFontData* pBestFontFace = pFontFace;
    FontMatchStatus aFontMatchStatus = {0,0,0, pTargetStyleName};
    for(; pFontFace; pFontFace = pFontFace->GetNextFace() )
        if( pFontFace->IsBetterMatch( rFSD, aFontMatchStatus ) )
            pBestFontFace = pFontFace;

    return pBestFontFace;
}

// =======================================================================

ImplFontSelectData::ImplFontSelectData( const Font& rFont,
    const String& rSearchName, const Size& rSize, float fExactHeight)
:   maSearchName( rSearchName ),
    mnWidth( rSize.Width() ),
    mnHeight( rSize.Height() ),
    mfExactHeight( fExactHeight),
    mnOrientation( rFont.GetOrientation() ),
    meLanguage( rFont.GetLanguage() ),
    mbVertical( rFont.IsVertical() ),
    mbNonAntialiased( false ),
    mpFontData( NULL ),
    mpFontEntry( NULL )
{
    maTargetName = maName;

    rFont.GetFontAttributes( *this );

    // normalize orientation between 0 and 3600
    if( 3600 <= (unsigned)mnOrientation )
    {
        if( mnOrientation >= 0 )
            mnOrientation %= 3600;
        else
            mnOrientation = 3600 - (-mnOrientation % 3600);
    }

    // normalize width and height
    if( mnHeight < 0 )
        mnHeight = -mnHeight;
    if( mnWidth < 0 )
        mnWidth = -mnWidth;
}

// -----------------------------------------------------------------------

ImplFontSelectData::ImplFontSelectData( const ImplFontData& rFontData,
    const Size& rSize, float fExactHeight, int nOrientation, bool bVertical )
:   ImplFontAttributes( rFontData ),
    mnWidth( rSize.Width() ),
    mnHeight( rSize.Height() ),
    mfExactHeight( fExactHeight ),
    mnOrientation( nOrientation ),
    meLanguage( 0 ),
    mbVertical( bVertical ),
    mbNonAntialiased( false ),
    mpFontData( &rFontData ),
    mpFontEntry( NULL )
{
    maTargetName = maSearchName = maName;
    // NOTE: no normalization for width/height/orientation
}