/*************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 * 
 * Copyright 2008 by Sun Microsystems, Inc.
 *
 * OpenOffice.org - a multi-platform office productivity suite
 *
 * $RCSfile: bitmap.hxx,v $
 * $Revision: 1.8 $
 *
 * This file is part of OpenOffice.org.
 *
 * OpenOffice.org is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 3
 * only, as published by the Free Software Foundation.
 *
 * OpenOffice.org is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License version 3 for more details
 * (a copy is included in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU Lesser General Public License
 * version 3 along with OpenOffice.org.  If not, see
 * <http://www.openoffice.org/license.html>
 * for a copy of the LGPLv3 License.
 *
 ************************************************************************/

#ifndef _SV_BITMAP_HXX
#define _SV_BITMAP_HXX
#ifdef AVS
#include <vcl/sv.h>
#include <vcl/dllapi.h>
#include <vcl/mapmod.hxx>
#include <tools/rc.hxx>
#include <vcl/region.hxx>
#endif
#include "../ASC/OpenOfficeBasic.h"
#include "../vcl/region.hxx"
#include "../vcl/mapmod.hxx"

namespace SVMCore{
// -----------
// - Defines -
// -----------

#define BMP_MIRROR_NONE				0x00000000UL
#define BMP_MIRROR_HORZ				0x00000001UL
#define BMP_MIRROR_VERT				0x00000002UL

// -----------------------------------------------------------------------------

#define BMP_SCALE_NONE				0x00000000UL
#define BMP_SCALE_FAST				0x00000001UL
#define BMP_SCALE_INTERPOLATE		0x00000002UL

// -----------------------------------------------------------------------------

#define BMP_DITHER_NONE				0x00000000UL
#define BMP_DITHER_MATRIX			0x00000001UL
#define BMP_DITHER_FLOYD			0x00000002UL
#define BMP_DITHER_FLOYD_16			0x00000004UL

// -----------------------------------------------------------------------------

#define BMP_VECTORIZE_NONE			BMP_VECTORIZE_OUTER
#define BMP_VECTORIZE_INNER			0x00000001UL
#define BMP_VECTORIZE_OUTER			0x00000002UL
#define BMP_VECTORIZE_BOUND_ONLY	0x00000004UL
#define BMP_VECTORIZE_REDUCE_EDGES	0x00000008UL

// -----------------------------------------------------------------------------

#define BMP_COL_TRANS					Color( 252, 3, 251 )
#define BMP_COLOR_MONOCHROME_THRESHOLD	128

// ---------
// - Enums -
// ---------

enum BmpConversion
{
    BMP_CONVERSION_NONE = 0,
#ifdef AVS
    BMP_CONVERSION_1BIT_THRESHOLD = 1,
    BMP_CONVERSION_1BIT_MATRIX = 2,
    BMP_CONVERSION_4BIT_GREYS = 3,
    BMP_CONVERSION_4BIT_COLORS = 4,
    BMP_CONVERSION_8BIT_GREYS = 5,
    BMP_CONVERSION_8BIT_COLORS = 6,
#endif
    BMP_CONVERSION_24BIT = 7,
#ifdef AVS
    BMP_CONVERSION_4BIT_TRANS = 8,
    BMP_CONVERSION_8BIT_TRANS = 9,
#endif
    BMP_CONVERSION_GHOSTED = 10
};

// ------------------------------------------------------------------------

enum BmpCombine
{
    BMP_COMBINE_COPY = 0,
    BMP_COMBINE_INVERT = 1,
    BMP_COMBINE_AND = 2,
    BMP_COMBINE_NAND = 3,
    BMP_COMBINE_OR = 4,
    BMP_COMBINE_NOR = 5,
    BMP_COMBINE_XOR = 6,
    BMP_COMBINE_NXOR = 7
};

// ------------------------------------------------------------------------

enum BmpReduce
{
	BMP_REDUCE_SIMPLE = 0,
	BMP_REDUCE_POPULAR = 1,
	BMP_REDUCE_MEDIAN = 2
};

// ------------------------------------------------------------------------

enum BmpEmboss
{
	BMP_EMBOSS_TOPLEFT = 0,
	BMP_EMBOSS_TOP = 1,
	BMP_EMBOSS_TOPRIGHT = 2,
	BMP_EMBOSS_LEFT = 3,
	BMP_EMBOSS_MIDDLE = 4,
	BMP_EMBOSS_RIGHT = 5,
	BMP_EMBOSS_BOTTOMLEFT = 6,
	BMP_EMBOSS_BOTTOM = 7,
	BMP_EMBOSS_BOTTOMRIGHT = 8
};

// ------------------------------------------------------------------------

enum BmpFilter
{
    BMP_FILTER_SMOOTH = 0,
    BMP_FILTER_SHARPEN = 1,
    BMP_FILTER_REMOVENOISE = 2,
	BMP_FILTER_SOBEL_GREY = 3,
	BMP_FILTER_EMBOSS_GREY = 4,
	BMP_FILTER_SOLARIZE = 5,
	BMP_FILTER_SEPIA = 6,
	BMP_FILTER_MOSAIC = 7,
	BMP_FILTER_POPART = 8,

	BMP_FILTER_UNKNOWN = 65535
};

// ------------------------------------------------------------------------

enum BmpColorMode
{
    BMP_COLOR_NORMAL = 0,
    BMP_COLOR_HIGHCONTRAST = 1,
	BMP_COLOR_MONOCHROME_BLACK = 2,
	BMP_COLOR_MONOCHROME_WHITE = 3
};

// --------------------
// - FilterParameters -
// --------------------

class  BmpFilterParam
{
	friend class Bitmap;
	friend class BitmapEx;
	friend class Animation;

private:
	BmpFilter	meFilter;
	ULONG		mnProgressStart;
	ULONG		mnProgressEnd;

public:
	struct MosaicTileSize
	{
		ULONG mnTileWidth;
		ULONG mnTileHeight;
	};

	struct EmbossAngles
	{
		USHORT mnAzimuthAngle100;
		USHORT mnElevationAngle100;
	};

private:
	union
	{
		USHORT	mnSepiaPercent;
		BYTE	mcSolarGreyThreshold;

		MosaicTileSize maMosaicTileSize;
		EmbossAngles maEmbossAngles;
	};

public:

	BmpFilterParam( ULONG nProgressStart = 0, ULONG nProgressEnd = 0 ) :
		meFilter( BMP_FILTER_UNKNOWN ), mnProgressStart( nProgressStart ), mnProgressEnd( nProgressEnd ) {}

	BmpFilterParam( BYTE cSolarGreyThreshold, ULONG nProgressStart = 0, ULONG nProgressEnd = 0 ) :
		meFilter( BMP_FILTER_SOLARIZE ), mnProgressStart( nProgressStart ), mnProgressEnd( nProgressEnd ),
		mcSolarGreyThreshold( cSolarGreyThreshold ) {}

	BmpFilterParam( USHORT nSepiaPercent, ULONG nProgressStart = 0, ULONG nProgressEnd = 0 ) :
		meFilter( BMP_FILTER_SEPIA ), mnProgressStart( nProgressStart ), mnProgressEnd( nProgressEnd ),
		mnSepiaPercent( nSepiaPercent ) {}

	BmpFilterParam( const Size& rMosaicTileSize, ULONG nProgressStart = 0, ULONG nProgressEnd = 0 ) :
		meFilter( BMP_FILTER_MOSAIC ), mnProgressStart( nProgressStart ), mnProgressEnd( nProgressEnd )
		{
			maMosaicTileSize.mnTileWidth = rMosaicTileSize.Width();
			maMosaicTileSize.mnTileHeight= rMosaicTileSize.Height();
		}
	BmpFilterParam( USHORT nEmbossAzimuthAngle100, USHORT nEmbossElevationAngle100,
					ULONG nProgressStart = 0, ULONG nProgressEnd = 0 ) :
		meFilter( BMP_FILTER_EMBOSS_GREY ), mnProgressStart( nProgressStart ), mnProgressEnd( nProgressEnd )
		{
			maEmbossAngles.mnAzimuthAngle100 = nEmbossAzimuthAngle100;
			maEmbossAngles.mnElevationAngle100 = nEmbossElevationAngle100;
		}
};

// ----------
// - Bitmap -
// ----------

class   BitmapReadAccess;
class   BitmapWriteAccess;
class   BitmapPalette;
class   ImpBitmap;
class   Color;
class   SvStream;
struct  DIBInfoHeader;
class   ResId;
class	GDIMetaFile;
class	AlphaMask;
class   OutputDevice;
class   SalBitmap;

struct BitmapSystemData
{
    #if defined( WNT ) || defined( OS2 )
    void* pDIB; // device independent byte buffer
    void* pDDB; // if not NULL then this is actually an HBITMAP
    #elif defined( QUARTZ )
    void* rImageContext;     //Image context (CGContextRef)
    #else
    void* aPixmap;
    #endif
    int mnWidth;
    int mnHeight;
};

class  Bitmap
{
private:

    ImpBitmap*              mpImpBmp;
    MapMode                 maPrefMapMode;
    Size                    maPrefSize;

//#if 0 // _SOLAR__PRIVATE

public:

     void                 ImplReleaseRef();

     void                 ImplMakeUnique();
     ImpBitmap*           ImplGetImpBitmap() const;
     void                 ImplSetImpBitmap( ImpBitmap* pImpBmp );
     void                 ImplAssignWithSize( const Bitmap& rBitmap );
     static BOOL          ImplReadDIB( SvStream& rIStm, Bitmap& rBmp, ULONG nOffset );
     static BOOL          ImplReadDIBFileHeader( SvStream& rIStm, ULONG& rOffset );
     static BOOL          ImplReadDIBInfoHeader( SvStream& rIStm, DIBInfoHeader& rHeader, sal_Bool& bTopDown );
     static BOOL          ImplReadDIBPalette( SvStream& rIStm, BitmapWriteAccess& rAcc, BOOL bQuad );
     static BOOL          ImplReadDIBBits( SvStream& rIStm, DIBInfoHeader& rHeader, BitmapWriteAccess& rAcc, sal_Bool bTopDown );
#ifdef AVS
     BOOL                 ImplWriteDIB( SvStream& rOStm, BitmapReadAccess& rAcc, BOOL bCompressed ) const;
     static BOOL          ImplWriteDIBFileHeader( SvStream& rOStm, BitmapReadAccess& rAcc );
     static BOOL          ImplWriteDIBPalette( SvStream& rOStm, BitmapReadAccess& rAcc );
     static BOOL          ImplWriteDIBBits( SvStream& rOStm, BitmapReadAccess& rAcc,
                                                             ULONG nCompression, sal_uInt32& rImageSize );
#endif
     static void          ImplDecodeRLE( BYTE* pBuffer, DIBInfoHeader& rHeader,
                                           BitmapWriteAccess& rAcc, BOOL bRLE4 );
#ifdef AVS
     static BOOL          ImplWriteRLE( SvStream& rOStm, BitmapReadAccess& rAcc, BOOL bRLE4 );
#endif
     BOOL                 ImplScaleFast( const double& rScaleX, const double& rScaleY );
     BOOL                 ImplScaleInterpolate( const double& rScaleX, const double& rScaleY );

     BOOL                 ImplMakeMono( BYTE cThreshold );
     BOOL                 ImplMakeMonoDither();
     BOOL                 ImplMakeGreyscales( USHORT nGreyscales );

     BOOL                 ImplConvertUp( USHORT nBitCount, Color* pExtColor = NULL );
     BOOL                 ImplConvertDown( USHORT nBitCount, Color* pExtColor = NULL );
	 BOOL					ImplConvertGhosted();
#ifdef AVS
	 BOOL					ImplDitherMatrix();
	 BOOL					ImplDitherFloyd();
	 BOOL					ImplDitherFloyd16();
	 BOOL					ImplReduceSimple( USHORT nColorCount );
	 BOOL					ImplReducePopular( USHORT nColorCount );
	 BOOL					ImplReduceMedian( USHORT nColorCount );
	 void					ImplMedianCut( ULONG* pColBuf, BitmapPalette& rPal, 
										   long nR1, long nR2, long nG1, long nG2, long nB1, long nB2,
										   long nColors, long nPixels, long& rIndex );
	 BOOL					ImplConvolute3( const long* pMatrix, long nDivisor, 
											const BmpFilterParam* pFilterParam, const Link* pProgress );
	 BOOL					ImplMedianFilter( const BmpFilterParam* pFilterParam, const Link* pProgress );
	 BOOL					ImplSobelGrey( const BmpFilterParam* pFilterParam, const Link* pProgress );
	 BOOL					ImplEmbossGrey( const BmpFilterParam* pFilterParam, const Link* pProgress );
	 BOOL					ImplSolarize( const BmpFilterParam* pFilterParam, const Link* pProgress );
	 BOOL					ImplSepia( const BmpFilterParam* pFilterParam, const Link* pProgress );
	 BOOL					ImplMosaic( const BmpFilterParam* pFilterParam, const Link* pProgress );
	 BOOL					ImplPopArt( const BmpFilterParam* pFilterParam, const Link* pProgress );
#endif
//#endif // PRIVATE

public:

                            Bitmap();
                            Bitmap( const Bitmap& rBitmap );
                            Bitmap( const Size& rSizePixel, USHORT nBitCount, const BitmapPalette* pPal = NULL );
#ifdef AVS
                            Bitmap( const ResId& rResId );
#endif
                            Bitmap( SalBitmap* pSalBitmap );
                            ~Bitmap();

    Bitmap&                 operator=( const Bitmap& rBitmap );
    inline BOOL             operator!() const;
#ifdef AVS
    inline BOOL             operator==( const Bitmap& rBitmap ) const;
    inline BOOL             operator!=( const Bitmap& rBitmap ) const;

	inline BOOL				IsSameInstance( const Bitmap& rBmp ) const;
	BOOL					IsEqual( const Bitmap& rBmp ) const;
#endif
	inline BOOL				IsEmpty() const;

	void					SetEmpty();

    inline const MapMode&   GetPrefMapMode() const;
    inline void             SetPrefMapMode( const MapMode& rMapMode );

    inline const Size&      GetPrefSize() const;
    inline void             SetPrefSize( const Size& rSize );

    Size                    GetSizePixel() const;
#ifdef AVS
	void					SetSizePixel( const Size& rNewSize );

    /**
     * The pixel size of a bitmap's source (e.g. an image file)
     * and the pixel size of its resulting bitmap can differ,
     * e.g. when the image reader has its preview mode enabled.
     */
    Size                    GetSourceSizePixel() const;
    void                    SetSourceSizePixel( const Size& );

#endif
    USHORT                  GetBitCount() const;
#ifdef AVS
    inline ULONG            GetColorCount() const;
    inline ULONG            GetSizeBytes() const;
#endif
	BOOL					HasGreyPalette() const;
#ifdef AVS
    /** get system dependent bitmap data
    
        @param rData
        The system dependent BitmapSystemData structure to be filled
        
        @return TRUE if the bitmap has a valid system object (e.g. not empty)
    */
    bool                    GetSystemData( BitmapSystemData& rData ) const;

	ULONG					GetChecksum() const;
#endif
    Bitmap                  CreateDisplayBitmap( OutputDevice* pDisplay );
#ifdef AVS
    Bitmap                  GetColorTransformedBitmap( BmpColorMode eColorMode ) const;
#endif
	static const BitmapPalette& GetGreyPalette( int nEntries );

public:
#ifdef AVS
    BOOL MakeMono( BYTE cThreshold );
#endif
    /** Convert bitmap format

    	@param eConversion
        The format this bitmap should be converted to.

        @return TRUE, if the conversion was completed successfully.
     */
    BOOL                    Convert( BmpConversion eConversion );
#ifdef AVS
    /** Reduce number of colors for the bitmap

    	@param nNewColorCount
        Maximal number of bitmap colors after the reduce operation

        @param eReduce
        Algorithm to use for color reduction

        @return TRUE, if the color reduction operation was completed successfully.
     */
	BOOL					ReduceColors( USHORT nNewColorCount, 
										  BmpReduce eReduce = BMP_REDUCE_SIMPLE );

    /** Apply a dither algorithm to the bitmap

    	This method dithers the bitmap inplace, i.e. a true color
    	bitmap is converted to a paletted bitmap, reducing the color
    	deviation by error diffusion.

    	@param nDitherFlags
        The algorithm to be used for dithering

        @param pDitherPal
        A custom palette to be used when dithering (not yet implemented, leave NULL)
     */
	BOOL					Dither( ULONG nDitherFlags = BMP_DITHER_MATRIX );

    /** Crop the bitmap

    	@param rRectPixel
        A rectangle specifying the crop amounts on all four sides of
        the bitmap. If the upper left corner of the bitmap is assigned
        (0,0), then this method cuts out the given rectangle from the
        bitmap. Note that the rectangle is clipped to the bitmap's
        dimension, i.e. negative left,top rectangle coordinates or
        exceeding width or height is ignored.

        @return TRUE, if cropping was performed successfully. If
        nothing had to be cropped, because e.g. the crop rectangle
        included the bitmap, FALSE is returned, too!
     */
    BOOL                    Crop( const Rectangle& rRectPixel );

    /** Expand the bitmap by pixel padding

    	@param nDX 
        Number of pixel to pad at the right border of the bitmap

    	@param nDY
        Number of scanlines to pad at the bottom border of the bitmap

        @param pInitColor
        Color to use for padded pixel

        @return TRUE, if padding was performed successfully. FALSE is
        not only returned when the operation failed, but also if
        nothing had to be done, e.g. because nDX and nDY were zero.
     */
    BOOL                    Expand( ULONG nDX, ULONG nDY, 
									const Color* pInitColor = NULL );

    /** Copy a rectangular area from another bitmap

    	@param rRectDst
        Destination rectangle in this bitmap. This is clipped to the
        bitmap dimensions.

        @param rRectSrc
        Source rectangle in pBmpSrc. This is clipped to the source
        bitmap dimensions. Note further that no scaling takes place
        during this copy operation, i.e. only the minimum of source
        and destination rectangle's width and height are used.

        @param pBmpSrc
        The source bitmap to copy from. If this argument is NULL, or
        equal to the object this method is called on, copying takes
        place within the same bitmap.

        @return TRUE, if the operation completed successfully. FALSE
        is not only returned when the operation failed, but also if
        nothing had to be done, e.g. because one of the rectangles are
        empty.
     */
    BOOL                    CopyPixel( const Rectangle& rRectDst,
									   const Rectangle& rRectSrc,
									   const Bitmap* pBmpSrc = NULL );

    /** Perform boolean operations with another bitmap

    	@param rMask
        The mask bitmap in the selected combine operation
        
        @param eCombine
        The combine operation to perform on the bitmap

        @return TRUE, if the operation was completed successfully.
     */
    BOOL                    CombineSimple( const Bitmap& rMask,
										   BmpCombine eCombine );

    /** Alpha-blend the given bitmap against a specified uniform
      	background color.

		@attention This method might convert paletted bitmaps to
		truecolor, to be able to represent every necessary color. Note
		that during alpha blending, lots of colors not originally
		included in the bitmap can be generated.

        @param rAlpha
        Alpha mask to blend with

        @param rBackgroundColor
        Background color to use for every pixel during alpha blending

        @return TRUE, if blending was successful, FALSE otherwise
     */
    BOOL 					Blend( const AlphaMask& rAlpha, 
                                   const Color& 	rBackgroundColor );
#endif
    /** Fill the entire bitmap with the given color

    	@param rFillColor
        Color value to use for filling

        @return TRUE, if the operation was completed successfully.
     */
    BOOL                    Erase( const Color& rFillColor );
#ifdef AVS
    /** Perform the Invert operation on every pixel

        @return TRUE, if the operation was completed successfully.
     */
    BOOL                    Invert();
#endif
    /** Mirror the bitmap

    	@param nMirrorFlags
        About which axis (horizontal, vertical, or both) to mirror

        @return TRUE, if the operation was completed successfully.        
     */
    BOOL                    Mirror( ULONG nMirrorFlags );

    /** Scale the bitmap

    	@param rNewSize
        The resulting size of the scaled bitmap

        @param nScaleFlag
        The algorithm to be used for scaling

        @return TRUE, if the operation was completed successfully.        
     */
    BOOL                    Scale( const Size& rNewSize,
								   ULONG nScaleFlag = BMP_SCALE_FAST );

    /** Scale the bitmap

    	@param rScaleX
        The scale factor in x direction.

    	@param rScaleY
        The scale factor in y direction.

        @return TRUE, if the operation was completed successfully.        
     */
    BOOL                    Scale( const double& rScaleX, const double& rScaleY, 
								   ULONG nScaleFlag = BMP_SCALE_FAST );

    /** Rotate bitmap by the specified angle

    	@param nAngle10
        The rotation angle in tenth of a degree. The bitmap is always rotated around its center.

        @param rFillColor
        The color to use for filling blank areas. During rotation, the
        bitmap is enlarged such that the whole rotation result fits
        in. The empty spaces around that rotated original bitmap are
        then filled with this color.

        @return TRUE, if the operation was completed successfully.        
     */
    BOOL                    Rotate( long nAngle10, const Color& rFillColor );

    /** Create on-off mask from bitmap

    	This method creates a bitmask from the bitmap, where every
    	pixel that equals rTransColor is set transparent, the rest
    	opaque.

        @param rTransColor
        Color value where the bitmask should be transparent

        @param nTol
        Tolerance value. Specifies the maximal difference between
        rTransColor and the individual pixel values, such that the
        corresponding pixel is still regarded transparent.

        @return the resulting bitmask.
     */
    Bitmap                  CreateMask( const Color& rTransColor, ULONG nTol = 0UL ) const;
#ifdef AVS
    /** Create region of similar colors in a given rectangle

    	@param rColor
        All pixel which have this color are included in the calculated region

        @param rRect
        The rectangle within which matching pixel are looked for. This
        rectangle is always clipped to the bitmap dimensions.

        @return the generated region.
     */
	Region					CreateRegion( const Color& rColor, const Rectangle& rRect ) const;
#endif
    /** Replace all pixel where the given mask is on with the specified color

    	@param rMask
        Mask specifying which pixel should be replaced
        
        @param rReplaceColor
        Color to be placed in all changed pixel

        @return TRUE, if the operation was completed successfully.        
     */
    BOOL                    Replace( const Bitmap& rMask, const Color& rReplaceColor );

    /** Merge bitmap with given background color according to specified alpha mask

    	@param rAlpha
        Alpha mask specifying the amount of background color to merge in
        
        @param rMergeColor
        Background color to be used for merging

        @return TRUE, if the operation was completed successfully.        
     */
    BOOL                    Replace( const AlphaMask& rAlpha, const Color& rMergeColor );

    /** Replace all pixel having the search color with the specified color

    	@param rSearchColor
        Color specifying which pixel should be replaced
        
        @param rReplaceColor
        Color to be placed in all changed pixel

        @param nTol
        Tolerance value. Specifies the maximal difference between
        rSearchColor and the individual pixel values, such that the
        corresponding pixel is still regarded a match.

        @return TRUE, if the operation was completed successfully.        
     */
    BOOL                    Replace( const Color& rSearchColor, const Color& rReplaceColor, ULONG nTol = 0 );
#ifdef AVS
    /** Replace all pixel having one the search colors with the corresponding replace color

    	@param pSearchColor
        Array of colors specifying which pixel should be replaced
        
        @param pReplaceColor
        Array of colors to be placed in all changed pixel

        @param nColorCount
        Size of the aforementioned color arrays

        @param nTol
        Tolerance value. Specifies the maximal difference between
        pSearchColor colors and the individual pixel values, such that
        the corresponding pixel is still regarded a match.

        @return TRUE, if the operation was completed successfully.        
     */
    BOOL                    Replace( const Color* pSearchColors, const Color* rReplaceColors, 
									 ULONG nColorCount, ULONG* pTols = NULL );

    /** Convert the bitmap to a PolyPolygon    	

    	This works by putting continuous areas of the same color into
    	a polygon, by tracing its bounding line.

    	@param rPolyPoly
        The resulting PolyPolygon

        @param nFlags
        Whether the inline or the outline of the color areas should be
        represented by the polygon

        @param pProgress
        A callback for showing the progress of the vectorization

        @return TRUE, if the operation was completed successfully.        
     */
	BOOL					Vectorize( PolyPolygon& rPolyPoly, 
									   ULONG nFlags = BMP_VECTORIZE_OUTER,
									   const Link* pProgress = NULL );

    /** Convert the bitmap to a meta file

    	This works by putting continuous areas of the same color into
    	polygons painted in this color, by tracing the area's bounding
    	line.

    	@param rMtf
        The resulting meta file

        @param cReduce
        If non-null, minimal size of bound rects for individual polygons. Smaller ones are ignored.

        @param nFlags
        Whether the inline or the outline of the color areas should be
        represented by the polygon

        @param pProgress
        A callback for showing the progress of the vectorization

        @return TRUE, if the operation was completed successfully.        
     */
	BOOL					Vectorize( GDIMetaFile& rMtf, BYTE cReduce = 0,
									   ULONG nFlags = BMP_VECTORIZE_INNER,
									   const Link* pProgress = NULL );

    /** Change various global color characteristics

    	@param nLuminancePercent
        Percent of luminance change, valid range [-100,100]. Values outside this range are clipped to the valid range.

    	@param nContrastPercent
        Percent of contrast change, valid range [-100,100]. Values outside this range are clipped to the valid range.

    	@param nChannelRPercent
        Percent of red channel change, valid range [-100,100]. Values outside this range are clipped to the valid range.

    	@param nChannelGPercent
        Percent of green channel change, valid range [-100,100]. Values outside this range are clipped to the valid range.

    	@param nChannelBPercent
        Percent of blue channel change, valid range [-100,100]. Values outside this range are clipped to the valid range.

        @param fGamma
        Exponent of the gamma function applied to the bitmap. The
        value 1.0 results in no change, the valid range is
        (0.0,10.0]. Values outside this range are regarded as 1.0.

        @param bInvert
        If TRUE, invert the channel values with the logical 'not' operator

        @return TRUE, if the operation was completed successfully.        
     */
	BOOL					Adjust( short nLuminancePercent = 0,
									short nContrastPercent = 0,
									short nChannelRPercent = 0, 
									short nChannelGPercent = 0, 
									short nChannelBPercent = 0,
									double fGamma = 1.0,
									BOOL bInvert = FALSE );

    /** Apply specified filter to the bitmap

    	@param eFilter
        The filter algorithm to apply

        @param pFilterParam
        Various parameter for the different bitmap filter algorithms

        @param pProgress
        A callback for showing the progress of the vectorization
        
        @return TRUE, if the operation was completed successfully.        
     */
	BOOL					Filter( BmpFilter eFilter, 
									const BmpFilterParam* pFilterParam = NULL,
									const Link* pProgress = NULL );
#endif
public:
    BitmapReadAccess*       AcquireReadAccess();
    BitmapWriteAccess*      AcquireWriteAccess();
    void                    ReleaseAccess( BitmapReadAccess* pAccess );

public:

    BOOL                    Read( SvStream& rIStm, BOOL bFileHeader = TRUE );
#ifdef AVS
    BOOL                    Write( SvStream& rOStm, BOOL bCompressed = TRUE, BOOL bFileHeader = TRUE ) const;
#endif
    friend  SvStream&        operator>>( SvStream& rIStm, Bitmap& rBitmap );
#ifdef AVS
    friend  SvStream&        operator<<( SvStream& rOStm, const Bitmap& rBitmap );
#endif
};

// -----------
// - Inlines -
// -----------

inline BOOL Bitmap::operator!() const
{
    return( mpImpBmp == NULL );
}
#ifdef AVS
// ------------------------------------------------------------------

inline BOOL Bitmap::operator==( const Bitmap& rBitmap ) const
{
    return( rBitmap.mpImpBmp == mpImpBmp );
}

// ------------------------------------------------------------------

inline BOOL Bitmap::operator!=( const Bitmap& rBitmap ) const
{
    return( rBitmap.mpImpBmp != mpImpBmp );
}

// ------------------------------------------------------------------

inline BOOL Bitmap::IsSameInstance( const Bitmap& rBitmap ) const
{
    return( rBitmap.mpImpBmp == mpImpBmp );
}
#endif
// ------------------------------------------------------------------

inline BOOL	Bitmap::IsEmpty() const
{
    return( mpImpBmp == NULL );
}

// ------------------------------------------------------------------

inline const MapMode& Bitmap::GetPrefMapMode() const
{
    return maPrefMapMode;
}

// ------------------------------------------------------------------

inline void Bitmap::SetPrefMapMode( const MapMode& rMapMode )
{
    maPrefMapMode = rMapMode;
}

// ------------------------------------------------------------------

inline const Size& Bitmap::GetPrefSize() const
{
    return maPrefSize;
}

// ------------------------------------------------------------------

inline void Bitmap::SetPrefSize( const Size& rSize )
{
    maPrefSize = rSize;
}
#ifdef AVS
// ------------------------------------------------------------------

inline ULONG Bitmap::GetColorCount() const
{
    return( 1UL << (ULONG) GetBitCount() );
}

// ------------------------------------------------------------------

inline ULONG Bitmap::GetSizeBytes() const
{
    const Size aSizePix( GetSizePixel() );
    return( ( (ULONG) aSizePix.Width() * aSizePix.Height() * GetBitCount() ) >> 3UL );
}

#endif
}//SVMCore
#endif // _SV_BITMAP_HXX
