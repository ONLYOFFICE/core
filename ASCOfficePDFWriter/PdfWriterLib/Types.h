#ifndef _TYPES_H
#define _TYPES_H

#pragma once

#include "Consts.h"
//----- Основные перечисления и структуры------------------------------------------------------

//#ifndef BOOL
//
//	typedef int BOOL;
//	#define TRUE  1
//	#define FALSE 0
//
//#endif

//#ifndef BYTE
//	typedef unsigned long DWORD;
//	typedef unsigned char BYTE;
//#endif

// Point структура
typedef  struct StPoint 
{
    float fX;
    float fY;
} TPoint;

// Rect структура
typedef  struct StRect 
{
    float  fLeft;
    float  fBottom;
    float  fRight;
    float  fTop;
} TRect;

typedef struct StRect Box;

// Date структура
typedef struct StDate 
{
    int    nYear;
    int    nMonth;
    int    nDay;
    int    nHour;
    int    nMinutes;
    int    nSeconds;
    char   nInd;
    int    nOffHour;
    int    nOffMinutes;
} TDate;

typedef struct StFontEncodingPair
{
	char sPdfFontName[LIMIT_MAX_NAME_LEN + 1];
	char sRealFontName[LIMIT_MAX_NAME_LEN + 1];
	char sEncodingName[LIMIT_MAX_NAME_LEN + 1];
} TFontEncodingPair;

typedef enum EInfoType 
{
    // Данные относительно даты создания и изменения.
    InfoCreationDate = 0,
    InfoModaDate,

    // Текстовые данные
    InfoAuthor,
    InfoCreator,
    InfoProducer,
    InfoTitle,
    InfoSubject,
    InfoKeyWords,
    InfoEOF
} InfoType;

typedef enum EPdfVer 
{
    PDF_1_2 = 0,
    PDF_1_3,
    PDF_1_4,
    PDF_1_5,
    PDF_1_6,
    PDF_EOF
} PDFVer;

typedef enum  EEncryptMode 
{
    EncryptR2    = 2,
    EncryptR3    = 3
} EncryptMode;

typedef void  (*ErrorHandler) (unsigned long nErrorNo, unsigned long nDetailNo, void *pUserData);
typedef void* (*AllocFunc)    (unsigned int nSize);
typedef void  (*FreeFunc)     (void  *pPointer);

// Структура для измерения ширины текста

typedef struct TTextWidth 
{
    unsigned int nNumchars;
    unsigned int nNumwords;

    unsigned int nWidth;
    unsigned int nNumspace;
} TextWidth;

// Структура для определения параметров пунктирных линий

typedef struct TDashMode 
{
    float        afPtn[8];
    unsigned int nNumPoints;
    float        fPhase;
} DashMode;

// TransMatrix структура
// Вид матрицы:
// | a b 0 |
// | c d 0 |
// | x y 1 |

typedef struct TTransMatrix 
{
    float   fA;
    float   fB;
    float   fC;
    float   fD;
    float   fX;
    float   fY;
} TransMatrix;


//---------------------------------------------------------------------------

typedef enum EColorSpace 
{
    CSDeviceGray = 0,
    CSDeviceRGB,
    CSDeviceCMYK,
    CSCalGray,
    CSCalRGB,
    CSLAB,
    CSICCBASED,
    CSSeparation,
    CSDeviceN,
    CSIndexed,
    CSPattern,
    CSEOF
} ColorSpace;

// RGB структура

typedef struct TRGBColor 
{
    float   r;
    float   g;
    float   b;

public:

	bool operator ==(const TRGBColor& oRight)const
	{
		if ( r == oRight.r && g == oRight.g && b == oRight.b )
			return true;

		return false;
	}

} RGBColor;

// CMYK структура

typedef struct TCMYKColor 
{
    float   c;
    float   m;
    float   y;
    float   k;
} CMYKColor;

// Тип окончания линии

typedef enum ELineCapStyle 
{
    ButtEnd = 0,
    RoundEnd,
    ProjectingSquareEnd,
    LineCapEOF
} LineCapStyle;

// Тип соединения линий

typedef enum ELineJoinStyle
{
    MiterJoin = 0,
    RoundJoin,
    BevelJoin,
    LineJoinEOF
} LineJoinStyle;

// Способ заполнения букв

typedef enum ETextRenderingMode 
{
    Fill = 0,
    Stroke,
    FillThenStroke,
    Invisible,
    FillClipping,
    StrokeClipping,
    FillStrokeClipping,
    Clipping,
    RenderingModeEOF
} TextRenderingMode;

typedef enum EWritingMode 
{
    WModeHorizontal = 0,
    WModeVertical,
    WModeEOF
} WritingMode;

typedef enum EPageLayout 
{
    PLSingle = 0,
    PLOneColumn,
    PLTwoColumnLeft,
    PLTwoCoulmnRight,
    PLEOF
} PageLayout;

typedef enum EPageMode 
{
    PageModeUseNone = 0,
    PageModeUseOutline,
    PageModeUseThumbs,
    PageModeFullScreen,
/*  PageModeUseOC,
    PageModeUseAttachments,
 */
    PageModeEOF
} PageMode;

typedef enum EPageNumStyle 
{
    PageNumStyleDecimal = 0,
    PageNumStyleUpperRoman,
    PageNumStyleLowerRoman,
    PageNumStyleUpperLetters,
    PageNumStyleLowerLetters,
    PageNumStyleEOF
} PageNumStyle;

typedef enum EDestinationType 
{
    XYZ = 0,
    Fit,
    FitH,
    FitV,
    FitR,
    FitB,
    FitBH,
    FitBV,
    DstEOF
} DestinationType;

typedef enum EAnnotType
{
    AnnotTextNotes,
    AnnotLink,
    AnnotSound,
    AnnotFreeText,
    AnnotStamp,
    AnnotSquare,
    AnnotCircle,
    AnnotStrikeOut,
    AnnotHighLight,
    AnnotUnderline,
    AnnotInk,
    AnnotFileAttachment,
    AnnotPopup
} AnnotType;

typedef enum EAnnotFlgs 
{
    AnnotInvisible,
    AnnotHidden,
    AnnotPrint,
    AnnotNoZoom,
    AnnotNoRotate,
    AnnotNoView,
    AnnotReadOnly
} AnnotFlgs;

typedef enum EAnnotHighlightMode 
{
    AnnotNoHighlight = 0,
    AnnotInvertBox,
    AnnotInvertBorder,
    AnnotDownAppearance,
    AnnotHighlightModeEOF
} AnnotHighlightMode;

typedef enum EAnnotIcon 
{
    AnnotIconComment = 0,
    AnnotIconKey,
    AnnotIconNote,
    AnnotIconHelp,
    AnnotIconNewParagraph,
    AnnotIconParagraph,
    AnnotIconInsert,
    AnnotIconEOF
} AnnotIcon;

// Borderd style

typedef enum EBSSubtype 
{
    BSSolid,
    BSDashed,
    BSBeveled,
    BSInset,
    BSUnderlined
} BSSubtype;

// Blend modes

typedef enum EBlendMode 
{
    BMNormal,
    BMMultiply,
    BMScreen,
    BMOverlay,
    BMDarken,
    BMLighten,
    BMColorDodge,
    BMColorBum,
    BMHardLight,
    BMSoftLight,
    BMDifference,
    BMExclusion,
    BMEOF
} BlendMode;

// Варианты перехода в слайдшоу

typedef enum ETransitionStyle 
{
    TSWipeRight = 0,
    TSWipeUp,
    TSWipeLeft,
    TSWipeDown,
    TSBarnDoorsHorizontalOut,
    TSBarnDoorsHorizontalIn,
    TSBarnDoorsVerticalOut,
    TSBarnDoorsVerticalIn,
    TSBoxOut,
    TSBoxIn,
    TSBlindsHorizontal,
    TSBlindsVertical,
    TSDissolve,
    TSGlitterRight,
    TSGlitterDown,
    TSGlitterTopLeftToBottomRight,
    TSReplace,
    TSEOF
} TransitionStyle; 

// Предопределенный размеры страниц

typedef enum EPageSizes 
{
    PageSizeLETTER = 0,
    PageSizeLEGAL,
    PageSizeA3,
    PageSizeA4,
    PageSizeA5,
    PageSizeB4,
    PageSizeB5,
    PageSizeEXECUTIVE,
    PageSizeUS4x6,
    PageSizeUS4x8,
    PageSizeUS5x7,
    PageSizeCOMM10,
    PageSizeEOF
} PageSizes;

// Расположение страницы

typedef enum EPageDirection
{
    PagePortrait = 0,
    PageLandscape
} PageDirection;

// Типы кодировок

typedef enum EEncoderType 
{
    EncoderTypeSingleByte,
    EncoderTypeDoubleByteBuilt,   // Для CID шрифтов, когда кодировку строим сами
	EncoderTypeDoubleByteStream,  // Для CID шрифтов, когда кодировка нам задана, и она используется только для записи
    EncoderTypeUninitialized,
    EncoderTypeUnknown, 
	EncoderTypeToUnicode
} EncoderType;


typedef enum EByteType 
{
    ByteTypeSingle = 0,
    ByteTypeLead,
    ByteTypeTrial,
    ByteTypeUnknown
} ByteType;

// Типы прилегания текста

typedef enum ETextAlignment 
{
    TAlignLeft = 0,
    TAlignRight,
    TAlignCenter,
    TAlignJustify
} TextAlignment;

#endif /* _TYPES_H */