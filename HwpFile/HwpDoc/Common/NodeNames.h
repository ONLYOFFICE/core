#ifndef NODENAMES_H
#define NODENAMES_H

#define MAX_TYPES 2
#define MAX_NODES 100

#include "../HanType.h"

namespace HWP
{
enum class ENode
{
	Body,
	Section,
	Paragraph,
	Text,
	Char,
	SectionDef,
	Table,
	Picture,
	Container,
	Ole,
	Equation,
	TextArt,
	Line,
	Rectangle,
	Ellipse,
	Arc,
	Polygon,
	Curve,
	ConnectLine,
	Tab,
	LineBreak,
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
	Default
};

static constexpr const char* NODE_NAMES[MAX_TYPES][MAX_NODES] = 
{
// HWPX
	{
		"",
		"hs:sec",
		"hp:p",
		"hp:run",
		"hp:t",
		"hp:secPr",
		"hp:tbl",
		"hp:pic",
		"hp:container",
		"hp:ole",
		"hp:equation",
		"hp:textart",
		"hp:line",
		"hp:rect",
		"hp:ellipse",
		"hp:arc",
		"hp:polygon",
		"hp:curve",
		"hp:connectLine",
		"hp:tab",
		"hp:lineBreak",
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
		"BODY",
		"SECTION",
		"P",
		"TEXT",
		"CHAR",
		"SECDEF",
		"TABLE",
		"PICTURE",
		"CONTAINER",
		"OLE",
		"EQUATION",
		"TEXTART",
		"LINE",
		"RECTANGLE",
		"ELLIPSE",
		"ARC",
		"POLYGON",
		"CURVE",
		"CONNECTLINE",
		"TAB",
		"LINEBREAK",
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

#define MAX_ARGUMENTS 6

enum class EArgument
{
	ParaShape,
	Style,
	InstId,
	PageBreak,
	ColumnBreak,
	CharShape
};

static constexpr const char* ARGUMENT_NAMES[MAX_TYPES][MAX_ARGUMENTS] = 
{
// HWPX
	{
		"paraPrIDRef",
		"styleIDRef",
		"id",
		"pageBreak",
		"columnBreak",
		"charPrIDRef"
	},
// HWPML
	{
		"ParaShape",
		"Style",
		"InstId",
		"PageBreak",
		"ColumnBreak",
		"CharShape"
	}
};

inline const char* GetArgumentName(EArgument eNode, EHanType eType)
{
	switch (eType)
	{
		case EHanType::HWPX:  return ARGUMENT_NAMES[0][static_cast<int>(eNode)];
		case EHanType::HWPML: return ARGUMENT_NAMES[1][static_cast<int>(eNode)];
		default: return "";
	}
}

}

#endif // NODENAMES_H
