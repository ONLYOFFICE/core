#pragma once

namespace Global
{
	static const WCHAR ColorIdentifier[17][12] = 
	{
		_T( "auto" ),
		_T( "black" ),
		_T( "blue" ),
		_T( "cyan" ),
		_T( "green" ),
		_T( "magenta" ),
		_T( "red" ),
		_T( "yellow" ),
		_T( "white" ),
		_T( "darkBlue" ),
		_T( "darkCyan" ),
		_T( "darkGreen" ),
		_T( "darkMagenta" ),
		_T( "darkRed" ),
		_T( "darkYellow" ),
		_T( "darkGray" ),
		_T( "lightGray" )
	};

	static const WCHAR UnderlineCode[56][16] =
	{
		_T( "none" ),
		_T( "single" ),
		_T( "words" ),
		_T( "double" ),
		_T( "dotted" ),
		_T( "notUsed1" ),
		_T( "thick" ),
		_T( "dash" ),
		_T( "notUsed2" ),
		_T( "dotDash" ),
		_T( "dotDotDash" ),
		_T( "wave" ),
		_T( "notUsed3" ),
		_T( "notUsed4" ),
		_T( "notUsed5" ),
		_T( "notUsed6" ),
		_T( "notUsed7" ),
		_T( "notUsed8" ),
		_T( "notUsed9" ),
		_T( "notUsed10" ),
		_T( "dottedHeavy" ),
		_T( "notUsed11" ),
		_T( "notUsed12" ),
		_T( "dashedHeavy" ),
		_T( "notUsed13" ),
		_T( "dashDotHeavy" ),
		_T( "dashDotDotHeavy" ),
		_T( "wavyHeavy" ),
		_T( "notUsed14" ),
		_T( "notUsed15" ),
		_T( "notUsed16" ),
		_T( "notUsed17" ),
		_T( "notUsed18" ),
		_T( "notUsed19" ),
		_T( "notUsed20" ),
		_T( "notUsed21" ),
		_T( "notUsed22" ),
		_T( "notUsed23" ),
		_T( "notUsed24" ),
		_T( "dashLong" ),
		_T( "notUsed25" ),
		_T( "notUsed26" ),
		_T( "notUsed27" ),
		_T( "wavyDouble" ),
		_T( "notUsed28" ),
		_T( "notUsed29" ),
		_T( "notUsed30" ),
		_T( "notUsed31" ),
		_T( "notUsed32" ),
		_T( "notUsed33" ),
		_T( "notUsed34" ),
		_T( "notUsed35" ),
		_T( "notUsed36" ),
		_T( "notUsed37" ),
		_T( "notUsed38" ),
		_T( "dashLongHeavy" )
	};

	static const WCHAR TabLeader[8][11] =
	{
		_T( "none" ),
		_T( "dot" ),
		_T( "hyphen" ),
		_T( "underscore" ),
		_T( "heavy" ),
		_T( "middleDot" ),
		_T( "none" ),
		_T( "none" )
	};

	static const WCHAR TabStop[7][8] =
	{
		_T( "left" ),
		_T( "center"),
		_T( "right" ),
		_T( "decimal" ),
		_T( "bar" ),
		_T( "clear" ),
		_T( "num" )
	};

	static const WCHAR TextAnimation[7][16] =
	{
		_T( "none" ),
		_T( "lights" ),
		_T( "blinkBackground" ),
		_T( "sparkle" ),
		_T( "antsBlack" ),
		_T( "antsRed" ),
		_T( "shimmer" )
	};

	static const WCHAR JustificationCode[10][15] =
	{
		_T( "left" ),
		_T( "center" ),
		_T( "right" ),
		_T( "both" ),
		_T( "distribute" ),
		_T( "mediumKashida" ),
		_T( "numTab" ),
		_T( "highKashida" ),
		_T( "lowKashida" ),
		_T( "thaiDistribute" )
	};

	static const WCHAR VerticalPositionCode[4][7] =
	{
		_T( "margin" ),
		_T( "page" ),
		_T( "text" ),
		_T( "none" )
	};

	static const WCHAR HorizontalPositionCode[4][7] =
	{
		_T( "text" ),
		_T( "margin" ),
		_T( "page" ),
		_T( "none" )
	};

	static const WCHAR TextFrameWrapping[6][10] =
	{
		_T( "auto" ),
		_T( "notBeside" ),
		_T( "around" ),
		_T( "none" ),
		_T( "tight" ),
		_T( "through" )
	};

	static const WCHAR TextFrameDropCapLocation[3][7] =
	{
		_T( "none" ),
		_T( "drop" ),
		_T( "margin" )
	};

	typedef enum _DashStyle
	{
		dashStyleSolid,
		dashStyleShortdash,
		dashStyleShortdot,
		dashStyleShortdashdot,
		dashStyleShortdashdotdot,
		dashStyleDot,
		dashStyleDash,
		dashStyleLongdash,
		dashStyleDashdot,
		dashStyleLongdashdot,
		dashStyleLongdashdotdot
	}DashStyle;

	static const WCHAR DashStyleMap[11][16] =
	{
		_T( "solid" ),
		_T( "shortdash" ),
		_T( "shortdot" ),
		_T( "shortdashdot" ),
		_T( "shortdashdotdot" ),
		_T( "dot" ),
		_T( "dash" ),
		_T( "longdash" ),
		_T( "dashdot" ),
		_T( "longdashdot" ),
		_T( "longdashdotdot" )
	};

	typedef enum _TextFlow
	{
		lrTb = 0,
		tbRl = 1,
		btLr = 3,
		lrTbV = 4,
		tbRlV = 5,
	} TextFlow;

	static const WCHAR TextFlowMap[5][6] =
	{
		_T( "lrTb" ),
		_T( "tbRl" ),
		_T( "btLr" ),
		_T( "lrTbV" ),
		_T( "tbRlV" ),
	};

	typedef enum _VerticalMergeFlag
	{
		fvmClear = 0,
		fvmMerge = 1,
		fvmRestart = 3
	} VerticalMergeFlag;

	typedef enum _VerticalAlign
	{ 
		top,
		center,
		bottom
	} VerticalAlign;

	static const WCHAR VerticalAlignMap[3][7] =
	{
		_T( "top" ),
		_T( "center" ),
		_T( "bottom" )
	};

	typedef enum _CellWidthType
	{
		nil,
		Auto,
		pct,
		dxa
	} CellWidthType;

	static const WCHAR CellWidthTypeMap[4][5] =
	{
		_T( "nil" ),
		_T( "auto" ),
		_T( "pct" ),
		_T( "dxa" )
	};

	typedef enum _BlipUsage
	{
		msoblipUsageDefault,
		msoblipUsageTexture,
		msoblipUsageMax = 255
	} BlipUsage;

	typedef enum _BlipType
	{
		msoblipERROR = 0, // An error occured during loading 
		msoblipUNKNOWN, // An unknown blip type 
		msoblipEMF, // Windows Enhanced Metafile 
		msoblipWMF, // Windows Metafile 
		msoblipPICT, // Macintosh PICT 
		msoblipJPEG, // JFIF 
		msoblipPNG, // PNG or GIF 
		msoblipDIB, // Windows DIB 
		msoblipTIFF = 17, // TIFF 
		msoblipCMYKJPEG = 18,// JPEG data in YCCK or CMYK color space 
		msoblipFirstClient = 32, // First client defined blip type 
		msoblipLastClient = 255 // Last client defined blip type 
	} BlipType;

	typedef enum _BlipSignature
	{
		msobiUNKNOWN = 0,
		msobiWMF = 0x216, // Metafile header then compressed WMF 
		msobiEMF = 0x3D4, // Metafile header then compressed EMF 
		msobiPICT = 0x542, // Metafile header then compressed PICT 
		msobiPNG = 0x6E0, // One byte tag then PNG data 
		msobiJPEG = 0x46A,
		msobiJFIF = msobiJPEG, // One byte tag then JFIF data 
		msobiDIB = 0x7A8, // One byte tag then DIB data 
		msobiCMYKJPEG = 0x6E2, // One byte tag then CMYK/YCCK JPEG data 
		msobiTIFF = 0x6e4, // One byte tag then TIFF data 
		msobiClient = 0x800, // Clients should set this bit 
	} BlipSignature;

	typedef enum _BlipFilter
	{
		msofilterAdaptive = 0,
		msofilterNone = 254,
		msofilterTest = 255
	} BlipFilter;
}