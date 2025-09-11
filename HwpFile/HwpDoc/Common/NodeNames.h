#ifndef NODENAMES_H
#define NODENAMES_H

#include "../HanType.h"

namespace HWP
{
enum class ENode
{
	//HEAD
	BeginNum,
	FaceNameList,
		FontFace,
			Font,
				SubFont,
				TypeInfo,
	BorderFillList,
		BorderFill,
			LeftBorder,
			RightBorder,     //10
			TopBorder,
			BottomBorder,
			Diagonal,
			FillBrush,
				WindowBrush,
				Gradation,
					Color,
				ImageBrush,
					Image,
	CharShapeList,           //20
		CharShape,
			FontId,
			Ratio,
			CharSpacing,
			RelSize,
			CharOffset,
			Italic,
			Bold,
			Underline,
			Outline,         //30
			Shadow,
			Emboss,
			Engrave,
			SuperScript,
			SubScript,
	TabDefList,
		TabDef,
			TabItem,
	NumberingList,
		Numbering,           //40
			ParaHead,
	BulletList,
		Bullet,
			//ParaHead
	ParaShapeList,
		ParaShape,
			ParaMargin,
			ParaBorder,
	StyleList,
		Style,

	//BODY
	Body,                    //50
	Section,
	Paragraph,
	Text,
	Char,
	SectionDef,
	Table,
	Picture,
	Container,
	Ole,
	Equation,                //60
	TextArt,
	Line,
	Rectangle,
	Ellipse,
	Arc,
	Polygon,
	Curve,
	ConnectLine,
	Tab,
	LineBreak,               //70
	Hyphen,
	NbSpace,
	FwSpace,

	//Only hwpx
	Ctrl,
	Lineseg,
	LinesegArray,
	Video,
	Switch,
	Case,
	Default,
	Slash,
	BackSlash
};

#define MAX_TYPES 2
#define MAX_NODES 100

static constexpr const char* NODE_NAMES[MAX_TYPES][MAX_NODES] = 
{
// HWPX
	{
		//HEAD
		"hh:beginNum",
		"hh:fontfaces",
			"hh:fontface",
				"hh:font",
					"hh:substFont",
					"hh:typeInfo",
		"hh:borderFills",
			"hh:borderFill",
				"hh:leftBorder",
				"hh:rightBorder",     //10
				"hh:topBorder",
				"hh:bottomBorder",
				"hh:diagonal",
				"hc:fillBrush",
					"hc:winBrush",
					"hc:gradation",
						"hc:color",
					"hc:imgBrush",
						"hc:img",
		"h:charProperties",           //20
			"hh:charPr",
				"hh:fontRef",
				"hh:ratio",
				"hh:spacing",
				"hh:relSz",
				"hh:offset",
				"hh:italic",
				"hh:bold",
				"hh:underline",
				"hh:outline",         //30
				"hh:shadow",
				"hh:emboss",
				"hh:engrave",
				"hh:supscript",
				"hh:subscript",
		"hh:tabProperties",
			"hh:tabPr",
				"hh:tabPr",
		"hh:numberings",
			"hh:numbering",           //40
				"hh:paraHead",
		"hh:bullets",
			"hh:bullet",
				//"PARAHEAD",
		"hh:paraProperties",
			"hh:paraPr",
				"hh:margin",
				"hh:border",
		"hh:styles",
			"hh:style",

		//BODY
		"",                           //50
		"hs:sec",
		"hp:p",
		"hp:run",
		"hp:t",
		"hp:secPr",
		"hp:tbl",
		"hp:pic",
		"hp:container",
		"hp:ole",
		"hp:equation",                //60
		"hp:textart",
		"hp:line",
		"hp:rect",
		"hp:ellipse",
		"hp:arc",
		"hp:polygon",
		"hp:curve",
		"hp:connectLine",
		"hp:tab",
		"hp:lineBreak",               //70
		"hp:hyphen",
		"hp:nbSpace",
		"hp:fwSpace"

		"hp:ctrl",
		"hp:lineseg",
		"hp:linesegarray",
		"hp:video",
		"hp:switch",
		"hp:case",
		"hp:default"
	},
// HWPML
	{
		//HEAD
		"BEGINNUMBER",
		"FACENAMELIST",
			"FONTFACE",
				"FONT",
					"SUBSTFONT",
					"TYPEINFO",
		"BORDERFILLLIST",
			"BORDERFILL",
				"LEFTBORDER",
				"RIGHTBORDER",     //10
				"TOPBORDER",
				"BOTTOMBORDER",
				"DIAGONAL",
				"FILLBRUSH",
					"WINDOWBRUSH",
					"GRADATION",
						"COLOR",
					"IMAGEBRUSH",
						"IMAGE",
		"CHARSHAPELIST",           //20
			"CHARSHAPE",
				"FONTID",
				"RATIO",
				"CHARSPACING",
				"RELSIZE",
				"CHAROFFSET",
				"ITALIC",
				"BOLD",
				"UNDERLINE",
				"OUTLINE",         //30
				"SHADOW",
				"EMBOSS",
				"ENGRAVE",
				"SUPERSCRIPT",
				"SUBSCRIPT",
		"TABDEFLIST",
			"TABDEF",
				"TABITEM",
		"NUMBERINGLIST",
			"NUMBERING",           //40
				"PARAHEAD",
		"BULLETLIST",
			"BULLET",
				//"PARAHEAD",
		"PARASHAPELIST",
			"PARASHAPE",
				"PARAMARGIN",
				"PARABORDER",
		"STYLELIST",
			"STYLE",
		//BODY
		"BODY",                    //50
		"SECTION",
		"P",
		"TEXT",
		"CHAR",
		"SECDEF",
		"TABLE",
		"PICTURE",
		"CONTAINER",
		"OLE",
		"EQUATION",                //60
		"TEXTART",
		"LINE",
		"RECTANGLE",
		"ELLIPSE",
		"ARC",
		"POLYGON",
		"CURVE",
		"CONNECTLINE",
		"TAB",
		"LINEBREAK",               //70
		"HYPHEN",
		"NBSPACE",
		"FWSPACE",

		"",
		"",
		"",
		"",
		"",
		"",
		""
	}
};

inline const char* GetNodeName(ENode eNode, EHanType eType)
{
	switch (eType)
	{
		case EHanType::HWPX:  return NODE_NAMES[0][static_cast<int>(eNode)];
		case EHanType::HWPML: return NODE_NAMES[1][static_cast<int>(eNode)];
		default: return "";
	}
}

enum class EAttribute
{
	ParaShape,
	Style,
	CharShape,
	Href,
	SubPath,
	MediaType,
	Type,
	FontName,
	FamilyType,
	SerifStyle,     //10
	Weight,
	Proportion,
	Contrast,
	StrokeVariation,
	ArmStyle,
	Letterform,
	Midline,
	XHeight,
	ThreeD,
	Shadow,         //20
	Slash,
	BackSlash,
	CrookedSlash,
	CounterSlash,
	CounterBackSlash,
	BreakCellSeparateLine,
	Width,
	Color,
	FaceColor,
	HatchColor,     //30
	HatchStyle,
	Alpha,
	Angle,
	CenterX,
	CenterY,
	Step,
	ColorNum,
	StepCenter,
	Value,
	Mode,           //40
	Bright,
	Effect,
	BinItem,
	Height,
	TextColor,
	ShadeColor,
	UseFontSpace,
	UseKerning,
	SymMask,
	BorderFillId,   //50
	Shape,
	OffsetX,
	OffsetY,
	AutoTabLeft,
	AutoTabRight,
	Leader,
	Start,
	Level,
	Aligment,
	UseInstWidth,   //60
	AutoIndent,
	WidthAdjust,
	TextOffsetType,
	TextOffset,
	NumFormat,
	Char,
	Image,
	Align,
	VerAlign,
	HeadingType,    //70
	TabDef,
	BreakLatinWord,
	BreakNonLatinWord,
	Condense,
	WidowOrphan,
	KeepWithNext,
	KeepLines,
	PageBreakBefore,
	FontLineHeight,
	SnapToGrid,    //80
	LineWrap,
	AutoSpaceEAsianEng,
	AutoSpaceEAsianNum,
	Indent,
	Left,
	Right,
	Prev,
	Next,
	LineSpacingType,
	LineSpacing,   //90
	BorderFill,
	OffsetLeft,
	OffsetRight,
	OffsetTop,
	OffsetBottom,
	Connect,
	IgnoreMargin,
	EngName,
	NextStyle,
	LangId,        //100
	InstId,
	LockForm,
	Name,

	PageBreak,
	ColumnBreak,
};

#define MAX_ATTRIBUTES 110

//TODO:: добавить все аргументы
static constexpr const char* ATTRUBUTE_NAMES[MAX_TYPES][MAX_ATTRIBUTES] = 
{
// HWPX
	{
		"paraPrIDRef",
		"styleIDRef",
		"charPrIDRef",
		"href",
		"sub-path",
		"media-type",
		"type",
		"face",
		"familyType",
		"serifStyle",     //10
		"weight",
		"proportion",
		"contrast",
		"strokeVariation",
		"armStyle",
		"letterform",
		"midline",
		"xHeight",
		"threeD",
		"shadow",         //20
		"",
		"",
		"",
		"",
		"",
		"breakCellSeparateLine",
		"width",
		"color",
		"faceColor",
		"hatchColor",     //30
		"hatchStyle",
		"alpha",
		"angle",
		"centerX",
		"centerY",
		"step",
		"colorNum",
		"stepCenter",
		"value",
		"mode",           //40
		"bright",
		"effect",
		"binaryItemIDRef",
		"height",
		"textColor",
		"shadeColor",
		"useFontSpace",
		"useKerning",
		"symMark",
		"borderFillIDRef",//50
		"shape",
		"offsetX",
		"offsetY",
		"autoTabLeft",
		"autoTabRight",
		"leader",
		"start",
		"level",
		"align",
		"useInstWidth",   //60
		"autoIndent",
		"widthAdjust",
		"textOffsetType",
		"textOffset",
		"numFormat",
		"charPrIDRef",
		"",
		"",
		"",
		"",               //70
		"tabPrIDRef",
		"breakLatinWord",
		"breakNonLatinWord",
		"",
		"widowOrphan",
		"keepWithNext",
		"",
		"pageBreakBefore",
		"",
		"",               //80
		"LineWrap",
		"eAsianEng",
		"eAsianNum",
		"",
		"",
		"",
		"",
		"",
		"",
		"",               //90
		"borderFillIDRef",
		"offsetLeft",
		"offsetRight",
		"offsetTop",
		"offsetBottom",
		"connect",
		"ignoreMargin",
		"engName",
		"nextStyleIDRef",
		"langID",         //100
		""
		"lockForm",
		"name",

		"pageBreak",
		"columnBreak",
	},
// HWPML
	{
		"ParaShape",
		"Style",
		"CharShape",
		"APath",
		"RPath",
		"Format",
		"Type",
		"Name",
		"FamilyType",
		"SerifStyle",     //10
		"Weight",
		"Proportion",
		"Contrast",
		"StrokeVariation",
		"ArmStyle",
		"Letterform",
		"Midline",
		"XHeight",
		"ThreeD",
		"Shadow",         //20
		"Slash",
		"BackSlash",
		"CrookedSlash",
		"CounterSlash",
		"CounterBackSlash",
		"BreakCellSeparateLine",
		"Width",
		"Color",
		"FaceColor",
		"HatchColor",     //30
		"HatchStyle",
		"Alpha",
		"Angle",
		"CenterX",
		"CenterY",
		"Step",
		"ColorNum",
		"StepCenter",
		"Value",
		"Mode",           //40
		"Bright",
		"Effect",
		"BinItem",
		"Height",
		"TextColor",
		"ShadeColor",
		"UseFontSpace",
		"UseKerning",
		"SymMask",
		"BorderFillId",   //50
		"Shape",
		"OffsetX",
		"OffsetY",
		"AutoTabLeft",
		"AutoTabRight",
		"Leader",
		"Start",
		"Level",
		"Aligment",
		"UseInstWidth",   //60
		"AutoIndent",
		"WidthAdjust",
		"TextOffsetType",
		"TextOffset",
		"NumFormat",
		"Char",
		"Image",
		"Align",
		"VerAlign",
		"HeadingType",    //70
		"TabDef",
		"BreakLatinWord",
		"BreakNonLatinWord",
		"Condense",
		"WidowOrphan",
		"KeepWithNext",
		"KeepLines",
		"PageBreakBefore",
		"FontLineHeight",
		"SnapToGrid",     //80
		"LineWrap",
		"AutoSpaceEAsianEng",
		"AutoSpaceEAsianNum",
		"Indent",
		"Left",
		"Right",
		"Prev",
		"Next",
		"LineSpacingType",
		"LineSpacing",    //90
		"BorderFill",
		"OffsetLeft",
		"OffsetRight",
		"OffsetTop",
		"OffsetBottom",
		"Connect",
		"IgnoreMargin",
		"EngName",
		"NextStyle",
		"LangId",         //100
		"InstId",
		"LockForm",
		"Name",

		"PageBreak",
		"ColumnBreak"
	}
};

inline const char* GetAttributeName(EAttribute eNode, EHanType eType)
{
	switch (eType)
	{
		case EHanType::HWPX:  return ATTRUBUTE_NAMES[0][static_cast<int>(eNode)];
		case EHanType::HWPML: return ATTRUBUTE_NAMES[1][static_cast<int>(eNode)];
		default: return "";
	}
}

#define MAX_VALUES 40

// TODO:: пока всё в одном месте. Возможно стоит лучше разделить по соответствующим классам
enum class EValue
{
	None,
	Center,
	CenterBelow,
	CenterAbove,
	All,
	RealPic,
	GrayScale,
	BlackWhite,
	Drop,
	Continuous,            //10
	Left,
	Right,
	Percent,
	HwpUnit,
	Digit,
	HangulSyllable,
	HangulJamo,
	CircledDigit,
	LatinSmall,
	CircledHangulSyllable, //20
	CircledHangulJamo,
	RomanSmall,
	Para,
	Char
};

static constexpr const char* VALUE_NAMES[MAX_TYPES][MAX_VALUES] = 
{
//HWPX
	{
		"NONE",
		"CENTER",
		"CENTER_BELOW",
		"CENTER_ABOVE",
		"ALL",
		"REAL_PIC",
		"GRAY_SCALE",
		"BLACK_WHITE",
		"DROP",
		"CONTINUOUS",              //10
		"LEFT",
		"RIGHT",
		"PERCENT",
		"HWPUNIT",
		"DIGIT",
		"HANGUL_SYLLABLE",
		"HANGUL_JAMO",
		"CIRCLED_DIGIT",
		"LATIN_SMALL",
		"CIRCLED_HANGUL_SYLLABLE", //20
		"CIRCLED_HANGUL_JAMO",
		"ROMAN_SMALL",
		"PARA",
		"CHAR"
	},
//HWPML
	{
		"None",
		"Center",
		"CenterBelow",
		"CenterAbove",
		"All",
		"RealPic",
		"GrayScale",
		"BlackWhite",
		"Drop",
		"Cont",                  //10
		"Left",
		"Right",
		"Percent",
		"HwpUnit",
		"Digit",
		"HangulSyllable",
		"HangulJamo",
		"CircledDigit",
		"LatinSmall",
		"CircledHangulSyllable", //20
		"CircledHangulJamo",
		"RomanSmall",
		"Para",
		"Char"
	}
};

inline const char* GetValueName(EValue eValue, EHanType eType)
{
	switch (eType)
	{
		case EHanType::HWPX:  return VALUE_NAMES[0][static_cast<int>(eValue)];
		case EHanType::HWPML: return VALUE_NAMES[1][static_cast<int>(eValue)];
		default: return "";
	}
}
}

#endif // NODENAMES_H
