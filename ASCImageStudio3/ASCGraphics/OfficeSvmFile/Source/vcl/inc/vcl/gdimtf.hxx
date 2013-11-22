/*************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 * 
 * Copyright 2008 by Sun Microsystems, Inc.
 *
 * OpenOffice.org - a multi-platform office productivity suite
 *
 * $RCSfile: gdimtf.hxx,v $
 * $Revision: 1.3 $
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

#ifndef _SV_GDIMTF_HXX
#define _SV_GDIMTF_HXX
#ifdef AVS
#include <vcl/dllapi.h>
#include <tools/gen.hxx>
#include <tools/list.hxx>
#include <tools/link.hxx>
#include <tools/string.hxx>
#include <vcl/mapmod.hxx>
#endif
#include "AvsBasic.h"
#include "tools/gen.hxx"
#include "tools/list.hxx"
#include "vcl/mapmod.hxx"

namespace SVMCore{

class OutputDevice;
class ImpLabelList;
class MetaAction;
class SvStream;
class Color;
class BitmapEx;
class Bitmap;
class Polygon;
class PolyPolygon;
class Gradient;
class SalGraphics;

// ---------------------
// - GDIMetaFile-Types -
// ---------------------

#define GDI_METAFILE_END                ((ULONG)0xFFFFFFFF)
#define GDI_METAFILE_LABEL_NOTFOUND     ((ULONG)0xFFFFFFFF)

#ifndef METAFILE_END
#define METAFILE_END					GDI_METAFILE_END
#endif

#ifndef METAFILE_LABEL_NOTFOUND
#define METAFILE_LABEL_NOTFOUND			GDI_METAFILE_LABEL_NOTFOUND
#endif

// -----------
// - Defines -
// -----------

#define MTF_MIRROR_NONE				0x00000000UL
#define MTF_MIRROR_HORZ				0x00000001UL
#define MTF_MIRROR_VERT				0x00000002UL

// ---------
// - Enums -
// ---------

enum MtfConversion
{
    MTF_CONVERSION_NONE = 0,
    MTF_CONVERSION_1BIT_THRESHOLD = 1,
    MTF_CONVERSION_8BIT_GREYS = 2
};

// -----------------------------
// - Color conversion routines -
// -----------------------------

//#if 0 // _SOLAR__PRIVATE

typedef Color (*ColorExchangeFnc)( const Color& rColor, const void* pColParam );
typedef BitmapEx (*BmpExchangeFnc)( const BitmapEx& rBmpEx, const void* pBmpParam );

//#endif // __PRIVATE

// ---------------
// - GDIMetaFile -
// ---------------

class  GDIMetaFile : protected List
{
private:
	SalGraphics*	m_poGraphics;
    MapMode         aPrefMapMode;
    Size            aPrefSize;
#ifdef AVS
    Link            aHookHdlLink;
    GDIMetaFile*    pPrev;
    GDIMetaFile*    pNext;
#endif
    OutputDevice*   pOutDev;
#ifdef AVS
    ImpLabelList*   pLabelList;
#endif
    BOOL            bPause;
    BOOL            bRecord;

//#if 0 // _SOLAR__PRIVATE
#ifdef AVS
	 static Color    ImplColAdjustFnc( const Color& rColor, const void* pColParam );
	 static BitmapEx ImplBmpAdjustFnc( const BitmapEx& rBmpEx, const void* pBmpParam );
#endif
	 static Color	   ImplColConvertFnc( const Color& rColor, const void* pColParam );
	 static BitmapEx ImplBmpConvertFnc( const BitmapEx& rBmpEx, const void* pBmpParam );

	 static Color	   ImplColMonoFnc( const Color& rColor, const void* pColParam );
	 static BitmapEx ImplBmpMonoFnc( const BitmapEx& rBmpEx, const void* pBmpParam );
#ifdef AVS
	 static Color	   ImplColReplaceFnc( const Color& rColor, const void* pColParam );
	 static BitmapEx ImplBmpReplaceFnc( const BitmapEx& rBmpEx, const void* pBmpParam );
#endif
	 void			   ImplExchangeColors( ColorExchangeFnc pFncCol, const void* pColParam, 
                                                       BmpExchangeFnc pFncBmp, const void* pBmpParam );
#ifdef AVS
     Point           ImplGetRotatedPoint( const Point& rPt, const Point& rRotatePt, 
                                                        const Size& rOffset, double fSin, double fCos );
     Polygon         ImplGetRotatedPolygon( const Polygon& rPoly, const Point& rRotatePt, 
                                                          const Size& rOffset, double fSin, double fCos );
     PolyPolygon     ImplGetRotatedPolyPolygon( const PolyPolygon& rPoly, const Point& rRotatePt, 
                                                              const Size& rOffset, double fSin, double fCos );
     void            ImplAddGradientEx( GDIMetaFile&          rMtf, 
                                                      const OutputDevice&   rMapDev,
                                                     const PolyPolygon&    rPolyPoly,
                                                      const Gradient&	  	rGrad 		);
#endif 
//#endif // __PRIVATE

protected:
#ifdef AVS
    virtual void    Linker( OutputDevice* pOut, BOOL bLink );
    virtual long    Hook();
#endif
public:
                    GDIMetaFile();
					GDIMetaFile( const GDIMetaFile& rMtf );
    virtual			~GDIMetaFile();
#ifdef AVS
    using List::operator==;
    using List::operator!=;
#endif
    GDIMetaFile&    operator=( const GDIMetaFile& rMtf );
#ifdef AVS
	BOOL			operator==( const GDIMetaFile& rMtf ) const;
	BOOL			operator!=( const GDIMetaFile& rMtf ) const { return !( *this == rMtf ); }
#endif
    void            Clear();
#ifdef AVS
	sal_Bool		IsEqual( const GDIMetaFile& rMtf ) const;
    BOOL            Mirror( ULONG nMirrorFlags );
    void            Move( long nX, long nY );
    void            Scale( double fScaleX, double fScaleY );
    void            Scale( const Fraction& rScaleX, const Fraction& rScaleY );
    void            Rotate( long nAngle10 );

	void			Adjust( short nLuminancePercent = 0, short nContrastPercent = 0,
							short nChannelRPercent = 0, short nChannelGPercent = 0, 
							short nChannelBPercent = 0, double fGamma = 1.0, BOOL bInvert = FALSE );
	void			Convert( MtfConversion eConversion );
    void			ReplaceColors( const Color& rSearchColor, const Color& rReplaceColor, ULONG nTol = 0 );
    void			ReplaceColors( const Color* pSearchColors, const Color* rReplaceColors, 
								   ULONG nColorCount, ULONG* pTols = NULL );
#endif
    GDIMetaFile     GetMonochromeMtf( const Color& rCol ) const;
#ifdef AVS
    void            Record( OutputDevice* pOutDev );
#endif
    BOOL            IsRecord() const { return bRecord; }
#ifdef AVS

    void            Play( GDIMetaFile& rMtf, ULONG nPos = GDI_METAFILE_END );
#endif
    void            Play( OutputDevice* pOutDev, ULONG nPos = GDI_METAFILE_END );
    void            Play( OutputDevice* pOutDev, const Point& rPos,
                          const Size& rSize, ULONG nPos = GDI_METAFILE_END );
#ifdef AVS
    void            Pause( BOOL bPause );
    BOOL            IsPause() const { return bPause; }

    void            Stop();
#endif
    void            WindStart();
#ifdef AVS
    void            WindEnd();
    void            Wind( ULONG nAction );
    void            WindPrev();
    void            WindNext();
#endif
    ULONG       GetActionCount() const { return Count(); }
    void            AddAction( MetaAction* pAction );
    void            AddAction( MetaAction* pAction, ULONG nPos );
#ifdef AVS
    void 			RemoveAction( ULONG nPos );
    MetaAction*     CopyAction( ULONG nPos ) const;
#endif
    MetaAction*     GetCurAction() const { return (MetaAction*) GetCurObject(); }
#ifdef AVS
    MetaAction*     GetAction( ULONG nAction ) const { return (MetaAction*) GetObject( nAction ); }
    MetaAction*     FirstAction() {	return (MetaAction*) First(); }
    MetaAction*     NextAction() { 	return (MetaAction*) Next(); }
    MetaAction*     ReplaceAction( MetaAction* pAction, ULONG nAction ) { return (MetaAction*) Replace( pAction, nAction ); }

    ULONG           GetActionPos( const String& rLabel );
    BOOL            InsertLabel( const String& rLabel, ULONG nActionPos );
    void            RemoveLabel( const String& rLabel );
    void            RenameLabel( const String& rLabel, const String& rNewLabel );
    ULONG           GetLabelCount() const;
    String          GetLabel( ULONG nLabel );

    BOOL            SaveStatus();
#endif
    const Size&     GetPrefSize() const { return aPrefSize; }
    void            SetPrefSize( const Size& rSize ) { aPrefSize = rSize; }

    const MapMode&  GetPrefMapMode() const { return aPrefMapMode; }
    void            SetPrefMapMode( const MapMode& rMapMode ) { aPrefMapMode = rMapMode; }
#ifdef AVS
    void            SetHookHdl( const Link& rLink ) { aHookHdlLink = rLink; }
    const Link&     GetHookHdl() const { return aHookHdlLink; }

	ULONG			GetChecksum() const;
    ULONG           GetSizeBytes() const;
#endif
	// Methoden zum Lesen und Schreiben des neuen Formats;
	// die Read-Methode kann auch das alte Format lesen
	SvStream&		Read( SvStream& rIStm );
#ifdef AVS
	SvStream&		Write( SvStream& rOStm );
#endif
	// Stream-Operatoren schreiben das alte Format (noch)
	// und lesen sowohl das alte wie auch das neue Format
    friend  SvStream& operator>>( SvStream& rIStm, GDIMetaFile& rGDIMetaFile );
#ifdef AVS
    friend  SvStream& operator<<( SvStream& rOStm, const GDIMetaFile& rGDIMetaFile );
#endif
	BOOL           CreateThumbnail( sal_uInt32 nWidth, sal_uInt32 nHeight,int nResizeType, BitmapEx& rBmpEx, const BitmapEx* pOverlay = NULL, const Rectangle* pOverlayRect = NULL ) const;
	void           SetGraphics( SalGraphics* poGraphics );

};
}//SVMCore
#endif // _SV_GDIMTF_HXX

