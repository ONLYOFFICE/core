/*************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 * 
 * Copyright 2008 by Sun Microsystems, Inc.
 *
 * OpenOffice.org - a multi-platform office productivity suite
 *
 * $RCSfile: metaact.hxx,v $
 * $Revision: 1.3.134.1 $
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

#ifndef _SV_METAACT_HXX
#define _SV_METAACT_HXX
#ifdef AVS
#include <vcl/dllapi.h>
#include <vcl/gradient.hxx>
#include <vcl/hatch.hxx>
#include <vcl/wall.hxx>
#include <vcl/font.hxx>
#include <tools/poly.hxx>
#include <vcl/bitmap.hxx>
#include <vcl/bitmapex.hxx>
#include <vcl/region.hxx>
#include <vcl/graph.hxx>
#include <vcl/outdev.hxx>
#include <vcl/gdimtf.hxx>
#include <vcl/gfxlink.hxx>
#include <vcl/lineinfo.hxx>
#endif
#include "OpenOfficeBasic.h"
#include "AvsBasic.h"
#include "tools/gen.hxx"
#include "tools/color.hxx"
#include "vcl/lineinfo.hxx"
#include "tools/poly.hxx"
#include "vcl/bitmap.hxx"
#include "vcl/bitmapex.hxx"
#include "vcl/gradient.hxx"
#include "vcl/hatch.hxx"
#include "vcl/wall.hxx"
#include "vcl/font.hxx"
#include "vcl/gdimtf.hxx"
#include "vcl/gfxlink.hxx"

#include "i18npool/lang.h"

namespace SVMCore{

class SvStream;
class OutputDevice;
class ImplMetaReadData;
class ImplMetaWriteData;


// -----------
// - Defines -
// -----------

#define META_NULL_ACTION					(0)
#define META_PIXEL_ACTION					(100)
#define META_POINT_ACTION					(101)
#define META_LINE_ACTION					(102)
#define META_RECT_ACTION					(103)
#define META_ROUNDRECT_ACTION				(104)
#define META_ELLIPSE_ACTION 				(105)
#define META_ARC_ACTION 					(106)
#define META_PIE_ACTION 					(107)
#define META_CHORD_ACTION					(108)
#define META_POLYLINE_ACTION				(109)
#define META_POLYGON_ACTION 				(110)
#define META_POLYPOLYGON_ACTION 			(111)
#define META_TEXT_ACTION					(112)
#define META_TEXTARRAY_ACTION				(113)
#define META_STRETCHTEXT_ACTION 			(114)
#define META_TEXTRECT_ACTION				(115)
#define META_BMP_ACTION 					(116)
#define META_BMPSCALE_ACTION				(117)
#define META_BMPSCALEPART_ACTION			(118)
#define META_BMPEX_ACTION					(119)
#define META_BMPEXSCALE_ACTION				(120)
#define META_BMPEXSCALEPART_ACTION			(121)
#define META_MASK_ACTION					(122)
#define META_MASKSCALE_ACTION				(123)
#define META_MASKSCALEPART_ACTION			(124)
#define META_GRADIENT_ACTION				(125)
#define META_HATCH_ACTION					(126)
#define META_WALLPAPER_ACTION				(127)
#define META_CLIPREGION_ACTION				(128)
#define META_ISECTRECTCLIPREGION_ACTION 	(129)
#define META_ISECTREGIONCLIPREGION_ACTION	(130)
#define META_MOVECLIPREGION_ACTION			(131)
#define META_LINECOLOR_ACTION				(132)
#define META_FILLCOLOR_ACTION				(133)
#define META_TEXTCOLOR_ACTION				(134)
#define META_TEXTFILLCOLOR_ACTION			(135)
#define META_TEXTALIGN_ACTION				(136)
#define META_MAPMODE_ACTION 				(137)
#define META_FONT_ACTION					(138)
#define META_PUSH_ACTION					(139)
#define META_POP_ACTION 					(140)
#define META_RASTEROP_ACTION				(141)
#define META_TRANSPARENT_ACTION 			(142)
#define META_EPS_ACTION 					(143)
#define META_REFPOINT_ACTION				(144)
#define META_TEXTLINECOLOR_ACTION			(145)
#define META_TEXTLINE_ACTION				(146)
#define META_FLOATTRANSPARENT_ACTION		(147)
#define META_GRADIENTEX_ACTION				(148)
#define META_LAYOUTMODE_ACTION 				(149)
#define META_TEXTLANGUAGE_ACTION 			(150)
#define META_OVERLINECOLOR_ACTION			(151)

#define META_COMMENT_ACTION 				(512)

// ------------------------------------------------------------------------

struct ImplMetaReadData
{
	rtl_TextEncoding		meActualCharSet;
};

// ------------------------------------------------------------------------

struct ImplMetaWriteData
{
	rtl_TextEncoding		meActualCharSet;
};

// ------------------------------------------------------------------------

#define DECL_META_ACTION( Name, nType ) 									\
						Meta##Name##Action();								\
protected:                                                                  \
	virtual 			~Meta##Name##Action();								\
public:                                                                     \
	virtual void		Execute( OutputDevice* pOut );						\
	/*virtual MetaAction* Clone();*/										\
	/*virtual void		Write( SvStream& rOStm, ImplMetaWriteData* pData );*/ \
	virtual void		Read( SvStream& rIStm, ImplMetaReadData* pData );

#define IMPL_META_ACTION( Name, nType ) 								\
Meta##Name##Action::Meta##Name##Action() :								\
	MetaAction( nType ) {}												\
Meta##Name##Action::~Meta##Name##Action() {}

// --------------
// - MetaAction -
// --------------

class  MetaAction
{
private:
	ULONG				mnRefCount;
	UINT16				mnType;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
protected:
	virtual 			~MetaAction();

public:
						MetaAction();
						MetaAction( USHORT nType );

	virtual void		Execute( OutputDevice* pOut );
#ifdef AVS
	virtual MetaAction* Clone();

	virtual void		Move( long nHorzMove, long nVertMove );
	virtual void		Scale( double fScaleX, double fScaleY );

	virtual void		Write( SvStream& rOStm, ImplMetaWriteData* pData );
#endif
	virtual void		Read( SvStream& rIStm, ImplMetaReadData* pData );
#ifdef AVS
	sal_Bool			IsEqual( const MetaAction& ) const;
#endif
	USHORT				GetType() const { return mnType; }
#ifdef AVS
	ULONG				GetRefCount() const { return mnRefCount; }
	void				ResetRefCount() { mnRefCount = 1; }
#endif
	void				Duplicate()  { mnRefCount++; }
	void				Delete() { if ( 0 == --mnRefCount ) delete this; }

public:
	static MetaAction*	ReadMetaAction( SvStream& rIStm, ImplMetaReadData* pData );
};

// -------------------
// - MetaPixelAction -
// -------------------

class  MetaPixelAction : public MetaAction
{
private:
	Point				maPt;
	Color				maColor;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( Pixel, META_PIXEL_ACTION )

						MetaPixelAction( const Point& rPt, const Color& rColor );
#ifdef AVS
	virtual void		Move( long nHorzMove, long nVertMove );
	virtual void		Scale( double fScaleX, double fScaleY );
#endif
	const Point&		GetPoint() const { return maPt; }
	const Color&		GetColor() const { return maColor; }
};

// -------------------
// - MetaPointAction -
// -------------------

class  MetaPointAction : public MetaAction
{
private:
	Point				maPt;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( Point, META_POINT_ACTION )

						MetaPointAction( const Point& rPt );
#ifdef AVS
	virtual void		Move( long nHorzMove, long nVertMove );
	virtual void		Scale( double fScaleX, double fScaleY );
#endif
	const Point&		GetPoint() const { return maPt; }
};

// ------------------
// - MetaLineAction -
// ------------------

class  MetaLineAction : public MetaAction
{
private:

	LineInfo			maLineInfo;
	Point				maStartPt;
	Point				maEndPt;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( Line, META_LINE_ACTION )

						MetaLineAction( const Point& rStart, const Point& rEnd );
						MetaLineAction( const Point& rStart, const Point& rEnd,
										const LineInfo& rLineInfo );
#ifdef AVS
	virtual void		Move( long nHorzMove, long nVertMove );
	virtual void		Scale( double fScaleX, double fScaleY );
#endif
	const Point&		GetStartPoint() const { return maStartPt; }
	const Point&		GetEndPoint() const { return maEndPt; }
	const LineInfo& 	GetLineInfo() const { return maLineInfo; }
};

// ------------------
// - MetaRectAction -
// ------------------

class  MetaRectAction : public MetaAction
{
private:

	Rectangle			maRect;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( Rect, META_RECT_ACTION )

						MetaRectAction( const Rectangle& rRect );
#ifdef AVS
	virtual void		Move( long nHorzMove, long nVertMove );
	virtual void		Scale( double fScaleX, double fScaleY );
#endif
	const Rectangle&	GetRect() const { return maRect; }
};

// -----------------------
// - MetaRoundRectAction -
// -----------------------

class  MetaRoundRectAction : public MetaAction
{
private:

	Rectangle			maRect;
	sal_uInt32			mnHorzRound;
	sal_uInt32			mnVertRound;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( RoundRect, META_ROUNDRECT_ACTION )

						MetaRoundRectAction( const Rectangle& rRect,
											 sal_uInt32 nHorzRound, sal_uInt32 nVertRound );
#ifdef AVS
	virtual void		Move( long nHorzMove, long nVertMove );
	virtual void		Scale( double fScaleX, double fScaleY );
#endif
	const Rectangle&	GetRect() const { return maRect; }
	sal_uInt32			GetHorzRound() const { return mnHorzRound; }
	sal_uInt32			GetVertRound() const { return mnVertRound; }
};

// ---------------------
// - MetaEllipseAction -
// ---------------------

class  MetaEllipseAction : public MetaAction
{
private:

	Rectangle			maRect;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( Ellipse, META_ELLIPSE_ACTION )

						MetaEllipseAction( const Rectangle& rRect );
#ifdef AVS
	virtual void		Move( long nHorzMove, long nVertMove );
	virtual void		Scale( double fScaleX, double fScaleY );
#endif
	const Rectangle&	GetRect() const { return maRect; }
};

// -----------------
// - MetaArcAction -
// -----------------

class  MetaArcAction : public MetaAction
{
private:

	Rectangle			maRect;
	Point				maStartPt;
	Point				maEndPt;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( Arc, META_ARC_ACTION )

						MetaArcAction( const Rectangle& rRect,
									   const Point& rStart, const Point& rEnd );
#ifdef AVS
	virtual void		Move( long nHorzMove, long nVertMove );
	virtual void		Scale( double fScaleX, double fScaleY );
#endif
	const Rectangle&	GetRect() const { return maRect; }
	const Point&		GetStartPoint() const { return maStartPt; }
	const Point&		GetEndPoint() const { return maEndPt; }
};

// -----------------
// - MetaPieAction -
// -----------------

class  MetaPieAction : public MetaAction
{
private:

	Rectangle			maRect;
	Point				maStartPt;
	Point				maEndPt;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( Pie, META_PIE_ACTION )

						MetaPieAction( const Rectangle& rRect,
									   const Point& rStart, const Point& rEnd );
#ifdef AVS
	virtual void		Move( long nHorzMove, long nVertMove );
	virtual void		Scale( double fScaleX, double fScaleY );
#endif
	const Rectangle&	GetRect() const { return maRect; }
	const Point&		GetStartPoint() const { return maStartPt; }
	const Point&		GetEndPoint() const { return maEndPt; }
};

// -------------------
// - MetaChordAction -
// -------------------

class  MetaChordAction : public MetaAction
{
private:

	Rectangle			maRect;
	Point				maStartPt;
	Point				maEndPt;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( Chord, META_CHORD_ACTION )

						MetaChordAction( const Rectangle& rRect,
										 const Point& rStart, const Point& rEnd );
#ifdef AVS
	virtual void		Move( long nHorzMove, long nVertMove );
	virtual void		Scale( double fScaleX, double fScaleY );
#endif
	const Rectangle&	GetRect() const { return maRect; }
	const Point&		GetStartPoint() const { return maStartPt; }
	const Point&		GetEndPoint() const { return maEndPt; }
};

// ----------------------
// - MetaPolyLineAction -
// ----------------------

class  MetaPolyLineAction : public MetaAction
{
private:

	LineInfo			maLineInfo;
	Polygon 			maPoly;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( PolyLine, META_POLYLINE_ACTION )

						MetaPolyLineAction( const Polygon& rPoly );
						MetaPolyLineAction( const Polygon& rPoly, const LineInfo& rLineInfo );
#ifdef AVS
	virtual void		Move( long nHorzMove, long nVertMove );
	virtual void		Scale( double fScaleX, double fScaleY );
#endif
	const Polygon&		GetPolygon() const { return maPoly; }
	const LineInfo& 	GetLineInfo() const { return maLineInfo; }
};

// ---------------------
// - MetaPolygonAction -
// ---------------------

class  MetaPolygonAction : public MetaAction
{
private:

	Polygon 			maPoly;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( Polygon, META_POLYGON_ACTION )

						MetaPolygonAction( const Polygon& rPoly );
#ifdef AVS
	virtual void		Move( long nHorzMove, long nVertMove );
	virtual void		Scale( double fScaleX, double fScaleY );
#endif
	const Polygon&		GetPolygon() const { return maPoly; }
};

// -------------------------
// - MetaPolyPolygonAction -
// -------------------------

class  MetaPolyPolygonAction : public MetaAction
{
private:

	PolyPolygon 		maPolyPoly;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( PolyPolygon, META_POLYPOLYGON_ACTION )

						MetaPolyPolygonAction( const PolyPolygon& rPolyPoly );
#ifdef AVS
	virtual void		Move( long nHorzMove, long nVertMove );
	virtual void		Scale( double fScaleX, double fScaleY );
#endif
	const PolyPolygon&	GetPolyPolygon() const { return maPolyPoly; }
};

// ------------------
// - MetaTextAction -
// ------------------

class  MetaTextAction : public MetaAction
{
private:

	Point				maPt;
	XubString				maStr;
	USHORT				mnIndex;
	USHORT				mnLen;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( Text, META_TEXT_ACTION )

						MetaTextAction( const Point& rPt, const XubString& rStr,
										USHORT nIndex, USHORT nLen );
#ifdef AVS
	virtual void		Move( long nHorzMove, long nVertMove );
	virtual void		Scale( double fScaleX, double fScaleY );
#endif
	const Point&		GetPoint() const { return maPt; }
	const XubString&	GetText() const { return maStr; }
	USHORT				GetIndex() const { return mnIndex; }
	USHORT				GetLen() const { return mnLen; }
};

// -----------------------
// - MetaTextArrayAction -
// -----------------------

class  MetaTextArrayAction : public MetaAction
{
private:

	Point				maStartPt;
	XubString			maStr;
	sal_Int32*			mpDXAry;
	USHORT				mnIndex;
	USHORT				mnLen;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
protected:
	virtual 			~MetaTextArrayAction();

public:
						MetaTextArrayAction();
						MetaTextArrayAction( const MetaTextArrayAction& rAction );
						MetaTextArrayAction( const Point& rStartPt,
											 const XubString& rStr,
											 const sal_Int32* pDXAry,
											 USHORT nIndex,
											 USHORT nLen );

	virtual void		Execute( OutputDevice* pOut );
#ifdef AVS
	virtual MetaAction* Clone();

	virtual void		Move( long nHorzMove, long nVertMove );
	virtual void		Scale( double fScaleX, double fScaleY );

	virtual void		Write( SvStream& rOStm, ImplMetaWriteData* pData );
#endif
	virtual void		Read( SvStream& rIStm, ImplMetaReadData* pData );

	const Point&		GetPoint() const { return maStartPt; }
	const XubString&	GetText() const { return maStr; }
	USHORT				GetIndex() const { return mnIndex; }
	USHORT				GetLen() const { return mnLen; }
	sal_Int32*			GetDXArray() const { return mpDXAry; }
};

// -------------------------
// - MetaStretchTextAction -
// -------------------------

class  MetaStretchTextAction : public MetaAction
{
private:

	Point				maPt;
	XubString			maStr;
	sal_uInt32			mnWidth;
	USHORT				mnIndex;
	USHORT				mnLen;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( StretchText, META_STRETCHTEXT_ACTION )

						MetaStretchTextAction( const Point& rPt, sal_uInt32 nWidth,
											   const XubString& rStr,
											   USHORT nIndex, USHORT nLen );
#ifdef AVS
	virtual void		Move( long nHorzMove, long nVertMove );
	virtual void		Scale( double fScaleX, double fScaleY );
#endif
	const Point&		GetPoint() const { return maPt; }
	const XubString&	GetText() const { return maStr; }
	sal_uInt32			GetWidth() const { return mnWidth; }
	USHORT				GetIndex() const { return mnIndex; }
	USHORT				GetLen() const { return mnLen; }
};

// ----------------------
// - MetaTextRectAction -
// ----------------------

class  MetaTextRectAction : public MetaAction
{
private:

	Rectangle			maRect;
	XubString			maStr;
	USHORT				mnStyle;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( TextRect, META_TEXTRECT_ACTION )

						MetaTextRectAction( const Rectangle& rRect,
											const XubString& rStr, USHORT nStyle );
#ifdef AVS
	virtual void		Move( long nHorzMove, long nVertMove );
	virtual void		Scale( double fScaleX, double fScaleY );
#endif
	const Rectangle&	GetRect() const { return maRect; }
	const XubString&	GetText() const { return maStr; }
	USHORT				GetStyle() const { return mnStyle; }
};

// ----------------------
// - MetaTextLineAction -
// ----------------------

class  MetaTextLineAction : public MetaAction
{
private:

	Point				maPos;
	long				mnWidth;
	FontStrikeout		meStrikeout;
	FontUnderline		meUnderline;
	FontUnderline		meOverline;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( TextLine, META_TEXTLINE_ACTION )

						MetaTextLineAction( const Point& rPos, long nWidth,
											FontStrikeout eStrikeout,
											FontUnderline eUnderline,
											FontUnderline eOverline );
#ifdef AVS
	virtual void		Move( long nHorzMove, long nVertMove );
	virtual void		Scale( double fScaleX, double fScaleY );
#endif
	const Point&		GetStartPoint() const { return maPos; }
	long				GetWidth() const { return mnWidth; }
	FontStrikeout		GetStrikeout() const { return meStrikeout; }
	FontUnderline		GetUnderline() const { return meUnderline; }
	FontUnderline		GetOverline()  const { return meOverline; }
};

// -----------------
// - MetaBmpAction -
// -----------------

class  MetaBmpAction : public MetaAction
{
private:

	Bitmap				maBmp;
	Point				maPt;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( Bmp, META_BMP_ACTION )

						MetaBmpAction( const Point& rPt, const Bitmap& rBmp );
#ifdef AVS
	virtual void		Move( long nHorzMove, long nVertMove );
	virtual void		Scale( double fScaleX, double fScaleY );
#endif
	const Bitmap&		GetBitmap() const { return maBmp; }
	const Point&		GetPoint() const { return maPt; }
};

// ----------------------
// - MetaBmpScaleAction -
// ----------------------

class  MetaBmpScaleAction : public MetaAction
{
private:

	Bitmap				maBmp;
	Point				maPt;
	Size				maSz;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( BmpScale, META_BMPSCALE_ACTION )

						MetaBmpScaleAction( const Point& rPt, const Size& rSz,
											const Bitmap& rBmp );
#ifdef AVS
	virtual void		Move( long nHorzMove, long nVertMove );
	virtual void		Scale( double fScaleX, double fScaleY );
#endif
	const Bitmap&		GetBitmap() const { return maBmp; }
	const Point&		GetPoint() const { return maPt; }
	const Size& 		GetSize() const { return maSz; }
};

// --------------------------
// - MetaBmpScalePartAction -
// --------------------------

class  MetaBmpScalePartAction : public MetaAction
{
private:

	Bitmap				maBmp;
	Point				maDstPt;
	Size				maDstSz;
	Point				maSrcPt;
	Size				maSrcSz;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( BmpScalePart, META_BMPSCALEPART_ACTION )

						MetaBmpScalePartAction( const Point& rDstPt, const Size& rDstSz,
												const Point& rSrcPt, const Size& rSrcSz,
												const Bitmap& rBmp );
#ifdef AVS
	virtual void		Move( long nHorzMove, long nVertMove );
	virtual void		Scale( double fScaleX, double fScaleY );
#endif
	const Bitmap&		GetBitmap() const { return maBmp; }
	const Point&		GetDestPoint() const { return maDstPt; }
	const Size& 		GetDestSize() const { return maDstSz; }
	const Point&		GetSrcPoint() const { return maSrcPt; }
	const Size& 		GetSrcSize() const { return maSrcSz; }
};

// -----------------
// - MetaBmpExAction -
// -----------------

class  MetaBmpExAction : public MetaAction
{
private:

	BitmapEx			maBmpEx;
	Point				maPt;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( BmpEx, META_BMPEX_ACTION )

						MetaBmpExAction( const Point& rPt, const BitmapEx& rBmpEx );
#ifdef AVS
	virtual void		Move( long nHorzMove, long nVertMove );
	virtual void		Scale( double fScaleX, double fScaleY );
#endif
	const BitmapEx& 	GetBitmapEx() const { return maBmpEx; }
	const Point&		GetPoint() const { return maPt; }
};

// ----------------------
// - MetaBmpExScaleAction -
// ----------------------

class  MetaBmpExScaleAction : public MetaAction
{
private:

	BitmapEx			maBmpEx;
	Point				maPt;
	Size				maSz;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( BmpExScale, META_BMPEXSCALE_ACTION )

						MetaBmpExScaleAction( const Point& rPt, const Size& rSz,
											  const BitmapEx& rBmpEx ) ;
#ifdef AVS
	virtual void		Move( long nHorzMove, long nVertMove );
	virtual void		Scale( double fScaleX, double fScaleY );
#endif
	const BitmapEx& 	GetBitmapEx() const { return maBmpEx; }
	const Point&		GetPoint() const { return maPt; }
	const Size& 		GetSize() const { return maSz; }
};

// ----------------------------
// - MetaBmpExScalePartAction -
// ----------------------------

class  MetaBmpExScalePartAction : public MetaAction
{
private:

	BitmapEx			maBmpEx;
	Point				maDstPt;
	Size				maDstSz;
	Point				maSrcPt;
	Size				maSrcSz;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( BmpExScalePart, META_BMPEXSCALEPART_ACTION )

						MetaBmpExScalePartAction( const Point& rDstPt, const Size& rDstSz,
												  const Point& rSrcPt, const Size& rSrcSz,
												  const BitmapEx& rBmpEx );
#ifdef AVS
	virtual void		Move( long nHorzMove, long nVertMove );
	virtual void		Scale( double fScaleX, double fScaleY );
#endif
	const BitmapEx& 	GetBitmapEx() const { return maBmpEx; }
	const Point&		GetDestPoint() const { return maDstPt; }
	const Size& 		GetDestSize() const { return maDstSz; }
	const Point&		GetSrcPoint() const { return maSrcPt; }
	const Size& 		GetSrcSize() const { return maSrcSz; }
};

// ------------------
// - MetaMaskAction -
// ------------------

class  MetaMaskAction : public MetaAction
{
private:

	Bitmap				maBmp;
	Color				maColor;
	Point				maPt;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( Mask, META_MASK_ACTION )

						MetaMaskAction( const Point& rPt,
										const Bitmap& rBmp,
										const Color& rColor );
#ifdef AVS
	virtual void		Move( long nHorzMove, long nVertMove );
	virtual void		Scale( double fScaleX, double fScaleY );
#endif
	const Bitmap&		GetBitmap() const { return maBmp; }
	const Color&		GetColor() const { return maColor; }
	const Point&		GetPoint() const { return maPt; }
};

// -----------------------
// - MetaMaskScaleAction -
// -----------------------

class  MetaMaskScaleAction : public MetaAction
{
private:

	Bitmap				maBmp;
	Color				maColor;
	Point				maPt;
	Size				maSz;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( MaskScale, META_MASKSCALE_ACTION )

						MetaMaskScaleAction( const Point& rPt, const Size& rSz,
											 const Bitmap& rBmp,
											 const Color& rColor );
#ifdef AVS
	virtual void		Move( long nHorzMove, long nVertMove );
	virtual void		Scale( double fScaleX, double fScaleY );
#endif
	const Bitmap&		GetBitmap() const { return maBmp; }
	const Color&		GetColor() const { return maColor; }
	const Point&		GetPoint() const { return maPt; }
	const Size& 		GetSize() const { return maSz; }
};

// ---------------------------
// - MetaMaskScalePartAction -
// ---------------------------

class  MetaMaskScalePartAction : public MetaAction
{
private:

	Bitmap				maBmp;
	Color				maColor;
	Point				maDstPt;
	Size				maDstSz;
	Point				maSrcPt;
	Size				maSrcSz;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( MaskScalePart, META_MASKSCALEPART_ACTION )

						MetaMaskScalePartAction( const Point& rDstPt, const Size& rDstSz,
												 const Point& rSrcPt, const Size& rSrcSz,
												 const Bitmap& rBmp,
												 const Color& rColor );
#ifdef AVS
	virtual void		Move( long nHorzMove, long nVertMove );
	virtual void		Scale( double fScaleX, double fScaleY );
#endif
	const Bitmap&		GetBitmap() const { return maBmp; }
	const Color&		GetColor() const { return maColor; }
	const Point&		GetDestPoint() const { return maDstPt; }
	const Size& 		GetDestSize() const { return maDstSz; }
	const Point&		GetSrcPoint() const { return maSrcPt; }
	const Size& 		GetSrcSize() const { return maSrcSz; }
};

// ----------------------
// - MetaGradientAction -
// ----------------------

class  MetaGradientAction : public MetaAction
{
private:

	Rectangle			maRect;
	Gradient			maGradient;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( Gradient, META_GRADIENT_ACTION )

						MetaGradientAction( const Rectangle& rRect, const Gradient& rGradient );
#ifdef AVS
	virtual void		Move( long nHorzMove, long nVertMove );
	virtual void		Scale( double fScaleX, double fScaleY );
#endif
	const Rectangle&	GetRect() const { return maRect; }
	const Gradient& 	GetGradient() const { return maGradient; }
};

// ------------------------
// - MetaGradientExAction -
// ------------------------

class  MetaGradientExAction : public MetaAction
{
private:

	PolyPolygon 		maPolyPoly;
	Gradient			maGradient;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( GradientEx, META_GRADIENTEX_ACTION )

						MetaGradientExAction( const PolyPolygon& rPolyPoly, const Gradient& rGradient );
#ifdef AVS
	virtual void		Move( long nHorzMove, long nVertMove );
	virtual void		Scale( double fScaleX, double fScaleY );
#endif
	const PolyPolygon&	GetPolyPolygon() const { return maPolyPoly; }
	const Gradient& 	GetGradient() const { return maGradient; }
};

// -------------------
// - MetaHatchAction -
// -------------------

class  MetaHatchAction : public MetaAction
{
private:

	PolyPolygon 		maPolyPoly;
	Hatch				maHatch;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( Hatch, META_HATCH_ACTION )

						MetaHatchAction( const PolyPolygon& rPolyPoly, const Hatch& rHatch );
#ifdef AVS
	virtual void		Move( long nHorzMove, long nVertMove );
	virtual void		Scale( double fScaleX, double fScaleY );
#endif
	const PolyPolygon&	GetPolyPolygon() const { return maPolyPoly; }
	const Hatch&		GetHatch() const { return maHatch; }
};

// -----------------------
// - MetaWallpaperAction -
// -----------------------

class  MetaWallpaperAction : public MetaAction
{
private:

	Rectangle			maRect;
	Wallpaper			maWallpaper;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( Wallpaper, META_WALLPAPER_ACTION )

						MetaWallpaperAction( const Rectangle& rRect,
											 const Wallpaper& rPaper );
#ifdef AVS
	virtual void		Move( long nHorzMove, long nVertMove );
	virtual void		Scale( double fScaleX, double fScaleY );
#endif
	const Rectangle&	GetRect() const { return maRect; }
	const Wallpaper&	GetWallpaper() const { return maWallpaper; }
};

// ------------------------
// - MetaClipRegionAction -
// ------------------------

class  MetaClipRegionAction : public MetaAction
{
private:

	Region				maRegion;
	BOOL				mbClip;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( ClipRegion, META_CLIPREGION_ACTION )

						MetaClipRegionAction( const Region& rRegion, BOOL bClip );
#ifdef AVS
	virtual void		Move( long nHorzMove, long nVertMove );
	virtual void		Scale( double fScaleX, double fScaleY );
#endif
	const Region&		GetRegion() const { return maRegion; }
	BOOL				IsClipping() const { return mbClip; }
};

// ---------------------------------
// - MetaISectRectClipRegionAction -
// ---------------------------------

class  MetaISectRectClipRegionAction : public MetaAction
{
private:

	Rectangle			maRect;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( ISectRectClipRegion, META_ISECTRECTCLIPREGION_ACTION )

						MetaISectRectClipRegionAction( const Rectangle& rRect );
#ifdef AVS
	virtual void		Move( long nHorzMove, long nVertMove );
	virtual void		Scale( double fScaleX, double fScaleY );
#endif
	const Rectangle&	GetRect() const { return maRect; }
};

// -----------------------------------
// - MetaISectRegionClipRegionAction -
// -----------------------------------

class  MetaISectRegionClipRegionAction : public MetaAction
{
private:

	Region				maRegion;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( ISectRegionClipRegion, META_ISECTREGIONCLIPREGION_ACTION )

						MetaISectRegionClipRegionAction( const Region& rRegion );
#ifdef AVS
	virtual void		Move( long nHorzMove, long nVertMove );
	virtual void		Scale( double fScaleX, double fScaleY );
#endif
	const Region&		GetRegion() const { return maRegion; }
};

// ----------------------------
// - MetaMoveClipRegionAction -
// ----------------------------

class  MetaMoveClipRegionAction : public MetaAction
{
private:

	long				mnHorzMove;
	long				mnVertMove;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( MoveClipRegion, META_MOVECLIPREGION_ACTION )

						MetaMoveClipRegionAction( long nHorzMove, long nVertMove );
#ifdef AVS
	virtual void		Scale( double fScaleX, double fScaleY );
#endif
	long				GetHorzMove() const { return mnHorzMove; }
	long				GetVertMove() const { return mnVertMove; }
};

// -----------------------
// - MetaLineColorAction -
// -----------------------

class  MetaLineColorAction : public MetaAction
{
private:

	Color				maColor;
	BOOL				mbSet;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( LineColor, META_LINECOLOR_ACTION )

						MetaLineColorAction( const Color& rColor, BOOL bSet );

	const Color&		GetColor() const { return maColor; }
	BOOL				IsSetting() const { return mbSet; }
};

// -----------------------
// - MetaFillColorAction -
// -----------------------

class  MetaFillColorAction : public MetaAction
{
private:

	Color				maColor;
	BOOL				mbSet;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( FillColor, META_FILLCOLOR_ACTION )

						MetaFillColorAction( const Color& rColor, BOOL bSet );

	const Color&		GetColor() const { return maColor; }
	BOOL				IsSetting() const { return mbSet; }
};

// -----------------------
// - MetaTextColorAction -
// -----------------------

class  MetaTextColorAction : public MetaAction
{
private:

	Color				maColor;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( TextColor, META_TEXTCOLOR_ACTION )

						MetaTextColorAction( const Color& rColor );

	const Color&		GetColor() const { return maColor; }
};

// ---------------------------
// - MetaTextFillColorAction -
// ---------------------------

class  MetaTextFillColorAction : public MetaAction
{
private:

	Color				maColor;
	BOOL				mbSet;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( TextFillColor, META_TEXTFILLCOLOR_ACTION )

						MetaTextFillColorAction( const Color& rColor, BOOL bSet );

	const Color&		GetColor() const { return maColor; }
	BOOL				IsSetting() const { return mbSet; }
};

// ---------------------------
// - MetaTextLineColorAction -
// ---------------------------

class  MetaTextLineColorAction : public MetaAction
{
private:

	Color				maColor;
	BOOL				mbSet;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( TextLineColor, META_TEXTLINECOLOR_ACTION )

						MetaTextLineColorAction( const Color& rColor, BOOL bSet );

	const Color&		GetColor() const { return maColor; }
	BOOL				IsSetting() const { return mbSet; }
};

// ---------------------------
// - MetaOverlineColorAction -
// ---------------------------

class  MetaOverlineColorAction : public MetaAction
{
private:

	Color				maColor;
	BOOL				mbSet;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( OverlineColor, META_OVERLINECOLOR_ACTION )

						MetaOverlineColorAction( const Color& rColor, BOOL bSet );

	const Color&		GetColor() const { return maColor; }
	BOOL				IsSetting() const { return mbSet; }
};

// -----------------------
// - MetaTextAlignAction -
// -----------------------

class  MetaTextAlignAction : public MetaAction
{
private:

	TextAlign			maAlign;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( TextAlign, META_TEXTALIGN_ACTION )

						MetaTextAlignAction( TextAlign aAlign );

	TextAlign			GetTextAlign() const { return maAlign; }
};

// ---------------------
// - MetaMapModeAction -
// ---------------------

class  MetaMapModeAction : public MetaAction
{
private:

	MapMode 			maMapMode;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( MapMode, META_MAPMODE_ACTION )

						MetaMapModeAction( const MapMode& rMapMode );
#ifdef AVS
	virtual void		Scale( double fScaleX, double fScaleY );
#endif
	const MapMode&		GetMapMode() const { return maMapMode; }
};

// ---------------------
// - MetaFontAction -
// ---------------------

class  MetaFontAction : public MetaAction
{
private:

	Font				maFont;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( Font, META_FONT_ACTION )

						MetaFontAction( const Font& rFont );
#ifdef AVS
	virtual void		Scale( double fScaleX, double fScaleY );
#endif
	const Font& 		GetFont() const { return maFont; }
};

// ------------------
// - MetaPushAction -
// ------------------

class  MetaPushAction : public MetaAction
{
private:

	USHORT				mnFlags;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( Push, META_PUSH_ACTION )

						MetaPushAction( USHORT nFlags );

	USHORT				GetFlags() const { return mnFlags; }
};

// -----------------
// - MetaPopAction -
// -----------------

class  MetaPopAction : public MetaAction
{
public:

		DECL_META_ACTION( Pop, META_POP_ACTION )
};

// ----------------------
// - MetaRasterOpAction -
// ----------------------

class  MetaRasterOpAction : public MetaAction
{
private:

	RasterOp			meRasterOp;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( RasterOp, META_RASTEROP_ACTION )

						MetaRasterOpAction( RasterOp eRasterOp );

	RasterOp			GetRasterOp() const { return meRasterOp; }
};

// -------------------------
// - MetaTransparentAction -
// -------------------------

class  MetaTransparentAction : public MetaAction
{
private:

	PolyPolygon 		maPolyPoly;
	USHORT				mnTransPercent;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( Transparent, META_TRANSPARENT_ACTION )

						MetaTransparentAction( const PolyPolygon& rPolyPoly, USHORT nTransPercent );
#ifdef AVS
	virtual void		Move( long nHorzMove, long nVertMove );
	virtual void		Scale( double fScaleX, double fScaleY );
#endif
	const PolyPolygon&	GetPolyPolygon() const { return maPolyPoly; }
	USHORT				GetTransparence() const { return mnTransPercent; }
};

// ------------------------------
// - MetaFloatTransparentAction -
// ------------------------------

class  MetaFloatTransparentAction : public MetaAction
{
private:

	GDIMetaFile 		maMtf;
	Point				maPoint;
	Size				maSize;
	Gradient			maGradient;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( FloatTransparent, META_FLOATTRANSPARENT_ACTION )

						MetaFloatTransparentAction( const GDIMetaFile& rMtf, const Point& rPos,
													const Size& rSize, const Gradient& rGradient );
#ifdef AVS
	virtual void		Move( long nHorzMove, long nVertMove );
	virtual void		Scale( double fScaleX, double fScaleY );
#endif
	const GDIMetaFile&	GetGDIMetaFile() const { return maMtf; }
	const Point&		GetPoint() const { return maPoint; }
	const Size& 		GetSize() const { return maSize; }
	const Gradient& 	GetGradient() const { return maGradient; }
};

// ---------------------
// - MetaDrawEPSAction -
// ---------------------

class  MetaEPSAction : public MetaAction
{
private:

	GfxLink 			maGfxLink;
	GDIMetaFile 		maSubst;
	Point				maPoint;
	Size				maSize;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( EPS, META_EPS_ACTION )

						MetaEPSAction( const Point& rPoint, const Size& rSize,
									   const GfxLink& rGfxLink, const GDIMetaFile& rSubst );
#ifdef AVS
	virtual void		Move( long nHorzMove, long nVertMove );
	virtual void		Scale( double fScaleX, double fScaleY );
#endif
	const GfxLink&		GetLink() const { return maGfxLink; }
	const GDIMetaFile&	GetSubstitute() const { return maSubst; }
	const Point&		GetPoint() const { return maPoint; }
	const Size& 		GetSize() const { return maSize; }
};

// ----------------------
// - MetaRefPointAction -
// ----------------------

class  MetaRefPointAction : public MetaAction
{
private:

	Point				maRefPoint;
	BOOL				mbSet;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( RefPoint, META_REFPOINT_ACTION )

						MetaRefPointAction( const Point& rRefPoint, BOOL bSet );

	const Point&		GetRefPoint() const { return maRefPoint; }
	BOOL				IsSetting() const { return mbSet; }
};

// ---------------------
// - MetaCommentAction -
// ---------------------

class  MetaCommentAction : public MetaAction
{
private:

	ByteString			maComment;
	sal_Int32			mnValue;
	sal_uInt32			mnDataSize;
	BYTE*				mpData;
#ifdef AVS
	 void	ImplInitDynamicData( const BYTE* pData, sal_uInt32 nDataSize );
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
protected:
						~MetaCommentAction();

public:
						MetaCommentAction( sal_Int32 nValue = 0L );
						MetaCommentAction( const MetaCommentAction& rAct );
						MetaCommentAction( const ByteString& rComment, sal_Int32 nValue = 0L, const BYTE* pData = NULL, sal_uInt32 nDataSize = 0UL );
						MetaCommentAction( const BYTE* pData, sal_uInt32 nDataSize );
#ifdef AVS
	virtual void		Move( long nHorzMove, long nVertMove );
	virtual void		Scale( double fScaleX, double fScaleY );
#endif
	virtual void		Execute( OutputDevice* pOut );
#ifdef AVS
	virtual MetaAction* Clone();
	virtual void		Write( SvStream& rOStm, ImplMetaWriteData* pData );
#endif
	virtual void		Read( SvStream& rIStm, ImplMetaReadData* pData );

	const ByteString&	GetComment() const { return maComment; }
	sal_Int32			GetValue() const { return mnValue; }
	sal_uInt32			GetDataSize() const { return mnDataSize; }
	const BYTE* 		GetData() const { return mpData; }
};

// ------------------------
// - MetaLayoutModeAction -
// ------------------------

class  MetaLayoutModeAction : public MetaAction
{
private:

    sal_uInt32 			mnLayoutMode;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( LayoutMode, META_LAYOUTMODE_ACTION )

						MetaLayoutModeAction( sal_uInt32 nLayoutMode );

    sal_uInt32			GetLayoutMode() const { return mnLayoutMode; }
};

// ------------------------
// - MetaTextLanguageAction -
// ------------------------

class  MetaTextLanguageAction : public MetaAction
{
private:

    LanguageType		meTextLanguage;
#ifdef AVS
	virtual	sal_Bool	Compare( const MetaAction& ) const;
#endif
public:
						DECL_META_ACTION( TextLanguage, META_TEXTLANGUAGE_ACTION )

						MetaTextLanguageAction( LanguageType );

    LanguageType		GetTextLanguage() const { return meTextLanguage; }
};
}//SVMCore

#endif // _SV_METAACT_HXX
