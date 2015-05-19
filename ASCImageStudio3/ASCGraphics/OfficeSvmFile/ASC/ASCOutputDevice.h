#pragma once
#include "OpenOfficeBasic.h"
#include "ASCBasic.h"
#include "tools/gen.hxx"
#include "vcl/mapmod.hxx"
#include "tools/fract.hxx"
#include "tools/color.hxx"
#include "vcl/lineinfo.hxx"
#include "tools/poly.hxx"
#include "vcl/bitmap.hxx"
#include "vcl/gradient.hxx"
#include "vcl/hatch.hxx"
#include "vcl/wall.hxx"
#include "vcl/gdimtf.hxx"
#include "vcl/gfxlink.hxx"
#include "vcl/font.hxx"
#include "tools/line.hxx"
#include "vcl/sallayout.hxx"
#include "vcl/outfont.hxx"

#include "i18npool/lang.h"



namespace SVMCore{

// ----------------
// - LayoutOption -
// ----------------

#define SAL_LAYOUT_BIDI_RTL                 0x0001
#define SAL_LAYOUT_BIDI_STRONG              0x0002
#define SAL_LAYOUT_RIGHT_ALIGN              0x0004
#define SAL_LAYOUT_KERNING_PAIRS            0x0010
#define SAL_LAYOUT_KERNING_ASIAN            0x0020
#define SAL_LAYOUT_VERTICAL                 0x0040
#define SAL_LAYOUT_COMPLEX_DISABLED         0x0100
#define SAL_LAYOUT_ENABLE_LIGATURES         0x0200
#define SAL_LAYOUT_SUBSTITUTE_DIGITS        0x0400
#define SAL_LAYOUT_KASHIDA_JUSTIFICATON     0x0800
#define SAL_LAYOUT_DISABLE_GLYPH_PROCESSING 0x1000
#define SAL_LAYOUT_FOR_FALLBACK             0x2000

// LayoutModes for Complex Text Layout
#define TEXT_LAYOUT_DEFAULT             ((ULONG)0x00000000)
#define TEXT_LAYOUT_BIDI_LTR            ((ULONG)0x00000000)
#define TEXT_LAYOUT_BIDI_RTL            ((ULONG)0x00000001)
#define TEXT_LAYOUT_BIDI_STRONG         ((ULONG)0x00000002)
#define TEXT_LAYOUT_TEXTORIGIN_LEFT     ((ULONG)0x00000004)
#define TEXT_LAYOUT_TEXTORIGIN_RIGHT    ((ULONG)0x00000008)
#define TEXT_LAYOUT_COMPLEX_DISABLED    ((ULONG)0x00000100)
#define TEXT_LAYOUT_ENABLE_LIGATURES    ((ULONG)0x00000200)
#define TEXT_LAYOUT_SUBSTITUTE_DIGITS   ((ULONG)0x00000400)

enum OutDevType { OUTDEV_DONTKNOW, OUTDEV_WINDOW, OUTDEV_PRINTER, OUTDEV_VIRDEV };

// ----------------------
// - OutputDevice-Types -
// ----------------------

// Flags for Push()
#define PUSH_LINECOLOR                  ((USHORT)0x0001)
#define PUSH_FILLCOLOR                  ((USHORT)0x0002)
#define PUSH_FONT                       ((USHORT)0x0004)
#define PUSH_TEXTCOLOR                  ((USHORT)0x0008)
#define PUSH_MAPMODE                    ((USHORT)0x0010)
#define PUSH_CLIPREGION                 ((USHORT)0x0020)
#define PUSH_RASTEROP                   ((USHORT)0x0040)
#define PUSH_TEXTFILLCOLOR              ((USHORT)0x0080)
#define PUSH_TEXTALIGN                  ((USHORT)0x0100)
#define PUSH_REFPOINT                   ((USHORT)0x0200)
#define PUSH_TEXTLINECOLOR              ((USHORT)0x0400)
#define PUSH_TEXTLAYOUTMODE             ((USHORT)0x0800)
#define PUSH_TEXTLANGUAGE               ((USHORT)0x1000)
#define PUSH_OVERLINECOLOR              ((USHORT)0x2000)
#define PUSH_ALLTEXT                    (PUSH_TEXTCOLOR | PUSH_TEXTFILLCOLOR | PUSH_TEXTLINECOLOR | PUSH_OVERLINECOLOR | PUSH_TEXTALIGN | PUSH_TEXTLAYOUTMODE | PUSH_TEXTLANGUAGE)
#define PUSH_ALLFONT                    (PUSH_ALLTEXT | PUSH_FONT)
#define PUSH_ALL                        ((USHORT)0xFFFF)

// Flags for DrawText()
#define TEXT_DRAW_DISABLE               ((USHORT)0x0001)
#define TEXT_DRAW_MNEMONIC              ((USHORT)0x0002)
#define TEXT_DRAW_MONO                  ((USHORT)0x0004)
#define TEXT_DRAW_CLIP                  ((USHORT)0x0008)
#define TEXT_DRAW_LEFT                  ((USHORT)0x0010)
#define TEXT_DRAW_CENTER                ((USHORT)0x0020)
#define TEXT_DRAW_RIGHT                 ((USHORT)0x0040)
#define TEXT_DRAW_TOP                   ((USHORT)0x0080)
#define TEXT_DRAW_VCENTER               ((USHORT)0x0100)
#define TEXT_DRAW_BOTTOM                ((USHORT)0x0200)
#define TEXT_DRAW_ENDELLIPSIS           ((USHORT)0x0400)
#define TEXT_DRAW_PATHELLIPSIS          ((USHORT)0x0800)
#define TEXT_DRAW_MULTILINE             ((USHORT)0x1000)
#define TEXT_DRAW_WORDBREAK             ((USHORT)0x2000)
#define TEXT_DRAW_NEWSELLIPSIS          ((USHORT)0x4000)

#define TEXT_DRAW_WORDBREAK_HYPHENATION (((USHORT)0x8000) | TEXT_DRAW_WORDBREAK)

// Flags for CopyArea()
#define COPYAREA_WINDOWINVALIDATE       ((USHORT)0x0001)

// Flags for DrawImage()
#define IMAGE_DRAW_DISABLE              ((USHORT)0x0001)
#define IMAGE_DRAW_HIGHLIGHT            ((USHORT)0x0002)
#define IMAGE_DRAW_DEACTIVE             ((USHORT)0x0004)
#define IMAGE_DRAW_COLORTRANSFORM      	((USHORT)0x0008)
#define IMAGE_DRAW_SEMITRANSPARENT      ((USHORT)0x0010)
#define IMAGE_DRAW_MONOCHROME_BLACK  	((USHORT)0x0020)
#define IMAGE_DRAW_MONOCHROME_WHITE  	((USHORT)0x0040)
#define IMAGE_DRAW_3DLOOK               0
#define IMAGE_DRAW_BTNLOOK              0

// WaveLine
#define WAVE_FLAT                       1
#define WAVE_SMALL                      2
#define WAVE_NORMAL                     3

// Grid
#define GRID_DOTS                       ((ULONG)0x00000001)
#define GRID_HORZLINES                  ((ULONG)0x00000002)
#define GRID_VERTLINES                  ((ULONG)0x00000004)
#define GRID_LINES                      (GRID_HORZLINES | GRID_VERTLINES)

// LayoutModes for Complex Text Layout
#define TEXT_LAYOUT_DEFAULT             ((ULONG)0x00000000)
#define TEXT_LAYOUT_BIDI_LTR            ((ULONG)0x00000000)
#define TEXT_LAYOUT_BIDI_RTL            ((ULONG)0x00000001)
#define TEXT_LAYOUT_BIDI_STRONG         ((ULONG)0x00000002)
#define TEXT_LAYOUT_TEXTORIGIN_LEFT     ((ULONG)0x00000004)
#define TEXT_LAYOUT_TEXTORIGIN_RIGHT    ((ULONG)0x00000008)
#define TEXT_LAYOUT_COMPLEX_DISABLED    ((ULONG)0x00000100)
#define TEXT_LAYOUT_ENABLE_LIGATURES    ((ULONG)0x00000200)
#define TEXT_LAYOUT_SUBSTITUTE_DIGITS   ((ULONG)0x00000400)

// DrawModes
#define DRAWMODE_DEFAULT                ((ULONG)0x00000000)
#define DRAWMODE_BLACKLINE              ((ULONG)0x00000001)
#define DRAWMODE_BLACKFILL              ((ULONG)0x00000002)
#define DRAWMODE_BLACKTEXT              ((ULONG)0x00000004)
#define DRAWMODE_BLACKBITMAP            ((ULONG)0x00000008)
#define DRAWMODE_BLACKGRADIENT          ((ULONG)0x00000010)
#define DRAWMODE_GRAYLINE               ((ULONG)0x00000020)
#define DRAWMODE_GRAYFILL               ((ULONG)0x00000040)
#define DRAWMODE_GRAYTEXT               ((ULONG)0x00000080)
#define DRAWMODE_GRAYBITMAP             ((ULONG)0x00000100)
#define DRAWMODE_GRAYGRADIENT           ((ULONG)0x00000200)
#define DRAWMODE_NOFILL                 ((ULONG)0x00000400)
#define DRAWMODE_NOBITMAP               ((ULONG)0x00000800)
#define DRAWMODE_NOGRADIENT             ((ULONG)0x00001000)
#define DRAWMODE_GHOSTEDLINE            ((ULONG)0x00002000)
#define DRAWMODE_GHOSTEDFILL            ((ULONG)0x00004000)
#define DRAWMODE_GHOSTEDTEXT            ((ULONG)0x00008000)
#define DRAWMODE_GHOSTEDBITMAP          ((ULONG)0x00010000)
#define DRAWMODE_GHOSTEDGRADIENT        ((ULONG)0x00020000)
#define DRAWMODE_WHITELINE              ((ULONG)0x00100000)
#define DRAWMODE_WHITEFILL              ((ULONG)0x00200000)
#define DRAWMODE_WHITETEXT              ((ULONG)0x00400000)
#define DRAWMODE_WHITEBITMAP            ((ULONG)0x00800000)
#define DRAWMODE_WHITEGRADIENT          ((ULONG)0x01000000)
#define DRAWMODE_SETTINGSLINE           ((ULONG)0x02000000)
#define DRAWMODE_SETTINGSFILL           ((ULONG)0x04000000)
#define DRAWMODE_SETTINGSTEXT           ((ULONG)0x08000000)
#define DRAWMODE_SETTINGSGRADIENT       ((ULONG)0x10000000)
#define DRAWMODE_NOTRANSPARENCY         ((ULONG)0x80000000)

// Antialiasing
#define ANTIALIASING_DISABLE_TEXT       ((USHORT)0x0001)
#define ANTIALIASING_ENABLE_B2DDRAW     ((USHORT)0x0002)
#define ANTIALIASING_PIXELSNAPHAIRLINE  ((USHORT)0x0004)

// AddFontSubstitute
#define FONT_SUBSTITUTE_ALWAYS          ((USHORT)0x0001)
#define FONT_SUBSTITUTE_SCREENONLY      ((USHORT)0x0002)

// Default-Font
#define DEFAULTFONT_SANS_UNICODE        ((USHORT)1)
#define DEFAULTFONT_SANS                ((USHORT)2)
#define DEFAULTFONT_SERIF               ((USHORT)3)
#define DEFAULTFONT_FIXED               ((USHORT)4)
#define DEFAULTFONT_SYMBOL              ((USHORT)5)
#define DEFAULTFONT_UI_SANS             ((USHORT)1000)
#define DEFAULTFONT_UI_FIXED            ((USHORT)1001)
#define DEFAULTFONT_LATIN_TEXT          ((USHORT)2000)
#define DEFAULTFONT_LATIN_PRESENTATION  ((USHORT)2001)
#define DEFAULTFONT_LATIN_SPREADSHEET   ((USHORT)2002)
#define DEFAULTFONT_LATIN_HEADING       ((USHORT)2003)
#define DEFAULTFONT_LATIN_DISPLAY       ((USHORT)2004)
#define DEFAULTFONT_LATIN_FIXED         ((USHORT)2005)
#define DEFAULTFONT_CJK_TEXT            ((USHORT)3000)
#define DEFAULTFONT_CJK_PRESENTATION    ((USHORT)3001)
#define DEFAULTFONT_CJK_SPREADSHEET     ((USHORT)3002)
#define DEFAULTFONT_CJK_HEADING         ((USHORT)3003)
#define DEFAULTFONT_CJK_DISPLAY         ((USHORT)3004)
#define DEFAULTFONT_CTL_TEXT            ((USHORT)4000)
#define DEFAULTFONT_CTL_PRESENTATION    ((USHORT)4001)
#define DEFAULTFONT_CTL_SPREADSHEET     ((USHORT)4002)
#define DEFAULTFONT_CTL_HEADING         ((USHORT)4003)
#define DEFAULTFONT_CTL_DISPLAY         ((USHORT)4004)

#define DEFAULTFONT_FLAGS_ONLYONE       ((ULONG)0x00000001)

struct ImplMapRes
{
    long                mnMapOfsX;          // Offset in X Richtung
    long                mnMapOfsY;          // Offset in Y Richtung
    long                mnMapScNumX;        // Skal.-faktor Zaehler X Richtung
    long                mnMapScNumY;        // Skal.-faktor Zaehler Y Richtung
    long                mnMapScDenomX;      // Skal.-faktor Nenner X Richtung
    long                mnMapScDenomY;      // Skal.-faktor Nenner Y Richtung
};

struct ImplThresholdRes
{
    long                mnThresLogToPixX;   // Schwellenwerte fuer Berechnung
    long                mnThresLogToPixY;   // mit BigInts
    long                mnThresPixToLogX;   // ""
    long                mnThresPixToLogY;   // ""
};

typedef std::vector< Rectangle > MetricVector;

// -----------------------
// - ImplDevFontListData -
// -----------------------

// flags for mnMatchType member
#define IMPL_DEVFONT_SCALABLE       ((ULONG)0x00000001)
#define IMPL_DEVFONT_SYMBOL         ((ULONG)0x00000002)
#define IMPL_DEVFONT_NONESYMBOL     ((ULONG)0x00000004)
#define IMPL_DEVFONT_LIGHT          ((ULONG)0x00000010)
#define IMPL_DEVFONT_BOLD           ((ULONG)0x00000020)
#define IMPL_DEVFONT_NORMAL         ((ULONG)0x00000040)
#define IMPL_DEVFONT_NONEITALIC     ((ULONG)0x00000100)
#define IMPL_DEVFONT_ITALIC         ((ULONG)0x00000200)

// TODO: rename ImplDevFontListData to PhysicalFontFamily
class ImplDevFontListData
{
public:
                        ImplDevFontListData( const String& rSearchName );
                        ~ImplDevFontListData();

    const String&       GetFamilyName() const    { return maName; }
    const String&       GetSearchName() const    { return maSearchName; }
    const String&       GetAliasNames() const    { return maMapNames; }
    bool                IsScalable() const       { return mpFirst->IsScalable(); }
    int                 GetMinQuality() const    { return mnMinQuality; }

    bool                AddFontFace( ImplFontData* );
#ifdef AVS
    void                InitMatchData( const vcl::FontSubstConfiguration&,
                            const String& rSearchName );
#endif
    ImplFontData*       FindBestFontFace( const ImplFontSelectData& rFSD ) const;
#ifdef AVS
    void                GetFontHeights( std::set<int>& rHeights ) const;
    void                UpdateDevFontList( ImplGetDevFontList& ) const;
    void                UpdateCloneFontList( ImplDevFontList&,
                            bool bScalable, bool bEmbeddable ) const;
#endif
private:
friend class ImplDevFontList; // TODO: remove soon
    ImplFontData*       mpFirst;            // linked list of physical font faces
    String              maName;             // Fontname (original font family name)
    String              maSearchName;       // normalized font family name
    String              maMapNames;         // fontname aliases
    ULONG               mnTypeFaces;        // Typeface Flags
    ULONG               mnMatchType;        // MATCH - Type
    String              maMatchFamilyName;  // MATCH - FamilyName
    FontWeight          meMatchWeight;      // MATCH - Weight
    FontWidth           meMatchWidth;       // MATCH - Width
    FontFamily          meFamily;
    FontPitch           mePitch;
    int                 mnMinQuality;       // quality of the worst font face
};


// ---------------
// - ImplCvtChar -
// ---------------

class ImplCvtChar
{
public:
    const sal_Unicode*  mpCvtTab;
    const char*         mpSubsFontName;
    sal_Unicode         (*mpCvtFunc)( sal_Unicode );
};

sal_Unicode ImplRecodeChar( const ImplCvtChar* pCvtData, sal_Unicode c );
void ImplRecodeString( const ImplCvtChar* pCvtData, String& rStr,
                       xub_StrLen nIndex, xub_StrLen nLen );
#ifdef AVS
const ImplCvtChar* ImplGetRecodeData( const String& rOrgFontName,
                                      const String& rMapFontName );

// ----------------------
// - ImplGetDevFontList -
// ----------------------

// an ImplGetDevFontList is created by an ImplDevFontList
// it becomes invalid when original ImplDevFontList is modified
class ImplGetDevFontList
{
private:
    std::vector<ImplFontData*> maDevFontVector;

public:
                        ImplGetDevFontList()        { maDevFontVector.reserve(1024); }
    void                Add( ImplFontData* pFace )  { maDevFontVector.push_back( pFace ); }
    ImplFontData*       Get( int nIndex ) const     { return maDevFontVector[ nIndex ]; }
    int                 Count() const               { return maDevFontVector.size(); }
};

// ----------------------
// - ImplGetDevSizeList -
// ----------------------

class ImplGetDevSizeList
{
private:
    String              maFontName;
    std::vector<int>    maSizeList;

public:
                        ImplGetDevSizeList( const String& rFontName )
                        : maFontName( rFontName ) { maSizeList.reserve( 32 ); }
    void                Add( int nHeight )      { maSizeList.push_back( nHeight ); }
    int                 Count() const           { return maSizeList.size(); }
    int                 Get( int nIndex ) const { return maSizeList[ nIndex ]; }
    const String&       GetFontName() const     { return maFontName; }
};

// ------------------------
// - ImplFontSubstitution -
// ------------------------
// nowadays these substitutions are needed for backward compatibility and tight platform integration:
// - substitutions from configuration entries (Tools->Options->FontReplacement and/or fontconfig)
// - device specific substitutions (e.g. for PS printer builtin fonts)
// - substitutions for missing fonts defined by configuration entries (generic and/or platform dependent fallbacks) 
// - substitutions for missing fonts defined by multi-token fontnames (e.g. fontname="SpecialFont;FallbackA;FallbackB")
// - substitutions for incomplete fonts (implicit, generic, EUDC and/or platform dependent fallbacks)
// - substitutions for missing symbol fonts by translating code points into other symbol fonts 

class ImplFontSubstitution
{
    // TODO: there is more commonality between the different substitutions
protected:
    virtual ~ImplFontSubstitution() {}
};

// ImplDirectFontSubstitution is for Tools->Options->FontReplacement and PsPrinter substitutions
// The clss is just a simple port of the unmaintainable manual-linked-list based mechanism
// TODO: get rid of this class when the Tools->Options->FontReplacement tabpage is gone for good

struct ImplFontSubstEntry
{
    String                  maName;
    String                  maReplaceName;
    String                  maSearchName;
    String                  maSearchReplaceName;
    USHORT                  mnFlags;

    ImplFontSubstEntry(  const String& rFontName, const String& rSubstFontName, USHORT nSubstFlags );
};

class ImplDirectFontSubstitution
:   public ImplFontSubstitution
{
private:
    typedef std::list<ImplFontSubstEntry> FontSubstList;
    FontSubstList maFontSubstList;
public:
    void    AddFontSubstitute( const String& rFontName, const String& rSubstName, USHORT nFlags );
    void    RemoveFontSubstitute( int nIndex );
    bool    GetFontSubstitute( int nIndex, String& rFontName, String& rSubstName, USHORT& rFlags ) const;
    int     GetFontSubstituteCount() const { return maFontSubstList.size(); };
    bool    Empty() const { return maFontSubstList.empty(); }
    void    Clear() { maFontSubstList.clear(); }

    bool    FindFontSubstitute( String& rSubstName, const String& rFontName, USHORT nFlags ) const;
};

// PreMatchFontSubstitution
// abstracts the concept of a configured font substitution
// before the availability of the originally selected font has been checked
class ImplPreMatchFontSubstitution
:   public ImplFontSubstitution
{
public:
    virtual bool FindFontSubstitute( ImplFontSelectData& ) const = 0;
};

// ImplGlyphFallbackFontSubstitution
// abstracts the concept of finding the best font to support an incomplete font
class ImplGlyphFallbackFontSubstitution
:   public ImplFontSubstitution
{
public:
    virtual bool FindFontSubstitute( ImplFontSelectData&, rtl::OUString& rMissingCodes ) const = 0;
};
#endif
// -----------------
// - ImplFontCache -
// -----------------
// TODO: closely couple with ImplDevFontList

class ImplFontCache
{
private:
    ImplFontEntry*      mpFirstEntry;
    int                 mnRef0Count;    // number of unreferenced ImplFontEntries
    bool                mbPrinter;
#ifdef AVS
    // cache of recently used font instances
    struct IFSD_Equal { bool operator()( const ImplFontSelectData&, const ImplFontSelectData& ) const; };
    struct IFSD_Hash { size_t operator()( const ImplFontSelectData& ) const; };
    typedef ::std::hash_map<ImplFontSelectData,ImplFontEntry*,IFSD_Hash,IFSD_Equal > FontInstanceList;
    FontInstanceList    maFontInstanceList;
#endif

    // cache of recently requested font names vs. selected font names
#ifdef AVS
    typedef ::std::hash_map<String,String,FontNameHash> FontNameList;
#endif
    typedef ::stdext::hash_map<String,String> FontNameList;
    FontNameList        maFontNameList;

public:
                        ImplFontCache( bool bPrinter );
                        ~ImplFontCache();

    ImplFontEntry*      GetFontEntry( ImplDevFontList*,
                             const Font&, const Size& rPixelSize, float fExactHeight,
				ImplDirectFontSubstitution* pDevSpecific );

    ImplFontEntry*      GetFontEntry( ImplDevFontList*,
		            ImplFontSelectData&, ImplDirectFontSubstitution* pDevSpecific );
#ifdef AVS
    ImplFontEntry*      GetGlyphFallbackFont( ImplDevFontList*, ImplFontSelectData&,
                            int nFallbackLevel, rtl::OUString& rMissingCodes );
#endif
    void                Release( ImplFontEntry* );
#ifdef AVS
    void                Invalidate();
#endif
};



struct ImplObjStack;
class SalGraphics;

class OutputDevice
{
friend class Bitmap;
private: SalGraphics* mpGraphics;
private: mutable ImplFontEntry* mpFontEntry;
private: mutable ImplFontCache*      mpFontCache;
private: mutable ImplDevFontList*    mpFontList;


private: USHORT mnBitCount;
private: LanguageType meTextLanguage;
private: RasterOp meRasterOp;
private: Color maOverlineColor;
private: OutDevType meOutDevType;
private: ULONG mnTextLayoutMode;
//private: BOOL bRecord;
private: ImplObjStack*       mpObjStack;
//ClipRegion
private: Region maRegion;           // contains the clip region, see SetClipRegion(...)
private: BOOL mbClipRegion:1;
private: BOOL mbInitClipRegion:1;
private: BOOL mbOutputClipped:1;
private: BOOL mbClipRegionSet:1;
//ClipRegion
//Line/FillColor
private: Color maLineColor;
private: BOOL mbLineColor:1;
private: BOOL mbInitLineColor:1;
private: Color maFillColor;
private: BOOL mbFillColor:1;
private: BOOL mbInitFillColor:1;
//Line/FillColor
//MapMode
private: MapMode maMapMode;
private: mutable BOOL mbMap:1;
//MapMode
//RefPoint
private: Point maRefPoint;
private: BOOL mbRefPoint:1;
//RefPoint
//Font
private: Font maFont;
private: mutable BOOL mbNewFont:1;
private: mutable BOOL mbInitFont:1;
private: mutable BOOL mbKerning:1;
//Font
//TextColor
private: Color maTextColor;
private: BOOL mbInitTextColor;
//TextColor
//TextLine
private: Color maTextLineColor;
//TextLine
    /// Additional output pixel offset, applied in LogicToPixel (used by SetPixelOffset/GetPixelOffset)
    long				mnOutOffOrigX;
    /// Additional output offset in _logical_ coordinates, applied in PixelToLogic (used by SetPixelOffset/GetPixelOffset)
    long				mnOutOffLogicX;
    /// Additional output pixel offset, applied in LogicToPixel (used by SetPixelOffset/GetPixelOffset)
    long				mnOutOffOrigY;
    /// Additional output offset in _logical_ coordinates, applied in PixelToLogic (used by SetPixelOffset/GetPixelOffset)
    long				mnOutOffLogicY;
    /// Output offset for device output in pixel (pseudo window offset within window system's frames)
    long                mnOutOffX;
    /// Output offset for device output in pixel (pseudo window offset within window system's frames)
    long                mnOutOffY;
    long                mnOutWidth;
    long                mnOutHeight;
//background
private: Wallpaper maBackground;
private: BOOL mbBackground:1;
//background
private: BOOL mbEnableRTL:1;
private: mutable BOOL mbTextLines:1;
private: mutable BOOL mbTextSpecial:1;
private: USHORT mnAntialiasing;
private:

    sal_Int32           mnDPIX;
    sal_Int32           mnDPIY;
    /// font specific text alignment offsets in pixel units
    mutable long        mnTextOffX;
    mutable long        mnTextOffY;
    mutable long        mnEmphasisAscent;
    mutable long        mnEmphasisDescent;

	//LanguageType        meTextLanguage;

	ImplMapRes          maMapRes;
	ImplThresholdRes    maThresRes;

public:
	OutputDevice();
    virtual ~OutputDevice();
public: sal_Int32	ImplGetDPIX() const { return mnDPIX; }
public: sal_Int32	ImplGetDPIY() const { return mnDPIY; }
private: virtual int ImplGetGraphics();
private: void ImplInitVirDev( long nDX, long nDY, USHORT nBitCount );

public: void ImplInitClipRegion();
public: bool ImplSelectClipRegion( const Region&, SalGraphics* pGraphics = NULL );
public: void ImplInitLineColor();
public: void ImplInitFillColor();
public: Polygon ImplSubdivideBezier( const Polygon& rPoly );
public: PolyPolygon ImplSubdivideBezier( const PolyPolygon& rPolyPoly );
private: Point ImplLogicToDevicePixel( const Point& rLogicPt ) const;
private: Size ImplLogicToDevicePixel( const Size& rLogicSize ) const;
private: Rectangle ImplLogicToDevicePixel( const Rectangle& rLogicRect ) const;
private: Polygon ImplLogicToDevicePixel( const Polygon& rLogicPoly ) const;
private: PolyPolygon ImplLogicToDevicePixel( const PolyPolygon& rLogicPolyPoly ) const;
private: LineInfo ImplLogicToDevicePixel( const LineInfo& rLineInfo ) const;

private: Region ImplPixelToDevicePixel( const Region& rRegion ) const;

public: void DrawPixel( const Point& rPt, const Color& rColor );
public: void DrawPixel( const Point& rPt );

public: void DrawLine( const Point& rStartPt, const Point& rEndPt );
public: void DrawLine( const Point& rStartPt, const Point& rEndPt, const LineInfo& rLineInfo );

public: void DrawRect( const Rectangle& rRect );
public: void DrawRect( const Rectangle& rRect, ULONG nHorzRount, ULONG nVertRound );

public: void DrawEllipse( const Rectangle& rRect );
public: void DrawArc( const Rectangle& rRect, const Point& rStartPt, const Point& rEndPt );
public: void DrawPie( const Rectangle& rRect, const Point& rStartPt, const Point& rEndPt );
public: void DrawChord( const Rectangle& rRect, const Point& rStartPt, const Point& rEndPt );

public: void DrawPolyLine( const Polygon& rPoly );
public: void DrawPolyLine( const Polygon& rPoly, const LineInfo& rLineInfo );
public: void ImpDrawPolyLineWithLineInfo(const Polygon& rPoly, const LineInfo& rLineInfo);

public: void DrawPolygon( const Polygon& rPoly );
public: void ImplDrawPolygon( const Polygon& rPoly, const PolyPolygon* pClipPolyPoly = NULL );
public: void ImplDrawPolyPolygon( const PolyPolygon& rPolyPoly, const PolyPolygon* pClipPolyPoly );
public: void DrawPolyPolygon( const PolyPolygon& rPolyPoly );
public: void ImplDrawPolyPolygon( USHORT nPoly, const PolyPolygon& rPolyPoly );
public: void ImplDrawText( const Rectangle& rRect,
                                              const String& rOrigStr, USHORT nStyle,
                                              MetricVector* pVector, String* pDisplayText );

public: void ImplDrawTextRect( long nBaseX, long nBaseY,
                                     long nX, long nY, long nWidth, long nHeight );
public: long GetTextWidth( const XubString& rStr, xub_StrLen nIndex = 0,
                                      xub_StrLen nLen = STRING_LEN ) const;
public: long GetTextArray( const XubString& rStr, sal_Int32* pDXAry = NULL,
                                      xub_StrLen nIndex = 0,
                                      xub_StrLen nLen = STRING_LEN ) const;
public: long GetTextHeight() const;
public:  static XubString    GetNonMnemonicString( const XubString& rStr, xub_StrLen& rMnemonicPos );
public: void DrawText( const Point& rStartPt, const XubString& rStr,
                                  xub_StrLen nIndex = 0, xub_StrLen nLen = STRING_LEN,
                                  MetricVector* pVector = NULL, String* pDisplayText = NULL );
public: void ImplDrawText( SalLayout& rSalLayout );
public: void ImplDrawTextBackground( const SalLayout& rSalLayout );
public: void ImplDrawTextDirect( SalLayout& rSalLayout, BOOL bTextLines );
public: void ImplDrawEmphasisMarks( SalLayout& rSalLayout );
public: void ImplDrawTextLines( SalLayout& rSalLayout,
    FontStrikeout eStrikeout, FontUnderline eUnderline, FontUnderline eOverline, BOOL bWordLine, BOOL bUnderlineAbove );
public: void ImplDrawTextLine( long nBaseX,
                                     long nX, long nY, long nWidth,
                                     FontStrikeout eStrikeout,
                                     FontUnderline eUnderline,
                                     FontUnderline eOverline,
                                     BOOL bUnderlineAbove );
public: void ImplDrawStraightTextLine( long nBaseX, long nBaseY,
                                             long nX, long nY, long nWidth,
                                             FontUnderline eTextLine,
                                             Color aColor,
                                             BOOL bIsAbove );
public: void ImplDrawStrikeoutChar( long nBaseX, long nBaseY,
                                          long nX, long nY, long nWidth,
                                          FontStrikeout eStrikeout,
                                          Color aColor );
public: void ImplDrawWaveTextLine( long nBaseX, long nBaseY,
                                         long nX, long nY, long nWidth,
                                         FontUnderline eTextLine,
                                         Color aColor,
                                         BOOL bIsAbove );
public: void ImplDrawWaveLine( long nBaseX, long nBaseY,
                                     long nStartX, long nStartY,
                                     long nWidth, long nHeight,
                                     long nLineWidth, short nOrientation,
                                     const Color& rColor );
public: void ImplDrawStrikeoutLine( long nBaseX, long nBaseY,
                                          long nX, long nY, long nWidth,
                                          FontStrikeout eStrikeout,
                                          Color aColor );

public: BOOL ImplIsUnderlineAbove( const Font& rFont );
public: bool ImplDrawRotateText( SalLayout& rSalLayout );
public: void ImplDrawSpecialText( SalLayout& rSalLayout );
public: void ImplInitTextColor();
public: SalLayout* ImplLayout( const String&, xub_StrLen nIndex,
                                            xub_StrLen nLen, const Point& rLogicPos = Point(0,0),
                                            long nLogicWidth=0, const sal_Int32* pLogicDXArray=NULL,
                                            bool bFilter = false )  const;
#ifdef QWE
public: SalLayout* ImplGlyphFallbackLayout( SalLayout* pSalLayout, ImplLayoutArgs& rLayoutArgs ) const;
#endif
public: ImplLayoutArgs ImplPrepareLayoutArgs( String& rStr,
                                       xub_StrLen nMinIndex, xub_StrLen nLen,
                                       long nPixelWidth, const sal_Int32* pDXArray ) const;

public: void ImplInitFont()  const;
public: bool ImplNewFont() const;
public: FontEmphasisMark ImplGetEmphasisMarkStyle( const Font& rFont ) const;
public: void ImplInitFontList() const;
public: void DrawTextArray( const Point& rStartPt, const XubString& rStr,
                                       const sal_Int32* pDXAry = NULL,
                                       xub_StrLen nIndex = 0,
                                       xub_StrLen nLen = STRING_LEN );
public: void DrawStretchText( const Point& rStartPt, ULONG nWidth,
                                         const XubString& rStr,
                                         xub_StrLen nIndex = 0, xub_StrLen nLen = STRING_LEN );
public: void DrawText( const Rectangle& rRect,
                                  const XubString& rStr, USHORT nStyle = 0,
                                  MetricVector* pVector = NULL, String* pDisplayText = NULL );
public: void DrawTextLine( const Point& rPos, long nWidth,
                                      FontStrikeout eStrikeout,
                                      FontUnderline eUnderline,
                                      FontUnderline eOverline,
                                      BOOL bUnderlineAbove = FALSE );

public: void DrawBitmap( const Point& rDestPt,
                                    const Bitmap& rBitmap );
public: void DrawBitmap( const Point& rDestPt, const Size& rDestSize,
                                    const Bitmap& rBitmap );
public: void DrawBitmap( const Point& rDestPt, const Size& rDestSize,
                                    const Point& rSrcPtPixel, const Size& rSrcSizePixel,
                                    const Bitmap& rBitmap );
public: void ImplDrawBitmap( const Point& rDestPt, const Size& rDestSize,
								   const Point& rSrcPtPixel, const Size& rSrcSizePixel,
								   const Bitmap& rBitmap, const ULONG nAction );
public: void DrawBitmapEx( const Point& rDestPt,
                                      const BitmapEx& rBitmapEx );
public: void DrawBitmapEx( const Point& rDestPt, const Size& rDestSize,
                                      const BitmapEx& rBitmapEx );
public: void DrawBitmapEx( const Point& rDestPt, const Size& rDestSize,
                                      const Point& rSrcPtPixel, const Size& rSrcSizePixel,
                                      const BitmapEx& rBitmapEx );
public: void ImplDrawBitmapEx( const Point& rDestPt, const Size& rDestSize,
										const Point& rSrcPtPixel, const Size& rSrcSizePixel,
										const BitmapEx& rBitmapEx, const ULONG nAction );
public: void ImplDrawAlpha( const Bitmap& rBmp, const AlphaMask& rAlpha,
								  const Point& rDestPt, const Size& rDestSize,
								  const Point& rSrcPtPixel, const Size& rSrcSizePixel );
public: Bitmap ImplBlendWithAlpha( Bitmap              aBmp,
                                         BitmapReadAccess*	 pP,
                                         BitmapReadAccess*	 pA,
                                         const Rectangle&    aDstRect,
                                         const sal_Int32     nOffY, 
                                         const sal_Int32     nDstHeight, 
                                         const sal_Int32     nOffX, 
                                         const sal_Int32     nDstWidth,
                                         const long*         pMapX,
                                         const long*         pMapY );
public: Bitmap ImplBlend( Bitmap              aBmp,
                                           BitmapReadAccess*   pP,
                                           BitmapReadAccess*   pA,
                                           const sal_Int32     nOffY,
                                           const sal_Int32     nDstHeight,
                                           const sal_Int32     nOffX,
                                           const sal_Int32     nDstWidth,
                                           const Rectangle&    aBmpRect,
                                           const Size&         aOutSz,
                                           const bool          bHMirr,
                                           const bool          bVMirr,
                                           const long*         pMapX,
                                           const long*         pMapY );
public: void DrawMask( const Point& rDestPt,
                                  const Bitmap& rBitmap, const Color& rMaskColor );
public: void DrawMask( const Point& rDestPt, const Size& rDestSize,
                                  const Bitmap& rBitmap, const Color& rMaskColor );
public: void DrawMask( const Point& rDestPt, const Size& rDestSize,
                                  const Point& rSrcPtPixel, const Size& rSrcSizePixel,
                                  const Bitmap& rBitmap, const Color& rMaskColor );
public: void ImplDrawMask( const Point& rDestPt, const Size& rDestSize,
								 const Point& rSrcPtPixel, const Size& rSrcSizePixel,
								 const Bitmap& rBitmap, const Color& rMaskColor,
								 const ULONG nAction );
public: void DrawGradient( const Rectangle& rRect, const Gradient& rGradient );
public: void DrawGradient( const PolyPolygon& rPolyPoly, const Gradient& rGradient );
public: void ImplDrawLinearGradient( const Rectangle& rRect,
										   const Gradient& rGradient,
										   BOOL bMtf, const PolyPolygon* pClipPolyPoly );
public: void ImplDrawComplexGradient( const Rectangle& rRect,
										    const Gradient& rGradient,
										    BOOL bMtf, const PolyPolygon* pClipPolyPoly );
public: void DrawHatch( const PolyPolygon& rPolyPoly, const Hatch& rHatch );
public: void ImplDrawHatch( const PolyPolygon& rPolyPoly, const Hatch& rHatch, BOOL bMtf );
public: void ImplCalcHatchValues( const Rectangle& rRect, long nDist, USHORT nAngle10,
										Point& rPt1, Point& rPt2, Size& rInc, Point& rEndPt1 );
public: void ImplDrawHatchLine( const Line& rLine, const PolyPolygon& rPolyPoly,
									  Point* pPtBuffer, BOOL bMtf );
public: void DrawWallpaper( const Rectangle& rRect, const Wallpaper& rWallpaper );
public: void ImplDrawWallpaper( long nX, long nY,
									  long nWidth, long nHeight,
									  const Wallpaper& rWallpaper );
public: void ImplDrawBitmapWallpaper( long nX, long nY,
											long nWidth, long nHeight,
											const Wallpaper& rWallpaper );
public: void ImplDrawGradientWallpaper( long nX, long nY,
											  long nWidth, long nHeight,
											  const Wallpaper& rWallpaper );
public: void ImplDrawColorWallpaper( long nX, long nY,
										   long nWidth, long nHeight,
										   const Wallpaper& rWallpaper );
public: void DrawTransparent( const PolyPolygon& rPolyPoly, USHORT nTransparencePercent );
public: void DrawTransparent( const GDIMetaFile& rMtf, const Point& rPos, const Size& rSize, const Gradient& rTransparenceGradient );

public: void DrawEPS( const Point& rPt, const Size& rSz,
                                 const GfxLink& rGfxLink, GDIMetaFile* pSubst = NULL );

bool OutputDevice::ImplIsAntiparallel() const;
private: void ImplSetClipRegion( const Region* pRegion );
public: void SetClipRegion();
public: void SetClipRegion( const Region& rRegion );
public: void MoveClipRegion( long nHorzMove, long nVertMove );
public: void IntersectClipRegion( const Rectangle& rRect );
public: void IntersectClipRegion( const Region& rRegion );
public: Region GetClipRegion() const;
public: void SetAntialiasing( USHORT nMode =  0 );
public: USHORT GetAntialiasing() const { return mnAntialiasing; }
public: Size GetPixelOffset() const;
public: void SetPixelOffset( const Size& rOffset );
public: Size GetOutputSize() const
        { return PixelToLogic( GetOutputSizePixel() ); }


public: void SetLineColor();
public: void SetLineColor( const Color& rColor );
public: const Color& GetLineColor() const { return maLineColor; }
public: void SetFillColor();
public: void SetFillColor( const Color& rColor );
public: const Color& GetFillColor() const { return maFillColor; }

public: void SetTextColor( const Color& rColor );
public: const Color& GetTextColor() const { return maTextColor; }
public: void SetTextFillColor();
public: void SetTextFillColor( const Color& rColor );
public: Color GetTextFillColor() const;
public: BOOL IsTextFillColor() const { return !maFont.IsTransparent(); }

public: void SetTextLineColor();
public: void SetTextLineColor( const Color& rColor );
public: const Color& GetTextLineColor() const { return maTextLineColor; }
public: BOOL IsTextLineColor() const { return (maTextLineColor.GetTransparency() == 0); }

public: void SetOverlineColor();
public: void SetOverlineColor( const Color& rColor );
public: const Color& GetOverlineColor() const { return maOverlineColor; }
public: BOOL IsOverlineColor() const { return (maOverlineColor.GetTransparency() == 0); }

public: void SetTextAlign( TextAlign eAlign );
public: TextAlign GetTextAlign(){ return maFont.GetAlign(); }

public: void                SetBackground();
public: void                SetBackground( const Wallpaper& rBackground );

public: const Wallpaper&    GetBackground() const { return maBackground; }
public: BOOL                IsBackground() const { return mbBackground; }

public: void SetMapMode();
public: void SetMapMode( const MapMode& rNewMapMode );
public: MapMode GetMapMode() const { return maMapMode; }
		
public: void SetRelativeMapMode( const MapMode& rNewMapMode );
public: void EnableMapMode( BOOL bEnable = TRUE );
public: BOOL IsMapModeEnabled() const { return mbMap; }
public: BOOL IsRTLEnabled() const { return mbEnableRTL; }
public: USHORT GetBitCount() const;

public: void SetDigitLanguage( LanguageType );
public: LanguageType GetDigitLanguage() const { return meTextLanguage; }
public: void SetLayoutMode( ULONG nTextLayoutMode );
public: ULONG GetLayoutMode() const { return mnTextLayoutMode; }
public: void SetRefPoint();
public: void SetRefPoint( const Point& rRefPoint );
public: const Point& GetRefPoint() const { return maRefPoint; }
public: BOOL IsRefPoint() const { return mbRefPoint; }

public: OutDevType GetOutDevType() const { return meOutDevType; }

public: void Push( USHORT nFlags = PUSH_ALL );
public: void Pop();

public: void SetFont( const Font& rNewFont );
public: Font GetFont(){ return maFont; }

public: void SetRasterOp( RasterOp eRasterOp );
public: RasterOp GetRasterOp( ){return meRasterOp;}
#ifdef QWE
public: void SetDrawMode( ULONG nDrawMode );
#endif
public: SalGraphics* GetGraphics(){ return mpGraphics; }

public: BOOL SetOutputSizePixel( const Size& rNewSize, BOOL bErase = TRUE );
public: BOOL SetOutputSize( const Size& rNewSize, BOOL bErase = TRUE )
							{ return SetOutputSizePixel( LogicToPixel( rNewSize ), bErase ); }
public: BOOL ImplSetOutputSizePixel( const Size& rNewSize, BOOL bErase );
public: long GetOutputWidthPixel() const { return mnOutWidth; }
public: long GetOutputHeightPixel() const { return mnOutHeight; }
public: long GetOutOffXPixel() const { return mnOutOffX; }
public: long GetOutOffYPixel() const { return mnOutOffY; }
public: Size GetOutputSizePixel() const { return Size( mnOutWidth, mnOutHeight ); }
public: Bitmap GetBitmap( const Point& rSrcPt, const Size& rSize ) const;

public: Point LogicToPixel( const Point& rLogicPt ) const;
public: Size LogicToPixel( const Size& rLogicSize ) const;
public: Rectangle LogicToPixel( const Rectangle& rLogicRect ) const;
public: Polygon LogicToPixel( const Polygon& rLogicPoly ) const;
public: PolyPolygon LogicToPixel( const PolyPolygon& rLogicPolyPoly ) const;
public: Region LogicToPixel( const Region& rLogicRegion )const;
public: Point LogicToPixel( const Point& rLogicPt,
                                      const MapMode& rMapMode ) const;
public: Size LogicToPixel( const Size& rLogicSize,
                                      const MapMode& rMapMode ) const;
public: Rectangle LogicToPixel( const Rectangle& rLogicRect,
                                      const MapMode& rMapMode ) const;
public: Polygon LogicToPixel( const Polygon& rLogicPoly,
                                      const MapMode& rMapMode ) const;
public: PolyPolygon LogicToPixel( const PolyPolygon& rLogicPolyPoly,
                                      const MapMode& rMapMode ) const;
public: Region LogicToPixel( const Region& rLogicRegion,
                                      const MapMode& rMapMode ) const;
public: Point PixelToLogic( const Point& rDevicePt ) const;
public: Size PixelToLogic( const Size& rDeviceSize ) const;
public: Rectangle PixelToLogic( const Rectangle& rDeviceRect ) const;
public: Polygon PixelToLogic( const Polygon& rDevicePoly ) const;
public: PolyPolygon PixelToLogic( const PolyPolygon& rDevicePolyPoly ) const;
public: Region PixelToLogic( const Region& rDeviceRegion ) const;
public: Point PixelToLogic( const Point& rDevicePt,
                                      const MapMode& rMapMode ) const;
public: Size PixelToLogic( const Size& rDeviceSize,
                                      const MapMode& rMapMode ) const;
public: Rectangle PixelToLogic( const Rectangle& rDeviceRect,
                                      const MapMode& rMapMode ) const;
public: Polygon PixelToLogic( const Polygon& rDevicePoly,
                                      const MapMode& rMapMode ) const;
public: PolyPolygon PixelToLogic( const PolyPolygon& rDevicePolyPoly,
                                      const MapMode& rMapMode ) const;
public: Region PixelToLogic( const Region& rDeviceRegion,
                                      const MapMode& rMapMode ) const;


public: Point LogicToLogic( const Point&      rPtSource,
                                      const MapMode*    pMapModeSource,
                                      const MapMode*    pMapModeDest ) const;
public: Size LogicToLogic( const Size&       rSzSource,
                                      const MapMode*    pMapModeSource,
                                      const MapMode*    pMapModeDest ) const;
public: Rectangle LogicToLogic( const Rectangle&  rRectSource,
                                      const MapMode*    pMapModeSource,
                                      const MapMode*    pMapModeDest ) const;
public: long* LogicToLogic( long*             pX,
                                      USHORT            nCount,
                                      const MapMode*    pMapModeSource,
                                      const MapMode*    pMapModeDest ) const;
public: static Point LogicToLogic( const Point&      rPtSource,
                                      const MapMode&    rMapModeSource,
                                      const MapMode&    rMapModeDest );
public: static Size LogicToLogic( const Size&       rSzSource,
                                      const MapMode&    rMapModeSource,
                                      const MapMode&    rMapModeDest );
public: static Rectangle LogicToLogic( const Rectangle&  rRectSource,
                                      const MapMode&    rMapModeSource,
                                      const MapMode&    rMapModeDest );
public: static long LogicToLogic( long              nLongSource,
                                      MapUnit           eUnitSource,
                                      MapUnit           eUnitDest );


public: long ImplLogicXToDevicePixel( long nX ) const;
public: long ImplLogicYToDevicePixel( long nY ) const;
public: long ImplDevicePixelToLogicWidth( long nWidth ) const;
public: long ImplDevicePixelToLogicHeight( long nHeight ) const;
public: float ImplFloatDevicePixelToLogicWidth( float ) const;
public: float ImplFloatDevicePixelToLogicHeight( float ) const;
// -----------------------------------------------------------------------
public: long ImplLogicWidthToDevicePixel( long nWidth ) const;
// -----------------------------------------------------------------------
public: long ImplLogicHeightToDevicePixel( long nHeight ) const;
public: float ImplFloatLogicHeightToDevicePixel( float fLogicHeight) const;

public: void Erase();
public: BOOL GetGlyphBoundRects( const Point& rOrigin, const String& rStr,
    int nIndex, int nLen, int nBase, MetricVector& rVector );
public: BOOL GetTextBoundRect( Rectangle& rRect,
    const String& rStr, xub_StrLen nBase, xub_StrLen nIndex,
    xub_StrLen nLen ) const;
//public: GDIMetaFile* GetConnectMetaFile() const { return mpMetaFile; }
};


typedef OutputDevice VirtualDevice;
}//SVMCore
