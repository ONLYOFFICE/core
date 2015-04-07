#pragma once

namespace TextMark
{
	static const wchar_t ParagraphEnd						= (wchar_t)	13;
	static const wchar_t HardLineBreak					= (wchar_t)	11;
	static const wchar_t BreakingHyphen					= (wchar_t)	4;
	static const wchar_t CellOrRowMark					= (wchar_t)	7;
	static const wchar_t NonRequiredHyphen				= (wchar_t)	31;
	static const wchar_t NonBreakingHyphen				= (wchar_t)	30;
	static const wchar_t NonBreakingSpace					= (wchar_t)	160;
	static const wchar_t Space							= (wchar_t)	32;
	static const wchar_t PageBreakOrSectionMark			= (wchar_t)	12;
	static const wchar_t ColumnBreak						= (wchar_t)	14;
	static const wchar_t Tab								= (wchar_t)	9;
	static const wchar_t FieldBeginMark					= (wchar_t)	19;
	static const wchar_t FieldSeparator					= (wchar_t)	20;
	static const wchar_t FieldEndMark						= (wchar_t)	21;

	//Special WCHARacters (chp.fSpec == 1)

	static const wchar_t CurrentPageNumber				= (wchar_t)	0;
	static const wchar_t Picture							= (wchar_t)	1;
	static const wchar_t AutoNumberedFootnoteReference	= (wchar_t)	2;
	static const wchar_t AutoNumberedEndnoteReference		= (wchar_t)	2;
	static const wchar_t FootnoteSeparator				= (wchar_t)	3;
	static const wchar_t FootnoteContinuation				= (wchar_t)	4;
	static const wchar_t AnnotationReference				= (wchar_t)	5;
	static const wchar_t LineNumber						= (wchar_t)	6;
	static const wchar_t HandAnnotationPicture			= (wchar_t)	7;
	static const wchar_t DrawnObject						= (wchar_t)	8;
	static const wchar_t Symbol							= (wchar_t)	40;
}