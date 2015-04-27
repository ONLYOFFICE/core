#ifndef _WMF_TYPES_H_
#define _WMF_TYPES_H_

// freetype
#include <ft2build.h>
#include FT_OUTLINE_H
//#include FT_SIZES_H
//#include FT_GLYPH_H
//#include FT_TRUETYPE_IDS_H
//#include FT_TRUETYPE_TABLES_H
//#include FT_XFREE86_H
//#include FT_ADVANCES_H

#ifndef BYTE
typedef unsigned char BYTE;
#endif

//---------------------------------------------------------------------------------------------------
// Преобразование типов
//---------------------------------------------------------------------------------------------------

#ifdef UShort_2_Short
#undef UShort_2_Short
#endif /* UShort_2_Short */
#define UShort_2_Short(X) (((X) & 0x8000) ? ((short) ((int) (X) - (int) 0x10000)) : ((short) (X)))

#ifdef UShort_2_Long
#undef UShort_2_Long
#endif /* UShort_2_Long */
#define UShort_2_Long(X) (((X) & 0x8000) ? ((int) (X) - (int) 0x10000) : ((int) (X)))

#define POINT_TO_INCH(X) ((double) (X) / (double) 72  )
#define INCH_TO_POINT(X) ((double) (X) * (double) 72  )
#define MM_TO_INCH(X)    ((double) (X) / (double) 25.4)

//---------------------------------------------------------------------------------------------------
// Modes for CWmfFile.SetMapMode
//---------------------------------------------------------------------------------------------------
#define MM_TEXT        1
#define MM_LOMETRIC    2
#define MM_HIMETRIC    3
#define MM_LOENGLISH   4
#define MM_HIENGLISH   5
#define MM_TWIPS       6
#define MM_ISOTROPIC   7
#define MM_ANISOTROPIC 8
#define MM_DPI         9

//---------------------------------------------------------------------------------------------------
// Regions
//---------------------------------------------------------------------------------------------------

#define ERROR         0
#define NULLREGION    1
#define SIMPLEREGION  2
#define COMPLEXREGION 3

#define RGN_AND  1
#define RGN_OR   2
#define RGN_XOR  3
#define RGN_DIFF 4
#define RGN_COPY 5

#define CLIP_INTERSECT 0x0001
#define CLIP_EXCLUDE   0x0002
#define CLIP_KEEPRGN   0x0004

//---------------------------------------------------------------------------------------------------
// Структуры для чтения файла
//---------------------------------------------------------------------------------------------------
struct TMemoryInfo
{	
	unsigned char *pMemory;     // Указатель на начало данных
	unsigned char *pPointer;    // Указатель на текущую позицию
							    
	int           lPos;        // Текущая позиция
	int           lLength;     // Размер данных
	bool           bNeedDelete; // Нужно ли освобождать память
};

#define WMF_NOT_OPEN         -1
#define WMF_OPEN_FROM_FILE    0
#define WMF_OPEN_FROM_MEMORY  1

//---------------------------------------------------------------------------------------------------
// Структуры для записи
//---------------------------------------------------------------------------------------------------
enum EWmfWrite
{	
	wmf_write_WMF = 0,
	wmf_write_XML = 1
};

struct TWmfWriteFileInfo
{	
	FILE     *pFile;
	EWmfWrite eType;
};


//---------------------------------------------------------------------------------------------------
// Заголовки Wmf-файла
//---------------------------------------------------------------------------------------------------

struct TWmfPlaceableMetaHeader
{	
	unsigned int   unKey;       // Magic number (всегда 9AC6CDD7h)
	unsigned short ushHandle;   // Metafile HANDLE number (всегда 0)
	short          shLeft;      // Левая координата в единицах метафайла
	short          shTop;       // Верхняя координата в единицах метафайла
	short          shRight;     // Правая координата в единицах метафайла
	short          shBottom;    // Верхняя координата в единицах метафайла
	unsigned short ushInch;     // Число единиц в одном дюйме (dpi)
	unsigned int   unReserved;  // Reserved (всегда 0)
	unsigned short ushChecksum; // Чексумма для предыдущих 20 байт
};

struct TWmfHead
{
	unsigned short ushFileType;        // Тип метафайла (0 = memory, 1 = disk)
	unsigned short ushHeaderSize;      // Размер данного заголовка (всегда 9)
	unsigned short ushVersion;         // Version of Microsoft Windows
	unsigned int   unFileSize;         // Общий размер данного метафайла в Unsigned Short
	unsigned short ushNumberOfObjects; // Общее количество графических объектов в файле
	unsigned int   unMaxRecordSize;    // Максимальный размер среди всех записей(Record) в данном файле
	unsigned short ushNumberOfMembers; // Not Used (всегда 0)
};

typedef struct _TWmfMetaHeader
{	
	TWmfPlaceableMetaHeader *pPlaceableMetaHeader;
	TWmfHead                *pHeader;

	FILE                    *pFileIn;
	int                     lPos;
	int                      nPlaceable;
} TWmfMetaHeader, *TWmfFile;


//---------------------------------------------------------------------------------------------------
// Ошибки
//---------------------------------------------------------------------------------------------------

enum EWmfError
{	
	wmf_error_None            = 0, // Нет оишбки
	wmf_error_NotEnoughMemory = 1, // Невозможно выделить память
	wmf_error_BadFile         = 2, // Проврежденный или недоступный файл
	wmf_error_BadFormat       = 3, // Метафайл поврежден
	wmf_error_EOF             = 4, // Непредаполагаем конец файла
	wmf_error_DeviceError     = 5, // 
	wmf_error_Internal        = 6, // Внутреняя ошибка
	wmf_error_Assert          = 7, // Внутреняя ошибка
	wmf_error_UserExit        = 8, //
	wmf_error_UnSupported     = 9  // Неподдерживается тег
};

//---------------------------------------------------------------------------------------------------
// Основные структуры
//---------------------------------------------------------------------------------------------------

// Координаты точки
struct TWmfCoordF
{	
	float fX;
	float fY;
};

// Прямоугольник
struct TWmfRectF
{
	TWmfCoordF oTL; // Левый верхний угол
	TWmfCoordF oBR; // Правый нижний угол
};

struct TWmfMapping
{
	char       *sName;
	char       *sMapping;

	/* I had been hoping to keep FT out of this file, but
	 * it seems easier just to use the FT encoding defs
	 * rather than create some kind of wrapper...
	 */
	FT_Encoding eEncoding;
};

struct TWmfFontMap
{
	char *sName;   // Wmf font name

	char *sNamePS; // Postscript font names
	char *sItalic;
	char *sBold;
	char *sBoldItalic;
};


struct TWmfRGB
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

struct TWmfBMP
{
	unsigned short ushWidth;
	unsigned short ushHeight;

	BYTE          *pData;
};

struct TWmfPen
{	
	unsigned short ushStyle;

	double         dWidth;
	double         dHeight;  // ignored

	TWmfRGB        oColor;
};

struct TWmfBrush
{
	unsigned short ushStyle;
	unsigned short ushHatch;

	TWmfRGB        oColor;
	TWmfBMP        oBitmap;
};

struct TWmfFont
{
	unsigned short ushHeight;
	unsigned short ushWidth;

	short          shEscapement;
	short          shOrientation;

	unsigned short ushWeight;

	unsigned char  unItalic;
	unsigned char  unUnderline;
	unsigned char  unStrikeOut;
	unsigned char  unCharSet;
	unsigned char  unOutPrecision;
	unsigned char  unClipPrecision;
	unsigned char  unQuality;
	unsigned char  unPitchAndFamily;

	char          *sFaceName;

	void          *pUserData;
};

// Structure containing list of XML attributes
struct TWmfAttributes
{
	char          *sName;

	char         **psAttrs;

	unsigned int  ulCount;
	unsigned int  ulMax;

	unsigned char *sBuffer;

	unsigned int  ulLength;
	unsigned int  ulOffset;
};

// Structure containing list of lists of XML attributes
struct TWmfAttributeStore
{	
	TWmfAttributes *pAttrlist;

	unsigned int   ulCount;
	unsigned int   ulMax;
};

struct TWmfRegion;
struct TWmfDC
{	
	TWmfBrush      *pBrush;
	TWmfPen        *pPen;
	TWmfFont       *pFont;

	TWmfRGB         oTextColor;
	TWmfRGB         oBGColor;

	unsigned short  ushTextAlign;
	unsigned short  ushBGMode;
	unsigned short  ushPolyFillMode;
	unsigned short  ushCharExtra;
	unsigned short  ushBreakExtra;

	unsigned short  ushROPMode;

	struct
	{
		int nX;
		int nY;
		int nWidth;
		int nHeight;
	} oWindow;

	double          dPixelWidth; // dpi
	double          dPixelHeight;

	unsigned short  ushMapMode;

	TWmfRegion     *pClip;
};

//---------------------------------------------------------------------------------------------------
// Структуры для рисования
//---------------------------------------------------------------------------------------------------

struct TWmfFlood
{	
	TWmfDC        *pDC;

	TWmfCoordF     oPoint;
	TWmfRGB        oColor;

	unsigned short ushType;

	double         dPixelWidth;
	double         dPixelHeight;
};

struct TWmfDrawPixel
{
	TWmfDC    *pDC;

	TWmfCoordF oPoint;
	TWmfRGB    oColor;

	double     dPixelWidth;
	double     dPixelHeight;
};

struct TWmfDrawArc
{	
	TWmfDC    *pDC;

	TWmfCoordF oTL; // Прямоугольник, в который вписан эллипс 
	TWmfCoordF oBR; // данной дуги

	TWmfCoordF oStart; // Начало дуги(при рисовании эллипса не используется)
	TWmfCoordF oEnd;   // Конец дуги (координаты относительно начала эллипса)
};

struct TWmfDrawLine
{	
	TWmfDC    *pDC;

	TWmfCoordF oFrom;
	TWmfCoordF oTo;
};

struct TWmfPolyLine
{	
	TWmfDC        *pDC;

	TWmfCoordF    *pPoints;

	unsigned short ushCount;
};

struct TWmfPolyPoly
{	
	TWmfDC         *pDC;

	TWmfCoordF    **ppPoints;    // ppPoints[i][*] - точки i-го полигона

	unsigned short *pCount;      // Количество точек в i-ом полигоне
	unsigned short ushPolyCount; // Количество полигонов
};

struct TWmfDrawRectangle
{	
	TWmfDC    *pDC;

	TWmfCoordF oTL;
	TWmfCoordF oBR;

	float      fWidth;  // Оси угловых закруглений (0, если прямоугольник без закругленных углов)
	float      fHeight; // 
};

struct TWmfPolyRectangle
{
	TWmfDC      *pDC;

	TWmfCoordF  *pTL;
	TWmfCoordF  *pBR;

	unsigned int unCount;

	float        fWidth;
	float        fHeight;
};

struct TWmfBMPRead
{
	int           lOffset;
	int           lLength;
	unsigned char *pBuffer;

	unsigned short ushWidth;
	unsigned short ushHeight;

	TWmfBMP        oBitmap;
};

struct TWmfBMPDraw
{
	TWmfDC         *pDC;

	TWmfCoordF      oPoint;
	TWmfBMP         oBitmap;

	unsigned int    unType;

	struct
	{	
		unsigned short ushX;
		unsigned short ushY;
		unsigned short ushW;
		unsigned short ushH;
	} oCrop;

	double          dPixelWidth;
	double          dPixelHeight;
};

struct TWmfROPDraw
{
	TWmfDC      *pDC;

	TWmfCoordF   oTL;
	TWmfCoordF   oBR;

	unsigned int unROP;

	double       dPixelWidth;
	double       dPixelHeight;
};

struct TWmfDrawText
{
	TWmfDC         *pDC;

	TWmfCoordF      oPoint;   // Исходная точка для рисования(с учетом TextAlign)
	TWmfCoordF      oOrigin;  // Исходная точка для рисования
	bool            bUseDx; 

	TWmfCoordF      oTL; // Clip
	TWmfCoordF      oBR; //

	struct
	{	
		TWmfCoordF oTL;
		TWmfCoordF oTR;
		TWmfCoordF oBL;
		TWmfCoordF oBR;
	} oBBox;

	char           *sText;

	unsigned short  ushFlags;

	double          dFontHeight;
	double          dFontRatio;  // Отношение ширины к высоте
};

struct TWmfUserData	
{
	TWmfDC *pDC;
	void   *pData;
};

struct TWmfFunctionReference
{
	void (*Device_Open )();
	void (*Device_Close)();
	void (*Device_Begin)();
	void (*Device_End  )();

	void (*Flood_Interior) (TWmfFlood*);
	void (*Flood_Exterior) (TWmfFlood*);

	void (*Draw_Pixel      ) (TWmfDrawPixel*);
	void (*Draw_Pie        ) (TWmfDrawArc*);
	void (*Draw_Chord      ) (TWmfDrawArc*);
	void (*Draw_Arc        ) (TWmfDrawArc*);
	void (*Draw_Ellipse    ) (TWmfDrawArc*);
	void (*Draw_Line       ) (TWmfDrawLine*);
	void (*Poly_Line       ) (TWmfPolyLine*);
	void (*Draw_Polygon    ) (TWmfPolyLine*);
	void (*Draw_Polypolygon) (TWmfPolyPoly*);
	void (*Draw_Rectangle  ) (TWmfDrawRectangle*);

	void (*Rop_Draw) (TWmfROPDraw*);
	void (*Bmp_Draw) (TWmfBMPDraw*);
	void (*Bmp_Read) (TWmfBMPRead*);
	void (*Bmp_Free) (TWmfBMP*);

	void (*Draw_Text) (TWmfDrawText*);

	void (*UserData_Init) (TWmfUserData*);
	void (*UserData_Copy) (TWmfUserData*);
	void (*UserData_Set ) (TWmfUserData*);
	void (*UserData_Free) (TWmfUserData*);

	void (*Region_Frame) (TWmfPolyRectangle*);
	void (*Region_Paint) (TWmfPolyRectangle*);
	void (*Region_Clip ) (TWmfPolyRectangle*);
};

//---------------------------------------------------------------------------------------------------
// Player
//---------------------------------------------------------------------------------------------------

#define PLAYER_SCANNED          (1 << 0)
#define PLAYER_PLAY             (1 << 1)
#define PLAYER_TLBR_D_SET       (1 << 2)

struct TWmfCoord
{
	unsigned int unX;
	unsigned int unY;
};

struct TWmfRegion
{
	unsigned int   unSize;
	unsigned int   unNumRects;

	unsigned short ushType; // NULL, SIMPLE, COMPLEX

	TWmfRectF     *pRects;
	TWmfRectF      oExtents;
};

struct TWmfObject
{
	int            nType;
	union
	{	
		TWmfBrush  oBrush;
		TWmfPen	   oPen;
		TWmfFont   oFont;

		int        nPalette;

		TWmfRegion oRegion;
	} uObject;
};

struct TWmfPlayer
{
	TWmfPen        oDefaultPen;
	TWmfBrush      oDefaultBrush;
	TWmfFont       oDefaultFont;

	TWmfDC        *pDC; // Текущее DC

	int            nDCStackMaxLen;
	int            nDCStackLength;
	TWmfDC       **ppDCStack;

	TWmfObject    *pObjects;

	TWmfRegion     oVisible; /* I don't understand this... what is the purpose of `visible'? */

	TWmfCoord      oCurrent; /* Current position */

	TWmfCoordF     oDisplayTL;    /* Bounding box from Display perspective */
	TWmfCoordF     oDisplayBR;

	TWmfCoordF     oBoundTL;
	TWmfCoordF     oBoundBR;

	TWmfCoordF     oViewportOrigin; /* Origin of Viewport */

	unsigned int   unViewportWidth; /* Display extents	 */
	unsigned int   unViewportHeight;

	unsigned char *pParameters; /* meta file parameter values */

	unsigned int  ulFlags;

	struct
	{
		int nX;
		int nY;
		int nWidth;
		int nHeight;
	} oWindow;
};

struct TWmfRecord
{	
	unsigned int  ulSize;
	unsigned int   unFunction;
	unsigned char *sParameter;
	int           lPosition;
};

//---------------------------------------------------------------------------------------------------
// Device Indepent Bitmap
//---------------------------------------------------------------------------------------------------

enum EWmfBitCount
{
	BI_BITCOUNT_0 = 0x0000,
	BI_BITCOUNT_1 = 0x0001,
	BI_BITCOUNT_2 = 0x0004,
	BI_BITCOUNT_3 = 0x0008,
	BI_BITCOUNT_4 = 0x0010,
	BI_BITCOUNT_5 = 0x0018,
	BI_BITCOUNT_6 = 0x0020
};

//--------------------------------------------------------------------------------------------------
// From wingdi.h
//--------------------------------------------------------------------------------------------------
/* PolyFill() Modes */
#define ALTERNATE                    1
#define WINDING                      2
#define POLYFILL_LAST                2

/* Background Modes */
#define TRANSPARENT         1
#define OPAQUE              2
#define BKMODE_LAST         2

/* Brush Styles */
#define BS_SOLID            0
#define BS_NULL             1
#define BS_HOLLOW           BS_NULL
#define BS_HATCHED          2
#define BS_PATTERN          3
#define BS_INDEXED          4
#define BS_DIBPATTERN       5
#define BS_DIBPATTERNPT     6
#define BS_PATTERN8X8       7
#define BS_DIBPATTERN8X8    8
#define BS_MONOPATTERN      9

/* Hatch Styles */
#define HS_HORIZONTAL       0       /* ----- */
#define HS_VERTICAL         1       /* ||||| */
#define HS_FDIAGONAL        2       /* \\\\\ */
#define HS_BDIAGONAL        3       /* ///// */
#define HS_CROSS            4       /* +++++ */
#define HS_DIAGCROSS        5       /* xxxxx */

/* Pen Styles */
#define PS_SOLID            0
#define PS_DASH             1       /* -------  */
#define PS_DOT              2       /* .......  */
#define PS_DASHDOT          3       /* _._._._  */
#define PS_DASHDOTDOT       4       /* _.._.._  */
#define PS_NULL             5
#define PS_INSIDEFRAME      6
#define PS_USERSTYLE        7
#define PS_ALTERNATE        8
#define PS_STYLE_MASK       0x0000000F

#define PS_ENDCAP_ROUND     0x00000000
#define PS_ENDCAP_SQUARE    0x00000100
#define PS_ENDCAP_FLAT      0x00000200
#define PS_ENDCAP_MASK      0x00000F00

#define PS_JOIN_ROUND       0x00000000
#define PS_JOIN_BEVEL       0x00001000
#define PS_JOIN_MITER       0x00002000
#define PS_JOIN_MASK        0x0000F000

#define PS_COSMETIC         0x00000000
#define PS_GEOMETRIC        0x00010000
#define PS_TYPE_MASK        0x000F0000

/* Text Alignment Options */
#define TA_NOUPDATECP                0
#define TA_UPDATECP                  1

#define TA_LEFT                      0
#define TA_RIGHT                     2
#define TA_CENTER                    6

#define TA_TOP                       0
#define TA_BOTTOM                    8
#define TA_BASELINE                  24

/* Binary raster ops */
#define R2_BLACK            1   /*  0       */
#define R2_NOTMERGEPEN      2   /* DPon     */
#define R2_MASKNOTPEN       3   /* DPna     */
#define R2_NOTCOPYPEN       4   /* PN       */
#define R2_MASKPENNOT       5   /* PDna     */
#define R2_NOT              6   /* Dn       */
#define R2_XORPEN           7   /* DPx      */
#define R2_NOTMASKPEN       8   /* DPan     */
#define R2_MASKPEN          9   /* DPa      */
#define R2_NOTXORPEN        10  /* DPxn     */
#define R2_NOP              11  /* D        */
#define R2_MERGENOTPEN      12  /* DPno     */
#define R2_COPYPEN          13  /* P        */
#define R2_MERGEPENNOT      14  /* PDno     */
#define R2_MERGEPEN         15  /* DPo      */
#define R2_WHITE            16  /*  1       */
#define R2_LAST             16

/* Ternary raster operations */
#define SRCCOPY             (DWORD)0x00CC0020 /* dest = source                   */
#define SRCPAINT            (DWORD)0x00EE0086 /* dest = source OR dest           */
#define SRCAND              (DWORD)0x008800C6 /* dest = source AND dest          */
#define SRCINVERT           (DWORD)0x00660046 /* dest = source XOR dest          */
#define SRCERASE            (DWORD)0x00440328 /* dest = source AND (NOT dest )   */
#define NOTSRCCOPY          (DWORD)0x00330008 /* dest = (NOT source)             */
#define NOTSRCERASE         (DWORD)0x001100A6 /* dest = (NOT src) AND (NOT dest) */
#define MERGECOPY           (DWORD)0x00C000CA /* dest = (source AND pattern)     */
#define MERGEPAINT          (DWORD)0x00BB0226 /* dest = (NOT source) OR dest     */
#define PATCOPY             (DWORD)0x00F00021 /* dest = pattern                  */
#define PATPAINT            (DWORD)0x00FB0A09 /* dest = DPSnoo                   */
#define PATINVERT           (DWORD)0x005A0049 /* dest = pattern XOR dest         */
#define DSTINVERT           (DWORD)0x00550009 /* dest = (NOT dest)               */
#define BLACKNESS           (DWORD)0x00000042 /* dest = BLACK                    */
#define WHITENESS           (DWORD)0x00FF0062 /* dest = WHITE                    */

/* Object Definitions for EnumObjects() */
#define OBJ_PEN             1
#define OBJ_BRUSH           2
#define OBJ_DC              3
#define OBJ_METADC          4
#define OBJ_PAL             5
#define OBJ_FONT            6
#define OBJ_BITMAP          7
#define OBJ_REGION          8
#define OBJ_METAFILE        9
#define OBJ_MEMDC           10
#define OBJ_EXTPEN          11
#define OBJ_ENHMETADC       12
#define OBJ_ENHMETAFILE     13
#define OBJ_COLORSPACE      14

#define ANSI_CHARSET            0
#define DEFAULT_CHARSET         1
#define SYMBOL_CHARSET          2
#define SHIFTJIS_CHARSET        128
#define HANGEUL_CHARSET         129
#define HANGUL_CHARSET          129
#define GB2312_CHARSET          134
#define CHINESEBIG5_CHARSET     136
#define OEM_CHARSET             255

#define JOHAB_CHARSET           130
#define HEBREW_CHARSET          177
#define ARABIC_CHARSET          178
#define GREEK_CHARSET           161
#define TURKISH_CHARSET         162
#define VIETNAMESE_CHARSET      163
#define THAI_CHARSET            222
#define EASTEUROPE_CHARSET      238
#define RUSSIAN_CHARSET         204

#define MAC_CHARSET             77
#define BALTIC_CHARSET          186

/* constants for the biCompression field */
#define BI_RGB        0L
#define BI_RLE8       1L
#define BI_RLE4       2L
#define BI_BITFIELDS  3L
#define BI_JPEG       4L
#define BI_PNG        5L

/* Metafile Functions */
#define META_SETBKCOLOR              0x0201
#define META_SETBKMODE               0x0102
#define META_SETMAPMODE              0x0103
#define META_SETROP2                 0x0104
#define META_SETRELABS               0x0105
#define META_SETPOLYFILLMODE         0x0106
#define META_SETSTRETCHBLTMODE       0x0107
#define META_SETTEXTCHAREXTRA        0x0108
#define META_SETTEXTCOLOR            0x0209
#define META_SETTEXTJUSTIFICATION    0x020A
#define META_SETWINDOWORG            0x020B
#define META_SETWINDOWEXT            0x020C
#define META_SETVIEWPORTORG          0x020D
#define META_SETVIEWPORTEXT          0x020E
#define META_OFFSETWINDOWORG         0x020F
#define META_SCALEWINDOWEXT          0x0410
#define META_OFFSETVIEWPORTORG       0x0211
#define META_SCALEVIEWPORTEXT        0x0412
#define META_LINETO                  0x0213
#define META_MOVETO                  0x0214
#define META_EXCLUDECLIPRECT         0x0415
#define META_INTERSECTCLIPRECT       0x0416
#define META_ARC                     0x0817
#define META_ELLIPSE                 0x0418
#define META_FLOODFILL               0x0419
#define META_PIE                     0x081A
#define META_RECTANGLE               0x041B
#define META_ROUNDRECT               0x061C
#define META_PATBLT                  0x061D
#define META_SAVEDC                  0x001E
#define META_SETPIXEL                0x041F
#define META_OFFSETCLIPRGN           0x0220
#define META_TEXTOUT                 0x0521
#define META_BITBLT                  0x0922
#define META_STRETCHBLT              0x0B23
#define META_POLYGON                 0x0324
#define META_POLYLINE                0x0325
#define META_ESCAPE                  0x0626
#define META_RESTOREDC               0x0127
#define META_FILLREGION              0x0228
#define META_FRAMEREGION             0x0429
#define META_INVERTREGION            0x012A
#define META_PAINTREGION             0x012B
#define META_SELECTCLIPREGION        0x012C
#define META_SELECTOBJECT            0x012D
#define META_SETTEXTALIGN            0x012E
#define META_CHORD                   0x0830
#define META_SETMAPPERFLAGS          0x0231
#define META_EXTTEXTOUT              0x0a32
#define META_SETDIBTODEV             0x0d33
#define META_SELECTPALETTE           0x0234
#define META_REALIZEPALETTE          0x0035
#define META_ANIMATEPALETTE          0x0436
#define META_SETPALENTRIES           0x0037
#define META_POLYPOLYGON             0x0538
#define META_RESIZEPALETTE           0x0139
#define META_DIBBITBLT               0x0940
#define META_DIBSTRETCHBLT           0x0b41
#define META_DIBCREATEPATTERNBRUSH   0x0142
#define META_STRETCHDIB              0x0f43
#define META_EXTFLOODFILL            0x0548
#define META_SETLAYOUT               0x0149
#define META_DELETEOBJECT            0x01f0
#define META_CREATEPALETTE           0x00f7
#define META_CREATEPATTERNBRUSH      0x01F9
#define META_CREATEPENINDIRECT       0x02FA
#define META_CREATEFONTINDIRECT      0x02FB
#define META_CREATEBRUSHINDIRECT     0x02FC
#define META_CREATEREGION            0x06FF

namespace MetaFile
{
#define META_EOF 0x0000
	struct TWmfColor
	{
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a; //Reserved Must be 0x00

		TWmfColor()
		{
			r = 0;
			g = 0;
			b = 0;
		}

		TWmfColor(unsigned char _r, unsigned char _g, unsigned char _b)
		{
			r = _r;
			g = _g;
			b = _b;
		}

		void Set(unsigned char _r, unsigned char _g, unsigned char _b)
		{
			r = _r;
			g = _g;
			b = _b;
		}

		void Init()
		{
			r = 0;
			g = 0;
			b = 0;
			a = 0;
		}

		void Copy(TWmfColor* pOther)
		{
			r = pOther->r;
			g = pOther->g;
			b = pOther->b;
			a = pOther->a;
		}

		TWmfColor& operator=(TWmfColor& oColor)
		{
			r = oColor.r;
			g = oColor.g;
			b = oColor.b;
			a = oColor.a;
			return *this;
		}
	};
	struct TWmfPaletteEntry
	{
		unsigned char Values;
		unsigned char Blue;
		unsigned char Green;
		unsigned char Red;
	};
	struct TWmfPointS
	{
		short x;
		short y;
	};
	struct TWmfRect
	{
		short Left;
		short Top;
		short Right;
		short Bottom;
	};
	struct TWmfPlaceable
	{
		unsigned int   Key;
		unsigned short HWmf;
		TWmfRect       BoundingBox;
		unsigned short Inch;
		unsigned int   Reserved;
		unsigned short Checksum;
	};
	struct TWmfHeader
	{
		unsigned short Type;
		unsigned short HeaderSize;
		unsigned short Version;
		unsigned int   Size;
		unsigned short NumberOfObjects;
		unsigned int   MaxRecord;
		unsigned short NumberOfMembers;
	};
	struct TWmfScanLine
	{
		unsigned short Left;
		unsigned short Right;
	};
	struct TWmfScanObject
	{
		unsigned short  Count;
		unsigned short  Top;
		unsigned short  Bottom;
		TWmfScanLine*   ScanLines;
		unsigned short  Count2;
	};
	struct TWmfWindow
	{
		short x;
		short y;
		short w;
		short h;

		void Init()
		{
			x = 0;
			y = 0;
			w = 1024;
			h = 1024;
		}

		void Copy(TWmfWindow& oOther)
		{
			x = oOther.x;
			y = oOther.y;
			w = oOther.w;
			h = oOther.h;
		}
	};
	struct TWmfLogBrush
	{
		unsigned short BrushStyle;
		TWmfColor      Color;
		unsigned short BurshHatch;
	};
}

#endif /* _WMF_TYPES_H_ */