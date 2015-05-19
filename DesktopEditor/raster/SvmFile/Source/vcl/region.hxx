/*************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 * 
 * Copyright 2008 by Sun Microsystems, Inc.
 *
 * OpenOffice.org - a multi-platform office productivity suite
 *
 * $RCSfile: region.hxx,v $
 * $Revision: 1.4 $
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

#ifndef _SV_REGION_HXX
#define _SV_REGION_HXX
#ifdef AVS
#include <tools/gen.hxx>
#include <vcl/sv.h>
#include <vcl/dllapi.h>

#include <basegfx/polygon/b2dpolypolygon.hxx>
#endif
#include "../tools/gen.hxx"

namespace SVMCore{

class ImplRegion;
class ImplRegionBand;
class Polygon;
class PolyPolygon;
struct ImplRegionInfo;

// --------------
// - RegionType -
// --------------

enum RegionType { REGION_NULL, REGION_EMPTY, REGION_RECTANGLE, REGION_COMPLEX };
enum RegionOverlapType { REGION_INSIDE, REGION_OVER, REGION_OUTSIDE };

typedef long RegionHandle;

// ----------
// - Region -
// ----------

class  Region
{
	friend class OutputDevice;
	friend class Window;
	friend class Bitmap;

private:
	ImplRegion* 		mpImplRegion;

	 void				ImplCopyData();

	 void				ImplCreateRectRegion( const Rectangle& rRect );
	 void				ImplCreatePolyPolyRegion( const PolyPolygon& rPolyPoly );

	 void				ImplPolyPolyRegionToBandRegionFunc();
	 inline void 		ImplPolyPolyRegionToBandRegion();

	 const ImplRegion*	ImplGetImplRegion() const { return mpImplRegion; }
	 ImplRegion* 		ImplGetImplRegion() { return mpImplRegion; }
	 BOOL				ImplGetFirstRect( ImplRegionInfo& rImplRegionInfo,
										  long& nX, long& nY, long& nWidth, long& nHeight ) const;
	 BOOL				ImplGetNextRect( ImplRegionInfo& rImplRegionInfo,
										 long& nX, long& nY, long& nWidth, long& nHeight ) const;
	 void				ImplBeginAddRect( );
	 BOOL				ImplAddRect( const Rectangle& rRect );
	 void				ImplEndAddRect( );
#ifdef AVS
#ifdef DBG_UTIL
	friend const char*	ImplDbgTestRegion( const void* pObj );
#endif
#endif
public:
					Region();
					Region( RegionType eType );
					Region( const Rectangle& rRect );
					Region( const Polygon& rPolygon );
					Region( const PolyPolygon& rPolyPoly );
#ifdef AVS
					Region( const basegfx::B2DPolyPolygon& );
#endif
					Region( const Region& rRegion );
					~Region();
	void			Move( long nHorzMove, long nVertMove );
#ifdef AVS
	void			Scale( double fScaleX, double fScaleY );
	BOOL			Union( const Rectangle& rRegion );
#endif
	BOOL			Intersect( const Rectangle& rRegion );
#ifdef AVS
	BOOL			Exclude( const Rectangle& rRegion );
	BOOL			XOr( const Rectangle& rRegion );
	BOOL			Union( const Region& rRegion );
#endif
	BOOL			Intersect( const Region& rRegion );
#ifdef AVS
	BOOL			Exclude( const Region& rRegion );
	BOOL			XOr( const Region& rRegion );
#endif
	RegionType		GetType() const;

	BOOL			IsEmpty() const { return GetType() == REGION_EMPTY; };

	BOOL			IsNull() const { return GetType() == REGION_NULL; };
#ifdef AVS
	void			SetEmpty();
	void			SetNull();
#endif
	Rectangle		GetBoundRect() const;
#ifdef AVS
    BOOL            HasPolyPolygon() const;
#endif
    PolyPolygon     GetPolyPolygon() const;
#ifdef AVS

    const basegfx::B2DPolyPolygon GetB2DPolyPolygon() const;

#endif
	ULONG			GetRectCount() const;
#ifdef AVS
	RegionHandle	BeginEnumRects();
	BOOL			GetEnumRects( RegionHandle hRegionHandle, Rectangle& rRect );
	BOOL			GetNextEnumRect( RegionHandle hRegionHandle, Rectangle& rRect )
						{ return GetEnumRects( hRegionHandle, rRect ); }
	void			EndEnumRects( RegionHandle hRegionHandle );

	BOOL			IsInside( const Point& rPoint ) const;
	BOOL			IsInside( const Rectangle& rRect ) const;
#endif
	BOOL			IsOver( const Rectangle& rRect ) const;

	Region& 		operator=( const Region& rRegion );
	Region& 		operator=( const Rectangle& rRect );
#ifdef AVS
	BOOL			operator==( const Region& rRegion ) const;
	BOOL			operator!=( const Region& rRegion ) const
						{ return !(Region::operator==( rRegion )); }
#endif
	friend  SvStream& operator>>( SvStream& rIStm, Region& rRegion );
	friend  SvStream& operator<<( SvStream& rOStm, const Region& rRegion );
#ifdef AVS
    /* workaround: faster conversion for PolyPolygons
     * if half of the Polygons contained in rPolyPoly are actually
     * rectangles, then the returned Region will be constructed by
     * XOr'ing the contained Polygons together; in the case of
     * only Rectangles this can be up to eight times faster than
     * Region( const PolyPolygon& ).
     * Caution: this is only useful if the Region is known to be
     * changed to rectangles; e.g. if being set as clip region
     */
    static Region GetRegionFromPolyPolygon( const PolyPolygon& rPolyPoly );
#endif
};

}//SVMCore
#endif	// _SV_REGION_HXX
