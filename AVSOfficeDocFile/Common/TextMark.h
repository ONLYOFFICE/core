#pragma once

namespace TextMark
{
	static const WCHAR ParagraphEnd						= (WCHAR)	13;
	static const WCHAR HardLineBreak					= (WCHAR)	11;
	static const WCHAR BreakingHyphen					= (WCHAR)	4;
	static const WCHAR CellOrRowMark					= (WCHAR)	7;
	static const WCHAR NonRequiredHyphen				= (WCHAR)	31;
	static const WCHAR NonBreakingHyphen				= (WCHAR)	30;
	static const WCHAR NonBreakingSpace					= (WCHAR)	160;
	static const WCHAR Space							= (WCHAR)	32;
	static const WCHAR PageBreakOrSectionMark			= (WCHAR)	12;
	static const WCHAR ColumnBreak						= (WCHAR)	14;
	static const WCHAR Tab								= (WCHAR)	9;
	static const WCHAR FieldBeginMark					= (WCHAR)	19;
	static const WCHAR FieldSeparator					= (WCHAR)	20;
	static const WCHAR FieldEndMark						= (WCHAR)	21;

	//Special WCHARacters (chp.fSpec == 1)

	static const WCHAR CurrentPageNumber				= (WCHAR)	0;
	static const WCHAR Picture							= (WCHAR)	1;
	static const WCHAR AutoNumberedFootnoteReference	= (WCHAR)	2;
	static const WCHAR AutoNumberedEndnoteReference		= (WCHAR)	2;
	static const WCHAR FootnoteSeparator				= (WCHAR)	3;
	static const WCHAR FootnoteContinuation				= (WCHAR)	4;
	static const WCHAR AnnotationReference				= (WCHAR)	5;
	static const WCHAR LineNumber						= (WCHAR)	6;
	static const WCHAR HandAnnotationPicture			= (WCHAR)	7;
	static const WCHAR DrawnObject						= (WCHAR)	8;
	static const WCHAR Symbol							= (WCHAR)	40;
}