#pragma once

#include "TablePropertyExceptions.h"
#include "ParagraphPropertyExceptions.h"
#include "CharacterPropertyExceptions.h"

namespace DocFileFormat
{
	typedef enum _StyleKind
	{
		paragraph_style = 1,
		character_style,
		table_style,
		numbering_style
	} StyleKind;

	static const WCHAR StyleKindMap[5][10] = 
	{
		_T( "0" ),
		_T( "paragraph" ),
		_T( "character" ),
		_T( "table" ),
		_T( "numbering" )
	};

	typedef enum _StyleIdentifier
	{
		Normal = 0,
		Heading1,
		Heading2,
		Heading3,
		Heading4,
		Heading5,
		Heading6,
		Heading7,
		Heading8,
		Heading9,
		Index1,
		Index2,
		Index3,
		Index4,
		Index5,
		Index6,
		Index7,
		Index8,
		Index9,
		TOC1,
		TOC2,
		TOC3,
		TOC4,
		TOC5,
		TOC6,
		TOC7,
		TOC8,
		TOC9,
		NormalIndent, 
		FootnoteText,
		AnnotationText,
		Header,
		Footer,
		IndexHeading,
		Caption,
		ToCaption,
		EnvelopeAddress,
		EnvelopeReturn,
		FootnoteReference,
		AnnotationReference,
		LineNumber,
		PageNumber,
		EndnoteReference,
		EndnoteText,
		TableOfAuthoring,
		Macro,
		TOAHeading,
		List,
		ListBullet,
		ListNumber,
		List2,
		List3,
		List4,
		List5,
		ListBullet2,
		ListBullet3,
		ListBullet4,
		ListBullet5,
		ListNumber2,
		ListNumber3,
		ListNumber4,
		ListNumber5,
		Title,
		Closing,
		Signature,
		NormalCharacter,
		BodyText,
		BodyTextIndent,
		ListContinue,
		ListContinue2,
		ListContinue3,
		ListContinue4,
		ListContinue5,
		MessageHeader,
		Subtitle,
		Salutation,
		Date,
		BodyText1I,
		BodyText1I2,
		NoteHeading,
		BodyText2,
		BodyText3,
		BodyTextIndent2,
		BodyTextIndent3,
		BlockQuote,
		Hyperlink,
		FollowedHyperlink,
		Strong,
		Emphasis,
		NavPane,
		PlainText,
		AutoSignature,
		FormTop,
		FormBottom,
		HtmlNormal,
		HtmlAcronym,
		HtmlAddress,
		HtmlCite,
		HtmlCode,
		HtmlDfn,
		HtmlKbd,
		HtmlPre,
		htmlSamp,
		HtmlTt,
		HtmlVar,
		TableNormal,
		AnnotationSubject,
		NormalList,
		OutlineList1,
		OutlineList2,
		OutlineList3,
		TableSimple,
		TableSimple2,
		TableSimple3,
		TableClassic1,
		TableClassic2,
		TableClassic3,
		TableClassic4,
		TableColorful1,
		TableColorful2,
		TableColorful3,
		TableColumns1,
		TableColumns2,
		TableColumns3,
		TableColumns4,
		TableColumns5,
		TableGrid1,
		TableGrid2,
		TableGrid3,
		TableGrid4,
		TableGrid5,
		TableGrid6,
		TableGrid7,
		TableGrid8,
		TableList1,
		TableList2,
		TableList3,
		TableList4,
		TableList5,
		TableList6,
		TableList7,
		TableList8,
		Table3DFx1,
		Table3DFx2,
		Table3DFx3,
		TableContemporary,
		TableElegant,
		TableProfessional,
		TableSubtle1,
		tableSubtle2,
		TableWeb1,
		TableWeb2,
		TableWeb3,
		Acetate,
		TableGrid,
		TableTheme,
		Max,
		User = 4094,
		Null = 4095
	} StyleIdentifier;

	static const WCHAR StyleIdentifierMap[159][20] =
	{
		_T( "Normal" ),
		_T( "Heading1" ),
		_T( "Heading2" ),
		_T( "Heading3" ),
		_T( "Heading4" ),
		_T( "Heading5" ),
		_T( "Heading6" ),
		_T( "Heading7" ),
		_T( "Heading8" ),
		_T( "Heading9" ),
		_T( "Index1" ),
		_T( "Index2" ),
		_T( "Index3" ),
		_T( "Index4" ),
		_T( "Index5" ),
		_T( "Index6" ),
		_T( "Index7" ),
		_T( "Index8" ),
		_T( "Index9" ),
		_T( "TOC1" ),
		_T( "TOC2" ),
		_T( "TOC3" ),
		_T( "TOC4" ),
		_T( "TOC5" ),
		_T( "TOC6" ),
		_T( "TOC7" ),
		_T( "TOC8" ),
		_T( "TOC9" ),
		_T( "NormalIndent" ), 
		_T( "FootnoteText" ),
		_T( "AnnotationText" ),
		_T( "Header" ),
		_T( "Footer" ),
		_T( "IndexHeading" ),
		_T( "Caption" ),
		_T( "ToCaption" ),
		_T( "EnvelopeAddress" ),
		_T( "EnvelopeReturn" ),
		_T( "FootnoteReference" ),
		_T( "AnnotationReference" ),
		_T( "LineNumber" ),
		_T( "PageNumber" ),
		_T( "EndnoteReference" ),
		_T( "EndnoteText" ),
		_T( "TableOfAuthoring" ),
		_T( "Macro" ),
		_T( "TOAHeading" ),
		_T( "List" ),
		_T( "ListBullet" ),
		_T( "ListNumber" ),
		_T( "List2" ),
		_T( "List3" ),
		_T( "List4" ),
		_T( "List5" ),
		_T( "ListBullet2" ),
		_T( "ListBullet3" ),
		_T( "ListBullet4" ),
		_T( "ListBullet5" ),
		_T( "ListNumber2" ),
		_T( "ListNumber3" ),
		_T( "ListNumber4" ),
		_T( "ListNumber5" ),
		_T( "Title" ),
		_T( "Closing" ),
		_T( "Signature" ),
		_T( "NormalCharacter" ),
		_T( "BodyText" ),
		_T( "BodyTextIndent" ),
		_T( "ListContinue" ),
		_T( "ListContinue2" ),
		_T( "ListContinue3" ),
		_T( "ListContinue4" ),
		_T( "ListContinue5" ),
		_T( "MessageHeader" ),
		_T( "Subtitle" ),
		_T( "Salutation" ),
		_T( "Date" ),
		_T( "BodyText1I" ),
		_T( "BodyText1I2" ),
		_T( "NoteHeading" ),
		_T( "BodyText2" ),
		_T( "BodyText3" ),
		_T( "BodyTextIndent2" ),
		_T( "BodyTextIndent3" ),
		_T( "BlockQuote" ),
		_T( "Hyperlink" ),
		_T( "FollowedHyperlink" ),
		_T( "Strong" ),
		_T( "Emphasis" ),
		_T( "NavPane" ),
		_T( "PlainText" ),
		_T( "AutoSignature" ),
		_T( "FormTop" ),
		_T( "FormBottom" ),
		_T( "HtmlNormal" ),
		_T( "HtmlAcronym" ),
		_T( "HtmlAddress" ),
		_T( "HtmlCite" ),
		_T( "HtmlCode" ),
		_T( "HtmlDfn" ),
		_T( "HtmlKbd" ),
		_T( "HtmlPre" ),
		_T( "htmlSamp" ),
		_T( "HtmlTt" ),
		_T( "HtmlVar" ),
		_T( "TableNormal" ),
		_T( "AnnotationSubject" ),
		_T( "NormalList" ),
		_T( "OutlineList1" ),
		_T( "OutlineList2" ),
		_T( "OutlineList3" ),
		_T( "TableSimple" ),
		_T( "TableSimple2" ),
		_T( "TableSimple3" ),
		_T( "TableClassic1" ),
		_T( "TableClassic2" ),
		_T( "TableClassic3" ),
		_T( "TableClassic4" ),
		_T( "TableColorful1" ),
		_T( "TableColorful2" ),
		_T( "TableColorful3" ),
		_T( "TableColumns1" ),
		_T( "TableColumns2" ),
		_T( "TableColumns3" ),
		_T( "TableColumns4" ),
		_T( "TableColumns5" ),
		_T( "TableGrid1" ),
		_T( "TableGrid2" ),
		_T( "TableGrid3" ),
		_T( "TableGrid4" ),
		_T( "TableGrid5" ),
		_T( "TableGrid6" ),
		_T( "TableGrid7" ),
		_T( "TableGrid8" ),
		_T( "TableList1" ),
		_T( "TableList2" ),
		_T( "TableList3" ),
		_T( "TableList4" ),
		_T( "TableList5" ),
		_T( "TableList6" ),
		_T( "TableList7" ),
		_T( "TableList8" ),
		_T( "Table3DFx1" ),
		_T( "Table3DFx2" ),
		_T( "Table3DFx3" ),
		_T( "TableContemporary" ),
		_T( "TableElegant" ),
		_T( "TableProfessional" ),
		_T( "TableSubtle1" ),
		_T( "tableSubtle2" ),
		_T( "TableWeb1" ),
		_T( "TableWeb2" ),
		_T( "TableWeb3" ),
		_T( "Acetate" ),
		_T( "TableGrid" ),
		_T( "TableTheme" ),
		_T( "Max" ),
		_T( "User" ),
		_T( "Null" )
	}; 

	class StyleSheetDescription
	{
		friend class StyleSheetMapping;
		friend class CharacterPropertiesMapping;

	private:
		/// The name of the style
		wstring xstzName;
		/// Invariant style identifier 
		StyleIdentifier sti;
		/// spare field for any temporary use, always reset back to zero! 
		bool fScratch;
		/// PHEs of all text with this style are wrong
		bool fInvalHeight;
		/// UPEs have been generated 
		bool fHasUpe;
		/// std has been mass-copied; if unused at save time, 
		/// style should be deleted 
		bool fMassCopy;
		/// style kind 
		StyleKind stk;
		/// base style 
		unsigned int istdBase;
		/// number of UPXs (and UPEs) 
		unsigned short cupx;
		/// next style
		unsigned int istdNext;
		/// offset to end of upx's, start of upe's 
		unsigned short bchUpe;
		/// auto redefine style when appropriate 
		bool fAutoRedef;
		/// hidden from UI? 
		bool fHidden;
		/// style already has valid sprmCRgLidX_80 in it 
		bool f97LidsSet;
		/// if f97LidsSet, says whether we copied the lid from sprmCRgLidX 
		/// into sprmCRgLidX_80 or whether we gotrid of sprmCRgLidX_80
		bool fCopyLang;
		/// HTML Threading compose style 
		bool fPersonalCompose;
		/// HTML Threading reply style 
		bool fPersonalReply;
		/// HTML Threading - another user's personal style 
		bool fPersonal;
		/// Do not export this style to HTML/CSS 
		bool fNoHtmlExport;
		/// Do not show this style in long style lists 
		bool fSemiHidden;
		/// Locked style? 
		bool fLocked;
		/// Style is used by a word feature, e.g. footnote
		bool fInternalUse;
		/// Is this style linked to another?
		unsigned int istdLink;
		/// Style has RevMarking history 
		bool fHasOriginalStyle;
		/// marks during merge which doc's style changed 
		unsigned int rsid;
		/// used temporarily during html export 
		unsigned int iftcHtml;
		/// A StyleSheetDescription can have a PAPX.
		/// If the style doesn't modify paragraph properties, papx is null.
		ParagraphPropertyExceptions* papx;
		/// A StyleSheetDescription can have a CHPX.
		/// If the style doesn't modify character properties, chpx is null.
		CharacterPropertyExceptions* chpx;
		/// A StyleSheetDescription can have a TAPX.
		/// If the style doesn't modify table properties, tapx is null.
		TablePropertyExceptions* tapx;

	public:
		/// Creates an empty STD object
		StyleSheetDescription();
		virtual ~StyleSheetDescription();
		/// Parses the bytes to retrieve a StyleSheetDescription
		StyleSheetDescription( byte* bytes, int size, int cbStdBase, IStream* dataStream );
	};
}